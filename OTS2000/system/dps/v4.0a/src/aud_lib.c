#include	<time.h>
#include	<stdio.h>

 
#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"
#include	"../../../dps/v4.0a/inc/dps_functions_def.h"

#include	"../../../dms/v4.0a/inc/ems_init_def.h"      //2011 0801 cxs add for aumalm interlock
#include	"../../../dps/v4.0a/inc/fault_table_def.h"   //2011 0801 cxs add

#ifdef	WINDOWS_OS
#include	"../inc/jtaud.h"
#include	"../inc/msaud.h"
#include	<direct.h>
#endif


#define		FAULT_AUD_ALM_PLAY_TIMES	1
#define		MAX_PLAY_ENTRY_NUM			5

#define	    AUD_ALM_STATE_POINT	         "AUD_WK_STA"

//#define	AUD_ALM_TYPE_POINT           "AUD_WK_TYPE"//chenxs
//#define	AUD_ALM_JUSTPLAY_NUM_POINT   "AUD_WK_JPN"//chenxs

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
BOOL	IsAudAlmEnable(void);
int		PlayAlmRec(REC_ENTRY_UNION *rec, BOOL fault_play_flag);
int		GetAnaStatusStrings(ANLG_REC *ana_rec, POINTER *point, char *str);
int		GetIndStatusStrings(IND_REC *ind_rec, POINTER *point, char *str);
void	InsertPlayList(AUD_ALM_STR *aud_str);
BOOL	PlayFaultRepeatAlm(void);
int   	GetaudalmCbaState(POINTER *point,int type_id,int ind_rt_state,int qlt_alm_flag);//2011 0801 chenxs add for aumalm interlock



SHM_RECORD		*shm_rec_addr;
FAULT_RECORD_TABLE *shm_fault_addr;//cxs
int	AUD_ALM_TYPE;//cxs
int	AUD_ALM_JUSTPLAY_NUM;//cxs

int	    AUD_ALM_READ_ANA_VALUE;//cxs
float	AUD_ALM_TYPE_F;//cxs
float	AUD_ALM_JUSTPLAY_NUM_F;//cxs
float	    AUD_ALM_READ_ANA_VALUE_F;//cxs

int		jt_aud_flag, ms_aud_flag, exLangState;
int		ms_voice_item;
int		single_audio_mode;

int     _station_alm_flag;
UCHAR   station_ids[MAX_STATION_NUM];//20120315 cxs
int     station_alm_flag_index[MAX_STATION_NUM];//20120315 cxs

char    station_names[MAX_STATION_NUM][STATION_NAME_SIZE];//20120315 cxs 

#define		unix_aud_play_cmd	"play -v 10"


