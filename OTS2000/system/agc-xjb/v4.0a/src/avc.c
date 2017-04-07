#ifdef SUN_OS
#include	<unistd.h>
#include	<sys/mman.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<errno.h>
#include	<signal.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>
#include	<sys/time.h>
#endif
#include        <math.h>

#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include    "../../../agc-xjb/v4.0a/inc/avc.h"
#include    "../../../cmd/v4.0a/inc/cmd_def.h"
#include    "../../../dms/v4.0a/inc/dms_std.h"
#include    "../../../dms/v4.0a/inc/dms_structure_def.h"
#include    "../../../dms/v4.0a/inc/os_type_def.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"

T_UNIT UNIT[UNIT_NUMB+1];
T_PLANT	PLANT[MAXSEGMENT];

const   float	RPFilterDB=10;
const   float	RPSetpDB=1;
const	int	UnitMoveTime=40; //40minutes
const   int     RPCompTime=120;
const   int     RPCompStepTime=30;
const	int	Time_2s=2;
const   int     Time_5s=5;
const   int     Time_10s=10;
const   int     Time_60s=60;
const   int   Time_5m=300;
const   int   Time_10m=600;
int Cycle=0;
int Move_Curve[MAXSEGMENT];
float AP_RtValue[UNIT_NUMB+1];
long SaveTime[MAXSEGMENT];
int AVC_Recalc[MAXSEGMENT];
int ControlState[MAXSEGMENT];
int RP_Faulty[MAXSEGMENT];

char    oldday;
char    logfilename[90]="";
char    hostname[20];
FILE            *fp;
struct tm  *ntime;

T_P_Q_Set ExcitData[MAXTYPES+1]; 

int Number_ANA=0;
ANA_MSG	ml_buf_ANA[64];
LAN_HEAD	lanhd_ANA,lanhd_IND;
int Number_IND=0;
SYS_MSG	ml_buf_IND[64];

extern int BdSms(char* sms);
extern int  LocateKeyword( FILE *, char *);
extern void CreateLockFile(char *);
extern int BroadCastSms(char* message, BOOL IsAlm);

//for parameter
float   MinReact;
float   MaxReact;
float	Unit_RP_MaxDev=40;
float   UnitRPMove=20;
float   RP_Step=25;
float   RPCompStepVal=30;
int		RP_AlarmFact;
int		VO_AlarmFact;
float   MaxVolt, MinVolt,Volt_A_Gain;

void AVC_Sleep(int Millisecond)
{
#ifdef SUN_OS
	usleep(Millisecond*1000);
#else
	Sleep(Millisecond);
#endif
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

void ReadTime()/*读实时时间*/
{
	time_t          shjian;	
	time ( &shjian );
    ntime = localtime ( &shjian );
}

void    WriteLogFile (char logtxt[])
{
    fp = fopen(logfilename, "a");
    if ( fp != NULL)
    {
        fwrite( logtxt, strlen( logtxt ),1,fp);
        fclose(fp);
    }
}

float ReadDB_ANA(char *PointName)
{
    float ReturnValue;
    // int stat;
    char err_str[80]="";
    DMS_COMMON common_aa;
    if(ReadEntryByNameStr(PointName,&common_aa)==-1)
    {
		sprintf(err_str,"No this point %s\n",PointName);
		Err_log(err_str);
		if(GetControlState()==1)
			BdSms(err_str);
		exit(0);
    }
    /*if(common_aa.var.anlg.value.status.chan_fail==1)
	return(DATA_BAD);
    stat= common_aa.var.anlg.value.status.lmt_state;
    if (stat==ANA_ALM_HI_ALARM||stat==ANA_ALM_HI_WARN)
	return(OVER_HIGH_LMT);*/
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
		if(GetControlState()==1)
			BdSms(err_str);
		exit(0);
    }
	ReturnValue=common_aa.var.anlg.value.status.chan_fail;
	return(ReturnValue);
}

float ReadDB_HighLmt(char *PointName)
{
    DMS_COMMON common_aa;
    ReadEntryByNameStr(PointName,&common_aa);
    return(common_aa.var.anlg.lmt_para.HI_WARN);
}

float ReadDB_LowLmt(char *PointName)
{
    DMS_COMMON common_aa;
    ReadEntryByNameStr(PointName,&common_aa);
    return(common_aa.var.anlg.lmt_para.LO_WARN);
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
        if(GetControlState()==1)
			BdSms(err_str);
		exit(0);
	}
    ReturnValue=common_aa.var.ind.status.state;
    return(ReturnValue);
}

int AVC_UpdateRTDB_ANA (char *PointName,float data)
{
    DMS_COMMON common_aa;
    char err_str[80]="";
    time_t  tim;
    struct tm   tmpt;
    int value;
    float OldValue=0;
    value=(int)(data*1000);
    if (ReadEntryByNameStr(PointName,&common_aa)==-1)
    {
		sprintf(err_str,"No this point %s\n",PointName);
		Err_log(err_str);
		return(-1);
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

int AVC_UpdateRTDB_IND (char *PointName,short data)
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
		return(-1);
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
    char err_str[80]="";
    COMMAND_MSG	ml_buf;
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
	ml_buf.status.result = 0;
    ml_buf.status.src=2;
    ml_buf.data.iValue=(int)(data*1000);
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
    strcpy(ml_buf.usr_login_name,"AVC");
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
    if(GetControlState()==1)
        lan_out ((char *)&ml_buf,lanhd,(unsigned char)0);
	return 0;
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

float VoltageCurve_Module(int segment)
{
    int i,tmp;
    float Value[48];
    USHORT Point_Num,Point_Num1;
    CURVE_DATA curve;
    time_t  tim;
    struct tm   tmpt;
    float	ReturnValue;
    DMS_COMMON common_aa;
    char ErrSms[100];
	
    tim=time (NULL);
    tmpt=*localtime (&tim);
    if((tmpt.tm_hour<23)||(tmpt.tm_min<59)||(tmpt.tm_sec<55))Move_Curve[segment]=0;
    if((tmpt.tm_hour==23)&&(tmpt.tm_min==59)&&(tmpt.tm_sec>55)&&(Move_Curve[segment]==0))
    { 
        GetCurvePtSegNumByNameStr( DB_NAME_MRDYQX[segment], &Point_Num1);
        GetCurvePtDataByNameStr(DB_NAME_MRDYQX[segment],&curve);
		
        if(ReadEntryByNameStr(DB_NAME_JRDYQX[segment],&common_aa)==-1)
		{
			printf("No point %s in DB!\n",DB_NAME_JRDYQX[segment]);
			sprintf(ErrSms,"No point %s in DB!",DB_NAME_JRDYQX[segment]);
			Err_log(ErrSms);
		}
        for(i=0;i<48;i++){Value[i]=curve.value[i];}
        CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value);
        
        for(i=48;i<96;i++){Value[i-48]=curve.value[i];}
        CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value);
		
        Move_Curve[segment]=1;
    }
    //if(((tmpt.tm_hour>=17)&&(tmpt.tm_hour<23))||((tmpt.tm_hour==23)&&(tmpt.tm_min<=30)))不作此限制
    {     
		tmp=ReadDB_IND(DB_NAME_Copy_MRDYQX[segment]);
		if((tmp==1)&&(PLANT[segment].RPCopyTime==0))
		{  
			GetCurvePtSegNumByNameStr( DB_NAME_TD_MRDYQX[segment], &Point_Num1);
			GetCurvePtDataByNameStr(DB_NAME_TD_MRDYQX[segment],&curve);
			
			if(ReadEntryByNameStr(DB_NAME_MRDYQX[segment],&common_aa)==-1)
			{
				printf("No point %s in DB!\n",DB_NAME_MRDYQX[segment]);
				sprintf(ErrSms,"No point %s in DB!",DB_NAME_MRDYQX[segment]);
				Err_log(ErrSms);
			}
			for(i=0;i<48;i++){Value[i]=curve.value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value);
			for(i=48;i<96;i++){Value[i-48]=curve.value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value);
			PLANT[segment].RPCopyTime=Time_5s;
		}
    }
	// printf("Current Time:%d:%d:%d,%d :%d : %d\n",tmpt.tm_year+1900,tmpt.tm_mon+1,tmpt.tm_mday,tmpt.tm_hour,tmpt.tm_min,tmpt.tm_sec);
	
	tmp=ReadDB_IND(DB_NAME_Copy_JRDYQX[segment]);
    if((tmp==1)&&(PLANT[segment].JR_RPCopyTime==0))
    {  
		GetCurvePtSegNumByNameStr( DB_NAME_TD_JRDYQX[segment], &Point_Num1);
		GetCurvePtDataByNameStr(DB_NAME_TD_JRDYQX[segment],&curve);
		
		if(ReadEntryByNameStr(DB_NAME_JRDYQX[segment],&common_aa)==-1)
		{
			printf("No point %s in DB!\n",DB_NAME_JRDYQX[segment]);
          		sprintf(ErrSms,"No point %s in DB!",DB_NAME_JRDYQX[segment]);
				Err_log(ErrSms);
		}
		for(i=0;i<48;i++){Value[i]=curve.value[i];}
		CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value);
		for(i=48;i<96;i++){Value[i-48]=curve.value[i];}
		CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value);
		PLANT[segment].JR_RPCopyTime=Time_5s;
    }
	
    GetCurvePtSegNumByNameStr( DB_NAME_JRDYQX[segment], &Point_Num);
    GetCurvePtDataByNameStr(DB_NAME_JRDYQX[segment],&curve);
    PLANT[segment].CurvePointNum=Point_Num;
	if((((tmpt.tm_hour<1)&&(tmpt.tm_min<20))||((tmpt.tm_hour==23)&&(tmpt.tm_min>45)))&&(PLANT[segment].FirstRun==0))
	{
		for(i=0;i<Point_Num-1;i++)
			PLANT[segment].Curve[i]=curve.value[i];
	}
	else if((tmpt.tm_hour<1)&&(tmpt.tm_min<20)&&(PLANT[segment].FirstRun==1))
	{
		for(i=0;i<Point_Num-1;i++)
			PLANT[segment].Curve[i]=curve.value[i];
		PLANT[segment].Curve[Point_Num-1]=curve.value[0];
	}
	else
	{
		for(i=0;i<Point_Num;i++)
			PLANT[segment].Curve[i]=curve.value[i];
	}
    i=tmpt.tm_hour*(Point_Num/24)+tmpt.tm_min*Point_Num/(24*60)-1;
	if(i<0)
		i=Point_Num-1;
    ReturnValue=PLANT[segment].Curve[i];
    return(ReturnValue);
}

float Unit_OELimit(int UnitNumber, float ActPower)
{
    int i1,i2,UType;
    float f1,f2,f3;
    float AP_Fact,Interpol_fact;
    float RP_MaxFact,RP_MaxLim;
	
    RP_MaxLim = 0;
    UType = UNIT[UnitNumber].Type;
	
    if (ExcitData[UType].S_Total < 0) {printf("Type %d S_Total error!\n",UType);exit(0);}
    AP_Fact = ActPower / ExcitData[UType].S_Total;
    if (AP_Fact>1) AP_Fact = 1;
    if (AP_Fact<0) AP_Fact = 0;
	
    i1 = 1;
    i2 = 0;
	while ((i1<MAXPQPOINTS) && (i2==0)) {
        f1 = ExcitData[UType].Voltage.AP[i1];
        f2 = ExcitData[UType].Voltage.AP[i1+1];
        if ((AP_Fact<=f1) && (AP_Fact>=f2)) {
			i2 = i1;
			f3 = f2 - f1;
        }
        i1++;
	}
	
	if ((i2>0) && (f3<0)) { 
		Interpol_fact = (AP_Fact-f1) / f3;
		f1 = ExcitData[UType].Voltage.RP_OE[i2];
		f2 = ExcitData[UType].Voltage.RP_OE[i2+1];
		RP_MaxFact = f1 + ((f2 - f1) * Interpol_fact); 
		RP_MaxLim = RP_MaxFact * ExcitData[UType].S_Total;
    }
    return(RP_MaxLim); 
}

float Unit_UELimit(int UnitNumber, float ActPower)
{
	int i1,i2,UType;
    float f1,f2,f3;
    float AP_Fact,Interpol_fact;
    float RP_MaxFact,RP_MinLim;
	
    RP_MinLim = 0;
    UType = UNIT[UnitNumber].Type;
	
	if (ExcitData[UType].S_Total < 0) {printf("Type %d S_Total error!\n",UType);exit(0);}
    AP_Fact = ActPower / ExcitData[UType].S_Total;
    if (AP_Fact>1) AP_Fact = 1;
    if (AP_Fact<0) AP_Fact = 0;
	
	i1 = 1;
    i2 = 0;
	while ((i1<MAXPQPOINTS) && (i2==0)) {
        f1 = ExcitData[UType].Voltage.AP[i1];
        f2 = ExcitData[UType].Voltage.AP[i1+1];
        if ((AP_Fact<=f1) && (AP_Fact>=f2)) {
			i2 = i1;
			f3 = f2 - f1;
        }
        i1++;
	}
	
	if ((i2>0) && (f3<0)) { 
		Interpol_fact = (AP_Fact-f1) / f3;
		f1 = ExcitData[UType].Voltage.RP_UE[i2];
		f2 = ExcitData[UType].Voltage.RP_UE[i2+1];
		RP_MaxFact = f1 + ((f2 - f1) * Interpol_fact); 
		RP_MinLim = RP_MaxFact * ExcitData[UType].S_Total;
    }
    return(RP_MinLim); 
}

