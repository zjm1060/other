#include "qframesyncfile.h"

#include <QDir>

#include "define.h"
#include "common.h"
#include "qmessagefiletransfer.h"
#include "qmessageparser.h"
#include "qfiletransferparser.h"

#define MAX_TICK_NUMBER		20

QFrameSyncFile::QFrameSyncFile(QClient* client, const QString& subPath, QWidget *parent)
	: QAbstractSync(parent),
	m_pFile(NULL),
	m_bQueryComputer(false),
	m_nDynamicID(QFileTransferParser::UserInvaild)
{
	m_pClient=client;
	m_nUsrID=QFileTransferParser::instance()->registerInterface(this);

	m_nTick=0;
	m_timer.setInterval(1000);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	m_strSubPath=subPath;
	remoteDeepRefresh();
}

QFrameSyncFile::~QFrameSyncFile()
{
	QFileTransferParser::instance()->unregisterInterface(m_nUsrID);
}

void QFrameSyncFile::recvRoot(const QString& root)
{
	m_strRoot=root;

	m_pFrameFileSystem[RemoteMachine]->setLocation(m_strRoot, false);

	setLocation(RemoteMachine, appendPath(root, m_strSubPath));
}

void QFrameSyncFile::recvDrive(const QVector<FileInfo>& drives)
{
	if (m_bQueryComputer)
	{
		m_bQueryComputer=false;

		m_pFrameFileSystem[RemoteMachine]->setFileInfo(drives, QComboBoxFileSystem::Computer);
		QString strLocation=m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->itemText(QComboBoxFileSystem::Computer);
		m_pFrameFileSystem[RemoteMachine]->setLocation(strLocation, false);
	}
	else
	{
		m_pFrameFileSystem[RemoteMachine]->setDrives(drives);

		queryRoot();
	}
}

void QFrameSyncFile::recvPath(const QString& path, int flag, const QVector<FileInfo>& files)
{
	if (flag == QMessageFileTransfer::QueryPathValid)
	{
		m_pFrameFileSystem[RemoteMachine]->setLocation(path, false);
		m_pFrameFileSystem[RemoteMachine]->setFileInfo(files);

		m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->setFocus();
	}
	else if (flag == QMessageFileTransfer::QueryPathNoExist)
	{
		appendTip(QTipsDelegate::Warning, tr("Server:")+"Directory \""+path+"\" "+tr("doesn't existed!"));

		QString strLocation=m_pFrameFileSystem[RemoteMachine]->location();

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
				appendTip(QTipsDelegate::Error, tr("Server:")+tr("The program has been deleted!"));
			}
		}
	}
}

void QFrameSyncFile::recvDir(const QString& path, int flag, const QVector<FileInfo>& files)
{
	if (flag == QMessageFileTransfer::QueryPathValid)
	{
		QDir dir(m_task.target);

		QString strName=m_listFileInfo[m_nIndex].name;
		QString strPath=QTreeViewTransferQueue::appendPathName(m_task.target, strName, m_task.isTop);
		dir.mkpath(strPath);

		int i=1;
		foreach (FileInfo info, files)
		{
			if (info.name == "." || info.name == "..")
			{
				continue;
			}

			info.name=strName+"/"+info.name;

			m_listFileInfo.insert(m_nIndex+i, info);

			i++;
		}
	}
	else
	{
		appendTip(QTipsDelegate::Warning, tr("Server:")+"\""+path+"\" "+tr("doesn't existed!"));

		m_bStatus=false;
	}

	m_nIndex++;
	downloads();
}

