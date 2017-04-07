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
	 sprintf(err_str,"No this point %s\n",PointName);
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
	 sprintf(err_str,"No this point %s\n",PointName);
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
	  sprintf(err_str,"No this point %s\n",PointName);
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
	  sprintf(err_str,"No this point %s\n",PointName);
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
        if(GetControlState()==1)
          lan_out ((char *)&ml_buf_IND,lanhd_IND,(unsigned char)0);
         Number_IND=0;
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
	printf("logfilename=%s\n",logfilename);
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

/*void PowerCurve_Module()
{
    int i;
    float Value[48];
    USHORT Point_Num,Point_Num1;
    CURVE_DATA curve;
    time_t tim;
    struct tm tmpt;
    DMS_COMMON common_aa;
    char ErrSms[100];
   
    tim=time (NULL);
    tmpt=*localtime (&tim);
    
    if((tmpt.tm_hour<23)||(tmpt.tm_min<59)||(tmpt.tm_sec<55))Move_Curve=0;
    if((tmpt.tm_hour==23)&&(tmpt.tm_min==59)&&(tmpt.tm_sec>55)&&(Move_Curve==0))
    { 
        GetCurvePtSegNumByNameStr( DB_NAME_MRFHQX, &Point_Num1);
        GetCurvePtDataByNameStr(DB_NAME_MRFHQX,&curve);
       
		if(ReadEntryByNameStr(DB_NAME_JRFHQX,&common_aa)==-1)
		{
           printf("No point %s in DB!\n",DB_NAME_JRFHQX);
           sprintf(ErrSms,"No point %s in DB!",DB_NAME_JRFHQX);
           Err_log(ErrSms);
         }
        for(i=0;i<48;i++){Value[i]=curve.value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value);
       
        for(i=48;i<96;i++){Value[i-48]=curve.value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value);
         
        Move_Curve=1;
    }
    //printf("Current Time:%d:%d:%d,%d :%d : %d\n",tmpt.tm_year+1900,tmpt.tm_mon+1,tmpt.tm_mday,tmpt.tm_hour,tmpt.tm_min,tmpt.tm_sec);
    GetCurvePtSegNumByNameStr( DB_NAME_JRFHQX, &Point_Num);
    GetCurvePtDataByNameStr(DB_NAME_JRFHQX,&curve);
    PUMP.CurvePointNum=Point_Num;
    for(i=0;i<Point_Num;i++)
        PUMP.Curve[i]=(int)curve.value[i];
}
*/
/*void Statistics_Unit()	
{
	int 	i,j;
	char	sbo_db_name[80];
	int		sbo_flag;	
	

	for (i=1; i<=IRRIGATION_GROUP_NUMB;i++)
	{		
		strcpy ( sbo_db_name, DB_NAME_AP_CZKT[ i ] );//该轮灌组正常参与轮灌
		//strcat ( sbo_db_name, "_YES.." );
	//	sbo_flag = InterLock_Judge ( sbo_db_name );	
		sbo_flag= ReadDB_IND(DB_NAME_AP_CZKT[ i ]);
		
		if(sbo_flag==1)
        {
            Irrigation_Group[i].AP_Avail = 1;               
		}
		else
		{
			Irrigation_Group[i].AP_Avail = 0; 
			for(j=1;j<=MAX_IRRIGATION_TURN;j++)
			{
				Irrigation_Group[i].Start[j]=0;
				Irrigation_Group[i].Stop[j]=0;
			}
		}
		strcpy ( sbo_db_name, DB_NAME_AP_CZKT[ i ] );
		strcat ( sbo_db_name, "_NO.." );
		sbo_flag = InterLock_Judge ( sbo_db_name );
		if(sbo_flag==1)
		{
			Irrigation_Group[i].AP_Avail=0;
			AIC_UpdateRTDB_IND(DB_NAME_AP_Group[i],0);			
		 }
		AIC_UpdateRTDB_IND(DB_NAME_AP_CZKT[i],(short)Irrigation_Group[i].AP_Avail);
	}  	
}
*/

