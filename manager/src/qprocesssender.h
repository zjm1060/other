#ifndef QPROCESSSENDER_H
#define QPROCESSSENDER_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <QTimer>
#include <QMap>
#include <QRunnable>

#include "qconnection.h"

class QProcessSender : public QObject
{
	Q_OBJECT

public:
	~QProcessSender();

protected:
	QProcessSender(QObject *parent=0);

public:
	static QProcessSender* instance();

public:
	void addClient(QConnection* client);
	void removeClient(QConnection* client);

private slots:
	void timeout();
	void finished(QMap<quint32, quint64> mapTimeProcess, quint64 totalTimeProcess, QByteArray buffer);

private:
	static QMutex			m_mutex;
	static QProcessSender*	m_pInstance;

	QList<QConnection*>		m_listClient;

	QMap<quint32, quint64>	m_mapTimeProcess;
	quint64	m_nTotalTimeProcess;

	QTimer	m_timer;
};

class QProcessSenderRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QProcessSenderRunnable(QMap<quint32, quint64> mapTimeProcess, quint64 totalTimeProcess, QObject* parent=0);
	virtual ~QProcessSenderRunnable();

public:
	virtual void run();

signals:
	void finished(QMap<quint32, quint64> mapTimeProcess, quint64 totalTimeProcess, QByteArray buffer);

private:
	QMap<quint32, quint64>	m_mapTimeProcess;
	quint64	m_nTotalTimeProcess;
};

#endif // QPROCESSSENDER_H
