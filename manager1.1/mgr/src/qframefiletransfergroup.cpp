#include "qframefiletransfergroup.h"

#include <QDir>

#include "common.h"
#include "qservermanager.h"
#include "qgrouptransferqueue.h"
#include "qtreeviewtransferqueue.h"
#include "qdlgedithost.h"
#include "qlogger.h"

QFrameFileTransferGroup::QFrameFileTransferGroup(const QString& record, const QString& subPath, QWidget *parent)
	: QFrame(parent),
	m_bSync(false),
	m_nRow(-1)
{
	setupUi(this);

	m_pListViewServers=new QListViewControlledServer(this);
	horizontalLayoutSource->addWidget(m_pListViewServers);

	connect(m_pListViewServers, SIGNAL(currentChanged(const QModelIndex&)), this, SLOT(currentChanged(const QModelIndex&)));

	m_pLinkButtonAdd=new QLinkButton(tr("Add"), this);
	horizontalLayoutRightButtons->addWidget(m_pLinkButtonAdd);
	connect(m_pLinkButtonAdd, SIGNAL(clicked()), this, SLOT(onClickedButtonAdd()));

	m_pLinkButtonRemove=new QLinkButton(tr("Remove"), this);
	m_pLinkButtonRemove->setEnabled(false);
	horizontalLayoutRightButtons->addWidget(m_pLinkButtonRemove);
	connect(m_pLinkButtonRemove, SIGNAL(clicked()), this, SLOT(onClickedButtonRemove()));

	m_pListViewReveiver=new QListViewFileReceiver(appendPath(getAppParentDir(), TRANSFER_SETTING_FILE), record, subPath, this);
	verticalLayoutReveiver->addWidget(m_pListViewReveiver);

	connect(m_pListViewReveiver, SIGNAL(selectChanged(bool)), this, SLOT(selectChanged(bool)));

	labelInfoIcon->setPixmap(findPixmap(":/images/error.png"));
	showTipsPixmap(false);

	m_pFrameFileSystem=new QFrameRemoteFileSystem("", subPath, this);
	horizontalLayoutFileSystem->addWidget(m_pFrameFileSystem);

	connect(m_pFrameFileSystem, SIGNAL(enterInvalidChar(bool, const QChar&)), this, SLOT(enterInvalidChar(bool, const QChar&)));
	connect(m_pFrameFileSystem, SIGNAL(focusStatusChanged(bool)), this, SLOT(focusStatusChanged(bool)));

	connect(m_pFrameFileSystem, SIGNAL(mouseRightButtonPress(QPoint)), this, SLOT(mouseRightButtonPress(QPoint)));

	m_pTransferInstructor=new QTransferInstructor(this);
	connect(m_pTransferInstructor, SIGNAL(sendMessage(QTransferInstructor::MessageType, const QString&)), 
		this, SLOT(recvMessage(QTransferInstructor::MessageType, const QString&)));
	connect(m_pTransferInstructor, SIGNAL(addSubTask(const QGroupTransferQueue::TransferQueueItem&, QVector<FileInfo>)),
		this, SLOT(addSubTask(const QGroupTransferQueue::TransferQueueItem&, QVector<FileInfo>)), Qt::DirectConnection);
	connect(m_pTransferInstructor, SIGNAL(transferedFileContents(const QGroupTransferQueue::TransferQueueItem&, quint64)), 
		this, SLOT(transferedFileContents(const QGroupTransferQueue::TransferQueueItem&, quint64)));

	m_pTreeViewTransferQueue=new QTreeViewGroupTransferQueue(this);
	m_pTreeViewTransferQueue->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	m_pTreeViewTransferQueue->hideColumn(QTreeViewGroupTransferQueue::StatusTitle);
	m_pTreeViewTransferQueue->showAbsolutePath(false);
	horizontalLayoutQueue->addWidget(m_pTreeViewTransferQueue);

	connect(m_pTreeViewTransferQueue, SIGNAL(mouseRightButtonPressed(QPoint)), this, SLOT(mouseRightButtonPressTransferQueue(QPoint)));
	connect(m_pTreeViewTransferQueue, SIGNAL(synchronize()), this, SLOT(synchronize()));

	m_pListViewPrompt=new QListViewPrompt(this);
	verticalLayoutPrompt->addWidget(m_pListViewPrompt);

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
}