void QFrameSyncFile::recvFileInfo(int flag, quint64 size)
{
	QString strName=m_listFileInfo[m_nIndex].name;
	QString strSourceName=QTreeViewTransferQueue::appendPathName(m_task.source, strName, m_task.isTop);

	switch (flag)
	{
	case QMessageFileTransfer::QueryFileValid:
		{
			SAFE_CLOSE_FILE(m_pFile);

			QString strFile=QTreeViewTransferQueue::appendPathName(m_task.target, strName+TEMP_FILE_SUFFIX, m_task.isTop);
			m_pFile=new QFile(strFile);

			int nFormat=m_task.format;
			if (nFormat == AutoFormat)
			{
				if (isTextFormat(strName))
				{
					nFormat=TextFormat;
				}
				else
				{
					nFormat=BinaryFormat;
				}
			}

			QIODevice::OpenMode openMode=QIODevice::WriteOnly;
			if (nFormat == TextFormat)
			{
				openMode|=QIODevice::Text;
			}
			else if (nFormat != BinaryFormat)
			{
				qDebug("Invalid file transfer format!");
			}

			if (m_pFile != NULL && m_pFile->open(openMode))
			{
				m_nSize=size;

				m_nFormat=QMessageFileTransfer::TextFormat;
				if (nFormat == BinaryFormat)
				{
					m_nFormat=BinaryFormat;
				}

				queryTransferFile(strSourceName, m_nFormat);
			}
			else
			{
				appendTip(QTipsDelegate::Error, tr("Server:")+" \" "+strFile+"\" "+tr("doesn't open!"));

				m_bStatus=false;

				//打开文件失败
				SAFE_DELETE_CLASS(m_pFile);

				m_nIndex++;
				downloads();
			}
		}
		break;
	case QMessageFileTransfer::QueryFileNoExist:
		{
			appendTip(QTipsDelegate::Error, tr("Server:")+" \""+strSourceName+"\" "+tr("doesn't exist!"));

			m_bStatus=false;

			m_nIndex++;
			downloads();
		}
		break;
	case QMessageFileTransfer::QueryFileUnreadable:
		{
			m_bStatus=false;

			appendTip(QTipsDelegate::Error, tr("Server:")+" \""+strSourceName+"\" "+tr("is unreadable!"));

			m_nIndex++;
			downloads();
		}
		break;
	default:
		qDebug("Receive file info error!");
		break;
	}
}

void QFrameSyncFile::recvFileData(int flag, const QByteArray& buffer)
{
	QString strSourceName=QTreeViewTransferQueue::appendPathName(m_task.source, m_listFileInfo[m_nIndex].name, m_task.isTop);

	switch (flag)
	{
	case QMessageFileTransfer::TransferNormal:
		{
			if (m_pFile != NULL)
			{
				quint64 nSize=m_pFile->write(buffer);
				if (nSize != buffer.size())
				{
					m_bStatus=false;

					appendTip(QTipsDelegate::Error, tr("Download")+" \""+strSourceName+"\""+tr("failed!"));

					m_pFile->remove();
					SAFE_CLOSE_FILE(m_pFile);

					m_nIndex++;
					downloads();
				}
				else
				{
					queryTransferFile(strSourceName, m_nFormat);
				}
			}
			else
			{
				m_bStatus=false;

				appendTip(QTipsDelegate::Error, tr("Download")+" \""+strSourceName+"\" "+tr("failed!"));
			}
		}
		break;
	case QMessageFileTransfer::TransferFinish:
		{
			if (m_pFile != NULL)
			{
				QString strFile=QTreeViewTransferQueue::appendPathName(m_task.target, m_listFileInfo[m_nIndex].name, m_task.isTop);
				if (QFile::exists(strFile))
				{
					QFile::remove(strFile);
				}

				if (m_pFile->rename(strFile))
				{
					appendTip(QTipsDelegate::Success, tr("Download:")+" "+strSourceName+" -> "+strFile);
				}
				else
				{
					m_bStatus=false;

					appendTip(QTipsDelegate::Error, tr("Download: Rename")+" \""+strSourceName+" \" "+tr("Failed!"));
				}

				SAFE_CLOSE_FILE(m_pFile);

				m_nIndex++;
				downloads();
			}
			else
			{
				m_bStatus=false;

				appendTip(QTipsDelegate::Error, tr("Download")+" \""+strSourceName+"\" "+tr("failed!"));
			}
		}
		break;
	case QMessageFileTransfer::TransferReadError:
		{
			m_bStatus=false;

			appendTip(QTipsDelegate::Error, tr("Download")+" \""+strSourceName+"\" "+tr("failed!"));

			if (m_pFile != NULL)
			{
				m_pFile->remove();
				SAFE_CLOSE_FILE(m_pFile);
			}

			m_nIndex++;
			downloads();
		}
		break;
	default:
		qDebug("Receive file data status error!");
		break;
	}
}

