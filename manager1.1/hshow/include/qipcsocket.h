#ifndef QIPCSOCKET_H
#define QIPCSOCKET_H

#include <QTimer>

#include "qsharedobject.h"
#include "qclient.h"
#include "qabstractreceiver.h"

class QIpcSocket;
typedef QSharedObjectPtr<QIpcSocket>	QSharedIpcSocketPtr;

class QAbstractController;

class QIpcSocket : public QObject, public QSharedObject, public QAbstractReceiver
{
	Q_OBJECT

public:
	~QIpcSocket();

protected:
	QIpcSocket(QObject* parent=0);

public:
	static QSharedIpcSocketPtr	instance();

public:
	void connectToHost();
	void reconnect();
	void close();

public:
	bool sendMessage(const QByteArray& buffer, int type);

public:
	QSharedClientPtr clientSocket();

	QString address() const;
	quint16 port() const;

public:
	void queryLocalServer();
	void queryExecCommand(const QStringList& arguments);

protected:
	virtual void recvLocalServer(int flag, const ServerInfo& info);

private slots:
	void clientQuit();
	void clientRecvMessage(QConnection* sender, QByteArray buffer);
	void clientStateChanged(QAbstractSocket::SocketState socketState);
	void clientError(QAbstractSocket::SocketError socketError);

	void timeout();

private:
	QSharedClientPtr		m_pClient;
	QAbstractController*	m_pView;

	quint32	m_nUsrID;
	quint16	m_nPort;	//¶Ë¿ÚºÅ

	QTimer	m_timer;
};

#endif // QPROCESSSOCKET_H
