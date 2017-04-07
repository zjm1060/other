/*******************************************************************************************************
FILENAME:	plcping.c
FUNCTION:	

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
 2005.11.1	start to modify the code for H9000 V4.0		yyp
********************************************************************************************************/
#include "../../../lan/v4.0a/inc/lan.h"
#include "../../../cmd/v4.0a/inc/cmd_functions.h"
#include "../inc/plc_com.h"

#define WIN32_LEAN_AND_MEAN

/* IP header structure */

typedef struct _iphdr
{
#ifdef _BIG_ENDIAN_
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


extern SOCKET    sockRaw;
#define DEF_PACKET_SIZE 32        /* Default packet size */
#define MAX_PACKET        1024    /* Max ICMP packet size */
#define MAX_IP_HDR_SIZE    60        /* Max IP header size w/options */
#define TIMELEN			5
IpDef ip[MAXIPNUM];
int ipnum;
int timeout=NO;
void GetIpStatusPtName(char *addr,unsigned char stnid,unsigned char devid,int cpuid,int netid);
void FillICMPData(char* icmp_data, int datasize);
USHORT checksum(USHORT* buffer, int size);
int DecodeICMPHeader(char* buf, int bytes, struct sockaddr_in *from);
int Ping(char* lpdest);
extern void * ThreadSend( void * arg );
extern void * ThreadRecv( void * arg );
void MyTimer(int ss);
int GetIdFromAddr(char *addr);
void FitIpWithHotCpu(int id);
void IpAddOne(char *str);


/********************************************************************************************************
* send ping package  
*********************************************************************************************************/
void * ThreadSend( void * arg )
{
	SOCKET	sockplc=INVALID_SOCKET;			
	int		i;
	short		loop_count=0;			/* for loop count */
	short		ret;				/* return value of  */
	int		errcount=0;
	char	ipstr[20];
	int		curcpu;
	char	log_str[256];

	timeout=NO;
	for(;;)
	{
		if(IsPlcmonHost == FALSE)
		{
			MySleep(100);
			continue;
		}
		for(i=0;i<ipnum;i++)
		{
			//added by yyp 2011.08.01
			strcpy(ipstr,ip[i].ip);
			if( plconf[index_stn[ip[i].stnid]][ip[i].devid].plctype == MBPLC && g_ipaddone==1 && 
				plconf[index_stn[ip[i].stnid]][ip[i].devid].cpu_net_mode != CPU_D_NET_D)
			{
				curcpu=rdb_cpu(ip[i].stnid,ip[i].devid);				
				if((curcpu == CPUA && ip[i].pos==1) || (curcpu == CPUB && ip[i].pos==0) )
				{
					IpAddOne(ipstr);
				}
			}
			if(( ret=Ping(ipstr)) == TRUE )
			{
				sprintf(log_str,"plcping.c: plc addr= %s send ok\n\n",ipstr);
				log_write(H9000_LOG_INFO,3,log_str);
			}
			else
			{
				sprintf(log_str,"plcping.c: plc addr= %s send failed\n\n",ipstr);
				log_write(H9000_LOG_INFO,2,log_str);
			}
			MySleep(100);
#ifndef	PLCMON_MULTITHREAD 
			ThreadRecv((void *)NULL);
#endif
		}

		MySleep(3000);
	}/** end for(;;) **/

	if(sockRaw != INVALID_SOCKET)
        closesocket(sockRaw);
#ifdef MSWIN32
	WSACleanup();
#endif		
	return NULL;
}

/********************************************************************************************************
* send ping package  
*********************************************************************************************************/
void *ThreadRecv( void * arg )
{
    int id;
	int i;
	int loopcount;
    int errcount;
    struct sockaddr_in from;
    int        bread, fromlen = sizeof(from);
	char ipstr[20];
    char    *icmp_data = NULL, recvbuf[MAX_PACKET];
    unsigned int addr = 0;
    USHORT    seq_no = 0;
	int nCount = 0;
    struct hostent *hp = NULL;
	int max_time;
	time_t now;
	char	log_str[256];

	errcount=0;
	loopcount=0;
	for(;;)
	{

		if(IsPlcmonHost == FALSE)
		{
			MySleep(100);
			continue;
		}
		
		loopcount++;
#ifndef PLCMON_MULTITHREAD		
		if(loopcount>=2) break;
#endif
		MySleep(80);
		time(&now);
		for(i=0;i<ipnum;i++)
		{
			//remove by yyp 2012.01.10
			//if(ip[i].status != NET_ERR)
			//{
				if(ipnum < 20) 
				{
					max_time = TIMELEN+5;
				}
				else if(ipnum >= 20  && ipnum < 40)
				{
					max_time = 2 * TIMELEN + 5;
				}
				else if(ipnum >= 40  && ipnum < 80)
				{
					max_time = 6 * TIMELEN;
				}				
				else
				{
					max_time = 12 * TIMELEN;
				}

				if(abs(now - ip[i].lasttime) > max_time)
				{

					sprintf(log_str,"plcping.c: %s is offline\n",ip[i].ip);	
					log_write(H9000_LOG_INFO,2,log_str);					
					ip[i].status = NET_ERR;
					if( plconf[ index_stn[ip[i].stnid] ][ ip[i].devid ].plctype == MBPLC )
					{
						FitIpWithHotCpu(i);						
					}
					BdINDByName(ip[i].logicalname,ip[i].status);
					ip[i].lasttime = now;
					MySleep(100);
				}
			//}
		}
		bread = SOCKET_ERROR;
		if(xnet_select(sockRaw, ICMP_TIMEOUT_S, ICMP_TIMEOUT_MS, READ_STATUS)>0)
		{
			bread = recvfrom(sockRaw, recvbuf, MAX_PACKET, 0, (struct sockaddr*)&from, &fromlen);
        }
        if(bread == SOCKET_ERROR)
       	{
            sprintf(log_str,"plcping.c: recvfrom() failed, timeout: bread=%d\n", bread);
			log_write(H9000_LOG_ERROR,2,log_str);
			continue;
        }
        if(DecodeICMPHeader(recvbuf, bread,&from) != TRUE )
        {
        	sprintf(log_str,"plcping.c: decode() error,recv bytes=%d\n",bread);
			log_write(H9000_LOG_ERROR,3,log_str);
			continue;
       	}
       	else
       	{
			id = GetIdFromAddr(inet_ntoa(from.sin_addr));
			if(id == -1) continue;
        	sprintf(log_str,"plcping.c: %s is online\n",inet_ntoa(from.sin_addr));
			log_write(H9000_LOG_INFO,2,log_str);
			ip[id].status = NET_OK;
			if( plconf[ index_stn[ip[id].stnid] ][ ip[id].devid ].plctype == MBPLC )
			{
				FitIpWithHotCpu(id);						
			}
			BdINDByName(ip[id].logicalname,ip[id].status);
			ip[id].lasttime = now;
       	}
		
	}/* end for */
	return NULL;
}
int Ping(char* lpdest)
{
    int	datasize; 
    struct sockaddr_in dest;
    char    *icmp_data = NULL,icmp_tmp[MAX_PACKET];
    unsigned int addr = 0;
    USHORT    seq_no = 0;
	int nCount = 0;
	int bwrote;    
    struct hostent *hp = NULL;
	icmp_data =icmp_tmp;

    memset(&dest, 0, sizeof(dest));

    dest.sin_family = AF_INET;
    
	if((dest.sin_addr.s_addr = inet_addr(lpdest)) == INADDR_NONE)
    {
		printf("inet_addr error ip=%s \n",lpdest);
		return FALSE;
    }
    datasize = sizeof(IcmpHeader);
    datasize += 10;
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
		if(xnet_select(sockRaw, ICMP_TIMEOUT_S, ICMP_TIMEOUT_MS, WRITE_STATUS)>0)
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
 
    }/* end while */
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
    char	log_str[256];

    iphdr = (IpHeader*)buf;
    iphdrlen = iphdr->h_len * 4;
#ifdef MSWIN32
    tick = GetTickCount();
#else
    tick = 0;	
#endif    
    
    if(bytes < iphdrlen + ICMP_MIN)
    {
        printf("plcping.c:Too Few bytes from %s\n",inet_ntoa(from->sin_addr));
        return FALSE;
    }
    icmphdr = (IcmpHeader*)(buf + iphdrlen);
    
    if(icmphdr->i_type != ICMP_ECHOREPLY)
    {
        printf("plcping.c: echo type %d error\n", icmphdr->i_type);
        return FALSE;
    }
#ifdef MSWIN32
    if(icmphdr->i_id != (USHORT)GetCurrentProcessId())
#else
    if(icmphdr->i_id != (USHORT)getpid())
#endif    
    {
        sprintf(log_str,"someone else's packet!\n");
        log_write(H9000_LOG_ERROR,2,log_str);
        return FALSE;
    }
    sprintf(log_str,"plcping.c: %d bytes from %s:", bytes, inet_ntoa(from->sin_addr));
    log_write(H9000_LOG_INFO,3,log_str);
    sprintf(log_str," icmp->seq = %d.", icmphdr->i_seq);
    log_write(H9000_LOG_INFO,3,log_str);
    sprintf(log_str," time: %d ms\n", tick-icmphdr->timestamp);
    log_write(H9000_LOG_INFO,3,log_str);
	if( (tick-icmphdr->timestamp) > 2000)
	{
		sprintf(log_str,"err: pkg timeout ", tick-icmphdr->timestamp);
		log_write(H9000_LOG_ERROR,2,log_str);
	}


    icmpcount++;
    return TRUE;
}

