#include "qtransferinstructor.h"

#include "define.h"
#include "common.h"
#include "qmessageparser.h"
#include "qfilecontroller.h"
#include "qserverdelegate.h"
#include "qmessageparser.h"
#include "qservermanager.h"
#include "qtreeviewtransferqueue.h"
#include "qreceiverparser.h"
#include "qreceivertransferfile.h"
#include "qlogger.h"

#define TRANSFER_MAX_TICK_NUMBER		120

QTransferInstructor::QTransferInstructor(QObject *parent)
	: QObject(parent)
	,m_pClient(NULL)
	,m_bQueryConnectToTarget(true)
{
	QSharedControllerPtr pController(new QFileController);
	m_pClientManager=new QClientManager(pController, this);
	connect(m_pClientManager, SIGNAL(connected(QClient*)), this, SLOT(connected(QClient*)));
	connect(m_pClientManager, SIGNAL(disconnected(QClient*)), this, SLOT(disconnected(QClient*)));
	connect(m_pClientManager, SIGNAL(connectFailed(QClient*)), this, SLOT(connectFailed(QClient*)));

	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_nTick=0;
	m_timer.setInterval(1000);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

QTransferInstructor::~QTransferInstructor()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);
}

void QTransferInstructor::timeout()
{
	m_nTick++;

	if (m_nTick > TRANSFER_MAX_TICK_NUMBER)
	{
		m_timer.stop();

		tip(UploadFail, tr("Failed because of timeout."));
	}
}

void QTransferInstructor::clear()
{
	m_task.sourceHost=ServerInfo();
	m_task.targetHost=ServerInfo();
}

void QTransferInstructor::startTick()
{
	m_nTick=0;
	if (!m_timer.isActive())
	{
		m_timer.start();
	}
}

void QTransferInstructor::stopTick()
{
	m_timer.stop();
}

void QTransferInstructor::addTask(const QGroupTransferQueue::TransferQueueItem& task)
{
	if (m_task.sourceHost != task.sourceHost || m_task.targetHost != task.targetHost)
	{
		m_bQueryConnectToTarget=true;
	}

	m_task=task;

	connectToServer(m_task.sourceHost);
}

void QTransferInstructor::addBackupTask(const QTreeViewGroupTransferQueue::TransferQueueBackupItem& task)
{
	queryBackup(task);

	startTick();
}

void QTransferInstructor::addCommandTask(const QTreeViewGroupTransferQueue::TransferQueueCommandItem& task)
{
	queryCommand(task);

	startTick();
}

void QTransferInstructor::tip(QTransferInstructor::MessageType type, const QString& message)
{
	logplusInfo(MANAGER_LOGGER_STR, message);

	emit sendMessage(type, message);
}

void QTransferInstructor::connectToServer(const ServerInfo& info)
{
	logplusDebug(MANAGER_LOGGER_STR, QString("Connecting to controlled server %1 ...").arg(info.hostName));

	QClient* pClient=m_pClientManager->addClient(info);
	Q_ASSERT_NO_NULL(pClient);

	if (pClient == m_pClient)
	{
		if (m_pClientManager->isConnected(pClient))
		{
// 			Server source, target;
// 			QServerManager::checkConnect(m_task.sourceHost, m_task.targetHost, &source, &target);
// 
// 			tip(ConnectToTargetSuccess, 
// 				tr("Succeed to connect")+" "+source.addr+(m_task.sourceHost.hostName.isEmpty()? "":"@"+m_task.sourceHost.hostName)+" -> "
// 				+target.addr+(m_task.targetHost.hostName.isEmpty()? "":"@"+m_task.targetHost.hostName));

			connected(pClient);
		}
		else
		{
			disconnected(pClient);
		}
	}
	else
	{
		m_pClientManager->remove(m_pClient);
		m_pClient=NULL;

		m_bQueryConnectToTarget=true;
		m_pClientManager->connectToHost(pClient);
	}
}

void QTransferInstructor::connected(QClient* client)
{
	logplusDebug(MANAGER_LOGGER_STR, "Connected to controlled server.");

	m_pClient=client;
	if (m_pClient != NULL && m_bQueryConnectToTarget)
	{
		queryConnectToHost(m_task.targetHost);
	}

	if (!m_bQueryConnectToTarget)
	{
		queryUpload();
	}
}

void QTransferInstructor::disconnected(QClient* client)
{
	if (m_pClientManager != NULL)
	{
		m_pClientManager->remove(client);
	}

	stopTick();
}

void QTransferInstructor::connectFailed(QClient* client)
{
	m_task.sourceHost=ServerInfo();
	m_task.targetHost=ServerInfo();

	tip(ConnectToSourceFailed, QString(tr("Connected to %1:%2 failed")).arg(client->serverAddr().toString()).arg(client->port()));

	if (m_pClientManager != NULL)
	{
		m_pClientManager->remove(client);
	}


	stopTick();
}

