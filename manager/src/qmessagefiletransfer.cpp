#include "qmessagefiletransfer.h"

#include <QThreadPool>
#include <QFileInfo>
#include <QDir>

#include "define.h"
#include "common.h"
#include "extern.h"

#define FILE_TRANSFER_TICK_TIMES	30
#define FILE_PACKET_SIZE	(8*1024)

QMessageFileTransfer::QMessageFileTransfer(QObject *parent)
	: QObject(parent)
{
	m_timer.setInterval(10*1000);

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	qRegisterMetaType<QFile*>("QFile*");
	qRegisterMetaType<QObject*>("QObject*");
}

QMessageFileTransfer::~QMessageFileTransfer()
{

}

bool QMessageFileTransfer::addTransferFile(QObject* client, quint32 usrID, QFile* file)
{
	if (client == 0 || file == NULL)
	{
		return false;
	}

	int nNum=m_mapFiles.count();

	if (m_mapFiles.contains(client))
	{
		QMap<quint32, FileHandle>& mapFiles=m_mapFiles[client];
		if (mapFiles.contains(usrID))
		{
			FileHandle& handle=mapFiles[usrID];
			if (handle.file != NULL)
			{
				SAFE_CLOSE_FILE(handle.file);
			}
		}
	}
	else
	{
		m_mapFiles.insert(client, QMap<quint32, FileHandle>());
	}
	
	FileHandle fileHandle;
	fileHandle.file=file;
	fileHandle.tick=0;

	QMap<quint32, FileHandle>& mapFiles=m_mapFiles[client];
	mapFiles.insert(usrID, fileHandle);

	if (nNum == 0)
	{
		m_timer.start();
	}

	return true;
}
void QMessageFileTransfer::removeTransferFile(QObject* client, quint32 usrID)
{
	if (client == NULL)
	{
		return;
	}

	if (m_mapFiles.contains(client))
	{
		QMap<quint32, FileHandle>& mapFiles=m_mapFiles[client];
		if (mapFiles.contains(usrID))
		{
			FileHandle& handle=mapFiles[usrID];
			if (handle.file != NULL)
			{
				SAFE_CLOSE_FILE(handle.file);
			}

			mapFiles.remove(usrID);
		}

		if (mapFiles.isEmpty())
		{
			m_mapFiles.remove(client);
		}

		if (m_mapFiles.count() == 0)
		{
			m_timer.stop();
		}
	}
}

QFile* QMessageFileTransfer::transferFile(QObject* client, quint32 usrID)
{
	QFile* file=NULL;

	if (client != NULL)
	{
		if (m_mapFiles.contains(client))
		{
			QMap<quint32, FileHandle>& mapFiles=m_mapFiles[client];
			if (mapFiles.contains(usrID))
			{
				mapFiles[usrID].tick=0;
				file=mapFiles[usrID].file;
			}
		}	
	}
	
	return file;
}

void QMessageFileTransfer::timeout()
{
	QMutableMapIterator<QObject*, QMap<quint32, FileHandle> > it(m_mapFiles);
	while (it.hasNext())
	{
		it.next();

 		QMap<quint32, FileHandle>& mapFiles=it.value();
 		QMutableMapIterator<quint32, FileHandle> iter(mapFiles);
 		while (iter.hasNext())
 		{
 			iter.next();
 
 			if (iter.value().tick > FILE_TRANSFER_TICK_TIMES)
 			{
 				SAFE_CLOSE_FILE(iter.value().file);
				iter.remove();
 			}
 			else
 			{
 				iter.value().tick++;
 			}
 		}
	}

	it.previous();
	while (it.hasNext())
	{
		it.next();
		if (it.value().isEmpty())
		{
			it.remove();
		}
	}

	if (m_mapFiles.isEmpty())
	{
		m_timer.stop();
	}
}

