/*******************************************************************************************************
FILENAME:	scanapp.c
FUNCTION:	
	
	  the source code file of plc communication module,whether the plc type
	is "GE" or "QUANTUM".include:
	(1) int main();
	(2) int ThreadScan(void *arg);
	(3) int GetHead(MYSOCKET sockplc, LCUID plcid)
	(4) int GetANA(MYSOCKET sockplc, LCUID plcid)
	(5) int GetIMP(MYSOCKET sockplc, LCUID plcid)
	(6) int GetSysMsg(MYSOCKET sockplc, LCUID plcid)
	(7) int GetPOL(MYSOCKET sockplc, LCUID plcid)
	(8) int GetINTAlm(MYSOCKET sockplc, LCUID plcid)
	(9) int WriteData(MYSOCKET sockplc, LCUID plcid)

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, Unix

HISTORY:
 2005.11.1	start to modify the code for H9000 V4.0		yyp
 2008.4.29	add AB plc code
********************************************************************************************************/
#include "../inc/plchead_all.h"

/********************************************************************************************************
 Read original head data from plc  
*********************************************************************************************************/

void * ThreadScanH( void * arg )
{
	MYSOCKET	sockplc;			
	LCUID		plcid;
	short		err_count=0;			/* error count of reading data function */
	short		loop_count=0;			/* for loop count */
	int			scanqlt_count;
	short		ret;					/* return value of  */
	unsigned char	stnid,idx,devid,tmphotnet,tmphotcpu;
	unsigned char	threadtype;
	int		waitms;
	int             FirstScanPrvgErr=1,FirstLinkStatErr=1,FirstHeadStatErr=1;
	int			FirstRun=1;
	short		OldScanPrvg,CurScanPrvg,OldLinkStat,CurLinkStat, OldHeadStat,CurHeadStat;	/* PLCScanPrvg of last loop,	current loop */
	int			err;
	char	log_str[256];
	scanqlt_count = 0;
	sockplc.sock = INVALID_SOCKET;
	MySleep(1000);
	plcid.stnid = ((THRARG *) arg)->stnid ;
	plcid.devid = ((THRARG *) arg)->devid ;	
	stnid=plcid.stnid;
	devid=plcid.devid; 
	idx = index_stn[stnid];
	threadtype=((THRARG *) arg)->thrtype;
	sprintf(log_str,"==== scanapp.c:  Start ThreadScanH Function plcid=%d.%d!\n",stnid,devid);
	log_printf(H9000_LOG_INFO,2,0,log_str);
	if(threadtype != THR_HEAD)
	{
		sprintf(log_str,"scanapp.c: in threadscanH,thrtype %d != THR_HEAD,error\n",threadtype);
		return NULL;
	}
	runvar[idx][devid].hotcpu=rdb_cpu(stnid,devid);
	runvar[idx][devid].hotnet=rdb_net(stnid,devid);
	
	runvar[idx][devid].cpua_state = CPUERR;
	runvar[idx][devid].cpub_state = CPUERR;
	runvar[idx][devid].FirstHead = YES;
	runvar[idx][devid].FirstSend = YES;
	runvar[idx][devid].FirstIntAlm = YES;
	runvar[idx][devid].cpu_flag_count =0;
	runvar[idx][devid].send_update_flag =0;
	runvar[idx][devid].cpu_flag =MAX_PLC_FLAG;
	shm_plc_data->seg[ segid[idx][devid] ].endpp=0;
	shm_plc_data->seg[ segid[idx][devid] ].ana_update_flag=0;	
	shm_plc_data->seg[ segid[idx][devid] ].rtd_update_flag=0;
	shm_plc_data->seg[ segid[idx][devid] ].obj_update_flag=0;	
	shm_plc_data->seg[ segid[idx][devid] ].imp_update_flag=0;				
	shm_plc_data->seg[ segid[idx][devid] ].sysmsg_update_flag=0;
	shm_plc_data->seg[ segid[idx][devid] ].head_thr_valid=NO; 
 	/*
 	BdPlcScanStat(plcid,SCANSTOP);
 	*/
	CurScanPrvg=CUR_WS_IN_HOST;
	OldLinkStat=LCU_ON;
	CurHeadStat=NO;
	/******* loop always *******/

	sprintf(log_str,"==== stn=%d,dev=%d thr =%d: loop to read from plc ...\n",stnid,devid,threadtype);
	log_printf(H9000_LOG_INFO,2,0,log_str);
	
	for(;;)
	{
		if(plconf[idx][devid].plctype==SIEPLC)
		{
			waitms=500;
		}
		else if(plconf[idx][devid].plctype==GEPLC)
		{
			waitms=500;
		}		
		else
		{
			waitms=100;		
		}
		MySleep(waitms);
#ifdef PLCDEBUG
		MySleep(5000);		
#endif		
		OldScanPrvg=CurScanPrvg;
		CurScanPrvg=GetPLCScanPrvg(plcid,&err);
		if( CurScanPrvg!=CUR_WS_IN_HOST )
		{
			if( OldScanPrvg==CUR_WS_IN_HOST )
			{
				shm_plc_data->seg[ segid[idx][devid] ].endpp=0;
				shm_plc_data->seg[ segid[idx][devid] ].head_thr_valid = NO; 
				/* BdPlcScanStat(plcid,SCANSTOP); */
				sprintf(log_str,"==== %s is not the plc %d.%d primary scan server, Prvg = %d,oldprvg=%d\n",
					hostname,stnid,devid,CurScanPrvg,OldScanPrvg);
				err_write(log_str);
				/* added by yyp 07.08.10 */
				if(err==1)
				{
					sprintf(log_str,"==== %s is not in the plc %d.%d scan server list\n", hostname,stnid,devid);
				}
				else if(err == 2)
				{
					sprintf(log_str,"==== one host in the plc %d.%d scan server list before %s is running \n",
						stnid,devid,hostname);	
				}
				else if(err == 3)
				{
					sprintf(log_str,"==== the net or host state of  %s is error \n",hostname);	
				}
				err_write(log_str);
				//2012.3.26 根据wzg提出的，在主采切换时，可能导致dpp广播老数据的情况，增加如下代码
				if(sockplc.sock!=INVALID_SOCKET)
				{
					DisconnectPLC(&sockplc,plcid);
				}
			}
			MySleep(500);
			continue;	
		}
		else
		{
			shm_plc_data->seg[ segid[idx][devid] ].endpp=1;			
			BdScanHost(plcid);
		}
		BdScanQlt(plcid, &scanqlt_count);			/* added by yyp 2008.12.22 */
		/******* read the plc link stat ,if unlink, then next loop *******/
		CurLinkStat=GetDevONOFFStat(plcid);
		if(CurLinkStat == LCU_OFF)
		{
			if( OldLinkStat != LCU_OFF )
			{				
				shm_plc_data->seg[ segid[idx][devid] ].head_thr_valid = NO; 
				BdPlcScanStat(plcid,SCANSTOP);
				sprintf(log_str,"==== Current Link state of plc %d.%d is offline \n",stnid,devid);
				log_printf(H9000_LOG_ERROR,1,0,log_str);	
			}
			OldLinkStat = LCU_OFF; 	
			MySleep(200);
			continue ;
		}
		
		tmphotnet=rdb_net(stnid,devid);
		tmphotcpu=rdb_cpu(stnid,devid);
		/******* if current net/cpu is not equal to last hotnet /hotcpu *******/
		if( runvar[idx][devid].hotnet !=tmphotnet || runvar[idx][devid].hotcpu !=tmphotcpu )
		{
			if(sockplc.sock!=INVALID_SOCKET)
			{
				sprintf(log_str,"==== scanapp.c: plc %d.%d hotnet or hotcpu error\n",stnid,devid);	
				log_printf(H9000_LOG_ERROR,1,0,log_str);
				DisconnectPLC(&sockplc,plcid);
			}
			if( plconf[idx][devid].plctype != ABPLC )
			{
				runvar[idx][devid].hotnet = tmphotnet;
				runvar[idx][devid].hotcpu = tmphotcpu;				
			}
			shm_plc_data->seg[ segid[idx][devid] ].head_thr_valid = NO; 
			BdPlcScanStat(plcid,SCANSTOP);
			MySleep(200);
		}
						
		/******* connect plc while not connected *******/
		if(sockplc.sock == INVALID_SOCKET)
		{
			sockplc=ConnectPLC(plcid, threadtype);
			if(sockplc.sock==INVALID_SOCKET)
			{
				//modified by yyp 2011.06.27 for siemens plc
				//MySleep(200);
				MySleep(2000);
				BdPlcLinkStat(plcid,LCU_ERR);
				continue;
			}
		}

		/******* call different functions according to the threadtype *******/
		switch(threadtype)
		{
		case THR_HEAD:
			ret=GetHead(sockplc,plcid);
			break;	
		default:
			break;
		}
 		if(ret==YES)
		{
			/***** broadcast link state and set firstscan,only in GetHead thread *****/
			BdPlcLinkStat(plcid,LCU_ON);
			OldHeadStat=CurHeadStat;
			CurHeadStat=GetHeadStat(shm_plc_data->seg[ segid[idx][devid] ].head,stnid,devid);
			if( CurHeadStat != YES )
			{
				/*
				if(OldHeadStat == YES)
				{
				*/
					if(sockplc.sock!=INVALID_SOCKET)
					{
						DisconnectPLC(&sockplc,plcid);
					}
					shm_plc_data->seg[ segid[idx][devid] ].head_thr_valid = NO; 
					BdPlcScanStat(plcid,SCANSTOP);
					sprintf(log_str,"==== scanapp.c: plc %d.%d HeadStat invalid\n",stnid,devid);	
					log_printf(H9000_LOG_ERROR,1,0,log_str);
				/*
				}
				*/
				MySleep(5000);	/* modified by yyp 2008.11.26 */
				continue;				
			}
			err_count=0;
			loop_count++;
			if(loop_count>=200)
			{
				loop_count=0;
			}
			//added by yyp 2011.6.16
			if(runvar[idx][devid].cpu_flag == shm_plc_data->seg[ segid[idx][devid] ].head.cpuflag)
			{
				runvar[idx][devid].cpu_flag_count++;
				if(runvar[idx][devid].cpu_flag_count >= 5) 
				{
					sprintf(log_str,"==== err:  plc %d.%d cpu_flag_count =%d reach max: cpu stop or app stop ",
						stnid,devid,runvar[idx][devid].cpu_flag_count);
					log_printf(H9000_LOG_ERROR,1,0,log_str);
					if(sockplc.sock!=INVALID_SOCKET)
					{
						DisconnectPLC(&sockplc,plcid);
					}
					shm_plc_data->seg[ segid[idx][devid] ].head_thr_valid = NO; 
					BdPlcScanStat(plcid,SCANSTOP);
					runvar[idx][devid].cpu_flag_count = 0;
					//added by yyp 2009.1.13
					if(plconf[idx][devid].isdualcpu==YES)
					{
						if(runvar[idx][devid].hotcpu==0)
						{
							runvar[idx][devid].hotcpu=1;
							runvar[idx][devid].hotnet=1;
						}
						else
						{
							runvar[idx][devid].hotcpu=0;
							runvar[idx][devid].hotnet=0;
						}
						BdNetHost(plcid,runvar[idx][devid].hotnet );
						BdCpuHost(plcid, runvar[idx][devid].hotcpu);		
					}
				}
				continue;
			}
			else
			{
				runvar[idx][devid].cpu_flag = shm_plc_data->seg[ segid[idx][devid] ].head.cpuflag;
				runvar[idx][devid].cpu_flag_count = 0;
				//BdPlcCpuStat(plcid,runvar[idx][devid].hotcpu,CPURUN);
				//for abplc, if hotnet != hotcpu, means the scada read data by route
				if( plconf[idx][devid].plctype == ABPLC && plconf[idx][devid].cpu_net_mode == CPU_D_NET_S )
				{
					if(runvar[idx][devid].hotcpu != runvar[idx][devid].hotnet)
					{
						if(sockplc.sock!=INVALID_SOCKET)
						{
							DisconnectPLC(&sockplc,plcid);
						}
					}
				}
			}
			if(CurHeadStat == YES)
			{
				BdPlcScanStat(plcid,SCANRUN);	
			}
			if(OldScanPrvg!=CUR_WS_IN_HOST || OldLinkStat !=LCU_ON || OldHeadStat != YES 
				|| shm_plc_data->seg[ segid[idx][devid] ].head_thr_valid !=YES )
			{
				if(GetHeadStat(shm_plc_data->seg[ segid[idx][devid] ].head, stnid,devid) == YES )
				{
					//if(OldScanPrvg!=CUR_WS_IN_HOST || runvar[idx][devid].FirstHead==YES)
					{
						/* modified by yyp 07.07.05 */
						shm_plc_data->seg[ segid[idx][devid] ].firstscan=1;
						runvar[idx][devid].objalm_ptr_end = shm_plc_data->seg[ segid[idx][devid] ].head.objalm_ptr;
						runvar[idx][devid].soealm_ptr_end = shm_plc_data->seg[ segid[idx][devid] ].head.soealm_ptr;
						shm_plc_data->seg[ segid[idx][devid] ].soealm_ptr = runvar[idx][devid].soealm_ptr_end;
						shm_plc_data->seg[ segid[idx][devid] ].objalm_ptr = runvar[idx][devid].objalm_ptr_end;						
					}					
					shm_plc_data->seg[ segid[idx][devid] ].head_thr_valid=YES; 
					shm_plc_data->seg[ segid[idx][devid] ].endpp=1;

			  		sprintf(log_str,"==== scanapp: plc %d.%d set firstscan ,firstscan=%d,endpp=%d,head_valid=%d\n",
			  			stnid,devid, shm_plc_data->seg[ segid[idx][devid] ].firstscan,
			  			shm_plc_data->seg[ segid[idx][devid] ].endpp,
			  			shm_plc_data->seg[ segid[idx][devid] ].head_thr_valid );
					log_printf(H9000_LOG_INFO,1,0,log_str);
				}
				else
				{
					sprintf(log_str,"==== scanapp.c: plc %d.%d GetHeadStat error\n",stnid,devid);	
					log_printf(H9000_LOG_ERROR,1,0,log_str);					
				}
				OldLinkStat =LCU_ON; 
			}
			if(g_ctrl_dualnet == YES )
			{
				BdNetHost(plcid,runvar[idx][devid].hotnet );
			}
			//modified by  yyp 09.03.10
			if(plconf[idx][devid].isdualcpu == YES)
			{
				BdCpuHost(plcid, runvar[idx][devid].hotcpu);
			}
			runvar[idx][devid].FirstHead=NO;				
		}
		else if(ret==NO)
		{
			err_count++;
			runvar[idx][devid].cpu_flag_count = 0;
			DisconnectPLC(&sockplc,plcid);
			MySleep(50);
			if(err_count>=MAXRETRY_HEAD)
			{
				shm_plc_data->seg[ segid[idx][devid] ].head_thr_valid=NO; 
				BdPlcScanStat(plcid,SCANSTOP);
				OldLinkStat =LCU_ERR; 
			  	sprintf(log_str,"==== scanapp.c: stnid=%d,devid=%d set endpp=0 ret = NO \n",stnid,devid);	
				log_printf(H9000_LOG_INFO,1,0,log_str);	
				BdPlcLinkStat(plcid,LCU_ERR);
				if(runvar[idx][devid].hotnet != 0)
				{
					tmphotnet = 0;
				}
				else
				{
					tmphotnet = 1;
				}
				BdNetHost(plcid, tmphotnet );
				err_count=0;
			}
			continue;
		}
		else
		{
			continue;
		}
#ifndef MULTITHREAD		
		if(ret == YES)
		{
			ret = ret && GetANA(sockplc,plcid);
			ret = ret && GetIMP(sockplc,plcid);
			ret = ret && GetSysMsg(sockplc,plcid);
			ret = ret && GetPOL(sockplc,plcid);
			ret = ret && GetINTAlm(sockplc,plcid);
			ret = ret && WriteData(sockplc,plcid); 
			if(ret == YES)
			{
				if(runvar[idx][devid].FirstIntAlm != 0)
				{
					runvar[idx][devid].FirstIntAlm = 0;
				}
			}
			else
			{
				DisconnectPLC(&sockplc,plcid);
			}
		}
#endif

	}/******* end or loop *******/
	return NULL;
}



