#include "qframesyncgroupreleasefile.h"

#include <QFileDialog>
#include <QTextStream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>

#include "define.h"
#include "common.h"
#include "qutildef.h"
#include "qimessage.h"
#include "qmessageboxex.h"
#include "qtipsdelegate.h"
#include "qglobalinfo.h"
#include "qlogger.h"
#include "qfileex.h"
#include "qdirex.h"

QFrameSyncGroupReleaseFile::QFrameSyncGroupReleaseFile(const QString& treeview, bool enableRestrict, QWidget *parent/* =0 */)
	: QFrame(parent)
	,m_pButtonWorkspace(NULL)
	,m_pTreeViewFiles(NULL)
	,m_bRefreshing(false)
	,m_pMonitor(NULL)
{
	setFrameShape(QFrame::StyledPanel);
	setFrameShadow(QFrame::Plain);

	QVBoxLayout* pLayout=new QVBoxLayout();
	setLayout(pLayout);

	QHBoxLayout* pLayoutWorkspace=new QHBoxLayout();
	pLayout->addLayout(pLayoutWorkspace);

	m_pButtonWorkspace=new QLinkButton(fileIcon(Dir), "", this);
//	m_pButtonWorkspace->setReadOnly(true);
	pLayoutWorkspace->addWidget(m_pButtonWorkspace);

	connect(m_pButtonWorkspace, SIGNAL(clicked()), this, SLOT(onClickedBtnWorkspace()));

	m_pButtonWorkspace->installEventFilter(this);

	pLayoutWorkspace->addStretch();

	m_pButtonRefresh=new QPushButton(QIcon(":/images/refresh.png"), "", this);
	pLayoutWorkspace->addWidget(m_pButtonRefresh);

	connect(m_pButtonRefresh, SIGNAL(clicked()), this, SLOT(currentTextChanged()));

	m_pButtonRefresh->installEventFilter(this);

	m_pTreeViewFiles=treeViewUpdateFileDynamicClass(treeview);

	if (m_pTreeViewFiles != NULL)
	{
		pLayout->addWidget(m_pTreeViewFiles);

		connect(m_pTreeViewFiles, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(doubleClicked(const QModelIndex&)));
		connect(m_pTreeViewFiles, SIGNAL(refreshModelFinished()), this, SLOT(refreshFileViewFinished()));
		connect(m_pTreeViewFiles, SIGNAL(selectedChanged()), this, SIGNAL(selectedChanged()));
		connect(m_pTreeViewFiles, SIGNAL(mouseRightButtonPressed(QPoint)), this, SLOT(mouseRightButtonPressed(QPoint)));

		setRestrictFilterEnabled(enableRestrict);

		m_pTreeViewFiles->installEventFilter(this);

		m_pMonitor=new QMonitorConfigureFile(appendPath(getAppParentDir(), QString("%1%2.xml")
			.arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH, m_pTreeViewFiles->record())), this);

		connect(m_pMonitor, SIGNAL(changed(const QMap<QString, QString>&)), this, SLOT(configureChanged(const QMap<QString, QString>&)));
		m_pMonitor->load();
	}

	//弹出菜单
	m_pMenu=new QMenu(this);
	m_pActions[ActionSelectOrDeselect]=new QAction(tr("&Select/Deselect"), m_pMenu);
	m_pActions[ActionSelectAll]=new QAction(tr("Select &All"), m_pMenu);
	m_pActions[ActionSelectNone]=new QAction(tr("Select &None"), m_pMenu);
	m_pActions[ActionRefresh]=new QAction(QIcon(":/images/refresh.png"), tr("&Refresh"), m_pMenu);
	
	m_pMenu->addAction(m_pActions[ActionSelectOrDeselect]);
	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionSelectAll]);
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

QFrameSyncGroupReleaseFile::~QFrameSyncGroupReleaseFile()
{
	m_pButtonWorkspace->removeEventFilter(this);
	m_pButtonRefresh->removeEventFilter(this);
	
	if (m_pTreeViewFiles != NULL)
	{
		m_pTreeViewFiles->removeEventFilter(this);
	}

	SAFE_DELETE_CLASS(m_pMonitor);
}