void MyTimer(int ss)
{
	timeout=YES;
}

void FitIpWithHotCpu(int id)
{
	unsigned char stnid,idx,devid;
	int NetID=0;
	int CpuID=0;
	int	curcpu;
	int	ip_cpu[4],ip_net[4];
	int	i;
	stnid=ip[id].stnid;
	devid=ip[id].devid;
	idx = index_stn[stnid];
	i = ip[id].pos; 
	if(plconf[idx][devid].plctype == MBPLC && plconf[idx][devid].isdualcpu == YES
		&& g_ctrl_dualnet == YES )
	{
		curcpu=rdb_cpu(stnid,devid);
		if(curcpu == CPUB)
		{		
			ip_cpu[0] =CPUB;
			ip_net[0] =NETA;
			ip_cpu[1] =CPUB;
			ip_net[1] =NETB;
			ip_cpu[2] =CPUA;
			ip_net[2] =NETA;
			ip_cpu[3] =CPUA;
			ip_net[3] =NETB;
		}
		else
		{
			ip_cpu[0] =CPUA;
			ip_net[0] =NETA;
			ip_cpu[1] =CPUA;
			ip_net[1] =NETB;
			ip_cpu[2] =CPUB;
			ip_net[2] =NETA;
			ip_cpu[3] =CPUB;
			ip_net[3] =NETB;			
		}
		GetIpStatusPtName(ip[id].logicalname,stnid,devid,ip_cpu[i],ip_net[i]);
	}
}

