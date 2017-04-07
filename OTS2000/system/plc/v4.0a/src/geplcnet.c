/******************************************************************************************
FILENAME:	geplcnet.c
FUNCTION:	
	some sub functions of plc comm  for "GE"

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
 2008.6.27  upgraded for AB								yyp
 2005.11.7	start to modify the code for H9000 V4.0		yyp
****************************************************************************************/

#include    "../inc/plchead_all.h"

struct 
{
	unsigned char prognum;
	unsigned char acclev;
	unsigned short scantime;

#ifdef _BIG_ENDIAN_

	unsigned short current_plc_mode  :4;
	unsigned short unused            :1;
	unsigned short cp_changed        :1;
	unsigned short oem_protected_bit :1;
	unsigned short run_switch_on     :1;
	unsigned short outputs_enabled   :1;
	unsigned short prg_attached      :1;
	unsigned short io_fault_present  :1;
	unsigned short plc_fault_present :1;
	unsigned short io_fault_since    :1;
	unsigned short plc_fault_since   :1;
	unsigned short const_sweep_mode  :1;
	unsigned short oversweep_flag    :1;
#else
	unsigned short oversweep_flag    :1;
	unsigned short const_sweep_mode  :1;
	unsigned short plc_fault_since   :1;
	unsigned short io_fault_since    :1;
	unsigned short plc_fault_present :1;
	unsigned short io_fault_present  :1;
	unsigned short prg_attached      :1;
	unsigned short outputs_enabled   :1;
	unsigned short run_switch_on     :1;
	unsigned short oem_protected_bit :1;
	unsigned short cp_changed        :1;
	unsigned short unused            :1;
	unsigned short current_plc_mode  :4;
#endif

} cpusts;

int SRP_error_code;
unsigned short max_r;
unsigned short max_ai;
unsigned short max_aq;
unsigned short max_i;
unsigned short max_q;
unsigned short max_t;
unsigned short max_m;
unsigned short max_s;
unsigned short max_g;
unsigned short major_type;
unsigned short minor_type;
unsigned char seq_num;
   
unsigned char SRTPconnection[SEND_SIZE_GE] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 };

unsigned char EstabSession[SEND_SIZE_GE] = {
    8, 0, 1, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0xc0,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 0x4f, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 }; 

unsigned char ChangeAccess[SEND_SIZE_GE] = {
    2, 0, 2, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 0xc0,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 0x21, 0xff, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 }; 

unsigned char SetPLCState[SEND_SIZE_GE] = {
    2, 0, 3, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 3, 0xc0,
    0, 0, 0, 0, 0x10, 0x0a, 0, 0,
    1, 1, 0x23, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 }; 

unsigned char Login[SEND_SIZE_GE] = {
    2, 0, 4, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 4, 0xc0,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 0x20, 0x11, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 }; 

unsigned char Logout[SEND_SIZE_GE] = {
    2, 0, 5, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 5, 0xc0,
    0, 0, 0, 0, 0x10, 0x0a, 0, 0,
    1, 1, 0x20, 4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 }; 

unsigned char SysReadReq_GE[SEND_SIZE_GE] = {
    2, 0, 6, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 6, 0xc0,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 4, 8, 0, 0, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0 };

unsigned char ShortWriteReq[SEND_SIZE_GE] = {
    2, 0, 8, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 8, 0xc0,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 7, 8, 9, 0, 4, 0,
    1, 0, 2, 0, 3, 0, 4, 0 };

unsigned char LongWriteReq[SEND_SIZE_GE] = {
    2, 0, 9, 0, 50, 0, 0, 0,
    0, 2, 0, 0, 0, 0, 0, 0,
    0, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 9, 0x80,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 50, 0, 0, 0, 0, 0,
    1, 1, 7, 8, 49, 0, 25, 0 };

unsigned char GetPLCType[SEND_SIZE_GE] = {
    2, 0, 10, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 10, 0xc0,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 4, 0xec, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 };

