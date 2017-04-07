#include "qrouteprobe.h"

#ifdef Q_OS_WIN
#include <Mstcpip.h>
#endif

#include <QNetworkInterface>
#include <QHostAddress>
#include <QList>

#include "qlogger.h"

#define MAX_NUM_REPEAT 2
#define TIME_OUT_ROUTE_PROBE	1000

QRouteProbe::QRouteProbe(QObject *parent)
	: QObject(parent)
{
	m_timer.setInterval(TIME_OUT_ROUTE_PROBE);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	m_pThreadRecv=new QThreadRecvRouteProbe();
	connect(m_pThreadRecv, SIGNAL(unreach(quint32)), this, SLOT(unreach(quint32)));
	connect(m_pThreadRecv, SIGNAL(finished()), this, SLOT(threadQuit()));

	m_pThreadSend=new QThreadSendRouteProbe();
	connect(m_pThreadSend, SIGNAL(probeSent()), this, SLOT(probeSent()));
	connect(m_pThreadSend, SIGNAL(finished()), this, SLOT(threadQuit()));

	LOGPLUS_DEBUG_STR(QLogger::root(), "Start send and recv threads to probe routes...");

	m_pThreadRecv->start();
	m_pThreadSend->start();
}

QRouteProbe::~QRouteProbe()
{
}

void QRouteProbe::destroy()
{
	LOGPLUS_DEBUG_STR(QLogger::root(), "Stop send and recv route probe threads.");

	m_pThreadSend->setStatus(QThreadSendRouteProbe::Running, false);
	m_pThreadRecv->setStatus(QThreadRecvRouteProbe::Running, false);
}

void QRouteProbe::threadQuit()
{
	if (m_pThreadSend == sender())
	{
		m_pThreadSend->deleteLater();
		m_pThreadSend=NULL;
	}
	else if (m_pThreadRecv == sender())
	{
		m_pThreadRecv->deleteLater();
		m_pThreadRecv=NULL;
	}

	if ((m_pThreadSend == NULL) && (m_pThreadRecv == NULL))
	{
		deleteLater();
	}
}

void QRouteProbe::addTask(quint32 ip)
{
	m_tasks.append(ip);

	nextTask(NewTask);
}

void QRouteProbe::addTask(const QVector<quint32>& ip)
{
	for (int i=0; i<ip.size(); i++)
	{
		m_tasks.append(ip[i]);
	}

	nextTask(NewTask);
}

void QRouteProbe::nextTask(StatusFlag flag)
{
	switch (flag)
	{
	case NewTask:
		{
			if (m_pThreadRecv == NULL)
			{
				return;
			}

			if (!m_tasks.isEmpty())
			{
				if (!m_pThreadRecv->status(QThreadRecvRouteProbe::Tasking))
				{
					m_pThreadRecv->setStatus(QThreadRecvRouteProbe::Tasking, true);
				}
			}
			else
			{
				m_pThreadRecv->setStatus(QThreadRecvRouteProbe::Tasking, false);

				QPair<quint32, quint32> pair;
				pair.first=m_task.first&0xffffff00;
				pair.second=pair.first;

				emit found(pair);

				return;
			}

			if (m_pThreadSend->status(QThreadSendRouteProbe::Tasking) || m_tasks.isEmpty())
			{
				return;
			}

			m_task.first=m_tasks.takeFirst();
			m_task.second=1;

			qDebug("Use IP=%s to find the nearest route.", QHostAddress(m_task.first).toString().toLocal8Bit().data());
		}
		break;
	case Repeat:
		{
			m_task.second++;
			if (m_task.second > MAX_NUM_REPEAT)
			{
				nextTask(NewTask);
				return;
			}
		}
		break;
	default:
		LOGPLUS_ERROR_STR(QLogger::root(), "QTraceRoute: Invalid Status!");
		break;
	}

	m_pThreadSend->addTask(m_task.first);
}

void QRouteProbe::unreach(quint32 from)
{
	m_timer.stop();
	m_pThreadSend->setStatus(QThreadSendRouteProbe::Tasking, false);

	QPair<quint32, quint32> pair;
	pair.first=m_task.first;
	pair.second=from;

	if (pair.first != pair.second)
	{
		m_tasks.clear();

		emit found(pair);

		qDebug("QRouteProbe: %s -> %s", QHostAddress(pair.first).toString().toLocal8Bit().data(),
			QHostAddress(pair.second).toString().toLocal8Bit().data());

		m_pThreadRecv->setStatus(QThreadRecvRouteProbe::Tasking, false);

		return;
	}

	nextTask(NewTask);
}

void QRouteProbe::probeSent()
{
	m_timer.start();
}

void QRouteProbe::timeout()
{
	m_timer.stop();
	m_pThreadSend->setStatus(QThreadSendRouteProbe::Tasking, false);

	qDebug("QRouteProbe: Probe timeout!");

	nextTask(Repeat);
}

QThreadSendRouteProbe::QThreadSendRouteProbe(QObject* parent)
:QThread(parent)
{
	setStatus(Running, true);
	setStatus(NewTask, false);
	setStatus(Tasking, false);

	m_socket=socket(AF_INET, SOCK_DGRAM, 0);
}

QThreadSendRouteProbe::~QThreadSendRouteProbe()
{
	closesocket(m_socket);
}

void QThreadSendRouteProbe::setStatus(QThreadSendRouteProbe::StatusFlag flag, bool status)
{
	QMutexLocker locker(&m_mutex);
	m_flags[flag]=status;
}

bool QThreadSendRouteProbe::status(QThreadSendRouteProbe::StatusFlag flag)
{
	bool f;
	QMutexLocker locker(&m_mutex);
	f=m_flags[flag];

	return f;
}

