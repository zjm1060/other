/************************************************************
*  FILE NAME:           mbs_com.C      						*
*  FUNCTION:            Controlling Station Main Process	*
*						Link Layer Communication Program	*
*  NAME :  					        						*
*  DATE:                DEC,2009                			*
*															*
************************************************************/

#define DEBUG
#include "..\inc\mbm_com.h"

int	SlaveNum;
SLAVE_REC	sSlaveRec[MAX_SLAVE_NUM+1];
MBDATA mbdata[MAX_SLAVE_NUM+1];

void sig_termp();
int CreatChildThread();

int main(int argc, char *argv[])
{
	int i;
	int id;

	CreateLockFile(".mbm_com.dxe");
// 	if(-1==InitSysLog(H9000_MODULE_MBM, "MBM"))
// 	{
// 		Err_log("Fail to InitSysLog!\n");
// 		exit(-1);
// 	}
	
	wsa_startup();


	gethostname(g_hostname,30);
	//Initialization
	if(-1==MountDB(DB_NAME))
	{
		err_write("\nFail to MountDB!");
		exit(-1);
	}
	if(lan_out_init () == FALSE)
	{
		err_write("\nFail to lan_out_init!");
		exit(-1);
	}

	for(i=0;i<=MAX_MASTER_NUM;i++)
	{
		mbdata[i].newsock = INVALID_SOCKET;
		mbdata[i].err_code = 0;
	}
	
	if(ReadMbmStnDef(MBM_STN_DEF_FILE)== FALSE)
	{
		err_write("\nReadMbmStnDef error\n");

		exit(-1);
	}

	sprintf(log_str,"Start to get point def file...\n");	
	log_write(INF,1,log_str);	
	for(id=0;id<SlaveNum;id++)
	{
		sSlaveRec[id].valid = TRUE;
		if(GenAnaSendTable(id)== FALSE)
		{
			sprintf(log_str,"\nGenAnaSendTable (%d) error",id);
			err_write(log_str);
			sSlaveRec[id].valid = FALSE;
			continue;
		}
		if(GenAnaRecvTable(id)== FALSE)
		{
			sprintf(log_str,"\nGenAnaRecvTable (%d) error",id);
			err_write(log_str);
			sSlaveRec[id].valid = FALSE;
			continue;
		}
		if(GenPolSendTable(id)== FALSE)
		{
			sprintf(log_str,"\nGenPolSendTable (%d) error",id);
			err_write(log_str);
			sSlaveRec[id].valid = FALSE;
			continue;
		}
		
		if(GenPolRecvTable(id)== FALSE)
		{
			sprintf(log_str,"\nGenPolRecvTable (%d) error",id);
			err_write(log_str);
			sSlaveRec[id].valid = FALSE;
			continue;
		}
	}
	
	if( CreatChildThread() == FALSE)
	{
		err_write("CreateChileThread() error");
		exit(0);
	}
#ifndef MULTITHREAD
	id = 0;
	ThreadCommM(&id);
#endif

	for(;;)
	{
		MySleep(2000);
	}
	sprintf(log_str,"mbm_com main process exit! \n"); 
	log_write(INF,1,log_str);
#ifdef MSWIN32
	WSACleanup();
#endif	
	UnMountDB();
}

/************************************************ Child Process function of each PLC ***********************************************************************************/
int CreatChildThread()
{
	int  i;
	DWORD thrid;
	HANDLE p_thrid;
	/******* Create Thread for MSWIN32  **********/

#ifndef MULTITHREAD
	MySleep(2000);		
	return TRUE;
#endif
	for( i=0; i<SlaveNum; i++ )
	{
		MySleep(2000);			

		p_thrid=CreateThread(NULL,
				MAX_STACK_SIZE, 
				(LPTHREAD_START_ROUTINE)ThreadCommM, 
		 		&sSlaveRec[i].stnid,
		 		0,
		 		(LPDWORD)&thrid);

		if( p_thrid !=NULL)
		{
			sprintf(log_str,"\nmbm_com.c: Create thread id= %d,thrid=%d OK",
				i,thrid);	
			log_write(INF,1,log_str);
		}
		else
		{
			sprintf(log_str,"mbm_com.c: Create thread id= %d failed", i);
			log_write(INF,1,log_str);
			return FALSE;
		}
	}
			
	return TRUE;
}


void sig_termp()
{
	int i;
	for(i=0;i<SlaveNum;i++)
	{
		if(mbdata[i].newsock != INVALID_SOCKET)
		closesocket(mbdata[i].newsock);
		mbdata[i].newsock = INVALID_SOCKET;
	}
	exit(0);
}