void InitialAIC_Data_On_Screen ()
{
	int i;
	
	for ( i= 1; i<= IRRIGATION_GROUP_NUMB; i++ )  	
		AIC_UpdateRTDB_IND(DB_NAME_Control_Command[i],WCZTHM);	
}

void ReadAICDefFile()
{
	FILE	*fp;
	char filename[100]="\\home\\ems\\h9000\\AIC\\AIC.DEF";
	char keyword[30]="";
	char StrTemp[50]="",StrTemp1[50]="";
	int i;
	char err_str[80]="";

	if( (fp=fopen(filename, "r"))==NULL )
	{
		printf("Fail to open %s\n", filename);
		UnMount_handle();
	}

	strcpy(keyword,"FieldNumber");
	
	if(-1==LocateKeyword(fp,keyword))
	{
		printf("filename=%s,%s not exist\n",filename,keyword);
		UnMount_handle();
	}
	printf("here\n");
	fscanf(fp,"%d\n",&Field_Num); 
	
	if((Field_Num>MAX_FIELD_NUMB)||(Field_Num<=0))
	{
		printf("田间数定义错误!\n");
		UnMount_handle();
	}
#ifdef DEBUG
	printf("Field_Num=%d\n",Field_Num);
#endif   
	PUMP.Field_num=Field_Num;

    strcpy(keyword,"IrrigationGroupNumber");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",StrTemp);
    IRRIGATION_GROUP_NUMB=atoi(StrTemp);
	if((IRRIGATION_GROUP_NUMB>MAX_IRRIGATION_GROUP_NUMB)||(IRRIGATION_GROUP_NUMB<=0))
	{
		printf("轮灌组数定义错误!\n");
		UnMount_handle();
	}
#ifdef DEBUG
	printf("IRRIGATION_GROUP_NUMB=%d\n",IRRIGATION_GROUP_NUMB);
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
	
	strcpy(keyword,"Control_Command");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Control_Command[i]);
		if(ReadEntryByNameStr(DB_NAME_Control_Command[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Control_Command[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Control_Command[i]);
			BdSms(err_str);
			exit(0);
		}
#ifdef DEBUG
		printf("DB_NAME_Control_Command[%d]=%s\n",i,DB_NAME_Control_Command[i]);
#endif
	}

		
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

	strcpy(keyword,"AP_Group");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AP_Group[i]);
		if(ReadEntryByNameStr(DB_NAME_AP_Group[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_AP_Group[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_Group[i]);
			BdSms(err_str);
			exit(0);
		}
#ifdef DEBUG
		printf("DB_NAME_AP_Group[%d]=%s\n",i,DB_NAME_AP_Group[i]);
#endif
	} 

	strcpy(keyword,"Group_STATUS");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Group_Status[i]);
		if(ReadEntryByNameStr(DB_NAME_Group_Status[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Group_Status[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Group_Status[i]);
			BdSms(err_str);
			exit(0);
		}
#ifdef DEBUG
		printf("DB_NAME_Group_Status[%d]=%s\n",i,DB_NAME_Group_Status[i]);
#endif
	} 
	
	strcpy(keyword,"AP_CZKT");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AP_CZKT[i]);
		if(ReadEntryByNameStr(DB_NAME_AP_CZKT[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_AP_CZKT[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_CZKT[i]);
			BdSms(err_str);
			exit(0);
		}
#ifdef DEBUG
		printf("DB_NAME_AP_CZKT[%d]=%s\n",i,DB_NAME_AP_CZKT[i]);
#endif
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
	
/*	strcpy(keyword,"Prvilege");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Prvilege[i]);
		if(ReadEntryByNameStr(DB_NAME_Prvilege[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Prvilege[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Prvilege[i]);
			BdSms(err_str);
			exit(0);
		}
#ifdef DEBUG
		printf("DB_NAME_Prvilege[%d]=%s\n",i,DB_NAME_Prvilege[i]);
#endif
	} 

	strcpy(keyword,"JRFHQX");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_JRFHQX);
	if(ReadEntryByNameStr(DB_NAME_JRFHQX,&common_aa)==-1)
	{
		printf("No this point %s\n",DB_NAME_JRFHQX);
		sprintf(err_str,"No this point %s\n",DB_NAME_JRFHQX);
		BdSms(err_str);
	//	exit(0);
	}
#ifdef DEBUG
	printf("DB_NAME_JRFHQX=%s\n",DB_NAME_JRFHQX);
#endif
	
	strcpy(keyword,"MRFHQX");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_MRFHQX);
	if(ReadEntryByNameStr(DB_NAME_MRFHQX,&common_aa)==-1)
	{
		printf("No this point %s\n",DB_NAME_MRFHQX);
		sprintf(err_str,"No this point %s\n",DB_NAME_MRFHQX);
	//	BdSms(err_str);
		exit(0);
	}
#ifdef DEBUG
	printf("DB_NAME_MRFHQX=%s\n",DB_NAME_MRFHQX);
#endif
*/
	strcpy(keyword,"STARTTIME");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)
	{
		/*fscanf(fp,"%s\n",DB_NAME_StartTime[i]);
		if(ReadEntryByNameStr(DB_NAME_StartTime[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_StartTime[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_StartTime[i]);
			BdSms(err_str);
			exit(0);
		}*/
		
		fscanf(fp,"%s\n",DB_NAME_Group_StartTime_Mon[i]);
#ifdef DEBUG
		printf("DB_NAME_Group_StartTime_Mon[%d]=%s\n",i,DB_NAME_Group_StartTime_Mon[i]);
#endif
		if(ReadEntryByNameStr(DB_NAME_Group_StartTime_Mon[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Group_StartTime_Mon[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Group_StartTime_Mon[i]);
			BdSms(err_str);
			exit(0);
		}
		fscanf(fp,"%s\n",DB_NAME_Group_StartTime_Day[i]);
		if(ReadEntryByNameStr(DB_NAME_Group_StartTime_Day[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Group_StartTime_Day[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Group_StartTime_Day[i]);
			BdSms(err_str);
			exit(0);
		}
		fscanf(fp,"%s\n",DB_NAME_Group_StartTime_Hour[i]);
		if(ReadEntryByNameStr(DB_NAME_Group_StartTime_Hour[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Group_StartTime_Hour[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Group_StartTime_Hour[i]);
			BdSms(err_str);
			exit(0);
		}
		fscanf(fp,"%s\n",DB_NAME_Group_StartTime_Min[i]);
		if(ReadEntryByNameStr(DB_NAME_Group_StartTime_Min[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Group_StartTime_Min[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Group_StartTime_Min[i]);
			BdSms(err_str);
			exit(0);
		}
#ifdef DEBUG
		//printf("DB_NAME_StartTime[%d]=%s\n",i,DB_NAME_StartTime[i]);
#endif
	} 

		strcpy(keyword,"PUMPTIME");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Group_PumpTime[i]);
		if(ReadEntryByNameStr(DB_NAME_Group_PumpTime[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Group_PumpTime[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Group_PumpTime[i]);
			BdSms(err_str);
			exit(0);
		}
#ifdef DEBUG
		printf("DB_NAME_Group_PumpTime[%d]=%s\n",i,DB_NAME_Group_PumpTime[i]);
#endif
	} 
	

	fclose(fp);
}

void LoadFieldIrrigationTime()
{
	FILE	*fp;
	char filename[100]="\\home\\ems\\h9000\\AIC\\Field_Irrigation_Time.DEF";
	char keyword[30]="";
	char StartTime[50]="",EndTime[50]="";//,StrTemp[100];	
	char time_str[80]="";
	int i,j=0,Field_Num,irrigation_hour;
	char Irrigation_time[50];
//	char *p;
	char *delim="-";
	char *Mon=".",start_mon,start_day,end_mon,end_day,start_hour,start_min,end_hour,end_min;
//	char day[5];

	if( (fp=fopen(filename, "r"))==NULL )
	{
		printf("Fail to open %s\n", filename);
		//UnMount_handle();
		return;
	}
	strcpy(keyword,"Begin");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=MAX_FIELD_NUMB;i++)
		for(j=1;j<=MAX_IRRIGATION_TURN;j++)
	{
		fscanf(fp,"%d",&Field_Num);
		if(Field_Num<1||Field_Num>MAX_FIELD_NUMB)
			printf("Field_Num is wrong!\n");
		fscanf(fp,"%d.%d %d:%d-%d.%d %d:%d",
			&start_mon,&start_day,&start_hour,&start_min,
			&end_mon,&end_day,&end_hour,&end_min);
		//StringtoTime(char *str,struct tm start_time,struct tm end_time);
		PUMP.Field_Sys[i][j].start_time.tm_year = ntime->tm_year-1900;
		PUMP.Field_Sys[i][j].start_time.tm_mon = start_mon;
		PUMP.Field_Sys[i][j].start_time.tm_mday = start_day;
		PUMP.Field_Sys[i][j].start_time.tm_hour = start_hour;
		PUMP.Field_Sys[i][j].start_time.tm_min = start_min;
		PUMP.Field_Sys[i][j].start_time.tm_sec = 0;
		PUMP.Field_Sys[i][j].start_time.tm_isdst = 0;

		PUMP.Field_Sys[i][j].end_time.tm_year = ntime->tm_year-1900;
		PUMP.Field_Sys[i][j].end_time.tm_mon = end_mon;
		PUMP.Field_Sys[i][j].end_time.tm_mday = end_day;
		PUMP.Field_Sys[i][j].end_time.tm_hour = end_hour;
		PUMP.Field_Sys[i][j].end_time.tm_min = end_min;


		AIC_UpdateRTDB_ANA( DB_NAME_Field_StartTime_Mon[i][j], PUMP.Field_Sys[i][j].start_time.tm_mon);
		AIC_UpdateRTDB_ANA(DB_NAME_Field_StartTime_Day[i][j],PUMP.Field_Sys[i][j].start_time.tm_mday);
		AIC_UpdateRTDB_ANA(DB_NAME_Field_StartTime_Hour[i][j],PUMP.Field_Sys[i][j].start_time.tm_hour);
		AIC_UpdateRTDB_ANA(DB_NAME_Field_StartTime_Min[i][j],PUMP.Field_Sys[i][j].start_time.tm_min);
		AIC_UpdateRTDB_ANA(DB_NAME_Field_PumpTime[i][j],PUMP.Field_Sys[i][j].Pumping_Time);		
		
			//printf("Irrigation_Group[%d].Pumping_Time=%d\n",i,Irrigation_Group[i].Pumping_Time);
//#ifdef DEBUG
//	printf("group=%d,time=%s,Irrigation_time=%d\n",group,StrTemp,Irrigation_time);
//#endif 
			
	}
	
    
}

void LoadGroupTimeFromFile()
{
	FILE	*fp;
	char filename[100]="\\home\\ems\\h9000\\AIC\\AIC_TIME.DEF";
	char keyword[30]="";
	char StartTime[50]="",EndTime[50]="",StrTemp[100];	
	char time_str[80]="";
	int j,group,field;
	char Irrigation_time[50];
	char *p;
	char *delim="-";
	char *Mon=".",*start_mon,*start_day,*end_date,*start_time,*start_hour,*start_min,*end_hour,*end_min;


	if( (fp=fopen(filename, "r"))==NULL )
	{
		printf("Fail to open %s\n", filename);
		//UnMount_handle();
		return;
	}
	strcpy(keyword,"Begin");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
//	for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)
	j=1;
	for(;;)
	{
		if(feof(fp))
			break;	

		fscanf(fp,"%d",&field);
		if(field<1||field>MAX_FIELD_NUMB)
			printf("field is wrong!\n");

		fscanf(fp,"%d",&group);
		if(group<1||group>IRRIGATION_GROUP_NUMB)
			printf("group is wrong!\n");

		fscanf(fp,"%s",StrTemp);//start_date
	//	printf("start_date=%s ",StrTemp);
		start_mon=strtok(StrTemp,".");
		printf("start_mon=%s",start_mon);

		while((start_day=strtok(NULL,".")))
			printf(" start_day=%s ",start_day);
	
		fscanf(fp,"%s",Irrigation_time);
		start_time=strtok(Irrigation_time,"-");		

		while((p=strtok(NULL,"-")))
		{			
			end_date=strtok(p,".");
		//	printf("end_mon=%s ",end_date);
			while((p=strtok(NULL,".")))
				printf(" end_day=%s\n",p);

		/*	while((p=strtok(NULL,".")))
				printf("day=%s ",p);*/

			start_hour=strtok(start_time,":");
			//printf("start_hour=%s ",start_time);

			while(start_min=strtok(NULL,":"))
				printf("start_min=%s\n",start_min);

		}
		fscanf(fp,"%s",EndTime);
		end_hour=strtok(EndTime,":");
		//printf("end_hour=%s ",end_hour);
		while(end_min=strtok(NULL,":"))
			printf("end_min=%s\n",end_min);
	
			printf("\n");

		//	for(j=1;j<=MAX_IRRIGATION_TURN;j++)
			{
				PUMP.Field_Sys[field].Irrigation_Group[group].start_time[j].tm_mon=start_mon-1;
				PUMP.Field_Sys[field].Irrigation_Group[group].start_time[j].tm_mday=start_day;
				PUMP.Field_Sys[field].Irrigation_Group[group].start_time[j].tm_hour=start_hour;
				PUMP.Field_Sys[field].Irrigation_Group[group].start_time[j].tm_min=start_min;
			//	Irrigation_Group[field].Pumping_Time[group]=irrigation_hour;

				AIC_UpdateRTDB_ANA( DB_NAME_Group_StartTime_Mon[field][group], PUMP.Field_Sys[field].Irrigation_Group[group].start_time[j].tm_mon);
				AIC_UpdateRTDB_ANA(DB_NAME_Group_StartTime_Day[field][group],PUMP.Field_Sys[field].Irrigation_Group[group].start_time[j].tm_mday);
				AIC_UpdateRTDB_ANA(DB_NAME_Group_StartTime_Hour[field][group],PUMP.Field_Sys[field].Irrigation_Group[group].start_time[j].tm_hour);
				AIC_UpdateRTDB_ANA(DB_NAME_Group_StartTime_Min[field][group],PUMP.Field_Sys[field].Irrigation_Group[group].start_time[j].tm_min);
		j++;
		
				//	AIC_UpdateRTDB_ANA(DB_NAME_Group_PumpTime[field],Irrigation_Group[field].Pumping_Time[group]);
				
			//printf("Irrigation_Group[%d].Pumping_Time=%d\n",i,Irrigation_Group[i].Pumping_Time);
//#ifdef DEBUG
//	printf("group=%d,time=%s,Irrigation_time=%d\n",group,StrTemp,Irrigation_time);
//#endif 
			}
	}
	
    
  
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
    int i1,j1,i2,j2;	
	int sbo_flag;
	char sbo_db_name[80];
	struct tm StartTime;
    
//	PowerCurve_Module();


	strcpy ( sbo_db_name, DB_NAME_QC_AIC );
    strcat ( sbo_db_name, "_NO.." );
    sbo_flag = InterLock_Judge ( sbo_db_name );
//	printf("!!!!\n");
	if(sbo_flag==1)
	{
		AIC_UpdateRTDB_IND ( DB_NAME_QC_AIC, 0);	
	}

	AIC_Period=ReadDB_ANA(DB_NAME_AIC_Period);
//	printf("Read AIC_Period=%d,DB_NAME_AIC_Period=%s\n",AIC_Period,DB_NAME_AIC_Period);

	if(AIC_Period<1)
	{
		AIC_Period=1;
		AIC_UpdateRTDB_ANA( DB_NAME_AIC_Period, (float)AIC_Period);
	}


/*	PUMP.Control_Right = ReadDB_IND (DB_NAME_Control_Right);
	printf("in ReadAIC_DataFromRTDB\n");*/
    PUMP.Closed_Loop = ReadDB_IND ( DB_NAME_AIC_CLOSED_LOOP );
	 

    PUMP.AIC_state=ReadDB_IND ( DB_NAME_AIC_STATE );//   执行/挂起

	if(PUMP.AIC_state==0)
	{
		PUMP.Closed_Loop=0;
		
		AIC_UpdateRTDB_IND(DB_NAME_AIC_CLOSED_LOOP,0);
		
	//	AIC_UpdateRTDB_IND(DB_NAME_Control_Right,0);
		
	/*	for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)
		{
				//printf("DB_NAME_Control_Right=%s\n",DB_NAME_AP_Group[i]);
			AIC_UpdateRTDB_IND(DB_NAME_AP_Group[i], 0);
		}*/
	}
for(i1=1;i1<=PUMP.Field_num;i1++)
{
	for(i2=1;i2<=MAX_IRRIGATION_TURN;i2++)
	{
		for(j1=1;j1<=IRRIGATION_GROUP_NUMB;j1++)
			for(j2=1;j2<=MAX_IRRIGATION_TURN;j2++)
		{		
			PUMP.Field_Sys[i1][i2].Irrigation_Group[j1][j2].Status = ReadDB_IND(DB_NAME_Group_Status[i]);
			StartTime.tm_year=ntime->tm_year;
			StartTime.tm_mon=ReadDB_ANA(DB_NAME_Group_StartTime_Mon[i][j])-1;
		
			StartTime.tm_mday=ReadDB_ANA(DB_NAME_Group_StartTime_Day[i][j]);
			StartTime.tm_hour=ReadDB_ANA(DB_NAME_Group_StartTime_Hour[i][j]);
			StartTime.tm_min=ReadDB_ANA(DB_NAME_Group_StartTime_Min[i][j]);
			StartTime.tm_sec=0;
		
			PUMP.Field_Sys[j].Irrigation_Group[i].tTime[j]=mktime(&StartTime);
			
			PUMP.Field_Sys[j].Irrigation_Group[i].start_time[j]=StartTime;
			if(PUMP.Field_Sys[j].Irrigation_Group[i].AP_Avail==0)
			{
				AIC_UpdateRTDB_IND( DB_NAME_Control_Command[i], WCZTHM);		
			}
		}
	}
}
//	Statistics_Unit();

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

void FillCommandBuf()
{
    int i,j,flag;
    char	logtxt[100];
	int cur_turn;
	flag=0;

   for(j=1;j<=PUMP.Field_num;j++)
    for (i=1;i<=IRRIGATION_GROUP_NUMB;i++) 
    {   
        if(PUMP.Field_Sys[j].Irrigation_Group[i].AP_Avail==0)continue;
		/*if(i==1)
		printf("group %d status=%d,Closed_Loop=%d,start=%d,stop=%d\n",
			i,Irrigation_Group[i].Status,PUMP.Closed_Loop,Irrigation_Group[i].Start,Irrigation_Group[i].Stop);
       */
		cur_turn = PUMP.Field_Sys[j].Irrigation_Group[i].Cur_Turn;
		if((PUMP.Field_Sys[j].Irrigation_Group[i].Start[cur_turn]==1)&&(PUMP.Field_Sys[j].Irrigation_Group[i].Status==IN_STANDBY)&&(PUMP.Closed_Loop==1)/*&&(Irrigation_Group[i].StartAction==0)*/)
        {
            PUMP.Field_Sys[j].Irrigation_Group[i].Start[cur_turn]=0;
			SendCommandToPLC(DB_NAME_Rt_Status[i],C_UNIT,0,KJML);
            printf("No:%d group Start Irrigation!\n",i);
			PUMP.Field_Sys[j].Irrigation_Group[i].Status=IN_PUMPING;
			AIC_UpdateRTDB_IND(DB_NAME_Group_Status[i],IN_PUMPING);
			AIC_UpdateRTDB_IND(DB_NAME_Control_Command[i],KJLTHM);
			
			flag=1;
			sprintf(logtxt, "%d月%02d日  %02d:%02d:%02d \n",ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec );
			WriteLogFile(logtxt);
			sprintf(logtxt,"No.Group %d Start Irrigation Command send out\n",i);
			WriteLogFile(logtxt);
        }
        if((PUMP.Field_Sys[j].Irrigation_Group[i].Stop[cur_turn]==1)&&(PUMP.Field_Sys[j].Irrigation_Group[i].Status==IN_PUMPING)&&(PUMP.Closed_Loop==1)/*&&(Irrigation_Group[i].StopAction==0)*/)
        {
            PUMP.Field_Sys[j].Irrigation_Group[i].Stop[cur_turn]=0;
			SendCommandToPLC(DB_NAME_Rt_Status[i],C_UNIT,0,TJML);
            printf("No:%d Group Stop Irrigation!\n",i);
			AIC_UpdateRTDB_IND(DB_NAME_Control_Command[i],TJLTHM);
			PUMP.Field_Sys[j].Irrigation_Group[i].Status=IN_PUMPED;
			AIC_UpdateRTDB_IND(DB_NAME_Group_Status[i],IN_PUMPED);
			flag=1;
			sprintf(logtxt, "%d月%02d日  %02d:%02d:%02d \n",ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec );
			WriteLogFile(logtxt);
			sprintf(logtxt,"No.Group %d Stop Irrigation Command send out\n",i);
			WriteLogFile(logtxt);
        }   
     
    }  
/*	if(flag==1)
	{
		for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)
		{
			sprintf(logtxt,"Unit No.%d Status=%d,AP_Avail=%d\n",
				i,Irrigation_Group[i].Status,Irrigation_Group[i].AP_Avail);
			WriteLogFile(logtxt);
		}
		sprintf(logtxt,"PUMP.Mode=%d PUMP.Control_Right=%d\n\n\n",PUMP.Mode,PUMP.Control_Right);
		WriteLogFile(logtxt);
	}*/
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
//	int Index;
//	short Point_Num,FHQX_INTERVAL;
/*	GetCurvePtSegNumByNameStr( DB_NAME_JRFHQX, &Point_Num);
	FHQX_INTERVAL=24*60/Point_Num;
	Index=(ntime->tm_hour*60+ntime->tm_min)/FHQX_INTERVAL;
	PUMP.IrrigationGroup=PUMP.Curve[Index];
	if(Irrigation_Group[Index].Status!=IN_PUMPING)
		Irrigation_Group[Index].Status=IN_PUMPING;
	return PUMP.IrrigationGroup;*/
	return 0;
}


int GetCurStartIrrigationGroup()
{
	int Index;
	short Point_Num,FHQX_INTERVAL;
	GetCurvePtSegNumByNameStr( DB_NAME_JRFHQX, &Point_Num);
	FHQX_INTERVAL=24*60/Point_Num;
	Index=(ntime->tm_hour*60+ntime->tm_min)/FHQX_INTERVAL;
/*	PUMP.IrrigationGroup=PUMP.Curve[Index];
	if(Irrigation_Group[Index].Status!=IN_PUMPING)
		Irrigation_Group[Index].Status=IN_PUMPING;*/
	return 0;
}


int GetCurStopIrrigationGroup()
{
	int Index;//,setting_time;
	short Point_Num,FHQX_INTERVAL;

	GetCurvePtSegNumByNameStr( DB_NAME_JRFHQX, &Point_Num);
	FHQX_INTERVAL=24*60/Point_Num;
	Index=(ntime->tm_hour*60+ntime->tm_min)/FHQX_INTERVAL;
//	PUMP.IrrigationGroup=PUMP.Curve[Index];
//	setting_time= ReadDB_ANA(DB_NAME_Group_PumpTime[PUMP.IrrigationGroup]);
/*	Irrigation_Group[Index].Pumped_time=Irrigation_Group[Index].Pumped_time+AIC_Period;
	if(Irrigation_Group[Index].Pumped_time>=setting_time)
	{
		Irrigation_Group[Index].Status=IN_PUMPED;
		return PUMP.IrrigationGroup;
	}
	else*/
		return 0;
}


void Unit_Supervision()
{
	int i;
	for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)
	{
	  /*  if((Irrigation_Group[i].Start==1)&&(Irrigation_Group[i].Status==IN_PUMPING))
		{
	    	Irrigation_Group[i].Start=0;
			AIC_UpdateRTDB_IND(DB_NAME_Control_Command[i],WCZTHM);
		}
		if((Irrigation_Group[i].Stop==1)&&(Irrigation_Group[i].Status!=IN_PUMPING))
		{
			Irrigation_Group[i].Stop=0;
			AIC_UpdateRTDB_IND(DB_NAME_Control_Command[i],WCZTHM);	
		}*/
//		printf("Irrigation_Group[%d].Status=%d\n",i,Irrigation_Group[i].Status);
	}
}

void FieldSysIrrigationModule()
{
//	GetPumpGroupByPrivilege();//根据优先权得到轮灌组
 /* GetCurStartIrrigationGroup();//得到当前需灌溉的组号
	GetCurStopIrrigationGroup();//得到当前需停止灌溉的组号
*/
	GetCurStartIrrigationField();//得到当前需灌溉的组号
//	GetCurStopIrrigationField();
	printf("here is AIC_Module\n");
	/*if(StartStopCalculation()==1)
	{
		StartStopModule();//启动该轮灌组的开阀、关阀模块（CHL）
		Unit_Supervision();//确认该组目前的工作状态，然后决定是否发令

		FillCommandBuf();//发出命令
	}*/
}

void AIC_Module()
{	
	int i;

	LoadFieldIrrigationTime();
	printf("###PUMP.Field_num=%d\n",PUMP.Field_num);
	for(i=1;i<=MAX_FIELD_NUMB;i++)
	{
		FieldSysIrrigationModule();
	}
	
}

void InitData()
{
	int i;

	for(i=1;i<=IRRIGATION_GROUP_NUMB;i++)
	{
		//Irrigation_Group[i].StartAction=0;
	//	Irrigation_Group[i].StopAction=0;
/*		Irrigation_Group[i].Pumped_time=0;
		Irrigation_Group[i].Start=0;
		Irrigation_Group[i].Stop=0;*/
	}
}

void main()
{
//	int sbo_flag;
//	char sbo_db_name[80];
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
    printf("hostname =%s\n",hostname);
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
	//LoadGroupTimeFromFile();
	InitData();  	

	signal( SIGINT, SIGINT_handler);
	if ( InterLockFileLoad () == -1 )
	{
		printf ( "in AIC: Load InterLock.dat error!\n" );
		exit ( 0 );
	}

	ReadTime ();
	
    oldday = ntime->tm_mday;
	
	sprintf(logfilename, "/home/ems/h9000/his/AIC/AICcmd.%02d",ntime->tm_mday);
	
	InitialAIC_Data_On_Screen ();


	for (;;)
	{
	//	printf("here is for loop\n");
		ReadTime ();//读当前时间
		oldmonth=ntime->tm_mon+1;
		oldday = ntime->tm_mday;
		oldhour= ntime->tm_hour;
		oldmin= ntime->tm_min;
		

        LogFileChangeDayAndWriteTime();

		ReadAIC_DataFromRTDB();
		
	//	strcpy ( sbo_db_name, DB_NAME_QC_AIC );
       // strcat ( sbo_db_name, "_YES.." );//执行/挂起
	//	printf("sbo_db_name=%s\n",sbo_db_name);

       // sbo_flag = InterLock_Judge ( sbo_db_name );
	//	if(sbo_flag==1)
		{
			if(AIC_Loop>=AIC_Period)
			{
				AIC_Loop=0;
				AIC_Module();
			}
		}
		//else        
           // InitialAIC_Data_On_Screen ();	
		
		AIC_Loop++;
printf("AIC_Loop=%d\n",AIC_Loop);

		if(Number_ANA>0)
        {
			if(GetControlState()==1)
				lan_out ((char *)&ml_buf_ANA,lanhd_ANA,(unsigned char)0);
			Number_ANA=0;
        }
        if(Number_IND>0)
        {
			if(GetControlState()==1)
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
