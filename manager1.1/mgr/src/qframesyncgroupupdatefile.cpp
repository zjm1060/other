#include "qframesyncgroupupdatefile.h"

#include <QFileDialog>
#include <QTextStream>

#include "define.h"
#include "common.h"
#include "qutildef.h"
#include "qimessage.h"
#include "qmainwndsyncfile.h"
#include "qmessageboxex.h"
#include "qtipsdelegate.h"
#include "qglobalinfo.h"
#include "qdlginstantupdate.h"
#include "qlogger.h"
#include "qframesyncgrouptransferfile.h"
#include "qfileex.h"
#include "qdirex.h"

QFrameSyncGroupUpdateFile::QFrameSyncGroupUpdateFile(const QString& treeview, QWidget *parent/* =0 */)
	: QFrame(parent)
	,m_pListViewServers(NULL)
	,m_pButtonWorkspace(NULL)
	,m_pTreeViewFiles(NULL)
	,m_bRefreshing(false)
	,m_pMonitor(NULL)
{
	qRegisterMetaType<QList<QString> >("QList<QString>");

	setupUi(this);

	setDefaultDescText(tr("Update uploaded files to the local workspace."));

	connect(pushButtonRefresh, SIGNAL(clicked()), this, SLOT(currentTextChanged()));

	//接收端
	m_pListViewServers=new QListViewSyncFileRequestServer(this);
	verticalLayoutRequest->addWidget(m_pListViewServers);

	connect(m_pListViewServers, SIGNAL(currentChaged(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));
	connect(m_pListViewServers, SIGNAL(sendTips(int, const QString&)), this, SLOT(appendTip(int, const QString&)));
	connect(m_pListViewServers, SIGNAL(loaded(QVector<QPair<QString, QString> >)), this, SLOT(serversLoaded(QVector<QPair<QString, QString> >)));

	m_pButtonWorkspace=new QLinkButton(fileIcon(Dir), "", this);
	verticalLayoutWorkspace->addWidget(m_pButtonWorkspace);

	connect(m_pButtonWorkspace, SIGNAL(clicked()), this, SLOT(onClickedBtnWorkspace()));

	m_pButtonWorkspace->installEventFilter(this);

	m_pButtonUpdate=new QPushButton(tr("Instant Update"), this);
	horizontalLayoutRefresh->addWidget(m_pButtonUpdate);

	connect(m_pButtonUpdate, SIGNAL(clicked()), this, SLOT(onClickedBtnInstantUpdate()));

	m_pButtonSync=new QPushButton(tr("Release"));
	horizontalLayoutRefresh->addWidget(m_pButtonSync);
	m_pButtonSync->setVisible(false);

	connect(m_pButtonSync, SIGNAL(clicked()), this, SLOT(onClickedBtnSync()));

	pushButtonRefresh->installEventFilter(this);
	m_pButtonUpdate->installEventFilter(this);
	m_pButtonSync->installEventFilter(this);

	m_pTreeViewFiles=treeViewUpdateFileDynamicClass(treeview);

	if (m_pTreeViewFiles != NULL)
	{
		verticalLayoutRequestFile->addWidget(m_pTreeViewFiles);

		connect(m_pTreeViewFiles, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(doubleClicked(const QModelIndex&)));
		connect(m_pTreeViewFiles, SIGNAL(findModified(QString, QString)), m_pListViewServers, SLOT(modified(QString, QString)));
		connect(m_pTreeViewFiles, SIGNAL(refreshModelFinished()), this, SLOT(refreshFileViewFinished()));
		connect(m_pTreeViewFiles, SIGNAL(selectedChanged()), this, SLOT(selectedChanged()));
		connect(m_pTreeViewFiles, SIGNAL(mouseRightButtonPressed(QPoint)), this, SLOT(mouseRightButtonPressed(QPoint)));

		m_pTreeViewFiles->installEventFilter(this);

		m_pMonitor=new QMonitorConfigureFile(appendPath(getAppParentDir(), QString("%1%2.xml")
			.arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH, m_pTreeViewFiles->record())), this);

		connect(m_pMonitor, SIGNAL(changed(const QMap<QString, QString>&)), this, SLOT(configureChanged(const QMap<QString, QString>&)));
		m_pMonitor->load();
	}

	//弹出菜单
	m_pMenu=new QMenu(this);
	m_pActions[ActionInstantUpdate]=new QAction(tr("Instant Update"), m_pMenu);
	m_pActions[ActionManualUpdate]=new QAction(tr("Manual Update"), m_pMenu);
	m_pActions[ActionSelectOrDeselect]=new QAction(tr("&Select/Deselect"), m_pMenu);
	m_pActions[ActionSelectNone]=new QAction(tr("Select &None"), m_pMenu);
	m_pActions[ActionRefresh]=new QAction(QIcon(":/images/refresh.png"), tr("&Refresh"), m_pMenu);

	m_pMenu->addAction(m_pActions[ActionInstantUpdate]);
	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionManualUpdate]);
	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionSelectOrDeselect]);
	m_pMenu->addAction(m_pActions[ActionSelectNone]);
	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionRefresh]);
	

	//菜单触发事件
	m_pSignalMapper=new QSignalMapper(this);
	for (int i=0; i<ActionEnd; i++)
	{
		connect(m_pActions[i], SIGNAL(triggered()), m_pSignalMapper, SLOT(map()));
		m_pSignalMapper->setMapping(m_pActions[i], i);
	}

	connect(m_pSignalMapper, SIGNAL(mapped(int)), this, SLOT(actionTriggered(int)));
}