void * ThreadScan( void * arg )
{
	MYSOCKET	sockplc;			
	LCUID		plcid;
	short		err_count=0;			/* error count of reading data function */
	short		loop_count=0;			/* for loop count */
	short		slavecount=0;
	short		ret;				/* return value of  */
	int			err;
	unsigned char	stnid,idx,devid;
	unsigned char	threadtype;
	int		waitms;
	short		CurScanPrvg,CurLinkStat, OldHeadStat,CurHeadStat;	/* PLCScanPrvg of last loop,	current loop */
	char	log_str[256];
	sockplc.sock = INVALID_SOCKET;
	plcid.stnid = ((THRARG *) arg)->stnid ;
	plcid.devid = ((THRARG *) arg)->devid ;	
	stnid=plcid.stnid;
	devid=plcid.devid; 
	idx = index_stn[stnid];
	threadtype=((THRARG *) arg)->thrtype;
	MySleep(5000);
	sprintf(log_str,"====scanapp.c: plc %d.%d Start ThreadScan Function!\n",stnid,devid);	
	log_printf(H9000_LOG_INFO,3,0,log_str);	
	CurScanPrvg=CUR_WS_IN_VICE;	
	CurHeadStat=YES;
	/******* loop always *******/
	sprintf(log_str,"==== scanapp.c stn=%d,dev=%d thr =%d: loop to read from plc ...\n",stnid,devid,threadtype);
	log_printf(H9000_LOG_INFO,2,0,log_str);	
	for(;;)
	{
		if(plconf[idx][devid].plctype==SIEPLC)
		{
			waitms=500;
		}
		else if(plconf[idx][devid].plctype==GEPLC)
		{
			waitms=500;
		}		
		else
		{
			waitms=100;
		}
		MySleep(waitms);

		OldHeadStat=CurHeadStat;		
		CurHeadStat = shm_plc_data->seg[ segid[idx][devid] ].head_thr_valid;
		
		if( CurHeadStat != YES)
		{	
			if(OldHeadStat == YES)
			{
				sprintf(log_str,"==== stn=%d,dev=%d thr =%d: head_thr is %d invalid ...",
					stnid,devid,threadtype,
					shm_plc_data->seg[ segid[idx][devid] ].head_thr_valid);
				log_printf(H9000_LOG_ERROR,2,0,log_str);
				DisconnectPLC(&sockplc,plcid);				
			}	
			MySleep(500);
			continue;
		}
				
		CurScanPrvg=GetPLCScanPrvg(plcid,&err);
		if( CurScanPrvg!=CUR_WS_IN_HOST )
		{
			MySleep(200);
			continue;
		}	

		/******* read the plc link stat ,if unlink, then next loop *******/
		CurLinkStat=GetDevONOFFStat(plcid);
		if(CurLinkStat == LCU_OFF)
		{
			MySleep(200);
			continue;
		}
           
		/******* if current net/cpu is not equal to last hotnet /hotcpu *******/
		/*
		if(hot_net[threadtype] != runvar[idx][devid].hotnet  
			 || hot_cpu[threadtype] != runvar[idx][devid].hotcpu  )
		{
			DisconnectPLC(&sockplc,plcid);
			sprintf(log_str,"==== plc %d.%d thr =%d error: thrnet=%d, curnet=%d,thrcpu=%d,curcpu=%d ..."
				,stnid,devid,threadtype,hot_net[threadtype],runvar[idx][devid].hotnet,
				hot_cpu[threadtype],runvar[idx][devid].hotcpu );
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			hot_net[threadtype] = runvar[idx][devid].hotnet;
			hot_cpu[threadtype] = runvar[idx][devid].hotcpu;
			MySleep(200);
			continue;
		}
		*/
		/******* connect plc while not connected *******/
		if(sockplc.sock == INVALID_SOCKET)
		{
			sockplc=ConnectPLCChild(plcid, threadtype);
			if(sockplc.sock == INVALID_SOCKET)
			{
				MySleep(200);
				BdPlcLinkStat(plcid,LCU_ERR);
				continue;
			}
		}

		/******* call different functions according to the threadtype *******/
		switch(threadtype)
		{
		case THR_ANA:
			ret=GetANA(sockplc,plcid);
			break;
		case THR_IMP:
			ret=GetIMP(sockplc,plcid);
			break;
		case THR_SYSMSG:
			ret=GetSysMsg(sockplc,plcid);
			break;
		case THR_POL:
			ret=GetPOL(sockplc,plcid);
			break;
		case THR_INTALM:
			ret=GetINTAlm(sockplc,plcid);
			break;
		case THR_WRITEDATA:
			ret=WriteData(sockplc,plcid);
			break;
		default:
			continue;
			break;
		}
		if(ret==TRUE)
		{
			if(threadtype==THR_INTALM)
			{
				runvar[idx][devid].FirstIntAlm = 0;
			}
			err_count=0;
			loop_count++;
			if(loop_count>=200)
			{
				loop_count=0;
			}
		}
		else if(ret==FALSE)
		{
			DisconnectPLC(&sockplc,plcid);
			err_count++;
			if(err_count>=MAXRETRY_CHILD)
			{
				err_count=0;
			}
			MySleep(200);			
			continue;
		}
		else
		{
			MySleep(200);
			continue;
		}
	}/******* end or loop *******/
	return NULL;
}


