#include "topocommon.h"

#include <QHostAddress>

#include "qlogger.h"
#include "define.h"

#ifdef Q_OS_WIN
#include <Iphlpapi.h>

#pragma comment(lib,"Iphlpapi.lib")

#else
#include <QRegExp>
#endif

#ifdef Q_OS_SOLARIS
// #include <sys/types.h>
// #include <sys/stream.h>
// #include <sys/strstat.h>
// 
// #include <sys/socket.h>
// #include <sys/sockio.h>
// #include <netinet/in.h>
// #include <net/if.h>
// #include <net/route.h>
// 
// #include <inet/arp.h>
// #include <inet/tcp.h>
// #include <netinet/igmp_var.h>
// #include <netinet/ip_mroute.h>
// 
// #include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
// #include <sys/systeminfo.h>
// #include <arpa/inet.h>

#include <stropts.h>
#include <sys/ioctl.h>
#include <sys/tihdr.h>
#include <inet/mib2.h>

typedef struct mib_item_s
{
	struct mib_item_s	*next_item;
	int			group;
	int			mib_id;
	int			length;
	void		*valp;
} mib_item_t;

#define	STR_EXPAND	4
#endif

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

	char* name=strInterface.toLatin1().data();
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

#ifdef Q_OS_WIN
QVector<ArpEntry> getArpTable()
{
	QVector<ArpEntry> arps;

	ULONG tableSize=0;
	::GetIpNetTable(NULL, &tableSize, FALSE);
	if (tableSize > 0)
	{
		unsigned char* buffer=new unsigned char[tableSize];
		PMIB_IPNETTABLE netTable=(PMIB_IPNETTABLE)buffer;
		if (::GetIpNetTable(netTable, &tableSize, FALSE) == NO_ERROR)
		{
			ArpEntry entry;
			for (int i=0; i<(int)netTable->dwNumEntries; i++)
			{
				if (netTable->table[i].dwPhysAddrLen == 0)
				{
					continue;
				}

				entry.mac=octetToString(netTable->table[i].bPhysAddr);
				if (entry.mac == "00:00:00:00:00:00" || entry.mac == "FF:FF:FF:FF:FF:FF")
				{
					continue;
				}

				quint32 nAddr=SWAP_4_BYTES(netTable->table[i].dwAddr);
				entry.address=QHostAddress(nAddr).toString();

				if (entry.address.endsWith(".0") || entry.address.endsWith(".255"))
				{
					continue;
				}

				arps.append(entry);
			}
		}

		SAFE_DELETE_ARRAY(buffer);
	}

	return arps;
}

#elif defined(Q_OS_SOLARIS)
/*
* mibfree: frees a linked list of type (mib_item_t *)
* returned by mibget(); this is NOT THE SAME AS
* mib_item_destroy(), so should be used for objects
* returned by mibget() only
*/
void mibfree(mib_item_t *firstitem)
{
	mib_item_t *lastitem;

	while (firstitem != NULL)
	{
		lastitem = firstitem;
		firstitem = firstitem->next_item;
		if (lastitem->valp != NULL)
			free(lastitem->valp);
		free(lastitem);
	}
}

