#include "/home/ems/system/ots/ots_hdb/inc/ots.h"

int	n_sequence,n_scene, n_step,n_show, n_group, n_entry, n_init,n_sceneseg, n_tfb;
int	n_timer, n_unit, n_model, n_nlf, n_nlf_data,n_param,n_param_seq, n_spare, n_nlf_crv;
int	ots_status;
int	next_step_mask;
double	time_cnt;
char g_ots_hdb_type[10];
char g_ots_hdb_ip[40];
char g_ots_hdb_sid[10];
int	nlf_rd_st;
char tch_bg_pic[50];
int g_ots_ver;
int g_all_send;
int g_hdb_write;

SEQUENCE_DEF	sequencedb[MAX_SEQUENCE];
SCENE_DEF		scenedb[MAX_SCENE];
STEP_DEF		stepdb[MAX_STEP];
SHOW_DEF		showdb[MAX_SHOW];
GROUP_DEF		groupdb[MAX_OR];
ENTRY_DEF		entrydb[MAX_AND];
INIT_DEF		initdb[MAX_INIT];
SCENESEG_DEF	scenesegdb[MAX_SCENESEG];
TIMER_DEF		timerdb[MAX_TIMER];
MODEL_DEF		mdb[MAX_MODEL_NUMBER];
TFB_DEF			tfbdb[MAX_TFB_NUM];
PARAM_DEF		paramdb[MAX_PARAM_NUM],paramseqdb[MAX_PARAM_NUM];
SPARE_DEF		sparedb[MAX_SPARE_NUMBER];
NLF_DEF			nlfdb[MAX_NLF];
HE_PTR_DEF		nlf_ptr[MAX_CURVES];
NLF_DATA_DEF	nlfdata;
UNIT_DATA_DEF	unitdb[MAX_UNIT_NUMBER];
SEQID_DEF		seqid,seqid_auto;

int CvtArrayToStr(char *str,int len)
{
	int i;
	if( *(str+len-1) != ' ')
	{
		return TRUE;
	}
	*(str+len-1) = '\0'; 
	for(i=len-2;i>0;i--)
	{
		if(*(str+i) != ' ')
		{
			break;
		}
	}
	*(str+i+1) = '\0'; 
	return TRUE;
}


void GetCurTimeStr( char * timestr)
{
	time_t now;
	struct tm datetime;
	time(&now);
	datetime=*localtime(&now);
	tmtostring(datetime,timestr);
	return;
}

int tmtostring(struct tm dtime, char *str)
{
	sprintf(str,"%4d-%02d-%02d %02d:%02d:%02d",dtime.tm_year+1900,dtime.tm_mon+1,  
		dtime.tm_mday,dtime.tm_hour,dtime.tm_min,dtime.tm_sec);
	return TRUE;
}

int	WriteRec(char *str)
{
	BdSms(str);
	return TRUE;
}

void ReadOtsDefFile()
{
	char FileName[100]="/home/ems/h9000/def/otsconfig.def";
	char StrTmp[100];
	FILE *fp;
	if (( fp = fopen ( FileName,"r"))==NULL)
	{
		printf ("==== can't open (w) the file %s! \n",FileName );
		return;
	}
	else
	{
		/* printf ( "\n==== open (w) %s  success! \n",FileName  ); */
	}
	while ( fscanf(fp,"%s",StrTmp)!=EOF )
	{
		if(strcmp(StrTmp,"BEGIN")==0) break;
	}        
	while ( fscanf(fp,"%s",StrTmp)!=EOF )
	{
		if(strcmp(StrTmp,"HDBTYPE")==0)
		{
			fscanf(fp,"%s",StrTmp);
			strcpy(g_ots_hdb_type,StrTmp);
		}
		if(strcmp(StrTmp,"HDBSID")==0)
		{
			fscanf(fp,"%s",StrTmp);
			strcpy(g_ots_hdb_sid,StrTmp);
		}
		if(strcmp(StrTmp,"HDBIP")==0)
		{
			fscanf(fp,"%s",StrTmp);
			strcpy(g_ots_hdb_ip,StrTmp);
		}
		if(strcmp(StrTmp,"TCHPIC")==0)
		{
			fscanf(fp,"%s",StrTmp);
			strcpy(tch_bg_pic,StrTmp);
		}
		if (strcmp(StrTmp,"VER")==0)
		{
			fscanf(fp,"%s",StrTmp);
			if (strcmp(StrTmp,"SIM_TRAIN")==0)
			{
				g_ots_ver = 0;
			}
			else if (strcmp(StrTmp,"SIM_OPT")==0)
			{
				g_ots_ver = 1;
			}
		}
		if(strcmp(StrTmp,"ALLSEND")==0)
		{
			fscanf(fp,"%d",&g_all_send);
		}
		if(strcmp(StrTmp,"HDBWRITE")==0)
		{
			fscanf(fp,"%d",&g_hdb_write);
		}
		if(strcmp(StrTmp,"END")==0)
		{
			break;
		}
	}
	fclose(fp);
}

