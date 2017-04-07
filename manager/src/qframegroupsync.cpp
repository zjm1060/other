#include "qframegroupsync.h"

#include <QDir>

#include "common.h"
#include "extern.h"
#include "qmessagefiletransfer.h"
#include "qservermanager.h"
#include "qtempsavepath.h"
#include "qdlgbrowsefolders.h"
#include "qtreeviewtransferqueue.h"

QFrameGroupSync::QFrameGroupSync(int kind, const QString subPath, QWidget *parent)
	: QFrame(parent),
	m_bSync(false),
	m_nRow(-1),
	m_nKind(kind),
	m_strSubPath(subPath)
{
	setupUi(this);

	m_pixmap[PixmapServer]=QPixmap(":/images/server.png");
	m_pixmap[PixmapMultiServer]=QPixmap(":/images/multiserver.png");
	m_pixmap[PixmapServerQuit]=QPixmap(":/images/serverquit.png");
	m_pixmap[PixmapSelect]=QPixmap(":/images/checkmark.png");

	QStringList labels;
	labels<<tr("title");

	for (int i=0; i<ListViewEnd; i++)
	{
		m_pListView[i]=new QListView(this);
		m_pListView[i]->setFrameShape(QFrame::NoFrame);
		m_pListView[i]->setResizeMode(QListView::Adjust);
		m_pListView[i]->setMovement(QListView::Static);

		m_pModel[i]=new QStandardItemModel(0, labels.size(), this);
		m_pModel[i]->setHorizontalHeaderLabels(labels);

		m_pListView[i]->setModel(m_pModel[i]);

		connect(m_pListView[i], SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(doubleClicked(const QModelIndex&)));
	}

	m_pListView[ListViewSource]->setViewMode(QListView::IconMode);
	m_pListView[ListViewSource]->setFlow(QListView::TopToBottom);
	m_pListView[ListViewSource]->setWrapping(false);

	scrollAreaSource->setWidget(m_pListView[ListViewSource]);
	scrollAreaSource->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_pServerDelegate=new QServerDelegate(this);
	m_pListView[ListViewTarget]->setItemDelegate(m_pServerDelegate);

	m_pListView[ListViewTarget]->setSelectionMode(QAbstractItemView::ExtendedSelection);
	connect(m_pListView[ListViewTarget], SIGNAL(clicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));

	scrollAreaTarget->setWidget(m_pListView[ListViewTarget]);

	labelInfoIcon->setPixmap(QPixmap(":/images/error.png"));
	showTipsPixmap(false);

	m_pFrameFileSystem=new QFrameRemoteFileSystem("", subPath, this);
	horizontalLayoutFileSystem->insertWidget(0, m_pFrameFileSystem);

	connect(m_pFrameFileSystem, SIGNAL(enterInvalidChar(bool, const QChar&)), this, SLOT(enterInvalidChar(bool, const QChar&)));
	connect(m_pFrameFileSystem, SIGNAL(focusStatusChanged(bool)), this, SLOT(focusStatusChanged(bool)));

	connect(m_pFrameFileSystem, SIGNAL(mouseRightButtonPress(QPoint)), this, SLOT(mouseRightButtonPress(QPoint)));

	m_pTransferInstructor=new QTransferInstructor(this);
	connect(m_pTransferInstructor, SIGNAL(sendMessage(QTransferInstructor::MessageType, const QString&)), 
		this, SLOT(recvMessage(QTransferInstructor::MessageType, const QString&)));
	connect(m_pTransferInstructor, SIGNAL(addSubTask(const QGroupTransferQueue::TransferQueueItem&, QVector<FileInfo>)),
		this, SLOT(addSubTask(const QGroupTransferQueue::TransferQueueItem&, QVector<FileInfo>)));

	m_pViewTransferQueue=new QGroupTransferQueue(this);
	m_pViewTransferQueue->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	horizontalLayoutQueue->insertWidget(0, m_pViewTransferQueue);

	connect(m_pViewTransferQueue, SIGNAL(mouseRightButtonPress(QPoint)), this, SLOT(mouseRightButtonPressTransferQueue(QPoint)));

	m_pDelegate=new QTipsDelegate(this);
	listView->setItemDelegate(m_pDelegate);

	m_pModelTips=new QStandardItemModel(0, 1, this);
	listView->setModel(m_pModelTips);

	connect(m_pViewTransferQueue, SIGNAL(synchronize()), this, SLOT(synchronize()));

	const QServerManager::ServerInfo& serverInfo=QServerManager::instance()->localServer();
	addServer(ListViewSource, serverInfo);
	addServer(ListViewTarget, serverInfo);

	const QMultiHash<QString, QServerManager::ServerInfo>& remoteServers=QServerManager::instance()->remoteServer();
	foreach (QServerManager::ServerInfo remoteServer, remoteServers)
	{
		addServer(ListViewSource, remoteServer);
		addServer(ListViewTarget, remoteServer);
	}

	//弹出菜单
	m_pMenu=new QMenu(this);
	m_pActions[ActionUpload]=new QAction(QIcon(":/images/menuupload.png"), tr("&Upload"), m_pMenu);
	m_pActions[ActionOpen]=new QAction(tr("&Open"), m_pMenu);
	m_pActions[ActionDelete]=new QAction(QIcon(":/images/delete.png"), tr("&Delete"), m_pMenu);
	m_pActions[ActionRefresh]=new QAction(QIcon(":/images/refresh.png"), tr("&Refresh"), m_pMenu);
	m_pActions[ActionNewDirectory]=new QAction(QIcon(":/images/newdirectory.png"), tr("&New Directory"), m_pMenu);
	m_pActions[ActionRename]=new QAction(tr("Rename"), m_pMenu);

	m_pMenu->addAction(m_pActions[ActionUpload]);
	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionOpen]);
	m_pMenu->addAction(m_pActions[ActionDelete]);

	m_pMenuFormat=new QMenu(tr("Format"), m_pMenu);
	m_pActionsFormat[TextFormat]=new QAction(tr("Text"), m_pMenuFormat);
	m_pActionsFormat[BinaryFormat]=new QAction(tr("Binary"), m_pMenuFormat);
	m_pActionsFormat[AutoFormat]=new QAction(tr("Auto"), m_pMenuFormat);

	m_pMenuFormat->addAction(m_pActionsFormat[TextFormat]);
	m_pMenuFormat->addAction(m_pActionsFormat[BinaryFormat]);
	m_pMenuFormat->addSeparator();
	m_pMenuFormat->addAction(m_pActionsFormat[AutoFormat]);

	m_pGroupFormat=new QActionGroup(m_pMenuFormat);
	for (int i=0; i<FormatEnd; i++)
	{
		m_pActionsFormat[i]->setCheckable(true);
		m_pActionsFormat[i]->setData(i);
		m_pGroupFormat->addAction(m_pActionsFormat[i]);
	}
	m_pGroupFormat->setExclusive(true);
	m_pActionsFormat[AutoFormat]->setChecked(true);

	m_pMenu->addSeparator();
	m_pMenu->addMenu(m_pMenuFormat);

	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionNewDirectory]);
	m_pMenu->addAction(m_pActions[ActionRename]);

	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionRefresh]);

	m_pMenuTask=new QMenu(this);
	m_pActions[ActionRetransmit]=new QAction(QIcon(":/images/retransmit.png"), tr("&Retransmit"), m_pMenuTask);
	m_pActions[ActionDeleteTask]=new QAction(QIcon(":/images/delete.png"), tr("&Delete"), m_pMenuTask);
	m_pActions[ActionDeleteAllTask]=new QAction(tr("&Delete All"), m_pMenuTask);

	m_pMenuTask->addAction(m_pActions[ActionRetransmit]);
	m_pMenuTask->addAction(m_pActions[ActionDeleteTask]);
	m_pMenuTask->addSeparator();
	m_pMenuTask->addAction(m_pActions[ActionDeleteAllTask]);

	//菜单触发事件
	m_pSignalMapper=new QSignalMapper(this);
	for (int i=0; i<ActionEnd; i++)
	{
		connect(m_pActions[i], SIGNAL(triggered()), m_pSignalMapper, SLOT(map()));
		m_pSignalMapper->setMapping(m_pActions[i], i);
	}

	connect(m_pSignalMapper, SIGNAL(mapped(int)), this, SLOT(actionTriggered(int)));

	if (m_pModel[ListViewSource]->rowCount() > 0)
	{
		doubleClicked(ListViewSource, m_pModel[ListViewSource]->index(0, 0));
	}
}