void Load_AVC_DEF(void)
{
    int	TypPos,i;
    char strTemp1[20],strTemp2[20],strTemp3[20],strTemp4[20];
    char FileName[100];
	char err_str[80]="";
    FILE	*fp;
	
    for(TypPos=1;TypPos<=MAXTYPES;TypPos++)
    {
#ifdef CASCADE
        strcpy(FileName,"/home/ems/h9000/agc-xjb-cas/");
#else
		strcpy(FileName,"/home/ems/h9000/agc-xjb/");
#endif
        sprintf(strTemp1,"AVC_DEF_%d.TXT",TypPos);
        strcat(FileName,strTemp1);
        if((fp=fopen(FileName,"r"))==NULL)
        {
            printf("Cannot open %s\n",FileName);
			sprintf(err_str, "Fail to open %s", FileName);
            Err_log(err_str);
            exit(0);
        }
		
        fscanf(fp,"%s %s %s ",strTemp1,strTemp2,strTemp3);  
        if((strcmp(strTemp1,"APP_POWER")==0)&&(strcmp(strTemp2,"=")==0))
            ExcitData[TypPos].S_Total=(float)atof(strTemp3);
		
        fscanf(fp,"%s %s %s %s ",strTemp1,strTemp2,strTemp3,strTemp4); 
        if(strcmp(strTemp1,"ROW_POS")!=0||strcmp(strTemp2,"AP_FACT")!=0||strcmp(strTemp3,"RP_OE_FACT")!=0||strcmp(strTemp4,"RP_UE_FACT")!=0)
        {
            printf("Read %s error!\n",FileName);
			sprintf(err_str, "Fail to open %s", FileName);
            Err_log(err_str);
            exit(0);
        }
		
        i=1;
        while(fscanf(fp,"%s %s %s %s ",strTemp1,strTemp2,strTemp3,strTemp4)!=EOF)
        {
            ExcitData[TypPos].Voltage.AP[i]=(float)atof(strTemp2);
            ExcitData[TypPos].Voltage.RP_OE[i]=(float)atof(strTemp3);
            ExcitData[TypPos].Voltage.RP_UE[i]=(float)atof(strTemp4);
			// printf("%d,%f,%f,%f\n",i,ExcitData[TypPos].Voltage.AP[i],ExcitData[TypPos].Voltage.RP_OE[i],
			//		 ExcitData[TypPos].Voltage.RP_UE[i]);
            i+=1;
        }
        fclose(fp);	 
    }  
}


void AVC_segment()
{  //sengment:
    int i;
	
	for(i=1;i<=4;i++)
		UNIT[i].segment=0;
   
	for(i=5;i<=8;i++)
		UNIT[i].segment=0;
}

void Statistics_Unit(int segment)	
{
    int i;
    float Plant_P_Sum,Plant_RP_Sum;
	
    PLANT[segment].P_Sum_AVC=0;
    PLANT[segment].RP_Sum=0;
    PLANT[segment].RP_Sum_AVC=0;
    PLANT[segment].RP_Sum_NotAVC=0;
    PLANT[segment].Num_RP_Avail=0;
    PLANT[segment].GenNumb=0;
    PLANT[segment].P_Sum=0;
    Plant_P_Sum=0;
    Plant_RP_Sum=0;
	
    for (i=1; i<=UNIT_NUMB;i++)
    {
		
        if(UNIT[i].segment!=segment)continue;
		
        UNIT[i].RP_Avail=0;
        
		if(UNIT[i].Status==UNIT_IN_GEN)
        {
            PLANT[segment].GenNumb+=1;
            PLANT[segment].RP_Sum+=UNIT[i].RP_RtValue;
            PLANT[segment].P_Sum+=UNIT[i].AP_RtValue; 
        }
		
        if((UNIT[i].RP_Sel==1)&&(UNIT[i].RP_Permit==1 )&&(UNIT[i].Status==UNIT_IN_GEN))
        {
            UNIT[i].RP_Avail=1;
            PLANT[segment].RP_Sum_AVC+=UNIT[i].RP_RtValue;
            PLANT[segment].Num_RP_Avail+=1;
            if(UNIT[i].ReduceLoad==0)
                PLANT[segment].P_Sum_AVC+=UNIT[i].AP_RtValue;
        }
        else
            PLANT[segment].RP_Sum_NotAVC+=UNIT[i].RP_RtValue;  
        AVC_UpdateRTDB_IND(DB_NAME_RP_CZKT[i],(short)UNIT[i].RP_Avail); 
        if(UNIT[i].RP_Avail==0)
			AVC_UpdateRTDB_ANA(DB_NAME_UNIT_SETPOINT[i],UNIT[i].RP_RtValue);

		UNIT[i].Last_RP_Avail=UNIT[i].RP_Avail;
    }   
    PLANT[segment].Last_RP_MODE=PLANT[segment].RP_MODE;
    PLANT[segment].Last_Close_Loop=PLANT[segment].Close_Loop;      
    if(PLANT[segment].FirstRun==1)
        PLANT[segment].Last_RP_Sum_NotAVC=PLANT[segment].RP_Sum_NotAVC;
    
	AVC_UpdateRTDB_ANA(DB_NAME_RP_Sum[segment],PLANT[segment].RP_Sum);
	
}

