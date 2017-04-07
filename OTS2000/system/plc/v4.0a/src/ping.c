#include "../inc/plchead_all.h"
#define WIN32_LEAN_AND_MEAN

/* IP header structure */

typedef struct _iphdr
{
#ifdef SUN
    unsigned int    version:4;        /* Version of IP*/
    unsigned int    h_len:4;        /* Length of the header */
#else
    unsigned int    h_len:4;        /* Length of the header */
    unsigned int    version:4;        /* Version of IP*/
#endif    
    unsigned char     tos;            /* Type of service*/
    unsigned short    total_len;        /* Total length of the packet*/
    unsigned short    ident;            /* Unique identifier*/
    unsigned short    frag_and_flags;    /* Flags*/
#ifdef SUN
    unsigned char    ttl;            /* Time to live*/
    unsigned char   proto;            /* Protocol (TCP, UDP, etc.)*/
#else
    unsigned short    ttl;            /* Time to live*/
    unsigned short   proto;            /* Protocol (TCP, UDP, etc.)*/
#endif    
    unsigned short    checksum;        /* IP checksum*/
    unsigned int        sourceIP;
    unsigned int        destIP;
}IpHeader;

#define ICMP_ECHO        8
#define ICMP_ECHOREPLY    0
#define ICMP_MIN        8    /* Minimun 8-byte ICMP packet (header) */


/* ICMP header structure */

typedef struct _icmphdr
{
    char      i_type;
    char      i_code;        /* Type sub code */
    USHORT    i_cksum;
    USHORT    i_id;
    USHORT    i_seq;
    ULONG timestamp;
}IcmpHeader;


#define DEF_PACKET_SIZE 32        /* Default packet size */
#define MAX_PACKET        1024    /* Max ICMP packet size */
#define MAX_IP_HDR_SIZE    60        /* Max IP header size w/options */

int	datasize;

void FillICMPData(char* icmp_data, int datasize);
USHORT checksum(USHORT* buffer, int size);
int DecodeICMPHeader(char* buf, int bytes, struct sockaddr_in *from);
int Ping(char* lpdest);

int main(int argc, char* argv[])
{
	char IpAddr[20];
	strcpy(IpAddr,argv[1]);
	if(Ping(IpAddr) == TRUE)
	{
		printf("%s is online\n",IpAddr);
	}
	else
	{
		printf("%s is offline\n",IpAddr);
	}	
    return 0;
}

int Ping(char* lpdest)
{
#ifdef MSWIN32 
    WSADATA    wsaData;
#endif   
    int i; 
    SOCKET    sockRaw = INVALID_SOCKET;
    struct sockaddr_in dest, from;
    int        bread, fromlen = sizeof(from), timeout = 1000, ret;
    char    *icmp_data = NULL,icmp_tmp[MAX_PACKET], recvbuf[MAX_PACKET],recv_tmp[MAX_PACKET];
    unsigned int addr = 0;
    USHORT    seq_no = 0;
        int nCount = 0;
        int bwrote;    
    struct hostent *hp = NULL;
	icmp_data =icmp_tmp;
#ifdef MSWIN32
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup() failed: %d\n", GetLastError());
        return FALSE;
    }
	sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if(sockRaw == INVALID_SOCKET)
    {
        printf("WSASocket() failed: %d\n", WSAGetLastError());
        return FALSE;
    }
#else
	if ((sockRaw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == INVALID_SOCKET)
	{
		printf(" socket() error\n");
		return FALSE;
	}	
#endif

	setuid(getuid());
	timeout = SOCK_TIMEOUT_S;

    memset(&dest, 0, sizeof(dest));

    dest.sin_family = AF_INET;
    
	if((dest.sin_addr.s_addr = inet_addr(lpdest)) == INADDR_NONE)
    {
            printf("inet_addr error ip=%s \n",lpdest);
            return FALSE;
    }

    datasize += sizeof(IcmpHeader);

    memset(icmp_data, 0, MAX_PACKET);
	
    FillICMPData(icmp_data, datasize);

    while(nCount<1)
    {
	nCount++;
        ((IcmpHeader*)icmp_data)->i_cksum = 0;
#ifdef MSWIN32
        ((IcmpHeader*)icmp_data)->timestamp = GetTickCount();
#else
		((IcmpHeader*)icmp_data)->timestamp = 0;
#endif
        ((IcmpHeader*)icmp_data)->i_seq = seq_no++;
        ((IcmpHeader*)icmp_data)->i_cksum = checksum((USHORT*)icmp_data, datasize);
	bwrote = SOCKET_ERROR;
	if(xnet_select(sockRaw, SOCK_TIMEOUT_S, SOCK_TIMEOUT_MS, WRITE_STATUS)>0)
	{
        	bwrote = sendto(sockRaw, icmp_data, datasize, 0, (struct sockaddr*)&dest, sizeof(dest));
	}
        if(bwrote == SOCKET_ERROR)
        {
            printf("sendto() failed: %d\n");
            return FALSE;
        }
        if(bwrote < datasize)
        {
            printf("Wrote %d bytes\n", bwrote);
            return FALSE;
        }
	bread = SOCKET_ERROR;
	if(xnet_select(sockRaw, SOCK_TIMEOUT_S, SOCK_TIMEOUT_MS, READ_STATUS)>0)
	{
        	bread = recvfrom(sockRaw, recvbuf, MAX_PACKET, 0, (struct sockaddr*)&from, &fromlen);
        }	
        if(bread == SOCKET_ERROR)
        {
            printf("recvfrom() failed:bread=%d\n",bread);
			return FALSE;
        }
		
        if(DecodeICMPHeader(recvbuf, bread, &from) != TRUE )
        {
        	printf("DecodeICMPHeader() error,recv bytes=%d\n",bread);
        }	
    }
    

    if(sockRaw != INVALID_SOCKET)
        closesocket(sockRaw);
#ifdef MSWIN32
	WSACleanup();
#endif	
	return TRUE;
}