void QFrameSyncFile::recvSymLink(int flag, FileInfo info, const QString& symLink)
{
	Q_UNUSED(info);

	QString strSourceName=QTreeViewTransferQueue::appendPathName(m_task.source, m_listFileInfo[m_nIndex].name, m_task.isTop);

	switch (flag)
	{
	case QMessageFileTransfer::QueryFileNoExist:
		{
			appendTip(QTipsDelegate::Error, 
				tr("Server:")+"\""+strSourceName+"\""+tr("doesn't exist!"));
		}
		break;
	case QMessageFileTransfer::QueryFileUnreadable:
		{
			appendTip(QTipsDelegate::Error, 
				tr("Server:")+"\""+strSourceName+"\""+tr("is unreadable!"));
		}
		break;
	case QMessageFileTransfer::QuerySymLinkNoLink:
		{
			appendTip(QTipsDelegate::Error, 
				tr("Server:")+"\""+strSourceName+"\""+tr("is invalid!"));
		}
		break;
	case QMessageFileTransfer::QueryFileValid:
		{
			QString strFile=QTreeViewTransferQueue::appendPathName(m_task.target, m_listFileInfo[m_nIndex].name, m_task.isTop);
			bool bRtn=QFile::link(symLink, strFile);
			if (bRtn)
			{
				appendTip(QTipsDelegate::Success, strSourceName+" -> "+strFile);
			}
		}
		break;
	default:
		qDebug("Receive symbol link error!");
		break;
	}

	if (flag != QMessageFileTransfer::QueryFileValid)
	{
		m_bStatus=false;
	}

	m_nIndex++;
	downloads();
}

