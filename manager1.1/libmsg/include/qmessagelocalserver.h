#ifndef QMESSAGESYSTEM_H
#define QMESSAGESYSTEM_H

#include <QObject>
#include <QRunnable>

#include "qmessageextern.h"

/**
*	@Desc 计算机本地服务器信息传输格式
*	@Author zlb
*	@Time 2012-2-16
*
*	QueryLocalServer:			QueryLocalServer|用户ID
*	AnswerLocalServer:			AnswerLocalServer|用户ID|请求状态(|计算机服务器信息)
**/

class MSG_EXPORT QMessageLocalServer : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessageLocalServer(QObject *parent=0);
	~QMessageLocalServer();

	enum {Type=MSG_LOCAL_SERVER};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int ackType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QObject* sender, QByteArray buffer, QObject* view=0);

private slots:
	void finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer);
};

class MSG_EXPORT QMessageLocalServerRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMessageLocalServerRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent=0);
	virtual ~QMessageLocalServerRunnable();

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

#endif // QMESSAGESYSTEM_H
