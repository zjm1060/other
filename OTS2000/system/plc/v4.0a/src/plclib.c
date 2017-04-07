/*******************************************************************************************************
FILENAME:	plclib.c
FUNCTION:	
	
	  the source code file of plc communication module,whether the plc type
	is "GE" or "QUANTUM".include:

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
 2008.6.27  upgraded for AB								yyp
 2005.11.1	start to modify the code for H9000 V4.0		yyp
********************************************************************************************************/

#include "../inc/plchead_all.h"
#ifdef SUN_OS
#include <netinet/tcp.h>
#endif

static int GetAddrByName(char *hostname1 ,unsigned long *ipaddr) 
{
	struct hostent  *hent;

	if(hostname1==NULL)
		return FALSE;
	if(strlen(hostname1)==0)
		return FALSE;
	if((*ipaddr = inet_addr(hostname1))== -1)

	{
	  if( (hent = gethostbyname(hostname1)) == NULL)
	    return FALSE;   
	  *ipaddr =*((unsigned long *)hent->h_addr); 
	}
	return TRUE;
}


/********************************************************************************************************
 distribute RecvPLC funtion between ge and quantum  by plctype
*********************************************************************************************************/

int RecvPLC(MYSOCKET sockplc, char *buffer, int count)
{
	int ret;
	ret=RecvPLC_MB(sockplc, buffer, count,0);
	return ret;
}

/********************************************************************************************************
 distribute WritePLC funtion between ge and quantum by plctype
*********************************************************************************************************/

int WritePLC(MYSOCKET sockplc, unsigned char seg, int ref, int size, unsigned char *buffer,LCUID plcid)
{
	int ret;
	unsigned char stnid,idx,devid;
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx=index_stn[stnid];
	if(size  == 0)
	{
		return TRUE;
	}	
	if(plconf[idx][devid].plctype==MBPLC || plconf[idx][devid].plctype==PREPLC )
	{
		if(plconf[idx][devid].plctype==MBPLC)
		{	
			ref = ref -1;
		}		
		ret=WritePLC_MB(sockplc, seg, ref, size, buffer);
	}
	else if(plconf[idx][devid].plctype==GEPLC)
	{
		ret=WritePLC_GE(sockplc, seg, ref, size, buffer);
	}
	else if(plconf[idx][devid].plctype==SIEPLC)
	{
		ret=WritePLC_SIE(sockplc, seg, ref, size, buffer);
	}
	else if(plconf[idx][devid].plctype==ABPLC)
	{
		ret=WritePLC_AB(sockplc, seg, ref, size, buffer);
	}
	return ret;	
}

/********************************************************************************************************
 distribute ReadPLC funtion between ge and quantum by plctype
*********************************************************************************************************/

int ReadPLC(MYSOCKET sockplc, unsigned char seg, int ref, int size, unsigned char *buffer,LCUID plcid)
{
	int ret;
	unsigned char stnid,idx,devid;
	stnid=plcid.stnid;
	devid=plcid.devid;	
	idx=index_stn[stnid];
	if(size  == 0)
	{
		return TRUE;
	}
	if(plconf[idx][devid].plctype==MBPLC || plconf[idx][devid].plctype==PREPLC)
	{
		if(plconf[idx][devid].plctype==MBPLC)
		{	
			ref = ref -1;
		}	
		ret=ReadPLC_MB(sockplc, seg, ref, size, buffer);
	}
	else if(plconf[idx][devid].plctype==GEPLC)
	{
		ret=ReadPLC_GE(sockplc, seg, ref, size, buffer);
	}
	else if(plconf[idx][devid].plctype==SIEPLC)
	{
		ret=ReadPLC_SIE(sockplc, seg, ref, size, buffer);
	}
	else if(plconf[idx][devid].plctype==ABPLC)
	{
		ret=ReadPLC_AB(sockplc, seg, ref, size, buffer);
	}
	return ret;
}