/********************************************************************************************************
 read plc head data
*********************************************************************************************************/

int GetHead(MYSOCKET sockplc,LCUID plcid)
{
	int 		seg;
	int 		ref;
	int 		result;
	int  		scanlen;
	struct plc_data_seg_head head;
	unsigned	char databuf[MAX_SCAN_BUF*2];  
	unsigned char	stnid,idx,devid;
	char	log_str[256];
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx = index_stn[stnid];
	if (sockplc.sock==INVALID_SOCKET)   return FALSE;

	/***************** Read Head Data From PLC **************************************************/
	memset(databuf,0,MAX_SCAN_BUF*2);


	if(plconf[idx][devid].plctype==SIEPLC)
	{	
		seg = HEADDB_NO;
		ref = HEADBASE_SIE/2;
	}		
	else if(plconf[idx][devid].plctype==MBPLC
		|| plconf[idx][devid].plctype==PREPLC)
	{
		seg=REGSEG;
		ref=HEADBASE_MB;
	}
	else if(plconf[idx][devid].plctype==GEPLC)
	{
		seg=REGSEG;
		ref=HEADBASE_GE;
	}
	else if(plconf[idx][devid].plctype==ABPLC)
	{
		seg=HEADTAG;
		ref=HEADBASE_AB;
	}
	else
	{
		sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,plcid.devid,plconf[idx][devid].plctype);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return(FALSE);
	}

	scanlen=(PLCHEADLEN+1)/2;
	result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf,plcid); 
	if(result==FALSE)
	{
		sprintf(log_str,"==== scanapp.c: plc %d.%d GetHead(): ReadPLC error \n",stnid,devid);
		log_printf(H9000_LOG_ERROR,1,0,log_str);	
		return(FALSE);
	}
	else
	{
		sprintf(log_str,"==== scanapp.c: plc %d.%d GetHead(): ReadPLC success. \n",stnid,devid);
		log_printf(H9000_LOG_INFO,2,0,log_str);			
	}
	/*****************decode original ana data **************************************************/
	memcpy(&head,databuf,2*scanlen);
#ifdef _BIG_ENDIAN_
	Swap2Head( &head );
#endif
	memcpy(&(shm_plc_data->seg[ segid[idx][devid] ].head),&head,2*scanlen);
	return TRUE;
}

/********************************************************************************************************
 read plc analog data
*********************************************************************************************************/