QFrameGroupSync::~QFrameGroupSync()
{
}

void QFrameGroupSync::addServer(int target, const QServerManager::ServerInfo& serverInfo)
{
	if (serverInfo.count > 0)
	{
		QVariant var;
		QStandardItem* pItem=NULL;

		QString strTip=serverInfo.hostName;

		int nRow;
		QList<QString> keys=serverInfo.servers.keys();
		foreach (QString key, keys)
		{
			QVector<QServerManager::Server> servers=serverInfo.servers[key];

			foreach (QServerManager::Server server, servers)
			{
				strTip+=QString("\n%1:%2").arg(server.addr).arg(server.port);

				if (pItem == NULL)
				{
					nRow=m_pModel[target]->rowCount();
					m_pModel[target]->insertRow(nRow);

					QPixmap& pixmap=serverInfo.count>1? m_pixmap[PixmapMultiServer]:m_pixmap[PixmapServer];

					pItem=m_pModel[target]->itemFromIndex(m_pModel[target]->index(nRow, 0));
					pItem->setEditable(false);
					m_pModel[target]->setData(m_pModel[target]->index(nRow, 0), serverInfo.hostName);
					m_pModel[target]->setData(m_pModel[target]->index(nRow, 0), QIcon(pixmap), Qt::DecorationRole);

					var.setValue(serverInfo);
					m_pModel[target]->setData(m_pModel[target]->index(nRow, 0), var, QServerDelegate::ServerInfoRole);

					m_pModel[target]->setData(m_pModel[target]->index(nRow, 0), true, QServerDelegate::ServerOnlineRole);	//是否在线
					m_pModel[target]->setData(m_pModel[target]->index(nRow, 0), false, QServerDelegate::ServerSelectRole);
					m_pModel[target]->setData(m_pModel[target]->index(nRow, 0), pixmap, QServerDelegate::PixmapRole);

					if (target == ListViewTarget)
					{
						new QTempSavePath(m_pModel[target], m_pModel[target]->index(nRow, 0), m_nKind, m_strSubPath, this);
					}
				}
			}
		}

		if (pItem != NULL)
		{
			pItem->setToolTip(strTip);
		}
	}
}