/********************************************************************************************************
 Connect to a given plc for head thread  using TCP/IP
 return socketid when successfully,INVALID_SOCKET otherwise.
*********************************************************************************************************/
MYSOCKET ConnectPLC(LCUID plcid, unsigned char threadtype)
{
	MYSOCKET sockplc ; 
	int bindret = TRUE;
	int error = -1, len = sizeof(int);
	int NetID=0;
	int CpuID=0;
	int IpID=0;
	int net_num,cpu_num;
	int result;
	char address[40];
	unsigned char stnid,idx,devid;
	unsigned short port;
	char log_str[256];
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx=index_stn[stnid];
	sockplc.sock = INVALID_SOCKET;
	GetPlcPort(plcid,threadtype,&port,&cpu_num, &net_num);

	if(threadtype != THR_HEAD && threadtype != THR_CMD)
	{
		sockplc.sock = INVALID_SOCKET;
		return sockplc;
	}

	runvar[idx][devid].hotnet=rdb_net(stnid,devid);
	runvar[idx][devid].hotcpu=rdb_cpu(stnid,devid);

	CpuID =0;
	result= SOCKET_ERROR;
	sprintf(log_str,"==== ConnectPLC(): port =%d ,stnid=%d ,devid=%d...\n",port,stnid,devid);
	log_printf(H9000_LOG_INFO,1,0,log_str);
	while(CpuID<cpu_num)
	{
		NetID = 0;
		while( NetID<net_num )
		{
			if(plconf[idx][devid].cpu_net_mode == CPU_S_NET_S)
			{
				runvar[idx][devid].hotnet = 0;
				runvar[idx][devid].hotcpu = 0;
			}
			else if(plconf[idx][devid].cpu_net_mode == CPU_S_NET_D)
			{
				runvar[idx][devid].hotcpu = 0;
			}
			else if(plconf[idx][devid].cpu_net_mode == CPU_D_NET_S)
			{
				if(runvar[idx][devid].hotnet != runvar[idx][devid].hotcpu)
				{
					runvar[idx][devid].hotnet = runvar[idx][devid].hotcpu;
				}
			}
			if(plconf[idx][devid].cpu_net_mode == CPU_D_NET_D && g_ipwithcpu != TRUE )
			{
				IpID = runvar[idx][devid].hotnet+runvar[idx][devid].hotcpu * 2;
			}
			else
			{
				IpID = runvar[idx][devid].hotnet;	
			}

			if(IpID >= plconf[idx][devid].ipnum)
			{
				sprintf(log_str,"plc%d.%d ConnectPLC(): ipid=%d >= ipnum=%d\n",stnid,devid,
					IpID,plconf[idx][devid].ipnum);
				log_printf(H9000_LOG_ERROR,1,0,log_str);
				break;
			}

			strcpy(address,plconf[idx][devid].ipaddr[IpID]);

			sockplc = CreateSock(plcid, address, port);
			result = sockplc.sock;

			if(result != SOCKET_ERROR)
			{
				break;
			}
			else
			{
#ifdef MSWIN32
				sprintf(log_str,"connect %s failure  errcode=%d stnid= %d devid= %d \n",\
					address,WSAGetLastError(),stnid,devid); 
#else
				sprintf(log_str,"connect %s failure  stnid=%d devid= %d  netid=%d cpuid=%d \n",\
					address,stnid,devid,runvar[idx][devid].hotnet, \
					runvar[idx][devid].hotcpu);
#endif
				log_printf(H9000_LOG_ERROR,1,0,log_str);

				DisconnectPLC(&sockplc,plcid);

				if(threadtype == THR_HEAD)
				{
					BdCommStat(plcid,NET_ERR);
				}

				if(g_ctrl_dualnet == YES)
				{
					if (runvar[idx][devid].hotnet==0)
					{
						runvar[idx][devid].hotnet=1;
					}
					else
					{
						runvar[idx][devid].hotnet=0;
					}
					if(plconf[idx][devid].cpu_net_mode == CPU_D_NET_S  && plconf[idx][devid].plctype != ABPLC )
					{
						runvar[idx][devid].hotcpu = runvar[idx][devid].hotnet;
					}
				}
				else
				{
					break;
				}					
			}
			NetID++;
			if(NetID>1)
			{
				break;
			}
		} /* end while NetID **/
				
		if(result != SOCKET_ERROR) break;
		
		if(plconf[idx][devid].cpu_net_mode == CPU_D_NET_D && g_ipwithcpu != TRUE)
		{
			
		}
		else
		{
			break;
		}
		if(plconf[idx][devid].isdualcpu==YES)
		{
			if( threadtype != THR_HEAD )
			{
				sockplc.sock = INVALID_SOCKET;
				return (sockplc);
			}			
			if( g_ipwithcpu != TRUE )
			{
				if(runvar[idx][devid].hotcpu==0)	
				{
					runvar[idx][devid].hotcpu=1;
				}
				else
				{
					runvar[idx][devid].hotcpu=0;
				}
			}
			else
			{
				break;
			}	
		}
		else
		{
			break;
		}
		CpuID++;
		if(CpuID>1)
		{
			break;
		}
	} /* end while CpuID */

	if(result == SOCKET_ERROR)
	{
		sockplc.sock = INVALID_SOCKET;
		return (sockplc);
	}
	else
	{
		if(threadtype == THR_HEAD)
		{		
			BdNetHost(plcid,runvar[idx][devid].hotnet );
			BdCpuHost(plcid,runvar[idx][devid].hotcpu );
			BdCommStat(plcid,NET_OK);
		}
		sprintf(log_str,"==== Connect %s sockplc=%d stnid=%d devid=%d netid=%d cpuid=%d  OK! \n",
			address,sockplc.sock,stnid,devid,runvar[idx][devid].hotnet,runvar[idx][devid].hotcpu); 
		log_printf(H9000_LOG_INFO,1,0,log_str);
		g_sock =  sockplc.sock;
		sockplc.readslave = FALSE;
		return (sockplc);
	}
}

