#include	<string.h>
#include	<math.h>
#include	<stdarg.h>

#include	"../inc/os_type_def.h"
#include	"../inc/dms_com.h"

#ifdef SUN_OS
#include	<pthread.h>
#endif


BOOL IsIndTypePoint(POINTER *pt)
{
	if(pt->data_type==SOE_TYPE || pt->data_type==POL_TYPE
		|| pt->data_type==SYSMSG_TYPE || pt->data_type==OUTS_TYPE)
		return TRUE;
	else
		return FALSE;
}


BOOL IsAnaTypePoint(POINTER *pt)
{
	if(pt->data_type==ANA_TYPE || pt->data_type==OUTA_TYPE
		|| pt->data_type==PARAM_TYPE || pt->data_type==CALC_TYPE)
		return TRUE;
	else
		return FALSE;
}


float AnlgDatatoFloat ( ANLG_DATA *anlg_data )
{
	double	value;

	value= (double)anlg_data->data;
	value= value*0.001;

	return (float)value;
}


void FloattoAnlgData( float value, ANLG_DATA *anlg_data )
{
    anlg_data->data = (GULONG)(value*1000.0);
}




BOOL IsPrimeNumber(int num)
{
	int	i, limit;
	
	if(num<=1)
		return FALSE;
	if(num==2 || num==3)
		return TRUE;
	if(!(num%2))
		return FALSE;

/*	limit= (int)sqrt(num)+1;*/
	limit= num/2+1;
	for(i=3; i<=limit; i=i+2)
	{
		if(!(num%i))
			return FALSE;
	}
	return TRUE;
}


int	FindLastPrimeNumber(int num)
{
	int		i;

	if(num<3)
		return -1;
	for(i=num; i>2; i--)
	{
		if(IsPrimeNumber(i))
			return i;
	}
	return -1;
}


void GetNameStrHash(char *namestr, GULONG *hash)
{
	int	i=0;

	*hash= 0;
	while(namestr[i])
	{
		*hash += namestr[i]<<((i%4)*8);
		i++;
	}
}



int	PointNametoStr(POINT_NAME *point_name, char *str)
{
	if( !point_name->stnName[0] )
		return -1;
	if( !point_name->grpName[0] )
		return -1;
	if( !point_name->recName[0] )
		return -1;
	if( !point_name->entryName[0] )
		return -1;
	sprintf(str, "%s.%s.%s.%s", point_name->stnName, point_name->grpName,	
		point_name->recName, point_name->entryName);	

	return 0;
}



int	StrtoPointName(char *str, POINT_NAME *point_name)
{
	int		i, j, len;
	char	*ptr[4];
	int		maxlen[4];
    char	err_str[128];
	char	*err[]= {"station", "group", "record", "entry"};

	if(!str[0])
	{
		sprintf(err_str, "point name empty: %s", str);
		Err_log(err_str);
		return -1;
	}

	ptr[0]= point_name->stnName;
	maxlen[0]= STATION_NAME_SIZE;
	ptr[1]= point_name->grpName;
	maxlen[1]= GROUP_NAME_SIZE;
	ptr[2]= point_name->recName;
	maxlen[2]= RECORD_NAME_SIZE;
	ptr[3]= point_name->entryName;
	maxlen[3]= ENTRY_NAME_SIZE;

	i= 0;
	j= 0;
	len= 0;
	while(str[i])
	{
		if(str[i]=='.')
		{
			if(!len)
			{
  				sprintf(err_str, "%s name empty in point name: %s", err[j], str);
  				Err_log(err_str);
  				return -1;
			}
			ptr[j][len]= 0;
			j++;
			if(j>=4)
			{
  				sprintf(err_str, "point name error: %s", str);
  				Err_log(err_str);
  				return -1;
			}
			len= 0;
		}
		else
		{
			ptr[j][len++]= str[i];
			if(len>=maxlen[j])
			{
  				sprintf(err_str, "%s name too long in point name: %s", err[j], str);
  				Err_log(err_str);
  				return -1;
			}
		}
		i++;
	}
	if(!len)
	{
		sprintf(err_str, "%s name empty in point name: %s", err[j], str);
		Err_log(err_str);
		return -1;
	}
	if(j!=3)
	{
		sprintf(err_str, "point name error: %s", str);
		Err_log(err_str);
		return -1;
	}
	ptr[j][len]= 0;
	return 0;	
}



