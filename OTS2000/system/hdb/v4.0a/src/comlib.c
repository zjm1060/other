/************************************************************************************************************************
FILENAME:	comlib.c

FUNCTION:	
	
	some functions of hdb library,include:

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	start to update the file from H9000 V3.0 to V4.0		yyp
2011.4.28    add "break;" in func of dateadd and dateminus in "case hour:"

*************************************************************************************************************************/

#include "../inc/hdbhead_all.h"

//extern char log_str[256];

int GetPtConf()
{
	unsigned char stnid,devid,type;
	int n,m,ptnum;
	n=0;
	for(stnid=1;stnid<=g_stn_num;stnid++)
	{
		for(devid=1;devid<=g_dev_num[stnid];devid++)
		{
			dev_pos[stnid][devid]=n;
			dev_id[n].stnid=stnid;
			dev_id[n].devid=devid;
			n++;
			m=0;
			for(type=1;type<=MAX_DATATYPE_NUM;type++)
			{
				pt_pos[stnid][devid][type] = m;
				ptnum=0;
				if(GetEntryNumById( stnid, devid, type, &ptnum)!=-1)
				{
					m+=ptnum;	
				}
			}
		}
	}
	hdb_dev_num = n;
//	sprintf(log_str,"\n==== GetPtConf(): total dev num =%d\n",hdb_dev_num);
//	log_write(INF,1,log_str);
	return TRUE;
}

int GetPtIdByName(unsigned char stnid, unsigned char devid, char * name,short * pt_id)
{
	POINTER	pt;
	char	buf[50];
	char	stn_name[STATION_NAME_SIZE];
	char	dev_name[GROUP_NAME_SIZE];
	if(GetStnNameById(stnid,stn_name)==-1) return(0);
	if(GetGroupNameById(stnid,devid,dev_name)==-1) return(0);
	strcpy(buf,stn_name);
	strcat(buf,".");
	strcat(buf,dev_name);
	strcat(buf,".ANA.");
	strcat(buf,name);
	if(GetPtIdByNameStr( buf, &pt )==-1) return(0);
	*pt_id = pt.pt_id;
	return TRUE;
}

void ReadDefFile()
{
        ReadAppDefFile();
        strcpy(SIDSTR,g_sid);
}

void BlankFillCopy ( char *dst, char *src,int len)
{
	while ( *src && len-- > 1 )
		*(dst++) = *(src++);

	while (len-- > 1)
		*(dst++) = ' ';
	*dst = '\0';
}

