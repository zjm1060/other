#include "qmessageperformer.h"

#include <QDir>

#include "qfilecontroller.h"
#include "qmessageparser.h"
#include "qmessagefiletransfor.h"
#include "qfiletransferparser.h"

QMessagePerformer::QMessagePerformer(QObject *parent)
	: QObject(parent)
{

}

QMessagePerformer::~QMessagePerformer()
{

}

QByteArray QMessagePerformer::query(int msgType, quint32 usrID, const QByteArray& contents)
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

bool QMessagePerformer::parse(QConnection* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	if (!m_mapPerformer.contains(sender))
	{
		QTempPerformer* pPerformer=new QTempPerformer(sender, view, this);
		m_mapPerformer.insert(sender, pPerformer);
	}

	m_mapPerformer[sender]->parse(buffer);

	return true;
}

QTempPerformer::QTempPerformer(QConnection* sender, QObject* view, QObject* parent)
:QObject(parent)
{
	m_pSender=sender;
	m_pView=view;

	m_pClientManager=new QClientManager(new QFileController(this), this);

	connect(m_pClientManager, SIGNAL(connected(QClient*)), this, SLOT(connected(QClient*)));
	connect(m_pClientManager, SIGNAL(disconnected(QClient*)), this, SLOT(disconnected(QClient*)));
}

QTempPerformer::~QTempPerformer()
{

}

void QTempPerformer::parse(const QByteArray& buffer)
{
	QDataStream out(buffer);
	out.setVersion(DATA_STREAM_VERSION);

	int nMsgType;
	quint32	nUsrID;
	out>>nMsgType>>nUsrID;

	switch (nMsgType)
	{
	case QMessagePerformer::QueryConnectToTarget:
		{
			QServerManager::ServerInfo info;
			out>>info;

			if (m_mapClient.contains(nUsrID))
			{
				m_mapClient[nUsrID].client=m_pClientManager->addClient(info);
			}
			else
			{
				ClientInfo client;
				client.client=m_pClientManager->addClient(info);
				client.transferInterface=new QPerformerFileTransfer(this);

				connect(client.transferInterface, SIGNAL(transferStatus(int, QByteArray)), 
					this, SLOT(transferStatus(int, QByteArray)));

				m_mapClient.insert(nUsrID, client);
			}

			m_mapClient[nUsrID].queryConnect=true;

			m_pClientManager->connectToHost(m_mapClient[nUsrID].client);
		}
		break;
	case QMessagePerformer::AnswerConnectToTarget:
		{
			int nStatus;
			out>>nStatus;

			QMetaObject::invokeMethod(m_pView, "recvConnectToHost", Q_ARG(quint32, nUsrID), Q_ARG(int, nStatus));
		}
		break;
	case QMessagePerformer::QueryUpload:
		{
			if (!m_mapClient.contains(nUsrID))
			{
				return;
			}

			QClient* pClient=m_mapClient[nUsrID].client;

			int nFileType;
			QString strSourceAddr,strTargetAddr;

			out>>nFileType>>strSourceAddr>>strTargetAddr;

			QByteArray buf;
			QDataStream stream(&buf, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);
			stream<<nFileType<<strTargetAddr;

			switch (nFileType)
			{
			case QMessageFileTransfer::Dir:
			case QMessageFileTransfer::HideDir:
				{
					QDir dir(strSourceAddr);
					if (!dir.exists())
					{
						sendMessageToController(nUsrID, QMessagePerformer::QueryUpload, QMessagePerformer::QuerySourceDirLost);
						return;
					}

					m_mapClient[nUsrID].transferInterface->setSourceDir(strSourceAddr);
				}
				break;
			case QMessageFileTransfer::File:
			case QMessageFileTransfer::HideFile:
				{
					int nFormat;
					out>>nFormat;

					if (QFile::exists(strSourceAddr))
					{
						QFile* file=new QFile(strSourceAddr);
						QIODevice::OpenMode openMode=QIODevice::ReadOnly;
						if (nFormat == QMessageFileTransfer::TextFormat)
						{
							openMode|=QIODevice::Text;
						}
						if (file->open(openMode))
						{
							m_mapClient[nUsrID].transferInterface->setSourceFile(file);
							stream<<nFormat;
						}
						else
						{
							SAFE_CLOSE_FILE(file);
							sendMessageToController(nUsrID, QMessagePerformer::QueryUpload, QMessagePerformer::QuerySourceFileReadFail);
							return;
						}
					}
					else
					{
						sendMessageToController(nUsrID, QMessagePerformer::QueryUpload, QMessagePerformer::QuerySourceFileReadLost);

						return;
					}
				}
				break;
			case QMessageFileTransfer::SymLinkDir:
			case QMessageFileTransfer::SymLinkHideDir:
			case QMessageFileTransfer::SymLinkFile:
			case QMessageFileTransfer::SymLinkHideFile:
				{
					if (QFile::exists(strSourceAddr))
					{
						stream<<QFile::symLinkTarget(strSourceAddr);
					}
					else
					{
						sendMessageToController(nUsrID, QMessagePerformer::QueryUpload, QMessagePerformer::QuerySourceFileReadLost);

						return;
					}
				}
				break;
			default:
				qDebug("QMessagePerformer::QueryUpload invalid file type!!!");
				break;
			}

			QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
			Q_ASSERT_NO_NULL(pMsg);
			buf=pMsg->query(QMessageFileTransfer::QueryUpload, m_mapClient[nUsrID].transferInterface->user(), buf);
			bool bRtn=pClient->sendMessage(buf, QMessageFileTransfer::Type);
			if (!bRtn)
			{
				sendMessageToController(nUsrID, QMessagePerformer::QueryUpload, QMessagePerformer::TargetDisconnected);
			}
		}
		break;
	case QMessagePerformer::AnswerUpload:
		{
			int nStatus;
			out>>nStatus;

			QByteArray buffer;
			if (nStatus == QMessagePerformer::QueryUploadDirSuccess)
			{
				int nSize;
				out>>nSize;
				buffer.resize(nSize);

				out.readRawData(buffer.data(), nSize);
			}

			QMetaObject::invokeMethod(m_pView, "recvSourceUpload", Q_ARG(quint32, nUsrID),
				Q_ARG(int, nStatus), Q_ARG(QByteArray, buffer));
		}
		break;
	default:
		break;
	}
}

