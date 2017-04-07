#ifndef QMEMORYSENDER_H
#define QMEMORYSENDER_H

#include "qabstractsender.h"
#include "qimessage.h"

class QMemorySender : public QAbstractSender
{
	Q_OBJECT

public:
	~QMemorySender();

protected:
	QMemorySender(QObject *parent=0);

public:
	virtual int message() const 
	{
		return MSG_SYSTEM_MEMORY;
	}

	virtual int ack() const
	{
		return QuerySystemMemory;
	}

public:
	static QMemorySender* instance();

private:
	static QMutex			g_mutex;
	static QMemorySender*	g_pInstance;
};

#endif // QMEMORYSENDER_H
