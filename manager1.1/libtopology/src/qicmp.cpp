 #include "qicmp.h"

#include <QHostAddress>

#include "qlogger.h"

#define DATA_LEN	56
#define TIME_OUT_ICMP	2000

QIcmp::QIcmp(QObject *parent)
:QObject(parent),
m_pThreadSend(NULL),
m_pThreadRecv(NULL)
{
	m_socket=socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (m_socket == -1)
	{
		logplusError(LIBTOPOLOGY_LOGGER_STR, "Init SOCK_RAW failed!");

		return;
	}

	int size=48*1024;           /* OK if setsockopt fails */
	setsockopt (m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof (size));

#ifndef Q_OS_WIN
	setuid(getuid());
#endif

#ifdef Q_OS_WIN
	m_pid=GetCurrentProcessId();
#else
	m_pid=(getpid() & 0xffff);     /* ICMP ID field is 16 bits */
#endif

	m_pThreadRecv=new QThreadRecvIcmp(m_socket, m_pid);
	connect(m_pThreadRecv, SIGNAL(icmpEcho(quint32)), this, SLOT(icmpEcho(quint32)));
	connect(m_pThreadRecv, SIGNAL(finished()), this, SLOT(threadQuit()));

	m_pThreadSend=new QThreadSendIcmp(m_socket, m_pid);
	connect(m_pThreadSend, SIGNAL(icmpStart()), this, SLOT(icmpStart()));
	connect(m_pThreadSend, SIGNAL(finished()), this, SLOT(threadQuit()));

	m_timer.setInterval(TIME_OUT_ICMP);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	logplusDebug(LIBTOPOLOGY_LOGGER_STR, "Start ICMP send and recv threads to find alive IP...");

	m_pThreadRecv->start();
	m_pThreadSend->start();
}

QIcmp::~QIcmp()
{
	if (m_socket != -1)
	{
		closesocket(m_socket);
	}
}

void QIcmp::destroy()
{
	logplusDebug(LIBTOPOLOGY_LOGGER_STR, "Stop ICMP send and recv threads.");

	m_pThreadRecv->setStatus(QThreadRecvIcmp::Running, false);
	m_pThreadSend->setStatus(QThreadSendIcmp::Running, false);
}

void QIcmp::threadQuit()
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

void QIcmp::addTask(quint32 segment)
{
	if (m_socket == -1)
	{
		return;
	}

	m_tasks.append(segment);

	nextTask();
}

void QIcmp::nextTask()
{
	if (m_pThreadSend == NULL || m_pThreadRecv == NULL)
	{
		logplusError(LIBTOPOLOGY_LOGGER_STR, "The icmp send thread or the recv thread is NULL!!");
		return;
	}

	if (!m_tasks.isEmpty())
	{
		if (!m_pThreadRecv->status(QThreadRecvIcmp::Tasking))
		{
			m_pThreadRecv->setStatus(QThreadRecvIcmp::Tasking, true);
		}
	}
	else
	{
		m_pThreadRecv->setStatus(QThreadRecvIcmp::Tasking, false);
	}

	if (m_pThreadSend->status(QThreadSendIcmp::Tasking) || m_tasks.isEmpty())
	{
		return;
	}

	m_task=m_tasks.takeFirst();
	m_alives.clear();

	logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Find alive IP in segment %1").arg(QHostAddress(m_task).toString()));
	m_pThreadSend->addTask(m_task);
}

void QIcmp::timeout()
{
	m_timer.stop();

	quint32 nSegment=m_task;
	QVector<quint32> alives=m_alives;

	m_pThreadSend->setStatus(QThreadSendIcmp::Tasking, false);
	nextTask();

	emit found(nSegment, alives);
}

void QIcmp::icmpEcho(quint32 ip)
{
	quint32 nSegment=ip&0xffffff00;
 
	if (nSegment != m_task)
	{
		return;
	}

	m_alives.append(ip);
}

void QIcmp::icmpStart()
{
	m_timer.start();
}

QThreadSendIcmp::QThreadSendIcmp(SOCKET socket, pid_t pid, QObject* parent)
:QThread(parent)
{
	m_socket=socket;
	m_pid=pid;

	setStatus(Running, true);
	setStatus(NewTask, false);
	setStatus(Tasking, false);
}

QThreadSendIcmp::~QThreadSendIcmp()
{

}

void QThreadSendIcmp::setStatus(QThreadSendIcmp::StatusFlag flag, bool status)
{
	QMutexLocker locker(&m_mutex);
	m_flags[flag]=status;
}

bool QThreadSendIcmp::status(QThreadSendIcmp::StatusFlag flag)
{
	bool f;
	QMutexLocker locker(&m_mutex);
	f=m_flags[flag];

	return f;
}

