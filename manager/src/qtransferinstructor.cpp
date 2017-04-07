#include "qtransferinstructor.h"

#include "define.h"
#include "common.h"
#include "qmessagefiletransfer.h"
#include "qmessageparser.h"
#include "qfiletransferparser.h"
#include "qfilecontroller.h"
#include "qserverdelegate.h"
#include "qmessageparser.h"
#include "qmessageperformer.h"
#include "qservermanager.h"
#include "qtreeviewtransferqueue.h"
#include "qframegroupsync.h"

QTransferInstructor::QTransferInstructor(QObject *parent)
	: QObject(parent),
	m_pClient(NULL)
{
	m_pClientManager=new QClientManager(new QFileController(this), this);
	connect(m_pClientManager, SIGNAL(connected(QClient*)), this, SLOT(connected(QClient*)));
	connect(m_pClientManager, SIGNAL(disconnected(QClient*)), this, SLOT(disconnected(QClient*)));

	m_nUsrID=QFileTransferParser::instance()->registerInterface(this);
}

QTransferInstructor::~QTransferInstructor()
{
	QFileTransferParser::instance()->unregisterInterface(m_nUsrID);
}

void QTransferInstructor::addTask(const QGroupTransferQueue::TransferQueueItem& task)
{
	m_task=task;

	connectToServer(m_task.sourceHost);
}

void QTransferInstructor::connectToServer(const QServerManager::ServerInfo& info)
{
	QClient* pClient=m_pClientManager->addClient(info);
	Q_ASSERT_NO_NULL(pClient);

	if (pClient == m_pClient)
	{
		if (m_pClientManager->isConnected(pClient))
		{
			connected(pClient);
		}
		else
		{
			disconnected(pClient);
		}
	}
	else
	{
		m_pClientManager->connectToHost(pClient);
	}
}

void QTransferInstructor::connected(QClient* client)
{
	m_pClient=client;
	if (m_pClient != NULL)
	{
		queryConnectToHost(m_task.targetHost);
	}
}

void QTransferInstructor::disconnected(QClient* client)
{
	sendMessage(ConnectToSourceFailed, QString(tr("Connected to %s:%d failed"))
		.arg(client->serverAddr().toString()).arg(client->port()));
}

void QTransferInstructor::queryConnectToHost(const QServerManager::ServerInfo& info)
{
	if (m_pClient == NULL)
	{
		return;
	}

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessagePerformer::Type);
	if (pMsg != NULL)
	{
		QString strSource, strTarget;
		if (!QServerManager::checkConnect(m_task.sourceHost, m_task.targetHost, strSource, strTarget))
		{
			strSource=QServerManager::findFirstAddress(m_task.sourceHost);
			strTarget=QServerManager::findFirstAddress(m_task.targetHost, true);
		}
		emit sendMessage(ConnectingToSource, 
			tr("connecting")+" "+m_task.sourceHost.hostName+":"+strSource+" -> "+m_task.targetHost.hostName+":"+strTarget);

		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		stream<<info;
		buffer=pMsg->query(QMessagePerformer::QueryConnectToTarget, m_nUsrID, buffer);

		m_pClient->sendMessage(buffer, QMessagePerformer::Type);
	}
}

void QTransferInstructor::queryUpload()
{
	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		QString strSource=QTreeViewTransferQueue::appendPathName(m_task.source, m_task.file.name, m_task.isTop);
		QString strTarget=QTreeViewTransferQueue::appendPathName(m_task.target, m_task.file.name, false);
		stream<<m_task.file.type<<strSource<<strTarget;

		QString strDesc=tr("directory");
		if (m_task.file.type == QMessageFileTransfer::File || m_task.file.type == QMessageFileTransfer::HideFile)
		{
			switch (m_task.format)
			{
			case QFrameGroupSync::TextFormat:
				{
					m_task.format=QMessageFileTransfer::TextFormat;
				}
				break;
			case QFrameGroupSync::BinaryFormat:
				{
					m_task.format=QMessageFileTransfer::BinaryFormat;
				}
				break;
			case QFrameGroupSync::AutoFormat:
				{
					if (isTextFormat(strSource))
					{
						m_task.format=QMessageFileTransfer::TextFormat;
					}
					else
					{
						m_task.format=QMessageFileTransfer::BinaryFormat;
					}
				}
				break;
			}

			stream<<m_task.format;

			strDesc=tr("file");
		}
		else if ((m_task.file.type | QMessageFileTransfer::SymLink) != 0 
			|| (m_task.file.type | QMessageFileTransfer::HideSymLink) != 0)
		{
			strDesc=tr("symbol link file");
		}

		emit sendMessage(QueryUpload, tr("Upload")+" "+strDesc+":"+strSource);

		buffer=pMsg->query(QMessagePerformer::QueryUpload, m_nUsrID, buffer);

		bool bRtn=m_pClient->sendMessage(buffer, QMessagePerformer::Type);
		if (!bRtn)
		{
			emit sendMessage(DisconnectedToSourceHost, tr("Disconnected to Source host!"));
		}
	}
}