void QFrameGroupSync::setTitleText(const QString& title)
{
	labelTitle->setText(title);
}

void QFrameGroupSync::setDefaultDescText(const QString& desc)
{
	m_strDefaultDesc=desc;

	labelDesc->setText(desc);
}

void QFrameGroupSync::setThemePixmap(const QPixmap& icon)
{
	labelIcon->setPixmap(icon);
}

void QFrameGroupSync::showTipsPixmap(bool show)
{
	if (show)
	{
		labelInfoIcon->show();
	}
	else
	{
		labelInfoIcon->hide();
	}
}

void QFrameGroupSync::clicked(const QModelIndex& index)
{
	if (m_pModel[ListViewTarget]->data(m_pModel[ListViewTarget]->index(index.row(), 0), QServerDelegate::ServerSelectRole).toBool())
	{
		QRect rect=m_pListView[ListViewTarget]->visualRect(index);
		QPoint pt=m_pListView[ListViewTarget]->mapFromGlobal(QCursor::pos());

		QRect r=m_pServerDelegate->toolIconRect(rect);
		if (r.contains(pt))
		{
			QServerManager::ServerInfo info=
				qvariant_cast<QServerManager::ServerInfo>(m_pModel[ListViewTarget]->data(m_pModel[ListViewTarget]->index(index.row(), 0), QServerDelegate::ServerInfoRole));
			
			QString strInitPath=m_pModel[ListViewTarget]->data(m_pModel[ListViewTarget]->index(index.row(), 0), QServerDelegate::ServerSaveRole).toString();
			QDlgBroseFolders dialog(info, strInitPath, m_strSubPath, this);
			dialog.setModal(true);
			if (dialog.exec() != 0)
			{
				strInitPath=dialog.text();
				m_pModel[ListViewTarget]->setData(m_pModel[ListViewTarget]->index(index.row(), 0), strInitPath, QServerDelegate::ServerSaveRole);

				QList<QString> keys=info.servers.keys();
				foreach (QString key, keys)
				{
					codeConfig(getAppParentDir()+TRANSFER_SETTING_FILE, QString("%1/%2").arg(key).arg(m_nKind), strInitPath);
				}
			}
		}
	}
}

