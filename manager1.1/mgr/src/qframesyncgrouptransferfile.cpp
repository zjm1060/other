#include "qframesyncgrouptransferfile.h"

#include "qlogger.h"
#include "qreceivertransferfile.h"
#include "qglobalinfo.h"

QFrameSyncGroupTransferFile::QFrameSyncGroupTransferFile(const QString& record, const QString& subPath, const QString& treeview, QWidget *parent/* =0 */)
	: QFrame(parent)
	,m_bSync(false)
	,m_nRow(-1)
	,m_strRecord(record)
{
	setupUi(this);

	setDefaultDescText(tr("Release the local updated files."));

	m_pButtonWorkspace=new QLinkButton(fileIcon(Dir), "", this);
	m_pButtonWorkspace->setReadOnly(true);
	verticalLayoutWorkspace->addWidget(m_pButtonWorkspace);

	m_pButtonCommand=new QLinkButton(tr("Associate"), this);
	horizontalLayoutButtons->addWidget(m_pButtonCommand);
	connect(m_pButtonCommand, SIGNAL(clicked()), this, SLOT(onClickedBtnCommand()));

	m_pButtonUpload=new QLinkButton(tr("Release"), this);
	horizontalLayoutButtons->addWidget(m_pButtonUpload);
	connect(m_pButtonUpload, SIGNAL(clicked()), this, SLOT(onClickedBtnCommit()));

	connect(pushButtonBack, SIGNAL(clicked()), this, SIGNAL(goBack()));

	m_pButtonCommand->installEventFilter(this);
	m_pButtonUpload->installEventFilter(this);

	m_pTreeViewFiles=treeViewUpdateFileDynamicClass(treeview);

	if (m_pTreeViewFiles != NULL)
	{
		verticalLayoutFiles->addWidget(m_pTreeViewFiles);

		connect(m_pTreeViewFiles, SIGNAL(mouseRightButtonPressed(QPoint)), this, SLOT(mouseRightButtonPressed(QPoint)));
		connect(m_pTreeViewFiles, SIGNAL(selectedChanged()), this, SLOT(selectedChanged()));

		m_pTreeViewFiles->installEventFilter(this);
	}

	m_pListViewReveiver=new QListViewFileReceiver(appendPath(getAppParentDir(), SYNC_H9000_FILE_COMMON_CONF_FILE), record, subPath, this);
	verticalLayoutReceivers->addWidget(m_pListViewReveiver);

	connect(m_pListViewReveiver, SIGNAL(selectedChanged()), this, SLOT(selectedChanged()));

	m_pTransferInstructor=new QTransferInstructor(this);
	connect(m_pTransferInstructor, SIGNAL(sendMessage(QTransferInstructor::MessageType, const QString&)), 
		this, SLOT(recvMessage(QTransferInstructor::MessageType, const QString&)));
	connect(m_pTransferInstructor, SIGNAL(addSubTask(const QGroupTransferQueue::TransferQueueItem&, QVector<FileInfo>)),
		this, SLOT(addSubTask(const QGroupTransferQueue::TransferQueueItem&, QVector<FileInfo>)), Qt::DirectConnection);
	connect(m_pTransferInstructor, SIGNAL(transferedFileContents(const QGroupTransferQueue::TransferQueueItem&, quint64)), 
		this, SLOT(transferedFileContents(const QGroupTransferQueue::TransferQueueItem&, quint64)));

	m_pTreeViewQueue=new QTreeViewGroupTransferQueue(this);
	m_pTreeViewQueue->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	m_pTreeViewQueue->hideColumn(QTreeViewGroupTransferQueue::StatusTitle);
	m_pTreeViewQueue->showAbsolutePath(false);
	verticalLayoutQueue->addWidget(m_pTreeViewQueue);

	connect(m_pTreeViewQueue, SIGNAL(synchronize()), this, SLOT(synchronize()));
	connect(m_pTreeViewQueue, SIGNAL(mouseRightButtonPressed(QPoint)), this, SLOT(mouseRightButtonPressed(QPoint)));

	m_pListViewPrompt=new QListViewPrompt(this);
	verticalLayoutPrompt->addWidget(m_pListViewPrompt);

	//弹出菜单
	m_pMenu=new QMenu(this);
	m_pActions[ActionUpload]=new QAction(QIcon(":/images/menuupload.png"), tr("&Release"), m_pMenu);
	m_pActions[ActionSelectAll]=new QAction(tr("Select &All"), m_pMenu);
	m_pActions[ActionSelectNone]=new QAction(tr("Select &None"), m_pMenu);

	m_pMenu->addAction(m_pActions[ActionUpload]);
	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionSelectAll]);
	m_pMenu->addAction(m_pActions[ActionSelectNone]);

	m_pMenuTask=new QMenu(this);
	m_pActions[ActionRetransmit]=new QAction(QIcon(":/images/retransmit.png"), tr("&Retransmit"), m_pMenuTask);
	m_pActions[ActionDeleteTask]=new QAction(QIcon(":/images/delete.png"), tr("&Delete"), m_pMenuTask);
	m_pActions[ActionDeleteAllTask]=new QAction(tr("&Delete All"), m_pMenuTask);

	m_pMenuTask->addAction(m_pActions[ActionRetransmit]);
	m_pMenuTask->addSeparator();
	m_pMenuTask->addAction(m_pActions[ActionDeleteTask]);
	m_pMenuTask->addAction(m_pActions[ActionDeleteAllTask]);

	//菜单触发事件
	m_pSignalMapper=new QSignalMapper(this);
	for (int i=0; i<ActionEnd; i++)
	{
		connect(m_pActions[i], SIGNAL(triggered()), m_pSignalMapper, SLOT(map()));
		m_pSignalMapper->setMapping(m_pActions[i], i);
	}

	connect(m_pSignalMapper, SIGNAL(mapped(int)), this, SLOT(actionTriggered(int)));

	m_pMenu->installEventFilter(this);
}

