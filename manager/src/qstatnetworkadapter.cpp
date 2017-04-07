#include "qstatnetworkadapter.h"

#include "define.h"
#include "common.h"

#ifdef Q_OS_SOLARIS
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <sys/sockio.h>
#endif

#define MAX_NUM_INTERFACES   16

QStatNetworkAdapter::QStatNetworkAdapter(QObject *parent)
: QObject(parent)
{
}

QStatNetworkAdapter::~QStatNetworkAdapter()
{
}

void QStatNetworkAdapter::beferQuery() 
{
	m_vecAdapter.clear(); 
}

void QStatNetworkAdapter::query()
{
#ifdef Q_OS_SOLARIS
	int fd, interface, retn = 0;

	struct ifreq buf[MAX_NUM_INTERFACES]; 
	struct ifconf ifc;
	struct arpreq arp;

	char* ptr;
	char mac[32];

	fd=socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
	{
		return;
	}

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = (caddr_t)buf;
	if (!ioctl(fd, SIOCGIFCONF, (char*)&ifc))
	{
		interface=ifc.ifc_len/sizeof(struct ifreq);

		while (interface-- > 0)
		{
			NetworkAdapter adapter;

			adapter.description=QString::fromLocal8Bit(buf[interface].ifr_name);

			if (!(ioctl(fd, SIOCGIFFLAGS, (char*)&buf[interface])))
			{
				if (buf[interface].ifr_flags & IFF_PROMISC)
				{
					retn++;
				}
			}
			else
			{
				continue;
			}

			if (!(ioctl(fd, SIOCGIFADDR, (char*)&buf[interface])))
			{
				adapter.IPAddress.append(
					QString::fromLocal8Bit(inet_ntoa(((struct sockaddr_in*)(&buf[interface].ifr_addr))->sin_addr)));
			}

			arp.arp_pa.sa_family = AF_INET;
			arp.arp_ha.sa_family = AF_INET;
			((struct sockaddr_in*)&arp.arp_pa)->sin_addr.s_addr=((struct sockaddr_in *)&buf[interface].ifr_addr)->sin_addr.s_addr;
			if (ioctl (fd, SIOCGARP, &arp) >= 0)
			{
				ptr=(char*)arp.arp_ha.sa_data;
				sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", 
					*(unsigned char*)ptr, *(unsigned char*)(ptr+1), *(unsigned char*)(ptr+2), 
					*(unsigned char*)(ptr+3), *(unsigned char*)(ptr+4), *(unsigned char*)(ptr+5));

				adapter.MACAddress=QString::fromLocal8Bit(mac);
			}

			if (!(ioctl(fd, SIOCGIFNETMASK, (char*)&buf[interface])))
			{
				adapter.IPSubnet.append(
					QString::fromLocal8Bit(inet_ntoa(((struct sockaddr_in*)(&buf[interface].ifr_addr))->sin_addr)));
			}

			m_vecAdapter.append(adapter);
		}
	}

	close (fd);

