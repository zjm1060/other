#ifndef QCONNECTION_H
#define QCONNECTION_H

#include "common.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QMutex>

class NETWORK_EXPORT QConnection : public QObject
{
	Q_OBJECT

	enum ConnectionStatus
	{
		WaitingForMessage=0,
		ReadingMessage,
		ReadyForUse,
		Quit
	};

public:
	QConnection(QTcpSocket* socket, QObject *parent=0);
	~QConnection();

public:
	QHostAddress peerAddress();
	quint16 peerPort();

public:
	void ref();
	void unref();

	bool isQuit();

public slots:
	bool sendMessage(const QByteArray& buffer, int msgType);

private slots:
	void readyRead();
	void disconnected();

signals:
	void recvMessage(QConnection* sender, QByteArray buffer);
	void deleteConnection(QTcpSocket* socket, QHostAddress addr, quint16 port);

private:
	QTcpSocket*		m_pSocket;			//连接套接字

	ConnectionStatus	m_eStatus;	//通讯状态
	QByteArray	m_buffer;			//通信信息缓冲区

	int		m_nNextBlockSize;		//通信信息块大小

	QMutex	m_mutex;
	int		m_nRef;			//连接引用次数
};

#endif // QCONNECTION_H