void ReadAVC_FromRTDB(int segment)
{
    int i,Sel_Bus,tmp,status;
    float CurveValue,OELimit,UELimit,RP_Min,RP_Max;
    float Sum_OELimit,Sum_UELimit,Sum_RP_Max,Sum_RP_Min;
    char ErrSms[100];
    USHORT status1;
    char    pairhostname[20];
    long Second;
	
    CurveValue=VoltageCurve_Module(segment);  
    PLANT[segment].RP_Faulty=0;
    PLANT[segment].AP_Faulty=0;
    RP_Min=0;
    RP_Max=0;
	
    for(i=1;i<=UNIT_NUMB;i++)
    { 
        if(UNIT[i].segment!=segment)continue;
        UNIT[i].Maint_Status=ReadDB_IND(DB_NAME_Maint_Status[i]);
        UNIT[i].RP_RtValue=ReadDB_ANA(DB_NAME_RP_RTVALUE[i]);
        
        UNIT[i].AP_RtValue=ReadDB_ANA(DB_NAME_AP_RTVALUE[i]);
		
        UNIT[i].RP_Sel=ReadDB_IND(DB_NAME_UNIT_AVCSEL[i]); 
        UNIT[i].Status=0;
        tmp=ReadDB_IND(DB_NAME_ONLINE_STA[i]);
        if(tmp==1)
			UNIT[i].Status=UNIT_IN_GEN; 
		tmp=ReadDB_IND(DB_NAME_STOP_STA[i]);
		if(tmp==1)
			UNIT[i].Status=0; 
        UNIT[i].RP_Permit=ReadDB_IND(DB_NAME_RP_AVAIL[i]);
        if(UNIT[i].Maint_Status==1)
            UNIT[i].Status=0;
        tmp=ReadDB_ANA_Chan(DB_NAME_AP_RTVALUE[i]);
        if(tmp==1)
        {
            UNIT[i].AP_Faulty=1;
            if(UNIT[i].Status==UNIT_IN_GEN)
				PLANT[segment].AP_Faulty=1;
        }
        tmp=ReadDB_ANA_Chan(DB_NAME_RP_RTVALUE[i]);
        if(tmp==1)
        {
			//    printf("No.%d reactive power data bad!\n",i);
            UNIT[i].RP_Faulty=1;
            if(UNIT[i].Status==UNIT_IN_GEN)
            {
				PLANT[segment].RP_Faulty=1;
				if(RP_Faulty[segment]==0)
				{
					sprintf(ErrSms,"%s:%d号机无功通道故障退AVC",PLANT[segment].English_Name,i);
					BroadCastSms(ErrSms,TRUE);
               	}
            }
        }  
        if(UNIT[i].Status!=UNIT_IN_GEN)
        {
            UNIT[i].AP_RtValue=0;
            UNIT[i].RP_RtValue=0;
        }
        UNIT[i].RP_Min=ReadDB_LowLmt(DB_NAME_RP_RTVALUE[i]);
        UNIT[i].RP_Max=ReadDB_HighLmt(DB_NAME_RP_RTVALUE[i]);
        OELimit=Unit_OELimit(i, UNIT[i].AP_RtValue);  
        UELimit=Unit_UELimit(i, UNIT[i].AP_RtValue);
        
        if(OELimit<UNIT[i].RP_Max)UNIT[i].RP_Max=OELimit;
        if(UELimit>UNIT[i].RP_Min)UNIT[i].RP_Min=UELimit;
        RP_Min+=UNIT[i].RP_Min;
        RP_Max+=UNIT[i].RP_Max;
        UNIT[i].ReduceLoad=ReadDB_IND(DB_NAME_ReduceLoad[i]);
        if(fabs(UNIT[i].RP_Min-UNIT[i].Last_RP_Min)>RPFilterDB)
        {
            UNIT[i].Last_RP_Min=UNIT[i].RP_Min;
            PLANT[segment].Lim_Change=1;
        }
        if(fabs(UNIT[i].RP_Max-UNIT[i].Last_RP_Max)>RPFilterDB)
        {
            UNIT[i].Last_RP_Max=UNIT[i].RP_Max;
            PLANT[segment].Lim_Change=1;   
        }
    }   
    RP_Faulty[segment]=PLANT[segment].RP_Faulty;
    Statistics_Unit(segment); 
    
    Sel_Bus=ReadDB_IND(DB_NAME_SEL_BUS[segment]);
    PLANT[segment].Sel_Bus=Sel_Bus; 
	
	if (segment==0)
		PLANT[segment].VO_RtValue=ReadDB_ANA(DB_NAME_VO_RTVALUE[Sel_Bus]);
	else
		PLANT[segment].VO_RtValue=ReadDB_ANA(DB_NAME_VO_RTVALUE[Sel_Bus+2]);
	AVC_UpdateRTDB_ANA(DB_NAME_VO_REACT_VALUE[segment],PLANT[segment].VO_RtValue);
	
	if(PLANT[segment].VO_RtValue==DATA_BAD)
        PLANT[segment].VO_Faulty=1;  
	
    PLANT[segment].RP_MODE=ReadDB_IND(DB_NAME_QC_AVC[segment]); 
    
	
    PLANT[segment].Close_Loop=ReadDB_IND(DB_NAME_CLOSED_LOOP[segment]);
    PLANT[segment].VO_Regulation=ReadDB_IND(DB_NAME_VO_REGULATION[segment]);
    PLANT[segment].VO_Curve=ReadDB_IND(DB_NAME_VO_CURVE[segment]);
    PLANT[segment].RP_Setting=ReadDB_IND(DB_NAME_RP_SETTING[segment]);
    PLANT[segment].VO_MaxChange=ReadDB_ANA(DB_NAME_VO_MAXCHANGE[segment]);
    PLANT[segment].RP_MaxChange=ReadDB_ANA(DB_NAME_RP_MAXCHANGE[segment]);
    PLANT[segment].VO_Compensation=ReadDB_IND(DB_NAME_VO_COMPENSATION[segment]);
    PLANT[segment].RP_Deadband=ReadDB_ANA(DB_NAME_RP_DEADBAND[segment]);
    PLANT[segment].VO_Deadband=ReadDB_ANA(DB_NAME_VO_DEADBAND[segment]);
    PLANT[segment].Comp_Deadband=ReadDB_ANA(DB_NAME_COMP_DEADBAND[segment]);
    PLANT[segment].VO_PowGenDB=ReadDB_ANA(DB_NAME_VO_PowGenDB[segment]);
    PLANT[segment].RPAnswerYes=ReadDB_IND(DB_NAME_RP_ANSWERYES[segment]); 
	PLANT[segment].Control_Right = ReadDB_IND(DB_NAME_CTRL_PRI[segment]);

	PLANT[segment].AllocationToUnit = ReadDB_IND(DB_NAME_AllocationToUnit[segment]);
	if (PLANT[segment].FirstRun==1)
	{
		PLANT[segment].Last_AllocationToUnit=PLANT[segment].AllocationToUnit;
	}
	if (PLANT[segment].AllocationToUnit!=PLANT[segment].Last_AllocationToUnit)
	{
		PLANT[segment].RP_MODE=0;
		AVC_UpdateRTDB_IND(DB_NAME_QC_AVC[segment],0);
	}
	PLANT[segment].Last_AllocationToUnit=PLANT[segment].AllocationToUnit;

	
    if((CurveValue<525)&&(PLANT[segment].VO_Curve==1))
    {
		PLANT[segment].RP_MODE=0;
		AVC_UpdateRTDB_IND(DB_NAME_QC_AVC[segment],0);
		sprintf(ErrSms,"%s:电压曲线电压值过低退AVC!",PLANT[segment].English_Name);
		if(GetControlState()==1)
			BroadCastSms(ErrSms,TRUE);
	}
	
    if(PLANT[segment].FirstRun==1)
    {
        PLANT[segment].VO_Work=PLANT[segment].VO_RtValue;
        PLANT[segment].VO_Setpoint=PLANT[segment].VO_RtValue;
        PLANT[segment].XVO_Setpoint=PLANT[segment].VO_RtValue;
        PLANT[segment].YVO_Setpoint=PLANT[segment].VO_RtValue;
        PLANT[segment].XRP_VoltCTRL = PLANT[segment].RP_Sum;  
        PLANT[segment].XVolt_Output=PLANT[segment].RP_Sum;
        PLANT[segment].RP_WorkSP=PLANT[segment].RP_Sum; 
        PLANT[segment].RP_Setpoint=PLANT[segment].RP_Sum;
        PLANT[segment].XRP_Setpoint=PLANT[segment].RP_Sum;
        PLANT[segment].YRP_Setpoint=PLANT[segment].RP_Sum;
		PLANT[segment].Last_Mode=5;
    }
	
    PLANT[segment].RP_Min=ReadDB_ANA(DB_NAME_RP_MIN[segment]);
    PLANT[segment].RP_Max=ReadDB_ANA(DB_NAME_RP_MAX[segment]);
    
    if(RP_Min>PLANT[segment].RP_Min)PLANT[segment].RP_Min=RP_Min;
    if(RP_Max<PLANT[segment].RP_Max)PLANT[segment].RP_Max=RP_Max;
    
    Sum_RP_Min=0;
    Sum_RP_Max=0;
    Sum_OELimit=0;
    Sum_UELimit=0;
    for(i=1;i<=UNIT_NUMB;i++)
    {
        if(UNIT[i].segment!=segment)continue;
        if(UNIT[i].RP_Avail==1)
        {
            Sum_RP_Max+=UNIT[i].RP_Max;
            Sum_RP_Min+=UNIT[i].RP_Min;
            OELimit=Unit_OELimit(i, UNIT[i].AP_RtValue);  
            UELimit=Unit_UELimit(i, UNIT[i].AP_RtValue);
            Sum_OELimit+=OELimit;
			Sum_UELimit+=UELimit;
			
        }
        else
        {
            Sum_RP_Max+=UNIT[i].RP_RtValue;
            Sum_RP_Min+=UNIT[i].RP_RtValue;
            Sum_OELimit+=UNIT[i].RP_RtValue;
			Sum_UELimit+=UNIT[i].RP_RtValue;
        }  
    }
    
    if(Sum_RP_Min>PLANT[segment].RP_Min)PLANT[segment].RP_Min=Sum_RP_Min;
    if(Sum_RP_Max<PLANT[segment].RP_Max)PLANT[segment].RP_Max=Sum_RP_Max;
	
    AVC_UpdateRTDB_ANA(DB_NAME_OELimit[segment],Sum_OELimit);
    AVC_UpdateRTDB_ANA(DB_NAME_UELimit[segment],Sum_UELimit);
    AVC_UpdateRTDB_ANA(DB_NAME_RP_XMIN[segment],PLANT[segment].RP_Min); 
    AVC_UpdateRTDB_ANA(DB_NAME_RP_XMAX[segment],PLANT[segment].RP_Max);

#ifdef CASCADE
	PLANT[segment].XRP_Setpoint=ReadDB_ANA(DB_NAME_RP_SETPOINT[segment]); 
#else
	if (PLANT[segment].Control_Right==0)
	{
		PLANT[segment].XRP_Setpoint=ReadDB_ANA(DB_NAME_RP_SETPOINT[segment]);
	}
	else if (PLANT[segment].Control_Right==1)
	{
		PLANT[segment].XRP_Setpoint=ReadDB_ANA(DB_NAME_WG_GD_CAS[segment]);
		AVC_UpdateRTDB_IND(DB_NAME_VO_REGULATION[segment],0);
        AVC_UpdateRTDB_IND(DB_NAME_VO_COMPENSATION[segment],0);
		AVC_UpdateRTDB_IND(DB_NAME_VO_CURVE[segment],0);
	}
#endif
	if((fabs(PLANT[segment].XRP_Setpoint-PLANT[segment].YRP_Setpoint)>=RPSetpDB)&&(PLANT[segment].RP_Setting==1))
	{
		PLANT[segment].YRP_Setpoint=PLANT[segment].XRP_Setpoint;
        if(PLANT[segment].YRP_Setpoint+0.5<PLANT[segment].RP_Min)
        {
			sprintf(ErrSms,"%s:无功设定越下限!",PLANT[segment].English_Name);
		
            if(GetControlState()==1)
				BroadCastSms(ErrSms,TRUE);
            PLANT[segment].YRP_Setpoint=PLANT[segment].RP_Setpoint;
            AVC_UpdateRTDB_ANA(DB_NAME_RP_SETPOINT[segment],PLANT[segment].RP_Setpoint);
        }
        else if(PLANT[segment].YRP_Setpoint>PLANT[segment].RP_Max)
        {
			sprintf(ErrSms,"%s:无功设定越上限!",PLANT[segment].English_Name);
			
            if(GetControlState()==1)
				BroadCastSms(ErrSms,TRUE);
            PLANT[segment].XRP_Setpoint=PLANT[segment].YRP_Setpoint=PLANT[segment].RP_Setpoint;
            AVC_UpdateRTDB_ANA(DB_NAME_RP_SETPOINT[segment],PLANT[segment].RP_Setpoint);
        }
		if(fabs(PLANT[segment].YRP_Setpoint-PLANT[segment].RP_Setpoint)>PLANT[segment].RP_MaxChange)
		{
			sprintf(ErrSms,"%s:无功设定越变幅!",PLANT[segment].English_Name);
 
			if(GetControlState()==1)
				BroadCastSms(ErrSms,TRUE); 
            PLANT[segment].XRP_Setpoint=PLANT[segment].YRP_Setpoint=PLANT[segment].RP_Setpoint;
            AVC_UpdateRTDB_ANA(DB_NAME_RP_SETPOINT[segment],PLANT[segment].RP_Setpoint);
        }
		else
			PLANT[segment].RP_Setpoint=PLANT[segment].YRP_Setpoint;
	}
    if(PLANT[segment].RP_Setting!=1)
    {
		PLANT[segment].RP_Setpoint=PLANT[segment].RP_Sum;
        PLANT[segment].XRP_Setpoint=PLANT[segment].RP_Sum;
        PLANT[segment].YRP_Setpoint=PLANT[segment].RP_Sum;
		PLANT[segment].VO_Compensation=0;
		AVC_UpdateRTDB_IND(DB_NAME_VO_COMPENSATION[segment],0);
        AVC_UpdateRTDB_ANA(DB_NAME_RP_SETPOINT[segment],PLANT[segment].RP_Sum);
	}
    if(((PLANT[segment].VO_Regulation==1)&&(PLANT[segment].VO_Compensation==1))
		||((PLANT[segment].VO_Regulation==1)&&(PLANT[segment].RP_Setting==1))
		||((PLANT[segment].VO_Regulation==1)&&(PLANT[segment].VO_Curve==1))
		||((PLANT[segment].VO_Compensation==1)&&(PLANT[segment].RP_Setting==1))
		||((PLANT[segment].VO_Compensation==1)&&(PLANT[segment].VO_Curve==1))
		||((PLANT[segment].RP_Setting==1)&&(PLANT[segment].VO_Curve==1)))
    {
		PLANT[segment].Mode=5;
        PLANT[segment].VO_Regulation=0;
        PLANT[segment].VO_Compensation=0;
        PLANT[segment].RP_Setting=0;
		PLANT[segment].VO_Curve =0;
        AVC_UpdateRTDB_IND(DB_NAME_VO_REGULATION[segment],0);
        AVC_UpdateRTDB_IND(DB_NAME_VO_COMPENSATION[segment],0);
        AVC_UpdateRTDB_IND(DB_NAME_RP_SETTING[segment],0);
		AVC_UpdateRTDB_IND(DB_NAME_VO_CURVE[segment],0);
		// printf("segment=%d Both VO_Regulation and VO_Compensation operated!So the two function exit!\n",segment);
    }
	
    if((PLANT[segment].RP_Setting!=1)&&(PLANT[segment].VO_Regulation!=1)&&(PLANT[segment].VO_Curve!=1)&&(PLANT[segment].VO_Compensation!=1))
    {
		PLANT[segment].Mode=5;
        PLANT[segment].Close_Loop=0;
        AVC_UpdateRTDB_IND(DB_NAME_CLOSED_LOOP[segment],0);
    }
	
    PLANT[segment].VO_Min=ReadDB_ANA(DB_NAME_VO_MIN[segment]);
    PLANT[segment].VO_Max=ReadDB_ANA(DB_NAME_VO_MAX[segment]); 
	if(PLANT[segment].VO_Min<MinVolt)PLANT[segment].VO_Min = MinVolt;
	if(PLANT[segment].VO_Max>MaxVolt)PLANT[segment].VO_Max = MaxVolt;
	
    //printf("VO_Min=%f,VO_Max=%f\n",PLANT[segment].VO_Min,PLANT[segment].VO_Max);
    if((PLANT[segment].VO_Regulation==1)||(PLANT[segment].VO_Compensation==1))
    {
		if(PLANT[segment].VO_Regulation==1)
			PLANT[segment].Mode=2;
		else
			PLANT[segment].Mode=4;
#ifdef CASCADE
		if (PLANT[segment].Control_Right==0)
		{
			PLANT[segment].XVO_Setpoint=ReadDB_ANA(DB_NAME_VO_SETPOINT[segment]);
		}
		else if (PLANT[segment].Control_Right==1)
		{
			PLANT[segment].XVO_Setpoint=ReadDB_ANA(DB_NAME_GD_VO_SETPOINT[segment]);
			AVC_UpdateRTDB_IND(DB_NAME_VO_COMPENSATION[segment],0);
			AVC_UpdateRTDB_IND(DB_NAME_RP_SETTING[segment],0);
			AVC_UpdateRTDB_IND(DB_NAME_VO_CURVE[segment],0);
		}
#else
		PLANT[segment].XVO_Setpoint=ReadDB_ANA(DB_NAME_VO_SETPOINT[segment]);
#endif
        if(fabs(PLANT[segment].XVO_Setpoint-PLANT[segment].YVO_Setpoint)>0.1)
        {
            PLANT[segment].YVO_Setpoint=PLANT[segment].XVO_Setpoint;
            if(PLANT[segment].YVO_Setpoint>PLANT[segment].VO_Max)
            {
				sprintf(ErrSms,"%s:电压设定越上限!",PLANT[segment].English_Name);
				
				if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
				PLANT[segment].XVO_Setpoint=PLANT[segment].YVO_Setpoint=PLANT[segment].VO_Setpoint;
				AVC_UpdateRTDB_ANA(DB_NAME_VO_SETPOINT[segment],PLANT[segment].VO_Setpoint);
            }
            else if(PLANT[segment].YVO_Setpoint<PLANT[segment].VO_Min)
            {
				sprintf(ErrSms,"%s:电压设定越下限!",PLANT[segment].English_Name);
				
                if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
				PLANT[segment].XVO_Setpoint=PLANT[segment].YVO_Setpoint=PLANT[segment].VO_Setpoint;
				AVC_UpdateRTDB_ANA(DB_NAME_VO_SETPOINT[segment],PLANT[segment].VO_Setpoint);
            }
            if(fabs(PLANT[segment].VO_Setpoint-PLANT[segment].YVO_Setpoint)>PLANT[segment].VO_MaxChange)
			{
				sprintf(ErrSms,"%s:电压设定越变幅!",PLANT[segment].English_Name);
				
                if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
                PLANT[segment].XVO_Setpoint=PLANT[segment].YVO_Setpoint=PLANT[segment].VO_Setpoint;
                AVC_UpdateRTDB_ANA(DB_NAME_VO_SETPOINT[segment],PLANT[segment].VO_Setpoint);
            }
            else 
                PLANT[segment].VO_Setpoint=PLANT[segment].YVO_Setpoint;
            if(PLANT[segment].VO_Setpoint>PLANT[segment].VO_Max)
				PLANT[segment].VO_Setpoint=PLANT[segment].VO_Max;   
            if(PLANT[segment].VO_Setpoint<PLANT[segment].VO_Min)
				PLANT[segment].VO_Setpoint=PLANT[segment].VO_Min; 
        }
		if(PLANT[segment].VO_Max<PLANT[segment].VO_Min)
		{
			sprintf(ErrSms,"%s:电压限值设置错误!",PLANT[segment].English_Name);
			
			if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
			PLANT[segment].RP_MODE=1;
			AVC_UpdateRTDB_IND(DB_NAME_QC_AVC[segment],0);
		}
    }
    else if(PLANT[segment].VO_Curve==1)
    {
		PLANT[segment].Mode=3;
        PLANT[segment].XVO_Setpoint=CurveValue; 
        if(fabs(PLANT[segment].XVO_Setpoint-PLANT[segment].YVO_Setpoint)>0.1)
        {
            PLANT[segment].YVO_Setpoint=PLANT[segment].XVO_Setpoint;
            if(PLANT[segment].YVO_Setpoint>PLANT[segment].VO_Max)
            {
				sprintf(ErrSms,"%s:电压曲线越上限!",PLANT[segment].English_Name);
			
                if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
				PLANT[segment].XVO_Setpoint=PLANT[segment].YVO_Setpoint=PLANT[segment].VO_Setpoint;
				AVC_UpdateRTDB_ANA(DB_NAME_VO_SETPOINT[segment],PLANT[segment].VO_Setpoint);
            }
            else if(PLANT[segment].YVO_Setpoint<PLANT[segment].VO_Min)
            {
				sprintf(ErrSms,"%s:电压曲线越下限!",PLANT[segment].English_Name);
				
                if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
				PLANT[segment].XVO_Setpoint=PLANT[segment].YVO_Setpoint=PLANT[segment].VO_Setpoint;
				AVC_UpdateRTDB_ANA(DB_NAME_VO_SETPOINT[segment],PLANT[segment].VO_Setpoint);
            }
			
            PLANT[segment].VO_Setpoint=PLANT[segment].YVO_Setpoint;
            if(PLANT[segment].VO_Setpoint>PLANT[segment].VO_Max)
				PLANT[segment].VO_Setpoint=PLANT[segment].VO_Max;   
            if(PLANT[segment].VO_Setpoint<PLANT[segment].VO_Min)
				PLANT[segment].VO_Setpoint=PLANT[segment].VO_Min; 
        } 
        AVC_UpdateRTDB_ANA(DB_NAME_VO_SETPOINT[segment],PLANT[segment].VO_RtValue); 
    }
    
    Second=time((time_t*)NULL);
    if(PLANT[segment].FirstRun==1)
		SaveTime[segment]=Second;
    if(PLANT[segment].VO_Curve==1)
    {
		if(abs(SaveTime[segment]-Second)>600)
		{
			PLANT[segment].RP_MODE=0;
			AVC_UpdateRTDB_IND(DB_NAME_QC_AVC[segment],0);
			sprintf(ErrSms,"%s:时间发生突变!",PLANT[segment].English_Name);
			if(GetControlState()==1)
				BroadCastSms(ErrSms,TRUE);
		}
    }
    SaveTime[segment]=Second;
	
    status=GetControlState();
    if(PLANT[segment].FirstRun==1)
		ControlState[segment]=status;
    if(-1!=GetDhswHostName(pairhostname))
    {
		GetHostState(pairhostname,&status1);
    }
    else
		status1=status;
	if((status1!=1)&&(status1!=2))
		status1=status;
    if((ControlState[segment]==2)&&(status==1)&&(PLANT[segment].RP_MODE==1)&&(status!=status1))
    {
		PLANT[segment].RP_MODE=0;
		AVC_UpdateRTDB_IND(DB_NAME_QC_AVC[segment],0);
		sprintf(ErrSms,"%s:主机发生切换!",PLANT[segment].English_Name);
		if(GetControlState()==1)
			BroadCastSms(ErrSms,TRUE);	
	}
	if(status!=status1)
		ControlState[segment]=status;
	
	for(i=1;i<=UNIT_NUMB;i++)
    {   
		if(UNIT[i].segment!=segment)continue;
		UNIT[i].XStatus=ReadDB_IND(DB_NAME_UNIT_STATUS[i]);
		if((PLANT[i].FirstRun!=1)&&(UNIT[i].Last_Status==1)&&(UNIT[i].Status!=1)&&(UNIT[i].XStatus!=4)&&(UNIT[i].XStatus!=5)&&(UNIT[i].Maint_Status!=1))
		{
			if(PLANT[segment].RP_MODE==1)
			{
				PLANT[segment].RP_MODE=0;
				AVC_UpdateRTDB_IND(DB_NAME_QC_AVC[segment],0);
				sprintf(ErrSms,"%s:%d号机状态异常退AVC!",PLANT[segment].English_Name,i);
				if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
			}
		}
	}

    if(PLANT[segment].RP_Setting==1)
		PLANT[segment].Mode=1;
    
    if((PLANT[segment].VO_Regulation==0)&&(PLANT[segment].VO_Compensation==0)&&(PLANT[segment].VO_Curve==0))
    {
		PLANT[segment].VO_Setpoint=PLANT[segment].VO_RtValue;
		PLANT[segment].XVO_Setpoint=PLANT[segment].VO_RtValue;
		PLANT[segment].YVO_Setpoint=PLANT[segment].VO_RtValue;
		PLANT[segment].VO_Work=PLANT[segment].VO_RtValue;
		AVC_UpdateRTDB_ANA(DB_NAME_VO_SETPOINT[segment],PLANT[segment].VO_RtValue);
    }
    
    if(PLANT[segment].RP_Faulty==1)
	{
		PLANT[segment].RP_MODE=0;
		AVC_UpdateRTDB_IND(DB_NAME_QC_AVC[segment],0);
		sprintf(ErrSms,"%s无功通道故障退AVC!",PLANT[segment].English_Name);
		if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
	}
    if(PLANT[segment].RP_MODE!=1)
    {
        PLANT[segment].Close_Loop=0;
        PLANT[segment].VO_Regulation=0;
        PLANT[segment].VO_Compensation=0;
        PLANT[segment].RP_Setting=0;
		PLANT[segment].VO_Curve=0;
        AVC_UpdateRTDB_IND(DB_NAME_CLOSED_LOOP[segment],0);
        AVC_UpdateRTDB_IND(DB_NAME_VO_REGULATION[segment],0);
        AVC_UpdateRTDB_IND(DB_NAME_VO_COMPENSATION[segment],0);
        AVC_UpdateRTDB_IND(DB_NAME_RP_SETTING[segment],0);
		AVC_UpdateRTDB_IND(DB_NAME_VO_CURVE[segment],0);
        PLANT[segment].Mode=5;
		// printf("segment=%d QC_AVC exit!All function exit!\n",segment);
    }  
    
	if((PLANT[segment].RP_MODE!=1)||(PLANT[segment].Num_RP_Avail<1))
    {
        PLANT[segment].VO_Work=PLANT[segment].VO_RtValue; 
        PLANT[segment].Mode=5;
    }
    
    if(PLANT[segment].Mode==5)
    {
        PLANT[segment].VO_Work=PLANT[segment].VO_RtValue;
        PLANT[segment].VO_Setpoint=PLANT[segment].VO_RtValue;
        PLANT[segment].XVO_Setpoint=PLANT[segment].VO_RtValue;
        PLANT[segment].YVO_Setpoint=PLANT[segment].VO_RtValue;
        PLANT[segment].XRP_VoltCTRL = PLANT[segment].RP_Sum;  
        PLANT[segment].XVolt_Output=PLANT[segment].RP_Sum;
        PLANT[segment].RP_WorkSP=PLANT[segment].RP_Sum; 
        PLANT[segment].RP_Setpoint=PLANT[segment].RP_Sum;
        PLANT[segment].XRP_Setpoint=PLANT[segment].RP_Sum;
        PLANT[segment].YRP_Setpoint=PLANT[segment].RP_Sum;
    }
    AVC_UpdateRTDB_ANA(DB_NAME_VO_RCTSETPOINT[segment],PLANT[segment].VO_Setpoint);
}

