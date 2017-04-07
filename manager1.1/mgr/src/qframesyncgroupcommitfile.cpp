#include "qframesyncgroupcommitfile.h"

#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include "define.h"
#include "common.h"
#include "qutildef.h"
#include "qmessageparser.h"
#include "qreceiverparser.h"
#include "qglobalinfo.h"
#include "qmainwndsyncfile.h"
#include "qmessageboxex.h"
#include "qprocesssocket.h"
#include "qreadwritexmlfile.h"
#include "qframesyncfileshowlog.h"
#include "qtipsdelegate.h"
#include "qframesyncgroupupdatefile.h"
#include "qglobalinfo.h"
#include "qlogger.h"

QFrameSyncGroupCommitFile::QFrameSyncGroupCommitFile(const QString& treeview, bool enableRestrict, QWidget *parent/* =0 */)
	: QFrame(parent)
	,m_pListViewServers(NULL)
	,m_bSync(false)
	,m_pButtonWorkspace(NULL)
{
	qRegisterMetaType<QList<QString> >("QList<QString>");

	setupUi(this);

	setDefaultDescText(tr("Commit your modified files."));

	connect(pushButtonCommit, SIGNAL(clicked()), this, SLOT(onClickedBtnOk()));

	//发送端
	m_pButtonWorkspace=new QLinkButton(fileIcon(Dir), "", this);
	verticalLayoutCommonFolder->addWidget(m_pButtonWorkspace);

	connect(m_pButtonWorkspace, SIGNAL(clicked()), this, SLOT(onClickedBtnWorkspace()));

	m_pButtonRefresh=new QLinkButton(tr("Refresh"), this);
	horizontalLayoutRefresh->addWidget(m_pButtonRefresh);

	connect(m_pButtonRefresh, SIGNAL(clicked()), this, SLOT(onClickedBtnRefresh()));

	m_pTreeView=treeViewSyncCommitDynamicClass(treeview);
	if (m_pTreeView != NULL)
	{
		setRestrictFilterEnabled(enableRestrict);

		m_pListViewServers=new QListViewSyncFileServer(m_pTreeView->record(), this);
		verticalLayoutServers->addWidget(m_pListViewServers);

		connect(m_pListViewServers, SIGNAL(sendTips(int, const QString&)), this, SLOT(appendTip(int, const QString&)));
		connect(m_pListViewServers, SIGNAL(selectedChanged()), this, SLOT(selectedChanged()));

		verticalLayoutSyncFile->addWidget(m_pTreeView);

		connect(m_pTreeView, SIGNAL(refreshModelFinished()), this, SLOT(refreshModelFinished()));
		connect(m_pTreeView, SIGNAL(mouseRightButtonPressed(QPoint)), this, SLOT(mouseRightButtonPressed(QPoint)));
		connect(m_pTreeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(doubleClicked(const QModelIndex&)));
		connect(m_pTreeView, SIGNAL(selectedChanged()), this, SLOT(selectedChanged()));
	}

	m_pTransferInstructor=new QTransferInstructor(this);
	connect(m_pTransferInstructor, SIGNAL(sendMessage(QTransferInstructor::MessageType, const QString&)), 
		this, SLOT(recvMessage(QTransferInstructor::MessageType, const QString&)));
	connect(m_pTransferInstructor, SIGNAL(addSubTask(const QGroupTransferQueue::TransferQueueItem&, QVector<FileInfo>)),
		this, SLOT(addSubTask(const QGroupTransferQueue::TransferQueueItem&, QVector<FileInfo>)), Qt::DirectConnection);
	connect(m_pTransferInstructor, SIGNAL(transferedFileContents(const QGroupTransferQueue::TransferQueueItem&, quint64)), 
		this, SLOT(transferedFileContents(const QGroupTransferQueue::TransferQueueItem&, quint64)));
	connect(m_pTransferInstructor, SIGNAL(backupProcess(int, int)), this, SLOT(backupProcess(int, int)));

	m_pTreeViewQueue=new QTreeViewGroupTransferQueue(this);
	m_pTreeViewQueue->showAbsolutePath(false);
	verticalLayoutTransferFile->addWidget(m_pTreeViewQueue);
	m_pTreeViewQueue->installEventFilter(this);

	connect(m_pTreeViewQueue, SIGNAL(mouseRightButtonPressed(QPoint)), this, SLOT(mouseRightButtonPressed(QPoint)));

	m_pMenu=new QMenu(this);
	m_pActions[ActionSync]=new QAction(QIcon(":/images/menuupload.png"), tr("&Commit"), m_pMenu);
	m_pActions[ActionCancel]=new QAction(tr("&Cancel"), m_pMenu);
	m_pActions[ActionDiff]=new QAction(tr("&Compare File"), m_pMenu);
	m_pActions[ActionSelectAll]=new QAction(tr("Select &All"), m_pMenu);
	m_pActions[ActionSelectNone]=new QAction(tr("Select &None"), m_pMenu);
	m_pActions[ActionShowLog]=new QAction(QIcon(":/images/log.png"), tr("Show &Log"), m_pMenu);
	m_pActions[ActionRefresh]=new QAction(QIcon(":/images/refresh.png"), tr("&Refresh"), m_pMenu);

	m_pMenu->addAction(m_pActions[ActionSync]);
	m_pMenu->addAction(m_pActions[ActionCancel]);

	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionDiff]);

	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionSelectAll]);
	m_pMenu->addAction(m_pActions[ActionSelectNone]);

	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionShowLog]);

	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionRefresh]);

	m_pMenuQueue=new QMenu(this);
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

	pushButtonCommit->installEventFilter(this);
	m_pButtonWorkspace->installEventFilter(this);
	m_pButtonRefresh->installEventFilter(this);
	if (m_pTreeView != NULL)
	{
		m_pTreeView->installEventFilter(this);
	}
	m_pMenu->installEventFilter(this);

	if (m_pTreeView != NULL)
	{
		QReadConfigureFormXmlFile configure(appendPath(getAppParentDir(), QString("%1%2.xml").arg(SYNC_H9000_FILE_LOCAL_PATH, m_pTreeView->record())));
		if (configure.read())
		{
			m_configure=configure.result();
		}
	}

	refreshFileSystem();
}

