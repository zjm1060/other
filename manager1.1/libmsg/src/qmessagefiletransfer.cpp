#include "qmessagefiletransfer.h"

#include <QFileInfo>

#include "common.h"

#include "qmibfactory.h"
#include "qutildef.h"
#include "qmessageparser.h"
#include "qconnectionmanager.h"
#include "qdirex.h"
#include "qutilreadwritexmlfile.h"
#include "qabstractfilecompare.h"
#include "qcscsdbscode.h"
#include "qlogger.h"

#define FILE_TRANSFER_TICK_TIMES	30
#define FILE_TRANSFER_FLOW_CTRL_TICK	2

QMessageFileTransfer::QMessageFileTransfer(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<QVector<FileInfo> >("QVector<FileInfo>");
	qRegisterMetaType<FileInfo>("FileInfo");
	qRegisterMetaType<QVector<QString> >("QVector<QString>");
}

QMessageFileTransfer::~QMessageFileTransfer()
{

}

QByteArray QMessageFileTransfer::query(int ackType, quint32 usrID, const QByteArray& contents)
{
	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<ackType<<usrID;

	if (contents.size() > 0)
	{
		in.writeRawData(contents.data(), contents.size());
	}

	return buffer;
}

bool QMessageFileTransfer::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageFileTransferManager::instance();	//初始化，设置timer所在线程

	QMessageFileTransferRunnable* pRunnable=new QMessageFileTransferRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QObject*, int, quint32, QByteArray)), 
		this, SLOT(finished(QObject*, int, quint32, QByteArray)));
	connect(pRunnable, SIGNAL(download(QObject*, quint32, QObject*)), this, SLOT(download(QObject*, quint32, QObject*)));
	connect(pRunnable, SIGNAL(upload(QObject*, quint32, QObject*)), this, SLOT(upload(QObject*, quint32, QObject*)));
	connect(pRunnable, SIGNAL(slientUpload(QObject*, quint32, QString, int, QObject*)), this, 
		SLOT(slientUpload(QObject*, quint32, QString, int, QObject*)), Qt::QueuedConnection);

	globalThreadPool()->start(pRunnable, 3);

// 	pRunnable->run();
// 	SAFE_DELETE_CLASS(pRunnable);

	return true;
}

void QMessageFileTransfer::finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer)
{
	if (sender == NULL)
	{
		return;
	}

	Q_WRITE_ONLY_DATA_STREAM(in, buf)

	switch (ackType)
	{
	case QueryRoot:
		in<<AnswerRoot;
		break;
	case QueryPath:
		in<<AnswerPath;
		break;
	case QueryDrive:
		in<<AnswerDrive;
		break;
	case QuerySymLink:
		in<<AnswerSymLink;
		break;
	case QueryFileInfo:
		in<<AnswerFileInfo;
		break;
	case QueryDownloadDir:
		in<<AnswerDownloadDir;
		break;
	case QueryDownloadFile:
		in<<AnswerDownloadFileInfo;
		break;
	case QueryTransferFile:
		in<<AnswerTransferFile;
		break;
	case QueryResumeDownloadFile:
		in<<AnswerResumeDownloadFile;
		break;
	case QueryDownloadSymLink:
		in<<AnswerDownloadSymLink;
		break;
	case QueryDownloadFlowControl:
		in<<AnswerDownloadFlowControl;
		break;
	case QueryUploadDir:
		in<<AnswerUploadDir;
		break;
	case QueryUploadFile:
		in<<AnswerUploadFile;
		break;
	case QueryUploadFileContent:
		in<<AnswerUploadFileContent;
		break;
	case QueryUploadSymlink:
		in<<AnswerUploadSymlink;
		break;
	case QueryUploadFlowControl:
		in<<AnswerUploadFlowControl;
		break;
	case QueryRename:
		in<<AnswerRename;
		break;
	case QueryNewDirectory:
		in<<AnswerNewDirectory;
		break;
	case QueryDelete:
		in<<AnswerDelete;
		break;
	case QueryInterruptTransfer:
		in<<AnswerInterruptTransfer;
		break;
	case QueryUploadSlientFileContent:
		in<<AnswerUploadSlientFileContent;
		break;
	case QueryBackup:
		in<<AnswerBackup;
		break;
	default:
		break;
	}

	in<<usrID;

	if (!buffer.isEmpty())
	{
		in.writeRawData(buffer.data(), buffer.size());
	}

	QObjectPtr ptr=QConnectionManager::instance()->connection(sender);
	if (ptr != NULL)
	{
		QMetaObject::invokeMethod(sender, "sendMessage", Qt::DirectConnection, Q_ARG(QByteArray, buf), Q_ARG(int, Type));
	}

	QMessageFileTransferManager::instance()->check();
}

void QMessageFileTransfer::download(QObject* sender, quint32 usrID, QObject* view)
{
	Q_WRITE_ONLY_DATA_STREAM(stream, buffer)
	stream<<QueryFileWriteSuccess;

	QIMessagePtr pMessage=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMessage != NULL)
	{
		buffer=pMessage->query(QueryTransferFile, usrID, buffer);

		QObjectPtr ptr=QConnectionManager::instance()->connection(sender);
		if (ptr != NULL)
		{
			parse(sender, buffer, view);
		}
	}
}

void QMessageFileTransfer::upload(QObject* sender, quint32 usrID, QObject* view)
{
	Q_WRITE_ONLY_DATA_STREAM(stream, buffer)
	stream<<QueryFileWriteSuccess;

	QIMessagePtr pMessage=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMessage != NULL)
	{
		buffer=pMessage->query(AnswerUploadFileContent, usrID, buffer);

		QObjectPtr ptr=QConnectionManager::instance()->connection(sender);
		if (ptr != NULL)
		{
			parse(sender, buffer, view);
		}
	}
}

void QMessageFileTransfer::slientUpload(QObject* sender, quint32 usrID, QString save, int format, QObject* view)
{
	Q_WRITE_ONLY_DATA_STREAM(stream, buffer)
	stream<<save<<format;

	QIMessagePtr pMessage=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMessage != NULL)
	{
		buffer=pMessage->query(QueryUploadSlientFileContent, usrID, buffer);

		QObjectPtr ptr=QConnectionManager::instance()->connection(sender);
		if (ptr != NULL)
		{
			parse(sender, buffer, view);
		}
	}
}

QMessageFileTransferRunnable::QMessageFileTransferRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageFileTransferRunnable::~QMessageFileTransferRunnable()
{

}

void QMessageFileTransferRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageFileTransferRunnable::run()
{
	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	int nAckType;
	quint32 nUsrID;
	out>>nAckType>>nUsrID;

	switch (nAckType)
	{
	case QueryRoot:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			stream<<getAppParentDir();
			emit finished(m_pSender, QueryRoot, nUsrID, buffer);
		}
		break;
	case AnswerRoot:
		{
			QString strRoot;
			out>>strRoot;

			QMetaObject::invokeMethod(m_pView, "recvRoot", Q_ARG(quint32, nUsrID), Q_ARG(QString, strRoot));
		}
		break;
	case QueryPath:
		{
			QString strPath;
			out>>strPath;

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QFileInfo fileInfo(strPath);
			if (fileInfo.isSymLink())
			{
				strPath=fileInfo.symLinkTarget();
			}

			stream<<strPath;

			QDir dir(strPath);
			if (!dir.exists())
			{
				stream<<QueryPathNoExist;
			}
			else
			{
				stream<<QueryPathValid;

				int nSortFlag;
				out>>nSortFlag;

				quint32 sort=QDir::IgnoreCase|QDir::DirsFirst;
				switch (nSortFlag&0x0f)
				{
				case SortName:
					sort|=QDir::Name;
					break;
				case SortTime:
					sort|=QDir::Time;
					break;
				case SortSize:
					sort|=QDir::Size;
					break;
				case SortType:
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

				QStringList		nameFilters;
				int	filters;

				out>>nameFilters>>filters;

				QVector<FileInfo> vecFileInfo;

				QFileInfoList list=dir.entryInfoList(nameFilters, (QDir::Filters)filters, (QDir::SortFlags)sort);
				entryInfoList(list, vecFileInfo, false);

				stream<<vecFileInfo;
			}

			emit finished(m_pSender, QueryPath, nUsrID, buffer);
		}
		break;
	case AnswerPath:
		{
			QString strPath;
			int nQueryFlag;

			out>>strPath>>nQueryFlag;

			Q_ASSERT((nQueryFlag == QueryPathNoExist) || (nQueryFlag == QueryPathValid));

			QVector<FileInfo> vecInfo;
			if (nQueryFlag == QueryPathValid)
			{
				out>>vecInfo;
			}

			QMetaObject::invokeMethod(m_pView, "recvPath", Q_ARG(quint32, nUsrID), Q_ARG(QString, strPath), 
				Q_ARG(int, nQueryFlag), Q_ARG(QVector<FileInfo>, vecInfo));
		}
		break;
	case QueryDrive:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QFileInfoList list=QDir::drives();

			QVector<FileInfo> vecFileInfo;
			entryInfoList(list, vecFileInfo, true);
			
			stream<<vecFileInfo;

			emit finished(m_pSender, QueryDrive, nUsrID, buffer);
		}
		break;
	case AnswerDrive:
		{
			QVector<FileInfo> vecInfo;
			out>>vecInfo;

			QMetaObject::invokeMethod(m_pView, "recvDrives", Q_ARG(quint32, nUsrID), Q_ARG(QVector<FileInfo>, vecInfo));
		}
		break;
	case QuerySymLink:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QString strFile;
			out>>strFile;

			QFileInfo info(strFile);
			if (!info.exists())
			{
				stream<<QueryFileNoExist;
			}
			else if (!info.isReadable())
			{
				stream<<QueryFileUnreadable;
			}
			else if (!info.isSymLink())
			{
				stream<<QueryFileNoSymLink;
			}
			else
			{
				stream<<QueryFileValid<<entryInfo(info, false)<<info.symLinkTarget();
			}

			emit finished(m_pSender, QuerySymLink, nUsrID, buffer);
		}
		break;
	case AnswerSymLink:
		{
			int nQueryFlag;
			QString strTargetPath;
			FileInfo info;

			out>>nQueryFlag;

			if (nQueryFlag == QueryFileValid)
			{
				out>>info>>strTargetPath;
			}

			QMetaObject::invokeMethod(m_pView, "recvSymLink", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag),
				Q_ARG(FileInfo, info), Q_ARG(QString, strTargetPath));
		}
		break;
	case QueryDownloadDir:
		{
			QString strDir;
			QString strSaveDir;
			out>>strDir>>strSaveDir;

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			stream<<strDir;

			QDir dir(strDir);
			if (!dir.exists())
			{
				stream<<QueryDirNoExist;
			}
			else
			{
				stream<<QueryDirValid<<strSaveDir;

				QVector<FileInfo> vecFileInfo;

				QFileInfoList list=dir.entryInfoList(QDir::AllEntries|QDir::System|QDir::NoDotAndDotDot, QDir::DirsFirst|QDir::Name|QDir::IgnoreCase);
				entryInfoList(list, vecFileInfo, false);

				stream<<vecFileInfo;
			}

			emit finished(m_pSender, QueryDownloadDir, nUsrID, buffer);
		}
		break;
	case AnswerDownloadDir:
		{
			QString strDir;
			QString strSaveDir;
			int nQueryFlag;
			out>>strDir>>nQueryFlag;

			QVector<FileInfo> vecInfo;
			int nFlag;

			Q_ASSERT(nQueryFlag==QueryDirNoExist || nQueryFlag==QueryDirValid);

			if (nQueryFlag == QueryDirValid)
			{
				out>>strSaveDir>>vecInfo;

				nFlag=QueryMakePathSuccess;
				QDirEx dir;
				if (!dir.mkpath(strSaveDir))
				{
					nFlag=QueryDirMakePathFailed;
				}
			}
			QMetaObject::invokeMethod(m_pView, "recvDownloadDir", Q_ARG(quint32, nUsrID), Q_ARG(QString, strDir),
				Q_ARG(int, nQueryFlag), Q_ARG(QString, strSaveDir), Q_ARG(int, nFlag), Q_ARG(QVector<FileInfo>, vecInfo));
		}
		break;
	case QueryFileInfo:
		{
			QString strFile;

			out>>strFile;

			Q_ASSERT(!strFile.isEmpty());

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			stream<<strFile;

			QFileInfo fileInfo(strFile);
			if (!fileInfo.exists())
			{
				stream<<QueryFileNoExist;
			}
			else
			{
				FileInfo info=entryInfo(fileInfo, false);
				stream<<QueryFileValid<<info;

				QString strFilePath=fileInfo.absolutePath();
				QString strBaseName=fileInfo.baseName();
				QString strSuffix=fileInfo.completeSuffix();
				int i=0;
				while (QFile::exists(strFile=appendPath(strFilePath, QString("%1[%2].%3").arg(strBaseName).arg(i).arg(strSuffix))))
				{
					i++;
				}
				stream<<strFile;
			}

			emit finished(m_pSender, QueryFileInfo, nUsrID, buffer);
		}
		break;
	case AnswerFileInfo:
		{
			QString strFile;
			int nQueryFalg;

			FileInfo info;
			QString strRename;

			out>>strFile>>nQueryFalg;

			Q_ASSERT(nQueryFalg==QueryFileNoExist || nQueryFalg==QueryFileValid);

			if (nQueryFalg == QueryFileValid)
			{
				out>>info>>strRename;
			}

			QMetaObject::invokeMethod(m_pView, "recvFileInfo", Q_ARG(quint32, nUsrID), Q_ARG(QString, strFile),
				Q_ARG(int, nQueryFalg), Q_ARG(FileInfo, info), Q_ARG(QString, strRename));
		}
		break;
	case QueryDownloadFile:
		{
			QString strFile;
			QString strSaveFile;
			int nFormat;

			out>>strFile>>nFormat>>strSaveFile;

			Q_ASSERT(!strFile.isEmpty());

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			stream<<strFile;

			QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);

			QFileInfo fileInfo(strFile);
			if (!fileInfo.exists())
			{
				stream<<QueryFileNoExist;
			}
			else if (!fileInfo.isReadable())
			{
				stream<<QueryFileUnreadable;
			}
			else
			{
				Q_ASSERT(nFormat == TextFormat || nFormat == BinaryFormat);

				QSharedFilePtr pFile(new QSharedFile(strFile));
				QIODevice::OpenMode openMode=QIODevice::ReadOnly;
				if (nFormat == TextFormat)
				{
					openMode|=QIODevice::Text;
				}

				if (pFile->open(openMode))
				{
					stream<<QueryFileValid<<strSaveFile<<nFormat<<fileInfo.size();
					QMessageFileTransferManager::instance()->addTransferFile(m_pSender, nUsrID, pFile);
				}
				else
				{
					stream<<QueryFileOpenFail;
				}
			}

			emit finished(m_pSender, QueryDownloadFile, nUsrID, buffer);
		}
		break;
	case AnswerDownloadFileInfo:
		{
			QString strFile;
			int nQueryFlag;

			out>>strFile>>nQueryFlag;

			Q_ASSERT(nQueryFlag==QueryFileNoExist || nQueryFlag==QueryFileUnreadable || nQueryFlag==QueryFileValid ||
				nQueryFlag==QueryFileOpenFail);

			QString strSaveFile;
			int nFormat;
			qint64 nFileSize=-1;
			int nFlag=QueryFileCreateFail;
			if (nQueryFlag == QueryFileValid)
			{
				out>>strSaveFile>>nFormat>>nFileSize;

				strSaveFile+=TEMP_FILE_SUFFIX;

				QSharedFilePtr pFile(new QSharedFile(strSaveFile));
				QIODevice::OpenMode openMode=QIODevice::WriteOnly;
				if (nFormat == TextFormat)
				{
					openMode|=QIODevice::Text;
				}

				Q_WRITE_ONLY_DATA_STREAM(stream, buffer)
				if (pFile->open(openMode))
				{
					nFlag=QueryFileCreateSuccess;

					QMessageFileTransferManager::instance()->addTransferFile(m_pSender, nUsrID, pFile);
				}
				
				stream<<nFlag;

				QIMessagePtr pMessage=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
				if (pMessage != NULL)
				{
					buffer=pMessage->query(QueryTransferFile, nUsrID, buffer);

					QObjectPtr ptr=QConnectionManager::instance()->connection(m_pSender);
					if (ptr != NULL)
					{
						QMetaObject::invokeMethod(m_pSender, "sendMessage", Q_ARG(QByteArray, buffer), Q_ARG(int, MSG_FILE_TRANSFER));
					}
				}
			}

			QMetaObject::invokeMethod(m_pView, "recvDownloadFileInfo", Q_ARG(quint32, nUsrID), Q_ARG(QString, strFile),
				Q_ARG(int, nQueryFlag), Q_ARG(QString, strSaveFile), Q_ARG(int, nFormat), Q_ARG(qint64, nFileSize), Q_ARG(int, nFlag));
		}
		break;
	case QueryTransferFile:
		{
			int nQueryFlag;
			int nFlag;
			out>>nQueryFlag;

			Q_ASSERT(nQueryFlag==QueryFileCreateSuccess || nQueryFlag==QueryFileCreateFail || nQueryFlag==QueryFileWriteLost ||
				nQueryFlag==QueryFileWriteSuccess || nQueryFlag==QueryFileWriteFail || nQueryFlag==QueryFileOpenFail ||
				nQueryFlag==QueryFileSeekFail);

			if (nQueryFlag==QueryFileCreateSuccess || nQueryFlag == QueryFileWriteSuccess)
			{
				QByteArray buf;
				nFlag=QueryFileReadLost;

				Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

				QSharedFilePtr pFile=QMessageFileTransferManager::instance()->transferFile(m_pSender, nUsrID);
				if (pFile != NULL)
				{
					nFlag=readContents(pFile.get(), buf, QConnectionManager::instance()->canHighRate(m_pSender));

					stream<<nFlag;

					if (nFlag == QueryFileReadSuccess)
					{
						stream<<pFile->nextBlock()<<buf.size();

						stream.writeRawData(buf.data(), buf.size());
					}
					else if (nFlag == QueryFileReadFinish)
					{
						stream<<pFile->nextBlock()<<(quint32)pFile->permissions();
					}

					if (nFlag == QueryFileReadFinish || nFlag == QueryFileReadFail)
					{
						QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
					}

					emit finished(m_pSender, QueryTransferFile, nUsrID, buffer);

					if (nFlag == QueryFileReadSuccess)
					{
						int nFlowCtl=QMessageFileTransferManager::instance()->flowControl(m_pSender, nUsrID);
						if (nFlowCtl != -1 && (nFlowCtl%FILE_TRANSFER_FLOW_CTRL_TICK) == 0)
						{
							QIMessagePtr pMessage=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
							if (pMessage != NULL)
							{
								buffer=pMessage->query(QueryDownloadFlowControl, nUsrID);

								QObjectPtr ptr=QConnectionManager::instance()->connection(m_pSender);
								if (ptr != NULL)
								{
									QMetaObject::invokeMethod(m_pSender, "sendMessage", Q_ARG(QByteArray, buffer), Q_ARG(int, MSG_FILE_TRANSFER));
								}
							}
						}
						else
						{
							emit download(m_pSender, nUsrID, m_pView);
						}
					}
				}
			}
			else
			{
				QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
			}
		}
		break;
	case AnswerTransferFile:
		{
			int nQueryFlag;
			int nFlag;
			QByteArray buffer;
			int nSize=0;

			out>>nQueryFlag;

			Q_ASSERT(nQueryFlag==QueryFileReadLost || nQueryFlag==QueryFileReadFail || nQueryFlag==QueryFileReadSuccess ||
				nQueryFlag==QueryFileReadFinish);

			nFlag=QueryFileWriteLost;

			if (nQueryFlag == QueryFileReadSuccess)
			{
				int nNumber;
				QSharedFilePtr pFile=QMessageFileTransferManager::instance()->transferFile(m_pSender, nUsrID);
				if (pFile != NULL)
				{
					out>>nNumber>>nSize;

					Q_ASSERT(nSize>=0);
					buffer.resize(nSize);

					out.readRawData(buffer.data(), nSize);

					quint64 nWriteSize=pFile->write(nNumber, buffer);
					if (nWriteSize != (quint64)-1)
					{
						nFlag=QueryFileWriteSuccess;

						nNumber=pFile->permissionsSerial();
						if (pFile->isCurrentBlock(nNumber))
						{
							QMetaObject::invokeMethod(m_pView, "recvTransferFileInfo", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag),
								Q_ARG(int, nFlag), Q_ARG(int, nSize));

							nFlag=pFile->setPermissions();

							if (nFlag != QueryExecuted)
							{
								QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
							}
						}
					}
					else
					{
						nFlag=QueryFileWriteFail;

						QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);

						Q_WRITE_ONLY_DATA_STREAM(stream, buf)
						stream<<nFlag;

						QIMessagePtr pMessage=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
						if (pMessage != NULL)
						{
							buffer=pMessage->query(QueryTransferFile, nUsrID, buf);

							QObjectPtr ptr=QConnectionManager::instance()->connection(m_pSender);
							if (ptr != NULL)
							{
								QMetaObject::invokeMethod(m_pSender, "sendMessage", Q_ARG(QByteArray, buffer), Q_ARG(int, MSG_FILE_TRANSFER));
							}
						}
					}
				}
			}
			else if (nQueryFlag == QueryFileReadFinish)
			{
				QSharedFilePtr pFile=QMessageFileTransferManager::instance()->transferFile(m_pSender, nUsrID);
				if (pFile != NULL)
				{
					int nNumber;
					quint32 nPermissions;
					out>>nNumber>>nPermissions;
					pFile->setPermissions(nNumber, (QFile::Permissions)nPermissions);

					if (pFile->isCurrentBlock(nNumber))
					{
						nFlag=pFile->setPermissions();
						if (nFlag != QueryExecuted)
						{
							QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
						}
					}
				}
			}
			else
			{
				QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
			}

			if (nFlag != QueryExecuted)
			{
				QMetaObject::invokeMethod(m_pView, "recvTransferFileInfo", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag),
					Q_ARG(int, nFlag), Q_ARG(int, nSize));
			}
		}
		break;
	case QueryDownloadFlowControl:
		{
			emit finished(m_pSender, QueryDownloadFlowControl, nUsrID);
		}
		break;
	case AnswerDownloadFlowControl:
		{
			emit download(m_pSender, nUsrID, m_pView);
		}
		break;
	case QueryResumeDownloadFile:
		{
			QString strFile;
			int nFormat;
			quint64 nPos;
			QDateTime	datetime;
			QString strSaveFile;

			out>>strFile>>nFormat>>nPos>>datetime>>strSaveFile;

			Q_ASSERT(!strFile.isEmpty());

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			stream<<strFile;

			QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);

			QFileInfo fileInfo(strFile);
			if (!fileInfo.exists())
			{
				stream<<QueryFileNoExist;
			}
			else if (!fileInfo.isReadable())
			{
				stream<<QueryFileUnreadable;
			}
			else if (fileInfo.lastModified() != datetime)
			{
				stream<<QueryFileModified;
			}
			else
			{
				Q_ASSERT(nFormat == TextFormat || nFormat == BinaryFormat);

				QSharedFilePtr pFile(new QSharedFile(strFile));
				QIODevice::OpenMode openMode=QIODevice::ReadOnly;
				if (nFormat == TextFormat)
				{
					openMode|=QIODevice::Text;
				}

				if (pFile->open(openMode))
				{
					if (pFile->seek(nPos))
					{
						stream<<QueryFileValid<<strSaveFile<<nFormat<<fileInfo.size()<<nPos;
						QMessageFileTransferManager::instance()->addTransferFile(m_pSender, nUsrID, pFile);
					}
					else
					{
						stream<<QueryFileSeekFail;
					}
				}
				else
				{
					stream<<QueryFileOpenFail;
				}
			}

			emit finished(m_pSender, QueryDownloadFile, nUsrID, buffer);
		}
		break;
	case AnswerResumeDownloadFile:
		{
			QString strFile;
			int nQueryFlag;

			QString strSaveFile;
			int nFormat;
			quint64 nSize;
			quint64 nPos;

			out>>strFile>>nQueryFlag;

			Q_ASSERT(nQueryFlag==QueryFileNoExist || nQueryFlag==QueryFileUnreadable || nQueryFlag==QueryFileModified || 
				nQueryFlag==QueryFileSeekFail || nQueryFlag==QueryFileValid || nQueryFlag==QueryFileOpenFail);

			QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);

			int nFlag=QueryUnknow;
			if (nQueryFlag == QueryFileValid)
			{
				out>>strSaveFile>>nFormat>>nSize>>nPos;

				Q_ASSERT(nFormat == TextFormat || nFormat == BinaryFormat);

				Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

				QSharedFilePtr pFile(new QSharedFile(strFile));
				QIODevice::OpenMode openMode=QIODevice::WriteOnly;
				if (nFormat == TextFormat)
				{
					openMode|=QIODevice::Text;
				}

				if (pFile->open(openMode))
				{
					if (pFile->seek(nPos))
					{
						nFlag=QueryFileWriteSuccess;
						QMessageFileTransferManager::instance()->addTransferFile(m_pSender, nUsrID, pFile);
					}
					else
					{
						nFlag=QueryFileSeekFail;
					}
				}
				else
				{
					nFlag=QueryFileOpenFail;
				}

				stream<<nFlag;

				QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
				buffer=pInstance->query(QueryTransferFile, nUsrID, buffer);

				QObjectPtr ptr=QConnectionManager::instance()->connection(m_pSender);
				if (ptr != NULL)
				{
					QMetaObject::invokeMethod(m_pSender, "sendMessage", Q_ARG(QByteArray, buffer), Q_ARG(int, MSG_FILE_TRANSFER));
				}
			}

			QMetaObject::invokeMethod(m_pView, "recvResumeDownloadFile", Q_ARG(quint32, nUsrID), Q_ARG(QString, strFile),
				Q_ARG(int, nQueryFlag), Q_ARG(QString, strSaveFile), Q_ARG(int, nFormat), Q_ARG(qint64, nSize), Q_ARG(int, nFlag));
		}
		break;
	case QueryDownloadSymLink:
		{
			QString strFile;
			QString strSaveFile;

			out>>strFile>>strSaveFile;

			Q_ASSERT(!strFile.isEmpty());

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QFileInfo fileInfo(strFile);
			if (!fileInfo.exists())
			{
				stream<<QueryFileNoExist;
			}
			else if (!fileInfo.isReadable())
			{
				stream<<QueryFileUnreadable;
			}
			else if (!fileInfo.isSymLink())
			{
				stream<<QuerySymLinkNoLink;
			}
			else
			{	
				quint32 nPermissions=(quint32)fileInfo.permissions();
				stream<<QueryFileValid<<strSaveFile<<fileInfo.symLinkTarget()<<nPermissions;
			}

			emit finished(m_pSender, QueryDownloadSymLink, nUsrID, buffer);
		}
		break;
	case AnswerDownloadSymLink:
		{
			int nQueryFlag, nFlag;
			out>>nQueryFlag;

			Q_ASSERT(nQueryFlag==QueryFileNoExist || nQueryFlag==QueryFileUnreadable
				|| nQueryFlag==QuerySymLinkNoLink || nQueryFlag==QueryFileValid);

			if (nQueryFlag == QueryFileValid)
			{
				QString strSaveFile;
				QString strSymLink;
				quint32 nPermissions;
				out>>strSaveFile>>strSymLink>>nPermissions;

				if (QFileEx::link(strSymLink, strSaveFile))
				{
					nFlag=QueryCreateSymLinkSuccess;
					if (!QFile::setPermissions(strSaveFile, (QFile::Permissions)nPermissions))
					{
						nFlag=QueryFileChangePermissionFail;
					}
				}
				else
				{
					nFlag=QueryCreateSymLinkFail;
				}
			}

			QMetaObject::invokeMethod(m_pView, "recvDownloadSymLink", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag), Q_ARG(int, nFlag));
		}
		break;
	case QueryUploadDir:
	case QueryUploadSymlink:
		{
			int nType;
			QString	strAddr;

			out>>nType>>strAddr;

			Q_ASSERT(!strAddr.isEmpty());

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			int nFlag;
			switch (nType)
			{
			case Dir:
			case HideDir:
				{
					nFlag=QueryMakePathFail;
					QDirEx dir;
					if (dir.mkpath(strAddr))
					{
						nFlag=QueryMakePathSuccess;
					}
				}
				break;
			case SymLinkDir:
			case SymLinkHideDir:
			case SymLinkFile:
			case SymLinkHideFile:
				{
					nFlag=QueryCreateSymLinkFail;

					QString strTarget;
					out>>strTarget;

					if (QFileEx::link(strTarget, strAddr))
					{
						nFlag=QueryCreateSymLinkSuccess;
					}
				}
				break;
			default:
				Q_ASSERT(false);
				break;
			}

			stream<<nFlag;

			emit finished(m_pSender, nAckType, nUsrID, buffer);
		}
		break;
	case QueryUploadSlientDir:
	case QueryUploadSlientSymlink:
		{
			int nType;
			QString	strAddr;

			out>>nType>>strAddr;

			Q_ASSERT(!strAddr.isEmpty());

			switch (nType)
			{
			case Dir:
			case HideDir:
				{
					QDirEx dir;
					dir.mkpath(strAddr);
				}
				break;
			case SymLinkDir:
			case SymLinkHideDir:
			case SymLinkFile:
			case SymLinkHideFile:
				{
					QString strTarget;
					out>>strTarget;

					QFileEx::link(strTarget, strAddr);
				}
				break;
			default:
				Q_ASSERT(false);
				break;
			}
		}
		break;
	case AnswerUploadDir:
	case AnswerUploadSymlink:
		{
			int nFlag;

			out>>nFlag;

			Q_ASSERT(nFlag==QueryMakePathFail || nFlag==QueryMakePathSuccess || nFlag==QueryCreateSymLinkFail || nFlag==QueryCreateSymLinkSuccess);

			QMetaObject::invokeMethod(m_pView, "recvUploadDirAndSymlink", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag));
		}
		break;
	case QueryUploadFile:
		{
			QString strFile;
			QString strSaveFile;
			int nFormat;

			out>>strSaveFile>>strFile>>nFormat;

			Q_ASSERT(nFormat == TextFormat || nFormat == BinaryFormat);

			QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QFileInfo info(strSaveFile+TEMP_FILE_SUFFIX);
			QDirEx dir;
			QString strPath=info.absolutePath();
			if (!dir.exists(strPath))
			{
				dir.mkpath(strPath);
			}

			QSharedFilePtr pFile(new QSharedFile(strSaveFile+TEMP_FILE_SUFFIX));
			QIODevice::OpenMode openMode=QIODevice::WriteOnly;
			if (nFormat == TextFormat)
			{
				openMode|=QIODevice::Text;
			}

			if (pFile->open(openMode))
			{
				stream<<QueryFileOpenSuccess;
				QMessageFileTransferManager::instance()->addTransferFile(m_pSender, nUsrID, pFile);
			}
			else
			{
				stream<<QueryFileOpenFail;
			}

			stream<<strFile<<nFormat;

			emit finished(m_pSender, QueryUploadFile, nUsrID, buffer);
		}
		break;
	case AnswerUploadFile:
		{
			int nQueryFlag;
			QString strFile;
			int nFormat;

			out>>nQueryFlag>>strFile>>nFormat;

			Q_ASSERT(nQueryFlag==QueryFileOpenSuccess || nQueryFlag==QueryFileOpenFail);
			Q_ASSERT(nFormat == TextFormat || nFormat == BinaryFormat);
			Q_ASSERT(!strFile.isEmpty());

			QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);

			int nFlag=QueryUnknow;
			if (nQueryFlag == QueryFileOpenSuccess)
			{
				Q_WRITE_ONLY_DATA_STREAM(stream, buf)

				QSharedFilePtr pFile(new QSharedFile(strFile));
				QIODevice::OpenMode openMode=QIODevice::ReadOnly;
				if (nFormat == TextFormat)
				{
					openMode|=QIODevice::Text;
				}

				if (pFile->open(openMode))
				{
					nFlag=QueryFileOpenSuccess;

					QMessageFileTransferManager::instance()->addTransferFile(m_pSender, nUsrID, pFile);
				}
				else
				{
					nFlag=QueryFileOpenFail;
				}

				stream<<nFlag;

				QObjectPtr ptr=QConnectionManager::instance()->connection(m_pSender);
				if (ptr != NULL)
				{
					if (nFlag == QueryFileOpenSuccess)
					{
						emit upload(m_pSender, nUsrID, m_pView);
					}
					else
					{
						QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
						if (pInstance != NULL)
						{
							QByteArray buffer=pInstance->query(QueryUploadFileContent, nUsrID, buf);

							QObjectPtr ptr=QConnectionManager::instance()->connection(m_pSender);
							if (ptr != NULL)
							{
								QMetaObject::invokeMethod(m_pSender, "sendMessage", Q_ARG(QByteArray, buffer), Q_ARG(int, MSG_FILE_TRANSFER));
							}
						}
					}
				}
			}

			QMetaObject::invokeMethod(m_pView, "recvUploadFile", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag), Q_ARG(int, nFlag));
		}
		break;
	case QueryUploadFileContent:
		{
			int nQueryFlag;

			out>>nQueryFlag;

			Q_ASSERT(nQueryFlag == QueryFileOpenFail || nQueryFlag == QueryFileReadSuccess || nQueryFlag == QueryFileReadFinish 
				|| nQueryFlag== QueryFileReadLost || nQueryFlag==QueryFileReadFail);

			int nFlag=QueryUnknow;
			quint64 nSize=0;

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			if (nQueryFlag == QueryFileReadSuccess)
			{
				int nNumber;
				out>>nNumber>>nSize;

				QByteArray buf;

				buf.resize(nSize);

				out.readRawData(buf.data(), nSize);

				nFlag=QueryFileWriteLost;
				QSharedFilePtr pFile=QMessageFileTransferManager::instance()->transferFile(m_pSender, nUsrID);
				if (pFile != NULL)
				{
					nFlag=QueryFileWriteFail;

					quint64 nWriteSize=pFile->write(nNumber, buf);
					if (nWriteSize != (quint64)-1)
					{
						nFlag=QueryFileWriteSuccess;

						nNumber=pFile->permissionsSerial();
						if (pFile->isCurrentBlock(nNumber))
						{
							nFlag=QueryFileWriteFinish;

							int nTempFlag=pFile->setPermissions();
							if (nTempFlag != QueryExecuted)
							{
								nFlag=nTempFlag;
							}

							QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
						}
					}
					else
					{
						QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
					}
				}
			}
			else if (nQueryFlag == QueryFileReadFinish)
			{
				nFlag=QueryFileWriteLost;
				QSharedFilePtr pFile=QMessageFileTransferManager::instance()->transferFile(m_pSender, nUsrID);
				if (pFile != NULL)
				{
					int nNumber;
					quint32 nPermissions;
					out>>nNumber>>nPermissions;

					pFile->setPermissions(nNumber, (QFile::Permissions)nPermissions);

					nFlag=QueryFileWriteSuccess;

					if (pFile->isCurrentBlock(nNumber))
					{
						nFlag=QueryFileWriteFinish;

						int nTempFlag=pFile->setPermissions();

						if (nTempFlag != QueryExecuted)
						{
							nFlag=nTempFlag;
							QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
						}
					}
				}
			}
			else
			{
				QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
			}

			if ((nQueryFlag == QueryFileReadSuccess || nQueryFlag == QueryFileReadFinish) && nFlag != QueryFileWriteSuccess)
			{
				stream<<nFlag;

				emit finished(m_pSender, QueryUploadFileContent, nUsrID, buffer);
			}
		}
		break;
	case AnswerUploadFileContent:
		{
			int nQueryflag;
			out>>nQueryflag;

			Q_ASSERT(nQueryflag==QueryFileWriteLost || nQueryflag==QueryFileWriteFail || nQueryflag==QueryFileWriteSuccess || 
				nQueryflag==QueryFileWriteFinish || nQueryflag == QueryFileRenameFail || nQueryflag == QueryFileChangePermissionFail ||
				nQueryflag==QueryFileTransferFinish);

			QByteArray buffer;
			int nFlag=QueryUnknow;
			quint64 nSize=0;
			if (nQueryflag == QueryFileWriteSuccess)
			{
				Q_WRITE_ONLY_DATA_STREAM(stream, buf)

				int nFlowCtl=QMessageFileTransferManager::instance()->flowControl(m_pSender, nUsrID);
				if (nFlowCtl != -1 && (nFlowCtl%FILE_TRANSFER_FLOW_CTRL_TICK) == 0)
				{
					nFlag=QueryFileSendFlowControl;
					QIMessagePtr pMessage=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
					if (pMessage != NULL)
					{
						buffer=pMessage->query(QueryUploadFlowControl, nUsrID);

						QObjectPtr ptr=QConnectionManager::instance()->connection(m_pSender);
						if (ptr != NULL)
						{
							QMetaObject::invokeMethod(m_pSender, "sendMessage", Q_ARG(QByteArray, buffer), Q_ARG(int, MSG_FILE_TRANSFER));
						}
					}
				}
				else
				{
					nFlag=QueryFileReadLost;

					QSharedFilePtr pFile=QMessageFileTransferManager::instance()->transferFile(m_pSender, nUsrID);
					if (pFile != NULL)
					{
						nFlag=readContents(pFile.get(), buffer, QConnectionManager::instance()->canHighRate(m_pSender));

						int nNumber=pFile->nextBlock();
						stream<<nFlag;
						if (nFlag == QueryFileReadFinish)
						{
							stream<<nNumber<<(quint32)pFile->permissions();
						}
						else if (nFlag == QueryFileReadSuccess)
						{
							nSize=buffer.size();
							stream<<nNumber<<nSize;

							stream.writeRawData(buffer.data(), nSize);
						}

						if (nFlag == QueryFileReadFinish || nFlag == QueryFileReadFail)
						{
							QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
						}

						QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
						buffer=pInstance->query(QueryUploadFileContent, nUsrID, buf);

						QObjectPtr ptr=QConnectionManager::instance()->connection(m_pSender);
						if (ptr != NULL)
						{

							QObjectPtr ptr=QConnectionManager::instance()->connection(m_pSender);
							if (ptr != NULL)
							{
								QMetaObject::invokeMethod(m_pSender, "sendMessage", Q_ARG(QByteArray, buffer), Q_ARG(int, MSG_FILE_TRANSFER));
							}

							if (nFlag == QueryFileReadSuccess)
							{
								emit upload(m_pSender, nUsrID, m_pView);
							}
						}
					}
				}
			}
			else
			{
				QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
			}

			QMetaObject::invokeMethod(m_pView, "recvUploadFile", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryflag), Q_ARG(int, nFlag), Q_ARG(quint64, nSize));
		}
		break;
	case QueryUploadSlientFile:
		{
			QString strSaveFile;
			QString strFile;
			int nFormat;

			out>>strSaveFile>>strFile>>nFormat;

			Q_ASSERT(nFormat == TextFormat || nFormat == BinaryFormat);
			Q_ASSERT(!strFile.isEmpty() || !strSaveFile.isEmpty());

			QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);

			int nFlag=QueryUnknow;

			QSharedFilePtr pFile(new QSharedFile(strFile));
			QIODevice::OpenMode openMode=QIODevice::ReadOnly;
			if (nFormat == TextFormat)
			{
				openMode|=QIODevice::Text;
			}

			if (pFile->open(openMode))
			{
				nFlag=QueryFileOpenSuccess;

				QMessageFileTransferManager::instance()->addTransferFile(m_pSender, nUsrID, pFile);
			}
			else
			{
				nFlag=QueryFileOpenFail;
			}

			QMetaObject::invokeMethod(m_pView, "recvUploadSlientFile", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag));

			if (nFlag == QueryFileOpenSuccess)
			{
				emit slientUpload(m_pSender, nUsrID, strSaveFile, nFormat, m_pView);
			}
		}
		break;
	case QueryUploadSlientFileContent:
		{
			QString strSaveFile;
			int nFormat;

			out>>strSaveFile>>nFormat;

			Q_ASSERT(nFormat == TextFormat || nFormat == BinaryFormat);
			Q_ASSERT(!strSaveFile.isEmpty());

			QByteArray buffer;
			int nFlag=QueryFileReadLost;
			quint64 nSize=0;

			Q_WRITE_ONLY_DATA_STREAM(stream, buf)

			stream<<strSaveFile<<nFormat;

			QSharedFilePtr pFile=QMessageFileTransferManager::instance()->transferFile(m_pSender, nUsrID);
			if (pFile != NULL)
			{
				nFlag=readContents(pFile.get(), buffer, QConnectionManager::instance()->canHighRate(m_pSender));

				int nNumber=pFile->nextBlock();
				stream<<nFlag;

				if (nFlag == QueryFileReadFinish)
				{
					stream<<nNumber<<(quint32)pFile->permissions();
				}
				else if (nFlag == QueryFileReadSuccess)
				{
					nSize=buffer.size();
					stream<<nNumber<<nSize;

					stream.writeRawData(buffer.data(), buffer.size());
				}

				if (nFlag == QueryFileReadFinish || nFlag == QueryFileReadFail)
				{
					QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
				}
			}
			else
			{
				stream<<nFlag;
			}

			QObjectPtr ptr=QConnectionManager::instance()->connection(m_pSender);
			if (ptr != NULL)
			{
				emit finished(m_pSender, QueryUploadSlientFileContent, nUsrID, buf);

				if (nFlag == QueryFileReadSuccess)
				{
					emit slientUpload(m_pSender, nUsrID, strSaveFile, nFormat, m_pView);
				}
			}

			QMetaObject::invokeMethod(m_pView, "recvUploadSlientFile", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag), Q_ARG(quint64, nSize));
		}
		break;
	case AnswerUploadSlientFileContent:
		{
			QString strSaveFile;
			int nFormat;
			int nQueryFlag;

			out>>strSaveFile>>nFormat>>nQueryFlag;

			Q_ASSERT(nQueryFlag == QueryFileReadSuccess || nQueryFlag == QueryFileReadFinish || nQueryFlag== QueryFileReadLost 
				|| nQueryFlag==QueryFileReadFail);
			Q_ASSERT(nFormat == TextFormat || nFormat == BinaryFormat);

			if (nQueryFlag == QueryFileReadLost || nQueryFlag==QueryFileReadFail)
			{
				QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
			}
			else
			{
				int nFlag=QueryUnknow;
				quint64 nSize=0;

				if (nQueryFlag == QueryFileReadSuccess)
				{
					int nNumber=-1;
					out>>nNumber>>nSize;

					QByteArray buf;

					buf.resize(nSize);

					nFlag=QueryFileWriteLost;
					if (out.readRawData(buf.data(), nSize) != nSize)
					{
						nFlag=QueryFileWriteFail;
						QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
					}
					else
					{
						QSharedFilePtr pFile=QMessageFileTransferManager::instance()->transferFile(m_pSender, nUsrID);
						if (pFile == NULL)
						{
							pFile=QSharedFile::createFile(strSaveFile, nFormat, m_pSender, nUsrID);
						}

						if (pFile != NULL)
						{
							nFlag=QueryFileWriteFail;

							quint64 nWriteSize=pFile->write(nNumber, buf);
							if (nWriteSize != (quint64)-1)
							{
								nFlag=QueryFileWriteSuccess;

								nNumber=pFile->permissionsSerial();
								if (pFile->isCurrentBlock(nNumber))
								{
									nFlag=pFile->setPermissions();
									if (nFlag != QueryExecuted)
									{
										QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
									}
								}
							}
							else
							{
								QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
							}
						}
					}
				}
				else if (nQueryFlag == QueryFileReadFinish)
				{
					int nNumber;
					quint32 nPermissions;
					out>>nNumber>>nPermissions;

					nFlag=QueryFileWriteLost;
					QSharedFilePtr pFile=QMessageFileTransferManager::instance()->transferFile(m_pSender, nUsrID);
					if (pFile == NULL)
					{
						pFile=QSharedFile::createFile(strSaveFile, nFormat, m_pSender, nUsrID);
					}

					if (pFile != NULL)
					{
						nFlag=QueryFileWriteFinish;

						pFile->setPermissions(nNumber, (QFile::Permissions)nPermissions);

						if (pFile->isCurrentBlock(nNumber))
						{
							nFlag=pFile->setPermissions();
							if (nFlag != QueryExecuted)
							{
								QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
							}
						}
					}
				}
				else
				{
					QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
				}
			}
		}
		break;
	case QueryResumeUploadFile:
		{
			QString strFile;
			QString strSaveFile;
			int nFormat;
			quint64 nPos;

			out>>strSaveFile>>strFile>>nFormat>>nPos;

			Q_ASSERT(nFormat == TextFormat || nFormat == BinaryFormat);
			Q_ASSERT(nPos>=0);

			QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QFileInfo fileInfo(strFile);
			if (!fileInfo.exists())
			{
				stream<<QueryFileNoExist;
			}
			else if (!fileInfo.isWritable())
			{
				stream<<QueryFileUnwritable;
			}
			else
			{
				QSharedFilePtr pFile(new QSharedFile(strSaveFile+TEMP_FILE_SUFFIX));
				QIODevice::OpenMode openMode=QIODevice::WriteOnly;
				if (nFormat == TextFormat)
				{
					openMode|=QIODevice::Text;
				}

				int nFlag;
				if (pFile->open(openMode))
				{
					nFlag=QueryFileOpenSuccess;

					if (!pFile->seek(nPos))
					{
						nFlag=QueryFileSeekFail;
					}

					stream<<nFlag;

					QMessageFileTransferManager::instance()->addTransferFile(m_pSender, nUsrID, pFile);
				}
				else
				{
					stream<<QueryFileOpenFail;
				}

				if (nFlag == QueryFileOpenSuccess)
				{
					stream<<strFile<<nFormat<<nPos;
				}
			}

			emit finished(m_pSender, QueryUploadFile, nUsrID, buffer);
		}
		break;
	case AnswerResumeUploadFile:
		{
			int nQueryFlag;

			out>>nQueryFlag;

			int nFlag=QueryUnknow;

			if (nQueryFlag == QueryFileOpenSuccess)
			{
				QString strFile;
				int nFormat;
				quint64 nPos;

				out>>strFile>>nFormat>>nPos;

				QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);

				Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

				QFileInfo fileInfo(strFile);
				if (!fileInfo.exists())
				{
					nFlag=QueryFileNoExist;
				}
				else if (!fileInfo.isReadable())
				{
					nFlag=QueryFileUnreadable;
				}
				else
				{
					QSharedFilePtr pFile(new QSharedFile(strFile));
					QIODevice::OpenMode openMode=QIODevice::ReadOnly;
					if (nFormat == TextFormat)
					{
						openMode|=QIODevice::Text;
					}

					int nFlag;
					if (pFile->open(openMode))
					{
						nFlag=QueryFileOpenSuccess;

						if (!pFile->seek(nPos))
						{
							nFlag=QueryFileSeekFail;
						}

						QMessageFileTransferManager::instance()->addTransferFile(m_pSender, nUsrID, pFile);
					}
					else
					{
						nFlag=QueryFileOpenFail;
					}

					stream<<nFlag;

					QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
					buffer=pInstance->query(QueryUploadFileContent, nUsrID, buffer);

					QObjectPtr ptr=QConnectionManager::instance()->connection(m_pSender);
					if (ptr != NULL)
					{
						QMetaObject::invokeMethod(m_pSender, "sendMessage", Q_ARG(QByteArray, buffer), Q_ARG(int, MSG_FILE_TRANSFER));
					}
				}

				emit finished(m_pSender, QueryUploadFile, nUsrID, buffer);
			}
		}
		break;
	case QueryUploadFlowControl:
		{
			emit finished(m_pSender, QueryUploadFlowControl, nUsrID);
		}
		break;
	case AnswerUploadFlowControl:
		{
			emit upload(m_pSender, nUsrID, m_pView);
		}
		break;
	case QueryRename:
		{
			QString strDir;
			QString strOld,strNow;

			out>>strDir>>strOld>>strNow;

			int nFlag=QueryRenamePathLost;
			QDir dir(appendPath(strDir, strOld));
			if (dir.exists())
			{
				nFlag=QueryRenameFail;
				if (dir.rename(appendPath(strDir, strOld), appendPath(strDir, strNow)))
				{
					nFlag=QueryRenameSuccess;
				}
			}

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			stream<<nFlag<<strDir<<strOld<<strNow;

			emit finished(m_pSender, QueryRename, nUsrID, buffer);
		}
		break;
	case AnswerRename:
		{
			QString strDir,strOld,strNow;
			int nFlag;

			out>>nFlag>>strDir>>strOld>>strNow;

			Q_ASSERT(nFlag==QueryRenamePathLost || nFlag==QueryRenameFail || nFlag==QueryRenameSuccess);

			QMetaObject::invokeMethod(m_pView, "recvRename", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag), Q_ARG(QString, strDir),
				Q_ARG(QString, strOld), Q_ARG(QString, strNow));
		}
		break;
	case QueryNewDirectory:
		{
			QString strDir;
			out>>strDir;

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QDirEx dir(strDir);
			if (dir.exists())
			{
				QString strPath;

				int i=1;
				while (true)
				{
					strPath=appendPath(strDir, QString(tr("New Directory %1")).arg(i));
					if (!dir.exists(strPath))
					{
						break;
					}

					i++;
				}

				if (dir.mkpath(strPath))
				{
					stream<<QueryNewSuccess<<entryInfo(QFileInfo(strPath), false);
				}
				else
				{
					stream<<QueryNewFail;
				}
			}
			else
			{
				stream<<QueryDirNoExist;
			}

			emit finished(m_pSender, QueryNewDirectory, nUsrID, buffer);
		}
		break;
	case AnswerNewDirectory:
		{
			int nFlag;
			out>>nFlag;

			Q_ASSERT(nFlag==QueryNewSuccess || nFlag==QueryNewFail || nFlag==QueryDirNoExist);

			FileInfo file;
			if (nFlag == QueryNewSuccess)
			{
				out>>file;
			}

			QMetaObject::invokeMethod(m_pView, "recvNewDirectory", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag), Q_ARG(FileInfo, file));
		}
		break;
	case QueryDelete:
		{
			QString strDir;
			QVector<QString> vecFileName;
			out>>strDir>>vecFileName;

			int nFlag=QueryDelSuccess;

			QMutableVectorIterator<QString> it(vecFileName);

			QString strName;
			while (it.hasNext())
			{
				it.next();

				strName=appendPath(strDir, it.value());

				QFileInfo info(strName);
				if (info.exists())
				{
					if (info.isSymLink() || info.isFile())
					{
						if (QFile::remove(strName))
						{
							it.remove();
						}
					}
					else if (info.isDir())
					{
						if (deleteDir(strName))
						{
							it.remove();
						}
					}
				}
				else
				{
					it.remove();
				}
			}

			if (!vecFileName.isEmpty())
			{
				nFlag=QueryDelFail;
			}

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			stream<<nFlag;
			if (nFlag == QueryDelFail)
			{
				stream<<vecFileName;
			}

			emit finished(m_pSender, QueryDelete, nUsrID, buffer);
		}
		break;
	case AnswerDelete:
		{
			int nFlag;
			QVector<QString> files;

			out>>nFlag;
			if (nFlag == QueryDelFail)
			{
				out>>files;
			}

			QMetaObject::invokeMethod(m_pView, "recvDelete", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag), Q_ARG(QVector<QString>, files));
		}
		break;
	case QueryInterruptTransfer:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			int nFlag=QueryFileRemoved;
			QSharedFilePtr pFile=QMessageFileTransferManager::instance()->transferFile(m_pSender, nUsrID);
			if (pFile != NULL)
			{
				pFile->close();

// 				nFlag=QueryFileRemoveFail;
// 				if (pFile->remove())
// 				{
// 					nFlag=QueryFileRemoveSuccess;
// 				}

				QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);
			}

			stream<<nFlag;

			emit finished(m_pSender, QueryInterruptTransfer, nUsrID, buffer);
		}
		break;
	case AnswerInterruptTransfer:
		{
			int nFlag;
			out>>nFlag;

			Q_ASSERT(nFlag==QueryFileRemoved || nFlag==QueryFileRemoveFail || nFlag==QueryFileRemoveSuccess);

			QMessageFileTransferManager::instance()->removeTransferFile(m_pSender, nUsrID);

			QMetaObject::invokeMethod(m_pView, "recvInterruptTransfer", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag));
		}
		break;
	case QueryBackup:
		{
			QString strSourceFolder,strTargetFolder;
			int nCount;

			bool bInvalid=false;
			FileInfo file;
			int nFinish=0;
			int nFailed=0;

			out>>strSourceFolder>>strTargetFolder>>nCount;
			if (!strSourceFolder.isEmpty() && !strTargetFolder.isEmpty() && nCount>=0)
			{
				QDateTime time=QDateTime::currentDateTime();
				for (int i=0; i<nCount; i++)
				{
					if (out.atEnd())
					{
						bInvalid=true;
						break;
					}

					out>>file;
					if ((file.type & (File|HideFile|SymLink)) != 0)
					{
						if (!QFileEx::copy(appendPath(strSourceFolder, file.name), appendPath(strTargetFolder, file.name)))
						{
							nFailed++;
						}
					}

					nFinish++;

					if (QDateTime::currentDateTime() > time.addSecs(1))
					{
						time=QDateTime::currentDateTime();

						Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
						stream<<QueryBackingUp<<nFinish<<nFinish-nFailed;

						emit finished(m_pSender, nAckType, nUsrID, buffer);

						QThread::currentThread()->wait(10);
					}
				}
			}

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<QueryBackUpFinish<<nFinish<<nFinish-nFailed;

			emit finished(m_pSender, nAckType, nUsrID, buffer);
		}
		break;
	case AnswerBackup:
		{
			int nFlag, nFinish, nSuccess;
			out>>nFlag>>nFinish>>nSuccess;

			QMetaObject::invokeMethod(m_pView, "recvBackup", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag), Q_ARG(int, nFinish), Q_ARG(int, nSuccess));
		}
		break;
	default:
		break;
	}
}