void QTransferInstructor::recvConnectToHost(int flag)
{
	switch (flag)
	{
	case QMessagePerformer::QueryConnectToTargetFailed:
		{
			emit sendMessage(ConnectToTargetFailed, tr("Connection failed!"));
		}
		break;
	case QMessagePerformer::QueryConnectToTargetSuccess:
		{
			emit sendMessage(ConnectToTargetSuccess, tr("Connection succeed!"));

			queryUpload();
		}
		break;
	default:
		qDebug("QTransferInstructor::recvConnectToHost invalid flag!!!");
	}
}

void QTransferInstructor::recvSourceUpload(int flag, QByteArray buffer)
{
	switch (flag)
	{
	case QMessagePerformer::TargetDisconnected:
		{
			QString strSource, strTarget;
			if (!QServerManager::checkConnect(m_task.sourceHost, m_task.targetHost, strSource, strTarget))
			{
				strSource=QServerManager::findFirstAddress(m_task.sourceHost);
				strTarget=QServerManager::findFirstAddress(m_task.targetHost, true);
			}
			emit sendMessage(UploadFail, 
				tr("Disconnected")+" "+m_task.sourceHost.hostName+":"+strSource+" -> "+m_task.targetHost.hostName+":"+strTarget);
		}
		break;
	case QMessagePerformer::QuerySourceDirLost:
		{
			emit sendMessage(UploadFail, tr("Source: Directory")+" "+
				QTreeViewTransferQueue::appendPathName(m_task.source, m_task.file.name, m_task.isTop)+" "+tr("not existed!"));
		}
		break;
	case QMessagePerformer::QuerySourceFileReadLost:
		{
			emit sendMessage(UploadFail, tr("Source: File")+" "+
				QTreeViewTransferQueue::appendPathName(m_task.source, m_task.file.name, m_task.isTop)+" "+tr("not existed!"));
		}
		break;
	case QMessagePerformer::QuerySourceFileReadFail:
		{
			emit sendMessage(UploadFail, tr("Source: Read file")+" "+
				QTreeViewTransferQueue::appendPathName(m_task.source, m_task.file.name, m_task.isTop)+" "+tr("failed!"));
		}
		break;
	case QMessagePerformer::QueryUploadDirSuccess:
		{
			QDataStream out(buffer);
			out.setVersion(DATA_STREAM_VERSION);

			QVector<FileInfo> files;
			FileInfo file;
			int nNum;
			out>>nNum;
			for (int i=0; i<nNum; i++)
			{
				out>>file;

				files.append(file);
			}
			
			emit addSubTask(m_task, files);

			emit sendMessage(UploadSuccess, tr("Target: Make directory")+" "+
				QTreeViewTransferQueue::appendPathName(m_task.target, m_task.file.name, false)+" "+tr("success."));
		}
		break;
	case QMessagePerformer::QueryTargetCreateFileFail:
		{
			emit sendMessage(UploadFail, tr("Target: Create file")+" "+
				QTreeViewTransferQueue::appendPathName(m_task.target, m_task.file.name, false)+TEMP_FILE_SUFFIX+" "+tr("failed!"));
		}
		break;
// 	case QMessagePerformer::QuerySourceTransferFileBengin:
// 		{
// 			emit sendMessage(QueryUpload, tr("Source: Begin to transfer file")+" "+
// 				QTreeViewTransferQueue::appendPathName(m_task.source, m_task.file.name, m_task.isTop));
// 		}
// 		break;
	case QMessagePerformer::QueryUploadSymLinkFail:
		{
			emit sendMessage(UploadFail, tr("Target: Create symbol link file")+" "+
				QTreeViewTransferQueue::appendPathName(m_task.target, m_task.file.name, false)+" "+tr("failed!"));
		}
		break;
	case QMessagePerformer::QueryUploadSymLinkSuccess:
		{
			emit sendMessage(UploadSuccess, tr("Target: Create symbol link file")+" "+
				QTreeViewTransferQueue::appendPathName(m_task.target, m_task.file.name, false)+" "+tr("success."));
		}
		break;
	case QMessagePerformer::QueryTargetFileWriteFail:
		{
			emit sendMessage(UploadFail, tr("Target: Write to file")+" "+
				QTreeViewTransferQueue::appendPathName(m_task.target, m_task.file.name, false)+" "+tr("failed!"));
		}
		break;
	case QMessagePerformer::QueryUploadFileSuccess:
		{
			emit sendMessage(UploadSuccess, tr("Source: Transfer file")+" "+
				QTreeViewTransferQueue::appendPathName(m_task.source, m_task.file.name, m_task.isTop)+" "+tr("success."));
		}
		break;
	case QMessagePerformer::QueryUploadFileFail:
		{
			emit sendMessage(UploadFail, tr("Target: Save to file")+" "+
				QTreeViewTransferQueue::appendPathName(m_task.target, m_task.file.name, false)+" "+tr("failed!"));
		}
		break;
	case QMessagePerformer::QueryTargetFileWriteLost:
		{
			emit sendMessage(UploadFail, tr("Target:")+" "+
				QTreeViewTransferQueue::appendPathName(m_task.target, m_task.file.name, false)+TEMP_FILE_SUFFIX+" "+tr("deleted!"));
		}
		break;
	default:
		qDebug("QTransferInstructor::recvSourceUpload invalid flag!!!");
		break;
	}
}
