#include "\home\ems\system\lan\v4.0a\inc\lan.h"
#include "../../../plc/v4.0a/inc/plc_com.h"
#include "../inc/mbc_com.h"


int DBread_by_name( char *name,  DB_DATA *db)
{ 
	DMS_COMMON      rs;
	int             datatype;  
	if(ReadEntryByNameStr( name, &rs) ==-1) return FALSE;
	datatype = rs.point.data_type; 
	db->dev_id    = rs.point.dev_id;
	db->data_type = rs.point.data_type;
	db->point_id  = rs.point.pt_id;
	db->ioattr	 = rs.fixed.ioattr;
	
	if((datatype == SOE_TYPE)||(datatype == POL_TYPE)||(datatype == SYSMSG_TYPE))
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
	else if((datatype == ANA_TYPE)||(datatype == PARAM_TYPE))
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
		db->LongValue = rs.var.imp.entry.rt_value;
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


unsigned short	GetCharBits (unsigned short v,unsigned short start_bit,unsigned short bit_num )
{
	unsigned short	tmp;
	unsigned short	tmp2	=0;
	
	tmp	= ( v >> start_bit ) & ( ~( (~tmp2) << bit_num ) );
	
	return	tmp;
}



int CloseSock(SOCKET *p_sockplc)
{
	struct linger linger;
	int   ret;
	char  addreuse=1;
#ifdef MSWIN32
	unsigned int block_flag = FALSE;
#endif
	
	if(*p_sockplc <=0) return FALSE;
	linger.l_onoff = 1;
	linger.l_linger = 0;
	setsockopt(*p_sockplc, SOL_SOCKET, SO_LINGER, (char *)&linger,sizeof(linger));
    setsockopt(*p_sockplc,SOL_SOCKET,SO_REUSEADDR,&addreuse,sizeof(addreuse));
#ifdef MSWIN32
	ioctlsocket(*p_sockplc,FIONBIO,&block_flag);
#else
	fcntl(*p_sockplc,F_SETFL,0);
#endif
	ret= closesocket(*p_sockplc);
	*p_sockplc=-1;
	if(ret==0) return TRUE;
	return FALSE;
}

void	PrintCommData(DWORD nbyte, char *buf )
{
	unsigned short d;
	
	for(d=1;d<=nbyte;d++)
	{
		printf("=%2x",buf[d-1]);
		if((d%20)==0)
			printf("\n");
	}
	printf("\n");
}



int Swap4Str(unsigned char *buf,int wordlen)
{
	int i=0;
	char tmp;
	while(i < wordlen*4 )
	{
		tmp=buf[i];
		buf[i] = buf[i+3];
		buf[i+3] = tmp;
		tmp=buf[i+1];
		buf[i+1] = buf[i+2];
		buf[i+2] = tmp;
		i=i+4;
	}
	return TRUE;
}