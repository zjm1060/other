#ifndef QROUTEPROBE_H
#define QROUTEPROBE_H

#include <QThread>
#include <QMutex>
#include <QLinkedList>
#include <QTimer>
#include <QPair>
#include <QVector>

#include "global.h"
#include "topocommon.h"

class QThreadSendRouteProbe;
class QThreadRecvRouteProbe;
class TOPOLOGY_EXPORT QRouteProbe : public QObject
{
	Q_OBJECT

public:
	QRouteProbe(QObject *parent=0);
	~QRouteProbe();

public:
	void destroy();

public:
	void addTask(quint32 ip);
	void addTask(const QVector<quint32>& ip);

signals:
	void found(QPair<quint32, quint32> route);

public slots:
	void probeSent();

	void timeout();		//¶¨Ê±Æ÷³¬Ê±
	void unreach(quint32 from);

	void threadQuit();

private:
	enum StatusFlag
	{
		NewTask,
		Repeat,
		StatusEnd
	};

	void nextTask(StatusFlag flag);

private:
	QThreadSendRouteProbe* m_pThreadSend;
	QThreadRecvRouteProbe* m_pThreadRecv;

	QTimer	m_timer;

	QLinkedList<quint32>	m_tasks;
	QPair<quint32, int>		m_task;

	QVector<QPair<quint32, quint32> >	m_routes;
};

class TOPOLOGY_EXPORT QThreadSendRouteProbe : public QThread
{
	Q_OBJECT

public:
	QThreadSendRouteProbe(QObject* parent=0);
	~QThreadSendRouteProbe();

public:
	enum StatusFlag
	{
		Running,
		NewTask,
		Tasking,
		StatusEnd
	};

public:
	void run();

	void addTask(quint32 task);

public:
	void setStatus(StatusFlag flag, bool status);
	bool status(StatusFlag flag);

signals:
	void probeSent();

private:
	void sendPacket(struct sockaddr_in& dest);

private:
	SOCKET	m_socket;

	mutable bool m_flags[StatusEnd];
	quint32	m_task;

	QMutex	m_mutex;
};

class TOPOLOGY_EXPORT QThreadRecvRouteProbe : public QThread
{
	Q_OBJECT

public:
	QThreadRecvRouteProbe(QObject* parent=0);
	~QThreadRecvRouteProbe();

public:
	enum StatusFlag
	{
		Init,
		Running,
		Tasking,
		StatusEnd
	};

public:
	void run();

public:
	void setStatus(StatusFlag flag, bool status);
	bool status(StatusFlag flag);

signals:
	void unreach(quint32 from);

private:
	void sendPacket(struct sockaddr_in& dest);
	void recvPacket();

private:
	SOCKET	m_socket;

	QMutex	m_mutex;
	mutable bool m_flags[StatusEnd];

	char m_recvPacket[PACKET_SIZE];
};

#endif // QROUTEPROBE_H
