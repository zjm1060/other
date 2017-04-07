#ifndef QMESSAGECSCS_H
#define QMESSAGECSCS_H

#include <QObject>
#include <QRunnable>

#include "qmessageextern.h"

/**
*	@Desc H9000相关信息传输格式
*	@Author zlb
*	@Time 2012-11-08
*
*	QueryCscsStruct					QueryCscsStruct|用户ID
*	AnswerCscsStruct				AnswerCscsStruct|用户ID|实时数据库厂站结构
*	QueryCscsStation				QueryCscsStation|用户ID|厂站ID
*	AnswerCscsStation				AnswerCscsStation|用户ID|厂站ID|状态(|厂站特征值）
*	QueryCscsGroupEigenvalue		QueryCscsGroupEigenvalue|用户ID|厂站ID|设备ID
*	AnswerCscsGroupEigenvalue		AnswerCscsGroupEigenvalue|用户ID|厂站ID|状态(|设备特征值)
*	QueryCscsDataTypeEigenvalue		QueryCscsDataTypeEigenvalue|用户ID|厂站ID|设备ID|数据类型
*	AnswerCscsDataTypeEigenvalue	AnswerCscsDataTypeEigenvalue|用户ID|厂站ID|设备ID|数据类型|状态(|数据特征值)
**/

class MSG_EXPORT QMessageCscs : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessageCscs(QObject *parent=0);
	~QMessageCscs();

	enum {Type=MSG_CSCS};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int msgType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QObject* sender, QByteArray buffer, QObject* view=0);

private slots:
	void finished(QObject* sender, int msgType, quint32 usrID, QByteArray buffer);
};

class MSG_EXPORT QMessageCscsRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMessageCscsRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent=0);
	virtual ~QMessageCscsRunnable();

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

#endif // QMESSAGECSCS_H
