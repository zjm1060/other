#include "qmessagefiletransfor.h"

#include <QThreadPool>
#include <QFileInfo>
#include <QDir>

#include "define.h"
#include "common.h"

#define FILE_TRANSFOR_TICK_TIMES	30
#define FILE_PACKET_SIZE	(4*1024)

QMessageFileTransfor::QMessageFileTransfor(QObject *parent)
	: QObject(parent)
{
	m_timer.setInterval(10*1000);

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

QMessageFileTransfor::~QMessageFileTransfor()
{

}

bool QMessageFileTransfor::addTransforFile(int usrID, QFile* file)
{
	if (file == NULL)
	{
		return false;
	}

	int nNum=m_mapFiles.count();

	if (m_mapFiles.contains(usrID))
	{
		FileHandle& handle=m_mapFiles[usrID];
		if (handle.file != NULL)
		{
			handle.file->close();
			SAFE_DELETE_CLASS(handle.file);
		}
	}

	FileHandle fileHandle;
	fileHandle.file=file;
	fileHandle.tick=0;

	m_mapFiles.insert(usrID, fileHandle);

	if (nNum == 0)
	{
		m_timer.start();
	}

	return true;
}
void QMessageFileTransfor::removeTransforFile(int usrID)
{
	if (m_mapFiles.contains(usrID))
	{
		FileHandle& handle=m_mapFiles[usrID];
		if (handle.file != NULL)
		{
			handle.file->close();
			SAFE_DELETE_CLASS(handle.file);
		}

		m_mapFiles.remove(usrID);
	}

	if (m_mapFiles.count() == 0)
	{
		m_timer.stop();
	}
}

QFile* QMessageFileTransfor::transforFile(int usrID)
{
	if (m_mapFiles.contains(usrID))
	{
		return m_mapFiles[usrID].file;
	}
	else
	{
		return NULL;
	}
}

void QMessageFileTransfor::timeout()
{
	QMutableMapIterator<int, FileHandle> it(m_mapFiles);

	while (it.hasNext())
	{
		it.next();
		if (it.value().tick > FILE_TRANSFOR_TICK_TIMES)
		{
			it.remove();
		}
		else
		{
			it.value().tick++;
		}
	}
}

QByteArray QMessageFileTransfor::query(int msgType, quint32 usrID, const QByteArray& contents)
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

bool QMessageFileTransfor::parse(QConnection* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageFileTransforRunnable* pRunnable=new QMessageFileTransforRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QConnection*, int, quint32, QByteArray)), 
		this, SLOT(finished(QConnection*, int, quint32, QByteArray)));

	QThreadPool::globalInstance()->start(pRunnable);

	return true;
}

void QMessageFileTransfor::finished(QConnection* sender, int msgType, quint32 usrID, QByteArray buffer)
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
	case QueryTransforFile:
	case QueryRetransforFile:
		in<<AnswerTransforFile<<usrID<<buffer.size();
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
	default:
		break;
	}

	sender->sendMessage(buf, Type);
}

