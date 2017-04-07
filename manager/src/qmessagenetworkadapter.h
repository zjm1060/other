#ifndef QMESSAGENETWORKADAPTER_H
#define QMESSAGENETWORKADAPTER_H

#include <QObject>
#include <QRunnable>

#include "qimessage.h"
#include "extern.h"

/**
*	@Desc 计算机网络适配器信息传输格式
*	@Author zlb
*	@Time 2011-4-13
*
*	QueryNetworkAdapter:			QueryNetworkAdapter
*	AnswerNetworkAdapter:			AnswerNetworkAdapter|请求状态(|计算机网络适配器信息)
**/

class QMessageNetworkAdapter : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessageNetworkAdapter(QObject *parent=0);
	~QMessageNetworkAdapter();

	enum {Type=MSG_NETWORK_ADAPTER};

	enum MsgType
	{
		QueryNetworkAdapter=0,
		AnswerNetworkAdapter
	};

	enum QueryResult
	{
		QueryFailed=0,		//获取计算机系统信息失败
		QuerySucceed		//获取计算机系统信息成功
	};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int msgType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QConnection* sender, QByteArray buffer, QObject* view=0);

private slots:
	void finished(QConnection* sender, int msgType, QByteArray buffer);
};

class QMessageNetworkAdapterRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMessageNetworkAdapterRunnable(QConnection* sender, const QByteArray& buffer, QObject* view, QObject* parent=0);
	virtual ~QMessageNetworkAdapterRunnable();

public:
	void setArg(QConnection* sender, const QByteArray& buffer, QObject* view);
	virtual void run();

signals:
	void finished(QConnection* sender, int msgType, QByteArray buffer);

private:
	QConnection*	m_pSender;
	QByteArray		m_buffer;
	QObject*		m_pView;
};

#endif // QMESSAGENETWORKADAPTER_H
