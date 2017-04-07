#include	"../../../dps/v4.0a/inc/dps_functions_def.h" 
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "../lib/dpslib.lib")
#pragma comment(lib, "../lib/ipclib.lib")
#pragma comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma comment(lib, "../../../lan/v4.0a/lib/lan.lib")


int	main(int argc, char *argv[])
{
	SYS_COMM_TABLE	*shm_syscomm_addr;
	EVENT_SEARCH	search;
	DATE_TIME		rec_file_time;
	FILE			*rec_fp, *dest_fp;
	char			rec_filename[256], dest_filename[256];
	int				rec_file_no, i, rec_num;
	long			file_size, rec_time, start_tt, end_tt, search_file_tt, curr_time_tt;
	char			err_buf[256];
	REC_ENTRY_UNION	rec;
	int				search_end_flag= 0;
	struct tm		file_tm;
	POINTER			point;
	char			ptNameStr[POINT_NAME_SIZE], ptLongname[POINT_LONGNAME_SIZE];

	CreateLockFile(".event_search.lock");
	if(-1==InitSysLog(H9000_MODULE_EVENTSEARCH, "EventSearch"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}


  	if(-1==MountDB(NULL))
  	{
		Err_log("Fail to MountDB!\n");
		exit(-1);
  	} 

	/***************      打开系统通信共享内存    **********/
	shm_syscomm_addr= (SYS_COMM_TABLE*) LinktoShareMem( SHM_SYSCOMM_TABLE_KEY_CHAR );
	if( shm_syscomm_addr == (SYS_COMM_TABLE*)-1 )
	{
		Err_log("Fail to link sys_comm shm");
		return -1;
	}
	shm_syscomm_addr->event_search.event_num= 0;
	search= shm_syscomm_addr->event_search;
	if(search.search_flag!=1)
		return 0;

	curr_time_tt= time(NULL);
	start_tt= GetAbsTimeFromDateTime(&search.start_t);
	end_tt= GetAbsTimeFromDateTime(&search.end_t);

	search.event_num= 0;
	if(end_tt<start_tt)
		return 0;

	sprintf(dest_filename, "%s%s", SYS_TMP_PATH, EVENT_SEARCH_TMP_FILE); 
	if(NULL==(dest_fp=fopen(dest_filename, "wb")))
	{
		sprintf(err_buf, "Fail to create %s", dest_filename);
		Err_log(err_buf);
		return -1;
	}

	rec_file_time= search.start_t;
	rec_file_time.hour= 0;
	rec_file_time.minute= 0;
	rec_file_time.second= 0;
	search_file_tt= GetAbsTimeFromDateTime(&rec_file_time);

	rec_file_no= 1;
	while(1)
	{
#ifdef ENGLISH_VER
		sprintf(rec_filename, "%sEVENT%02d-%s-%04dNo%02d.REC", EVENT_LOG_PATH,
			rec_file_time.day, mthstr[rec_file_time.month], rec_file_time.year, rec_file_no++); 
#else                
		sprintf(rec_filename, "%sEVENT%04d%02d%02dNo%02d.REC", EVENT_LOG_PATH,
			rec_file_time.year, rec_file_time.month, rec_file_time.day, rec_file_no++); 
#endif
		if(NULL==(rec_fp=fopen(rec_filename, "rb")))
		{
			search_file_tt +=60*60*24;
			if(search_file_tt>=curr_time_tt)
				break;

			file_tm= *localtime(&search_file_tt);
			rec_file_time.year= file_tm.tm_year+1900;
			rec_file_time.month=file_tm.tm_mon+1;
			rec_file_time.day= file_tm.tm_mday;
			rec_file_no= 1;
			continue;
		}
		
		fseek(rec_fp,0L,2);
		file_size = ftell(rec_fp);
		if(file_size==0 || (file_size%REC_ENTRY_SIZE) || file_size>(MAX_FILE_RECORD_NUM*REC_ENTRY_SIZE))
		{
			if(file_size)
			{
				sprintf(err_buf, "File %s length error %d", rec_filename, file_size);
				Err_log(err_buf);
			}
			fclose(rec_fp);
			continue;
		}
		fseek(rec_fp, -(int)REC_ENTRY_SIZE, 2);
		if(1!=fread(&rec, REC_ENTRY_SIZE, 1, rec_fp))
		{
			sprintf(err_buf, "Fail to read file %s", rec_filename);
			Err_log(err_buf);
			fclose(rec_fp);
			continue;
		}

		SwapRecEntryByte(&rec);
		rec_time= GetTimefromRec(&rec);
		if(rec_time<start_tt)
		{
			fclose(rec_fp);
			continue;
		}
		
		rec_num= file_size/REC_ENTRY_SIZE;
		fseek(rec_fp, 0, 0);
		for(i=0; i<rec_num; i++)
		{
			if(1!=fread(&rec, REC_ENTRY_SIZE, 1, rec_fp))
			{
				sprintf(err_buf, "Fail to read file %s", rec_filename);
				Err_log(err_buf);
				fclose(rec_fp);
				break;
			}

			SwapRecEntryByte(&rec);
			rec_time= GetTimefromRec(&rec);
			if(rec_time<start_tt)
				continue;
			if(rec_time>end_tt)
			{
				search_end_flag= 1;
				break;
			}
			
			if(-1==GetPointFromRec(&rec, &point))
				continue;
			if(search.search_type==EVENT_SEARCH_POINT_ID)
			{
				if(search.pt_id.stn_id!=point.stn_id || search.pt_id.dev_id!=point.dev_id
					|| search.pt_id.data_type!=point.data_type || search.pt_id.pt_id!=point.pt_id)
					continue;
			}
			else if(search.search_type==EVENT_SEARCH_POINT_NAME)
			{
				if(-1==GetPtNameById(&point, ptNameStr, NULL))
					continue;
				if(!strstr(ptNameStr, search.search_str))
					continue;
			}
			else if(search.search_type==EVENT_SEARCH_POINT_LONGNAME)
			{
				if(-1==GetPtLongnameById(&point, ptLongname))
					continue;
				if(!strstr(ptLongname, search.search_str))
					continue;
			}
			SwapRecEntryByte(&rec);
			fwrite(&rec, REC_ENTRY_SIZE, 1, dest_fp);
			search.event_num++;
		}
		fclose(rec_fp);
		if(search_end_flag)
			break;
	}
	
	fclose(dest_fp);
	shm_syscomm_addr->event_search.search_flag= 0;
	shm_syscomm_addr->event_search.event_num= search.event_num;
	return 0;
}