void QTransferInstructor::queryConnectToHost(const ServerInfo& info)
{
	if (m_pClient == NULL)
	{
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_CONTROLLED);
	if (pMsg != NULL)
	{
		Server source, target;
		QServerManager::checkConnect(m_task.sourceHost, m_task.targetHost, &source, &target);

		tip(ConnectingToSource, 
			tr("connecting")+" "+source.addr+(m_task.sourceHost.hostName.isEmpty()? "":"@"+m_task.sourceHost.hostName)+" -> "+
			target.addr+":"+QString::number(target.port)+(m_task.targetHost.hostName.isEmpty()? "":"@"+m_task.targetHost.hostName));

		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<info;
		buffer=pMsg->query(QueryConnectToTarget, m_nUsrID, buffer);

		m_pClient->sendMessage(buffer, MSG_CONTROLLED);
	}
}

void QTransferInstructor::queryUpload()
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_CONTROLLED);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

		QString strSource=QReceiverTransferFile::appendPathName(m_task.source, m_task.file.name, m_task.isTop);
		QString strTarget=QReceiverTransferFile::appendPathName(m_task.target, m_task.file.name, false);
		stream<<m_task.file.type<<strSource<<strTarget;

		QString strDesc=tr("directory");
		if (m_task.file.type == File || m_task.file.type == HideFile)
		{
			if (m_task.format == AutoFormat)
			{
				if (isTextFormat(strSource))
				{
					m_task.format=TextFormat;
				}
				else
				{
					m_task.format=BinaryFormat;
				}
			}

			stream<<m_task.format;

			strDesc=tr("file");

			buffer=pMsg->query(m_task.isSlient? QueryUploadSlientFileToTarget:QueryUploadFileToTarget, m_nUsrID, buffer);
		}
		else if ((m_task.file.type & SymLink) != 0 || (m_task.file.type & HideSymLink) != 0)
		{
			strDesc=tr("symbol link file");

			buffer=pMsg->query(m_task.isSlient? QueryUploadSlientSymlinkToTarget:QueryUploadSymlinkToTarget, m_nUsrID, buffer);
		}
		else
		{
			buffer=pMsg->query(m_task.isSlient? QueryUploadSlientDirToTarget:QueryUploadDirToTarget, m_nUsrID, buffer);
		}

		tip(QueryUpload, tr("Upload")+" "+strDesc+":"+strSource+" --> "+strTarget);

		bool bRtn=m_pClient->sendMessage(buffer, MSG_CONTROLLED);
		if (!bRtn)
		{
			tip(DisconnectedToSourceHost, tr("Disconnected to Source host!"));
		}
	}

	startTick();
}

void QTransferInstructor::queryInterruptTransferToTarget()
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_CONTROLLED);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		buffer=pMsg->query(QueryInterruptTransferToTarget, m_nUsrID);

		QString strSource=QReceiverTransferFile::appendPathName(m_task.source, m_task.file.name, m_task.isTop);

		tip(InterruptTransfer, tr("Stop to Upload")+" "+strSource);

		bool bRtn=m_pClient->sendMessage(buffer, MSG_CONTROLLED);
		if (!bRtn)
		{
			tip(DisconnectedToSourceHost, tr("Disconnected to Source host!"));
		}
	}

	startTick();
}

void QTransferInstructor::queryBackup(const QTreeViewGroupTransferQueue::TransferQueueBackupItem& task)
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_FILE_TRANSFER);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
		stream<<task.source<<task.target<<task.files;

		buffer=pMsg->query(QueryBackup, m_nUsrID, buffer);

		tip(BackingUp, tr("Backing up %1 ---> %2").arg(task.source).arg(task.target));

		bool bRtn=m_pClient->sendMessage(buffer, MSG_FILE_TRANSFER);
		if (!bRtn)
		{
			tip(DisconnectedToSourceHost, tr("Disconnected to Source host!"));
		}
	}

	startTick();
}

void QTransferInstructor::queryCommand(const QTreeViewGroupTransferQueue::TransferQueueCommandItem& task)
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_CONTROLLED);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
		stream<<task.workspace<<task.files;

		buffer=pMsg->query(QueryExecSyncCommandToTarget, m_nUsrID, buffer);

		tip(SyncCommand, tr("Query to execute preseted commands."));

		bool bRtn=m_pClient->sendMessage(buffer, MSG_CONTROLLED);
		if (!bRtn)
		{
			tip(DisconnectedToSourceHost, tr("Disconnected to Source host!"));
		}
	}

	startTick();
}

