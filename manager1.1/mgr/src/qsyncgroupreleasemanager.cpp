#include "qsyncgroupreleasemanager.h"

#include "common.h"
#include "qglobalinfo.h"

#include <QMessageBox>

QSyncGroupReleaseManager::QSyncGroupReleaseManager(QWidget *parent)
	: QFrame(parent)
	,m_bSync(false)
	,m_nRow(-1)
{
	memset(m_pFrameFiles, 0, sizeof(QFrameSyncGroupReleaseFile*)*ButtonEnd);
	memset(m_pFrameReceiver, 0, sizeof(QFrameFileReceiver*)*ButtonEnd);

	m_ui=new Ui::FrameSyncGroupReleaseFile;
	m_ui->setupUi(this);

	connect(m_ui->pushButtonRelease, SIGNAL(clicked()), this, SLOT(onClickedBtnRelease()));

	m_pButtonList=new QButtonList(this);
	m_pButtonList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	m_pButtonList->setFixedWidth(205);

	m_pButtonList->addCheckableButton(tr("H9000 Config Files"), ButtonConfigFiles);
	m_pButtonList->addCheckableButton(tr("H9000 Pictures"), ButtonPictures);
	m_pButtonList->addCheckableButton(tr("H9000 DB Define files"), ButtonDBDefine);
	m_pButtonList->addCheckableButton(tr("H9000 Data Files"), ButtonDatFiles);
	m_pButtonList->addCheckableButton(tr("H9000 Real-time DB File"), ButtonRtdbFile);
	m_pButtonList->addCheckableButton(tr("H9000 IEC104 Files"), ButtonIEC104Files);
	m_pButtonList->addCheckableButton(tr("H9000 Sound Files"), ButtonSoundFiles);

	m_ui->horizontalLayoutButtons->addWidget(m_pButtonList);

	connect(m_pButtonList, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));
	connect(m_pButtonList, SIGNAL(buttonChecked(int, bool)), this, SLOT(buttonChecked(int, bool)));

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
	m_ui->horizontalLayoutQueue->addWidget(m_pTreeViewQueue);

	connect(m_pTreeViewQueue, SIGNAL(synchronize()), this, SLOT(synchronize()));
	connect(m_pTreeViewQueue, SIGNAL(mouseRightButtonPressed(QPoint)), this, SLOT(mouseRightButtonPressed(QPoint)));

	m_pListViewPrompt=new QListViewPrompt(this);
	m_ui->horizontalLayoutPrompt->addWidget(m_pListViewPrompt);

	m_pMenuQueue=new QMenu(this);
	m_pActions[ActionCancel]=new QAction(tr("&Cancel"), m_pMenuQueue);
	m_pActions[ActionDeleteAll]=new QAction(QIcon(":/images/delete.png"), tr("&Delete All"), m_pMenuQueue);
	m_pActions[ActionRetransmit]=new QAction(QIcon(":/images/retransmit.png"), tr("&Retransmit"), m_pMenuQueue);

	m_pMenuQueue->addAction(m_pActions[ActionRetransmit]);
	m_pMenuQueue->addSeparator();
	m_pMenuQueue->addAction(m_pActions[ActionCancel]);
	m_pMenuQueue->addAction(m_pActions[ActionDeleteAll]);

	//菜单触发事件
	m_pSignalMapper=new QSignalMapper(this);
	for (int i=0; i<ActionEnd; i++)
	{
		connect(m_pActions[i], SIGNAL(triggered()), m_pSignalMapper, SLOT(map()));
		m_pSignalMapper->setMapping(m_pActions[i], i);
	}

	connect(m_pSignalMapper, SIGNAL(mapped(int)), this, SLOT(actionTriggered(int)));

	m_pActions[ActionCancel]->setEnabled(false);

	m_pButtonList->click(ButtonConfigFiles);
}

QSyncGroupReleaseManager::~QSyncGroupReleaseManager()
{
	
}

void QSyncGroupReleaseManager::setServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	Q_UNUSED(servers);

	for (int i=0; i<ButtonEnd; i++)
	{
		if (m_pFrameReceiver[i] != NULL)
		{
			m_pFrameReceiver[i]->refreshWorkspace();
		}
	}

	setMenuState(false);
}

