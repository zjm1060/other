#ifndef QCLIENT_H
#define QCLIENT_H

#include <QTcpSocket>
#include <QHostAddress>

#include "qsharedobject.h"
#include "qconnection.h"
#include "qutildef.h"

#define MAX_RECONNECT_COUNT	2

class NETWORK_EXPORT QClient : public QTcpSocket, public QSharedObject
{
	Q_OBJECT

public:
	QClient(const QHostAddress& serverAddr, quint16 port, QObject *parent=0);
	QClient(const ServerInfo& serverInfo, QObject *parent=0);
	~QClient();

public:
	void connectToHost();
	void closeSocket();

	bool sendMessage(const QByteArray& buffer, int msgType);

public:
	const QHostAddress& serverAddr() const;
	quint16 port() const;

	void setServerAddr(const QString& addr);
	void setPort(quint16 port);

	QConnection* connection();

signals:
	void recvMessage(QConnection* sender, QByteArray buffer);
	void quit();
	void clientStateChanged(QAbstractSocket::SocketState socketState);

public slots:
	void socketStateChanged(QAbstractSocket::SocketState socketState);

private slots:
	void deleteConnection(QConnection* connection);

private:
	QHostAddress	m_serverAddr;		//欲连接的服务器ip地址
	quint16			m_nPort;			//欲连接的服务器端口号

	int				m_nIndex;			//当前使用的服务器IP索引
	QVector<Server>	m_vecAddrs;			//可连接的IP地址
	ServerInfo		m_server;			//服务器的IP信息

	QConnection*	m_pConnection;
	bool			m_bClosed;			//通道被关闭

private:
	static QMap<QString, Server>	m_sMapFirstServer;	//各主机首选连接地址
};

typedef QSharedObjectPtr<QClient>	QSharedClientPtr;

#endif // QCLIENT_H