void QTransferInstructor::recvConnectToTarget(int flag)
{
	Server source, target;
	QServerManager::checkConnect(m_task.sourceHost, m_task.targetHost, &source, &target);

	switch (flag)
	{
	case QueryConnectToTargetFailed:
		{
			m_bQueryConnectToTarget=true;

			tip(ConnectToTargetFailed, 
				tr("Failed to connect")+" "+source.addr+(m_task.sourceHost.hostName.isEmpty()? "":"@"+m_task.sourceHost.hostName)+" -> "
				+target.addr+(m_task.targetHost.hostName.isEmpty()? "":"@"+m_task.targetHost.hostName));
		}
		break;
	case QueryConnectToTargetSuccess:
		{
			m_bQueryConnectToTarget=false;

			tip(ConnectToTargetSuccess, 
				tr("Succeed to connect")+" "+source.addr+(m_task.sourceHost.hostName.isEmpty()? "":"@"+m_task.sourceHost.hostName)+" -> "
				+target.addr+(m_task.targetHost.hostName.isEmpty()? "":"@"+m_task.targetHost.hostName));

			logplusDebug(MANAGER_LOGGER_STR, "Connected to target server.");

			queryUpload();
		}
		break;
	case TargetDisconnected:
		{
			m_bQueryConnectToTarget=true;

			tip(DisconnectedToTargetHost, tr("Disconnected!"));
			tip(DisconnectedToTargetHost, 
				tr("Disconnect")+" "+source.addr+(m_task.sourceHost.hostName.isEmpty()? "":"@"+m_task.sourceHost.hostName)+" -> "+
				target.addr+(m_task.targetHost.hostName.isEmpty()? "":"@"+m_task.targetHost.hostName));
		}
		break;
	default:
		{
			m_bQueryConnectToTarget=true;

			qDebug("QTransferInstructor::recvConnectToHost invalid flag!!!");
		}
		break;
	}
}

void QTransferInstructor::recvUploadDirToTarget(int flag, const QString& sourcePath, const QString& targetPath, const QVector<FileInfo>& files)
{
	stopTick();

	switch (flag)
	{
	case QueryPathNoExist:
		{
			tip(UploadFail, tr("Source: Directory %1 not existed!").arg(sourcePath));
		}
		break;
	case QueryMakePathFail:
		{
			tip(UploadFail, tr("Target: Failed to make the directory %1 !").arg(sourcePath));
		}
		break;
	case QueryMakePathSuccess:
		{
			tip(UploadDirSuccess, tr("Target: Success to make the directory %1.").arg(targetPath));

			emit addSubTask(m_task, files);
		}
		break;
	default:
		break;
	}
}

void QTransferInstructor::recvUploadSlientDirToTarget(int flag, const QString& sourcePath, const QString& targetPath, const QVector<FileInfo>& files)
{
	stopTick();

	switch (flag)
	{
	case QueryPathNoExist:
		{
			tip(UploadFail, tr("Source: Directory %1 not existed!").arg(sourcePath));
		}
		break;
	case QueryMakePathSuccess:
		{
			tip(UploadDirSuccess, tr("Source: Success to transfer the directory %1.").arg(targetPath));

			emit addSubTask(m_task, files);
		}
		break;
	default:
		break;
	}
}

void QTransferInstructor::recvUploadSymlinkToTarget(int flag)
{
	stopTick();

	switch (flag)
	{
	case QueryFileNoExist:
		{
			tip(UploadFail, tr("Source: Failed to find the symbol link file %1 !")
				.arg(QReceiverTransferFile::appendPathName(m_task.source, m_task.file.name, m_task.isTop)));
		}
		break;
	case QuerySymLinkNoLink:
		{
			tip(UploadFail, tr("Source: The file %1 is not the symbol link file!")
				.arg(QReceiverTransferFile::appendPathName(m_task.source, m_task.file.name, m_task.isTop)));
		}
		break;
	case QueryCreateSymLinkFail:
		{
			tip(UploadFail, tr("Target: Failed to create the symbol link file %1 !")
				.arg(QReceiverTransferFile::appendPathName(m_task.target, m_task.file.name, false)));
		}
		break;
	case QueryCreateSymLinkSuccess:
		{
			tip(UploadSuccess, tr("Target: Succeed to create the symbol link file %1 .")
				.arg(QReceiverTransferFile::appendPathName(m_task.target, m_task.file.name, false)));
		}
		break;
	}
}

