#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include    "../../../AIC/v4.0a/inc/AIC.h"
#include    "../../../cmd/v4.0a/inc/cmd_def.h"
#include    "../../../dms/v4.0a/inc/dms_std.h"
#include    "../../../dms/v4.0a/inc/dms_structure_def.h"
#include    "../../../dms/v4.0a/inc/os_type_def.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include    "../../../AIC/v4.0a/inc/macro.h"

void ReadTime ();

int Number_ANA=0;
ANA_MSG	ml_buf_ANA[64];
LAN_HEAD	lanhd_ANA,lanhd_IND;
int Number_IND=0;
SYS_MSG	ml_buf_IND[64];

int Move_Curve=1;

struct tm  *ntime;

char    hostname[20];

extern int BdSms(char* sms);
extern int LocateKeyword( FILE *, char *);
extern void CreateLockFile(char *);
extern int	InterLock_Judge (char *);

float MaxPower=500;

long int StringtoTime(char *str,struct tm start_time,struct tm end_time)
{
	time_t t_of_day;
	long int time;
//struct tm start_time,end_time;
int Start_month,Start_day, Start_hour,Start_min,End_month,End_day,End_hour,End_min;

ReadTime();

sscanf(str,"%d.%d %d:%d-%d.%d %d:%d",
	   &Start_month,&Start_day,&Start_hour,&Start_min,
	   &End_month,&End_day,&End_hour,&End_min);
start_time.tm_year = ntime->tm_year-1900;
start_time.tm_mon = Start_month;
start_time.tm_mday = Start_day;
start_time.tm_hour = Start_hour;
start_time.tm_min = Start_min;
start_time.tm_sec = 0;
start_time.tm_isdst = 0;

end_time.tm_year = ntime->tm_year-1900;
end_time.tm_mon = End_month;
end_time.tm_mday = End_day;
end_time.tm_hour = End_hour;
end_time.tm_min = End_min;
end_time.tm_sec = 0;
end_time.tm_isdst = 0;


t_of_day=mktime(&start_time);
////cout<<ctime(&t_of_day)<<endl;
//cout <<t_of_day<<endl;

time = t_of_day;
return time;

}



void UnMount_handle()
{
	UnMountDB();
    printf ("AIC exit!(Ctrl_c or kill -2 pid)\n");
    lan_finish();
    exit (0);
}

void SIGINT_handler (int signal_handler)
{
#ifdef DEBUG
	 printf ( "\nSIGVAL=%d", signal_handler);
#endif
	 lan_finish();
	 UnMountDB();
	 exit (0 );
}

int GetControlState()
{
    USHORT state;
    GetHostState(hostname,&state);
    return state;
}

int Compare_SYS_MSG(SYS_MSG Ind1,SYS_MSG Ind2)
{
    if((Ind1.type_id==Ind2.type_id)&&(Ind1.stn_id==Ind2.stn_id)
        &&(Ind1.dev_id==Ind2.dev_id)&&(Ind1.data_type==Ind2.data_type)
        &&(Ind1.number==Ind2.number)&&(Ind1.point_id==Ind2.point_id)
        &&(Ind1.status==Ind2.status)&&(Ind1.host_id==Ind2.host_id))
    	  return 0;
    else
     	  return 1;
}

float ReadDB_ANA(char *PointName)
{
    float ReturnValue;
	char err_str[80]="";
    DMS_COMMON common_aa;
    if(ReadEntryByNameStr(PointName,&common_aa)==-1)
    {
		sprintf(err_str,"ReadDB_ANA::No this point %s\n",PointName);
		Err_log(err_str);
		exit(0);
    }
   
    ReturnValue=common_aa.var.anlg.fValue;
    return(ReturnValue);
}
int ReadDB_ANA_Chan(char *PointName)
{
	DMS_COMMON common_aa;
	int ReturnValue;
	char err_str[80]="";
    if(ReadEntryByNameStr(PointName,&common_aa)==-1)
    {
		sprintf(err_str,"ReadDB_ANA_Chan::No this point %s\n",PointName);
		Err_log(err_str);
		exit(0);
    }
	ReturnValue=common_aa.var.anlg.value.status.chan_fail;
	return(ReturnValue);
}

int ReadDB_IND(char *PointName)
{
    int ReturnValue;
    char err_str[80]="";
    DMS_COMMON common_aa;
    if(ReadEntryByNameStr(PointName,&common_aa)==-1)
    {
		sprintf(err_str,"ReadDB_IND::No this point %s\n",PointName);
	  Err_log(err_str);
      exit(0);
    }
    ReturnValue=common_aa.var.ind.status.state;
    return(ReturnValue);
}

int AIC_UpdateRTDB_ANA (char *PointName,float data)
{
     DMS_COMMON common_aa;
     char err_str[80]="";
    time_t  tim;
    struct tm   tmpt;
    int value;
    float OldValue=0;
    value=(int)data*1000;
    if (ReadEntryByNameStr(PointName,&common_aa)==-1)
    {
		sprintf(err_str,"AIC_UpdateRTDB_ANA::No this point %s\n",PointName);
	  Err_log(err_str);
      exit(0);
     }
    
    OldValue=common_aa.var.anlg.fValue; 	
    if(fabs(OldValue-data)<0.001)
        return 0;    
    
    ml_buf_ANA[Number_ANA].type_id =DPS_ANALOG;
    ml_buf_ANA[Number_ANA].stn_id =common_aa.point.stn_id;
    ml_buf_ANA[Number_ANA].dev_id =common_aa.point.dev_id;
    ml_buf_ANA[Number_ANA].data_type=common_aa.point.data_type;
    ml_buf_ANA[Number_ANA].number=1;
    ml_buf_ANA[Number_ANA].point_id = common_aa.point.pt_id;

    ml_buf_ANA[Number_ANA].status=0;
    memcpy(ml_buf_ANA[Number_ANA].data,&value,4);
    GetLocalHostId(&ml_buf_ANA[Number_ANA].host_id);

    tim=time (NULL);
    tmpt=*localtime (&tim);
    ml_buf_ANA[Number_ANA].sec=tmpt.tm_sec;
    ml_buf_ANA[Number_ANA].min=tmpt.tm_min;
    ml_buf_ANA[Number_ANA].hour=tmpt.tm_hour;
    ml_buf_ANA[Number_ANA].day=tmpt.tm_mday;
    ml_buf_ANA[Number_ANA].month=tmpt.tm_mon+1;
    ml_buf_ANA[Number_ANA].year=tmpt.tm_year+1900;
    
   
#ifdef SUN_OS
    Swap2Byte(&ml_buf_ANA[Number_ANA].point_id);
    Swap2Byte(&ml_buf_ANA[Number_ANA].status);
    Swap2Byte(&ml_buf_ANA[Number_ANA].number);
    Swap4Byte(&ml_buf_ANA[Number_ANA].data);
    Swap2Byte(&ml_buf_ANA[Number_ANA].year);
#endif

    Number_ANA+=1;
    lanhd_ANA.length=ANAMSG_SIZE*Number_ANA;
#ifdef SUN_OS
    Swap2Byte(&lanhd_ANA.length);
#endif
    
    lanhd_ANA.dp_type=BJ_DB_TIME;
    lanhd_ANA.dest_id =0;
    GetLocalHostId(&lanhd_ANA.src_id);
    lanhd_ANA.dest_stn =ml_buf_ANA[0].stn_id;
    lanhd_ANA.packet_num =0; 
    if(Number_ANA>=64)
    {
    	if(GetControlState()==1)
           lan_out ((char *)&ml_buf_ANA,lanhd_ANA,(unsigned char)0); 
        Number_ANA=0; 
    }
    return 0;
}

int AIC_UpdateRTDB_IND (char *PointName,short data)
{
   DMS_COMMON common_aa; 
   char err_str[80]="";
    time_t  tim;
    struct tm   tmpt;
    int OldValue,i,Two_Identical;
    Two_Identical=0;
	
    if (ReadEntryByNameStr(PointName,&common_aa)==-1)
    {
	sprintf(err_str,"No this point %s\n",PointName);
	Err_log(err_str);
	
	exit(0);
    }
	//else
	//	printf("in AIC_UpdateRTDB_IND::point= %s\n",PointName);
	
    OldValue=common_aa.var.ind.status.state;  
    if(OldValue==data)
        return  0;

    ml_buf_IND[Number_IND].type_id =DPS_IND_ALM;
    ml_buf_IND[Number_IND].stn_id =common_aa.point.stn_id;
    ml_buf_IND[Number_IND].dev_id =common_aa.point.dev_id;
    ml_buf_IND[Number_IND].data_type=common_aa.point.data_type;
    ml_buf_IND[Number_IND].number=1;
    ml_buf_IND[Number_IND].point_id = common_aa.point.pt_id;

    ml_buf_IND[Number_IND].status=data+256;
 
    GetLocalHostId(&ml_buf_IND[Number_IND].host_id);
    tim=time (NULL);
    tmpt=*localtime (&tim);
    ml_buf_IND[Number_IND].sec=tmpt.tm_sec; 
    ml_buf_IND[Number_IND].min=tmpt.tm_min;
    ml_buf_IND[Number_IND].hour=tmpt.tm_hour;
    ml_buf_IND[Number_IND].day=tmpt.tm_mday;
    ml_buf_IND[Number_IND].month=tmpt.tm_mon+1;
    ml_buf_IND[Number_IND].year=tmpt.tm_year+1900;
    lanhd_IND.length=SYSMSG_SIZE*(Number_IND+1);

#ifdef SUN_OS
    Swap2Byte(&ml_buf_IND[Number_IND].point_id);
    Swap2Byte(&ml_buf_IND[Number_IND].number);
    Swap2Byte(&ml_buf_IND[Number_IND].status);
    Swap2Byte(&ml_buf_IND[Number_IND].year);
    Swap2Byte(&lanhd_IND.length);
#endif

    lanhd_IND.dp_type=BJ_DB_TIME;
    lanhd_IND.dest_id =0;
    GetLocalHostId(&lanhd_IND.src_id);
    lanhd_IND.dest_stn =ml_buf_IND[0].stn_id;
    lanhd_IND.packet_num =0;
    
    for(i=0;i<Number_IND;i++)
    {
    	if(Compare_SYS_MSG(ml_buf_IND[Number_IND],ml_buf_IND[i])==0)
    	{
    		Two_Identical=1;
    		break;
    	}		
    }
	
    if(Two_Identical==1)
    {
    	lanhd_IND.length=SYSMSG_SIZE*(Number_IND);
#ifdef SUN_OS
	Swap2Byte(&lanhd_IND.length);
#endif
    }
    else
    {
    	Number_IND+=1;
    } 
    
    if(Number_IND>=64)
    {
        //if(GetControlState()==1)
		{
          lan_out ((char *)&ml_buf_IND,lanhd_IND,(unsigned char)0);
		  //printf("in Aic_UpdateRTDB_IND,PointName=%s\n",PointName);
         Number_IND=0;
		}
    }
	
    return 0;
}

