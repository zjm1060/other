#ifndef QMESSAGEREMOTESERVER_H
#define QMESSAGEREMOTESERVER_H

#include <QObject>
#include <QRunnable>

#include "qmessageextern.h"

/**
*	@Desc 远端服务器信息传输格式
*	@Author zlb
*	@Time 2012-3-29
*
*	QueryRemoteServerList:			QueryRemoteServerList|用户ID
*	AnswerRemoteServerList:			AnswerRemoteServerList|用户ID|请求状态(|计算机服务器信息)
**/

class MSG_EXPORT QMessageRemoteServer : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessageRemoteServer(QObject *parent=0);
	~QMessageRemoteServer();

	enum {Type=MSG_REMOTE_SERVER};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int ackType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QObject* sender, QByteArray buffer, QObject* view=0);

private slots:
	void finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer);
};

class MSG_EXPORT QMessageRemoteServerRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMessageRemoteServerRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent=0);
	virtual ~QMessageRemoteServerRunnable();

public:
	void setArg(QObject* sender, const QByteArray& buffer, QObject* view);
	virtual void run();

signals:
	void finished(QObject* sender, int msgType, quint32 usrID, QByteArray buffer);

private:
	QObject*		m_pSender;
	QByteArray		m_buffer;
	QObject*		m_pView;
};

#endif // QMESSAGEREMOTESERVER_H