void QTransferInstructor::recvUploadFileToTarget(int flag, quint64 size)
{
	switch (flag)
	{
	case QueryFileNoExist:
		{
			stopTick();

			tip(UploadFail, tr("Source: Failed to find the file %1 !")
				.arg(QReceiverTransferFile::appendPathName(m_task.source, m_task.file.name, m_task.isTop)));
		}
		break;
	case QuerySourceFileOpenFail:
		{
			stopTick();

			tip(UploadFail, tr("Source: Failed to open the file %1 !")
				.arg(QReceiverTransferFile::appendPathName(m_task.source, m_task.file.name, m_task.isTop)));
		}
		break;
	case QuerySourceFileReadLost:
		{
			stopTick();

			tip(UploadFail, tr("Source: File %1 does not existed!")
				.arg(QReceiverTransferFile::appendPathName(m_task.source, m_task.file.name, m_task.isTop)));
		}
		break;
	case QuerySourceFileReadFail:
		{
			stopTick();

			tip(UploadFail, tr("Source: Failed to read the file %1 !")
				.arg(QReceiverTransferFile::appendPathName(m_task.source, m_task.file.name, m_task.isTop)));
		}
		break;
	case QuerySourceTransferFile:
		{
			m_nTick=0;

			emit transferedFileContents(m_task, size);
		}
		break;
	case QuerySourceSendFlowControl:
		{
			m_nTick=0;
		}
		break;
	case QueryTargetFileCreateFail:
		{
			stopTick();

			tip(UploadFail, tr("Target: Failed to create the file %1 !")
				.arg(QReceiverTransferFile::appendPathName(m_task.target, m_task.file.name, false)));
		}
		break;
	case QueryTargetFileWriteLost:
	case QueryTargetFileWriteFail:
		{
			stopTick();

			tip(UploadFail, tr("Target: Failed to write the file %1 !")
				.arg(QReceiverTransferFile::appendPathName(m_task.target, m_task.file.name, false)));
		}
		break;
	case QuerySourceTransferFileFinish:
		{
			if (!m_task.isSlient)
			{
				break;
			}
		}
	case QueryTargetFileWriteFinish:
		{
			stopTick();

			tip(UploadSuccess, tr("Source: Success to transfer the file %1 .")
				.arg(QReceiverTransferFile::appendPathName(m_task.source, m_task.file.name, false)));
		}
		break;
	case QueryTargetFileWriteSuccess:
		{
			m_nTick=0;
		}
		break;
	case QueryTargetFileChangePermissionFail:
		{
			stopTick();

			tip(UploadWarning, tr("Target: Failed to change permissions of the file %1 !")
				.arg(QReceiverTransferFile::appendPathName(m_task.source, m_task.file.name, false)));

			tip(UploadSuccess, tr("Source: Success to transfer the file %1 .")
				.arg(QReceiverTransferFile::appendPathName(m_task.source, m_task.file.name, false)));
		}
		break;
	default:
		qDebug("QTransferInstructor::recvUploadFileToTarget invalid flag!!!");
		break;
	}
}

void QTransferInstructor::recvBackup(int flag, int finish, int success)
{
	switch (flag)
	{
	case QueryBackingUp:
		{
			m_nTick=0;

			emit backupProcess(finish, success);
		}
		break;
	case QueryBackUpFinish:
		{
			stopTick();

			tip(BackUpFinished, tr("Finished to back up, %1 success, a total of %2.").arg(success).arg(finish));
		}
		break;
	}
}

void QTransferInstructor::recvCommandKillProcessFail(QString processName, quint32 processId, QString path)
{
	m_nTick=0;

	tip(SyncCommandFailed, tr("Failed to kill process %1, pid=%2, path=%3.").arg(processName).arg(processId).arg(path));
}

void QTransferInstructor::recvCommandCreateProcessFail(QString processName)
{
	m_nTick=0;
	 
	tip(SyncCommandFailed, tr("Failed to create process %1.").arg(processName));
}

void QTransferInstructor::recvCommandFoundProcessesFail()
{
	m_nTick=0;

	tip(SyncCommandFailed, tr("Failed to read process information."));
}

void QTransferInstructor::recvCommandFoundCommandFail()
{
	m_nTick=0;

	tip(SyncCommandFailed, tr("Failed to read the commands. Maybe H9000 is not starting!"));
}

void QTransferInstructor::recvCommandExecCommand(int flag, QString command)
{
	m_nTick=0;

	switch (flag)
	{
	case CscsCommandNoExisted:
		tip(SyncCommandFailed, tr("Failed to identify the command \"%1\".").arg(command));
		break;
	case CscsCommandParamError:
		tip(SyncCommandFailed, tr("Command \"%1\" is not implemented for error parameters.").arg(command));
		break;
	case CscsCommandFailed:
		tip(SyncCommandFailed, tr("Failed to execute command \"%1\".").arg(command));
		break;
	case CscsCommandSuccess:
		break;
	default:
		break;
	}
}

void QTransferInstructor::recvCommandFinish()
{
	stopTick();

	tip(SyncCommandFinished, tr("Finished to execute preseted commands."));
}

