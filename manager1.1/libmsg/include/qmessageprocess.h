#ifndef QMESSAGEPROCESS_H
#define QMESSAGEPROCESS_H

#include <QObject>
#include <QRunnable>

#include "qimessage.h"
#include "qmessageextern.h"

/**
*	@Desc 计算机系统进程信息传输格式
*	@Author zlb
*	@Time 2011-4-15
*	@modify	2012-2-24
*
*	QueryRefProcess			QueryRefProcess|用户ID
*	QueryUnrefProcess		QueryUnrefProcess|用户ID
*	QueryProcess			QueryProcess|用户ID
*	AnswerProcess			QueryProcess|用户ID|请求状态(|计算机系统进程信息|物理内存大小)
*	QueryCreateProcess		QueryCreateProcess|用户ID|命令行
*	AnswerCreateProcess		AnswerCreateProccess|用户ID|命令行|执行结果
*	QueryTerminateProcess	QueryTerminateProcess|用户ID|进程ID
*	AnswerTerminateProcess	AnswerTerminateProcess|用户ID|进程ID|终止状态
**/

class MSG_EXPORT QMessageProcess : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessageProcess(QObject *parent=0);
	~QMessageProcess();

	enum {Type=MSG_SYSTEM_PROCESS};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int ackType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QObject* sender, QByteArray buffer, QObject* view=0);

private slots:
	void finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer);
};

class MSG_EXPORT QMessageProcessRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMessageProcessRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent=0);
	virtual ~QMessageProcessRunnable();

public:
	void setArg(QObject* sender, const QByteArray& buffer, QObject* view);
	virtual void run();

signals:
	void finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer=QByteArray());

private:
	QObject*		m_pSender;
	QByteArray		m_buffer;
	QObject*		m_pView;
};

#endif // QMESSAGEPROCESS_H
