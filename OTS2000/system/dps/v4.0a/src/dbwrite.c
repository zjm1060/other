#include	<time.h>


#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include	"../../../pas/v4.0a/inc/sgzy.h"
#include	"../inc/dps_rec_def.h"
#include	"../inc/dps_type_def.h"
#include	"../inc/ipc_common.h"
#include	"../inc/dps_functions_def.h"


int			InitIPC(void);
void		UpdateEventRecFileName(void);
void		UpdateInfoRecFileName(void);
void		UpdateEventTxtFileName(void);
void		CreateNewEventRecFile(void);
void		CreateNewEventTxtFile(void);
void		CreateNewInfoRecFile(void);
int			FindLastLogFileName(void);
int			RestoreEventTableFromFile(void);
int			RestoreInfoTableFromFile(void);
int			DataAreaProcess(void);
int			DealAllSendData( USHORT data_seg_ptr );
int			DealCommandData( USHORT data_seg_ptr );
int			DealAlarmData(char *data_ptr, UCHAR	proc_id);
int			DealNoChangeNoSendData(char *data_ptr);
void		InsertRecToEventTable(REC_ENTRY_UNION *rec_entry);
void		InsertRecToInfoTable(REC_ENTRY_UNION *rec_entry);
void		UpdateEventRecShmHead(void);
void		UpdateInfoRecShmHead(void);
int	        GetAlmCbaState(POINTER *point,int type_id,int ind_rt_state,int qlt_alm_flag);
int			DealLmsData( char *data_ptr );
int			DealSmsData( char *data_ptr, BOOL isAlm );
int			UpdateImpStatistic(POINTER *point, DATE_TIME time, GULONG imp_accum);
void		UpdateImpData(IMP_DATA *imp, int increase);
void		GetImpTimeType( UCHAR stn_id, DATE_TIME time, int *type );
int			FloattoRecentInt(double fValue);
GULONG		UFloattoRecentUInt(double fValue);
void		PowerCmdLogtoFile(REC_ENTRY_UNION *rec_entry);
void *      realarm_rec_do(void *para);//cxs add start
void        clear_should_realarm_rec_entry_by_index(int index_in);//cxs

int				cur_mday;
struct tm		cur_daytime;

SHM_RECORD		*shm_event_rec_addr;
SHM_RECV_DATA	*shm_data_recv_addr;
SGZY_SHM_DATA	*shm_sgzy_addr;
SYS_COMM_TABLE	*shm_syscomm_addr;
INFO_RECORD		*shm_info_rec_addr;

USHORT			data_recv_seg_ptr;
GULONG			event_file_sn;
GULONG			cur_event_rec_sn;
GULONG			cur_info_rec_sn;
BOOL			new_event_rec_flag;
BOOL			new_info_rec_flag;
BOOL			log_to_rec_file_flag;
BOOL			log_to_text_file_flag;
BOOL			powerAdjLogFlag;
char			event_rec_filename[256], event_text_filename[256];
char			info_rec_filename[256];
FILE			*event_rec_file, *event_text_file, *info_rec_file;
int				enable_info_rec_table_flag=0;

int QLT_ALM_TYPE=0;//cxs start
float QLT_ALM_TYPE_F=0; 

#define max_should_realarm_rec_num  6000//add for realarm


struct  
{
	int inited;
	POINTER point[max_should_realarm_rec_num];
	int used[max_should_realarm_rec_num];
	int max_recheck_times[max_should_realarm_rec_num];//20120317
 
	USHORT alm_state[max_should_realarm_rec_num];//20120317
	REC_ENTRY_UNION rec_entry_realarm_union[max_should_realarm_rec_num];
}should_realarm_rec_entry;
//cxs end

/*#define	DEBUG*/

BOOL	IsEventTxtLog(int argc, char *argv[])	
{
	int		i;
	
	for(i=1; i<argc; i++)
	{
		if(!strcmp(argv[i], "-notxt"))
			return FALSE;
	}
	return TRUE;
}

void clear_should_realarm_rec_entry_by_index(int index_in)
{
	if(index_in<0 || index_in>=max_should_realarm_rec_num)
		return;
	
	memset(&should_realarm_rec_entry.rec_entry_realarm_union[index_in],0,sizeof(should_realarm_rec_entry.rec_entry_realarm_union[index_in]));
	should_realarm_rec_entry.max_recheck_times[index_in]=0;
 
	should_realarm_rec_entry.alm_state[index_in]=0;
	should_realarm_rec_entry.point[index_in].data_type=0;
    should_realarm_rec_entry.point[index_in].dev_id=0;
	should_realarm_rec_entry.point[index_in].stn_id=0;
	should_realarm_rec_entry.point[index_in].pt_id=0;
    should_realarm_rec_entry.point[index_in].update_src=0;

	should_realarm_rec_entry.used[index_in]=0;
}

void *    realarm_rec_do(void *para)//cxs add start
{
	int i;
	POINTER point;
	int type_id;
    int ind_rt_state;
	DMS_COMMON	common_all;	//20120317
	for(;;)
	{
		for(i=0;i<max_should_realarm_rec_num;i++)
		{
			if(should_realarm_rec_entry.used[i]==1)
			{	
				should_realarm_rec_entry.max_recheck_times[i]--;//20120317 add start
				if(should_realarm_rec_entry.max_recheck_times[i]<1)
				{
					clear_should_realarm_rec_entry_by_index(i);
					continue;
				}
				//20120317 add end


				point.data_type=should_realarm_rec_entry.point[i].data_type;
			 
				point.stn_id=should_realarm_rec_entry.point[i].stn_id;
				point.dev_id=should_realarm_rec_entry.point[i].dev_id;
				point.pt_id=should_realarm_rec_entry.point[i].pt_id;
				point.update_src=should_realarm_rec_entry.point[i].update_src;


				type_id=should_realarm_rec_entry.rec_entry_realarm_union[i].ana_rec.type_id;

				if(type_id==DPS_IND_ALM)
					ind_rt_state=should_realarm_rec_entry.rec_entry_realarm_union[i].ind_rec.ind_status.rt_state;
				else
					ind_rt_state=-1;

				if( GetAlmCbaState(&point,type_id,ind_rt_state,0) )
				{
					InsertRecToEventTable(&should_realarm_rec_entry.rec_entry_realarm_union[i]);
					clear_should_realarm_rec_entry_by_index(i);
					continue;
				}
			 

				//20120317 add start
				if(type_id== DPS_ANA_ALM)
				{
					if(-1==ReadEntryById(&point , &common_all))
					{
						clear_should_realarm_rec_entry_by_index(i);
						continue;
					}
					if(common_all.var.anlg.value.status.lmt_state!=should_realarm_rec_entry.alm_state[i])
					{
						clear_should_realarm_rec_entry_by_index(i);
						continue;
					}
				}
				else if(type_id== DPS_IND_ALM)
				{
					if(-1==ReadEntryById(&point , &common_all))
					{
						clear_should_realarm_rec_entry_by_index(i);
						continue;
					}
					if(common_all.var.ind.status.rt_state!=should_realarm_rec_entry.alm_state[i])
					{
						clear_should_realarm_rec_entry_by_index(i);
						continue;
					}
					
				}
				else
				{
					clear_should_realarm_rec_entry_by_index(i);
					continue;
				}
				//20120317 add end
			}
		}
		sleep(10);	
	}
}



int	main(int argc, char *argv[])
{
	char		err_buf[256];
	time_t		time_tt;

	CreateLockFile(".dbwrite.lock");
	if(-1==InitSysLog(H9000_MODULE_DBWRITE, "Dbwrite"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}

	//cxs
#ifdef	WINDOWS_OS
	
#else
	pthread_t	h_thread;
#endif	//cxs


	if(-1==MountDB(NULL))
	{
		Err_log("Fail to MountDB!\n");
		exit(-1);
	}

	if(-1==InterLockFileLoad())
	{
		Err_log("Fail to Load InterLock File!\n");
		exit(-1);
	}
	
	if(-1==InitIPC())
	{
		Err_log("Fail to initialize IPCs!\n");
		exit(-1);
	}

	if(-1==InitStrTextDefine())
	{
		Err_log("Fail to Init str text define!\n");
		exit(-1);
	}

	enable_info_rec_table_flag= IsInfoRecTable();

	cur_event_rec_sn= 0;
	event_file_sn= 1;

	if(IsEventFileLog())
		log_to_rec_file_flag= TRUE;
	else
		log_to_rec_file_flag= FALSE;

	if(IsEventTxtLog(argc, argv))
		log_to_text_file_flag= TRUE;
	else
		log_to_text_file_flag= FALSE;
		
	if(IsHostNoHardDisk()) /*无硬盘*/
	{
		log_to_rec_file_flag= FALSE;
		log_to_text_file_flag= FALSE;
	}

	time_tt= time(NULL);
	cur_daytime= *localtime(&time_tt);
	cur_mday= cur_daytime.tm_mday;

	if(log_to_rec_file_flag)
	{
		RestoreEventTableFromFile();
		if((event_rec_file = fopen(event_rec_filename, "ab+"))==NULL)
		{
			sprintf(err_buf, "Failed to open(create) %s", event_rec_filename);
			Err_log(err_buf);
		}
		else
			setbuf(event_rec_file, NULL);

		RestoreInfoTableFromFile();
		if((info_rec_file = fopen(info_rec_filename, "ab+"))==NULL)
		{
			sprintf(err_buf, "Failed to open(create) %s", info_rec_filename);
			Err_log(err_buf);
		}
		else
			setbuf(info_rec_file, NULL);
	}
	if(log_to_text_file_flag)
	{
		UpdateEventTxtFileName();
		if((event_text_file = fopen(event_text_filename, "a+"))==NULL)
		{
			sprintf(err_buf, "Failed to open(create) %s", event_text_filename);
			Err_log(err_buf);
		}
		else
			setbuf(event_text_file, NULL);
	}


	if(-1==GetEmsVariable("QLT_ALM_TYPE", &QLT_ALM_TYPE_F))//cxs
		QLT_ALM_TYPE_F= 0;
	QLT_ALM_TYPE=(int)QLT_ALM_TYPE_F;//cxs


	powerAdjLogFlag= IsPowerAdjLog();

	data_recv_seg_ptr= shm_data_recv_addr->head;
	
	
	//cxs add start
#ifdef	WINDOWS_OS
	if(!CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)realarm_rec_do, 0, 0, NULL))
#else
		if(pthread_create(&h_thread, NULL, realarm_rec_do, NULL))
#endif
		{
			printf("\nFail to Create Dbwrite Thread for realarm\n");
			Err_log("Fail to Create Dbwrite Thread for realarm!\n");
			SendTraceMsg(0, 0, "Fail to Create Dbwrite Thread for realarm!");
		}
		else
			SendTraceMsg(0, 0, "Succeed to Create Dbwrite Thread for realarm!");
		//cxs add end
		
		for( ;; )
	{
		if(WaitforSem(SEM_RECVDATA, -1) == -1)
		{
			Err_log("Fail to wait recv_data semaphore!\n");
			exit(-1);
		}

#ifdef	DEBUG
		printf("\nDPS: recv data sem arrive");
#endif

		time_tt= time(NULL);
		cur_daytime= *localtime(&time_tt);
		if(cur_daytime.tm_mday!= cur_mday)
		{
			cur_mday= cur_daytime.tm_mday;
			cur_event_rec_sn= 0;
			event_file_sn= 1;
			CreateNewEventRecFile();
			CreateNewEventTxtFile();
			CreateNewInfoRecFile();
		}
		
		new_event_rec_flag= 0;
		new_info_rec_flag= 0;
		DataAreaProcess();
		
		if(new_event_rec_flag)
			ReleaseReadSem(SEM_RECORD);
		if(new_info_rec_flag)
			ReleaseReadSem(SEM_INFO_REC);
	}

}



int InitIPC()
{
	/*      打开事件记录区共享内存      */
	shm_event_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_event_rec_addr == (SHM_RECORD*)-1 )
	{
		Err_log("Fail to link event record shm");
		return -1;
	}

	
	/*      打开数据区共享内存    */
	shm_data_recv_addr= (SHM_RECV_DATA*) LinktoShareMem( SHM_RECV_KEY_CHAR );
	if( shm_data_recv_addr == (SHM_RECV_DATA*)-1 )
	{
		Err_log("Fail to link data_recv shm");
		return -1;
	}

	/*      打开事故追忆共享内存    */
	shm_sgzy_addr= (SGZY_SHM_DATA*) LinktoShareMem( SHM_SGZY_KEY_CHAR );
	if( shm_sgzy_addr == (SGZY_SHM_DATA*)-1 )
	{
		Err_log("Fail to link sgzy shm");
		return -1;
	}

	/*      打开系统通信共享内存    */
	shm_syscomm_addr= (SYS_COMM_TABLE*) LinktoShareMem( SHM_SYSCOMM_TABLE_KEY_CHAR );
	if( shm_syscomm_addr == (SYS_COMM_TABLE*)-1 )
	{
		Err_log("Fail to link sys_comm shm");
		return -1;
	}

	/*      打开信息记录区共享内存    */
	shm_info_rec_addr= (INFO_RECORD*) LinktoShareMem( SHM_INFO_RECORD_KEY_CHAR );
	if( shm_info_rec_addr == (INFO_RECORD*)-1 )
	{
		Err_log("Fail to link info record shm");
		return -1;
	}

	/*      打开数据区信号量      */
	if( LinktoSemSet(SEM_RECVDATA) == -1 )
	{
		Err_log("Fail to link data_recv semaphore\n");
		return -1;
	}
	
	/*      打开事件记录区信号量        */
	if( LinktoSemSet(SEM_RECORD) == -1 )
	{
		Err_log("Fail to link record semaphore\n");
		return -1;
	}

	/*      打开信息记录区信号量        */
	if( LinktoSemSet(SEM_INFO_REC) == -1 )
	{
		Err_log("Fail to link info record semaphore\n");
		return -1;
	}

	/*      打开事故追忆信号量        */
	if( LinktoSemSet(SEM_SGZY) == -1 )
	{
		Err_log("Fail to link sgzy semaphore\n");
		return -1;
	}
	memset(&should_realarm_rec_entry,0,sizeof(should_realarm_rec_entry));//cxs
	should_realarm_rec_entry.inited=1;
	return 0;
}


void UpdateEventRecFileName(void)
{
#ifdef ENGLISH_VER
	sprintf( event_rec_filename,"%sEVENT%02d-%s-%04dNo%02d.REC", 
		EVENT_LOG_PATH, cur_daytime.tm_mday, mthstr[cur_daytime.tm_mon],cur_daytime.tm_year+1900, event_file_sn);
#else                
	sprintf( event_rec_filename,"%sEVENT%04d%02d%02dNo%02d.REC", 
		EVENT_LOG_PATH, cur_daytime.tm_year+1900,cur_daytime.tm_mon+1,cur_daytime.tm_mday, event_file_sn);
#endif
}


void UpdateInfoRecFileName(void)
{
#ifdef ENGLISH_VER
	sprintf( info_rec_filename,"%sINFO%02d-%s-%04d.REC", 
		EVENT_LOG_PATH, cur_daytime.tm_mday, mthstr[cur_daytime.tm_mon],cur_daytime.tm_year+1900);
#else                
	sprintf( info_rec_filename,"%sINFO%04d%02d%02d.REC", 
		EVENT_LOG_PATH, cur_daytime.tm_year+1900,cur_daytime.tm_mon+1,cur_daytime.tm_mday);
#endif
}



void UpdateEventTxtFileName(void)
{
#ifdef ENGLISH_VER
	sprintf( event_text_filename,"%sEVENT%02d-%s-%04d.txt", 
		EVENT_TEXT_LOG_PATH, cur_daytime.tm_mday, mthstr[cur_daytime.tm_mon],cur_daytime.tm_year+1900);
#else                
	sprintf( event_text_filename,"%sEVENT%04d%02d%02d.txt", 
		EVENT_TEXT_LOG_PATH, cur_daytime.tm_year+1900,cur_daytime.tm_mon+1,cur_daytime.tm_mday);
#endif
}