int SendCommandToPLC (char *PointName,int Type_id,float data,short Status)
{
    DMS_COMMON common_aa;
    COMMAND_MSG	ml_buf;
    char err_str[80]="";
    time_t  tim;
    struct tm   tmpt;
    LAN_HEAD	lanhd;
 
    if (ReadEntryByNameStr(PointName,&common_aa)==-1)
    {
	sprintf(err_str,"No this point %s\n",PointName);
	Err_log(err_str);
	return(-1);
    }

    ml_buf.type_id =Type_id;
    ml_buf.stn_id =common_aa.point.stn_id;
    ml_buf.dev_id =common_aa.point.dev_id;
    ml_buf.data_type=common_aa.point.data_type;
    ml_buf.point_id = common_aa.point.pt_id;

    ml_buf.status.opr= Status;
    ml_buf.status.state=0;
    ml_buf.status.src=1;
    ml_buf.data.iValue=(int)data*1000;
    ml_buf.ctrl_addr=common_aa.fixed.ctrl_addr;
    GetLocalHostId(&ml_buf.host_id);
    ml_buf.usr_id=0;

    tim=time (NULL);
    tmpt=*localtime (&tim);
    ml_buf.second=tmpt.tm_sec;
    ml_buf.minute=tmpt.tm_min;
    ml_buf.hour=tmpt.tm_hour;
    ml_buf.day=tmpt.tm_mday;
    ml_buf.month=tmpt.tm_mon+1;
    ml_buf.year=tmpt.tm_year+1900;
    strcpy(ml_buf.usr_login_name,"AIC");
    lanhd.length=sizeof(COMMAND_MSG);

#ifdef SUN_OS
    Swap2Byte(&ml_buf.point_id);
    Swap2Byte(&ml_buf.status);
    Swap4Byte(&ml_buf.data.iValue);
    Swap2Byte(&ml_buf.ctrl_addr);
    Swap2Byte(&ml_buf.year);
    Swap2Byte(&lanhd.length);
#endif

    lanhd.dp_type=COMM_TIME;
    lanhd.dest_id =ml_buf.dev_id;
    lanhd.src_id =ml_buf.host_id;
    lanhd.dest_stn =ml_buf.stn_id;
    lanhd.packet_num =0;
    if(GetControlState()==1)
        lan_out ((char *)&ml_buf,lanhd,(unsigned char)0);
    return 0;
}

void ReadTime ()/*读实时时间*/
{
	time_t          shjian;	
	time ( &shjian );
	ntime = localtime ( &shjian );
}

void    WriteLogFile (char logtxt[])
{
	FILE *fp;
//	printf("logfilename=%s\n",logfilename);
    fp = fopen(logfilename, "a");
    if ( fp != NULL)
    {
        fwrite( logtxt, strlen( logtxt ),1,fp);
        fclose(fp);
     }
	fclose(fp);
}
void CurveAllPoint(unsigned char stn,unsigned char dev,unsigned short num,unsigned short pt,unsigned short offset,float *fdata)
{
	LAN_HEAD	head;
	COMMON_MSG  	msg;
	short		i;
	time_t     	re_time,newtime;
	struct  tm      *devtime;
	char 		buf[MAX_BUF_SIZE]; 
	
	head.dest_id=0;
	GetLocalHostId((UCHAR	*)&head.src_id);
	head.dp_type=ALL_DB_TIME;

	
	memset(buf,0,MAX_BUF_SIZE);
	msg.type_id 	= DPS_CURVE_ALL;
	
	msg.stn_id = stn;	
	msg.dev_id = dev;
	msg.data_type=CURVE_TYPE;
	msg.point_id = pt;
	
	GetLocalHostId((UCHAR	*)&msg.host_id);
	re_time=time(&newtime);
	devtime=localtime(&newtime);
	msg.ms  =0;
	msg.sec  =devtime->tm_sec;
	msg.min  =devtime->tm_min;
	msg.hour  =devtime->tm_hour;
        msg.day  =devtime->tm_mday;
        msg.month  =devtime->tm_mon+1;
        msg.year  =devtime->tm_year+1900; 
               
	head.length=8;
	
	memcpy(buf+head.length,&offset,2);
#ifdef SUN_OS
	Swap2Byte(&buf[head.length]);
#endif	
	head.length=head.length+2;
	
	for(i=0;i<num;i++)
	{	
		if(head.length>=(MAX_BUF_SIZE-18))
		{
			printf(" length >MAX_BUF_SIZE\n");
			 break;
		}
		
		memcpy(buf+head.length,&fdata[i],4);
#ifdef SUN_OS
		Swap4Byte(&buf[head.length]);
#endif			
		head.length=head.length+4;
	}
	
	msg.number=i;
#ifdef SUN_OS
	Swap2Byte(&msg.number);	
	Swap2Byte(&msg.point_id);
	Swap2Byte(&msg.ms);
	Swap2Byte(&msg.year);
#endif	
	memcpy(buf,&msg,8);
	
	memcpy(buf+head.length,&msg.host_id, 8);
	
	head.length=head.length+8;
	
	
#ifdef SUN_OS
	Swap2Byte(&head.length);
#endif	
	if(GetControlState()==1)		
 	    lan_out((char *)buf,head,0);
	return;
}


void CheckControlable()	
{
	int 	i,j;
	char	sbo_db_name[80];
	int		sbo_flag;	
	
	for(i=1;i<=FIELD_NUM;i++)
		for(j=1;j<=IRRIGATION_TURN_NUMB;j++)
	{
		strcpy ( sbo_db_name, DB_NAME_FIELD_AP_CZKT[ i ] );//该轮灌组正常参与轮灌
		strcat ( sbo_db_name, "_YES.." );
		//printf("field::sbo_db_name=%s\n",sbo_db_name);

		sbo_flag = InterLock_Judge ( sbo_db_name );	
		
		if(sbo_flag==1)
		{
			PUMP.Field_Gate[i].AP_Avail=1;
		}
		else
		{
			PUMP.Field_Gate[i].AP_Avail=0;
		}
		//printf("CZKT=%s\n",DB_NAME_FIELD_AP_CZKT[i]);
		AIC_UpdateRTDB_IND(DB_NAME_FIELD_AP_CZKT[i],(short)PUMP.Field_Gate[i].AP_Avail);
	}

	for(i=1;i<=FIELD_NUM;i++)
	for(j=1; j<=IRRIGATION_GROUP_NUMB;j++)
	{		
		strcpy ( sbo_db_name, DB_NAME_GROUP_AP_CZKT[ i ][j] );//该轮灌组正常参与轮灌
		strcat ( sbo_db_name, "_YES.." );
		//printf("group sbo_db_name=%s\n",sbo_db_name);

		sbo_flag = InterLock_Judge ( sbo_db_name );			
		
		if(sbo_flag==1)
        {
            PUMP.Field_Sys[i].Irrigation_Group[j].AP_Avail = 1;               
		}
		else
		{
			PUMP.Field_Sys[i].Irrigation_Group[j].AP_Avail= 0; 			
		}
		/*strcpy ( sbo_db_name, DB_NAME_AP_CZKT[ i ][j] );
		strcat ( sbo_db_name, "_NO.." );
		sbo_flag = InterLock_Judge ( sbo_db_name );
		if(sbo_flag==1)
		{
			Irrigation_Group[i].AP_Avail=0;
			AIC_UpdateRTDB_IND(DB_NAME_GROUP_AP_CZKT[i][j],0);			
		 }*/
		//printf("DB_NAME_GROUP_AP_CZKT[%d][%d]=%s\n",i,j,DB_NAME_GROUP_AP_CZKT[i][j]);
		
		AIC_UpdateRTDB_IND(DB_NAME_GROUP_AP_CZKT[i][j],(short)PUMP.Field_Sys[i].Irrigation_Group[j].AP_Avail);
	}  	
}


void InitialAIC_Data_On_Screen ()//控制命令初始化为无操作
{
	int i,m;
	//printf("InitialAIC_Data_On_Screen here\n");
	for ( i= 1; i<= FIELD_NUM; i++ )  
	{
		AIC_UpdateRTDB_IND(DB_NAME_Field_Control_Command[i],WCZTHM);	
		
	}
	for ( i= 1; i<= FIELD_NUM; i++ )  
	for ( m= 1; m<=IRRIGATION_GROUP_NUMB ; m++ )  
	{
			AIC_UpdateRTDB_IND(DB_NAME_IrrigationGroup_Control_Command[i][m],WCZTHM);	
	}
	//printf("InitialAIC_Data_On_Screen end\n");
}

