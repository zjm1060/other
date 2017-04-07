#include	<time.h>
#include "stdio.h"


#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include	"../../../lan/v4.0a/inc/lan_private.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"



#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma comment(lib, "../../../dms/v4.0a/lib/dblib.lib")





#define	FORWARD_ADDR_DEFINE_FILE	"forward_address.def"  		
#define MAX_FORWARD_ADDR_NUM	4


int			InitIPC(void);


SHM_RECV_DATA	*shm_data_recv_addr;
char	forward_addr_table[MAX_FORWARD_ADDR_NUM][256];
int		forward_addr_num;
SOCKET	send_sock[MAX_FORWARD_ADDR_NUM];


int GetForwardAddr(void)
{
	FILE	*fp;
	char	str[256];
	char	filename[256] = FORWARD_ADDR_DEFINE_FILE;
	 

	chdir(SYS_DEFINE_PATH);
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		SendTraceMsg(0,0, "Fail to open %s", filename);
		return -1;
	}	

	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if(sscanf(str, "%s", forward_addr_table[forward_addr_num])!=1)
			continue;

		if(-1==inet_addr(forward_addr_table[forward_addr_num]))
		{
			SendTraceMsg(0,0,"ip addr illegal: %s", forward_addr_table[forward_addr_num]);
			fclose(fp);
			return -1;
		}
		forward_addr_num++;		
	}

	fclose(fp);
	return 0;
}


SOCKET udp_sock_open(char *ipaddr)
{
	SOCKET		sock;
	int			on;
	SOCKADDR_IN  udp_addr;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) 
	{
		SendTraceMsg(0,0,"Fail to create socket");
		return(INVALID_SOCKET);
	}

	on=1;
	if(setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(char*)&on,sizeof(on)) == -1 )
	{
		SendTraceMsg(0,0,"Fail to set broadcast socket");
		closesocket(sock);
		return(INVALID_SOCKET);
	}    
	
	udp_addr.sin_addr.s_addr=inet_addr(ipaddr);
	udp_addr.sin_family	= AF_INET;
	udp_addr.sin_port	= htons(UDP_PORT);

	if(connect(sock,(struct sockaddr *)&udp_addr,sizeof(struct sockaddr_in))==SOCKET_ERROR) 
	{
		SendTraceMsg(0,0,"connect %s error", ipaddr);
		closesocket(sock);
		return(INVALID_SOCKET);		
	}

	return(sock);
}

int InitSock(void)
{
	int		i;

	for(i=0; i<forward_addr_num; i++)
	{
		if(INVALID_SOCKET==(send_sock[i]=udp_sock_open(forward_addr_table[i])))
			return -1;
	}
	return 0;
}


int InitIPC()
{
	/*      打开数据区共享内存    */
	shm_data_recv_addr= (SHM_RECV_DATA*) LinktoShareMem( SHM_RECV_KEY_CHAR );
	if( shm_data_recv_addr == (SHM_RECV_DATA*)-1 )
	{
		Err_log("Fail to link data_recv shm");
		return -1;
	}
	
	
	/*      打开数据区信号量      */
	if( LinktoSemSet(SEM_RECVDATA) == -1 )
	{
		Err_log("Fail to link data_recv semaphore\n");
		return -1;
	}
	
	return 0;
}



int	main(int argc, char *argv[])
{
	USHORT	data_recv_seg_ptr;
	USHORT	seg_ptr;		
#ifdef	WINDOWS_OS
	WSADATA 	WSAData;
#endif
	

	CreateLockFile(".lan_forward.lock");
	if(-1==InitSysLog(H9000_MODULE_LANFORWARD, "LanForward"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}

#ifdef	WINDOWS_OS
	if(WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
	{
		SendTraceMsg(0,0,"Fail to WSAStartup");
		exit(-1);
	}
#endif


	
	if(-1==InitIPC())
	{
		Err_log("Fail to initialize IPCs!\n");
		exit(-1);
	}

	if(-1==GetForwardAddr())
	{
		Err_log("Fail to get forward addr!\n");
		exit(-1);
	}

	if(forward_addr_num==0)
	{
		Err_log("No forward addr is defined!\n");
		exit(-1);
	}

	if(-1==InitSock())
	{
		Err_log("Fail to init socket!\n");
		exit(-1);
	}

	data_recv_seg_ptr= shm_data_recv_addr->head;
	for( ;; )
	{
		if(WaitforSem(SEM_RECVDATA, -1) == -1)
		{
			Err_log("Fail to wait recv_data semaphore!\n");
			exit(-1);
		}

		while(shm_data_recv_addr->head != data_recv_seg_ptr)
		{
			USHORT data_length;
			int		i;

			seg_ptr= data_recv_seg_ptr++;
			if(data_recv_seg_ptr >= MAX_RECV_DATA_SEG_NUM)
				data_recv_seg_ptr= 0;
			
			memcpy(&data_length, &shm_data_recv_addr->shm_data_seg[seg_ptr].lan_head.length, 2);
			Swap2Byte(&data_length);
			if( data_length==0 || data_length>SHM_DATA_SEG_SIZE)	
				continue;
			
			for(i=0; i<forward_addr_num; i++)
			{
				if(INVALID_SOCKET==send_sock[i])
					send_sock[i]=udp_sock_open(forward_addr_table[i]);

				if(INVALID_SOCKET==send_sock[i])
					continue;

				if(SOCKET_ERROR==send(send_sock[i], (char*)&shm_data_recv_addr->shm_data_seg[seg_ptr], LANH_SIZE+data_length, 0))
				{
					SendTraceMsg(0,0,"Fail to send broadcast data");
					closesocket(send_sock[i]);
					send_sock[i]= INVALID_SOCKET;
				}			
			}
		}
	}
}



