/************************************************************************************************************************************************
FILENAME:	Dpp.c
FUNCTION:	
		functions of  establish index of dpp_data:Init_DppData()
		functions of  main() of dpp.dxe
		
VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:	2006.04.20		ycx
************************************************************************************************************************************************/



#include "../../../dms/v4.0a/inc/dms_com.h"
#include "../../../dps/v4.0a/inc/dps_rec_def.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../dps/v4.0a/inc/ipc_common.h"
#include "../../../lan/v4.0a/inc/lan_common.h"
#include "../../../plc/v4.0a/inc/lcu_data.h"
#include "../../../dpp/v4.0a/inc/dppdef.h"
#include "../../../dpp/v4.0a/inc/dpp_functions.h"

/************* global variable for dpp *********/

//DPP_DATA 	dpp_data[MAX_PLCDATA_SEG_NUM];


SYS_COMM_TABLE	*shm_syscomm_addr;

/*
unsigned short  dpp_data_id[MAX_STN_NUM][MAX_LCU_NUM];

int Init_DppData()
{

	unsigned char i,j;
	unsigned int lcunum[MAX_STN_NUM];
	short total = 0;
	short curid = 0;
	unsigned int stnnum;

	for(i=1;i<MAX_STN_NUM;i++)
	{
	  lcunum[i]=0;
	}

	if(GetStationNum(&stnnum) == -1)
	{
		printf("Init_DppData(): call GetStationNum error\n");
		return FALSE;
	}

 	for(i=1;i<=stnnum;i++) 
  	{  	
		UCHAR stnId;
		GetStnIdByIndex(i, &stnId);
		if(GetGroupNumById( stnId, &lcunum[i] )==-1)
		{
		   printf("Init_DppData(): call GetGroupNumById error,stnid=%d \n",stnId);
		   return FALSE;
		}
		else
		{
		   total += lcunum[i];	
		}

	}
	if( total >= MAX_PLCDATA_SEG_NUM )
	{
		printf("Init_DppData() failed!,the lcu count %d is bigger than MAX\n", total);
		return FALSE;			
	}
	
 	for(i=1;i<=stnnum;i++) 	
	{
		UCHAR stnId;
		GetStnIdByIndex(i, &stnId);
		for(j=1;j<=lcunum[i];j++ )
		{
		  	curid++;
		  									
			dpp_data_id[i][j] = curid ;
			
			dpp_data[curid].stnid = stnId;
			
			dpp_data[curid].devid = j;			
				  		
		}
	}
	
	if(total == curid)
	{
		  printf("Init_DppData()Successfully!\n");  
		  
		return TRUE;
	}
	else
	{
		printf("Init_DppData()failed!\n");
		return FALSE;		
	}
}

*/

//LCUID		lcu_list[MAX_PLCDATA_SEG_NUM]; /*wzg 070506*/
LCUID_INC_IDX     lcu_list[MAX_PLCDATA_SEG_NUM];//cxs20111205
 
#ifdef SUN_OS     
   
