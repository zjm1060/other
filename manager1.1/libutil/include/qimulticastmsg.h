#ifndef QIMULTICASTMSG_H
#define QIMULTICASTMSG_H

#include <QDataStream>

#include "global.h"
#include "qsharedobject.h"

enum MulticastMsgType
{
	MULTICAST_MSG_ELECT
};

enum ElectMsgAckType
{
	QueryElectLeader,
	AnswerElectLeader,
	QueryVote,
	AnswerVote
};

class UTIL_EXPORT QIMulticastMsg : public QSharedObject
{
public:
	QIMulticastMsg(){};
	virtual ~QIMulticastMsg(){};

public:
	virtual int type() const=0;

	virtual QByteArray query(int ackType, const QByteArray& contents="")=0;		//发起请求
	virtual bool parse(const QString& from, const QByteArray& buffer, QObject* view=NULL)=0;
};

typedef QSharedObjectPtr<QIMulticastMsg> QIMulticastMsgPtr;

typedef void (*CreateMulticastMsgInstanceFunc)(QVector<QIMulticastMsgPtr>&);		//消息类动态库必须提供的接口

#endif // QIMULTICASTMSG_H
