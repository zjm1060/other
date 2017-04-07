#include "qframeremotefilesystem.h"

#include <QHBoxLayout>
#include <QFileDialog>
#include <QMouseEvent>
#include <QMessageBox>
#include <QLineEdit>

#include "define.h"
#include "common.h"
#include "qmessageparser.h"
#include "qfilecontroller.h"
#include "qreceiverparser.h"

QFrameRemoteFileSystem::QFrameRemoteFileSystem(const QString& initPath, const QString& subPath, QWidget *parent)
	: QFrameFileSystem(parent)
	,m_pClient(NULL)
	,m_bQueryComputer(false)
	,m_strInitPath(initPath)
	,m_strSubPath(subPath)
	,m_filters(QDir::NoFilter)
{
	QSharedControllerPtr pController(new QFileController);
	m_pClientManager=new QClientManager(pController, this);
	connect(m_pClientManager, SIGNAL(connected(QClient*)), this, SLOT(connected(QClient*)));

	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	connect(this, SIGNAL(clickedToolButton(int)), this, SLOT(clickedToolButton(int)));
	connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(locationCurrentIndexChanged(QString)));

	connect(comboBoxFileSystem(), SIGNAL(editTextChanged(const QString&)), this, SLOT(editTextChangedLocation(const QString&)));
	connect(comboBoxFileSystem(), SIGNAL(enterPressed(QString)), this, SLOT(enterPressed(QString)));
	connect(comboBoxFileSystem(), SIGNAL(focusStatusChanged(bool)), this, SIGNAL(focusStatusChanged(bool)));

	connect(treeViewFileSystem(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClickedLocation(const QModelIndex&)));
	connect(treeViewFileSystem(), SIGNAL(mouseRightButtonPress(QPoint)), this, SLOT(mouseRightButtonPressed(QPoint)));
	connect(treeViewFileSystem(), SIGNAL(dataChanged(QModelIndex, QString, QString)), this, SLOT(dataChanged(QModelIndex, QString, QString)));
}

QFrameRemoteFileSystem::~QFrameRemoteFileSystem()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);

	m_pClientManager->removeAll();
	SAFE_DELETE_CLASS(m_pClientManager);
}

void QFrameRemoteFileSystem::setClient(const ServerInfo& info)
{
	if (m_pClient != NULL)
	{
		m_pClientManager->remove(m_pClient);
		m_pClient=NULL;
	}

	clear();

	QClient* pClient=m_pClientManager->addClient(info);
	m_pClientManager->connectToHost(pClient);
}

void QFrameRemoteFileSystem::setFilters(const QStringList& nameFilters, QDir::Filters filters)
{
	m_nameFilters=nameFilters;
	m_filters=filters;
}

QVector<TransferFileItem> QFrameRemoteFileSystem::selectedFileItems()
{
	QVector<TransferFileItem> items;

	QTreeViewFileSystem* pFileSystem=treeViewFileSystem();
	if (pFileSystem == NULL)
	{
		return items;
	}

	QModelIndexList rows=pFileSystem->selectedRows();;
	if (rows.size() <= 0)
	{
		return items;
	}

	QString strName;
	TransferFileItem item;
	foreach (QModelIndex index, rows)
	{
		strName=pFileSystem->filename(index);
		if (strName == "..")
		{
			continue;
		}

		item.size=pFileSystem->size(index);
		item.type=pFileSystem->type(index);
		item.typeDesc=pFileSystem->typeDesc(index);
		item.name=strName;
		
		items.append(item);
	}

	return items;
}

QVector<FileInfo> QFrameRemoteFileSystem::selectedItems()
{
	QVector<FileInfo> files;

	QTreeViewFileSystem* pFileSystem=treeViewFileSystem();
	if (pFileSystem == NULL)
	{
		return files;
	}

	QModelIndexList rows=pFileSystem->selectedRows();;
	if (rows.size() <= 0)
	{
		return files;
	}

	FileInfo file;
	foreach (QModelIndex index, rows)
	{
		file=pFileSystem->fileInfo(index);
		if (file.name == "..")
		{
			continue;
		}

		files.append(file);
	}

	return files;
}

void QFrameRemoteFileSystem::connected(QClient* client)
{
	m_pClient=client;
	if (m_pClient != NULL)
	{
		deepRefresh();
	}
}