void QFrameGroupSync::doubleClicked(const QModelIndex& index)
{
	QObject* pObj=sender();
	int target=-1;
	for (int i=0; i<ListViewEnd; i++)
	{
		if (pObj == m_pListView[i])
		{
			target=i;
			break;
		}
	}

	doubleClicked(target, index);
}

void QFrameGroupSync::doubleClicked(int target, const QModelIndex& index)
{
	switch (target)
	{
	case ListViewSource:
		{
			bool bSelect=m_pModel[target]->data(m_pModel[target]->index(index.row(), 0), QServerDelegate::ServerSelectRole).toBool();
			if (bSelect)
			{
				break;
			}

			QVariant decoration;
			QPixmap pixmap;

			int nRows=m_pModel[target]->rowCount();
			for (int i=0; i<nRows; i++)
			{
				if (m_pModel[target]->data(m_pModel[target]->index(i, 0), QServerDelegate::ServerSelectRole).toBool())
				{
					decoration=m_pModel[target]->data(m_pModel[target]->index(i, 0), QServerDelegate::PixmapRole);
					pixmap=qvariant_cast<QPixmap>(decoration);

					m_pModel[target]->setData(m_pModel[target]->index(i, 0), QIcon(pixmap), Qt::DecorationRole);
					m_pModel[target]->setData(m_pModel[target]->index(i, 0), false, QServerDelegate::ServerSelectRole);
				}
			}

			decoration=m_pModel[target]->data(m_pModel[target]->index(index.row(), 0), QServerDelegate::PixmapRole);
			pixmap=qvariant_cast<QPixmap>(decoration);
			pixmap=addPixmapToPixmap(m_pixmap[PixmapSelect], pixmap, 0);

			m_pModel[target]->setData(m_pModel[target]->index(index.row(), 0), QIcon(pixmap), Qt::DecorationRole);
			m_pModel[target]->setData(m_pModel[target]->index(index.row(), 0), true, QServerDelegate::ServerSelectRole);

			QServerManager::ServerInfo serverInfo=
				qvariant_cast<QServerManager::ServerInfo>(m_pModel[target]->data(m_pModel[target]->index(index.row(), 0), QServerDelegate::ServerInfoRole));
			
			m_pFrameFileSystem->setClient(serverInfo);
		}
		break;
	case ListViewTarget:
		{
			QVariant decoration=m_pModel[target]->data(m_pModel[target]->index(index.row(), 0), QServerDelegate::PixmapRole);
			QPixmap pixmap=qvariant_cast<QPixmap>(decoration);

			bool bSelect=m_pModel[target]->data(m_pModel[target]->index(index.row(), 0), QServerDelegate::ServerSelectRole).toBool();
			if (!bSelect)
			{
				pixmap=addPixmapToPixmap(m_pixmap[PixmapSelect], pixmap, 0);
			}

			m_pModel[target]->setData(m_pModel[target]->index(index.row(), 0), QIcon(pixmap), Qt::DecorationRole);
			m_pModel[target]->setData(m_pModel[target]->index(index.row(), 0), !bSelect, QServerDelegate::ServerSelectRole);
		}
		break;
	default:
		break;
	}
}