void QThreadSendRouteProbe::run()
{
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;

	quint16 nPort=(quint16)parseConfig(getAppParentDir()+INI_FILE, TOPOLOGY_UNREACH_PORT, 65535).toUInt();
	nPort=nPort>30000? nPort:65535;

	addr.sin_port=htons(nPort);

	while (status(Running))
	{
		if (!status(NewTask))
		{
			msleep(100);
			continue;
		}

		setStatus(Tasking, true);
		setStatus(NewTask, false);

		addr.sin_addr.s_addr = htonl(m_task);
		sendPacket(addr);

		emit probeSent();
	}

	LOGPLUS_DEBUG_STR(QLogger::root(), "QThreadSendRouteProbe: Thread quit!");
}

void QThreadSendRouteProbe::addTask(quint32 task)
{
	qDebug("QRouteProbe: Add alive IP %s", QHostAddress(task).toString().toLocal8Bit().data());

	m_task=task;
	
	setStatus(NewTask, true);
}

void QThreadSendRouteProbe::sendPacket(struct sockaddr_in& dest)
{
	char buf[]="ROUTE";

	qDebug("QThreadSendRouteProbe: Send packet to %s", inet_ntoa(dest.sin_addr));

	sendto(m_socket, buf, strlen(buf),0, (struct sockaddr *)&dest, sizeof(dest));
}

QThreadRecvRouteProbe::QThreadRecvRouteProbe(QObject* parent)
:QThread(parent)
{
	setStatus(Init, true);
	setStatus(Running, true);
	setStatus(Tasking, false);

	m_socket=socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (m_socket == -1)
	{
		setStatus(Init, false);

		LOGPLUS_DEBUG_STR(QLogger::root(), "QThreadRecvTraceRoute: socket() SOCK_RAW error!");
		return;
	}

	struct timeval tv;
	tv.tv_sec=TIME_OUT_ROUTE_PROBE/1000;
	tv.tv_usec=(TIME_OUT_ROUTE_PROBE%1000);
	setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));

	int size=32*1024;
	setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof (size));

	QList<QHostAddress> addresses=QNetworkInterface::allAddresses();
	QMutableListIterator<QHostAddress> it(addresses);
	while (it.hasNext())
	{
		QHostAddress address=it.next();
		if (address.protocol() != QAbstractSocket::IPv4Protocol || address == QHostAddress::LocalHost)
		{
			it.remove();
		}
	}

	if (addresses.size() == 0)
	{
		setStatus(Init, false);

		LOGPLUS_DEBUG_STR(QLogger::root(), "QThreadRecvRouteProbe: Find local ip failed!");
		return;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;

	quint16 nPort=(quint16)parseConfig(getAppParentDir()+INI_FILE, TOPOLOGY_ROUTE_PROBE_PORT, 53244).toUInt();
	nPort=nPort>30000? nPort:53244;

	addr.sin_port=htons(nPort);

	_inet_aton(addresses[0].toString().toLocal8Bit().data(), &addr.sin_addr);

	bind(m_socket, (struct sockaddr*)&addr, sizeof(addr));

#ifdef Q_OS_WIN
	u_long value=1;
	ioctlsocket(m_socket, SIO_RCVALL, &value);
#else
	if (!setPromiscFlag(m_socket, addresses[0]))
	{
		setStatus(Init, false);
		return;
	}

	setuid(getuid());
#endif
}

QThreadRecvRouteProbe::~QThreadRecvRouteProbe()
{
	if (m_socket != -1)
	{
		closesocket(m_socket);
	}
}

void QThreadRecvRouteProbe::run()
{
	if (!status(Init))
	{
		return;
	}

	int nLen;

	socklen_t fromlen;

	struct sockaddr_in from;
	fromlen=sizeof(from);

	while(status(Running))
	{
		if(((nLen=recvfrom(m_socket, m_recvPacket, sizeof(m_recvPacket), 0, (struct sockaddr *)&from, &fromlen)) <0)
			|| !status(Tasking))
		{
			msleep(100);

			continue;
		}

		int iphdrlen;
		struct ip *ip;
		Icmp *icmp;

		ip=(struct ip *)m_recvPacket;
		if (ip->ip_p != IPPROTO_ICMP)
		{
			continue;
		}

		iphdrlen=ip->ip_hl<<2;    /*求ip报头长度,即ip报头的长度标志乘4*/
		icmp=(Icmp *)(m_recvPacket+iphdrlen);  /*越过ip报头,指向ICMP报头*/
		nLen-=iphdrlen;            /*ICMP报头及ICMP数据报的总长度*/
		if( nLen<8)                /*小于ICMP报头长度则不合理*/
		{
			qDebug("ICMP packets\'s length is less than 8!");
			continue;
		}

		//端口不可达
		if( (icmp->icmp_type == ICMP_UNREACH) && (icmp->icmp_code == ICMP_UNREACH_PORT) )
		{
			qDebug("QThreadRecvRouteProbe: unreach %s", inet_ntoa(from.sin_addr));

			emit unreach(ntohl(from.sin_addr.s_addr));
		}
	}

	LOGPLUS_DEBUG_STR(QLogger::root(), "QThreadRecvRouteProbe: Thread quit!");
}

void QThreadRecvRouteProbe::setStatus(QThreadRecvRouteProbe::StatusFlag flag, bool status)
{
	QMutexLocker locker(&m_mutex);
	m_flags[flag]=status;
}

bool QThreadRecvRouteProbe::status(QThreadRecvRouteProbe::StatusFlag flag)
{
	bool f;
	QMutexLocker locker(&m_mutex);
	f=m_flags[flag];

	return f;
}