int	main(int argc, char *argv[])
{
 	UCHAR 		i,j,m;
	pthread_t	tid[MAX_PLCDATA_SEG_NUM];
	pthread_t        tid_terminated;

	int		hthreadid[MAX_PLCDATA_SEG_NUM];
	GULONG		stnNum,plcNum,id,id_sum;
	char		groupTypeName[100];
	char		err_buf[256];
	/* LCUID           lcuno[MAX_PLCDATA_SEG_NUM];*/

	float tempreadnum;
	
	CreateLockFile(".dpp.dxe");
	if(-1==InitSysLog(H9000_MODULE_DPP, "DPP"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	
	/* mount share memory*/
	
	if(-1==MountDB(NULL))     {Err_log("dpp.dxe call MountDB() error.......");exit(0);}	
	
	if(lan_out_init() == FALSE)        {Err_log("dpp.dxe  call Lan_out_init() error.......");exit(0);}

	if(get_lcu_shm(0) == FALSE)        {Err_log("dpp.dxe  call get_lcu_shm()  error.......");exit(0);}

	if(InitPlcDataShm(0) == FALSE)     {Err_log("dpp.dxe call InitPlcDataShm() error.......");exit(0);}
	
	shm_syscomm_addr= (SYS_COMM_TABLE*) LinktoShareMem( SHM_SYSCOMM_TABLE_KEY_CHAR );
	
	if( shm_syscomm_addr == (SYS_COMM_TABLE*)-1 )
	{
		Err_log("Fail to link sys_comm shm");
		exit(0);
	}

/* 	if(Init_DppData() == FALSE)        
	{
		Err_log("dpp.dxe call Init_DppData() error.......");
		exit(0);
}*/
	
	//cxs 20110607 start
	/*	QUANTUM_SOE_PT_NUM_IN_MODULE=32;
	SIEMENS_SOE_PT_NUM_IN_MODULE=16;
	GE9030_SOE_PT_NUM_IN_MODULE=32;
	GE9070_SOE_PT_NUM_IN_MODULE=16;
	ABCONTROL_SOE_PT_NUM_IN_MODULE=16;
	*/
	if(-1==GetEmsVariable("QUANTUM_MODSOENUM", &tempreadnum))
		QUANTUM_SOE_PT_NUM_IN_MODULE= 32;
	else
		QUANTUM_SOE_PT_NUM_IN_MODULE=(USHORT)(tempreadnum);
	
	if(-1==GetEmsVariable("SIEMENS_MODSOENUM", &tempreadnum))
		SIEMENS_SOE_PT_NUM_IN_MODULE= 16;
	else
		SIEMENS_SOE_PT_NUM_IN_MODULE=(USHORT)(tempreadnum);
	
	if(-1==GetEmsVariable("GE9030_MODSOENUM", &tempreadnum))
		GE9030_SOE_PT_NUM_IN_MODULE= 32;
	else
		GE9030_SOE_PT_NUM_IN_MODULE=(USHORT)(tempreadnum);
	
	if(-1==GetEmsVariable("GE9070_MODSOENUM", &tempreadnum))
		GE9070_SOE_PT_NUM_IN_MODULE= 16;
	else
		GE9070_SOE_PT_NUM_IN_MODULE=(USHORT)(tempreadnum);
	
	if(-1==GetEmsVariable("ABCONTROL_MODSOENUM", &tempreadnum))
		ABCONTROL_SOE_PT_NUM_IN_MODULE= 16;
	else
		ABCONTROL_SOE_PT_NUM_IN_MODULE=(USHORT)(tempreadnum);
	
	//cxs 20110607 end

	/* read dpp sendata file*/
	//moved to each lcu by wzg 20120312
	//dpp_sendata_flag = ReadDefineFile(&dpp_sendata);
	
	id = 0;

	if(-1==GetStationNum(&stnNum))     {Err_log("dpp.dxe call GetStationNum() error.......");exit(0);}
	
	/* printf("\nstnNum=%d   ",stnNum); */

	for(i=1;i<=stnNum;i++)
	{
		UCHAR stnId;

		if(-1==GetStnIdByIndex(i, &stnId))
			continue;


		if(-1==GetGroupNumById(stnId,&plcNum) )
			continue;
		
		/* printf("plcNum=%d\n",plcNum); */
		
	 	for(j=1;j<=plcNum;j++)
		{
			
			if(-1==GetGroupTypeNameById(stnId,j,groupTypeName)) continue;
			
			/* printf("%s\n",groupTypeName); */

			if(strcmp(groupTypeName,"RTP")==NULL || strcmp(groupTypeName,"SYS")==NULL)  continue;

			
		/*	lcuno[id].stnid = stnId;
			lcuno[id].devid = j; */
		
		/*	wzg 070506*/
           	if(id>=MAX_PLCDATA_SEG_NUM)
			{
				Err_log("Too much lcu over MAX_PLCDATA_SEG_NUM");
				exit(-1);
			}
			
			
			lcu_list[id].lcu_id.stnid = stnId;
			lcu_list[id].lcu_id.devid = j;
			lcu_list[id].stn_idx=i;//cxs
			lcu_list[id].dev_idx=j;//cxs

			hthreadid[id]=pthread_create(&tid[id],NULL,scada,(void *)id);

             
		/*	hthreadid[id]=thr_create(NULL,
										1024*1024*10,
										scada, 
		 								(void *)(&lcuno[id]),
		 								0,
		 								&tid[id]);*/

			if(hthreadid[id]==-1)
			{

 				sprintf(err_buf,"first create scada thread of station %d : lcu %d failed ",stnId,j);
 				Err_log(err_buf);
				exit(-1);
 			}
			else
			{	
				sprintf(err_buf,"first create scada thread of station %d : lcu %d ok ",stnId,j);
				Err_log(err_buf);
			}							
			
			id++;
		}
	}

	id_sum = id;

/*
	while( !thr_join(0, &tid_terminated, NULL) )
	{
	     sleep(1);
			    
	     for(id=0;id<id_sum;id++)
	     {
		
		 / *if(tid_terminated==tid[id])
		 {
			sprintf(err_buf,"scada thread of station %d : lcu %d terminated ",lcuno[id].stnid,lcuno[id].devid);
			Err_log(err_buf);

			if(-1== thr_create(NULL, 1024*1024*10, scada, (void *)(&lcuno[id]), 0, &tid[id]) )	  
			{
			      sprintf(err_buf,"re-create scada thread of station %d : lcu %d failed \n",lcuno[id].stnid,lcuno[id].devid);
	  		      Err_log(err_buf);
	  		      
			      exit(-1);
				  
			}
		 }* /

			/ *wzg 070506* /
			 if(tid_terminated==tid[id])
			 {
				sprintf(err_buf,"scada thread of station %d : lcu %d terminated ",lcu_list[id].stnid,lcu_list[id].devid);
				Err_log(err_buf);

				if(-1== thr_create(NULL, 1024*1024*10, scada, (void *)id, 0, &tid[id]) )	  
				{
					  sprintf(err_buf,"re-create scada thread of station %d : lcu %d failed \n",lcu_list[id].stnid,lcu_list[id].devid);
	  				  Err_log(err_buf);
	  				  
					  exit(-1);
					  
				}
			 }
	     }

	 }                         
*/
	for(;;)
		sleep(1000000);
	
	UnMountDB();

	printf("/n dpp.dxe exit!!!!!!!!!!!!!!!/n");

	exit(0);
}

#endif


#ifdef	WINDOWS_OS
int	main(int argc, char *argv[])
{
 	UCHAR 	i,j;
	GULONG	stnNum,plcNum,id;
	char	groupTypeName[100];
	DWORD	threadID[MAX_PLCDATA_SEG_NUM];
	HANDLE	hthreadid[MAX_PLCDATA_SEG_NUM];
	short	status;
	char	err_buf[256];
	char 		hostname[20];
	unsigned char	stnid,devid;

/*	LCUID   lcuno[MAX_PLCDATA_SEG_NUM];*/

	WORD	wvr,wVersionRequested;
	WSADATA	WSAData,wsaData1;  

	float tempreadnum;

	CreateLockFile(".dpp.dxe");
	if(-1==InitSysLog(H9000_MODULE_DPP, "DPP"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}

	wvr=MAKEWORD(2,2);
 	
 	if ((status = WSAStartup(wvr, &WSAData)) != 0)
	{
		Err_log("### Dpp---main() WSAStartup error!\n");
		return 0;
    }
                                        
	/* mount share memory*/
	
	if(-1==MountDB(NULL))     {Err_log("dpp.dxe call MountDB() error.......");exit(0);}	
	
	if(lan_out_init() == FALSE)        {Err_log("dpp.dxe  call Lan_out_init() error.......");exit(0);}

	if(get_lcu_shm(0) == FALSE)        {Err_log("dpp.dxe  call get_lcu_shm()  error.......");exit(0);}

	if(InitPlcDataShm(0) == FALSE)     {Err_log("dpp.dxe call InitPlcDataShm() error.......");exit(0);}
	
	shm_syscomm_addr= (SYS_COMM_TABLE*) LinktoShareMem( SHM_SYSCOMM_TABLE_KEY_CHAR );
	
	if( shm_syscomm_addr == (SYS_COMM_TABLE*)-1 )
	{
		Err_log("Fail to link sys_comm shm");
		return 0;
	}

	/* 	
	if(Init_DppData() == FALSE)        
	{
		Err_log("dpp.dxe call Init_DppData() error.......");
		exit(0);
	}*/

	//cxs 20110607 start
	/*	QUANTUM_SOE_PT_NUM_IN_MODULE=32;
	SIEMENS_SOE_PT_NUM_IN_MODULE=16;
	GE9030_SOE_PT_NUM_IN_MODULE=32;
	GE9070_SOE_PT_NUM_IN_MODULE=16;
	ABCONTROL_SOE_PT_NUM_IN_MODULE=16;
	*/
	if(-1==GetEmsVariable("QUANTUM_MODSOENUM", &tempreadnum))
		QUANTUM_SOE_PT_NUM_IN_MODULE= 32;
	else
		QUANTUM_SOE_PT_NUM_IN_MODULE=(USHORT)(tempreadnum);
	
	if(-1==GetEmsVariable("SIEMENS_MODSOENUM", &tempreadnum))
		SIEMENS_SOE_PT_NUM_IN_MODULE= 16;
	else
		SIEMENS_SOE_PT_NUM_IN_MODULE=(USHORT)(tempreadnum);
	
	if(-1==GetEmsVariable("GE9030_MODSOENUM", &tempreadnum))
		GE9030_SOE_PT_NUM_IN_MODULE= 32;
	else
		GE9030_SOE_PT_NUM_IN_MODULE=(USHORT)(tempreadnum);
	
	if(-1==GetEmsVariable("GE9070_MODSOENUM", &tempreadnum))
		GE9070_SOE_PT_NUM_IN_MODULE= 16;
	else
		GE9070_SOE_PT_NUM_IN_MODULE=(USHORT)(tempreadnum);
	
	if(-1==GetEmsVariable("ABCONTROL_MODSOENUM", &tempreadnum))
		ABCONTROL_SOE_PT_NUM_IN_MODULE= 16;
	else
		ABCONTROL_SOE_PT_NUM_IN_MODULE=(USHORT)(tempreadnum);
	
	//cxs 20110607 end
	/* read dpp sendata file*/
	//moved to each lcu by wzg 20120312
	//dpp_sendata_flag = ReadDefineFile(&dpp_sendata);
	
	
	if(g_is_island==FALSE)	/* if is not in IPCMODE */
	{
		id = 0;

		if(-1==GetStationNum(&stnNum))     {Err_log("dpp.dxe call GetStationNum() error.......");exit(0);}
	
		/* printf("\nstnNum=%d   ",stnNum); */

		for(i=1;i<=stnNum;i++)
		{
			UCHAR stnId;
			if(-1==GetStnIdByIndex(i, &stnId))
				continue;
		
			if(-1==GetGroupNumById(stnId,&plcNum) )	continue;
		
			/* printf("plcNum=%d\n",plcNum); */
		
			for(j=1;j<=plcNum;j++)
			{
			
				if(-1==GetGroupTypeNameById(stnId,j,groupTypeName)) continue;
			
				/* printf("%s\n",groupTypeName); */

				if(strcmp(groupTypeName,"RTP")==NULL || strcmp(groupTypeName,"SYS")==NULL)  continue;

			/*  lcuno[id].stnid = stnId;
				lcuno[id].devid = j;
		
 				hthreadid[id]=CreateThread(NULL,
									0,
									(LPTHREAD_START_ROUTINE)scada, 
		 							(LPVOID)(&lcuno[id]),
		 							0,
		 							(LPDWORD)&threadID[id]);*/
                       
			if(id>=MAX_PLCDATA_SEG_NUM)
			{
				Err_log("Too much lcu over MAX_PLCDATA_SEG_NUM");
				exit(-1);
			}

			lcu_list[id].lcu_id.stnid = stnId;
			lcu_list[id].lcu_id.devid = j;
			lcu_list[id].stn_idx=i;//cxs
			lcu_list[id].dev_idx=j;//cxs

		
 			hthreadid[id]=CreateThread(NULL,
									0,
									(LPTHREAD_START_ROUTINE)scada, 
		 							(LPVOID)id,
		 							0,
		 							(LPDWORD)&threadID[id]);

				if(hthreadid[id]==NULL)
				{

 					sprintf(err_buf,"first create scada thread of station %d : lcu %d failed ",stnId,j);
 					Err_log(err_buf);
 				}
				else
				{	
					sprintf(err_buf,"first create scada thread of station %d : lcu %d ok ",stnId,j);
					Err_log(err_buf);
				}							
			
				id++;
			}
		}
	}
	else		/* if is in IPCMODE */
	{
		gethostname(hostname,20);
		#ifdef DEBUG
		printf("hostname =%s\n",hostname);
		#endif

		if(GetIPCConf()==0)	
		{
		printf("\n==== dpp.exe: GetIPCConf() error!\n");
		exit(0);
		}
		else
		{
		printf("\n==== GetIPCConf() successfully!\n");
		}

		if(GetIdByHostname(hostname,&stnid,&devid)==0)	
		{
			printf("\n==== dpp.exe: GetIdByHostname(%s) error!\n",hostname);
			exit(0);
		}
		else
		{
			printf("\n==== GetIdByHostname successfully, stnid=%d,devid=%d!\n",stnid,devid);
		}

		id=0;

	/*	lcuno[id].stnid = stnid;
		lcuno[id].devid = devid;

 		hthreadid[id]=CreateThread(NULL,
								0,
								(LPTHREAD_START_ROUTINE)scada, 
		 						(LPVOID)(&lcuno[id]),
		 						0,
		 						(LPDWORD)&threadID[id]);
                   

		if(hthreadid[id]==NULL)
		{

 			sprintf(err_buf,"first create scada thread of station %d : lcu %d error (err_num=%d )",lcuno[id].stnid,lcuno[id].devid,GetLastError ());
 			Err_log(err_buf);
 		}
		else
		{	
			sprintf(err_buf,"first create scada thread of station %d : lcu %d ok ",lcuno[id].stnid,lcuno[id].devid);
			Err_log(err_buf);
		}
	*/

		lcu_list[id].lcu_id.stnid = stnid;
		lcu_list[id].lcu_id.devid = devid;

 		hthreadid[id]=CreateThread(NULL,
								0,
								(LPTHREAD_START_ROUTINE)scada, 
		 						(LPVOID)id,
		 						0,
		 						(LPDWORD)&threadID[id]);
                   

		if(hthreadid[id]==NULL)
		{

 			sprintf(err_buf,"first create scada thread of station %d : lcu %d error (err_num=%d )",lcu_list[id].lcu_id.stnid,lcu_list[id].lcu_id.devid,GetLastError ());
 			Err_log(err_buf);
 		}
		else
		{	
			sprintf(err_buf,"first create scada thread of station %d : lcu %d ok ",lcu_list[id].lcu_id.stnid,lcu_list[id].lcu_id.devid);
			Err_log(err_buf);
		}

	}


	for(;;) MySleep(1000);
	
	UnMountDB();

	printf("\n dpp.dxe exit!!!!!!!!!!!!!!!\n");

	exit(0);	

}
#endif