QMessageFileTransforRunnable::QMessageFileTransforRunnable(QConnection* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageFileTransforRunnable::~QMessageFileTransforRunnable()
{

}

void QMessageFileTransforRunnable::setArg(QConnection* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageFileTransforRunnable::run()
{
	QDataStream out(m_buffer);
	out.setVersion(DATA_STREAM_VERSION);

	int nMsgType;
	quint32 nUsrID;
	out>>nMsgType>>nUsrID;

	switch (nMsgType)
	{
	case QMessageFileTransfor::QueryRoot:
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			stream<<getAppParentDir();
			emit finished(m_pSender, QMessageFileTransfor::QueryRoot, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfor::AnswerRoot:
		{
			QString strRoot;
			out>>strRoot;

			QMetaObject::invokeMethod(m_pView, "recvRoot", Q_ARG(quint32, nUsrID), Q_ARG(QString, strRoot));
		}
		break;
	case QMessageFileTransfor::QueryPath:
		{
			QString strPath;
			out>>strPath;

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			QDir dir(strPath);
			if (!dir.exists())
			{
				stream<<QMessageFileTransfor::QueryPathNoExist;
			}
			else
			{
				stream<<QMessageFileTransfor::QueryPathValid;

				int nSortFlag;
				out>>nSortFlag;

				quint32 sort=QDir::IgnoreCase;
				switch (nSortFlag&0x0f)
				{
				case QMessageFileTransfor::SortName:
					sort|=QDir::Name;
					break;
				case QMessageFileTransfor::SortTime:
					sort|=QDir::Time;
					break;
				case QMessageFileTransfor::SortSize:
					sort|=QDir::Size;
					break;
				case QMessageFileTransfor::SortType:
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

				QFileInfoList list=dir.entryInfoList(QDir::AllEntries, (QDir::SortFlags)sort);
				entryInfoList(list, vecFileInfo);

				stream<<vecFileInfo.size();
				foreach (FileInfo fileInfo, vecFileInfo)
				{
					stream<<fileInfo.type<<fileInfo.name<<fileInfo.size<<fileInfo.isReadable
						<<fileInfo.isWritable<<fileInfo.isExecutable<<fileInfo.lastModified;
				}
			}

			emit finished(m_pSender, QMessageFileTransfor::QueryPath, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfor::QueryDrive:
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			QFileInfoList list=QDir::drives();
			stream<<list.size();
			foreach (QFileInfo fileInfo, list)
			{
				stream<<QMessageFileTransfor::Drive<<fileInfo.filePath()<<(quint64)0<<fileInfo.isReadable()
					<<fileInfo.isWritable()<<false<<fileInfo.lastModified();
			}

			emit finished(m_pSender, QMessageFileTransfor::QueryDrive, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfor::AnswerPath:
	case QMessageFileTransfor::AnswerDir:
		{
			int nQueryFlag;
			out>>nQueryFlag;

			Q_ASSERT((nQueryFlag == QMessageFileTransfor::QueryPathNoExist) || (nQueryFlag == QMessageFileTransfor::QueryPathValid));

			QVector<FileInfo> vecInfo;
			if (nQueryFlag == QMessageFileTransfor::QueryPathValid)
			{
				FileInfo info;
				int nNum;
				out>>nNum;
				for (int i=0; i<nNum; i++)
				{
					out>>info.type>>info.name>>info.size>>info.isReadable>>info.isWritable>>info.isExecutable>>info.lastModified;

					vecInfo.append(info);
				}
			}

			if (nMsgType == QMessageFileTransfor::AnswerPath)
			{
				QMetaObject::invokeMethod(m_pView, "recvPath", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag),
					Q_ARG(QVector<FileInfo>, vecInfo));
			}
			else
			{
				QMetaObject::invokeMethod(m_pView, "recvDir", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag),
					Q_ARG(QVector<FileInfo>, vecInfo));
			}
		}
		break;
	case QMessageFileTransfor::AnswerDrive:
		{
			QVector<FileInfo> vecInfo;
			FileInfo info;
			int nNum;
			out>>nNum;
			for (int i=0; i<nNum; i++)
			{
				out>>info.type>>info.name>>info.size>>info.isReadable>>info.isWritable>>info.isExecutable>>info.lastModified;

				vecInfo.append(info);
			}

			QMetaObject::invokeMethod(m_pView, "recvDrive", Q_ARG(quint32, nUsrID), Q_ARG(QVector<FileInfo>, vecInfo));
		}
		break;
	case QMessageFileTransfor::QueryFile:
		{
			QString strFile;
			out>>strFile;

			Q_ASSERT(!strFile.isEmpty());

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			QFileInfo fileInfo(strFile);
			if (!fileInfo.exists())
			{
				stream<<QMessageFileTransfor::QueryFileNoExist;
			}
			else if (!fileInfo.isReadable())
			{
				stream<<QMessageFileTransfor::QueryFileUnreadable;
			}
			else
			{
				stream<<QMessageFileTransfor::QueryFileValid<<fileInfo.size();

				QMessageFileTransfor* pTransfor=static_cast<QMessageFileTransfor*>(parent());
				if (pTransfor != NULL)
				{
					QFile* file=new QFile(strFile);
					file->open(QIODevice::ReadOnly);
					pTransfor->addTransforFile(nUsrID, file);
				}
			}
			
			emit finished(m_pSender, QMessageFileTransfor::QueryFile, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfor::AnswerFileInfo:
		{
			int nQueryFlag;
			out>>nQueryFlag;

			Q_ASSERT(nQueryFlag==QMessageFileTransfor::QueryFileNoExist || nQueryFlag==QMessageFileTransfor::QueryFileUnreadable
				|| nQueryFlag==QMessageFileTransfor::QueryFileValid);

			qint64 nFileSize=-1;
			if (nQueryFlag == QMessageFileTransfor::QueryFileValid)
			{
				out>>nFileSize;
			}

			QMetaObject::invokeMethod(m_pView, "recvFileInfo", Q_ARG(int, nUsrID), Q_ARG(int, nQueryFlag), Q_ARG(qint64, nFileSize));
		}
		break;
	case QMessageFileTransfor::QueryTransforFile:
		{
			int nFormat;
			out>>nFormat;

			Q_ASSERT(nFormat==QMessageFileTransfor::TextFormat || nFormat==QMessageFileTransfor::DataFormat);

			QMessageFileTransfor* pTransfor=static_cast<QMessageFileTransfor*>(parent());
			if (pTransfor != NULL)
			{
				QFile* file=pTransfor->transforFile(nUsrID);
				if (file != NULL)
				{
					QByteArray buffer;

					readContents(file, nFormat, buffer);

					emit finished(m_pSender, QMessageFileTransfor::QueryTransforFile, nUsrID, buffer);
				}
			}
		}
		break;
	case QMessageFileTransfor::QueryRetransforFile:
		{
			int nFormat;
			quint64 nPos;
			out>>nFormat>>nPos;

			Q_ASSERT(nFormat==QMessageFileTransfor::TextFormat || nFormat==QMessageFileTransfor::DataFormat);

			QMessageFileTransfor* pTransfor=static_cast<QMessageFileTransfor*>(parent());
			if (pTransfor != NULL)
			{
				QFile* file=pTransfor->transforFile(nUsrID);
				if (file != NULL)
				{
					file->seek(nPos);

					QByteArray buffer;

					readContents(file, nFormat, buffer);

					emit finished(m_pSender, QMessageFileTransfor::QueryTransforFile, nUsrID, buffer);
				}
			}
		}
		break;
	case QMessageFileTransfor::AnswerTransforFile:
		{
			int nSize;
			out>>nSize;

			Q_ASSERT(nSize>=0);
			QByteArray buffer;
			buffer.resize(nSize);

			int nRet=out.writeRawData(buffer.data(), nSize);
			if (nRet != nSize)
			{
				QMetaObject::invokeMethod(m_pView, "recvFileData", Q_ARG(quint32, nUsrID), Q_ARG(bool, false));
			}
			else
			{
				QMetaObject::invokeMethod(m_pView, "recvFileData", Q_ARG(quint32, nUsrID), Q_ARG(bool, true), Q_ARG(QByteArray, buffer));
			}
		}
		break;
	case QMessageFileTransfor::QueryDir:
		{
			QString strDir;
			out>>strDir;

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);

			QDir dir(strDir);
			if (!dir.exists())
			{
				stream<<QMessageFileTransfor::QueryPathNoExist;
			}
			else
			{
				stream<<QMessageFileTransfor::QueryPathValid;

				QVector<FileInfo> vecFileInfo;

				QFileInfoList list=dir.entryInfoList(QDir::NoFilter, QDir::DirsFirst|QDir::Name|QDir::IgnoreCase);
				entryInfoList(list, vecFileInfo);

				stream<<vecFileInfo.size();
				foreach (FileInfo fileInfo, vecFileInfo)
				{
					stream<<fileInfo.type<<fileInfo.name<<fileInfo.size<<fileInfo.isReadable
						<<fileInfo.isWritable<<fileInfo.isExecutable<<fileInfo.lastModified;
				}
			}

			emit finished(m_pSender, QMessageFileTransfor::QueryDir, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfor::QuerySymLink:
		{
			QString strFile;
			out>>strFile;

			Q_ASSERT(!strFile.isEmpty());

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			QFileInfo fileInfo(strFile);
			if (!fileInfo.exists())
			{
				stream<<QMessageFileTransfor::QueryFileNoExist;
			}
			else if (!fileInfo.isReadable())
			{
				stream<<QMessageFileTransfor::QueryFileUnreadable;
			}
			else if (!fileInfo.isSymLink())
			{
				stream<<QMessageFileTransfor::QuerySymLinkNoLink;
			}
			else
			{
				stream<<QMessageFileTransfor::QueryFileValid<<fileInfo.symLinkTarget();
			}

			emit finished(m_pSender, QMessageFileTransfor::QuerySymLink, nUsrID, buffer);
		}
		break;
	case QMessageFileTransfor::AnswerSymLink:
		{
			int nQueryFlag;
			out>>nQueryFlag;

			Q_ASSERT(nQueryFlag==QMessageFileTransfor::QueryFileNoExist || nQueryFlag==QMessageFileTransfor::QueryFileUnreadable
				|| nQueryFlag==QMessageFileTransfor::QuerySymLinkNoLink || nQueryFlag==QMessageFileTransfor::QueryFileValid);

			QString strSymLink;
			if (nQueryFlag == QMessageFileTransfor::QueryFileValid)
			{
				out>>strSymLink;
			}

			QMetaObject::invokeMethod(m_pView, "recvSymLinkInfo", Q_ARG(quint32, nUsrID), 
				Q_ARG(int, nQueryFlag), Q_ARG(QString, strSymLink));
		}
		break;
	default:
		break;
	}
}

void QMessageFileTransforRunnable::entryInfoList(QFileInfoList& list, QVector<FileInfo>& files)
{
	FileInfo fileInfo;
	foreach (QFileInfo info, list)
	{
		if (info.isDir())
		{
			if (!info.isHidden())
			{
				fileInfo.type=QMessageFileTransfor::Dir;
			}
			else
			{
				fileInfo.type=QMessageFileTransfor::HideDir;
			}
		}
		else if (info.isFile())
		{
			if (!info.isHidden())
			{
				fileInfo.type=QMessageFileTransfor::File;
			}
			else
			{
				fileInfo.type=QMessageFileTransfor::HideFile;
			}
		}
		else if (info.isSymLink())
		{
			if (!info.isHidden())
			{
				fileInfo.type=QMessageFileTransfor::SymLink;
			}
			else
			{
				fileInfo.type=QMessageFileTransfor::HideSymLink;
			}
		}

		fileInfo.name=info.fileName();
		fileInfo.size=info.isFile()? info.size():0;
		fileInfo.isReadable=info.isReadable();
		fileInfo.isWritable=info.isWritable();
		fileInfo.isExecutable=info.isExecutable();
		fileInfo.lastModified=info.lastModified();

		files.append(fileInfo);
	}
}

void QMessageFileTransforRunnable::readContents(QFile* file, int format, QByteArray& buffer)
{
	Q_ASSERT_NO_NULL(file);

	if (format == QMessageFileTransfor::TextFormat)
	{
		file->setTextModeEnabled(true);

		QTextStream stream(file);
		if (!stream.atEnd())
		{
			buffer=stream.read(FILE_PACKET_SIZE).toLocal8Bit();
		}
	}
	else
	{
		QDataStream stream(file);
		stream.setVersion(DATA_STREAM_VERSION);

		buffer.resize(FILE_PACKET_SIZE);
		int nSize=stream.readRawData(buffer.data(), FILE_PACKET_SIZE);
		if (nSize != -1)
		{
			buffer.resize(nSize);
		}
		else
		{
			buffer.clear();
		}
	}

	file->setTextModeEnabled(false);
}
