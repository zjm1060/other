#ifndef QINTERPROCESSSERVER_H
#define QINTERPROCESSSERVER_H

#include <QLocalServer>
#include <QMap>

#include "global.h"
#include "qsharedobject.h"

class NETWORK_EXPORT QInterProcessServer : public QObject, public QSharedObject
{
	Q_OBJECT

public:
	QInterProcessServer(const QString& name, QObject *parent=NULL);
	~QInterProcessServer();

public:
	enum ConnectionStatus
	{
		WaitingForMessage=0,
		ReadingMessage,
		ReadyForUse,
		Quit
	};

public:
	bool onListen();
	void onClose();

signals:
	void deleteConnection(const QString& name);

private slots:
	void newConnection();
	void readyRead();
	void disconnected();

	void recvMessage(const QByteArray& buffer);

private:
	QString		m_strName;

	QLocalServer*	m_pServer;

	ConnectionStatus	m_eStatus;	//通讯状态
	QByteArray	m_buffer;			//通信信息缓冲区

	int		m_nNextBlockSize;		//通信信息块大小

	bool	m_bLock;				//正在处理消息，其他返回
};

typedef QSharedObjectPtr<QInterProcessServer>	QInterProcessServerPtr;

#endif // QINTERPROCESSSERVER_H
