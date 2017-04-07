/**************************************************************************************###

FILENAME:	lan_inb.c

FUNCTION:	
	
	  receive each data of lan broadcasted by UDP, and saved it in lan recv shared memory.
	  if the package id is same with any old package id,the throw away it .

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	start to update the file from H9000 V3.0 to V4.0		yyp

*************************************************************************************###*/

#include "../inc/lanhead_all.h"

struct net_cmd *shm_comm_cmd_addr;
void main(int argc, char *argv[])
{
	short	sock;
	int		i;
	int		mode;//0-simu,1-jk
	wsa_startup();

#ifdef SUN
	signal(SIGINT,lan_finish);
	signal(SIGKILL,lan_finish);
	signal(SIGTERM,lan_finish);
#endif
	if (argc <= 1)
	{
		mode = 0;
		printf("\nlan_inb work in simu mode port = %d",UDP_PORT);
	}
	else if (strcmp(argv[1],"-jk") == 0)
	{
		mode = 1;
		printf("\nlan_inb work in jk mode port = %d",UDP_PORT_JK);
	}
	//CreateLockFile(".lan_inb.dxe");
	if(-1==InitSysLog(H9000_MODULE_LANINB, "LanInb"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	if(MountDB(DB_NAME) == -1)
	{
		printf("==== lan_inb.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}	
	if(lan_init_all()==FALSE)
	{
#ifdef DEBUG
		printf("lan_inb.dxe: call lan_int_all() error\n");
#endif
		exit(1);
	}
	if(get_ots_shm() !=TRUE )
	{
		return FALSE;
	}
	
	if( g_is_island == TRUE )
	{
		shm_recv_addr->island = TRUE;
	}
	else
	{
		shm_recv_addr->island = FALSE;
	}
	/* clean the receive shared memory segment */
	for(i=0;i<MAX_RECV_NUM;i++) 
	{
		shm_recv_addr->shm_data_seg[i].lan_head.length=0;
		shm_recv_addr->shm_data_seg[i].lan_head.dest_id=0;
		shm_recv_addr->shm_data_seg[i].lan_head.src_id=0;
		shm_recv_addr->shm_data_seg[i].lan_head.dp_type=0;
		memset(shm_recv_addr->shm_data_seg[i].data,0,MAX_BUF_SIZE);
	}
	
	/* INITIAL recv SEM */
	if(LinktoSemSet(SEM_RECVDATA)==-1)
	{
		printf("lan_outb.c:  call LinktoSemSet(SEM_RECVDATA) return -1 error\n ");
		exit(-1);
	}
	for(;;)
	{		
		if (mode == 1)	sock=udp_init(UDP_PORT_JK);
		else	sock=udp_init(UDP_PORT);
		
		if(sock!=INVALID_SOCKET)	break;
		
		printf("udp init fault,continue......\n");
		
		MySleep(1000);	 
	}

	printf("\nwaiting for new data to be recept.......\n");
	
	net_recv(sock);
}

