#include	<time.h>
#include	<stdio.h>

 
#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"
#include	"../../../dps/v4.0a/inc/dps_functions_def.h"

#include	"../../../dms/v4.0a/inc/ems_init_def.h"   
#include	"../../../dps/v4.0a/inc/fault_table_def.h"  

#ifdef	WINDOWS_OS
#include	"../inc/jtaud.h"
#include	"../inc/msaud.h"
#include	<direct.h>
#endif


#define		FAULT_AUD_ALM_PLAY_TIMES	1
#define		MAX_PLAY_ENTRY_NUM			5

#define	AUD_ALM_STATE_POINT	         "AUD_WK_STA"
#define	AUD_FAULT_ALM_STATE_POINT	 "AUD_FAULT_WK_STA"
 
#pragma	comment(lib, "../lib/msaud.lib")
#pragma	comment(lib, "../lib/jtaud.lib")
#pragma	comment(lib, "../lib/jTTS_ma.lib")
#pragma	comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma	comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma	comment(lib, "../../../lan/v4.0a/lib/lan.lib")
#pragma	comment(lib, "../../../dps/v4.0a/lib/dpslib.lib")

#pragma	comment(lib, "wsock32.lib")

 
typedef struct
{
	char	pt_name[POINT_NAME_SIZE];
	char	stn_comment[STATION_COMMENT_SIZE];
	char	dev_comment[GROUP_COMMENT_SIZE];
	char	entry_longname[ENTRY_LONGNAME_SIZE];
	char	status_str[64];
} AUD_ALM_STR;

struct
{
	AUD_ALM_STR	aud_str;
	int			times;
} play_list[MAX_PLAY_ENTRY_NUM];

int		InitIPC(void);
//BOOL	IsAudAlmEnable(void);
BOOL	IsFault_AudAlmEnable(void);
int		PlayAlmRec(REC_ENTRY_UNION *rec, BOOL fault_play_flag);
int		GetAnaStatusStrings(ANLG_REC *ana_rec, POINTER *point, char *str);
int		GetIndStatusStrings(IND_REC *ind_rec, POINTER *point, char *str);
void	InsertPlayList(AUD_ALM_STR *aud_str);
 
int   	GetaudalmCbaState(POINTER *point,int type_id,int ind_rt_state,int qlt_alm_flag);//2011 0801 chenxs add for aumalm interlock

int	AUD_FAULT_ALM_TIMESPA;
int	AUD_FAIL_ALM_TIMESPA;

float	AUD_FAULT_ALM_TIMESPA_F;
float	AUD_FAIL_ALM_TIMESPA_F;

SHM_RECORD		*shm_rec_addr;

int		jt_aud_flag, ms_aud_flag, exLangState;
int		ms_voice_item;
int		single_audio_mode;

#define		unix_aud_play_cmd	"play -v 10"

int PlayStrAudio(BOOL is_faultalmpt,BOOL is_failalmpt)
{
	char	faultalmptfile[256];
	char	failalmptfile[256];
	
#ifndef WINDOWS_OS
	char	string[256];
#endif
 
	sprintf(faultalmptfile, "%sfaultalmptfile.wav", AUD_FILE_PATH);
	sprintf(failalmptfile, "%sfailalmptfile.wav", AUD_FILE_PATH);
#ifdef	WINDOWS_OS
	


	if(is_faultalmpt)
	{
		return	PlaySound(faultalmptfile, NULL, SND_SYNC|SND_FILENAME|SND_NODEFAULT);
	}
	else if(is_failalmpt)
	{
		return	PlaySound(failalmptfile, NULL, SND_SYNC|SND_FILENAME|SND_NODEFAULT);
	}

	return 0;
	
#else /*SUN_OS*/
	
	if(is_faultalmpt)
	{
		sprintf(string, "%s %s", unix_aud_play_cmd, faultalmptfile);
		system(string);
		return 0;
	}
	else if(is_failalmpt)
	{
		sprintf(string, "%s %s", unix_aud_play_cmd, failalmptfile);
		system(string);
		return 0;
	}

	return 0;
#endif
}