void CreateNewEventRecFile(void)
{
	char	err_buf[256];

	if(log_to_rec_file_flag)
	{
		UpdateEventRecFileName();

		if(event_rec_file)
			fclose(event_rec_file);
		if((event_rec_file = fopen(event_rec_filename, "wb+"))==NULL)
		{
			sprintf(err_buf, "Failed to open(create) %s", event_rec_filename);
			Err_log(err_buf);
		}
		else
			setbuf(event_rec_file, NULL);
	}
}


void CreateNewEventTxtFile(void)
{
	char	err_buf[256];

	if(log_to_text_file_flag)
	{
		UpdateEventTxtFileName();

		if(event_text_file)
			fclose(event_text_file);
		if((event_text_file = fopen(event_text_filename, "w+"))==NULL)
		{
			sprintf(err_buf, "Failed to open(create) %s", event_text_filename);
			Err_log(err_buf);
		}
		else
			setbuf(event_text_file, NULL);
	}
}


void CreateNewInfoRecFile(void)
{
	char	err_buf[256];

	if(log_to_rec_file_flag)
	{
		UpdateInfoRecFileName();

		if(info_rec_file)
			fclose(info_rec_file);
		if((info_rec_file = fopen(info_rec_filename, "wb+"))==NULL)
		{
			sprintf(err_buf, "Failed to open(create) %s", info_rec_filename);
			Err_log(err_buf);
		}
		else
			setbuf(info_rec_file, NULL);
	}
}



int RestoreEventTableFromFile(void)
{
	long		file_size;
	FILE		*fp;
	char		err_buf[256];

	event_file_sn = 0;
	
	while(1)
	{
		event_file_sn++;
		UpdateEventRecFileName();
		if((fp= fopen(event_rec_filename, "a+"))==NULL)
			return -1;

		fseek(fp,0L,2);
		file_size= ftell(fp);
		fclose(fp);
		if(file_size%REC_ENTRY_SIZE) /*file size error*/
		{
			sprintf(err_buf, "File %s length error %d", event_rec_filename, file_size);
			Err_log(err_buf);
			
			/*illegal file will be truncated*/
			if((fp= fopen(event_rec_filename, "wb"))==NULL)
			{
				sprintf(err_buf, "Failed to open(create) %s", event_rec_filename);
				Err_log(err_buf);
			}
			else
				fclose(fp);
			break;
		}

		if(file_size >= REC_ENTRY_SIZE*MAX_FILE_RECORD_NUM) /*file full size*/
			continue; /*continue to check next file*/
		else /*file_size==0*/
			break; /*find the last file*/
	}


	if(shm_event_rec_addr->head.count>0)
		cur_event_rec_sn= shm_event_rec_addr->rec[MAX_SHM_RECORD_NUM-1].ana_rec.rec_sn;

	UpdateEventRecFileName();

	return 0;
}


int RestoreInfoTableFromFile(void)
{
	long		file_size;
	FILE		*fp;
	char		err_buf[256];

	UpdateInfoRecFileName();
	if((fp= fopen(info_rec_filename, "a+"))==NULL)
		return -1;

	fseek(fp,0L,2);
	file_size= ftell(fp);
	fclose(fp);
	if(file_size%REC_ENTRY_SIZE) /*file size error*/
	{
		sprintf(err_buf, "File %s length error %d", info_rec_filename, file_size);
		Err_log(err_buf);
		
		/*illegal file will be truncated*/
		if((fp= fopen(info_rec_filename, "wb"))==NULL)
		{
			sprintf(err_buf, "Failed to open(create) %s", info_rec_filename);
			Err_log(err_buf);
		}
		else
			fclose(fp);
		return -1;
	}

	if(shm_info_rec_addr->head.count>0)
		cur_info_rec_sn= shm_info_rec_addr->rec[shm_info_rec_addr->head.count-1].ana_rec.rec_sn;

	return 0;
}


int	DataAreaProcess(void)
{
	char		err_buf[256];
	USHORT		data_length, data_num, point_num;
	UCHAR		dp_type, dest_id, src_id, type_id, proc_id;
	USHORT		length_factor;
	char		*data_ptr;
	USHORT		seg_ptr;		

#ifdef	DEBUG
	printf("\nDPS: shm head= %d, ptr= %d", shm_data_recv_addr->head, data_recv_seg_ptr);
#endif

	while(shm_data_recv_addr->head != data_recv_seg_ptr)
	{
#ifdef	DEBUG
		printf("\nDPS: shm head= %d, ptr= %d", shm_data_recv_addr->head, data_recv_seg_ptr);
#endif

		seg_ptr= data_recv_seg_ptr++;
		if(data_recv_seg_ptr >= MAX_RECV_DATA_SEG_NUM)
			data_recv_seg_ptr= 0;
		
		memcpy(&data_length, &shm_data_recv_addr->shm_data_seg[seg_ptr].lan_head.length, 2);
		dp_type		= shm_data_recv_addr->shm_data_seg[seg_ptr].lan_head.dp_type;
		dest_id		= shm_data_recv_addr->shm_data_seg[seg_ptr].lan_head.dest_id;
		src_id		= shm_data_recv_addr->shm_data_seg[seg_ptr].lan_head.src_id;
		proc_id		= shm_data_recv_addr->shm_data_seg[seg_ptr].lan_head.dest_stn;
		
		Swap2Byte(&data_length);
		if( data_length==0 )	
			continue;

		if( data_length>SHM_DATA_SEG_SIZE )	
		{
			sprintf(err_buf, "datagram length overrange %d %d %d", data_length, seg_ptr, dp_type);
			Err_log(err_buf);
			continue;
		}
		
		/********  data_single processing  ************/
		if( dp_type==BJ_DB_TIME )
		{
			for( data_num=0; data_num<data_length; )
			{
				if((data_length-data_num)<6)
				{
					sprintf(err_buf, "length error in alm datagram: length=%d, dp_type=%d, src_id=%d, proc_id=%d", 
						data_length, dp_type, src_id, proc_id);
					Err_log(err_buf);
					break;
				}

				data_ptr= &shm_data_recv_addr->shm_data_seg[seg_ptr].data[data_num];
				type_id= shm_data_recv_addr->shm_data_seg[seg_ptr].data[data_num];

				/********  ana_imp_data_alm processing  ************/
				if( type_id==DPS_ANA_ALM || type_id==DPS_IMP_ALM )
				{
					length_factor= 8;
					memcpy(&point_num, &shm_data_recv_addr->shm_data_seg[seg_ptr].data[data_num+4], 2);
					Swap2Byte(&point_num);
					data_num = data_num+14+length_factor*point_num;
					if(data_num>data_length)
					{
						sprintf(err_buf, "length error in alm datagram: length=%d, dp_type=%d, src_id=%d, proc_id=%d, type_id=%d",
							data_length, dp_type, src_id, proc_id, type_id);
						Err_log(err_buf);
						break;
					}
					
					DealAlarmData(data_ptr, proc_id);
				}

				/********  ind_data_alm processing  ************/
				else if( type_id==DPS_IND_ALM )
				{
					length_factor= 4;
					memcpy(&point_num, &shm_data_recv_addr->shm_data_seg[seg_ptr].data[data_num+4], 2);
					Swap2Byte(&point_num);
					data_num = data_num+16+length_factor*point_num;
					if(data_num>data_length)
					{
						sprintf(err_buf, "length error in alm datagram: length=%d, dp_type=%d, src_id=%d, proc_id=%d, type_id=%d",
							data_length, dp_type, src_id, proc_id, type_id);
						Err_log(err_buf);
						break;
					}

					DealAlarmData(data_ptr, proc_id);
				}

				/********  no_change_no_send data processing  ********/
				else if( type_id==DPS_ANALOG || type_id==DPS_IMPULSE 
					|| type_id==DPS_CURVE ) 
				{
					length_factor= 8;
					memcpy(&point_num, &shm_data_recv_addr->shm_data_seg[seg_ptr].data[data_num+4], 2);
					Swap2Byte(&point_num);
					data_num = data_num+14+length_factor*point_num;
					if(data_num>data_length)
					{
						sprintf(err_buf, "length error in alm datagram: length=%d, dp_type=%d, src_id=%d, proc_id=%d, type_id=%d",
							data_length, dp_type, src_id, proc_id, type_id);
						Err_log(err_buf);
						break;
					}

					DealNoChangeNoSendData(data_ptr);
				}
				else if( type_id==DPS_DEV_FB ) 
				{
					length_factor= 8;
					memcpy(&point_num, &shm_data_recv_addr->shm_data_seg[seg_ptr].data[data_num+4], 2);
					Swap2Byte(&point_num);
					data_num = data_num+16+length_factor*point_num;
					if(data_num>data_length)
					{
						sprintf(err_buf, "length error in alm datagram: length=%d, dp_type=%d, src_id=%d, proc_id=%d, type_id=%d",
							data_length, dp_type, src_id, proc_id, type_id);
						Err_log(err_buf);
						break;
					}

					DealNoChangeNoSendData(data_ptr);
				}

				/********  LMS data processing  ********/
				else if( type_id==DPS_LMS ) 
				{
					data_num = data_num+12;
					if(data_num>data_length)
					{
						sprintf(err_buf, "length error in alm datagram: length=%d, dp_type=%d, src_id=%d, proc_id=%d, type_id=%d",
							data_length, dp_type, src_id, proc_id, type_id);
						Err_log(err_buf);
						break;
					}

					DealLmsData(data_ptr);
				}

				/********  LMS data processing  ********/
				else if( type_id==DPS_SMS || type_id==DPS_SM_ALM ) 
				{
					data_num = data_num+32;
					if(data_num>data_length)
					{
						sprintf(err_buf, "length error in alm datagram: length=%d, dp_type=%d, src_id=%d, proc_id=%d, type_id=%d",
							data_length, dp_type, src_id, proc_id, type_id);
						Err_log(err_buf);
						break;
					}

					if(type_id==DPS_SMS)
						DealSmsData(data_ptr, FALSE);
					else
						DealSmsData(data_ptr, TRUE);
				}

				else
				{
					sprintf(err_buf, "Unsupported type %d in alm datagram", type_id);
					Err_log(err_buf);
					break;
				}

			}/*for()*/
		}
		
		/********  data_all processing  ********/
		else if( dp_type==ALL_DB_TIME )
			DealAllSendData( seg_ptr );	

		/********  command data processing  ********/
		else if( dp_type==COMM_RECORD_TIME||dp_type==COMMB_TIME ) 
			DealCommandData( seg_ptr );						


	}/*while()*/

	return 0;
}



int	DealLmsData( char *data_ptr )
{
	REC_ENTRY_UNION	rec_entry;

	memcpy(&rec_entry.lms_rec.type_id, data_ptr, 12);
	InsertRecToEventTable(&rec_entry);
	return 0;
}


int	DealSmsData( char *data_ptr, BOOL isAlm )
{
	REC_ENTRY_UNION	rec_entry;

	memcpy(&rec_entry.sms_rec.type_id, data_ptr, 32);
	if(!isAlm && enable_info_rec_table_flag)
		InsertRecToInfoTable(&rec_entry);
	else
		InsertRecToEventTable(&rec_entry);
	return 0;
}


int	DealAllSendData( USHORT data_seg_ptr )
{
	char		err_buf[256];
	POINTER		point;
	USHORT		data_length, data_num;
	USHORT		point_ptr, number;
	USHORT		length_factor, length_fix;
	DATE_TIME	time;
	char		*data_ptr;
	UCHAR		host_id, type_id;
	WR_VAR_UNION	new_value;
	IOST		iost={0};
	USHORT		scanEnable;
	USHORT		seg_offset;
	FLOAT		curve_value[MAX_CURVE_SEG_NUM];
	DMS_COMMON	common_all;

		
	data_length	= shm_data_recv_addr->shm_data_seg[data_seg_ptr].lan_head.length;
	Swap2Byte(&data_length);

	for( data_num=0; data_num<data_length; )
	{
		if((data_length-data_num)<8)
		{
			sprintf(err_buf, "length error in all_send datagram");
			Err_log(err_buf);
			return -1;
		}

		data_ptr= &shm_data_recv_addr->shm_data_seg[data_seg_ptr].data[data_num];
		type_id			= *data_ptr;
		point.stn_id	= *(data_ptr+1);
		point.dev_id	= *(data_ptr+2);
		point.data_type	= *(data_ptr+3);
		memcpy(&number, data_ptr+4, 2);
		Swap2Byte(&number);
		memcpy(&point.pt_id, data_ptr+6, 2);
		Swap2Byte(&point.pt_id);

		if(type_id==DPS_ANA_ALL||type_id==DPS_IMP_ALL)
		{
			length_factor= 6;
			length_fix= 16;
		}
		else if(type_id==DPS_STATUS_ALL)
		{
			length_factor= 2;
			length_fix= 16;
		}
		else if(type_id==DPS_CURVE_ALL)
		{
			length_factor= 4;
			length_fix= 18;
		}
		else if(type_id==DPS_DEV_FB_ALL)
		{
			length_factor= 6;
			length_fix= 16;
		}
		else if(type_id==DPS_TIME_ALL)
		{
			length_factor= 8;
			length_fix= 16;
		}
		else
		{
			sprintf(err_buf, "Unsupported type %d in all_send datagram", type_id);
			Err_log(err_buf);
			return -1;
		}
		
		data_num = data_num+length_fix+length_factor*number;
		if(data_num>data_length)
		{
			sprintf(err_buf, "length error in all_send datagram");
			Err_log(err_buf);
			return -1;
		}
		
		data_ptr= &shm_data_recv_addr->shm_data_seg[data_seg_ptr].data[data_num-8];
		host_id		= *(data_ptr);
		point.update_src= host_id;
		time.second	= *(data_ptr+1);
		time.minute	= *(data_ptr+2);
		time.hour	= *(data_ptr+3);
		time.day	= *(data_ptr+4);
		time.month	= *(data_ptr+5);
		time.ms		= 0;
		memcpy(&time.year, data_ptr+6, 2);
		Swap2Byte(&time.year);
		Swap2Byte(&time.ms);

#ifdef	DEBUG
		printf("\n%04d-%02d-%02d %02d:%02d:%02d type_id= %d, data_length= %d", 
			time.year, time.month, time.day, time.hour,
			time.minute, time.second, type_id, data_length);
#endif

		data_ptr= &shm_data_recv_addr->shm_data_seg[data_seg_ptr].data[data_num-length_factor*number-8];
		switch( type_id )
		{
		case	DPS_ANA_ALL:			 
			for(point_ptr=0; point_ptr<number; point_ptr++,point.pt_id++,data_ptr += length_factor)
			{
				if(-1==IsPtScanEnableById(&point, &scanEnable))
				{
					sprintf(err_buf, "No such point %d-%d-%d-%d in db",
						point.stn_id, point.dev_id, point.data_type, point.pt_id);
					Err_log(err_buf);
					break;
				}
				
				if(scanEnable)
				{
					memcpy(&new_value.anlg.status, data_ptr, 2);
					memcpy(&new_value.anlg.data, data_ptr+2, 4);
					Swap2Byte(&new_value.anlg.status);
					Swap4Byte(&new_value.anlg.data);
					iost.noUpdate= 0;
					SetPtValueIostById( &point, new_value, iost, time, RW_UPDATE_TIME
 						|RW_RT_VALUE|RW_UPDATED|RW_LMT_STATE|RW_TREND_STATE
 						|RW_CHAN_FAIL|RW_CTRL_BLOCK|RW_OVERFLOW );
				}
			}
			break;

		case	DPS_STATUS_ALL:			
			for(point_ptr=0; point_ptr<number; point_ptr++,point.pt_id++,data_ptr += length_factor)
			{
				if(-1==IsPtScanEnableById(&point, &scanEnable))
				{
					sprintf(err_buf, "No such point %d-%d-%d-%d in db",
						point.stn_id, point.dev_id, point.data_type, point.pt_id);
					Err_log(err_buf);
					break;
				}
				
				if(scanEnable)
				{
					memcpy(&new_value.ind, data_ptr, 2);
					Swap2Byte(&new_value.ind);
					iost.noUpdate= 0;
					SetPtValueIostById( &point, new_value, iost, time, 
						RW_UPDATE_TIME|RW_RT_VALUE|RW_UPDATED|RW_CHAN_FAIL|RW_MAN_SET );
				}				
			}
			break;

		case	DPS_IMP_ALL:			 
			for(point_ptr=0; point_ptr<number; point_ptr++,point.pt_id++,data_ptr += length_factor)
			{
				if(-1==IsPtScanEnableById(&point, &scanEnable))
				{
					sprintf(err_buf, "No such point %d-%d-%d-%d in db",
						point.stn_id, point.dev_id, point.data_type, point.pt_id);
					Err_log(err_buf);
					break;
				}
				
				if(scanEnable)
				{
					memcpy(&new_value.imp.status, data_ptr, 2);
					Swap2Byte(&new_value.imp.status);
					memcpy(&new_value.imp.iValue, data_ptr+2, 4);
					Swap4Byte(&new_value.imp.iValue);

					iost.noUpdate= 0;
					if(RW_IMP_ACCUM==new_value.imp.status.option)
						UpdateImpStatistic(&point, time, new_value.imp.iValue);
					SetPtValueIostById( &point, new_value, iost, time, 
						RW_UPDATE_TIME|RW_RT_VALUE|RW_UPDATED|RW_CHAN_FAIL);//|RW_MAN_SET );
				}
			}
			break;

		case	DPS_CURVE_ALL:
			memcpy(&seg_offset, data_ptr-2, 2);
			Swap2Byte(&seg_offset);
			
			if((seg_offset+number)>=MAX_CURVE_SEG_NUM)
			{
				sprintf(err_buf, "Type %d curve error: offset= %d, num= %d", type_id, seg_offset, number);
				Err_log(err_buf);
				break;
			}

			memcpy(&curve_value, data_ptr, number*4);
			for(point_ptr=0; point_ptr<number; point_ptr++)
				Swap4Byte(&curve_value[point_ptr]);

			SetCurvePtDataById( &point, curve_value, seg_offset, number, time );
			iost.noUpdate= 0;
			SetPtValueIostById( &point, new_value, iost, time, RW_UPDATE_TIME|RW_UPDATED );
			break;

		case	DPS_DEV_FB_ALL:			 
			for(point_ptr=0; point_ptr<number; point_ptr++,point.pt_id++,data_ptr += length_factor)
			{
				if(-1==IsPtScanEnableById(&point, &scanEnable))
				{
					sprintf(err_buf, "No such point %d-%d-%d-%d in db",
						point.stn_id, point.dev_id, point.data_type, point.pt_id);
					Err_log(err_buf);
					break;
				}
				
				if(scanEnable)
				{
					ReadEntryById(&point, &common_all);
					memcpy(&common_all.var.devobj.fb_status1, data_ptr, 2);
					Swap2Byte(&common_all.var.devobj.fb_status1);
					memcpy(&common_all.var.devobj.fb_status2, data_ptr+2, 2);
					Swap2Byte(&common_all.var.devobj.fb_status2);
					memcpy(&common_all.var.devobj.param, data_ptr+4, 2);
					Swap2Byte(&common_all.var.devobj.param);
					SetDevObjPtVarById( &point, common_all.var.devobj );
					iost.noUpdate= 0;
					SetPtValueIostById( &point, new_value, iost, time, RW_UPDATE_TIME|RW_UPDATED );
				}
			}
			break;

		case	DPS_TIME_ALL:			 
			for(point_ptr=0; point_ptr<number; point_ptr++,point.pt_id++,data_ptr += length_factor)
			{
				if(-1==IsPtScanEnableById(&point, &scanEnable))
				{
					sprintf(err_buf, "No such point %d-%d-%d-%d in db",
						point.stn_id, point.dev_id, point.data_type, point.pt_id);
					Err_log(err_buf);
					break;
				}
				
				if(scanEnable)
				{
					ReadEntryById(&point, &common_all);
					memcpy(&common_all.var.time.status, data_ptr, 2);
					Swap2Byte(&common_all.var.time.status);
					memcpy(&common_all.var.time.value_sec, data_ptr+2, 4);
					Swap4Byte(&common_all.var.time.value_sec);
					memcpy(&common_all.var.time.value_ms, data_ptr+6, 2);
					Swap2Byte(&common_all.var.time.value_ms);
					SetTimePtVarById(&point, common_all.var.time );
					iost.noUpdate= 0;
					SetPtValueIostById( &point, new_value, iost, time, RW_UPDATE_TIME|RW_UPDATED );
				}
			}
			break;
			
		default:
			sprintf(err_buf, "Unsupported type %d in allSend datagram", type_id);
			Err_log(err_buf);
			break;
		}/*switch( type_id )*/
	}/*for()*/

	return 0;
}


