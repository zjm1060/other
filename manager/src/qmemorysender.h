#ifndef QMEMORYSENDER_H
#define QMEMORYSENDER_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <QTimer>
#include <QMap>
#include <QRunnable>

#include "qconnection.h"

class QMemorySender : public QObject
{
	Q_OBJECT

public:
	~QMemorySender();

protected:
	QMemorySender(QObject *parent=0);

public:
	static QMemorySender* instance();

public:
	void addClient(QConnection* client);
	void removeClient(QConnection* client);

private slots:
	void timeout();
	void finished(QByteArray buffer);

private:
	static QMutex			m_mutex;
	static QMemorySender*	m_pInstance;

	QList<QConnection*>		m_listClient;

	QTimer	m_timer;
};

class QMemorySenderRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMemorySenderRunnable(QObject* parent=0);
	virtual ~QMemorySenderRunnable();

public:
	virtual void run();

signals:
	void finished(QByteArray buffer);
};

#endif // QMEMORYSENDER_H