void QFrameSyncFile::recvUpload(int flag)
{
	QString strFile=QTreeViewTransferQueue::appendPathName(m_task.target, m_listFileInfo[m_nIndex].name, m_task.isTop);

	switch (flag)
	{
	case QMessageFileTransfer::QueryMakePathFail:
		{
			m_bStatus=false;

			appendTip(QTipsDelegate::Error, tr("Upload directory")+" \""+strFile+"\" "+tr("Failed!")
				+tr("May be insufficient permissions"));

			m_listFileInfo.clear();
		}
		break;
	case QMessageFileTransfer::QueryMakePathSuccess:
		{
			QString strName=m_listFileInfo[m_nIndex].name;
			QString strPath=QTreeViewTransferQueue::appendPathName(m_task.source, strName, m_task.isTop);
			QDir dir(strPath);

			QVector<FileInfo> files;

			QFileInfoList list=dir.entryInfoList(QDir::AllEntries | QDir::System | QDir::NoDotAndDotDot, 
				QDir::DirsFirst|QDir::Name|QDir::IgnoreCase);
			QMessageFileTransferRunnable::entryInfoList(list, files);

			int i=1;
			foreach (FileInfo info, files)
			{
				info.name=strName+"/"+info.name;

				m_listFileInfo.insert(m_nIndex+i, info);

				i++;
			}
		}
		break;
	case QMessageFileTransfer::QueryCreateFileFail:
		{
			m_bStatus=false;

			appendTip(QTipsDelegate::Error, tr("Upload File")+" \""+strFile+"\" "+tr("Failed!")
				+tr("May be insufficient permissions"));
		}
		break;
	case QMessageFileTransfer::QueryCreateFileSuccess:
		{
			SAFE_CLOSE_FILE(m_pFile);

			QString strName=m_listFileInfo[m_nIndex].name;
			QString strPath=QTreeViewTransferQueue::appendPathName(m_task.source, strName, m_task.isTop);
			m_pFile=new QFile(strPath);

			int nFormat=m_task.format;
			if (nFormat == AutoFormat)
			{
				if (isTextFormat(strName))
				{
					nFormat=TextFormat;
				}
				else
				{
					nFormat=BinaryFormat;
				}
			}

			QIODevice::OpenMode openMode=QIODevice::ReadOnly;
			if (nFormat == TextFormat)
			{
				openMode|=QIODevice::Text;
			}
			else if (nFormat != BinaryFormat)
			{
				qDebug("Invalid file transfer format!");
			}

			if (m_pFile != NULL && m_pFile->open(openMode))
			{
				appendTip(QTipsDelegate::Upload, tr("Begin to upload")+" \""+strPath+"\"");

				queryUploadFile();

				return;
			}
			else
			{
				m_bStatus=false;

				//打开文件失败
				SAFE_DELETE_CLASS(m_pFile);

				appendTip(QTipsDelegate::Error, "\""+strFile+"\" "+tr("doesn't open!"));
			}
		}
		break;
	case QMessageFileTransfer::QueryUploadSymLinkFail:
		{
			m_bStatus=false;

			appendTip(QTipsDelegate::Error, tr("Upload symbol link")+" \""+strFile+"\" "+tr("Failed!")
				+tr("May be insufficient permissions"));
		}
		break;
	case QMessageFileTransfer::QueryUploadSymLinkSuccess:
		{
			appendTip(QTipsDelegate::Success, tr("Upload symbol link")+" \""+strFile+"\" "+tr("success!")
				+tr("May be insufficient permissions"));
		}
		break;
	default:
		break;
	}

	m_nIndex++;
	uploads();
}

void QFrameSyncFile::recvUploadFile(int flag)
{
	QString strName=m_listFileInfo[m_nIndex].name;
	QString strPath=QTreeViewTransferQueue::appendPathName(m_task.target, strName, m_task.isTop);

	switch (flag)
	{
	case QMessageFileTransfer::QueryUploadFileWriteSuccess:
		{
			queryUploadFile();
		}
		break;
	case QMessageFileTransfer::QueryUploadFileWriteFail:
		{
			m_bStatus=false;

			appendTip(QTipsDelegate::Error, tr("Upload File")+" \""+strPath+" \""+tr("failed!"));

			SAFE_CLOSE_FILE(m_pFile);

			m_nIndex++;
			uploads();
		}
		break;
	case QMessageFileTransfer::QueryUploadFileSuccess:
		{
			appendTip(QTipsDelegate::Success, tr("Upload File")+" \""+strPath+" \""+tr("success!"));

			m_nIndex++;
			uploads();
		}
		break;
	case QMessageFileTransfer::QueryUploadFileFail:
		{
			m_bStatus=false;

			appendTip(QTipsDelegate::Error, tr("Upload File")+" \""+strPath+" \""+tr("failed!")+tr("Save file failed!"));

			SAFE_CLOSE_FILE(m_pFile);

			m_nIndex++;
			uploads();
		}
		break;
	case QMessageFileTransfer::QueryUploadFileWriteLost:
		{
			m_bStatus=false;

			appendTip(QTipsDelegate::Error, tr("Upload File")+" \""+strPath+" \""+tr("failed!")+tr("File handle closed!"));

			SAFE_CLOSE_FILE(m_pFile);

			m_nIndex++;
			uploads();
		}
		break;
	default:
		break;
	}
}

void QFrameSyncFile::recvRename(int row, QString old, QString now, int flag)
{
	switch (flag)
	{
	case QMessageFileTransfer::RenameFail:
	case QMessageFileTransfer::RenamePathLost:
		{
			m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem()->setFileName(row, old);
		}
		break;
	case QMessageFileTransfer::RenameSuccess:
		{
			m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem()->setFileName(row, now);
		}
		break;
	default:
		break;
	}
}