float Voltage_Control(int segment)
{
    float Volt_Power,CalcDroop,Power_Add;
    float Volt_Dev, AbsVolt_Dev, MinPow, MaxPow, MaxRPAdd;
    int VoltDevDir;
	
    Volt_Dev = 0;
    AbsVolt_Dev = 0;
    MaxRPAdd = (float)(MaxReact *0.4);  
    if ((PLANT[segment].VO_Compensation ==1) ||   
		(PLANT[segment].VO_Regulation == 1) )    
    {
        if (PLANT[segment].VO_Work < PLANT[segment].VO_Setpoint) 
            PLANT[segment].VO_Work += (float)0.02;  
        if (PLANT[segment].VO_Work > PLANT[segment].VO_Setpoint) 
            PLANT[segment].VO_Work -= (float)0.02;
 	//	printf("************VO_Work=%f,VO_Setpoint=%f,VO_RtValue=%f\n",PLANT[segment].VO_Work,
    //      PLANT[segment].VO_Setpoint,PLANT[segment].VO_RtValue);
        Volt_Dev = (float)(PLANT[segment].VO_Work - PLANT[segment].VO_RtValue);
        AbsVolt_Dev = (float)fabs(Volt_Dev); 
        //printf("Volt_Dev=%f,RP_Setpoint=%f,VO_PowGenDB=%f\n",Volt_Dev,PLANT[segment].RP_Setpoint,PLANT[segment].VO_PowGenDB);
        if (PLANT[segment].VO_Compensation == 1) 
        {
            MinPow = PLANT[segment].RP_Setpoint - PLANT[segment].VO_PowGenDB; 
            MaxPow = PLANT[segment].RP_Setpoint + PLANT[segment].VO_PowGenDB;
            if (MinPow < PLANT[segment].RP_Min) MinPow = PLANT[segment].RP_Min;
            if (MaxPow > PLANT[segment].RP_Max) MaxPow = PLANT[segment].RP_Max;
        } 
        else 
        {
            MinPow = PLANT[segment].RP_Min;
            MaxPow = PLANT[segment].RP_Max;
        }
		
        if (AbsVolt_Dev > PLANT[segment].VO_Deadband) 
        {
            if(AbsVolt_Dev >PLANT[segment].Comp_Deadband)
                PLANT[segment].VO_CTRL_Alarm = 1;
            if (PLANT[segment].RP_Sum > PLANT[segment].XRP_VoltCTRL) 
                PLANT[segment].XRP_VoltCTRL += 2;
            if (PLANT[segment].RP_Sum < PLANT[segment].XRP_VoltCTRL) 
                PLANT[segment].XRP_VoltCTRL -= 2;
			
            CalcDroop = (float)(PLANT[segment].XVoltDoopAdaptFact *
				fabs((Volt_Dev / PLANT[segment].VO_Work) * Volt_A_Gain));  
			
            if (CalcDroop > 0.3) CalcDroop = (float)0.3;
            if (PLANT[segment].XAVC_Unit_Comp == 1) CalcDroop *= (float)0.3;
            VoltDevDir = PLANT[segment].XVoltDevDir;
            if (Volt_Dev>0) VoltDevDir = 1;
            if (Volt_Dev<0) VoltDevDir = -1;
			
            if (PLANT[segment].XVoltDevDir != 0)
            {
                if (PLANT[segment].XVoltDevDir != VoltDevDir) 
                {
                    if (PLANT[segment].XVoltDoopAdaptFact > 0.2) 
                    {
                        PLANT[segment].XVoltDoopAdaptFact*= 0.75;
                        PLANT[segment].XVoltMaxDev = 0;
                    }
                }
                else 
                {
                    if (PLANT[segment].XVoltDoopAdaptFact < 1.5) 
                    {
                        if (AbsVolt_Dev >= PLANT[segment].XVoltMaxDev)
                        {
                            PLANT[segment].XVoltMaxDev = AbsVolt_Dev;
                            if (PLANT[segment].VO_CTRL_Alarm==1) 
                                PLANT[segment].XVoltDoopAdaptFact+= (float)0.01;
                            else
                                PLANT[segment].XVoltDoopAdaptFact+= (float)0.0025;
                        }
                    }
                }
            }
            PLANT[segment].XVoltDevDir = VoltDevDir;
            Power_Add = CalcDroop * PLANT[segment].P_Sum;  
            if (Power_Add > MaxRPAdd) Power_Add = MaxRPAdd;
            if (Volt_Dev>0) 
                PLANT[segment].XVolt_Output = PLANT[segment].XRP_VoltCTRL + Power_Add;
            else
                PLANT[segment].XVolt_Output = PLANT[segment].XRP_VoltCTRL - Power_Add;
 		//	  printf("in Voltage_Control:XVolt_Output=%f,XRP_VoltCTRL=%f,Power_Add=%f,MinPow=%f,MaxPow=%f\n",PLANT[segment].XVolt_Output,
 		//	     PLANT[segment].XRP_VoltCTRL, Power_Add,MinPow,MaxPow);
        } 
        else  
            PLANT[segment].VO_CTRL_Alarm = 0;
        if (PLANT[segment].XVolt_Output > MaxPow) 
            PLANT[segment].XVolt_Output = MaxPow;
		
        if (PLANT[segment].XVolt_Output < MinPow) 
            PLANT[segment].XVolt_Output = MinPow;
		
        PLANT[segment].VO_Deviation = AbsVolt_Dev;
		
        if ((PLANT[segment].VO_Regulation==1)&&(AbsVolt_Dev <= PLANT[segment].VO_Deadband))
            PLANT[segment].XVolt_Output = PLANT[segment].RP_WorkSP;
        if ((PLANT[segment].VO_Compensation == 1)&&(AbsVolt_Dev <= PLANT[segment].Comp_Deadband))
            PLANT[segment].XVolt_Output = PLANT[segment].RP_WorkSP;
		
        Volt_Power = PLANT[segment].XVolt_Output;
		
        if((PLANT[segment].VO_Compensation ==1)&&(PLANT[segment].FirstRun==0)&&(PLANT[segment].XVolt_Output!=PLANT[segment].Last_XVolt_Output)&&((PLANT[segment].XVolt_Output==MinPow)||(PLANT[segment].XVolt_Output==MaxPow)))
			PLANT[segment].CompLimPos=1;
        PLANT[segment].Last_XVolt_Output=PLANT[segment].XVolt_Output;
    } 
    else
    {
        PLANT[segment].XRP_VoltCTRL = PLANT[segment].RP_Sum;
        PLANT[segment].XVolt_Output = PLANT[segment].RP_WorkSP;
        PLANT[segment].VO_CTRL_Alarm = 0;
        PLANT[segment].XVoltDevDir = 0;
        PLANT[segment].XVoltDoopAdaptFact = 1.0;
        PLANT[segment].XVoltMaxDev = 0.0;
        Volt_Power = PLANT[segment].XVolt_Output;
    }  
    
//	printf("in Voltage_Cntrol:return value is %f\n",Volt_Power);
	return Volt_Power;  
}