QFrameSyncGroupCommitFile::~QFrameSyncGroupCommitFile()
{
	pushButtonCommit->removeEventFilter(this);
	m_pButtonWorkspace->removeEventFilter(this);
	m_pButtonRefresh->removeEventFilter(this);
	if (m_pTreeView != NULL)
	{
		m_pTreeView->removeEventFilter(this);
	}
	m_pMenu->removeEventFilter(this);
	m_pTreeViewQueue->removeEventFilter(this);
}

void QFrameSyncGroupCommitFile::setRestrictFilterEnabled(bool enable)
{
	if (m_pTreeView != NULL)
	{
		m_pTreeView->setRestrictFilterEnabled(enable);
	}
}

bool QFrameSyncGroupCommitFile::writeConfigure()
{
	bool bRet=false;

	if (m_pTreeView != NULL)
	{
		QWriteConfigureToXmlFile writer(appendPath(getAppParentDir(), QString("%1%2.xml").arg(SYNC_H9000_FILE_LOCAL_PATH, m_pTreeView->record())),
			m_configure);
		bRet=writer.write();
	}

	return bRet;
}

bool QFrameSyncGroupCommitFile::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == pushButtonCommit || watched == m_pButtonWorkspace || watched == m_pButtonRefresh || 
		watched == m_pTreeView || watched == m_pMenu || watched == m_pTreeViewQueue)
	{
		if (event->type() == QEvent::Enter)
		{
			if (watched == pushButtonCommit)
			{
				appendTip(QReceiverTransferFile::Information, tr("Click to commit selected files in the current workspace to selected target servers."));
			}
			else if (watched == m_pButtonWorkspace)
			{
				appendTip(QReceiverTransferFile::Information, tr("Click to switch the workspace."));
			}
			else if (watched == m_pButtonRefresh)
			{
				appendTip(QReceiverTransferFile::Information, tr("Click to reload the workspace."));
			}
			else if (watched == m_pTreeView)
			{
				appendTip(QReceiverTransferFile::Information, tr("Select files that you want to commit."));
			}
			else if (watched == m_pMenu)
			{
				if (!m_pListViewServers->hasSelected())
				{
					appendTip(QReceiverTransferFile::Warning, tr("Please select an end at least that you want to commit to!"));
				}
				else if (!m_pTreeView->hasSelected())
				{
					appendTip(QReceiverTransferFile::Warning, tr("Please select a file at least that you want to commit!"));
				}
			}
			else if (watched == m_pTreeViewQueue)
			{
				if (!m_transferTip.second.isEmpty())
				{
					appendTip(m_transferTip.first, m_transferTip.second);
				}
			}
		}
		else if (event->type() == QEvent::Leave)
		{
			setDefaultDescText(m_strDefaultDesc);
		}
	}

	return QFrame::eventFilter(watched, event);
}