int GetANA(MYSOCKET sockplc,LCUID plcid)
{
	int 		seg;
	int 		ref;
	int 		result;
	int  		scanlen;
	int 		offlen=0;
	char	log_str[256];
	struct plc_data_seg_head head;
	unsigned	char databuf[MAX_SCAN_BUF*2];  
	unsigned char	stnid,idx,devid;
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx = index_stn[stnid];
	if (sockplc.sock==INVALID_SOCKET)   return FALSE;

	/***************** get shm data head **************************************************/
	memcpy(&head,&(shm_plc_data->seg[ segid[idx][devid] ].head),PLCHEADLEN);
	/***************** Read Ana Data From PLC **************************************************/
	memset(databuf,0,MAX_SCAN_BUF*2);
	seg=REGSEG;	
	ref = head.ai_start_addr;		/* yyp 2008.4.14 modified */
	if(plconf[idx][devid].plctype==SIEPLC)
	{	
		seg = ANADB_NO;
		ref = ref/2;
	}

	scanlen=0;
	scanlen+=head.ai_size;
	scanlen+=2*Mod16(head.ai_size);	
	scanlen+=head.ao_size;
	scanlen+=2*Mod16(head.ao_size);	
	scanlen+=head.aic_size;
	scanlen+=head.aicomm_size;
	scanlen+=2*Mod16(head.aicomm_size);	
	if( scanlen == 0 )
	{
		return TRUE;
	}
	result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
	if(result==FALSE)
	{
		sprintf(log_str,"==== scanapp.c: plc %d.%d GetANA(): ReadPLC error \n",stnid,devid);
		log_printf(H9000_LOG_ERROR,1,0,log_str);	
		return(FALSE);
	}
	else
	{
                
		sprintf(log_str,"==== scanapp.c: plc %d.%d GetANA(): ReadPLC success. \n",stnid,devid);
		log_printf(H9000_LOG_INFO,2,0,log_str);			
	}	
	/*****************decode original ana data **************************************************/
	offlen=0;
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].ai_data,databuf+offlen,2*head.ai_size);
        
	offlen+=2*head.ai_size;
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].ai_qlt,databuf+offlen,2*Mod16(head.ai_size));
	offlen+=2*Mod16(head.ai_size);
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].ai_en,databuf+offlen,2*Mod16(head.ai_size));	
	offlen+=2*Mod16(head.ai_size);		

	/*****************decode original aout data **************************************************/
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].ao_data,databuf+offlen,2*head.ao_size);
	offlen+=2*head.ao_size;
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].ao_qlt,databuf+offlen,2*Mod16(head.ao_size));
	offlen+=2*Mod16(head.ao_size);
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].ao_en,databuf+offlen,2*Mod16(head.ao_size));	
	offlen+=2*Mod16(head.ao_size);	

	/*****************decode original ana calculate data ****************************************/
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].aic_data,databuf+offlen,2*head.aic_size);
	offlen+=2*head.aic_size;
	
	/*****************decode original ana communication data ************************************/
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].aicomm_data,databuf+offlen,2*head.aicomm_size);
	offlen+=2*head.aicomm_size;
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].aicomm_qlt,databuf+offlen,2*Mod16(head.aicomm_size));
	offlen+=2*Mod16(head.aicomm_size);
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].aicomm_en,databuf+offlen,2*Mod16(head.aicomm_size));	
	offlen+=2*Mod16(head.aicomm_size);	
	
	shm_plc_data->seg[ segid[idx][devid] ].ana_update_flag++;
	if(shm_plc_data->seg[ segid[idx][devid] ].ana_update_flag>=MAX_FLAG_NUM)
	{
		shm_plc_data->seg[ segid[idx][devid] ].ana_update_flag=0;
	}
	return TRUE;
}

/********************************************************************************************************
 read plc IMP RTD PARAM data
*********************************************************************************************************/

int GetIMP(MYSOCKET sockplc,LCUID plcid)
{
	int 		seg;
	int 		ref;
	int 		result;
	int  		scanlen;
	int 		offlen=0;
	struct plc_data_seg_head head;
	unsigned	char databuf[MAX_SCAN_BUF*2];  
	unsigned char	stnid,idx,devid;
	char	log_str[256];
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx = index_stn[stnid];
	if (sockplc.sock==INVALID_SOCKET)   return FALSE;

	/***************** get shm data head **************************************************/
	memcpy(&head,&(shm_plc_data->seg[ segid[idx][devid] ].head),PLCHEADLEN);
	

	/***************** Read Obj Data From PLC **************************************************/
	if( head.obj_size>0 )
	{
		memset(databuf,0,MAX_SCAN_BUF*2);
		seg=REGSEG;
		ref = head.obj_start_addr;		/* yyp 2008.4.14 modified */
		if(plconf[idx][devid].plctype==SIEPLC)
		{	
			seg = OBJDB_NO;
			ref = ref/2;
		}		

		scanlen=0;
		scanlen+=3*head.obj_size;
		result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
		if(result==FALSE)
		{
  			sprintf(log_str,"==== scanapp.c: plc %d.%d GetIMP(): ReadPLC OBJ error......\n",stnid,devid);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return(FALSE);
		}

		/*****************decode obj data **************************************************/
		offlen=0;
		memcpy(shm_plc_data->seg[ segid[idx][devid] ].obj_data,databuf,2*scanlen);
		shm_plc_data->seg[ segid[idx][devid] ].obj_update_flag++;
		if(shm_plc_data->seg[ segid[idx][devid] ].obj_update_flag>=MAX_FLAG_NUM)
		{
			shm_plc_data->seg[ segid[idx][devid] ].obj_update_flag=0;
		}
	}

	/***************** Read RTD Data From PLC **************************************************/

	if( head.rtd_size>0 )
	{
		memset(databuf,0,MAX_SCAN_BUF*2);
		seg=REGSEG; 
		ref=head.rtd_start_addr;	
		if(plconf[idx][devid].plctype==SIEPLC)
		{	
			seg=ANADB_NO;
			ref =  ref/2;
		}                
		scanlen=0;
		scanlen+=head.rtd_size;
		scanlen+=2*Mod16(head.rtd_size);	
                result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
		if(result==FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d GetIMP(): ReadPLC RTD error......\n",stnid,devid);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return(FALSE);
		}
		else
		{
                                                
			sprintf(log_str,"==== scanapp.c: plc %d.%d GetIMP(): ReadPLC success. \n",stnid,devid);
			log_printf(H9000_LOG_INFO,2,0,log_str);			
		}
		/*****************decode original RTD data **************************************************/
		offlen=0;
		memcpy(shm_plc_data->seg[ segid[idx][devid] ].rtd_data,databuf,2*head.rtd_size);
		offlen+=2*head.rtd_size;
		memcpy(shm_plc_data->seg[ segid[idx][devid] ].rtd_qlt,databuf+offlen,2*Mod16(head.rtd_size));
		offlen+=2*Mod16(head.rtd_size);
		memcpy(shm_plc_data->seg[ segid[idx][devid] ].rtd_en,databuf+offlen,2*Mod16(head.rtd_size));	
		shm_plc_data->seg[ segid[idx][devid] ].rtd_update_flag++;
		if(shm_plc_data->seg[ segid[idx][devid] ].rtd_update_flag>=MAX_FLAG_NUM)
		{
			shm_plc_data->seg[ segid[idx][devid] ].rtd_update_flag=0;
		}
	}


	/***************** Read IMP and IMP_COMM  Data From PLC **************************************************/

	if( head.imp_size>0 || head.impcomm_size>0 )
	{
		memset(databuf,0,MAX_SCAN_BUF*2);
		seg=REGSEG;
		ref=head.imp_start_addr;
		if(plconf[idx][devid].plctype==SIEPLC)
		{	
			seg=ANADB_NO;
			ref =  ref/2;
		} 		
		scanlen=0;
		scanlen+=head.imp_size *2;
		scanlen+=1*Mod16(head.imp_size);
		scanlen+=head.impcomm_size*2;
		scanlen+=1*Mod16(head.impcomm_size);		
		result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
		if(result==FALSE)
		{
         		sprintf(log_str,"==== scanapp.c: plc %d.%d GetIMP(): ReadPLC IMP error......\n",stnid,devid);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return(FALSE);
		}
           	
		/*****************decode original IMP and IMP_COMM data **************************************************/
		offlen=0;
		memcpy(shm_plc_data->seg[ segid[idx][devid] ].pi_data,databuf+offlen,4*head.imp_size);
		offlen+=4*head.imp_size;
		memcpy(shm_plc_data->seg[ segid[idx][devid] ].pi_qlt,databuf+offlen,2*Mod16(head.imp_size));
		offlen+=2*Mod16(head.imp_size);

		memcpy(shm_plc_data->seg[ segid[idx][devid] ].picomm_data,databuf+offlen,4*head.impcomm_size);
		offlen+=4*head.impcomm_size;
		memcpy(shm_plc_data->seg[ segid[idx][devid] ].picomm_qlt,databuf+offlen,2*Mod16(head.impcomm_size));
		shm_plc_data->seg[ segid[idx][devid] ].imp_update_flag++;
		if(shm_plc_data->seg[ segid[idx][devid] ].imp_update_flag>=MAX_FLAG_NUM)
		{
			shm_plc_data->seg[ segid[idx][devid] ].imp_update_flag=0;
		}

	}

	
	/***************** Read Param Data From PLC and decode when updated *****************************/        
            
	if(head.param_update_flag==PARAMFLAGPLC && head.param_size>0)
	{
		memset(databuf,0,MAX_SCAN_BUF*2);
		seg=REGSEG;
		ref = head.param_start_addr;
		if(plconf[idx][devid].plctype==SIEPLC)
		{	
			seg = PAMDB_NO;
			ref = ref/2;
		}		
		scanlen=0;
		scanlen+=head.param_size;
		result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
		if(result==FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d GetIMP(): ReadPLC Param error......\n",stnid,devid);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return(FALSE);
		}
		memcpy(shm_plc_data->seg[ segid[idx][devid] ].param_data,databuf,2*scanlen);
	}
	/* modified by yyp 07.08.09 */
	shm_plc_data->seg[ segid[idx][devid] ].param_update_flag++;
	if(shm_plc_data->seg[ segid[idx][devid] ].param_update_flag>=MAX_FLAG_NUM)
	{
		shm_plc_data->seg[ segid[idx][devid] ].param_update_flag=0;
	}	


	return TRUE;
}

