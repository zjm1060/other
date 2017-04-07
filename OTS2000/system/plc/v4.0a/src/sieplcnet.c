/****************************************************************************************
FILENAME:	siemensplc.c
FUNCTION:	
	some sub functions of plc comm  for "SIEMENS400"	

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
 2007.04
****************************************************************************************/

#if defined(WIN32)
#define  EWOULDBLOCK    WSAEWOULDBLOCK
#define  EISCONN        WSAEISCONN
#define  EINPROGRESS    WSAEINPROGRESS
#define  NET_EINTR		WSAEINTR
#define	 EALREADY		WSAEALREADY
#define  ENOTSOCK       WSAENOTSOCK
#elif defined(__unix) || defined(_AIX)
#define  NET_EINTR		EINTR
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#define  SOCKET	int
#define  SOCKET_ERROR	-1
#if defined (__sun)
#include <sys/errno.h>
#endif
#endif


#include    "../inc/plchead_all.h"



#define PORTCFGFILE  "/home/ems/h9000/def/siesockport.def"


#define PLC_SCAN_LEN        	12
#define PLC_CMD_LEN		13
#define WR_PLC_ACK_LEN          12



#define  DEVICE_ERROR_BASE	-100
#define  DEVICE_NAME_ERROR				DEVICE_ERROR_BASE - 1
#define  DEVICE_HOST_ERROR				DEVICE_ERROR_BASE - 2
#define  DEVICE_PORT_ERROR				DEVICE_ERROR_BASE - 3
#define  DEVICE_CREATE_SOCK_ERROR		DEVICE_ERROR_BASE - 4
#define  DEVICE_CONNECT_ERROR			DEVICE_ERROR_BASE - 5
#define  DEVICE_CONNECT_TIMEOUT			DEVICE_ERROR_BASE - 6
#define  DEVICE_ID_ERROR			DEVICE_ERROR_BASE - 7
#define  DEVICE_READ_ERROR			DEVICE_ERROR_BASE - 8
#define  DEVICE_WRITE_ERROR			DEVICE_ERROR_BASE - 9

#define  ONE_DELAY	5000
#define  DB_TYPE  1
#define  FETECH_REQ_HEAD_LEN  8
#define  FETECH_REQ_LEN  16

#define  WRITE_REQ_HEAD_LEN  8
#define  WRITE_REQ_LEN  16

#define  WRITE_RES_LEN  16

/*
FETCH request frame FETCH response frame
0 System ID =”S” 0 System ID =”S”
1 =”5” 1 =”5”
2 Length of header =16d. 2 Length of header =16d.
3 ID OP code =01 3 ID OP code =01
4 Length OP code =03 4 Length OP code =03
5 OP code =05 5 OP code =06
6 ORG field =03 6 Ack field =0Fh
7 Length ORG field =08 7 Length ack field =03
8 ORG ID 8 Error field =No
9 DBNR 9 Empty field =FFh
*/

char FetchReqHead[8]={'S','5',16,1,3,5,3,8};
char FetchResHead[8]={'S','5',16,1,3,6,15,3};
char WriteReqHead[8]={'S','5',16,1,3,3,3,8};
char WriteResHead[8]={'S','5',16,1,3,4,15,3};


int ReadPortCfg()
{
	FILE *fp;
    char str[200];
	char host_name[50];
	int port;
	int i=0,j=1;
	unsigned char host_id;
	char log_str[256];
	fp=fopen(PORTCFGFILE,"r");
	if(fp == NULL) 
	{
		err_write( "siesockport.def配置文件不存在!\n" );
		return FALSE;
	}

	for(i=0;i<MAX_WS_HOST_NUM+1;i++)
	{
		sieport[i] = 0;
	}
		
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( strstr(str, "#") )	
			continue;
		if(strlen(str) <= 5 )
			continue;
		sscanf(str,"%s %d",host_name, &port);
		if(GetHostId(host_name, &host_id) == -1)
		{
			continue;
		}
		if( port % 1000 !=0 ) 
		{
			sprintf(log_str,"==== ReadPortCfg err,port=%d",port);
			err_write(log_str);
			return FALSE;
		}
		sieport[host_id] = port;		
	}
	if(sieport[g_localhost_id] == 0)
	{
		sprintf(log_str,"==== ReadPortCfg no host %s port define",g_hostname);
		err_write(log_str);
	}
	fclose(fp);
	return TRUE;
}


int get_last_error()
{
#if defined (WIN32)
    return GetLastError();
#elif defined (__unix) || defined(_AIX)
	
#if defined (__sun)
	return errno;
#endif

#endif
}

