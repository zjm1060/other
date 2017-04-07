#ifndef QMESSAGECONFIGURE_H
#define QMESSAGECONFIGURE_H

#include <QObject>
#include <QRunnable>
#include <QVector>

#include "qmessageextern.h"

/**
*	@Desc 传输格式
*	@Author zlb
*	@Time 2013-04-07
*
*	QuerySyncAssociationConfigure	QuerySyncAssociationConfigure|用户ID
*	AnswerSyncAssociationConfigure	AnswerSyncAssociationConfigure|用户ID|文件关联配置信息
*	QueryWriteSyncAssociateionConfig	QueryWriteSyncAssociateionConfig|用户ID|文件关联配置信息
*	AnswerWriteSyncAssociateionConfig	AnswerWriteSyncAssociateionConfig|用户ID|更新状态
*	QueryExecSyncConfigureCommand	QueryExecSyncConfigureCommand|用户ID|工作目录|文件信息
*	AnswerExecSyncConfigureCommand	AnswerExecSyncConfigureCommand|用户ID|状态（原因）
**/

class MSG_EXPORT QMessageConfigure : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessageConfigure(QObject *parent=0);
	~QMessageConfigure();

	enum {Type=MSG_CONFIGURE};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int ackType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QObject* sender, QByteArray buffer, QObject* view=0);

private slots:
	void finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer);
};

class MSG_EXPORT QMessageConfigureRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMessageConfigureRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent=0);
	virtual ~QMessageConfigureRunnable();

signals:
	void finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer=QByteArray());

public:
	void setArg(QObject* sender, const QByteArray& buffer, QObject* view);
	virtual void run();

private:
	QObject*		m_pSender;
	QByteArray		m_buffer;
	QObject*		m_pView;
};

#endif // QMESSAGECONFIGURE_H
