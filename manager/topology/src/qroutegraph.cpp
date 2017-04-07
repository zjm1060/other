#include "qroutegraph.h"

#include <Mstcpip.h>
#include <WS2TCPIP.H>

#include <QNetworkInterface>
#include <QHostAddress>
#include <QList>
#include <QMutableListIterator>

#define MAX_NUM_REPEAT	2

QRouteGraph::QRouteGraph(QObject *parent)
: QObject(parent)
{
	m_timer.setInterval(500);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	m_pThreadRecv=new QThreadRecvRouteGraph();
	connect(m_pThreadRecv, SIGNAL(timeout(quint32)), this, SLOT(timeout(quint32)));
	connect(m_pThreadRecv, SIGNAL(unreach(quint32)), this, SLOT(unreach(quint32)));

	m_pThreadSend=new QThreadSendRouteGraph();
	connect(m_pThreadSend, SIGNAL(sent()), this, SLOT(sent()));
}

QRouteGraph::~QRouteGraph()
{
	terminateThread();

	SAFE_DELETE_CLASS(m_pThreadSend);
	SAFE_DELETE_CLASS(m_pThreadRecv);
}

void QRouteGraph::terminateThread()
{
	if (m_pThreadSend != NULL)
	{
		m_pThreadSend->setStatus(QThreadSendRouteGraph::Running, false);
	}

	if (m_pThreadRecv != NULL)
	{
		m_pThreadRecv->setStatus(QThreadRecvRouteGraph::Running, false);
	}

	if (m_pThreadSend != NULL)
	{
		m_pThreadSend->wait();
	}

	if (m_pThreadRecv != NULL)
	{
		m_pThreadRecv->wait();
	}
}

void QRouteGraph::addTask(quint32 ip)
{
	m_tasks.append(ip);
}

void QRouteGraph::exec(quint32 nearest)
{
	if (m_tasks.isEmpty())
	{
		qDebug("\nQRouteGraph: Found %d routes! Cann\'t find topology between routes!", nearest==-1? 0:1);
		return;
	}

	qDebug("\nFind routes topology...");

	m_pThreadRecv->start();
	m_pThreadSend->start();

	m_nNearestRoute=nearest;

	nextTask(NewTask);
}

void QRouteGraph::timeout()
{
	m_timer.stop();

	nextTask(Repeat);
}

void QRouteGraph::timeout(quint32 from)
{
	m_timer.stop();

	if (from != m_nSourceRoute)
	{
		record(from);

		m_nSourceRoute=from;
	}

	nextTask(TtlInc);
}

void QRouteGraph::unreach(quint32 from)
{
	m_timer.stop();

	if (from != m_nSourceRoute)
	{
		record(from);
	}

	m_nSourceRoute=m_nNearestRoute;

	nextTask(NewTask);
}

void QRouteGraph::record(quint32 from)
{
	QPair<quint32, quint32> pair;
	pair.first=m_nSourceRoute;;
	pair.second=from;

	bool bExist=false;
	for (int i=0; i<m_graph.size(); i++)
	{
		if (m_graph[i] == pair)
		{
			bExist=true;
			return;
		}
	}

	if (!bExist)
	{
		m_graph.append(pair);

		qDebug("QRouteGraph: %s <--> %s", QHostAddress(pair.first).toString().toLocal8Bit().data(), 
			QHostAddress(pair.second).toString().toLocal8Bit().data());
	}
}