void QFrameGroupSync::focusStatusChanged(bool has)
{
	if (has)
	{
		m_pFrameFileSystem->editTextChangedLocation(m_pFrameFileSystem->locationText());
	}
	else
	{
		showTipsPixmap(false);

		labelDesc->setText(m_strDefaultDesc);
	}
}

void QFrameGroupSync::enterInvalidChar(bool invalid, const QChar& ch)
{
	if (invalid)
	{
		showTipsPixmap(true);

		labelDesc->setText(QString(ch)+" "+tr("is an invalid character in your remote location."));
	}
	else
	{
		showTipsPixmap(false);

		labelDesc->setText(tr("Enter the remote target folder."));
	}
}

void QFrameGroupSync::synchronize()
{
	if (m_bSync)
	{
		return;
	}

	QGroupTransferQueue::TransferQueueItem task;
	int nRow=m_pViewTransferQueue->fetchTask(task);
	if (nRow == -1)
	{
		return;
	}

	m_pModelTips->setRowCount(0);

	m_bSync=true;
	appendTip(QTipsDelegate::Information, tr("Synchronize..."));

	transfer(nRow, task);
}

void QFrameGroupSync::fetchTask()
{
	if (!m_bSync)
	{
		return;
	}

	QGroupTransferQueue::TransferQueueItem task;
	m_nRow=m_pViewTransferQueue->fetchTask(task);
	transfer(m_nRow, task);
}

void QFrameGroupSync::transfer(int row, const QGroupTransferQueue::TransferQueueItem& task)
{
	if (row == -1)
	{
		finished();
		return;
	}

	m_nRow=row;

	m_pTransferInstructor->addTask(task);
}

void QFrameGroupSync::recvMessage(QTransferInstructor::MessageType type, const QString& message)
{
	switch (type)
	{
	case QTransferInstructor::ConnectingToSource:
	case QTransferInstructor::ConnectToSourceSuccess:
	case QTransferInstructor::ConnectToTargetSuccess:
		{
			appendTip(QTipsDelegate::Information, message);
		}
		break;
	case QTransferInstructor::ConnectToSourceFailed:
	case QTransferInstructor::ConnectToTargetFailed:
	case QTransferInstructor::DisconnectedToSourceHost:
	case QTransferInstructor::UploadFail:
		{
			appendTip(QTipsDelegate::Error, message);
			m_pViewTransferQueue->setItemStatus(m_nRow, QGroupTransferQueue::Failed);

			fetchTask();
		}
		break;
	case QTransferInstructor::QueryUpload:
		{
			appendTip(QTipsDelegate::Upload, message);
			m_pViewTransferQueue->setItemStatus(m_nRow, QGroupTransferQueue::Transfering);
		}
		break;
	case QTransferInstructor::UploadSuccess:
		{
			appendTip(QTipsDelegate::Success, message);
			m_pViewTransferQueue->setItemStatus(m_nRow, QGroupTransferQueue::Finished);

			fetchTask();
		}
		break;
	default:
		qDebug("QFrameGroupSync::recvMessage invalid message type!!!");
		break;
	}
}