QFrameSyncGroupUpdateFile::~QFrameSyncGroupUpdateFile()
{
	m_pButtonWorkspace->removeEventFilter(this);
	pushButtonRefresh->removeEventFilter(this);
	m_pButtonUpdate->removeEventFilter(this);
	m_pButtonSync->removeEventFilter(this);
	
	if (m_pTreeViewFiles != NULL)
	{
		m_pTreeViewFiles->removeEventFilter(this);
	}

	SAFE_DELETE_CLASS(m_pMonitor);
}

void QFrameSyncGroupUpdateFile::writeConfigure()
{
	if (m_pMonitor != NULL)
	{
		m_pMonitor->save(m_configure);
	}
}

void QFrameSyncGroupUpdateFile::configureChanged(const QMap<QString, QString>& configure)
{
	m_configure=configure;

	refreshWorkspace();
}

void QFrameSyncGroupUpdateFile::setMenuState(bool refresh)
{
	m_bRefreshing=refresh;
	if (m_bRefreshing)
	{
		appendTip(QReceiverTransferFile::Information, tr("Loading, please wait..."));

		m_pListViewServers->setEnabled(false);

		m_pButtonWorkspace->setEnabled(false);
		pushButtonRefresh->setEnabled(false);
		m_pButtonUpdate->setEnabled(false);
		m_pButtonSync->setEnabled(false);
	}
	else
	{
		appendTip(QReceiverTransferFile::Information, tr("Loaded."));

		m_pListViewServers->setEnabled(true);

		pushButtonRefresh->setEnabled(true);
		m_pButtonWorkspace->setEnabled(true);

		if (m_pTreeViewFiles != NULL)
		{
			m_pButtonUpdate->setEnabled(m_pTreeViewFiles->hasSelected());
			m_pButtonSync->setEnabled(m_pTreeViewFiles->rowCount()>0);
		}
	}
}

void QFrameSyncGroupUpdateFile::mouseRightButtonPressed(QPoint pos)
{
	if (m_pTreeViewFiles == NULL || m_bRefreshing)
	{
		return;
	}

	m_pActions[ActionInstantUpdate]->setEnabled(m_pTreeViewFiles->hasSelected());

	QModelIndexList items=m_pTreeViewFiles->manuSelectItems();
	m_pActions[ActionManualUpdate]->setEnabled(!items.isEmpty());
	m_pActions[ActionSelectOrDeselect]->setEnabled(!items.isEmpty());

	m_pMenu->popup(m_pTreeViewFiles->mapToGlobal(pos));
}