void QSyncGroupReleaseManager::setupUi(int id)
{
	Q_ASSERT(id>=0 && id<ButtonEnd);

	if (m_pFrameFiles[id] == NULL)
	{
		//添加文件列表
		switch (id)
		{
		case ButtonConfigFiles:
			{
				m_pFrameFiles[id]=new QFrameSyncGroupReleaseFile("QTreeViewUpdateConfFile", true);
			}
			break;
		case ButtonPictures:
			{
				m_pFrameFiles[id]=new QFrameSyncGroupReleaseFile("QTreeViewUpdateDspFile", true);
			}
			break;
		case ButtonDBDefine:
			{
				m_pFrameFiles[id]=new QFrameSyncGroupReleaseFile("QTreeViewUpdateDbFile", true);
			}
			break;
		case ButtonDatFiles:
			{
				m_pFrameFiles[id]=new QFrameSyncGroupReleaseFile("QTreeViewUpdateDatFile", true);
			}
			break;
		case ButtonRtdbFile:
			{
				m_pFrameFiles[id]=new QFrameSyncGroupReleaseFile("QTreeViewUpdateRtdbFile", false);
			}
			break;
		case ButtonIEC104Files:
			{
				m_pFrameFiles[id]=new QFrameSyncGroupReleaseFile("QTreeViewUpdateXLDIEC104File", true);
			}
			break;
		case ButtonSoundFiles:
			{
				m_pFrameFiles[id]=new QFrameSyncGroupReleaseFile("QTreeViewUpdateVoiceFile", true);
			}
			break;
		default:
			break;
		}

		SAFE_HIDE_WIDGET(m_pFrameFiles[id]);

		m_pFrameFiles[id]->setLocalLastModifiedEnabled(true);
		
		m_ui->horizontalLayoutWorkspace->addWidget(m_pFrameFiles[id]);

		if (m_pFrameFiles[id] != NULL)
		{
			connect(m_pFrameFiles[id], SIGNAL(loaded(bool)), this, SLOT(loaded(bool)));
			connect(m_pFrameFiles[id], SIGNAL(selectedChanged()), this, SLOT(selectedChanged()));
			connect(m_pFrameFiles[id], SIGNAL(sendTip(int, QString)), this, SLOT(prompt(int, QString)));
		}
	}

	if (m_pFrameReceiver[id] == NULL)
	{
		//添加文件列表
		switch (id)
		{
		case ButtonConfigFiles:
			{
				m_pFrameReceiver[id]=new QFrameFileReceiver("config", H9000_DEF_PATH);
			}
			break;
		case ButtonPictures:
			{
				m_pFrameReceiver[id]=new QFrameFileReceiver("dsp", H9000_DSP_PATH);
			}
			break;
		case ButtonDBDefine:
			{
				m_pFrameReceiver[id]=new QFrameFileReceiver("database", H9000_DATABASE_PATH);
			}
			break;
		case ButtonDatFiles:
			{
				m_pFrameReceiver[id]=new QFrameFileReceiver("dat", H9000_DAT_PATH);
			}
			break;
		case ButtonRtdbFile:
			{
				m_pFrameReceiver[id]=new QFrameFileReceiver("rtdb", appendPath(H9000_DAT_PATH, H9000_TMP_PATH));
			}
			break;
		case ButtonIEC104Files:
			{
				m_pFrameReceiver[id]=new QFrameFileReceiver("iec104", H9000_IEC104_PATH);
			}
			break;
		case ButtonSoundFiles:
			{
				m_pFrameReceiver[id]=new QFrameFileReceiver("wav", H9000_VOICE_PATH);
			}
			break;
		default:
			break;
		}

		SAFE_HIDE_WIDGET(m_pFrameReceiver[id]);
		
		m_ui->horizontalLayoutServers->addWidget(m_pFrameReceiver[id]);

		if (m_pFrameReceiver[id] != NULL)
		{
			connect(m_pFrameReceiver[id], SIGNAL(selectedChanged()), this, SLOT(selectedChanged()));
			connect(m_pFrameReceiver[id], SIGNAL(sendTip(int, QString)), this, SLOT(prompt(int, QString)));
		}
	}
}

void QSyncGroupReleaseManager::buttonChecked(int id, bool checked)
{
	if (checked)
	{
		setupUi(id);
	}

	selectedChanged();
}

void QSyncGroupReleaseManager::selectedChanged()
{
	bool bLoaded=checkLoaded();

	setMenuState(!bLoaded);
}

