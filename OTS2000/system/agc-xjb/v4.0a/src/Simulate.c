

#include	<stdio.h>
#include        <math.h>

#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include    "../../../cmd/v4.0a/inc/cmd_def.h"
#include    "../../../dms/v4.0a/inc/dms_std.h"
#include    "../../../dms/v4.0a/inc/dms_structure_def.h"
#include    "../../../dms/v4.0a/inc/os_type_def.h"
#include	"/home/ems/system/dps/v4.0a/inc/dps_rec_def.h"
#include	"/home/ems/system/dps/v4.0a/inc/ipc_common.h"


#define UNIT_NUMB   8
#define NAME_SIZE    30
#define	MAXSEGMENT   3
#define MAXSEGMENTAVC 2
DATE_TIME	update_time;
float   APStep=1;
float   RPStep=1;
float    AP_Setpoint=500;
int     Simulate_SetPoint=0;
int Direction=1;

POINTER point_AP[UNIT_NUMB];
POINTER point_RP[UNIT_NUMB];
POINTER point_KJ[UNIT_NUMB];
POINTER point_TJ[UNIT_NUMB];
time_t		tim;
struct tm	tmpt;
int   Time_120s=240;
int  Number_ANA=0;
ANA_MSG	ml_buf_ANA[64];
LAN_HEAD	lanhd_ANA,lanhd_IND;
int Number_IND=0;
SYS_MSG	ml_buf_IND[64];

SHM_RECORD *shm_rec_addr;

float   BeginAPValue[UNIT_NUMB];
float   BeginRPValue[UNIT_NUMB];
float   EndAPValue[UNIT_NUMB];
float   EndRPValue[UNIT_NUMB];
int     AP_Flag[UNIT_NUMB]={0};
int     RP_Flag[UNIT_NUMB]={0};
int     KJ_Flag[UNIT_NUMB]={0};
int     KJ_Time[UNIT_NUMB]={0};
int     TJ_Flag[UNIT_NUMB]={0};
int     TJ_Time[UNIT_NUMB]={0};

float AP_RtValue[UNIT_NUMB+1];
float Unit_Status[UNIT_NUMB+1];
float AP_Permit[UNIT_NUMB+1];
float RP_RtValue[UNIT_NUMB+1];
float RP_Permit[UNIT_NUMB+1];


