#include "qtraceroute.h"

#ifdef Q_OS_WIN
#include <Mstcpip.h>
#include <WS2TCPIP.H>
#endif

#include <QNetworkInterface>
#include <QHostAddress>
#include <QList>
#include <QMutableListIterator>

#include "qlogger.h"

#define MAX_NUM_REPEAT	3
#define TIME_OUT_TRACE_ROUTE	1000

QTraceRoute::QTraceRoute(QObject *parent)
: QObject(parent)
{
	m_nMaxHops=parseConfig(getAppParentDir()+INI_FILE, TOPOLOTY_MAX_HOPS, 6).toUInt();
	m_nMaxHops=m_nMaxHops>1? m_nMaxHops:1;

	m_timer.setInterval(TIME_OUT_TRACE_ROUTE);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	m_pThreadSend=new QThreadSendTraceRoute();
	connect(m_pThreadSend, SIGNAL(traceRoute()), this, SLOT(traceRoute()));
	connect(m_pThreadSend, SIGNAL(finished()), this, SLOT(threadQuit()));

	m_pThreadRecv=new QThreadRecvTraceRoute();
	connect(m_pThreadRecv, SIGNAL(timeout(quint32)), this, SLOT(timeout(quint32)));
	connect(m_pThreadRecv, SIGNAL(unreach(quint32)), this, SLOT(unreach(quint32)));
	connect(m_pThreadRecv, SIGNAL(finished()), this, SLOT(threadQuit()));

	LOGPLUS_DEBUG_STR(QLogger::root(), "Start send and recv threads to trace route...");

	m_pThreadRecv->start();
	m_pThreadSend->start();
}

QTraceRoute::~QTraceRoute()
{
}

void QTraceRoute::destroy()
{
	LOGPLUS_DEBUG_STR(QLogger::root(), "Stop send and recv trace route threads.");

	m_pThreadSend->setStatus(QThreadSendTraceRoute::Running, false);
	m_pThreadRecv->setStatus(QThreadRecvTraceRoute::Running, false);
}

void QTraceRoute::threadQuit()
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

void QTraceRoute::addTask(quint32 segment)
{
	qDebug("QTraceRoute: Add network segment %s to find near router port", 
		QHostAddress(segment).toString().toLocal8Bit().data());

	m_tasks.append(segment);

	nextTask(NewTask);
}

void QTraceRoute::nextTask(StatusFlag flag)
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
				if (!m_pThreadRecv->status(QThreadRecvTraceRoute::Tasking))
				{
					m_pThreadRecv->setStatus(QThreadRecvTraceRoute::Tasking, true);
				}
			}
			else
			{
				m_pThreadRecv->setStatus(QThreadRecvTraceRoute::Tasking, false);
			}

			if (m_pThreadSend->status(QThreadSendTraceRoute::Tasking) || m_tasks.isEmpty())
			{
				return;
			}

			m_routes.clear();

			m_task.first=m_tasks.takeFirst()+1;
			m_task.second=1;
			m_nRepeat=1;

			qDebug("Use IP %s to find the nearest route.", QHostAddress(m_task.first).toString().toLocal8Bit().data());
		}
		break;
	case Repeat:
		{
			m_nRepeat++;
			if (m_nRepeat > MAX_NUM_REPEAT)
			{
				emit found(m_routes);

				nextTask(NewTask);
				return;
			}
		}
		break;
	case TtlInc:
		{
			m_nRepeat=1;
			m_task.second++;
			if (m_task.second > m_nMaxHops)
			{
				emit found(m_routes);

				nextTask(NewTask);
				return;
			}
		}
		break;
	default:
		qDebug("QTraceRoute: Invalid Status!");
		break;
	}

	m_pThreadSend->addTask(m_task.first, m_task.second);
}

void QTraceRoute::timeout()
{
	m_timer.stop();

	m_pThreadSend->setStatus(QThreadSendTraceRoute::Tasking, false);
	nextTask(Repeat);
}

void QTraceRoute::timeout(quint32 from)
{
	m_timer.stop();

	record(QPair<quint32, int>(from, m_task.second));

	m_pThreadSend->setStatus(QThreadSendTraceRoute::Tasking, false);
	nextTask(TtlInc);
}

void QTraceRoute::unreach(quint32 from)
{
	m_timer.stop();

	if (m_task.first != from)
	{
		record(QPair<quint32, int>(from, m_task.second));
	}

	emit found(m_routes);

	m_pThreadSend->setStatus(QThreadSendTraceRoute::Tasking, false);
	nextTask(NewTask);
}