QByteArray QMessageFileTransfer::query(int msgType, quint32 usrID, const QByteArray& contents)
{
	QByteArray buffer;
	QDataStream in(&buffer, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	in<<msgType<<usrID;
	if (contents.size() > 0)
	{
		in.writeRawData(contents.data(), contents.size());
	}

	return buffer;
}

bool QMessageFileTransfer::parse(QConnection* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageFileTransferRunnable* pRunnable=new QMessageFileTransferRunnable(sender, buffer, view, this);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QConnection*, int, quint32, QByteArray, int)), 
		this, SLOT(finished(QConnection*, int, quint32, QByteArray, int)));

	QThreadPool::globalInstance()->start(pRunnable);

	return true;
}

void QMessageFileTransfer::finished(QConnection* sender, int msgType, quint32 usrID, QByteArray buffer, int status)
{
	if (sender == NULL)
	{
		return;
	}

	QByteArray buf;
	QDataStream in(&buf, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	switch (msgType)
	{
	case QueryRoot:
		in<<AnswerRoot<<usrID;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	case QueryPath:
		in<<AnswerPath<<usrID;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	case QueryFile:
		in<<AnswerFileInfo<<usrID;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	case QueryTransferFile:
	case QueryRetransmitFile:
		in<<AnswerTransferFile<<usrID<<status<<buffer.size();
		in.writeRawData(buffer.data(), buffer.size());
		break;
	case QueryDir:
		in<<AnswerDir<<usrID;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	case QuerySymLink:
		in<<AnswerSymLink<<usrID;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	case QueryDrive:
		in<<AnswerDrive<<usrID;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	case QueryUpload:
		in<<AnswerUpload<<usrID;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	case QueryUploadFile:
		in<<AnswerUploadFile<<usrID;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	case QueryRename:
		in<<AnswerRename<<usrID;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	case QueryNewDirectory:
		in<<AnswerNewDirectory<<usrID;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	case QueryDelete:
		in<<AnswerDelete<<usrID;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	default:
		break;
	}

	sender->sendMessage(buf, Type);
}

QMessageFileTransferRunnable::QMessageFileTransferRunnable(QConnection* sender, const QByteArray& buffer, 
														   QObject* view, QMessageFileTransfer* message, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view, message);
}

QMessageFileTransferRunnable::~QMessageFileTransferRunnable()
{

}

void QMessageFileTransferRunnable::setArg(QConnection* sender, const QByteArray& buffer, QObject* view, QMessageFileTransfer* message)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
	m_pMessage=message;
}

