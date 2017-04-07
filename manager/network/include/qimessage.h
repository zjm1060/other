#ifndef QIMESSAGE_H
#define QIMESSAGE_H

#include <QDataStream>

#include "global.h"
#include "qconnection.h"

class QIMessage
{
public:
	QIMessage(){};
	virtual ~QIMessage(){};

public:
	virtual int type() const=0;

	virtual QByteArray query(int msgType, quint32 usrID=-1, const QByteArray& contents="")=0;		//·¢ÆðÇëÇó
	virtual bool parse(QConnection* sender, QByteArray buffer, QObject* view=0)=0;
};

#endif // QIMESSAGE_H