void InitIpDef()
{
	unsigned char idx, devid;
	char devname[20],typestr[20];
	int NetID=0;
	int CpuID=0;
	int		net_num,cpu_num;
	int		curcpu;
	int		ip_cpu[4],ip_net[4];
	int		i;
	time_t now;
	char log_str[256];

	time(&now);
	ipnum = 0;
	for(idx=1;idx<=g_stn_num;idx++)
	{
		UCHAR stnid;
		GetStnIdByIndex(idx, &stnid);
		for(devid=1;devid<=g_dev_num[idx];devid++)
		{
#ifndef PLCMON_MULTIDEV
			if(stnid>1 || devid>1)
			{
				return;
			}
#endif			
			MySleep(50);
		  	if(GetGroupNameById(stnid,devid,devname)==-1) continue;
			if(plconf[idx][devid].valid!=1)	continue;
			if(strcmp(devname,"")==0) 	continue;
			
			GetPlcTypeStr(stnid, devid, typestr);
			if(strcmp(typestr,"SYS") == 0 || strcmp(typestr,"RTP") == 0)
			{
				continue;
			}

			if(plconf[idx][devid].plctype == MBPLC  
				|| plconf[idx][devid].plctype == PREPLC
				|| plconf[idx][devid].plctype == SIEPLC
				|| plconf[idx][devid].plctype == GEPLC	
				|| plconf[idx][devid].plctype == ABPLC	)			
			{
				for(i=0; i < plconf[idx][devid].ipnum ; i++)
				{
					/* added by yyp 2008.11.9 */
					if( ( plconf[idx][devid].cpu_net_mode != CPU_D_NET_D ) && i>=2 )
					{
						continue;
					}
					strcpy(ip[ipnum].ip,plconf[idx][devid].ipaddr[i]);
					if(g_ctrl_dualnet == YES)
					{
						net_num = 2;
					}
					else
					{
						net_num = 1;
					}
					if(plconf[idx][devid].isdualcpu == YES)
					{
						cpu_num = 2;
					}
					else
					{
						cpu_num = 1;
					}
					ip_cpu[0] =CPUA;
					ip_net[0] =NETA;
					if(plconf[idx][devid].isdualcpu == YES && g_ctrl_dualnet == YES)
					{
						if(plconf[idx][devid].cpu_net_mode == CPU_D_NET_D)
						{
							ip_cpu[1] =CPUA;
							ip_net[1] =NETB;
							ip_cpu[2] =CPUB;
							ip_net[2] =NETA;
							ip_cpu[3] =CPUB;
							ip_net[3] =NETB;
						}
						else
						{
							ip_cpu[1] =CPUB;
							ip_net[1] =NETB;
						}
					}
					else if(plconf[idx][devid].isdualcpu == YES && g_ctrl_dualnet == NO)
					{
						ip_cpu[1] =CPUB;
						ip_net[1] =NETA;
					}
					else if(plconf[idx][devid].isdualcpu == NO && g_ctrl_dualnet == YES)
					{
						ip_cpu[1] =CPUA;
						ip_net[1] =NETB;
					}
					if(plconf[idx][devid].plctype == MBPLC && plconf[idx][devid].isdualcpu == YES
						 && g_ctrl_dualnet == YES )
					{
						curcpu=rdb_cpu(stnid,devid);
						if(curcpu == CPUB)
						{		
							ip_cpu[0] =CPUB;
							ip_net[0] =NETA;
							ip_cpu[1] =CPUB;
							ip_net[1] =NETB;
							ip_cpu[2] =CPUA;
							ip_net[2] =NETA;
							ip_cpu[3] =CPUA;
							ip_net[3] =NETB;
						}
						else
						{
							ip_cpu[0] =CPUA;
							ip_net[0] =NETA;
							ip_cpu[1] =CPUA;
							ip_net[1] =NETB;
							ip_cpu[2] =CPUB;
							ip_net[2] =NETA;
							ip_cpu[3] =CPUB;
							ip_net[3] =NETB;			
						}
					}
					GetIpStatusPtName(ip[ipnum].logicalname,stnid,devid,ip_cpu[i],ip_net[i]);
					ip[ipnum].stnid = stnid;
					ip[ipnum].devid = devid;
					ip[ipnum].pos = i;
					ip[ipnum].status = -1;
					ip[ipnum].lasttime  = now;
					ipnum++;
				}/* end for */
				
			}/* end if */
		}
	}
	sprintf(log_str,"plcmon: ipnum=%d \n ", ipnum);
	log_write(H9000_LOG_INFO,1,log_str);
}