void QMessageFileTransferRunnable::run()
{
	QDataStream out(m_buffer);
	out.setVersion(DATA_STREAM_VERSION);

	int nMsgType;
	quint32 nUsrID;
	out>>nMsgType>>nUsrID;

	switch (nMsgType)
	{
	case QMessageFileTransfer::QueryRoot:
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			stream<<getAppParentDir();
			emit finished(m_pSender, QMessageFileTransfer::QueryRoot, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfer::AnswerRoot:
		{
			QString strRoot;
			out>>strRoot;

			QMetaObject::invokeMethod(m_pView, "recvRoot", Q_ARG(quint32, nUsrID), Q_ARG(QString, strRoot));
		}
		break;
	case QMessageFileTransfer::QueryPath:
		{
			QString strPath;
			out>>strPath;

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			QFileInfo fileInfo(strPath);
			if (fileInfo.isSymLink())
			{
				strPath=fileInfo.symLinkTarget();
			}

			stream<<strPath;

			QDir dir(strPath);
			if (!dir.exists())
			{
				stream<<QMessageFileTransfer::QueryPathNoExist;
			}
			else
			{
				stream<<QMessageFileTransfer::QueryPathValid;

				int nSortFlag;
				out>>nSortFlag;

				quint32 sort=QDir::IgnoreCase;
				switch (nSortFlag&0x0f)
				{
				case QMessageFileTransfer::SortName:
					sort|=QDir::Name;
					break;
				case QMessageFileTransfer::SortTime:
					sort|=QDir::Time;
					break;
				case QMessageFileTransfer::SortSize:
					sort|=QDir::Size;
					break;
				case QMessageFileTransfer::SortType:
					sort|=QDir::Type;
					break;
				default:
					sort|=QDir::NoSort;
					break;
				}

				if ((nSortFlag & 0x10) != 0)
				{
					sort|=QDir::Reversed;
				}

				QVector<FileInfo> vecFileInfo;

				QFileInfoList list=dir.entryInfoList(QDir::AllEntries | QDir::System, (QDir::SortFlags)sort);
				entryInfoList(list, vecFileInfo);

				stream<<vecFileInfo.size();
				foreach (FileInfo fileInfo, vecFileInfo)
				{
					stream<<fileInfo;
				}
			}

			emit finished(m_pSender, QMessageFileTransfer::QueryPath, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfer::QueryDrive:
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			QFileInfoList list=QDir::drives();

			stream<<list.size();
			foreach (QFileInfo fileInfo, list)
			{
				stream<<QMessageFileTransfer::Drive<<fileInfo.filePath()<<(quint64)0<<fileInfo.isReadable()
					<<fileInfo.isWritable()<<false<<fileInfo.lastModified();
			}

			emit finished(m_pSender, QMessageFileTransfer::QueryDrive, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfer::AnswerPath:
	case QMessageFileTransfer::AnswerDir:
		{
			QString strPath;

			int nQueryFlag;
			out>>strPath>>nQueryFlag;

			Q_ASSERT((nQueryFlag == QMessageFileTransfer::QueryPathNoExist) || (nQueryFlag == QMessageFileTransfer::QueryPathValid));

			QVector<FileInfo> vecInfo;
			if (nQueryFlag == QMessageFileTransfer::QueryPathValid)
			{
				FileInfo info;
				int nNum;
				out>>nNum;
				for (int i=0; i<nNum; i++)
				{
					out>>info;

					vecInfo.append(info);
				}
			}

			if (nMsgType == QMessageFileTransfer::AnswerPath)
			{
				QMetaObject::invokeMethod(m_pView, "recvPath", Q_ARG(quint32, nUsrID), Q_ARG(QString, strPath), 
					Q_ARG(int, nQueryFlag), Q_ARG(QVector<FileInfo>, vecInfo));
			}
			else
			{
				QMetaObject::invokeMethod(m_pView, "recvDir", Q_ARG(quint32, nUsrID), Q_ARG(QString, strPath),
					Q_ARG(int, nQueryFlag), Q_ARG(QVector<FileInfo>, vecInfo));
			}
		}
		break;
	case QMessageFileTransfer::AnswerDrive:
		{
			QVector<FileInfo> vecInfo;
			FileInfo info;
			int nNum;
			out>>nNum;
			for (int i=0; i<nNum; i++)
			{
				out>>info;

				vecInfo.append(info);
			}

			QMetaObject::invokeMethod(m_pView, "recvDrive", Q_ARG(quint32, nUsrID), Q_ARG(QVector<FileInfo>, vecInfo));
		}
		break;
	case QMessageFileTransfer::QueryFile:
		{
			QString strFile;
			out>>strFile;

			Q_ASSERT(!strFile.isEmpty());

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			QFileInfo fileInfo(strFile);
			if (!fileInfo.exists())
			{
				stream<<QMessageFileTransfer::QueryFileNoExist;
			}
			else if (!fileInfo.isReadable())
			{
				stream<<QMessageFileTransfer::QueryFileUnreadable;
			}
			else
			{
				stream<<QMessageFileTransfer::QueryFileValid<<fileInfo.size();
			}
			
			QMetaObject::invokeMethod(m_pMessage, "removeTransferFile", 
				Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID));

			emit finished(m_pSender, QMessageFileTransfer::QueryFile, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfer::AnswerFileInfo:
		{
			int nQueryFlag;
			out>>nQueryFlag;

			Q_ASSERT(nQueryFlag==QMessageFileTransfer::QueryFileNoExist || nQueryFlag==QMessageFileTransfer::QueryFileUnreadable
				|| nQueryFlag==QMessageFileTransfer::QueryFileValid);

			qint64 nFileSize=-1;
			if (nQueryFlag == QMessageFileTransfer::QueryFileValid)
			{
				out>>nFileSize;
			}

			QMetaObject::invokeMethod(m_pView, "recvFileInfo", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag), Q_ARG(qint64, nFileSize));
		}
		break;
	case QMessageFileTransfer::QueryTransferFile:
		{
			QString strFile;
			int nFormat;
			out>>strFile>>nFormat;

			Q_ASSERT(nFormat == QMessageFileTransfer::TextFormat || nFormat == QMessageFileTransfer::BinaryFormat);

			if (m_pMessage != NULL)
			{
				QByteArray buffer;
				int nStatus=QMessageFileTransfer::TransferReadError;

				QFile* file=m_pMessage->transferFile(m_pSender, nUsrID);
				if (file == NULL)
				{
					if (m_pMessage != NULL)
					{
						file=new QFile(strFile);
						QIODevice::OpenMode openMode=QIODevice::ReadOnly;
						if (nFormat == QMessageFileTransfer::TextFormat)
						{
							openMode|=QIODevice::Text;
						}

						file->open(openMode);

						QMetaObject::invokeMethod(m_pMessage, "addTransferFile", 
							Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID), Q_ARG(QFile*, file));
					}
				}

				if (file != NULL)
				{
					nStatus=readContents(file, buffer);

					if (nStatus == QMessageFileTransfer::TransferFinish)
					{
						QMetaObject::invokeMethod(m_pMessage, "removeTransferFile", 
							Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID));
					}
				}

				emit finished(m_pSender, QMessageFileTransfer::QueryTransferFile, nUsrID, buffer, nStatus);
			}
		}
		break;
	case QMessageFileTransfer::QueryRetransmitFile:
		{
			QString strFile;
			int nFormat;
			quint64 nPos;
			out>>strFile>>nFormat>>nPos;

			Q_ASSERT(nFormat==QMessageFileTransfer::TextFormat || nFormat==QMessageFileTransfer::BinaryFormat);

			if (m_pMessage != NULL)
			{
				QByteArray buffer;
				int nStatus=QMessageFileTransfer::TransferReadError;

				QFile* file=new QFile(strFile);

				if (file != NULL)
				{
					QIODevice::OpenMode openMode=QIODevice::ReadOnly;
					if (nFormat == QMessageFileTransfer::TextFormat)
					{
						openMode|=QIODevice::Text;
					}

					file->open(openMode);

					QMetaObject::invokeMethod(m_pMessage, "addTransferFile", 
						Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID), Q_ARG(QFile*, file));

					file->seek(nPos);

					nStatus=readContents(file, buffer);
					if (nStatus == QMessageFileTransfer::TransferFinish)
					{
						QMetaObject::invokeMethod(m_pMessage, "removeTransferFile", 
							Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID));
					}
				}

				emit finished(m_pSender, QMessageFileTransfer::QueryTransferFile, nUsrID, buffer, nStatus);
			}
		}
		break;
	case QMessageFileTransfer::AnswerTransferFile:
		{
			int nStatus;
			QByteArray buffer;

			out>>nStatus;

			if (nStatus == QMessageFileTransfer::TransferNormal)
			{
				int nSize;
				out>>nSize;

				Q_ASSERT(nSize>=0);
				buffer.resize(nSize);

				out.readRawData(buffer.data(), nSize);
			}

			QMetaObject::invokeMethod(m_pView, "recvFileData", Q_ARG(quint32, nUsrID), Q_ARG(int, nStatus),
				Q_ARG(QByteArray, buffer));
		}
		break;
	case QMessageFileTransfer::QueryDir:
		{
			QString strDir;
			out>>strDir;

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);

			stream<<strDir;

			QDir dir(strDir);
			if (!dir.exists())
			{
				stream<<QMessageFileTransfer::QueryPathNoExist;
			}
			else
			{
				stream<<QMessageFileTransfer::QueryPathValid;

				QVector<FileInfo> vecFileInfo;

				QFileInfoList list=dir.entryInfoList(QDir::AllEntries | QDir::System, QDir::DirsFirst|QDir::Name|QDir::IgnoreCase);
				entryInfoList(list, vecFileInfo);

				stream<<vecFileInfo.size();
				foreach (FileInfo fileInfo, vecFileInfo)
				{
					stream<<fileInfo;
				}
			}

			emit finished(m_pSender, QMessageFileTransfer::QueryDir, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfer::QuerySymLink:
		{
			QString strFile;
			out>>strFile;

			Q_ASSERT(!strFile.isEmpty());

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			QFileInfo fileInfo(strFile);
			if (!fileInfo.exists())
			{
				stream<<QMessageFileTransfer::QueryFileNoExist;
			}
			else if (!fileInfo.isReadable())
			{
				stream<<QMessageFileTransfer::QueryFileUnreadable;
			}
			else if (!fileInfo.isSymLink())
			{
				stream<<QMessageFileTransfer::QuerySymLinkNoLink;
			}
			else
			{
				FileInfo info=entryInfo(fileInfo);
				
				stream<<QMessageFileTransfer::QueryFileValid<<info<<fileInfo.symLinkTarget();
			}

			emit finished(m_pSender, QMessageFileTransfer::QuerySymLink, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfer::AnswerSymLink:
		{
			int nQueryFlag;
			out>>nQueryFlag;

			Q_ASSERT(nQueryFlag==QMessageFileTransfer::QueryFileNoExist || nQueryFlag==QMessageFileTransfer::QueryFileUnreadable
				|| nQueryFlag==QMessageFileTransfer::QuerySymLinkNoLink || nQueryFlag==QMessageFileTransfer::QueryFileValid);

			QString strSymLink;
			FileInfo info;
			if (nQueryFlag == QMessageFileTransfer::QueryFileValid)
			{
				out>>info>>strSymLink;
			}

			QMetaObject::invokeMethod(m_pView, "recvSymLinkInfo", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag), 
				Q_ARG(FileInfo, info), Q_ARG(QString, strSymLink));
		}
		break;
	case QMessageFileTransfer::QueryUpload:
		{
			int nFileType;
			QString strAddr;

			out>>nFileType>>strAddr;

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			int nFlag;
			switch (nFileType)
			{
			case QMessageFileTransfer::Dir:
			case QMessageFileTransfer::HideDir:
				{
					nFlag=QMessageFileTransfer::QueryMakePathFail;
					QDir dir(strAddr);
					bool bRtn=dir.mkpath(strAddr);
					if (bRtn)
					{
						nFlag=QMessageFileTransfer::QueryMakePathSuccess;
					}
				}
				break;
			case QMessageFileTransfer::File:
			case QMessageFileTransfer::HideFile:
				{
					int nFormat;
					out>>nFormat;

					nFlag=QMessageFileTransfer::QueryCreateFileFail;
					strAddr+=TEMP_FILE_SUFFIX;

					QFile* file=new QFile(strAddr);
					QIODevice::OpenMode openMode=QIODevice::WriteOnly;
					if (nFormat == QMessageFileTransfer::TextFormat)
					{
						openMode|=QIODevice::Text;
					}
					if (file->open(openMode))
					{
						nFlag=QMessageFileTransfer::QueryCreateFileSuccess;
					}
					
					QMetaObject::invokeMethod(m_pMessage, "addTransferFile", 
						Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID), Q_ARG(QFile*, file));
				}
				break;
			case QMessageFileTransfer::SymLinkDir:
			case QMessageFileTransfer::SymLinkHideDir:
			case QMessageFileTransfer::SymLinkFile:
			case QMessageFileTransfer::SymLinkHideFile:
				{
					nFlag=QMessageFileTransfer::QueryUploadSymLinkFail;

					QString strTarget;
					out>>strTarget;

					if (QFile::link(strTarget, strAddr))
					{
						nFlag=QMessageFileTransfer::QueryUploadSymLinkSuccess;
					}
				}
				break;
			default:
				nFlag=QMessageFileTransfer::QueryUnknow;
				break;
			}


			stream<<nFlag;
			emit finished(m_pSender, QMessageFileTransfer::QueryUpload, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfer::AnswerUpload:
		{
			int nFlag;
			out>>nFlag;

			QMetaObject::invokeMethod(m_pView, "recvUpload", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag));
		}
		break;
	case QMessageFileTransfer::QueryUploadFile:
		{
			int nFlag;
			out>>nFlag;

			QFile* pFile=m_pMessage->transferFile(m_pSender, nUsrID);
			if (pFile != NULL)
			{
				switch (nFlag)
				{
				case QMessageFileTransfer::TransferNormal:
					{
						int nSize;
						out>>nSize;

						QByteArray buffer;

						Q_ASSERT(nSize>=0);
						buffer.resize(nSize);

						out.readRawData(buffer.data(), nSize);

						nFlag=QMessageFileTransfer::QueryUploadFileWriteSuccess;
						quint64 nWriteSize=pFile->write(buffer);
						if (nWriteSize != buffer.size())
						{
							nFlag=QMessageFileTransfer::QueryUploadFileWriteFail;

							QMetaObject::invokeMethod(m_pMessage, "removeTransferFile", 
								Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID));
						}
					}
					break;
				case QMessageFileTransfer::TransferReadError:
					{
						nFlag=QMessageFileTransfer::QueryUploadFileWriteFail;

						QMetaObject::invokeMethod(m_pMessage, "removeTransferFile", 
							Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID));
					}
					break;
				case QMessageFileTransfer::TransferFinish:
					{
						nFlag=QMessageFileTransfer::QueryUploadFileSuccess;

						QString strFile=pFile->fileName();
						strFile=strFile.left(strFile.size()-QString(TEMP_FILE_SUFFIX).size());

						if (QFile::exists(strFile))
						{
							QFile::remove(strFile);
						}

						if (!pFile->rename(strFile))
						{
							nFlag=QMessageFileTransfer::QueryUploadFileFail;
						}

						QMetaObject::invokeMethod(m_pMessage, "removeTransferFile", 
							Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID));
					}
					break;
				default:
					break;
				}
			}
			else
			{
				nFlag = QMessageFileTransfer::QueryUploadFileWriteLost;
			}

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			stream<<nFlag;
			emit finished(m_pSender, QMessageFileTransfer::QueryUploadFile, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfer::AnswerUploadFile:
		{
			int nFlag;
			out>>nFlag;

			QMetaObject::invokeMethod(m_pView, "recvUploadFile", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag));
		}
		break;
	case QMessageFileTransfer::QueryRename:
		{
			QString strLocation;
			int nRow;
			QString strOld,strNow;

			out>>strLocation>>nRow>>strOld>>strNow;

			int nFlag=QMessageFileTransfer::RenamePathLost;
			QDir dir(appendPath(strLocation, strOld));
			if (dir.exists())
			{
				nFlag=QMessageFileTransfer::RenameFail;
				if (dir.rename(appendPath(strLocation, strOld), appendPath(strLocation, strNow)))
				{
					nFlag=QMessageFileTransfer::RenameSuccess;
				}
			}

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			stream<<nRow<<strOld<<strNow<<nFlag;

			emit finished(m_pSender, QMessageFileTransfer::QueryRename, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfer::AnswerRename:
		{
			int nRow;
			QString strOld,strNow;
			int nFlag;

			out>>nRow>>strOld>>strNow>>nFlag;

			QMetaObject::invokeMethod(m_pView, "recvRename", Q_ARG(quint32, nUsrID), Q_ARG(int, nRow),
				Q_ARG(QString, strOld), Q_ARG(QString, strNow), Q_ARG(int, nFlag));
		}
		break;
	case QMessageFileTransfer::QueryNewDirectory:
		{
			QString strLocation;
			out>>strLocation;

			QDir dir(strLocation);
			QString strPath;

			int i=1;
			while (true)
			{
				strPath=appendPath(strLocation, QString(tr("New Directory %1")).arg(i));
				if (!dir.exists(strPath))
				{
					break;
				}

				i++;
			}

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			if (dir.mkpath(strPath))
			{
				stream<<QMessageFileTransfer::NewSuccess<<entryInfo(QFileInfo(strPath));
			}
			else
			{
				stream<<QMessageFileTransfer::NewFail;
			}

			emit finished(m_pSender, QMessageFileTransfer::QueryNewDirectory, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfer::AnswerNewDirectory:
		{
			int nFlag;
			out>>nFlag;
			FileInfo file;
			if (nFlag == QMessageFileTransfer::NewSuccess)
			{
				out>>file;
			}

			QMetaObject::invokeMethod(m_pView, "recvNewDirectory", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag), Q_ARG(FileInfo, file));
		}
		break;
	case QMessageFileTransfer::QueryDelete:
		{
			QString strLocation;
			int nNum;
			out>>strLocation>>nNum;

			int nFlag=QMessageFileTransfer::DelSuccess;

			QString strName;
			bool bSuccess=true;
			for (int i=0; i<nNum; i++)
			{
				out>>strName;
				strName=appendPath(strLocation, strName);

				QFileInfo info(strName);
				if (info.exists())
				{
					if (info.isSymLink() || info.isFile())
					{
						bSuccess&=QFile::remove(strName);
					}
					else if (info.isDir())
					{
						bSuccess&=deleteDir(strName);
					}
				}
			}

			if (!bSuccess)
			{
				nFlag=QMessageFileTransfer::DelFail;
			}

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			stream<<nFlag;

			emit finished(m_pSender, QMessageFileTransfer::QueryDelete, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfer::AnswerDelete:
		{
			int nFlag;
			out>>nFlag;

			QMetaObject::invokeMethod(m_pView, "recvDelete", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag));
		}
		break;
	default:
		break;
	}
}

bool QMessageFileTransferRunnable::deleteDir(const QString& path)
{
	bool bSuccess=true;

	QDir dir(path);
	if (dir.exists())
	{
		QFileInfoList list=dir.entryInfoList(QDir::AllEntries | QDir::System | QDir::NoDotAndDotDot);
		foreach (QFileInfo info, list)
		{
			if (info.isSymLink() || info.isFile())
			{
				bSuccess&=QFile::remove(info.absoluteFilePath());
			}
			else if (info.isDir())
			{
				bSuccess&=deleteDir(info.absoluteFilePath());
			}
		}

		bSuccess&=dir.rmdir(path);
	}

	return bSuccess;
}

void QMessageFileTransferRunnable::entryInfoList(QFileInfoList& list, QVector<FileInfo>& files)
{
	foreach (QFileInfo info, list)
	{
		files.append(entryInfo(info));
	}
}

QDataStream& operator<<(QDataStream& out, const FileInfo& info)
{
	out<<info.type<<info.name<<info.size<<info.isReadable<<info.isWritable<<info.isExecutable<<info.lastModified;

	return out;
}

QDataStream& operator>>(QDataStream& in, FileInfo& info)
{
	in>>info.type>>info.name>>info.size>>info.isReadable>>info.isWritable>>info.isExecutable>>info.lastModified;

	return in;
}

int QMessageFileTransferRunnable::readContents(QFile* file, QByteArray& buffer)
{
	Q_ASSERT_NO_NULL(file);

	int nStatus=QMessageFileTransfer::TransferReadError;

	buffer.resize(FILE_PACKET_SIZE);
	int nSize=file->read(buffer.data(), FILE_PACKET_SIZE);
	if (nSize != -1)
	{
		if (nSize != FILE_PACKET_SIZE)
		{
			buffer.resize(nSize);
		}

		if (nSize == 0)
		{
			nStatus=QMessageFileTransfer::TransferFinish;
		}
		else
		{
			nStatus=QMessageFileTransfer::TransferNormal;
		}
	}
	else
	{
		buffer.clear();
	}

	return nStatus;
}