QFrameFileTransferGroup::~QFrameFileTransferGroup()
{
	
}

void QFrameFileTransferGroup::setServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	m_pListViewServers->refreshServerList(servers);

	QListIterator<QPair<ServerInfo, int> > it(servers);
	while (it.hasNext())
	{
		const QPair<ServerInfo, int>& info=it.next();

		if (m_pListViewReveiver != NULL)
		{
			m_pListViewReveiver->addReceiver(info);
		}
	}
}

void QFrameFileTransferGroup::setAddedServerList(const QList<Server>& servers)
{
	QListIterator<Server> it(servers);
	while (it.hasNext())
	{
		const Server& server=it.next();

		if (m_pListViewReveiver != NULL)
		{
			m_pListViewReveiver->addReceiver(server);
		}
	}
}

void QFrameFileTransferGroup::setTitleText(const QString& title)
{
	labelTitle->setText(title);
}

void QFrameFileTransferGroup::setDefaultDescText(const QString& desc)
{
	m_strDefaultDesc=desc;

	labelDesc->setText(desc);
}

void QFrameFileTransferGroup::setThemePixmap(const QPixmap& icon)
{
	labelIcon->setPixmap(icon);
}

void QFrameFileTransferGroup::showTipsPixmap(bool show)
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

void QFrameFileTransferGroup::currentChanged(const QModelIndex& index)
{
	if (m_pListViewServers == NULL || !index.isValid())
	{
		return;
	}

	ServerInfo info=m_pListViewServers->serverInfoFromIndex(index);
	m_pFrameFileSystem->setClient(info);
}

void QFrameFileTransferGroup::focusStatusChanged(bool has)
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

void QFrameFileTransferGroup::enterInvalidChar(bool invalid, const QChar& ch)
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

void QFrameFileTransferGroup::synchronize()
{
	if (m_bSync)
	{
		return;
	}

	m_pListViewPrompt->clear();
	m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, tr("Synchronize..."));

	m_bSync=true;

	fetchTask();
}

void QFrameFileTransferGroup::fetchTask()
{
	if (!m_bSync)
	{
		return;
	}

	int nRole;
	QVariant var;
	int nStatus;
	QModelIndex index=m_pTreeViewTransferQueue->fetchTask(nRole, var, nStatus);

	if (nRole != QTreeViewGroupTransferQueue::BackUpRole)
	{
		QGroupTransferQueue::TransferQueueItem item=qvariant_cast<QGroupTransferQueue::TransferQueueItem>(var);
		transfer(index, item);
	}
}

void QFrameFileTransferGroup::transfer(QModelIndex index, const QGroupTransferQueue::TransferQueueItem& task)
{
	if (!index.isValid())
	{
		//同步完毕
		finished();

		return;
	}

	m_index=index;
	m_task=task;

	m_pTreeViewTransferQueue->expand(m_index.parent());

	m_pTransferInstructor->addTask(m_task);
}

