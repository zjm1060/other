#include	<time.h>


#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include	"../inc/dps_rec_def.h"
#include	"../inc/dps_type_def.h"
#include	"../inc/ipc_common.h"




void	main()
{
	time_t		time_tt;
	FILE		*fp;
	char		err_buf[256];


	/*      打开数据区信号量      */
	if( LinktoSemSet(SEM_RECVDATA) == -1 )
	{
		Err_log(DPS_MODULE, "\nFail to link data_recv semaphore\n");
		exit(-1);
	}
	
	
	for(;;)
	{
		ReleaseReadSem(SEM_RECVDATA);
		sleep(2);
	}
}
	
	
