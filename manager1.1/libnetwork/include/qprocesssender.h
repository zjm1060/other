#ifndef QPROCESSSENDER_H
#define QPROCESSSENDER_H

#include <QObject>
#include <QMutex>

#include "qabstractsender.h"
#include "qmessageparser.h"

class QProcessSender : public QAbstractSender
{
	Q_OBJECT

public:
	~QProcessSender();

protected:
	QProcessSender(QObject *parent=0);

public:
	static QProcessSender* instance();

protected:
	virtual int message() const
	{
		return MSG_SYSTEM_PROCESS;
	}

	virtual int ack() const
	{
		return QueryProcess;
	}

private:
	static QMutex			m_mutex;
	static QProcessSender*	m_pInstance;
};
#endif // QPROCESSSENDER_H