char DB_NAME_AP_RTVALUE[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_UNIT_STATUS[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_UNIT_Permit[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RP_RTVALUE[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RP_AVAIL[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_OPERATOR_SETVALUE[MAXSEGMENT][NAME_SIZE];
char DB_NAME_ONLINE_STAT[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_STOP_STAT[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_ONLINE_STA[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_STOP_STA[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_STARTING[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_STOPPING[UNIT_NUMB+1][NAME_SIZE];


/*参数设置*/
char DB_NAME_AP_Deadband[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_DEADBAND[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_DEADBAND[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_CompDeadband[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_DEADBAND[MAXSEGMENT][NAME_SIZE];
char DB_NAME_COMP_DEADBAND[MAXSEGMENT][NAME_SIZE];
char DB_NAME_SS_DEADBAND[MAXSEGMENT][NAME_SIZE];
char DB_NAME_EC_DEADBAND[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VZ_DEADBAND[MAXSEGMENT][NAME_SIZE];
char DB_NAME_AP_ChangeMax[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_ChangeMax[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_MAXCHANGE[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_MAXCHANGE[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_PowGenDB[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_PowGenDB[MAXSEGMENT][NAME_SIZE];
char DB_NAME_SR_MaxChange[MAXSEGMENT][NAME_SIZE];
char DB_NAME_SR_MaxValue[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_Min[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_Max[MAXSEGMENT][NAME_SIZE];
char DB_NAME_AP_MIN[MAXSEGMENT][NAME_SIZE];
char DB_NAME_AP_MAX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_MIN[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_MAX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_MIN[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_MAX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_WATERHEAD[NAME_SIZE];
char DB_NAME_PRVILEGE[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_FR_ACTVALUE[6][NAME_SIZE];
char DB_NAME_VO_RTVALUE[6][NAME_SIZE];

float AP_Deadband[MAXSEGMENT];
float RP_Deadband[MAXSEGMENT];
float FR_Deadband[MAXSEGMENT];
float FR_CompDeadband[MAXSEGMENT];
float VO_Deadband[MAXSEGMENT];
float VO_Comp_Deadband[MAXSEGMENT];
float SS_Deadband[MAXSEGMENT];
float EC_Deadband[MAXSEGMENT];
float VZ_Deadband[MAXSEGMENT];
float AP_ChangeMax[MAXSEGMENT];
float RP_ChangeMax[MAXSEGMENT];
float FR_ChangeMax[MAXSEGMENT];
float VO_ChangeMax[MAXSEGMENT];
float FR_PowGenDB[MAXSEGMENT];
float VO_PowGenDB[MAXSEGMENT];
float SR_MaxChange[MAXSEGMENT];
float SR_MaxValue[MAXSEGMENT];
float FR_Min[MAXSEGMENT];
float FR_Max[MAXSEGMENT];
float AP_Min[MAXSEGMENT];
float AP_Max[MAXSEGMENT];
float RP_Min[MAXSEGMENT];
float RP_Max[MAXSEGMENT];
float VO_Min[MAXSEGMENT];
float VO_Max[MAXSEGMENT];
float WaterHead;
int   privilege[UNIT_NUMB+1];
float FR_ActValue[2];
float VO_RtValue[2];

extern int	GetAnaPtfValueById( POINTER *, FLOAT *);
extern int  LocateKeyword( FILE *, char *);
extern void CreateLockFile(char *);

void Quit (int sn)
{
    UnMountDB();
    printf ("Simulate exit!(Ctrl_c or kill -2 pid)\n");
    lan_finish(0);
    exit (0);
}

void WriteRTDB_ANA(char *PointName,float data)
{
    DATE_TIME       write_time;
    char    err_str[128];
    GetSysTime( &write_time );
    if( -1 == SetAnaPtfValueByNameStr( PointName,data,write_time))
    {
        sprintf (err_str,"WriteRTDB_ANA;EC:002 %s write fail !!!",PointName);		
        Err_log(err_str);
    }
}

/*int WriteRTDB_ANA (char *PointName,float data)
{
    DMS_COMMON common_aa;
    time_t  tim;
    struct tm   tmpt;
    int value;
    float OldValue=0;
    value=(int)(data*1000);
    if (ReadEntryByNameStr(PointName,&common_aa)==-1)
        return(-1);
    
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
        lan_out ((char *)&ml_buf_ANA,lanhd_ANA,(unsigned char)0); 
        Number_ANA=0; 
    }
    return 0;
}*/

int WriteRTDB_IND(char *name,short status)
{
	WR_VAR_UNION	new_value;
	char		name_buf[POINT_NAME_SIZE];
	int		ret;
	enum RW_FLAG	rw_flag=0;
	IOST		iost={0};
	time_t		tim;
	struct tm	tmpt;
	DATE_TIME	update_time;

	strcpy (name_buf,name);
	new_value.ind.rt_state = status;
	tim=time(&tim);
	tmpt = *localtime(&tim);
	update_time.ms		= 0;
	update_time.second	= tmpt.tm_sec;
	update_time.minute	= tmpt.tm_min;
	update_time.hour	= tmpt.tm_hour;
	update_time.day		= tmpt.tm_mday;
	update_time.month	= tmpt.tm_mon+1;
	update_time.year	= tmpt.tm_year+1900;
	rw_flag = RW_UPDATE_TIME | RW_RT_VALUE;

	ret = SetPtValueIostByNameStr( name_buf, new_value, iost, update_time, rw_flag);
	if (ret == -1)
		printf ( "\navc: %s No this point in the database when WriteRTDB_IND!\n",name_buf);
	return(ret);
}
/*
int WriteRTDB_IND (char *PointName,short data)
{
    DMS_COMMON common_aa; 
    time_t  tim;
    struct tm   tmpt;
    int OldValue;
    if (ReadEntryByNameStr(PointName,&common_aa)==-1)
	return(-1);
	
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
    Number_IND+=1;
    if(Number_IND>=64)
    {
        lan_out ((char *)&ml_buf_IND,lanhd_IND,(unsigned char)0);
        Number_IND=0;
     }
	return 0;
}*/

void ReadAGCDefFile(void)
{
	FILE	*fp;
	char filename[40]="/home/ems/h9000/agc-xjb/AGC.DEF";
	char err_str[30]="";
	char keyword[30]="";
	char StrTemp[50]="";
	int i;
        DMS_COMMON common_aa;

	if( (fp=fopen(filename, "r"))==NULL )
	{
            sprintf(err_str, "Fail to open %s", filename);
            Err_log(err_str);
            exit(0);
	}
        
        /* KEYWORD=="WATER_HEAD" */
	strcpy(keyword, "WATER_HEAD");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        fscanf(fp,"%s\n",DB_NAME_WATERHEAD);
        if(ReadEntryByNameStr(DB_NAME_WATERHEAD,&common_aa)==-1)
        {
            printf("No this point %s\n",DB_NAME_WATERHEAD);
            exit(0);
        }

	/* KEYWORD=="UNIT_STATUS" */
	strcpy(keyword, "UNIT_STATUS");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_UNIT_STATUS[i]);
            if(ReadEntryByNameStr(DB_NAME_UNIT_STATUS[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_UNIT_STATUS[i]);
                exit(0);
            }
        }
        
        /* KEYWORD=="AP_RTVALUE" */
	strcpy(keyword, "AP_RTVALUE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_AP_RTVALUE[i]);
            if(ReadEntryByNameStr(DB_NAME_AP_RTVALUE[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_AP_RTVALUE[i]);
                exit(0);
            }
        }
        
        
        /* KEYWORD=="UNIT_Permit" */
	strcpy(keyword, "UNIT_Permit");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=1;i<=UNIT_NUMB;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_UNIT_Permit[i]);
            if(ReadEntryByNameStr(DB_NAME_UNIT_Permit[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_UNIT_Permit[i]);
                exit(0);
            }
        }  
        
        /* KEYWORD=="OPERATOR_SETVALUE" */
	strcpy(keyword, "OPERATOR_SETVALUE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
     for(i=0;i<MAXSEGMENT;i++)
     {
         fscanf(fp,"%s\n",DB_NAME_OPERATOR_SETVALUE[i]);
         if(ReadEntryByNameStr(DB_NAME_OPERATOR_SETVALUE[i],&common_aa)==-1)
         {
            printf("No this point %s\n",DB_NAME_OPERATOR_SETVALUE[i]);
            exit(0);
         }
      }  

	     /* KEYWORD=="FR_DEADBAND" */
	strcpy(keyword, "FR_DEADBAND");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_FR_DEADBAND[i]);
            if(ReadEntryByNameStr(DB_NAME_FR_DEADBAND[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_FR_DEADBAND[i]);
                exit(0);
            }
        }
        
         /* KEYWORD=="FR_CompDeadband" */
	strcpy(keyword, "FR_CompDeadband");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_FR_CompDeadband[i]);
            if(ReadEntryByNameStr(DB_NAME_FR_CompDeadband[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_FR_CompDeadband[i]);
                exit(0);
            }
        }
        
        
        /* KEYWORD=="SS_DEADBAND" */
	strcpy(keyword, "SS_DEADBAND");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_SS_DEADBAND[i]);
            if(ReadEntryByNameStr(DB_NAME_SS_DEADBAND[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_SS_DEADBAND[i]);
                exit(0);
            }
        }
        
        /* KEYWORD=="VZ_DEADBAND" */
	strcpy(keyword, "VZ_DEADBAND");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_VZ_DEADBAND[i]);
            if(ReadEntryByNameStr(DB_NAME_VZ_DEADBAND[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_VZ_DEADBAND[i]);
                exit(0);
            }
        }
        
        /* KEYWORD=="AP_ChangeMax" */
	strcpy(keyword, "AP_ChangeMax");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_AP_ChangeMax[i]);
            if(ReadEntryByNameStr(DB_NAME_AP_ChangeMax[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_AP_ChangeMax[i]);
                exit(0);
            }
        }

	   /* KEYWORD=="AP_Deadband" */
	strcpy(keyword, "AP_Deadband");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_AP_Deadband[i]);
            if(ReadEntryByNameStr(DB_NAME_AP_Deadband[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_AP_Deadband[i]);
                exit(0);
            }
        }

	/* KEYWORD=="FR_PowGenDB" */
	strcpy(keyword, "FR_PowGenDB");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_FR_PowGenDB[i]);
            if(ReadEntryByNameStr(DB_NAME_FR_PowGenDB[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_FR_PowGenDB[i]);
                exit(0);
            }
        }

	  /* KEYWORD=="EC_DEADBAND" */
	strcpy(keyword, "EC_DEADBAND");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_EC_DEADBAND[i]);
            if(ReadEntryByNameStr(DB_NAME_EC_DEADBAND[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_EC_DEADBAND[i]);
                exit(0);
            }
        }   

        /* KEYWORD=="FR_ChangeMax" */
	strcpy(keyword, "FR_ChangeMax");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_FR_ChangeMax[i]);
            if(ReadEntryByNameStr(DB_NAME_FR_ChangeMax[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_FR_ChangeMax[i]);
                exit(0);
            }
        }  

	 /* KEYWORD=="SR_MaxChange" */
	strcpy(keyword, "SR_MaxChange");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
            Err_log(err_str);
            exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_SR_MaxChange[i]);
            if(ReadEntryByNameStr(DB_NAME_SR_MaxChange[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_SR_MaxChange[i]);
                exit(0);
            }
        } 
        
        /* KEYWORD=="SR_MaxValue" */
	strcpy(keyword, "SR_MaxValue");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
            Err_log(err_str);
            exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_SR_MaxValue[i]);
            if(ReadEntryByNameStr(DB_NAME_SR_MaxValue[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_SR_MaxValue[i]);
                exit(0);
            }
        } 

	 /* KEYWORD=="FR_Min" */
	strcpy(keyword, "FR_Min");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_FR_Min[i]);
            if(ReadEntryByNameStr(DB_NAME_FR_Min[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_FR_Min[i]);
                exit(0);
            }
        }
        
        /* KEYWORD=="FR_Max" */
	strcpy(keyword, "FR_Max");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_FR_Max[i]);
            if(ReadEntryByNameStr(DB_NAME_FR_Max[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_FR_Max[i]);
                exit(0);
            }
        }   

	 /* KEYWORD=="AP_MIN" */
	strcpy(keyword, "AP_MIN");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_AP_MIN[i]);
            if(ReadEntryByNameStr(DB_NAME_AP_MIN[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_AP_MIN[i]);
                exit(0);
            }
        }
        
        /* KEYWORD=="AP_MAX" */
	strcpy(keyword, "AP_MAX");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENT;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_AP_MAX[i]);
            if(ReadEntryByNameStr(DB_NAME_AP_MAX[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_AP_MAX[i]);
                exit(0);
            }
        }
        
        /* KEYWORD=="PRVILEGE" */
	strcpy(keyword, "PRVILEGE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=1;i<=UNIT_NUMB;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_PRVILEGE[i]);
            if(ReadEntryByNameStr(DB_NAME_PRVILEGE[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_PRVILEGE[i]);
                exit(0);
            }
        }
        
        /* KEYWORD=="FR_ACTVALUE" */
	strcpy(keyword, "FR_ACTVALUE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<2;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_FR_ACTVALUE[i]);
            if(ReadEntryByNameStr(DB_NAME_FR_ACTVALUE[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_FR_ACTVALUE[i]);
                exit(0);
            }
        }
        
        /* KEYWORD=="ONLINE_STA" */
	strcpy(keyword, "ONLINE_STA");
	if(-1==LocateKeyword(fp, keyword))
	{
	     sprintf(err_str, "Fail to find %s keyword", keyword);
	     Err_log(err_str);
	     exit(0);
	}
        for(i=1;i<=UNIT_NUMB;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_ONLINE_STA[i]);
            if(ReadEntryByNameStr(DB_NAME_ONLINE_STA[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_ONLINE_STA[i]);
                sprintf(err_str,"No this point %s\n",DB_NAME_ONLINE_STA[i]);
	        Err_log(err_str);
                exit(0);
            }
        }
        
        /* KEYWORD=="STOP_STA" */
	strcpy(keyword, "STOP_STA");
	if(-1==LocateKeyword(fp, keyword))
	{
	     sprintf(err_str, "Fail to find %s keyword", keyword);
	     Err_log(err_str);
	     exit(0);
	}
        for(i=1;i<=UNIT_NUMB;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_STOP_STA[i]);
            if(ReadEntryByNameStr(DB_NAME_STOP_STA[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_STOP_STA[i]);
                sprintf(err_str,"No this point %s\n",DB_NAME_STOP_STA[i]);
	        Err_log(err_str);
                exit(0);
            }
        }
        
        /* KEYWORD=="STARTING" */
	strcpy(keyword, "STARTING");
	if(-1==LocateKeyword(fp, keyword))
	{
	    sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    exit(0);
	}
        for(i=1;i<=UNIT_NUMB;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_STARTING[i]);
            if(ReadEntryByNameStr(DB_NAME_STARTING[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_STARTING[i]);
                sprintf(err_str,"No this point %s\n",DB_NAME_STARTING[i]);
	        Err_log(err_str);
                exit(0);
            }
        }
        
        /* KEYWORD=="STOPPING" */
	strcpy(keyword, "STOPPING");
	if(-1==LocateKeyword(fp, keyword))
	{
	    sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    exit(0);
	}
        for(i=1;i<=UNIT_NUMB;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_STOPPING[i]);
            if(ReadEntryByNameStr(DB_NAME_STOPPING[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_STOPPING[i]);
                sprintf(err_str,"No this point %s\n",DB_NAME_STOPPING[i]);
	        Err_log(err_str);
                exit(0);
            }
        }
	fclose(fp);	
}

void ReadAVCDefFile(void)
{
	FILE	*fp;
	char filename[40]="/home/ems/h9000/agc-xjb/AVC.DEF";
	char err_str[30]="";
	char keyword[30]="";
	char StrTemp[50]="";
	int i;
    DMS_COMMON common_aa;

	if( (fp=fopen(filename, "r"))==NULL )
	{
            sprintf(err_str, "Fail to open %s", filename);
            Err_log(err_str);
            exit(0);;
	}

        
        /* KEYWORD=="UNIT_AVAIL" */
	strcpy(keyword, "UNIT_AVAIL");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_RP_AVAIL[i]);
            if(ReadEntryByNameStr(DB_NAME_RP_AVAIL[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_RP_AVAIL[i]);
                exit(0);
            }
        }
        
        /* KEYWORD=="RP_RTVALUE" */
	strcpy(keyword, "RP_RTVALUE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}  
	for(i=1;i<=UNIT_NUMB;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_RP_RTVALUE[i]);
            if(ReadEntryByNameStr(DB_NAME_RP_RTVALUE[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_RP_RTVALUE[i]);
                exit(0);
            }
        }
     

	     /* KEYWORD=="RP_DEADBAND" */
	strcpy(keyword, "RP_DEADBAND");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENTAVC;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_RP_DEADBAND[i]); 
             if(ReadEntryByNameStr(DB_NAME_RP_DEADBAND[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_RP_DEADBAND[i]);
                exit(0);
            }
        }
        
        /* KEYWORD=="VO_DEADBAND" */
	strcpy(keyword, "VO_DEADBAND");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENTAVC;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_VO_DEADBAND[i]); 
            if(ReadEntryByNameStr(DB_NAME_VO_DEADBAND[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_VO_DEADBAND[i]);
                exit(0);
            }
        }

	/* KEYWORD=="VO_MaxChange" */
	strcpy(keyword, "VO_MaxChange");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENTAVC;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_VO_MAXCHANGE[i]);
            if(ReadEntryByNameStr(DB_NAME_VO_MAXCHANGE[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_VO_MAXCHANGE[i]);
                exit(0);
            }
        }
        
        /* KEYWORD=="RP_MaxChange" */
	strcpy(keyword, "RP_MaxChange");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENTAVC;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_RP_MAXCHANGE[i]);
            if(ReadEntryByNameStr(DB_NAME_RP_MAXCHANGE[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_RP_MAXCHANGE[i]);
                exit(0);
            }
        }

	 /* KEYWORD=="VO_GENDB" */
	strcpy(keyword, "VO_GENDB");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENTAVC;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_VO_PowGenDB[i]); 
            if(ReadEntryByNameStr(DB_NAME_VO_PowGenDB[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_VO_PowGenDB[i]);
                exit(0);
            }
        }

	/* KEYWORD=="COMP_DEADBAND" */
	strcpy(keyword, "COMP_DEADBAND");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENTAVC;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_COMP_DEADBAND[i]);
            if(ReadEntryByNameStr(DB_NAME_COMP_DEADBAND[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_COMP_DEADBAND[i]);
                exit(0);
            }
        }
        
         /* KEYWORD=="RP_MIN" */
	strcpy(keyword, "RP_MIN");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENTAVC;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_RP_MIN[i]); 
            if(ReadEntryByNameStr(DB_NAME_RP_MIN[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_RP_MIN[i]);
                exit(0);
            }
        }
        
        /* KEYWORD=="RP_MAX" */
	strcpy(keyword, "RP_MAX");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENTAVC;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_RP_MAX[i]); 
            if(ReadEntryByNameStr(DB_NAME_RP_MAX[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_RP_MAX[i]);
                exit(0);
            }
        }
        
         /* KEYWORD=="VO_MIN" */
	strcpy(keyword, "VO_MIN");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENTAVC;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_VO_MIN[i]); 
            if(ReadEntryByNameStr(DB_NAME_VO_MIN[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_VO_MIN[i]);
                exit(0);
            }
        }
       
        /* KEYWORD=="VO_MAX" */
	strcpy(keyword, "VO_MAX");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<MAXSEGMENTAVC;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_VO_MAX[i]); 
            if(ReadEntryByNameStr(DB_NAME_VO_MAX[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_VO_MAX[i]);
                exit(0);
            }
        }
        
        /* KEYWORD=="VO_RTVALUE" */
	strcpy(keyword, "VO_RTVALUE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
        for(i=0;i<2;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_VO_RTVALUE[i]);    
            if(ReadEntryByNameStr(DB_NAME_VO_RTVALUE[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_VO_RTVALUE[i]);
                exit(0);
            }
        }
	fclose(fp);
}

int	ReadDefFile(void)
{
	FILE	*fp;
	char filename[40]="/home/ems/h9000/agc-xjb/Simulate.DEF";
	char err_str[30]="";
	char keyword[30]="";
        char strTemp[5];
	int i;
	DMS_COMMON common_aa;

	if( (fp=fopen(filename, "r"))==NULL )
	{
            sprintf(err_str, "Fail to open %s", filename);
            Err_log(err_str);
            return -1;
	}
        
        /* KEYWORD=="WaterHead" */
	strcpy(keyword, "WaterHead");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		return -1;
        }
        fscanf(fp,"%s\n",strTemp);
        WaterHead=(float)atof(strTemp);

	/* KEYWORD=="AP_Step" */
	strcpy(keyword, "AP_Step");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		return -1;
        }
        fscanf(fp,"%s\n",strTemp);
        APStep=(float)atof(strTemp);
        printf("APStep=%f\n",APStep);

	/* KEYWORD=="RP_Step" */
	strcpy(keyword, "RP_Step");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
            Err_log(err_str);
            return -1;
        }
        fscanf(fp,"%s\n",strTemp);
        RPStep=(float)atof(strTemp);
        printf("RPStep=%f\n",RPStep);

	/* KEYWORD=="AP_RtValue" */
	strcpy(keyword, "AP_RtValue");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
            Err_log(err_str);
            return -1;
        }
	for(i=1;i<=UNIT_NUMB;i++)
	{
            fscanf(fp,"%s\n",strTemp);
            AP_RtValue[i]=(float)atof(strTemp);
          //  printf("AP_RtValue[%d]=%f\n",i,AP_RtValue[i]);
	}

	/* KEYWORD=="Unit_Status" */
	strcpy(keyword, "Unit_Status");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
            Err_log(err_str);
            return -1;
        }
	for(i=1;i<=UNIT_NUMB;i++)
	{
            fscanf(fp,"%s\n",strTemp);
            Unit_Status[i]=(float)atof(strTemp);
            printf("Unit_Status[%d]=%f\n",i,Unit_Status[i]);
	}

	/* KEYWORD=="AP_Permit" */
	strcpy(keyword, "AP_Permit");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
            Err_log(err_str);
            return -1;
        }
	for(i=1;i<=UNIT_NUMB;i++)
	{
            fscanf(fp,"%s\n",strTemp);
            AP_Permit[i]=(float)atof(strTemp);
          //  printf("AP_Permit[%d]=%f\n",i,AP_Permit[i]);
	}
        
        /* KEYWORD=="RP_RtValue" */
	strcpy(keyword, "RP_RtValue");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
            Err_log(err_str);
            return -1;
        }
	for(i=1;i<=UNIT_NUMB;i++)
	{
            fscanf(fp,"%s\n",strTemp);
            RP_RtValue[i]=(float)atof(strTemp);
            //printf("RP_RtValue[%d]=%f\n",i,RP_RtValue[i]);
	}
        
        /* KEYWORD=="RP_Permit" */
	strcpy(keyword, "RP_Permit");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
            Err_log(err_str);
            return -1;
        }
	for(i=1;i<=UNIT_NUMB;i++)
	{
            fscanf(fp,"%s\n",strTemp);
            RP_Permit[i]=(float)atof(strTemp);
            //printf("RP_Permit[%d]=%f\n",i,RP_Permit[i]);
	}
        
        /*Simulate_SetPoint*/
        strcpy(keyword, "Simulate_SetPoint");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
        fscanf(fp,"%s\n",strTemp);
        Simulate_SetPoint=(int)atof(strTemp);
        printf("Simulate_SetPoint=%d\n",Simulate_SetPoint);

	/*AP_Deadband*/
    strcpy(keyword, "AP_Deadband");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        AP_Deadband[i]=(float)atof(strTemp);
	}

	/*RP_Deadband*/
    strcpy(keyword, "RP_Deadband");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        RP_Deadband[i]=(float)atof(strTemp);
	}

	/*FR_Deadband*/
    strcpy(keyword, "FR_Deadband");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        FR_Deadband[i]=(float)atof(strTemp);
	}

	/*FR_CompDeadband*/
    strcpy(keyword, "FR_CompDeadband");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        FR_CompDeadband[i]=(float)atof(strTemp);
	}
    
	/*VO_Deadband*/
    strcpy(keyword, "VO_Deadband");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        VO_Deadband[i]=(float)atof(strTemp);
	}

		/*VO_Comp_Deadband*/
    strcpy(keyword, "VO_Comp_Deadband");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        VO_Comp_Deadband[i]=(float)atof(strTemp);
	}

		/*SS_Deadband*/
    strcpy(keyword, "SS_Deadband");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        SS_Deadband[i]=(float)atof(strTemp);
	}

	/*EC_Deadband*/
    strcpy(keyword, "EC_Deadband");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        EC_Deadband[i]=(float)atof(strTemp);
	}

	/*VZ_Deadband*/
    strcpy(keyword, "VZ_Deadband");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        VZ_Deadband[i]=(float)atof(strTemp);
	}

	/*AP_ChangeMax*/
    strcpy(keyword, "AP_ChangeMax");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        AP_ChangeMax[i]=(float)atof(strTemp);
	}

	/*RP_ChangeMax*/
    strcpy(keyword, "RP_ChangeMax");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        RP_ChangeMax[i]=(float)atof(strTemp);
	}

	/*FR_ChangeMax*/
    strcpy(keyword, "FR_ChangeMax");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        FR_ChangeMax[i]=(float)atof(strTemp);
	}

	/*VO_ChangeMax*/
    strcpy(keyword, "VO_ChangeMax");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        VO_ChangeMax[i]=(float)atof(strTemp);
	}

	/*FR_PowGenDB*/
    strcpy(keyword, "FR_PowGenDB");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        FR_PowGenDB[i]=(float)atof(strTemp);
	}

	/*VO_PowGenDB*/
    strcpy(keyword, "VO_PowGenDB");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        VO_PowGenDB[i]=(float)atof(strTemp);
	} 

	/*SR_MaxChange*/
    strcpy(keyword, "SR_MaxChange");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        SR_MaxChange[i]=(float)atof(strTemp);
	} 

	/*SR_MaxValue*/
    strcpy(keyword, "SR_MaxValue");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        SR_MaxValue[i]=(float)atof(strTemp);
	} 

	/*FR_Min*/
    strcpy(keyword, "FR_Min");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        FR_Min[i]=(float)atof(strTemp);
	} 

	/*FR_Max*/
    strcpy(keyword, "FR_Max");
	if(-1==LocateKeyword(fp, keyword))
	{
        sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
    }
	for(i=0;i<MAXSEGMENT;i++)
	{
        fscanf(fp,"%s\n",strTemp);
        FR_Max[i]=(float)atof(strTemp);
	} 

	/*AP_MIN*/
        strcpy(keyword, "AP_MIN");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
        }
	for(i=0;i<MAXSEGMENT;i++)
	{
            fscanf(fp,"%s\n",strTemp);
            AP_Min[i]=(float)atof(strTemp);
	} 

	/*AP_MAX*/
        strcpy(keyword, "AP_MAX");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
        }
	for(i=0;i<MAXSEGMENT;i++)
	{
            fscanf(fp,"%s\n",strTemp);
            AP_Max[i]=(float)atof(strTemp);
	} 

        /*RP_Min*/
        strcpy(keyword, "RP_Min");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
        }
	for(i=0;i<MAXSEGMENT;i++)
	{
            fscanf(fp,"%s\n",strTemp);
            RP_Min[i]=(float)atof(strTemp);
	} 

	/*RP_Max*/
        strcpy(keyword, "RP_Max");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
        }
	for(i=0;i<MAXSEGMENT;i++)
	{
            fscanf(fp,"%s\n",strTemp);
            RP_Max[i]=(float)atof(strTemp);
	} 
        
        /*VO_Min*/
        strcpy(keyword, "VO_Min");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
        }
	for(i=0;i<MAXSEGMENT;i++)
	{
            fscanf(fp,"%s\n",strTemp);
            VO_Min[i]=(float)atof(strTemp);
	} 

	/*VO_Max*/
        strcpy(keyword, "VO_Max");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
        }
	for(i=0;i<MAXSEGMENT;i++)
	{
            fscanf(fp,"%s\n",strTemp);
            VO_Max[i]=(float)atof(strTemp);
	} 
        
        /*privilege*/
        strcpy(keyword, "privilege");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
        }
	for(i=1;i<=UNIT_NUMB;i++)
	{
            fscanf(fp,"%s\n",strTemp);
            privilege[i]=atoi(strTemp);
	} 
     
        /*FR_ActValue*/
        strcpy(keyword, "FR_ActValue");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
	    Err_log(err_str);
	    return -1;
        }
	for(i=0;i<2;i++)
	{
            fscanf(fp,"%s\n",strTemp);
            FR_ActValue[i]=(float)atof(strTemp);
	} 
        
        /*VO_RtValue*/
        strcpy(keyword, "VO_RtValue");
	if(-1==LocateKeyword(fp, keyword))
	{
            sprintf(err_str, "Fail to find %s keyword", keyword);
            printf("Fail to find %s keyword\n", keyword);
	    Err_log(err_str);
	    return -1;
        }
	for(i=0;i<2;i++)
	{
            fscanf(fp,"%s\n",strTemp);
            VO_RtValue[i]=(float)atof(strTemp);
	}

	/* KEYWORD=="ONLINE_STAT" */
	strcpy(keyword, "ONLINE_STAT");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		printf("Fail to find %s keyword\n", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_ONLINE_STAT[i]);
            printf("%s\n",DB_NAME_ONLINE_STAT[i]);
            if(ReadEntryByNameStr(DB_NAME_ONLINE_STAT[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_ONLINE_STAT[i]);
                exit(0);
            }
        }
  	        
        /* KEYWORD=="STOP_STAT" */
	strcpy(keyword, "STOP_STAT");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
        {
            fscanf(fp,"%s\n",DB_NAME_STOP_STAT[i]);
            if(ReadEntryByNameStr(DB_NAME_STOP_STAT[i],&common_aa)==-1)
            {
                printf("No this point %s\n",DB_NAME_STOP_STAT[i]);
                exit(0);
            }
        }
       fclose(fp);
	   return 0;
}
float ReadDB_ANA(char *PointName)
{
    float ReturnValue;
    DMS_COMMON common_aa;
    if(ReadEntryByNameStr(PointName,&common_aa)==-1)
	exit(0);
    ReturnValue=common_aa.var.anlg.fValue;
    return(ReturnValue);
}


