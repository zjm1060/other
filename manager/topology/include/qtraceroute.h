#ifndef QTRACEROUTE_H
#define QTRACEROUTE_H

#include <QThread>
#include <QTimer>
#include <QVector>
#include <QPair>
#include <QMutex>
#include <QLinkedList>

#include "global.h"
#include "topocommon.h"

/**
*	路由器跟踪，同时只能有一实例运行
**/

class QThreadSendTraceRoute;
class QThreadRecvTraceRoute;
class TOPOLOGY_EXPORT QTraceRoute : public QObject
{
	Q_OBJECT
	
public:
	QTraceRoute(QObject *parent=0);
	~QTraceRoute();

public:
	void destroy();

public:
	void addTask(quint32 segment);

signals:
	void found(QVector<QPair<quint32, int> > routes);

public slots:
	void traceRoute();

	void timeout();		//定时器超时

	void timeout(quint32 from);
	void unreach(quint32 from);

	void threadQuit();

private:
	void record(const QPair<quint32, int>& record);

private:
	enum StatusFlag
	{
		NewTask,
		Repeat,
		TtlInc,
		StatusEnd
	};

	void nextTask(StatusFlag flag);

private:
	QThreadSendTraceRoute* m_pThreadSend;
	QThreadRecvTraceRoute* m_pThreadRecv;

	QTimer	m_timer;

	QLinkedList<quint32>	m_tasks;
	QPair<quint32, int>		m_task;
	int m_nRepeat;

	QVector<QPair<quint32, int> >	m_routes;

	int	m_nMaxHops;
};

class TOPOLOGY_EXPORT QThreadSendTraceRoute : public QThread
{
	Q_OBJECT

public:
	QThreadSendTraceRoute(QObject *parent=0);
	~QThreadSendTraceRoute();

public:
	void run();

	void addTask(quint32 ip, int ttl);

public:
	enum StatusFlag
	{
		Running,
		NewTask,
		Tasking,
		StatusEnd
	};

public:
	void setStatus(StatusFlag flag, bool status);
	bool status(StatusFlag flag);

signals:
	void traceRoute();

private:
	void sendPacket(struct sockaddr_in& dest, int ttl);

private:
	SOCKET	m_socket;

	QMutex	m_mutex;
	QPair<quint32, int>	m_task;

	mutable bool m_flags[StatusEnd];
};

class TOPOLOGY_EXPORT QThreadRecvTraceRoute : public QThread
{
	Q_OBJECT

public:
	QThreadRecvTraceRoute(QObject *parent=0);
	~QThreadRecvTraceRoute();

public:
	enum StatusFlag
	{
		Init,
		Running,
		Tasking,
		StatusEnd
	};

public:
	void setStatus(StatusFlag flag, bool status);
	bool status(StatusFlag flag);

public:
	void run();

signals:
	void timeout(quint32 from);
	void unreach(quint32 from);

private:
	SOCKET	m_socket;

	QMutex	m_mutex;
	mutable bool m_flags[StatusEnd];

	char m_recvPacket[PACKET_SIZE];
};

#endif // QTRACEROUTE_H