unsigned short	GetCharBits (unsigned short v,unsigned short start_bit,unsigned short bit_num )
{
	unsigned short	tmp;
	unsigned short	tmp2	=0;

	tmp	= ( v >> start_bit ) & ( ~( (~tmp2) << bit_num ) );

	return	tmp;

}

int GetPointNumber( POINTER * p_point )
{
	unsigned int num=0;
	if(GetEntryNumById( p_point->stn_id, p_point->dev_id, p_point->data_type, &num ) == -1)
	{
		return FALSE;
	}
	return num;
}

int SetPtScanEnable(POINTER *p_pt, unsigned short status)
{
	DMS_COMMON      rs;
	unsigned short	state;
	WR_VAR_UNION new_value;
	DATE_TIME	date_time;

	if(IsPtScanEnableById( p_pt, &state ) == -1 )
	{
		return FALSE;
	}
	if(state == status) return TRUE;
	if(ReadEntryById( p_pt, &rs) ==-1) return FALSE;
	memset(&new_value,0,sizeof(WR_VAR_UNION));
	GetSysTime(&date_time);
	rs.fixed.iost.scanEnable = status;
	if(SetPtValueIostById( p_pt, new_value, rs.fixed.iost, date_time, RW_SCAN_ENABLE )==-1)
	{
		return FALSE;
	}
	return TRUE;
}

int SetAllPtScanEnable()
{
	int i,j,DataType,Number,PointId;
	POINTER			point;
	for(i=1;i<=g_stn_num;i++)
	{
		for(j=1;j<=g_dev_num[i];j++)
		{
			for(DataType=1;DataType<=MAX_DATATYPE_NUM;DataType++)
			{
				point.stn_id	  =	i;	
				point.dev_id      =	j;
				point.data_type   =	DataType;
				point.pt_id    =	0;
				Number = GetPointNumber ( &point );
				for(PointId=0;PointId<Number;PointId++)
				{
					point.pt_id    =	PointId;
					SetPtScanEnable(&point, 1);
				}
			}
		}
	}
	return TRUE;
}

int ReadDBByName( char *name,  DB_DATA *db)
{ 
	DMS_COMMON      rs;
	int             datatype;  
	if(ReadEntryByNameStr( name, &rs) ==-1) return FALSE;
	datatype = rs.point.data_type; 
	db->dev_id    = rs.point.dev_id;
	db->data_type = rs.point.data_type;
	db->point_id  = rs.point.pt_id;
	db->ioattr	 = rs.fixed.ioattr;

	if((datatype == SOE_TYPE)||(datatype == POL_TYPE)||(datatype == SYSMSG_TYPE)
		||(datatype == OUTS_TYPE))
	{ 
 		memcpy(&db->status, &rs.var.ind.status,2);
 		db->quality	= GetCharBits(db->status,14,2); 
		db->year	 = rs.var.ind.update_time.year;    
		db->month	 = rs.var.ind.update_time.month;     
		db->day	 = rs.var.ind.update_time.day;  
		db->hour	 = rs.var.ind.update_time.hour;
		db->minute    = rs.var.ind.update_time.minute;
		db->second    = rs.var.ind.update_time.second;
		db->ms	 = rs.var.ind.update_time.ms;
	}
 	else if(datatype == DEVOBJ_TYPE)
 	{
 		memcpy(&db->status, &rs.var.devobj.fb_status1,2);
 		db->quality	= 0; 
		db->year	 = rs.var.devobj.update_time.year;    
		db->month	 = rs.var.devobj.update_time.month;     
		db->day	 = rs.var.devobj.update_time.day;  
		db->hour	 = rs.var.devobj.update_time.hour;
		db->minute    = rs.var.devobj.update_time.minute;
		db->second    = rs.var.devobj.update_time.second;
		db->ms	 = rs.var.devobj.update_time.ms;	
	}
 	else if((datatype == ANA_TYPE)||(datatype == PARAM_TYPE) || (datatype == OUTA_TYPE))
 	{ 
   		memcpy(&db->status,&rs.var.anlg.value.status,2);
   		db->status	= GetCharBits(db->status,14,2);
  		db->fvalue	= (float)rs.var.anlg.value.data/1000;
   		db->year	= rs.var.anlg.update_time.year;    
   		db->month	= rs.var.anlg.update_time.month;     
   		db->day		= rs.var.anlg.update_time.day;  
   		db->hour	= rs.var.anlg.update_time.hour;
   		db->minute	= rs.var.anlg.update_time.minute;
   		db->second    = rs.var.anlg.update_time.second;   
 	}
 	else if((datatype == IMP_TYPE))
 	{ 
   		db->fvalue = rs.var.imp.entry.d_accum;
		db->LongValue = rs.var.imp.entry.base_value;
   		db->year	 = rs.var.anlg.update_time.year;    
   		db->month	 = rs.var.anlg.update_time.month;     
   		db->day	 = rs.var.anlg.update_time.day;  
   		db->hour	 = rs.var.anlg.update_time.hour;
   		db->minute    = rs.var.anlg.update_time.minute;
   		db->second    = rs.var.anlg.update_time.second;  
 	}
 	else 
 	{ 
		return(FALSE); 
	}
	 return(TRUE);
}


