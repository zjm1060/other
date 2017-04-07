/*******************************************************************************************************************************
FILENAME:	
FUNCTION:		

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, XP

HISTORY:
2008.4.10	start to modify the code for H9000 V4.0		zyf
********************************************************************************************************************************/
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include "\home\ems\system\iec103\v4.0a\inc\iec103_com.h"

extern unsigned char anainf[STN_NUM];
extern unsigned char dioinf[STN_NUM];
extern unsigned char dinf[STN_NUM][MAX_YX_NUM];
extern unsigned char dfun[STN_NUM][MAX_YX_NUM];
extern unsigned char ainf[STN_NUM][MAX_YC_NUM];


int		YcNum,	YxNum;

int ReadDefineFile()
{	int		i,	j,	id,	InfoID, FunID;
	float	PntNum;
	FILE	*fp;
	char	c_name[200],	l_name[50];
	DMS_COMMON common_aa; 
	//DB_DATA db_data; 

	for(i=0;i<STN_NUM;i++)
	{		dioinf[i] = 0;	anainf[i] = 0;		}

	for(j=0;j<STN_NUM;j++)
	{
		for(i=0;i<MAX_YC_NUM;i++)
		{		strcpy(rtu[j].yc[i].c_name,"");	strcpy(rtu[j].yc[i].l_name,"");	rtu[j].yc[i].range=0;	rtu[j].yc[i].PntNum=1.0;	}	
	
		for(i=0;i<MAX_YX_NUM;i++)
		{		strcpy(rtu[j].yx[i].c_name,"");	strcpy(rtu[j].yx[i].l_name,"");	}
	}

	for(j=0;j<STN_NUM;j++)
	{

	/*---- read yc define file ----*/
	if( strcmp(yc_file[j],"")==0 )continue;
	while((fp=fopen(yc_file[j],"r"))==NULL)
	{ printf("iec103.EXE: %s can not open\n",yc_file[j]); Sleep(10*1000 ); }
	
	while( fscanf(fp,"%s",c_name)!=EOF )
	{	if( strstr(c_name,"*/")!=NULL ) break;	}
	
	for(id=0;;id++)
	{
		if( fscanf(fp,"%d %s %s %d %f",&i,c_name,l_name,&InfoID,&PntNum)==EOF)
		{ YcNum = id;
		  anainf[j] = id;
		  printf("READ %s OK! (YcNum=%d)\n",yc_file[j],YcNum); 
		  fclose( fp ); break; 
		}

		strcpy( rtu[j].yc[id].c_name, c_name ); strcpy( rtu[j].yc[id].l_name, l_name ); 
		rtu[j].yc[id].InfoID = InfoID;
		ainf[j][id]			 = InfoID;

		while( ReadEntryByNameStr( rtu[j].yc[id].l_name, &common_aa )==-1 )
		{	printf("No %s in RDB\n",rtu[j].yc[id].l_name); Sleep(5*1000); }

		rtu[j].yc[id].dev_id	=	common_aa.point.dev_id;
		rtu[j].yc[id].data_type=	common_aa.point.data_type;
		rtu[j].yc[id].point_id	=	common_aa.point.pt_id;
		rtu[j].yc[id].HI_limit	=	common_aa.var.anlg.eng_para.HI_CODE;
		rtu[j].yc[id].fValueOld =	common_aa.var.anlg.fValue;
		rtu[j].yc[id].PntNum	=	PntNum;
/*
		printf("%3d%20s %18s    %5.2f %5.2f  ( %d_%d_%d : %5.2f ) InfoID=%d PntNum=%.3f\n",
			    id,rtu[j].yc[id].c_name, rtu[j].yc[id].l_name, rtu[j].yc[id].range, rtu[j].yc[id].HI_limit,
			    rtu[j].yc[id].dev_id, rtu[j].yc[id].data_type, rtu[j].yc[id].point_id, rtu[j].yc[id].fValueOld, 
			    rtu[j].yc[id].InfoID, rtu[j].yc[id].PntNum
			  );
*/		
	}
	//for(id=0;id<STN_NUM;id++)
	//{	for(i=0;i<40;i++)printf("%d ",rtu[id].yc[i].InfoID); printf("\n"); }

	/*---- read yx define file ----*/
	if( strcmp(yx_file[j],"")==0 )continue;
	while((fp=fopen(yx_file[j],"r"))==NULL)
	{ printf("iec103.EXE: %s can not open\n",yx_file[j]); Sleep(10*1000 ); } 

	while( fscanf(fp,"%s",c_name)!=EOF )
	{	if( strstr(c_name,"*/")!=NULL ) break;	}

	for(id=0;;id++)
	{
		if( fscanf(fp,"%d %s %s %d %d",&id,c_name,l_name,&FunID,&InfoID)==EOF)
		{ YxNum     = id; 
		  dioinf[j] = id;
		  printf("READ %s OK! (YxNum=%d)\n",yx_file[j],YxNum); 
		  fclose( fp ); break; 
		}

		strcpy( rtu[j].yx[id].c_name, c_name ); strcpy( rtu[j].yx[id].l_name, l_name ); 
		rtu[j].yx[id].InfoID = InfoID;
		dinf[j][id]			 = InfoID;
		
		rtu[j].yx[id].FunID  = FunID;
		dfun[j][id]			 = FunID;

		while( ReadEntryByNameStr( rtu[j].yx[id].l_name, &common_aa )==-1 )
		{	printf("No %s in RDB\n",rtu[j].yx[id].l_name); Sleep(5*1000); }

		rtu[j].yx[id].dev_id	=	common_aa.point.dev_id;
		rtu[j].yx[id].data_type=	common_aa.point.data_type;
		rtu[j].yx[id].point_id	=	common_aa.point.pt_id;
		rtu[j].yx[id].StatusOld=	common_aa.var.ind.status.state;

		printf("%3d %20s %20s   ( %d_%d_%d : %d )  FunID=%3d InfoID=%d\n",
			    id,rtu[j].yx[id].c_name, rtu[j].yx[id].l_name,
			    rtu[j].yx[id].dev_id,    rtu[j].yx[id].data_type, 
				rtu[j].yx[id].point_id,  rtu[j].yx[id].StatusOld,
				rtu[j].yx[id].FunID,  	 rtu[j].yx[id].InfoID
			  );
			  
	}
	//for(id=0;id<STN_NUM;id++)
	//{	for(i=0;i<40;i++)printf("%d ",rtu[id].yx[i].InfoID); printf("\n"); }


	}//end for "for(j=0;j<STN_NUM;j++)"
	
	return( SUCCESS );
}