void QFrameSyncGroupCommitFile::setTitleText(const QString& title)
{
	labelTitle->setText(title);
}

void QFrameSyncGroupCommitFile::setDefaultDescText(const QString& desc)
{
	m_strDefaultDesc=desc;

	appendTip(QReceiverTransferFile::Information, desc);
}

void QFrameSyncGroupCommitFile::setThemePixmap(const QPixmap& icon)
{
	labelIcon->setPixmap(icon);
}

void QFrameSyncGroupCommitFile::showTipsPixmap(bool show)
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

void QFrameSyncGroupCommitFile::refreshFileSystem()
{
	if (m_pTreeView == NULL)
	{
		return;
	}

	m_bSync=false;
	m_pActions[ActionCancel]->setEnabled(false);
	m_pActions[ActionSelectAll]->setEnabled(false);
	m_pActions[ActionSelectNone]->setEnabled(false);
	m_pActions[ActionRefresh]->setEnabled(false);
	m_pButtonWorkspace->setEnabled(false);
	m_pButtonRefresh->setEnabled(false);

	readCommonFolder();
}

void QFrameSyncGroupCommitFile::refreshServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	m_pListViewServers->refreshServerList(servers);
}

void QFrameSyncGroupCommitFile::readCommonFolder()
{
	if (m_pTreeView == NULL)
	{
		return;
	}

	QString strFolder;
	if (m_configure.contains("Workspace"))
	{
		strFolder=m_configure["Workspace"];
		convertToStdDir(strFolder);
	}
	else
	{
		strFolder=appendPath(getAppParentDir(), m_pTreeView->commonFolder());
		m_configure["Workspace"]=strFolder;
	}

	m_pButtonWorkspace->setText(strFolder);

	writeConfigure();

	currentTextChanged();
}

void QFrameSyncGroupCommitFile::currentTextChanged()
{
	m_bSync=false;
	m_pActions[ActionCancel]->setEnabled(false);
	m_pActions[ActionSelectAll]->setEnabled(false);
	m_pActions[ActionSelectNone]->setEnabled(false);
	m_pActions[ActionRefresh]->setEnabled(false);
	m_pButtonWorkspace->setEnabled(false);
	m_pButtonRefresh->setEnabled(false);
	pushButtonCommit->setEnabled(false);

	m_mapWnd.clear();

	QString strWorkspace=m_pButtonWorkspace->text();
	if (m_pTreeView != NULL)
	{
		m_pTreeView->check(strWorkspace, appendPath(getAppParentDir(), SYNC_H9000_FILE_LOCAL_PATH));
	}
}

void QFrameSyncGroupCommitFile::refreshModelFinished()
{
	m_bSync=true;
	m_pActions[ActionCancel]->setEnabled(false);
	m_pActions[ActionSelectAll]->setEnabled(true);
	m_pActions[ActionSelectNone]->setEnabled(true);
	m_pActions[ActionRefresh]->setEnabled(true);
	m_pButtonWorkspace->setEnabled(true);
	m_pButtonRefresh->setEnabled(true);

	selectedChanged();
}

void QFrameSyncGroupCommitFile::selectedChanged()
{
	if (m_pListViewServers->hasSelected() && m_pTreeView->hasSelected())
	{
		pushButtonCommit->setEnabled(true);
	}
	else
	{
		pushButtonCommit->setEnabled(false);
	}
}

