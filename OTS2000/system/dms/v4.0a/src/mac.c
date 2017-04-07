
#include	"../inc/os_type_def.h"
#include    "../inc/dms_com.h"


BOOL SameMacAddr(char *mac1, char *mac2)
{
	int	i;

	for(i=0; i<6; i++)
	{
		if(mac1[i]!=mac2[i])
			return FALSE;
	}
	return TRUE;
}

#ifdef WINDOWS_OS
#include   <windows.h>     
#include   <wincon.h>     
#include   <stdlib.h>     
#include   <stdio.h>     
#include   <time.h>     
    
    
#define	SIZEOF_DEVICE_NAME	256

BOOL list_netcard_mac(unsigned char *netcard_name, char *mac_addr)
{
	HANDLE hDev;
	char	name[256];

	sprintf(name, "\\\\.\\%s", netcard_name);	
	hDev= CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);	
	if(hDev != INVALID_HANDLE_VALUE)
	{
		char	buf[256];
		int		size;
		int		inbuf= 0x01010101;//OID_802_3_PERMANENT_ADDRESS
		int		dwIoControlCode= 0x00170002;//IOCTL_NDIS_QUERY_STATS
		
		memset(buf, 0, sizeof(buf));
		if(DeviceIoControl(hDev, dwIoControlCode, &inbuf, 4, buf, sizeof(buf), (LPDWORD)&size, NULL))
		{
			//printf("mac hard addr: %02x:%02x:%02x:%02x:%02x:%02x\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5] );
			memcpy(mac_addr, buf, 6);
			CloseHandle(hDev);
			return TRUE;
		}
		
// 		inbuf= 0x01010102;//OID_802_3_CURRENT_ADDRESS
// 		if(DeviceIoControl(hDev, dwIoControlCode, &inbuf, 4, buf, sizeof(buf), (LPDWORD)&size, NULL))
// 			printf("mac soft addr: %02x:%02x:%02x:%02x:%02x:%02x\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5] );
		
		CloseHandle(hDev);
	}
	return FALSE;
}


BOOL GetAnaPtLmtBase(char *mac_addr)
{
	char key_name[ SIZEOF_DEVICE_NAME];
	unsigned char device_info[ SIZEOF_DEVICE_NAME];
	FILETIME file_time;
	char	mac[20];

	HKEY hkey,hsub;
	int index;
	DWORD size;

	index = 0 ;

	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards",
		0, KEY_READ, &hkey) == ERROR_SUCCESS)
	{
		size = SIZEOF_DEVICE_NAME;

		while( RegEnumKeyEx(hkey,index,key_name,&size,NULL,NULL,NULL,&file_time) == ERROR_SUCCESS)
		{
			//get_device_info(index,full_name,device_info,device_description);
			if(RegOpenKeyEx(hkey,key_name, 0, KEY_READ, &hsub) == ERROR_SUCCESS)
			{
				size= SIZEOF_DEVICE_NAME;
				if(RegQueryValueEx(hsub, "ServiceName", NULL, NULL, device_info, &size)==ERROR_SUCCESS)
				{
					if(list_netcard_mac(device_info, mac))
					{
						if(SameMacAddr(mac, mac_addr))
						{
							RegCloseKey( hsub);
							RegCloseKey( hkey);
							return TRUE;
						}
					}
				}
				RegCloseKey( hsub);
			}

			index++;

			size = SIZEOF_DEVICE_NAME;
		}

		RegCloseKey( hkey);
	}

	return FALSE;
}



#elif defined(_AIX)
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

#define	MAX_IFS	16

