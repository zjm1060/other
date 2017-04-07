#include "topocommon.h"

#ifndef Q_OS_WIN

#include <QNetworkInterface>
#include <net/if.h>

#include "common.h"

bool setPromiscFlag(SOCKET s, QHostAddress address)
{
	QString strInterface;
	bool bExisted=false;

	QList<QNetworkInterface> interfaces=QNetworkInterface::allInterfaces();
	foreach (QNetworkInterface face, interfaces)
	{
		QList<QNetworkAddressEntry> entries=face.addressEntries();
		foreach (QNetworkAddressEntry entry, entries)
		{
			if (entry.ip() == address)
			{
				bExisted=true;
				strInterface=face.name();
				break;
			}
		}

		if (bExisted)
		{
			break;
		}
	}

	struct ifreq ifr;

	char* name=strInterface.toAscii().data();
	strncpy(ifr.ifr_name, name, strlen(name)+1); 
	if((ioctl(s, SIOCGIFFLAGS, &ifr) == -1)) //获得flag
	{
		qDebug("setPromiscFlag(): Get socket flags failed!");
		return false;
	} 

	ifr.ifr_flags |= IFF_PROMISC; //重置flag标志


	if(ioctl(s, SIOCSIFFLAGS, &ifr) == -1 ) //改变模式
	{ 
		qDebug("setPromiscFlag(): Set socket flags failed!");
		return false;
	}

	return true;
}
#endif