void QFrameSyncFile::recvNewDirectory(int flag, const FileInfo& file)
{
	switch (flag)
	{
	case QMessageFileTransfer::NewFail:
		{
			appendTip(QTipsDelegate::Warning, tr("Remote: New directory failed! May be insufficient privileges!"));
		}
		break;
	case QMessageFileTransfer::NewSuccess:
		{
			QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem();
			QModelIndex index=pTreeViewFileSystem->appendFileInfo(file);
			pTreeViewFileSystem->setCurrentEdit(index);
		}
		break;
	default:
		qDebug("Invalid create directory op status");
		break;
	}
}

void QFrameSyncFile::recvDelete(int flag)
{
	switch (flag)
	{
	case QMessageFileTransfer::DelFail:
		{
			appendTip(QTipsDelegate::Warning, 
				tr("Remote: Delete file(s) or file folder(s) failed! May be in use, or insufficient privileges!"));
		}
		break;
	case QMessageFileTransfer::DelSuccess:
		{
			refresh(RemoteMachine);
		}
		break;
	default:
		qDebug("Invalid delete op status");
		break;
	}
}

void QFrameSyncFile::transfer(int row, const QTreeViewTransferQueue::TransferQueueItem& task)
{
	m_nTick=0;
	m_timer.start();

	m_nDynamicID=QFileTransferParser::instance()->registerInterface(this);

	transferFile(row, task);
}

void QFrameSyncFile::transferFile(int row, const QTreeViewTransferQueue::TransferQueueItem& task)
{
	m_listFileInfo.clear();
	m_nIndex=0;

	if (row == -1)
	{
		//同步完毕
		finished();
		return;
	}

	if (m_nDynamicID == QFileTransferParser::UserInvaild)
	{
		return;
	}

	m_nRow=row;
	m_task=task;
	m_bStatus=true;

	FileInfo file;
	file.name=task.name;
	file.type=task.type;
	m_listFileInfo.append(file);

	if (m_task.direction == QTreeViewTransferQueue::Download)
	{
		downloads();
	}
	else if (m_task.direction == QTreeViewTransferQueue::Upload)
	{
		uploads();
	}
}

void QFrameSyncFile::downloads()
{
	if (m_nIndex < m_listFileInfo.size())
	{
		FileInfo file=m_listFileInfo[m_nIndex];
		QString strPath=QTreeViewTransferQueue::appendPathName(m_task.source, file.name, m_task.isTop);
		if (file.type == QMessageFileTransfer::Dir || file.type == QMessageFileTransfer::HideDir)
		{
			queryDir(strPath);
		}
		else if (file.type == QMessageFileTransfer::File || file.type == QMessageFileTransfer::HideFile)
		{
			queryFile(strPath);
		}
		else if ((file.type & QMessageFileTransfer::SymLink) != 0 || (file.type & QMessageFileTransfer::SymLink) != 0)
		{
			querySymLink(strPath);
		}
	}
	else
	{
		fetchTask();
	}
}

void QFrameSyncFile::uploads()
{
	if (!m_bSync)
	{
		return;
	}

	if (m_nIndex < m_listFileInfo.size())
	{
		queryUpload();
	}
	else
	{
		fetchTask();
	}
}

void QFrameSyncFile::fetchTask()
{
	if (!m_bSync)
	{
		return;
	}

	QFileTransferParser::instance()->unregisterInterface(m_nDynamicID);
	m_nDynamicID=QFileTransferParser::instance()->registerInterface(this);

	m_pViewTransferQueue->setItemStatus(m_nRow, m_bStatus? QTreeViewTransferQueue::Finished:QTreeViewTransferQueue::Failed);
	m_nRow=m_pViewTransferQueue->fetchTask(m_task);
	transferFile(m_nRow, m_task);
}