int	GetPtIdFromStr(char *str, POINTER *point)
{
	POINT_NAME  point_name;
	int			stn_id, dev_id, data_type, pt_id;
	
	if(-1==StrtoPointName(str, &point_name))
	{
		printf("\nInvalid point name");
		return -1;
	}

	if(point_name.recName[0]>='0' && point_name.recName[0]<='9')
	{
		sscanf(point_name.stnName, "%d", &stn_id);
		sscanf(point_name.grpName, "%d", &dev_id);
		sscanf(point_name.recName, "%d", &data_type);
		sscanf(point_name.entryName, "%d", &pt_id);
		point->stn_id= (UCHAR)stn_id;
		point->dev_id= (UCHAR)dev_id;
		point->data_type= (UCHAR)data_type;
		point->pt_id= (USHORT)pt_id;
		return 0;
	}
	else
		return GetPtIdByNameStr(str, point);
}



void CrossCopy( void *strDest, void *strSrc, int nNum)
{
	int n;
	
#ifdef	_BIG_ENDIAN_
    for (n=0;n<nNum;n++)
		*((char *)strDest +n)=*((char*)strSrc+nNum-1-n);
#else
    for (n=0;n<nNum;n++)
		*((char *)strDest +n)=*((char*)strSrc+n);
#endif

}



#ifdef	_BIG_ENDIAN_
void Swap2Byte(void *pShort)
{
	char tmp; 

	tmp =  *((char *)pShort); 
	*((char *)pShort)=*((char*)pShort+1); 
	*((char*)pShort+1) = tmp; 
}


void Swap4Byte(void *pInt)
{
	char tmp;
	
	tmp =  *((char *)pInt); 
	*((char *)pInt)=*((char*)pInt+3); 
	*((char*)pInt+3) = tmp; 
	tmp =  *((char *)pInt+1); 
	*((char *)pInt+1)=*((char*)pInt+2); 
	*((char*)pInt+2) = tmp; 
}




void SwapImpDataByte( IMP_DATA *pImp)
{
	Swap4Byte(&pImp->day);
	Swap4Byte(&pImp->month);
	Swap4Byte(&pImp->year);
}


void SwapImpEngParaByte( IMP_ENG_PARA *eng)
{
	Swap4Byte(&eng->ENG_CVT);
	Swap4Byte(&eng->CONSTANT);
	Swap4Byte(&eng->DAY_RATE);
	Swap4Byte(&eng->YEAR_RATE);
	Swap4Byte(&eng->PT);
	Swap4Byte(&eng->CT);
}



void SwapAnaEngParaByte( ANA_ENG_PARA *eng)
{
	Swap4Byte(&eng->D_BAND);
	Swap4Byte(&eng->ENG_CVT);
	Swap4Byte(&eng->HI_RANGE);
	Swap4Byte(&eng->LO_RANGE);
	Swap4Byte(&eng->ZERO);
	Swap4Byte(&eng->COMPSATE);
	Swap4Byte(&eng->HI_CODE);
	Swap4Byte(&eng->LO_CODE);
}



void SwapLmtParaByte( LMT_PARA *lmt)
{
	Swap4Byte(&lmt->HI_ALARM);
	Swap4Byte(&lmt->HI_WARN);
	Swap4Byte(&lmt->HI_RECOVER);
	Swap4Byte(&lmt->LO_ALARM);
	Swap4Byte(&lmt->LO_WARN);
	Swap4Byte(&lmt->LO_RECOVER);
	Swap4Byte(&lmt->TR_WARN);
	Swap4Byte(&lmt->TR_RECOVER);
}


void SwapCharaDataByte( CHARA_DATA *chara)
{
	Swap2Byte(&chara->max_no);
	Swap2Byte(&chara->min_no);
	Swap2Byte(&chara->max_rate_no);
	Swap2Byte(&chara->min_rate_no);
	Swap4Byte(&chara->max_value);
	Swap4Byte(&chara->min_value);
	Swap4Byte(&chara->avg_value);
	Swap4Byte(&chara->std_value);
	Swap4Byte(&chara->max_rate);
	Swap4Byte(&chara->min_rate);
}