/********************************************************************************************************
 read plc SysMsg data
*********************************************************************************************************/

int GetSysMsg(MYSOCKET sockplc,LCUID plcid)
{
	int 		seg;
	int 		ref;
	int 		result;
	int  		scanlen;
	int 		offlen=0;	
	struct plc_data_seg_head head;
	unsigned	char databuf[MAX_SCAN_BUF*2];  
	unsigned char	stnid,idx,devid;
	char	log_str[256];
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx = index_stn[stnid];
	if (sockplc.sock==INVALID_SOCKET)   return FALSE;

	/***************** get shm data head **************************************************/
	memcpy(&head,&(shm_plc_data->seg[ segid[idx][devid] ].head),PLCHEADLEN);
	/***************** Read Soe  Data From PLC **************************************************/
	if( head.soe_size>0 )
	{	
		memset(databuf,0,MAX_SCAN_BUF*2);
		seg=REGSEG;
		ref=head.soe_start_addr;		

		if(plconf[idx][devid].plctype==SIEPLC)
		{	
			seg=SOEDB_NO;
			ref =  ref/2;
		}                 
		scanlen=0;
		scanlen+=3*Mod16(head.soe_size);	
		result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
		if(result==FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d GetSysMsg(): ReadPLC error......\n",stnid,devid);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return(FALSE);
		}
                
		/*****************decode original soe  data **************************************************/
		offlen=0;
		memcpy(shm_plc_data->seg[ segid[idx][devid] ].soe_data,databuf,2*Mod16(head.soe_size));
		offlen+=2*Mod16(head.soe_size);
		memcpy(shm_plc_data->seg[ segid[idx][devid] ].soe_qlt,databuf+offlen,2*Mod16(head.soe_size));
		offlen+=2*Mod16(head.soe_size);
		memcpy(shm_plc_data->seg[ segid[idx][devid] ].soe_en,databuf+offlen,2*Mod16(head.soe_size));
	}
	/***************** Read SysMsg  Data From PLC **************************************************/
	if( head.msg_size>0 )
	{
		memset(databuf,0,MAX_SCAN_BUF*2);
		seg=REGSEG;
		ref=head.msg_start_addr;
		if(plconf[idx][devid].plctype==SIEPLC)
		{	
			seg=ANADB_NO;
			ref =  ref/2;
		} 		
		scanlen=0;
		scanlen+=1*Mod16(head.msg_size);	
		result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
		if(result==FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d GetSysMsg(): ReadPLC error......\n",stnid,devid);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return(FALSE);
		}
		else
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d GetSysMsg(): ReadPLC success. \n",stnid,devid);
			log_printf(H9000_LOG_INFO,2,0,log_str);			
		}		
		/*****************decode original SysMsg  data **************************************************/
		offlen=0;
		memcpy(shm_plc_data->seg[ segid[idx][devid] ].msg_data,databuf,2*Mod16(head.msg_size));
		shm_plc_data->seg[ segid[idx][devid] ].sysmsg_update_flag++;
		if(shm_plc_data->seg[ segid[idx][devid] ].sysmsg_update_flag>=MAX_FLAG_NUM)
		{
			shm_plc_data->seg[ segid[idx][devid] ].sysmsg_update_flag=0;
		}
	}
	return TRUE;
}

/********************************************************************************************************
 read plc POL data
*********************************************************************************************************/

int GetPOL(MYSOCKET sockplc,LCUID plcid)
{
	int 		seg;
	int 		ref;
	int 		result;
	int  		scanlen;
	int 		offlen=0;
	short		pol_ptr_end;
	struct plc_data_seg_head head;
	unsigned	char databuf[MAX_SCAN_BUF*2];  
	unsigned char	stnid,idx,devid;
	char	log_str[256];
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx = index_stn[stnid];
	if (sockplc.sock==INVALID_SOCKET)   return FALSE;
	pol_ptr_end = runvar[idx][devid].pol_ptr_end ;
	/***************** get shm data head and pol pointer ******************************************/
	memcpy(&head,&(shm_plc_data->seg[ segid[idx][devid] ].head),PLCHEADLEN);
		
	/***************** Read POL Status From PLC and decode ********************************/
	memset(databuf,0,MAX_SCAN_BUF*2);
	seg=REGSEG;
	ref = head.pol_start_addr;
	if(plconf[idx][devid].plctype==SIEPLC)
	{	
		seg = POLDB_NO;
		ref = ref/2;
	}		
	scanlen=7;
	scanlen+=3*Mod16(head.pol_size);
	scanlen+=3*Mod16(head.dout_size);
	scanlen+=1*Mod16(head.polc_size);
	scanlen+=3*Mod16(head.polcomm_size);
	if( scanlen == 0 )
	{
		return TRUE;
	}
	if(plconf[idx][devid].plctype!=MBPLC)
	{	
		result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf,plcid);
	}
	else
	{
		ref = ref -1;
		result = ReadPLC_MULT_MB(sockplc, (unsigned char)seg, ref, scanlen, databuf);
	}
	if(result==FALSE)
	{
		sprintf(log_str,"==== scanapp.c: plc %d.%d GetPOL(): ReadPLC error......\n",stnid,devid);
		log_printf(H9000_LOG_ERROR,1,0,log_str);	
		return(FALSE);
	}
	else
	{
		sprintf(log_str,"==== scanapp.c: plc %d.%d GetPOL(): ReadPLC success. \n",stnid,devid);
		log_printf(H9000_LOG_INFO,2,0,log_str);			
	}       
	/*****************decode original POL  data **************************************************/
	offlen=0;
	memcpy(&(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_ptr_end].backup),databuf+offlen,14);
#ifdef _BIG_ENDIAN_
	Swap2Str((unsigned char *) &(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_ptr_end].backup),7);
#endif
	offlen+=14;
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_ptr_end].pol_data,databuf+offlen,2*Mod16(head.pol_size));
       
	offlen+=2*Mod16(head.pol_size);
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_ptr_end].dout_data,databuf+offlen,2*Mod16(head.dout_size));
	offlen+=2*Mod16(head.dout_size);	
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_ptr_end].pol_qlt,databuf+offlen,2*Mod16(head.pol_size));
	offlen+=2*Mod16(head.pol_size);
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_ptr_end].pol_en,databuf+offlen,2*Mod16(head.pol_size));
	offlen+=2*Mod16(head.pol_size);			
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_ptr_end].dout_qlt,databuf+offlen,2*Mod16(head.dout_size));
	offlen+=2*Mod16(head.dout_size);
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_ptr_end].dout_en,databuf+offlen,2*Mod16(head.dout_size));
	offlen+=2*Mod16(head.dout_size);
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_ptr_end].polc_data,databuf+offlen,2*Mod16(head.polc_size));
	offlen+=2*Mod16(head.polc_size);
	
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_ptr_end].polcomm_data,databuf+offlen,2*Mod16(head.polcomm_size));
	offlen+=2*Mod16(head.polcomm_size);
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_ptr_end].polcomm_qlt,databuf+offlen,2*Mod16(head.polcomm_size));
	offlen+=2*Mod16(head.polcomm_size);
	memcpy(shm_plc_data->seg[ segid[idx][devid] ].pol_data[pol_ptr_end].polcomm_en,databuf+offlen,2*Mod16(head.polcomm_size));
		
	pol_ptr_end++;
	if(pol_ptr_end>=MAX_POL_BUF)
	{
		pol_ptr_end=0;
	}
	runvar[idx][devid].pol_ptr_end = pol_ptr_end;	
	shm_plc_data->seg[ segid[idx][devid] ].pol_seg_ptr = pol_ptr_end;
	return TRUE;
}