void QFrameSyncGroupCommitFile::actionTriggered(int index)
{
	if (m_pTreeView == NULL)
	{
		return;
	}

	switch (index)
	{
	case ActionSync:
		onClickedBtnOk();
		break;
	case ActionCancel:
		onClickedBtnCancel();
		break;
	case ActionDiff:
		{
			QModelIndexList items=m_pTreeView->selectedRows();
			foreach (QModelIndex item, items)
			{
				doubleClicked(item);
			}
		}
		break;
	case ActionSelectAll:
		onClickedBtnSelectAll();
		break;
	case ActionSelectNone:
		onClickedBtnSelectNone();
		break;
	case ActionShowLog:
		onClickedBtnShowLog();
		break;
	case ActionRefresh:
		onClickedBtnRefresh();
		break;
	case ActionRetransmit:
		{
			m_bSync=false;
			m_pActions[ActionCancel]->setEnabled(true);
			m_pActions[ActionSelectAll]->setEnabled(false);
			m_pActions[ActionSelectNone]->setEnabled(false);
			m_pActions[ActionRefresh]->setEnabled(false);

			m_pTreeView->setItemsCheckable(false);
			m_pButtonWorkspace->setEnabled(false);
			m_pButtonRefresh->setEnabled(false);

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

			appendTip(QReceiverTransferFile::Information, tr("Synchronize..."));

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

void QFrameSyncGroupCommitFile::onClickedBtnShowLog()
{
	QFrameSyncFileShowLog* log=new QFrameSyncFileShowLog(appendPath(getAppParentDir(), SYNC_H9000_FILE_LOCAL_PATH), m_pTreeView->record());
	log->show();
}

void QFrameSyncGroupCommitFile::onClickedBtnOk()
{
	QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"), tr("Are you sure to commit?"),
		QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);
	if (button == QMessageBox::Cancel)
	{
		return;
	}

	if (!QGlobalInfo::instance()->hasPrivileges(FileSynchronizationPrivilege, CommitFilePrivilege))
	{
		QMessageBox::warning(this, tr("Warning"), tr("You do not have sufficient permissions to commit modified files!"),
			QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	if (!m_bSync)
	{
		return;
	}

	appendTip(QReceiverTransferFile::Information, tr("Synchronize..."));

	m_bSync=false;
	m_pActions[ActionCancel]->setEnabled(true);
	m_pActions[ActionSelectAll]->setEnabled(false);
	m_pActions[ActionSelectNone]->setEnabled(false);
	m_pActions[ActionRefresh]->setEnabled(false);
	m_pActions[ActionShowLog]->setEnabled(false);

	m_pTreeView->setItemsCheckable(false);
	m_pButtonWorkspace->setEnabled(false);
	m_pButtonRefresh->setEnabled(false);

	m_task.sourceHost=QGlobalInfo::instance()->local();
	if (m_task.sourceHost.count == 0)
	{
		appendTip(QReceiverTransferFile::Error, tr("Network failure! please check your network!"));

		//同步完毕
		finished();
		return;
	}

	if (!m_configure.contains("Revision"))
	{
		m_configure.insert("Revision", "0");
	}
	else
	{
		int nRevision=m_configure["Revision"].toInt()+1;
		m_configure["Revision"]=QString("%1").arg(nRevision);
	}

	writeConfigure();

	m_task.source=m_pButtonWorkspace->text();
	m_task.format=AutoFormat;
	m_task.isTop=false;

	ServerInfo info;
	int nRow=-1;

	QVector<QPair<FileInfo, int> > files=m_pTreeView->selectedItems();

	QString strRecordPath=appendPath(getAppParentDir(), QString("%1%2/").arg(SYNC_H9000_FILE_LOCAL_PATH).arg(m_pTreeView->record()))+m_configure["Revision"];
	saveBackupRecord(appendPath(strRecordPath, QString("%1.xml").arg(m_pTreeView->record())), files);
	FileInfo record=entryInfo(QFileInfo(appendPath(strRecordPath, QString("%1.xml").arg(m_pTreeView->record()))), false);

	do 
	{
		nRow=m_pListViewServers->nextSelected(nRow, info);

		if (nRow == -1)
		{
			break;
		}

		if (info.count == 0)
		{
			continue;
		}

		m_task.targetHost=info;
		m_task.isSlient=m_pListViewServers->isAddedItem(nRow);

		Server server;
		QServerManager::findFirstServer(m_task.sourceHost, &server);
		m_task.target=appendPath(info.home.isEmpty()? "/home/ems/h9000":info.home, QString("/ini/data/%1@%2/%3/%4").arg(server.addr)
			.arg(m_task.sourceHost.hostName).arg(m_pTreeView->record()).arg(m_configure["Revision"]));

		QTreeViewGroupTransferQueue::TransferQueuePeerItem peer;
		peer.isSlient=m_task.isSlient;
		peer.source=m_task.sourceHost;
		peer.target=m_task.targetHost;

		QTreeViewGroupTransferQueue::TransferQueueFileItem item;
		item.format=AutoFormat;
		item.isTop=false;
		item.source=m_task.source;
		item.target=m_task.target;

		QModelIndex pindex=m_pTreeViewQueue->appendPeerItem(peer);

		for (int i=0; i<files.size(); i++)
		{
			item.file.name=files[i].first.name;
			item.file.size=files[i].first.size;
			item.file.type=files[i].first.type;
			item.file.typeDesc=fileType(files[i].first);
			m_pTreeViewQueue->appendItem(item, files[i].second==NonVersioned? Added:files[i].second, pindex);
		}

		//传输更改文件
		item.source=strRecordPath;
		item.file.name=record.name;
		item.file.size=record.size;
		item.file.type=record.type;
		item.file.typeDesc=fileType(record);
		m_pTreeViewQueue->appendItem(item, NonVersioned, pindex);
	} while (true);

	QTreeViewGroupTransferQueue::TransferQueueBackupItem item;
	item.source=m_task.source;
	item.target=appendPath(getAppParentDir(), QString("%1%2/").arg(SYNC_H9000_FILE_LOCAL_PATH).arg(m_pTreeView->record()))+m_configure["Revision"];

	item.files.clear();
 	for (int i=0; i<files.size(); i++)
 	{
		if (files[i].second != Deleted)
		{
			item.files.append(files[i].first);
		}
 	}

	m_pTreeViewQueue->appendBackupItem(item);

	fetchTask();
}

void QFrameSyncGroupCommitFile::doubleClicked(const QModelIndex& index)
{
	if (m_pTreeView == NULL || m_pButtonWorkspace==NULL)
	{
		return;
	}

	if (!m_mapWnd.contains(index))
	{
		QPair<FileInfo, int> file;

		if (!m_pTreeView->fetchItem(index, file))
		{
			return;
		}

		if (!QGlobalInfo::instance()->hasPrivileges(FileSynchronizationPrivilege, CommitFilePrivilege))
		{
			QMessageBox::warning(this, tr("Warning"), tr("You do not have sufficient permissions to modify the file!"),
				QMessageBox::Ok, QMessageBox::Ok);
			return;
		}

		if (!isTextFormat(file.first.name))
		{
			QMessageBox::information(this, tr("Information"), tr("It is not supported to diff binary files!"),
				QMessageBox::Ok, QMessageBox::Ok);
			return;
		}

		QString strSource;
		int nRevision=-1;
		if ((file.second == Modified || file.second == Unmodified || file.second == Added) && m_configure.contains("Revision"))
		{
			strSource=appendPath(getAppParentDir(), QString("%1%2").arg(SYNC_H9000_FILE_LOCAL_PATH).arg(m_pTreeView->record()));
			nRevision=m_configure["Revision"].toInt();
			while (nRevision >= 0)
			{
				if (QFile::exists(appendPath(strSource, QString("%1/%2").arg(nRevision).arg(file.first.name))))
				{
					strSource=appendPath(strSource, QString("%1/%2").arg(nRevision).arg(file.first.name));
					break;
				}

				nRevision--;
			}

			writeConfigure();
		}

		QString strFolder=m_pButtonWorkspace->text();
		QString strTarget=appendPath(strFolder, file.first.name);

		QString strConfigure=appendPath(getAppParentDir(), QString("%1%2.xml").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH)
			.arg(m_pTreeView->record()));

		QMainWndSyncFile* pWnd=new QMainWndSyncFile(strSource, nRevision, strTarget, strConfigure);
		pWnd->showMaximized();

		connect(pWnd, SIGNAL(closing()), this, SLOT(closing()));

		m_mapWnd.insert(index, pWnd);
	}
	else
	{
		QMainWndSyncFile* pWnd=dynamic_cast<QMainWndSyncFile*>(m_mapWnd[index]);
		if (pWnd != NULL)
		{
			pWnd->raise();
			pWnd->activateWindow();
		}
	}
}

void QFrameSyncGroupCommitFile::closing()
{
	QObject* pWnd=sender();
	QMutableMapIterator<QModelIndex, QObject*> it(m_mapWnd);
	while (it.hasNext())
	{
		it.next();

		QObject* pt=it.value();
		if (pWnd == pt)
		{
			it.remove();

			break;
		}
	}
}

void QFrameSyncGroupCommitFile::onClickedBtnRefresh()
{
	currentTextChanged();
}

void QFrameSyncGroupCommitFile::onClickedBtnWorkspace()
{
	if (!m_configure.contains("SwitchWorkspaceWarning") || m_configure["SwitchWorkspaceWarning"] == "true")
	{
		QMessageBoxEx message(QMessageBox::Warning, tr("Warning"), tr("Once the workspace is changed, the logs will be lost!"),
			QMessageBox::Ok|QMessageBox::Cancel, this);

		message.setDefaultButton(QMessageBox::Cancel);

		int ret=message.exec();

		if ((ret&CheckButton) != 0)
		{
			m_configure["SwitchWorkspaceWarning"]="false";

			writeConfigure();
		}

		if ((ret&QMessageBox::Cancel) != 0)
		{
			return;
		}
	}

	QString strFolder=m_pButtonWorkspace->text();
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Switch Workspace"), strFolder, 
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	convertToStdDir(strDir);
	if (strDir.isEmpty() || strFolder == strDir)
	{
		return;
	}

	m_configure["Workspace"]=strDir;

	writeConfigure();

	m_pButtonWorkspace->setText(strDir);
	
	QString strXmlFile=appendPath(getAppParentDir(), QString("%1%2.xml").arg(SYNC_H9000_FILE_LOCAL_PATH, m_pTreeView->record()));
	QWriteWorkspaceToXmlFile writer(strXmlFile, strDir);

	if (writeConfigure() && writer.write(false))
	{
		onClickedBtnRefresh();
	}
	else
	{
		m_pButtonWorkspace->setText(strFolder);

		QMessageBox::warning(this, tr("Warning"), tr("Failed to switch the workspace,\n because of the errors of writing to the record file!"));
	}
}

void QFrameSyncGroupCommitFile::onClickedBtnCancel()
{
	cancel();
}

void QFrameSyncGroupCommitFile::onClickedBtnSelectAll()
{
	setItemsCheckState(Qt::Checked);
}

void QFrameSyncGroupCommitFile::onClickedBtnSelectNone()
{
	setItemsCheckState(Qt::Unchecked);
}

void QFrameSyncGroupCommitFile::setItemsCheckState(int state)
{
	if (m_pTreeView == NULL)
	{
		return;
	}

	int nRowCount=m_pTreeView->rowCount();
	for (int nRow=0; nRow<nRowCount; nRow++)
	{
		if (m_pTreeView->itemCheckState(nRow) != state)
		{
			m_pTreeView->setItemCheckState(nRow, state);
		}
	}
}

void QFrameSyncGroupCommitFile::mouseRightButtonPressed(QPoint pos)
{
	if (sender() == m_pTreeView)
	{
		if (m_bSync && m_pListViewServers->hasSelected() && m_pTreeView->hasSelected())
		{
			m_pActions[ActionSync]->setEnabled(true);
		}
		else
		{
			m_pActions[ActionSync]->setEnabled(false);
		}

		if (m_pTreeView->selectedRows().isEmpty())
		{
			m_pActions[ActionDiff]->setEnabled(false);
		}
		else
		{
			m_pActions[ActionDiff]->setEnabled(true);
		}

		m_pMenu->popup(m_pTreeView->mapToGlobal(pos));
	}
	else if (sender() == m_pTreeViewQueue)
	{
		if (m_bSync && m_pTreeViewQueue->taskCount() > 0)
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
}

void QFrameSyncGroupCommitFile::appendTip(int flag, const QString& tip)
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
	case QReceiverTransferFile::Backup:
		pixmap=findPixmap(":/images/backup.png");
		break;
	default:
		break;
	}

	logplus(flag, tip);

	labelInfoIcon->setPixmap(pixmap);
	labelDesc->setText(tip);

	showTipsPixmap(!pixmap.isNull());
}

void QFrameSyncGroupCommitFile::logplus(int flag, const QString& tip)
{
	if (flag == QReceiverTransferFile::Error)
	{
		logplusError(MANAGER_LOGGER_STR, tip);
	}
	else if (flag == QReceiverTransferFile::Warning)
	{
		logplusWarning(MANAGER_LOGGER_STR, tip);
	}
	else
	{
		logplusInfo(MANAGER_LOGGER_STR, tip);
	}
}

void QFrameSyncGroupCommitFile::appendTransferTip(int flag, const QString& tip)
{
	m_transferTip=QPair<int, QString>(flag, tip);
	if (m_pTreeViewQueue->hasFocus())
	{
		appendTip(flag, tip);
	}
	else
	{
		logplus(flag, tip);
	}
}

void QFrameSyncGroupCommitFile::recvMessage(QTransferInstructor::MessageType type, const QString& message)
{
	switch (type)
	{
	case QTransferInstructor::ConnectToTargetSuccess:
		{
			m_pTreeViewQueue->setItemTransferStatus(m_index.parent().row(), QTreeViewGroupTransferQueue::Connected, m_index.parent().parent());
			appendTransferTip(QReceiverTransferFile::Information, message);
		}
		break;
	case QTransferInstructor::ConnectingToSource:
		{
			m_pTreeViewQueue->setItemTransferStatus(m_index.parent().row(), QTreeViewGroupTransferQueue::Connecting, m_index.parent().parent());
		}
	case QTransferInstructor::ConnectToSourceSuccess:
		{
			appendTransferTip(QReceiverTransferFile::Information, message);
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
			appendTransferTip(QReceiverTransferFile::Error, message);

			m_pTreeViewQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Failed, m_index.parent());

			fetchTask();
		}
		break;
	case QTransferInstructor::QueryUpload:
		{
			appendTransferTip(QReceiverTransferFile::Upload, message);

			m_pTreeViewQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Transfering, m_index.parent());
		}
		break;
	case QTransferInstructor::UploadSuccess:
	case QTransferInstructor::BackUpFinished:
	case QTransferInstructor::UploadDirSuccess:
		{
			appendTransferTip(QReceiverTransferFile::Success, message);

			m_pTreeViewQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Finished, m_index.parent());

			fetchTask();
		}
		break;
	case QTransferInstructor::UploadWarning:
		{
			appendTransferTip(QReceiverTransferFile::Warning, message);

			m_pTreeViewQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::Transfering, m_index.parent());
		}
		break;
	case QTransferInstructor::BackingUp:
		{
			appendTransferTip(QReceiverTransferFile::Backup, message);

			m_pTreeViewQueue->setItemTransferStatus(m_index.row(), QTreeViewGroupTransferQueue::BackingUp, m_index.parent());
		}
		break;
	default:
		qDebug("QFrameFileTransferGroup::recvMessage invalid message type!!!");
		break;
	}
}