QMutex QSharedFile::g_mutex;
QSharedFile::QSharedFile(const QString& name)
:QFileEx(name)
{
	init();
}

QSharedFile::QSharedFile(QObject* parent)
:QFileEx(parent)
{
	init();
}

QSharedFile::QSharedFile(const QString& name, QObject* parent)
:QFileEx(name, parent)
{
	init();
}

QSharedFile::~QSharedFile()
{
	close();
}

void QSharedFile::init()
{
	m_nNextBlock=0;
	m_permissions.first=-1;
}

QSharedFilePtr QSharedFile::createFile(const QString& filename, int format, QObject* client, quint32 usrID)
{
	QMutexLocker locker(&g_mutex);

	QSharedFilePtr pFile=QMessageFileTransferManager::instance()->transferFile(client, usrID);
	if (pFile != NULL)
	{
		return pFile;
	}

	QFileInfo info(filename+TEMP_FILE_SUFFIX);
	QDirEx dir;
	QString strPath=info.absolutePath();
	if (!dir.exists(strPath))
	{
		dir.mkpath(strPath);
	}

	pFile=new QSharedFile(filename+TEMP_FILE_SUFFIX);
	QIODevice::OpenMode openMode=QIODevice::WriteOnly;
	if (format == TextFormat)
	{
		openMode|=QIODevice::Text;
	}

	if (pFile->open(openMode))
	{
		QMessageFileTransferManager::instance()->addTransferFile(client, usrID, pFile);
	}
	else
	{
		pFile=NULL;
	}

	return pFile;
}

