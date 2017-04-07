#ifndef QMESSAGENETWORKADAPTER_H
#define QMESSAGENETWORKADAPTER_H

#include <QObject>
#include <QRunnable>

#include "qmessageextern.h"

/**
*	@Desc 计算机网络适配器信息传输格式
*	@Author zlb
*	@Time 2011-4-13
*
*	QueryNetworkAdapter:			QueryNetworkAdapter|用户ID
*	AnswerNetworkAdapter:			AnswerNetworkAdapter|用户ID|请求状态(|计算机网络适配器信息)
**/

class MSG_EXPORT QMessageNetworkAdapter : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessageNetworkAdapter(QObject *parent=0);
	~QMessageNetworkAdapter();

	enum {Type=MSG_NETWORK_ADAPTER};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int ackType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QObject* sender, QByteArray buffer, QObject* view=0);

private slots:
	void finished(QObject* sender, int msgType, quint32 usrID, QByteArray buffer);
};

class MSG_EXPORT QMessageNetworkAdapterRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMessageNetworkAdapterRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent=0);
	virtual ~QMessageNetworkAdapterRunnable();

public:
	void setArg(QObject* sender, const QByteArray& buffer, QObject* view);
	virtual void run();

signals:
	void finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer);

private:
	QObject*		m_pSender;
	QByteArray		m_buffer;
	QObject*		m_pView;
};

#endif // QMESSAGENETWORKADAPTER_H