/********************************************************************************************************
 read plc INT Alarm data
*********************************************************************************************************/

int GetINTAlm(MYSOCKET sockplc,LCUID plcid)
{
	int	seg;
	int	ref;
	int	result;
	int	scanlen;
	int	offlen=0;
	int	soealm_base,objalm_base,soealm_max,objalm_max;
	char	log_str[256];
	struct plc_data_seg_head	head;
	unsigned char	databuf[MAX_SCAN_BUF*2];  
	unsigned char	stnid,idx,devid;
	stnid=plcid.stnid;
	devid=plcid.devid;
	idx = index_stn[stnid];
	if (sockplc.sock==INVALID_SOCKET)   return FALSE;
	/***************** get shm data head **************************************************/
	memcpy(&head,&(shm_plc_data->seg[ segid[idx][devid] ].head),PLCHEADLEN);

	soealm_base = head.soealm_start_addr;
	objalm_base = head.objalm_start_addr;
	soealm_max	= head.soealm_max;
	objalm_max	= head.objalm_max;
	if(plconf[idx][devid].plctype==SIEPLC)
	{	
		soealm_base = soealm_base/2;
		objalm_base = objalm_base/2;	
	}		

	if(runvar[idx][devid].FirstIntAlm == YES)
	{
		/*
		runvar[idx][devid].soealm_ptr_end = head.soealm_ptr;
		runvar[idx][devid].objalm_ptr_end = head.objalm_ptr;
		*/
		runvar[idx][devid].FirstIntAlm = NO;
	}
	sprintf(log_str,"==== soealm_ptr_end=%d, head.soealm_ptr=%d\n",runvar[idx][devid].soealm_ptr_end,head.soealm_ptr);	
	log_write(H9000_LOG_INFO,2,log_str);
 
	if(runvar[idx][devid].soealm_ptr_end==head.soealm_ptr)	
	{
		MySleep(200);
		/* return TRUE; */
	}
	else
	{
		/***************** Read Soe Alarm Data From PLC **************************************************/
		if(runvar[idx][devid].soealm_ptr_end<head.soealm_ptr)
		{
			/****** read soe alarm from end ptr to head ptr ******/	
			memset(databuf,0,MAX_SCAN_BUF*2);
	
			ref = 1 + soealm_base + runvar[idx][devid].soealm_ptr_end * SOE_ALM_LEN;
			scanlen = 0;
			scanlen += (head.soealm_ptr - runvar[idx][devid].soealm_ptr_end) * SOE_ALM_LEN;
			if(plconf[idx][devid].plctype==SIEPLC)
				seg= SOEALMDB_NO;
			else
				seg = REGSEG;			
			result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
			if(result==FALSE)
			{
				sprintf(log_str,"==== scanapp.c: plc %d.%d GetINTAlm(): ReadPLC error......\n",stnid,devid);
				log_printf(H9000_LOG_ERROR,1,0,log_str);	
				return(FALSE);
			}
			memcpy(shm_plc_data->seg[ segid[idx][devid] ].soealm_buf + 
				2 * runvar[idx][devid].soealm_ptr_end * SOE_ALM_LEN, databuf, 2 * scanlen);
		}
		else if(runvar[idx][devid].soealm_ptr_end > head.soealm_ptr)
		{
			/****** read soe alarm from end ptr to soealm_max ******/	
			memset(databuf,0,MAX_SCAN_BUF*2);
			if(plconf[idx][devid].plctype==SIEPLC)
				seg= SOEALMDB_NO;
			else
				seg = REGSEG;
			
			ref = 1 + soealm_base + runvar[idx][devid].soealm_ptr_end * SOE_ALM_LEN;
			
			scanlen = 0;
			scanlen += (soealm_max - runvar[idx][devid].soealm_ptr_end) * SOE_ALM_LEN;
                        
			result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
			if(result==FALSE)
			{
				sprintf(log_str,"==== scanapp.c: plc %d.%d GetINTAlm(): ReadPLC error......\n",stnid,devid);
				log_printf(H9000_LOG_ERROR,1,0,log_str);	
				return(FALSE);
			}
			memcpy(shm_plc_data->seg[ segid[idx][devid] ].soealm_buf + 
				2 * runvar[idx][devid].soealm_ptr_end * SOE_ALM_LEN, databuf, 2 * scanlen);		
			
			/****** read soe alarm from 0 to head ptr ******/
			if(head.soealm_ptr !=0)
			{
				memset(databuf,0,MAX_SCAN_BUF*2);
				if(plconf[idx][devid].plctype==SIEPLC)
					seg= SOEALMDB_NO;
				else
					seg = REGSEG;
				
				ref = 1 + soealm_base;
                                
				scanlen = 0;
				scanlen += (head.soealm_ptr - 0 ) * SOE_ALM_LEN;
                                
				result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
				if(result==FALSE)
				{
					sprintf(log_str,"GetINTAlm(): ReadPLC error \n");
					return(FALSE);
				}
				memcpy(shm_plc_data->seg[ segid[idx][devid] ].soealm_buf, databuf, 2 * scanlen);	
			}
		} /* end if */
		runvar[idx][devid].soealm_ptr_end = head.soealm_ptr;
		shm_plc_data->seg[ segid[idx][devid] ].soealm_ptr = runvar[idx][devid].soealm_ptr_end;
		runvar[idx][devid].FirstIntAlm = 0;
	}/* end if */

	/* added by yyp 07.07.10 */
	shm_plc_data->seg[ segid[idx][devid] ].soealm_ptr = runvar[idx][devid].soealm_ptr_end;
	
	if(runvar[idx][devid].objalm_ptr_end==head.objalm_ptr)	
	{
		MySleep(200);
		/* return TRUE; */
	}
	else
	{
		
		/***************** Read obj Alarm Data From PLC **************************************************/
		if(runvar[idx][devid].objalm_ptr_end<head.objalm_ptr)
		{
			/****** read obj alarm from end ptr to head ptr ******/	
			memset(databuf,0,MAX_SCAN_BUF*2);
			if(plconf[idx][devid].plctype==SIEPLC)
				seg= OBJALMDB_NO;
			else
				seg = REGSEG;
			ref = 1 + objalm_base + runvar[idx][devid].objalm_ptr_end * OBJ_ALM_LEN;
			scanlen = 0;
			scanlen += (head.objalm_ptr - runvar[idx][devid].objalm_ptr_end) * OBJ_ALM_LEN;
			result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
			if(result==FALSE)
			{
				sprintf(log_str,"==== scanapp.c: plc %d.%d GetINTAlm(): ReadPLC error......\n",stnid,devid);
				log_printf(H9000_LOG_ERROR,1,0,log_str);	
				return(FALSE);
			}
			memcpy(shm_plc_data->seg[ segid[idx][devid] ].objalm_buf + 
				2 * runvar[idx][devid].objalm_ptr_end * OBJ_ALM_LEN, databuf, 2 * scanlen);
		}
		else if(runvar[idx][devid].objalm_ptr_end > head.objalm_ptr)
		{
			/****** read obj alarm from end ptr to objalm_max ******/	
			memset(databuf,0,MAX_SCAN_BUF*2);
			if(plconf[idx][devid].plctype==SIEPLC)
				seg= OBJALMDB_NO;
			else
				seg = REGSEG;
			ref = 1 + objalm_base + runvar[idx][devid].objalm_ptr_end * OBJ_ALM_LEN;
			scanlen = 0;
			scanlen += (objalm_max - runvar[idx][devid].objalm_ptr_end) * OBJ_ALM_LEN;
			result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
			if(result==FALSE)
			{
				sprintf(log_str,"==== scanapp.c: plc %d.%d GetINTAlm(): ReadPLC error......\n",stnid,devid);
				log_printf(H9000_LOG_ERROR,1,0,log_str);	

				return(FALSE);
			}
			memcpy(shm_plc_data->seg[ segid[idx][devid] ].objalm_buf + 
				2 * runvar[idx][devid].objalm_ptr_end * OBJ_ALM_LEN, databuf, 2 * scanlen);		
			
			/****** read obj alarm from 0 to head ptr ******/
			if(head.objalm_ptr !=0)
			{
				memset(databuf,0,MAX_SCAN_BUF*2);
				if(plconf[idx][devid].plctype==SIEPLC)
					seg= OBJALMDB_NO;
				else
					seg = REGSEG;
				ref = 1 + objalm_base;
				scanlen = 0;
				scanlen += (head.objalm_ptr - 0 ) * OBJ_ALM_LEN;
				result = ReadPLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
				if(result==FALSE)
				{
					sprintf(log_str,"GetINTAlm(): ReadPLC error \n");
					return(FALSE);
				}
				memcpy(shm_plc_data->seg[ segid[idx][devid] ].objalm_buf, databuf, 2 * scanlen);	
			}
		} /* end if */
		runvar[idx][devid].objalm_ptr_end = head.objalm_ptr;
		shm_plc_data->seg[ segid[idx][devid] ].objalm_ptr = runvar[idx][devid].objalm_ptr_end;
		runvar[idx][devid].FirstIntAlm = 0;
		/*
		shm_plc_data->seg[ segid[idx][devid] ].objalm_update_flag++;
		if(shm_plc_data->seg[ segid[idx][devid] ].objalm_update_flag>=MAX_FLAG_NUM)
		{
			shm_plc_data->seg[ segid[idx][devid] ].objalm_update_flag=0;
		}
		*/
	}/* end if */
	shm_plc_data->seg[ segid[idx][devid] ].objalm_ptr = runvar[idx][devid].objalm_ptr_end;

	return TRUE;
}

