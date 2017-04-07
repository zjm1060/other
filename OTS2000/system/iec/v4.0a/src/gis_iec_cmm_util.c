#include "../inc/gis_iec_cmm_def.h"
#include "../inc/gis_iec104_com.h"
#include "../../../dps/v4.0a/inc/dps_functions_def.h"
#include	<stdarg.h>




int		ouput_info_filter= OUTPUT_ERROR;
#define MAX_THREAD_NUM	12
struct 
{
	struct 
	{
		unsigned int id;
		FILE		*fp;
		int			day;
		int			stn_no;
	} file[MAX_THREAD_NUM];
} dsp_info_file;



IEC_COMM_DSP_BUF *comm_dsp_buf_ptr= (IEC_COMM_DSP_BUF *)-1;

void SetOutputFilter(int filter)
{
	ouput_info_filter= filter;
}

#ifdef SUN_OS
#define GetCurrentThreadId()	pthread_self()	
#endif

#ifdef WINDOWS_OS
void CreateCommDspBuf(void)
{
	HANDLE		hMapObject;
	char		name[128];
	
	sprintf(name, "IEC_COMM_DSP_BUF");
	hMapObject= CreateFileMapping((HANDLE)-1, NULL, PAGE_READWRITE, 0, sizeof(IEC_COMM_DSP_BUF), name);
	if( hMapObject == NULL )
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to CreateFileMapping: %s errno=%d\n", name, GetLastError());
		return;
	}
	
	comm_dsp_buf_ptr = MapViewOfFile (hMapObject,FILE_MAP_ALL_ACCESS,0, 0, 0);  
	if (comm_dsp_buf_ptr == NULL)
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to MapViewOfFile: %s errno=%d\n", name, GetLastError());
		CloseHandle(hMapObject);
		comm_dsp_buf_ptr= (IEC_COMM_DSP_BUF *)-1;
		return;
	}
}
void*	LinktoCommDspBuf(void)
{
	HANDLE	hMapObject;
	void	*shm_addr;
	char	shm_name[128];
	
	sprintf(shm_name, "IEC_COMM_DSP_BUF");
    hMapObject = OpenFileMapping (FILE_MAP_ALL_ACCESS, FALSE, shm_name);
	if (hMapObject == NULL)
	{
		OutputDspInfo(OUTPUT_ERROR, "Fail to OpenFileMapping: %s errno=%d\n", shm_name, GetLastError());
		return (void*)-1;
	}
	shm_addr = MapViewOfFile (hMapObject,FILE_MAP_ALL_ACCESS,0, 0, 0);  
	if (shm_addr == NULL)
	{
		OutputDspInfo(OUTPUT_ERROR, "Fail to MapViewOfFile: %s errno=%d\n", shm_name, GetLastError());
		CloseHandle(hMapObject);
		return (void*)-1;
	}
	CloseHandle(hMapObject);
	return shm_addr;
}

#else if	SUN_OS
void CreateCommDspBuf(void)
{
	char	err_str[128];
	int		shm_id;
	void	*shm_addr;
	key_t	key;
	char	keyfile[128];
	char	*shm_name= "IEC104_COMM_DSP_BUF";
	
	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	if( (key=ftok(keyfile, 'b'))== -1 )
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to generate %s key: %d!", shm_name, errno);
		return;
	}
	if( (shm_id=shmget(key, sizeof(IEC_COMM_DSP_BUF), IPC_CREAT|0660)) == -1 )
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to create %s share memory: %d!", shm_name, errno);
		return;
	}
    
    if( (comm_dsp_buf_ptr=shmat(shm_id, 0, 0)) == (void*)-1 )
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to shmat %s share memory: %d!", shm_name, errno);
		return;
	}
	OutputDspInfo(0,"\nCreate %s share memory: 0x%x", shm_name, key);
}
void*	LinktoCommDspBuf(void)
{
	key_t	key;
	int		shm_id;
	char	err_str[128];
	char	keyfile[128];
	
	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	if( (key=ftok(keyfile, 'b'))== -1 )
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to generate IEC104_COMM_DSP_BUF key!\n");
		return (void*)-1;
	}
	if( (shm_id=shmget(key, 0, 0660)) == -1 )
	{
		OutputDspInfo(OUTPUT_ERROR,"Fail to shmget IEC104_COMM_DSP_BUF share memory!");
		return (void*)-1;
	}
	
    return shmat(shm_id, 0, 0);
}
#endif