void InitData(void)
{
    int i;
    for(i=1;i<=UNIT_NUMB;i++)
    {
        WriteRTDB_ANA(DB_NAME_AP_RTVALUE[i],AP_RtValue[i]);
        WriteRTDB_IND(DB_NAME_UNIT_STATUS[i],(short)Unit_Status[i]);
        WriteRTDB_IND(DB_NAME_UNIT_Permit[i],(short)AP_Permit[i]);
        WriteRTDB_ANA(DB_NAME_RP_RTVALUE[i],RP_RtValue[i]);
        WriteRTDB_IND(DB_NAME_RP_AVAIL[i],(short)RP_Permit[i]);
        WriteRTDB_ANA(DB_NAME_PRVILEGE[i],privilege[i]);
        if(Unit_Status[i]==1)
        {
            WriteRTDB_IND(DB_NAME_STOP_STAT[i],0);
            WriteRTDB_IND(DB_NAME_ONLINE_STAT[i],1);
            WriteRTDB_IND(DB_NAME_STOP_STA[i],0);
            WriteRTDB_IND(DB_NAME_ONLINE_STA[i],1);
         }
         else if(Unit_Status[i]==2)
         {
            WriteRTDB_IND(DB_NAME_STOP_STAT[i],1);
            WriteRTDB_IND(DB_NAME_ONLINE_STAT[i],0);
            WriteRTDB_IND(DB_NAME_STOP_STA[i],1);
            WriteRTDB_IND(DB_NAME_ONLINE_STA[i],0);
         }
    }
    AP_Setpoint=ReadDB_ANA(DB_NAME_OPERATOR_SETVALUE[0]);
    
    WriteRTDB_ANA(DB_NAME_WATERHEAD,WaterHead);
	for(i=0;i<MAXSEGMENT;i++)
	{
		WriteRTDB_ANA(DB_NAME_AP_Deadband[i],AP_Deadband[i]);
		WriteRTDB_ANA(DB_NAME_FR_DEADBAND[i],FR_Deadband[i]);
		WriteRTDB_ANA(DB_NAME_FR_CompDeadband[i],FR_CompDeadband[i]);
		WriteRTDB_ANA(DB_NAME_SS_DEADBAND[i],SS_Deadband[i]);
		WriteRTDB_ANA(DB_NAME_EC_DEADBAND[i],EC_Deadband[i]);
		WriteRTDB_ANA(DB_NAME_VZ_DEADBAND[i],VZ_Deadband[i]);
		WriteRTDB_ANA(DB_NAME_AP_ChangeMax[i],AP_ChangeMax[i]);
		WriteRTDB_ANA(DB_NAME_FR_ChangeMax[i],FR_ChangeMax[i]);
		WriteRTDB_ANA(DB_NAME_FR_PowGenDB[i],FR_PowGenDB[i]);
		WriteRTDB_ANA(DB_NAME_SR_MaxChange[i],SR_MaxChange[i]);
		WriteRTDB_ANA(DB_NAME_SR_MaxValue[i],SR_MaxValue[i]);
		WriteRTDB_ANA(DB_NAME_FR_Min[i],FR_Min[i]);
		WriteRTDB_ANA(DB_NAME_FR_Max[i],FR_Max[i]);
		WriteRTDB_ANA(DB_NAME_AP_MIN[i],AP_Min[i]);
		WriteRTDB_ANA(DB_NAME_AP_MAX[i],AP_Max[i]);
	} 
	for(i=0;i<MAXSEGMENTAVC;i++)
	{
		WriteRTDB_ANA(DB_NAME_RP_DEADBAND[i],RP_Deadband[i]);
		WriteRTDB_ANA(DB_NAME_VO_DEADBAND[i],VO_Deadband[i]);
		WriteRTDB_ANA(DB_NAME_COMP_DEADBAND[i],VO_Comp_Deadband[i]);
        WriteRTDB_ANA(DB_NAME_RP_MAXCHANGE[i],RP_ChangeMax[i]);
		WriteRTDB_ANA(DB_NAME_VO_MAXCHANGE[i],VO_ChangeMax[i]);
		WriteRTDB_ANA(DB_NAME_VO_PowGenDB[i],VO_PowGenDB[i]);
        WriteRTDB_ANA(DB_NAME_RP_MIN[i],RP_Min[i]);
		WriteRTDB_ANA(DB_NAME_RP_MAX[i],RP_Max[i]);
		WriteRTDB_ANA(DB_NAME_VO_MIN[i],VO_Min[i]);
		WriteRTDB_ANA(DB_NAME_VO_MAX[i],VO_Max[i]);
	} 
        for(i=0;i<2;i++)
        {
            WriteRTDB_ANA(DB_NAME_FR_ACTVALUE[i],FR_ActValue[i]);
            WriteRTDB_ANA(DB_NAME_VO_RTVALUE[i],VO_RtValue[i]);
        }
}