void QFrameSyncGroupReleaseFile::writeConfigure()
{
	if (m_pMonitor != NULL)
	{
		m_pMonitor->save(m_configure);
	}
}

void QFrameSyncGroupReleaseFile::configureChanged(const QMap<QString, QString>& configure)
{
	m_configure=configure;

	refreshWorkspace();
}

bool QFrameSyncGroupReleaseFile::isLoaded() const
{
	return !m_bRefreshing;
}

void QFrameSyncGroupReleaseFile::setRestrictFilterEnabled(bool enable)
{
	if (m_pTreeViewFiles != NULL)
	{
		m_pTreeViewFiles->setRestrictFilterEnabled(enable);
	}
}

void QFrameSyncGroupReleaseFile::setLocalLastModifiedEnabled(bool enable)
{
	if (m_pTreeViewFiles != NULL)
	{
		m_pTreeViewFiles->setLocalLastModifiedEnabled(enable);
	}
}

void QFrameSyncGroupReleaseFile::setMenuState(bool refresh)
{
	m_bRefreshing=refresh;
	if (m_bRefreshing)
	{
		emit loaded(false);

		m_pButtonWorkspace->setEnabled(false);
		m_pButtonRefresh->setEnabled(false);
	}
	else
	{
		emit loaded(true);

		m_pButtonRefresh->setEnabled(true);
		m_pButtonWorkspace->setEnabled(true);
	}
}

bool QFrameSyncGroupReleaseFile::eventFilter(QObject* watched, QEvent* event)
{
	if (!m_bRefreshing)
	{
		if (event->type() == QEvent::Enter)
		{
			if (watched == m_pButtonWorkspace)
			{
				emit sendTip(QReceiverTransferFile::Information, tr("Click to switch your workspace."));
			}
			else if (watched == m_pButtonRefresh)
			{
				emit sendTip(QReceiverTransferFile::Information, tr("Click to recheck the workspace."));
			}
			else if (watched == m_pTreeViewFiles)
			{
				emit sendTip(QReceiverTransferFile::Information, tr("Select files to be released, and click \"Release\" button to release."));
			}
		}
		else if (event->type() == QEvent::Leave)
		{
			emit sendTip(QReceiverTransferFile::Information, "");
		}
	}

	return QFrame::eventFilter(watched, event);
}

void QFrameSyncGroupReleaseFile::refreshFileViewFinished()
{
	if (m_pTreeViewFiles != NULL)
	{
		m_pTreeViewFiles->checkItems(Latest);

		m_pTreeViewFiles->hideColumn(QTreeViewUpdateFile::StatusTitle);
	}

	setMenuState(false);
}

bool QFrameSyncGroupReleaseFile::hasSelected() const
{
	if (m_pTreeViewFiles != NULL)
	{
		return m_pTreeViewFiles->hasSelected();
	}

	return false;
}

QVector<QPair<DiffFile, FileInfo> > QFrameSyncGroupReleaseFile::selectedItems()
{
	if (m_pTreeViewFiles != NULL)
	{
		return m_pTreeViewFiles->selectedItems();
	}

	return QVector<QPair<DiffFile, FileInfo> >();
}

QString QFrameSyncGroupReleaseFile::workspace() const
{
	return m_pButtonWorkspace->text();
}

QString QFrameSyncGroupReleaseFile::record() const
{
	return m_pTreeViewFiles->record();
}

void QFrameSyncGroupReleaseFile::mouseRightButtonPressed(QPoint pos)
{
	if (m_pTreeViewFiles == NULL || !isLoaded())
	{
		return;
	}

	m_pMenu->popup(m_pTreeViewFiles->mapToGlobal(pos));
}

