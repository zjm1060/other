#ifndef QCONNECTION_H
#define QCONNECTION_H

#include "common.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QMutex>

#include "qsharedobject.h"

class NETWORK_EXPORT QConnection : public QObject, public QSharedObject
{
	Q_OBJECT

public:
	QConnection(QTcpSocket* socket, QObject *parent=0);
	~QConnection();

public:
	enum ConnectionStatus
	{
		WaitingForMessage=0,
		ReadingMessage,
		ReadyForUse,
		Quit
	};

public:
	QHostAddress peerAddress();
	quint16 peerPort();

	QTcpSocket*	connectionSocket();

public:
	bool isQuit();

public slots:
	Q_INVOKABLE bool sendMessage(const QByteArray& buffer, int msgType);
	Q_INVOKABLE bool addReceiver(quint32 usrID, int msgType);
	Q_INVOKABLE bool removeReceiver(quint32 usrID, int msgType);

public slots:
	void readyRead();
	Q_INVOKABLE void disconnected();

private:
	QByteArray addHeaderToPacket(const QByteArray& buffer, int msgType);

signals:
	void recvMessage(QConnection* sender, QByteArray buffer);
	void deleteConnection(QConnection* connection);

private:
	QTcpSocket*		m_pSocket;			//连接套接字,需要手动删除

	ConnectionStatus	m_eStatus;	//通讯状态
	QByteArray	m_buffer;			//通信信息缓冲区

	int		m_nEncryptType;			//加密类型
	int		m_nNextBlockSize;		//通信信息块大小

	bool	m_bLock;				//正在处理消息，其他返回

	QMutex	m_mutex;
};

typedef QSharedObjectPtr<QConnection>	QConnectionPtr;

#endif // QCONNECTION_H