void QFrameSyncGroupUpdateFile::actionTriggered(int index)
{
	if (m_pTreeViewFiles == NULL)
	{
		return;
	}

	switch (index)
	{
	case ActionInstantUpdate:
		onClickedBtnInstantUpdate();
		break;
	case ActionManualUpdate:
		{
			QModelIndexList items=m_pTreeViewFiles->manuSelectItems();
			foreach (QModelIndex item, items)
			{
				if (!item.isValid() || item.column() != 0)
				{
					continue;
				}

				doubleClicked(m_pTreeViewFiles->mapFromSourceItem(item));
			}
		}
		break;
	case ActionSelectOrDeselect:
		{
			QModelIndexList items=m_pTreeViewFiles->manuSelectItems();
			foreach (QModelIndex item, items)
			{
				if (!item.isValid() || item.column() != 0)
				{
					continue;
				}

				int nState=m_pTreeViewFiles->itemCheckState(item.row(), item.parent());

				m_pTreeViewFiles->setItemCheckState(item.row(), nState == Qt::Checked? Qt::Unchecked:Qt::Checked, item.parent());
			}
		}
		break;
	case ActionSelectNone:
		{
			int nRowCount=m_pTreeViewFiles->rowCount();
			for (int nRow=0; nRow<nRowCount; nRow++)
			{
				m_pTreeViewFiles->setItemCheckState(nRow, Qt::Unchecked);
			}
		}
		break;
	case ActionRefresh:
		currentTextChanged();
		break;
	default:
		break;
	}
}

void QFrameSyncGroupUpdateFile::setTitleText(const QString& title)
{
	labelTitle->setText(title);
}

void QFrameSyncGroupUpdateFile::setDefaultDescText(const QString& desc)
{
	m_strDefaultDesc=desc;

	appendTip(QReceiverTransferFile::Information, desc);
}

void QFrameSyncGroupUpdateFile::setThemePixmap(const QPixmap& icon)
{
	labelIcon->setPixmap(icon);
}

void QFrameSyncGroupUpdateFile::showTipsPixmap(bool show)
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

void QFrameSyncGroupUpdateFile::appendTip(int flag, const QString& tip)
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

bool QFrameSyncGroupUpdateFile::eventFilter(QObject* watched, QEvent* event)
{
	if (!m_bRefreshing)
	{
		if (event->type() == QEvent::Enter)
		{
			if (watched == m_pButtonWorkspace)
			{
				appendTip(QReceiverTransferFile::Information, tr("Click to switch your workspace."));
			}
			else if (watched == pushButtonRefresh)
			{
				appendTip(QReceiverTransferFile::Information, tr("Click to recheck the workspace."));
			}
			else if (watched == m_pButtonUpdate)
			{
				if (m_pButtonUpdate->isEnabled())
				{
					appendTip(QReceiverTransferFile::Information, tr("Click to update the selected files of the workspace."));
				}
				else
				{
					appendTip(QReceiverTransferFile::Warning, tr("After selecting a source from the host list, select files to be updated!"));
				}
			}
			else if (watched == m_pButtonSync)
			{
				if (m_pButtonSync->isEnabled())
				{
					appendTip(QReceiverTransferFile::Information, tr("Click to transfer files to other servers."));
				}
				else
				{
					appendTip(QReceiverTransferFile::Warning, tr("After selecting a source from the host list, select files to be transferred!"));
				}
			}
			else if (watched == m_pTreeViewFiles)
			{
				appendTip(QReceiverTransferFile::Information, tr("Select files to be updated, and click \"instant update\" button to update."));
			}
		}
		else if (event->type() == QEvent::Leave)
		{
			appendTip(QReceiverTransferFile::Information, "");
		}
	}

	return QFrame::eventFilter(watched, event);
}

