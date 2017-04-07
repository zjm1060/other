#ifndef QMULTICASTMSGELECT_H
#define QMULTICASTMSGELECT_H

#include <QObject>
#include <QRunnable>

#include "qmessageextern.h"

/**
*	@Desc 选举领导者信息传输格式
*	@Author zlb
*	@Time 2012-8-14
*
*	QueryElectLeader:		QueryElectLeader(请求选举leader)
*	AnswerElectLeader:		AnswerElectLeader|Leader在位时间|ServerInfo结构
*	QueryVote				QueryVote|性能参数
*	AnswerVote				AnswerVote(反对被选举为leader)
**/

class MSG_EXPORT QMulticastMsgElect : public QObject, public QIMulticastMsg
{
	Q_OBJECT

public:
	QMulticastMsgElect(QObject *parent=0);
	~QMulticastMsgElect();

	enum {Type=MULTICAST_MSG_ELECT};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int msgType, const QByteArray& contents="");
	virtual bool parse(const QString& from, const QByteArray& buffer, QObject* view=0);
};

#endif // QMULTICASTMSGELECT_H