void GetFieldValvePt()
{
		FILE	*fp;
	char filename[100]="\\home\\ems\\h9000\\AIC\\Field_gate.DEF";
	char keyword[30]="";
	char tmp[100]="";
	int i,m;
	char err_str[80]="";

	if( (fp=fopen(filename, "r"))==NULL )
	{
		printf("Fail to open %s\n", filename);
		UnMount_handle();
	}

	for(i=1;i<=MAX_FIELD_VALVE_NUM;i++)	
	{	
		sprintf(keyword,"%d",i);
		if(-1==LocateKeyword(fp,keyword))
			continue;
	
		fscanf(fp,"%s\n",PUMP.Field_Gate[i].db_name_valve);
		//printf("keyword=%s,fread=%s\n",keyword,PUMP.Field_Gate[i].db_name_valve);	
		
		if(ReadEntryByNameStr(PUMP.Field_Gate[i].db_name_valve,&common_aa)==-1)
		{
			printf("4No this point %s\n",PUMP.Field_Gate[i].db_name_valve);
			sprintf(err_str,"No this point %s\n",PUMP.Field_Gate[i].db_name_valve);
			BdSms(err_str);
			exit(0);
		}
#ifdef DEBUG
		printf("PUMP.Field_Gate[%d].db_name_valve=%s\n",i,PUMP.Field_Gate[i].db_name_valve);
#endif
	}	

	fclose(fp);
}

void GetGroupValvePt()
{
		FILE	*fp;
	char filename[100]="\\home\\ems\\h9000\\AIC\\Field_group_gate.DEF";
	char keyword[30]="";
	char StrTemp[50]="",StrTemp1[50]="",tmp[100]="";
	int i,j,m,n,k;
	char err_str[80]="";

	if( (fp=fopen(filename, "r"))==NULL )
	{
		printf("Fail to open %s\n", filename);
		UnMount_handle();
	}

	for(i=1;i<=FIELD_NUM;i++)
	for(m=1;m<=IRRIGATION_GROUP_NUMB;m++)		
	{
			sprintf(keyword,"%d%d",i,m);
			if(-1==LocateKeyword(fp,keyword))
				continue;
			for(k=1;k<=MAX_VALVE_NUM_IN_GROUP;k++)
			{
				strcpy(PUMP.Field_Sys[i].Irrigation_Group[m].db_name_valve[k],"");
				fscanf(fp,"%s",PUMP.Field_Sys[i].Irrigation_Group[m].db_name_valve[k]);
				if(strchr(PUMP.Field_Sys[i].Irrigation_Group[m].db_name_valve[k],'[')!=0)
					break;
				if(ReadEntryByNameStr(PUMP.Field_Sys[i].Irrigation_Group[m].db_name_valve[k],&common_aa)==-1)
				{
					printf("5No this point %s\n",PUMP.Field_Sys[i].Irrigation_Group[m].db_name_valve[k]);
					sprintf(err_str,"No this point %s\n",PUMP.Field_Sys[i].Irrigation_Group[m].db_name_valve[k]);
					BdSms(err_str);
					exit(0);
				}
			}
#ifdef DEBUG
		printf("PUMP.Field_Sys[%d].Irrigation_Group[%d].db_name_valve[%d]=%s\n",i,m,k,PUMP.Field_Sys[i].Irrigation_Group[m].db_name_valve[k]);
#endif
	}	
	
	fclose(fp);

}

void ReadAICDefFile()
{
	FILE	*fp;
	char filename[100]="\\home\\ems\\h9000\\AIC\\AIC.DEF";
	char keyword[30]="";
	char StrTemp[50]="",StrTemp1[50]="",tmp[100]="";
	int i,j,m,n;
	char err_str[80]="";

	if( (fp=fopen(filename, "r"))==NULL )
	{
		printf("Fail to open %s\n", filename);
		UnMount_handle();
	}

	strcpy(keyword,"AIC_STATE");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();

	fscanf(fp,"%s\n",DB_NAME_AIC_STATE);
	if(ReadEntryByNameStr(DB_NAME_AIC_STATE,&common_aa)==-1)
	{
		printf("No this point %s\n",DB_NAME_AIC_STATE);
		sprintf(err_str,"No this point %s\n",DB_NAME_AIC_STATE);
		BdSms(err_str);
		exit(0);
	}
#ifdef DEBUG
	printf("DB_NAME_AIC_STATE=%s\n",DB_NAME_AIC_STATE);
#endif

	 strcpy(keyword,"AIC_Period");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_AIC_Period);
	
	if(ReadEntryByNameStr(DB_NAME_AIC_Period,&common_aa)==-1)
	{
		printf("No this point %s\n",DB_NAME_AIC_Period);
		sprintf(err_str,"No this point %s\n",DB_NAME_AIC_Period);
		BdSms(err_str);
		exit(0);
	}
#ifdef DEBUG
	printf("DB_NAME_AIC_Period=%s\n",DB_NAME_AIC_Period);
#endif	

	strcpy(keyword,"AIC_CLOSED_LOOP");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_AIC_CLOSED_LOOP);
	if(ReadEntryByNameStr(DB_NAME_AIC_CLOSED_LOOP,&common_aa)==-1)
	{
		printf("No this point %s\n",DB_NAME_AIC_CLOSED_LOOP);
		sprintf(err_str,"No this point %s\n",DB_NAME_AIC_CLOSED_LOOP);
		BdSms(err_str);
		exit(0);
	}
#ifdef DEBUG
	printf("DB_NAME_AIC_CLOSED_LOOP=%s\n",DB_NAME_AIC_CLOSED_LOOP);
#endif

	

	strcpy(keyword,"QC_AIC");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_QC_AIC);
	if(ReadEntryByNameStr(DB_NAME_QC_AIC,&common_aa)==-1)
	{
		printf("No this point %s\n",DB_NAME_QC_AIC);
		sprintf(err_str,"No this point %s\n",DB_NAME_QC_AIC);
		BdSms(err_str);
		exit(0);
	}
#ifdef DEBUG
	printf("DB_NAME_QC_AIC=%s\n",DB_NAME_QC_AIC);
#endif 	

	strcpy(keyword,"FieldNumber");
	
	if(-1==LocateKeyword(fp,keyword))
	{
		printf("filename=%s,%s not exist\n",filename,keyword);
		UnMount_handle();
	}
	printf("here is ReadAICDefFile!\n");
	fscanf(fp,"%d\n",&FIELD_NUM); 
	
	if((FIELD_NUM>MAX_FIELD_NUMB)||(FIELD_NUM<=0))
	{
		printf("田间数定义错误!\n");
		UnMount_handle();
	}
#ifdef DEBUG
	printf("FIELD_NUM=%d\n",FIELD_NUM);
#endif   
	PUMP.Field_num=FIELD_NUM;

    strcpy(keyword,"IrrigationGroupNumber");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%d\n",&IRRIGATION_GROUP_NUMB);
    
	if((IRRIGATION_GROUP_NUMB>MAX_IRRIGATION_GROUP_NUMB)||(IRRIGATION_GROUP_NUMB<=0))
	{
		printf("轮灌组数定义错误!\n");
		UnMount_handle();
	}
#ifdef DEBUG
	printf("IRRIGATION_GROUP_NUMB=%d\n",IRRIGATION_GROUP_NUMB);
#endif   


    strcpy(keyword,"IrrigationTurnNumber");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%d\n",&IRRIGATION_TURN_NUMB);
    
	if((IRRIGATION_TURN_NUMB>MAX_IRRIGATION_TURN)||(IRRIGATION_TURN_NUMB<=0))
	{
		printf("轮灌次数定义错误!\n");
		UnMount_handle();
	}
#ifdef DEBUG
	printf("IRRIGATION_TURN_NUMB=%d\n",IRRIGATION_TURN_NUMB);