void QFrameSyncGroupCommitFile::addSubTask(const QGroupTransferQueue::TransferQueueItem& task, QVector<FileInfo> files)
{
	Q_UNUSED(task);
	Q_UNUSED(files);
// 	if (files.count() <= 0)
// 	{
// 		return;
// 	}
// 
// 	QTreeViewGroupTransferQueue::TransferQueueFileItem item;
// 	item.isTop=false;
// 	item.source=QTreeViewTransferQueue::appendPathName(task.source, task.file.name, task.isTop);
// 
// 	item.format=task.format;
// 	item.target=QTreeViewTransferQueue::appendPathName(task.target, task.file.name, false);
// 	for (int i=0; i<files.size(); i++)
// 	{
// 		item.file.name=files[i].name;
// 		item.file.size=files[i].size;
// 		item.file.type=files[i].type;
// 		item.file.typeDesc=fileType(files[i]);
// 
// 		m_pTreeViewQueue->insertItem(m_index.row()+i+1, item, m_index.parent());
// 	}
}

void QFrameSyncGroupCommitFile::transferedFileContents(const QGroupTransferQueue::TransferQueueItem& task, quint64 size)
{
	if (m_task != task)
	{
		return;
	}

	m_pTreeViewQueue->addItemProcess(m_index.row(), size, m_index.parent());
}