mib_item_t *mibget(int sd)
{
	/*
	* buf is an automatic for this function, so the
	* compiler has complete control over its alignment;
	* it is assumed this alignment is satisfactory for
	* it to be casted to certain other struct pointers
	* here, such as struct T_optmgmt_ack * .
	*/
	uintptr_t	buf[512 / sizeof (uintptr_t)];
	int			flags;
	int			i, j, getcode;
	struct strbuf		ctlbuf, databuf;
	struct T_optmgmt_req	*tor = (struct T_optmgmt_req *)buf;
	struct T_optmgmt_ack	*toa = (struct T_optmgmt_ack *)buf;
	struct T_error_ack	*tea = (struct T_error_ack *)buf;
	struct opthdr		*req;
	mib_item_t		*first_item = NULL;
	mib_item_t		*last_item  = NULL;
	mib_item_t		*temp;

	tor->PRIM_type = T_SVR4_OPTMGMT_REQ;
	tor->OPT_offset = sizeof (struct T_optmgmt_req);
	tor->OPT_length = sizeof (struct opthdr);
	tor->MGMT_flags = T_CURRENT;


	/*
	* Note: we use the special level value below so that IP will return
	* us information concerning IRE_MARK_TESTHIDDEN routes.
	*/
	req = (struct opthdr *)&tor[1];
	req->level = EXPER_RAWIP;
	req->name  = 0;
	req->len   = 1;

	ctlbuf.buf = (char *)buf;
	ctlbuf.len = tor->OPT_length + tor->OPT_offset;
	flags = 0;
	if (putmsg(sd, &ctlbuf, (struct strbuf *)0, flags) == -1)
	{
		logplusError(LIBTOPOLOGY_LOGGER_STR, "mibget: putmsg(ctl) failed");
		goto error_exit;
	}

	/*
	* Each reply consists of a ctl part for one fixed structure
	* or table, as defined in mib2.h.  The format is a T_OPTMGMT_ACK,
	* containing an opthdr structure.  level/name identify the entry,
	* len is the size of the data part of the message.
	*/
	req = (struct opthdr *)&toa[1];
	ctlbuf.maxlen = sizeof (buf);
	j = 1;
	for (;;)
	{
		flags = 0;
		getcode = getmsg(sd, &ctlbuf, (struct strbuf *)0, &flags);
		if (getcode == -1)
		{
			logplusError(LIBTOPOLOGY_LOGGER_STR, "mibget getmsg(ctl) failed");
			goto error_exit;
		}

		if (getcode == 0 && ctlbuf.len >= sizeof (struct T_optmgmt_ack) 
			&& toa->PRIM_type == T_OPTMGMT_ACK && toa->MGMT_flags == T_SUCCESS && req->len == 0)
		{
			return (first_item);		/* this is EOD msg */
		}

		if (ctlbuf.len >= sizeof (struct T_error_ack) && tea->PRIM_type == T_ERROR_ACK)
		{
			goto error_exit;
		}

		if (getcode != MOREDATA || ctlbuf.len < sizeof (struct T_optmgmt_ack) || toa->PRIM_type != T_OPTMGMT_ACK || toa->MGMT_flags != T_SUCCESS)
		{
			goto error_exit;
		}

		temp = (mib_item_t *)malloc(sizeof (mib_item_t));
		if (temp == NULL)
		{
			logplusError(LIBTOPOLOGY_LOGGER_STR, "mibget malloc failed");
			goto error_exit;
		}

		if (last_item != NULL)
			last_item->next_item = temp;
		else
			first_item = temp;
		last_item = temp;
		last_item->next_item = NULL;
		last_item->group = req->level;
		last_item->mib_id = req->name;
		last_item->length = req->len;
		last_item->valp = malloc((int)req->len);
		if (last_item->valp == NULL)
			goto error_exit;

		databuf.maxlen = last_item->length;
		databuf.buf    = (char *)last_item->valp;
		databuf.len    = 0;
		flags = 0;
		getcode = getmsg(sd, (struct strbuf *)0, &databuf, &flags);
		if (getcode == -1)
		{
			logplusError(LIBTOPOLOGY_LOGGER_STR, "mibget getmsg(data) failed");
			goto error_exit;
		}
		else if (getcode != 0)
		{
			goto error_exit;
		}

		j++;
	}
	/* NOTREACHED */

error_exit:;
	mibfree(first_item);
	return (NULL);
}

int mibopen(void)
{
	int	sd;

	sd = open("/dev/arp", O_RDWR);
	if (sd == -1)
	{
		logplusError(LIBTOPOLOGY_LOGGER_STR, "arp open failed!");
		return (-1);
	}

	if (ioctl(sd, I_PUSH, "tcp") == -1)
	{
		logplusError(LIBTOPOLOGY_LOGGER_STR, "tcp I_PUSH failed!");
		(void) close(sd);
		return (-1);
	}

	if (ioctl(sd, I_PUSH, "udp") == -1)
	{
		logplusError(LIBTOPOLOGY_LOGGER_STR, "udp I_PUSH failed!");
		(void) close(sd);
		return (-1);
	}

	if (ioctl(sd, I_PUSH, "icmp") == -1)
	{
		logplusError(LIBTOPOLOGY_LOGGER_STR, "icmp I_PUSH failed!");
		(void) close(sd);
		return (-1);
	}

	return (sd);
}