int BdPollingCmd(unsigned char stnid, unsigned char devid)
{      
	COMMAND_MSG     msg;
	LAN_HEAD        head;
	DATE_TIME	date_time;
	
	SetAllPtScanEnable();
	GetSysTime(&date_time);
	
	msg.type_id= C_DATA_POLLING;
	msg.stn_id= stnid;
	msg.dev_id= devid;
	msg.data_type= SYSMSG_TYPE;
	msg.point_id= 0;
	msg.status.src= 0;
	msg.data.iValue= 0;
	msg.ctrl_addr= 0;
	GetSrcId((UCHAR	*)&msg.host_id);
	msg.usr_id= -1;
	msg.second= date_time.second;
	msg.minute= date_time.minute;
	msg.hour= date_time.hour;
	msg.day= date_time.day;
	msg.month= date_time.month;
	msg.year= date_time.year;
	strcpy(msg.usr_login_name, "SYSTEM");
	
	head.length= 32;
	head.dp_type= COMM_SIM;
	head.dest_id= 1;
	GetSrcId((UCHAR	*)&head.src_id);
	
#ifdef _BIG_ENDIAN_
	Swap2Byte(&head.length);
	Swap2Byte(&msg.point_id);
	Swap2Byte(&msg.status);
	Swap4Byte(&msg.data);
	Swap2Byte(&msg.ctrl_addr);
	Swap2Byte(&msg.year);
#endif
	lan_out ((char *)&msg,head,0);  
	return(0);
}


int GetYx( char yx_LogiName[][55] )
{	
	char FileName[100]="/home/ems/h9000/his/trap/yx_table.txt";
	FILE *fp;
	char StrTmp[100];
	int i,yxNum;
	yxNum = 0;
	for(i=0;i<MAX_YX_NUM;i++) strcpy(yx_LogiName[i],"");
	if (( fp = fopen ( FileName,"r"))==NULL)
	{
		printf ("==== can't open (w) the file %s! \n",FileName );
		return 0;
	}
	while ( fscanf(fp,"%s",StrTmp)!=EOF )
	{
		fscanf(fp,"%s",StrTmp);
		strcpy(yx_LogiName[yxNum++],StrTmp);
		fscanf(fp,"%s",StrTmp);	
	}
	return yxNum;
}

int GetYc( char yc_LogiName[][55] )
{	
	char FileName[100]="/home/ems/h9000/his/trap/yc_table.txt";
	FILE *fp;
	char StrTmp[100];
	int i,ycNum;
	ycNum = 0;
	for(i=0;i<MAX_YC_NUM;i++) strcpy(yc_LogiName[i],"");
	if (( fp = fopen ( FileName,"r"))==NULL)
	{
		printf ("==== can't open (w) the file %s! \n",FileName );
		return 0;
	}
	while ( fscanf(fp,"%s",StrTmp)!=EOF )
	{
		fscanf(fp,"%s",StrTmp);
		strcpy(yc_LogiName[ycNum++],StrTmp);
		fscanf(fp,"%s",StrTmp);	
	}
	return ycNum;
}