void QFrameSyncGroupCommitFile::backupProcess(int finished, int success)
{
	Q_UNUSED(success);

	m_pTreeViewQueue->setItemProcess(m_index.row(), finished, m_index.parent());
}

void QFrameSyncGroupCommitFile::syncFinished()
{
	m_bSync=true;
	m_pActions[ActionCancel]->setEnabled(false);
	m_pActions[ActionSelectAll]->setEnabled(true);
	m_pActions[ActionSelectNone]->setEnabled(true);
	m_pActions[ActionRefresh]->setEnabled(true);
	m_pActions[ActionShowLog]->setEnabled(true);

	m_pTreeView->setItemsCheckable(true);
	m_pButtonWorkspace->setEnabled(true);
	m_pButtonRefresh->setEnabled(true);

	m_transferTip.second="";

	m_pTreeView->viewport()->update();
}

void QFrameSyncGroupCommitFile::transferFile(const QModelIndex& index, int role, const QVariant& item)
{
	if (!index.isValid())
	{
		//同步完毕
		m_pTreeView->save();

		finished();

		onClickedBtnRefresh();

		return;
	}

	m_index=index;

	if (role != QTreeViewGroupTransferQueue::BackUpRole)
	{
		m_task=qvariant_cast<QGroupTransferQueue::TransferQueueItem>(item);

		m_pTreeViewQueue->expand(m_index.parent());

		m_pTransferInstructor->addTask(m_task);
	}
	else
	{
		QTreeViewGroupTransferQueue::TransferQueueBackupItem task=qvariant_cast<QTreeViewGroupTransferQueue::TransferQueueBackupItem>(item);
		m_pTransferInstructor->addBackupTask(task);
	}
}

