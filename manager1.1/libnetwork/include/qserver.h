#ifndef QSERVER_H
#define QSERVER_H

#include <QTcpServer>
#include <QMap>

#include "global.h"
#include "qsharedobject.h"
#include "qconnection.h"
#include "qcontrolledview.h"

class NETWORK_EXPORT QServer : public QTcpServer, public QSharedObject
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
	void setAddress(const QHostAddress& serverAddr);
	const QHostAddress& address() const;

	void setPort(quint16 port);
	quint16 port() const;

public:
	void informStatusChanged(quint64 sign);

private slots:
	void recvMessage(QConnection* sender, QByteArray buffer);
	void deteteConnection(QConnection* connection);

protected:
	void incomingConnection(int socketDescriptor);

private:
	QHostAddress	m_serverAddr;	//ipµØÖ·
	quint16			m_nPort;		//¶Ë¿ÚºÅ

	QControlledView*	m_pView;
};

typedef QSharedObjectPtr<QServer>	QServerPtr;

#endif // QSERVER_H