void QFrameRemoteFileSystem::deepRefresh()
{
	clear();

	queryDrives();
}

void QFrameRemoteFileSystem::gotoHome()
{
	deepRefresh();
}

void QFrameRemoteFileSystem::gotoLocation(const QString& location)
{
	int itemType=comboBoxFileSystem()->topItemType(location);
	switch (itemType)
	{
	case QComboBoxFileSystem::OtherItem:
		{
			queryPath(location);
		}
		break;
	case QComboBoxFileSystem::Computer:
		{
			m_bQueryComputer=true;
			queryDrives();
		}
		break;
	default:
		break;
	}
}

QString QFrameRemoteFileSystem::locationText()
{
	return comboBoxFileSystem()->lineEdit()->text();
}

QString QFrameRemoteFileSystem::currentLocationText()
{
	return comboBoxFileSystem()->currentLocation();
}

bool QFrameRemoteFileSystem::canUpload()
{
	QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QModelIndexList rows=pTreeViewFileSystem->selectedRows();

		QString strLocation=comboBoxFileSystem()->currentLocation();

		if (!rows.isEmpty())
		{
			if (comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
			{
				if (rows.size() != 1)
				{
					return true;
				}
				else
				{
					QString strName=pTreeViewFileSystem->filename(rows[0]);
					if (strName != "..")
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool QFrameRemoteFileSystem::canDelete()
{
	QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QModelIndexList rows=pTreeViewFileSystem->selectedRows();

		QString strLocation=comboBoxFileSystem()->currentLocation();

		if (!rows.isEmpty())
		{
			if (comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
			{
				if (rows.size() != 1)
				{
					return true;
				}
				else
				{
					QString strName=pTreeViewFileSystem->filename(rows[0]);
					if (strName != "..")
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool QFrameRemoteFileSystem::canOpen()
{
	QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QModelIndexList rows=pTreeViewFileSystem->selectedRows();

		if (rows.size() == 1)
		{
			int nType=pTreeViewFileSystem->type(rows[0]);
			if ((nType == Drive) || (nType & Dir) != 0 || (nType & HideDir) != 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool QFrameRemoteFileSystem::canRename()
{
	QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QModelIndexList rows=pTreeViewFileSystem->selectedRows();
		QString strLocation=comboBoxFileSystem()->currentLocation();

		if (rows.size() == 1)
		{
			if (comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
			{
				QString strName=pTreeViewFileSystem->filename(rows[0]);
				if (strName != "..")
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool QFrameRemoteFileSystem::canNewDir()
{
	QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QString strLocation=comboBoxFileSystem()->currentLocation();

		if (comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
		{
			return true;
		}
	}

	return false;
}

void QFrameRemoteFileSystem::queryRoot()
{
	if (m_pClient == NULL)
	{
		emit unconnected();
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryRoot, m_nUsrID);

		bool bRtn=m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
		if (!bRtn)
		{
			emit serverQuit();
		}
	}
}

void QFrameRemoteFileSystem::queryDrives()
{
	if (m_pClient == NULL)
	{
		emit unconnected();
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryDrive, m_nUsrID);
		bool bRtn=m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
		if (!bRtn)
		{
			emit serverQuit();
		}
	}
}

void QFrameRemoteFileSystem::queryPath(const QString& path)
{
	if (m_pClient == NULL)
	{
		emit unconnected();
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		int nSortColumn=treeViewFileSystem()->sortColumn();
		int nSort=0;
		switch (nSortColumn)
		{
		case 0:
			nSort=SortName;
			break;
		case 1:
			nSort=SortSize;
			break;
		case 2:
			nSort=SortType;
			break;
		case 3:
			nSort=SortTime;
			break;
		default:
			break;
		}
		stream<<path<<nSort<<m_nameFilters<<m_filters;

		buffer=pMsg->query(QueryPath, m_nUsrID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
		if (!bRtn)
		{
			emit serverQuit();
		}
	}
}

void QFrameRemoteFileSystem::queryRename(const QString& location, const QString& old, const QString& now)
{
	if (m_pClient == NULL)
	{
		emit unconnected();
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

		stream<<location<<old<<now;

		buffer=pMsg->query(QueryRename, m_nUsrID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
		if (!bRtn)
		{
			emit serverQuit();
		}
	}
}

void QFrameRemoteFileSystem::queryNewDirectory(const QString& location)
{
	if (m_pClient == NULL)
	{
		emit unconnected();
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

		stream<<location;

		buffer=pMsg->query(QueryNewDirectory, m_nUsrID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
		if (!bRtn)
		{
			emit serverQuit();
		}
	}
}

void QFrameRemoteFileSystem::queryDelete(const QString& location, const QVector<QString>& files)
{
	if (m_pClient == NULL)
	{
		emit unconnected();
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

		stream<<location<<files;

		buffer=pMsg->query(QueryDelete, m_nUsrID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
		if (!bRtn)
		{
			emit serverQuit();
		}
	}
}

void QFrameRemoteFileSystem::recvRoot(const QString& root)
{
	m_strRoot=root;

	setLocation(m_strRoot, false);

	if (m_strInitPath.isEmpty())
	{
		gotoLocation(appendPath(root, m_strSubPath));
	}
	else
	{
		gotoLocation(m_strInitPath);
	}
}


void QFrameRemoteFileSystem::recvDrives(const QVector<FileInfo>& drives)
{
	if (m_bQueryComputer)
	{
		m_bQueryComputer=false;

		setFileInfo(drives, QComboBoxFileSystem::Computer);
		QString strLocation=comboBoxFileSystem()->itemText(QComboBoxFileSystem::Computer);
		setLocation(strLocation, false);
	}
	else
	{
		setDrives(drives);

		queryRoot();
	}
}

void QFrameRemoteFileSystem::recvPath(const QString& path, int flag, const QVector<FileInfo>& files)
{
	if (flag == QueryPathValid)
	{
		if (!m_strInitPath.isEmpty())
		{
			m_strInitPath.clear();
		}

		setLocation(path, false);
		setFileInfo(files);

		comboBoxFileSystem()->setFocus();
	}
	else if (flag == QueryPathNoExist)
	{
		emit dirNoExist(path);

		if (m_strInitPath.isEmpty())
		{
			QString strLocation=location();

			if (strLocation != path)
			{
				queryPath(strLocation);
			}
			else
			{
				if (strLocation != m_strRoot)
				{
					queryPath(m_strRoot);
				}
				else
				{
					emit rootDirDeleted();
				}
			}
		}
		else
		{
			m_strInitPath.clear();

			gotoLocation(appendPath(m_strRoot, m_strSubPath));
		}
		
	}
}

void QFrameRemoteFileSystem::recvNewDirectory(int flag, const FileInfo& file)
{
	switch (flag)
	{
	case QueryNewFail:
		{
			emit tip(Warning, tr("New directory failed! May be insufficient privileges!"));
		}
		break;
	case QueryNewSuccess:
		{
			QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();
			if (pTreeViewFileSystem != NULL)
			{
				QModelIndex index=pTreeViewFileSystem->appendFileInfo(file);

				pTreeViewFileSystem->setCurrentEdit(index);
			}
		}
		break;
	case QueryDirNoExist:
		{
			emit tip(Error, tr("New directory failed! The path is not existed!"));
		}
	default:
		qDebug("Invalid create directory op status");
		break;
	}
}

void QFrameRemoteFileSystem::recvDelete(int flag, const QVector<QString>& files)
{
	Q_UNUSED(files);

	switch (flag)
	{
	case QueryDelFail:
		{
			for (int i=0; i<files.size(); i++)
			{
				emit tip(Warning, 
					tr("Remote: Failed to delete \"%1\"! May be in use, or insufficient privileges!").arg(files[i]));
			}
		}
		break;
	case QueryDelSuccess:
		{
			emit tip(Information, tr("Remote: Delete file(s) or folder(s) successfully!"));
		}
		break;
	default:
		qDebug("Invalid delete op status");
		break;
	}

	refresh();
}

void QFrameRemoteFileSystem::recvRename(int flag, const QString& dir, const QString& old, const QString& now)
{
	Q_UNUSED(old);
	Q_UNUSED(now);

	switch (flag)
	{
	case QueryRenameFail:
		{
			emit tip(Error, tr("Remote: Rename failed! May be exists, in use or insufficient privileges!"));
		}
		break;
	case QueryRenamePathLost:
		{
			emit tip(Error, tr("Remote: Rename failed! The directory path has not exist!"));
		}
		break;
	case QueryRenameSuccess:
		break;
	default:
		break;
	}

	if (dir == comboBoxFileSystem()->currentLocation())
	{
		queryPath(dir);
	}
}

void QFrameRemoteFileSystem::clickedToolButton(int type)
{
	switch (type)
	{
	case QFrameFileSystem::ButtonHome:
		{
			gotoHome();
		}
		break;
	case QFrameFileSystem::ButtonUp:
		{
			QString strLocation=location();
			if (comboBoxFileSystem()->topItemType(strLocation) != -1)
			{
				break;
			}

			strLocation=comboBoxFileSystem()->parentLocation(strLocation);
			gotoLocation(strLocation);
		}
		break;
	default:
		break;
	}
}

void QFrameRemoteFileSystem::locationCurrentIndexChanged(QString location)
{
	gotoLocation(location);
}

void QFrameRemoteFileSystem::enterPressed(QString location)
{
	gotoLocation(location);
}

void QFrameRemoteFileSystem::editTextChangedLocation(const QString& location)
{
	QString strLocation=QDir::fromNativeSeparators(location);

	QChar ch;
	if (!strLocation.isEmpty() && comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
	{
		if (hasInvalidChar(strLocation, &ch))
		{
			emit enterInvalidChar(true, ch);
			return;
		}
	}

	emit enterInvalidChar(false, ch);
}

void QFrameRemoteFileSystem::doubleClickedLocation()
{
	QModelIndexList rows=treeViewFileSystem()->selectedRows();;

	if (rows.size() != 1)
	{
		return;
	}

	doubleClickedLocation(rows[0]);
}

void QFrameRemoteFileSystem::doubleClickedLocation(const QModelIndex& index)
{
	QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();
	QString strFileName=pTreeViewFileSystem->filename(index);
	QString strLocation;
	if (strFileName == "..")
	{
		strLocation=getParentDir(location());
	}
	else
	{
		int nType=pTreeViewFileSystem->type(index);

		if (nType == Dir || nType == HideDir || nType == SymLinkDir || nType == SymLinkHideDir)
		{
			strLocation=appendPath(location(), strFileName);
		}
		else if ((nType & Drive) != 0)
		{
			strLocation=strFileName;
		}
	}

	if (!strLocation.isEmpty())
	{
		gotoLocation(strLocation);
	}
}

void QFrameRemoteFileSystem::dataChanged(QModelIndex index, QString old, QString now)
{
	QString strLocation=comboBoxFileSystem()->currentLocation();
	if (comboBoxFileSystem()->topItemType(strLocation) != -1)
	{
		treeViewFileSystem()->setFileName(index, old);
	}
	else
	{
		queryRename(strLocation, old, now);
	}
}

void QFrameRemoteFileSystem::refresh()
{
	locationCurrentIndexChanged(comboBoxFileSystem()->currentLocation());
}

void QFrameRemoteFileSystem::newDirectory()
{
	QString strLocation=comboBoxFileSystem()->currentLocation();
	if (comboBoxFileSystem()->topItemType(strLocation) != -1)
	{
		return;
	}
	else
	{
		queryNewDirectory(strLocation);
	}
}

void QFrameRemoteFileSystem::delDirectory()
{
	QString strLocation=comboBoxFileSystem()->currentLocation();
	if (comboBoxFileSystem()->topItemType(strLocation) != -1)
	{
		return;
	}
	else
	{
		QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"),
			tr("Will permanently delete these files! Are you sure to delete?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::No);

		if (button == QMessageBox::Yes)
		{
			QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();
			QModelIndexList list=pTreeViewFileSystem->selectedRows();

			QVector<QString> files;
			foreach (QModelIndex index, list)
			{
				QString strFileName=pTreeViewFileSystem->filename(index);
				if (strFileName != ".." || strFileName != ".")
				{
					files.append(strFileName);
				}
			}

			queryDelete(strLocation, files);
		}
	}
}

void QFrameRemoteFileSystem::rename()
{
	QModelIndexList list;
	QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();

	if (pTreeViewFileSystem == NULL)
	{
		return;
	}

	list=pTreeViewFileSystem->selectedRows();
	if (list.size() == 1)
	{
		pTreeViewFileSystem->edit(list[0]);
	}
}

void QFrameRemoteFileSystem::mouseRightButtonPressed(QPoint pos)
{
	emit mouseRightButtonPress(treeViewFileSystem()->mapToGlobal(pos));
}
