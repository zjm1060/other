#ifndef QMIBMANAGER_H
#define QMIBMANAGER_H

#include "global.h"

#include <QObject>
#include <QTimer>
#include <QRunnable>

class UTIL_EXPORT QMibManager : public QObject
{
	Q_OBJECT

public:
	~QMibManager();

protected:
	QMibManager(QObject *parent=0);

public:
	static QMibManager* instance();

public:
	void setInterval(int interval);

	void start(bool collect);
	void stop();

private slots:
	void timeout();

signals:
	void changed(quint64 sign);
	void collected(int type);

public:
	Q_INVOKABLE void finished(int type);

private:
	bool		m_bCollect;	//是否仅仅收集数据

	QTimer		m_timer;

	bool	m_bFirst;
};

class UTIL_EXPORT QMibTask : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMibTask(int type, QObject* view);
	~QMibTask();

public:
	void run();

private:
	int	m_nType;
	QObject* m_pView;
};

#endif // QMIBMANAGER_H
