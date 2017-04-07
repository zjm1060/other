#ifndef QMESSAGEPROCESS_H
#define QMESSAGEPROCESS_H

#include <QObject>
#include <QRunnable>

#include "qimessage.h"
#include "extern.h"

/**
*	@Desc 计算机系统进程信息传输格式
*	@Author zlb
*	@Time 2011-4-15
*
*	QueryProcess			QueryProcess
*	AnswerProcess			QueryProcess|请求状态(|计算机系统进程信息)
*	QueryCreateProcess		QueryCreateProcess|命令行
*	AnswerCreateProcess		AnswerCreateProccess|命令行|执行结果
*	QueryTerminateProcess	QueryTerminateProcess|进程ID
*	AnswerTerminateProcess	AnswerTerminateProcess|进程ID|终止状态
*	QueryUnrefProcess		QueryUnrefProcess
**/

class QMessageProcess : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessageProcess(QObject *parent=0);
	~QMessageProcess();

	enum {Type=MSG_SYSTEM_PROCESS};

	enum MsgType
	{
		QueryProcess=0,
		AnswerProcess,
		QueryCreateProcess,
		AnswerCreateProcess,
		QueryTerminateProcess,
		AnswerTerminateProcess,
		QueryUnrefProcess			//不再需要进程信息
	};

	enum QueryResult
	{
		QueryFailed=0,		//获取计算机系统进程信息失败
		QuerySucceed		//获取计算机系统进程信息成功
	};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int msgType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QConnection* sender, QByteArray buffer, QObject* view=0);

private slots:
	void finished(QConnection* sender, int msgType, QByteArray buffer);
};

class QMessageProcessRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMessageProcessRunnable(QConnection* sender, const QByteArray& buffer, QObject* view, QObject* parent=0);
	virtual ~QMessageProcessRunnable();

public:
	void setArg(QConnection* sender, const QByteArray& buffer, QObject* view);
	virtual void run();

signals:
	void finished(QConnection* sender, int msgType, QByteArray buffer=QByteArray());

private:
	QConnection*	m_pSender;
	QByteArray		m_buffer;
	QObject*		m_pView;
};

#endif // QMESSAGEPROCESS_H
