/****************************************************************************************
FILENAME:	mbplcnet.c
FUNCTION:	
	some sub functions of plc comm  for "QUANTUM"	

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
 2005.11.7	start to modify the code for H9000 V4.0		yyp
****************************************************************************************/

#include    "../inc/plchead_all.h"

#define PLC_SCAN_LEN        	12
#define PLC_CMD_LEN				13
#define WR_PLC_ACK_LEN          12

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
int  SendPLC_MB(MYSOCKET sockplc, char *buffer, int count)
{
	int result;
	int error = 0, len = sizeof(int);
	char log_str[256];
	result = SOCKET_ERROR;
	if(sockplc.sock == INVALID_SOCKET)
	{
		sprintf(log_str,"SendPLC_MB():error sockplc=%d ",sockplc.sock);	
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return SOCKET_ERROR;
	}
	if(count <= 0 )
	{
		sprintf(log_str,"SendPLC_MB(): error,count=%d\n",count);
		log_printf(H9000_LOG_ERROR,1,0,log_str);	
		return SOCKET_ERROR;
	}

	getsockopt(sockplc.sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
	if (error != 0)
	{
		sprintf(log_str,"SendPLC_MB(): getsock error=%d ",error);	
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return SOCKET_ERROR;
	}

	if(xnet_select(sockplc.sock, SOCK_TIMEOUT_S, SOCK_TIMEOUT_MS, WRITE_STATUS)>0)
	{
		result = send(sockplc.sock, (char *)buffer, count, 0);
	}
	else
	{

	}
	error = 0;
	getsockopt(sockplc.sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
	if (error != 0)
	{
		sprintf(log_str,"SendPLC_MB(): getsock after send error=%d ",error);	
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return SOCKET_ERROR;		
	}
	
	if ( result == SOCKET_ERROR )
	{	
		sprintf(log_str,"SendPLC_MB():error send() timeout ");	
		log_printf(H9000_LOG_ERROR,1,0,log_str);
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
int  RecvPLC_MB(MYSOCKET sockplc, char *buffer, int count, int type)
{
	int result;
	int error = 0, len = sizeof(int);	
	int i;
	int timeout_s,timeout_ms;
	char log_str[256];
	for (i = 0; i < count; i += result) 
	{
		if(sockplc.sock == INVALID_SOCKET)
		{
			sprintf(log_str,"RecvPLC_MB():error sockplc=%d ",sockplc.sock);	
			log_printf(H9000_LOG_ERROR,1,0,log_str);		
			return FALSE;
		}
		
		getsockopt(sockplc.sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
		if (error != 0)
		{
			sprintf(log_str,"RecvPLC_MB(): getsock error=%d ",error);	
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return FALSE;		
		}
        	
		result = SOCKET_ERROR;
		if(type ==1)
		{
			timeout_s = 2 * SOCK_TIMEOUT_S;
			timeout_ms = SOCK_TIMEOUT_MS_0X;
		}
		else
		{
			timeout_s = SOCK_TIMEOUT_S;
			timeout_ms = SOCK_TIMEOUT_MS;			
		}			
		if(xnet_select(sockplc.sock, timeout_s, timeout_ms, READ_STATUS)>0)
		{
			result = recv(sockplc.sock, (char *)(buffer + i), count - i, RECV_FLAGS);
		}
		else
		{
		
		}
		
		error = 0;
        getsockopt(sockplc.sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
       	if (error != 0)
        {
		sprintf(log_str,"RecvPLC_MB(): getsock after recv error=%d ",error);	
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;		
        }
		
		if ((result == SOCKET_ERROR) ||( result < RECVPDUHEAD))
		{
			sprintf(log_str,"RecvPLC_MB():error recv() timeout or result invalid. res=%d \n",result);	
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return (FALSE);
		}
		else if(result == RECVPDUHEAD)
		{
			sprintf(log_str,"RecvPLC_MB(): error because recv() response fault, errno =%d, exno=%d\n",
				(unsigned char)buffer[i + RECVPDUHEAD -2], (unsigned char)buffer [i + RECVPDUHEAD-1]);	
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return (FALSE);				
		}
	} /*** end for ***/
	if (i != count)
	{
		sprintf(log_str,"RecvPLC_MB(): error return FALSE because recvcount!= required\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return (FALSE);
	}
	
	sprintf(log_str,"==== RecvPLC_MB, recvcount=%d >>\n",count);
	log_printf(H9000_LOG_INFO,2,0,log_str);

#ifdef PLCDEBUG
	for(i=0;i<count; i++)
	{
		printf("%02x=",(unsigned char)buffer[i]);
		if(i==(RECVPDUHEAD-1))	
		{	
			printf("\n");
		}
		else
		{
		if((i+1-RECVPDUHEAD)%20 == 0)
		{
			printf("\n");
		}
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
int ReadPLC_MB(MYSOCKET sockplc, unsigned char seg, int ref, int size, unsigned char *buffer)
{
	int result;
	int i, j, len;
	int count;
	int offset;
	unsigned char dummy_send[MAX_RECV_SIZE];
	unsigned char dummy_recv[MAX_RECV_SIZE];
	unsigned char tmpbuf[2048];
	int           base_addr;
	char	log_str[256];

	memset(tmpbuf,0,2048);	
	if( ref<0 || size <=0 || size >= MAX_SCAN_BUF )
	{
		sprintf(log_str,"ReadPLC_MB(): ref =%d or size =%d invalid\n",ref,size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);		
		return FALSE;
	}
	sprintf(log_str,"==== Run ReadPLC_MB, seg=%d, ref=%d, size=%d \n",seg,ref,size);
	log_printf(H9000_LOG_INFO,2,0,log_str);
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
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return (FALSE);
		}

		sprintf(log_str,"==== ReadPLC_MB, sendcount=%d ,id = %d/%d >>\n",result,j+1,count);
		log_printf(H9000_LOG_INFO,2,0,log_str);
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
			log_printf(H9000_LOG_ERROR,1,0,log_str);				
			return(FALSE);
		}
		/* check out */
		if(dummy_send[7]!=dummy_recv[7] || dummy_recv[8]!= dummy_send[11] * 2 )
		{
			sprintf(log_str,"ReadPLC_MB: check error, query bytes=%d recv bytes=%d\n",
				dummy_send[11]*2,dummy_recv[8]);
			log_printf(H9000_LOG_ERROR,1,0,log_str);				
			return(FALSE);			
		}		
		for(i=0;i<2*len;)
		{
			tmpbuf[offset+i+1]= dummy_recv[ i+ RECVPDUHEAD]; 
			tmpbuf[offset+i]= dummy_recv[ i+ RECVPDUHEAD+1]; 
			i=i+2;
		}
		offset=offset+2*len;
	}
	memcpy(buffer,tmpbuf,offset);
	return (TRUE);
}

/**********************************************************************************************************
read data from a given plc through given socket  using TCP/IP。send multiple request
in one scan time,then wait for all response.return TRUE when successfully,FALSE otherwise。
sockplc:	socket link to plc
ref:		starting address of	plc shared memory to be read
size:		word size of data to be read
buffer:		pointer of data  to be stored
***********************************************************************************************************/
int ReadPLC_MULT_MB(MYSOCKET sockplc, unsigned char seg, int ref, int size, unsigned char *buffer )
{
	int result;
	int i, j, len;
	int count;
	int offset=0;
	int lencount=0;
	unsigned char dummy_send[MAX_RECV_SIZE];
	unsigned char dummy_recv[MAX_RECV_SIZE];
	unsigned char tmpbuf[2048];
	int           base_addr;
	char	log_str[256];
	memset(tmpbuf,0,2048);		
	base_addr = ref;
	count =(size-1) / ( MAX_QUERY_SIZE ) + 1;
	if( ref<0 || size <=0 || size >= MAX_SCAN_BUF )
	{
		sprintf(log_str,"ReadPLC_MULT_MB(): ref =%d or size =%d invalid\n",ref,size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;
	}	
	sprintf(log_str,"==== Run ReadPLC_MULT_MB, seg=%d, ref=%d, size=%d ,count=%d \n",seg,ref,size,count);
	log_printf(H9000_LOG_INFO,2,0,log_str);
	
	for(j=0;j<count;j++)
	{
		memset(dummy_send,0,MAX_RECV_SIZE);	
		ref=base_addr + lencount;
		memcpy(dummy_send, SysReadReq, PLC_SCAN_LEN);
		dummy_send[7] = (unsigned char) READ_0X;
		dummy_send[8] = (unsigned char)(ref >> 8);           /* Reg. high Addr.(1 word)*/
		dummy_send[9] = (unsigned char)(ref & 0xff);         /* Low Addr. */   
		len = MAX_QUERY_SIZE * 16;
		if(j==count-1)	len = ( size -j* MAX_QUERY_SIZE ) * 16;
		dummy_send[10] = (unsigned char)(len >> 8);               	/* size,high Addr. */
		dummy_send[11] = (unsigned char)(len & 0xff);      /* Low Addr. */
		
		if(len==0)	continue;
		result = SOCKET_ERROR;
		result = SendPLC_MB(sockplc, (char *)dummy_send, PLC_SCAN_LEN);
		if (result == SOCKET_ERROR || result !=PLC_SCAN_LEN)
		{
			sprintf(log_str,"ReadPLC_MULT_MB():error send() timeout or result=%d",result);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return (FALSE);
		}

		lencount += len;
		
		sprintf(log_str,"==== ReadPLC_MULT_MB, sendcount=%d ,  id = %d/%d >>\n",result,j+1,count);
		log_printf(H9000_LOG_INFO,2,0,log_str);

#ifdef 	PLCDEBUG
		for(i=0;i<result;i++)
		{
			printf("=%02x",(unsigned char) dummy_send[i]);
			if((i+1)%20 == 0)
			{
				printf("\n");
			}		
		}
#endif	
	} /* end for */

	for(j=0;j<count;j++)
	{
		memset(dummy_recv,0,MAX_RECV_SIZE);	
		len = MAX_QUERY_SIZE * 2;      /* byte number */
		if(j==count-1)	len = ( size -j* MAX_QUERY_SIZE ) * 2;
		if(len==0)	continue;		
		sprintf(log_str,"==== ReadPLC_MULT_MB, lencount=%d,len=%d\n",lencount,len);
		log_printf(H9000_LOG_INFO,2,0,log_str);
		result = RecvPLC_MB(sockplc, (char *)dummy_recv, len + RECVPDUHEAD,1);
		if (result==FALSE  )
		{
			sprintf(log_str,"ReadPLC_MULT_MB(): call RecvPLC_MB() error\n");
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return(FALSE);
		}
		/* Swap2Bytes because modbus low is head by 0X */
		for(i=0;i< len;)
		{
			tmpbuf[offset+i]= dummy_recv[ i+ RECVPDUHEAD ];  
			tmpbuf[offset+i+1]= dummy_recv[ i+ RECVPDUHEAD + 1]; 
			i=i+2;
		}
		offset=offset + len;
	} /* end for */
	
	/* check out */
	if(dummy_send[7]!=dummy_recv[7] || dummy_recv[8]!= len )
	{
		sprintf(log_str,"ReadPLC_MULT_MB: check error, query bytes=%d recv bytes=%d\n",
			len,dummy_recv[8]);
		log_printf(H9000_LOG_ERROR,1,0,log_str);				
		return(FALSE);			
	}
	
	memcpy(buffer,tmpbuf,offset);
	/*
	printf("==== ReadPLC_MULT_MB, buf size=%d\n",offset);
	for(i=0;i<offset;i++)
	{
		printf("=%02x",(unsigned char) buffer[i]);
		if((i+1)%20 == 0)
		{
			printf("\n");
		}		
	}
	*/
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
int WritePLC_MB(MYSOCKET sockplc, unsigned char seg, int ref, int size, unsigned char *buffer)
{
	int result;
	int i;
	unsigned char dummy_send[MAX_RECV_SIZE];
	unsigned char dummy_recv[MAX_RECV_SIZE];
	char	log_str[256];
	if( ref<0 || size <=0 || size >= MAX_SCAN_BUF )
	{
		sprintf(log_str,"WritePLC_MB(): ref =%d or size =%d invalid\n",ref,size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;
	}

	sprintf(log_str,"==== WritePLC_MB():sock=%d,seg=%d,ref=%d,size=%d\n",sockplc.sock,seg,ref,size);
	log_printf(H9000_LOG_INFO,2,0,log_str);
	/***** package command  code ************/
	if(size > 100)
	{
		sprintf(log_str,"WritePLC_MB(): error size is %d, the Max is 100\n",size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return (FALSE);
	}
	
	memcpy(dummy_send,SysWriteReq, PLC_CMD_LEN);
	dummy_send[5] = dummy_send[5] + (unsigned char)(size*2);              
	dummy_send[8] = (unsigned char)(ref >> 8);           /* Reg. high Addr.(1 word) */
	dummy_send[9] = (unsigned char)(ref & 0xff);         /* Low Addr.*/
	dummy_send[10] = 0;                                  /* Note:word number must less than 255(high=0) */
	dummy_send[11] = (unsigned char)(size & 0xff);       /* Word number (low) must less than 100 for quantum */
	dummy_send[12] = dummy_send[11]*2;	               /* byte number (low)*/
	for(i=0;i<2*size;)
	{
		dummy_send[ i+ PLC_CMD_LEN] = buffer[i+1];		/*only for modicon*/   
		dummy_send[ i+ PLC_CMD_LEN+1] = buffer[i];
		i = i+2;
	}

	result = SOCKET_ERROR;
	result = SendPLC_MB(sockplc, (char *)dummy_send, dummy_send[5]+6);
	if (result == SOCKET_ERROR )
	{
		sprintf(log_str,"WritePLC_MB():error send() timeout or result=%d",result);
		log_printf(H9000_LOG_ERROR,1,0,log_str);	
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
		log_printf(H9000_LOG_ERROR,1,0,log_str);		
		return (FALSE);
	}
	return (TRUE);
}
