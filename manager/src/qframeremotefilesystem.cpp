#include "qframeremotefilesystem.h"

#include <QHBoxLayout>
#include <QFileDialog>
#include <QMouseEvent>
#include <QMessageBox>
#include <QLineEdit>

#include "define.h"
#include "common.h"
#include "qmessagefiletransfer.h"
#include "qmessageparser.h"
#include "qfiletransferparser.h"
#include "qfilecontroller.h"

QFrameRemoteFileSystem::QFrameRemoteFileSystem(const QString& initPath, const QString& subPath, QWidget *parent)
	: QFrame(parent),
	m_pClient(NULL),
	m_bQueryComputer(false),
	m_strInitPath(initPath),
	m_strSubPath(subPath)
{
	m_pClientManager=new QClientManager(new QFileController(this), this);
	connect(m_pClientManager, SIGNAL(connected(QClient*)), this, SLOT(connected(QClient*)));

	m_nUsrID=QFileTransferParser::instance()->registerInterface(this);

	m_pFrameFileSystem=new QFrameFileSystem(this);
	QHBoxLayout* pLayout=new QHBoxLayout(this);
	setLayout(pLayout);
	pLayout->addWidget(m_pFrameFileSystem);

	connect(m_pFrameFileSystem, SIGNAL(clickedToolButton(int)), this, SLOT(clickedToolButton(int)));
	connect(m_pFrameFileSystem, SIGNAL(currentIndexChanged(QString)), this, SLOT(locationCurrentIndexChanged(QString)));

	connect(m_pFrameFileSystem->comboBoxFileSystem(), SIGNAL(editTextChanged(const QString&)),
		this, SLOT(editTextChangedLocation(const QString&)));
	connect(m_pFrameFileSystem->comboBoxFileSystem(), SIGNAL(enterPressed(QString)),
		this, SLOT(enterPressed(QString)));
	connect(m_pFrameFileSystem->comboBoxFileSystem(), SIGNAL(focusStatusChanged(bool)),
		this, SIGNAL(focusStatusChanged(bool)));

	connect(m_pFrameFileSystem->treeViewFileSystem(), SIGNAL(doubleClicked(const QModelIndex &)), 
		this, SLOT(doubleClickedLocation(const QModelIndex&)));
	connect(m_pFrameFileSystem->treeViewFileSystem(), SIGNAL(mouseRightButtonPress(QPoint)), 
		this, SLOT(mouseRightButtonPressed(QPoint)));
	connect(m_pFrameFileSystem->treeViewFileSystem(), SIGNAL(dataChanged(QModelIndex, QString, QString)), 
		this, SLOT(dataChanged(QModelIndex, QString, QString)));
}

QFrameRemoteFileSystem::~QFrameRemoteFileSystem()
{
	QFileTransferParser::instance()->unregisterInterface(m_nUsrID);
}

void QFrameRemoteFileSystem::setClient(const QServerManager::ServerInfo& info)
{
	if (m_pClient != NULL)
	{
		m_pClientManager->remove(m_pClient);
		m_pClient=NULL;
	}

	QClient* pClient=m_pClientManager->addClient(info);
	m_pClientManager->connectToHost(pClient);
}

QVector<QFrameRemoteFileSystem::TransferFileItem> QFrameRemoteFileSystem::selectedFileItems()
{
	QVector<QFrameRemoteFileSystem::TransferFileItem> items;

	QTreeViewFileSystem* pFileSystem=m_pFrameFileSystem->treeViewFileSystem();
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
	QFrameRemoteFileSystem::TransferFileItem item;
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
	m_pFrameFileSystem->clear();

	queryDrives();
}

void QFrameRemoteFileSystem::gotoHome()
{
	deepRefresh();
}

void QFrameRemoteFileSystem::gotoLocation(const QString& location)
{
	int itemType=m_pFrameFileSystem->comboBoxFileSystem()->topItemType(location);
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
	return m_pFrameFileSystem->comboBoxFileSystem()->lineEdit()->text();
}

QString QFrameRemoteFileSystem::currentLocationText()
{
	return m_pFrameFileSystem->comboBoxFileSystem()->currentLocation();
}