void DellBlank (char *dst, char *src, int len)
{
	while ( *src && len-- > 1 )
	{
		if( *src != ' ' ) { *(dst++) = *src; }
			
		src++;
	}
	*dst = '\0';
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


int GetLocalStat( char *name )
{	
	unsigned short status;
	if(GetHostState(name, &status) == -1 )
	{
		return( CUR_WS_IN_VICE ); 
	}
	if(status != 1) status =  CUR_WS_IN_VICE;
	return(status);
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

int IsPtIntoHdb(POINTER * p_point)
{

	/* modified by yyp 08.01.19*/
	/*
 	int	ishdb;	
	if(IsPtHdbRecordById( p_point, &ishdb ) ==  -1)
	{
		return FALSE;
	}
	else
	{
		if(ishdb != TRUE)	
		{
			return FALSE;
		}	
	}
	*/
	return TRUE;
}

int IsPtIntoCurveHdb(POINTER * p_point)
{
 	/* modified by yyp 11.01.10 */
	
	int	ishdb;	

	if(IsPtHdbRecordById( p_point, &ishdb ) ==  -1)
	{
		return FALSE;
	}
	else
	{
		if(ishdb != TRUE)	
		{
			return FALSE;
		}	
	}
	return TRUE;
}

/*************************************** get the time added interval from curtime **************************************/
int DateAdd(struct tm *p_datetime,unsigned char interval)
{
	time_t  res;
	switch (interval)
	{
	case ONE_SEC:
		p_datetime->tm_sec=p_datetime->tm_sec+1;		
		break;
	case ONE_MIN:
		p_datetime->tm_min=p_datetime->tm_min+1;
		break;
	case FIVE_MIN:
		p_datetime->tm_min=p_datetime->tm_min+5;
		break;
	case FIFTEEN_MIN:
		p_datetime->tm_min=p_datetime->tm_min+15;
		break;
	case ONE_HOUR:
		p_datetime->tm_hour=p_datetime->tm_hour+1;
		break;
	case ONE_DAY:
		p_datetime->tm_mday=p_datetime->tm_mday+1;
		break;
	default:
		return(FALSE) ;
		break;
	}	
	p_datetime->tm_isdst =-1;
	res=mktime( p_datetime );
	return TRUE;
}

/*************************************** get the time added interval from curtime **************************************/
struct tm DateAddSec(struct tm datetime,int num)
{
	time_t  res;

	datetime.tm_sec=datetime.tm_sec+num;		
	datetime.tm_isdst =-1;
	res=mktime( &datetime );

	return datetime;
}

int DateMinus(struct tm *p_datetime,unsigned char interval)
{
	time_t  res;
	switch (interval)
	{
	case ONE_SEC:
		p_datetime->tm_sec=p_datetime->tm_sec-1;		
		break;
	case ONE_MIN:
		p_datetime->tm_min=p_datetime->tm_min-1;
		break;
	case FIVE_MIN:
		p_datetime->tm_min=p_datetime->tm_min-5;
		break;
	case FIFTEEN_MIN:
		p_datetime->tm_min=p_datetime->tm_min-15;
		break;
	case ONE_HOUR:
		p_datetime->tm_hour=p_datetime->tm_hour-1;
		break;
	case ONE_DAY:
		p_datetime->tm_mday=p_datetime->tm_mday-1;
		break;
	default:
		return(FALSE) ;
		break;
	}	
	p_datetime->tm_isdst =-1;
	res=mktime( p_datetime );
	return TRUE;
}

/*************************************** convert char string to datetime***** **************************************/
int StrToTime(char *timestr,struct tm *p_datetime,unsigned char interval,unsigned char flag)
{
	int year,mon,day,hour,min,sec;
	sscanf(timestr,"%d-%d-%d %d:%d:%d",&year,&mon,&day,
	 				&hour,&min,&sec);
	if(flag==1)
	{
	switch (interval)
	{
	case ONE_SEC:
		break;
	case ONE_MIN:
		sec=0;
		break;
	case FIVE_MIN:
		sec=0;
		min= ((int)(min/5))*5 ;		
		break;
	case FIFTEEN_MIN:
		sec=0;
		min= ((int)(min/15))*15 ;
		break;
	case ONE_HOUR:
		sec=0;
		min=0;
		break;
	default:
		return(FALSE) ;
		break;
	}
	} /*end if*/
	 p_datetime->tm_year=year-1900;
	 p_datetime->tm_mon=mon-1;

	 p_datetime->tm_mday=day;
	 p_datetime->tm_hour=hour;
	 p_datetime->tm_min=min;
	 p_datetime->tm_sec=sec;
	 p_datetime->tm_isdst =-1;
	 mktime( p_datetime );
	 return(TRUE);	
}

/*************************************** convert struct tm variable to char [22] ***** ************************************/
int tmtostring(struct tm dtime, char *str)
{
	sprintf(str,"%4d-%02d-%02d %02d:%02d:%02d",dtime.tm_year+1900,dtime.tm_mon+1,  
		dtime.tm_mday,dtime.tm_hour,dtime.tm_min,dtime.tm_sec);
	return TRUE;
}

char ascstr[30];
char *myasctime(struct tm dtime)
{
	sprintf(ascstr,"%4d-%02d-%02d %02d:%02d:%02d",dtime.tm_year+1900,dtime.tm_mon+1,
 
		dtime.tm_mday,dtime.tm_hour,dtime.tm_min,dtime.tm_sec);
	return ascstr;		
}

void sig_termp()
{
	printf("\n==== Get signal, exit ====\n");
	exit(0);
}

unsigned short	GetCharBits (unsigned short v,unsigned short start_bit,unsigned short bit_num )
{
	unsigned short	tmp;
	unsigned short	tmp2	=0;

	tmp	= ( v >> start_bit ) & ( ~( (~tmp2) << bit_num ) );

	return	tmp;

}

int GetHdbIDByName(unsigned int *p_id,char * logicalname)
{
	POINTER point;	
	int nDev,nPt;
	*p_id = -1;
	if(GetPtIdByNameStr( logicalname, &point)==-1)
	{
		return FALSE;
	}
	nDev=dev_pos[point.stn_id][point.dev_id];
	nPt = pt_pos[point.stn_id][point.dev_id][point.data_type] + point.pt_id;
	*p_id = hdb_pt_id[nDev][nPt].id;
	
	if(*p_id == -1)
	{
		printf("logicalname=%s,%d,%d,%d,%d not found \n",logicalname,point.stn_id,point.dev_id,point.data_type,point.pt_id);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


int GetHdbCondByName(unsigned int *p_cond,char * logicalname)
{
	POINTER point;
	int nDev,nPt;	
	*p_cond = TRUE;
	if(GetPtIdByNameStr( logicalname, &point)==-1)
	{
		return FALSE;
	} 
	nDev=dev_pos[point.stn_id][point.dev_id];
	nPt = pt_pos[point.stn_id][point.dev_id][point.data_type] + point.pt_id;

	*p_cond = hdb_pt_id[nDev][nPt].iscond;
	
	if(*p_cond == -1)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

int GetPtHdbOffsetByName( char *logicalname, float *p_offset) 
{
	POINTER point;	
	int nDev,nPt;	
	*p_offset = -1;
	if(GetPtIdByNameStr( logicalname, &point)==-1)
	{
		return FALSE;
	} 
	nDev=dev_pos[point.stn_id][point.dev_id];
	nPt = pt_pos[point.stn_id][point.dev_id][point.data_type] + point.pt_id;
	*p_offset = hdb_pt_id[nDev][nPt].offset;
	
	if(*p_offset == -1)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
	return TRUE;
}


/********************************************************************************************************
 Get Dev Condition from  Hdb
*********************************************************************************************************/
void GetDevCond()
{
  	DB_DATA		db_data;
	IND_DATA	status;
	unsigned char StnId,DevId;
	for(StnId=1;StnId<MAX_STN_NUM+1;StnId++)
	{
		for(DevId=1;DevId<MAX_LCU_NUM+1;DevId++)
		{
			cond[StnId][DevId].status = 0;
			if(cond[StnId][DevId].id != -1)
			{
				if( ReadDBByName( cond[StnId][DevId].logicalname, &db_data )!=TRUE )
				{
					printf("No %s in RDB\n",cond[StnId][DevId].logicalname);	
					cond[StnId][DevId].status = 0;
					continue;
				}
				else
				{
					/* needed to be modified yyp  */
					memcpy(&status,&(db_data.status),2);
					cond[StnId][DevId].status= status.state;
				}
			}

		}
	}

}