void OutputDspInfo(
				   int nFilter,	// how to handle and where to report
				   // 0: default - normal flag
				   // else flags
				   const char* format,// from here the function uses printf syntax
				   ...
				   )
{
	int		bufptr,len,charlen, i;
    va_list ap;
    char	buf[1024], *ptr= buf;
	char	timeStr[128];
	unsigned int threadId;
	DATE_TIME	curr_time;

	if(nFilter<ouput_info_filter)
		return;
	
	GetSysShortTimeStr(timeStr);
	
	threadId= GetCurrentThreadId();

	ptr += sprintf(buf, "\n%d %s: ", threadId, timeStr);
	
    va_start(ap, format);
    ptr += vsprintf(ptr, format, ap);
	va_end(ap);
	*ptr= 0;
	
	printf("%s", buf);
	
	for(i=0; i<MAX_THREAD_NUM; i++)
	{
		if(dsp_info_file.file[i].id==threadId)
			break;
	}
	if(i<MAX_THREAD_NUM && dsp_info_file.file[i].fp)
	{
		GetSysTime(&curr_time);
		if(dsp_info_file.file[i].day != curr_time.day)
		{
			char	filename[256];
			
			fclose(dsp_info_file.file[i].fp);

			if(dsp_info_file.file[i].stn_no==-1)
				sprintf(filename, "%s%s_COM_LOG_MAIN%04d%02d%02d.txt", IEC_LOGPATH, "IEC104",
				curr_time.year, curr_time.month, curr_time.day);
			else
				sprintf(filename, "%s%s_COM_LOG%d_%d_%04d%02d%02d.txt", IEC_LOGPATH, "IEC104", 
				dsp_info_file.file[i].stn_no, threadId, curr_time.year, curr_time.month, curr_time.day);
			
			dsp_info_file.file[i].fp= fopen(filename, "a");
			dsp_info_file.file[i].day= curr_time.day;
		}

		fprintf(dsp_info_file.file[i].fp, "%s", buf);
		fflush(dsp_info_file.file[i].fp);
	}
	
	if ((long *)comm_dsp_buf_ptr==(long *)-1)
		return;
	
	bufptr = comm_dsp_buf_ptr->header_ptr;
	len = strlen(buf);
	charlen = DSP_INFO_SIZE - bufptr;
	if(charlen < len)
	{
		memcpy(&comm_dsp_buf_ptr->buf[bufptr],buf,charlen);
		bufptr = len - charlen;
		memcpy(&comm_dsp_buf_ptr->buf[0],&buf[charlen],bufptr);
		comm_dsp_buf_ptr->header_ptr = bufptr;
	}
	else
	{
		memcpy(&comm_dsp_buf_ptr->buf[bufptr],buf,len);
		charlen  = bufptr + len;
		if(charlen >= DSP_INFO_SIZE)
			comm_dsp_buf_ptr->header_ptr = 0;
		else
			comm_dsp_buf_ptr->header_ptr = charlen;
	}
}


void ClearOutputFilterState(void)
{
	int	i;

	for(i=0; i<MAX_THREAD_NUM; i++)
	{
		if(dsp_info_file.file[i].fp)
			fclose(dsp_info_file.file[i].fp);
	}
	memset(&dsp_info_file, 0, sizeof(dsp_info_file));
}

