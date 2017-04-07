#include "qcontrolledview.h"

#include <QDir>

#include "qconnectionmanager.h"
#include "qreceiverparser.h"
#include "qfilecontroller.h"
#include "qimessage.h"
#include "qmessageparser.h"
#include "qlogger.h"

QControlledView::QControlledView(QObject *parent)
	: QObject(parent)
{

}

QControlledView::~QControlledView()
{

}

void QControlledView::queryConnectToTarget(QObject* sender, quint32 usrID, ServerInfo info)
{
	if (!m_mapPerformer.contains(sender))
	{
		QTempPerformer* pPerformer=new QTempPerformer(sender, this);
		m_mapPerformer.insert(sender, pPerformer);
	}

	m_mapPerformer[sender]->queryConnectToTarget(usrID, info);
}

void QControlledView::queryUploadDirToTarget(QObject* sender, quint32 usrID, int fileType, QString sourcePath, QString targetPath, bool slient)
{
	if (m_mapPerformer.contains(sender))
	{
		m_mapPerformer[sender]->queryUploadDirToTarget(usrID, fileType, sourcePath, targetPath, slient);
	}
}

void QControlledView::queryUploadSymlinkToTarget(QObject* sender, quint32 usrID, int fileType, QString sourcePath, QString targetPath, bool slient)
{
	if (m_mapPerformer.contains(sender))
	{
		m_mapPerformer[sender]->queryUploadSymlinkToTarget(usrID, fileType, sourcePath, targetPath, slient);
	}
}

void QControlledView::queryUploadFileToTarget(QObject* sender, quint32 usrID, int fileType, QString sourcePath, QString targetPath, int format, bool slient)
{
	if (m_mapPerformer.contains(sender))
	{
		m_mapPerformer[sender]->queryUploadFileToTarget(usrID, fileType, sourcePath, targetPath, format, slient);
	}
}

void QControlledView::queryInterruptTransferToTarget(QObject* sender, quint32 usrID)
{
	if (m_mapPerformer.contains(sender))
	{
		m_mapPerformer[sender]->queryInterruptTransferToTarget(usrID);
	}
}

void QControlledView::queryExecSyncCommandToTarget(QObject* sender, quint32 usrID, QString workspace, QVector<FileInfo> files)
{
	if (m_mapPerformer.contains(sender))
	{
		m_mapPerformer[sender]->queryExecSyncCommandToTarget(usrID, workspace, files);
	}
}

QTempPerformer::QTempPerformer(QObject* sender, QObject* parent)
:QObject(parent)
{
	m_pSender=sender;

	QSharedControllerPtr pController(new QFileController);
	m_pClientManager=new QClientManager(pController, this);

	connect(m_pClientManager, SIGNAL(connected(QClient*)), this, SLOT(connected(QClient*)));
	connect(m_pClientManager, SIGNAL(connectFailed(QClient*)), this, SLOT(connectFailed(QClient*)));
	connect(m_pClientManager, SIGNAL(disconnected(QClient*)), this, SLOT(disconnected(QClient*)));
}

QTempPerformer::~QTempPerformer()
{

}

void QTempPerformer::queryConnectToTarget(quint32 usrID, const ServerInfo& info)
{
	if (m_mapClient.contains(usrID))
	{
		m_mapClient[usrID].client=m_pClientManager->addClient(info);
	}
	else
	{
		ClientInfo client;
		client.client=m_pClientManager->addClient(info);
		client.transferInterface=new QPerformerFileTransfer(this);

		connect(client.transferInterface, SIGNAL(transferStatus(int, QByteArray)), this, SLOT(transferStatus(int, QByteArray)));

		m_mapClient.insert(usrID, client);
	}

	m_mapClient[usrID].queryConnect=true;

	m_pClientManager->connectToHost(m_mapClient[usrID].client);
}