unsigned char GetConfigMemSizes[SEND_SIZE_GE] = {
    2, 0, 11, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 11, 0xc0,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 4, 0xe6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 };

unsigned char TaskReadReq[SEND_SIZE_GE] = {
    2, 0, 12, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 12, 0xc0,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 5, 4, 29, 0, 5, 0,
    'P', 'R', 'G', 'N', 'A', 'M', 'E', 0};

unsigned char TaskWriteReq[SEND_SIZE_GE] = {
    2, 0, 13, 0, 38, 0, 0, 0,
    0, 2, 0, 0, 0, 0, 0, 0,
    0, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 15, 0x80,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 38, 0, 0, 0, 0, 0,
    1, 1, 8, 4, 29, 0, 15, 0 };

unsigned char PBlockReadReq[SEND_SIZE_GE] = {
    2, 0, 14, 0, 16, 0, 0, 0,
    0, 2, 0, 0, 0, 0, 0, 0,
    0, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 14, 0x80,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 8, 0, 0, 0, 0, 0,
    1, 1, 6, 0, 19, 0, 10, 0};

unsigned char PBlockWriteReq[SEND_SIZE_GE] = {
    2, 0, 15, 0, 36, 0, 0, 0,
    0, 2, 0, 0, 0, 0, 0, 0,
    0, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 15, 0x80,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 36, 0, 0, 0, 0, 0,
    1, 1, 9, 0, 19, 0, 10, 0 };

unsigned char TODWriteReq[SEND_SIZE_GE] = {
    2, 0, 16, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 16, 0xc0,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 36, 1, 0, 0x30, 0x12, 9,
    1, 0x98, 5, 0, 0, 0, 0, 0 };        /* set time to 12:30:00 9 Jan 98 */

unsigned char ReadTaskList[SEND_SIZE_GE] = {
    2, 0, 17, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 17, 0xc0,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 };

unsigned char ReadPBlockList[SEND_SIZE_GE] = {
    2, 0, 18, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 18, 0xc0,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 39, 'T', 'A', 'S', 'K', '0',
    '0', '1', 0, 0, 0, 0, 0, 0 };

unsigned char ReadCntrlInfo[SEND_SIZE_GE] = {
    2, 0, 19, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 19, 0xc0,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 67, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 };

unsigned char ShortStatusReq[SEND_SIZE_GE] = {
    2, 0, 20, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 20, 0xc0,
    0, 0, 0, 0, 0x10, 0x0e, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 };

/*
 * SizeCalc - compute number of bytes from seg sel and native size of xfer
 */

int SizeCalc(unsigned char seg, int ref, int size)
{
	switch (seg) 
	{
	case 0 :                                /* Local Data Table (%L) */
	case 4 :                                /* Program Data Table (%P) */
	case 8 :                                /* Register Table (%R) */
	case 10 :                               /* Analog Input Table (%AI) */
	case 12 :                               /* Analog Output Table (%AQ) */

		return (2 * size);                  /* size is in words */

	case 16 :                               /* Discrete Input Table, byte mode (%I) */
	case 18 :                               /* Discrete Output Table, byte mode (%Q) */
	case 20 :                               /* Discrete Temporary Table, byte mode (%T) */
	case 22 :                               /* Discrete Internal Table, byte mode (%M) */
	case 24 :                               /* Discrete System A Table, byte mode (%SA) */
	case 26 :                               /* Discrete System B Table, byte mode (%SB) */
	case 28 :                               /* Discrete System C Table, byte mode (%SC) */
	case 30 :                               /* Discrete System D Table, byte mode (%S) */
	case 56 :                               /* Genius Global Data Table, byte mode (%G, %GA, ...) */

		return (size);                      /* size is in bytes */

	case 70 :                               /* Discrete Input Table, bit mode (%I) */
	case 72 :                               /* Discrete Output Table, bit mode (%Q) */
	case 74 :                               /* Discrete Temporary Table, bit mode (%T) */
	case 76 :                               /* Discrete Internal Table, bit mode (%M) */
	case 78 :                               /* Discrete System A Table, bit mode (%SA) */
	case 80 :                               /* Discrete System B Table, bit mode (%SB) */
	case 82 :                               /* Discrete System C Table, bit mode (%SC) */
	case 84 :                               /* Discrete System D Table, bit mode (%S) */
	case 86 :                               /* Discrete Global Data Table, bit mode (%G, %GA, ...) */

		return ((((ref - 1) % 8) + size + 7) >> 3);           /* size is in bits - adjust for starting offset and round up to number of bytes */
	}

	return (0);
}