void AVC_Calculation(int segment, int AVC_Unit_Run, float AP_Joint, float RP_Joint)
{
    int NumJCRun,NumAVC,UnitPos,i1,i2;
    int EqDis,LimVar,AVC_COS_PHI;
    float ReactVar,UnitSetp,SetpJCAP,SetpJCRP;
    float RP_JC_Add,UnitAVCSetp[UNIT_NUMB+1],temp;
    int UnitAVC[UNIT_NUMB+1];
    int Limited[UNIT_NUMB+1]={0};
    int	UnitNum[UNIT_NUMB+1]={0};
    float UnitRPValue[UNIT_NUMB+1],jzmp[UNIT_NUMB+1];
    int Num1,Num2,ComputeNum;
    NumJCRun = AVC_Unit_Run;
    SetpJCAP = AP_Joint;
    SetpJCRP = RP_Joint;
    RP_JC_Add = RP_Joint;
    NumAVC = 0;
    i2 = 0;
    LimVar = 1;
    AVC_COS_PHI = 0;
	
    PLANT[segment].RP_Dev=RP_Joint-PLANT[segment].RP_Sum_AVC;
    printf("\nsegment=%d,AVC_Unit_Run=%d,AP_Joint=%f,RP_Joint=%f\n",segment,AVC_Unit_Run,AP_Joint,RP_Joint);
    while ((LimVar == 1) && (i2 < UNIT_NUMB) && (NumJCRun > 0)) 
    {
        i2++;
        LimVar = 0;
        if (SetpJCAP < 10) 
        {  
			EqDis = 1;
			if (NumJCRun > 0)   
				ReactVar = SetpJCRP / NumJCRun; 
			else 
				ReactVar = 0;
        }
        else
        {
            EqDis = 0;
            ReactVar = SetpJCRP / SetpJCAP;   
        }
		
        for (i1=1; i1<=UNIT_NUMB; i1++) 
        {
            if((UNIT[i1].segment != segment)||(UNIT[i1].Status!=UNIT_IN_GEN))continue;
            if(UNIT[i1].ReduceLoad==1)
            {
                UNIT[i1].XRP_Setp=0;
                UNIT[i1].RPCompMode=3;
                UNIT[i1].RPCompTime=Time_5m;
                continue;
            }
            
            if (UNIT[i1].RP_Avail==1)
            {
                if(Limited[i1] ==1)  continue;
                if (EqDis == 1) 
                    UnitSetp = (float)floor(ReactVar); 
                else 
					UnitSetp = (float)floor(UNIT[i1].AP_RtValue * ReactVar);  
				
				
				// printf("RP_Min=%f,RP_Max=%f\n",UNIT[i1].RP_Min,UNIT[i1].RP_Max);
                if (UnitSetp < UNIT[i1].RP_Min) 
                {
					UnitSetp = UNIT[i1].RP_Min;
					LimVar = 1;
					Limited[i1] = 1;
					NumJCRun--;
					SetpJCRP = SetpJCRP - UnitSetp;
					SetpJCAP = SetpJCAP - UNIT[i1].AP_RtValue;
                }
				
                if (UnitSetp > UNIT[i1].RP_Max) 
                {
					UnitSetp = UNIT[i1].RP_Max;
					LimVar = 1;
					Limited[i1] = 1;
					NumJCRun--;
					SetpJCRP = SetpJCRP - UnitSetp;
					SetpJCAP = SetpJCAP - UNIT[i1].AP_RtValue;
                }
				
                if (i2 == 1) 
                {
					if (NumAVC <=UNIT_NUMB) 
					{
						NumAVC+=1;
						UnitAVC[NumAVC] = i1;
					}
                }
                UnitAVCSetp[i1] = UnitSetp;  
                
            } 
            else 
            {
				if (UNIT[i1].Status == UNIT_IN_GEN) 
					jzmp[i1] = UNIT[i1].RP_RtValue;
            } 
        } 
    }
	
    i2 = 0;
    for (i1=1; i1<=NumAVC; i1++) 
    {
		UnitPos = UnitAVC[i1]; 
		if (fabs(UnitAVCSetp[UnitPos] - UNIT[UnitPos].RP_Setp) >= RP_Step) 
			i2++;
    }
    if (i2==NumAVC) AVC_COS_PHI = 1;
    if ((AVC_COS_PHI == 1)||(PLANT[segment].Last_Num_Avail!=PLANT[segment].Num_RP_Avail))
    {
        for (i1=1; i1<=NumAVC; i1++) 
        {
			UnitPos = UnitAVC[i1];
			jzmp[UnitPos] = UnitAVCSetp[UnitPos];  
        }
        if(PLANT[segment].Last_Num_Avail!=PLANT[segment].Num_RP_Avail)
            PLANT[segment].Last_Num_Avail=PLANT[segment].Num_RP_Avail;
    }
    else if (AVC_COS_PHI == 0)
    { 
        RP_JC_Add = RP_Joint;
        for (i1=1; i1<=NumAVC; i1++) 
        {
            if(UNIT[i1].ReduceLoad==1)continue;
            UnitPos = UnitAVC[i1];
            if(fabs(UnitAVCSetp[UnitPos] - UNIT[UnitPos].RP_Setp) >= RP_Step)   //GCL
                jzmp[UnitPos] = UnitAVCSetp[UnitPos];
            else
                jzmp[UnitPos] = UNIT[UnitPos].RP_Setp;
            if(jzmp[UnitPos]>UNIT[UnitPos].RP_Max)jzmp[UnitPos]=UNIT[UnitPos].RP_Max;
            if(jzmp[UnitPos]<UNIT[UnitPos].RP_Min)jzmp[UnitPos]=UNIT[UnitPos].RP_Min;
			
            RP_JC_Add-= jzmp[UnitPos];
        }
        for(i1=1;i1<=UNIT_NUMB;i1++)
			Limited[i1]=0;
        ComputeNum=0;  
        while((RP_JC_Add!=0)&&(ComputeNum<10))
        {   
            ComputeNum+=1;  
            for(i1=1;i1<=UNIT_NUMB;i1++)
            {
                if(UNIT[i1].segment!=segment)continue;
                if (UNIT[i1].RP_Avail!=1)continue;
                if(UNIT[i1].ReduceLoad==1)continue;
                if(UNIT[i1].AP_RtValue<1)UNIT[i1].AP_RtValue=1;
                UnitRPValue[i1]=jzmp[i1]/UNIT[i1].AP_RtValue;  
            }
            Num2=0;
            for(i1=1;i1<=UNIT_NUMB;i1++)
            {
                temp=32767.0;
                Num1=0;
                if(UNIT[i1].segment!=segment)continue;
                if ((UNIT[i1].RP_Avail!=1)||(Limited[i1]==1))continue;
                if(UNIT[i1].ReduceLoad==1)continue;
                for(i2=1;i2<=UNIT_NUMB;i2++)
                {
                    if(UNIT[i2].segment!=segment)continue;
                    if ((UNIT[i2].RP_Avail!=1)||(Limited[i2]==1))continue;
                    if(UNIT[i2].ReduceLoad==1)continue;
                    if(UnitRPValue[i2]<temp)
                    {
                        temp=UnitRPValue[i2];
                        Num1=i2;
                    }
                }
                if(Num1!=0)
                {
                    Num2+=1;
                    UnitRPValue[Num1]=32767.0;
                    UnitNum[Num2]=Num1;  
                }
            } 
			
            if(RP_JC_Add>0)
                UnitPos=UnitNum[1];
            else
                UnitPos=UnitNum[Num2]; 
            temp=jzmp[UnitPos];
            if(RP_JC_Add>RP_Step)
            {					
                if(jzmp[UnitPos]+RP_Step>UNIT[UnitPos].RP_Max)
                {
                    jzmp[UnitPos]=UNIT[UnitPos].RP_Max;
                    Limited[UnitPos]=1;
                }
                else
                    jzmp[UnitPos]+=RP_Step;
                RP_JC_Add-=jzmp[UnitPos]-temp; 
            }
            else if(RP_JC_Add<-RP_Step)
            {
                if(jzmp[UnitPos]-RP_Step<UNIT[UnitPos].RP_Min)
                {
                    jzmp[UnitPos]=UNIT[UnitPos].RP_Min;
                    Limited[UnitPos]=1;
                }
                else
                    jzmp[UnitPos]-=RP_Step;
                RP_JC_Add-=jzmp[UnitPos]-temp;
            }
            else
            {  
                if(jzmp[UnitPos]+RP_JC_Add>UNIT[UnitPos].RP_Max)
                {
                    jzmp[UnitPos]=UNIT[UnitPos].RP_Max;
                    Limited[UnitPos]=1;
                }
                else if(jzmp[UnitPos]+RP_JC_Add<UNIT[UnitPos].RP_Min)
                {
                    jzmp[UnitPos]=UNIT[UnitPos].RP_Min;
                    Limited[UnitPos]=1;
                }
                else
                    jzmp[UnitPos]+=RP_JC_Add;
                RP_JC_Add-=jzmp[UnitPos]-temp;
            }  
        }
    }       
    for(i1=1;i1<=UNIT_NUMB;i1++)
    {
        if(UNIT[i1].segment!=segment)continue;
        AP_RtValue[i1]=UNIT[i1].AP_RtValue;
		
        if(UNIT[i1].ReduceLoad==1)
        {
            UNIT[i1].XRP_Setp=0;
            printf("No.%d ReduceLoad\n",i1);
        }
        else
			UNIT[i1].XRP_Setp=jzmp[i1];
		if((UNIT[i1].RP_Avail==0)&&(UNIT[i1].Status==UNIT_IN_GEN))
            UNIT[i1].XRP_Setp=UNIT[i1].RP_RtValue;
        if(UNIT[i1].Status!=UNIT_IN_GEN)
            UNIT[i1].XRP_Setp=0;
        printf("Unit No.%d XRP_Setp=%f\n",i1,UNIT[i1].XRP_Setp);
    }
}

void AVC_Module(int segment)
{
    float RP_DeadB,DB_RP_WorkSP,RP_WorkSP,RP_Joint;
    int i,MoveTime;
    RP_WorkSP=0;
    RP_Joint=0;
    MoveTime=3;
    
    RP_DeadB = PLANT[segment].RP_Deadband;    
	
    if ((PLANT[segment].VO_Compensation == 1) ||
		(PLANT[segment].VO_Regulation == 1))
    {
        RP_WorkSP = PLANT[segment].Volt_Power;    
        DB_RP_WorkSP = RP_DeadB; 
    } 
    else 
    {
        RP_WorkSP = PLANT[segment].RP_Setpoint;
        //printf("In AVC_Module:RP_Setpoint=%f\n",PLANT[segment].RP_Setpoint);
        DB_RP_WorkSP = 5.0;
    }
	// printf("In AVC_Modue: RP_Worksp=%f,%f\n",RP_WorkSP,PLANT[segment].RP_WorkSP);
    if ((fabs(PLANT[segment].RP_WorkSP - RP_WorkSP) > DB_RP_WorkSP) ||(PLANT[segment].CompLimPos==1))
    {
        printf("The change value of reactive power work value has passed deadband or reach limit!\n");
        PLANT[segment].CompLimPos=0;
        PLANT[segment].RP_WorkSP = RP_WorkSP;  
        AVC_Recalc[segment] = 1;
#ifdef CASCADE
		if (!PLANT[segment].AllocationToUnit)
		{
			SendCommandToPLC(DB_NAME_CasToPlantValue[segment],C_OP_VALUE_SET,PLANT[segment].RP_WorkSP,0);
		}
#endif
    }
    AVC_UpdateRTDB_ANA(DB_NAME_RP_RCTSETPOINT[segment],PLANT[segment].RP_WorkSP);
	
    if(PLANT[segment].Last_Num_Avail!=PLANT[segment].Num_RP_Avail)
    {
        printf("The Number of Unit in Joint Control has changed!"); 
        AVC_Recalc[segment] = 1;
        MoveTime=1;
    }
    PLANT[segment].RP_Deviation=PLANT[segment].RP_WorkSP-PLANT[segment].RP_Sum;
    if((fabs(PLANT[segment].RP_Deviation>RP_DeadB))&&(PLANT[segment].Num_RP_Avail>0))
    {
        if(PLANT[segment].RP_Alarm_Time<Time_10m)
        {
            PLANT[segment].RP_Alarm_Time+=MoveTime;
        }
        else
        {
            AVC_UpdateRTDB_IND(DB_NAME_RP_Dev_Alm[segment],1);
        }
    }
    else
    {
        AVC_UpdateRTDB_IND(DB_NAME_RP_Dev_Alm[segment],0);
        PLANT[segment].RP_Alarm_Time=0;
    }
    if(fabs(PLANT[segment].Last_RP_Sum_NotAVC-PLANT[segment].RP_Sum_NotAVC)>10)
    {
        printf("RP not in AVC change over 10\n");
        PLANT[segment].Last_RP_Sum_NotAVC=PLANT[segment].RP_Sum_NotAVC;
        AVC_Recalc[segment] = 1;
    }
	
    if(PLANT[segment].Lim_Change==1)
    {
		printf("Limit change\n");
        PLANT[segment].Lim_Change=0;
        AVC_Recalc[segment]=1;
    }
    if((PLANT[segment].Mode!=5)&&(PLANT[segment].Last_Mode==5))
    {
		printf("AVC电厂投入功能%d!\n",PLANT[segment].Mode);	
		AVC_Recalc[segment]=1;
    }
	PLANT[segment].Last_Mode=PLANT[segment].Mode;
    for(i=1;i<=UNIT_NUMB;i++)
    {
        if(UNIT[i].segment!=segment)continue;
        if(fabs(UNIT[i].AP_RtValue-AP_RtValue[i])>=100)
        {
            printf("No.%d 有功实发值变化超过100MW",i);
            AVC_Recalc[segment]=1;
        }
    }
#ifdef CASCADE
	if (!PLANT[segment].AllocationToUnit)
	{
		return;
	}
#endif
    for(i=1;i<=UNIT_NUMB;i++)
    {
        if(UNIT[i].segment!=segment)continue;
        if(UNIT[i].RP_Avail!=1)continue;
        if(UNIT[i].Last_ReduceLoad!=UNIT[i].ReduceLoad)
        {
            UNIT[i].Last_ReduceLoad=UNIT[i].ReduceLoad; 
            if(UNIT[i].ReduceLoad==1)
            {
				AVC_Recalc[segment] =1;
				printf("Stop and Reduce!\n");
            }
        }
    }
	
    RP_Joint = PLANT[segment].RP_WorkSP - PLANT[segment].RP_Sum_NotAVC;
    if (AVC_Recalc[segment] == 1)
    {  
        AVC_Calculation(segment,PLANT[segment].Num_RP_Avail,PLANT[segment].P_Sum_AVC,RP_Joint);
    }
}

void ReadParameter(void)
{
	FILE	*fp;
#ifdef CASCADE
	char filename[64]="/home/ems/h9000/agc-xjb-cas/Parameter.DEF";
#else
	char filename[64]="/home/ems/h9000/agc-xjb/Parameter.DEF";
#endif
	char err_str[80]="";
	char keyword[30]="";
	char StrTemp[20]="";
	char StrTemp1[20]="";
	char StrTemp2[20]="";
	char StrTemp3[20]="";
	int i;
	
	if( (fp=fopen(filename, "r"))==NULL )
	{
		sprintf(err_str, "Fail to open %s", filename);
		Err_log(err_str);
		exit(0);
	}
	
	/*KEYOWRD=="Type"*/
	strcpy(keyword, "Type");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s %s\n",StrTemp1,StrTemp2);
		sprintf(StrTemp3,"%dF",i);
		if(strncmp(StrTemp1,StrTemp3,3)==0)
		{
			UNIT[i].Type=atoi(StrTemp2);
		}
		else
		{
			strcpy(err_str, "[Type] Unit No error!");
			Err_log(err_str);
			exit(0);
		}
		if(UNIT[i].Type>3)
		{
			strcpy(err_str, "The type of Unit error!");
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="MinReact" */
	strcpy(keyword, "MinReact");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp);
	MinReact=(float)atof(StrTemp);
	
	/* KEYWORD=="MaxReact" */
	strcpy(keyword, "MaxReact");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp);
	MaxReact=(float)atof(StrTemp);
	
	/* KEYWORD=="Unit_RP_MaxDev" */
	strcpy(keyword, "Unit_RP_MaxDev");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp);
	Unit_RP_MaxDev=(float)atof(StrTemp);
	
	/* KEYWORD=="UnitRPMove" */
	strcpy(keyword, "UnitRPMove");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp);
	UnitRPMove=(float)atof(StrTemp); 
	
    /* KEYWORD=="RP_Step" */
	strcpy(keyword, "RP_Step");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp);
	RP_Step=(float)atof(StrTemp);
	
	/* KEYWORD=="RPCompStepVal" */
	strcpy(keyword, "RPCompStepVal");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp);
	RPCompStepVal=(float)atof(StrTemp);
	
	/* KEYWORD=="RP_AlarmFact" */
	strcpy(keyword, "RP_AlarmFact");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp);
	RP_AlarmFact=atoi(StrTemp);
	
	/* KEYWORD=="VO_AlarmFact" */
	strcpy(keyword, "VO_AlarmFact");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp);
	VO_AlarmFact=atoi(StrTemp);
	
	/* KEYWORD=="MinVolt" */
	strcpy(keyword, "MinVolt");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp);
	MinVolt=(float)atof(StrTemp);
	
	/* KEYWORD=="MaxVolt" */
	strcpy(keyword, "MaxVolt");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp);
	MaxVolt=(float)atof(StrTemp);
	
	/* KEYWORD=="Volt_A_Gain" */
	strcpy(keyword, "Volt_A_Gain");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp);
	Volt_A_Gain=(float)atof(StrTemp);    
	
	fclose(fp);
}

