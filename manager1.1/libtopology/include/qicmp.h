#ifndef QICMP_H
#define QICMP_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QLinkedList>
#include <QTimer>
#include <QVector>
#include <QMap>

#include "global.h"
#include "common.h"
#include "topocommon.h"

class QThreadSendIcmp;
class QThreadRecvIcmp;
class TOPOLOGY_EXPORT QIcmp : public QObject
{
	Q_OBJECT
	
public:
	QIcmp(QObject *parent=0);
	~QIcmp();

public:
	void destroy();

public:
	void addTask(quint32 segment);

signals:
	void found(quint32 segment, QVector<quint32> alives);

public slots:
	void icmpEcho(quint32 ip);
	void icmpStart();

	void timeout();		//定时器超时

	void threadQuit();

private:
	void nextTask();

private:
	SOCKET	m_socket;

	pid_t   m_pid;		//进程ID

	QThreadSendIcmp*	m_pThreadSend;
	QThreadRecvIcmp*	m_pThreadRecv;

	QTimer	m_timer;

	QLinkedList<quint32>	m_tasks;

	quint32 m_task;
	QVector<quint32>	m_alives;
};

class TOPOLOGY_EXPORT QThreadSendIcmp : public QThread
{
	Q_OBJECT

public:
	QThreadSendIcmp(SOCKET socket, pid_t pid, QObject* parent=0);
	~QThreadSendIcmp();

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

	void addTask(quint32 segment);

	void setStatus(StatusFlag flag, bool status);
	bool status(StatusFlag flag);

signals:
	void icmpStart();

private:
	quint16 chksum(quint16 *addr, int len);		//校验和算法
	int pack(int pack_no);		//设置ICMP报头

	void sendPacket(struct sockaddr_in& dest);

private:
	SOCKET	m_socket;
	pid_t   m_pid;		//进程ID

	quint32	m_task;

	QMutex	m_mutex;
	mutable bool m_flags[StatusEnd];

	char m_sendPacket[PACKET_SIZE];		//接收缓冲区
};

class TOPOLOGY_EXPORT QThreadRecvIcmp : public QThread
{
	Q_OBJECT

public:
	QThreadRecvIcmp(SOCKET socket, pid_t pid, QObject* parent=0);
	~QThreadRecvIcmp();

public:
	enum StatusFlag
	{
		Running,
		Tasking,
		StatusEnd
	};

public:
	void run();

	void setStatus(StatusFlag flag, bool status);
	bool status(StatusFlag flag);

signals:
	void icmpEcho(quint32 ip);

private:
	SOCKET	m_socket;
	pid_t	m_pid;

	QMutex	m_mutex;
	mutable bool m_flags[StatusEnd];

	char m_recvPacket[PACKET_SIZE];
};

#endif // QICMP_H