void QFrameGroupSync::addSubTask(const QGroupTransferQueue::TransferQueueItem& task, QVector<FileInfo> files)
{
	if (files.count() <= 0)
	{
		return;
	}

	QGroupTransferQueue::TransferQueueItem item;

	item.sourceHost=task.sourceHost;
	item.targetHost=task.targetHost;

	item.isTop=false;
	item.source=QTreeViewTransferQueue::appendPathName(task.source, task.file.name, task.isTop);

	item.format=task.format;

	item.target=QTreeViewTransferQueue::appendPathName(task.target, task.file.name, false);

	QFrameRemoteFileSystem::TransferFileItem file;
	for (int i=0; i<files.size(); i++)
	{
		file.name=files[i].name;
		file.size=files[i].size;
		file.type=files[i].type;
		file.typeDesc=fileType(files[i]);

		item.file=file;

		m_pViewTransferQueue->insertItem(m_nRow+i+1, item);
	}
}

void QFrameGroupSync::mouseRightButtonPress(QPoint pos)
{
	bool hasSelect;
	for (int i=0; i<m_pModel[ListViewTarget]->rowCount(); i++)
	{
		hasSelect=m_pModel[ListViewTarget]->data(m_pModel[ListViewTarget]->index(i, 0), QServerDelegate::ServerSelectRole).toBool();
		if (hasSelect)
		{
			break;
		}
	}

	if (hasSelect)
	{
		m_pActions[ActionUpload]->setEnabled(m_pFrameFileSystem->canUpload());
	}
	else
	{
		m_pActions[ActionUpload]->setEnabled(false);
	}

	m_pActions[ActionDelete]->setEnabled(m_pFrameFileSystem->canDelete());
	m_pActions[ActionOpen]->setEnabled(m_pFrameFileSystem->canOpen());
	m_pActions[ActionRename]->setEnabled(m_pFrameFileSystem->canRename());
	m_pActions[ActionNewDirectory]->setEnabled(m_pFrameFileSystem->canNewDir());

	m_pMenu->popup(pos);
}

void QFrameGroupSync::mouseRightButtonPressTransferQueue(QPoint pos)
{
	if (m_pViewTransferQueue == NULL)
	{
		return;
	}

	QModelIndexList list=m_pViewTransferQueue->selectedRows();
	if (list.isEmpty())
	{
		m_pActions[ActionRetransmit]->setEnabled(false);
		m_pActions[ActionDeleteTask]->setEnabled(false);
	}
	else
	{
		bool bRetransmit=false;
		foreach (QModelIndex index, list)
		{
			if (m_pViewTransferQueue->itemStatus(index.row()) == QGroupTransferQueue::Failed)
			{
				bRetransmit=true;
				break;
			}
		}

		m_pActions[ActionRetransmit]->setEnabled(bRetransmit);
		m_pActions[ActionDeleteTask]->setEnabled(true);
	}

	if (m_pViewTransferQueue->taskCount() > 0)
	{
		m_pActions[ActionDeleteAllTask]->setEnabled(true);
	}
	else
	{
		m_pActions[ActionDeleteAllTask]->setEnabled(false);
	}

	m_pMenuTask->popup(m_pViewTransferQueue->mapToGlobal(pos));
}

void QFrameGroupSync::appendTip(int flag, const QString& tip)
{
	if (m_pModelTips == NULL)
	{
		return;
	}

	int nRow=m_pModelTips->rowCount();
	m_pModelTips->insertRow(nRow);

	m_pModelTips->itemFromIndex(m_pModelTips->index(nRow, 0))->setEditable(false);
	m_pModelTips->setData(m_pModelTips->index(nRow, 0), flag, Qt::UserRole);
	m_pModelTips->setData(m_pModelTips->index(nRow, 0), tip);

	QItemSelectionModel* pModel=listView->selectionModel();
	if (pModel != NULL && !pModel->hasSelection())
	{
		listView->scrollTo(m_pModelTips->index(nRow, 0));
	}
}

void QFrameGroupSync::finished()
{
	m_bSync=false;

	appendTip(QTipsDelegate::Information, tr("Synchronization finished."));
}