void QFrameFileTransferGroup::recvMessage(QTransferInstructor::MessageType type, const QString& message)
{
	switch (type)
	{
	case QTransferInstructor::ConnectToTargetSuccess:
		{
			m_pTreeViewTransferQueue->setItemTransferStatus(m_index.parent().row(), QTreeViewGroupTransferQueue::Connected, m_index.parent().parent());
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, message);
		}
		break;
	case QTransferInstructor::ConnectingToSource:
		{
			m_pTreeViewTransferQueue->setItemTransferStatus(m_index.parent().row(), QTreeViewGroupTransferQueue::Connecting, m_index.parent().parent());
		}
	case QTransferInstructor::ConnectToSourceSuccess:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, message);
		}
		break;
	case QTransferInstructor::ConnectToTargetFailed:
	case QTransferInstructor::DisconnectedToSourceHost:
	case QTransferInstructor::ConnectToSourceFailed:
		{
			m_pTreeViewTransferQueue->setItemTransferStatus(m_index.parent().row(), QTreeViewGroupTransferQueue::Unconneced, m_index.parent().parent());

			m_pTreeViewTransferQueue->collapse(m_index.parent());
		}
	case QTransferInstructor::UploadFail:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Error, message);
			m_pTreeViewTransferQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Failed, m_index.parent());

			fetchTask();
		}
		break;
	case QTransferInstructor::QueryUpload:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Upload, message);
			m_pTreeViewTransferQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Transfering, m_index.parent());
		}
		break;
	case QTransferInstructor::UploadSuccess:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Success, message);
			m_pTreeViewTransferQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Finished, m_index.parent());

			fetchTask();
		}
		break;
	case QTransferInstructor::UploadDirSuccess:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Success, message);
		}
		break;
	case QTransferInstructor::UploadWarning:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Warning, message);
			m_pTreeViewTransferQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Transfering, m_index.parent());
		}
		break;
	case QTransferInstructor::InterruptTransfer:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Warning, message);
		}
		break;
	default:
		logplusInfo(MANAGER_LOGGER_STR, QString("QFrameFileTransferGroup::recvMessage invalid message type %1!!!").arg(type));
		break;
	}
}

void QFrameFileTransferGroup::addSubTask(const QGroupTransferQueue::TransferQueueItem& task, QVector<FileInfo> files)
{
	if (files.count() <= 0)
	{
		return;
	}

	QTreeViewGroupTransferQueue::TransferQueueFileItem item;

	item.isTop=false;
	item.source=QReceiverTransferFile::appendPathName(task.source, task.file.name, task.isTop);

	item.format=task.format;

	item.target=QReceiverTransferFile::appendPathName(task.target, task.file.name, false);

	TransferFileItem file;
	for (int i=0; i<files.size(); i++)
	{
		file.name=files[i].name;
		file.size=files[i].size;
		file.type=files[i].type;
		file.typeDesc=fileType(files[i]);

		item.file=file;

		m_pTreeViewTransferQueue->insertItem(m_index.row()+i+1, item, NonVersioned, m_index.parent());
	}

	m_pTreeViewTransferQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Finished, m_index.parent());
	fetchTask();
}

void QFrameFileTransferGroup::transferedFileContents(const QGroupTransferQueue::TransferQueueItem& task, quint64 size)
{
	if (m_task != task)
	{
		return;
	}

	m_pTreeViewTransferQueue->addItemProcess(m_index.row(), size, m_index.parent());
}

void QFrameFileTransferGroup::onClickedButtonAdd()
{
	QDlgEditHost dialog(this);
	dialog.setModal(true);
	if (dialog.exec() != 0)
	{
		Server server=dialog.host();

		if (m_pListViewReveiver->addReceiver(server))
		{
			emit addServer(server);
		}
	}
}

void QFrameFileTransferGroup::onClickedButtonRemove()
{
	QList<Server> servers=m_pListViewReveiver->removeSelectedReceiver();
	if (!servers.isEmpty())
	{
		emit removeServers(servers);
	}
}

void QFrameFileTransferGroup::selectChanged(bool removable)
{
	m_pLinkButtonRemove->setEnabled(removable);
}

void QFrameFileTransferGroup::mouseRightButtonPress(QPoint pos)
{
	if (m_pListViewReveiver == NULL)
	{
		return;
	}

	bool hasSelect=m_pListViewReveiver->hasSelected();

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

void QFrameFileTransferGroup::mouseRightButtonPressTransferQueue(QPoint pos)
{
	if (m_pTreeViewTransferQueue == NULL)
	{
		return;
	}

	QModelIndexList list=m_pTreeViewTransferQueue->selectedRows();
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
			if (m_pTreeViewTransferQueue->itemTransferStatus(index.row(), index.parent()) == QTreeViewGroupTransferQueue::Failed)
			{
				bRetransmit=true;
				break;
			}
		}

		m_pActions[ActionRetransmit]->setEnabled(bRetransmit);
		m_pActions[ActionDeleteTask]->setEnabled(true);
	}

	if (m_pTreeViewTransferQueue->taskCount() > 0)
	{
		m_pActions[ActionDeleteAllTask]->setEnabled(true);
	}
	else
	{
		m_pActions[ActionDeleteAllTask]->setEnabled(false);
	}

	m_pMenuTask->popup(m_pTreeViewTransferQueue->mapToGlobal(pos));
}