void QThreadSendIcmp::run()
{
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;

	while (status(Running))
	{
		if (!status(NewTask))
		{
			msleep(100);
			continue;
		}

		setStatus(NewTask, false);
		setStatus(Tasking, true);

		for (int i=1; i<255; i++)
		{
			addr.sin_addr.s_addr = htonl(m_task+i);

			sendPacket(addr);
		}

		emit icmpStart();
	}

	logplusDebug(LIBTOPOLOGY_LOGGER_STR, "QThreadSendIcmp: Thread quit!");
}

void QThreadSendIcmp::addTask(quint32 segment)
{
	if (status(Tasking))
	{
		logplusDebug(LIBTOPOLOGY_LOGGER_STR, "QThreadSendIcmp: A task is running! Add task failed!");
		return;
	}

	logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("QThreadSendIcmp: Send ICMP in segment %1").arg(QHostAddress(segment).toString()));

	m_task=segment;

	setStatus(NewTask, true);
}

quint16 QThreadSendIcmp::chksum(quint16 *addr, int len)
{       
	int nleft=len;
	int sum=0;
	quint16 *w=addr;
	quint16 answer=0;

	/*把ICMP报头二进制数据以2字节为单位累加起来*/
	while(nleft>1)
	{       
		sum+=*w++;
		nleft-=2;
	}

	/*若ICMP报头为奇数个字节，会剩下最后一字节。把最后一个字节视为一个2字节数据的高字节，这个2字节数据的低字节为0，继续累加*/
	if( nleft==1)
	{       
		*(quint8*)(&answer)=*(quint8*)w;
		sum+=answer;
	}

	sum=(sum>>16)+(sum&0xffff);
	sum+=(sum>>16);
	answer=~sum;

	return answer;
}

int QThreadSendIcmp::pack(int pack_no)
{       
	int packsize;
	Icmp* icmp;
	struct timeval *tval;

	icmp=(Icmp*)m_sendPacket;
	icmp->icmp_type=ICMP_ECHO;
	icmp->icmp_code=0;
	icmp->icmp_cksum=0;
	icmp->icmp_seq=pack_no;
	icmp->icmp_id=m_pid;
	packsize=8+DATA_LEN;
	tval= (struct timeval *)icmp->icmp_data;
	gettimeofday(tval,NULL);    /*记录发送时间*/
	icmp->icmp_cksum=chksum( (quint16*)icmp, packsize); /*校验算法*/

	return packsize;
}

void QThreadSendIcmp::sendPacket(struct sockaddr_in& dest)
{
	int packetsize;
	packetsize=pack(0); /*设置ICMP报头*/

//	logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("QThreadSendIcmp: Send ICMP to %1").arg(inet_ntoa(dest.sin_addr)));

	sendto(m_socket, m_sendPacket, packetsize,0, (struct sockaddr *)&dest, sizeof(dest));
}

QThreadRecvIcmp::QThreadRecvIcmp(SOCKET socket, pid_t pid, QObject* parent)
:QThread(parent)
{
	m_socket=socket;
	m_pid=pid;

	setStatus(Running, true);
	setStatus(Tasking, false);
}

QThreadRecvIcmp::~QThreadRecvIcmp()
{

}

void QThreadRecvIcmp::run()
{
	int nLen;

	socklen_t fromlen;

	struct sockaddr_in from;
	fromlen=sizeof(from);

	struct ip *ip;
	Icmp *icmp;
	int iphdrlen;

	while(status(Running))
	{
		if(((nLen=recvfrom(m_socket, m_recvPacket, sizeof(m_recvPacket), 0, (struct sockaddr *)&from, &fromlen)) <0)
			|| !status(Tasking))
		{
			msleep(100);
			continue;
		}

		ip=(struct ip*)m_recvPacket;
		iphdrlen=ip->ip_hl<<2;    /*求ip报头长度,即ip报头的长度标志乘4*/
		icmp=(Icmp *)(m_recvPacket+iphdrlen);  /*越过ip报头,指向ICMP报头*/
		nLen-=iphdrlen;				/*ICMP报头及ICMP数据报的总长度*/
		if(nLen < 8)				/*小于ICMP报头长度则不合理*/
		{
			logplusWarning(LIBTOPOLOGY_LOGGER_STR, "ICMP packets\'s length is less than 8");

			continue;
		}

		/*确保所接收的是我所发的的ICMP的回应*/
		if( (icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id == m_pid) )
		{
			logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("QThreadRecvIcmp: recv ICMP packet from %1").arg(inet_ntoa(from.sin_addr)));

			emit icmpEcho(ntohl(from.sin_addr.s_addr));
		}
	}

	logplusDebug(LIBTOPOLOGY_LOGGER_STR, "QThreadRecvIcmp: Thread quit!");
}

void QThreadRecvIcmp::setStatus(QThreadRecvIcmp::StatusFlag flag, bool status)
{
	QMutexLocker locker(&m_mutex);
	m_flags[flag]=status;
}

bool QThreadRecvIcmp::status(QThreadRecvIcmp::StatusFlag flag)
{
	bool f;
	QMutexLocker locker(&m_mutex);
	f=m_flags[flag];

	return f;
}