void QFrameGroupSync::actionTriggered(int index)
{
	switch (index)
	{
	case ActionUpload:
		{
			QGroupTransferQueue::TransferQueueItem item;

			bool hasSelect=false;
			for (int i=0; i<m_pModel[ListViewSource]->rowCount(); i++)
			{
				if (m_pModel[ListViewSource]->data(m_pModel[ListViewSource]->index(i, 0),
					QServerDelegate::ServerSelectRole).toBool())
				{
					hasSelect=true;

					item.sourceHost=qvariant_cast<QServerManager::ServerInfo>
						(m_pModel[ListViewSource]->data(m_pModel[ListViewSource]->index(i, 0), QServerDelegate::ServerInfoRole));

					break;
				}
			}

			if (!hasSelect)
			{
				break;
			}

			item.isTop=m_pFrameFileSystem->canNewDir()? false:true;
			item.source=m_pFrameFileSystem->currentLocationText();

			item.format=m_pGroupFormat->checkedAction()->data().toInt();

			QVector<QFrameRemoteFileSystem::TransferFileItem> files=m_pFrameFileSystem->selectedFileItems();
			if (files.count() <= 0)
			{
				break;
			}

			for (int i=0; i<m_pModel[ListViewTarget]->rowCount(); i++)
			{
				hasSelect=m_pModel[ListViewTarget]->data(m_pModel[ListViewTarget]->index(i, 0),
					QServerDelegate::ServerSelectRole).toBool();
				if (!hasSelect)
				{
					continue;
				}

				item.targetHost=qvariant_cast<QServerManager::ServerInfo>
					(m_pModel[ListViewTarget]->data(m_pModel[ListViewTarget]->index(i, 0), QServerDelegate::ServerInfoRole));

				item.target=m_pModel[ListViewTarget]->data(m_pModel[ListViewTarget]->index(i, 0),
					QServerDelegate::ServerSaveRole).toString();

				for (int i=0; i<files.size(); i++)
				{
					item.file=files[i];

					m_pViewTransferQueue->appendItem(item);
				}
			}
		}
		break;
	case ActionOpen:
		{
			m_pFrameFileSystem->doubleClickedLocation();
		}
		break;
	case ActionDelete:
		{
			m_pFrameFileSystem->delDirectory();
		}
		break;
	case ActionRefresh:
		{
			m_pFrameFileSystem->refresh();
		}
		break;
	case ActionNewDirectory:
		{
			m_pFrameFileSystem->newDirectory();
		}
		break;
	case ActionRename:
		{
			m_pFrameFileSystem->rename();
		}
		break;
	case ActionRetransmit:
		{
			QModelIndexList list=m_pViewTransferQueue->selectedRows();
			if (!list.isEmpty())
			{
				appendTip(QTipsDelegate::Information, tr("Begin to retransmit..."));
				foreach (QModelIndex index, list)
				{
					if (m_pViewTransferQueue->itemStatus(index.row()) == QGroupTransferQueue::Failed)
					{
						m_pViewTransferQueue->setItemStatus(index.row(), QGroupTransferQueue::Waiting);
					}
				}
			}
		}
		break;
	case ActionDeleteTask:
		{
// 			appendTip(QTipsDelegate::Information, tr("Delete tasks..."));
// 			QModelIndexList list=m_pViewTransferQueue->selectedRows();
// 			while (!list.isEmpty())
// 			{
// 				if (m_pViewTransferQueue->itemStatus(list[0].row()) == QTreeViewTransferQueue::Transfering)
// 				{
// 					continue;
// 				}
// 
// 				m_pViewTransferQueue->removeItem(list[0].row());
// 
// 				list=m_pViewTransferQueue->selectedRows();
// 			}
// 
// 			m_pViewTransferQueue->startSynchronize();
		}
		break;
	case ActionDeleteAllTask:
		{
// 			appendTip(QTipsDelegate::Information, tr("Delete all tasks..."));
// 
// 			m_pViewTransferQueue->clear();
		}
		break;
	default:
		break;
	}
}
