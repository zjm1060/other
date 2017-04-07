#include	"../../../dms/v4.0a/inc/dms_com.h"
#include <stdio.h>
#include <stdlib.h> 

extern char	local_ipaddr[MAX_HOST_IPADDR_NUM][MAX_IPADDR_SIZE];
extern int		local_sum_net_num;

#ifdef	SUN_OS
#include 	<sys/socket.h>  /* for AF_INET        */
#include 	<netinet/in.h>  /* for struct in_addr */
#include 	<arpa/inet.h>   /* for inet_ntoa()    */
#include	<unistd.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<netdb.h>
#include	<sys/ioctl.h>
#include	<sys/time.h>
#include	<net/if.h>
#include	<errno.h>
#ifndef _AIX
#include	<sys/sockio.h>
#endif


struct  in_addr local_net_addr[MAX_HOST_IPADDR_NUM];

int InitLocalAdapterInfo(void)
{
	int i;

	for(i=0;i<local_sum_net_num;i++)
	{
		if(-1==(local_net_addr[i].s_addr= inet_addr(local_ipaddr[i])))
		{
			SendTraceMsg(0,0,"local net ipaddr is errno: %s", local_ipaddr[i]);
			return -1;
		}
	}
	return 0;
}

#ifdef _AIX
/****************************************************************************
this one is for AIX 
****************************************************************************/
void GetLocalNetLineState(int local_line_connected[])
{
	char buff[8192];
	int fd;
	struct ifconf ifc;
	struct ifreq *ifr=NULL;
	struct in_addr ipaddr;
	int	net, len;

	for(net=0;net<local_sum_net_num;net++)
		local_line_connected[net]= 0;	
	
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{
		SendTraceMsg(0,0,"Fail to create a sock for get local net line state");
		return;
	}
	
	ifc.ifc_len = sizeof(buff);
	ifc.ifc_buf = buff;
	
	if (ioctl(fd, SIOCGIFCONF, &ifc) != 0) 
	{
		SendTraceMsg(0,0,"Fail to get interface conf: errno=%d", errno);
		close(fd);
		return;
	}
	
	ifr = ifc.ifc_req;
	len = ifc.ifc_len;
	
	/* Loop through interfaces, looking for given IP address */
	while(len>0)
	{
		unsigned int inc;

		inc = ifr->ifr_addr.sa_len;

        if (ifr->ifr_addr.sa_family == AF_INET)
        {
			if (ioctl(fd, SIOCGIFADDR, ifr) != 0) 
			{
				SendTraceMsg(0,0,"Fail to get interface %s ipaddr: errno=%d", ifr->ifr_name, errno);
				goto next;
			}
	
			ipaddr = (*(struct sockaddr_in *)&ifr->ifr_addr).sin_addr;

			for(net=0;net<local_sum_net_num;net++)
			{
				if(ipaddr.s_addr!=local_net_addr[net].s_addr)
					continue;

				//find same ipaddr
				if (ioctl(fd, SIOCGIFFLAGS, ifr) != 0) 
				{
					SendTraceMsg(0,0,"Fail to get net%d %s flags: errno=%d", net, ifr->ifr_name, errno);
					goto next;
				}
				
				if (!(ifr->ifr_flags & IFF_UP)) 
				{
					SendTraceMsg(0,0,"net%d %s is down", net, ifr->ifr_name);
					goto next;
				}
				
				if (!(ifr->ifr_flags & IFF_BROADCAST)) 
				{
					SendTraceMsg(0,0,"net%d %s: BROADCAST is unsupported", net, ifr->ifr_name);
					goto next;
				}
				
				if (!(ifr->ifr_flags & IFF_RUNNING)) 
				{
					SendTraceMsg(0,0,"net%d %s is not lined", net, ifr->ifr_name);
					goto next;
				}
				
				local_line_connected[net]= 1;	
				SendTraceMsg(0,0,"net%d %s is up and lined", net, ifr->ifr_name);
				
			}	//for
		}//if ( AF_INET)

	next:
		/*
		* Patch from Archie Cobbs (archie (at) whistle.com).  The
		* addresses in the SIOCGIFCONF interface list have a
		* minimum size. Usually this doesn't matter, but if
		* your machine has tunnel interfaces, etc. that have
		* a zero length "link address", this does matter.  */
		
		if (inc < sizeof(ifr->ifr_addr))
			inc = sizeof(ifr->ifr_addr);
		inc += IFNAMSIZ;
		
		ifr = (struct ifreq*) (((char*) ifr) + inc);
		len -= inc;		
	}
	
	close(fd);

} //_AIX