int  ReadStationAlmflagDefFile()//cxs add 20120315
{
 
	int		i,j;	
	FILE	*fp;
	char	temp_str[MAX_CHAR_NUM_IN_LINE];
 	char	temp_stations[MAX_CHAR_NUM_IN_LINE];
	char	def_filename[256];
	char	local_hostname[MAX_HOST_NAME_SIZE];
 	char	temp_hostname[MAX_HOST_NAME_SIZE];
	char    *dvl,*dvlC;
	int stnNum;
#ifdef	WINDOWS_OS
	WSADATA		WSAData;
#endif

	if(-1==GetStationNum(&stnNum))  
	{
		Err_log("audalm call GetStationNum() error.......");
		exit(-1);
	}
	for(i=0;i<MAX_STATION_NUM;i++) 
	{
		station_ids[i]=0; 
		station_alm_flag_index[i]=0;
		memset(&station_names[i],0,STATION_NAME_SIZE);
	 
	}
	
	for(i=1;i<=stnNum && i<MAX_STATION_NUM;i++)
	{
		if(-1==GetStnIdByIndex((UCHAR)(i), &(station_ids[i])))
			continue;
		if(-1==GetStnNameById(station_ids[i],station_names[i]))
		{
			memset(&station_names[i],0,STATION_NAME_SIZE);
			station_ids[i]=0;
			continue;
		}
	}
	
#ifdef	WINDOWS_OS
 
	if(WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
	{
		Err_log("AUDALM: Fail to initialize net lib!\n");
		BroadCastSms("AUDALM: Fail to initialize net lib", TRUE);
		exit(0);
	}
#endif
	
	if(gethostname(local_hostname, sizeof(local_hostname)))	
	{
		Err_log("AUDALM: Fail to gethostname!\n");
		BroadCastSms("AUDALM: Fail to gethostname", TRUE);
		exit(0);
	}
	
#ifdef	WINDOWS_OS
	strupr(local_hostname);
#endif
	dvlC=";";

	sprintf(def_filename, "%s%s", SYS_DEFINE_PATH, AUDALM_STATION_DEFINE_FILE);
	
	if(fp=fopen(def_filename,"r"))	
	{
		while( fscanf(fp,"%s",temp_str)!=EOF )	 
		{ 
			if( strstr(temp_str,"*/")!=NULL ) 
				break;	
		}	
		
		while(fgets(temp_str, sizeof(temp_str), fp))
		{ 
			if(sscanf(temp_str,"%s %s",temp_hostname,temp_stations)>1)
			{ 

				char * str_tmp =strstr(temp_str,temp_hostname);
				j=strlen(temp_hostname);
				memset(temp_stations,0,MAX_CHAR_NUM_IN_LINE);
				for(i=j;i<MAX_CHAR_NUM_IN_LINE && (int)(i-j) < (int)(strlen(str_tmp));i++)
				{
					temp_stations[i-j]=str_tmp[i];
				}
				if(strcmp(local_hostname,temp_hostname)==0)
				{
					j=1;
					for(i=0;i<MAX_CHAR_NUM_IN_LINE;i++)
					{
						if(temp_stations[i]<33)
							temp_stations[i]=59;
					}
				 
					dvl=strtok(temp_stations,dvlC);
				
					while(dvl)
					{
					 
						for(i=0;i<MAX_STATION_NUM;i++)
						{
							if(strcmp(station_names[i],dvl)==0)
							{
								station_alm_flag_index[j]=station_ids[i];
								j++;
								if(_station_alm_flag!=1)
									_station_alm_flag=1;
								if(j>=MAX_STATION_NUM)
									break;
							}
						}
						
						dvl=strtok(NULL,dvlC);
					}
					
					break;
				}
			}
		}
		
		SendTraceMsg(0,0, "read audalm station define %s OK! \n",def_filename);
		fclose( fp );
	}
	else
		SendTraceMsg(0,0, "Fail to audalm station define file %s", def_filename);

		return 1;
}



int PlayStrAudio(AUD_ALM_STR *aud_str, BOOL man_voice)
{
	char	string[256];
	char	audiofile[256];
	int     i;

	sprintf(audiofile, "%ssingle_alarm.wav", AUD_FILE_PATH);


#ifdef	WINDOWS_OS
	
	i=0;
	if(AUD_ALM_READ_ANA_VALUE!=1)//cxs
	{
	
		//for(i=0;i<60;i++)20120920 cancle cxs
		for(i=0;i<sizeof(aud_str->status_str[i])-4;i++)//20120920 add cxs
		{
			//if(aud_str->status_str[i]>=48 && aud_str->status_str[i]<=57 ) //20120920 cancle cxs
			if(aud_str->status_str[i]>='0' && aud_str->status_str[i]<='9' )//20120920 add cxs
				continue;
			else if(aud_str->status_str[i]=='.')
				continue;
			else
			{
				i--;
				break;
			}
		}
		if(i<0)
			i=0;

	}
	sprintf(string, "%s%s %s %s  ", aud_str->stn_comment, aud_str->dev_comment, 
		aud_str->entry_longname, &aud_str->status_str[i]);
 
	FilterAudStr(string);
	Err_log(string);

	if(single_audio_mode)
		return PlaySound(audiofile, NULL, SND_SYNC|SND_FILENAME|SND_NODEFAULT);

	if(ms_aud_flag && jt_aud_flag)
	{
		if(man_voice)
			return PlayStrAudioMS(string);
		else
			return PlayStrAudioJT(string);
	}
	else if(ms_aud_flag)
		return PlayStrAudioMS(string);
	else
		return PlayStrAudioJT(string);

#else /*SUN_OS*/
	if(single_audio_mode)
	{
		sprintf(string, "%s %s", unix_aud_play_cmd, audiofile);
		system(string);
		return 0;
	}

	sprintf(string, "%s %s%s.wav", unix_aud_play_cmd, AUD_FILE_PATH, aud_str->pt_name);
	system(string); 
	sprintf(string, "%s %s%s.wav", unix_aud_play_cmd, AUD_FILE_PATH, aud_str->status_str);
	system(string);
	SendTraceMsg(0, 0, "%s%s", aud_str->pt_name, aud_str->status_str);
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
	USHORT		rec_ptr, pos,
		fault_ptr,fault_ptr_pos,fault_ptr_f_pos;//chenxs add
	POINTER		point;
	BOOL		faultPt, audAlm;
	BOOL		new_play_flag, fault_play_flag;
 
	int ind_rt_state;//cxs
	int _i=0;//cxs



	/*/just test!!!!!!!!!!!!!!!

	char	igc_def_file[512];
	FILE    *IGC_fpdef;
    char    igctimetest[512];

	sprintf(igc_def_file, "%s%s", SYS_DEFINE_PATH, "AUDALM_TIMETEST.txt");

	//just test!!!!!!!!!!!!!!!*/


	AUD_ALM_TYPE=0;//chenxs
 	AUD_ALM_JUSTPLAY_NUM=-1;//chenxs
	AUD_ALM_READ_ANA_VALUE=1;


	CreateLockFile(".audAlm.lock");
	if(-1==InitSysLog(H9000_MODULE_AUDALM, "AudAlm"))
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

	_station_alm_flag=0;
	ReadStationAlmflagDefFile();

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
	fault_ptr = shm_fault_addr->head.header_ptr;//cxs
	
	if(-1==GetEmsVariable("AUD_ALM_TYPE", &AUD_ALM_TYPE_F))//cxs
		AUD_ALM_TYPE_F= 0;
	AUD_ALM_TYPE=(int)AUD_ALM_TYPE_F;
	
	if(-1==GetEmsVariable("AUD_ALM_JUSTPLAY_NUM", &AUD_ALM_JUSTPLAY_NUM_F))//cxs
		AUD_ALM_JUSTPLAY_NUM_F= -1;
	AUD_ALM_JUSTPLAY_NUM=(int)AUD_ALM_JUSTPLAY_NUM_F;

	if(-1==GetEmsVariable("AUD_ALM_READ_ANA_VALUE", &AUD_ALM_READ_ANA_VALUE_F))//cxs
		AUD_ALM_READ_ANA_VALUE_F= 1;
	AUD_ALM_READ_ANA_VALUE=(int)AUD_ALM_READ_ANA_VALUE_F;
	
	InterLockFileLoad();

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

		if(!IsAudAlmEnable())
		{
			rec_ptr= shm_rec_addr->head.header_ptr;
			continue;
		}
			
		if(AUD_ALM_TYPE==1)//chenxs add
		{
			if(shm_fault_addr->head.count>0)
			{
				for(fault_ptr_pos=0;fault_ptr_pos<shm_fault_addr->head.count;fault_ptr_pos++)
				{
					for(fault_ptr_f_pos=0;fault_ptr_f_pos<shm_fault_addr->head.count;fault_ptr_f_pos++)
					{
						if(shm_fault_addr->rec[fault_ptr_f_pos].ana_rec.type_id!=DPS_ANA_ALM 
							&& shm_fault_addr->rec[fault_ptr_f_pos].ana_rec.type_id!=DPS_IND_ALM)
							continue;
						
						GetPointFromRec(&shm_fault_addr->rec[fault_ptr_f_pos], &point);
						
						if(_station_alm_flag==1)//20120316
						{
							for(_i=1;_i<MAX_STATION_NUM;_i++)
							{
								if(station_alm_flag_index[_i]==point.stn_id)
								{
									_i=-1;
									break;
								}
							}

							if(_i>-1)
								continue;
						}
						
						if(-1==IsPtAudAlmById(&point, &audAlm))
							continue;
						if(!audAlm)
							continue;
						
						if(-1==IsPtFaultById(&point, &audAlm))//fault audAlm
							continue;
						if(!audAlm)
							continue;

						if(-1==IsPtAlmCbaById(&point, &audAlm))//AlmCba audAlm
							continue;
						if(audAlm)
						{
							if(shm_fault_addr->rec[fault_ptr_f_pos].ana_rec.type_id==DPS_IND_ALM)
								ind_rt_state=shm_fault_addr->rec[fault_ptr_f_pos].ind_rec.ind_status.rt_state;
							else
								ind_rt_state=-1;
							if( !GetaudalmCbaState(&point,shm_fault_addr->rec[fault_ptr_f_pos].ana_rec.type_id,ind_rt_state,0) )
									continue;
						}

						PlayAlmRec(&shm_fault_addr->rec[fault_ptr_f_pos], 0);
						if(!IsAudAlmEnable())//cxs
						break;
					}

					if(shm_fault_addr->rec[fault_ptr_pos].ana_rec.type_id!=DPS_ANA_ALM 
						&& shm_fault_addr->rec[fault_ptr_pos].ana_rec.type_id!=DPS_IND_ALM)
						continue;
					
					GetPointFromRec(&shm_fault_addr->rec[fault_ptr_pos], &point);
					if(_station_alm_flag==1)//20120316
					{
						for(_i=1;_i<MAX_STATION_NUM;_i++)
						{
							if(station_alm_flag_index[_i]==point.stn_id)
							{
								_i=-1;
								break;
							}
						}
						
						if(_i>-1)
							continue;
					}

					if(-1==IsPtAudAlmById(&point, &audAlm))
						continue;
					if(!audAlm)
						continue;
				 
					if(-1==IsPtAlmCbaById(&point, &audAlm))//AlmCba audAlm
						continue;
					if(audAlm)
					{
						if(shm_fault_addr->rec[fault_ptr_pos].ana_rec.type_id==DPS_IND_ALM)
							ind_rt_state=shm_fault_addr->rec[fault_ptr_pos].ind_rec.ind_status.rt_state;
						else
							ind_rt_state=-1;
						if( !GetaudalmCbaState(&point,shm_fault_addr->rec[fault_ptr_pos].ana_rec.type_id,ind_rt_state,0) )
							continue;
					}

					PlayAlmRec(&shm_fault_addr->rec[fault_ptr_pos], 0);
					if(!IsAudAlmEnable())//cxs
						break;
				}
			}
			else
				goto AUD_FAULT;

			continue;
		}
		else
		{	
AUD_FAULT:		
		while(1)//OLD FUC
		{
			if(rec_ptr == shm_rec_addr->head.header_ptr)
			{
				if(!PlayFaultRepeatAlm())
					break;
				else
					continue;
			}
			
		 

			new_play_flag= 0;
			fault_play_flag= 0;
			for(pos=rec_ptr; pos!=shm_rec_addr->head.header_ptr; 
			pos++,pos=(pos>=shm_rec_addr->head.buf_size)? 0:pos )
			{


				//chenxs add LS FOR PBG test S
#ifdef	WINDOWS_OS

			
				if(shm_rec_addr->rec[pos].ana_rec.type_id==DPS_SMS)
				{
					char	string[256];
					char	audiofile[256];
				 
					sprintf(audiofile, "%ssingle_alarm.wav", AUD_FILE_PATH);
					sprintf(string, "%s", shm_rec_addr->rec[pos].sms_rec.message);
					
					if(string[0]==0)
						continue;

					FilterAudStr(string);
					Err_log(string);
					
					if(single_audio_mode)
						 PlaySound(audiofile, NULL, SND_SYNC|SND_FILENAME|SND_NODEFAULT);
					
			       if(ms_aud_flag)
						 PlayStrAudioMS(string);
					else
						 PlayStrAudioJT(string);
					continue;
				}
			
#endif
				//chenxs add LS FOR PBG test E



				if(shm_rec_addr->rec[pos].ana_rec.type_id!=DPS_ANA_ALM 
					&& shm_rec_addr->rec[pos].ana_rec.type_id!=DPS_IND_ALM)
					continue;
				
				//chenxs add start
				if(AUD_ALM_TYPE==2)
				{
					if(pos<shm_rec_addr->head.header_ptr)
					{
						if(shm_rec_addr->head.header_ptr-pos>AUD_ALM_JUSTPLAY_NUM)
						{
							pos=shm_rec_addr->head.header_ptr-AUD_ALM_JUSTPLAY_NUM;
							pos=(pos<0)? 0:pos;
							pos=(pos>=shm_rec_addr->head.buf_size)? 0:pos;
							rec_ptr= pos;
						}
					}
					else
					{
						if(shm_rec_addr->head.buf_size-pos+shm_rec_addr->head.header_ptr+1>AUD_ALM_JUSTPLAY_NUM)
						{
							pos=shm_rec_addr->head.buf_size-shm_rec_addr->head.header_ptr+1-AUD_ALM_JUSTPLAY_NUM;
							pos=(pos<0)? 0:pos;
							pos=(pos>=shm_rec_addr->head.buf_size)? 0:pos;
							rec_ptr= pos;
						}	
					}	
				}
				//chenxs add end
				
				GetPointFromRec(&shm_rec_addr->rec[pos], &point);

				if(_station_alm_flag==1)//20120316
				{
					for(_i=1;_i<MAX_STATION_NUM;_i++)
					{
						if(station_alm_flag_index[_i]==point.stn_id)
						{
							_i=-1;
							break;
						}
					}
					
					if(_i>-1)
						continue;
				}

				if(-1==IsPtAudAlmById(&point, &audAlm))
					continue;
				if(!audAlm)
					continue;
				
				if(!IsAlmRecord(&shm_rec_addr->rec[pos]))
					continue;
				
				//interlock  chenxs 20110802 start
				if(-1==IsPtAlmCbaById(&point, &audAlm))//AlmCba audAlm
					continue;
				if(audAlm)
				{
					if(shm_rec_addr->rec[pos].ana_rec.type_id==DPS_IND_ALM)
						ind_rt_state=shm_rec_addr->rec[pos].ind_rec.ind_status.rt_state;
					else
						ind_rt_state=-1;
					if( !GetaudalmCbaState(&point,shm_rec_addr->rec[pos].ana_rec.type_id,ind_rt_state,0) )
						continue;
				}

				//interlock  chenxs 20110802 end
				
				if(!new_play_flag)
				{
					new_play_flag= 1;
					rec_ptr= pos;
				}
				
				IsPtFaultById(&point, &faultPt);
				if(faultPt)
				{
					new_play_flag= 1;
					fault_play_flag= 1;
					rec_ptr= pos;
					break;
				}
				if(!IsAudAlmEnable())//cxs
					break;
			}

			if(!IsAudAlmEnable())//cxs
			{
				rec_ptr= shm_rec_addr->head.header_ptr;
				break;
			}

			/*/just test!!!!!!!!!!!!!!!

			GetPointFromRec(&shm_rec_addr->rec[rec_ptr], &point);
			if((IGC_fpdef=fopen(igc_def_file,"a+"))==NULL)
			{
				printf("igc.c:can't open(w) the error record file %s!\n",igc_def_file);
				fclose(IGC_fpdef);
				
			}

			GetSysTimeStr(igctimetest);
			fprintf (IGC_fpdef,"\naudAlm start POINT:%d.%d.%d.%d time:%s\n",point.stn_id,point.dev_id,point.data_type,point.pt_id,&igctimetest);
			fclose(IGC_fpdef);
			PlayAlmRec(&shm_rec_addr->rec[rec_ptr], 0);
			rec_ptr++;
			if(rec_ptr>=shm_rec_addr->head.buf_size)
				rec_ptr= 0;
			continue;
			//just test!!!!!!!!!!!!!!!*/


			
			if(fault_play_flag)
				PlayAlmRec(&shm_rec_addr->rec[rec_ptr], fault_play_flag);
			else if(!PlayFaultRepeatAlm() && new_play_flag)
				PlayAlmRec(&shm_rec_addr->rec[rec_ptr], fault_play_flag);
			
			rec_ptr++;
			if(rec_ptr>=shm_rec_addr->head.buf_size)
				rec_ptr= 0;

		}
		}
	}
	InterLockFree();
}