int	DealNoChangeNoSendData(char *data_ptr)
{
	char		err_buf[256];
	POINTER		point;
	USHORT		point_num, point_ptr;
	DATE_TIME	time;
	USHORT		length_factor;
	UCHAR		host_id, type_id;
	WR_VAR_UNION	new_value={0};
	IOST		iost={0};
	FLOAT		fData;
	USHORT		seg_offset;
	DMS_COMMON	common_all;
	OBJ_REC		obj_rec;

	type_id			= *data_ptr;
	point.stn_id	= *(data_ptr+1);
	point.dev_id	= *(data_ptr+2);
	point.data_type	= *(data_ptr+3);
	memcpy(&point_num, data_ptr+4, 2);
	Swap2Byte(&point_num);

	
	if(type_id==DPS_DEV_FB)
		memcpy(&obj_rec.type_id, data_ptr, 4);	/*type_id, stn_id, dev_id, data_type*/

	length_factor= 8;
	data_ptr += (length_factor*point_num+6);
	if(type_id==DPS_DEV_FB)
	{
		memcpy(&time.ms, data_ptr, 2);
		Swap2Byte(&time.ms);
		host_id		= *(data_ptr+2);
		time.second	= *(data_ptr+3);
		time.minute	= *(data_ptr+4);
		time.hour	= *(data_ptr+5);
		time.day	= *(data_ptr+6);
		time.month	= *(data_ptr+7);
		memcpy(&time.year, data_ptr+8, 2);
		Swap2Byte(&time.year);

		memcpy(&obj_rec.ms, data_ptr, 2);	/*ms*/
		memcpy(&obj_rec.host_id, data_ptr+2, 8);	/*host_id, time*/
	}
	else
	{
		time.ms		= 0;
		host_id		= *(data_ptr);
		time.second	= *(data_ptr+1);
		time.minute	= *(data_ptr+2);
		time.hour	= *(data_ptr+3);
		time.day	= *(data_ptr+4);
		time.month	= *(data_ptr+5);
		memcpy(&time.year, data_ptr+6, 2);
		Swap2Byte(&time.year);
	}

	point.update_src= host_id;


#ifdef	DEBUG
		printf("\n%04d-%02d-%02d %02d:%02d:%02d:%03d type_id= %d", 
			time.year, time.month, time.day, time.hour,
			time.minute, time.second, time.ms, type_id);
#endif

	data_ptr -= length_factor*point_num;
	switch( type_id )
	{
	case	DPS_ANALOG:			 
		for(point_ptr=0; point_ptr<point_num; point_ptr++,data_ptr += length_factor)
		{
			memcpy(&point.pt_id, data_ptr, 2);
			Swap2Byte(&point.pt_id);

			memcpy(&new_value.anlg.status, data_ptr+2, 2);
			memcpy(&new_value.anlg.data, data_ptr+4, 4);
			Swap2Byte(&new_value.anlg.status);
			Swap4Byte(&new_value.anlg.data);
			iost.noUpdate= 0;
			iost.scanEnable= 1;
			if(-1==SetPtValueIostById( &point, new_value, iost, time, 
				RW_UPDATE_TIME|RW_RT_VALUE|RW_UPDATED|RW_SCAN_ENABLE|RW_LMT_STATE
				|RW_TREND_STATE|RW_CHAN_FAIL|RW_CTRL_BLOCK|RW_OVERFLOW))
			{
				sprintf(err_buf, "No such point %d-%d-%d-%d in db",
					point.stn_id, point.dev_id, point.data_type, point.pt_id);
				Err_log(err_buf);
				continue;
			}
		}
		break;

	case	DPS_IMPULSE:			 
		for(point_ptr=0; point_ptr<point_num; point_ptr++,data_ptr += length_factor)
		{
			memcpy(&point.pt_id, data_ptr, 2);
			Swap2Byte(&point.pt_id);

			memcpy(&new_value.imp.status, data_ptr+2, 2);
			Swap2Byte(&new_value.imp.status);
			memcpy(&new_value.imp.iValue, data_ptr+4, 4);
			Swap4Byte(&new_value.imp.iValue);

			iost.noUpdate= 0;
			iost.scanEnable= 1;
			if(RW_IMP_ACCUM==new_value.imp.status.option)
				UpdateImpStatistic(&point, time, new_value.imp.iValue);
			if(-1==SetPtValueIostById( &point, new_value, iost, time, 
				RW_UPDATE_TIME|RW_RT_VALUE|RW_UPDATED|RW_SCAN_ENABLE
				|RW_CHAN_FAIL|RW_MAN_SET))
			{
				sprintf(err_buf, "No such point %d-%d-%d-%d in db",
					point.stn_id, point.dev_id, point.data_type, point.pt_id);
				Err_log(err_buf);
				continue;
			}
		}
		break;

	case	DPS_CURVE:			 
		for(point_ptr=0; point_ptr<point_num; point_ptr++,data_ptr += length_factor)
		{
			memcpy(&point.pt_id, data_ptr, 2);
			Swap2Byte(&point.pt_id);

			memcpy(&seg_offset, data_ptr+2, 2);
			Swap2Byte(&seg_offset);
			memcpy(&fData, data_ptr+4, 4);
			Swap4Byte(&fData);
			if( seg_offset>=MAX_CURVE_SEG_NUM )
			{
				sprintf(err_buf, "Type_id %d curve offset error: %d", type_id, seg_offset);
				Err_log(err_buf);
				continue;
			}
			if(-1==SetCurvePtDataById( &point, &fData, seg_offset, 1, time ))
			{
				sprintf(err_buf, "No such point %d-%d-%d-%d in db",
					point.stn_id, point.dev_id, point.data_type, point.pt_id);
				Err_log(err_buf);
				continue;
			}
			iost.noUpdate= 0;
			iost.scanEnable= 1;
			SetPtValueIostById( &point, new_value, iost, time, RW_UPDATE_TIME|RW_UPDATED );
		}
		break;

	case	DPS_DEV_FB:			 
		for(point_ptr=0; point_ptr<point_num; point_ptr++,data_ptr += length_factor)
		{
			memcpy(&point.pt_id, data_ptr, 2);
			Swap2Byte(&point.pt_id);

			if(-1==ReadEntryById(&point, &common_all))
			{
				sprintf(err_buf, "No such point %d-%d-%d-%d in db",
					point.stn_id, point.dev_id, point.data_type, point.pt_id);
				Err_log(err_buf);
				continue;
			}

			memcpy(&common_all.var.devobj.fb_status1, data_ptr+2, 2);
			Swap2Byte(&common_all.var.devobj.fb_status1);
			memcpy(&common_all.var.devobj.fb_status2, data_ptr+4, 2);
			Swap2Byte(&common_all.var.devobj.fb_status2);
			memcpy(&common_all.var.devobj.param, data_ptr+6, 2);
			Swap2Byte(&common_all.var.devobj.param);
			SetDevObjPtVarById( &point, common_all.var.devobj );
			iost.noUpdate= 0;
			iost.scanEnable= 1;
			SetPtValueIostById( &point, new_value, iost, time, RW_UPDATE_TIME|RW_UPDATED );
			
			if(!shm_syscomm_addr->dev_alm_disable[point.stn_id][point.dev_id]
				&& common_all.fixed.iost.almEnable)
			{
				memcpy(&obj_rec.pt_id, data_ptr, 8);	/*point_id, status1, status2, param*/
				SwapRecEntryByte((REC_ENTRY_UNION*)&obj_rec);
			}
		}
		break;

	default:
		sprintf(err_buf, "Unsupported type %d in noChangeNoSend datagram", type_id);
		Err_log(err_buf);
		return -1;
	}/*switch( type_id )*/

	return 0;
}



