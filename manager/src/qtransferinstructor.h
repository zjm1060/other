#ifndef QTRANSFERINSTRUCTOR_H
#define QTRANSFERINSTRUCTOR_H

#include <QAbstractItemModel>

#include "extern.h"
#include "qclientmanager.h"
#include "qabstractfiletransfer.h"
#include "qgrouptransferqueue.h"

class QTransferInstructor : public QObject, public QAbstractFileTransfer
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
		UploadSuccess
	};

public:
	void addTask(const QGroupTransferQueue::TransferQueueItem& task);

	void connectToServer(const QServerManager::ServerInfo& info);

public:
	void queryConnectToHost(const QServerManager::ServerInfo& info);

	void queryUpload();

signals:
	void sendMessage(QTransferInstructor::MessageType type, const QString& message);

	void addSubTask(const QGroupTransferQueue::TransferQueueItem& task, QVector<FileInfo> files);

protected:
	virtual void recvConnectToHost(int flag);
	virtual void recvSourceUpload(int flag, QByteArray buffer);

private slots:
	void connected(QClient* client);
	void disconnected(QClient* client);

private:
	QClient*			m_pClient;
	QClientManager*		m_pClientManager;

	quint32		m_nUsrID;		//用户ID

	QGroupTransferQueue::TransferQueueItem	m_task;		//任务信息
};

#endif // QTRANSFERINSTRUCTOR_H