void delay_t(int n)
{
#if defined (WIN32)
	Sleep(n/1000);
	
#elif defined (__unix) || defined(_AIX)
	struct timeval tt;
	int    ret;
	while(TRUE)
	{			
		tt.tv_sec=n/1000000;
		tt.tv_usec=n%1000000;
		ret=select(0,0,0,0,&tt);
		if(ret<0){
			if(get_last_error()==NET_EINTR)
			{
				n=n/2;
				if(n>=50000)	continue;
			}
		}
		break;
	}
	
#endif
}

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

/***************************************************************************************
read data from socket under stream schema
re
****************************************************************************************/
int RecvPLC_SIE(MYSOCKET sock,unsigned char *buf,int size,int retry)
{
	int  rxn,rxntime=0;
	char *ptr=(char *)buf;
	int  rxnum=0;
	/* int  i; */

	if(size<=0) return 0;
	if(sock.sock<=0) return DEVICE_ID_ERROR;
	while(TRUE)
	{
		rxn = SOCKET_ERROR;
		if(xnet_select(sock.sock, SOCK_TIMEOUT_S, SOCK_TIMEOUT_MS, READ_STATUS)>0)
		{
			rxn = recv(sock.sock,ptr,size,0);
		}
		if(rxn<=0)
		{
			int err=get_last_error();
	
			printf("read port err===%d\n",err);      
			if(err==EWOULDBLOCK || err==NET_EINTR)
			{
				if(++rxntime>=retry) return rxnum;

				MySleep(500);
				continue;
			}
			else return DEVICE_READ_ERROR;
		}
		/*
		printf("\n Recv buf:\n");
		for(i=0;i<rxn;i++)
		{
			printf("=%02x",(unsigned char) buf[i]);
			if((i+1)%20 == 0)
			{
				printf("\n");
			}		
		}
		printf("\n");
		*/
		rxnum+=rxn;
		ptr+=rxn;
		size-=rxn;
		if(size<=0) break;
	}
	return rxnum;
}

/***************************************************************************************
write a socket under stream schema
****************************************************************************************/

int SendPLC_SIE(MYSOCKET sock,unsigned char  *buf,int size,int retry)
{
	int txn,txntime=0;
	char *ptr=(char *)buf;
	int txnum=0;
	/* int i; */

	if(size<=0) return 0;
	if(sock.sock<=0) return DEVICE_ID_ERROR;

	while(TRUE)
	{
		if(xnet_select(sock.sock, SOCK_TIMEOUT_S, SOCK_TIMEOUT_MS, WRITE_STATUS)>0)
		{		
			txn=send(sock.sock,ptr,size,0);
		}
		if(txn<=0)
		{
			int err=get_last_error();
			if(err==EWOULDBLOCK || err==NET_EINTR)
			{
				if(++txntime>=retry) return txnum;

				MySleep(500);
				continue;
			}
			else 
			return DEVICE_WRITE_ERROR;
		}
		else
		{
			/*
			printf("\n Send buf:\n");
			for(i=0;i<txn;i++)
			{
				printf("=%02x",(unsigned char) buf[i]);
				if((i+1)%20 == 0)
				{
					printf("\n");
				}		
			}
			printf("\n");
			*/
		}
		
		txnum+=txn;
		ptr+=txn;
		size-=txn;
		if(size<=0) break;
	}
	return txnum;
}



/***************************************************************************************
read data from a given plc through given socket  using TCP/IP。
return TRUE when successfully,FALSE otherwise。
sockplc:	socket link to plc
dbname:     db block number,e.g.db10
ref:		starting address of	plc shared memory to be read
size:		word size of data to be read
buffer:		pointer of data  to be stored
****************************************************************************************/
int ReadPLC_SIE(MYSOCKET sockplc, unsigned char dbname, int ref, int size, unsigned char *buffer)
{
	int result,i;
	char tmpbuf;
	unsigned char rxpsdu[4096];
	unsigned char txpsdu[4096];
	memcpy(txpsdu, FetchReqHead, FETECH_REQ_HEAD_LEN);
	ref = ref  * 2;
	txpsdu[8] =  DB_TYPE;
	txpsdu[9] =  dbname;
	txpsdu[10] =	(unsigned char)(ref >> 8);      //high byte of addr
	txpsdu[11] = (unsigned char)(ref & 0xff);     //low byte of addr
	txpsdu[12] =	(unsigned char)(size >> 8);      //high byte of addr
	txpsdu[13] = (unsigned char)(size & 0xff);     //low byte of addr
	
	txpsdu[14] =  0xff;
	txpsdu[15] = 2;
	
	result = SendPLC_SIE(sockplc, txpsdu, FETECH_REQ_LEN,1);
	if (result !=FETECH_REQ_LEN)
	{
		printf("fail to write plc in read connect result=%d\n",result);	
		return FALSE;
		
	}
	
	/* modified by yyp 07.07.31 */
	/* MySleep(200); */
	result = RecvPLC_SIE(sockplc, rxpsdu, FETECH_REQ_LEN+2*size,1);
	
	if (result != (FETECH_REQ_LEN+2*size)  )
	{
		printf("fail to read plc in read connect result===%d\n",result);	
		return(FALSE);
	}
	else 
	{
		if((rxpsdu[0] != FetchResHead[0])
			||(rxpsdu[1] != FetchResHead[1])
			||(rxpsdu[2] != FetchResHead[2])
			||(rxpsdu[3] != FetchResHead[3])
			||(rxpsdu[4] != FetchResHead[4])
			||(rxpsdu[5] != FetchResHead[5])
			||(rxpsdu[6] != FetchResHead[6])
			||(rxpsdu[7] != FetchResHead[7])
			||(rxpsdu[8] != 0)
			||(rxpsdu[9] != 0xff)
			||(rxpsdu[10] != 0x07))
		{
			return FALSE;
		}
		
		memcpy(buffer,&rxpsdu[16],2*size);
	}
	/* added by yyp 07.07.31 */
	for(i=0;i<2*size;)
	{
		tmpbuf= buffer[i]; 
		buffer[i]= buffer[i+1]; 
		buffer[i+1] = tmpbuf;
		i=i+2;
	}
	return TRUE;
}