QFrameSyncGroupTransferFile::~QFrameSyncGroupTransferFile()
{
	m_pButtonCommand->removeEventFilter(this);
	m_pButtonUpload->removeEventFilter(this);

	if (m_pTreeViewFiles != NULL)
	{
		m_pTreeViewFiles->removeEventFilter(this);
	}

	m_pMenu->removeEventFilter(this);
}

void QFrameSyncGroupTransferFile::setTitleText(const QString& title)
{
	labelTitle->setText(title);
}

void QFrameSyncGroupTransferFile::setDefaultDescText(const QString& desc)
{
	if (desc.isEmpty())
	{
		return;
	}

	m_strDefaultDesc=desc;

	prompt(QReceiverTransferFile::Information, desc);
}

void QFrameSyncGroupTransferFile::setThemePixmap(const QPixmap& icon)
{
	labelIcon->setPixmap(icon);
}

void QFrameSyncGroupTransferFile::showTipsPixmap(bool show)
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

void QFrameSyncGroupTransferFile::setServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	if (m_pListViewReveiver == NULL)
	{
		return;
	}

	QListIterator<QPair<ServerInfo, int> > it(servers);
	while (it.hasNext())
	{
		const QPair<ServerInfo, int>& info=it.next();

		m_pListViewReveiver->addReceiver(info);
	}
}

void QFrameSyncGroupTransferFile::setWorkspace(const QString& workspace)
{
	if (m_pButtonWorkspace != NULL)
	{
		m_pButtonWorkspace->setText(workspace);
	}
}

void QFrameSyncGroupTransferFile::setTreeModel(QTreeModel* model)
{
	if (m_pTreeViewFiles != NULL)
	{
		m_pTreeViewFiles->setCustomModel(model);

		m_pButtonUpload->setEnabled(m_pTreeViewFiles->hasSelected());
	}
}

void QFrameSyncGroupTransferFile::prompt(int flag, const QString& tip)
{
	if (tip.isEmpty())
	{
		setDefaultDescText(m_strDefaultDesc);
		return;
	}

	QPixmap pixmap;
	switch (flag)
	{
	case QReceiverTransferFile::Information:
		pixmap=findPixmap(":/images/information.png");
		break;
	case QReceiverTransferFile::Warning:
		pixmap=findPixmap(":/images/warning.png");
		break;
	case QReceiverTransferFile::Error:
		pixmap=findPixmap(":/images/error1.png");
		break;
	case QReceiverTransferFile::Download:
		pixmap=findPixmap(":/images/download.png");
		break;
	case QReceiverTransferFile::Upload:
		pixmap=findPixmap(":/images/upload.png");
		break;
	case QReceiverTransferFile::Success:
		pixmap=findPixmap(":/images/tick.png");
		break;
	case QReceiverTransferFile::Unknown:
		pixmap=findPixmap(":/images/unknown.png");
		break;
	default:
		break;
	}

	labelInfoIcon->setPixmap(pixmap);
	labelDesc->setText(tip);

	showTipsPixmap(!pixmap.isNull());
}