void QTraceRoute::record(const QPair<quint32, int>& record)
{
	for (int i=0; i<m_routes.size(); i++)
	{
		if (m_routes[i].first == record.first)
		{
			return;
		}
	}

	m_routes.append(record);
}

void QTraceRoute::traceRoute()
{
	m_timer.start();
}

QThreadSendTraceRoute::QThreadSendTraceRoute(QObject* parent)
:QThread(parent)
{
	setStatus(Running, true);
	setStatus(NewTask, false);
	setStatus(Tasking, false);

	m_socket=socket(AF_INET, SOCK_DGRAM, 0);
}

QThreadSendTraceRoute::~QThreadSendTraceRoute()
{
	closesocket(m_socket);
}

void QThreadSendTraceRoute::addTask(quint32 ip, int ttl)
{
	ttl=ttl>0? ttl:1;

	m_task=QPair<quint32, int>(ip, ttl);

	setStatus(NewTask, true);
}

void QThreadSendTraceRoute::setStatus(QThreadSendTraceRoute::StatusFlag flag, bool status)
{
	QMutexLocker locker(&m_mutex);
	m_flags[flag]=status;
}

bool QThreadSendTraceRoute::status(QThreadSendTraceRoute::StatusFlag flag)
{
	bool f;
	QMutexLocker locker(&m_mutex);
	f=m_flags[flag];

	return f;
}

void QThreadSendTraceRoute::run()
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

		setStatus(NewTask, false);
		setStatus(Tasking, true);

		addr.sin_addr.s_addr = htonl(m_task.first);

		sendPacket(addr, m_task.second);

		emit traceRoute();
	}

	LOGPLUS_DEBUG_STR(QLogger::root(), "QThreadSendTraceRoute: Thread quit!");
}

void QThreadSendTraceRoute::sendPacket(struct sockaddr_in& dest, int ttl)
{
	char buf[]="ROUTE";

	setsockopt(m_socket, IPPROTO_IP, IP_TTL, (char*)&ttl, sizeof(ttl));

	qDebug("QThreadSendTraceRoute: Send packet to %s, TTL=%d", inet_ntoa(dest.sin_addr), ttl);

	sendto(m_socket, buf, strlen(buf),0, (struct sockaddr *)&dest, sizeof(dest));
}

QThreadRecvTraceRoute::QThreadRecvTraceRoute(QObject* parent)
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
	tv.tv_sec=TIME_OUT_TRACE_ROUTE/1000;
	tv.tv_usec=(TIME_OUT_TRACE_ROUTE%1000);
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

	if (addresses.isEmpty())
	{
		setStatus(Init, false);

		LOGPLUS_DEBUG_STR(QLogger::root(), "QThreadRecvTraceRoute: Find local IP failed!");
		return;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;

	quint16 nPort=(quint16)parseConfig(getAppParentDir()+INI_FILE, TOPOLOGY_TRACE_ROUTE_PORT, 53234).toUInt();
	nPort=nPort>30000? nPort:53234;

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

QThreadRecvTraceRoute::~QThreadRecvTraceRoute()
{
	if (m_socket != -1)
	{
		closesocket(m_socket);
	}
}

void QThreadRecvTraceRoute::run()
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
			|| (!status(Tasking))) 
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
			qDebug("QThreadRecvTraceRoute: unreach %s", inet_ntoa(from.sin_addr));

			emit unreach(ntohl(from.sin_addr.s_addr));
		}
		else if( (icmp->icmp_type == ICMP_TIMXCEED) && (icmp->icmp_code == ICMP_TIMXCEED_INTRANS) )
		{
			qDebug("QThreadRecvTraceRoute: route time out %s", inet_ntoa(from.sin_addr));

			emit timeout(ntohl(from.sin_addr.s_addr));
		}
	}

	qDebug("QThreadRecvTraceRoute: Thread quit!");
}

void QThreadRecvTraceRoute::setStatus(QThreadRecvTraceRoute::StatusFlag flag, bool status)
{
	QMutexLocker locker(&m_mutex);
	m_flags[flag]=status;
}

bool QThreadRecvTraceRoute::status(QThreadRecvTraceRoute::StatusFlag flag)
{
	bool f;
	QMutexLocker locker(&m_mutex);
	f=m_flags[flag];

	return f;
}