void QRouteGraph::nextTask(StatusFlag flag)
{
	switch (flag)
	{
	case NewTask:
		{
			if (m_tasks.isEmpty())
			{
				qDebug("\nFind routes topology finished!");

				finished();
				return;
			}

			m_nSourceRoute=m_nNearestRoute;

			m_task.first=m_tasks.takeFirst();
			m_task.second=1;
			m_nRepeat=1;

			qDebug("Use IP %s to find the connected route.", QHostAddress(m_task.first).toString().toLocal8Bit().data());
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
	case TtlInc:
		{
			m_task.second++;
		}
		break;
	default:
		qDebug("QTraceRoute: Invalid Status!");
		break;
	}

	m_pThreadSend->addTask(m_task.first, m_task.second);
}

void QRouteGraph::sent()
{
	m_timer.start();
}

void QRouteGraph::finished()
{
	m_timer.stop();

	terminateThread();

	emit finished(m_graph);
}

QThreadSendRouteGraph::QThreadSendRouteGraph(QObject* parent)
:QThread(parent)
{
	setStatus(NewTask, false);
	setStatus(Running, true);

	m_socket=socket(AF_INET, SOCK_DGRAM, 0);
}

QThreadSendRouteGraph::~QThreadSendRouteGraph()
{
	closesocket(m_socket);
}

void QThreadSendRouteGraph::addTask(quint32 ip, int ttl)
{
	ttl=ttl>0? ttl:1;

	QMutexLocker locker(&m_mutex);
	m_task=QPair<quint32, int>(ip, ttl);

	m_flags[NewTask]=true;
}

void QThreadSendRouteGraph::setStatus(QThreadSendRouteGraph::StatusFlag flag, bool status)
{
	QMutexLocker locker(&m_mutex);
	m_flags[flag]=status;
}

bool QThreadSendRouteGraph::status(QThreadSendRouteGraph::StatusFlag flag)
{
	bool f;
	QMutexLocker locker(&m_mutex);
	f=m_flags[flag];

	return f;
}

void QThreadSendRouteGraph::run()
{
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;

	quint16 nPort=(quint16)parseConfig(getAppParentDir()+TOPOLOGY_INI_FILE, TOPOLOGY_UNREACH_PORT, 65535).toUInt();
	nPort=nPort>30000? nPort:65535;
	addr.sin_port=htons(nPort);

	while (status(Running))
	{
		if (!status(NewTask))
		{
			usleep(100);
			continue;
		}

		setStatus(NewTask, false);

		QPair<quint32, int> task;
		m_mutex.lock();
		task=m_task;
		m_mutex.unlock();

		addr.sin_addr.s_addr = htonl(task.first);

		sendPacket(addr, task.second);

		emit sent();
	}
}

void QThreadSendRouteGraph::sendPacket(struct sockaddr_in& dest, int ttl)
{
	char buf[]="ROUTE";

	setsockopt(m_socket, IPPROTO_IP, IP_TTL, (char*)&ttl, sizeof(ttl));

	qDebug("QThreadSendRouteGraph: Send packet to %s, TTL=%d", inet_ntoa(dest.sin_addr), ttl);

	sendto(m_socket, buf, strlen(buf),0, (struct sockaddr *)&dest, sizeof(dest));
}

QThreadRecvRouteGraph::QThreadRecvRouteGraph(QObject* parent)
:QThread(parent)
{
	m_flags[Init]=true,
	m_flags[Running]=true;

	m_socket=socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (m_socket == -1)
	{
		m_flags[Init]=false;

		qDebug("QThreadRecvRouteGraph: socket() SOCK_RAW error!");
		return;
	}

	struct timeval tv;
	tv.tv_sec=3;
	tv.tv_usec=0;
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
		m_flags[Init]=false;

		qDebug("QThreadRecvRouteGraph: Find local IP failed!");
		return;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;

	quint16 nPort=(quint16)parseConfig(getAppParentDir()+TOPOLOGY_INI_FILE, TOPOLOGY_ROUTE_GRAPH_PORT, 53254).toUInt();
	nPort=nPort>30000? nPort:53254;
	addr.sin_port=htons(nPort);

	_inet_aton(addresses[0].toString().toLocal8Bit().data(), &addr.sin_addr);

	bind(m_socket, (struct sockaddr*)&addr, sizeof(addr));

	u_long value=1;
	ioctlsocket(m_socket, SIO_RCVALL, &value);

#ifndef Q_OS_WIN
	setuid(getuid());
#endif
}

QThreadRecvRouteGraph::~QThreadRecvRouteGraph()
{
	closesocket(m_socket);
}

void QThreadRecvRouteGraph::run()
{
	if (m_flags[Init])
	{
		int nLen,fromlen;

		struct sockaddr_in from;
		fromlen=sizeof(from);

		while(status(Running))
		{
			if( (nLen=recvfrom(m_socket, m_recvPacket, sizeof(m_recvPacket), 0, (struct sockaddr *)&from, &fromlen)) <0)
			{
				usleep(100);

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
				qDebug("QThreadRecvRouteGraph: unreach %s", inet_ntoa(from.sin_addr));

				emit unreach(ntohl(from.sin_addr.s_addr));
			}
			else if( (icmp->icmp_type == ICMP_TIMXCEED) && (icmp->icmp_code == ICMP_TIMXCEED_INTRANS) )
			{
				qDebug("QThreadRecvRouteGraph: route time out %s", inet_ntoa(from.sin_addr));

				emit timeout(ntohl(from.sin_addr.s_addr));
			}
		}
	}
}

void QThreadRecvRouteGraph::setStatus(QThreadRecvRouteGraph::StatusFlag flag, bool status)
{
	QMutexLocker locker(&m_mutex);
	m_flags[flag]=status;
}

bool QThreadRecvRouteGraph::status(QThreadRecvRouteGraph::StatusFlag flag)
{
	bool f;
	QMutexLocker locker(&m_mutex);
	f=m_flags[flag];

	return f;
}