#endif   
		

	
	strcpy(keyword,"FIELD_Control_Command");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",tmp);

	for(i=1;i<=FIELD_NUM;i++)	
	{
		sprintf(DB_NAME_Field_Control_Command[i],"FIELD%d.SYS.SYS.%s",i,tmp);
		if(ReadEntryByNameStr(DB_NAME_Field_Control_Command[i],&common_aa)==-1)
		{
				printf("No this point %s\n",DB_NAME_Field_Control_Command[i]);
				sprintf(err_str,"No this point %s\n",DB_NAME_Field_Control_Command[i]);
				BdSms(err_str);
				exit(0);
		}
#ifdef DEBUG
		printf("DB_NAME_Field_Control_Command[%d]=%s\n",i,DB_NAME_Field_Control_Command[i]);
#endif
	}	

	strcpy(keyword,"GROUP_Control_Command");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",tmp);

	for(i=1;i<=FIELD_NUM;i++)
		for(m=1;m<=IRRIGATION_GROUP_NUMB;m++)
		{
				sprintf(DB_NAME_IrrigationGroup_Control_Command[i][m],"FIELD%d.GROUP%d.SYS.%s",i,m,tmp);
				if(ReadEntryByNameStr(DB_NAME_IrrigationGroup_Control_Command[i][m],&common_aa)==-1)
				{
					printf("1No this point %s\n",DB_NAME_IrrigationGroup_Control_Command[i][m]);
					sprintf(err_str,"No this point %s\n",DB_NAME_IrrigationGroup_Control_Command[i][m]);
					BdSms(err_str);
					exit(0);
				}
#ifdef DEBUG
		printf("DB_NAME_IrrigationGroup_Control_Command[%d][%d]=%s\n",i,m,DB_NAME_IrrigationGroup_Control_Command[i][m]);
#endif
		}

	
	

	strcpy(keyword,"FIELD_AP_Group");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",tmp);
	for(i=1;i<=FIELD_NUM;i++)
	{
		sprintf(DB_NAME_AP_Group_Field[i],"FIELD%d.SYS.SYS.%s",i,tmp);
		if(ReadEntryByNameStr(DB_NAME_AP_Group_Field[i],&common_aa)==-1)
		{
			printf("2No this point %s\n",DB_NAME_AP_Group_Field[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_Group_Field[i]);
			BdSms(err_str);
			exit(0);
		}
#ifdef DEBUG
		printf("DB_NAME_AP_Group_Field[%d]=%s\n",i,DB_NAME_AP_Group_Field[i]);
#endif
	}

	strcpy(keyword,"GROUP_AP_Group");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",tmp);

	for(i=1;i<=FIELD_NUM;i++)
	for(j=1;j<=IRRIGATION_GROUP_NUMB;j++)	
	{
		sprintf(DB_NAME_AP_Group_Group[i][j],"FIELD%d.GROUP%d.SYS.%s",i,j,tmp);
		if(ReadEntryByNameStr(DB_NAME_AP_Group_Group[i][j],&common_aa)==-1)
		{
			printf("3No this point %s\n",DB_NAME_AP_Group_Group[i][j]);
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_Group_Group[i][j]);
			BdSms(err_str);
			exit(0);
		}
#ifdef DEBUG
		printf("DB_NAME_AP_Group[%d]=%s\n",i,DB_NAME_AP_Group[i]);
#endif
	}

	
	strcpy(keyword,"Field_STATUS");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();

	fscanf(fp,"%s\n",tmp);

	for(i=1;i<=FIELD_NUM;i++)	
	{		
		sprintf(DB_NAME_Field_Status[i],"FIELD%d.SYS.SYS.%s",i,tmp);
		if(ReadEntryByNameStr(DB_NAME_Field_Status[i],&common_aa)==-1)
		{
			printf("4No this point %s\n",DB_NAME_Field_Status[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Field_Status[i]);
			BdSms(err_str);
			exit(0);
		}
#ifdef DEBUG
		printf("DB_NAME_Field_Status[%d]=%s\n",i,DB_NAME_Field_Status[i]);
#endif
	}
	

		strcpy(keyword,"Group_STATUS");
		if(-1==LocateKeyword(fp,keyword))
			UnMount_handle();
		fscanf(fp,"%s\n",tmp);

		for(i=1;i<=FIELD_NUM;i++)
		for(m=1;m<=IRRIGATION_GROUP_NUMB;m++)		
		{
			sprintf(DB_NAME_Group_Status[i][m],"FIELD%d.GROUP%d.SYS.%s",i,m,tmp);
			if(ReadEntryByNameStr(DB_NAME_Group_Status[i][m],&common_aa)==-1)
			{
				printf("5No this point %s\n",DB_NAME_Group_Status[i][m]);
				sprintf(err_str,"No this point %s\n",DB_NAME_Group_Status[i][m]);
				BdSms(err_str);
				exit(0);
			}
#ifdef DEBUG
		printf("DB_NAME_Group_Status[%d][%d]=%s\n",i,m,DB_NAME_Group_Status[i][m]);
#endif
		}
	
	
	strcpy(keyword,"FIELD_AP_CZKT");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",tmp);
	for(i=1;i<=FIELD_NUM;i++)
	{
		sprintf(DB_NAME_FIELD_AP_CZKT[i],"FIELD%d.SYS.SYS.%s",i,tmp);
		//printf("#### DB_NAME_FIELD_AP_CZKT[%d]=%s\n",i,DB_NAME_FIELD_AP_CZKT[i]);
		if(ReadEntryByNameStr(DB_NAME_FIELD_AP_CZKT[i],&common_aa)==-1)
		{
			printf("6No this point %s\n",DB_NAME_FIELD_AP_CZKT[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_FIELD_AP_CZKT[i]);
			BdSms(err_str);
			exit(0);
		}
#ifdef DEBUG
		printf("DB_NAME_FIELD_AP_CZKT[%d]=%s\n",i,DB_NAME_FIELD_AP_CZKT[i]);
#endif
	} 

	strcpy(keyword,"GROUP_AP_CZKT");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",tmp);

	for(j=1;j<=FIELD_NUM;j++)
	for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)
	{
		sprintf(DB_NAME_GROUP_AP_CZKT[j][i],"FIELD%d.GROUP%d.SYS.%s",j,i,tmp);
		if(ReadEntryByNameStr(DB_NAME_GROUP_AP_CZKT[j][i],&common_aa)==-1)
		{
			//printf("7No this point %s\n",DB_NAME_GROUP_AP_CZKT[j][i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_GROUP_AP_CZKT[j][i]);
			BdSms(err_str);
			exit(0);
		}
#ifdef DEBUG
		printf("DB_NAME_GROUP_AP_CZKT[%d][%d]=%s\n",j,i,DB_NAME_GROUP_AP_CZKT[j][i]);
#endif
	} 
	
	strcpy(keyword,"FIELD_STARTTIME");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",tmp);

	for(i=1;i<=FIELD_NUM;i++)
	for(j=1;j<=IRRIGATION_TURN_NUMB;j++)
	{
		sprintf(DB_NAME_Field_StartTime[i][j],"FIELD%d.SYS.ANA.%s%d",i,tmp,j);
		if(ReadEntryByNameStr(DB_NAME_Field_StartTime[i][j],&common_aa)==-1)
		{
				printf("8No this point %s\n",DB_NAME_Field_StartTime[i][j]);
				sprintf(err_str,"No this point %s\n",DB_NAME_Field_StartTime[i][j]);
				BdSms(err_str);
				exit(0);
		}
		
#ifdef DEBUG
		printf("DB_NAME_Field_StartTime[%d][%d]=%s\n",i,j,DB_NAME_Field_StartTime[i][j]);
#endif
	}

	strcpy(keyword,"FIELD_ENDTIME");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",tmp);

	for(i=1;i<=FIELD_NUM;i++)
	for(j=1;j<=IRRIGATION_TURN_NUMB;j++)
	{
		sprintf(DB_NAME_Field_EndTime[i][j],"FIELD%d.SYS.ANA.%s%d",i,tmp,j);		
		
		if(ReadEntryByNameStr(DB_NAME_Field_EndTime[i][j],&common_aa)==-1)
		{
				printf("9No this point %s\n",DB_NAME_Field_EndTime[i][j]);
				sprintf(err_str,"No this point %s\n",DB_NAME_Field_EndTime[i][j]);
				BdSms(err_str);
				exit(0);
		}
#ifdef DEBUG
		printf("DB_NAME_Field_EndTime[%d][%d]=%s\n",i,j,DB_NAME_Field_EndTime[i][j]);
#endif

	}	

	strcpy(keyword,"GROUP_STARTTIME");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",tmp);	

	for(i=1;i<=FIELD_NUM;i++)
	for(m=1;m<=IRRIGATION_GROUP_NUMB;m++)
	for(n=1;n<=IRRIGATION_TURN_NUMB;n++)
		{				
			sprintf(DB_NAME_Group_StartTime[i][m][n],"FIELD%d.GROUP%d.ANA.%s%d",i,m,tmp,n);
			if(ReadEntryByNameStr(DB_NAME_Group_StartTime[i][m][n],&common_aa)==-1)
			{
				printf("10No this point %s\n",DB_NAME_Group_StartTime[i][m][n]);
				sprintf(err_str,"No this point %s\n",DB_NAME_Group_StartTime[i][m][n]);
				BdSms(err_str);
				exit(0);
			}
			
			
#ifdef DEBUG
		printf("DB_NAME_Group_StartTime[%d][%d][%d]=%s\n",i,m,n,DB_NAME_Group_StartTime[i][m][n]);		
#endif
		}

	strcpy(keyword,"GROUP_ENDTIME");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",tmp);	

	for(i=1;i<=FIELD_NUM;i++)
	for(m=1;m<=IRRIGATION_GROUP_NUMB;m++)
	for(n=1;n<=IRRIGATION_TURN_NUMB;n++)
		{				
			sprintf(DB_NAME_Group_EndTime[i][m][n],"FIELD%d.GROUP%d.ANA.%s%d",i,m,tmp,n);		

			if(ReadEntryByNameStr(DB_NAME_Group_EndTime[i][m][n],&common_aa)==-1)
			{
				printf("11No this point %s\n",DB_NAME_Group_EndTime[i][m][n]);
				sprintf(err_str,"No this point %s\n",DB_NAME_Group_EndTime[i][m][n]);
				BdSms(err_str);
				exit(0);
			}
			
#ifdef DEBUG		
		printf("DB_NAME_Group_EndTime[%d][%d][%d]=%s\n",i,m,n,DB_NAME_Group_EndTime[i][m][n]);
#endif
		}
	 
	strcpy(keyword,"FIELD_PUMPTIME");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",tmp);	

	for(i=1;i<=FIELD_NUM;i++)
	for(j=1;j<=IRRIGATION_TURN_NUMB;j++)
	{
		sprintf(DB_NAME_Field_PumpTime[i][j],"FIELD%d.SYS.ANA.%s%d",i,tmp,j);
		if(ReadEntryByNameStr(DB_NAME_Field_PumpTime[i][j],&common_aa)==-1)
			{
				printf("12No this point %s\n",DB_NAME_Field_PumpTime[i][j]);
				sprintf(err_str,"No this point %s\n",DB_NAME_Field_PumpTime[i][j]);
				BdSms(err_str);
				exit(0);
			}
#ifdef DEBUG
		printf("DB_NAME_Field_PumpTime[%d][%d]=%s\n",i,j,DB_NAME_Field_PumpTime[i][j]);
#endif
	}

	strcpy(keyword,"GROUP_PUMPTIME");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",tmp);	

	for(i=1;i<=FIELD_NUM;i++)
	for(m=1;m<=IRRIGATION_GROUP_NUMB;m++)
	for(n=1;n<=IRRIGATION_TURN_NUMB;n++)
	{
			sprintf(DB_NAME_Group_PumpTime[i][m][n],"FIELD%d.GROUP%d.ANA.%s%d",i,m,tmp,n);
			if(ReadEntryByNameStr(DB_NAME_Group_PumpTime[i][m][n],&common_aa)==-1)
			{
				printf("13No this point %s\n",DB_NAME_Group_PumpTime[i][m][n]);
				sprintf(err_str,"No this point %s\n",DB_NAME_Group_PumpTime[i][m][n]);
				BdSms(err_str);
				exit(0);
			}
#ifdef DEBUG
		printf("DB_NAME_Group_PumpTime[%d][%d][%d]=%s\n",i,m,n,DB_NAME_Group_PumpTime[i][m][n]);
#endif
	}	
	//printf("###fclose\n");
	fclose(fp);
}

