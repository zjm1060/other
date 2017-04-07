/****************************************************************************************
FILENAME:	mbplcnet.c
FUNCTION:	
	some sub functions of plc comm  for "QUANTUM"	

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
 2005.11.7	start to modify the code for H9000 V4.0		yyp
****************************************************************************************/
#include "\home\ems\system\lan\v4.0a\inc\lan.h"
#include "../../../plc/v4.0a/inc/plc_com.h"
#include "../inc/mbc_com.h"

unsigned char SysReadReq[12] = {
    0, 0, 0, 0, 0, 6, 
    1, 0, 0, 0, 0, 0};
    

unsigned char SysWriteReq[13] = {
    0, 0,    0, 0, 0, 7,
    1, 0x10, 0, 0, 0, 0,0 };

/***************************************************************************************
send data to a given plc through given socket  using TCP/IP。
return TRUE when successfully,FALSE otherwise。
sockplc:	socket link to plc
count:		size of data to be sent
buffer:		pointer of data  to be sent
****************************************************************************************/
int  SendPLC_MB(SOCKET sockplc, char *buffer, int count)
{
	int result;
	int error = 0, len = sizeof(int);
	result = SOCKET_ERROR;
	if(sockplc == INVALID_SOCKET)
	{
		sprintf(log_str,"SendPLC_MB():error sockplc=%d ",sockplc);	
		log_printf(ERR,1,PLC_APP,log_str);
		return SOCKET_ERROR;
	}
	if(count <= 0 )
	{
		sprintf(log_str,"SendPLC_MB(): error,count=%d\n",count);
		log_printf(ERR,1,PLC_APP,log_str);	
		return SOCKET_ERROR;
	}

	getsockopt(sockplc, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
	if (error != 0)
	{
		sprintf(log_str,"SendPLC_MB(): getsock error=%d ",error);	
		log_printf(ERR,1,PLC_APP,log_str);
		return SOCKET_ERROR;
	}

	if(xnet_select(sockplc, SOCK_TIMEOUT_S, SOCK_TIMEOUT_MS, WRITE_STATUS)>0)
	{
		result = send(sockplc, (char *)buffer, count, 0);
	}
	else
	{

	}
	error = 0;
	getsockopt(sockplc, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
	if (error != 0)
	{
		sprintf(log_str,"SendPLC_MB(): getsock after send error=%d ",error);	
		log_printf(ERR,1,PLC_APP,log_str);
		return SOCKET_ERROR;		
	}
	
	if ( result == SOCKET_ERROR )
	{	
		sprintf(log_str,"SendPLC_MB():error send() timeout ");	
		log_printf(ERR,1,PLC_APP,log_str);
	}	
		
	return result;
}

/***************************************************************************************
receive data from a given plc through given socket  using TCP/IP。
return TRUE when successfully,FALSE otherwise。
sockplc:	socket link to plc
count:		size of data to be received
buffer:		pointer of data  to be stored
****************************************************************************************/
int  RecvPLC_MB(SOCKET sockplc, char *buffer, int count, int type)
{
	int result;
	int error = 0, len = sizeof(int);	
	int i;
	int timeout_s,timeout_ms;
	for (i = 0; i < count; i += result) 
	{
		if(sockplc == INVALID_SOCKET)
		{
			sprintf(log_str,"RecvPLC_MB():error sockplc=%d ",sockplc);	
			log_printf(ERR,1,PLC_APP,log_str);		
			return FALSE;
		}
		
		getsockopt(sockplc, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
		if (error != 0)
		{
			sprintf(log_str,"RecvPLC_MB(): getsock error=%d ",error);	
			log_printf(ERR,1,PLC_APP,log_str);
			return FALSE;		
		}
        	
		result = SOCKET_ERROR;
		if(type ==1)
		{
			timeout_s = 2 * SOCK_TIMEOUT_S;
			timeout_ms = SOCK_TIMEOUT_MS_0X;
		}
		else if(type == 2 )
		{
			timeout_s = SOCK_TIMEOUT_S_MBS;
			timeout_ms = 0;	
	
		}
		else
		{
			timeout_s = SOCK_TIMEOUT_S;
			timeout_ms = SOCK_TIMEOUT_MS;					
		}
		if(xnet_select(sockplc, timeout_s, timeout_ms, READ_STATUS)>0)
		{
			result = recv(sockplc, (char *)(buffer + i), count - i, RECV_FLAGS);
		}
		else
		{
		
		}
		error = 0;
        getsockopt(sockplc, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
       	if (error != 0)
        {
			sprintf(log_str,"RecvPLC_MB(): getsock after recv error=%d ",error);	
			log_printf(ERR,1,PLC_APP,log_str);
			return FALSE;		
        }
		
		if ( result == SOCKET_ERROR )
		{
			sprintf(log_str,"RecvPLC_MB():error recv() timeout or result invalid. res=%d \n",result);	
			log_printf(ERR,1,PLC_APP,log_str);
			return (FALSE);
		}
		else if(result == RECVPDUHEAD)
		{
			sprintf(log_str,"RecvPLC_MB(): error because recv() response fault, errno =%d, exno=%d\n",
				(unsigned char)buffer[i + RECVPDUHEAD -2], (unsigned char)buffer [i + RECVPDUHEAD-1]);	
			log_printf(ERR,1,PLC_APP,log_str);
			return (FALSE);				
		}
	} /*** end for ***/
	if (i != count)
	{
		sprintf(log_str,"RecvPLC_MB(): error return FALSE because recvcount!= required\n");
		log_printf(ERR,1,PLC_APP,log_str);
		return (FALSE);
	}
	
	sprintf(log_str,"\n==== RecvPLC_MB, recvcount=%d >>\n",count);
	log_printf(INF,1,PLC_APP,log_str);

#ifdef PLCDEBUG
	for(i=0;i<count; i++)
	{
		printf("%02x=",(unsigned char)buffer[i]);
		if((i+1)%20 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
#endif 
	return (count);
}

/***************************************************************************************
read data from a given plc through given socket  using TCP/IP。
return TRUE when successfully,FALSE otherwise。
sockplc:	socket link to plc
ref:		starting address of	plc shared memory to be read
size:		word size of data to be read
buffer:		pointer of data  to be stored
****************************************************************************************/
int ReadPLC_MB(SOCKET sockplc, unsigned char seg, int ref, int size, unsigned char *buffer)
{
	int result;
	int i, j, len;
	int count;
	int offset;
	unsigned char dummy_send[MAX_RECV_SIZE];
	unsigned char dummy_recv[MAX_RECV_SIZE];
	unsigned char tmpbuf[2048];
	int           base_addr;
	memset(tmpbuf,0,2048);	
	if( ref<0 || size <=0 || size >= MAX_SCAN_BUF )
	{
		sprintf(log_str,"ReadPLC_MB(): ref =%d or size =%d invalid\n",ref,size);
		log_printf(ERR,1,PLC_APP,log_str);		
		return FALSE;
	}
	sprintf(log_str,"==== Run ReadPLC_MB, seg=%d, ref=%d, size=%d \n",seg,ref,size);
	log_printf(INF,2,PLC_APP,log_str);
	base_addr = ref;
	/* bsize = MAX_QUERY_SIZE; */
	count =(size-1) / ( MAX_QUERY_SIZE ) + 1;
	offset=0;
	for(j=0;j<count;j++)
	{
		ref=base_addr + j*MAX_QUERY_SIZE;
		memcpy(dummy_send, SysReadReq, PLC_SCAN_LEN);
		dummy_send[7] = (unsigned char) READ_4X;
		dummy_send[8] = (unsigned char)(ref >> 8);           /* Reg. high Addr.(1 word)*/
		dummy_send[9] = (unsigned char)(ref & 0xff);         /* Low Addr. */   
		dummy_send[10] = 0;                                 /* Note:byte number must less than 255(high=0) */
		dummy_send[11] = (unsigned char)(MAX_QUERY_SIZE & 0xff);      /* Word number (low) */
	
		if(j==count-1)	dummy_send[11] = size -j* MAX_QUERY_SIZE; 
		len=dummy_send[11];
		if(len==0)	continue;
		result = SOCKET_ERROR;
		result = SendPLC_MB(sockplc, (char *)dummy_send, PLC_SCAN_LEN);
		if (result == SOCKET_ERROR || result !=PLC_SCAN_LEN)
		{
			sprintf(log_str,"error send() timeout or result=%d\n",result);
			log_printf(ERR,1,PLC_APP,log_str);	
			return (FALSE);
		}

		sprintf(log_str,"\n==== ReadPLC_MB, sendcount=%d ,id = %d/%d >>\n",result,j+1,count);
		log_printf(INF,1,PLC_APP,log_str);
#ifdef 	PLCDEBUG
		for(i=0;i<result;i++)
		{
			printf("=%02x",(unsigned char) dummy_send[i]);
			if((i+1)%20 == 0)
			{
				printf("\n");
			}		
		}
		printf("\n");
#endif
		result = RecvPLC_MB(sockplc, (char *)dummy_recv, len*2+RECVPDUHEAD,0);
		if (result==FALSE  )
		{
			sprintf(log_str,"ReadPLC_MB: call RecvPLC_MB() error\n");
			log_printf(ERR,1,PLC_APP,log_str);				
			return(FALSE);
		}
		/* check out */
		if(dummy_send[7]!=dummy_recv[7] || dummy_recv[8]!= dummy_send[11] * 2 )
		{
			sprintf(log_str,"ReadPLC_MB: check error, query bytes=%d recv bytes=%d\n",
				dummy_send[11]*2,dummy_recv[8]);
			log_printf(ERR,1,PLC_APP,log_str);				
			return(FALSE);			
		}
		/*
		for(i=0;i<2*len;)
		{
			tmpbuf[offset+i+1]= dummy_recv[ i+ RECVPDUHEAD]; 
			tmpbuf[offset+i]= dummy_recv[ i+ RECVPDUHEAD+1]; 
			i=i+2;
		}
		*/
		memcpy(tmpbuf+offset,dummy_recv+RECVPDUHEAD,2*len);
		offset=offset+2*len;
	}
	memcpy(buffer,tmpbuf,offset);
	return (TRUE);
}

/***************************************************************************************
write data or cmd to a given plc through given socket  using TCP/IP。
return TRUE when successfully,FALSE otherwise。
sockplc:	socket link to plc
ref:		starting address of	plc shared memory to be written
size:		word size of data to be written
buffer:		pointer of data to be written
****************************************************************************************/
int WritePLC_MB(SOCKET sockplc, unsigned char seg, int ref, int size, unsigned char *buffer)
{
	int result;
	int i,j;
	int len;
	int count;
	int	base_addr;
	unsigned char dummy_send[MAX_RECV_SIZE];
	unsigned char dummy_recv[MAX_RECV_SIZE];
	if( ref<0 || size <=0 || size >= MAX_SCAN_BUF )
	{
		sprintf(log_str,"WritePLC_MB(): ref =%d or size =%d invalid\n",ref,size);
		log_printf(ERR,1,PLC_APP,log_str);
		return FALSE;
	}

	sprintf(log_str,"==== WritePLC_MB():sock=%d,seg=%d,ref=%d,size=%d\n",sockplc,seg,ref,size);
	log_printf(INF,2,PLC_APP,log_str);
	base_addr = ref;
	count =(size-1) / ( MAX_QUERY_SIZE ) + 1;
	for(j=0;j<count;j++)
	{
		ref=base_addr + j * MAX_QUERY_SIZE;	
		memset(dummy_send,0,MAX_RECV_SIZE);
		memcpy(dummy_send,SysWriteReq, PLC_CMD_LEN);
		dummy_send[8] = (unsigned char)(ref >> 8);           /* Reg. high Addr.(1 word) */
		dummy_send[9] = (unsigned char)(ref & 0xff);         /* Low Addr.*/
		dummy_send[10] = 0;                                  /* Note:word number must less than 255(high=0) */
		dummy_send[11] = (unsigned char)(MAX_QUERY_SIZE & 0xff);      /* Word number (low) */
		if(j==count-1)	dummy_send[11] = size -j* MAX_QUERY_SIZE; 
		dummy_send[5] = dummy_send[5] + (unsigned char)(dummy_send[11]*2);  
		len=dummy_send[11];
		if(len==0)	continue;
		dummy_send[12] = dummy_send[11]*2;	                 /* byte number (low)*/
		memcpy(dummy_send+PLC_CMD_LEN, buffer + j * MAX_QUERY_SIZE, len*2);	

		result = SOCKET_ERROR;
		result = SendPLC_MB(sockplc, (char *)dummy_send, dummy_send[5]+6);
		if (result == SOCKET_ERROR )
		{
			sprintf(log_str,"WritePLC_MB():error send() timeout or result=%d",result);
			log_printf(ERR,1,PLC_APP,log_str);	
			return (FALSE);
		}
		else
		{
		
		}
	#ifdef PLCDEBUG
		printf("\n==== WritePLC_MB(): Write data >>\n");
		for(i=0;i<result;i++)
		{
			printf("=%02x",dummy_send[i]);
			if((i+1)%20 == 0)
			{
				printf("\n");
			}		
		}
		printf("\n");
	#endif
		result = RecvPLC_MB(sockplc, (char *)dummy_recv, WR_PLC_ACK_LEN,0);
		if (result==FALSE )
		{
			sprintf(log_str,"WritePLC_MB(): call RecvPLC_MB() error\n");
			log_printf(ERR,1,PLC_APP,log_str);		
			return (FALSE);
		}
	}
	return (TRUE);
}
