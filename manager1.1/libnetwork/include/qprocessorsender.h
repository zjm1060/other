#ifndef QPROCESSORSENDER_H
#define QPROCESSORSENDER_H

#include <QObject>
#include <QMutex>

#include "qabstractsender.h"
#include "qmessageparser.h"

class QProcessorSender : public QAbstractSender
{
	Q_OBJECT

public:
	~QProcessorSender();

protected:
	QProcessorSender(QObject *parent=0);

public:
	static QProcessorSender* instance();

protected:
	virtual int message() const
	{
		return MSG_PROCESSOR;
	}

	virtual int ack() const
	{
		return QueryRuntimeProcessor;
	}

private:
	static QMutex			m_mutex;
	static QProcessorSender*	m_pInstance;
};

#endif // QPROCESSORSENDER_H
