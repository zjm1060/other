#ifndef QINTERPROCESSCLIENT_H
#define QINTERPROCESSCLIENT_H

#include <QLocalSocket>

#include "qsharedobject.h"
#include "qutildef.h"

class NETWORK_EXPORT QInterProcessClient : public QLocalSocket, public QSharedObject
{
	Q_OBJECT

public:
	QInterProcessClient(const QString& name, QObject *parent=0);
	~QInterProcessClient();

public:
	void connectToHost();
	void closeSocket();

	bool sendMessage(const QByteArray& buffer, int msgType);

public slots:
	void connected();
	void disconnected();

	void stateChanged(QLocalSocket::LocalSocketState socketState);

signals:
	void clientConnected();
	void clientQuit();

private:
	QString			m_strName;

	bool			m_bClosed;			//通道被关闭
};

typedef QSharedObjectPtr<QInterProcessClient>	QInterProcessClientPtr;

#endif // QINTERPROCESSCLIENT_H
