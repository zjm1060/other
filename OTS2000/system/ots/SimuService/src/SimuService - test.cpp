// test1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots.h"
//#include "/home/ems/system/lan/v4.0a/inc/lan_common.h"
//extern SHM_RECORD *shm_rec_addr;

int _tmain(int argc, _TCHAR* argv[])
{
/*	if(MountDB(DB_NAME) == -1)
	{
		printf("==== SimuService.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}

	if(lan_init_all() == FALSE)
	{
		printf("==== SimuService: call Lan_init_all() error\n");		
		exit(0);
	}

	if(get_rec_shm() == FALSE )
	{
		printf("\nhdbwrite.c: call get_rec_shm() error\n");		
		exit(0);
	}
*/
	DMS_COMMON	dbentry;
	SHM_RECORD *shm_rec_addr;
	IND_REC	*pInd_alm;

	short	rcv_end,rcv_pnt;
	/*	获取记录区共享内存地址  */
	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR);
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		printf(" Fail to shmat shm_rec_addr\n");

		exit(0);
	}
	rcv_end = shm_rec_addr->head.header_ptr;
	for(;;)
	{
		while ( rcv_end!=shm_rec_addr->head.header_ptr)
		{
			rcv_pnt = rcv_end;
			rcv_end++;
			if(rcv_end >= MAX_SHM_RECORD_NUM)
				rcv_end = 0;

			pInd_alm = &shm_rec_addr->rec[rcv_pnt].ind_rec;

			printf("%d====%d===%d===%d\n",shm_rec_addr->rec[rcv_pnt].ind_rec.stn_id,shm_rec_addr->rec[rcv_pnt].ind_rec.dev_id,shm_rec_addr->rec[rcv_pnt].ind_rec.data_type,shm_rec_addr->rec[rcv_pnt].ind_rec.pt_id);
			printf("%d====%d===%d===%d\n",pInd_alm->stn_id,pInd_alm->dev_id,pInd_alm->data_type,pInd_alm->pt_id);
		} // while 1
		Sleep(1000);
		printf("************************\n");


	}


	return 0;
}