void LoadFieldIrrigationTime()
{
	FILE	*fp;
	char filename[100]="\\home\\ems\\h9000\\AIC\\Field_Irrigation_Time.DEF";
	char keyword[30]="";
	struct tm		startTime,endTime;
	char StartTime[50]="",EndTime[50]="";	
	char time_str[80]="";
	int i,j=0,Field_Num;
	char *delim="-";
	char *Mon=".";
	int start_mon1,start_day1,end_mon1,end_day1,start_hour1,start_min1,end_hour1,end_min1;

	if( (fp=fopen(filename, "r"))==NULL )
	{
		printf("Fail to open %s\n", filename);
		UnMount_handle();
		return;
	}
	fseek( fp, 0L, SEEK_SET );
	strcpy(keyword,"Begin");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();		
	

	for(i=1;i<=FIELD_NUM;i++)
	{
		fscanf(fp,"%d",&Field_Num);
		//printf("!!!!Field_Num=%d\n",Field_Num);
		if(Field_Num<1||Field_Num>MAX_FIELD_NUMB)
		{
			printf("Field_Num is wrong!Field_Num=%d\n",Field_Num);
			break;
		}
				
		for(j=1;j<=IRRIGATION_TURN_NUMB;j++)
		{	
			fscanf(fp,"%d.%d %d:%d-%d.%d %d:%d",
				&start_mon1,&start_day1,&start_hour1,&start_min1,
				&end_mon1,&end_day1,&end_hour1,&end_min1);
		//StringtoTime(char *str,struct tm start_time,struct tm end_time);
			/*printf("LoadFieldIrrigationTime::%d.%d %d:%d-%d.%d %d:%d \n",start_mon1,start_day1,start_hour1,start_min1,
			end_mon1,end_day1,end_hour1,end_min1);*/
		startTime.tm_year = ntime->tm_year;//-1900;
		startTime.tm_mon = start_mon1-1;

		startTime.tm_mday = start_day1;
		startTime.tm_hour = start_hour1;
		startTime.tm_min = start_min1;
		startTime.tm_sec = 0;
		startTime.tm_isdst = 0;
		PUMP.Field_Gate[i].start_time[j] =mktime(&startTime);

		endTime.tm_year = ntime->tm_year;//-1900;
		endTime.tm_mon = end_mon1-1;
	//	printf("startTime.tm_mon=%d,endTime.tm_mon=%d\n",startTime.tm_mon,endTime.tm_mon);
		endTime.tm_mday = end_day1;
		endTime.tm_hour = end_hour1;
		endTime.tm_min = end_min1;
		endTime.tm_sec = 0;
		endTime.tm_isdst = 0;
		PUMP.Field_Gate[i].end_time[j]= mktime(&endTime);


		AIC_UpdateRTDB_ANA( DB_NAME_Field_StartTime[i][j], PUMP.Field_Gate[i].start_time[j]);
		
		AIC_UpdateRTDB_ANA(DB_NAME_Field_EndTime[i][j],PUMP.Field_Gate[i].end_time[j]);
		AIC_UpdateRTDB_ANA(DB_NAME_Field_PumpTime[i][j],PUMP.Field_Gate[i].Pumping_Time[j]);		
		//printf("i=%d j=%d updated,start_mon=%d\n",i,j,start_mon1);
//#ifdef DEBUG
	//printf("group=%d,time=%s,Irrigation_time=%d\n",group,StrTemp,Irrigation_time);
//#endif 
			
	}
	}
	fclose(fp);
  
}

void LoadGroupTimeFromFile()
{
	char tmp[20];
	FILE	*fp;
	char filename[100]="\\home\\ems\\h9000\\AIC\\Group_Irrigation_TIME.DEF";
	char keyword[30]="";
	char StartTime[50]="",EndTime[50]="";	
	char time_str[80]="";
	int i,j,m,group,field;
	char *delim="-";
	struct tm		startTime,endTime;
	char *Mon=".";
	int start_mon,start_day,end_mon,end_day,start_hour,start_min,end_hour,end_min;

	if( (fp=fopen(filename, "r"))==NULL )
	{
		printf("Fail to open %s\n", filename);
		UnMount_handle();
		return;
	}
	fseek( fp, 0L, SEEK_SET );
	strcpy(keyword,"Begin");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
printf("in LoadGroupTimeFromFile\n");	
	for(;;)
	{		
		/*if(feof(fp)||)
			break;	*/
		fscanf(fp,"%s",tmp);
		//printf("tmp=%s\n",tmp);
		if(strcmp(tmp,"END")==0)
		{
			printf("break!!!\n");
			break;
		}
		else
			field= atoi(tmp);
		//fscanf(fp,"%d",&field);
	//	printf("here is for loop,field=%d,tmp=%s\n",field,tmp);
		if(field<1||field>MAX_FIELD_NUMB)
		{			
			//printf("field is wrong!\n");
			break;
		}

		fscanf(fp,"%d",&group);
		if(group<1||group>IRRIGATION_GROUP_NUMB)
		{
			printf("group is wrong!\n");
			break;
		}

		for(j=1;j<=MAX_IRRIGATION_TURN;j++)
		{
			fscanf(fp,"%d.%d %d:%d-%d.%d %d:%d",
				&start_mon,&start_day,&start_hour,&start_min,
				&end_mon,&end_day,&end_hour,&end_min);
			//printf("load group time: field %d group %d turn %d start_mon=%d,start_day=%d\n",field,group,j,start_mon,start_day);
			startTime.tm_year = ntime->tm_year;//-1900;
			startTime.tm_mon = start_mon-1;
			startTime.tm_mday = start_day;
			startTime.tm_hour = start_hour;
			startTime.tm_min = start_min;
			startTime.tm_sec = 0;
			startTime.tm_isdst = 0;
		

			endTime.tm_year = ntime->tm_year;//-1900;
			endTime.tm_mon = end_mon-1;
			endTime.tm_mday = end_day;
			endTime.tm_hour = end_hour;
			endTime.tm_min = end_min;
			endTime.tm_sec = 0;
			endTime.tm_isdst = 0;
		
			i=field;
			m=group;
			{
				PUMP.Field_Sys[i].Irrigation_Group[m].start_time[j] =mktime(&startTime);
				PUMP.Field_Sys[i].Irrigation_Group[m].end_time[j]= mktime(&endTime);
				AIC_UpdateRTDB_ANA( DB_NAME_Group_StartTime[i][m][j], PUMP.Field_Sys[i].Irrigation_Group[m].start_time[j]);
				AIC_UpdateRTDB_ANA(DB_NAME_Group_EndTime[i][m][j],PUMP.Field_Sys[i].Irrigation_Group[m].end_time[j]);
				AIC_UpdateRTDB_ANA(DB_NAME_Group_PumpTime[i][m][j],PUMP.Field_Sys[i].Irrigation_Group[m].Pumping_Time[j]);	
			}
		}		
	}
  fclose(fp);
}

void   LogFileChangeDayAndWriteTime()
{
	char   logfilename[50];
	FILE *fp;

	if( oldday != ntime->tm_mday )
	{
		oldday = ntime->tm_mday;
		sprintf(logfilename, "/home/ems/h9000/his/AIC/AICcmd.%02d",ntime->tm_mday);
	    fp=fopen(logfilename, "w");
	    fclose(fp);
	}
}