bool QFrameRemoteFileSystem::canUpload()
{
	QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem->treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QModelIndexList rows=pTreeViewFileSystem->selectedRows();

		QString strLocation=m_pFrameFileSystem->comboBoxFileSystem()->currentLocation();

		if (!rows.isEmpty())
		{
			if (m_pFrameFileSystem->comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
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
	QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem->treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QModelIndexList rows=pTreeViewFileSystem->selectedRows();

		QString strLocation=m_pFrameFileSystem->comboBoxFileSystem()->currentLocation();

		if (!rows.isEmpty())
		{
			if (m_pFrameFileSystem->comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
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
	QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem->treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QModelIndexList rows=pTreeViewFileSystem->selectedRows();

		if (rows.size() == 1)
		{
			int nType=pTreeViewFileSystem->type(rows[0]);
			if ((nType == QMessageFileTransfer::Drive) || (nType & QMessageFileTransfer::Dir) != 0 
				|| (nType & QMessageFileTransfer::HideDir) != 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool QFrameRemoteFileSystem::canRename()
{
	QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem->treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QModelIndexList rows=pTreeViewFileSystem->selectedRows();
		QString strLocation=m_pFrameFileSystem->comboBoxFileSystem()->currentLocation();

		if (rows.size() == 1)
		{
			if (m_pFrameFileSystem->comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
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
	QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem->treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QString strLocation=m_pFrameFileSystem->comboBoxFileSystem()->currentLocation();

		if (m_pFrameFileSystem->comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
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
		unconnected();
		return;
	}

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageFileTransfer::QueryRoot, m_nUsrID);

		bool bRtn=m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
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
		unconnected();
		return;
	}

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageFileTransfer::QueryDrive, m_nUsrID);
		bool bRtn=m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
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
		unconnected();
		return;
	}

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		int nSortColumn=m_pFrameFileSystem->treeViewFileSystem()->sortColumn();
		int nSort=0;
		switch (nSortColumn)
		{
		case 0:
			nSort=QMessageFileTransfer::SortName;
			break;
		case 1:
			nSort=QMessageFileTransfer::SortSize;
			break;
		case 2:
			nSort=QMessageFileTransfer::SortType;
			break;
		case 3:
			nSort=QMessageFileTransfer::SortTime;
			break;
		default:
			break;
		}
		stream<<path<<nSort;

		buffer=pMsg->query(QMessageFileTransfer::QueryPath, m_nUsrID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
		if (!bRtn)
		{
			emit serverQuit();
		}
	}
}

void QFrameRemoteFileSystem::queryRename(const QString& location, int row, const QString& old, const QString& now)
{
	if (m_pClient == NULL)
	{
		unconnected();
		return;
	}

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		stream<<location<<row<<old<<now;

		buffer=pMsg->query(QMessageFileTransfer::QueryRename, m_nUsrID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
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
		unconnected();
		return;
	}

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		stream<<location;

		buffer=pMsg->query(QMessageFileTransfer::QueryNewDirectory, m_nUsrID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
		if (!bRtn)
		{
			emit serverQuit();
		}
	}
}

void QFrameRemoteFileSystem::queryDelete(const QString& location, const QStringList& files)
{
	if (m_pClient == NULL)
	{
		unconnected();
		return;
	}

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		int nNum=files.size();
		stream<<location<<nNum;
		foreach (QString file, files)
		{
			stream<<file;
		}

		buffer=pMsg->query(QMessageFileTransfer::QueryDelete, m_nUsrID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
		if (!bRtn)
		{
			emit serverQuit();
		}
	}
}

void QFrameRemoteFileSystem::recvRoot(const QString& root)
{
	m_strRoot=root;

	m_pFrameFileSystem->setLocation(m_strRoot, false);

	if (m_strInitPath.isEmpty())
	{
		gotoLocation(appendPath(root, m_strSubPath));
	}
	else
	{
		gotoLocation(m_strInitPath);
	}
}


void QFrameRemoteFileSystem::recvDrive(const QVector<FileInfo>& drives)
{
	if (m_bQueryComputer)
	{
		m_bQueryComputer=false;

		m_pFrameFileSystem->setFileInfo(drives, QComboBoxFileSystem::Computer);
		QString strLocation=m_pFrameFileSystem->comboBoxFileSystem()->itemText(QComboBoxFileSystem::Computer);
		m_pFrameFileSystem->setLocation(strLocation, false);
	}
	else
	{
		m_pFrameFileSystem->setDrives(drives);

		queryRoot();
	}
}

void QFrameRemoteFileSystem::recvPath(const QString& path, int flag, const QVector<FileInfo>& files)
{
	if (flag == QMessageFileTransfer::QueryPathValid)
	{
		if (!m_strInitPath.isEmpty())
		{
			m_strInitPath.clear();
		}

		m_pFrameFileSystem->setLocation(path, false);
		m_pFrameFileSystem->setFileInfo(files);

		m_pFrameFileSystem->comboBoxFileSystem()->setFocus();
	}
	else if (flag == QMessageFileTransfer::QueryPathNoExist)
	{
		emit dirNoExist(path);

		if (m_strInitPath.isEmpty())
		{
			QString strLocation=m_pFrameFileSystem->location();

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
	case QMessageFileTransfer::NewFail:
		{
			emit tip(Warning, tr("New directory failed! May be insufficient privileges!"));
		}
		break;
	case QMessageFileTransfer::NewSuccess:
		{
			QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem->treeViewFileSystem();
			QModelIndex index=pTreeViewFileSystem->appendFileInfo(file);
			pTreeViewFileSystem->setCurrentEdit(index);
		}
		break;
	default:
		qDebug("Invalid create directory op status");
		break;
	}
}

void QFrameRemoteFileSystem::recvDelete(int flag)
{
	switch (flag)
	{
	case QMessageFileTransfer::DelFail:
		{
			emit tip(Warning, 
				"Delete file(s) or file folder(s) failed! May be in use, or insufficient privileges!");
		}
		break;
	case QMessageFileTransfer::DelSuccess:
		{
			refresh();
		}
		break;
	default:
		qDebug("Invalid delete op status");
		break;
	}
}

void QFrameRemoteFileSystem::recvRename(int row, QString old, QString now, int flag)
{
	switch (flag)
	{
	case QMessageFileTransfer::RenameFail:
	case QMessageFileTransfer::RenamePathLost:
		{
			m_pFrameFileSystem->treeViewFileSystem()->setFileName(row, old);
		}
		break;
	case QMessageFileTransfer::RenameSuccess:
		{
			m_pFrameFileSystem->treeViewFileSystem()->setFileName(row, now);
		}
		break;
	default:
		break;
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
			QString strLocation=m_pFrameFileSystem->location();
			if (m_pFrameFileSystem->comboBoxFileSystem()->topItemType(strLocation) != -1)
			{
				break;
			}

			strLocation=m_pFrameFileSystem->comboBoxFileSystem()->parentLocation(strLocation);
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
	if (!strLocation.isEmpty() && 
		m_pFrameFileSystem->comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
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
	QModelIndexList rows=m_pFrameFileSystem->treeViewFileSystem()->selectedRows();;

	if (rows.size() != 1)
	{
		return;
	}

	doubleClickedLocation(rows[0]);
}

void QFrameRemoteFileSystem::doubleClickedLocation(const QModelIndex& index)
{
	QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem->treeViewFileSystem();
	QString strFileName=pTreeViewFileSystem->filename(index);
	QString strLocation;
	if (strFileName == "..")
	{
		strLocation=getParentDir(m_pFrameFileSystem->location());
	}
	else
	{
		int nType=pTreeViewFileSystem->type(index);

		if (nType == QMessageFileTransfer::Dir || nType == QMessageFileTransfer::HideDir
			|| nType == QMessageFileTransfer::SymLinkDir || nType == QMessageFileTransfer::SymLinkHideDir)
		{
			strLocation=appendPath(m_pFrameFileSystem->location(), strFileName);
		}
		else if ((nType & QMessageFileTransfer::Drive) != 0)
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
	QString strLocation=m_pFrameFileSystem->comboBoxFileSystem()->currentLocation();
	if (m_pFrameFileSystem->comboBoxFileSystem()->topItemType(strLocation) != -1)
	{
		m_pFrameFileSystem->treeViewFileSystem()->setFileName(index, old);
	}
	else
	{
		queryRename(strLocation, index.row(), old, now);
	}
}

void QFrameRemoteFileSystem::refresh()
{
	locationCurrentIndexChanged(m_pFrameFileSystem->comboBoxFileSystem()->currentLocation());
}

void QFrameRemoteFileSystem::newDirectory()
{
	QString strLocation=m_pFrameFileSystem->comboBoxFileSystem()->currentLocation();
	if (m_pFrameFileSystem->comboBoxFileSystem()->topItemType(strLocation) != -1)
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
	QString strLocation=m_pFrameFileSystem->comboBoxFileSystem()->currentLocation();
	if (m_pFrameFileSystem->comboBoxFileSystem()->topItemType(strLocation) != -1)
	{
		return;
	}
	else
	{
		QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"),
			tr("Will permanently delete these files!\nAre you sure to delete?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::No);

		if (button == QMessageBox::Yes)
		{
			QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem->treeViewFileSystem();
			QModelIndexList list=pTreeViewFileSystem->selectedRows();

			QStringList files;
			foreach (QModelIndex index, list)
			{
				files<<pTreeViewFileSystem->filename(index);
			}

			queryDelete(strLocation, files);
		}
	}
}

void QFrameRemoteFileSystem::rename()
{
	QModelIndexList list;
	QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem->treeViewFileSystem();

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
	emit mouseRightButtonPress(m_pFrameFileSystem->treeViewFileSystem()->mapToGlobal(pos));
}