void SwapCurveDataByte( CURVE_DATA *data)
{
	int		i;

	for(i=0;i<MAX_CURVE_SEG_NUM;i++)
		Swap4Byte(&data->value[i]);
}


void SwapStcDataByte( STC_DATA *stc)
{
	Swap2Byte(&stc->norm_open);
	Swap2Byte(&stc->norm_close);
	Swap2Byte(&stc->fault_open);
	Swap2Byte(&stc->fault_close);
}


void SwapDateTimeByte( DATE_TIME *date_time)
{
	Swap2Byte(&date_time->ms);
	Swap2Byte(&date_time->year);
}
#endif /*_BIG_ENDIAN_*/



void AsciiToHex(char *asc, char *bin, int size)
{
	int	i, len= size*2;
	
	for(i=0; i<len; i=i+2)
	{
		char	value, value1;
		
		if(asc[i]>='0' && asc[i]<='9')
			value= asc[i]-'0';
		else if(asc[i]>='a' && asc[i]<='f')
			value= asc[i]-'a'+10;
		else if(asc[i]>='A' && asc[i]<='F')
			value= asc[i]-'A'+10;
		else
			value= 0;
		
		if(asc[i+1]>='0' && asc[i+1]<='9')
			value1= asc[i+1]-'0';
		else if(asc[i+1]>='a' && asc[i+1]<='f')
			value1= asc[i+1]-'a'+10;
		else if(asc[i+1]>='A' && asc[i+1]<='F')
			value1= asc[i+1]-'A'+10;
		else
			value1= 0;
		
		bin[i/2]= (value<<4) | value1;
	}
}


void GetSysTime( DATE_TIME *date_time)
{
#ifdef	WINDOWS_OS
	SYSTEMTIME	sysTime;
	GetLocalTime(&sysTime);	
	date_time->year= (USHORT)sysTime.wYear;
	date_time->month= (UCHAR)sysTime.wMonth;
	date_time->day= (UCHAR)sysTime.wDay;
	date_time->hour= (UCHAR)sysTime.wHour;
	date_time->minute= (UCHAR)sysTime.wMinute;
	date_time->second= (UCHAR)sysTime.wSecond;
	date_time->ms= (USHORT)sysTime.wMilliseconds;
#endif

#ifdef	SUN_OS
	struct	timeval	time_val;
	struct tm	tm_time;
	gettimeofday(&time_val, NULL);
	tm_time= *localtime(&time_val.tv_sec);
	date_time->year= tm_time.tm_year+1900;
	date_time->month=tm_time.tm_mon+1;
	date_time->day= tm_time.tm_mday;
	date_time->hour= tm_time.tm_hour;
	date_time->minute= tm_time.tm_min;
	date_time->second= tm_time.tm_sec;
	date_time->ms= time_val.tv_usec/1000;
#endif
}

void GetSysTimeEx( DATE_TIME *date_time, time_t *sec)
{
#ifdef	WINDOWS_OS
	SYSTEMTIME	sysTime;

	GetLocalTime(&sysTime);	
	if(date_time)
	{
		date_time->year= (USHORT)sysTime.wYear;
		date_time->month= (UCHAR)sysTime.wMonth;
		date_time->day= (UCHAR)sysTime.wDay;
		date_time->hour= (UCHAR)sysTime.wHour;
		date_time->minute= (UCHAR)sysTime.wMinute;
		date_time->second= (UCHAR)sysTime.wSecond;
		date_time->ms= (USHORT)sysTime.wMilliseconds;
	}
	if(sec)
	{
		struct tm	tm_time;
		tm_time.tm_sec = sysTime.wSecond;
		tm_time.tm_min = sysTime.wMinute;
		tm_time.tm_hour = sysTime.wHour;
		tm_time.tm_mday = sysTime.wDay;
		tm_time.tm_mon = sysTime.wMonth - 1;
		tm_time.tm_year =  sysTime.wYear - 1900;
		tm_time.tm_isdst = 0;
		*sec= mktime(&tm_time);
	}
#endif

#ifdef	SUN_OS
	struct	timeval	time_val;

	gettimeofday(&time_val, NULL);
	if(date_time)
	{
		struct tm	tm_time;
		tm_time= *localtime(&time_val.tv_sec);
		date_time->year= tm_time.tm_year+1900;
		date_time->month=tm_time.tm_mon+1;
		date_time->day= tm_time.tm_mday;
		date_time->hour= tm_time.tm_hour;
		date_time->minute= tm_time.tm_min;
		date_time->second= tm_time.tm_sec;
		date_time->ms= time_val.tv_usec/1000;
	}
	if(sec)
		*sec= time_val.tv_sec;
#endif



}