void QTempPerformer::queryUploadDirToTarget(quint32 usrID, int fileType, const QString& sourcePath, const QString& targetPath, bool slient)
{
	if (!m_mapClient.contains(usrID))
	{
		return;
	}

	QClient* pClient=m_mapClient[usrID].client;
	if (pClient == NULL)
	{
		return;
	}

	Q_ASSERT(fileType==Dir || fileType==HideDir);
	QDir dir(sourcePath);
	if (!dir.exists())
	{
		sendMessageToController(usrID, slient? QueryUploadSlientDirToTarget:QueryUploadDirToTarget, QueryPathNoExist);
		return;
	}

	QPerformerFileTransfer::Task task;
	task.uid=usrID;
	task.fileType=fileType;
	task.sourcePath=sourcePath;
	task.targetPath=targetPath;

	m_mapClient[usrID].transferInterface->addTask(task);

	Q_WRITE_ONLY_DATA_STREAM(stream, buffer)
	
	stream<<fileType<<targetPath;

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	Q_ASSERT_NO_NULL(pMsg);
	QByteArray buf=pMsg->query(slient? QueryUploadSlientDir:QueryUploadDir, task.usrID, buffer);
	bool bRtn=pClient->sendMessage(buf, MSG_FILE_TRANSFER);
	if (!bRtn)
	{
		sendMessageToController(usrID, QueryConnectToTarget, TargetDisconnected);
	}
	else if (slient)
	{
		Q_WRITE_ONLY_DATA_STREAM(s, buf)

		QVector<FileInfo> files;

		QFileInfoList list=dir.entryInfoList(QDir::AllEntries | QDir::System | QDir::NoDotAndDotDot, 
			QDir::DirsFirst|QDir::Name|QDir::IgnoreCase);
		entryInfoList(list, files, false);

		s<<task.sourcePath<<task.targetPath<<files;

		sendMessageToController(usrID, QueryUploadSlientDirToTarget, QueryMakePathSuccess, buf);
	}
}

void QTempPerformer::queryUploadSymlinkToTarget(quint32 usrID, int fileType, const QString& sourcePath, const QString& targetPath, bool slient)
{
	if (!m_mapClient.contains(usrID))
	{
		return;
	}

	QClient* pClient=m_mapClient[usrID].client;
	if (pClient == NULL)
	{
		return;
	}

	Q_ASSERT(fileType==SymLinkDir || fileType==SymLinkHideDir || fileType==SymLinkFile || fileType==SymLinkHideFile);
	QFileInfo info(sourcePath);
	if (!info.exists())
	{
		sendMessageToController(usrID, slient? QueryUploadSlientSymlinkToTarget:QueryUploadSymlinkToTarget, QueryFileNoExist);
		return;
	}

	if (!info.isSymLink())
	{
		sendMessageToController(usrID, slient? QueryUploadSlientSymlinkToTarget:QueryUploadSymlinkToTarget, QuerySymLinkNoLink);
		return;
	}

	QPerformerFileTransfer::Task task;
	task.uid=usrID;
	task.fileType=fileType;
	task.sourcePath=sourcePath;
	task.targetPath=targetPath;

	m_mapClient[usrID].transferInterface->addTask(task);

	Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

	stream<<fileType<<targetPath<<info.symLinkTarget();

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	Q_ASSERT_NO_NULL(pMsg);
	QByteArray buf=pMsg->query(slient? QueryUploadSlientDir:QueryUploadDir, task.usrID, buffer);
	bool bRtn=pClient->sendMessage(buf, MSG_FILE_TRANSFER);
	if (!bRtn)
	{
		sendMessageToController(usrID, QueryConnectToTarget, TargetDisconnected);
	}
	else if (slient)
	{
		sendMessageToController(usrID, QueryUploadSlientSymlinkToTarget, QueryCreateSymLinkSuccess);
	}
}