int	DealAlarmData(char *data_ptr, UCHAR	proc_id)
{
	char		err_buf[256];
	POINTER		point;
	USHORT		point_num, point_ptr;
	DATE_TIME	time;
	USHORT		length_factor;
	UCHAR		host_id, type_id;
	WR_VAR_UNION	new_value;
	IOST		iost={0};
	enum RW_FLAG	rw_flag;
	REC_ENTRY_UNION	rec_entry;
	DMS_COMMON	common_all;	
	int			alm_flag;
	int         realarm_flag=0;//cxs
 
	type_id			= *data_ptr;
	point.stn_id	= *(data_ptr+1);
	point.dev_id	= *(data_ptr+2);
	point.data_type	= *(data_ptr+3);
	memcpy(&point_num, data_ptr+4, 2);
	Swap2Byte(&point_num);
	
	memcpy(&rec_entry.ana_rec.type_id, data_ptr, 4);	/*type_id, stn_id, dev_id, data_type*/

	switch( type_id )
	{
	case	DPS_ANA_ALM:			 
			length_factor= 8;
		data_ptr += (length_factor*point_num+6);
		host_id		= *(data_ptr);
		time.second	= *(data_ptr+1);
		time.minute	= *(data_ptr+2);
		time.hour	= *(data_ptr+3);
		time.day	= *(data_ptr+4);
		time.month	= *(data_ptr+5);
		memcpy(&time.year, data_ptr+6, 2);
		Swap2Byte(&time.year);

#ifdef	DEBUG
		printf("\n%04d-%02d-%02d %02d:%02d:%02d type_id= %d", 
			time.year, time.month, time.day, time.hour,
			time.minute, time.second, type_id);
#endif

		memcpy(&rec_entry.ana_rec.host_id, data_ptr, 8);	/*host_id, time*/

		data_ptr -= length_factor*point_num;
		for(point_ptr=0; point_ptr<point_num; point_ptr++,data_ptr += length_factor)
		{
			memcpy(&point.pt_id, data_ptr, 2);
			Swap2Byte(&point.pt_id);
			
			if(-1==ReadEntryById(&point , &common_all))
			{
				sprintf(err_buf, "No such point %d-%d-%d-%d in db",
					point.stn_id, point.dev_id, point.data_type, point.pt_id);
				Err_log(err_buf);
				continue;
			}
			
			memcpy(&new_value.anlg.status, data_ptr+2, 2);
			Swap2Byte(&new_value.anlg.status);
			memcpy(&new_value.anlg.data, data_ptr+4, 4);
			Swap4Byte(&new_value.anlg.data);

			realarm_flag=0;//20120316
			alm_flag=1;

			iost= common_all.fixed.iost;
			iost.noUpdate= 0;
			iost.scanEnable= 1;
			rw_flag= 0;
			rw_flag |= RW_UPDATE_TIME|RW_RT_VALUE|RW_UPDATED|RW_SCAN_ENABLE;
			if(new_value.anlg.status.alm_type==ANA_ALM_TYPE_LMT)
				rw_flag |= RW_LMT_STATE;
			else if(new_value.anlg.status.alm_type==ANA_ALM_TYPE_TR)
				rw_flag |= RW_TREND_STATE;
			else if(new_value.anlg.status.alm_type==ANA_ALM_TYPE_CHANFAIL)
				rw_flag |= RW_CHAN_FAIL;
			else if(new_value.anlg.status.alm_type==ANA_ALM_TYPE_CTRLBLOCK)
				rw_flag |= RW_CTRL_BLOCK;
			else if(new_value.anlg.status.alm_type==ANA_ALM_TYPE_OVERFLOW)
				rw_flag |= RW_OVERFLOW;
			
			if(!shm_syscomm_addr->dev_alm_disable[point.stn_id][point.dev_id]
				&& common_all.fixed.iost.almEnable)
			{
				if(new_value.anlg.status.alm_type==ANA_ALM_TYPE_LMT 
					|| new_value.anlg.status.alm_type==ANA_ALM_TYPE_TR)//cxs
				{
					if(common_all.fixed.ioattr.almCba)
					{
						if( !GetAlmCbaState(&point,rec_entry.ana_rec.type_id,-1,0) )
						{
							alm_flag= 0;
							realarm_flag=1;//20120316
						}
					}
				}
				else if(new_value.anlg.status.alm_type==ANA_ALM_TYPE_CHANFAIL)//cxs
				{
					if(QLT_ALM_TYPE==2)//cxs add
					{
						alm_flag=0;
					}
					else
					{
						if(common_all.fixed.ioattr.almCba)
						{
							if( !GetAlmCbaState(&point,rec_entry.ana_rec.type_id,-1,1))
							{
								alm_flag= 0;
								realarm_flag=1;//20120316
							}
						}
					}
				}
				
				if(alm_flag)
				{
					if(((common_all.fixed.ioattr.fail || common_all.fixed.ioattr.fault)
						&& ((new_value.anlg.status.alm_type==ANA_ALM_TYPE_LMT && new_value.anlg.status.lmt_state>0 && new_value.anlg.status.lmt_state<5)
						|| (new_value.anlg.status.alm_type==ANA_ALM_TYPE_TR && new_value.anlg.status.trend_state==1)))
						|| (new_value.anlg.status.alm_type==ANA_ALM_TYPE_CHANFAIL && new_value.anlg.status.chan_fail==1)
						|| (new_value.anlg.status.alm_type==ANA_ALM_TYPE_OVERFLOW && new_value.anlg.status.overflow==1))
					{
						rw_flag |= RW_ALM_NOACK;
						iost.almNoAck= 1;
						if(common_all.fixed.iost.almNoAck)
						{
							rw_flag |= RW_ALM_REPEAT;
							iost.almRepeat= 1;
						}
					}

					memcpy(&rec_entry.ana_rec.pt_id, data_ptr, 8);	/*point_id, status, data*/
					rec_entry.ana_rec.iost= iost;
					Swap2Byte(&rec_entry.ana_rec.iost);
					rec_entry.ana_rec.proc_id= proc_id;
					InsertRecToEventTable(&rec_entry);
				}
				//else if(new_value.anlg.status.alm_type!=ANA_ALM_TYPE_CHANFAIL  && realarm_flag==1)//cxs for relaarm start
				else if(new_value.anlg.status.alm_type==ANA_ALM_TYPE_LMT  && realarm_flag==1)//20120317
				{
					int ii;
					for(ii=0;ii<max_should_realarm_rec_num;ii++)
					{
						if(should_realarm_rec_entry.used[ii]==0)
						{	

							//20120317 add start
							should_realarm_rec_entry.alm_state[ii]=new_value.anlg.status.lmt_state;
							
							should_realarm_rec_entry.max_recheck_times[ii]=10000;//?

							
							should_realarm_rec_entry.point[ii].data_type=point.data_type;
							should_realarm_rec_entry.point[ii].stn_id=point.stn_id;
							should_realarm_rec_entry.point[ii].dev_id=point.dev_id;
							should_realarm_rec_entry.point[ii].pt_id=point.pt_id;
							should_realarm_rec_entry.point[ii].update_src=point.update_src;
							

							//20120317 add end
							
							memcpy(&should_realarm_rec_entry.rec_entry_realarm_union[ii],&rec_entry,sizeof(rec_entry));
					
							if(((common_all.fixed.ioattr.fail || common_all.fixed.ioattr.fault)
								&& ((new_value.anlg.status.alm_type==ANA_ALM_TYPE_LMT && new_value.anlg.status.lmt_state>0 && new_value.anlg.status.lmt_state<5)
								|| (new_value.anlg.status.alm_type==ANA_ALM_TYPE_TR && new_value.anlg.status.trend_state==1)))
								|| (new_value.anlg.status.alm_type==ANA_ALM_TYPE_CHANFAIL && new_value.anlg.status.chan_fail==1)
								|| (new_value.anlg.status.alm_type==ANA_ALM_TYPE_OVERFLOW && new_value.anlg.status.overflow==1))
							{
								should_realarm_rec_entry.rec_entry_realarm_union[ii].ana_rec.iost.almNoAck= 1;
								if(common_all.fixed.iost.almNoAck)
								{
									//rw_flag |= RW_ALM_REPEAT;
									should_realarm_rec_entry.rec_entry_realarm_union[ii].ana_rec.iost.almRepeat= 1;
								}
							}
							memcpy(&should_realarm_rec_entry.rec_entry_realarm_union[ii].ana_rec.pt_id, data_ptr, 8);	/*point_id, status, data*/
							Swap2Byte(&should_realarm_rec_entry.rec_entry_realarm_union[ii].ana_rec.iost);
							should_realarm_rec_entry.rec_entry_realarm_union[ii].ana_rec.proc_id= proc_id;
					
							should_realarm_rec_entry.used[ii]=1;
							break;
						}
					}
				}//cxs for relaarm end
			}

			point.update_src= host_id;
			SetPtValueIostById(&point, new_value, iost, time, rw_flag );
		}
		break;

	case	DPS_IMP_ALM:			 
		length_factor= 8;
		data_ptr += (length_factor*point_num+6);
		host_id		= *(data_ptr);
		time.second	= *(data_ptr+1);
		time.minute	= *(data_ptr+2);
		time.hour	= *(data_ptr+3);
		time.day	= *(data_ptr+4);
		time.month	= *(data_ptr+5);
		memcpy(&time.year, data_ptr+6, 2);
		Swap2Byte(&time.year);

#ifdef	DEBUG
		printf("\n%04d-%02d-%02d %02d:%02d:%02d type_id= %d", 
			time.year, time.month, time.day, time.hour,
			time.minute, time.second, type_id);
#endif

		memcpy(&rec_entry.ana_rec.host_id, data_ptr, 8);	/*host_id, time*/

		data_ptr -= length_factor*point_num;
		for(point_ptr=0; point_ptr<point_num; point_ptr++,data_ptr += length_factor)
		{
			memcpy(&point.pt_id, data_ptr, 2);
			Swap2Byte(&point.pt_id);

			if(-1==ReadEntryById(&point , &common_all))
			{
				sprintf(err_buf, "No such point %d-%d-%d-%d in db",
					point.stn_id, point.dev_id, point.data_type, point.pt_id);
				Err_log(err_buf);
				continue;
			}
			
			memcpy(&new_value.imp.status, data_ptr+2, 2);
			Swap2Byte(&new_value.imp.status);
			memcpy(&new_value.imp.iValue, data_ptr+4, 4);
			Swap4Byte(&new_value.imp.iValue);

			iost= common_all.fixed.iost;
			iost.noUpdate= 0;
			iost.scanEnable= 1;
			rw_flag= 0;
			rw_flag |= RW_UPDATE_TIME|RW_RT_VALUE|RW_UPDATED|RW_SCAN_ENABLE;
			if(new_value.imp.status.alm_type==IMP_ALM_TYPE_CHANFAIL)
				rw_flag |= RW_CHAN_FAIL;
			else if(new_value.imp.status.alm_type==IMP_ALM_TYPE_MANFORCE)
				rw_flag |= RW_MAN_SET;

			if(!shm_syscomm_addr->dev_alm_disable[point.stn_id][point.dev_id]
				&& common_all.fixed.iost.almEnable)
			{
				alm_flag= 1;

				/*if(common_all.fixed.ioattr.almCba)
				{
					if( !GetAlmCbaState(&point) )
					alm_flag= 0;
				}*/
				
				if(alm_flag)
				{
					if(new_value.imp.status.alm_type==IMP_ALM_TYPE_CHANFAIL && new_value.imp.status.chan_fail==1)
					{
						rw_flag |= RW_ALM_NOACK;
						iost.almNoAck= 1;
						if(common_all.fixed.iost.almNoAck)
						{
							rw_flag |= RW_ALM_REPEAT;
							iost.almRepeat= 1;
						}
					}

					memcpy(&rec_entry.ana_rec.pt_id, data_ptr, 8);	/*point_id, status, data*/
					rec_entry.ana_rec.iost= iost;
					Swap2Byte(&rec_entry.ana_rec.iost);
					rec_entry.ana_rec.proc_id= proc_id;
					InsertRecToEventTable(&rec_entry);
				}
			}

			point.update_src= host_id;
			/*if(RW_IMP_ACCUM==new_value.imp.status.option)
				UpdateImpStatistic(&point, time, new_value.imp.iValue);*/
			SetPtValueIostById( &point, new_value, iost, time, rw_flag );
		}
		break;

	case	DPS_IND_ALM:
		length_factor= 4;
		data_ptr += (length_factor*point_num+6);
		memcpy(&time.ms, data_ptr, 2);
		Swap2Byte(&time.ms);
		host_id		= *(data_ptr+2);
		time.second	= *(data_ptr+3);
		time.minute	= *(data_ptr+4);
		time.hour	= *(data_ptr+5);
		time.day	= *(data_ptr+6);
		time.month	= *(data_ptr+7);
		memcpy(&time.year, data_ptr+8, 2);
		Swap2Byte(&time.year);

#ifdef	DEBUG
		printf("\n%04d-%02d-%02d %02d:%02d:%02d type_id= %d", 
			time.year, time.month, time.day, time.hour,
			time.minute, time.second, type_id);
#endif

		memcpy(&rec_entry.ind_rec.ms, data_ptr, 2);	/*ms*/
		memcpy(&rec_entry.ind_rec.host_id, data_ptr+2, 8);	/*host_id, time*/
		
		data_ptr -= length_factor*point_num;
		for(point_ptr=0; point_ptr<point_num; point_ptr++,data_ptr += length_factor)
		{
			memcpy(&point.pt_id, data_ptr, 2);
			Swap2Byte(&point.pt_id);

			if(-1==ReadEntryById(&point , &common_all))
			{
				sprintf(err_buf, "No such point %d-%d-%d-%d in db",
					point.stn_id, point.dev_id, point.data_type, point.pt_id);
				Err_log(err_buf);
				continue;
			}
			
			memcpy(&new_value.ind, data_ptr+2, 2);
			Swap2Byte(&new_value.ind);

			iost= common_all.fixed.iost;
			iost.noUpdate= 0;
			iost.scanEnable= 1;
			rw_flag= 0;
			rw_flag |= RW_UPDATE_TIME|RW_RT_VALUE|RW_UPDATED|RW_SCAN_ENABLE|RW_FAULT_TAG;
			if(new_value.ind.alm_type==IND_ALM_TYPE_CHANFAIL)
				rw_flag |= RW_CHAN_FAIL;
			else if(new_value.ind.alm_type==IND_ALM_TYPE_MANFORCE)
				rw_flag |= RW_MAN_SET;

			if(common_all.fixed.ioattr.statistic && new_value.ind.alm_type==IND_ALM_TYPE_CHANGE)
			{
				if(0==new_value.ind.rt_state && 0==new_value.ind.fault_tag)
					rw_flag |= RW_ADD_NORMAL_OPEN;
				else if(0==new_value.ind.rt_state && 1==new_value.ind.fault_tag)
					rw_flag |= RW_ADD_FAULT_OPEN;
				else if(1==new_value.ind.rt_state && 0==new_value.ind.fault_tag)
					rw_flag |= RW_ADD_NORMAL_CLOSE;
				else if(1==new_value.ind.rt_state && 1==new_value.ind.fault_tag)
					rw_flag |= RW_ADD_FAULT_CLOSE;
			}

			if(!shm_syscomm_addr->dev_alm_disable[point.stn_id][point.dev_id]
				&& common_all.fixed.iost.almEnable)
			{
			
			 
			
					alm_flag= 1;
					realarm_flag=0;//20120316
					if(new_value.ind.alm_type==IND_ALM_TYPE_CHANGE)
					{
						if(!common_all.fixed.ioattr.onAlm && new_value.ind.rt_state==1)
							alm_flag= 0;
						if(!common_all.fixed.ioattr.offAlm && new_value.ind.rt_state==0 && alm_flag)//cxs  && alm_flag
							alm_flag= 0;
						if(alm_flag && common_all.fixed.ioattr.almCba)//cxs
						{
							if( !GetAlmCbaState(&point,rec_entry.ana_rec.type_id,new_value.ind.rt_state ,0) )
							{
								alm_flag= 0;
								realarm_flag=1;
							}

						}
					}
					else if(new_value.ind.alm_type==IND_ALM_TYPE_CHANFAIL)
					{	
						if(QLT_ALM_TYPE==2)//cxs add
						{
							alm_flag=0;
						}
						else
						{
							if(alm_flag && common_all.fixed.ioattr.almCba)//cxs
							{
								if( !GetAlmCbaState(&point,rec_entry.ana_rec.type_id,new_value.ind.rt_state ,1) )
								{
									alm_flag= 0;
									realarm_flag=1;
								}
							}
						}
					}
			 
			 
				if(alm_flag)
				{
					if( ((common_all.fixed.ioattr.fail || common_all.fixed.ioattr.fault)
						&& (new_value.ind.alm_type==IND_ALM_TYPE_CHANGE) 
						&& ((new_value.ind.fault_tag==1)
						|| (new_value.ind.rt_state!=common_all.fixed.ioattr.normalState)))
						|| (new_value.ind.alm_type==IND_ALM_TYPE_CHANFAIL && new_value.ind.chan_fail==1))
					{
						rw_flag |= RW_ALM_NOACK;
						iost.almNoAck= 1;
						if(common_all.fixed.iost.almNoAck)
						{
							rw_flag |= RW_ALM_REPEAT;
							iost.almRepeat= 1;
						}
					}

					memcpy(&rec_entry.ind_rec.pt_id, data_ptr, 4);	/*point_id, status*/
					rec_entry.ind_rec.iost= iost;
					Swap2Byte(&rec_entry.ind_rec.iost);
					rec_entry.ind_rec.proc_id= proc_id;
					if(enable_info_rec_table_flag && point.data_type==SYSMSG_TYPE 
						&& !common_all.fixed.ioattr.keySignal)
						InsertRecToInfoTable(&rec_entry);
					else
						InsertRecToEventTable(&rec_entry);
				}
			//	else if(new_value.ind.alm_type!=IND_ALM_TYPE_CHANFAIL && realarm_flag==1)//cxs for relaarm start
				else if(new_value.ind.alm_type==IND_ALM_TYPE_CHANGE && realarm_flag==1)//20120317
				{
					int ii;

					for(ii=0;ii<max_should_realarm_rec_num;ii++)
					{
						if(should_realarm_rec_entry.used[ii]==0)
						{
							//20120317 add start
						 	should_realarm_rec_entry.alm_state[ii]= new_value.ind.rt_state;
					 
							should_realarm_rec_entry.max_recheck_times[ii]=10000;//?


							
							should_realarm_rec_entry.point[ii].data_type=point.data_type;
							should_realarm_rec_entry.point[ii].stn_id=point.stn_id;
							should_realarm_rec_entry.point[ii].dev_id=point.dev_id;
							should_realarm_rec_entry.point[ii].pt_id=point.pt_id;
							should_realarm_rec_entry.point[ii].update_src=point.update_src;
							//20120317 add end


							memcpy(&should_realarm_rec_entry.rec_entry_realarm_union[ii],&rec_entry,sizeof(rec_entry));
					
							if( ((common_all.fixed.ioattr.fail || common_all.fixed.ioattr.fault)
								&& (new_value.ind.alm_type==IND_ALM_TYPE_CHANGE) 
								&& ((new_value.ind.fault_tag==1)
								|| (new_value.ind.rt_state!=common_all.fixed.ioattr.normalState)))
								|| (new_value.ind.alm_type==IND_ALM_TYPE_CHANFAIL && new_value.ind.chan_fail==1))
							{
								should_realarm_rec_entry.rec_entry_realarm_union[ii].ana_rec.iost.almNoAck= 1;
								if(common_all.fixed.iost.almNoAck)
								{
									//rw_flag |= RW_ALM_REPEAT;
									should_realarm_rec_entry.rec_entry_realarm_union[ii].ana_rec.iost.almRepeat= 1;
								}
							}
							memcpy(&should_realarm_rec_entry.rec_entry_realarm_union[ii].ind_rec.pt_id, data_ptr, 4);	/*point_id, status*/
							Swap2Byte(&should_realarm_rec_entry.rec_entry_realarm_union[ii].ind_rec.iost);
							should_realarm_rec_entry.rec_entry_realarm_union[ii].ind_rec.proc_id= proc_id;
							should_realarm_rec_entry.used[ii]=1;
							break;
						}
					}
				}//cxs for relaarm end
			}

			point.update_src= host_id;
			SetPtValueIostById( &point, new_value, iost, time, rw_flag );

			if(common_all.fixed.ioattr.faultRec && new_value.ind.alm_type==IND_ALM_TYPE_CHANGE
				&& common_all.fixed.ioattr.normalState!=new_value.ind.rt_state
				&& !shm_sgzy_addr->sghd.sg_flg)
			{
				shm_sgzy_addr->sghd.alarm.ptr= point;
				memcpy(&shm_sgzy_addr->sghd.alarm.status, &new_value.ind, 2);
				shm_sgzy_addr->sghd.alarm.time= time;
				shm_sgzy_addr->sghd.sg_flg= 1;
			}
		}
		break;

	default:
		sprintf(err_buf, "Unsupported type %d in alm datagram", type_id);
		Err_log(err_buf);
		return -1;
	}/*switch( type_id )*/

	return 0;
}