void ReadAIC_DataFromRTDB()
{
    int i1,j1,j2;	
	int sbo_flag;
	char sbo_db_name[80];    

	strcpy ( sbo_db_name, DB_NAME_QC_AIC );
    strcat ( sbo_db_name, "_NO.." );
    sbo_flag = InterLock_Judge ( sbo_db_name );

	if(sbo_flag==1)
	{
		//printf("DB_NAME_AIC_STATE=%s\n",DB_NAME_AIC_STATE);
		AIC_UpdateRTDB_IND ( DB_NAME_AIC_STATE, 0);	//全厂AIC闭锁条件不满足，将其状态置为挂起
		printf("sbo_flag=1,全厂AIC闭锁条件不满足\n");
	}
	AIC_Period=ReadDB_ANA(DB_NAME_AIC_Period);

//	printf("Read AIC_Period=%d,DB_NAME_AIC_Period=%s\n",AIC_Period,DB_NAME_AIC_Period);

	if(AIC_Period<1)
	{
		AIC_Period=5;
		AIC_UpdateRTDB_ANA( DB_NAME_AIC_Period, (float)AIC_Period);
	}


/*	PUMP.Control_Right = ReadDB_IND (DB_NAME_Control_Right);
	printf("in ReadAIC_DataFromRTDB\n");*/
    PUMP.Closed_Loop = ReadDB_IND ( DB_NAME_AIC_CLOSED_LOOP );
	 

    PUMP.AIC_state=ReadDB_IND ( DB_NAME_AIC_STATE );//   执行/挂起

	if(PUMP.AIC_state==0)
	{
		PUMP.Closed_Loop=0;
		//printf("DB_NAME_AIC_CLOSED_LOOP=%s\n",DB_NAME_AIC_CLOSED_LOOP);
		AIC_UpdateRTDB_IND(DB_NAME_AIC_CLOSED_LOOP,0);
		
	//	AIC_UpdateRTDB_IND(DB_NAME_Control_Right,0);
		
	/*	for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)
		{
				//printf("DB_NAME_Control_Right=%s\n",DB_NAME_AP_Group[i]);
			AIC_UpdateRTDB_IND(DB_NAME_AP_Group[i], 0);
		}*/
	}

	CheckControlable();
	for(i1=1;i1<=PUMP.Field_num;i1++)
		for(j1=1;j1<=IRRIGATION_TURN_NUMB;j1++)
	{
		//PUMP.Field_Gate[i1][j1].Status = ReadDB_IND(DB_NAME_Field_Status[i1]);
		PUMP.Field_Gate[i1].start_time[j1]=ReadDB_ANA(DB_NAME_Field_StartTime[i1][j1]);
		PUMP.Field_Gate[i1].end_time[j1]=ReadDB_ANA(DB_NAME_Field_EndTime[i1][j1]);
		if(PUMP.Field_Gate[i1].AP_Avail==0)
		{
				AIC_UpdateRTDB_IND( DB_NAME_Field_Control_Command[i1], WCZTHM);		
		}
	}
	for(i1=1;i1<=PUMP.Field_num;i1++)
	{	

		for(j1=1;j1<=IRRIGATION_GROUP_NUMB;j1++)
		for(j2=1;j2<=IRRIGATION_TURN_NUMB;j2++)
		{		
			//PUMP.Field_Sys[i1].Irrigation_Group[j1].Status[j2] = ReadDB_IND(DB_NAME_Group_Status[i1][j1]);
			
			PUMP.Field_Sys[i1].Irrigation_Group[j1].start_time[j2]=ReadDB_ANA(DB_NAME_Group_StartTime[i1][j1][j2]);
			PUMP.Field_Sys[i1].Irrigation_Group[j1].end_time[j2]=ReadDB_ANA(DB_NAME_Group_EndTime[i1][j1][j2]);
			if(PUMP.Field_Sys[i1].Irrigation_Group[j1].AP_Avail==0)
			{
				AIC_UpdateRTDB_IND( DB_NAME_IrrigationGroup_Control_Command[i1][j1], WCZTHM);		
			}
		}	
	}
	

}

/*void GetPumpGroupByPrivilege()//将每个轮灌组的曲线排次序
{
	int i,First_Start,First_Stop,tmp1,tmp2;
	
	First_Start=0;
	First_Stop=0;
	tmp1=1000;
	tmp2=-1;

	PUMP.Group_Start=0;
	PUMP.Group_Stop=0;
	for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)//找出优先权最高的轮灌组
	{
		if(Irrigation_Group[i].AP_Avail==0)
		{
			printf("!!!group %d is not in group\n",i);
			continue;
		}
		if((Irrigation_Group[i].Status!=IN_PUMPED)&&(Irrigation_Group[i].Privilege<tmp1))
		{
			tmp1=(int)Irrigation_Group[i].Privilege;
			First_Start=i;
		}
		if((Irrigation_Group[i].Status==IN_PUMPING)&&(Irrigation_Group[i].Privilege>tmp2))
		{
			tmp2=(int)Irrigation_Group[i].Privilege;
			First_Stop=i;
		}
		
	}

	if(First_Start>0)
	{		
		PUMP.Group_Start=First_Start;
	}
	else
		printf("no group start\n");
	if(First_Stop>0)
	{		
		PUMP.Group_Stop=First_Stop;
	}
}

void StartStopModule()
{
	int Turn_Start,Turn_Stop;
	Turn_Start=0;
	Turn_Stop=0;
	if(PUMP.Start>0)
	{
		Turn_Start=PUMP.Turn_Start;	
		Irrigation_Group[Turn_Start].Start=1;
		printf("in StartStopModule:: Irrigation_Group[%d].Start=%d\n",Turn_Start,Irrigation_Group[Turn_Start].Start);
	}
	if(PUMP.Turn_Stop>0)
	{
		Turn_Stop=PUMP.Turn_Stop;	
		Irrigation_Group[Turn_Stop].Stop=1;
		printf("in StartStopModule:: Irrigation_Group[%d].Stop=%d\n",Turn_Stop,Irrigation_Group[Turn_Stop].Stop);
	}
}*/

void FieldGateFillCommandBuf()
{
    int i,j,flag,needStart,needStop,cur_turn;
    char	logtxt[100];
	flag=0;
	needStart=0;
	needStop=0;

	for(i=1;i<=PUMP.Field_num;i++)
    //for (j=1;j<=MAX_IRRIGATION_TURN;j++) 
	{
		if(PUMP.Field_Gate[i].Start==1)
			needStart++;
		else if(PUMP.Field_Gate[i].Stop==1)
			needStop++;
	}
	if(needStop>0)//先开下一轮灌组阀门，再关上一组阀门
	{
		if(needStart>0)
		{
			for(i=1;i<=PUMP.Field_num;i++)
				//for (j=1;j<=MAX_IRRIGATION_TURN;j++)
				{
					if(PUMP.Field_Gate[i].Stop==1)
						PUMP.Field_Gate[i].Stop=0;
				}
		}
	}
	
   for(i=1;i<=PUMP.Field_num;i++)    
    {   	
       // if(PUMP.Field_Gate[i].AP_Avail==0)continue;
		
		cur_turn = PUMP.Field_Gate[i].cur_turn;
		
		//printf("111 Fill:::PUMP.Field_Gate[%d].Status[%d]=%d\n",i,cur_turn,PUMP.Field_Gate[i].Status[cur_turn]);

		if((PUMP.Field_Gate[i].Start==1)&&(PUMP.Field_Gate[i].Status[cur_turn]==IN_STANDBY)/*&&(PUMP.Closed_Loop==1*/)
        {			
            PUMP.Field_Gate[i].Start=0;
			SendCommandToPLC(PUMP.Field_Gate[i].db_name_valve,C_GATE,0,OPEN);
            printf("No:%d Field Gate Start Irrigation!\n",i);
			PUMP.Field_Gate[i].Status[cur_turn]=IN_PUMPING;
			if(cur_turn>1)
				PUMP.Field_Gate[i].Status[cur_turn-1]= IN_STANDBY;

				printf("222 Fill:::PUMP.Field_Gate[%d].Status[%d]=%d\n",i,cur_turn,PUMP.Field_Gate[i].Status[cur_turn]);

			AIC_UpdateRTDB_IND(DB_NAME_Field_Status[i],IN_PUMPING);
			AIC_UpdateRTDB_IND(DB_NAME_Field_Control_Command[i],KJLTHM);
			printf("DB_NAME_Field_Control_Command[%d]=%s\n",i,DB_NAME_Field_Control_Command[i]);
			
			flag=1;
			sprintf(logtxt, "%d月%02d日  %02d:%02d:%02d \n",ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec );
			WriteLogFile(logtxt);
			sprintf(logtxt,"No. %d Field Gate Start Irrigation Command send out\n",i);
			WriteLogFile(logtxt);
        }
        if((PUMP.Field_Gate[i].Stop==1)&&(PUMP.Field_Gate[i].Status[cur_turn]==IN_PUMPING)/*&&(PUMP.Closed_Loop==1*/)
        {			
            PUMP.Field_Gate[i].Stop=0;
			SendCommandToPLC(PUMP.Field_Gate[i].db_name_valve,C_GATE,0,CLOSE);
            printf("No:%d Field Gate Stop Irrigation!\n",i);
			AIC_UpdateRTDB_IND(DB_NAME_Field_Control_Command[i],TJLTHM);
			PUMP.Field_Gate[i].Status[cur_turn]=IN_PUMPED;
			AIC_UpdateRTDB_IND(DB_NAME_Field_Status[i],IN_PUMPED);
			flag=1;
			sprintf(logtxt, "%d月%02d日  %02d:%02d:%02d \n",ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec );
			WriteLogFile(logtxt);
			sprintf(logtxt,"No. %d Field Gate Stop Irrigation Command send out\n",i);
			WriteLogFile(logtxt);
        }  
     
    }  
}