void GetAudModeDefine(int argc, char *argv[])
{
	int		i;
	
	single_audio_mode= 0;
	for(i=1; i<argc; i++)
	{
		if(!strcmp(argv[i], "-s"))
		{
			single_audio_mode= 1;
			return;
		}
	}
}

void GetAudLibDefine(int argc, char *argv[])	
{
	int		i;
	
	ms_aud_flag= 0;
		jt_aud_flag= 1;
	for(i=1; i<argc; i++)
	{
		if(!strncmp(argv[i], "-m", 2))
		{
			ms_aud_flag= 1;
			jt_aud_flag=0;
			sscanf(&argv[i][2], "%d", &ms_voice_item);
		}
		else 
		{
			jt_aud_flag= 1;
	 
			ms_aud_flag= 0;
		}
	}
}

#ifdef WINDOWS_OS
void InitAudLib(int ms_flag, int jt_flag, int ms_voice)
{
	if(ms_aud_flag==1)
	{
		ReleaseAudioLibraryMS();
		if(-1==InitAudioLibraryMS(0, -7, 100, -4, NULL, ms_voice))
		{
			Err_log("Fail to initialize microsoft audio kernal!\n");
			BroadCastSms("Fail to initialize microsoft audio kernal", TRUE);
			exit(-1);
		}
	}
	if(jt_aud_flag==1)
	{
		ReleaseAudioLibraryJT();
		if(-1==InitAudioLibraryJT(5, 9, 5, NULL))
		{
			Err_log("Fail to initialize jt audio kernal!\n");
			BroadCastSms("Fail to initialize jt audio kernal", TRUE);
			exit(-1);
		}
	}
}
#endif