void ReadDefFile(void)
{
	FILE	*fp;
#ifdef CASCADE
	char filename[40]="/home/ems/h9000/agc-xjb-cas/AVC.DEF";
#else
	char filename[40]="/home/ems/h9000/agc-xjb/AVC.DEF";
#endif
	char err_str[80]="";
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

	strcpy(keyword,"STATION_NAME");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",PLANT[i].English_Name);
		fscanf(fp,"%s\n",PLANT[i].Chinese_Name);
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
			sprintf(err_str, "No this point %s\n",DB_NAME_UNIT_STATUS[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="UNIT_AVCSEL" */
	strcpy(keyword, "UNIT_AVCSEL");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_UNIT_AVCSEL[i]);
		if(ReadEntryByNameStr(DB_NAME_UNIT_AVCSEL[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_UNIT_AVCSEL[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_UNIT_AVCSEL[i]);
			Err_log(err_str);
			exit(0);
		}
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
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_AVAIL[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="UNIT_AVCON" */
	strcpy(keyword, "RP_CZKT");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_CZKT[i]);
		if(ReadEntryByNameStr(DB_NAME_RP_CZKT[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RP_CZKT[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_CZKT[i]);
			Err_log(err_str);
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
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_RTVALUE[i]);
			Err_log(err_str);
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
			sprintf(err_str, "No this point %s\n",DB_NAME_AP_RTVALUE[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="SEL_BUS" */
	strcpy(keyword, "SEL_BUS");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_SEL_BUS[i]);
		if(ReadEntryByNameStr(DB_NAME_SEL_BUS[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_SEL_BUS[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_SEL_BUS[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="VO_REGULATION" */
	strcpy(keyword, "VO_REGULATION");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_VO_REGULATION[i]);
		if(ReadEntryByNameStr(DB_NAME_VO_REGULATION[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VO_REGULATION[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_VO_REGULATION[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	
	/* KEYWORD=="VO_CURVE" */
	strcpy(keyword, "VO_CURVE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_VO_CURVE[i]);
		if(ReadEntryByNameStr(DB_NAME_VO_CURVE[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VO_CURVE[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_VO_CURVE[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="RP_SETTING" */
	strcpy(keyword, "RP_SETTING");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_SETTING[i]);
		if(ReadEntryByNameStr(DB_NAME_RP_SETTING[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RP_SETTING[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_SETTING[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="VO_COMPENSATION" */
	strcpy(keyword, "VO_COMPENSATION");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_VO_COMPENSATION[i]);
		if(ReadEntryByNameStr(DB_NAME_VO_COMPENSATION[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VO_COMPENSATION[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_VO_COMPENSATION[i]);
			Err_log(err_str);
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
	for(i=0;i<4;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_VO_RTVALUE[i]);    
		if(ReadEntryByNameStr(DB_NAME_VO_RTVALUE[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VO_RTVALUE[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_VO_RTVALUE[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="QC_AVC" */
	strcpy(keyword, "QC_AVC");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_QC_AVC[i]); 
		if(ReadEntryByNameStr(DB_NAME_QC_AVC[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_QC_AVC[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_QC_AVC[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="OPENCLOSE_LOOP" */
	strcpy(keyword, "OPENCLOSE_LOOP");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_CLOSED_LOOP[i]); 
		if(ReadEntryByNameStr(DB_NAME_CLOSED_LOOP[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_CLOSED_LOOP[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_CLOSED_LOOP[i]);
			Err_log(err_str);
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
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_DEADBAND[i]); 
		if(ReadEntryByNameStr(DB_NAME_RP_DEADBAND[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RP_DEADBAND[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_DEADBAND[i]);
			Err_log(err_str);
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
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_VO_DEADBAND[i]); 
		if(ReadEntryByNameStr(DB_NAME_VO_DEADBAND[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VO_DEADBAND[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_VO_DEADBAND[i]);
			Err_log(err_str);
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
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_MIN[i]); 
		if(ReadEntryByNameStr(DB_NAME_RP_MIN[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RP_MIN[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_MIN[i]);
			Err_log(err_str);
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
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_MAX[i]); 
		if(ReadEntryByNameStr(DB_NAME_RP_MAX[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RP_MAX[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_MAX[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="RP_XMIN" */
	strcpy(keyword, "RP_XMIN");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_XMIN[i]); 
		if(ReadEntryByNameStr(DB_NAME_RP_XMIN[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RP_XMIN[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_XMIN[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="RP_XMAX" */
	strcpy(keyword, "RP_XMAX");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_XMAX[i]); 
		if(ReadEntryByNameStr(DB_NAME_RP_XMAX[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RP_XMAX[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_XMAX[i]);
			Err_log(err_str);
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
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_VO_MIN[i]); 
		if(ReadEntryByNameStr(DB_NAME_VO_MIN[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VO_MIN[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_VO_MIN[i]);
			Err_log(err_str);
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
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_VO_MAX[i]); 
		if(ReadEntryByNameStr(DB_NAME_VO_MAX[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VO_MAX[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_VO_MAX[i]);
			Err_log(err_str);
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
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_VO_PowGenDB[i]); 
		if(ReadEntryByNameStr(DB_NAME_VO_PowGenDB[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VO_PowGenDB[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_VO_PowGenDB[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="RP_SETPOINT" */
	strcpy(keyword, "RP_SETPOINT");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_SETPOINT[i]); 
		if(ReadEntryByNameStr(DB_NAME_RP_SETPOINT[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RP_SETPOINT[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_SETPOINT[i]);
			Err_log(err_str);
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
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_COMP_DEADBAND[i]);
		if(ReadEntryByNameStr(DB_NAME_COMP_DEADBAND[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_COMP_DEADBAND[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_COMP_DEADBAND[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="VO_SETPOINT" */
	strcpy(keyword, "VO_SETPOINT");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_VO_SETPOINT[i]);
		if(ReadEntryByNameStr(DB_NAME_VO_SETPOINT[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VO_SETPOINT[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_VO_SETPOINT[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="UNIT_SETPOINT" */
	strcpy(keyword, "UNIT_SETPOINT");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_UNIT_SETPOINT[i]);
		if(ReadEntryByNameStr(DB_NAME_UNIT_SETPOINT[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_UNIT_SETPOINT[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_UNIT_SETPOINT[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="RPAnswerYes" */
	strcpy(keyword, "RPAnswerYes");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_ANSWERYES[i]);
		if(ReadEntryByNameStr(DB_NAME_RP_ANSWERYES[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RP_ANSWERYES[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_ANSWERYES[i]);
			Err_log(err_str);
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
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_VO_MAXCHANGE[i]);
		if(ReadEntryByNameStr(DB_NAME_VO_MAXCHANGE[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VO_MAXCHANGE[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_VO_MAXCHANGE[i]);
			Err_log(err_str);
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
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_MAXCHANGE[i]);
		if(ReadEntryByNameStr(DB_NAME_RP_MAXCHANGE[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RP_MAXCHANGE[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_MAXCHANGE[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="[RP_RctSetPoint]" */
	strcpy(keyword, "RP_RctSetPoint");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_RCTSETPOINT[i]);
		if(ReadEntryByNameStr(DB_NAME_RP_RCTSETPOINT[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RP_RCTSETPOINT[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_RCTSETPOINT[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="[VO_RctSetPoint]" */
	strcpy(keyword, "VO_RctSetPoint");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_VO_RCTSETPOINT[i]);
		if(ReadEntryByNameStr(DB_NAME_VO_RCTSETPOINT[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VO_RCTSETPOINT[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_VO_RCTSETPOINT[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="RP_FAIL" */
	strcpy(keyword, "RP_FAIL");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_FAIL[i]);
		if(ReadEntryByNameStr(DB_NAME_RP_FAIL[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RP_FAIL[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_FAIL[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="FR_FAIL" */
	strcpy(keyword, "VO_FAIL");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_VO_FAIL[i]);
		if(ReadEntryByNameStr(DB_NAME_VO_FAIL[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VO_FAIL[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_VO_FAIL[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="RP_Sum" */
	strcpy(keyword, "RP_Sum");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_Sum[i]);
		if(ReadEntryByNameStr(DB_NAME_RP_Sum[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RP_Sum[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_Sum[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="Maint_Status" */
	strcpy(keyword, "Maint_Status");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Maint_Status[i]); 
		if(ReadEntryByNameStr(DB_NAME_Maint_Status[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Maint_Status[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_Maint_Status[i]);
			Err_log(err_str);
			exit(0);
		}
	}  
	
	
	/* KEYWORD=="ReduceLoad" */
	strcpy(keyword, "ReduceLoad");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_ReduceLoad[i]); 
		if(ReadEntryByNameStr(DB_NAME_ReduceLoad[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_ReduceLoad[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_ReduceLoad[i]);
			Err_log(err_str);
			exit(0);
		}
	}  
	
	/* KEYWORD=="Unit_RP_Fail" */
	strcpy(keyword, "Unit_RP_Fail");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Unit_RP_Fail[i]); 
		if(ReadEntryByNameStr(DB_NAME_Unit_RP_Fail[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Unit_RP_Fail[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_Unit_RP_Fail[i]);
			Err_log(err_str);
			exit(0);
		}
	} 
	
	/* KEYWORD=="RP_Dev_Alm" */
	strcpy(keyword, "RP_Dev_Alm");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_Dev_Alm[i]);
		if(ReadEntryByNameStr(DB_NAME_RP_Dev_Alm[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RP_Dev_Alm[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_RP_Dev_Alm[i]);
			Err_log(err_str);
			exit(0);
		}
	}    
	
	/* KEYWORD=="Copy_MRDYQX" */
	strcpy(keyword, "Copy_MRDYQX");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Copy_MRDYQX[i]);
		if(ReadEntryByNameStr(DB_NAME_Copy_MRDYQX[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Copy_MRDYQX[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_Copy_MRDYQX[i]);
			Err_log(err_str);
			exit(0);
		}
	} 
	
	/* KEYWORD=="MLKG" */
	strcpy(keyword, "MLKG");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",DB_NAME_MLKG);
	if(ReadEntryByNameStr(DB_NAME_MLKG,&common_aa)==-1)
	{
		printf("No this point %s\n",DB_NAME_MLKG);
		sprintf(err_str,"No this point %s\n",DB_NAME_MLKG);
		Err_log(err_str);
		exit(0);
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
	
	/* KEYWORD=="OELimit" */
	strcpy(keyword, "OELimit");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_OELimit[i]);
		if(ReadEntryByNameStr(DB_NAME_OELimit[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_OELimit[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_OELimit[i]);
			Err_log(err_str);
			exit(0);
		}
	} 
	
	/* KEYWORD=="UELimit" */
	strcpy(keyword, "UELimit");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_UELimit[i]);
		if(ReadEntryByNameStr(DB_NAME_UELimit[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_UELimit[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_UELimit[i]);
			Err_log(err_str);
			exit(0);
		}
	} 
	
	/* KEYWORD=="WG_GD" */
	strcpy(keyword, "WG_GD");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_WG_GD[i]); 
		if(ReadEntryByNameStr(DB_NAME_WG_GD[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_WG_GD[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_WG_GD[i]);
			Err_log(err_str);
			exit(0);
		}
	}  
	
	/* KEYWORD=="JRDYQX" */
	strcpy(keyword, "JRDYQX");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_JRDYQX[i]);
		if(ReadEntryByNameStr(DB_NAME_JRDYQX[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_JRDYQX[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_JRDYQX[i]);
			Err_log(err_str);
			exit(0);
		}
	} 
	
	/* KEYWORD=="MRDYQX" */
	strcpy(keyword, "MRDYQX");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_MRDYQX[i]);
		if(ReadEntryByNameStr(DB_NAME_MRDYQX[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_MRDYQX[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_MRDYQX[i]);
			Err_log(err_str);
			exit(0);
		}
	} 
	
	/* KEYWORD=="TD_JRDYQX" */
	strcpy(keyword, "TD_JRDYQX");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_TD_JRDYQX[i]);
		if(ReadEntryByNameStr(DB_NAME_TD_JRDYQX[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_TD_JRDYQX[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_TD_JRDYQX[i]);
			Err_log(err_str);
			exit(0);
		}
	} 
	
	/* KEYWORD=="TD_MRDYQX" */
	strcpy(keyword, "TD_MRDYQX");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_TD_MRDYQX[i]);
		if(ReadEntryByNameStr(DB_NAME_TD_MRDYQX[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_TD_MRDYQX[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_TD_MRDYQX[i]);
			Err_log(err_str);
			exit(0);
		}
	} 
	
	/* KEYWORD=="Copy_JRDYQX" */
	strcpy(keyword, "Copy_JRDYQX");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Copy_JRDYQX[i]);
		if(ReadEntryByNameStr(DB_NAME_Copy_JRDYQX[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Copy_JRDYQX[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_Copy_JRDYQX[i]);
			Err_log(err_str);
			exit(0);
		}
	} 
	
	/* KEYWORD=="VO_REACT_VALUE" */
	strcpy(keyword, "VO_REACT_VALUE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_VO_REACT_VALUE[i]);
		if(ReadEntryByNameStr(DB_NAME_VO_REACT_VALUE[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VO_REACT_VALUE[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_VO_REACT_VALUE[i]);
			Err_log(err_str);
			exit(0);
		}
	} 

		/* KEYWORD=="WG_GD_CAS" */
		strcpy(keyword, "WG_GD_CAS");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_WG_GD_CAS[i]);
		if(ReadEntryByNameStr(DB_NAME_WG_GD_CAS[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_WG_GD_CAS[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_WG_GD_CAS[i]);
			Err_log(err_str);
			exit(0);
		}
	} 

		/* KEYWORD=="CTRL_PRI" */
		strcpy(keyword, "CTRL_PRI");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_CTRL_PRI[i]);
		if(ReadEntryByNameStr(DB_NAME_CTRL_PRI[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_CTRL_PRI[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_CTRL_PRI[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="AllocationToUnit" */
	strcpy(keyword, "AllocationToUnit");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AllocationToUnit[i]);
		if(ReadEntryByNameStr(DB_NAME_AllocationToUnit[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_AllocationToUnit[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_AllocationToUnit[i]);
			Err_log(err_str);
			exit(0);
		}
	}
#ifdef CASCADE	
	/* KEYWORD=="CasToPlantValue" */
	strcpy(keyword, "CasToPlantValue");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_CasToPlantValue[i]);
		if(ReadEntryByNameStr(DB_NAME_CasToPlantValue[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_CasToPlantValue[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_AllocationToUnit[i]);
			Err_log(err_str);
			exit(0);
		}
	}

	/* KEYWORD=="GD_VO_SETPOINT" */
	strcpy(keyword, "GD_VO_SETPOINT");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_GD_VO_SETPOINT[i]);
		if(ReadEntryByNameStr(DB_NAME_GD_VO_SETPOINT[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_GD_VO_SETPOINT[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_GD_VO_SETPOINT[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
#endif
	fclose(fp);
}

//Supervision of units
void Unit_Supervision(int segment)
{
    int i1,INC_Dir,Unit_Comp,CompNextStep,Status;
#ifdef CASCADE
	if (!PLANT[segment].AllocationToUnit)
	{
		return;
	}
#endif   
    Unit_Comp=0;
    CompNextStep=1;
    if(PLANT[segment].RPAnswerYesTime>0)
    {
        PLANT[segment].RPAnswerYesTime--;
        if(PLANT[segment].RPAnswerYesTime==1)
        {
            PLANT[segment].RPAnswerYes=0;
            AVC_UpdateRTDB_IND(DB_NAME_RP_ANSWERYES[segment],0);
        }
    }
    else 
	{
		if(PLANT[segment].RPAnswerYesTime==0)
		{
			Status=ReadDB_IND(DB_NAME_RP_ANSWERYES[segment]);
			if(Status!=0)
				AVC_UpdateRTDB_IND(DB_NAME_RP_ANSWERYES[segment],0);
		}
	}
	
	if(PLANT[segment].RPCopyTime>0)
    {
        PLANT[segment].RPCopyTime--;
        if(PLANT[segment].RPCopyTime==1)
        {
			PLANT[segment].RPCopyTime=0;
			AVC_UpdateRTDB_IND(DB_NAME_Copy_MRDYQX[segment],0);
		}
	}
	else 
	{
		if(PLANT[segment].RPCopyTime==0)
		{
			Status=ReadDB_IND(DB_NAME_Copy_MRDYQX[segment]);
			if(Status!=0)
				AVC_UpdateRTDB_IND(DB_NAME_Copy_MRDYQX[segment],0);
		}
	}
	
	if(PLANT[segment].JR_RPCopyTime>0)
    {
        PLANT[segment].JR_RPCopyTime--;
        if(PLANT[segment].JR_RPCopyTime==1)
        {
			PLANT[segment].JR_RPCopyTime=0;
			AVC_UpdateRTDB_IND(DB_NAME_Copy_JRDYQX[segment],0);
		}
	}
	else 
	{
		if(PLANT[segment].JR_RPCopyTime==0)
		{
			Status=ReadDB_IND(DB_NAME_Copy_JRDYQX[segment]);
			if(Status!=0)
				AVC_UpdateRTDB_IND(DB_NAME_Copy_JRDYQX[segment],0);
		}
	}
	
	
    if((PLANT[segment].RP_MODE==1)&&(PLANT[segment].Mode!=5))
    {   
        if(PLANT[segment].XAVC_Unit_Comp==0)
        {
            if(fabs(PLANT[segment].RP_Sum-PLANT[segment].RP_WorkSP)>RP_AlarmFact*PLANT[segment].RP_Deadband)
            {   
                if(PLANT[segment].RP_Fail_Time<Time_60s)
                    PLANT[segment].RP_Fail_Time++;
                else
                    AVC_UpdateRTDB_IND(DB_NAME_RP_FAIL[segment],1);
            }
            else
            {
                PLANT[segment].RP_Fail_Time=0;
                AVC_UpdateRTDB_IND(DB_NAME_RP_FAIL[segment],0);
            }
        }
        else
        {
            PLANT[segment].RP_Fail_Time=0;
            AVC_UpdateRTDB_IND(DB_NAME_RP_FAIL[segment],0);
        }
    }
    else
    {
        PLANT[segment].RP_Fail_Time=0;
        AVC_UpdateRTDB_IND(DB_NAME_RP_FAIL[segment],0);
    }
    
    if((PLANT[segment].RP_MODE==1)&&((PLANT[segment].VO_Regulation==1)||(PLANT[segment].VO_Compensation==1)))
    {
        if(PLANT[segment].Num_RP_Avail>0)
        {   
            if(fabs(PLANT[segment].VO_Work-PLANT[segment].VO_RtValue)>VO_AlarmFact*PLANT[segment].VO_Deadband)
            {
                if(PLANT[segment].VO_Fail_Time<Time_60s)
                    PLANT[segment].VO_Fail_Time++;
                else
                    AVC_UpdateRTDB_IND(DB_NAME_VO_FAIL[segment],1);
            }
            else
            {
                PLANT[segment].VO_Fail_Time=0;
                AVC_UpdateRTDB_IND(DB_NAME_VO_FAIL[segment],0);
            }
        }
        else
        {
            PLANT[segment].VO_Fail_Time=0;
            AVC_UpdateRTDB_IND(DB_NAME_VO_FAIL[segment],0);
        }
    }
    else
    {
        PLANT[segment].VO_Fail_Time=0;
        AVC_UpdateRTDB_IND(DB_NAME_VO_FAIL[segment],0);
    }
    
    for (i1=1; i1<=UNIT_NUMB; i1++) 
    {
        if(UNIT[i1].segment!=segment)continue;
		UNIT[i1].Last_Status = UNIT[i1].Status;
        if(UNIT[i1].ReduceLoad==2)
        {
            UNIT[i1].RP_Avail=0;
            AVC_UpdateRTDB_IND(DB_NAME_UNIT_AVCSEL[i1],0);
            UNIT[i1].RPCompMode=0;
        }
        if(UNIT[i1].RP_FailTime>0)
        {
            UNIT[i1].RP_FailTime--;
            if(UNIT[i1].RP_FailTime==1)
            {
                UNIT[i1].RP_FailTime=0;
                AVC_UpdateRTDB_IND(DB_NAME_Unit_RP_Fail[i1],0);
            }
        }
        else
        {
			UNIT[i1].RP_FailTime=0;
			AVC_UpdateRTDB_IND(DB_NAME_Unit_RP_Fail[i1],0);
        }
        
        if (UNIT[i1].RPCompMode > 0) 
        {
            if (UNIT[i1].RPCompTime > 0) 
            {
                UNIT[i1].RPCompTime--;
                //Deactivate unit AVC compensation if setpoint reached
                if (fabs(UNIT[i1].RP_RtValue - UNIT[i1].XRP_Setp) < RPFilterDB) 
                    UNIT[i1].RPCompMode = 0;
            } 
            else 
                UNIT[i1].RPCompMode = 0;
        }
        
        if (fabs(UNIT[i1].RP_RtValue - UNIT[i1].YRP_Setp)>UnitRPMove)CompNextStep=0;
        
        //*** Setpoints for reactive power
        if (UNIT[i1].RP_Avail==1)
		{
            //Unit in AVC mode + Power Regulation
            if (PLANT[segment].Close_Loop==1) 
            {
                //Unit setpoint was changed
                if (fabs(UNIT[i1].RP_Setp - UNIT[i1].XRP_Setp) > RPSetpDB) 
                {
                    UNIT[i1].RP_Setp_Calc = 1;
                    UNIT[i1].XRP_Err_Detect.HL_Value = UNIT[i1].XRP_Setp + Unit_RP_MaxDev;
                    UNIT[i1].XRP_Err_Detect.LL_Value = UNIT[i1].XRP_Setp - Unit_RP_MaxDev;
                    if (UNIT[i1].XRP_Setp > UNIT[i1].RP_RtValue) 
                        INC_Dir = 1;
                    else 
                        INC_Dir = 0;
					
                    if ((UNIT[i1].XRP_Err_Detect.MoveTime == 0) ||
						(UNIT[i1].XRP_Err_Detect.INC_Dir != INC_Dir))
                        UNIT[i1].XRP_Err_Detect.LastValue = UNIT[i1].RP_RtValue;
                    
                    UNIT[i1].XRP_Err_Detect.MoveTime = UnitMoveTime;
                    UNIT[i1].XRP_Err_Detect.INC_Dir = INC_Dir;
					
                    UNIT[i1].RP_Setp = UNIT[i1].XRP_Setp;
                    UNIT[i1].XRP_Err_Detect.Enabled = 0;
                }
				if(UNIT[i1].Last_RP_Avail==0)
                {
                    UNIT[i1].RPCompMode=1;
                    UNIT[i1].RPCompTime=RPCompTime; 
                } 
            }
            else
            {
                //Unit in AVC mode - but unit in open loop mode
                UNIT[i1].XRP_Err_Detect.Enabled = 0;
                UNIT[i1].RPCompMode = 0;
                UNIT[i1].RPCompTime = 0;
				UNIT[i1].XRP_Err_Detect.MoveTime = UnitMoveTime;
                if (UNIT[i1].Status != UNIT_IN_GEN)
                {
                    UNIT[i1].XRP_Setp=0;
                    UNIT[i1].RP_Setp = UNIT[i1].XRP_Setp;
                } 
                else 
                {
                    UNIT[i1].RP_Setp = UNIT[i1].XRP_Setp;
                }
            }
			
            //Handling of unit control error - reactive power
            if (UNIT[i1].Status==UNIT_IN_GEN) 
            {
                //Setpoint for reactive power is not reached
                if ((UNIT[i1].RP_RtValue < UNIT[i1].XRP_Err_Detect.LL_Value) ||
					(UNIT[i1].RP_RtValue > UNIT[i1].XRP_Err_Detect.HL_Value))
                {
                    //Detection of unit moving
                    if (UNIT[i1].XRP_Err_Detect.INC_Dir == 1) 
                    {
                        //Detect unit is increasing
                        if ((UNIT[i1].RP_RtValue - UNIT[i1].XRP_Err_Detect.LastValue) > UnitRPMove)
                        {
                            UNIT[i1].XRP_Err_Detect.LastValue = UNIT[i1].RP_RtValue;
                            if (UNIT[i1].RP_RtValue < UNIT[i1].XRP_Err_Detect.HL_Value) 
                            {
                                UNIT[i1].XRP_Err_Detect.MoveTime = UnitMoveTime;
                                UNIT[i1].XRP_Err_Detect.Moving = 1;
                            }
                        }
                    } 
                    else
                    {
                        //Detect unit is decreasing
                        if ((UNIT[i1].XRP_Err_Detect.LastValue - UNIT[i1].RP_RtValue) > UnitRPMove)
                        {
                            UNIT[i1].XRP_Err_Detect.LastValue = UNIT[i1].RP_RtValue;
                            if (UNIT[i1].RP_RtValue > UNIT[i1].XRP_Err_Detect.LL_Value) 
                            {
                                UNIT[i1].XRP_Err_Detect.MoveTime = UnitMoveTime;
                                UNIT[i1].XRP_Err_Detect.Moving = 1;
                            }
                        }
                    }
					
                    //Detect reactive power control failure
                    if ((UNIT[i1].XRP_Err_Detect.Enabled == 1) &&
						(PLANT[segment].Close_Loop == CLOSE_LOOP))
                    {
                        //Error detection timer
                        if (UNIT[i1].XRP_Err_Detect.MoveTime > 0) 
                        {
                            UNIT[i1].XRP_Err_Detect.MoveTime--;
                            if (UNIT[i1].XRP_Err_Detect.MoveTime == 0)
                            {
                                UNIT[i1].XRP_Err_Detect.Moving = 0;
                                UNIT[i1].RP_Setp_Calc = 0;
                                UNIT[i1].RPCompMode = 0;
                            }
                        } 
                        else 
                        {
                            if (fabs(UNIT[i1].RP_RtValue - UNIT[i1].RP_Setp) > Unit_RP_MaxDev)
                            {
                                UNIT[i1].RP_Fail = 1;
                                UNIT[i1].RP_Setp_Calc = 0;
                                UNIT[i1].RPCompMode = 0;
                                UNIT[i1].RP_Avail=0;
                                AVC_UpdateRTDB_IND(DB_NAME_UNIT_AVCSEL[i1],0);
                                AVC_UpdateRTDB_IND(DB_NAME_Unit_RP_Fail[i1],1);
                                UNIT[i1].RP_FailTime=Time_10s;
                            }
                        }
                    }
                }
                else 
                {
                    //Reset reactive power control failure
                    UNIT[i1].RP_Fail = 0;
					
                    //Indication unit is moving
                    if (UNIT[i1].XRP_Err_Detect.MoveTime > 0) 
                    {
                        UNIT[i1].XRP_Err_Detect.MoveTime--;
                        if (UNIT[i1].XRP_Err_Detect.MoveTime == 0) 
                            UNIT[i1].XRP_Err_Detect.Moving = 0;
                    }
                }
            } 
        } 
        else 
        {
            UNIT[i1].RP_Fail = 0;
            UNIT[i1].RPCompMode = 0;
            UNIT[i1].RPCompTime = 0;
            UNIT[i1].XRP_Setp = UNIT[i1].RP_RtValue;
            UNIT[i1].RP_Setp = UNIT[i1].XRP_Setp;
            UNIT[i1].YRP_Setp = UNIT[i1].XRP_Setp;
            UNIT[i1].ZRP_Setp = UNIT[i1].XRP_Setp;
			
            UNIT[i1].XRP_Err_Detect.HL_Value = UNIT[i1].RP_Setp + Unit_RP_MaxDev;
            UNIT[i1].XRP_Err_Detect.LL_Value = UNIT[i1].RP_Setp - Unit_RP_MaxDev;
            UNIT[i1].XRP_Err_Detect.LastValue = UNIT[i1].RP_RtValue;
            UNIT[i1].XRP_Err_Detect.MoveTime = UnitMoveTime;
            UNIT[i1].XRP_Err_Detect.Enabled = 0;
        }
        if(UNIT[i1].RPCompMode>0)
        {
            Unit_Comp=1;   
        }
    }
    PLANT[segment].XAVC_Unit_Comp=Unit_Comp;
    if((PLANT[segment].XRP_Comp_Time>Time_5s)&&(CompNextStep==1))
        PLANT[segment].XRP_Comp_Time=Time_5s;
}

void AVC_Distribution(int segment)
{
	int U_COMP,U_JUMP,UC,UJ,i1,i2,i3;
	int U_COMP_Mem[20]; //Units available for compensation
	int U_JUMP_Mem[20]; //Units starting, stopping, crossing vib. zone
	int U_JUCO_Mem[20]; //Relation between units (JUMP -> COMP)
	int U_CO_Used[20]; //Unit is used for compensation
	float RP_Diff_J,RP_Diff_C,RP_Step1;
	int  CompFunc,CompModeMax;
#ifdef CASCADE
	if (!PLANT[segment].AllocationToUnit)
	{
		return;
	}
#endif	
    CompFunc = 0;
    CompModeMax=0;
    if (PLANT[segment].XAVC_Unit_Comp == 1) 
    {
		//Find highest compensation mode
		RP_Diff_J = 0;
		for (i1=1; i1<=UNIT_NUMB; i1++) 
		{
			if (UNIT[i1].segment == segment) 
			{
				if ((UNIT[i1].RPCompMode >CompModeMax) &&(UNIT[i1].RP_Setp_Calc == 1))
				{
					CompModeMax=UNIT[i1].RPCompMode;
				}
			}
		}
		
		//*** Calculate number of units for compensation function
		U_COMP = 0;  //Number of units available for compensation
		U_JUMP = 0;  
		for (i1=1; i1<=UNIT_NUMB; i1++)
		{
			U_CO_Used[i1] = 0;
			//Unit is visible for selected segmet
			if (UNIT[i1].segment == segment) 
			{
				//Unit in AVC mode
				if ((UNIT[i1].RP_Avail == 1) && (PLANT[segment].Close_Loop == CLOSE_LOOP))
				{
					if ((UNIT[i1].Status==UNIT_IN_GEN) && (UNIT[i1].RP_Setp_Calc==1))
					{
						//Compensation of unit in AVC
						if ((UNIT[i1].RPCompMode > 0) &&(UNIT[i1].RPCompMode==CompModeMax))
						{
							U_JUMP++;
							U_JUMP_Mem[U_JUMP] = i1;
							U_JUCO_Mem[U_JUMP] = 0;
						} 
						else 
						{
							U_COMP++;
							U_COMP_Mem[U_COMP] = i1;
							//UNIT[i1].RPCompMode=0;
						}
					} 
					else
					{
						UNIT[i1].YRP_Setp = UNIT[i1].RP_Setp;
						UNIT[i1].XRP_Err_Detect.Enabled = 1;
					}
				}
				else 
				{
					UNIT[i1].YRP_Setp = UNIT[i1].RP_Setp;
					UNIT[i1].XRP_Err_Detect.Enabled = 1;
				}
			}
		}
		
		//*** Define relation in between units
		for (i1=1; i1<=U_JUMP; i1++) 
		{
			UJ = U_JUMP_Mem[i1];
			i2 = 0;
			i3 = 0;
			while ((i2 == 0) && (i3 < U_COMP))
			{
				i3++;
				UC = U_COMP_Mem[i3];
				if ((UNIT[UJ].XRP_Err_Detect.INC_Dir != UNIT[UC].XRP_Err_Detect.INC_Dir) &&
					(U_CO_Used[UC] == 0))
				{
					i2 = UC;
				}
			}
			
			//For this unit a compensation is not possible
			if (i2 == 0)
			{
				UNIT[UJ].YRP_Setp = UNIT[UJ].RP_Setp;
				UNIT[UJ].XRP_Err_Detect.Enabled = 1;
				UNIT[UJ].RP_Setp_Calc = 0;
				U_JUMP_Mem[i1] = 0;
				U_JUCO_Mem[i1] = 0;
			} 
			else
			{
				U_JUCO_Mem[i1] = i2;
				U_CO_Used[i2] = 1;
			}
		}
		if ((U_JUMP > 0) && (U_COMP >= U_JUMP)) CompFunc = 1; 
		// printf("U_JUMP=%d,U_COMP=%d\n",U_JUMP,U_COMP);
    }
	
    if (CompFunc == 1) 
    {
		if (PLANT[segment].XRP_Comp_Time > 0)
			PLANT[segment].XRP_Comp_Time--;
		else
		{
			PLANT[segment].XRP_Comp_Time = RPCompStepTime;
			printf("*******RP Comp Mode!\n");
			for (i1=1; i1<=U_JUMP; i1++)
			{
				UJ = U_JUMP_Mem[i1];
				UC = U_JUCO_Mem[i1];
				if ((UJ > 0) && (UC > 0)) 
				{
					//Possible step to change power
					RP_Step1 = RPCompStepVal;
					RP_Diff_J = (float)fabs(UNIT[UJ].RP_Setp - UNIT[UJ].YRP_Setp);
					RP_Diff_C = (float)fabs(UNIT[UC].RP_Setp - UNIT[UC].YRP_Setp);
					if (RP_Diff_J < RP_Diff_C) 
					{
						if (RP_Diff_J < (RPCompStepVal + RPCompStepVal))
						{
							RP_Step1 = RP_Diff_J;
							UNIT[UJ].RP_Setp_Calc = 0;
							//PLANT[segment].XAVC_Unit_Comp=0;
						}
					}
					else 
					{
						if (RP_Diff_C < (RPCompStepVal + RPCompStepVal)) 
						{
							RP_Step1 = RP_Diff_C;
							UNIT[UC].RP_Setp_Calc = 0;
						}
					}
					if (UNIT[UJ].XRP_Err_Detect.INC_Dir == 1)
					{
						UNIT[UJ].YRP_Setp += RP_Step1;
						UNIT[UC].YRP_Setp -= RP_Step1;
					}
					else
					{
						UNIT[UJ].YRP_Setp -= RP_Step1;
						UNIT[UC].YRP_Setp += RP_Step1;
					}
				}
			}
			//*** Compensation Function End ****************************************
		}
    }
    else
    {
		for (i1=1; i1<=UNIT_NUMB; i1++) 
		{
			if (UNIT[i1].segment == segment)
			{
				UNIT[i1].YRP_Setp = UNIT[i1].RP_Setp;
				UNIT[i1].XRP_Err_Detect.Enabled = 1;
			}
		}
    }
}

void Quit (int sn)
{
    UnMountDB();
    printf ("AVC exit!(Ctrl_c or kill -2 pid)\n");
    lan_finish(0);
    exit (0);
}

void InitData(void)
{
    int i,segment;
    for(i=1;i<=UNIT_NUMB;i++)
    {
        UNIT[i].RP_RtValue=ReadDB_ANA(DB_NAME_RP_RTVALUE[i]);
        UNIT[i].XRP_Setp=UNIT[i].RP_RtValue;
        UNIT[i].ZRP_Setp=UNIT[i].YRP_Setp=UNIT[i].RP_Setp=UNIT[i].RP_RtValue;
        AVC_UpdateRTDB_IND(DB_NAME_Unit_RP_Fail[i],0);
    }
    for(segment=0;segment<MAXSEGMENT;segment++)
    {
		Move_Curve[segment]=1;
		SaveTime[segment]=0;
		AVC_Recalc[segment]=0;
		ControlState[segment]=0;
		RP_Faulty[segment]=0;
        PLANT[segment].Last_Num_Avail=0;
        PLANT[segment].GenNumb=0;
        PLANT[segment].XVoltDoopAdaptFact=1; 
        PLANT[segment].FirstRun=1;
        PLANT[segment].RPCopyTime=0;
		PLANT[segment].JR_RPCopyTime=0;
		PLANT[segment].CompLimPos=0;
    }
}
void    LogFileChangeDayAndWriteTime()
{
    char Str[80]="",Str1[80]="";
    int i;
    
    if( oldday != ntime->tm_mday )
    { 
        if(ntime->tm_mon+1>=4)
		{
			for(i=0;i<9;i++)
			{
#ifdef SUN_OS 
				strcpy(Str,"rm /home/ems/h9000/his/agc/");
#else
				strcpy(Str,"del /s \\home\\ems\\h9000\\his\\agc\\");
#endif
				if(ntime->tm_mon+1-3-i>=1)
				{
					sprintf(Str1,"AVCcmd-DD.%02d%02d*",ntime->tm_year+1900,ntime->tm_mon+1-3-i);
					strcat(Str,Str1);
					system(Str);
				}
				else
					break;
			}
#ifdef SUN_OS
			strcpy(Str,"rm /home/ems/h9000/his/agc/");
#else
			strcpy(Str,"del /s \\home\\ems\\h9000\\his\\agc\\");
#endif
			sprintf(Str1,"AVCcmd-DD.%02d*",ntime->tm_year+1900-1);
			strcat(Str,Str1);
			system(Str);
		}
		else
		{
			for(i=9;i>0;i--)
			{
#ifdef SUN_OS
				strcpy(Str,"rm /home/ems/h9000/his/agc/");
#else
				strcpy(Str,"del /s \\home\\ems\\h9000\\his\\agc\\");
#endif
				if(ntime->tm_mon+1+12-3-i<=12)
				{
					sprintf(Str1,"AVCcmd-xjb.%02d%02d*",ntime->tm_year+1900-1,ntime->tm_mon+1+12-3-i);
					strcat(Str,Str1);
					system(Str);
				}
				else 
					break;
			}
		}
        oldday = ntime->tm_mday;
		memset(logfilename,0,90);
		sprintf( logfilename, "/home/ems/h9000/his/agc/AVCcmd-xjb.%02d%02d%02d",ntime->tm_year+1900,ntime->tm_mon+1,ntime->tm_mday);
		if((fp=fopen(logfilename,"r"))==NULL)
		{
			fp=fopen(logfilename, "w");
			strcpy(Str,"**************************************************\n");
			fwrite(Str,strlen(Str),1,fp);
			strcpy(Str,"      IWHR Technology\n");
			fwrite(Str,strlen(Str),1,fp);
			strcpy(Str,"      H9000 V4.0 system Command Record\n");
			fwrite(Str,strlen(Str),1,fp);
			strcpy(Str,"**************************************************\n");
			fwrite(Str,strlen(Str),1,fp);
        }
        fclose(fp);  
    }
}
void FillCommandBuf(int segment)
{
    int i,Flag,Number[UNIT_NUMB],num;
    char	logtxt[200];
#ifdef CASCADE
	if (!PLANT[segment].AllocationToUnit)
	{
		return;
	}
#endif	
    Flag=0;
    num=0;
    if(PLANT[segment].Mode==5)
	{
		AVC_Recalc[segment] =0;
		return;
	}
    for (i=1;i<=UNIT_NUMB;i++) 
    {
        if(UNIT[i].segment!=segment)continue; 
        AVC_UpdateRTDB_ANA(DB_NAME_UNIT_SETPOINT[i],UNIT[i].YRP_Setp);
        if(UNIT[i].RP_Avail!=1)continue;
        
        if((fabs(UNIT[i].YRP_Setp-UNIT[i].ZRP_Setp)>=1)&&(UNIT[i].Status==UNIT_IN_GEN))
        { 
			//printf("YRP_Setp=%f,ZRP_Setp=%f\n",UNIT[i].YRP_Setp,UNIT[i].ZRP_Setp);
            if(( PLANT[segment].Close_Loop==CLOSE_LOOP)||(PLANT[segment].RPAnswerYes==1))
            {
                UNIT[i].ZRP_Setp=UNIT[i].YRP_Setp;
                Flag=1;
                Number[num++]=i;
                SendCommandToPLC(DB_NAME_WG_GD[i],C_RPOWER,UNIT[i].ZRP_Setp,0);
                printf("Unit No.%d Reactive Power Command Send out!\n",i);
                if(PLANT[segment].RPAnswerYes==1)
                    PLANT[segment].RPAnswerYesTime=Time_5s;  
            }
        }
    }
	AVC_Recalc[segment]=0;
	if(Flag==1)
	{  
		sprintf(logtxt,"\n%02d月%02d日  %02d:%02d:%02d,segment=%d,RP_Setting=%d,VO_Regulation=%d,VO_Curve=%d,VO_Compensation=%d,Sel_Bus=%d,RP_Setpoint=%f\n",
			ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec,segment,PLANT[segment].RP_Setting,PLANT[segment].VO_Regulation,
			PLANT[segment].VO_Curve,PLANT[segment].VO_Compensation,PLANT[segment].Sel_Bus,PLANT[segment].RP_Setpoint);
		WriteLogFile ( logtxt );
		sprintf( logtxt, "\n %d月%02d日  %02d:%02d:%02d PLANT[%d].RP_WorkSP=%f\n",
			ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec,
			segment,PLANT[segment].RP_WorkSP);
		WriteLogFile ( logtxt );
		for(i=0;i<num;i++)
		{
			sprintf(logtxt," %d月%02d日  %02d:%02d:%02d No.%d AP_RtValue=%f,RP_RtValue=%f,RP_Setp is %f!\n",
				ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec,
				Number[i],UNIT[Number[i]].AP_RtValue,UNIT[Number[i]].RP_RtValue,UNIT[Number[i]].ZRP_Setp);
			WriteLogFile ( logtxt );
		}
	}
}

void main(int argc,char *argv[])
{
    int	segment; 
#ifndef SUN_OS
	WORD	wvr;
	WSADATA	WSAData;  
	wvr=MAKEWORD(1,1);
	
	WSAStartup(wvr, &WSAData);
#endif	    
    gethostname(hostname,20);
    printf("hostname =%s\n",hostname);
    SetModuleName("AVC-xjb");
    //SetErrLogMode(argc,argv);
    CreateLockFile(".avc-xjb.lock");
    signal(SIGINT,Quit);
    if(MountDB(DATABASE_NAME)==-1)
    {
        printf("\nFail to MountDB!");
        exit(-1);
    }
    lan_out_init();
	
    Load_AVC_DEF();
    ReadParameter();
    ReadDefFile();
    InitData();
    ReadTime();
    LogFileChangeDayAndWriteTime();
    for (;;)
    {
		ReadTime();
		LogFileChangeDayAndWriteTime();
		if(Cycle%5==0)
			AVC_segment();
		for(segment=0;segment<MAXSEGMENT;segment++)
        {   
			if(Cycle%5==0)
				ReadAVC_FromRTDB(segment);
			PLANT[segment].Volt_Power = Voltage_Control(segment); 
			if(Cycle%5==0)
			{
				AVC_Module(segment);
				Unit_Supervision(segment);
				AVC_Distribution(segment);
				FillCommandBuf(segment);
            }
            PLANT[segment].FirstRun=0;
        }
        
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
		
		Cycle++;
		if(Cycle>10000)
			Cycle=1;
        AVC_Sleep(200);
    }
}

