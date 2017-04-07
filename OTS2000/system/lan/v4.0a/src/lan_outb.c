/****************************************************************************************#

FILENAME:	lan_outb.c

FUNCTION:	
	
	  send each seg of the send shared memory to the lan by UDP

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	start to update the file from H9000 V3.0 to V4.0		yyp

***************************************************************************************#*/

#include "../inc/lanhead_all.h"

void main(int argc, char *argv[])
{
	wsa_startup();

#ifdef SUN
	signal(SIGINT,lan_finish);
	signal(SIGKILL,lan_finish);
	signal(SIGTERM,lan_finish);
#endif
	CreateLockFile(".lan_outb.dxe");
	if(-1==InitSysLog(H9000_MODULE_LANOUTB, "LanOutb"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	
	if(MountDB(DB_NAME) == -1)
	{
		printf("==== lan_outb.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}
	if(lan_init_all()==FALSE)
	{
#ifdef DEBUG
		printf("lan_outb.c:  lan_init_all() error \n");
#endif
		exit(1);
	}
	if(get_ots_shm() !=TRUE )
	{
		return FALSE;
	}
	if(-1==LinktoSemSet(SEM_RD_SEND))
	{	
		printf("lan_outb.c:  call LinktoSemSet(SEM_RD_SEND) return -1 error\n ");
		exit(-1);
	}	
	net_send();
	UnMountDB();
}