void QTempPerformer::connected(QClient* client)
{
	QMutableMapIterator<quint32, ClientInfo> it(m_mapClient);
	while (it.hasNext())
	{
		it.next();

		ClientInfo& info=it.value();
		if (info.queryConnect && (info.client == client))
		{
			info.queryConnect=false;

			quint32 nUsrID=it.key();

			sendMessageToController(nUsrID, QMessagePerformer::QueryConnectToTarget, QMessagePerformer::QueryConnectToTargetSuccess);
		}
	}
}

void QTempPerformer::disconnected(QClient* client)
{
	QMutableMapIterator<quint32, ClientInfo> it(m_mapClient);
	while (it.hasNext())
	{
		it.next();

		ClientInfo& info=it.value();
		if (info.queryConnect && info.client == client)
		{
			info.queryConnect=false;

			quint32 nUserId=it.key();

			sendMessageToController(nUserId, QMessagePerformer::QueryConnectToTarget, QMessagePerformer::QueryConnectToTargetFailed);
		}
	}
}

void QTempPerformer::transferStatus(int status, QByteArray buffer)
{
	QObject* pSender=sender();

	quint32 nUsrID=-1;
	QMutableMapIterator<quint32, ClientInfo> it(m_mapClient);
	while (it.hasNext())
	{
		it.next();

		ClientInfo& info=it.value();
		if (info.transferInterface == pSender)
		{
			nUsrID=it.key();
		}
	}

	if (nUsrID == -1)
	{
		return;
	}

	switch (status)
	{
	case QMessagePerformer::QueryUploadDirSuccess:
		sendMessageToController(nUsrID, QMessagePerformer::QueryUpload, status, buffer);
		break;
	case QMessagePerformer::QuerySourceDirLost:
	case QMessagePerformer::QueryTargetMakeDirFail:
	case QMessagePerformer::QuerySourceFileReadLost:
	case QMessagePerformer::QuerySourceFileReadFail:
	case QMessagePerformer::QueryTargetCreateFileFail:
	case QMessagePerformer::QuerySourceTransferFileBengin:
	case QMessagePerformer::QuerySourceTransferFileFinish:
	case QMessagePerformer::QueryTargetFileWriteLost:
	case QMessagePerformer::QueryTargetFileWriteFail:
	case QMessagePerformer::QueryTargetFileWriteSuccess:
	case QMessagePerformer::QueryUploadFileSuccess:
	case QMessagePerformer::QueryUploadFileFail:
	case QMessagePerformer::QueryUploadSymLinkSuccess:
	case QMessagePerformer::QueryUploadSymLinkFail:
		sendMessageToController(nUsrID, QMessagePerformer::QueryUpload, status);
		break;
	case QMessagePerformer::QuerySourceTransferFile:
		{
			QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
			Q_ASSERT_NO_NULL(pMsg);
			QByteArray buf=pMsg->query(QMessageFileTransfer::QueryUploadFile, m_mapClient[nUsrID].transferInterface->user(), buffer);
			bool bRtn=m_mapClient[nUsrID].client->sendMessage(buf, QMessageFileTransfer::Type);
			if (!bRtn)
			{
				sendMessageToController(nUsrID, QMessagePerformer::QueryUpload, QMessagePerformer::TargetDisconnected);
			}
		}
		break;
	default:
		qDebug("QTempPerformer::transferStatus invalid status!!!");
	}
}