/********************************************************************************************************
 Connect to a given plc for child thread  using TCP/IP
 return socketid when successfully,INVALID_SOCKET otherwise.
*********************************************************************************************************/
MYSOCKET ConnectPLCChild(LCUID plcid, unsigned char threadtype)
{
	MYSOCKET sockplc ; 
	int bindret = TRUE;
	int error = -1, len = sizeof(int);
	int NetID=0;
	int CpuID=0;
	int IpID=0;
	int net_num,cpu_num;
	int result;
	char address[40];
	char	log_str[256];
	unsigned char stnid,idx,devid;
	unsigned short port;
	if(threadtype == THR_HEAD || threadtype == THR_CMD)
	{
		sockplc.sock = INVALID_SOCKET;
		return sockplc;
	}
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx=index_stn[stnid];
	sockplc.sock = INVALID_SOCKET;
	GetPlcPort(plcid,threadtype,&port,&cpu_num, &net_num);

	CpuID =0;
	result= SOCKET_ERROR;
	sprintf(log_str,"==== ConnectPLC Child(): port =%d ,stnid=%d ,devid=%d...\n",port,stnid,devid);
	log_printf(H9000_LOG_INFO,1,0,log_str);
	if(plconf[idx][devid].cpu_net_mode == CPU_D_NET_D && g_ipwithcpu != TRUE )
	{
		IpID = runvar[idx][devid].hotnet+runvar[idx][devid].hotcpu * 2;
	}
	else
	{
		IpID = runvar[idx][devid].hotnet;	
	}

	if(IpID >= plconf[idx][devid].ipnum)
	{
		sprintf(log_str,"plc%d.%d ConnectPLC(): ipid=%d >= ipnum=%d\n",stnid,devid,
			IpID,plconf[idx][devid].ipnum);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		sockplc.sock = INVALID_SOCKET;
		return (sockplc);
	}

	strcpy(address,plconf[idx][devid].ipaddr[IpID]);

	sockplc = CreateSock(plcid, address, port);
	result = sockplc.sock;

	if(result == SOCKET_ERROR)
	{
#ifdef MSWIN32
		sprintf(log_str,"connect %s failure  errcode=%d stnid= %d devid= %d \n",\
			address,WSAGetLastError(),stnid,devid); 
#else
		sprintf(log_str,"connect %s failure  stnid=%d devid= %d  netid=%d cpuid=%d \n",\
			address,stnid,devid,runvar[idx][devid].hotnet, \
			runvar[idx][devid].hotcpu);
#endif
		log_printf(H9000_LOG_ERROR,1,0,log_str);

		DisconnectPLC(&sockplc,plcid);
		sockplc.sock = INVALID_SOCKET;
		return (sockplc);
	}			
	else
	{
		sprintf(log_str,"==== ConnectPLC Child %s sockplc=%d stnid=%d devid=%d netid=%d cpuid=%d  OK! \n",
			address,sockplc.sock,stnid,devid,runvar[idx][devid].hotnet,runvar[idx][devid].hotcpu); 
		log_printf(H9000_LOG_INFO,1,0,log_str);
		g_sock =  sockplc.sock;
		sockplc.readslave = FALSE;
		return (sockplc);
	}
}