void QTempPerformer::queryUploadFileToTarget(quint32 usrID, int fileType, QString sourcePath, QString targetPath, int format, bool slient)
{
	if (!m_mapClient.contains(usrID))
	{
		return;
	}

	QClient* pClient=m_mapClient[usrID].client;
	if (pClient == NULL)
	{
		return;
	}

	Q_ASSERT(fileType==File || fileType==HideFile);
	QFileInfo info(sourcePath);
	if (!info.exists())
	{
		sendMessageToController(usrID, QueryUploadFileToTarget, QueryFileNoExist);
		return;
	}

	QPerformerFileTransfer::Task task;
	task.uid=usrID;
	task.fileType=fileType;
	task.format=format;
	task.sourcePath=sourcePath;
	task.targetPath=targetPath;

	m_mapClient[usrID].transferInterface->addTask(task);

	Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

	stream<<targetPath<<sourcePath<<format;

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	Q_ASSERT_NO_NULL(pMsg);
	QByteArray buf=pMsg->query(slient? QueryUploadSlientFile:QueryUploadFile, task.usrID, buffer);
	if (!slient)
	{
		bool bRtn=pClient->sendMessage(buf, MSG_FILE_TRANSFER);
		if (!bRtn)
		{
			sendMessageToController(usrID, QueryConnectToTarget, TargetDisconnected);
		}
	}
	else
	{
		m_pClientManager->parseMessage(pClient, MSG_FILE_TRANSFER, buf);
	}
}

void QTempPerformer::queryInterruptTransferToTarget(quint32 usrID)
{
	if (!m_mapClient.contains(usrID))
	{
		return;
	}

	QClient* pClient=m_mapClient[usrID].client;
	QPerformerFileTransfer* pTransfer=m_mapClient[usrID].transferInterface;
	if (pClient != NULL && pTransfer != NULL)
	{
		QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
		Q_ASSERT_NO_NULL(pMsg);
		QByteArray buf=pMsg->query(QueryInterruptTransfer, pTransfer->task().usrID);
		bool bRtn=pClient->sendMessage(buf, MSG_FILE_TRANSFER);
		if (!bRtn)
		{
			sendMessageToController(usrID, QueryConnectToTarget, TargetDisconnected);
		}
	}
}

void QTempPerformer::queryExecSyncCommandToTarget(quint32 usrID, const QString& workspace, const QVector<FileInfo>& files)
{
	if (!m_mapClient.contains(usrID))
	{
		return;
	}

	QClient* pClient=m_mapClient[usrID].client;
	QPerformerFileTransfer* pTransfer=m_mapClient[usrID].transferInterface;
	if (pClient != NULL && pTransfer != NULL)
	{
		QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_CONFIGURE);
		Q_ASSERT_NO_NULL(pMsg);

		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
		stream<<workspace<<files;

		buffer=pMsg->query(QueryExecSyncConfigureCommand, pTransfer->task().usrID, buffer);

		bool bRtn=pClient->sendMessage(buffer, MSG_CONFIGURE);
		if (!bRtn)
		{
			sendMessageToController(usrID, QueryConnectToTarget, TargetDisconnected);
		}
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

			sendMessageToController(nUsrID, QueryConnectToTarget, QueryConnectToTargetSuccess);
		}
	}
}

void QTempPerformer::connectFailed(QClient* client)
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

			sendMessageToController(nUserId, QueryConnectToTarget, QueryConnectToTargetFailed);
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

			sendMessageToController(nUserId, QueryConnectToTarget, TargetDisconnected);
		}
	}
}

void QTempPerformer::transferStatus(int status, QByteArray buffer)
{
	QObject* pSender=sender();

	quint32 nUsrID=QReceiverParser::AllUser;
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

	if (nUsrID == QReceiverParser::AllUser)
	{
		return;
	}

	switch (status)
	{
	case QueryMakePathFail:
	case QueryMakePathSuccess:
		sendMessageToController(nUsrID, QueryUploadDirToTarget, status, buffer);
		break;
	case QueryCreateSymLinkFail:
	case QueryCreateSymLinkSuccess:
		sendMessageToController(nUsrID, QueryUploadSymlinkToTarget, status);
		break;
	case QuerySourceFileOpenFail:
	case QuerySourceFileReadLost:
	case QuerySourceFileReadFail:
	case QuerySourceTransferFile:
	case QuerySourceTransferFileFinish:
	case QuerySourceSendFlowControl:
	case QueryTargetFileCreateFail:
	case QueryTargetFileWriteLost:
	case QueryTargetFileWriteFinish:
	case QueryTargetFileWriteSuccess:
	case QueryTargetFileChangePermissionFail:
	case QueryTargetFileWriteFail:
		sendMessageToController(nUsrID, QueryUploadFileToTarget, status, buffer);
		break;
	case QueryCommandFoundProcessesFail:
	case QueryCommandKillProcessFail:
	case QueryCommandCreateProcessFail:
	case QueryCommandFoundCommandFail:
	case QueryCommandExecCommand:
	case QueryCommandFinish:
		sendMessageToController(nUsrID, QueryExecSyncCommandToTarget, status, buffer);
		break;
	default:
		qDebug("QTempPerformer::transferStatus invalid status!!!");
		break;
	}
}