void QSyncGroupReleaseManager::buttonClicked(int id)
{
	Q_ASSERT(id >=0 && id < ButtonEnd);

	for (int i=0; i<ButtonEnd; i++)
	{
		SAFE_HIDE_WIDGET(m_pFrameFiles[i]);
		SAFE_HIDE_WIDGET(m_pFrameReceiver[i]);
	}

	setupUi(id);

	switch (id)
	{
	case ButtonConfigFiles:
		{
			setThemePixmap(findPixmap(":/images/config.png"));
			setTitleText(tr("H9000 configure files"));
			setDefaultDescText(tr("Select the local updated H9000 configure files to release."));
		}
		break;
	case ButtonPictures:
		{
			setThemePixmap(findPixmap(":/images/picture.png"));
			setTitleText(tr("H9000 dsp files"));
			setDefaultDescText(tr("Select the local updated H9000 dsp files to release."));
		}
		break;
	case ButtonDBDefine:
		{
			setThemePixmap(findPixmap(":/images/dbfile.png"));
			setTitleText(tr("H9000 database define files"));
			setDefaultDescText(tr("Select the local updated H9000 database define files to release."));
		}
		break;
	case ButtonDatFiles:
		{
			setThemePixmap(findPixmap(":/images/dat.png"));
			setTitleText(tr("H9000 data files"));
			setDefaultDescText(tr("Select the local updated H9000 data files to release."));
		}
		break;
	case ButtonRtdbFile:
		{
			setThemePixmap(findPixmap(":/images/dat.png"));
			setTitleText(tr("H9000 RTDB file"));
			setDefaultDescText(tr("Select the local updated H9000 real-time database file to release.")+
				"<font color=#FF0000>"+tr("!!! Note:Don't cover directly real-time database file!")+"</font>");
		}
		break;
	case ButtonIEC104Files:
		{
			setThemePixmap(findPixmap(":/images/iec.png"));
			setTitleText(tr("H9000 IEC104 files"));
			setDefaultDescText(tr("Select the local updated H9000 IEC104 files to release."));
		}
		break;
	case ButtonSoundFiles:
		{
			setThemePixmap(findPixmap(":/images/voice.png"));
			setTitleText(tr("H9000 voice files"));
			setDefaultDescText(tr("Select the local updated H9000 voice files to release."));
		}
		break;
	default:
		break;
	}

	m_pFrameFiles[id]->show();
	m_pFrameReceiver[id]->show();
}

bool QSyncGroupReleaseManager::checkLoaded() const
{
	bool bLoaded=true;

	for (int i=0; i<ButtonEnd; i++)
	{
		if (m_pFrameFiles[i] != NULL && !m_pFrameFiles[i]->isLoaded())
		{
			bLoaded=false;

			break;
		}
	}

	return bLoaded;
}

void QSyncGroupReleaseManager::loaded(bool state)
{
	if (state)
	{
		bool bLoaded=checkLoaded();

		if (bLoaded)
		{
			prompt(QReceiverTransferFile::Information, tr("Loaded."));
		}

		setMenuState(!bLoaded);
	}
	else
	{
		setMenuState(true);
	}
}

void QSyncGroupReleaseManager::setMenuState(bool refresh)
{
	if (refresh)
	{
		prompt(QReceiverTransferFile::Information, tr("Loading, please wait..."));

		m_ui->pushButtonRelease->setEnabled(false);
	}
	else
	{
		bool hasSelected=false;

		QVector<int> indexes=m_pButtonList->checkedButtons();
		if (!indexes.isEmpty())
		{
			for (int i=0; i<indexes.size(); i++)
			{
				int index=indexes[i];
				Q_ASSERT(index >=0 && index < ButtonEnd);

				if (m_pFrameFiles[index] != NULL && m_pFrameFiles[index]->hasSelected() && 
					m_pFrameReceiver[index] != NULL && m_pFrameReceiver[index]->hasSelected())
				{
					hasSelected=true;
				}
			}
		}

		m_ui->pushButtonRelease->setEnabled(hasSelected);
	}
}

void QSyncGroupReleaseManager::prompt(int flag, const QString& tip)
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

	m_ui->labelInfoIcon->setPixmap(pixmap);
	m_ui->labelDesc->setText(tip);

	showTipsPixmap(!pixmap.isNull());
}

void QSyncGroupReleaseManager::setTitleText(const QString& title)
{
	m_ui->labelTitle->setText(title);
}

void QSyncGroupReleaseManager::setDefaultDescText(const QString& desc)
{
	if (desc.isEmpty())
	{
		return;
	}

	m_strDefaultDesc=desc;

	prompt(QReceiverTransferFile::Information, desc);
}

void QSyncGroupReleaseManager::setThemePixmap(const QPixmap& icon)
{
	m_ui->labelIcon->setPixmap(icon);
}

void QSyncGroupReleaseManager::showTipsPixmap(bool show)
{
	if (show)
	{
		m_ui->labelInfoIcon->show();
	}
	else
	{
		m_ui->labelInfoIcon->hide();
	}
}

void QSyncGroupReleaseManager::mouseRightButtonPressed(QPoint pos)
{
	if (!m_bSync && m_pTreeViewQueue->taskCount() > 0)
	{
		m_pActions[ActionRetransmit]->setEnabled(true);
		m_pActions[ActionDeleteAll]->setEnabled(true);
	}
	else
	{
		m_pActions[ActionRetransmit]->setEnabled(false);
		m_pActions[ActionDeleteAll]->setEnabled(false);
	}

	m_pMenuQueue->popup(m_pTreeViewQueue->mapToGlobal(pos));
}