/*
 * OffsetCalc - compute offset into memory based on  seg sel and reference
 */

int OffsetCalc(unsigned char seg, int ref)
{
	switch (seg) 
	{

	case 0 :                                /* Local Data Table (%L) */
	case 4 :                                /* Program Data Table (%P) */
	case 8 :                                /* Register Table (%R) */
	case 10 :                               /* Analog Input Table (%AI) */
	case 12 :                               /* Analog Output Table (%AQ) */
	case 70 :                               /* Discrete Input Table, bit mode (%I) */
	case 72 :                               /* Discrete Output Table, bit mode (%Q) */
	case 74 :                               /* Discrete Temporary Table, bit mode (%T) */
	case 76 :                               /* Discrete Internal Table, bit mode (%M) */
	case 78 :                               /* Discrete System A Table, bit mode (%SA) */
	case 80 :                               /* Discrete System B Table, bit mode (%SB) */
	case 82 :                               /* Discrete System C Table, bit mode (%SC) */
	case 84 :                               /* Discrete System D Table, bit mode (%S) */
	case 86 :                               /* Discrete Global Data Table, bit mode (%G, %GA, ...) */

		return (ref - 1);                   /* offset is 1 less than reference number */

	case 16 :                               /* Discrete Input Table, byte mode (%I) */
	case 18 :                               /* Discrete Output Table, byte mode (%Q) */
	case 20 :                               /* Discrete Temporary Table, byte mode (%T) */
	case 22 :                               /* Discrete Internal Table, byte mode (%M) */
	case 24 :                               /* Discrete System A Table, byte mode (%SA) */
	case 26 :                               /* Discrete System B Table, byte mode (%SB) */
	case 28 :                               /* Discrete System C Table, byte mode (%SC) */
	case 30 :                               /* Discrete System D Table, byte mode (%S) */
	case 56 :                               /* Genius Global Data Table, byte mode (%G, %GA, ...) */

		return ((ref - 1) >> 3);            /* offset is a byte offset but ref is a bit offset - div by 8 */
	}

	return (0);
}