void QFrameFileTransferGroup::finished()
{
	m_bSync=false;

	m_pTransferInstructor->clear();

	m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, tr("Synchronization finished."));
}

void QFrameFileTransferGroup::actionTriggered(int index)
{
	switch (index)
	{
	case ActionUpload:
		{
			QGroupTransferQueue::TransferQueueItem task;

			ServerInfo info;
			bool hasSelect=m_pListViewServers->selectedServer(info);

			if (!hasSelect)
			{
				break;
			}

			task.sourceHost=info;
			task.isTop=m_pFrameFileSystem->canNewDir()? false:true;
			task.source=m_pFrameFileSystem->currentLocationText();

			task.format=m_pGroupFormat->checkedAction()->data().toInt();

			QVector<TransferFileItem> files=m_pFrameFileSystem->selectedFileItems();
			if (files.count() <= 0)
			{
				break;
			}

			int nRow=-1;
			do 
			{
				nRow=m_pListViewReveiver->nextSelected(nRow, info);

				if (nRow == -1)
				{
					break;
				}

				if (info.count == 0)
				{
					continue;
				}

				task.targetHost=info;
				task.isSlient=m_pListViewReveiver->isAddedItem(nRow);
				task.target=m_pListViewReveiver->savePath(nRow);

				QTreeViewGroupTransferQueue::TransferQueuePeerItem peer;
				peer.isSlient=task.isSlient;
				peer.source=task.sourceHost;
				peer.target=task.targetHost;

				QTreeViewGroupTransferQueue::TransferQueueFileItem item;
				item.format=task.format;
				item.isTop=task.isTop;
				item.source=task.source;
				item.target=task.target;

				QModelIndex pindex=m_pTreeViewTransferQueue->appendPeerItem(peer);

				for (int i=0; i<files.size(); i++)
				{
					item.file=files[i];

					m_pTreeViewTransferQueue->appendItem(item, NonVersioned, pindex);
				}

			} while (true);

			synchronize();
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
			QModelIndexList list=m_pTreeViewTransferQueue->selectedRows();
			if (!list.isEmpty())
			{
				m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, tr("Begin to retransmit..."));
				foreach (QModelIndex index, list)
				{
					if (m_pTreeViewTransferQueue->itemTransferStatus(index.row(), index.parent()) == QTreeViewGroupTransferQueue::Failed)
					{
						m_pTreeViewTransferQueue->setItemTransferStatus(index.row(), QTreeViewGroupTransferQueue::Waiting, index.parent());
					}
				}
			}
		}
		break;
	case ActionDeleteTask:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, tr("Delete tasks..."));
			QModelIndexList list=m_pTreeViewTransferQueue->selectedRows();
			while (!list.isEmpty())
			{
				if (m_pTreeViewTransferQueue->itemTransferStatus(list[0].row(), list[0].parent()) == QTreeViewGroupTransferQueue::Transfering)
				{
					if (list.size() <= 1)
					{
						m_pTransferInstructor->queryInterruptTransferToTarget();
					}
					else
					{
						m_pTreeViewTransferQueue->removeItem(list[1].row(), list[1].parent());

						list=m_pTreeViewTransferQueue->selectedRows();
						continue;
					}
				}

				m_pTreeViewTransferQueue->removeItem(list[0].row(), list[0].parent());

				list=m_pTreeViewTransferQueue->selectedRows();
			}
		}
		break;
	case ActionDeleteAllTask:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, tr("Delete all tasks..."));

			m_pTreeViewTransferQueue->clear();

			m_pTransferInstructor->queryInterruptTransferToTarget();
		}
		break;
	default:
		break;
	}
}
