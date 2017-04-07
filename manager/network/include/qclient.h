#ifndef QCLIENT_H
#define QCLIENT_H

#include <QTcpSocket>
#include <QHostAddress>

#include "qconnection.h"
#include "qservermanager.h"

#define MAX_RECONNECT_COUNT	10

class NETWORK_EXPORT QClient : public QTcpSocket
{
	Q_OBJECT

public:
	QClient(const QHostAddress& serverAddr, quint16 port, QObject *parent=0);
	QClient(const QServerManager::ServerInfo& serverInfo, QObject *parent=0);
	~QClient();

public:
	void connectToHost();
	void close();

	bool sendMessage(const QByteArray& buffer, int msgType);

public:
	const QHostAddress& serverAddr() const;
	quint16 port() const;

	void setServerAddr(const QString& addr);
	void setPort(quint16 port);

signals:
	void recvMessage(QConnection* sender, QByteArray buffer);
	void quit();
	void clientStateChanged(QAbstractSocket::SocketState socketState);

public slots:
	void socketStateChanged(QAbstractSocket::SocketState socketState);

private slots:
	void deleteConnection(QTcpSocket* socket, QHostAddress addr, quint16 port);

private:
	QHostAddress	m_serverAddr;		//欲连接的服务器ip地址
	quint16			m_nPort;			//欲连接的服务器端口号

	QConnection*	m_pConnection;
};

#endif // QCLIENT_H