/********************************************************************************************************
Test the Network condition of a plc   using TCP/IP
return socketid when successfully,INVALID_SOCKET otherwise.
applied only for AB plc now by 2009.2.2 yyp 
*********************************************************************************************************/
BOOL TestConnPLC(LCUID plcid, unsigned char cpuid,unsigned char netid, unsigned char threadtype)
{
	MYSOCKET sockplc ; 
	int error = -1, len = sizeof(int);
	int IpID=0;
	int result;
	char address[40];
	int net_num,cpu_num;
	unsigned char stnid,idx,devid;
	unsigned short port;
	char	log_str[256];
	stnid= plcid.stnid;
	devid = plcid.devid; 
	idx=index_stn[stnid];
	GetPlcPort(plcid,threadtype,&port,&cpu_num, &net_num);
	if(plconf[idx][devid].plctype != ABPLC)
	{
		sprintf(log_str,"plc%d.%d TestConnPLC() is only applied for ABPLC",stnid,devid);
		err_write(log_str);
		return FALSE;
	}
	if(plconf[idx][devid].cpu_net_mode == CPU_D_NET_D && plconf[idx][devid].plctype != MBPLC)
	{
		IpID = netid+cpuid * 2;
	}
	else
	{
		IpID = netid;	
	}

	if(IpID >= plconf[idx][devid].ipnum)
	{
		sprintf(log_str,"plc%d.%d TestConnPLC(): ipid=%d >= ipnum=%d\n",stnid,devid,
			IpID,plconf[idx][devid].ipnum);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;
	}

	strcpy(address,plconf[idx][devid].ipaddr[IpID]);

	sockplc = CreateSock(plcid, address, port);
	result = sockplc.sock;

	if(result != SOCKET_ERROR)
	{
		sprintf(log_str,"==== TestConnect %s sockplc=%d stnid=%d devid=%d netid=%d cpuid=%d  OK! \n",
			address,sockplc.sock,stnid,devid,netid,cpuid); 
		log_printf(H9000_LOG_INFO,1,0,log_str);	
		DisconnectPLC(&sockplc,plcid);	
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

int GetPlcPort(LCUID plcid, unsigned char threadtype,unsigned short * p_port,int * p_cpunum, int * p_netnum)
{
	unsigned char stnid,idx,devid;
	unsigned short startport;
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx=index_stn[stnid];
	startport = (unsigned short)sieport[g_localhost_id];
	if(plconf[idx][devid].plctype == SIEPLC )
	{
		switch(threadtype) 
		{
		case THR_CMD:
			*p_port=startport + 10 * (devid-1);
			break;
		case THR_HEAD:
			*p_port=startport + 1 + 10 * (devid-1);		/* modified by yyp 07.12.3 */
			break;
		case THR_ANA:
			*p_port=startport + 2 + 10 * (devid-1);
			break;
		case THR_INTALM:
			*p_port=startport + 3 + 10 * (devid-1);
			break;
		case THR_POL:
			*p_port=startport + 4 + 10 * (devid-1);
            break;
		case THR_SYSMSG:
			*p_port=startport + 5 + 10 * (devid-1);
			break;
		case THR_IMP:
			*p_port=startport + 6 + 10 * (devid-1);
			break;
		case THR_WRITEDATA:
			*p_port=startport + 7 + 10 * (devid-1);
			break;
		}
	}
	else
		*p_port=plconf[idx][devid].port;
	if(g_ctrl_dualnet == YES)
	{
		*p_netnum = 2;
	}
	else
	{
		*p_netnum = 1;
	}
	if(plconf[idx][devid].isdualcpu == YES)
	{
		if( plconf[idx][devid].plctype != MBPLC )
		{
			*p_cpunum = 2;
		}
		else
		{
			*p_cpunum =1 ;
		}
	}
	else
	{
		*p_cpunum = 1;
	}
	return TRUE;
}

MYSOCKET CreateSock(LCUID plcid, char *address, unsigned short port)
{
	MYSOCKET sockplc; 
	struct sockaddr_in pin;
	int bindret = TRUE;
	struct timeval tmval;
	fd_set set;
	int sel;
#ifndef MSWIN32
	int f ;
#endif
	int error = -1, len = sizeof(int);
	char	log_str[256];
 
	struct hostent *hp;
	unsigned int opt_val;
	int result,on,i;
	unsigned char stnid,idx,devid;
	sockplc.sock = INVALID_SOCKET;
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx=index_stn[stnid];
	pin.sin_family = AF_INET;
	pin.sin_port = htons(port);
	result= SOCKET_ERROR;
	port = 44818;
	hp=gethostbyname(address);
	if (hp)
	{
		pin.sin_addr.s_addr = ((struct in_addr*)(hp->h_addr))->s_addr;
	}
	else
 	{
		pin.sin_addr.s_addr = inet_addr(address);
	}
	if ((sockplc.sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		DisconnectPLC(&sockplc,plcid);
		sprintf(log_str,"ConnectPLC(): socket() error\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		sockplc.sock = SOCKET_ERROR;
		return (sockplc);
	}

	opt_val = (char) NODELAY;
	setsockopt(sockplc.sock, IPPROTO_TCP, TCP_NODELAY, (char *)&opt_val, sizeof(char));
	on=1;
	setsockopt(sockplc.sock,SOL_SOCKET,SO_KEEPALIVE,(char *)&on,sizeof (on));
#ifndef MSWIN32
	f = fcntl(sockplc.sock, F_GETFL);
	fcntl(sockplc.sock, F_SETFL, f | O_NONBLOCK);	
#else
	opt_val   =   1;   
	result = ioctlsocket(sockplc.sock,   FIONBIO,   &opt_val); 
#endif
	if(plconf[idx][devid].plctype == SIEPLC) 
	{
		bindret = BindLocal(plcid,sockplc,port);
	}

	if(bindret == TRUE)
	{
		for(i=0;i<MAXRETRY_CONN;i++)
		{
			result=connect(sockplc.sock, (struct sockaddr*)&(pin), sizeof(pin)); 
			if (result == SOCKET_ERROR)
			{
				tmval.tv_sec = 0;
				tmval.tv_usec = CONN_TIMEOUT_MS * 1000;
				FD_ZERO(&set);
				FD_SET(sockplc.sock, &set);
				if ((sel=select(sockplc.sock + 1, NULL, &set, NULL, &tmval)) > 0)
				{
						getsockopt(sockplc.sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
						if (error == 0)
						{
							result = TRUE;
							break;
						}
				}
				MySleep(1000);		/* modified by yyp for 200 to 1000 on 2008.12.4 */
				continue; 
			}
			else
			{
				break;
			}
		} 
	}
	if(result != SOCKET_ERROR)
	{
#ifndef MSWIN32
		fcntl(sockplc.sock,F_SETFL,O_NDELAY);
#endif		
		/* added by yyp 07.08.29 */
		if(plconf[idx][devid].plctype == GEPLC)
		{
			if(ConnSession(sockplc,plcid) != TRUE)
			{
				DisconnectPLC(&sockplc,plcid);
				result = SOCKET_ERROR;
			}
		}
		else if(plconf[idx][devid].plctype == ABPLC)
		{
			if(RegisterSession(plcid,&sockplc) != TRUE)
			{
				DisconnectPLC(&sockplc,plcid);
				result = SOCKET_ERROR;
			}
		}
	}
	if(result == SOCKET_ERROR)
	{
		DisconnectPLC(&sockplc,plcid);
		sockplc.sock = result;
	}
	return (sockplc);
}

int BindLocal(LCUID plcid,MYSOCKET sockplc,unsigned short port)
{
	struct sockaddr_in serveraddr;
	char address_host[40];
	unsigned char stnid,idx,devid;
	int bindcount=0;
	int bindret = TRUE;
	char	log_str[256];
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx=index_stn[stnid];
	if(plconf[idx][devid].plctype == SIEPLC) 
	{
		memset((char *) &serveraddr,0,sizeof (struct sockaddr_in));
		serveraddr.sin_family      = AF_INET;
		if(g_ctrl_net_independ == TRUE)
		{
			GetHostIpaddr(g_hostname,2+runvar[idx][devid].hotnet,address_host);
		}
		else
		{
			GetHostIpaddr(g_hostname,runvar[idx][devid].hotnet,address_host);
		}
		serveraddr.sin_addr.s_addr = inet_addr(address_host); 
		serveraddr.sin_port        = htons((unsigned short)port); 
		bindcount = 0 ;
		bindret = TRUE;
		while(bind(sockplc.sock,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) != 0) 
		{
			sprintf(log_str,"==== plc%d.%d bind error,port=%d, addr=%s,hotnet=%d try again...",stnid,devid,port,address_host,
				runvar[idx][devid].hotnet); 
			log_printf(H9000_LOG_INFO,1,0,log_str);	
			MySleep(2000);
			bindcount++;
			if(bindcount>30)
			{
				bindret = FALSE;
				break;				/* added by yyp 2008.12.23 */
			}
		}
		if(bindret == TRUE)
		{
			sprintf(log_str,"==== plc%d.%d bind sucess, port=%d,addr=%s,hotnet=%d...",stnid,devid,port,address_host,
				runvar[idx][devid].hotnet); 
			log_printf(H9000_LOG_INFO,1,0,log_str);
		}						
		
	}
	return bindret;
}


/********************************************************************************************************
 DisConnect from a given plc  using TCP/IP
*********************************************************************************************************/

int DisconnectPLC(MYSOCKET *p_sockplc,LCUID plcid)
{

/*
	if (p_sockplc->sock==INVALID_SOCKET)   return TRUE;
	sprintf(log_str,"plc %d.%d close socket %d ",plcid.stnid,plcid.devid,p_sockplc->sock);
	log_printf(H9000_LOG_INFO,2,0,log_str);	
	closesocket(p_sockplc->sock);
	p_sockplc->sock=INVALID_SOCKET;
	return TRUE;
*/
	struct linger linger;
	int   ret;
	char  addreuse=1;
 #ifdef MSWIN32
	unsigned int block_flag = FALSE;
#endif
	
	if(p_sockplc->sock <=0) return FALSE;
	linger.l_onoff = 1;
	linger.l_linger = 0;
	setsockopt(p_sockplc->sock, SOL_SOCKET, SO_LINGER, (char *)&linger,sizeof(linger));
    setsockopt(p_sockplc->sock,SOL_SOCKET,SO_REUSEADDR,&addreuse,sizeof(addreuse));
#ifdef MSWIN32
	ioctlsocket(p_sockplc->sock,FIONBIO,&block_flag);
#else
	fcntl(p_sockplc->sock,F_SETFL,0);
#endif
	ret= closesocket(p_sockplc->sock);
	p_sockplc->sock=-1;
	if(ret==0) return TRUE;
	return FALSE;
}