void Unit_Distribute(void)
{
    int i;
    WR_VAR_UNION	new_value;
    enum RW_FLAG	rw_flag=0;
    IOST		iost={0};
    
    tim=time(&tim);
    tmpt = *localtime(&tim);
    update_time.ms	= 0;
    update_time.second = tmpt.tm_sec;
    update_time.minute	= tmpt.tm_min;
    update_time.hour	= tmpt.tm_hour;
    update_time.day		= tmpt.tm_mday;
    update_time.month	= tmpt.tm_mon+1;
    update_time.year	= tmpt.tm_year+1900;
       
    for(i=0;i<UNIT_NUMB;i++)
    {
        if(AP_Flag[i]==1)
        {
            if(EndAPValue[i]-BeginAPValue[i]>APStep)
                BeginAPValue[i]+=APStep;
            else if(BeginAPValue[i]-EndAPValue[i]>APStep)
                BeginAPValue[i]-=APStep;
            else
            {
                BeginAPValue[i]=EndAPValue[i];
                AP_Flag[i]=0;
            }  
            printf("No.%d有功值为%f\n",i+1+14,BeginAPValue[i]);
            WriteRTDB_ANA(DB_NAME_AP_RTVALUE[i+1],BeginAPValue[i]);
        }
        if(RP_Flag[i]==1)
        {
            if(EndRPValue[i]-BeginRPValue[i]>RPStep)
                BeginRPValue[i]+=RPStep;
            else if(BeginRPValue[i]-EndRPValue[i]>RPStep)
                BeginRPValue[i]-=RPStep;
            else
            {
                BeginRPValue[i]=EndRPValue[i];
                RP_Flag[i]=0;
            }
            printf("No.%d无功值为%f\n",i+1+14,BeginRPValue[i]);
            WriteRTDB_ANA(DB_NAME_RP_RTVALUE[i+1],BeginRPValue[i]);
        }
	}
	for(i=0;i<UNIT_NUMB;i++)
       {
        if(KJ_Time[i]>0)
            KJ_Time[i]--;
        if(TJ_Time[i]>0)
            TJ_Time[i]--;  
        //printf("No.%d  KJ_Time=%d,TJ_Time=%d\n",i,KJ_Time[i],TJ_Time[i]);
        if((KJ_Flag[i]==1)&&(KJ_Time[i]==1))
        {
            WriteRTDB_IND(DB_NAME_STOP_STAT[point_KJ[i].dev_id],0);
            WriteRTDB_IND(DB_NAME_ONLINE_STAT[point_KJ[i].dev_id],1);
            WriteRTDB_IND(DB_NAME_STOP_STA[point_KJ[i].dev_id],0);
            WriteRTDB_IND(DB_NAME_ONLINE_STA[point_KJ[i].dev_id],1);
            WriteRTDB_IND(DB_NAME_STARTING[point_KJ[i].dev_id],0);
            KJ_Flag[i]=0; 
            new_value.ind.rt_state = 1;
            rw_flag = RW_UPDATE_TIME | RW_RT_VALUE;
            SetPtValueIostById( &point_KJ[i],  new_value, iost, update_time, rw_flag );
            printf("\nNo.%d Unit is generating!\n",point_KJ[i].dev_id+14);
        }
        else if((TJ_Flag[i]==1)&&(TJ_Time[i]==1))
        {
            WriteRTDB_IND(DB_NAME_ONLINE_STAT[point_TJ[i].dev_id],0);
            WriteRTDB_IND(DB_NAME_STOP_STAT[point_TJ[i].dev_id],1);
            WriteRTDB_IND(DB_NAME_ONLINE_STA[point_TJ[i].dev_id],0);
            WriteRTDB_IND(DB_NAME_STOP_STA[point_TJ[i].dev_id],1);
            WriteRTDB_IND(DB_NAME_STOPPING[point_KJ[i].dev_id],0);
            TJ_Flag[i]=0;
            new_value.ind.rt_state = 2;
            rw_flag = RW_UPDATE_TIME | RW_RT_VALUE;
            SetPtValueIostById( &point_TJ[i],  new_value, iost, update_time, rw_flag );
            printf("stn_id=%d,dev_id=%d,data_type=%d,pt_id=%d\n",point_TJ[i].stn_id,point_TJ[i].dev_id,
                    point_TJ[i].data_type,point_TJ[i].pt_id);            
            printf("\nNo.%d Unit is no load!\n",point_TJ[i].dev_id+14);  
        }
    }
    if((Simulate_SetPoint==1)&&(update_time.minute%5==0)&&(update_time.second==1))
    {
        if(AP_Setpoint>4400)Direction=-1;
        if(AP_Setpoint<550) Direction=1;
        if(Direction==1)
            AP_Setpoint+=100;
        else if(Direction==-1)
            AP_Setpoint-=100;
        WriteRTDB_ANA(DB_NAME_OPERATOR_SETVALUE[2],AP_Setpoint);
    }
}
    