void QTempPerformer::sendMessageToController(quint32 usrID, int msgType, int status, const QByteArray& buffer)
{
	QByteArray buf;
	QDataStream in(&buf, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	switch (msgType)
	{
	case QMessagePerformer::QueryConnectToTarget:
		{
			in<<QMessagePerformer::AnswerConnectToTarget;
		}
		break;
	case QMessagePerformer::QueryUpload:
		{
			in<<QMessagePerformer::AnswerUpload;
		}
		break;
	}
	in<<usrID<<status;
	if (!buffer.isEmpty())
	{
		in<<buffer.size();
		in.writeRawData(buffer.data(), buffer.size());
	}

	m_pSender->sendMessage(buf, QMessagePerformer::Type);
}

QPerformerFileTransfer::QPerformerFileTransfer(QObject* parent)
:QObject(parent),
m_pFile(NULL)
{
	m_nUsrID=QFileTransferParser::instance()->registerInterface(this);
}

QPerformerFileTransfer::~QPerformerFileTransfer()
{
	SAFE_CLOSE_FILE(m_pFile);

	QFileTransferParser::instance()->unregisterInterface(m_nUsrID);
}

void QPerformerFileTransfer::setSourceDir(const QString& dir)
{
	m_strDir=dir;
}

void QPerformerFileTransfer::setSourceFile(QFile* file)
{
	SAFE_CLOSE_FILE(m_pFile);
	m_pFile=file;
}

quint32 QPerformerFileTransfer::user()
{
	return m_nUsrID;
}

void QPerformerFileTransfer::recvUpload(int flag)
{
	switch (flag)
	{
	case QMessageFileTransfer::QueryMakePathFail:
		{
			emit transferStatus(QMessagePerformer::QueryTargetMakeDirFail);
		}
		break;
	case QMessageFileTransfer::QueryMakePathSuccess:
		{
			QDir dir(m_strDir);
			if (dir.exists())
			{
				QByteArray buffer;
				QDataStream stream(&buffer, QIODevice::WriteOnly);
				stream.setVersion(DATA_STREAM_VERSION);

				QVector<FileInfo> vecFileInfo;

				QFileInfoList list=dir.entryInfoList(QDir::AllEntries | QDir::System | QDir::NoDotAndDotDot, 
					QDir::DirsFirst | QDir::Name |QDir::IgnoreCase);
				QMessageFileTransferRunnable::entryInfoList(list, vecFileInfo);

				stream<<vecFileInfo.size();
				foreach (FileInfo fileInfo, vecFileInfo)
				{
					stream<<fileInfo;
				}

				emit transferStatus(QMessagePerformer::QueryUploadDirSuccess, buffer);
			}
			else
			{
				emit transferStatus(QMessagePerformer::QuerySourceDirLost);
			}
		}
		break;
	case QMessageFileTransfer::QueryCreateFileFail:
		{
			emit transferStatus(QMessagePerformer::QueryTargetCreateFileFail);
		}
		break;
	case QMessageFileTransfer::QueryCreateFileSuccess:
		{
			emit transferStatus(QMessagePerformer::QuerySourceTransferFileBengin);

			transferFile();
		}
		break;
	case QMessageFileTransfer::QueryUploadSymLinkFail:
		{
			emit transferStatus(QMessagePerformer::QueryUploadSymLinkFail);
		}
		break;
	case QMessageFileTransfer::QueryUploadSymLinkSuccess:
		{
			emit transferStatus(QMessagePerformer::QueryUploadSymLinkSuccess);
		}
		break;
	default:
		break;
	}
}

void QPerformerFileTransfer::recvUploadFile(int flag)
{
	switch (flag)
	{
	case QMessageFileTransfer::QueryUploadFileWriteSuccess:
		{
			transferFile();
		}
		break;
	case QMessageFileTransfer::QueryUploadFileWriteFail:
		{
			SAFE_CLOSE_FILE(m_pFile);

			emit transferStatus(QMessagePerformer::QueryTargetFileWriteFail);
		}
		break;
	case QMessageFileTransfer::QueryUploadFileSuccess:
		{
			emit transferStatus(QMessagePerformer::QueryUploadFileSuccess);
		}
		break;
	case QMessageFileTransfer::QueryUploadFileFail:
		{
			SAFE_CLOSE_FILE(m_pFile);

			emit transferStatus(QMessagePerformer::QueryUploadFileFail);
		}
		break;
	case QMessageFileTransfer::QueryUploadFileWriteLost:
		{
			SAFE_CLOSE_FILE(m_pFile);

			emit transferStatus(QMessagePerformer::QueryTargetFileWriteLost);
		}
		break;
	default:
		break;
	}
}

void QPerformerFileTransfer::transferFile()
{
	if (m_pFile == NULL)
	{
		return;
	}

	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);
	stream.setVersion(DATA_STREAM_VERSION);

	QByteArray contents;
	int nStatus=QMessageFileTransferRunnable::readContents(m_pFile, contents);
	if (nStatus == QMessageFileTransfer::TransferReadError)
	{
		SAFE_CLOSE_FILE(m_pFile);

		emit transferStatus(QMessagePerformer::QuerySourceFileReadFail);
	}
	else
	{
		if (nStatus == QMessageFileTransfer::TransferFinish)
		{
			SAFE_CLOSE_FILE(m_pFile);
		}

		stream<<nStatus<<contents.size();
		stream.writeRawData(contents.data(), contents.size());

		emit transferStatus(QMessagePerformer::QuerySourceTransferFile, buffer);
	}
}