/********************************************************************************************************
 write date to plc
*********************************************************************************************************/

int WriteData(MYSOCKET sockplc,LCUID plcid)
{
	int 		seg;
	int 		ref;
	int 		result;
	int  		scanlen;
	int		offlen=0;
	struct plc_data_seg_head	head;
	char	log_str[256];
	
	unsigned char	databuf[MAX_SCAN_BUF*2];
	char	tmpbuf[MAX_SCAN_BUF*2];
	char 	smsbuf[120];
	unsigned char	stnid,idx,devid;
	stnid = plcid.stnid;
	devid = plcid.devid; 
	idx = index_stn[stnid];
	if (sockplc.sock==INVALID_SOCKET)   return FALSE;
	MySleep(200);
	/***************** get shm data head **************************************************/
	memcpy(&head,&(shm_plc_data->seg[ segid[idx][devid] ].head),PLCHEADLEN);
	
	/****** write sending data to plc if updated ******/

        
	if(shm_plc_send->seg[idx][devid].send_update_flag != runvar[idx][devid].send_update_flag)
	{
		
		sprintf(log_str,"==== scanapp.c: plc %d.%d  shm_flag.send_update_flag=%d and runvar.send_update_flag=%d\n",
		stnid,devid,shm_plc_send->seg[idx][devid].send_update_flag, runvar[idx][devid].send_update_flag);
		log_printf(H9000_LOG_INFO,2,0,log_str);
		runvar[idx][devid].send_update_flag = shm_plc_send->seg[idx][devid].send_update_flag;		
		if(shm_plc_send->seg[idx][devid].ai_size > SEND_ANA_SIZE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d send ai_size =%d error......\n",
				stnid,devid,shm_plc_send->seg[idx][devid].ai_size);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			sprintf(smsbuf,"LCU%d.%d 下送ANA=%d错误",stnid,devid,shm_plc_send->seg[idx][devid].ai_size);
			BdSms(smsbuf);	
			return(FALSE);		
		}
		if( shm_plc_send->seg[idx][devid].pol_size > SEND_POL_SIZE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d send pol_size =%d error......\n",
				stnid,devid,shm_plc_send->seg[idx][devid].pol_size);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			sprintf(smsbuf,"LCU%d.%d 下送POL=%d错误",stnid,devid,shm_plc_send->seg[idx][devid].pol_size);
			BdSms(smsbuf);				
			return(FALSE);			
		}


		runvar[idx][devid].send_update_flag = shm_plc_send->seg[idx][devid].send_update_flag; 
		memset(tmpbuf,0,MAX_SCAN_BUF*2);
		memset(databuf,0,MAX_SCAN_BUF*2);
		seg=REGSEG;
		ref=head.send_start_addr;
		if(plconf[idx][devid].plctype==SIEPLC)
		{	
			seg = SENDDB_NO;
			ref = ref/2;
			shm_plc_send->seg[idx][devid].aiqlt_base = SENDANAQLTBASE_SIE;
			shm_plc_send->seg[idx][devid].pol_base = SENDPOLBASE_SIE;
		}		
		else if(plconf[idx][devid].plctype==MBPLC
			|| plconf[idx][devid].plctype==PREPLC)
		{
			shm_plc_send->seg[idx][devid].aiqlt_base = SENDANAQLTBASE_MB;
			shm_plc_send->seg[idx][devid].pol_base = SENDPOLBASE_MB;
		}
		else if(plconf[idx][devid].plctype==GEPLC)
		{
			shm_plc_send->seg[idx][devid].aiqlt_base = SENDANAQLTBASE_GE;
			shm_plc_send->seg[idx][devid].pol_base = SENDPOLBASE_GE;			
		}
		else if(plconf[idx][devid].plctype==ABPLC)
		{
			shm_plc_send->seg[idx][devid].aiqlt_base = SENDANAQLTBASE_AB;
			shm_plc_send->seg[idx][devid].pol_base = SENDPOLBASE_AB;			
		}
		else
		{
			sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,
				plcid.devid,plconf[idx][devid].plctype);
			log_printf(H9000_LOG_ERROR,1,0,log_str);
			return(FALSE);
		}		

		scanlen = SEND_DATA_SIZE;
		memcpy(databuf,&(shm_plc_send->seg[idx][devid]),scanlen*2);
#ifdef _BIG_ENDIAN_
		if(plconf[idx][devid].plctype!=SIEPLC)
			Swap2Str((unsigned char *)databuf,5);
#endif	
               
		result=WritePLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
		if(result==FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d write plc sending data error......\n",stnid,devid);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return(FALSE);
		}
		else
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d flag=%d write  plc sending data successfully\n",
				stnid,devid,shm_plc_send->seg[idx][devid].send_update_flag);
			log_printf(H9000_LOG_INFO,2,0,log_str);	
		}

	} /* end if */
			 	
 	/****** write param data to plc if updated ******/

	/****** removed by yyp 06.08.09 needed to be modified by yyp 07.08.01
	if(shm_plc_data->seg[ segid[idx][devid] ].param_update_flag == PARAMFLAGSCADA )
	{
		if(shm_plc_data->seg[ segid[idx][devid] ].param_size >0 &&
			shm_plc_data->seg[ segid[idx][devid] ].param_size <= PARAM_SIZE)
		{	
			seg=REGSEG;
			ref=head.param_start_addr;
			if(plconf[idx][devid].plctype==SIEPLC)
			{	
				seg = SENDDB_NO;
				ref = ref/2;
			}		

			scanlen = 3;
			scanlen += shm_plc_data->seg[ segid[idx][devid] ].param_size;	
			memcpy(databuf,&(shm_plc_data->seg[ segid[idx][devid] ].param_flag),scanlen*2);
#ifdef _BIG_ENDIAN_
			if(plconf[idx][devid].plctype!=SIEPLC)
				Swap2Str(databuf,scanlen);
#endif			
			result=WritePLC(sockplc, (unsigned char)seg, ref, scanlen, databuf, plcid);
			if(result==FALSE)
			{
				sprintf(log_str,"==== scanapp.c: plc %d.%d write param data error......\n",stnid,devid);
				log_printf(H9000_LOG_ERROR,1,0,log_str);	
				return(FALSE);
			}
			else
			{
				sprintf(log_str,"==== scanapp.c: plc %d.%d write param data successfully!\n",stnid,devid);
				log_printf(H9000_LOG_INFO,2,0,log_str);	
			}
		}
	}
	*****/

	runvar[idx][devid].timeloopcount ++;
	if( runvar[idx][devid].timeloopcount >= 1)
	{	
		/******  write run flag to plc ******/
		if(WriteRunFlag(sockplc,plcid) == FALSE)
		{
			sprintf(log_str,"==== scanapp.c: plc %d.%d write run flag error\n",stnid,devid);
			log_printf(H9000_LOG_ERROR,1,0,log_str);	
			return 	FALSE;
		}
		runvar[idx][devid].timeloopcount = 0;
		
	}
	/****** if reaching time, write time to plc ******/
	if(g_sync_time == 1)
	{
		/*
		if(runvar[idx][devid].FirstSend == YES)
		{
		*/
		runvar[idx][devid].timecmdloopcount ++;
		if( runvar[idx][devid].timecmdloopcount >= 6015)
		{
			if(WriteTimeCmd(sockplc,plcid) == FALSE)
			{
				sprintf(log_str,"==== scanapp.c: plc %d.%d write time cmd error\n",stnid,devid);
				log_printf(H9000_LOG_ERROR,1,0,log_str);	
				return 	FALSE;
			}
			runvar[idx][devid].timecmdloopcount = 0;
		}
		/*
			runvar[idx][devid].FirstSend  = NO;
		}
		*/
	}
	return TRUE;
}