#else
/* this works for Linux 2.2, Solaris 2.5, SunOS4, HPUX 10.20, OSF1
  V4.0, Ultrix 4.4, SCO Unix 3.2, IRIX 6.4 and FreeBSD 3.2.
  
It probably also works on any BSD style system.  */

void GetLocalNetLineState(int local_line_connected[])
{
	struct ifconf ifc;
	char buff[8192];
	int fd, i, n;
	struct ifreq *ifr=NULL;
	struct in_addr ipaddr;
	int	net;
	
	for(net=0;net<local_sum_net_num;net++)
		local_line_connected[net]= 0;	
	
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{
		SendTraceMsg(0,0,"Fail to create a sock for get local net line state");
		return;
	}
	
	ifc.ifc_len = sizeof(buff);
	ifc.ifc_buf = buff;
	
	if (ioctl(fd, SIOCGIFCONF, &ifc) != 0) 
	{
		SendTraceMsg(0,0,"Fail to get interface conf: errno=%d", errno);
		close(fd);
		return;
	}
	
	ifr = ifc.ifc_req;
	n = ifc.ifc_len / sizeof(struct ifreq);
	
	/* Loop through interfaces, looking for given IP address */
	for (i=0;i<n;i++) 
	{
		if (ioctl(fd, SIOCGIFADDR, &ifr[i]) != 0) 
		{
			SendTraceMsg(0,0,"Fail to get interface %s ipaddr: errno=%d", ifr[i].ifr_name, errno);
			continue;
		}
		
		ipaddr = (*(struct sockaddr_in *)&ifr[i].ifr_addr).sin_addr;
		for(net=0;net<local_sum_net_num;net++)
		{
			if(ipaddr.s_addr!=local_net_addr[net].s_addr)
				continue;
			
			//find same ipaddr
			if (ioctl(fd, SIOCGIFFLAGS, &ifr[i]) != 0) 
			{
				SendTraceMsg(0,0,"Fail to get net%d %s flags: errno=%d", net, ifr[i].ifr_name, errno);
				break;
			}
			
			if (!(ifr[i].ifr_flags & IFF_UP)) 
			{
				SendTraceMsg(0,0,"net%d %s is down", net, ifr[i].ifr_name);
				break;
			}
			
			if (!(ifr[i].ifr_flags & IFF_BROADCAST)) 
			{
				SendTraceMsg(0,0,"net%d %s: BROADCAST is unsupported", net, ifr[i].ifr_name);
				break;
			}
			
			if (!(ifr[i].ifr_flags & IFF_RUNNING)) 
			{
				SendTraceMsg(0,0,"net%d %s is not lined", net, ifr[i].ifr_name);
				break;
			}
			
			local_line_connected[net]= 1;	
			SendTraceMsg(0,0,"net%d %s is up and lined", net, ifr[i].ifr_name);
			
		}	
		
	}
	
	close(fd);
}
#endif //BSD system


// void GetLocalNetLineState(int local_line_connected[])
// {
// 	struct lifconf lifc;
// 	char buff[8192];
// 	int fd, i, n;
// 	struct lifreq *lifr=NULL;
// 	struct in_addr ipaddr;
// 	int	net;
// 	
// 	for(net=0;net<local_sum_net_num;net++)
// 		local_line_connected[net]= 0;	
// 
// 	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
// 	{
// 		SendTraceMsg(0,0,"Fail to create a sock for get local net line state");
// 		return;
// 	}
// 	
// 	lifc.lifc_len = sizeof(buff);
// 	lifc.lifc_buf = buff;
// 	
// 	if (ioctl(fd, SIOCGLIFCONF, &lifc) != 0) 
// 	{
// 		SendTraceMsg(0,0,"Fail to get interface conf: errno=%d", errno);
// 		close(fd);
// 		return;
// 	}
// 	
// 	lifr = lifc.lifc_req;
// 	n = lifc.lifc_len / sizeof(struct lifreq);
// 	
// 	/* Loop through interfaces, looking for given IP address */
// 	for (i=n-1;i>=0;i--) 
// 	{
// 		if (ioctl(fd, SIOCGLIFADDR, &lifr[i]) != 0) 
// 		{
// 			SendTraceMsg(0,0,"Fail to get interface %s ipaddr: errno=%d", lifr[i].lifr_name, errno);
// 			continue;
// 		}
// 		
// 		ipaddr = (*(struct sockaddr_in *)&lifr[i].lifr_addr).sin_addr;
// 		for(net=0;net<local_sum_net_num;net++)
// 		{
// 			if(ipaddr.s_addr!=local_net_addr[net].s_addr)
// 				continue;
// 
// 			//find same ipaddr
// 			if (ioctl(fd, SIOCGLIFFLAGS, &lifr[i]) != 0) 
// 			{
// 				SendTraceMsg(0,0,"Fail to get net%d %s flags: errno=%d", net, lifr[i].lifr_name, errno);
// 				break;
// 			}
// 			
// 			if (!(lifr[i].lifr_flags & IFF_UP)) 
// 			{
// 				SendTraceMsg(0,0,"net%d %s is down", net, lifr[i].lifr_name);
// 				break;
// 			}
// 			
// 			if (!(lifr[i].lifr_flags & IFF_UP)) 
// 			{
// 				SendTraceMsg(0,0,"net%d %s is down", net, lifr[i].lifr_name);
// 				break;
// 			}
// 
// 			if (!(lifr[i].lifr_flags & IFF_BROADCAST)) 
// 			{
// 				SendTraceMsg(0,0,"net%d %s: BROADCAST is unsupported", net, lifr[i].lifr_name);
// 				break;
// 			}
// 			
// 			if (!(lifr[i].lifr_flags & IFF_RUNNING)) 
// 			{
// 				SendTraceMsg(0,0,"net%d %s is not running", net, lifr[i].lifr_name);
// 				break;
// 			}
// 
// 			local_line_connected[net]= 1;	
// 
// 		}	
// 		
// 	}
// 	
// 	close(fd);
// }
#endif //UNIX