bool QFrameSyncGroupTransferFile::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::Enter)
	{
		if (watched == m_pButtonCommand)
		{
			prompt(QReceiverTransferFile::Information, tr("Click to set the default operations when the associated files are modified."));
		}
		else if (watched == m_pButtonUpload)
		{
			if (m_pButtonUpload->isEnabled())
			{
				prompt(QReceiverTransferFile::Information, tr("Click to release the local updated files."));
			}
			else
			{
				prompt(QReceiverTransferFile::Warning, tr("Before releasing files, the files and receivers must be selected!"));
			}
		}
		else if (watched == m_pTreeViewFiles)
		{
			prompt(QReceiverTransferFile::Information, tr("Select files that you want to release."));
		}
		else if (watched == m_pMenu)
		{
			if (!m_pListViewReveiver->hasSelected())
			{
				prompt(QReceiverTransferFile::Warning, tr("Please select a receivers at least that you want to release!"));
			}
			else if (!m_pTreeViewFiles->hasSelected())
			{
				prompt(QReceiverTransferFile::Warning, tr("Please select a file at least that you want to release!"));
			}
		}
	}
	else if (event->type() == QEvent::Leave)
	{
		prompt(QReceiverTransferFile::Information, "");
	}

	return QFrame::eventFilter(watched, event);
}

void QFrameSyncGroupTransferFile::mouseRightButtonPressed(QPoint pos)
{
	QObject* pSender=sender();
	if (pSender == m_pTreeViewFiles)
	{
		if (!m_bSync && m_pListViewReveiver->hasSelected() && m_pTreeViewFiles->hasSelected())
		{
			m_pActions[ActionUpload]->setEnabled(true);
		}
		else
		{
			m_pActions[ActionUpload]->setEnabled(false);
		}

		QItemSelectionModel* pModel=m_pTreeViewFiles->selectionModel();
		if (pModel != NULL && !pModel->selectedIndexes().isEmpty())
		{
			m_pActions[ActionSelectNone]->setEnabled(true);
		}
		else
		{
			m_pActions[ActionSelectNone]->setEnabled(false);
		}

		m_pMenu->popup(m_pTreeViewFiles->mapToGlobal(pos));
	}
	else if (pSender == m_pTreeViewQueue)
	{
		QModelIndexList list=m_pTreeViewQueue->selectedRows();
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
				if (index.isValid() && m_pTreeViewQueue->itemTransferStatus(index.row(), index.parent()) == QTreeViewGroupTransferQueue::Failed)
				{
					bRetransmit=true;
					break;
				}
			}

			m_pActions[ActionRetransmit]->setEnabled(bRetransmit);
			m_pActions[ActionDeleteTask]->setEnabled(true);
		}

		if (m_pTreeViewQueue->taskCount() > 0)
		{
			m_pActions[ActionDeleteAllTask]->setEnabled(true);
		}
		else
		{
			m_pActions[ActionDeleteAllTask]->setEnabled(false);
		}

		m_pMenuTask->popup(m_pTreeViewQueue->mapToGlobal(pos));
	}
}

void QFrameSyncGroupTransferFile::selectedChanged()
{
	if (m_pTreeViewFiles == NULL || m_pListViewReveiver == NULL)
	{
		return;
	}

	m_pButtonUpload->setEnabled(m_pTreeViewFiles->hasSelected() && m_pListViewReveiver->hasSelected());
}