int QFrameSyncGroupCommitFile::findFileFormat(const QString& name, int format) const
{
	int nFormat=format;
	if (nFormat == AutoFormat)
	{
		if (isTextFormat(name))
		{
			nFormat=TextFormat;
		}
		else
		{
			nFormat=BinaryFormat;
		}
	}

	return nFormat;
}

void QFrameSyncGroupCommitFile::fetchTask()
{
	if (m_bSync)
	{
		return;
	}

	int nRole;
	QVariant var;
	int nStatus;
	QModelIndex index=m_pTreeViewQueue->fetchTask(nRole, var, nStatus);
	if (index.isValid())
	{
		if (nRole != QTreeViewGroupTransferQueue::BackUpRole && nStatus == Deleted)
		{
			m_pTreeViewQueue->setItemTransferStatus(index.row(), QTreeViewGroupTransferQueue::Finished, index.parent());

			fetchTask();
			return;
		}
	}

	transferFile(index, nRole, var);
}

void QFrameSyncGroupCommitFile::cancel()
{
	m_pTransferInstructor->queryInterruptTransferToTarget();

	finished();
}

void QFrameSyncGroupCommitFile::finished()
{
	m_pTransferInstructor->clear();

	appendTip(QReceiverTransferFile::Information, tr("Synchronization finished."));

	syncFinished();
}

