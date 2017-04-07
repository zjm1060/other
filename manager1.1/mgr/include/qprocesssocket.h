#ifndef QPROCESSSOCKET_H
#define QPROCESSSOCKET_H

#include <QTimer>

#include "qsharedobject.h"
#include "qclient.h"

class QProcessSocket;
typedef QSharedObjectPtr<QProcessSocket>	QSharedProcessSocketPtr;

class QProcessSocket : public QObject, public QSharedObject
{
	Q_OBJECT

public:
	~QProcessSocket();

protected:
	QProcessSocket(QObject* parent=0);

public:
	static QSharedProcessSocketPtr	instance();

public:
	void setView(QObject* view);	//设置消息接收者
	void setUser(quint32 id);

	void connectToHost();
	void reconnect();
	void close();

public:
	bool sendMessage(const QByteArray& buffer, int type);

public:
	QString address() const;
	quint16 port() const;

public:
	void queryLocalServer();
	void queryRemoteServerList();
	void queryTopology();
	void queryExecCommand(const QStringList& arguments);

private slots:
	void clientQuit();
	void clientRecvMessage(QConnection* sender, QByteArray buffer);
	void clientStateChanged(QAbstractSocket::SocketState socketState);
	void clientError(QAbstractSocket::SocketError socketError);

	void timeout();

private:
	QSharedClientPtr	m_pClient;

	QObject* m_pView;
	quint32  m_nUsrID;

	QTimer	m_timer;
};

#endif // QPROCESSSOCKET_H