//cxs add start 20110912 for audalm
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
		//sprintf(ptNameStr1,"%s.._%d",ptNameStr,ind_rt_state);
		can_alarm =InterLock_Judge( ptNameStr1 );
		if(can_alarm!=-1)
		{
			if(can_alarm==FALSE)
				return can_alarm;
		}
		
	}
	sprintf(ptNameStr1,"%s%s",ptNameStr,AUDALM_CBA_SUFFIX_DEFINE);
	//sprintf(ptNameStr1,"%s.%s..%s",ptName.stnName,ptName.grpName,AUDALM_CBA_SUFFIX_DEFINE);
	
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
 
	/*      打开故障区共享内存   chenxs 20110803 add     */
	shm_fault_addr= (FAULT_RECORD_TABLE*) LinktoShareMem( SHM_FAULT_TABLE_KEY_CHAR );
	if( shm_fault_addr == (FAULT_RECORD_TABLE*)-1 )
	{
		Err_log("Fail to link fault record shm");
		return -1;
	}

	return 0;
}


BOOL	IsAudAlmEnable(void)
{
	char	err_str[128];
	POINTER	pt;
	USHORT	state;
	//added by yyp 2012.12.11
	return TRUE;

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


int	PlayAlmRec(REC_ENTRY_UNION *rec, BOOL fault_play_flag)
{
	ALM_STR		alm_str;
	AUD_ALM_STR	aud_str;
	POINTER		point;

	aud_str.stn_comment[0]= 0;
	aud_str.dev_comment[0]= 0;
	aud_str.entry_longname[0]= 0;
	aud_str.status_str[0]= 0;
	aud_str.pt_name[0]= 0;

	GetPointFromRec(rec, &point);
	GetPtNameById(&point, aud_str.pt_name, NULL);
	GetStnCommentById(point.stn_id, aud_str.stn_comment);
	GetGroupCommentById(point.stn_id, point.dev_id, aud_str.dev_comment);
	GetEntryLongnameById(&point, aud_str.entry_longname);
	GetAlmStrings(rec, &alm_str, NULL, NULL);
	strcpy(aud_str.status_str, alm_str.state_str);
	FilterAudStr(aud_str.status_str);
	if(fault_play_flag)
		InsertPlayList(&aud_str);
	PlayStrAudio(&aud_str, fault_play_flag);
	return 0;
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


BOOL PlayFaultRepeatAlm(void)
{
	int		i, max_times, pos;

	pos= 0;
	max_times= 0;
	for(i=0; i<MAX_PLAY_ENTRY_NUM; i++)
	{
		if(max_times<play_list[i].times)
		{
			max_times= play_list[i].times;
			pos= i;
		}
	}
	
	if(max_times)
	{
		play_list[pos].times--;
		PlayStrAudio(&play_list[pos].aud_str, TRUE);
		return TRUE;
	}
	return FALSE;
}