void QTempPerformer::sendMessageToController(quint32 usrID, int msgType, int status, const QByteArray& buffer)
{
	Q_ASSERT_NO_NULL(m_pSender);

	Q_WRITE_ONLY_DATA_STREAM(in, buf)

	switch (msgType)
	{
	case QueryConnectToTarget:
		{
			in<<AnswerConnectToTarget;
		}
		break;
	case QueryUploadDirToTarget:
		{
			in<<AnswerUploadDirToTarget;
		}
		break;
	case QueryUploadSymlinkToTarget:
		{
			in<<AnswerUploadSymlinkToTarget;
		}
		break;
	case QueryUploadFileToTarget:
		{
			in<<AnswerUploadFileToTarget;
		}
		break;
	case QueryUploadSlientDirToTarget:
		{
			in<<AnswerUploadSlientDirToTarget;
		}
		break;
	case QueryUploadSlientSymlinkToTarget:
		{
			in<<AnswerUploadSlientSymlinkToTarget;
		}
		break;
	case QueryExecSyncCommandToTarget:
		{
			in<<AnswerExecSyncCommandToTarget;
		}
		break;
	default:
		break;
	}

	in<<usrID<<status;
	if (!buffer.isEmpty())
	{
		in<<buffer.size();
		in.writeRawData(buffer.data(), buffer.size());
	}

	QObjectPtr ptr=QConnectionManager::instance()->connection(m_pSender);
	if (ptr != NULL)
	{
		QMetaObject::invokeMethod(m_pSender, "sendMessage", Q_ARG(QByteArray, buf), Q_ARG(int, MSG_CONTROLLED));
	}
	else
	{
		//与控制端断开连接，取消文件传输
		if (m_mapClient.contains(usrID))
		{
			QClient* pClient=m_mapClient[usrID].client;
			QPerformerFileTransfer* pTransfer=m_mapClient[usrID].transferInterface;
			if (pClient != NULL && pTransfer != NULL)
			{
				QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
				Q_ASSERT_NO_NULL(pMsg);
				QByteArray buf=pMsg->query(QueryInterruptTransfer, pTransfer->task().usrID);
				pClient->sendMessage(buf, MSG_FILE_TRANSFER);
			}
		}
	}
}

QPerformerFileTransfer::QPerformerFileTransfer(QObject* parent)
:QObject(parent)
{
	m_task.usrID=QReceiverParser::AllUser;
}

QPerformerFileTransfer::~QPerformerFileTransfer()
{
	if (m_task.usrID != QReceiverParser::AllUser)
	{
		QReceiverParser::instance()->unregisterInterface(m_task.usrID);
	}
}

void QPerformerFileTransfer::addTask(QPerformerFileTransfer::Task& task)
{
	if (m_task.usrID != QReceiverParser::AllUser)
	{
		QReceiverParser::instance()->unregisterInterface(m_task.usrID);
	}

	m_task=task;

	m_task.usrID=task.usrID=QReceiverParser::instance()->registerInterface(this);
}

const QPerformerFileTransfer::Task& QPerformerFileTransfer::task() const
{
	return m_task;
}

void QPerformerFileTransfer::recvUploadDirAndSymlink(int flag)
{
	Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

	if (flag == QueryMakePathSuccess)
	{
		QDir dir(m_task.sourcePath);

		QVector<FileInfo> files;

		QFileInfoList list=dir.entryInfoList(QDir::AllEntries | QDir::System | QDir::NoDotAndDotDot, 
			QDir::DirsFirst|QDir::Name|QDir::IgnoreCase);
		entryInfoList(list, files, false);

		stream<<m_task.sourcePath<<m_task.targetPath<<files;
	}

	emit transferStatus(flag, buffer);
}