int QSharedFile::nextBlock()
{
	return m_nNextBlock++;
}

bool QSharedFile::isCurrentBlock(int number) const
{
	return m_nNextBlock==number;
}

qint64 QSharedFile::write(int number, const QByteArray& byteArray)
{
	if (number < 0)
	{
		return (quint64)-1;
	}

	m_mutex.lock();

	quint64 nSize=0;
	quint64 nTemp=0;
	if (isCurrentBlock(number))
	{
		nTemp=QFile::write(byteArray);
		if (nTemp != byteArray.size())
		{
			nSize=(quint64)-1;
		}
		else
		{
			nSize+=nTemp;
		}

		nextBlock();

		if (nSize != (quint64)-1)
		{
			while (m_contents.contains(m_nNextBlock))
			{
				nTemp=QFile::write(m_contents[m_nNextBlock]);
				if (nTemp != m_contents[m_nNextBlock].size())
				{
					nSize=(quint64)-1;
					break;
				}
				else
				{
					nSize+=nTemp;
				}

				m_contents.remove(m_nNextBlock);

				nextBlock();
			}
		}
	}
	else
	{
		m_contents.insert(number, byteArray);
	}

	m_mutex.unlock();

	return nSize;
}

void QSharedFile::setPermissions(int number, QFile::Permissions permissionSpec)
{
	m_mutex.lock();

	m_permissions.first=number;
	m_permissions.second=permissionSpec;

	m_mutex.unlock();
}