void QFrameSyncGroupTransferFile::actionTriggered(int index)
{
	if (m_pTreeViewFiles == NULL || m_pTreeViewQueue == NULL || m_pListViewPrompt == NULL)
	{
		return;
	}

	switch (index)
	{
	case ActionUpload:
		onClickedBtnCommit();
		break;
	case ActionSelectAll:
		onClickedBtnSelectAll();
		break;
	case ActionSelectNone:
		onClickedBtnSelectNone();
		break;
	case ActionRetransmit:
		{
			QModelIndexList list=m_pTreeViewQueue->selectedRows();
			if (!list.isEmpty())
			{
				m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, tr("Begin to retransmit..."));
				foreach (QModelIndex index, list)
				{
					if (m_pTreeViewQueue->itemTransferStatus(index.row(), index.parent()) == QTreeViewGroupTransferQueue::Failed)
					{
						m_pTreeViewQueue->setItemTransferStatus(index.row(), QTreeViewGroupTransferQueue::Waiting, index.parent());
					}
				}
			}
		}
		break;
	case ActionDeleteTask:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, tr("Delete tasks..."));
			QModelIndexList list=m_pTreeViewQueue->selectedRows();
			while (!list.isEmpty())
			{
				if (m_pTreeViewQueue->itemTransferStatus(list[0].row(), list[0].parent()) == QTreeViewGroupTransferQueue::Transfering)
				{
					if (list.size() <= 1)
					{
						m_pTransferInstructor->queryInterruptTransferToTarget();
					}
					else
					{
						m_pTreeViewQueue->removeItem(list[1].row(), list[1].parent());

						list=m_pTreeViewQueue->selectedRows();
						continue;
					}
				}

				m_pTreeViewQueue->removeItem(list[0].row(), list[0].parent());

				list=m_pTreeViewQueue->selectedRows();
			}
		}
		break;
	case ActionDeleteAllTask:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, tr("Delete all tasks..."));

			m_pTreeViewQueue->clear();

			m_pTransferInstructor->queryInterruptTransferToTarget();
		}
		break;
	default:
		break;
	}
}

void QFrameSyncGroupTransferFile::setFileItemsCheckState(int state)
{
	if (m_pTreeViewFiles != NULL)
	{
		int nRowCount=m_pTreeViewFiles->rowCount();
		for (int nRow=0; nRow<nRowCount; nRow++)
		{
			m_pTreeViewFiles->setItemCheckState(nRow, state);
		}

		selectedChanged();
	}
}

void QFrameSyncGroupTransferFile::onClickedBtnSelect()
{
	if (m_pTreeViewFiles == NULL)
	{
		return;
	}

	QItemSelectionModel* pSelectionModel=m_pTreeViewFiles->selectionModel();
	if (pSelectionModel != NULL)
	{
		QModelIndexList indexes=pSelectionModel->selectedIndexes();
		foreach (QModelIndex index, indexes)
		{
			index=m_pTreeViewFiles->mapToSourceItem(index);
			m_pTreeViewFiles->setItemCheckState(index.row(), Qt::Checked, index.parent());
		}

		selectedChanged();
	}
}

void QFrameSyncGroupTransferFile::onClickedBtnSelectAll()
{
	setFileItemsCheckState(Qt::Checked);
}

void QFrameSyncGroupTransferFile::onClickedBtnSelectNone()
{
	setFileItemsCheckState(Qt::Unchecked);
}

void QFrameSyncGroupTransferFile::onClickedBtnCommand()
{
	if (m_pButtonWorkspace == NULL)
	{
		return;
	}

	emit command(m_pButtonWorkspace->text());
}

void QFrameSyncGroupTransferFile::onClickedBtnCommit()
{
	if (m_pTreeViewFiles == NULL || !m_pTreeViewFiles->hasSelected() || m_pListViewReveiver == NULL || !m_pListViewReveiver->hasSelected())
	{
		return;
	}

	QVector<QPair<DiffFile, FileInfo> > files=m_pTreeViewFiles->selectedItems();
	if (files.count() <= 0)
	{
		return;
	}

	QGroupTransferQueue::TransferQueueItem task;

	task.sourceHost=QGlobalInfo::instance()->local();
	task.isTop=false;
	task.source=m_pButtonWorkspace->text();

	task.format=AutoFormat;

	ServerInfo info;

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

		QTreeViewGroupTransferQueue::TransferQueueCommandItem command;
		command.name="Command";
		command.workspace=task.target;

		QModelIndex pindex=m_pTreeViewQueue->appendPeerItem(peer);

		TransferFileItem file;
		for (int i=0; i<files.size(); i++)
		{
			file.name=files[i].second.name;
			file.size=files[i].second.size;
			file.type=files[i].second.type;
			file.typeDesc=fileType(files[i].second);
			item.file=file;

			m_pTreeViewQueue->appendItem(item, NonVersioned, pindex);

			command.files.append(files[i].second);
		}

		if (!command.files.isEmpty())
		{
			m_pTreeViewQueue->appendCommandItem(command, pindex);
		}

	} while (true);

	synchronize();
}

void QFrameSyncGroupTransferFile::synchronize()
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