void QSyncGroupReleaseManager::onClickedBtnCancel()
{
	int nRole;
	QVariant var;
	int nStatus;
	QModelIndex index=m_pTreeViewQueue->fetchTask(nRole, var, nStatus);
	if (index.isValid() && nRole == QTreeViewGroupTransferQueue::FileRole)
	{
		m_pTransferInstructor->queryInterruptTransferToTarget();

		finished();
	}
}

void QSyncGroupReleaseManager::actionTriggered(int index)
{
	if (m_pTreeViewQueue == NULL)
	{
		return;
	}

	switch (index)
	{
	case ActionCancel:
		onClickedBtnCancel();
		break;
	case ActionRetransmit:
		{
			m_bSync=true;
			m_pActions[ActionCancel]->setEnabled(true);

			int nNumRows=m_pTreeViewQueue->taskCount();
			for (int i=0; i<nNumRows; i++)
			{
				m_pTreeViewQueue->setItemTransferStatus(i, QTreeViewGroupTransferQueue::Waiting);

				QModelIndex index=m_pTreeViewQueue->index(i);
				int nRows=m_pTreeViewQueue->taskCount(index);
				for (int j=0; j<nRows; j++)
				{
					m_pTreeViewQueue->setItemTransferStatus(j, QTreeViewGroupTransferQueue::Waiting, index);
				}
			}

			prompt(QReceiverTransferFile::Information, tr("Synchronize..."));

			fetchTask();
		}
		break;
	case ActionDeleteAll:
		m_pTreeViewQueue->clear();
		break;
	default:
		break;
	}
}

void QSyncGroupReleaseManager::onClickedBtnRelease()
{
	if (m_pButtonList == NULL)
	{
		return;
	}

	QVector<int> indexes=m_pButtonList->checkedButtons();
	if (indexes.isEmpty())
	{
		return;
	}

	QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"),
		tr("Will cover the corresponding files in the selected servers, and the files will not be recovered!\nPlease make sure the latest files in the local server! Are you sure to cover?"),
		QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);
	if (button == QMessageBox::Cancel)
	{
		return;
	}

	for (int i=0; i<indexes.size(); i++)
	{
		int index=indexes[i];
		Q_ASSERT(index>=0 && index<ButtonEnd);

		if (m_pFrameFiles[index] == NULL || !m_pFrameFiles[index]->hasSelected() || m_pFrameReceiver == NULL || !m_pFrameReceiver[index]->hasSelected())
		{
			continue;
		}

		QVector<QPair<DiffFile, FileInfo> > files=m_pFrameFiles[index]->selectedItems();
		if (files.count() <= 0)
		{
			return;
		}

		QGroupTransferQueue::TransferQueueItem task;

		task.sourceHost=QGlobalInfo::instance()->local();
		task.isTop=false;
		task.source=m_pFrameFiles[index]->workspace();
		if (task.source.isEmpty())
		{
			continue;
		}

		task.format=AutoFormat;

		ServerInfo info;

		int nRow=-1;
		do 
		{
			nRow=m_pFrameReceiver[index]->nextSelected(nRow, info);

			if (nRow == -1)
			{
				break;
			}

			if (info.count == 0)
			{
				continue;
			}

			task.targetHost=info;
			task.isSlient=m_pFrameReceiver[index]->isAddedItem(nRow);
			task.target=m_pFrameReceiver[index]->savePath(nRow);

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
	}

	synchronize();
}

void QSyncGroupReleaseManager::synchronize()
{
	if (m_bSync)
	{
		return;
	}

	m_pListViewPrompt->clear();
	m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, tr("Synchronize..."));

	m_bSync=true;

	m_pActions[ActionCancel]->setEnabled(true);

	fetchTask();
}

void QSyncGroupReleaseManager::fetchTask()
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

void QSyncGroupReleaseManager::transfer(const QModelIndex& index, int role, const QVariant& item)
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

void QSyncGroupReleaseManager::finished()
{
	m_bSync=false;
	m_pActions[ActionCancel]->setEnabled(false);

	m_pTransferInstructor->clear();

	m_pListViewPrompt->appendTip(QReceiverTransferFile::Information, tr("Synchronization finished."));
}

void QSyncGroupReleaseManager::recvMessage(QTransferInstructor::MessageType type, const QString& message)
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

void QSyncGroupReleaseManager::addSubTask(const QGroupTransferQueue::TransferQueueItem& task, QVector<FileInfo> files)
{
	Q_UNUSED(task);
	Q_UNUSED(files);
}

void QSyncGroupReleaseManager::transferedFileContents(const QGroupTransferQueue::TransferQueueItem& task, quint64 size)
{
	if (m_task != task)
	{
		return;
	}

	m_pTreeViewQueue->addItemProcess(m_index.row(), size, m_index.parent());
}