void GetSysTimeStr( char *timeStr)
{
	DATE_TIME	date_time;

	GetSysTime(&date_time);
	sprintf(timeStr, "%04d-%02d-%02d %02d:%02d:%02d.%03d", date_time.year,
		date_time.month, date_time.day, date_time.hour,
		date_time.minute, date_time.second, date_time.ms);
}

void GetSysShortTimeStr( char *timeStr)
{
	DATE_TIME	date_time;
	
	GetSysTime(&date_time);
	sprintf(timeStr, "%02d:%02d:%02d.%03d", date_time.hour,
		date_time.minute, date_time.second, date_time.ms);
}


void PrintfWithCurrTime(char *str)
{
	char	timestr[256];
	GetSysTimeStr(timestr);
	printf("\n%s: %s", timestr, str);
}

void PrintfWithCurrShortTime(char *str)
{
	char	timestr[256];
	GetSysShortTimeStr(timestr);
	printf("\n%s: %s", timestr, str);
}




#define IS_QUOTE(c)	(((c)=='\'')||((c)=='\"'))

int	GetStringsFromStr(char *bufptr, int strbuf_num, char strbuf[][MAX_CHAR_NUM_IN_LINE])
{
	int		i, substr_ptr, num, quote_field_start, new_substr_start;
	char	buffer[MAX_CHAR_NUM_IN_LINE+1];
	
	for(i=0; i<strbuf_num; i++)
		strbuf[i][0]= 0;
	
	if(bufptr[0]==0 || bufptr[0]==0x0d || bufptr[0]==0x0a) /*空行、换行符、回车符*/
		return 0;
	
	if(bufptr[0]=='#' || bufptr[0]=='!') /*#和!表示注释*/
		return 0;
	
	if(strlen(bufptr) >= sizeof(buffer))
	{
		strncpy(buffer, bufptr, sizeof(buffer)-1);
		buffer[sizeof(buffer)-1]= 0;
	}
	else
		strcpy(buffer, bufptr);
	

	i= 0;
	while(buffer[i])
	{
		if ( ( buffer[i] == '=') || ( buffer[i] == ',') || ( buffer[i] == '\t'))
			buffer[i] = ' ';
		
		if ( (buffer[i] == 0x0d) || (buffer[i] == 0x0a) ) /*去掉行末的回车符和换行符*/
		{
			buffer[i] = 0;
			break;
		}
		i++;
	}
	
	i= 0;
	substr_ptr= 0;
	num= 0;
	quote_field_start= 0;
	new_substr_start= 0;
	while(buffer[i])
	{
		if(IS_QUOTE(buffer[i])) /*引号*/
		{
			if(quote_field_start) /*后引号*/
			{
				if(buffer[i+1]==' ') /*后引号后面有空格*/
				{
					quote_field_start= 0; /*引号域结束*/
					strbuf[num][substr_ptr]= 0; /*结束当前的子串*/
					new_substr_start= 0;
					num++;
					if (num>=strbuf_num) /*以前错误使用MAX_STR_NUM_IN_LINE*/
						return num; 
					substr_ptr= 0;
					i += 2;
					continue;
				}
				if(buffer[i+1]==0) /*后引号后面字符串结束*/
				{
					strbuf[num][substr_ptr]= 0; /*结束当前的子串*/
					return num+1; 
				}
			}
			else /*前引号*/
			{
				if(i==0 || buffer[i-1]==' ')/*前引号 前面必须有空格，除非字符串开始*/
				{
					quote_field_start= 1; /*引号域开始*/
					i++;
					continue;
				}
			}
		}
		
		if(quote_field_start) /*引号域内拷贝所有字符*/
		{
			strbuf[num][substr_ptr++] = buffer[i];
			i++;
			new_substr_start= 1;
			continue;
		}
		
		if(buffer[i] != ' ') /*拷贝非空格字符*/
		{
			strbuf[num][substr_ptr++] = buffer[i];
			new_substr_start= 1;
		}
		else if((i>0) && (buffer[i-1]!=' ') && (buffer[i]==' ')) /*空格前为非空格字符*/
		{   
			strbuf[num][substr_ptr]= 0; /*结束当前的子串*/
			new_substr_start= 0;
			num++;
			if (num>=strbuf_num) /*以前错误使用MAX_STR_NUM_IN_LINE*/
				return num; 
			substr_ptr= 0;
		}
		i++;
	} /*while*/

	if(new_substr_start)
		strbuf[num][substr_ptr]= 0;	/*结束当前的子串*/
	return num+new_substr_start;
}