void QFrameSyncGroupReleaseFile::actionTriggered(int index)
{
	if (m_pTreeViewFiles == NULL)
	{
		return;
	}

	switch (index)
	{
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

			emit selectedChanged();
		}
		break;
	case ActionSelectAll:
		{
			int nRowCount=m_pTreeViewFiles->rowCount();
			for (int nRow=0; nRow<nRowCount; nRow++)
			{
				m_pTreeViewFiles->setItemCheckState(nRow, Qt::Checked);
			}

			emit selectedChanged();
		}
		break;
	case ActionSelectNone:
		{
			int nRowCount=m_pTreeViewFiles->rowCount();
			for (int nRow=0; nRow<nRowCount; nRow++)
			{
				m_pTreeViewFiles->setItemCheckState(nRow, Qt::Unchecked);
			}

			emit selectedChanged();
		}
		break;
	case ActionRefresh:
		currentTextChanged();
		break;
	default:
		break;
	}
}

void QFrameSyncGroupReleaseFile::refreshWorkspace()
{
	if (m_pTreeViewFiles == NULL)
	{
		return;
	}

	setMenuState(true);

	readWorkspace();
}

void QFrameSyncGroupReleaseFile::readWorkspace()
{
	if (m_pTreeViewFiles == NULL)
	{
		return;
	}

	QString strFolder=appendPath(getAppParentDir(), m_pTreeViewFiles->commonFolder());
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

void QFrameSyncGroupReleaseFile::currentTextChanged()
{
	if (m_pTreeViewFiles != NULL)
	{
		m_pTreeViewFiles->clear();
		
		setMenuState(true);

		QString strHostName, strAddress;
		if (m_configure.contains("Compare_HostName") && m_configure.contains("Compare_Address"))
		{
			strHostName=m_configure["Compare_HostName"];
			strAddress=m_configure["Compare_Address"];
		}

		m_pTreeViewFiles->setWorkspace(m_pButtonWorkspace->text(), strHostName, strAddress);
	}
}

void QFrameSyncGroupReleaseFile::onClickedBtnWorkspace()
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

void QFrameSyncGroupReleaseFile::doubleClicked(const QModelIndex& index)
{
// 	if (m_pTreeViewFiles == NULL || m_pListViewServers == NULL)
// 	{
// 		return;
// 	}
// 	
// 	if (!m_mapWnd.contains(index))
// 	{
// 		QPair<DiffFile, FileInfo> peer;
// 
// 		if (!m_pTreeViewFiles->fetchItem(index, peer))
// 		{
// 			return;
// 		}
// 
// 		if (!QGlobalInfo::instance()->hasPrivileges(FileSynchronizationPrivilege, UpdateFilePrivilege))
// 		{
// 			QMessageBox::warning(this, tr("Warning"), tr("You do not have sufficient permissions to update files!"),
// 				QMessageBox::Ok, QMessageBox::Ok);
// 			return;
// 		}
// 
// 		QPair<QString, QString> server=m_pListViewServers->server(m_pListViewServers->currentIndex());
// 		if (server.second.isEmpty())
// 		{
// 			return;
// 		}
// 
// 		if (peer.first.status == Delete)
// 		{
// 			QString strFolder=m_pButtonWorkspace->text();
// 			QString strTarget=appendPath(strFolder, peer.second.name);
// 
// 			QMessageBox::StandardButton btn=QMessageBox::warning(this, tr("Warning"), QString(tr("To delete the file \"%1\",\nAre you sure?")).arg(strTarget), 
// 				QMessageBox::Cancel|QMessageBox::Ok, QMessageBox::Cancel);
// 			if (btn == QMessageBox::Ok)
// 			{
// 				if (isDir(peer.second.type))
// 				{
// 					QDirEx dir(strTarget);
// 					if (dir.exists())
// 					{
// 						if (!deleteDir(strTarget))
// 						{
// 							QMessageBox::critical(this, tr("Error"), QString(tr("Failed to delete the folder \"%1\"!")).arg(strTarget), 
// 								QMessageBox::Ok, QMessageBox::Ok);
// 
// 							return;
// 						}
// 					}
// 				}
// 				else if (isFile(peer.second.type))
// 				{
// 					if (QFileEx::exists(strTarget))
// 					{
// 						if (!QFileEx::remove(strTarget))
// 						{
// 							QMessageBox::critical(this, tr("Error"), QString(tr("Failed to delete the file \"%1\"!")).arg(strTarget), 
// 								QMessageBox::Ok, QMessageBox::Ok);
// 
// 							return;
// 						}
// 					}
// 				}
// 
// 				m_pTreeViewFiles->itemModified(index);
// 			}
// 		}
// 		else if (peer.first.status == Add)
// 		{
// 			if (peer.first.version == -1)
// 			{
// 				return;
// 			}
// 
// 			QString strSource=appendPath(getAppParentDir(), QString("%1%2/%3/%4").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH)
// 				.arg(server.first.isEmpty()? server.second:server.second+"@"+server.first).arg(m_pTreeViewFiles->record()).arg(peer.first.path));
// 
// 			QString strFolder=m_pButtonWorkspace->text();
// 			QString strTarget=appendPath(strFolder, peer.second.name);
// 			if (strTarget.isEmpty())
// 			{
// 				return;
// 			}
// 
// 			if (isDir(peer.second.type))
// 			{
// 				QDirEx dir;
// 				if (dir.mkpath(strTarget))
// 				{
// 					QMessageBox::critical(this, tr("Error"), QString(tr("Failed to create the folder \"%1\"!")).arg(strTarget), 
// 						QMessageBox::Ok, QMessageBox::Ok);
// 
// 					return;
// 				}
// 			}
// 			else if (isFile(peer.second.type))
// 			{
// 				if (!QFileEx::copy(strSource, strTarget))
// 				{
// 					QMessageBox::critical(this, tr("Error"), QString(tr("Failed to copy the file \"%1\" to \"%2\"!")).arg(strSource).arg(strTarget), 
// 						QMessageBox::Ok, QMessageBox::Ok);
// 
// 					return;
// 				}
// 			}
// 
// 			m_pTreeViewFiles->itemModified(index);
// 		}
// 		else
// 		{
// 			if (!isFile(peer.second.type))
// 			{
// 				return;
// 			}
// 
// 			QString strSource;
// 			if (peer.first.version > -1)
// 			{
// 				strSource=appendPath(getAppParentDir(), QString("%1%2/%3/%4").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH)
// 					.arg(server.first.isEmpty()? server.second:server.second+"@"+server.first).arg(m_pTreeViewFiles->record()).arg(peer.first.path));
// 			}
// 
// 			QString strFolder=m_pButtonWorkspace->text();
// 			QString strTarget=appendPath(strFolder, peer.second.name);
// 			if (strTarget.isEmpty())
// 			{
// 				return;
// 			}
// 
// 			if ((!strSource.isEmpty() && !isTextFormat(strSource)) || !isTextFormat(strTarget))
// 			{
// 				QMessageBox::information(this, tr("Information"), tr("It is not supported to diff binary files!"),
// 					QMessageBox::Ok, QMessageBox::Ok);
// 
// 				return;
// 			}
// 
// 			QString strConfigure=appendPath(getAppParentDir(), QString("%1%2.xml").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH)
// 				.arg(m_pTreeViewFiles->record()));
// 
// 			QMainWndSyncFile* pWnd=new QMainWndSyncFile(strSource, peer.first.version, strTarget, strConfigure, true);
// 
// 			connect(pWnd, SIGNAL(modified()), this, SLOT(modified()));
// 			connect(pWnd, SIGNAL(closing()), this, SLOT(closing()));
// 
// 			pWnd->showMaximized();
// 
// 			m_mapWnd.insert(index, pWnd);
// 		}
// 	}
// 	else
// 	{
// 		QMainWndSyncFile* pWnd=dynamic_cast<QMainWndSyncFile*>(m_mapWnd[index]);
// 		if (pWnd != NULL)
// 		{
// 			pWnd->raise();
// 			pWnd->activateWindow();
// 		}
// 	}
}