#elif defined(Q_OS_AIX)
	FILE *pp, *pSub;
	char buf[256]; 
	char cmd[64];

	QString strContent;
	QStringList lstSubContent;
	QStringList lstAddr;

	if( (pp = popen("/usr/sbin/lsdev -Cc if", "r")) == NULL )
	{ 
		printf("popen() error!\n"); 
		return;
	} 

	while(fgets(buf, sizeof buf, pp)) 
	{
		strContent=QString::fromLocal8Bit(buf);
		if (strContent.isEmpty())
		{
			continue;
		}

		lstSubContent=strContent.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		if (lstSubContent.size() == 0)
		{
			continue;
		}

		NetworkAdapter adapter;

		adapter.description=lstSubContent[0];

		sprintf(cmd, "/usr/sbin/lsattr -El %s", adapter.description.toLocal8Bit().data());
		if( (pSub = popen(cmd, "r")) != NULL )
		{
			while(fgets(buf, sizeof buf, pSub))
			{
				strContent=QString::fromLocal8Bit(buf);
				if (strContent.isEmpty())
				{
					continue;
				}

				lstSubContent=strContent.split(QRegExp("\\s+"), QString::SkipEmptyParts);
				if (lstSubContent[0] == "alias4" && lstSubContent.size() >=2)
				{
					if (lstSubContent[1].contains(QRegExp("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}")))
					{
						lstAddr=lstSubContent[1].split(QChar(','));
						adapter.IPAddress.append(lstAddr[0]);
						if (lstAddr.size() == 2)
						{
							adapter.IPSubnet.append(lstAddr[1]);
						}
						else
						{
							adapter.IPSubnet.append("");
						}
					}
				}
				else if (lstSubContent[0] == "netaddr" && lstSubContent.size() >=2)
				{
					if (lstSubContent[1].contains(QRegExp("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}")))
					{
						adapter.IPAddress.append(lstSubContent[1]);
					}
				}
				else if (lstSubContent[0] == "netmask" && lstSubContent.size() >=2)
				{
					if (lstSubContent[1].contains(QRegExp("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}")))
					{
						adapter.IPSubnet.append(lstSubContent[1]);
					}
				}
			}

			pclose(pSub);
		}

		int gateway=-1;
		int cost=-1;
		int nameserver=-1;
		sprintf(cmd, "/usr/sbin/mktcpip -S %s", adapter.description.toLocal8Bit().data());
		if( (pSub = popen(cmd, "r")) != NULL )
		{
			while(fgets(buf, sizeof buf, pSub))
			{
				strContent=QString::fromLocal8Bit(buf);
				if (strContent.isEmpty())
				{
					continue;
				}

				lstSubContent=strContent.split(":");
				if (strContent.contains("gateway") || strContent.contains("nameserv"))
				{
					for (int i=0; i<lstSubContent.size(); i++)
					{
						if (lstSubContent[i] == "gateway")
						{
							gateway=i;
						}
						else if (lstSubContent[i] == "cost")
						{
							cost=i;
						}
						else if (lstSubContent[i] == "nameserv")
						{
							nameserver=i;
						}
					}
				}
				else
				{
					if (gateway != -1 && lstSubContent.size() > gateway)
					{
						if (!lstSubContent[gateway].isEmpty())
						{
							adapter.defaultIPGateway.append(lstSubContent[gateway]);
						}
					}

					if (cost != -1 && lstSubContent.size() > cost)
					{
						if (!lstSubContent[cost].isEmpty())
						{
							adapter.gatewayCostMetric.append(lstSubContent[cost].toUShort());
						}
					}

					if (nameserver != -1 && lstSubContent.size() > nameserver)
					{
						if (!lstSubContent[nameserver].isEmpty())
						{
							adapter.DNSServer.append(lstSubContent[nameserver]);
						}
					}
				}
			}

			pclose(pSub);
		}

		if (adapter.description.startsWith("en") || adapter.description.startsWith("et"))
		{
			int num=adapter.description.right(adapter.description.length()-2).toInt();

			bool bFind=false;
			for (int i=0; i<m_vecAdapter.size(); i++)
			{
				if (m_vecAdapter[i].description == QString("en%1").arg(num) || 
					m_vecAdapter[i].description == QString("et%1").arg(num))
				{
					bFind=true;

					adapter.MACAddress=m_vecAdapter[i].MACAddress;
					break;
				}
			}

			if (!bFind)
			{
				sprintf(cmd, "/usr/sbin/lscfg -vl ent%d", num);
				if( (pSub = popen(cmd, "r")) != NULL )
				{
					while(fgets(buf, sizeof buf, pSub))
					{
						strContent=QString::fromLocal8Bit(buf).trimmed();
						if (strContent.isEmpty())
						{
							continue;
						}

						strContent.replace(QRegExp("[.]{2,}"), " ");
						lstSubContent=strContent.split(QRegExp("\\s+"), QString::SkipEmptyParts);
						if (lstSubContent[0] == "Network" && lstSubContent[1] == "Address" && lstSubContent.size() >=3)
						{
							if (!lstSubContent[2].isEmpty())
							{
								adapter.MACAddress=lstSubContent[2].left(2);
								for (int i=2; i<lstSubContent[2].size(); i+=2)
								{
									adapter.MACAddress+=":"+lstSubContent[2].mid(i, 2);
								}
							}
						}
					}

					pclose(pSub);
				}
			}
		}
		
		m_vecAdapter.append(adapter);
	}

	pclose(pp);
#endif
}