//=========================================================================//

void Broadcast_ana( int n, int id )				   
{ LAN_HEAD		  lanhead;
  ANA_MSG  		  msg;
  time_t  tim;
  struct tm   datetime;

	  /*********/
	msg.type_id =DPS_ANALOG;
    msg.stn_id =1;
    msg.dev_id =rtu[n].yc[id].dev_id;
    msg.data_type=rtu[n].yc[id].data_type;
    msg.number=1;
    msg.point_id = rtu[n].yc[id].point_id;

    msg.status=rtu[n].yc[id].status;
    memcpy(msg.data,&(rtu[n].yc[id].value),4);
    GetLocalHostId(&msg.host_id);

    tim=time (NULL);
    datetime=*localtime (&tim);
    msg.sec=datetime.tm_sec;
    msg.min=datetime.tm_min;
    msg.hour=datetime.tm_hour;
    msg.day=datetime.tm_mday;
    msg.month=datetime.tm_mon+1;
    msg.year=datetime.tm_year+1900;
    
    lanhead.length=ANAMSG_SIZE;
    
    lanhead.dp_type=BJ_DB_TIME;
    lanhead.dest_id =0;
    GetLocalHostId(&lanhead.src_id);
    lanhead.dest_stn =msg.stn_id;
    lanhead.packet_num =0; 

	  if( strcmp(rtu[n].yc[id].l_name,"")==0 )
	  {
		  printf("iec103.EXE:: yc point %d is not defined in %s!!!\n",id,yc_file[n]);
	  }
	  else
	  {
		  lan_out ((char *)&msg,lanhead,(unsigned char)0); 		  
	  }
	  
	  rtu[n].yc[id].fValueOld = rtu[n].yc[id].fValueNew;
	  
}

void Broadcast_sw( int n, int id, int soe_ms, int soe_sec, int soe_min )				   
{ LAN_HEAD		  lanhead;
  SYS_MSG  		  msg;
  struct tm       datetime;
  long   t;

	t=time(NULL); datetime=*localtime(&t);
	msg.type_id=DPS_IND_ALM;
    msg.stn_id =1;
    msg.dev_id =rtu[n].yx[id].dev_id;
    msg.data_type=rtu[n].yx[id].data_type;
    msg.number=1;
    msg.point_id = rtu[n].yx[id].point_id;
    msg.status=rtu[n].yx[id].StatusNew;
    GetLocalHostId(&msg.host_id);
	  if( soe_ms==-1)
	  { msg.ms      = 0;
	    msg.sec		= datetime.tm_sec;
	    msg.min		= datetime.tm_min;
	  }
	  else
	  { msg.ms      = soe_ms;
	    msg.sec		= soe_sec;
	    msg.min		= soe_min;
	  }
    msg.hour=datetime.tm_hour;
    msg.day=datetime.tm_mday;
    msg.month=datetime.tm_mon+1;
    msg.year=datetime.tm_year+1900;

    lanhead.length=SYSMSG_SIZE;
	lanhead.dp_type=BJ_DB_TIME;
    lanhead.dest_id =0;
    GetLocalHostId(&lanhead.src_id);
    lanhead.dest_stn =msg.stn_id;
    lanhead.packet_num =0;  

	  if( strcmp(rtu[n].yx[id].l_name,"")==0 )
	  {
		 printf("iec103.EXE:: yx point %d is not defined in %s !!!\n",id,yx_file[n]);	 
	  }
	  else
	  {
		lan_out ((char *)&msg,lanhead,(unsigned char)0);
	  }

	  rtu[n].yx[id].StatusOld = rtu[n].yx[id].StatusNew;
}


void ProcYx( int n, int id, int status, unsigned short ms, unsigned char sec, unsigned char min )
{
	rtu[n].yx[id].StatusNew=status+256; 
	if( rtu[n].yx[id].StatusOld != rtu[n].yx[id].StatusNew )
	{
		Broadcast_sw( n, id, ms, sec, min );
	}
}

void ProcYc( int n, int id, short value )
{
	DMS_COMMON db_data;
	rtu[n].yc[id].fValueNew = (float)value * rtu[n].yc[id].PntNum;  
	if( rtu[n].yc[id].fValueNew != rtu[n].yc[id].fValueOld )
	{
		rtu[n].yc[id].status = 0;
		rtu[n].yc[id].value  = rtu[n].yc[id].fValueNew*1000 ;
		Broadcast_ana( n, id );
	}
}


int GetStnId( unsigned char StnAddr )
{	int i;
	for(i=0;i<=STN_NUM;i++)
	{
		if( StnAddr==station_addr[i] ) return(i);
	}
	
	for(;;)
	{ 
		printf("iec103.exe: Can't find %x in the station_addr[] !!!\n");
		Sleep(3*1000);
	}	
}

unsigned char GetStnAddr( int StnId )
{	int i;
	i = station_addr[StnId];
	return( (unsigned char)i );
}