void QFrameSyncGroupCommitFile::saveBackupRecord(const QString& filename, QVector<QPair<FileInfo, int> >& files)
{
	QWriteBackupToXmlFile writer(filename, m_pButtonWorkspace->text(), files);
	writer.write();
}

QWriteBackupToXmlFile::QWriteBackupToXmlFile(const QString& filename, const QString& path, const QVector<QPair<FileInfo, int> >& files)
:QAbstractWriteXmlFile(filename)
,m_strPath(path)
,m_files(files)
{

}

QWriteBackupToXmlFile::~QWriteBackupToXmlFile()
{

}

void QWriteBackupToXmlFile::save(QDomDocument& doc, QDomElement& parent)
{
	parent.setAttribute("path", m_strPath);

	for (int i=0; i<m_files.size(); i++)
	{
		int nStatus=(m_files[i].second==NonVersioned? Added:m_files[i].second);
		QFileInfo file(appendPath(m_strPath, m_files[i].first.name));
		if (!file.exists())
		{
			nStatus=Deleted;
			m_files[i].first.lastModified=QDateTime::currentDateTime();
		}

		QDomElement node=doc.createElement("file");

		node.setAttribute("path", m_files[i].first.name);
		node.setAttribute("type", m_files[i].first.type);
		node.setAttribute("size", m_files[i].first.size);
		node.setAttribute("modified", m_files[i].first.lastModified.toString(FILE_MODIFIED_DATETIME_FORMAT));
		node.setAttribute("status", nStatus);

		parent.appendChild(node);
	}
}