int	DealCommandData( USHORT data_seg_ptr )
{
	char		err_buf[256];
	POINTER		point, pair_pt;
	USHORT		data_length, data_num;
	DATE_TIME	time;
	WR_VAR_UNION	new_value;
	IOST		iost={0};
	IOATTR		ioattr={0};
	COMMAND_STATUS	cmd_status;
	union
	{
	GLONG		iValue;
	FLOAT		fValue;
	} data;
	int			rw_flag=0;
	REC_ENTRY_UNION	rec_entry;
	COMMAND_MSG	cmd_msg;
	ANA_ENG_PARA	eng_para;
	LMT_PARA	lmt_para;
	DMS_COMMON	common_all;
	STC_DATA	stc_data;
	USHORT		seg_offset;
	GULONG		imp_day_rate,imp_month_rate,imp_year_rate;
	double		imp_cvt;
	int			increase;
	int			type;
		
	data_length	= shm_data_recv_addr->shm_data_seg[data_seg_ptr].lan_head.length;
	Swap2Byte(&data_length);

	for( data_num=0; data_num<data_length; data_num += sizeof(COMMAND_MSG))
	{
		if((data_length-data_num)<sizeof(COMMAND_MSG))
		{
			sprintf(err_buf, "length error in command datagram");
			Err_log(err_buf);
			return -1;
		}

		memcpy(&cmd_msg, &shm_data_recv_addr->shm_data_seg[data_seg_ptr].data[data_num], sizeof(COMMAND_MSG));

		point.stn_id	= cmd_msg.stn_id;
		point.dev_id	= cmd_msg.dev_id;
		point.data_type	= cmd_msg.data_type;
		point.pt_id		= cmd_msg.point_id;
		Swap2Byte(&point.pt_id);
		point.update_src= cmd_msg.host_id;

		if(!IsPointExist( &point ))
		{
			sprintf(err_buf, "No such point %d-%d-%d-%d in db",
				point.stn_id, point.dev_id, point.data_type, point.pt_id);
			Err_log(err_buf);
			continue;
		}

		if(strlen(cmd_msg.usr_login_name) >= MAX_USR_NAME_SIZE)
		{
			sprintf(err_buf, "usr_name length error in command datagram: %s", cmd_msg.usr_login_name);
			Err_log(err_buf);
			cmd_msg.usr_login_name[MAX_USR_NAME_SIZE-1]= 0;;
		}

		cmd_status= cmd_msg.status;
		Swap2Byte(&cmd_status);

		data.iValue= cmd_msg.data.iValue;
		Swap4Byte(&data);

		time.second	= cmd_msg.second;
		time.minute	= cmd_msg.minute;
		time.hour	= cmd_msg.hour;
		time.day	= cmd_msg.day;
		time.month	= cmd_msg.month;
		time.year	= cmd_msg.year;
		Swap2Byte(&time.year);
		time.ms		= 0;
		Swap2Byte(&time.ms);

#ifdef	DEBUG
		printf("\n%04d-%02d-%02d %02d:%02d:%02d type_id= %d", 
			time.year, time.month, time.day, time.hour,
			time.minute, time.second, cmd_msg.type_id);
#endif

		if(cmd_msg.type_id==DPS_ANA_PARAM_SET 
			|| cmd_msg.type_id==DPS_IMP_PARAM_SET
			|| cmd_msg.type_id==DPS_C_ONOFF_SET)
		{
			memcpy(&rec_entry.param_set_rec.type_id, &cmd_msg.type_id, 12);
			rec_entry.param_set_rec.host_id= cmd_msg.host_id;
			memcpy(&rec_entry.param_set_rec.second, &cmd_msg.second, 15);
		}
		else
			memcpy(&rec_entry.cmd_rec.type_id, &cmd_msg.type_id, sizeof(COMMAND_MSG));

		switch( cmd_msg.type_id )
		{
		case	C_UNIT:			 
			if( (point.data_type!=SOE_TYPE)&&(point.data_type!=POL_TYPE)
				&&(point.data_type!=DEVOBJ_TYPE)&&(point.data_type!=SYSMSG_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}
			if(point.data_type==DEVOBJ_TYPE)
			{
				ReadEntryById(&point, &common_all);
				common_all.var.devobj.cmd_time= time;
				strcpy(common_all.var.devobj.usr_name, cmd_msg.usr_login_name);
				common_all.var.devobj.host_id= cmd_msg.host_id;
				memcpy(&common_all.var.devobj.status, &cmd_status, 2);
				memcpy(&common_all.var.devobj.data, &data, 4);
				common_all.var.devobj.fb_status2.orderm= cmd_status.opr;
				SetDevObjPtVarById( &point, common_all.var.devobj );
			}

			InsertRecToEventTable(&rec_entry);
			break;

		case	C_BREAKER:			 
			if( (point.data_type!=SOE_TYPE)&&(point.data_type!=POL_TYPE)
				&&(point.data_type!=DEVOBJ_TYPE)&&(point.data_type!=SYSMSG_TYPE)
				&&(point.data_type!=OUTS_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			if(point.data_type==DEVOBJ_TYPE)
			{
				ReadEntryById(&point, &common_all);
				common_all.var.devobj.cmd_time= time;
				strcpy(common_all.var.devobj.usr_name, cmd_msg.usr_login_name);
				common_all.var.devobj.host_id= cmd_msg.host_id;
				memcpy(&common_all.var.devobj.status, &cmd_status, 2);
				memcpy(&common_all.var.devobj.data, &data, 4);
				common_all.var.devobj.fb_status2.orderm= cmd_status.opr;
				SetDevObjPtVarById( &point, common_all.var.devobj );
			}

			InsertRecToEventTable(&rec_entry);
			break;

		case	C_SWITCH:			 
			if( (point.data_type!=SOE_TYPE)&&(point.data_type!=POL_TYPE)
				&&(point.data_type!=DEVOBJ_TYPE)&&(point.data_type!=SYSMSG_TYPE) 
				&&(point.data_type!=OUTS_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			if(point.data_type==DEVOBJ_TYPE)
			{
				ReadEntryById(&point, &common_all);
				common_all.var.devobj.cmd_time= time;
				strcpy(common_all.var.devobj.usr_name, cmd_msg.usr_login_name);
				common_all.var.devobj.host_id= cmd_msg.host_id;
				memcpy(&common_all.var.devobj.status, &cmd_status, 2);
				memcpy(&common_all.var.devobj.data, &data, 4);
				common_all.var.devobj.fb_status2.orderm= cmd_status.opr;
				SetDevObjPtVarById( &point, common_all.var.devobj );
			}

			InsertRecToEventTable(&rec_entry);
			break;

		case	C_APOWER:
		case	C_RPOWER:      
			if( (point.data_type!=ANA_TYPE)&&(point.data_type!=PARAM_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			if(cmd_status.src!=0 && !powerAdjLogFlag)
				PowerCmdLogtoFile(&rec_entry);
			else
				InsertRecToEventTable(&rec_entry);

			//if(cmd_status.opr==0) /*modified by wzg 20080908*/
			{
				if(point.data_type==ANA_TYPE)
				{
					if(-1==GetPairPtById(&point, &pair_pt))
						break;
				}
				else
					pair_pt= point;

				memset(&new_value.anlg.status, 0, 2);
				new_value.anlg.data= data.iValue;
				iost.noUpdate= 0;
				rw_flag= RW_UPDATE_TIME|RW_RT_VALUE|RW_UPDATED;
				SetPtValueIostById( &pair_pt, new_value, iost, time, rw_flag );
			}
			break;

		case	C_DEVICE:			 
			if( (point.data_type!=SOE_TYPE)&&(point.data_type!=DEVOBJ_TYPE)
				&&(point.data_type!=POL_TYPE)&&(point.data_type!=SYSMSG_TYPE) 
				&&(point.data_type!=OUTS_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			if(point.data_type==DEVOBJ_TYPE)
			{
				ReadEntryById(&point, &common_all);
				common_all.var.devobj.cmd_time= time;
				strcpy(common_all.var.devobj.usr_name, cmd_msg.usr_login_name);
				common_all.var.devobj.host_id= cmd_msg.host_id;
				memcpy(&common_all.var.devobj.status, &cmd_status, 2);
				memcpy(&common_all.var.devobj.data, &data, 4);
				common_all.var.devobj.fb_status2.orderm= cmd_status.opr;
				SetDevObjPtVarById( &point, common_all.var.devobj );
			}

			InsertRecToEventTable(&rec_entry);
			break;

		case	C_GATE:			 
			if( (point.data_type!=ANA_TYPE)&&(point.data_type!=SOE_TYPE)
				&&(point.data_type!=POL_TYPE)&&(point.data_type!=SYSMSG_TYPE)
				&&(point.data_type!=DEVOBJ_TYPE)&&(point.data_type!=PARAM_TYPE))
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			if(point.data_type==DEVOBJ_TYPE)
			{
				ReadEntryById(&point, &common_all);
				common_all.var.devobj.cmd_time= time;
				strcpy(common_all.var.devobj.usr_name, cmd_msg.usr_login_name);
				common_all.var.devobj.host_id= cmd_msg.host_id;
				memcpy(&common_all.var.devobj.status, &cmd_status, 2);
				memcpy(&common_all.var.devobj.data, &data, 4);
				common_all.var.devobj.fb_status2.orderm= cmd_status.opr;
				SetDevObjPtVarById( &point, common_all.var.devobj );
			}

			InsertRecToEventTable(&rec_entry);
			break;

		case	C_TRANS:			 
			if( (point.data_type!=ANA_TYPE)&&(point.data_type!=SOE_TYPE)
				&&(point.data_type!=POL_TYPE)&&(point.data_type!=SYSMSG_TYPE)
				&&(point.data_type!=DEVOBJ_TYPE)&&(point.data_type!=PARAM_TYPE))
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			if(point.data_type==DEVOBJ_TYPE)
			{
				ReadEntryById(&point, &common_all);
				common_all.var.devobj.cmd_time= time;
				strcpy(common_all.var.devobj.usr_name, cmd_msg.usr_login_name);
				common_all.var.devobj.host_id= cmd_msg.host_id;
				memcpy(&common_all.var.devobj.status, &cmd_status, 2);
				memcpy(&common_all.var.devobj.data, &data, 4);
				common_all.var.devobj.fb_status2.orderm= cmd_status.opr;
				SetDevObjPtVarById( &point, common_all.var.devobj );
			}

			InsertRecToEventTable(&rec_entry);
			break;

 		case	C_SPEED_STEP:
 		case	C_VOLT_STEP:
			if( (point.data_type!=ANA_TYPE)&&(point.data_type!=PARAM_TYPE))
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			InsertRecToEventTable(&rec_entry);

			if(point.data_type==ANA_TYPE)
			{
				if(-1==GetPairPtById(&point, &pair_pt))
					break;
				if(-1==GetAnaPtValueById(&point, &new_value.anlg))
					break;
			}
			else
			{
				if(-1==GetPairPtById(&point, &pair_pt))
					break;
				if(-1==GetAnaPtValueById(&pair_pt, &new_value.anlg))
					break;
				pair_pt= point;
			}

			memset(&new_value.anlg.status, 0, 2);
			iost.noUpdate= 0;
			rw_flag= RW_UPDATE_TIME|RW_RT_VALUE|RW_UPDATED;
			SetPtValueIostById( &pair_pt, new_value, iost, time, rw_flag );
			break;

		case	C_ONOFF_FORCE:			 
			if( (point.data_type!=OUTS_TYPE) && (point.data_type!=POL_TYPE) 
				&& (point.data_type!=SOE_TYPE) && (point.data_type!=SYSMSG_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			InsertRecToEventTable(&rec_entry);
			break;

		case	C_ANA_FORCE:
			if( (point.data_type!=OUTA_TYPE) && (point.data_type!=ANA_TYPE) 
				&& (point.data_type!=CALC_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			InsertRecToEventTable(&rec_entry);
			break;			

		case	C_IO_CTRL_BLOCK:
			if( (point.data_type!=ANA_TYPE)&&(point.data_type!=OUTA_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			InsertRecToEventTable(&rec_entry);
			break;			

		case	C_DEV_MODE:			 
			if( (point.data_type!=SOE_TYPE)&&(point.data_type!=POL_TYPE)&&(point.data_type!=SYSMSG_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			InsertRecToEventTable(&rec_entry);
			break;

		case	C_SIGNAL_RESET:			 
			if( (point.data_type!=SOE_TYPE)&&(point.data_type!=POL_TYPE)&&(point.data_type!=SYSMSG_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			InsertRecToEventTable(&rec_entry);
			break;

 		case	C_LCU_ONOFF:
			if( (point.data_type!=SOE_TYPE)&&(point.data_type!=POL_TYPE)&&(point.data_type!=SYSMSG_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			memset(&new_value.ind, 0, 2);
			if(cmd_status.opr==1)
				new_value.ind.rt_state= 0;
			else if(cmd_status.opr==2)
				new_value.ind.rt_state= 1;
			else
			{
				sprintf(err_buf, "Command %d operate error: %d", cmd_msg.type_id, cmd_status.opr);
				Err_log(err_buf);
				break;
			}

			InsertRecToEventTable(&rec_entry);

			iost.noUpdate= 0;
			rw_flag= RW_UPDATE_TIME|RW_RT_VALUE|RW_UPDATED;
			SetPtValueIostById( &point, new_value, iost, time, rw_flag );
			break;
    
		case	TIME_SYNC:
			//modified by wzg 20120330 不记录对时令
// 			if( (point.data_type!=SOE_TYPE)&&(point.data_type!=POL_TYPE)&&(point.data_type!=SYSMSG_TYPE) )
// 			{
// 				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
// 				Err_log(err_buf);
// 				break;
// 			}
// 
// 			InsertRecToEventTable(&rec_entry);
			break;

 		case	C_HOST_SYS:
			if( point.data_type!=SYSMSG_TYPE )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			memset(&new_value.ind, 0, 2);
			if(cmd_status.opr==1)
				new_value.ind.rt_state= 1;
			else if(cmd_status.opr==2)
				new_value.ind.rt_state= 2;
			else
			{
				sprintf(err_buf, "Command %d operate error: %d", cmd_msg.type_id, cmd_status.opr);
				Err_log(err_buf);
				break;
			}

			InsertRecToEventTable(&rec_entry);

			iost.noUpdate= 0;
			rw_flag= RW_UPDATE_TIME|RW_RT_VALUE|RW_UPDATED;
			SetPtValueIostById( &point, new_value, iost, time, rw_flag );
			break;

		case	C_PLC_SWITCH:
			if( (point.data_type!=SOE_TYPE)&&(point.data_type!=POL_TYPE)&&(point.data_type!=SYSMSG_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			InsertRecToEventTable(&rec_entry);
			break;

 		case	C_NET_SWITCH:
			if( point.data_type!=SYSMSG_TYPE )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			memset(&new_value.ind, 0, 2);
			if(cmd_status.opr>0)
				new_value.ind.rt_state= cmd_status.opr-1;
			else
			{
				sprintf(err_buf, "Command %d operate error: %d", cmd_msg.type_id, cmd_status.opr);
				Err_log(err_buf);
				break;
			}
			
			InsertRecToEventTable(&rec_entry);

			iost.noUpdate= 0;
			rw_flag= RW_UPDATE_TIME|RW_RT_VALUE|RW_UPDATED;
			SetPtValueIostById( &point, new_value, iost, time, rw_flag );
			break;

 		case	C_OP_VALUE_SET:
			if( (point.data_type!=ANA_TYPE)&&(point.data_type!=PARAM_TYPE)&&(point.data_type!=CURVE_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			InsertRecToEventTable(&rec_entry);

			if( point.data_type==CURVE_TYPE )
			{
				seg_offset= cmd_msg.ctrl_addr;
				Swap2Byte(&seg_offset);
				if( seg_offset>=MAX_CURVE_SEG_NUM )
				{
					sprintf(err_buf, "Command %d offset error: %d", cmd_msg.type_id, seg_offset);
					Err_log(err_buf);
					break;
				}
				data.fValue= (float)(data.iValue/1000.0);
				SetCurvePtDataById( &point, &data.fValue, seg_offset, 1, time );
			}
			else
			{
				if(cmd_status.opr==0)
				{
					memset(&new_value.anlg.status, 0, 2);
					new_value.anlg.data= data.iValue;
					iost.noUpdate= 0;
					rw_flag= RW_UPDATE_TIME|RW_RT_VALUE|RW_UPDATED|RW_MAN_SET;
					SetPtValueIostById( &point, new_value, iost, time, rw_flag );
				}
				else
				{
					sprintf(err_buf, "Command %d operate error: %d", cmd_msg.type_id, cmd_status.opr);
					Err_log(err_buf);
					break;
				}
			}
			break;

 		case	C_SYS_MODE_SET:
			if( (point.data_type!=SOE_TYPE)&&(point.data_type!=POL_TYPE)&&(point.data_type!=SYSMSG_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			memset(&new_value.ind, 0, 2);
			if(cmd_status.opr<16)
				new_value.ind.rt_state= cmd_status.opr;
			else
			{
				sprintf(err_buf, "Command %d operate error: %d", cmd_msg.type_id, cmd_status.opr);
				Err_log(err_buf);
				break;
			}
			
			InsertRecToEventTable(&rec_entry);

			iost.noUpdate= 0;
			rw_flag= RW_UPDATE_TIME|RW_RT_VALUE|RW_UPDATED|RW_MAN_SET;
			SetPtValueIostById( &point, new_value, iost, time, rw_flag );
			break;

		case	C_DATA_POLLING:			 
			shm_syscomm_addr->data_poll[point.stn_id][point.dev_id].data_type= point.data_type;
			shm_syscomm_addr->data_poll[point.stn_id][point.dev_id].start_pt_id= point.pt_id;
			shm_syscomm_addr->data_poll[point.stn_id][point.dev_id].pt_num= data.iValue;
			shm_syscomm_addr->data_poll[point.stn_id][point.dev_id].poll_flag= 1;			
			break;

		case	C_LCU_PARAM:
			if( (point.data_type!=ANA_TYPE)&&(point.data_type!=PARAM_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			InsertRecToEventTable(&rec_entry);
			break;

		case	DPS_ANA_PARAM_SET:
			if( (point.data_type!=ANA_TYPE)&&(point.data_type!=PARAM_TYPE) 
				&&(point.data_type!=OUTA_TYPE)&& (point.data_type!=CALC_TYPE))
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			GetAnaPtEngLmtParaById(&point, &eng_para, &lmt_para);
			switch( cmd_status.opr )
			{  
			case	ANA_PARAM_SET_D_BAND:
				rec_entry.param_set_rec.data_old.fValue= eng_para.D_BAND;
				rw_flag= RW_D_BAND;
				break;
			case	ANA_PARAM_SET_ENG_CVT:
				rec_entry.param_set_rec.data_old.fValue= eng_para.ENG_CVT;
				rw_flag= RW_ENG_CVT;
				break;
			case	ANA_PARAM_SET_HI_RANGE:
				rec_entry.param_set_rec.data_old.fValue= eng_para.HI_RANGE;
				rw_flag= RW_HI_RANGE;
				break;
			case	ANA_PARAM_SET_LO_RANGE:
				rec_entry.param_set_rec.data_old.fValue= eng_para.LO_RANGE;
				rw_flag= RW_LO_RANGE;
				break;
			case	ANA_PARAM_SET_ZERO:
				rec_entry.param_set_rec.data_old.fValue= eng_para.ZERO;
				rw_flag= RW_ZERO;
				break;
			case	ANA_PARAM_SET_COMPSATE:
				rec_entry.param_set_rec.data_old.fValue= eng_para.COMPSATE;
				rw_flag= RW_COMPSATE;
				break;
			case	ANA_PARAM_SET_TR_WARN:
				rec_entry.param_set_rec.data_old.fValue= lmt_para.TR_WARN;
				rw_flag= RW_TR_WARN;
				break;
			case	ANA_PARAM_SET_TR_RECOVER:
				rec_entry.param_set_rec.data_old.fValue= lmt_para.TR_RECOVER;
				rw_flag= RW_TR_RECOVER;
				break;		
			case	ANA_PARAM_SET_HI_ALARM:
				rec_entry.param_set_rec.data_old.fValue= lmt_para.HI_ALARM;
				rw_flag= RW_HI_ALARM;
				break;
			case	ANA_PARAM_SET_HI_WARN:
				rec_entry.param_set_rec.data_old.fValue= lmt_para.HI_WARN;
				rw_flag= RW_HI_WARN;
				break;
			case	ANA_PARAM_SET_LO_WARN:
				rec_entry.param_set_rec.data_old.fValue= lmt_para.LO_WARN;
				rw_flag= RW_LO_WARN;
				break;
			case	ANA_PARAM_SET_LO_ALARM:
 				rec_entry.param_set_rec.data_old.fValue= lmt_para.LO_ALARM;
				rw_flag= RW_LO_ALARM;
				break;
			case	ANA_PARAM_SET_HI_RECOVER:
				rec_entry.param_set_rec.data_old.fValue= lmt_para.HI_RECOVER;
				rw_flag= RW_HI_RECOVER;
				break;
			case	ANA_PARAM_SET_LO_RECOVER:
				rec_entry.param_set_rec.data_old.fValue= lmt_para.LO_RECOVER;
				rw_flag= RW_LO_RECOVER;
				break;
			case	ANA_PARAM_SET_HI_CODE:
				rec_entry.param_set_rec.data_old.fValue= eng_para.HI_CODE;
				rw_flag= RW_HI_CODE;
				break;
			case	ANA_PARAM_SET_LO_CODE:
				rec_entry.param_set_rec.data_old.fValue= eng_para.LO_CODE;
				rw_flag= RW_LO_CODE;
				break;
			default:
				sprintf(err_buf, "Command %d operate error: %d", cmd_msg.type_id, cmd_status.opr);
				Err_log(err_buf);
				continue;
			}
			Swap4Byte(&rec_entry.param_set_rec.data_old.fValue);
			InsertRecToEventTable(&rec_entry);

			SetAnaPtEngLmtParaById(&point, data.fValue, rw_flag);
			break;

		case	DPS_IMP_PARAM_SET:
			if( point.data_type!=IMP_TYPE )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}
			
			ReadEntryById(&point, &common_all);
			imp_cvt= common_all.var.imp.entry.eng_para.CT*common_all.var.imp.entry.eng_para.PT
				*common_all.var.imp.entry.eng_para.ENG_CVT;
			imp_day_rate= common_all.var.imp.entry.eng_para.DAY_RATE;
			imp_month_rate= common_all.var.imp.entry.eng_para.YEAR_RATE;
			imp_year_rate= common_all.var.imp.entry.eng_para.YEAR_RATE;
			switch( cmd_status.opr )
			{  
			case	IMP_PARAM_SET_BASE_ACCUM:
			case	IMP_PARAM_SET_D_ACCUM:
				if(cmd_status.opr==IMP_PARAM_SET_BASE_ACCUM)
				{
					increase= data.iValue-common_all.var.imp.entry.base_value;
					rec_entry.param_set_rec.data_old.iValue= common_all.var.imp.entry.base_value;
					common_all.var.imp.entry.base_value= data.iValue;
					common_all.var.imp.entry.d_accum
						= (float)((double)(common_all.var.imp.entry.rt_value
						+common_all.var.imp.entry.base_value)*imp_cvt/imp_year_rate);
				}
				else
				{
					increase= FloattoRecentInt(data.fValue*imp_year_rate/imp_cvt
						-common_all.var.imp.entry.rt_value-common_all.var.imp.entry.base_value);
					rec_entry.param_set_rec.data_old.fValue= common_all.var.imp.entry.d_accum;
					common_all.var.imp.entry.d_accum= data.fValue;
					common_all.var.imp.entry.base_value
						= UFloattoRecentUInt(common_all.var.imp.entry.d_accum*imp_year_rate/imp_cvt
						-common_all.var.imp.entry.rt_value);
				}

				if(time.year!=common_all.var.imp.entry.update_time.year)
				{
					common_all.var.imp.entry.ppeak.year= 0;
					common_all.var.imp.entry.peak.year= 0;
					common_all.var.imp.entry.valley.year= 0;
					common_all.var.imp.entry.normal.year= 0;
					common_all.var.imp.entry.last.year= common_all.var.imp.entry.sum.year;
					common_all.var.imp.entry.sum.year= 0;
					common_all.var.imp.entry.init.year= common_all.var.imp.entry.base_value
						+common_all.var.imp.entry.rt_value;
				}	
				if(time.month!=common_all.var.imp.entry.update_time.month)
				{
					common_all.var.imp.entry.ppeak.month= 0;
					common_all.var.imp.entry.peak.month= 0;
					common_all.var.imp.entry.valley.month= 0;
					common_all.var.imp.entry.normal.month= 0;
					common_all.var.imp.entry.last.month= common_all.var.imp.entry.sum.month;
					common_all.var.imp.entry.sum.month= 0;
					common_all.var.imp.entry.init.month= common_all.var.imp.entry.base_value
						+common_all.var.imp.entry.rt_value;
				}	
				if(time.day!=common_all.var.imp.entry.update_time.day)
				{
					common_all.var.imp.entry.ppeak.day= 0;
					common_all.var.imp.entry.peak.day= 0;
					common_all.var.imp.entry.valley.day= 0;
					common_all.var.imp.entry.normal.day= 0;
					common_all.var.imp.entry.last.day= common_all.var.imp.entry.sum.day;
					common_all.var.imp.entry.sum.day= 0;
					common_all.var.imp.entry.init.day= common_all.var.imp.entry.base_value
						+common_all.var.imp.entry.rt_value;
				}

				GetImpTimeType(point.stn_id, time, &type);
				switch(type)
				{
				case IMP_PPEAK:
					UpdateImpData(&common_all.var.imp.entry.ppeak, increase);
					break;

				case IMP_PEAK:
					UpdateImpData(&common_all.var.imp.entry.peak, increase);
					break;

				case IMP_VALLEY:
					UpdateImpData(&common_all.var.imp.entry.valley, increase);
					break;

				case IMP_NORMAL:
					UpdateImpData(&common_all.var.imp.entry.normal, increase);
					break;
				}

				common_all.var.imp.entry.sum.year= common_all.var.imp.entry.ppeak.year
					+common_all.var.imp.entry.peak.year+common_all.var.imp.entry.valley.year
					+common_all.var.imp.entry.normal.year;
				common_all.var.imp.entry.sum.month= common_all.var.imp.entry.ppeak.month
					+common_all.var.imp.entry.peak.month+common_all.var.imp.entry.valley.month
					+common_all.var.imp.entry.normal.month;
				common_all.var.imp.entry.sum.day= common_all.var.imp.entry.ppeak.day
					+common_all.var.imp.entry.peak.day+common_all.var.imp.entry.valley.day
					+common_all.var.imp.entry.normal.day;
				break;

			case	IMP_PARAM_SET_PEAK_DAY:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.peak.day*imp_cvt/imp_day_rate);
				common_all.var.imp.entry.peak.day= (GULONG)(data.fValue*imp_day_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_PEAK_MONTH:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.peak.month*(imp_cvt/imp_month_rate));
				common_all.var.imp.entry.peak.month= (GULONG)(data.fValue*imp_month_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_PEAK_YEAR:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.peak.year*(imp_cvt/imp_year_rate));
				common_all.var.imp.entry.peak.year= (GULONG)(data.fValue*imp_year_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_VALLEY_DAY:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.valley.day*(imp_cvt/imp_day_rate));
				common_all.var.imp.entry.valley.day= (GULONG)(data.fValue*imp_day_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_VALLEY_MONTH:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.valley.month*(imp_cvt/imp_month_rate));
				common_all.var.imp.entry.valley.month= (GULONG)(data.fValue*imp_month_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_VALLEY_YEAR:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.valley.year*(imp_cvt/imp_year_rate));
				common_all.var.imp.entry.valley.year= (GULONG)(data.fValue*imp_year_rate/imp_cvt);
				break;		
			case	IMP_PARAM_SET_NORM_DAY:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.normal.day*(imp_cvt/imp_day_rate));
				common_all.var.imp.entry.normal.day= (GULONG)(data.fValue*imp_day_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_NORM_MONTH:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.normal.month*(imp_cvt/imp_month_rate));
				common_all.var.imp.entry.normal.month= (GULONG)(data.fValue*imp_month_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_NORM_YEAR:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.normal.year*(imp_cvt/imp_year_rate));
				common_all.var.imp.entry.normal.year= (GULONG)(data.fValue*imp_year_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_SUM_DAY:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.sum.day*(imp_cvt/imp_day_rate));
				common_all.var.imp.entry.sum.day= (GULONG)(data.fValue*imp_day_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_SUM_MONTH:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.sum.month*(imp_cvt/imp_month_rate));
				common_all.var.imp.entry.sum.month= (GULONG)(data.fValue*imp_month_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_SUM_YEAR:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.sum.year*(imp_cvt/imp_year_rate));
				common_all.var.imp.entry.sum.year= (GULONG)(data.fValue*imp_year_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_PPEAK_DAY:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.ppeak.day*(imp_cvt/imp_day_rate));
				common_all.var.imp.entry.ppeak.day= (GULONG)(data.fValue*imp_day_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_PPEAK_MONTH:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.ppeak.month*(imp_cvt/imp_month_rate));
				common_all.var.imp.entry.ppeak.month= (GULONG)(data.fValue*imp_month_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_PPEAK_YEAR:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.ppeak.year*(imp_cvt/imp_year_rate));
				common_all.var.imp.entry.ppeak.year= (GULONG)(data.fValue*imp_year_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_INIT_DAY:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.init.day*(imp_cvt/imp_day_rate));
				common_all.var.imp.entry.init.day= (GULONG)(data.fValue*imp_day_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_INIT_MONTH:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.init.month*(imp_cvt/imp_month_rate));
				common_all.var.imp.entry.init.month= (GULONG)(data.fValue*imp_month_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_INIT_YEAR:
				rec_entry.param_set_rec.data_old.fValue= (FLOAT)(common_all.var.imp.entry.init.year*(imp_cvt/imp_year_rate));
				common_all.var.imp.entry.init.year= (GULONG)(data.fValue*imp_year_rate/imp_cvt);
				break;
			case	IMP_PARAM_SET_CVT_F:
				rec_entry.param_set_rec.data_old.fValue= common_all.var.imp.entry.eng_para.ENG_CVT;
				common_all.var.imp.entry.eng_para.ENG_CVT= data.fValue;
				break;
			case	IMP_PARAM_SET_PT:
				rec_entry.param_set_rec.data_old.fValue= common_all.var.imp.entry.eng_para.PT;
				common_all.var.imp.entry.eng_para.PT= data.fValue;
				break;
			case	IMP_PARAM_SET_CT:
				rec_entry.param_set_rec.data_old.fValue= common_all.var.imp.entry.eng_para.CT;
				common_all.var.imp.entry.eng_para.CT= data.fValue;
				break;
			case	IMP_PARAM_SET_CONSTANT:
				rec_entry.param_set_rec.data_old.iValue= common_all.var.imp.entry.eng_para.CONSTANT;
				common_all.var.imp.entry.eng_para.CONSTANT= data.iValue;
				break;
			default:
				sprintf(err_buf, "Command %d operate error: %d", cmd_msg.type_id, cmd_status.opr);
				Err_log(err_buf);
				continue;
			}
			Swap4Byte(&rec_entry.param_set_rec.data_old.fValue);
			InsertRecToEventTable(&rec_entry);

			SetImpPtVarById(&point, common_all.var.imp.entry);
			break;

		case	DPS_C_ONOFF_SET:
			if( (point.data_type!=OUTS_TYPE)&&(point.data_type!=SOE_TYPE)
				&&(point.data_type!=POL_TYPE)&&(point.data_type!=SYSMSG_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			if( data.iValue<0 || data.iValue>65535 )
			{
				sprintf(err_buf, "Command %d operate value overrange: %d", cmd_msg.type_id, data.iValue);
				Err_log(err_buf);
				break;
			}

			GetIndPtSTCById(&point, &stc_data);
			switch( cmd_status.opr )
			{  
			case	IND_PARAM_SET_NORM_OPEN_NUM:
				rec_entry.param_set_rec.data_old.iValue= stc_data.norm_open;
				rw_flag= RW_STC_NORMAL_OPEN;
				break;
			case	IND_PARAM_SET_NORM_CLOSE_NUM:
				rec_entry.param_set_rec.data_old.iValue= stc_data.norm_close;
				rw_flag= RW_STC_NORMAL_CLOSE;
				break;
			case	IND_PARAM_SET_FAULT_OPEN_NUM:
				rec_entry.param_set_rec.data_old.iValue= stc_data.fault_open;
				rw_flag= RW_STC_FAULT_OPEN;
				break;
			case	IND_PARAM_SET_FAULT_CLOSE_NUM:
				rec_entry.param_set_rec.data_old.iValue= stc_data.fault_close;
				rw_flag= RW_STC_FAULT_CLOSE;
				break;
			default:
				sprintf(err_buf, "Command %d operate error: %d", cmd_msg.type_id, cmd_status.opr);
				Err_log(err_buf);
				continue;
			}
			Swap4Byte(&rec_entry.param_set_rec.data_old.iValue);
			InsertRecToEventTable(&rec_entry);

			SetIndPtStcById(&point, (USHORT)data.iValue, rw_flag);
			break;
			
		case	DPS_IOST_SET:
			switch( cmd_status.opr )
			{  
			case	IOST_SET_SCAN_STATE:
				iost.scanEnable= cmd_status.state;
				rw_flag= RW_SCAN_ENABLE;
				break;
			case	IOST_SET_ALM_STATE:
				iost.almEnable= cmd_status.state;
				rw_flag= RW_ALM_ENABLE;
				break;
			case	IOST_SET_ALM_NOACK:
				iost.almNoAck= cmd_status.state;
				iost.almRepeat= 0;
				rw_flag= RW_ALM_NOACK|RW_ALM_REPEAT;
				break;
			case	IOST_SET_CTRL_LOCK:
				iost.ctrlLock= cmd_status.state;
				rw_flag= RW_CTRL_LOCK;
				break;
			case	IOST_SET_NO_UPDATE:
				iost.noUpdate= cmd_status.state;
				rw_flag= RW_UPDATED;
				break;
			case	IOST_SET_ALM_REPEAT:
				iost.almRepeat= cmd_status.state;
				rw_flag= RW_ALM_REPEAT;
				break;
			case	IOST_SET_TAG_STATE:
				iost.tagState= cmd_status.state;
				rw_flag= RW_TAG_STATE;
				break;
			default:
				sprintf(err_buf, "Command %d operate error: %d", cmd_msg.type_id, cmd_status.opr);
				Err_log(err_buf);
				continue;
			}
			if(cmd_status.opr!=IOST_SET_CTRL_LOCK && cmd_status.opr!=IOST_SET_ALM_NOACK)
				InsertRecToEventTable(&rec_entry);

			SetPtValueIostById( &point, new_value, iost, time, rw_flag );
			break;
			
		case	DPS_IOATTR_SET:
			data.iValue= data.iValue & 0xFFFF;
			switch( cmd_status.opr )
			{  
			case	IOATTR_SET_HDB_PERIOD:
				if(data.iValue<0 || data.iValue>7)
				{
					sprintf(err_buf, "Command %d operate %d para error: %d", 
						cmd_msg.type_id, cmd_status.opr, data.iValue);
					Err_log(err_buf);
					continue;
				}
				ioattr.hdbPeriod= data.iValue;
				rw_flag= RW_HDB_PERIOD;
				break;
			case	IOATTR_SET_PROCTYPE:
				if(data.iValue<0 || data.iValue>31)
				{
					sprintf(err_buf, "Command %d operate %d para error: %d", 
						cmd_msg.type_id, cmd_status.opr, data.iValue);
					Err_log(err_buf);
					continue;
				}
				ioattr.procType= data.iValue;
				rw_flag= RW_PROCTYPE;
				break;
			case	IOATTR_SET_ALM_CBA:
				ioattr.almCba= cmd_status.state;
				rw_flag= RW_ALM_CBA;
				break;
			case	IOATTR_SET_STATISTIC:
				ioattr.statistic= cmd_status.state;
				rw_flag= RW_STATISTIC;
				break;
			case	IOATTR_SET_FAULT_PNT:
				ioattr.fault= cmd_status.state;
				rw_flag= RW_FAULT;
				break;
			case	IOATTR_SET_FAIL_PNT:
				ioattr.fail= cmd_status.state;
				rw_flag= RW_FAIL;
				break;
			case	IOATTR_SET_CTRL_PNT:
				ioattr.ctrl= cmd_status.state;
				rw_flag= RW_CTRL;
				break;
			case	IOATTR_SET_SOE_PNT:
				ioattr.soe= cmd_status.state;
				rw_flag= RW_SOE;
				break;
			case	IOATTR_SET_TREND_ALM:
				ioattr.trendAlm= cmd_status.state;
				rw_flag= RW_TREND_ALM;
				break;
			case	IOATTR_SET_KEY_SIGNAL:
				ioattr.keySignal= cmd_status.state;
				rw_flag= RW_KEY_SIGNAL;
				break;
			case	IOATTR_SET_FAULT_REC:
				ioattr.faultRec= cmd_status.state;
				rw_flag= RW_FAULT_REC;
				break;
			case	IOATTR_SET_AUD_ALM:
				ioattr.audAlm= cmd_status.state;
				rw_flag= RW_AUD_ALM;
				break;
			case	IOATTR_SET_NORM_CLOSE:
				ioattr.normalClose= cmd_status.state;
				rw_flag= RW_NORM_CLOSE;
				break;
			case	IOATTR_SET_DOUBLE_PNT:
				ioattr.doublePt= cmd_status.state;
				rw_flag= RW_DOUBLE_PT;
				break;
			case	IOATTR_SET_SYS_VIRTUAL:
				ioattr.sysVirtual= cmd_status.state;
				rw_flag= RW_SYS_VIRTUAL;
				break;
			case	IOATTR_SET_LCU_VIRTUAL:
				ioattr.lcuVirtual= cmd_status.state;
				rw_flag= RW_LCU_VIRTUAL;
				break;
			case	IOATTR_SET_CASCADE_PNT:
				ioattr.cascade= cmd_status.state;
				rw_flag= RW_CASCADE;
				break;
			case	IOATTR_SET_HDB_REC:
				ioattr.hdbRec= cmd_status.state;
				rw_flag= RW_HDB_REC;
				break;
			case	IOATTR_SET_HDB_CURVE:
				ioattr.hdbCurve= cmd_status.state;
				rw_flag= RW_HDB_CURVE;
				break;
			case	IOATTR_SET_ON_ALM:
				ioattr.onAlm= cmd_status.state;
				rw_flag= RW_ON_ALM;
				break;
			case	IOATTR_SET_OFF_ALM:
				ioattr.offAlm= cmd_status.state;
				rw_flag= RW_OFF_ALM;
				break;
			case	IOATTR_SET_NORM_STATE:
				ioattr.normalState= cmd_status.state;
				rw_flag= RW_NORM_STATE;
				break;
			default:
				sprintf(err_buf, "Command %d operate error: %d", cmd_msg.type_id, cmd_status.opr);
				Err_log(err_buf);
				continue;
			}

			InsertRecToEventTable(&rec_entry);
			SetPtIoattrById( &point, ioattr, rw_flag );
			break;
			
 		case	DPS_IO_GROUP_SET:
			data.iValue= data.iValue & 0xFFFF;
			if(data.iValue<0 || data.iValue>65535)
			{
				sprintf(err_buf, "Command %d operate %d para error: %d", 
					cmd_msg.type_id, cmd_status.opr, data.iValue);
				Err_log(err_buf);
				break;
			}
			InsertRecToEventTable(&rec_entry);
			SetPtIoGroupById( &point, (USHORT)data.iValue );
			break;

 		case	DPS_MAN_VALUE_SET:
			if( (point.data_type!=ANA_TYPE)&&(point.data_type!=PARAM_TYPE)&&(point.data_type!=CURVE_TYPE) &&(point.data_type!=OUTA_TYPE))
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			InsertRecToEventTable(&rec_entry);

			if( point.data_type==CURVE_TYPE )
			{
				seg_offset= cmd_msg.ctrl_addr;
				Swap2Byte(&seg_offset);
				if( seg_offset>=MAX_CURVE_SEG_NUM )
				{
					sprintf(err_buf, "Command %d offset error: %d", cmd_msg.type_id, seg_offset);
					Err_log(err_buf);
					break;
				}
				data.fValue= (float)(data.iValue/1000.0);
				SetCurvePtDataById( &point, &data.fValue, seg_offset, 1, time );
			}
			else
			{
				if(cmd_status.opr==0)
				{
					memset(&new_value.anlg.status, 0, 2);
					new_value.anlg.data= data.iValue;
					new_value.anlg.status.chan_fail= 0;
					iost.almEnable= 0;
					iost.scanEnable= 0;
					iost.noUpdate= 0;
					rw_flag= RW_RT_VALUE|RW_ALM_ENABLE|RW_SCAN_ENABLE|RW_CHAN_FAIL|RW_UPDATED;
					SetPtValueIostById( &point, new_value, iost, time, rw_flag );
				}
				else if(cmd_status.opr==1)
				{
				}
				else
				{
					sprintf(err_buf, "Command %d operate error: %d", cmd_msg.type_id, cmd_status.opr);
					Err_log(err_buf);
					break;
				}
			}
			break;

 		case	DPS_MAN_STAT_SET:
			if( (point.data_type!=SOE_TYPE)&&(point.data_type!=POL_TYPE)&&(point.data_type!=SYSMSG_TYPE)&&(point.data_type!=OUTS_TYPE) )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			InsertRecToEventTable(&rec_entry);

			memset(&new_value.ind, 0, 2);
			new_value.ind.rt_state= cmd_status.opr;
			iost.almEnable= 0;
			iost.scanEnable= 0;
			iost.noUpdate= 0;
			rw_flag= RW_RT_VALUE|RW_ALM_ENABLE|RW_SCAN_ENABLE|RW_CHAN_FAIL|RW_UPDATED;
			SetPtValueIostById( &point, new_value, iost, time, rw_flag );
			break;

 		case	DPS_MAN_CURVE_SET:
			if( point.data_type!=CURVE_TYPE )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}

			InsertRecToEventTable(&rec_entry);
			
			seg_offset= cmd_msg.ctrl_addr;
			Swap2Byte(&seg_offset);
			if( seg_offset>=MAX_CURVE_SEG_NUM )
			{
				sprintf(err_buf, "Command %d offset error: %d", cmd_msg.type_id, seg_offset);
				Err_log(err_buf);
				break;
			}
			data.fValue= (float)(data.iValue/1000.0);
			SetCurvePtDataById( &point, &data.fValue, seg_offset, 1, time );
			break;

		case	DPS_MAN_TIME_SET:
			if( point.data_type!=TIME_TYPE )
			{
				sprintf(err_buf, "Command %d data_type error: %d", cmd_msg.type_id, point.data_type);
				Err_log(err_buf);
				break;
			}
			
			ReadEntryById(&point, &common_all);
			common_all.var.time.update_time= time;
			strcpy(common_all.var.time.usr_name, cmd_msg.usr_login_name);
			common_all.var.time.host_id= cmd_msg.host_id;
			memcpy(&common_all.var.time.status, &cmd_status, 2);
			memcpy(&common_all.var.time.value_sec, &data.iValue, 4);
			memcpy(&common_all.var.time.value_ms, &cmd_msg.ctrl_addr, 2);
			Swap2Byte(&common_all.var.time.value_ms);
			SetTimePtVarById(&point, common_all.var.time);

			InsertRecToEventTable(&rec_entry);
			break;
			
		default:
			sprintf(err_buf, "Unsupported command type %d", cmd_msg.type_id);
			Err_log(err_buf);
			return -1;
		}/*switch( type_id )*/
	}/*for()*/

	return 0;
}



/*AGC AVC cmd log*/
void	PowerCmdLogtoFile(REC_ENTRY_UNION *rec_entry)
{
	char		text_str[512];
	char		err_buf[256];
	char		log_filename[256];
	FILE		*fp;
	REC_ENTRY_UNION	record;

	record= *rec_entry;
	SwapRecEntryByte(&record);

	if(rec_entry->ana_rec.type_id==C_APOWER)
		sprintf(log_filename, "%s%s%04d%02d%02d", AGC_LOG_PATH, AGC_LOG_FILE,
			record.cmd_rec.year, record.cmd_rec.month, record.cmd_rec.day);
	else
		sprintf(log_filename, "%s%s%04d%02d%02d", AGC_LOG_PATH, AVC_LOG_FILE,
			record.cmd_rec.year, record.cmd_rec.month, record.cmd_rec.day);

	if((fp = fopen(log_filename, "a+"))==NULL)
	{
		sprintf(err_buf, "Failed to open(create) %s", log_filename);
		Err_log(err_buf);
		return;
	}
	setbuf(fp, NULL);

	GetEventTxtString(&record, text_str);

	fprintf(fp, "%s\n", text_str);
	fclose(fp);
}



void InsertRecToEventTable(REC_ENTRY_UNION *rec_entry)
{
	long		file_size;
	char		text_str[512];
	int			curr_rec_ptr;
	char		err_buf[256];

	cur_event_rec_sn++;
	rec_entry->ana_rec.rec_sn= cur_event_rec_sn;
	Swap4Byte(&rec_entry->ana_rec.rec_sn);
	shm_event_rec_addr->rec[shm_event_rec_addr->head.header_ptr]= *rec_entry;
	SwapRecEntryByte(&shm_event_rec_addr->rec[shm_event_rec_addr->head.header_ptr]);
	curr_rec_ptr= shm_event_rec_addr->head.header_ptr;
	UpdateEventRecShmHead();

	file_size= 0;
	if(log_to_rec_file_flag && event_rec_file)
	{
		if(access(event_rec_filename, 2))
		{
			if(event_rec_file)
				fclose(event_rec_file);
			if((event_rec_file = fopen(event_rec_filename, "w+"))==NULL)
			{
				sprintf(err_buf, "Failed to open(create) %s", event_rec_filename);
				Err_log(err_buf);
			}
			else
				setbuf(event_rec_file, NULL);
		}

		fseek(event_rec_file,0L,2);
		fwrite(rec_entry, REC_ENTRY_SIZE, 1, event_rec_file);
		fseek(event_rec_file,0L,2);
		file_size= ftell(event_rec_file);
	}

	if(log_to_text_file_flag && event_text_file)
	{
		GetEventTxtString(&shm_event_rec_addr->rec[curr_rec_ptr], text_str);

		if(access(event_text_filename, 2))
		{
			if(event_text_file)
				fclose(event_text_file);
			if((event_text_file = fopen(event_text_filename, "w+"))==NULL)
			{
				sprintf(err_buf, "Failed to open(create) %s", event_text_filename);
				Err_log(err_buf);
			}
			else
				setbuf(event_text_file, NULL);
		}

		fseek(event_text_file,0L,2);
		fprintf(event_text_file, "%s\n", text_str);
		fseek(event_text_file,0L,2);
	}

	if(file_size>=REC_ENTRY_SIZE*MAX_FILE_RECORD_NUM)
	{
		event_file_sn++;
		CreateNewEventRecFile();
	}		
}


void InsertRecToInfoTable(REC_ENTRY_UNION *rec_entry)
{
	char		text_str[512];
	int			curr_rec_ptr;
	char		err_buf[256];

	cur_info_rec_sn++;
	rec_entry->ana_rec.rec_sn= cur_info_rec_sn;
	Swap4Byte(&rec_entry->ana_rec.rec_sn);
	shm_info_rec_addr->rec[shm_info_rec_addr->head.header_ptr]= *rec_entry;
	SwapRecEntryByte(&shm_info_rec_addr->rec[shm_info_rec_addr->head.header_ptr]);
	curr_rec_ptr= shm_info_rec_addr->head.header_ptr;
	UpdateInfoRecShmHead();

	if(log_to_rec_file_flag && info_rec_file)
	{
		if(access(info_rec_filename, 2))
		{
			if(info_rec_file)
				fclose(info_rec_file);
			if((info_rec_file = fopen(info_rec_filename, "w+"))==NULL)
			{
				sprintf(err_buf, "Failed to open(create) %s", info_rec_filename);
				Err_log(err_buf);
			}
			else
				setbuf(info_rec_file, NULL);
		}

		fseek(info_rec_file,0L,2);
		fwrite(rec_entry, REC_ENTRY_SIZE, 1, info_rec_file);
	}

	if(log_to_text_file_flag && event_text_file)
	{
		GetEventTxtString(&shm_info_rec_addr->rec[curr_rec_ptr], text_str);

		if(access(event_text_filename, 2))
		{
			if(event_text_file)
				fclose(event_text_file);
			if((event_text_file = fopen(event_text_filename, "w+"))==NULL)
			{
				sprintf(err_buf, "Failed to open(create) %s", event_text_filename);
				Err_log(err_buf);
			}
			else
				setbuf(event_text_file, NULL);
		}

		fseek(event_text_file,0L,2);
		fprintf(event_text_file, "%s\n", text_str);
		fseek(event_text_file,0L,2);
	}
}


void UpdateEventRecShmHead(void)
{
	USHORT		header_ptr;
	
	header_ptr= shm_event_rec_addr->head.header_ptr;
	header_ptr++;
	if(header_ptr>=MAX_SHM_RECORD_NUM)
		header_ptr= 0;
	shm_event_rec_addr->head.header_ptr= header_ptr;

	if(shm_event_rec_addr->head.count<MAX_SHM_RECORD_NUM)
		shm_event_rec_addr->head.count++;

	new_event_rec_flag= 1;
}

void UpdateInfoRecShmHead(void)
{
	USHORT		header_ptr;
	
	header_ptr= shm_info_rec_addr->head.header_ptr;
	header_ptr++;
	if(header_ptr>=MAX_INFO_RECORD_NUM)
		header_ptr= 0;
	shm_info_rec_addr->head.header_ptr= header_ptr;

	if(shm_info_rec_addr->head.count<MAX_INFO_RECORD_NUM)
		shm_info_rec_addr->head.count++;

	new_info_rec_flag= 1;
}
 
int	GetAlmCbaState(POINTER *point,int type_id,int ind_rt_state,int qlt_alm_flag)//cxs chg
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

	if(qlt_alm_flag==1)
	{
		sprintf(ptNameStr1,"%s.%s..%s",ptName.stnName,ptName.grpName,QALM_DCBA_SUFFIX_DEFINE);
	
		can_alarm =InterLock_Judge( ptNameStr1 );
		
		if(can_alarm!=-1)
		{
			if(can_alarm==FALSE)
				return can_alarm;
		}
		
		GetPtIoGroupById(point, &iogrp);
		
		sprintf(ptNameStr1,"%s.%s_%d..%s",ptName.stnName,ptName.grpName,iogrp,QALM_GCBA_SUFFIX_DEFINE);
		
		can_alarm =InterLock_Judge( ptNameStr1 );
		if(can_alarm!=-1)
		{
			if(can_alarm==FALSE)
				return can_alarm;
		}
		
		sprintf(ptNameStr1,"%s%s",ptNameStr,QALM_CBA_SUFFIX_DEFINE);
		
		can_alarm =InterLock_Judge( ptNameStr1 );
		if(can_alarm!=-1)
		{
			if(can_alarm==FALSE)
				return can_alarm;
		}

	}
	else
	{
		sprintf(ptNameStr1,"%s.%s..%s",ptName.stnName,ptName.grpName,ALM_DCBA_SUFFIX_DEFINE);	

		can_alarm =InterLock_Judge( ptNameStr1 );
		if(can_alarm!=-1)
		{
			if(can_alarm==FALSE)
				return can_alarm;
		}

		GetPtIoGroupById(point, &iogrp);

		sprintf(ptNameStr1,"%s.%s_%d..%s",ptName.stnName,ptName.grpName,iogrp,ALM_GCBA_SUFFIX_DEFINE);
		
		can_alarm =InterLock_Judge( ptNameStr1 );
		if(can_alarm!=-1)
		{
			if(can_alarm==FALSE)
				return can_alarm;
		}


		
		if(type_id==DPS_IND_ALM)
		{
			
			sprintf(ptNameStr1,"%s%s_%d",ptNameStr,ALM_CBA_SUFFIX_DEFINE,ind_rt_state);
			can_alarm =InterLock_Judge( ptNameStr1 );
			if(can_alarm!=-1)
			{
				if(can_alarm==FALSE)
					return can_alarm;
			}
			
		}


		sprintf(ptNameStr1,"%s%s",ptNameStr,ALM_CBA_SUFFIX_DEFINE);
		
		can_alarm =InterLock_Judge( ptNameStr1 );
		if(can_alarm!=-1)
		{
			if(can_alarm==FALSE)
				return can_alarm;
		}

	}



	
	return can_alarm;
}



#define	IMP_TIME_SEG_NUM	3
#define	IMP_PPEAK_START_ENTRYNAME	"PPEAK_S_TIME"
#define	IMP_PEAK_START_ENTRYNAME	"PEAK_S_TIME"
#define	IMP_VALLEY_START_ENTRYNAME	"VALLEY_S_TIME"
#define	IMP_PPEAK_END_ENTRYNAME		"PPEAK_E_TIME"
#define	IMP_PEAK_END_ENTRYNAME		"PEAK_E_TIME"
#define	IMP_VALLEY_END_ENTRYNAME	"VALLEY_E_TIME"

void	GetImpTimeType( UCHAR stn_id, DATE_TIME time, int *type )
{
	float		ppeak_time_start[IMP_TIME_SEG_NUM], ppeak_time_end[IMP_TIME_SEG_NUM];	
	float		peak_time_start[IMP_TIME_SEG_NUM], peak_time_end[IMP_TIME_SEG_NUM];	
	float		valley_time_start[IMP_TIME_SEG_NUM], valley_time_end[IMP_TIME_SEG_NUM];	
	char		err_str[128];
	POINTER		pt;
	char		entryName[ENTRY_NAME_SIZE];
	int			i;
	float		fValue, hour;

	pt.stn_id= stn_id;
	if(-1==GetSysLcuId(stn_id, &pt.dev_id))
	{
		sprintf(err_str, "Station %d no sys lcu", stn_id);
		Err_log(err_str);
		*type= IMP_NORMAL;
		return;
	}
	pt.data_type= PARAM_TYPE;

	for(i=0; i<IMP_TIME_SEG_NUM; i++)
	{
		ppeak_time_start[i]= 0;
		ppeak_time_end[i]= 0;
		peak_time_start[i]= 0;
		peak_time_end[i]= 0;
		valley_time_start[i]= 0;
		valley_time_end[i]= 0;

		sprintf(entryName, "%s%d", IMP_PPEAK_START_ENTRYNAME, i+1);
		if(-1!=GetEntryIdByName(entryName, &pt))
		{
			if(-1!=GetAnaPtfValueById(&pt, &fValue))
				ppeak_time_start[i]= fValue;
		}
		sprintf(entryName, "%s%d", IMP_PPEAK_END_ENTRYNAME, i+1);
		if(-1!=GetEntryIdByName(entryName, &pt))
		{
			if(-1!=GetAnaPtfValueById(&pt, &fValue))
				ppeak_time_end[i]= fValue;
		}
		sprintf(entryName, "%s%d", IMP_PEAK_START_ENTRYNAME, i+1);
		if(-1!=GetEntryIdByName(entryName, &pt))
		{
			if(-1!=GetAnaPtfValueById(&pt, &fValue))
				peak_time_start[i]= fValue;
		}
		sprintf(entryName, "%s%d", IMP_PEAK_END_ENTRYNAME, i+1);
		if(-1!=GetEntryIdByName(entryName, &pt))
		{
			if(-1!=GetAnaPtfValueById(&pt, &fValue))
				peak_time_end[i]= fValue;
		}
		sprintf(entryName, "%s%d", IMP_VALLEY_START_ENTRYNAME, i+1);
		if(-1!=GetEntryIdByName(entryName, &pt))
		{
			if(-1!=GetAnaPtfValueById(&pt, &fValue))
				valley_time_start[i]= fValue;
		}
		sprintf(entryName, "%s%d", IMP_VALLEY_END_ENTRYNAME, i+1);
		if(-1!=GetEntryIdByName(entryName, &pt))
		{
			if(-1!=GetAnaPtfValueById(&pt, &fValue))
				valley_time_end[i]= fValue;
		}
	}

	hour= (float)(time.hour+time.minute/60.+time.second/3600.);
	for(i=0; i<IMP_TIME_SEG_NUM; i++)
	{
		if(ppeak_time_start[i]<=hour && hour<ppeak_time_end[i])
		{
			*type= IMP_PPEAK;
			return;
		}
		if(peak_time_start[i]<=hour && hour<peak_time_end[i])
		{
			*type= IMP_PEAK;
			return;
		}
		if(valley_time_start[i]<=hour && hour<valley_time_end[i])
		{
			*type= IMP_VALLEY;
			return;
		}
	}
	*type= IMP_NORMAL;
	return;
}


int UpdateImpStatistic(POINTER *point, DATE_TIME time, GULONG imp_accum)
{
	DMS_COMMON	common_all;
	int			type;
	GLONG		increase;
	char		err_str[128];
	
	if( point->data_type!=IMP_TYPE )
	{
		sprintf(err_str, "UpdateImpStatistic() data_type error: %d", point->data_type);
		Err_log(err_str);
		return -1;
	}
	
	if(-1==ReadEntryById(point, &common_all))
	{
		sprintf(err_str, "No such point %d-%d-%d-%d in db",
			point->stn_id, point->dev_id, point->data_type, point->pt_id);
		Err_log(err_str);
		return -1;
	}

	if(imp_accum>=common_all.var.imp.entry.rt_value)
		increase= imp_accum-common_all.var.imp.entry.rt_value;
	else
	{
		FILE	*fp;
		increase= imp_accum-common_all.var.imp.entry.rt_value;
/*		increase= imp_accum;
		common_all.var.imp.entry.base_value += common_all.var.imp.entry.rt_value;
*/
		if(fp=fopen("/home/ems/h9000/tmp/imp_abnormal.txt", "a+"))
		{
			char	str[256];
			GetSysTimeStr(str);
			fprintf(fp, "\n%s: %d.%d.%d.%d imp_accum= %d rt_value= %d", str, point->stn_id,
				point->dev_id, point->data_type, point->pt_id, imp_accum, common_all.var.imp.entry.rt_value);
			fclose(fp);			
		}
	}
		
	common_all.var.imp.entry.rt_value= imp_accum;

	if(time.year!=common_all.var.imp.entry.update_time.year)
	{
		common_all.var.imp.entry.ppeak.year= 0;
		common_all.var.imp.entry.peak.year= 0;
		common_all.var.imp.entry.valley.year= 0;
		common_all.var.imp.entry.normal.year= 0;
		common_all.var.imp.entry.init.year= common_all.var.imp.entry.base_value
			+common_all.var.imp.entry.rt_value;
		common_all.var.imp.entry.last.year= common_all.var.imp.entry.sum.year;
		common_all.var.imp.entry.sum.year= 0;
	}	
	if(time.month!=common_all.var.imp.entry.update_time.month)
	{
		common_all.var.imp.entry.ppeak.month= 0;
		common_all.var.imp.entry.peak.month= 0;
		common_all.var.imp.entry.valley.month= 0;
		common_all.var.imp.entry.normal.month= 0;
		common_all.var.imp.entry.init.month= common_all.var.imp.entry.base_value
			+common_all.var.imp.entry.rt_value;
		common_all.var.imp.entry.last.month= common_all.var.imp.entry.sum.month;
		common_all.var.imp.entry.sum.month= 0;
	}	
	if(time.day!=common_all.var.imp.entry.update_time.day)
	{
		common_all.var.imp.entry.ppeak.day= 0;
		common_all.var.imp.entry.peak.day= 0;
		common_all.var.imp.entry.valley.day= 0;
		common_all.var.imp.entry.normal.day= 0;
		common_all.var.imp.entry.init.day= common_all.var.imp.entry.base_value
			+common_all.var.imp.entry.rt_value;
		common_all.var.imp.entry.last.day= common_all.var.imp.entry.sum.day;
		common_all.var.imp.entry.sum.day= 0;
	}	

	GetImpTimeType(point->stn_id, time, &type);
	switch(type)
	{
	case IMP_PPEAK:
		common_all.var.imp.entry.ppeak.year += increase;
		common_all.var.imp.entry.ppeak.month += increase;
		common_all.var.imp.entry.ppeak.day += increase;
		break;

	case IMP_PEAK:
		common_all.var.imp.entry.peak.year += increase;
		common_all.var.imp.entry.peak.month += increase;
		common_all.var.imp.entry.peak.day += increase;
		break;

	case IMP_VALLEY:
		common_all.var.imp.entry.valley.year += increase;
		common_all.var.imp.entry.valley.month += increase;
		common_all.var.imp.entry.valley.day += increase;
		break;

	case IMP_NORMAL:
		common_all.var.imp.entry.normal.year += increase;
		common_all.var.imp.entry.normal.month += increase;
		common_all.var.imp.entry.normal.day += increase;
		break;
	}

	common_all.var.imp.entry.sum.year= common_all.var.imp.entry.ppeak.year
		+common_all.var.imp.entry.peak.year+common_all.var.imp.entry.valley.year
		+common_all.var.imp.entry.normal.year;
	common_all.var.imp.entry.sum.month= common_all.var.imp.entry.ppeak.month
		+common_all.var.imp.entry.peak.month+common_all.var.imp.entry.valley.month
		+common_all.var.imp.entry.normal.month;
	common_all.var.imp.entry.sum.day= common_all.var.imp.entry.ppeak.day
		+common_all.var.imp.entry.peak.day+common_all.var.imp.entry.valley.day
		+common_all.var.imp.entry.normal.day;

	common_all.var.imp.entry.d_accum
		= (float)((common_all.var.imp.entry.rt_value+common_all.var.imp.entry.base_value)
		*(common_all.var.imp.entry.eng_para.CT
		*common_all.var.imp.entry.eng_para.PT
		*common_all.var.imp.entry.eng_para.ENG_CVT/common_all.var.imp.entry.eng_para.YEAR_RATE));

	return SetImpPtVarById(point, common_all.var.imp.entry);
}


void UpdateImpData(IMP_DATA *imp, int increase)
{
	GULONG		value;

	if(increase>0)
	{
		imp->year += increase;
		imp->month += increase;
		imp->day += increase;
	}
	else
	{
		value= -increase;
		if(imp->year>=value)
			imp->year -= value;
		else
			imp->year= 0;

		if(imp->month>=value)
			imp->month -= value;
		else
			imp->month= 0;

		if(imp->day>=value)
			imp->day -= value;
		else
			imp->day= 0;
	}
}


int	FloattoRecentInt(double fValue)
{
	int		iValue;

	iValue= (int)fValue;
	if(fValue<0)
	{
		if((iValue-fValue)>=0.5)
			return iValue-1;
		else
			return iValue;		
	}
	else
	{
		if((fValue-iValue)>=0.5)
			return iValue+1;
		else
			return iValue;
	}
}


GULONG	UFloattoRecentUInt(double fValue)
{
	int		iValue;

	iValue= (GULONG)fValue;
	if((fValue-iValue)>=0.5)
		return iValue+1;
	else
		return iValue;
}