BOOL GetAnaPtLmtBase(char *mac_addr)
{     
    int sock,len;
    struct ifconf ifc;
	struct ifreq *ifr;
	char buff[8192];
	struct arpreq arp;
	
	
    sock = socket(AF_INET, SOCK_DGRAM, 0);	
	
    ifc.ifc_len = sizeof(buff);
    ifc.ifc_req = (void*)buff;
    if (ioctl(sock, SIOCGIFCONF, &ifc) < 0)
    {
        SendTraceMsg(0,0,"ioctl(SIOCGIFCONF): errno= %d", errno);
		close(sock);
        return FALSE;
    }	
	
	ifr = ifc.ifc_req;
	len = ifc.ifc_len;

    while(len>0)
    {
		unsigned int inc= ifr->ifr_addr.sa_len;

//         if (ifr->ifr_addr.sa_family == AF_INET)
//         {
// 			if (ioctl(sock, SIOCGIFADDR, ifr) != 0) 
// 			{
// 				SendTraceMsg(0,0,"%s ioctl(SIOCGIFADDR) failed: %d\n", ifr->ifr_name, errno);
// 				goto next;
// 			}
// 			printf("\nnet %s %s", ifr->ifr_name, inet_ntoa((*(struct sockaddr_in *)&ifr->ifr_addr).sin_addr));
// 		}
		
		if (ifr->ifr_addr.sa_family == AF_LINK)
		{
			if(SameMacAddr((char*)&(ifr->ifr_addr.sa_data[9]), mac_addr))
			{
				close(sock);
				return TRUE;
			}			
		}

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
	
	close(sock);
	return FALSE;
} //AIX

#elif defined(SUN_OS)
#include <sys/sockio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdlib.h>
#include <unistd.h>
#include <stropts.h>
#include <strings.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if_arp.h>

#define	MAX_IFS	128

BOOL GetAnaPtLmtBase(char *mac_addr)
{     
    int sock;
    struct ifconf ifc;
	struct ifreq *ifr, *ifend;
    struct ifreq ifr_temp;
    struct ifreq ifs[MAX_IFS];
	struct arpreq arp;
	
	
    sock = socket(AF_INET, SOCK_DGRAM, 0);	
	
    ifc.ifc_len = sizeof(ifs);
    ifc.ifc_req = ifs;
    if (ioctl(sock, SIOCGIFCONF, &ifc) < 0)
    {
        SendTraceMsg(0,0,"ioctl(SIOCGIFCONF): errno= %d", errno);
		close(sock);
        return FALSE;
    }	
	
    ifend = ifs + (ifc.ifc_len / sizeof(struct ifreq));
    for (ifr = ifc.ifc_req; ifr < ifend; ifr++)
    {
        if (ifr->ifr_addr.sa_family == AF_INET)
        {
			memset(&ifr_temp, 0, sizeof(ifr_temp));
			
			strncpy(ifr_temp.ifr_name, ifr->ifr_name, sizeof(ifr_temp.ifr_name));
			
			if (ioctl (sock, SIOCGIFADDR, &ifr_temp) < 0)
			{
				SendTraceMsg(0,0,"%s ioctl(SIOCGIFADDR) failed: %d\n", ifr_temp.ifr_name, errno);
				continue;
			}
			
			arp.arp_pa.sa_family = AF_INET;
			arp.arp_ha.sa_family = AF_INET;
			((struct sockaddr_in*)&arp.arp_pa)->sin_addr.s_addr=((struct sockaddr_in *)&ifr_temp.ifr_addr)->sin_addr.s_addr;
			if (ioctl (sock, SIOCGARP, &arp) < 0)
			{
				SendTraceMsg(0,0,"%s ioctl(SIOCGARP) failed: %d\n", ifr_temp.ifr_name, errno);
				continue;
			}
			
			if(SameMacAddr((char*)arp.arp_ha.sa_data, mac_addr))
			{
				close(sock);
				return TRUE;
			}			
			
			
			// 			if (ioctl (sock, SIOCGENADDR, &ifr_temp) < 0)
			// 				printf("%s ioctl(SIOCGENADDR) failed: errno= %d\n", ifr_temp.ifr_name, errno);
			// 			else
			// 				printf("Device %s -> Ethernet %02x:%02x:%02x:%02x:%02x:%02x\n", ifr_temp.ifr_name,
			// 					ifr_temp.ifr_enaddr[0], ifr_temp.ifr_enaddr[1], ifr_temp.ifr_enaddr[2],
			// 					ifr_temp.ifr_enaddr[3], ifr_temp.ifr_enaddr[4], ifr_temp.ifr_enaddr[5]);
		}
    }
	
	close(sock);
	return FALSE;
}

#endif //SUN_OS


/*linux
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdlib.h>

#define	MAX_IFS	128

int main(int   argc,   char*   argv[])   
{     
	struct ifreq *ifr, *ifend;
    struct ifreq ifreq;
    struct ifconf ifc;
    struct ifreq ifs[MAX_IFS];
    int socket;
	
	
    socket = socket(AF_INET, SOCK_DGRAM, 0);	
	
    ifc.ifc_len = sizeof(ifs);
    ifc.ifc_req = ifs;
    if (ioctl(socket, SIOCGIFCONF, &ifc) < 0)
    {
        printf("ioctl(SIOCGIFCONF): error\n");
		close(socket);
        return 0;
    }	
	
    ifend = ifs + (ifc.ifc_len / sizeof(struct ifreq));
    for (ifr = ifc.ifc_req; ifr < ifend; ifr++)
    {
        if (ifr->ifr_addr.sa_family == AF_INET)
        {
            strncpy(ifreq.ifr_name, ifr->ifr_name, sizeof(ifreq.ifr_name));
            if (ioctl (socket, SIOCGIFHWADDR, &ifreq) < 0)
            {
				printf("SIOCGIFHWADDR(%s): %m\n", ifreq.ifr_name);
				close(socket);
				return 0;
            }			
			
			printf("Device %s -> Ethernet %02x:%02x:%02x:%02x:%02x:%02x\n", ifreq.ifr_name,
				(int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[0],
				(int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[1],
				(int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[2],
				(int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[3],
				(int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[4],
				(int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[5]);
		}
    }
	
	close(socket);
    return 0;
}

*/