void QFrameSyncGroupUpdateFile::selectedChanged()
{
	m_pButtonUpdate->setEnabled(m_pTreeViewFiles->hasSelected());
}

void QFrameSyncGroupUpdateFile::refreshFileViewFinished()
{
	setMenuState(false);

	modified();
}

void QFrameSyncGroupUpdateFile::serversLoaded(QVector<QPair<QString, QString> > servers)
{
	setMenuState(false);

	if (m_pTreeViewFiles == NULL || m_pButtonWorkspace == NULL)
	{
		return;
	}

	QModelIndex index=m_pListViewServers->currentItem();
	if (!index.isValid())
	{
		if (m_configure.contains("Compare_HostName") && m_configure.contains("Compare_Address"))
		{
			index=m_pListViewServers->findServer(m_configure["Compare_HostName"], m_configure["Compare_Address"]);
			m_pListViewServers->clicked(index);
		}
	}

	clicked(index);

	for (int i=0; i<servers.size(); i++)
	{
		const QPair<QString, QString>& server=servers[i];
		m_pTreeViewFiles->checkModified(m_pButtonWorkspace->text(), server.first, server.second);
	}
}

void QFrameSyncGroupUpdateFile::refreshWorkspace()
{
	if (m_pTreeViewFiles == NULL)
	{
		return;
	}

	setMenuState(true);

	readWorkspace();
}

void QFrameSyncGroupUpdateFile::readWorkspace()
{
	if (m_pTreeViewFiles == NULL)
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
		strFolder=appendPath(getAppParentDir(), m_pTreeViewFiles->commonFolder());
		m_configure["Workspace"]=strFolder;

		writeConfigure();
	}

	m_pButtonWorkspace->setText(strFolder);

	currentTextChanged();
}

void QFrameSyncGroupUpdateFile::currentTextChanged()
{
//	m_pTreeViewFiles->save();

	if (m_pListViewServers != NULL && m_pTreeViewFiles != NULL)
	{
		m_pTreeViewFiles->clear();

		setMenuState(true);

		m_pListViewServers->loadServers(m_pTreeViewFiles->record());
	}
}

void QFrameSyncGroupUpdateFile::clicked(const QModelIndex& index)
{
	m_mapWnd.clear();

	if (!index.isValid())
	{
		return;
	}

	setMenuState(true);

	QPair<QString, QString> peer=m_pListViewServers->server(index);

	if (m_configure["Compare_HostName"] != peer.first || m_configure["Compare_Address"] != peer.second)
	{
		m_configure["Compare_HostName"]=peer.first;
		m_configure["Compare_Address"]=peer.second;

		writeConfigure();
	}

	m_pTreeViewFiles->setWorkspace(m_pButtonWorkspace->text(), peer.first, peer.second);
}