void QPerformerFileTransfer::recvUploadFile(int queryFlag, int flag, quint64 size)
{
	Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

	int nStatus=QueryTargetFileWriteSuccess;

	switch (queryFlag)
	{
	case QueryFileOpenSuccess:
		{
			if (flag == QueryFileOpenFail)
			{
				nStatus=QuerySourceFileOpenFail;
			}
		}
		break;
	case QueryFileOpenFail:
		{
			nStatus=QueryTargetFileCreateFail;
		}
		break;
	case QueryFileWriteSuccess:
		{
			if (flag == QueryFileReadLost)
			{
				nStatus=QuerySourceFileReadLost;
			}
			else if (flag == QueryFileReadFail)
			{
				nStatus=QuerySourceFileReadFail;
			}
			else if (flag == QueryFileReadSuccess)
			{
				nStatus=QuerySourceTransferFile;

				stream<<size;
			}
			else if (flag == QueryFileReadFinish)
			{
				nStatus=QuerySourceTransferFileFinish;
			}
			else if (flag == QueryFileSendFlowControl)
			{
				nStatus=QuerySourceSendFlowControl;
			}
		}
		break;
	case QueryFileWriteFail:
	case QueryFileRenameFail:
		{
			nStatus=QueryTargetFileWriteFail;
		}
		break;
	case QueryFileWriteLost:
		{
			nStatus=QueryTargetFileWriteLost;
		}
		break;
	case QueryFileWriteFinish:
	case QueryFileTransferFinish:
		{
			nStatus=QueryTargetFileWriteFinish;
		}
		break;
	case QueryFileChangePermissionFail:
		{
			nStatus=QueryTargetFileChangePermissionFail;
		}
		break;
	default:
		logplusWarning(LIBNETWORK_LOGGER_STR, QString("QPerformerFileTransfer::recvUploadFile invalid flag=%1!").arg(queryFlag));
		break;
	}

	emit transferStatus(nStatus, buffer);
}

void QPerformerFileTransfer::recvUploadSlientFile(int flag, quint64 size)
{
	Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

	int nStatus=QueryTargetFileWriteSuccess;

	if (flag == QueryFileOpenFail)
	{
		nStatus=QuerySourceFileOpenFail;
	}
	else if (flag == QueryFileReadLost)
	{
		nStatus=QuerySourceFileReadLost;
	}
	else if (flag == QueryFileReadFail)
	{
		nStatus=QuerySourceFileReadFail;
	}
	else if (flag == QueryFileOpenSuccess || flag == QueryFileReadSuccess)
	{
		nStatus=QuerySourceTransferFile;

		stream<<size;
	}
	else if (flag == QueryFileReadFinish)
	{
		nStatus=QuerySourceTransferFileFinish;
	}

	emit transferStatus(nStatus, buffer);
}

void QPerformerFileTransfer::recvCommandKillProcessFail(QString processName, quint32 processId, QString path)
{
	Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

	stream<<processName<<processId<<path;

	emit transferStatus(QueryCommandKillProcessFail, buffer);
}

void QPerformerFileTransfer::recvCommandCreateProcessFail(QString processName)
{
	Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

	stream<<processName;

	emit transferStatus(QueryCommandCreateProcessFail, buffer);
}

void QPerformerFileTransfer::recvCommandFoundProcessesFail()
{
	emit transferStatus(QueryCommandFoundProcessesFail);
}

void QPerformerFileTransfer::recvCommandFoundCommandFail()
{
	emit transferStatus(QueryCommandFoundCommandFail);
}

void QPerformerFileTransfer::recvCommandExecCommand(int flag, QString command)
{
	Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

	stream<<flag<<command;

	emit transferStatus(QueryCommandExecCommand, buffer);
}

void QPerformerFileTransfer::recvCommandFinish()
{
	emit transferStatus(QueryCommandFinish);
}