int	main(int argc, char *argv[])
{
	USHORT		rec_ptr;
    POINTER		point;
	BOOL		audAlm;
	BOOL		new_play_flag, fault_play_flag;
	USHORT fault_ptr_f_pos;
	int ind_rt_state;
 
	BOOL is_faultalmpt;
	BOOL is_failalmpt;
	int  ret_faultalmpt;
	int  ret_failalmpt;
	DMS_COMMON		common_all;
	int  fault_almed[MAX_SHM_RECORD_NUM];

	time_t time_fault;
	//struct tm *tp_fault;
	time_t time_fail;
	//struct tm *tp_fail;
	int inited_fault;
	int inited_fail;
	
	time_t time_fault_last;
	//struct tm *tp_fault_last;
	time_t time_fail_last;
	//struct tm *tp_fail_last;

	
	time_fault=time(NULL);
	//tp_fault=localtime(&time_fault);

	time_fault_last=time(NULL);
	//tp_fault_last=localtime(&time_fault_last);

	time_fail=time(NULL);
	//tp_fail=localtime(&time_fail);
	
	time_fail_last=time(NULL);
	//tp_fail_last=localtime(&time_fail_last);

	inited_fault=0;
	inited_fail=0;

 
	CreateLockFile(".Fault_audAlm.lock");
	if(-1==InitSysLog(H9000_MODULE_FAULT_AUDALM, "Fault_AudAlm"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	
	
	if(-1==InitIPC())
	{
		Err_log("Fail to initialize IPCs!\n");
		exit(-1);
	}

	if(-1==MountDB(NULL))
	{
		Err_log("Fail to MountDB!\n");
		exit(-1);
	}

	if(FALSE==lan_out_init())
	{
		Err_log("Fail to init lan out!\n");
		exit(-1);
	}
	
	if(-1==InitStrTextDefine())
	{
		Err_log("Fail to Init str text define!\n");
		exit(-1);
	}

	exLangState= GetExLangState();

#ifdef WINDOWS_OS
	GetAudLibDefine(argc, argv);
	if(ms_aud_flag==0 && jt_aud_flag==0)
	{
		Err_log("No audio lib be defined!\n");
		BroadCastSms("No audio lib be defined", TRUE);
		exit(-1);
	}
	InitAudLib(ms_aud_flag, jt_aud_flag, ms_voice_item);
#endif

	GetAudModeDefine(argc, argv);
	
	rec_ptr= shm_rec_addr->head.header_ptr;
	
	
	if(-1==GetEmsVariable("AUD_FAULT_ALM_TIMESPA", &AUD_FAULT_ALM_TIMESPA_F))
		AUD_FAULT_ALM_TIMESPA_F= 0;
	if(AUD_FAULT_ALM_TIMESPA_F<0)
		AUD_FAULT_ALM_TIMESPA_F=0;
	if(AUD_FAULT_ALM_TIMESPA_F>6000)
		AUD_FAULT_ALM_TIMESPA_F=6000;
	AUD_FAULT_ALM_TIMESPA=(int)AUD_FAULT_ALM_TIMESPA_F;
	
	if(-1==GetEmsVariable("AUD_FAIL_ALM_TIMESPA", &AUD_FAIL_ALM_TIMESPA_F))
		AUD_FAIL_ALM_TIMESPA_F= 0;
	if(AUD_FAIL_ALM_TIMESPA_F<0)
		AUD_FAIL_ALM_TIMESPA_F=0;
	if(AUD_FAIL_ALM_TIMESPA_F>6000)
		AUD_FAIL_ALM_TIMESPA_F=6000;
	AUD_FAIL_ALM_TIMESPA=(int)AUD_FAIL_ALM_TIMESPA_F;
	

	for( ;; )
	{
#ifdef WINDOWS_OS
		if(GetExLangState() != exLangState)
			InitAudLib(ms_aud_flag, jt_aud_flag, ms_voice_item);
#endif
		
		if(WaitforSem(SEM_RECORD, -1) == -1)
		{
			Err_log("Fail to wait record semaphore!\n");
			exit(-1);
		}

		Err_log("new event arrive");

		if(!IsFault_AudAlmEnable())
		{
			rec_ptr= shm_rec_addr->head.header_ptr;
			continue;
		}
	
		
		for(fault_ptr_f_pos=0; fault_ptr_f_pos<MAX_SHM_RECORD_NUM;fault_ptr_f_pos++)
		{
			fault_almed[fault_ptr_f_pos]=0;
		}
		while(1) 
		{
			
			if(rec_ptr == shm_rec_addr->head.header_ptr)
			{
				break;
			}
			
			new_play_flag= 0;
			fault_play_flag= 0;
			for(; rec_ptr!=shm_rec_addr->head.header_ptr;
			rec_ptr++,rec_ptr=(rec_ptr>=shm_rec_addr->head.buf_size)? 0:rec_ptr )
			{
				for(fault_ptr_f_pos=rec_ptr; fault_ptr_f_pos!=shm_rec_addr->head.header_ptr; 
				fault_ptr_f_pos++,fault_ptr_f_pos=(fault_ptr_f_pos>=shm_rec_addr->head.buf_size)? 0:fault_ptr_f_pos )
				{
					
					time_fault=time(NULL);
					//tp_fault=localtime(&time_fault);
					
					
					if(inited_fault==0 || (long)(time_fault)-(long)(AUD_FAULT_ALM_TIMESPA)>(long)(time_fault_last))
					{
						if(shm_rec_addr->rec[fault_ptr_f_pos].ana_rec.type_id!=DPS_ANA_ALM 
							&& shm_rec_addr->rec[fault_ptr_f_pos].ana_rec.type_id!=DPS_IND_ALM)
							continue;
						
						
						if(fault_almed[fault_ptr_f_pos]==1)//
							continue;
						
						GetPointFromRec(&shm_rec_addr->rec[fault_ptr_f_pos], &point);
						if(-1==IsPtAudAlmById(&point, &audAlm))
							continue;
						if(!audAlm)
							continue;
						
						
						if(!IsAlmRecord(&shm_rec_addr->rec[fault_ptr_f_pos]))
							continue;
						
						ReadEntryById(&point, &common_all);
						if(!common_all.fixed.iost.almNoAck)
							continue;
						
						if(-1==IsPtAlmCbaById(&point, &audAlm))//AlmCba audAlm
							continue;
						if(audAlm)
						{
							if(shm_rec_addr->rec[fault_ptr_f_pos].ana_rec.type_id==DPS_IND_ALM)
								ind_rt_state=shm_rec_addr->rec[fault_ptr_f_pos].ind_rec.ind_status.rt_state;
							else
								ind_rt_state=-1;
							if( !GetaudalmCbaState(&point,shm_rec_addr->rec[fault_ptr_f_pos].ana_rec.type_id,ind_rt_state,0) )
								continue;
						}
						
						is_faultalmpt=0;
						ret_faultalmpt=IsPtFaultById(&point,&is_faultalmpt);
						if(ret_faultalmpt==-1)
							is_faultalmpt=0;
						
						if(!is_faultalmpt)
							continue;
						
						PlayStrAudio(is_faultalmpt, 0);	
						fault_almed[fault_ptr_f_pos]=1;
						
						inited_fault=1;
						
						time_fault_last=time(NULL);
						//tp_fault_last=localtime(&time_fault_last);
						
						if(!IsFault_AudAlmEnable())
							break;
					}
					else
					{
						break;
					}
				}

				
				if(!IsFault_AudAlmEnable())
				{
					rec_ptr= shm_rec_addr->head.header_ptr;
					break;
				}
				
				time_fail=time(NULL);
				//tp_fail=localtime(&time_fail);
				
				
				if(inited_fail==0 || (long)(time_fail)-(long)(AUD_FAIL_ALM_TIMESPA)>(long)(time_fail_last))
				{
					
					if(fault_almed[rec_ptr]==1)//
						continue;
					
					if(shm_rec_addr->rec[rec_ptr].ana_rec.type_id!=DPS_ANA_ALM 
						&& shm_rec_addr->rec[rec_ptr].ana_rec.type_id!=DPS_IND_ALM)
						continue;
					
					
					GetPointFromRec(&shm_rec_addr->rec[rec_ptr], &point);
					if(-1==IsPtAudAlmById(&point, &audAlm))
						continue;
					if(!audAlm)
						continue;
					
					
					if(!IsAlmRecord(&shm_rec_addr->rec[rec_ptr]))
						continue;
					
					
					ReadEntryById(&point, &common_all);
					if(!common_all.fixed.iost.almNoAck)
						continue;
					
					if(-1==IsPtAlmCbaById(&point, &audAlm)) 
						continue;
					if(audAlm)
					{
						if(shm_rec_addr->rec[rec_ptr].ana_rec.type_id==DPS_IND_ALM)
							ind_rt_state=shm_rec_addr->rec[rec_ptr].ind_rec.ind_status.rt_state;
						else
							ind_rt_state=-1;
						if( !GetaudalmCbaState(&point,shm_rec_addr->rec[rec_ptr].ana_rec.type_id,ind_rt_state,0) )
							continue;
					}
					
					ret_failalmpt=0;
					ret_failalmpt=IsPtFailById(&point,&is_failalmpt);
					if(ret_failalmpt==-1)
						is_failalmpt=0;
					
					if(!is_failalmpt)
						continue;
					PlayStrAudio(0, is_failalmpt);
					fault_almed[rec_ptr]=1;

					inited_fail=1;
					
					time_fail_last=time(NULL);
					//tp_fail_last=localtime(&time_fail_last);
				}
				else
				{
					rec_ptr= shm_rec_addr->head.header_ptr;
					break;
				}
			}
		}
	}
	
}


int	GetaudalmCbaState(POINTER *point,int type_id,int ind_rt_state,int qlt_alm_flag)
{
	char	ptNameStr[POINT_NAME_SIZE+12];
	char	ptNameStr1[POINT_NAME_SIZE+16];
	BOOL    can_alarm;
	
	USHORT  iogrp=0;
	POINT_NAME	ptName;
	
	if(-1==GetPtNameById(point, ptNameStr, NULL))
		return -1;
	if(-1==StrtoPointName((char *)(&ptNameStr), &ptName))
		return -1;
	
	can_alarm=TRUE;
	
	
	
	sprintf(ptNameStr1,"%s.%s..%s",ptName.stnName,ptName.grpName,AUDALM_DCBA_SUFFIX_DEFINE);	
	
	can_alarm =InterLock_Judge( ptNameStr1 );
	if(can_alarm!=-1)
	{
		if(can_alarm==FALSE)
			return can_alarm;
	}
	
	GetPtIoGroupById(point, &iogrp);
	
	sprintf(ptNameStr1,"%s.%s_%d..%s",ptName.stnName,ptName.grpName,iogrp,AUDALM_GCBA_SUFFIX_DEFINE);
	
	can_alarm =InterLock_Judge( ptNameStr1 );
	if(can_alarm!=-1)
	{
		if(can_alarm==FALSE)
			return can_alarm;
	}
	
	if(type_id==DPS_IND_ALM)
	{
		sprintf(ptNameStr1,"%s%s_%d",ptNameStr,AUDALM_CBA_SUFFIX_DEFINE,ind_rt_state);
 		can_alarm =InterLock_Judge( ptNameStr1 );
		if(can_alarm!=-1)
		{
			if(can_alarm==FALSE)
				return can_alarm;
		}
		
	}
	sprintf(ptNameStr1,"%s%s",ptNameStr,AUDALM_CBA_SUFFIX_DEFINE);
 	
	can_alarm =InterLock_Judge( ptNameStr1 );
	if(can_alarm!=-1)
	{
		if(can_alarm==FALSE)
			return can_alarm;
	}
	

	
	return can_alarm;
}
 

int InitIPC()
{
	/*      打开记录区共享内存      */
	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		Err_log("Fail to link record shm");
		return -1;
	}
	
	/*      打开记录区信号量        */
	if( LinktoSemSet(SEM_RECORD) == -1 )
	{
		Err_log("Fail to link record semaphore\n");
		return -1;
	}
 
 
	return 0;
}


/*BOOL	IsAudAlmEnable(void)
{
	char	err_str[128];
	POINTER	pt;
	USHORT	state;

	if(-1==GetLocalHostStnId(&pt.stn_id))
		return TRUE;

	if(-1==GetSysLcuId(pt.stn_id, &pt.dev_id))
	{
		sprintf(err_str, "Station %d no sys lcu", pt.stn_id);
		Err_log(err_str);
		return TRUE;
	}

	pt.data_type= SYSMSG_TYPE;

	if(-1==GetEntryIdByName(AUD_ALM_STATE_POINT, &pt))
		return TRUE;

	if(-1==GetIndPtStateById(&pt, &state))
		return TRUE;

	if(state)
		return TRUE;
	else
		return FALSE;
}
*/
BOOL	IsFault_AudAlmEnable(void)
{
	char	err_str[128];
	POINTER	pt;
	USHORT	state;
	
	if(-1==GetLocalHostStnId(&pt.stn_id))
		return TRUE;
	
	if(-1==GetSysLcuId(pt.stn_id, &pt.dev_id))
	{
		sprintf(err_str, "Station %d no sys lcu", pt.stn_id);
		Err_log(err_str);
		return TRUE;
	}
	
	pt.data_type= SYSMSG_TYPE;
	
	if(-1==GetEntryIdByName(AUD_FAULT_ALM_STATE_POINT, &pt))
		return TRUE;
	
	if(-1==GetIndPtStateById(&pt, &state))
		return TRUE;
	
	if(state)
		return TRUE;
	else
		return FALSE;
}
 

void InsertPlayList(AUD_ALM_STR *aud_str)
{
	int		i, min_times, pos;

	pos= 0;
	min_times= FAULT_AUD_ALM_PLAY_TIMES;
	for(i=0; i<MAX_PLAY_ENTRY_NUM; i++)
	{
		if(play_list[i].times==0)
		{
			play_list[i].times= FAULT_AUD_ALM_PLAY_TIMES-1;
			play_list[i].aud_str= *aud_str;
			return;
		}
		if(min_times>play_list[i].times)
		{
			min_times= play_list[i].times;
			pos= i;
		}
	}
	play_list[pos].times= FAULT_AUD_ALM_PLAY_TIMES-1;
	play_list[pos].aud_str= *aud_str;
}