void QFrameSyncGroupUpdateFile::doubleClicked(const QModelIndex& index)
{
	if (m_pTreeViewFiles == NULL || m_pListViewServers == NULL)
	{
		return;
	}
	
	if (!m_mapWnd.contains(index))
	{
		QPair<DiffFile, FileInfo> peer;

		if (!m_pTreeViewFiles->fetchItem(index, peer))
		{
			return;
		}

		if (!QGlobalInfo::instance()->hasPrivileges(FileSynchronizationPrivilege, UpdateFilePrivilege))
		{
			QMessageBox::warning(this, tr("Warning"), tr("You do not have sufficient permissions to update files!"),
				QMessageBox::Ok, QMessageBox::Ok);
			return;
		}

		QPair<QString, QString> server=m_pListViewServers->server(m_pListViewServers->currentItem());
		if (server.second.isEmpty())
		{
			return;
		}

		if (peer.first.status == Delete)
		{
			QString strFolder=m_pButtonWorkspace->text();
			QString strTarget=appendPath(strFolder, peer.second.name);

			QMessageBox::StandardButton btn=QMessageBox::warning(this, tr("Warning"), QString(tr("To delete the file \"%1\",\nAre you sure?")).arg(strTarget), 
				QMessageBox::Cancel|QMessageBox::Ok, QMessageBox::Cancel);
			if (btn == QMessageBox::Ok)
			{
				if (isDir(peer.second.type))
				{
					QDirEx dir(strTarget);
					if (dir.exists())
					{
						if (!deleteDir(strTarget))
						{
							QMessageBox::critical(this, tr("Error"), QString(tr("Failed to delete the folder \"%1\"!")).arg(strTarget), 
								QMessageBox::Ok, QMessageBox::Ok);

							return;
						}
					}
				}
				else if (isFile(peer.second.type))
				{
					if (QFileEx::exists(strTarget))
					{
						if (!QFileEx::remove(strTarget))
						{
							QMessageBox::critical(this, tr("Error"), QString(tr("Failed to delete the file \"%1\"!")).arg(strTarget), 
								QMessageBox::Ok, QMessageBox::Ok);

							return;
						}
					}
				}

				m_pTreeViewFiles->itemModified(index);
			}
		}
		else if (peer.first.status == Add)
		{
			if (peer.first.version == -1)
			{
				return;
			}

			QString strSource=appendPath(getAppParentDir(), QString("%1%2/%3/%4").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH)
				.arg(server.first.isEmpty()? server.second:server.second+"@"+server.first).arg(m_pTreeViewFiles->record()).arg(peer.first.path));

			QString strFolder=m_pButtonWorkspace->text();
			QString strTarget=appendPath(strFolder, peer.second.name);
			if (strTarget.isEmpty())
			{
				return;
			}

			QMessageBox::StandardButton btn=QMessageBox::warning(this, tr("Warning"), QString(tr("To add the file \"%1\",\nAre you sure?")).arg(strTarget), 
				QMessageBox::Cancel|QMessageBox::Ok, QMessageBox::Cancel);
			if (btn == QMessageBox::Ok)
			{
				if (isDir(peer.second.type))
				{
					QDirEx dir;
					if (dir.mkpath(strTarget))
					{
						QMessageBox::critical(this, tr("Error"), QString(tr("Failed to create the folder \"%1\"!")).arg(strTarget), 
							QMessageBox::Ok, QMessageBox::Ok);

						return;
					}
				}
				else if (isFile(peer.second.type))
				{
					if (!QFileEx::copy(strSource, strTarget))
					{
						QMessageBox::critical(this, tr("Error"), QString(tr("Failed to copy the file \"%1\" to \"%2\"!")).arg(strSource).arg(strTarget), 
							QMessageBox::Ok, QMessageBox::Ok);

						return;
					}
				}

				m_pTreeViewFiles->itemModified(index);
			}
		}
		else
		{
			if (!isFile(peer.second.type))
			{
				return;
			}

			QString strSource;
			if (peer.first.version > -1)
			{
				strSource=appendPath(getAppParentDir(), QString("%1%2/%3/%4").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH)
					.arg(server.first.isEmpty()? server.second:server.second+"@"+server.first).arg(m_pTreeViewFiles->record()).arg(peer.first.path));
			}

			QString strFolder=m_pButtonWorkspace->text();
			QString strTarget=appendPath(strFolder, peer.second.name);
			if (strTarget.isEmpty())
			{
				return;
			}

			if ((!strSource.isEmpty() && !isTextFormat(strSource)) || !isTextFormat(strTarget))
			{
				QMessageBox::information(this, tr("Information"), tr("It is not supported to diff binary files!"),
					QMessageBox::Ok, QMessageBox::Ok);

				return;
			}

			QString strConfigure=appendPath(getAppParentDir(), QString("%1%2.xml").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH)
				.arg(m_pTreeViewFiles->record()));

			QMainWndSyncFile* pWnd=new QMainWndSyncFile(strSource, peer.first.version, strTarget, strConfigure, true);

			connect(pWnd, SIGNAL(modified()), this, SLOT(modified()));
			connect(pWnd, SIGNAL(closing()), this, SLOT(closing()));

			pWnd->showMaximized();

			m_mapWnd.insert(index, pWnd);
		}
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