void IrrigationGroupFillCommandBuf()
{
    int i,m,k,flag,needStart,needStop;
    char	logtxt[100];
	int cur_turn;
	flag=0;

	needStart=0;
	needStop=0;

	for(i=1;i<=PUMP.Field_num;i++)
    for(m=1;m<=MAX_IRRIGATION_GROUP_NUMB;m++)	
	{
		if(PUMP.Field_Sys[i].Irrigation_Group[m].Start==1)
			needStart++;
		else if(PUMP.Field_Sys[i].Irrigation_Group[m].Stop==1)
			needStop++;
	}
	if(needStop>0)//先开下一轮灌组阀门，再关上一组阀门
	{
		if(needStart>0)
		{
			for(i=1;i<=PUMP.Field_num;i++)
				for(m=1;m<=MAX_IRRIGATION_GROUP_NUMB;m++)					
					{
						if(PUMP.Field_Sys[i].Irrigation_Group[m].Stop==1)
							PUMP.Field_Sys[i].Irrigation_Group[m].Stop=0;
					}
		}
	}
	
    for(i=1;i<=PUMP.Field_num;i++)    
	for(m=1;m<=MAX_IRRIGATION_GROUP_NUMB;m++)	
    {   
        //if(PUMP.Field_Sys[i].Irrigation_Group[m][n].AP_Avail==0)continue;		
		cur_turn = PUMP.Field_Sys[i].Irrigation_Group[m].cur_turn;
		//printf("%d field %d group cur_turn=%d\n",i,m,cur_turn);

		if((PUMP.Field_Sys[i].Irrigation_Group[m].Start==1)
			&&(PUMP.Field_Sys[i].Irrigation_Group[m].Status[cur_turn]==IN_STANDBY)
			/*&&(PUMP.Closed_Loop==1)*/)
        {
            PUMP.Field_Sys[i].Irrigation_Group[m].Start=0;
			for(k=1;k<=MAX_VALVE_NUM_IN_GROUP;k++)
			{
				if(strcmp(PUMP.Field_Sys[i].Irrigation_Group[m].db_name_valve[k],"")!=0)
				SendCommandToPLC(PUMP.Field_Sys[i].Irrigation_Group[m].db_name_valve[k],C_GATE,0,OPEN);
			}
            printf("Field: %d group: %d Start Irrigation!\n",i,m);
			PUMP.Field_Sys[i].Irrigation_Group[m].Status[cur_turn]=IN_PUMPING;
			if(cur_turn>1)
				PUMP.Field_Sys[i].Irrigation_Group[m].Status[cur_turn-1]=IN_STANDBY;
			//AIC_UpdateRTDB_IND(DB_NAME_Group_Status[i][m],IN_PUMPING);
			//printf("###DB_NAME_Group_Status[%d][%d]=%s\n",i,m,DB_NAME_Group_Status[i][m]);
			AIC_UpdateRTDB_IND(DB_NAME_IrrigationGroup_Control_Command[i][m],KJLTHM);
			
			flag=1;
			sprintf(logtxt, "%d月%02d日  %02d:%02d:%02d \n",ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec );
			WriteLogFile(logtxt);
			sprintf(logtxt,"No. %d field %d Group Start Irrigation Command send out\n",i,m);
			WriteLogFile(logtxt);
        }
        if((PUMP.Field_Sys[i].Irrigation_Group[m].Stop==1)
			&&(PUMP.Field_Sys[i].Irrigation_Group[m].Status[cur_turn]==IN_PUMPING)
		/*	&&(PUMP.Closed_Loop==1)*/)
        {
            PUMP.Field_Sys[i].Irrigation_Group[m].Stop=0;
			for(k=1;k<=MAX_VALVE_NUM_IN_GROUP;k++)
			{
				if(strcmp(PUMP.Field_Sys[i].Irrigation_Group[m].db_name_valve[k],"")!=0)
				SendCommandToPLC(PUMP.Field_Sys[i].Irrigation_Group[m].db_name_valve[k],C_GATE,0,CLOSE);
			}
            printf("Field: %d group: %d  Stop Irrigation!\n",i,m);
			AIC_UpdateRTDB_IND(DB_NAME_IrrigationGroup_Control_Command[i][m],TJLTHM);
			PUMP.Field_Sys[i].Irrigation_Group[m].Status[cur_turn]=IN_PUMPED;
			//AIC_UpdateRTDB_IND(DB_NAME_Group_Status[i][m],IN_PUMPED);
			flag=1;
			sprintf(logtxt, "%d月%02d日  %02d:%02d:%02d \n",ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec );
			WriteLogFile(logtxt);
			sprintf(logtxt,"No. %d field %d Group Stop Irrigation Command send out\n",i,m);
			WriteLogFile(logtxt);
        }       
    }  
}

/*int StartStopCalculation()//Irrigation_Group[i].Pumping_Time start_time
{
	int Have_pumped_time;
	int CurIndex=PUMP.Group_Start;
	int PumpingTime=Irrigation_Group[CurIndex].Pumping_Time *60*60;

	if(CurIndex<=0||CurIndex>IRRIGATION_GROUP_NUMB)
	{
		printf("no Group need Irrigation!!\n");
		return 0;
	}
	Irrigation_Group[CurIndex].tTime=mktime(&Irrigation_Group[CurIndex].start_time);
	ReadTime();

	Have_pumped_time=mktime(ntime)-Irrigation_Group[CurIndex].tTime;
	
	printf("group %d have_pumped_time=%d\n",CurIndex,mktime(ntime)-mktime(&Irrigation_Group[CurIndex].start_time));
	if(Have_pumped_time>=0)//已到启动时间
	{
	//	printf("PumpingTime=%d\n",PumpingTime);
		if(Have_pumped_time>=PumpingTime)//灌溉完成
		{			
			if(Irrigation_Group[CurIndex].Status==IN_PUMPING)
			{
				Irrigation_Group[CurIndex].Stop=1;
				PUMP.Group_Start=0;
				PUMP.Group_Stop=CurIndex;
				printf("start_stop calculation:: stop group %d\n",CurIndex);
				return 1;
			}
			else
			{
				printf("start_stop calculation:: group %d pumped\n",CurIndex);
				Irrigation_Group[CurIndex].Status=IN_PUMPED;
				printf("DB_NAME_Group_Status[%d]=%s\n",CurIndex,DB_NAME_Group_Status[CurIndex]);
				AIC_UpdateRTDB_IND(DB_NAME_Group_Status[CurIndex],IN_PUMPED);
				printf("here\n");
				return 0;
			}
		}
		else
		{
			if(Irrigation_Group[CurIndex].Status==IN_PUMPING)
			{
				Irrigation_Group[CurIndex].Pumped_time =Have_pumped_time;
				return 0;
			}
			else if(Irrigation_Group[CurIndex].Status==IN_PUMPED)
			{
				printf("have pumped\n");
				return 0;
			}
			else
			{
				Irrigation_Group[CurIndex].Pumped_time =Have_pumped_time;
				Irrigation_Group[CurIndex].Start=1;
				PUMP.Group_Stop=0;
				PUMP.Group_Start=CurIndex;
				printf("start_stop calculation:: start group %d\n",CurIndex);
				return 1;
			}
		}

	}
	else//未到启动时间
	{
		printf("here return 0\n");
		return 0;
	}	
}
*/
int GetCurStartIrrigationField()
{
	int need_startOrstop=0;
	int i,j;

	ReadTime ();

	for(i=1;i<=MAX_FIELD_NUMB;i++)
	{
		for(j=1;j<=MAX_IRRIGATION_TURN;j++)			
		if(PUMP.Field_Gate[i].start_time[j]<mktime(&(*ntime))
			&& PUMP.Field_Gate[i].end_time[j]>mktime(&(*ntime)))
		{
		//	printf("Field_Gate[%d] in irrigation time,start_time=%d,end_time=%d,ntime=%d\n",i,
			//	PUMP.Field_Gate[i].start_time,PUMP.Field_Gate[i].end_time,mktime(&(*ntime)));
			PUMP.Field_Gate[i].cur_turn = j;
			if(PUMP.Field_Gate[i].Status[j]==IN_PUMPING)
				continue;
			else
			{
				PUMP.Field_Gate[i].Start=1;
				need_startOrstop++;
				printf("!!!in GetCurStartIrrigationField:PUMP.Field_Gate[%d].Status[%d]=%d\n",i,j,PUMP.Field_Gate[i].Status[j]);
			}
			//printf("field %d cur time is in %d turn\n",i,j);
		}
		else
		{
			if(PUMP.Field_Gate[i].start_time[j]>mktime(&(*ntime)))
				continue;
			if(PUMP.Field_Gate[i].end_time[j]<mktime(&(*ntime)))//pumped
			{
				if(PUMP.Field_Gate[i].Status[j]==IN_PUMPING)
				{
					PUMP.Field_Gate[i].Stop=1;
				/*printf("Field_Gate[%d][%d] in irrigation time,start_time=%d,end_time=%d,ntime=%d\n",i,j,
						PUMP.Field_Gate[i].start_time[j],PUMP.Field_Gate[i].end_time[j],mktime(&(*ntime)));*/
					printf("!!!in GetCurStartIrrigationField:PUMP.Field_Gate[%d].Stop=1,Field_Gate[%d].Status[%d]=%d\n",i,i,j,PUMP.Field_Gate[i].Status[j]);
					need_startOrstop++;
				}
				else
					continue;
			}
		}
		
	}	
	printf("in GetCurStartIrrigationField:need_startOrstop=%d\n",need_startOrstop);
	return need_startOrstop;
}


