#ifndef QSERVER_H
#define QSERVER_H

#include <QTcpServer>

#include "global.h"
#include "qconnection.h"

class NETWORK_EXPORT QServer : public QTcpServer
{
	Q_OBJECT

public:
	QServer(const QHostAddress& serverAddr, quint16 port, QObject *parent=NULL);
	QServer(const QString& addr, quint16 port, QObject *parent=NULL);
	~QServer();

public:
	bool onListen();
	void onClose();

public:
	void setServerAddress(const QHostAddress& serverAddr);
	QHostAddress serverAddress() const;

	void setServerPort(quint16 port);
	quint16 serverPort() const;

private slots:
	void recvMessage(QConnection* sender, QByteArray buffer);
	void deteteConnection(QTcpSocket* socket, QHostAddress addr, quint16 port);

protected:
	void incomingConnection(int socketDescriptor);

private:
	QHostAddress	m_serverAddr;	//ipµØÖ·
	quint16			m_nPort;		//¶Ë¿ÚºÅ
};

#endif // QSERVER_H