int QSharedFile::setPermissions()
{
	QMutexLocker locker(&m_mutex);

	int nRet=QueryExecuted;
	if (m_permissions.first != -1 && m_contents.isEmpty())
	{
		nRet=QueryFileTransferFinish;
		QString strFile=fileName();
		strFile=strFile.left(strFile.size()-QString(TEMP_FILE_SUFFIX).size());

		if (QFile::exists(strFile))
		{
			QFile::remove(strFile);
		}

		if (!QFile::rename(strFile))
		{
			nRet=QueryFileRenameFail;
		}

		if (!QFile::setPermissions(m_permissions.second))
		{
			nRet=QueryFileChangePermissionFail;
		}
		
		m_permissions.first=-1;
	}

	return nRet;
}

int QSharedFile::permissionsSerial()
{
	QMutexLocker locker(&m_mutex);

	return m_permissions.first;
}

QMutex QMessageFileTransferManager::g_mutex;
QMessageFileTransferManager::QMessageFileTransferManager(QObject* parent)
:QObject(parent)
{
	m_timer.setInterval(10*1000);

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

QMessageFileTransferManager::~QMessageFileTransferManager()
{
	m_timer.stop();
}

QMessageFileTransferManager* QMessageFileTransferManager::instance()
{
	static QMessageFileTransferManager* pInstance=new QMessageFileTransferManager;

	return pInstance;
}

void QMessageFileTransferManager::check()
{
	int nNum=0;

	QMutableMapIterator<QObject*, QMap<quint32, FileHandle> > it(m_mapFiles);
	while (it.hasNext())
	{
		it.next();

		const QMap<quint32, FileHandle>& files=it.value();
		if (!files.isEmpty())
		{
			nNum+=files.size();
		}
		else
		{
			it.remove();
		}
	}

	if (nNum != 0)
	{
		if (!m_timer.isActive())
		{
			m_timer.start();
		}
	}
	else
	{
		if (m_timer.isActive())
		{
			m_timer.stop();
		}
	}
}

bool QMessageFileTransferManager::addTransferFile(QObject* client, quint32 usrID, const QSharedFilePtr& file)
{
	if (client == NULL || file == NULL)
	{
		return false;
	}

	QMutexLocker locker(&g_mutex);

	if (!m_mapFiles.contains(client))
	{
		m_mapFiles.insert(client, QMap<quint32, FileHandle>());
	}

	FileHandle fileHandle;
	fileHandle.file=file;
	fileHandle.tick=0;
	fileHandle.flowCtl=0;

	QMap<quint32, FileHandle>& mapFiles=m_mapFiles[client];
	mapFiles.insert(usrID, fileHandle);

	return true;
}
void QMessageFileTransferManager::removeTransferFile(QObject* client, quint32 usrID)
{
	if (client == NULL)
	{
		return;
	}

	QMutexLocker locker(&g_mutex);

	if (m_mapFiles.contains(client))
	{
		if (m_mapFiles[client].contains(usrID))
		{
			m_mapFiles[client].remove(usrID);
		}

		if (m_mapFiles[client].isEmpty())
		{
			m_mapFiles.remove(client);
		}
	}
}

QSharedFilePtr QMessageFileTransferManager::transferFile(QObject* client, quint32 usrID)
{
	QSharedFilePtr pFile;

	QMutexLocker locker(&g_mutex);
	
	if (client != NULL)
	{
		if (m_mapFiles.contains(client))
		{
			QMap<quint32, FileHandle>& mapFiles=m_mapFiles[client];
			if (mapFiles.contains(usrID))
			{
				mapFiles[usrID].tick=0;
				pFile=mapFiles[usrID].file;
			}
		}	
	}

	return pFile;
}

int QMessageFileTransferManager::flowControl(QObject* client, quint32 usrID)
{
	if (client != NULL)
	{
		QMutexLocker locker(&g_mutex);

		if (m_mapFiles.contains(client))
		{
			QMap<quint32, FileHandle>& mapFiles=m_mapFiles[client];
			if (mapFiles.contains(usrID))
			{
				return ++mapFiles[usrID].flowCtl;
			}
		}	
	}

	return -1;
}

void QMessageFileTransferManager::timeout()
{
	if (m_mapFiles.isEmpty())
	{
		return;
	}

	QMutexLocker locker(&g_mutex);

	QMutableMapIterator<QObject*, QMap<quint32, FileHandle> > it(m_mapFiles);
	while (it.hasNext())
	{
		it.next();

		QMap<quint32, FileHandle>& mapFiles=it.value();
		if (mapFiles.isEmpty())
		{
			continue;
		}

		QMutableMapIterator<quint32, FileHandle> iter(mapFiles);
		while (iter.hasNext())
		{
			iter.next();

			if (iter.value().tick > FILE_TRANSFER_TICK_TIMES)
			{
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
}

REGISTER_MESSAGE_INSTANCE(QMessageFileTransfer) 
