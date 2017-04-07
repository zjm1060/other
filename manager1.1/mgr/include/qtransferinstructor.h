#ifndef QTRANSFERINSTRUCTOR_H
#define QTRANSFERINSTRUCTOR_H

#include <QAbstractItemModel>

#include "qimessage.h"
#include "qclientmanager.h"
#include "qabstractreceiver.h"
#include "qgrouptransferqueue.h"
#include "qtreeviewgrouptransferqueue.h"

class QTransferInstructor : public QObject, public QAbstractReceiver
{
	Q_OBJECT

public:
	QTransferInstructor(QObject *parent=0);
	~QTransferInstructor();

public:
	enum MessageType
	{
		ConnectingToSource,
		ConnectToSourceFailed,
		ConnectToSourceSuccess,
		ConnectToTargetFailed,
		ConnectToTargetSuccess,
		DisconnectedToSourceHost,
		DisconnectedToTargetHost,
		QueryUpload,
		UploadFail,
		UploadSuccess,
		UploadDirSuccess,		//上传文件夹，仅仅说明状态，在addSubTask中执行操作
		UploadWarning,
		InterruptTransfer,
		BackingUp,
		BackUpFailed,
		BackUpFinished,
		SyncCommand,
		SyncCommandFailed,
		SyncCommandFinished
	};

public:
	void addTask(const QGroupTransferQueue::TransferQueueItem& task);
	void addBackupTask(const QTreeViewGroupTransferQueue::TransferQueueBackupItem& task);
	void addCommandTask(const QTreeViewGroupTransferQueue::TransferQueueCommandItem& task);

	void connectToServer(const ServerInfo& info);

	void clear();

public:
	void queryConnectToHost(const ServerInfo& info);
	void queryUpload();
	void queryInterruptTransferToTarget();
	void queryBackup(const QTreeViewGroupTransferQueue::TransferQueueBackupItem& task);
	void queryCommand(const QTreeViewGroupTransferQueue::TransferQueueCommandItem& task);

signals:
	void sendMessage(QTransferInstructor::MessageType type, const QString& message);

	void addSubTask(const QGroupTransferQueue::TransferQueueItem& task, QVector<FileInfo> files);	//该信号槽必须采用直连方式
	void transferedFileContents(const QGroupTransferQueue::TransferQueueItem& task, quint64 size);

	void backupProcess(int finished, int success);

protected:
	virtual void recvConnectToTarget(int flag);
	virtual void recvUploadDirToTarget(int flag, const QString& sourcePath, const QString& targetPath, const QVector<FileInfo>& files);
	virtual void recvUploadSlientDirToTarget(int flag, const QString& sourcePath, const QString& targetPath, const QVector<FileInfo>& files);
	virtual void recvUploadSymlinkToTarget(int flag);
	virtual void recvUploadFileToTarget(int flag, quint64 size);

	virtual void recvBackup(int flag, int finish, int success);

	virtual void recvCommandKillProcessFail(QString processName, quint32 processId, QString path);
	virtual void recvCommandCreateProcessFail(QString processName);
	virtual void recvCommandFoundProcessesFail();
	virtual void recvCommandFoundCommandFail();
	virtual void recvCommandExecCommand(int flag, QString command);
	virtual void recvCommandFinish();

private slots:
	void connected(QClient* client);
	void disconnected(QClient* client);
	void connectFailed(QClient* client);

private slots:
	void timeout();

private:
	void startTick();
	void stopTick();

	void tip(QTransferInstructor::MessageType type, const QString& message);

private:
	QClient*			m_pClient;
	QClientManager*		m_pClientManager;

	quint32		m_nUsrID;		//用户ID

	bool m_bQueryConnectToTarget;
	QGroupTransferQueue::TransferQueueItem	m_task;		//任务信息

private:
	QTimer		m_timer;		//掉线传输超时
	int			m_nTick;
};

#endif // QTRANSFERINSTRUCTOR_H
