#ifndef QABSTRACTSENDER_H
#define QABSTRACTSENDER_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <QTimer>

#include "qconnection.h"

class QAbstractSender : public QObject
{
	Q_OBJECT

public:
	~QAbstractSender();

protected:
	QAbstractSender(int interval, QObject *parent=0);

public:
	void addClient(QConnection* client, quint32 usrID);
	void removeClient(QConnection* client, quint32 usrID);

protected:
	virtual int message() const=0;
	virtual int ack() const=0;

private slots:
	void timeout();

private:
	QMutex	m_mutex;
	QList<QPair<QConnection*, quint32> >		m_listClient;

	QTimer	m_timer;
};
#endif // QABSTRACTSENDER_H