void InitOutputFilter(char *comm_task, int stn_no)
{
	char filename[256];
	FILE	*fp;
	int		i;
	unsigned int threadId= GetCurrentThreadId();
	DATE_TIME	curr_time;

	ouput_info_filter= OUTPUT_ERROR;
	if(stn_no==-1)
		sprintf(filename, "%s%s_LOG.def", IEC_HOMEPATH, comm_task);
	else
		sprintf(filename, "%s%s_LOG%d.def", IEC_HOMEPATH, comm_task, stn_no);
	if(fp= fopen(filename, "r"))
	{
		char	buff[256];
		
		while(fgets(buff,256,fp))
		{
			if(buff[0]=='!' || buff[0]=='#')
				continue;
			
			if(1!=sscanf(buff, "%d", &ouput_info_filter))
				continue;

			break;
		}
		
		fclose(fp);
	}
	

	GetSysTime(&curr_time);

	if(stn_no==-1)
		sprintf(filename, "%s%s_COM_LOG_MAIN%04d%02d%02d.txt", IEC_LOGPATH, comm_task,
			curr_time.year, curr_time.month, curr_time.day);
	else
		sprintf(filename, "%s%s_COM_LOG%d_%d_%04d%02d%02d.txt", IEC_LOGPATH, comm_task, 
		stn_no, threadId, curr_time.year, curr_time.month, curr_time.day);

	for(i=0; i<MAX_THREAD_NUM; i++)
	{
		if(dsp_info_file.file[i].fp==0)
			break;
	}
	if(i>=MAX_THREAD_NUM)
		return;
	dsp_info_file.file[i].fp= fopen(filename, "a");
	dsp_info_file.file[i].id= threadId;
	dsp_info_file.file[i].day= curr_time.day;
	dsp_info_file.file[i].stn_no= stn_no;
}


void	PrintSendData(unsigned char sendbuf[], unsigned int nbsend)
{
	unsigned int d;
	char	infobuf[1024], *ptr= infobuf;
	
	ptr += sprintf(ptr, "Send data byte num %u:",nbsend);
	for(d=0;d<nbsend;d++)
	{
		if((d==0) || (d==6) || ((d>6)&&(!((d-6)%20))))
			ptr += sprintf(ptr,"\n<< ");
		else if((d>6)&&(!((d-6)%10)))
			ptr += sprintf(ptr,"  ");
		
		ptr += sprintf(ptr,"%02X ",sendbuf[d]);
		
		if((ptr-infobuf)>=1000)
		{
			*ptr= 0;
			OutputDspInfo(0, "%s", infobuf);
			ptr= infobuf;
		}
	}
	ptr += sprintf(ptr,"\n");
	*ptr= 0;
	OutputDspInfo(0, "%s", infobuf);
}

void	PrintRecvData(unsigned char recvbuf[], unsigned int nbrecv)
{
	unsigned int d;
	char	infobuf[1024], *ptr= infobuf;
	
	ptr += sprintf(ptr, "Received data byte num %u:",nbrecv);
	for(d=0;d<nbrecv;d++)
	{
		if((d==0) || (d==6) || ((d>6)&&(!((d-6)%20))))
			ptr += sprintf(ptr,"\n>> ");
		else if((d>6)&&(!((d-6)%10)))
			ptr += sprintf(ptr,"  ");
		
		ptr += sprintf(ptr,"%02X ",recvbuf[d]);
		
		if((ptr-infobuf)>=1000)
		{
			*ptr= 0;
			OutputDspInfo(0, "%s", infobuf);
			ptr= infobuf;
		}
	}
	ptr += sprintf(ptr,"\n");
	*ptr= 0;
	OutputDspInfo(0, "%s", infobuf);
}


int UpdateHostCommState(char *host, char *comm_name, int chan_no, unsigned short status)
{
	char	name_buf[128];
	POINTER		point;
	USHORT	old_state;
	
	if(GetHostStnId(host, &point.stn_id))
	{
		OutputDspInfo(OUTPUT_ERROR, "GetHostStnId() %s No this hostname point in database file!\n",host);
		return -1;
	}
	
	if(GetHostSysLcuId(host, &point.dev_id))
	{
		OutputDspInfo(OUTPUT_ERROR, "GetHostSysLcuId() %s No this hostname point in database file!\n",host);
		return -1;
	}
	
	point.data_type= SYSMSG_TYPE;
	
	sprintf(name_buf, "%s_%s%d", host, comm_name,chan_no);
	if(-1==GetEntryIdByName(name_buf, &point))
	{
		OutputDspInfo(OUTPUT_ERROR, "%s No this com channel status point in db!\n",	name_buf);
		return -1;
	}
	
	GetIndPtRtStateById(&point, &old_state);
	if(old_state != status)
	{
		BroadCastIndPtState(point, 0, status);
		OutputDspInfo(OUTPUT_WARN, "update %s %s chan %d state %d", host, comm_name,chan_no,status);
	}
	return 0;
}