/*
int	GetStringsFromStr ( char *bufptr, int strbuf_num, char strbuf[][MAX_CHAR_NUM_IN_LINE])
{
	int		i, j, num, len, init, quote_mark;
	char	buffer[MAX_CHAR_NUM_IN_LINE];
	
	for(i=0; i<strbuf_num; i++)
		strbuf[i][0]= 0;
	
	if((bufptr[0]=='#') || (bufptr[0]=='!')) //#和!表示注释
		return 0;
	
	if(strlen(bufptr) >= sizeof(buffer))
	{
		strncpy(buffer, bufptr, sizeof(buffer)-1);
		buffer[sizeof(buffer)]= 0;
	}
	else
		strcpy(buffer, bufptr);
	
	len= strlen(buffer);
	
	for(j=0; j<len; j++)
	{
		if ( ( buffer[j] == '=') || ( buffer[j] == ',') 
			|| ( buffer[j] == '\t') || ( buffer[j] == ':') )
			buffer[j] = ' ';
		
		if ( (buffer[j] == 0x0d) || (buffer[j] == 0x0a) ) //去掉行末的回车符和换行符
		{
			buffer[j] = 0;
			break;
		}
	}
	
	if( buffer[0]==0 ) //空字符串
		return 0;
	
	j= 0;
	num= 0;
	init= 0;
	quote_mark= 0;
	for(i=0; i<len; i++ )
	{
		if((buffer[i]=='\"') || (buffer[i]=='\'')) //单引号和双引号
		{
			if(quote_mark) //后引号
			{
				if((buffer[i+1]!=' ') && (buffer[i+1]!=0)) //后引号后面必须有空格，除非字符串结束
				{
					strbuf[num][j]= 0;
					return -1;
				}
				quote_mark= 0; //引号域结束
			}
			else //前引号
			{
				if((i>0) && buffer[i-1]!=' ') //前引号前面必须有空格，除非字符串开始
				{
					strbuf[num][j]= 0;
					return -1;
				}
				quote_mark= 1; //引号域开始
			}
			continue;
		}
		
		if(quote_mark) //引号域内拷贝所有字符
		{
			strbuf[num][j++] = buffer[i];
			init= 1;
			continue;
		}
		
		if(buffer[i] != ' ') //拷贝非空格字符
		{
			strbuf[num][j++] = buffer[i];
			init= 1;
		}
		else if((buffer[i]==' ') && (buffer[i+1]!=' ') && (buffer[i+1]!=0) && init)
		{//空格后为非空格字符，且字符串未结束，且已经提取过非空格字符
			strbuf[num][j]= 0; //结束当前的字符串，开始新的字符串
			j= 0;
			num++;
			if (num>=str_num)
				return num;
		}
	}
	strbuf[num][j]= 0;	//结束当前的字符串
	if(quote_mark) //如果引号域没有闭合，则格式错误
		return -1;
	return num+1;
}
*/