void QFrameSyncGroupTransferFile::recvMessage(QTransferInstructor::MessageType type, const QString& message)
{
	switch (type)
	{
	case QTransferInstructor::ConnectToTargetSuccess:
		{
			m_pTreeViewQueue->setItemTransferStatus(m_index.parent().row(), QTreeViewGroupTransferQueue::Connected, m_index.parent().parent());
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, message);
		}
		break;
	case QTransferInstructor::ConnectingToSource:
		{
			m_pTreeViewQueue->setItemTransferStatus(m_index.parent().row(), QTreeViewGroupTransferQueue::Connecting, m_index.parent().parent());
		}
	case QTransferInstructor::ConnectToSourceSuccess:
	case QTransferInstructor::SyncCommand:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, message);
		}
		break;
	case QTransferInstructor::ConnectToTargetFailed:
	case QTransferInstructor::DisconnectedToSourceHost:
		{
			m_pTreeViewQueue->setItemTransferStatus(m_index.parent().row(), QTreeViewGroupTransferQueue::Unconneced, m_index.parent().parent());

			m_pTreeViewQueue->collapse(m_index.parent());
		}
	case QTransferInstructor::ConnectToSourceFailed:
	case QTransferInstructor::UploadFail:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Error, message);
			m_pTreeViewQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Failed, m_index.parent());

			fetchTask();
		}
		break;
	case QTransferInstructor::QueryUpload:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Upload, message);
			m_pTreeViewQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Transfering, m_index.parent());
		}
		break;
	case QTransferInstructor::UploadSuccess:
	case QTransferInstructor::UploadDirSuccess:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Success, message);
			m_pTreeViewQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Finished, m_index.parent());

			fetchTask();
		}
		break;
	case QTransferInstructor::UploadWarning:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Warning, message);
			m_pTreeViewQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Transfering, m_index.parent());
		}
		break;
	case QTransferInstructor::SyncCommandFailed:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Error, message);
		}
		break;
	case QTransferInstructor::SyncCommandFinished:
		{
			m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, message);
			m_pTreeViewQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Finished, m_index.parent());

			fetchTask();
		}
		break;
	default:
		qDebug("QFrameFileTransferGroup::recvMessage invalid message type!!!");
		break;
	}
}

void QFrameSyncGroupTransferFile::addSubTask(const QGroupTransferQueue::TransferQueueItem& task, QVector<FileInfo> files)
{
	Q_UNUSED(task);
	Q_UNUSED(files);
}

void QFrameSyncGroupTransferFile::transferedFileContents(const QGroupTransferQueue::TransferQueueItem& task, quint64 size)
{
	if (m_task != task)
	{
		return;
	}

	m_pTreeViewQueue->addItemProcess(m_index.row(), size, m_index.parent());
}

void QFrameSyncGroupTransferFile::fetchTask()
{
	if (!m_bSync)
	{
		return;
	}

	int nRole;
	QVariant var;
	int nStatus;
	QModelIndex index=m_pTreeViewQueue->fetchTask(nRole, var, nStatus);

	transfer(index, nRole, var);
}

void QFrameSyncGroupTransferFile::transfer(const QModelIndex& index, int role, const QVariant& item)
{
	if (!index.isValid())
	{
		//同步完毕
		finished();

		return;
	}

	m_index=index;

	if (role == QTreeViewGroupTransferQueue::FileRole)
	{
		m_task=qvariant_cast<QGroupTransferQueue::TransferQueueItem>(item);

		m_pTreeViewQueue->expand(m_index.parent());

		m_pTransferInstructor->addTask(m_task);
	}
	else if (role == QTreeViewGroupTransferQueue::BackUpRole)
	{
		QTreeViewGroupTransferQueue::TransferQueueBackupItem task=qvariant_cast<QTreeViewGroupTransferQueue::TransferQueueBackupItem>(item);
		m_pTransferInstructor->addBackupTask(task);
	}
	else if (role == QTreeViewGroupTransferQueue::CommandRole)
	{
		QTreeViewGroupTransferQueue::TransferQueueCommandItem task=qvariant_cast<QTreeViewGroupTransferQueue::TransferQueueCommandItem>(item);
		if (m_pTreeViewQueue->taskCount(index.parent()) == 1)
		{
			m_pTransferInstructor->addCommandTask(task);
		}
		else
		{
			m_pTreeViewQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Skip, m_index.parent());

			fetchTask();
		}
	}
}

void QFrameSyncGroupTransferFile::finished()
{
	m_bSync=false;

	m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, tr("Synchronization finished."));
}
