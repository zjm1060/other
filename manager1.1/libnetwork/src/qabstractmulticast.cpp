#include "qabstractmulticast.h"

#include "common.h"

#ifdef Q_OS_WIN
#include <WS2tcpip.h>
#else
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#endif

#include "qdeafactory.h"
#include "qlogger.h"
#include "qutildef.h"
#include "qglobalinfo.h"

QAbstractMulticast::QAbstractMulticast(QObject* parent/* =0 */)
:QObject(parent)
{
	QString strAddr=parseConfig(getAppParentDir()+INI_FILE, "network/multicastAddress", "225.0.0.36").toString();
	m_address=QHostAddress(strAddr);

	m_nDefaultPort=m_nPort=(quint16)parseConfig(getAppParentDir()+INI_FILE, "network/multicastPort", 2536).toUInt();
}

QAbstractMulticast::~QAbstractMulticast()
{

}

void QAbstractMulticast::setPort(quint16 port)
{
	m_nPort=port;
}

quint16 QAbstractMulticast::defaultPort()
{
	return m_nDefaultPort;
}

bool QAbstractMulticast::bindAdapter(int fd)
{
	Q_UNUSED(fd);

	return true;
}

void QAbstractMulticast::adapterChanged(QUdpSocket* s, const QString & address/* =QString */)
{
	if (s == NULL)
	{
		return;
	}

	int fd;  
	struct ip_mreq mreq;  

	u_int yes=1; /*** MODIFICATION TO ORIGINAL */  

	/* create what looks like an ordinary UDP socket */  
	if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0)   
	{
		logplusError(LIBNETWORK_LOGGER_STR, "Multicast: socket error!");

		return;  
	}  


	/**** MODIFICATION TO ORIGINAL */  
	/* allow multiple sockets to use the same PORT number */  
	if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,(char*)&yes,sizeof(yes)) < 0)   
	{  
		logplusError(LIBNETWORK_LOGGER_STR, "Multicast: Reusing ADDR failed!");
	}  
	/*** END OF MODIFICATION TO ORIGINAL */  

	/* set up destination address */
	if (!bindAdapter(fd))
	{
		return;
	}

	if (!address.isEmpty())
	{
		struct in_addr inaddr;

		if(_inet_aton(address.toLatin1().data(), &inaddr))	//设置默认组播发送端口
		{
//			logplusDebug(LIBNETWORK_LOGGER_STR, QString("Multicast: setsockopt IP_MULTICAST_IF in address %1!").arg(address));
			if(setsockopt(fd,IPPROTO_IP, IP_MULTICAST_IF,(char *)&inaddr, sizeof(inaddr)) < 0)
			{
				logplusError(LIBNETWORK_LOGGER_STR, "Multicast: setsockopt IP_MULTICAST_IF failed!");
			}
		}
	}

//	logplusDebug(LIBNETWORK_LOGGER_STR, "Multicast: setsockopt IP_ADD_MEMBERSHIP in any address!");

	/* use setsockopt() to request that the kernel join a multicast group */  
	mreq.imr_multiaddr.s_addr=inet_addr(m_address.toString().toLatin1());  
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);  
	if (setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char*)&mreq,sizeof(mreq)) < 0)   
	{  
		logplusError(LIBNETWORK_LOGGER_STR, "Multicast: setsockopt failed!");
	} 

	s->setSocketDescriptor(fd);
}