int RecvPLC_GE(MYSOCKET sockplc, char *buffer, int count)
{
	int result;
	int error = 0, len = sizeof(int);	
	int i;
	int timeout_s,timeout_ms;
	char log_str[256];
	if(sockplc.sock == INVALID_SOCKET)
	{
		sprintf(log_str,"RecvPLC_GE():error sockplc=%d ",sockplc.sock);	
		log_printf(H9000_LOG_ERROR,1,0,log_str);		
		return FALSE;
	}

	for (i = 0; i < count; i += result) 
	{
		if(sockplc.sock == INVALID_SOCKET)
		{
			sprintf(log_str,"RecvPLC_GE():error sockplc=%d ",sockplc.sock);	
			log_printf(H9000_LOG_ERROR,1,0,log_str);		
			return FALSE;
		}		
		getsockopt(sockplc.sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
		if (error != 0)
		{
			sprintf(log_str,"RecvPLC_GE(): getsock error=%d ",error);	
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return FALSE;		
		}
		result = SOCKET_ERROR;
		timeout_s = SOCK_TIMEOUT_S;
		timeout_ms = SOCK_TIMEOUT_MS;			
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
			sprintf(log_str,"RecvPLC_GE(): getsock after recv error=%d ",error);	
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return FALSE;
		}
		
		if ((result == SOCKET_ERROR) ||( result == 0))
		{
			sprintf(log_str,"RecvPLC_GE():error recv() timeout or result invalid. res=%d \n",result);	
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return (FALSE);
		}
	}
	if (i != count)
	{
		sprintf(log_str,"RecvPLC_GE(): error return FALSE because recvcount!= required\n");
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return (FALSE);
	}
#ifdef PLCDEBUG
	printf("\n\nRecvPLC_GE >>\n");
	for(i=0;i<count; i++)
	{
		printf("%02x=",(unsigned char)buffer[i]);

		if( (i+1)%20 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
#endif 
	return (count);
}



/***************************************************************************************
send data to a given plc through given socket  using TCP/IP¡£
return TRUE when successfully,FALSE otherwise¡£
sockplc:	socket link to plc
count:		size of data to be sent
buffer:		pointer of data  to be sent
****************************************************************************************/
int  SendPLC_GE(MYSOCKET sockplc, char *buffer, int count)
{
#ifdef PLCDEBUG
	int i;
#endif
	int result;
	int error = 0, len = sizeof(int);
	char log_str[256];
	result = SOCKET_ERROR;
	if(sockplc.sock == INVALID_SOCKET)
	{
		sprintf(log_str,"SendPLC_GE():error sockplc=%d ",sockplc.sock);	
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return SOCKET_ERROR;
	}
	if(count <= 0 )
	{
		sprintf(log_str,"SendPLC_GE(): error,count=%d\n",count);
		log_printf(H9000_LOG_ERROR,1,0,log_str);	
		return SOCKET_ERROR;
	}

	getsockopt(sockplc.sock, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
	if (error != 0)
	{
		sprintf(log_str,"SendPLC_GE(): getsock error=%d ",error);	
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return SOCKET_ERROR;
	}

#ifdef PLCDEBUG
	for(i=0;i<count; i++)
	{
		printf("%02x=",(unsigned char)buffer[i]);

		if( (i+1)%20 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
#endif
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
		sprintf(log_str,"SendPLC_GE(): getsock after send error=%d ",error);	
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return SOCKET_ERROR;		
	}
	
	if ( result == SOCKET_ERROR )
	{	
		sprintf(log_str,"SendPLC_GE():error send() timeout ");	
		log_printf(H9000_LOG_ERROR,1,0,log_str);
	}
		
	return result;
}


int ReadPLC_GE(MYSOCKET sockplc, unsigned char seg, int ref, int size, unsigned char *buffer)
{
	int result;
	int j;
	int wordlen;
	int count;
	int bsize;
	int offset;
	unsigned char dummy_send[SEND_SIZE_GE];
	unsigned char dummy_recv[MAX_QUERY_SIZE_GE * 2];
	int           base_addr;
	char log_str[256];

	if( ref<0 || size <=0 || size >= MAX_QUERY_SIZE_GE )
	{
		sprintf(log_str,"ReadPLC_GE(): ref =%d or size =%d invalid\n",ref,size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);		
		return FALSE;
	}
	sprintf(log_str,"==== Run ReadPLC_GE, seg=%d, ref=%d, size=%d \n",seg,ref,size);
	log_printf(H9000_LOG_INFO,2,0,log_str);
	bsize = SizeCalc(seg, ref, size);
	ref = OffsetCalc(seg, ref);
	base_addr = ref;
	count =(bsize/2-1) / ( MAX_QUERY_SIZE_GE ) + 1;
	offset=0;
	for(j=0;j<count;j++)
	{
		memcpy(dummy_send, SysReadReq_GE, sizeof(dummy_send));
		dummy_send[30] = dummy_send[2] = seq_num++;  /* needed to be modified by yyp 07.08.29 */
		/*
		* hack alert : assumed any xfer bsize > 2048 bytes is with a word oriented memory
		*  amoung other things the 1024 magic number above assumes word memory
		*/
		ref=base_addr + j*MAX_QUERY_SIZE_GE;
		if (j != count-1) 
		{
			wordlen = MAX_QUERY_SIZE_GE;
			dummy_send[46] = (unsigned char)MAX_QUERY_SIZE_GE;
			dummy_send[47] = (unsigned char)(MAX_QUERY_SIZE_GE >> 8);
		} 
		else 
		{
			wordlen = bsize/2 -j * MAX_QUERY_SIZE_GE; 
			dummy_send[46] = (unsigned char)wordlen;
			dummy_send[47] = (unsigned char)(wordlen >> 8);
		}
		dummy_send[43] = seg;
		dummy_send[44] = (unsigned char)ref;
		dummy_send[45] = (unsigned char)(ref >> 8);
		
		result = SOCKET_ERROR;
		result = SendPLC_GE(sockplc, (char *)dummy_send, SEND_SIZE_GE);
		if (result == SOCKET_ERROR || result !=SEND_SIZE_GE)
		{
			sprintf(log_str,"error send() timeout or result=%d\n",result);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return (FALSE);
		}
		sprintf(log_str,"==== ReadPLC_GE, sendcount=%d ,id = %d/%d >>\n",result,j+1,count);
		log_printf(H9000_LOG_INFO,2,0,log_str);

		result = RecvPLC_GE(sockplc, (char *)dummy_recv, SEND_SIZE_GE );    
		if (result != SEND_SIZE_GE   )
		{
			sprintf(log_str,"ReadPLC_GE: call RecvPLC_GE() error, res = %d\n",result);
			log_printf(H9000_LOG_ERROR,1,0,log_str);				
			return(FALSE);
		}
		/* check out */
		if(dummy_recv[0] !=3  )
		{
			sprintf(log_str,"ReadPLC_GE: check error, dummy[0]=%d\n",dummy_recv[0]);
			log_printf(H9000_LOG_ERROR,1,0,log_str);				
			return(FALSE);			
		}		
		if (dummy_recv[31] == 0x94) 
		{
			result = RecvPLC_GE(sockplc, (char *)buffer+offset, (((short)dummy_recv[5]) << 8) + dummy_recv[4]);   
			if (result != wordlen *2 ) 
			{
				sprintf(log_str,"ReadPLC_GE: RecvPLC_GE() error, result=%d\n",result);
				log_printf(H9000_LOG_ERROR,1,0,log_str);			
				return (FALSE);
			}
		} 
		else if (dummy_recv[31] == 0xd4) 
		{
			memcpy(buffer+offset, dummy_recv + 44, wordlen *2);
			return (TRUE);
		} 
		else if ((dummy_recv[31] != 0xd1) || (dummy_recv[42] != 7)) 
		{
			sprintf(log_str,"ReadPLC_GE: check error, dummy[31]=%d, dummy[42]=%d \n",
				dummy_recv[31], dummy_recv[42]);
			log_printf(H9000_LOG_ERROR,1,0,log_str);				
			return (FALSE);
		}
		offset=offset+2*wordlen;
	}/* end for */
	return (TRUE);
}

int WritePLC_GE(MYSOCKET sockplc, unsigned char seg, int ref, int size, unsigned char *buffer)
{
	int result;
	int j;
	int wordlen;
	int count;
	int bsize;
	int offset;
	unsigned char dummy_send[SEND_SIZE_GE];
	unsigned char dummy_recv[MAX_QUERY_SIZE_GE * 2];
	int           base_addr;
	char	log_str[256];

	if( ref<0 || size <=0 || size >= MAX_QUERY_SIZE_GE )
	{
		sprintf(log_str,"WritePLC_GE(): ref =%d or size =%d invalid\n",ref,size);
		log_printf(H9000_LOG_ERROR,1,0,log_str);		
		return FALSE;
	}
	sprintf(log_str,"==== Run WritePLC_GE, seg=%d, ref=%d, size=%d \n",seg,ref,size);
	log_printf(H9000_LOG_INFO,2,0,log_str);
	bsize = SizeCalc(seg, ref, size);
	ref = OffsetCalc(seg, ref);
	base_addr = ref;
	count =(size-1) / ( MAX_QUERY_SIZE_GE ) + 1;
	offset=0;
	for(j=0;j<count;j++)
	{
		if (bsize > 8)	/* long write or short write ? */
		{                        
			memcpy(dummy_send, LongWriteReq, sizeof(dummy_send));
			dummy_send[30] = dummy_send[2] = seq_num++;	/* ?? yyp */
			if (j != count-1) 
			{
				wordlen = MAX_QUERY_SIZE_GE *2;
			} 
			else 
			{
				wordlen = bsize/2 -j * MAX_QUERY_SIZE_GE; 
			}
			dummy_send[4] = (unsigned char)(wordlen*2);
			dummy_send[5] = (unsigned char)((wordlen*2) >> 8);
			dummy_send[54] = (unsigned char)wordlen;
			dummy_send[55] = (unsigned char)(wordlen >> 8);			

			dummy_send[51] = seg;
			dummy_send[52] = (unsigned char)(base_addr+offset);
			dummy_send[53] = (unsigned char)((base_addr+offset) >> 8);
			offset += wordlen;
			sprintf(log_str,"==== WritePLC_GE, size =%d id = %d/%d >>\n",SEND_SIZE_GE,j+1,count);
			log_printf(H9000_LOG_INFO,2,0,log_str);			
			result = SendPLC_GE(sockplc, (char *)dummy_send, SEND_SIZE_GE);
			if (result == SOCKET_ERROR || result !=SEND_SIZE_GE)
			{
				sprintf(log_str,"error SendPLC_GE() timeout or result=%d\n",result);
				log_printf(H9000_LOG_ERROR,1,0,log_str);	
				return (FALSE);
			}
			sprintf(log_str,"==== WritePLC_GE, size =%d id = %d/%d >>\n",wordlen*2,j+1,count);
			log_printf(H9000_LOG_INFO,2,0,log_str);			
			result = SendPLC_GE(sockplc, (char *)buffer, wordlen*2);
			if (result == SOCKET_ERROR || result !=wordlen*2)
			{
				sprintf(log_str,"error SendPLC_GE() timeout or result=%d\n",result);
				log_printf(H9000_LOG_ERROR,1,0,log_str);	
				return (FALSE);
			}
			result = RecvPLC_GE(sockplc, (char *)dummy_recv, SEND_SIZE_GE);    
			if (result != SEND_SIZE_GE   )
			{
				sprintf(log_str,"WritePLC_GE: call RecvPLC_GE() error, res = %d\n",result);
				log_printf(H9000_LOG_ERROR,1,0,log_str);				
				return(FALSE);
			}
			if(dummy_recv[0] !=3  )
			{
				sprintf(log_str,"WritePLC_GE: check error, dummy[0]=%d\n",dummy_recv[0]);
				log_printf(H9000_LOG_ERROR,1,0,log_str);				
				return(FALSE);			
			}
			if (dummy_recv[31] == 0xd4) 
			{
				continue;
			} 
			else if ((dummy_recv[31] != 0xd1) || (dummy_recv[42] != 7))
			{   
				sprintf(log_str,"WritePLC_GE: check error, dummy[31]=%d, dummy[42]=%d \n",
					dummy_recv[31], dummy_recv[42]);
				log_printf(H9000_LOG_ERROR,1,0,log_str);				
				return (FALSE);
			}
		} 
		else 
		{                                /* short write */
			wordlen = bsize/2 -j * MAX_QUERY_SIZE_GE; 
			
			memcpy(dummy_send, ShortWriteReq, sizeof(dummy_send));
			dummy_send[30] = dummy_send[2] = seq_num++;
			dummy_send[43] = seg;
			dummy_send[44] = (unsigned char)(base_addr+offset);
			dummy_send[45] = (unsigned char)((base_addr+offset) >> 8);
			dummy_send[46] = (unsigned char)wordlen;
			dummy_send[47] = (unsigned char)(wordlen >> 8);
			//modified by yyp 2011.2.18	
			memcpy(dummy_send + 48, buffer+offset, wordlen*2);
			offset += wordlen;
			result = SendPLC_GE(sockplc, (char *)dummy_send, SEND_SIZE_GE);
			if (result == SOCKET_ERROR || result !=SEND_SIZE_GE)
			{
				sprintf(log_str,"error SendPLC_GE() timeout or result=%d\n",result);
				log_printf(H9000_LOG_ERROR,1,0,log_str);	
				return (FALSE);
			}
			result = RecvPLC_GE(sockplc, (char *)dummy_recv, SEND_SIZE_GE);    
			if (result != SEND_SIZE_GE   )
			{
				sprintf(log_str,"WritePLC_GE: call RecvPLC_GE() error, res = %d\n",result);
				log_printf(H9000_LOG_ERROR,1,0,log_str);				
				return(FALSE);
			}
			if(dummy_recv[0] !=3  )
			{
				sprintf(log_str,"WritePLC_GE: check error, dummy[0]=%d\n",dummy_recv[0]);
				log_printf(H9000_LOG_ERROR,1,0,log_str);				
				return(FALSE);			
			}
			if (dummy_recv[31] == 0xd4) 
			{
				continue;
			} 
			else if ((dummy_recv[31] != 0xd1) || (dummy_recv[42] != 7))
			{   
				sprintf(log_str,"WritePLC_GE: check error, dummy[31]=%d, dummy[42]=%d \n",
					dummy_recv[31], dummy_recv[42]);
				log_printf(H9000_LOG_ERROR,1,0,log_str);				
				return (FALSE);
			}
		}
	}
	return (TRUE);
}

int ConnSession(MYSOCKET sockplc,LCUID plcid)
{
	int result;
	char	log_str[256];

	unsigned char dummy_recv[MAX_QUERY_SIZE_GE * 2];
	result = SendPLC_GE(sockplc, (char *)SRTPconnection, SEND_SIZE_GE);
	if (result == SOCKET_ERROR || result !=SEND_SIZE_GE)
	{
		sprintf(log_str,"error SendPLC_GE() timeout or result=%d\n",result);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		DisconnectPLC(&sockplc,plcid);	
		return (FALSE);
	}
	result = RecvPLC_GE(sockplc, (char *)dummy_recv, SEND_SIZE_GE);    
	if (result != SEND_SIZE_GE )
	{
		sprintf(log_str,"ConnSession: call RecvPLC_GE() error, res = %d\n",result);
		log_printf(H9000_LOG_ERROR,1,0,log_str);				
		DisconnectPLC(&sockplc,plcid);
		return(FALSE);
	}
	if(dummy_recv[0] !=1  )
	{
		sprintf(log_str,"WritePLC_GE: check error, dummy[0]=%d\n",dummy_recv[0]);
		log_printf(H9000_LOG_ERROR,1,0,log_str);				
		return(FALSE);			
	}
	result = SendPLC_GE(sockplc, (char *)EstabSession, sizeof(EstabSession));
	if (result == SOCKET_ERROR || result !=SEND_SIZE_GE)
	{
		sprintf(log_str,"error SendPLC_GE() timeout or result=%d\n",result);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		DisconnectPLC(&sockplc,plcid);	
		return (FALSE);
	}
	result = RecvPLC_GE(sockplc, (char *)dummy_recv, SEND_SIZE_GE);    
	if (result != SEND_SIZE_GE )
	{
		sprintf(log_str,"ConnSession: call RecvPLC_GE() error, res = %d\n",result);
		log_printf(H9000_LOG_ERROR,1,0,log_str);				
		DisconnectPLC(&sockplc,plcid);
		return(FALSE);
	}
	if(dummy_recv[0] !=3  )
	{
		sprintf(log_str,"WritePLC_GE: check error, dummy[0]=%d\n",dummy_recv[0]);
		log_printf(H9000_LOG_ERROR,1,0,log_str);				
		return(FALSE);			
	}	
	return TRUE;
}