void QFrameSyncFile::queryRoot()
{
	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageFileTransfer::QueryRoot, m_nUsrID);

		bool bRtn=m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
		if (!bRtn)
		{
			appendTip(QTipsDelegate::Error, tr("Disconnected to server!"));
		}
	}
}

void QFrameSyncFile::queryDrives()
{
	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageFileTransfer::QueryDrive, m_nUsrID);
		bool bRtn=m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
		if (!bRtn)
		{
			appendTip(QTipsDelegate::Error, tr("Disconnected to server!"));
		}
	}
}

void QFrameSyncFile::queryPath(const QString& path)
{
	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		int nSortColumn=m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem()->sortColumn();
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
			appendTip(QTipsDelegate::Error, tr("Disconnected to server!"));
		}
	}
}

void QFrameSyncFile::queryFile(const QString& file)
{
	if (m_nDynamicID == QFileTransferParser::UserInvaild)
	{
		return;
	}

	m_nTick=0;

	appendTip(QTipsDelegate::Download, tr("Query to download")+" \""+file+"\" ...");

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		stream<<file;

		buffer=pMsg->query(QMessageFileTransfer::QueryFile, m_nDynamicID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
		if (!bRtn)
		{
			cancel();
			appendTip(QTipsDelegate::Warning, tr("Disconnected to server!"));
		}
	}
}

void QFrameSyncFile::queryDir(const QString& path)
{
	if (m_nDynamicID == QFileTransferParser::UserInvaild)
	{
		return;
	}

	m_nTick=0;

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		stream<<path;

		buffer=pMsg->query(QMessageFileTransfer::QueryDir, m_nDynamicID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
		if (!bRtn)
		{
			appendTip(QTipsDelegate::Warning, tr("Disconnected to server!"));

			cancel();
		}
	}
}

void QFrameSyncFile::querySymLink(const QString& symLink)
{
	if (m_nDynamicID == QFileTransferParser::UserInvaild)
	{
		return;
	}

	m_nTick=0;

	appendTip(QTipsDelegate::Download, tr("Query to download symbol link file")+" \""+symLink+"\" ...");

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		stream<<symLink;

		buffer=pMsg->query(QMessageFileTransfer::QuerySymLink, m_nDynamicID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
		if (!bRtn)
		{
			cancel();
			appendTip(QTipsDelegate::Warning, tr("Disconnected to server!"));
		}
	}
}

void QFrameSyncFile::queryTransferFile(const QString& file, int format)
{
	if (m_nDynamicID == QFileTransferParser::UserInvaild)
	{
		return;
	}

	m_nTick=0;

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		stream<<file<<format;

		buffer=pMsg->query(QMessageFileTransfer::QueryTransferFile, m_nDynamicID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
		if (!bRtn)
		{
			cancel();
			appendTip(QTipsDelegate::Warning, tr("Disconnected to server!"));
		}
	}
}

void QFrameSyncFile::queryUpload()
{
	if (m_nDynamicID == QFileTransferParser::UserInvaild)
	{
		return;
	}

	if (!m_bSync)
	{
		return;
	}

	m_nTick=0;

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		const FileInfo& file=m_listFileInfo[m_nIndex];
		QString strPath=QTreeViewTransferQueue::appendPathName(m_task.target, file.name, m_task.isTop);
		stream<<file.type<<strPath;
		if (file.type == QMessageFileTransfer::File || file.type == QMessageFileTransfer::HideFile)
		{
			int nFormat=m_task.format;
			if (nFormat == AutoFormat)
			{
				if (isTextFormat(file.name))
				{
					nFormat=TextFormat;
				}
				else
				{
					nFormat=BinaryFormat;
				}
			}
			stream<<nFormat;
		}
		else if ((file.type | QMessageFileTransfer::SymLink) != 0 || (file.type | QMessageFileTransfer::HideSymLink) != 0)
		{
			stream<<QFile::symLinkTarget(QTreeViewTransferQueue::appendPathName(m_task.source, file.name, m_task.isTop));
		}

		buffer=pMsg->query(QMessageFileTransfer::QueryUpload, m_nDynamicID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
		if (!bRtn)
		{
			cancel();
			appendTip(QTipsDelegate::Warning, tr("Disconnected to server!"));
		}
	}
}