int GetHostCommState(char *host, char *comm_name, int chan_no, unsigned short *status)
{
	char	name_buf[128];
	POINTER		point;
	
	if(GetHostStnId(host, &point.stn_id))
	{
		OutputDspInfo(OUTPUT_ERROR, "GetHostStnId() %s No this hostname point in database file!\n",host);
		*status= 0;
		return -1;
	}
	
	if(GetHostSysLcuId(host, &point.dev_id))
	{
		OutputDspInfo(OUTPUT_ERROR, "GetHostSysLcuId() %s No this hostname point in database file!\n",host);
		*status= 0;
		return -1;
	}
	
	point.data_type= SYSMSG_TYPE;
	
	sprintf(name_buf, "%s_%s%d", host, comm_name,chan_no);
	if(-1==GetEntryIdByName(name_buf, &point))
	{
		OutputDspInfo(OUTPUT_ERROR, "%s No this com channel status point in db!\n",	name_buf);
		*status= 0;
		return -1;
	}
	
	return GetIndPtRtStateById(&point, status);
}

int UpdateCtrlPrivChan(char *stnName, char *host, unsigned short ctrlPrivChannel)
{
	char	name_buf[128];
	POINTER		point;
	USHORT	old_state;
	
	if(GetHostStnId(host, &point.stn_id))
	{
		OutputDspInfo(OUTPUT_ERROR, "GetHostStnId() %s No this hostname point in database file!\n",host);
		return -1;
	}
	
	if(GetHostSysLcuId(host, &point.dev_id))
	{
		OutputDspInfo(OUTPUT_ERROR, "GetHostSysLcuId() %s No this hostname point in database file!\n",host);
		return -1;
	}
	
	point.data_type= SYSMSG_TYPE;
	
	sprintf(name_buf, "%s_CTRL_PRIV_CHAN", stnName);
	if(-1==GetEntryIdByName(name_buf, &point))
	{
		OutputDspInfo(OUTPUT_ERROR, "%s No this com ctrl priv channel point in db!\n",	name_buf);
		return -1;
	}
	
	GetIndPtRtStateById(&point, &old_state);
	if(old_state != ctrlPrivChannel)
	{
		BroadCastIndPtState(point, 0, ctrlPrivChannel);	
		OutputDspInfo(OUTPUT_WARN, "update ctrl priv channel: %d", ctrlPrivChannel);
	}
	return 0;
}



int GetCtrlPrivChannel(char *stnName, char *host, unsigned short *ctrlPrivChannel)
{
	char	name_buf[128];
	POINTER		point;
	
	if(GetHostStnId(host, &point.stn_id))
	{
		OutputDspInfo(OUTPUT_ERROR, "GetHostStnId() %s No this hostname point in database file!\n",host);
		*ctrlPrivChannel= 0;
		return -1;
	}
	
	if(GetHostSysLcuId(host, &point.dev_id))
	{
		OutputDspInfo(OUTPUT_ERROR, "GetHostSysLcuId() %s No this hostname point in database file!\n",host);
		*ctrlPrivChannel= 0;
		return -1;
	}
	
	point.data_type= SYSMSG_TYPE;
	
	sprintf(name_buf, "%s_CTRL_PRIV_CHAN", stnName);
	if(-1==GetEntryIdByName(name_buf, &point))
	{
		OutputDspInfo(OUTPUT_ERROR, "%s No this com ctrl priv channel point in db!\n",	name_buf);
		*ctrlPrivChannel= 0;
		return -1;
	}
	
	return GetIndPtRtStateById(&point, ctrlPrivChannel);
}