#ifdef	WINDOWS_OS
#include "../inc/iphlpapi.h"
#pragma comment(lib,"../lib/Iphlpapi.lib") 

char AdapterName[MAX_HOST_IPADDR_NUM][MAX_ADAPTER_NAME_LENGTH + 4];


BOOL IsLocalNetLineConnected(int netNo)
{
	HANDLE hDev;
	char	name[256];
	
	sprintf(name, "\\\\.\\%s", AdapterName[netNo]);	
	hDev= CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);	
	if(hDev != INVALID_HANDLE_VALUE)
	{
		ULONG	nConnectedState;
		int		size;
		int		inbuf= 0x00010114;//OID_GEN_MEDIA_CONNECT_STATUS
		int		dwIoControlCode= 0x00170002;//IOCTL_NDIS_QUERY_STATS
		
		if(DeviceIoControl(hDev, dwIoControlCode, &inbuf, 4, &nConnectedState, sizeof(nConnectedState), (LPDWORD)&size, NULL))
		{
			CloseHandle(hDev);
			SendTraceMsg(0,0,"Net %d line is %s", netNo, (nConnectedState == 0)? "connected":"disconnected");
			return (nConnectedState == 0);
		}
		else
            SendTraceMsg(0,0,"DeviceIoControl returned an error = %d\n", GetLastError());
		
		CloseHandle(hDev);
	}
	SendTraceMsg(0,0,"Fail to open net %d dev %s", netNo, name);
	return FALSE;
}

void GetLocalNetLineState(int local_line_connected[])
{
	int	j;
	
	for(j=0; j<local_sum_net_num; j++)
	{
		if(IsLocalNetLineConnected(j)) 
			local_line_connected[j]= 1;
		else
			local_line_connected[j]= 0;
	}
}

int InitLocalAdapterInfo(void)
{
	PIP_ADAPTER_INFO pAdapterInfo; 
	PIP_ADAPTER_INFO pAdapter = NULL; 
	DWORD dwRetVal = 0; 
	ULONG ulOutBufLen; 

	pAdapterInfo=(PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO)); 
	ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	// 第一次调用GetAdapterInfo获取ulOutBufLen大小 
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) 
	{ 
		free(pAdapterInfo); 
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
	}
	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) != NO_ERROR) 
	{ 
		SendTraceMsg(0,0,"Fail to get adapters name"); 
		free(pAdapterInfo); 
		return -1;
	}

	SendTraceMsg(0,0,"Succeed to get adapters info"); 

	pAdapter = pAdapterInfo; 
	while (pAdapter) 
	{ 
		//可能网卡有多IP,因此通过循环去判断
		IP_ADDR_STRING *pIpAddrString =&(pAdapter->IpAddressList);
		do 
		{
			int i;
			for(i=0; i<local_sum_net_num; i++)
			{
				if(!strcmp(pIpAddrString->IpAddress.String, local_ipaddr[i]))
				{
					strcpy(AdapterName[i], pAdapter->AdapterName);
					break;
				}
			}
			pIpAddrString=pIpAddrString->Next;
		} while (pIpAddrString);

		pAdapter = pAdapter->Next; 
	} 
	return 0;
}
#endif  //WINDOWS_OS
