#ifndef QPROCESSORSENDER_H
#define QPROCESSORSENDER_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <QTimer>
#include <QMap>
#include <QRunnable>

#include "qconnection.h"

class QProcessorSender : public QObject
{
	Q_OBJECT

public:
	~QProcessorSender();

protected:
	QProcessorSender(QObject *parent=0);

public:
	static QProcessorSender* instance();

public:
	void addClient(QConnection* client);
	void removeClient(QConnection* client);

private slots:
	void timeout();
	void finished(QByteArray buffer);

private:
	static QMutex			m_mutex;
	static QProcessorSender*	m_pInstance;

	QList<QConnection*>		m_listClient;

	QMap<quint32, quint64>	m_mapTimeProcess;
	quint64	m_nTotalTimeProcess;

	QTimer	m_timer;
};

class QProcessorSenderRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QProcessorSenderRunnable(QObject* parent=0);
	virtual ~QProcessorSenderRunnable();

public:
	virtual void run();

signals:
	void finished(QByteArray buffer);

private:
};

#endif // QPROCESSORSENDER_H