void QFrameSyncGroupUpdateFile::modified()
{
	QObject* pWnd=sender();
	QMapIterator<QModelIndex, QObject*> it(m_mapWnd);
	while (it.hasNext())
	{
		it.next();

		QObject* pt=it.value();
		if (pWnd == pt)
		{
			m_pTreeViewFiles->itemModified(it.key());

			break;
		}
	}

	m_pListViewServers->setServerItemModifiedStatus(m_pListViewServers->currentIndex(), m_pTreeViewFiles->hasModifiedItem());
}

void QFrameSyncGroupUpdateFile::closing()
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

void QFrameSyncGroupUpdateFile::onClickedBtnWorkspace()
{
	if (!m_configure.contains("SwitchWorkspaceWarning") || m_configure["SwitchWorkspaceWarning"] == "true")
	{
		QMessageBoxEx message(QMessageBox::Warning, tr("Warning"), tr("Once the workspace is changed, the records will be lost!"),
			QMessageBox::Ok|QMessageBox::Cancel, this);

		message.setDefaultButton(QMessageBox::Ok);

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
	if (strDir.isEmpty() || strFolder == strDir)
	{
		return;
	}

	m_pButtonWorkspace->setText(strDir);
	
	m_configure["Workspace"]=strDir;
	writeConfigure();

	currentTextChanged();
}

void QFrameSyncGroupUpdateFile::onClickedBtnInstantUpdate()
{
	if (m_pTreeViewFiles == NULL || m_pListViewServers == NULL || m_pButtonWorkspace == NULL)
	{
		return;
	}

	QPair<QString, QString> server=m_pListViewServers->server(m_pListViewServers->currentItem());
	if (server.second.isEmpty())
	{
		return;
	}

	QVector<QPair<DiffFile, FileInfo> >  files=m_pTreeViewFiles->selectedItems();
	if (files.isEmpty())
	{
		return;
	}

	QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"),
		tr("Are you sure to update?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	if (button == QMessageBox::No)
	{
		return;
	}

	logplusInfo(MANAGER_LOGGER_STR, "Immediately update...");

	QString strSource=appendPath(getAppParentDir(), QString("%1%2/%3/").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH)
		.arg(server.first.isEmpty()? server.second:server.second+"@"+server.first).arg(m_pTreeViewFiles->record()));
	QString strTarget=m_pButtonWorkspace->text();
	QString strConfigure=appendPath(getAppParentDir(), QString("%1%2.xml").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH)
		.arg(m_pTreeViewFiles->record()));

	QDlgInstantUpdate dlg(this);
	dlg.setModal(true);
	dlg.setRecordFile(strTarget, server.first, server.second);
	dlg.setTasks(strTarget, strSource, strConfigure, files);

	dlg.exec();

	logplusInfo(MANAGER_LOGGER_STR, "Update completed.");

	currentTextChanged();
}

void QFrameSyncGroupUpdateFile::onClickedBtnSync()
{
	QTreeModel* pModel=NULL;
	if (m_pTreeViewFiles != NULL)
	{
		pModel=m_pTreeViewFiles->copy();
	}

	emit sync(m_pButtonWorkspace->text(), pModel);
}

QSyncFileGroupReadCommonFolderRunnable::QSyncFileGroupReadCommonFolderRunnable(const QString& file,QObject* parent/* =0 */)
:QObject(parent)
,m_strFile(file)
{

}

QSyncFileGroupReadCommonFolderRunnable::~QSyncFileGroupReadCommonFolderRunnable()
{

}

void QSyncFileGroupReadCommonFolderRunnable::run()
{
	QList<QString>	folders;

	QReadWorkspaceFromXmlFile reader(m_strFile);
	if (reader.read())
	{
		folders=reader.result();
	}

	emit finished(folders);
}