/***************************************************************************************
Write Run Flag to a WORD in plc data memory . RunFlag switch between 1 and 2 
every loop of Reading plc data.Address is defined as PLCRUNFLAG in "lcu_data.h"?
return TRUE when successfully,FALSE otherwise.
****************************************************************************************/
int WriteRunFlag(MYSOCKET sockplc,LCUID plcid)
{
	int 		seg;
	int 		ref;
	int 		result;
	int  		scanlen;
	unsigned char	stnid,idx,devid;
	unsigned	short runflag;			/*small size buf */
	char	log_str[256];
	stnid = plcid.stnid;
	devid = plcid.devid; 
	idx = index_stn[stnid];
	seg=REGSEG;
	if(plconf[idx][devid].plctype==SIEPLC)
	{	
		seg = SENDDB_NO;
		ref = PLCRUNADDR_SIE/2;
	}		
	else if(plconf[idx][devid].plctype==MBPLC
		|| plconf[idx][devid].plctype==PREPLC)
	{
		ref=PLCRUNADDR_MB;
	}
	else if(plconf[idx][devid].plctype==GEPLC)
	{
		ref=PLCRUNADDR_GE;
	}
	else if(plconf[idx][devid].plctype==ABPLC)
	{
		ref=PLCRUNADDR_AB;
	}
	else
	{
		sprintf(log_str,"==== plc %d.%d type err: plctype =%d",plcid.stnid,plcid.devid,plconf[idx][devid].plctype);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return(FALSE);
	}
	scanlen=1;
	runflag = runvar[idx][devid].plc_run_flag;
	runvar[idx][devid].plc_run_flag ++;
	if(runvar[idx][devid].plc_run_flag >= MAX_PLC_FLAG)
	{
		runvar[idx][devid].plc_run_flag =1;
	}
#ifdef _BIG_ENDIAN_
	if(plconf[idx][devid].plctype!=SIEPLC)
		Swap2Byte(&runflag);
#endif
	result=WritePLC(sockplc, (unsigned char)seg, ref, scanlen, (unsigned char *)&runflag, plcid);
	if(result==FALSE)
	{
		sprintf(log_str,"==== scanapp.c: plc %d.%d Write run flag data error\n",stnid,devid);
		log_printf(H9000_LOG_ERROR,1,0,log_str);	
		return(FALSE);
	}
	else
	{
		sprintf(log_str,"==== scanapp.c: plc %d.%d Write run flag data Success!\n",stnid,devid);
		log_printf(H9000_LOG_INFO,2,0,log_str);	
	}
	return TRUE;
}


/***************************************************************************************
 WriteTimeCmd(SOCKET sockplc,LCUID plcid)
****************************************************************************************/
int WriteTimeCmd(MYSOCKET sockplc,LCUID plcid)
{
	LAN_HEAD        head;
	COMMAND_MSG	msg;
	int		length;
	char	log_str[256];
	char		buf[COMMAND_SIZE];
	char            namebuf[40];
	char		stn_name[STATION_NAME_SIZE];
	char		dev_name[GROUP_NAME_SIZE];	
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	POINTER		pt;
	unsigned	char stnid,idx,devid;
	stnid = plcid.stnid;
	devid = plcid.devid;
	idx = index_stn[stnid];	
	if(-1==GetStnNameById(stnid, stn_name))
	{
		return FALSE;
	}
	if(-1==GetGroupNameById(stnid, devid, dev_name))
	{
		return FALSE;
	}	
	sprintf(namebuf, "%s.%s.%s", stn_name, dev_name, "SYS.TIME_SYNC");
	
	if(-1==GetPtIdByNameStr(namebuf, &pt))
	{
		sprintf(log_str,"\nno such point %s in db\n",namebuf);
		log_printf(H9000_LOG_ERROR,1,0,log_str);
		return FALSE;
	}
	msg.type_id = TIME_SYNC;
	msg.stn_id = pt.stn_id;
	msg.dev_id = pt.dev_id;
	msg.data_type = SYSMSG_TYPE;
	msg.point_id = pt.pt_id;	
	msg.status.opr = 0;
	msg.status.result = 0;
	msg.status.state = 0;
	msg.status.src = 0;
	msg.data.iValue = 0;
	GetSrcId(&msg.host_id);
	msg.usr_id=0;
	strcpy(msg.usr_login_name,"plcscan");
	if(GetPtCtrlAddrById( &pt, &msg.ctrl_addr ) == -1)
	{
		printf("GetPtCtrlAddrById eror\n");
		return FALSE;
	}
	
	re_time=time(&newtime);
	devtime=localtime(&newtime);
	msg.second  =devtime->tm_sec;
	msg.minute  =devtime->tm_min;
	msg.hour  =devtime->tm_hour;
	msg.day  =devtime->tm_mday;
 	msg.month  =devtime->tm_mon+1;
	msg.year  =devtime->tm_year+1900; 
	head.dest_id=msg.dev_id;
	head.src_id = msg.host_id;
	head.dp_type=COMM_TIME;
	head.length = COMMAND_SIZE;
	head.dest_stn = msg.stn_id;
	length = head.length;
	Swap2Byte(&head.length);	
	Swap2Byte(&msg.point_id);
	Swap2Byte(&msg.status);
	Swap4Byte(&msg.data);
	Swap2Byte(&msg.ctrl_addr);
	Swap2Byte(&msg.year);
	memcpy(buf,&msg,COMMAND_SIZE);
	lan_out((char *)&msg,head,0);	
	return TRUE;
}

int BdScanQlt(LCUID plcid, int *p_count)
{
	unsigned char stnid,devid;
	short scanhost,lcuonoff,plclinkstate,plcscanstat;
	stnid= plcid.stnid  ;
	devid = plcid.devid ;
	GetScanHost(plcid, &scanhost);
	if(g_localhost_id != (unsigned char)scanhost) return FALSE; 

	GetLCUONOFFStat(plcid, &lcuonoff);
	GetPlcLinkStat(plcid, &plclinkstate);
	GetPlcScanStat(plcid, &plcscanstat);
	if(lcuonoff != LCU_ON)
	{
		if(*p_count>=MAXRETRY_QLT)
		{
			BdPlcLinkStatQlt(plcid,1);
			BdPlcScanStatQlt(plcid,1);
			BdNetWkQlt(plcid,1);
			BdCpuWkQlt(plcid,1);
			BdCpuStatQlt(plcid,1);
			*p_count = 0;
		}
		else
		{
			(*p_count)++;			
		}
	}
	else if(plclinkstate != LCU_ON)
	{

		if(*p_count>=MAXRETRY_QLT)
		{
			BdPlcScanStatQlt(plcid,1);
			BdNetWkQlt(plcid,1);
			BdCpuWkQlt(plcid,1);
			BdCpuStatQlt(plcid,1);
			*p_count = 0;
		}
		else
		{
			(*p_count)++;			
		}	
	}
	else if(plcscanstat != SCANRUN)
	{
		if(*p_count>=MAXRETRY_QLT)
		{
			BdNetWkQlt(plcid,1);
			BdCpuWkQlt(plcid,1);
			BdCpuStatQlt(plcid,1);
			*p_count = 0;
		}
		else
		{
			(*p_count)++;			
		}				
	}

	if(lcuonoff == LCU_ON)
	{
		BdPlcLinkStatQlt(plcid,0);
		if(plclinkstate == LCU_ON)
		{
			BdPlcScanStatQlt(plcid,0);
			if(plcscanstat == SCANRUN)
			{
				*p_count = 0;
				BdNetWkQlt(plcid,0);
				BdCpuWkQlt(plcid,0);
				BdCpuStatQlt(plcid,0);				
			}
		}
	}
	return TRUE;
}
