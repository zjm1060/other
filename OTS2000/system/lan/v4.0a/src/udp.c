/***************************************************************************************
FILENAME:	udp.c

FUNCTION:	
	
	source code of udp function,include:
 	SOCKET 	Udp_init(void);
 	SOCKET 	Udp_open(char *udphost);
 	void 	UdpClose(void);
 	int  	int Udp_send(SOCKET sock,char *buff,int len);
 	int	 UdpRecv(sock,buff,len);

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	start to update the file from H9000 V3.0 to V4.0		yyp

****************************************************************************************/

#include "../inc/lanhead_all.h"

#ifdef	WINDOWS_OS
  #define		errno					WSAGetLastError()
#endif

SOCKADDR_IN		udp_addr[2];
SOCKADDR_IN		udp_addr_recv;
SOCKET udp_init(int port)
{
	SOCKET 			sock;
	int len,val;
	struct ip_mreq mreq;
	/* Create a socket for the communications */
    
    sock = socket(AF_INET, SOCK_DGRAM,0);
	if (sock == INVALID_SOCKET) 
	{
            printf("\nudp.c:: creat socket() failed\n");
            closesocket(sock);
            return(INVALID_SOCKET);
    	}
	len = sizeof(val);
	val = MAX_UDPRCV_BUF;
	if(setsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char *)&val,len) == -1 )
	{
	
		printf("setsockopt error\n");
	}
	if(getsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char *)&val,&len) == -1 )
	{
		printf("setsockopt error\n");
	}
	else
	{
#ifdef DEBUG
		printf("\nSO_RCVBUF =%d\n",val);
#endif
	}	

    memset((char *) &udp_addr_recv,0,sizeof(struct sockaddr_in));
	udp_addr_recv.sin_family      = AF_INET;
	udp_addr_recv.sin_addr.s_addr = htonl(INADDR_ANY);
	udp_addr_recv.sin_port        = htons(port);
    
    if ( bind(sock,(struct sockaddr *)&udp_addr_recv,sizeof(struct sockaddr_in))==SOCKET_ERROR) 
	{
#ifdef MSWIN32
		printf("\nudp.c:: bind error:%d is the error.\n ", WSAGetLastError());
#else
		printf("\nudp.c:: bind error\n ");
#endif
		closesocket(sock);
		return(INVALID_SOCKET);		
	}
	
	if(g_is_grp_brd == TRUE)
	{
		//added by yyp for multi brd  2010.10.9
		mreq.imr_multiaddr.s_addr=inet_addr(g_brd_ip[0]);
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);
		if ( setsockopt ( sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,(const char *) &mreq,	sizeof(struct ip_mreq)) == -1) 
		{
            printf("\nudp.c:: setsockopt(IP_ADD_MEMBERSHIP) %s failed\n",g_brd_ip[0]);
            closesocket(sock);
            return(INVALID_SOCKET);
		}
	}

	return(sock);
}

SOCKET udp_open(char hostid,short net)
{
	SOCKET			sock;
	int len,val;
	int			on;
	int dwRoute = 16;
	if(net > 1)
	{
		printf("udp.c: error, net id =%d is too big!\n",net );
		return(INVALID_SOCKET);
	}
	sock = socket(AF_INET, SOCK_DGRAM,0);
	if (sock == INVALID_SOCKET) 
	{
#ifdef DEBUG           
       		printf("\nudp.c:: creat socket() failed\n");
#endif            
		closesocket(sock);
		return(INVALID_SOCKET);
    	}
	len = sizeof(val);
	val = MAX_UDPSND_BUF;    	
	if(setsockopt(sock,SOL_SOCKET,SO_SNDBUF,(char *)&val,len) == -1 )
	{
		printf("setsockopt error\n");
	}    			

	udp_addr[net].sin_addr.s_addr=inet_addr(g_brd_ip[net]);
	printf("\nudp.c:: host.addr=%s\n",g_brd_ip[net]);
  	udp_addr[net].sin_family          = AF_INET;
	udp_addr[net].sin_port            = htons(UDP_PORT);
	if(g_is_grp_brd != TRUE)
	{	
		on=1;
		if(setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(char*)&on,sizeof(on)) == -1 )
		{
			printf("setsockopt SO_BROADCAST failed: %s\n", strerror(errno));
		}
	}
	else
	{
		//added by yyp for multi brd  2010.10.9
		setsockopt(sock,IPPROTO_IP,IP_MULTICAST_TTL,(char*)&dwRoute,sizeof(dwRoute));
		//for dual net, needed to be modified later 2010.10.9
		//unsigned long addr = inet_addr("192.168.3.2");		
		//setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr, sizeof(addr));
	}
	return(sock);
}

void udp_close(SOCKET  sock)
{
	closesocket(sock);
} 

int udp_send(SOCKET sock,char *buff,int len,short net)
{                        
	int    			rc;
	rc=sendto(sock,buff,len,0,(struct sockaddr *)&udp_addr[net], sizeof(udp_addr[net]));
	if(rc!=len)
	{        
#ifdef MSWIN32
	#ifdef DEBUG         
		printf( "\nudp.c:: sendto ERROR %d \n",WSAGetLastError());
	#endif
#else
		printf( "\nudp.c:: sendto ERROR len= %d, errno= %d %s\n", len, errno, strerror(errno));

#endif

		return(-1);
	}
	return(rc);
}

int udp_recv(SOCKET	sock,char *buff,int len)
{
	int      		nbytes;
	int             fromlen;
	SOCKADDR_IN	    from_addr;
	fromlen=sizeof(from_addr);
	nbytes=recvfrom(sock, buff,len,0,(struct sockaddr *)&from_addr,&fromlen);
	udp_addr_recv.sin_addr.s_net = 10;
	udp_addr_recv.sin_addr.s_host = 176;
	udp_addr_recv.sin_addr.s_lh = 134;
//	udp_addr_recv.sin_addr.s_net = inet_addr("10.176.134.255");
	if(nbytes<=0) 
	{
#ifdef MSWIN32
	#ifdef DEBUG
		printf("\nudp.c:: recvfrom ERROR %d \n",WSAGetLastError());
	#endif
#else
	#ifdef DEBUG
		printf("\nudp.c:: recvfrom ERROR %d \n");
	#endif	
#endif

	return(-1); 
	}
	return(nbytes);
}