void QFrameSyncFile::queryUploadFile()
{
	if (m_nDynamicID == QFileTransferParser::UserInvaild)
	{
		return;
	}

	if (!m_bSync)
	{
		return;
	}


	m_nTick=0;

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		QByteArray contents;
		int nStatus=QMessageFileTransferRunnable::readContents(m_pFile, contents);
		if (nStatus == QMessageFileTransfer::TransferReadError)
		{
			const FileInfo& file=m_listFileInfo[m_nIndex];
			QString strPath=QTreeViewTransferQueue::appendPathName(m_task.source, file.name, m_task.isTop);

			appendTip(QTipsDelegate::Error, tr("Upload the file")+"\""+strPath+"\" "+"Failed!");

			SAFE_CLOSE_FILE(m_pFile);

			m_nIndex++;
			uploads();
		}
		else
		{
			if (nStatus == QMessageFileTransfer::TransferFinish)
			{
				SAFE_CLOSE_FILE(m_pFile);
			}

			if (!m_bSync)
			{
				const FileInfo& file=m_listFileInfo[m_nIndex];
				QString strPath=QTreeViewTransferQueue::appendPathName(m_task.source, file.name, m_task.isTop);
				appendTip(QTipsDelegate::Error, tr("Upload the file")+" \""+strPath+" \" "+tr("failed!"));
				return;
			}

			stream<<nStatus<<contents.size();
			stream.writeRawData(contents.data(), contents.size());

			buffer=pMsg->query(QMessageFileTransfer::QueryUploadFile, m_nDynamicID, buffer);

			bool bRtn=m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
			if (!bRtn)
			{
				cancel();
				appendTip(QTipsDelegate::Warning, tr("Disconnected to server!"));

				return;
			}
		}
	}
}

void QFrameSyncFile::queryRename(const QString& location, int row, const QString& old, const QString& now)
{
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
			appendTip(QTipsDelegate::Warning, tr("Disconnected to server!"));
		}
	}
}

void QFrameSyncFile::queryNewDirectory(const QString& location)
{
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
			appendTip(QTipsDelegate::Warning, tr("Disconnected to server!"));
		}
	}
}

void QFrameSyncFile::queryDelete(const QString& location, const QStringList& files)
{
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
			appendTip(QTipsDelegate::Warning, tr("Disconnected to server!"));
		}
	}
}

void QFrameSyncFile::cancel()
{
	QFileTransferParser::instance()->unregisterInterface(m_nDynamicID);
	m_nDynamicID=QFileTransferParser::UserInvaild;

	m_timer.stop();
	m_listFileInfo.clear();

	if (m_pFile != NULL)
	{
		m_pFile->remove();
		SAFE_CLOSE_FILE(m_pFile);
	}

	finished();
}

void QFrameSyncFile::timeout()
{
	m_nTick++;

	if (m_nTick > MAX_TICK_NUMBER)
	{
		cancel();
		appendTip(QTipsDelegate::Error, tr("Transfer time out."));
	}
}

void QFrameSyncFile::finished()
{
	m_timer.stop();

	QAbstractSync::finished();
	
	appendTip(QTipsDelegate::Information, tr("Synchronization finished."));
}

void QFrameSyncFile::gotoRemoteHome()
{
	remoteDeepRefresh();
}

void QFrameSyncFile::gotoRemoteLocation(const QString& location)
{
	int itemType=m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->topItemType(location);
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

void QFrameSyncFile::remoteDeepRefresh()
{
	m_pFrameFileSystem[RemoteMachine]->clear();

	if (!m_bSync)
	{
		m_pModelTips->setRowCount(0);
	}

	queryDrives();
}