int GetCurStartIrrigationGroup()
{
	int need_startOrstop=0;
	int i,m,n;

	ReadTime ();

	for(i=1;i<=MAX_FIELD_NUMB;i++)
	{
			for(m=1;m<=MAX_IRRIGATION_GROUP_NUMB;m++)
				for(n=1;n<=MAX_IRRIGATION_TURN;n++)
				{
					//printf("!!! field[%d].group[%d].start_time[%d]=%d,ntime=%d\n",i,m,n,PUMP.Field_Sys[i].Irrigation_Group[m].start_time[n],mktime(&(*ntime)));
					
					//printf("in GetCurStartIrrigationGroup:PUMP.Field_Sys[%d].Irrigation_Group[%d].Status[%d]=%d\n",i,m,n,PUMP.Field_Sys[i].Irrigation_Group[m].Status[n]);		
					if(PUMP.Field_Sys[i].Irrigation_Group[m].start_time[n]<mktime(&(*ntime))
						&& PUMP.Field_Sys[i].Irrigation_Group[m].end_time[n]>mktime(&(*ntime)))
					{
						PUMP.Field_Sys[i].Irrigation_Group[m].cur_turn=n;
						if(PUMP.Field_Sys[i].Irrigation_Group[m].Status[n]==IN_PUMPING)
							continue;
						else
						{
							PUMP.Field_Sys[i].Irrigation_Group[m].Start=1;
							
							printf("GetCurStartIrrigationGroup::%d %d %d need start\n",i,m,n);
							need_startOrstop++;
						}
						//printf("field %d group %d is in %d turn\n",i,m,n);
					}
					else
					{
						if(PUMP.Field_Sys[i].Irrigation_Group[m].start_time[n]>mktime(&(*ntime)))
							continue;
						if(PUMP.Field_Sys[i].Irrigation_Group[m].end_time[n]<mktime(&(*ntime)))//pumped
						{
							if(PUMP.Field_Sys[i].Irrigation_Group[m].Status[n]==IN_PUMPING)
							{
								PUMP.Field_Sys[i].Irrigation_Group[m].Stop=1;
								need_startOrstop++;
								printf("GetCurStartIrrigationGroup::%d %d %d.stop==1\n",i,m,n);
							}
							else
								continue;
						}
					}
				}
	}	
	return need_startOrstop;
	
}

void FieldGate_Supervision()
{
	int i,j;
	for(i=1;i<=FIELD_NUM;i++)
		for(j=1;j<=IRRIGATION_TURN_NUMB;j++)
	{
	   if((PUMP.Field_Gate[i].Start==1)&&(PUMP.Field_Gate[i].Status[j]==IN_PUMPING))
		{
	    	PUMP.Field_Gate[i].Start=0;
			AIC_UpdateRTDB_IND(DB_NAME_Field_Control_Command[i],WCZTHM);
			printf("field_gate %d NO operation\n",i);
		}
		if((PUMP.Field_Gate[i].Stop==1)&&(PUMP.Field_Gate[i].Status[j]!=IN_PUMPING))
		{
			PUMP.Field_Gate[i].Stop=0;
			AIC_UpdateRTDB_IND(DB_NAME_Field_Control_Command[i],WCZTHM);
			printf("field_gate %d no operation,Field_Gate[%d].Status[%d]=%d\n",i,i,j,PUMP.Field_Gate[i].Status[j]);
		}
//		printf("Irrigation_Group[%d].Status[%d]=%d\n",i,Irrigation_Group[i].Status[n]);
	}
}

void IrrigationGroup_Supervision()
{
	int i,m,n;
	for(i=1;i<=FIELD_NUM;i++)		
			for(m=1;m<=IRRIGATION_GROUP_NUMB;m++)	
				for(n=1;n<=MAX_IRRIGATION_TURN;n++)
				{
					if((PUMP.Field_Sys[i].Irrigation_Group[m].Start==1)
						&&(PUMP.Field_Sys[i].Irrigation_Group[m].Status[n]==IN_PUMPING))
					{		   
							PUMP.Field_Sys[i].Irrigation_Group[m].Start=0;
							AIC_UpdateRTDB_IND(DB_NAME_IrrigationGroup_Control_Command[i][m],WCZTHM);
		   
					}
					if((PUMP.Field_Sys[i].Irrigation_Group[m].Stop==1)
						&&(PUMP.Field_Sys[i].Irrigation_Group[m].Status[n]!=IN_PUMPING))
					{
						PUMP.Field_Sys[i].Irrigation_Group[m].Stop=0;
						AIC_UpdateRTDB_IND(DB_NAME_IrrigationGroup_Control_Command[i][m],WCZTHM);	
					}
				}
}

void IrrigationGroupModule()
{
	int need_start_stop=0;

	LoadGroupTimeFromFile();

 	need_start_stop=GetCurStartIrrigationGroup();//得到当前需灌溉的田间系统号

printf("in IrrigationGroupModule::need_start_stop=%d\n",need_start_stop);

	if(need_start_stop>0)
	{
	//	StartStopModule();//启动该轮灌组的开阀、关阀模块（CHL）
		IrrigationGroup_Supervision();//复归开启、关闭命令

		IrrigationGroupFillCommandBuf();//发出命令
	}	
}


void FieldSysIrrigationModule()
{
	int need_start_stop=0;

	LoadFieldIrrigationTime();//correct

	//printf("here is FieldSysIrrigationModule\n");

 	need_start_stop=GetCurStartIrrigationField();//得到当前需灌溉的田间系统号
//printf("FieldSysIrrigationModule:need_start_stop=%d\n",need_start_stop);
	if(need_start_stop>0)	//correct
	{
	//	StartStopModule();//启动该轮灌组的开阀、关阀模块（CHL）
		FieldGate_Supervision();//确认该田间系统目前的工作状态，然后决定是否发令

		FieldGateFillCommandBuf();//发出命令
	}
	
	
}

void AIC_Module()
{		
	FieldSysIrrigationModule();	

	IrrigationGroupModule();
	
}

void InitData()
{
	int i,j,m,n;

	for(i=1;i<=MAX_FIELD_NUMB;i++)
	{
		PUMP.Field_Gate[i].Start=0;
		PUMP.Field_Gate[i].Stop=0;
		PUMP.Field_Gate[i].cur_turn=0;
		PUMP.Field_Gate[i].AP_Avail=0;
		for(j=1;j<=MAX_IRRIGATION_TURN;j++)
		{		
			PUMP.Field_Gate[i].Pumped_time[j]=0;
			PUMP.Field_Gate[i].Pumping_Time[j]=0;
			PUMP.Field_Gate[i].Status[j]=0;
		}
	}
	for(i=1;i<=MAX_FIELD_NUMB;i++)
	for(m=1;m<=MAX_IRRIGATION_GROUP_NUMB;m++)		
	{
		PUMP.Field_Sys[i].Irrigation_Group[m].Start=0;
		PUMP.Field_Sys[i].Irrigation_Group[m].Stop=0;		
		PUMP.Field_Sys[i].Irrigation_Group[m].AP_Avail=0;
		PUMP.Field_Sys[i].Irrigation_Group[m].cur_turn=0;
		for(n=1;n<=MAX_IRRIGATION_TURN;n++)
		{
			PUMP.Field_Sys[i].Irrigation_Group[m].Status[n]=0;	
			PUMP.Field_Sys[i].Irrigation_Group[m].Pumped_time[n]=0;
			PUMP.Field_Sys[i].Irrigation_Group[m].Pumping_Time[n]=0;
		}	
	}
	printf("InitData end\n");
}

void main()
{
	int sbo_flag;
	char sbo_db_name[80];
#ifdef WINDOWS_OS
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 	                               
		return;	
#endif
	
	gethostname(hostname,20);
    //printf("hostname =%s\n",hostname);
	SetModuleName("AIC");
    CreateLockFile(".AIC.lock");
    signal(SIGINT,SIGINT_handler);
    if(-1==MountDB(DATABASE_NAME))
    {
		Err_log("Fail to MountDB!\n");
		exit(-1);
    } 
  	 
    if(!lan_out_init())
    {
		Err_log("Fail to init lan module!\n");
		exit(-1);
    }
	ReadAICDefFile();
	printf("ReadAICDefFile end!!\n");
	GetFieldValvePt();
	GetGroupValvePt();
//	printf("###DB_NAME_AIC_Period=%s\n",DB_NAME_AIC_Period);

	InitData();  	

	signal( SIGINT, SIGINT_handler);
	
	if ( InterLockFileLoad () == -1 )
	{
		printf ( "in AIC: Load InterLock.dat error!\n" );
		exit ( 0 );
	}

	ReadTime ();

	printf("ReadTime end\n");

    oldday = ntime->tm_mday;
	
	sprintf(logfilename, "/home/ems/h9000/his/AIC/AICcmd.%02d",ntime->tm_mday);
	//printf("before InitialAIC_Data_On_Screen end,logfilename=%s\n",logfilename);
	InitialAIC_Data_On_Screen ();


	for (;;)
	{
		//printf("here is for loop\n");
		ReadTime ();//读当前时间
		oldmonth=ntime->tm_mon+1;
		oldday = ntime->tm_mday;
		oldhour= ntime->tm_hour;
		oldmin= ntime->tm_min;
		

        LogFileChangeDayAndWriteTime();
//printf("here is ReadAIC_DataFromRTDB\n");
		ReadAIC_DataFromRTDB();
		
		strcpy ( sbo_db_name, DB_NAME_QC_AIC );
        strcat ( sbo_db_name, "_YES.." );//执行/挂起
	//	printf("sbo_db_name=%s\n",sbo_db_name);

        sbo_flag = InterLock_Judge ( sbo_db_name );
		if(sbo_flag==1)
		{
			printf("AIC_Loop=%d,AIC_Period=%d\n",AIC_Loop,AIC_Period);
			if(AIC_Loop>=AIC_Period)
			{
				AIC_Loop=0;
				AIC_Module();
			}
		}
		else     
		{
            InitialAIC_Data_On_Screen ();	
			printf("qc_aic 挂起\n");
		}
		
		AIC_Loop++;
printf("AIC_Loop=%d\n",AIC_Loop);

		if(Number_ANA>0)
        {
			//if(GetControlState()==1)
				lan_out ((char *)&ml_buf_ANA,lanhd_ANA,(unsigned char)0);
			Number_ANA=0;
        }
        if(Number_IND>0)
        {
			//if(GetControlState()==1)
				lan_out ((char *)&ml_buf_IND,lanhd_IND,(unsigned char)0);
			Number_IND=0;
		}
		
#ifdef SUN
		sleep(AIC_Period);
#else
		Sleep(AIC_Period*1000);
#endif
	}
}