int 	WritePLC_SIE(MYSOCKET sockplc, unsigned char dbname, int ref, int size, unsigned char *buffer)
{
	int result;
#ifdef 	PLCDEBUG
	int i;
#endif
	char	log_str[256];
	unsigned char txpsdu[4096];
	unsigned char rxpsdu[16];  //write response bytes
	ref = ref  * 2;	
	if(( ref<0 ) || (size <=0 ))
	{
		sprintf(log_str,"WritePLC_SIE(): ref =%d or size =%d invalid\n",ref,size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);		
		return FALSE;
	}
	sprintf(log_str,"==== Run WritePLC_SIE, dbname=%d, ref=%d, size=%d \n",dbname,ref,size);
	log_printf(H9000_LOG_INFO,2,0,log_str);
	
	memcpy(txpsdu, WriteReqHead, WRITE_REQ_HEAD_LEN);
	txpsdu[8] =  DB_TYPE;
	txpsdu[9] =  dbname;
	txpsdu[10] =	(unsigned char)(ref >> 8);      //high byte of addr
	txpsdu[11] = (unsigned char)(ref & 0xff);     //low byte of addr
	txpsdu[12] =	(unsigned char)(size >> 8);      //high byte of addr
	txpsdu[13] = (unsigned char)(size & 0xff);     //low byte of addr
	txpsdu[14] =  0xff;
	txpsdu[15] = 2;
	memcpy(&txpsdu[16],buffer,2*size);
	result = SendPLC_SIE(sockplc, txpsdu, WRITE_REQ_LEN+2*size,1);
	if (result != (WRITE_REQ_LEN+2*size) )
	{
		printf("SendPLC_SIE len error result=%d\n",result);
		sprintf(log_str,"SendPLC_SIE len error result=%d\n",result);
		log_printf(H9000_LOG_ERROR,1,0,log_str);	
		return FALSE;
	}
	sprintf(log_str,"==== ReadPLC_SIE, sendcount=%d >>\n",result);
	log_printf(H9000_LOG_INFO,2,0,log_str);
#ifdef 	PLCDEBUG
	for(i=0;i<result;i++)
	{
		printf("=%02x",(unsigned char) txpsdu[i]);
		if((i+1)%20 == 0)
		{
			printf("\n");
		}		
	}
	printf("\n");
#endif
	/* modified by yyp */
	/* MySleep(200); */
	result = RecvPLC_SIE(sockplc, rxpsdu, WRITE_RES_LEN,1);
	if (result !=  WRITE_RES_LEN )
	{
		printf("WritePLC_SIE:reas response error result===%d\n",result);
		sprintf(log_str,"WritePLC_SIE: call RecvPLC_SIE() error\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);				
		return(FALSE);
	}
	else
	{
		if((rxpsdu[0] != WriteResHead[0])
			||(rxpsdu[1] != WriteResHead[1])
			||(rxpsdu[2] != WriteResHead[2])
			||(rxpsdu[3] != WriteResHead[3])
			||(rxpsdu[4] != WriteResHead[4])
			||(rxpsdu[5] != WriteResHead[5])
			||(rxpsdu[6] != WriteResHead[6])
			||(rxpsdu[7] != WriteResHead[7])
			||(rxpsdu[8] != 0)
			||(rxpsdu[9] != 0xff)
			||(rxpsdu[10] != 0x07))
		{	
			return FALSE;
		}
	}
	
	return (TRUE);	
}