void FillICMPData(char* icmp_data, int datasize)
{
    IcmpHeader*    icmp_hdr = NULL;
    char*        datapart = NULL;
    
    icmp_hdr = (IcmpHeader*)icmp_data;
    icmp_hdr->i_type = ICMP_ECHO;        
    icmp_hdr->i_code = 0;
#ifdef MSWIN32
    icmp_hdr->i_id = (USHORT)GetCurrentProcessId();
#else
    icmp_hdr->i_id = (USHORT)getpid();	
#endif    
    icmp_hdr->i_cksum = 0;
    icmp_hdr->i_seq = 0;
    
    datapart = icmp_data + sizeof(IcmpHeader);
    memset(datapart, 'E', datasize - sizeof(IcmpHeader));
}

USHORT checksum(USHORT* buffer, int size)
{
    unsigned long cksum = 0 ;
    
    while(size > 1)
    {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if(size)
    {
        cksum += *(UCHAR*)buffer;
    }
    cksum = (cksum>>16)+(cksum&0xffff);
    cksum += (cksum>>16);
    return (USHORT)(~cksum);
}

int DecodeICMPHeader(char* buf, int bytes, struct sockaddr_in *from)
{
    IpHeader*    iphdr = NULL;
    IcmpHeader*    icmphdr = NULL;
    unsigned short iphdrlen;
    unsigned int        tick;
    static int    icmpcount = 0;
    
    iphdr = (IpHeader*)buf;
    iphdrlen = iphdr->h_len * 4;
#ifdef MSWIN32
    tick = GetTickCount();
#else
    tick = 0;	
#endif    
    
    if(bytes < iphdrlen + ICMP_MIN)
    {
        printf("Too Few bytes from %s\n",inet_ntoa(from->sin_addr));
        return FALSE;
    }
    icmphdr = (IcmpHeader*)(buf + iphdrlen);
    
    if(icmphdr->i_type != ICMP_ECHOREPLY)
    {
        printf("nonecho type %d recvd\n", icmphdr->i_type);
        return FALSE;
    }
#ifdef MSWIN32
    if(icmphdr->i_id != (USHORT)GetCurrentProcessId())
#else
    if(icmphdr->i_id != (USHORT)getpid())
#endif    
    {
        printf("someone else's packet!\n");
        return FALSE;
    }
    printf("%d bytes from %s:", bytes, inet_ntoa(from->sin_addr));
    printf(" icmp->seq = %d.", icmphdr->i_seq);
    printf(" time: %d ms", tick-icmphdr->timestamp);
    printf("\n");
    
    icmpcount++;
    return TRUE;
}


/*
s    - SOCKET
sec  - timeout seconds
usec - timeout microseconds
x    - select status
*/
SOCKET xnet_select(SOCKET s, int sec, int usec, short x)
{
 int st = errno;
 struct timeval to;
 fd_set fs;
 to.tv_sec = sec;
 to.tv_usec = usec;
 FD_ZERO(&fs);
 FD_SET(s, &fs);
 switch(x)
 {
  case READ_STATUS:
  st = select(s+1, &fs, 0, 0, &to);
  break;
  case WRITE_STATUS:
  st = select(s+1, 0, &fs, 0, &to);
  break;
  case EXCPT_STATUS:
  st = select(s+1, 0, 0, &fs, &to);
  break;
 }
 return(st);
}