void GetIpStatusPtName(char *logicalname,unsigned char stnid,unsigned char devid,int cpuid,int netid)
{
	char		tmp[100],buf[100];
	unsigned char idx;
	idx = index_stn[stnid];
	if(GetStnNameById(stnid,tmp)==-1) return;
	strcpy(buf,tmp);
	strcat(buf,".");
	GetGroupNameById(stnid,devid,tmp);
	strcat(buf,tmp);
	strcat(buf,".SYS.");
	if(plconf[idx][devid].cpu_net_mode != CPU_D_NET_D)
	{
		
		if(netid!=NETB)
			
			strcat(buf,"CPU_NETA");
		else
			
			strcat(buf,"CPU_NETB");		
		
	}
	else
	{
		if(cpuid != CPUB)
		{
			if(netid != NETB)
				strcat(buf,"CPUA_NETA");
			else
				strcat(buf,"CPUA_NETB");
		}
		else
		{
			if(netid != NETB)
				strcat(buf,"CPUB_NETA");
			else
				strcat(buf,"CPUB_NETB");		
		}
	}
	strcpy(logicalname,buf);
	return;	
}


int GetIdFromAddr(char *addr)
{
	int i;
	char	ipstr[20];
	int		curcpu;
	for(i=0;i<ipnum;i++)
	{
		//added by yyp 2011.08.01
		strcpy(ipstr,ip[i].ip);
		if( plconf[ index_stn[ip[i].stnid] ][ip[i].devid].plctype == MBPLC && g_ipaddone==1 && 
			plconf[ index_stn[ip[i].stnid] ][ip[i].devid].cpu_net_mode != CPU_D_NET_D)
		{
			curcpu=rdb_cpu(ip[i].stnid,ip[i].devid);				
			if((curcpu == CPUA && ip[i].pos==1) || (curcpu == CPUB && ip[i].pos==0) )
			{
				IpAddOne(ipstr);
			}
		}
		if( strcmp(ipstr,addr)==0 )
		{
			return i;
		}
	}
	return -1;
}

void IpAddOne(char *str)
{
	unsigned int ipd1,ipd2,ipd3,ipd4;
	sscanf(str,"%d.%d.%d.%d", &ipd1,&ipd2,&ipd3,&ipd4);
	ipd4 = ipd4 + 1;
	sprintf(str,"%d.%d.%d.%d", ipd1,ipd2,ipd3,ipd4);
}