void arp_report(mib_item_t *item, int ipNetToMediaEntrySize, QVector<ArpEntry>& table)
{
	int		jtemp = 0;
	char		ifname[LIFNAMSIZ + 1];
	char		abuf[MAXHOSTNAMELEN + 1];
	char		maskbuf[STR_EXPAND * OCTET_LENGTH + 1];
	char		flbuf[32];	/* ACE_F_ flags */
	char		xbuf[STR_EXPAND * OCTET_LENGTH + 1];
	mib2_ipNetToMediaEntry_t	*np;

	ArpEntry entry;

	/* 'for' loop 1: */
	for (; item; item = item->next_item)
	{
		if (!(item->group == MIB2_IP && item->mib_id == MIB2_IP_MEDIA))
			continue; /* 'for' loop 1 */

		/* 'for' loop 2: */
		for (np = (mib2_ipNetToMediaEntry_t *)item->valp; (char *)np < (char *)item->valp+item->length;
			np = (mib2_ipNetToMediaEntry_t *)((char *)np+ipNetToMediaEntrySize))
		{
			entry.address=QHostAddress(SWAP_4_BYTES(np->ipNetToMediaNetAddress)).toString();
			if (np->ipNetToMediaPhysAddress.o_length <= 0)
			{
				continue;
			}
			
			entry.mac=octetToString((unsigned char*)np->ipNetToMediaPhysAddress.o_bytes);

			if (entry.address.endsWith(".0") || entry.address.endsWith(".255"))
			{
				continue;
			}

			table.append(entry);
		} /* 'for' loop 2 ends */
	} /* 'for' loop 1 ends */
}

QVector<ArpEntry> getArpTable()
{
	mib_item_t	*item = NULL;
	int		sd = -1;

	QVector<ArpEntry> table;

	sd = mibopen();
	if (sd == -1)
	{
		logplusError(LIBTOPOLOGY_LOGGER_STR, "can't open mib stream");
		return table;
	}

	if ((item = mibget(sd)) == NULL)
	{
		close(sd);
		logplusError(LIBTOPOLOGY_LOGGER_STR, "mibget() failed");
		return table;
	}

	/* Extract constant sizes - need do once only */
	int ipNetToMediaEntrySize;

	for (mib_item_t	*it=item; it; it = it->next_item)
	{
		if (it->mib_id != 0)
			continue; /* 'for' loop 1 */

		if (it->group == MIB2_IP)
		{
			mib2_ip_t	*ip = (mib2_ip_t *)it->valp;

			ipNetToMediaEntrySize = ip->ipNetToMediaEntrySize;

			break;
		}
	} /* 'for' loop 1 ends */

	arp_report(item, ipNetToMediaEntrySize, table);

	mibfree(item);
	close(sd);

	return table;
}

#else
QVector<ArpEntry> getArpTable()
{
	QVector<ArpEntry> arps;

	FILE *pp;
	char buf[256];

	QString strContent;

	if( (pp = popen("/usr/sbin/arp -a", "r")) == NULL )
	{ 
		printf("popen() error!\n"); 
		return arps;
	} 

	QRegExp rx("((\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])(\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])){3})");
	QRegExp rx2("(([0-9A-Fa-f]{1,2})(:[0-9A-Fa-f]{1,2}){5})");

	int nPos;
	while(fgets(buf, sizeof buf, pp)) 
	{
		strContent=QString::fromLocal8Bit(buf).trimmed();
//		logplusDebug(LIBTOPOLOGY_LOGGER_STR, strContent);
		if (strContent.isEmpty())
		{
			continue;
		}

		ArpEntry entry;

		nPos=rx.indexIn(strContent);
		if (nPos == -1)
		{
			continue;
		}

		entry.address=rx.cap(0);
		if (entry.address.endsWith(".0") || entry.address.endsWith(".255"))
		{
			continue;
		}

		nPos=rx2.indexIn(strContent);
		entry.mac=convertToStdMAC(rx2.cap(0));
		if (!entry.mac.isEmpty())
		{
			arps.append(entry);

//			logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1 ---> %2").arg(entry.address).arg(entry.mac));
		}
	}

	pclose(pp);

	return arps;
}
#endif

QString octetToString(const unsigned char* oct)
{
// 	char strMac[18];
// 
// #ifndef Q_OS_AIX
// 	sprintf(strMac, "%02X:%02X:%02X:%02X:%02X:%02X", 
// 		*(unsigned char*)oct, *(unsigned char*)(oct+1), *(unsigned char*)(oct+2), 
// 		*(unsigned char*)(oct+3), *(unsigned char*)(oct+4), *(unsigned char*)(oct+5));
// #else
// 	sprintf(strMac, "%hhX:%hhX:%hhX:%hhX:%hhX:%hhX", 
// 		*(unsigned char*)oct, *(unsigned char*)(oct+1), *(unsigned char*)(oct+2), 
// 		*(unsigned char*)(oct+3), *(unsigned char*)(oct+4), *(unsigned char*)(oct+5));
// #endif
// 
// 	return strMac;

	QString strMacAddress;
	for (int i=0; i<6; i++)
	{
		strMacAddress+=(i==0? QString("%1").arg(*(oct+i), 2, 16, QLatin1Char('0')):QString(":%1").arg(*(oct+i), 2, 16, QLatin1Char('0')));
	}

	return strMacAddress.toUpper();
}