void main(void)
{
    int head_ptr,UnitNo;
    float value;
	char passwd[20];
     
// 	if(strcmp(passwd,"Eeeeeee")!=0)
// 	{
// 		printf("请输入密码:" );
// 		scanf("%s", passwd);
// 	}
// 	if(strcmp(passwd,"Eeeeeee")!=0)
// 	{
// 		printf("你没有运行的权限!\n" );
// 		return;
// 	}

    SetModuleName("SIMULATE");
    CreateLockFile(".Simulate.lock");
    signal(SIGINT,Quit);
    if(-1==MountDB(DATABASE_NAME))
    {
		Err_log("Fail to MountDB!\n");
		exit(-1);
    } 
    lan_out_init();
    ReadDefFile();  
    ReadAGCDefFile();
    ReadAVCDefFile();
    InitData();

	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		printf("\niec104s: Fail to shmat shm_rec_addr: errno\n");
		exit(0);
	}	
    head_ptr=shm_rec_addr->head.header_ptr;
    for (;;)
    {
		printf("head_ptr=%d,shm_rec_addr->head.header_ptr=%d\n",head_ptr,shm_rec_addr->head.header_ptr);
		while(head_ptr!=shm_rec_addr->head.header_ptr)
		{
            if((shm_rec_addr->rec[head_ptr].cmd_rec.type_id==C_APOWER)||
				(shm_rec_addr->rec[head_ptr].cmd_rec.type_id==C_RPOWER))
            {
				UnitNo = shm_rec_addr->rec[head_ptr].ind_rec.dev_id; 
                value=shm_rec_addr->rec[head_ptr].cmd_rec.data.iValue;
				value/=1000;
                if(shm_rec_addr->rec[head_ptr].cmd_rec.type_id==C_APOWER)
                {  
                    point_AP[UnitNo-1].stn_id=shm_rec_addr->rec[head_ptr].ind_rec.stn_id;
                    point_AP[UnitNo-1].dev_id=shm_rec_addr->rec[head_ptr].ind_rec.dev_id; 
                    point_AP[UnitNo-1].data_type=shm_rec_addr->rec[head_ptr].ind_rec.data_type;
                    point_AP[UnitNo-1].pt_id=shm_rec_addr->rec[head_ptr].ind_rec.pt_id; 
                    BeginAPValue[UnitNo-1]=ReadDB_ANA(DB_NAME_AP_RTVALUE[UnitNo]);
                    EndAPValue[UnitNo-1]=value;
                    AP_Flag[UnitNo-1]=1;
                }
                else
                {
                    point_RP[UnitNo-1].stn_id=shm_rec_addr->rec[head_ptr].ind_rec.stn_id;
                    point_RP[UnitNo-1].dev_id=UnitNo;
                    point_RP[UnitNo-1].data_type=shm_rec_addr->rec[head_ptr].ind_rec.data_type;
                    point_RP[UnitNo-1].pt_id=shm_rec_addr->rec[head_ptr].ind_rec.pt_id;
                    BeginRPValue[UnitNo-1]=ReadDB_ANA(DB_NAME_RP_RTVALUE[UnitNo]);
                    EndRPValue[UnitNo-1]=value;
                    RP_Flag[UnitNo-1]=1;
                }
                printf("\nNo.%d Unit 有功(无功)令！目标值为%f\n",UnitNo,value);
            }
            else  if(shm_rec_addr->rec[head_ptr].cmd_rec.type_id==C_UNIT)
            {
				UnitNo = shm_rec_addr->rec[head_ptr].ind_rec.dev_id;
                if(shm_rec_addr->rec[head_ptr].cmd_rec.cmd_status.opr==23)
                {
                    point_KJ[UnitNo-1].stn_id=shm_rec_addr->rec[head_ptr].ind_rec.stn_id;
                    point_KJ[UnitNo-1].dev_id=shm_rec_addr->rec[head_ptr].ind_rec.dev_id;
                    point_KJ[UnitNo-1].data_type=shm_rec_addr->rec[head_ptr].ind_rec.data_type;
                    point_KJ[UnitNo-1].pt_id=shm_rec_addr->rec[head_ptr].ind_rec.pt_id;
                    KJ_Flag[UnitNo-1]=1; 
                    KJ_Time[UnitNo-1]=25;
                    WriteRTDB_IND(DB_NAME_STARTING[UnitNo],1);  
                    printf("\nNo.%d Unit Start Command!\n",point_KJ[UnitNo-1].dev_id);
                }
                else if(shm_rec_addr->rec[head_ptr].cmd_rec.cmd_status.opr==26)
                {
                    point_TJ[UnitNo-1].stn_id=shm_rec_addr->rec[head_ptr].ind_rec.stn_id;
                    point_TJ[UnitNo-1].dev_id=shm_rec_addr->rec[head_ptr].ind_rec.dev_id;
                    point_TJ[UnitNo-1].data_type=shm_rec_addr->rec[head_ptr].ind_rec.data_type;
                    point_TJ[UnitNo-1].pt_id=shm_rec_addr->rec[head_ptr].ind_rec.pt_id;
                    TJ_Flag[UnitNo-1]=1; 
                    TJ_Time[UnitNo-1]=10;
                    WriteRTDB_IND(DB_NAME_STOPPING[UnitNo],1);
                    printf("\nNo.%d Unit Stop Command!\n",point_TJ[UnitNo].dev_id);
                }	
            }
			head_ptr++;
			if(head_ptr>=MAX_SHM_RECORD_NUM)
				head_ptr=0;
		}
		
        Unit_Distribute();
        if(Number_ANA>0)
        {
			lan_out ((char *)&ml_buf_ANA,lanhd_ANA,(unsigned char)0);
			Number_ANA=0;
        }
        if(Number_IND>0)
        {
			lan_out ((char *)&ml_buf_IND,lanhd_IND,(unsigned char)0);
			Number_IND=0;
		}
#ifdef SUN_OS
        usleep(1000000);
#else
		Sleep(1000);
#endif
    }
}




