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
#include    "../../../agc-xjb/v4.0a/inc/agc.h"
#include    "../../../cmd/v4.0a/inc/cmd_def.h"
#include    "../../../dms/v4.0a/inc/dms_std.h"
#include    "../../../dms/v4.0a/inc/dms_structure_def.h"
#include    "../../../dms/v4.0a/inc/os_type_def.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"


T_PLANT PLANT[MAXSEGMENT];
T_UNIT UNIT[UNIT_NUMB+1];
T_EffSet UnitData[MAXTYPES+1];
T_AGCCurve AGC_Unit_Data[MAXTYPES+1];    
T_AGCVibration AGC_Unit_Vibr[MAXTYPES+1]; 
Water_Msg	WaterMsg;

int Move_Curve[MAXSEGMENT];
int Dispatch[MAXSEGMENT];
struct tm  *ntime;
int Cycle=0;
long SaveTime[MAXSEGMENT];
int AGC_Recalc[MAXSEGMENT];
int ControlState[MAXSEGMENT];
int AP_Faulty[MAXSEGMENT];

//for water head 
float WaterHeadTempValue[10]={0};
int WaterHeadNumber=0;
int WaterHeadFlag=0;
float SaveWaterHead=0;
int AlarmFlag=0;
float SaveFiveMinuteWh=0;

int Number_ANA=0;
ANA_MSG	ml_buf_ANA[64];
LAN_HEAD	lanhd_ANA,lanhd_IND;
int Number_IND=0;
SYS_MSG	ml_buf_IND[64];

const int	StartStopDelay=50;
const int StopCmdTime = 300;
const	int Time_2s=2;
const	int	Time_15s=15;
const	int Time_5s=5;
const   int Time_10s=10;
const	int Time_20s=20;
const	int	StartStopBlockTime=60;
const int VibAlarmTime = 60;
const	int CommandTime=5;
const	int	ResponseDelay=20;  /*20秒*/
const int MaxStartTime = 600;  /*10分钟*/
const	int	UnitMoveTime=40;
const int MaxStopTime =600;
const	int	APCompStepTime=30;
const	int StartCompTime=180;
const	int StopCompTime=600;
const	int	Time_30s=30;
const int VibCompTime = 120;    /*2分钟*/
const int Time_60s=60;
const int Time_10m=600;
float APSetpDB=1.0;
float APFilterDB=8.0;

//for parameter
int  AheadOfSchedule;
int  AutoStartStop[UNIT_NUMB+1]={0};
float MinPower;
float	MaxPower;
float   AP_Step;
float	UnitAPMove;
float   APCompStepVal;
float APStopMin;
float	APStopLimit;
float	Unit_AP_MaxDev;
float	MINHEAD;
float	MAXHEAD;
int		AP_AlarmFact;
int		FR_AlarmFact;
int		Max_Power_Gap=500;
float   MaxFreq, MinFreq,Freq_A_Gain;
float	Sch_Inc_Value,Sch_Dec_Value;
float	WH_Value;
float   WorkSP_Max_Change;

unsigned char	src_id;
char    oldday;
char    logfilename[90];
char    hostname[20];
FILE    *fp;

void SetUnitVibMinMax(int);
float GetUnitCurrentMin(int,float);
float GetUnitCurrentMax(int,float);

extern int BdSms(char* sms);
extern int LocateKeyword( FILE *, char *);
extern void CreateLockFile(char *);
extern int BroadCastSms(char* message, BOOL IsAlm);

void AGC_Sleep(int Millisecond)
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

float ReadDB_ANA(char *PointName)
{
    float ReturnValue;
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

int AGC_UpdateRTDB_ANA (char *PointName,float data)
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



int AGC_UpdateRTDB_IND (char *PointName,short data)
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
	ml_buf.status.result = 0;
    ml_buf.status.src=1;
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
    strcpy(ml_buf.usr_login_name,"AGC");
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

void Calculate_AGC_Curve(int UType, float WaterHead)//已知机组类型号和水头，由已知水头对应点插值求
{
	int i1,i2,i3,i4,i5,z1,z2,LoopVar;
	int pos1,pos2,pos3,pos4,VIB_Pos;
	float f1,f2,f3;
	float WH,minWH,maxWH,WHfact;
	float AP,minAP,maxAP,APfact;
	float ActPower,NextActPower,OutPow,Eff;
	float Flow1,Flow2,Eff1,Eff2,Eff_Res,Flow_Res,InPow_Res,dInp_dOut;
	T_AGCVibration Unit_Vibr_1,Unit_Vibr_2;
	int ZeroCalc;
	
	
	for (i1=1; i1<=VIBRATION_NUMB; i1++) {
		Unit_Vibr_1.Vibration[i1].Used = 0;
		Unit_Vibr_1.Vibration[i1].AP_Begin = 0;
		Unit_Vibr_1.Vibration[i1].AP_End = 0;
		Unit_Vibr_1.Vibration[i1].AP_Size = 0;
		Unit_Vibr_2.Vibration[i1].Used = 0;
		Unit_Vibr_2.Vibration[i1].AP_Begin = 0;
		Unit_Vibr_2.Vibration[i1].AP_End = 0;
		Unit_Vibr_2.Vibration[i1].AP_Size = 0;
	}
	
	if ((UType>0) && (UType<=MAXTYPES)) {
		//Clear memory for vibration zones
		AGC_Unit_Vibr[UType].Used = 0;
		for (i1=1; i1<=VIBRATION_NUMB; i1++) {
			AGC_Unit_Vibr[UType].Vibration[i1].Used = 0;
			AGC_Unit_Vibr[UType].Vibration[i1].AP_Begin = 0;
			AGC_Unit_Vibr[UType].Vibration[i1].AP_End = 0;
			AGC_Unit_Vibr[UType].Vibration[i1].AP_Size = 0;
		}
		
		//Find position of curves in relation to water head
		WH = WaterHead;
		minWH = 10000;
		maxWH = 0;
		i2 = MAXDEFPOINTS;
		i4 = 1;
		pos1 = 0;
		for (i1=1; i1<i2; i1++) {
			f1 = UnitData[UType].WaterHead[i1].WaterHead;
			f2 = UnitData[UType].WaterHead[i1+1].WaterHead;
			if (f1>maxWH) {
				maxWH = f1;
				i4 = i1;
			}
			if (f2>maxWH) {
				maxWH = f2;
				i4 = i1 + 1;
			}
			if ((f1>0) && (f1<minWH)) minWH = f1;
			if ((WH>=f1) && (WH<f2)) pos1 = i1;  //pos1
		}
		if (pos1>0) {
			pos2 = pos1 + 1;
		} else {
			if (WH <= minWH) {
				pos1 = 1;
				pos2 = 2;
			}
			if (WH >= maxWH) {
				pos1 = i4 - 1;
				pos2 = i4;
			}
		}
		
		if (pos2 > MAXDEFPOINTS) {
			pos2 = MAXDEFPOINTS;
			pos1 = pos2 - 1;
		}
		if (pos1 < 1) {
			pos1 = 1;
			pos2 = 2;
		}
		
		//Calculate interpolation factor for waterhead
		f1 = UnitData[UType].WaterHead[pos1].WaterHead;
		f2 = UnitData[UType].WaterHead[pos2].WaterHead;
		f3 = f2 - f1;
		if (WH<f1) WH = f1;
		if (WH>f2) WH = f2;
		
		WHfact = 0;
		if (f3>0) {
			WHfact = (WH-f1) / f3;
		}
		
		//Calculate power limits for actual efficiency curve
		f1 = UnitData[UType].WaterHead[pos1].MinPower;
		f2 = UnitData[UType].WaterHead[pos2].MinPower;
		f3 = f1 + ((f2 - f1) * WHfact);
		if (f3<MinPower) f3 = MinPower;
		AGC_Unit_Data[UType].MinPower = f3;
		f1 = UnitData[UType].WaterHead[pos1].MaxPower;
		f2 = UnitData[UType].WaterHead[pos2].MaxPower;
		f3 = f1 + ((f2 - f1) * WHfact);
		if (f3>MaxPower) f3 = MaxPower;
		AGC_Unit_Data[UType].MaxPower = f3;
		
		//Find vibration zones for first position
		i2 = 0;
		i3 = 0;
		i4 = 0;
		f1 = 0;
		f2 = 0;
		VIB_Pos = pos1;  
		for (i1=1; i1<=MAXDEFPOINTS; i1++) {
			i2 = UnitData[UType].WaterHead[VIB_Pos].EffCurve[i1].VibZone; 
			if ((i3==0) && (i2==1)) {
				i3 = 1;
				f1 = UnitData[UType].WaterHead[VIB_Pos].EffCurve[i1].OutPow;
			}
			if ((i3==1) && (i2==3)) {
				if (AGC_Unit_Vibr[UType].Used==0) AGC_Unit_Vibr[UType].Used = 1;
				i3 = 0;
				f2 = UnitData[UType].WaterHead[VIB_Pos].EffCurve[i1].OutPow;
				i4++;
				if ((i4>0) && (i4<=VIBRATION_NUMB) && (f2 > f1)) {
					Unit_Vibr_1.Vibration[i4].Used = 1;
					Unit_Vibr_1.Vibration[i4].AP_Begin = f1;
					Unit_Vibr_1.Vibration[i4].AP_End = f2;
					Unit_Vibr_1.Vibration[i4].AP_Size = f2 - f1;
				}
			}
		}
		z1 = i4;
		//Find vibration zones for second position
		i2 = 0;
		i3 = 0;
		i4 = 0;
		f1 = 0;
		f2 = 0;
		VIB_Pos = pos2;
		for (i1=1; i1<=MAXDEFPOINTS; i1++) {
			i2 = UnitData[UType].WaterHead[VIB_Pos].EffCurve[i1].VibZone;
			if ((i3==0) && (i2==1)) {
				i3 = 1;
				f1 = UnitData[UType].WaterHead[VIB_Pos].EffCurve[i1].OutPow;
			}
			if ((i3==1) && (i2==3)) {
				i3 = 0;
				f2 = UnitData[UType].WaterHead[VIB_Pos].EffCurve[i1].OutPow;
				i4++;
				if ((i4>0) && (i4<=VIBRATION_NUMB) && (f2 > f1)) {
					Unit_Vibr_2.Vibration[i4].Used = 1;
					Unit_Vibr_2.Vibration[i4].AP_Begin = f1;
					Unit_Vibr_2.Vibration[i4].AP_End = f2;
					Unit_Vibr_2.Vibration[i4].AP_Size = f2 - f1;
				}
			}
		}
		z2 = i4;
		//Calculation of vibration zones
		if (z1==z2) {
			//Equal number of zones - Interpolation possible
			for (i1=1; i1<=VIBRATION_NUMB; i1++) {
				if (Unit_Vibr_1.Vibration[i1].Used == 1) {  
					AGC_Unit_Vibr[UType].Vibration[i1].Used = 1;
					f1 = Unit_Vibr_1.Vibration[i1].AP_Begin;
					f2 = Unit_Vibr_2.Vibration[i1].AP_Begin;
					f3 = f1 + ((f2 - f1) * WHfact);
					AGC_Unit_Vibr[UType].Vibration[i1].AP_Begin = f3;
					f1 = Unit_Vibr_1.Vibration[i1].AP_End;
					f2 = Unit_Vibr_2.Vibration[i1].AP_End;
					f3 = f1 + ((f2 - f1) * WHfact);
					AGC_Unit_Vibr[UType].Vibration[i1].AP_End = f3;
					
					f1 = AGC_Unit_Vibr[UType].Vibration[i1].AP_Begin;
					f2 = AGC_Unit_Vibr[UType].Vibration[i1].AP_End;
					AGC_Unit_Vibr[UType].Vibration[i1].AP_Size = f2 - f1;
					
				}
			}
		} else {
			//Different number of zones
			for (i1=1; i1<=VIBRATION_NUMB; i1++) {
				if (WHfact<0.5) {
					if (Unit_Vibr_1.Vibration[i1].Used == 1) {
						AGC_Unit_Vibr[UType].Vibration[i1].Used = 1;
						f1 = Unit_Vibr_1.Vibration[i1].AP_Begin;
						f2 = Unit_Vibr_1.Vibration[i1].AP_End;
						AGC_Unit_Vibr[UType].Vibration[i1].AP_Begin = f1;
						AGC_Unit_Vibr[UType].Vibration[i1].AP_End = f2;
						AGC_Unit_Vibr[UType].Vibration[i1].AP_Size = f2 - f1;
					}
				} else {
					if (Unit_Vibr_2.Vibration[i1].Used == 1) {
						AGC_Unit_Vibr[UType].Vibration[i1].Used = 1;
						f1 = Unit_Vibr_2.Vibration[i1].AP_Begin;
						f2 = Unit_Vibr_2.Vibration[i1].AP_End;
						AGC_Unit_Vibr[UType].Vibration[i1].AP_Begin = f1;
						AGC_Unit_Vibr[UType].Vibration[i1].AP_End = f2;
						AGC_Unit_Vibr[UType].Vibration[i1].AP_Size = f2 - f1;
					}
				}
			}
		}
		
		/*最大值在振动区里*/
		for (i5=VIBRATION_NUMB; i5>=1; i5--) {
			if ((AGC_Unit_Data[UType].MaxPower > AGC_Unit_Vibr[UType].Vibration[i5].AP_Begin) &&
				(AGC_Unit_Data[UType].MaxPower < AGC_Unit_Vibr[UType].Vibration[i5].AP_End))
			{
				AGC_Unit_Data[UType].MaxPower = AGC_Unit_Vibr[UType].Vibration[i5].AP_Begin;
			}
		}
		/*最小有功大于振动区下限，最小有功小于振动区上限，将振动区上限作为最小有功*/
		for (i5=1; i5<=VIBRATION_NUMB; i5++) {
			if ((AGC_Unit_Data[UType].MinPower > AGC_Unit_Vibr[UType].Vibration[i5].AP_Begin) &&
				(AGC_Unit_Data[UType].MinPower < AGC_Unit_Vibr[UType].Vibration[i5].AP_End))
			{
				AGC_Unit_Data[UType].MinPower = AGC_Unit_Vibr[UType].Vibration[i5].AP_End;
				/*振动区无效*/
				AGC_Unit_Vibr[UType].Vibration[i5].Used = 0;
				AGC_Unit_Vibr[UType].Vibration[i5].AP_Begin = 0;
				AGC_Unit_Vibr[UType].Vibration[i5].AP_End = 0;
				AGC_Unit_Vibr[UType].Vibration[i5].AP_Size = 0;
			}
		}
		/*第一个振动区下限减有功步长后小于机组最小有功*/
		f1 = AGC_Unit_Vibr[UType].Vibration[1].AP_Begin - AP_Step;
		if ((f1 > APStopMin) && (f1 < AGC_Unit_Data[UType].MinPower))
		{
			if (AGC_Unit_Vibr[UType].Vibration[1].AP_End > APStopMin) {
				AGC_Unit_Data[UType].MinPower = AGC_Unit_Vibr[UType].Vibration[1].AP_End;
				/*振动区无效*/
				AGC_Unit_Vibr[UType].Vibration[1].Used = 0;
				AGC_Unit_Vibr[UType].Vibration[1].AP_Begin = 0;
				AGC_Unit_Vibr[UType].Vibration[1].AP_End = 0;
				AGC_Unit_Vibr[UType].Vibration[1].AP_Size = 0;
			}
		}
		
		ActPower = 0;
		LoopVar = 0;
		while (LoopVar<MAXAGCPOINTS) {
			LoopVar++;
			
			//Reset values
			AGC_Unit_Data[UType].EffCurve[LoopVar].Eff = 0;
			AGC_Unit_Data[UType].EffCurve[LoopVar].Flow = 0;
			AGC_Unit_Data[UType].EffCurve[LoopVar].InpPow = 0;
			AGC_Unit_Data[UType].EffCurve[LoopVar].dInp_dOut = 0;
			AGC_Unit_Data[UType].EffCurve[LoopVar].VibZone = 0;
			AGC_Unit_Data[UType].EffCurve[LoopVar].OutPow = ActPower;
			
			//Check that power is in vibration zone
			NextActPower = ActPower + AP_Step;
			i1 = 0;
			i2 = 0;
			i3 = 0;
			while ((i1<VIBRATION_NUMB) && (i2==0)) {
				i1++;
				f1 = AGC_Unit_Vibr[UType].Vibration[i1].AP_Begin;   //Begin of zone
				f2 = AGC_Unit_Vibr[UType].Vibration[i1].AP_End;     //End of zone
				if (AGC_Unit_Vibr[UType].Vibration[i1].Used == 1)
				{
					//Check that power is inside a normal vibration zone
					if ((ActPower>=f1) && (ActPower<f2)) {
						i2 = i1;
						i3 = 1;
					}
					
					//Check that a very small vibration zone is in between two power steps
					if ((f1>=ActPower) && (f2<=NextActPower)) {
						i2 = i1;
						if ((f1-ActPower) > APFilterDB) {
							i3 = 3;                               //Create new step
						} else {
							i3 = 2;                               //Itegrate vib.zone into step
						}
					}
				}
			}
			
			//Power is in vibration zone
			if (i2>0) {
				//Power is inside a normal vibration zone
				if (i3==1) {
					AGC_Unit_Data[UType].EffCurve[LoopVar].VibZone = 1;      //Begin of zone
					AGC_Unit_Data[UType].EffCurve[LoopVar].OutPow = AGC_Unit_Vibr[UType].Vibration[i2].AP_Begin;
					if (LoopVar<MAXAGCPOINTS) {
						LoopVar++;
						AGC_Unit_Data[UType].EffCurve[LoopVar].VibZone = 3;      //End of zone
						AGC_Unit_Data[UType].EffCurve[LoopVar].OutPow = AGC_Unit_Vibr[UType].Vibration[i2].AP_End;
					}
					ActPower = AGC_Unit_Data[UType].EffCurve[LoopVar].OutPow;
				}
				//Very small vibr.zone in between two power steps - Integrate into step
				if (i3==2) {
					AGC_Unit_Data[UType].EffCurve[LoopVar].VibZone = 1;      //Begin of zone
					AGC_Unit_Data[UType].EffCurve[LoopVar].OutPow = AGC_Unit_Vibr[UType].Vibration[i2].AP_Begin;
					if (LoopVar<MAXAGCPOINTS) {
						LoopVar++;
						AGC_Unit_Data[UType].EffCurve[LoopVar].VibZone = 3;      //End of zone
						AGC_Unit_Data[UType].EffCurve[LoopVar].OutPow = AGC_Unit_Vibr[UType].Vibration[i2].AP_End;
					}
					ActPower = AGC_Unit_Data[UType].EffCurve[LoopVar].OutPow;
				}
				//Very small vibr.zone in between two power steps - Create new step
				if (i3==3) {
					if (LoopVar<MAXAGCPOINTS) {
						LoopVar++;
						AGC_Unit_Data[UType].EffCurve[LoopVar].VibZone = 1;      //Begin of zone
						AGC_Unit_Data[UType].EffCurve[LoopVar].OutPow = AGC_Unit_Vibr[UType].Vibration[i2].AP_Begin;
					}
					if (LoopVar<MAXAGCPOINTS) {
						LoopVar++;
						AGC_Unit_Data[UType].EffCurve[LoopVar].VibZone = 3;      //End of zone
						AGC_Unit_Data[UType].EffCurve[LoopVar].OutPow = AGC_Unit_Vibr[UType].Vibration[i2].AP_End;
					}
					ActPower = AGC_Unit_Data[UType].EffCurve[LoopVar].OutPow;
				}
			}
			
			//Increase power - for next row in table
			ActPower += AP_Step;
			if (ActPower>AGC_Unit_Data[UType].MaxPower) ActPower = AGC_Unit_Data[UType].MaxPower;
		}
		
		//Calculate actual efficiency curve
		for (LoopVar=1; LoopVar<=MAXAGCPOINTS; LoopVar++) {
			//Calculate efficiency and flow for curve 1
			AP = AGC_Unit_Data[UType].EffCurve[LoopVar].OutPow;
			minAP = 10000;
			maxAP = 0;
			i2 = MAXDEFPOINTS;
			i3 = pos1;
			i4 = 1;
			pos3 = 0;
			for (i1=1; i1<i2; i1++) {
				f1 = UnitData[UType].WaterHead[i3].EffCurve[i1].OutPow;
				f2 = UnitData[UType].WaterHead[i3].EffCurve[i1+1].OutPow;
				if (f1>maxAP) {
					maxAP = f1;
					i4 = i1;
				}
				if (f2>maxAP) {
					maxAP = f1;
					i4 = i1 + 1;
				}
				if ((f1>0) && (f1<minAP)) minAP = f1;
				if ((AP>=f1) && (AP<f2)) pos3 = i1;
			}
			if (pos3>0) {
				pos4 = pos3 + 1;
			} else {
				if (AP < minAP) {
					pos3 = 1;
					pos4 = 2;
				}
				if (AP > maxAP) {
					pos3 = i4 - 1;
					pos4 = i4;
				}
			}
			ZeroCalc = 0;
			f1 = UnitData[UType].WaterHead[i3].EffCurve[pos3].OutPow;
			f2 = UnitData[UType].WaterHead[i3].EffCurve[pos4].OutPow;
			if (AP<0) AP = 0;
			if (AP>maxAP) AP = maxAP;
			if (AP<minAP) ZeroCalc = 1;
			if (ZeroCalc==1) {
				f2 = f1;
				f1 = 0;
			}
			f3 = f2 - f1;
			
			APfact = 0;
			if (f3 > 0) APfact = (AP-f1) / f3;
			
			f1 = UnitData[UType].WaterHead[i3].EffCurve[pos3].Flow;
			f2 = UnitData[UType].WaterHead[i3].EffCurve[pos4].Flow;
			if (ZeroCalc==1) {
				f2 = f1;
				f1 = 0;
			}
			Flow1 = f1 + ((f2 - f1) * APfact);
			
			f1 = UnitData[UType].WaterHead[i3].EffCurve[pos3].Eff;
			f2 = UnitData[UType].WaterHead[i3].EffCurve[pos4].Eff;
			if (ZeroCalc==1) {
				f2 = f1;
				f1 = 0;
			}
			Eff1 = f1 + ((f2 - f1) * APfact);
			
			//Calculate efficiency and flow for curve 2
			minAP = 10000;
			maxAP = 0;
			i2 = MAXDEFPOINTS;
			i3 = pos2;
			i4 = 1;
			pos3 = 0;
			for (i1=1; i1<i2; i1++) {
				f1 = UnitData[UType].WaterHead[i3].EffCurve[i1].OutPow;
				f2 = UnitData[UType].WaterHead[i3].EffCurve[i1+1].OutPow;
				if (f1>maxAP) {
					maxAP = f1;
					i4 = i1;
				}
				if (f2>maxAP) {
					maxAP = f1;
					i4 = i1 + 1;
				}
				if ((f1>0) && (f1<minAP)) minAP = f1;
				if ((AP>=f1) && (AP<f2)) pos3 = i1;
			}
			if (pos3>0) {
				pos4 = pos3 + 1;
			} else {
				if (AP < minAP) {
					pos3 = 1;
					pos4 = 2;
				}
				if (AP > maxAP) {
					pos3 = i4 - 1;
					pos4 = i4;
				}
			}
			
			ZeroCalc = 0;
			f1 = UnitData[UType].WaterHead[i3].EffCurve[pos3].OutPow;
			f2 = UnitData[UType].WaterHead[i3].EffCurve[pos4].OutPow;
			if (AP<0) AP = 0;
			if (AP>maxAP) AP = maxAP;
			if (AP<minAP) ZeroCalc = 1;
			if (ZeroCalc==1) {
				f2 = f1;
				f1 = 0;
			}
			f3 = f2 - f1;
			
			APfact = 0;
			if (f3 > 0) APfact = (AP-f1) / (f2-f1);
			
			f1 = UnitData[UType].WaterHead[i3].EffCurve[pos3].Flow;
			f2 = UnitData[UType].WaterHead[i3].EffCurve[pos4].Flow;
			if (ZeroCalc==1) {
				f2 = f1;
				f1 = 0;
			}
			Flow2 = f1 + ((f2 - f1) * APfact);
			
			f1 = UnitData[UType].WaterHead[i3].EffCurve[pos3].Eff;
			f2 = UnitData[UType].WaterHead[i3].EffCurve[pos4].Eff;
			if (ZeroCalc==1) {
				f2 = f1;
				f1 = 0;
			}
			Eff2 = f1 + ((f2 - f1) * APfact);
			
			//*** Calculation of interpolated values
			Eff_Res = Eff1 + ((Eff2 - Eff1) * WHfact);
			Flow_Res = Flow1 + ((Flow2 - Flow1) * WHfact);
			
			//Input Power and Slope of dInp/dOutp
			if (LoopVar<2) {
				f1 = 0;
				f2 = 0;
			} else {
				f1 = AGC_Unit_Data[UType].EffCurve[LoopVar-1].OutPow;
				f2 = AGC_Unit_Data[UType].EffCurve[LoopVar-1].InpPow;
			}
			OutPow = AGC_Unit_Data[UType].EffCurve[LoopVar].OutPow;
			Eff = Eff_Res / 100;
			if (Eff>0.01) {
				InPow_Res = OutPow / Eff;
				f3 = OutPow - f1;
				if ((LoopVar<2) || (f3==0)) {
					dInp_dOut = 0;
				} else {
					dInp_dOut = (InPow_Res - f2) / f3;
				}
			} else {
				InPow_Res = 0;
				dInp_dOut = 0;
			}
			
			//Store data in efficiency table
			AGC_Unit_Data[UType].EffCurve[LoopVar].Eff = Eff_Res;
			AGC_Unit_Data[UType].EffCurve[LoopVar].Flow = Flow_Res;
			AGC_Unit_Data[UType].EffCurve[LoopVar].InpPow = InPow_Res;
			AGC_Unit_Data[UType].EffCurve[LoopVar].dInp_dOut = dInp_dOut;
    }
  }
}

float Calculate_Input_Power(int UnitNumber, float UnitPower)
{
	int UType;
	int i1,i2;
	int pos1,pos2;
	float f1,f2,f3,f4;
	float minAP,maxAP,APfact,UPower,Eff;
	float InputPower;
	
	UType = UNIT[UnitNumber].Type;
	UPower = UnitPower;
	InputPower = UnitPower * 5;
	if ((UType>0) && (UType<=MAXTYPES)) {
		minAP = 10000;
		maxAP = 0;
		
		/*找出有功在曲线中的位置*/
		i2 = 1;
		pos1 = 0;
		pos2 = 0;
		for (i1=1; i1<MAXAGCPOINTS; i1++) {
			f1 = AGC_Unit_Data[UType].EffCurve[i1].OutPow;
			f2 = AGC_Unit_Data[UType].EffCurve[i1+1].OutPow;
			if (f1>maxAP) {
				maxAP = f1;
				i2 = i1;
			}
			if (f2>maxAP) {
				maxAP = f2;
				i2 = i1 + 1;
			}
			if ((f1>0) && (f1<minAP)) minAP = f1;
			if ((UPower>=f1) && (UPower<f2)) pos1 = i1;
		} 
		if (pos1>0) {
			pos2 = pos1 + 1;
		} else {
			if (UPower < minAP) {
				UPower = minAP;
				pos1 = 1;
				pos2 = 2;
			}
			if (UPower >= maxAP) {
				UPower = maxAP;
				pos1 = i2 - 1;
				pos2 = i2;
				if (i2 < 2) {
					pos1 = 1;
					pos2 = 2;
				}
			}
		}
		
		if (pos2 > MAXAGCPOINTS) {
			pos2 = MAXAGCPOINTS;
			pos1 = pos2 - 1;
		}
		if (pos1 < 1) {
			pos1 = 1;
			pos2 = 2;
		}
		
		/*计算插值因子Calculate interpolation factor*/
		f1 = AGC_Unit_Data[UType].EffCurve[pos1].OutPow;
		f2 = AGC_Unit_Data[UType].EffCurve[pos2].OutPow;
		f4 = UPower;
		if (f4<f1) f4 = f1;
		if (f4>f2) f4 = f2;
		f3 = f2-f1;
		if (f3>0) {
			APfact = (f4-f1) / f3;
			
			/*计算效率Calculate efficiency*/
			f1 = AGC_Unit_Data[UType].EffCurve[pos1].Eff;
			f2 = AGC_Unit_Data[UType].EffCurve[pos2].Eff;
			Eff = (f1 + ((f2 - f1) * APfact)) / 100;  
			if (Eff<0.4) Eff = (float)0.4;  
			if (Eff>0.995) Eff =(float) 0.995;
			InputPower = UnitPower / Eff;
		}
	}
	return InputPower;
}
/*检查机组是否在振动区*/
int CheckPowerVibZone(int UnitType, float UnitPower, float Deadband)
{
    int VibZone;
    int i1;
    float f1,f2;
	
    i1 = 0;
    VibZone = 0;
    if ((UnitType>0) && (UnitType<=MAXTYPES)) 
    {
        while ((i1<VIBRATION_NUMB) && (VibZone==0)) 
		{
            i1++;
            f1 = AGC_Unit_Vibr[UnitType].Vibration[i1].AP_Begin + Deadband;
            f2 = AGC_Unit_Vibr[UnitType].Vibration[i1].AP_End - Deadband;
            if ((UnitPower>=f1) && (UnitPower<f2)) VibZone = i1;
        }
    }
    return VibZone;
}

int CalcUnitZone(int UnitType, float UnitPower)
{
	int Zone;
	int i1;
	float f1;
	//[1][Vibration][2][Virbation][3][Vibration][4][Vibration]   etc. ...
	Zone = 1;
	if ((UnitType>0) && (UnitType<=MAXTYPES)) {
		for (i1=1; i1<=VIBRATION_NUMB; i1++) {
			if (AGC_Unit_Vibr[UnitType].Vibration[i1].Used==1) {
				f1 = (AGC_Unit_Vibr[UnitType].Vibration[i1].AP_End +AGC_Unit_Vibr[UnitType].Vibration[i1].AP_Begin)/(float)2.0;/*以振动区中点作为判据*/
				if (UnitPower>f1) Zone++;
			}
		}
	}
	return Zone;
}

int JustUnitinVibEdge(int UnitType, float UnitPower)
{
	int Zone;
	int i1;
	
	//[1][Vibration][2][Virbation][3][Vibration][4][Vibration]   etc. ...
	Zone = 0;
	if ((UnitType>0) && (UnitType<=MAXTYPES)) {
		for (i1=1; i1<=VIBRATION_NUMB; i1++) {
			if (AGC_Unit_Vibr[UnitType].Vibration[i1].Used==1) {
				if(UnitPower==AGC_Unit_Vibr[UnitType].Vibration[i1].AP_Begin)
					Zone = 1;
				else if(UnitPower== AGC_Unit_Vibr[UnitType].Vibration[i1].AP_End)
					Zone =3;
			}
		}
	}
	return Zone;
}


void Statistics_Unit(int segment)	/*求机组工况*/
{
	int 	i,Num_AP_Avail=0,AGC_Generating=0,AGC_Standby=0,Num_Unit_Run=0;
	float	p_sum=0,p_sum_agc=0,p_sum_no_agc=0,Plant_sum=0;
	
	for (i=1; i<=UNIT_NUMB;i++)
	{ 
		if(UNIT[i].segment!=segment)continue;
		
		if(UNIT[i].Status==UNIT_IN_GEN)p_sum+=UNIT[i].AP_RtValue;
		UNIT[i].AP_Avail = 0;
		if ((UNIT[i].AP_Sel==1)&&(UNIT[i].AP_Permit==1))
		{
			if ( UNIT[i].Status == STAND_BY )
				AGC_Standby++;
			else if (UNIT[i].Status == UNIT_IN_GEN )
			{
				AGC_Generating++;
				Num_Unit_Run++;
				p_sum_agc+=UNIT[i].AP_RtValue;
			}
			UNIT[i].AP_Avail = 1;    
			Num_AP_Avail++;
		}
		else
		{
			if (UNIT[i].Status == UNIT_IN_GEN )
			{
				Num_Unit_Run++;
				p_sum_no_agc+=UNIT[i].AP_RtValue;
			}
		}
		AGC_UpdateRTDB_IND(DB_NAME_AP_CZKT[i],(short)UNIT[i].AP_Avail);  
		if((UNIT[i].Last_AP_Avail!=UNIT[i].AP_Avail)&&(UNIT[i].AP_Avail==1)&&(PLANT[segment].FirstRun==0))
		{  
			UNIT[i].XAPCompMode=1;   
			UNIT[i].APCompTime = StartCompTime;
		}
// 		if((UNIT[i].Last_AP_Avail==1)&&(UNIT[i].AP_Avail==0)&&(PLANT[segment].FirstRun==0)&&(PLANT[segment].Mode!=5)&&(PLANT[segment].AP_Faulty==0)&&(PLANT[segment].Closed_Loop!=OPEN_LOOP)&&(PLANT[segment].AP_MODE==1))
// 		{
// 			if((UNIT[i].AP_RtValue>100)&&(UNIT[i].Status==UNIT_IN_GEN))
// 				SendCommandToPLC(DB_NAME_YG_GD[i],C_APOWER,UNIT[i].AP_RtValue,0);
// 		}
// 		if((PLANT[segment].Last_AP_MODE==1)&&(PLANT[segment].AP_MODE==0)&&(UNIT[i].Last_AP_Avail==1)&&(PLANT[segment].Last_Closed_Loop!=OPEN_LOOP)&&(PLANT[segment].AP_Faulty==0)&&(PLANT[segment].FirstRun==0))
// 		{
// 			if((PLANT[segment].Mode!=5)&&(UNIT[i].AP_RtValue>100)&&(UNIT[i].Status==UNIT_IN_GEN))
// 				SendCommandToPLC(DB_NAME_YG_GD[i],C_APOWER,UNIT[i].AP_RtValue,0); 
// 		}
		UNIT[i].Last_AP_Avail=UNIT[i].AP_Avail;
	}
	PLANT[segment].Last_AP_MODE=PLANT[segment].AP_MODE;
	PLANT[segment].Last_Closed_Loop=PLANT[segment].Closed_Loop;
	PLANT[segment].p_sum=p_sum;
	PLANT[segment].AGC_Standby=AGC_Standby;
	PLANT[segment].AGC_Generating=AGC_Generating;
	PLANT[segment].Num_Unit_Run=Num_Unit_Run;
	PLANT[segment].p_sum_agc=p_sum_agc;
	PLANT[segment].Num_AP_Avail=Num_AP_Avail;
	PLANT[segment].p_sum_no_agc=p_sum_no_agc;  


	AGC_UpdateRTDB_ANA(DB_NAME_AP_RtSum[segment],PLANT[segment].p_sum);

	
	// printf("in Statistics_Unit:PLANT[%d].p_sum=%f,AGC_Controllable=%d,AGC_Generating=%d,AGC_Standby=%d\n",
	//	segment,PLANT[segment].p_sum,PLANT[segment].Num_AP_Avail,PLANT[segment].AGC_Generating,PLANT[segment].AGC_Standby);
}



void CalculatePowerZone(int segment) /*计算机组保持当前运行状态不跨越振动区的有功范围*/
{
    int i;
    PLANT[segment].UplmtCurrentZone=PLANT[segment].LowlmtCurrentZone=0;
    for(i=1;i<=UNIT_NUMB;i++)
    {
        if(UNIT[i].segment!=segment)continue;
        if((UNIT[i].Status==UNIT_IN_GEN)&&(UNIT[i].AP_Avail==1))
        {
            PLANT[segment].UplmtCurrentZone+=UNIT[i].CurrentUplmt;
            PLANT[segment].LowlmtCurrentZone+=UNIT[i].CurrentLowlmt;
        }
    }
    printf("in CalculatePowerZone:PLANT[%d].UplmtCurrentZone=%f,PLANT[%d].LowlmtCurrentZone=%f\n",segment,
		PLANT[segment].UplmtCurrentZone,segment,PLANT[segment].LowlmtCurrentZone);
	
}
float Calculate_Unit_Flow(int UnitNumber, float PowerValue)
{
	int UType;
	int i1,i2;
	int pos1,pos2;
	float f1,f2,f3;
	float minAP,maxAP,APfact;
	float Flow;
	
	UType = UNIT[UnitNumber].Type;
	Flow = 0;
	
	//Calculation of water flow
	if ((UType>0) && (UType<=MAXTYPES)) {
		minAP = 10000;
		maxAP = 0;
		
		//Find position in curve
		i2 = 1;
		pos1 = 0;
		pos2 = 0;
		for (i1=1; i1<MAXAGCPOINTS; i1++) {
			f1 = AGC_Unit_Data[UType].EffCurve[i1].OutPow;
			f2 = AGC_Unit_Data[UType].EffCurve[i1+1].OutPow;
			if (f1>maxAP) {
				maxAP = f1;
				i2 = i1;
			}
			if (f2>maxAP) {
				maxAP = f2;
				i2 = i1 + 1;
			}
			if ((f1>0) && (f1<minAP)) minAP = f1;
			if ((PowerValue>=f1) && (PowerValue<f2)) pos1 = i1;
		}
		if (pos1>0) {
			pos2 = pos1 + 1;
		} else {
			if (PowerValue <= minAP) {
				pos1 = 1;
				pos2 = 2;
			}
			if (PowerValue >= maxAP) {
				pos1 = i2 - 1;
				pos2 = i2;
				if (i2 < 2) {
					pos1 = 1;
					pos2 = 2;
				}
			}
		}
		
		if (pos2 > MAXAGCPOINTS) {
			pos2 = MAXAGCPOINTS;
			pos1 = pos2 - 1;
		}
		if (pos1 < 1) {
			pos1 = 1;
			pos2 = 2;
		}
		
		//Calculate interpolation factor
		f1 = AGC_Unit_Data[UType].EffCurve[pos1].OutPow;
		f2 = AGC_Unit_Data[UType].EffCurve[pos2].OutPow;
		if (PowerValue<f1) PowerValue = f1;
		if (PowerValue>f2) PowerValue = f2;
		f3 = f2-f1;
		if (f3>0) {
			APfact = (PowerValue-f1) / f3;
			
			//Calculate unit flow
			f1 = AGC_Unit_Data[UType].EffCurve[pos1].Flow;
			f2 = AGC_Unit_Data[UType].EffCurve[pos2].Flow;
			Flow = f1 + ((f2 - f1) * APfact);
		}	
	}
	return Flow;
}

void Calculate_Unit_Eff_Flow(int UnitNumber, float UnitPower)
{
	int UType;
	int i1,i2;
	int pos1,pos2;
	float f1,f2,f3;
	float minAP,maxAP,APfact;
	
	
	UType = UNIT[UnitNumber].Type;
	
	//Calculation of efficiency, input power and actual water flow
	if ((UType>0) && (UType<=MAXTYPES)) {
		minAP = 10000;
		maxAP = 0;
		
		//Find position in curve
		i2 = 1;
		pos1 = 0;
		pos2 = 0;
		for (i1=1; i1<MAXAGCPOINTS; i1++) {
			f1 = AGC_Unit_Data[UType].EffCurve[i1].OutPow;
			f2 = AGC_Unit_Data[UType].EffCurve[i1+1].OutPow;
			if (f1>maxAP) {
				maxAP = f1;
				i2 = i1;
			}
			if (f2>maxAP) {
				maxAP = f2;
				i2 = i1 + 1;
			}
			if ((f1>0) && (f1<minAP)) minAP = f1;
			if ((UnitPower>=f1) && (UnitPower<f2)) pos1 = i1;
		}
		if (pos1>0) {
			pos2 = pos1 + 1;
		} else {
			if (UnitPower <= minAP) {
				pos1 = 1;
				pos2 = 2;
			}
			if (UnitPower >= maxAP) {
				pos1 = i2 - 1;
				pos2 = i2;
				if (i2 < 2) {
					pos1 = 1;
					pos2 = 2;
				}
			}
		}
		
		if (pos2 > MAXAGCPOINTS) {
			pos2 = MAXAGCPOINTS;
			pos1 = pos2 - 1;
		}
		if (pos1 < 1) {
			pos1 = 1;
			pos2 = 2;
		}
		
		//Calculate interpolation factor
		f1 = AGC_Unit_Data[UType].EffCurve[pos1].OutPow;
		f2 = AGC_Unit_Data[UType].EffCurve[pos2].OutPow;
		if (UnitPower<f1) UnitPower = f1;
		if (UnitPower>f2) UnitPower = f2;
		f3 = f2-f1;
		if (f3>0) {
			APfact = (UnitPower-f1) / f3;
			
			//Calculate unit flow
			f1 = AGC_Unit_Data[UType].EffCurve[pos1].Flow;
			f2 = AGC_Unit_Data[UType].EffCurve[pos2].Flow;
			UNIT[UnitNumber].Flow = f1 + ((f2 - f1) * APfact);
			
			//Calculate unit efficiency
			f1 = AGC_Unit_Data[UType].EffCurve[pos1].Eff;
			f2 = AGC_Unit_Data[UType].EffCurve[pos2].Eff;
			UNIT[UnitNumber].Efficiency = f1 + ((f2 - f1) * APfact);
			
			//Calculate unit input power
			f1 = UNIT[UnitNumber].Efficiency / 100;
			if (f1>0.3) {
				UNIT[UnitNumber].InputPower = UNIT[UnitNumber].AP_RtValue / f1;
			} else {
				UNIT[UnitNumber].InputPower = (float)(UNIT[UnitNumber].AP_RtValue * 1.5);
			}
		}
	}
}


void AllocationCalculation(int segment)
{
    int i,i1,i2,i3,UnitUpVib[UNIT_NUMB+1]={0},UnitLowVib[UNIT_NUMB+1]={0},UpVibNum=0,UnitNumber=0,
		temp=0,LowVibNum=0,Compute_Num=0,Compute_Num1,num[UNIT_NUMB+1],flag[UNIT_NUMB+1],Result=0;
    float sum_factor=0,jzmp[UNIT_NUMB+1]={0},Current_Min=0,Current_Max=0,jzmp1[UNIT_NUMB+1]={0},
		mp[UNIT_NUMB+1]={0.0},Min,Max,p[UNIT_NUMB+1]={0},AP_Input,AP_Output;
    float Min1[4]={0},Min2[4]={0},Max1[4]={0},Max2[4]={0},Gap[4]={0},Allocation[4][UNIT_NUMB+1]={0},
		Efficiency[4]={0};
    int Direction=0,Zone1=0,Zone2=0,NotInVib=0,Start=0;
    int Stop_Priority=0,Stop_Num=0,AllocationInVib=0;
    float Unit_AGC_Min=0;
	
    for(i=1;i<=UNIT_NUMB;i++)
        if((UNIT[i].Status==UNIT_IN_GEN)&&(UNIT[i].AP_Avail==1))
        {
            if((UNIT[i].segment!=segment)||(UNIT[i].StopAction==1))continue;
            UNIT[i].XAP_Setp=(float)floor(UNIT[i].AP_Setp);
            sum_factor+=factor[i];
            UnitNumber+=1;  
            if(UNIT[i].XAPCompMode == 3)Start=1;
        }
		printf("in AllocationCalculation:PLANT[%d].AGC_P_Set=%f\n",segment,PLANT[segment].AGC_P_Set);
		
		i1=i2=0;
		for(i=1;i<=UNIT_NUMB;i++)
		{
			if((UNIT[i].segment!=segment)||(UNIT[i].StopAction==1))continue;
			if((UNIT[i].Status==UNIT_IN_GEN)&&(UNIT[i].AP_Avail==1))
			{ 
				mp[i]=jzmp[i]=(float)floor(PLANT[segment].AGC_P_Set*factor[i]/sum_factor); 
				if((PLANT[segment].AP_Dev>0)&&(jzmp[i]<UNIT[i].AP_Setp)&&(Start==0))
					jzmp[i]=UNIT[i].AP_Setp;
				if((PLANT[segment].AP_Dev<0)&&(jzmp[i]>UNIT[i].AP_Setp)&&(Start==0))
					jzmp[i]=UNIT[i].AP_Setp;
				i3=CheckPowerVibZone(UNIT[i].Type, mp[i],0);
				if((i3==0)&&(fabs(mp[i]-UNIT[i].AP_Setp)>AP_Step))
					NotInVib+=1;
				
				if(jzmp[i]>UNIT[i].CurrentUplmt)
				{
					jzmp[i]=UNIT[i].CurrentUplmt;
					if(jzmp[i]!=UNIT[i].AP_Max)
					{
						UnitLowVib[++i1]=i;
						LowVibNum+=1;
						printf("LowVibNum=%d\n",LowVibNum);
					}
				}
				else if(jzmp[i]<UNIT[i].CurrentLowlmt)
				{
					jzmp[i]=UNIT[i].CurrentLowlmt;
					if(jzmp[i]!=UNIT[i].AP_Min)
					{
						UnitUpVib[++i2]=i;
						UpVibNum+=1;
						printf("UpVibNum=%d\n",UpVibNum);
					}
				}
				if(fabs(jzmp[i]-UNIT[i].AP_Setp)<AP_Step)jzmp[i]=UNIT[i].AP_Setp;
				
				if(UNIT[i].XAPCompMode == 3)
				{
					jzmp[i]=mp[i];
					if(jzmp[i]<UNIT[i].AP_Min)jzmp[i]=UNIT[i].AP_Min;
					if(jzmp[i]>UNIT[i].AP_Max)jzmp[i]=UNIT[i].AP_Max;
					i3=CheckPowerVibZone(UNIT[i].Type, jzmp[i],0);
					if(i3!=0)
						jzmp[i]=AGC_Unit_Vibr[UNIT[i].Type].Vibration[i3].AP_Begin;
				}
				Min=GetUnitCurrentMin(i,jzmp[i]);
				Max=GetUnitCurrentMax(i,jzmp[i]);
				Current_Min+=Min;
				Current_Max+=Max;
				jzmp1[i]=jzmp[i];
				printf("No.%d=%f,mp=%f\n",i,jzmp[i],mp[i]);
			}  
		}
		if(UnitNumber==NotInVib)
		{
			for(i=1;i<=UNIT_NUMB;i++)
			{
				if((UNIT[i].segment!=segment)||(UNIT[i].StopAction==1))continue;
				if((UNIT[i].Status==UNIT_IN_GEN)&&(UNIT[i].AP_Avail==1))
					jzmp[i]=mp[i];
				UpVibNum=0;
				LowVibNum=0;
			}
		}
		//printf("Current_Min=%f,Current_Max=%f\n",Current_Min,Current_Max);
		if(UpVibNum>1)
		{
			for(i=1;i<UpVibNum;i++)
				for(i1=i+1;i1<=UpVibNum;i1++)
				{
					if(jzmp[UnitUpVib[i1]]>jzmp[UnitUpVib[i]])
					{
						temp=UnitUpVib[i];
						UnitUpVib[i]=UnitUpVib[i1];
						UnitUpVib[i1]=temp;
					}
				}
		}
		if(LowVibNum>1)
		{
			for(i=1;i<LowVibNum;i++)
				for(i1=i+1;i1<=LowVibNum;i1++)
				{
					if(jzmp[UnitLowVib[i1]]<jzmp[UnitLowVib[i]])
					{
						temp=UnitLowVib[i];
						UnitLowVib[i]=UnitLowVib[i1];
						UnitLowVib[i1]=temp;
					}
				}
		}
		if(PLANT[segment].AGC_P_Set<Current_Min)
			Direction=-1;
		else if(PLANT[segment].AGC_P_Set>Current_Max)
			Direction=1;
		else if(Start==1)
			Direction=-1;
		else
		{
			if(PLANT[segment].AP_Dev>=0)
				Direction=1;
			else
				Direction=-1;
		}
		
		while(Compute_Num<=3)
		{    
			Min1[Compute_Num]=Min2[Compute_Num]=10000;
			Max1[Compute_Num]=Max2[Compute_Num]=-1;
			if((Direction==-1)&&(Compute_Num!=0))
			{
				i2=UnitUpVib[Compute_Num];
				if(Compute_Num>UpVibNum)break;
				jzmp[i2]=jzmp1[i2]-AP_Step;
				i3=CheckPowerVibZone(UNIT[i2].Type, jzmp[i2],0);
				if(i3!=0)
					jzmp[i2]=AGC_Unit_Vibr[UNIT[i2].Type].Vibration[i3].AP_Begin;
				if(jzmp[i2]<UNIT[i2].AP_Min)jzmp[i2]=UNIT[i2].AP_Min;
				if(jzmp[i2]>UNIT[i2].AP_Max)jzmp[i2]=UNIT[i2].AP_Max;
			}
			else if((Direction==1)&&(Compute_Num!=0))
			{
				i2=UnitLowVib[Compute_Num];
				if(Compute_Num>LowVibNum)break;
				jzmp[i2]=jzmp1[i2]+AP_Step;
				i3=CheckPowerVibZone(UNIT[i2].Type, jzmp[i2],0);
				if(i3!=0)
					jzmp[i2]=AGC_Unit_Vibr[UNIT[i2].Type].Vibration[i3].AP_End;
				if(jzmp[i2]<UNIT[i2].AP_Min)jzmp[i2]=UNIT[i2].AP_Min;
				if(jzmp[i2]>UNIT[i2].AP_Max)jzmp[i2]=UNIT[i2].AP_Max;
			}
			
			Gap[Compute_Num]=PLANT[segment].AGC_P_Set;
			for(i=1;i<=UNIT_NUMB;i++)
			{
				if((UNIT[i].segment!=segment)||(UNIT[i].StopAction==1))continue;
				if((UNIT[i].Status==UNIT_IN_GEN)&&(UNIT[i].AP_Avail==1))
				{
					if(jzmp[i]<UNIT[i].AP_Min)jzmp[i]=UNIT[i].AP_Min;
					if(jzmp[i]>UNIT[i].AP_Max)jzmp[i]=UNIT[i].AP_Max;
					Allocation[Compute_Num][i]=jzmp[i];
					Gap[Compute_Num]-=Allocation[Compute_Num][i];
					// printf("************%d***Gap=%f,*%f\n",Compute_Num,Gap[Compute_Num],jzmp[i]);
				}
			}
			//printf("in AllocationOverVZDeadband:Gap[%d]=%f\n",Compute_Num,Gap[Compute_Num]);
			
			Compute_Num1=0;
			while((Gap[Compute_Num]!=0)&&(Compute_Num1<=10))
			{  
				Compute_Num1++;
				for(i=1;i<=UNIT_NUMB;i++)
				{
					num[i]=0;     
					flag[i]=0;
				}
				for(i=1;i<=UnitNumber;i++)
				{
					i2=0;Max=-32767;
					for(i3=1;i3<=UNIT_NUMB;i3++)
					{
						if((UNIT[i3].segment!=segment)||(UNIT[i3].StopAction==1))continue;
						if((UNIT[i3].Status==UNIT_IN_GEN)&&(UNIT[i3].AP_Avail==1)&&(flag[i3]==0))
						{ 
							if(Allocation[Compute_Num][i3]>=Max)
							{
								Max=Allocation[Compute_Num][i3];
								i2=i3;
							}
						}
					}
					if(i2!=0)
					{
						num[i]=i2;      /*从大到小的顺序排列*/
						flag[i2]=1;   
					} 
				}
				for(i=1;i<=UnitNumber;i++)
				{
					if(Gap[Compute_Num]<0)
						i2=num[i];
					else
						i2=num[UnitNumber+1-i];  
					if(UNIT[i2].segment!=segment)continue;
					if((UNIT[i2].Status==UNIT_IN_GEN)&&(UNIT[i2].AP_Avail==1))
					{
						Min=GetUnitCurrentMin(i2,Allocation[Compute_Num][i2]);
						Max=GetUnitCurrentMax(i2,Allocation[Compute_Num][i2]);
						
						if(fabs(Gap[Compute_Num])>AP_Step)
						{
							p[i2]=Allocation[Compute_Num][i2];
							if(Gap[Compute_Num]>0)
								Allocation[Compute_Num][i2]+=AP_Step;
							else
								Allocation[Compute_Num][i2]-=AP_Step;
							if((Direction==1)&&(Allocation[Compute_Num][i2]<jzmp[i2])&&(Compute_Num1<=2))
								Allocation[Compute_Num][i2]=jzmp[i2];
							if((Direction==-1)&&(Allocation[Compute_Num][i2]>jzmp[i2])&&(Compute_Num1<=2))
								Allocation[Compute_Num][i2]=jzmp[i2];
							if(Allocation[Compute_Num][i2]>Max)Allocation[Compute_Num][i2]=Max;
							if(Allocation[Compute_Num][i2]<Min)Allocation[Compute_Num][i2]=Min;
							Gap[Compute_Num]-=Allocation[Compute_Num][i2]-p[i2];
						}
						else
						{   //printf("Allocation[%d][%d]=%f\n",Compute_Num,i2,Allocation[Compute_Num][i2]);
							p[i2]=Allocation[Compute_Num][i2];  
							Allocation[Compute_Num][i2]+=Gap[Compute_Num];
							if((Direction==1)&&(Allocation[Compute_Num][i2]<jzmp[i2])&&(Compute_Num1<=2))
								Allocation[Compute_Num][i2]=jzmp[i2];
							if((Direction==-1)&&(Allocation[Compute_Num][i2]>jzmp[i2])&&(Compute_Num1<=2))
								Allocation[Compute_Num][i2]=jzmp[i2];
							if(Allocation[Compute_Num][i2]>Max)Allocation[Compute_Num][i2]=Max;
							if(Allocation[Compute_Num][i2]<Min)Allocation[Compute_Num][i2]=Min;
							Gap[Compute_Num]-=Allocation[Compute_Num][i2]-p[i2];
						}
						if(Allocation[Compute_Num][i2]>UNIT[i2].AP_Max)
							Allocation[Compute_Num][i2]=UNIT[i2].AP_Max;
						else if(Allocation[Compute_Num][i2]<UNIT[i2].AP_Min)
							Allocation[Compute_Num][i2]=UNIT[i2].AP_Min;
					}
				}
			}
			
			for(i=1;i<=UNIT_NUMB;i++)
			{
				if((UNIT[i].segment!=segment)||(UNIT[i].StopAction==1))continue;
				if((UNIT[i].Status==UNIT_IN_GEN)&&(UNIT[i].AP_Avail==1))
				{   
					Zone1=JustUnitinVibEdge(UNIT[i].Type, Allocation[Compute_Num][i]);
					
					if((PLANT[segment].AP_Dev>0)&&(Zone1==1))
					{
						if(Allocation[Compute_Num][i]>Max1[Compute_Num])
							Max1[Compute_Num]=Allocation[Compute_Num][i];
						if(Allocation[Compute_Num][i]<Min1[Compute_Num])
							Min1[Compute_Num]=Allocation[Compute_Num][i];
					}
					else if((PLANT[segment].AP_Dev<0)&&(Zone1==3))
					{
						if(Allocation[Compute_Num][i]>Max1[Compute_Num])
							Max1[Compute_Num]=Allocation[Compute_Num][i];
						if(Allocation[Compute_Num][i]<Min1[Compute_Num])
							Min1[Compute_Num]=Allocation[Compute_Num][i];
					}
					else 
					{
						if(Allocation[Compute_Num][i]>Max2[Compute_Num])
							Max2[Compute_Num]=Allocation[Compute_Num][i];
						if(Allocation[Compute_Num][i]<Min2[Compute_Num])
							Min2[Compute_Num]=Allocation[Compute_Num][i];
					}
					printf("Allocation[%d][%d]=%f,",Compute_Num,i,Allocation[Compute_Num][i]);
				} 
			}
			printf("\nGap=%f,Min1=%f,Max1=%f,Min2=%f,Max2=%f\n",Gap[Compute_Num],Min1[Compute_Num],
				Max1[Compute_Num],Min2[Compute_Num],Max2[Compute_Num]);
			Compute_Num++;
    }  
    for(i=0;i<=3;i++)
    {
        if(Direction==1){if(i>LowVibNum)break;}
        if(Direction==-1){if(i>UpVibNum)break;}
        AP_Input=AP_Output=0;
        for(i1=1;i1<=UNIT_NUMB;i1++)
        {
            if(UNIT[i1].segment!=segment)continue;
            if((UNIT[i1].Status==UNIT_IN_GEN)&&(UNIT[i1].AP_Avail==1))
            {
                AP_Output+=Allocation[i][i1];
                AP_Input+=Calculate_Input_Power(i1,Allocation[i][i1]);
            }
        }
        if(AP_Input>0)
            Efficiency[i]=AP_Output/AP_Input*100;
        else 
            Efficiency[i]=0;
        printf("Efficiency[%d]=%f\n",i,Efficiency[i]);
    }
    printf("AP_Dev=%f,Direction=%d,LowVibNum=%d,UpVibNum=%d\n",PLANT[segment].AP_Dev,Direction,LowVibNum,UpVibNum);
    for(i=0;i<3;i++)
    {
        if(Direction==1){if(i>=LowVibNum)break;}
        if(Direction==-1){if(i>=UpVibNum)break;}
        printf("Gap[%d]=%f,Gap[%d]=%f\n",i,Gap[i],i+1,Gap[i+1]);
        printf("Direction=%d,Min1[%d]=%f,Max1[%d]=%f,Min2[%d]=%f,Max2[%d]=%f\n",Direction,
			i,Min1[i],i,Max1[i],i,Min2[i],i,Max2[i]);
        printf("Efficiency[%d]=%f,Efficiency[%d]=%f,EC_Deadband=%f\n",i,Efficiency[i],i+1,Efficiency[i+1],PLANT[segment].EC_Deadband);
        
        if((fabs(Gap[i])-fabs(Gap[i+1])>PLANT[segment].VZ_Deadband)
			||((Min1[i]-Max2[i]>3*PLANT[segment].VZ_Deadband)&&(Min1[i]!=10000)&&(Max2[i]!=-1)&&(PLANT[segment].AP_Dev<0)&&(Gap[i+1]==0))
			||((Min2[i]-Max1[i]>3*PLANT[segment].VZ_Deadband)&&(Min2[i]!=10000)&&(Max1[i]!=-1)&&(PLANT[segment].AP_Dev>0)&&(Gap[i+1]==0))
			||((Efficiency[i+1]-Efficiency[i]>PLANT[segment].EC_Deadband)&&(Gap[i+1]==0)))
        {
            Result=i+1;
        }
        else
        {
            Result=i;
            break;
        }
    }
	
	AGC_UpdateRTDB_ANA(DB_NAME_Gap[segment],-Gap[Result]);
	//下列针对功率缺额大于振动区的情况
	if((fabs(Gap[Result])>Max_Power_Gap)&&(PLANT[segment].AGC_P_Set<PLANT[segment].AP_Max))
	{
		if((Start==1)&&(Gap[Result]<0))
		{
			for(i=1;i<=UNIT_NUMB;i++)
			{
				if(UNIT[i].XAPCompMode==3)
				{
					Allocation[Result][i]+=Gap[Result];
					UNIT[i].AllocationInVib=1;
					break;
				}
			}
		}
		else
		{
			temp=0;
			for(i=1;i<=UNIT_NUMB;i++)
				temp+=(int)UNIT[i].Privilege;
			for(i=1;i<=UNIT_NUMB;i++)
			{
				if(UNIT[i].segment!=segment)continue;
				if(UNIT[i].AP_Avail==0)continue;
				if(temp==0)
				{
					if((UNIT[i].Generating_Time>Stop_Priority)&&(UNIT[i].XAnswerNo==0)
						&&(UNIT[i].Status==UNIT_IN_GEN)&&(UNIT[i].StopBlockTime==0))
					{
						Stop_Priority=(int)UNIT[i].Generating_Time;
						Stop_Num=i;
					}
				}
				else  
				{
					if((UNIT[i].Privilege>Stop_Priority)&&(UNIT[i].XAnswerNo==0)
						&&(UNIT[i].Status==UNIT_IN_GEN)&&(UNIT[i].StopBlockTime==0))
					{
						Stop_Priority=(int)UNIT[i].Privilege;
						Stop_Num=i;  
					}
				} 
				if(UNIT[i].AllocationInVib==1)AllocationInVib=1;
				if(UNIT[i].Status==UNIT_IN_GEN)
					Unit_AGC_Min+=UNIT[i].AP_Min;
			}
			for(i=1;i<=UNIT_NUMB;i++)
			{
				if(UNIT[i].segment!=segment)continue;
				if(UNIT[i].AP_Avail==0)continue;
				if(AllocationInVib==1)
				{
					if(UNIT[i].AllocationInVib==1)
					{
						if((Allocation[Result][i]+Gap[Result]>0)
							&&(Allocation[Result][i]+Gap[Result]<UNIT[i].AP_Max))
						{
							Allocation[Result][i]+=Gap[Result];
							UNIT[i].AllocationInVib=1;
							break;
						}
					}
				}
				else if(PLANT[segment].AGC_P_Set<Unit_AGC_Min)
				{
					if((Allocation[Result][Stop_Num]+Gap[Result]>0)
						&&(Allocation[Result][Stop_Num]+Gap[Result]<UNIT[Stop_Num].AP_Max))
					{
						Allocation[Result][Stop_Num]+=Gap[Result];
						UNIT[Stop_Num].AllocationInVib=1;
						break;
					}
				}
				else if((Allocation[Result][i]+Gap[Result]>UNIT[i].AP_Min)
					&&(Allocation[Result][i]+Gap[Result]<UNIT[i].AP_Max))
				{
					Allocation[Result][i]+=Gap[Result];
					UNIT[i].AllocationInVib=1;
					break;
				}
			}
		}
	}
	else
	{
		for(i=1;i<=UNIT_NUMB;i++)
			UNIT[i].AllocationInVib=0;
	}
    for(i=1;i<=UNIT_NUMB;i++)
    {
        if((UNIT[i].segment!=segment)||(UNIT[i].StopAction==1))continue;
        if((UNIT[i].Status==UNIT_IN_GEN)&&(UNIT[i].AP_Avail==1))
        {
            UNIT[i].XAP_Setp=Allocation[Result][i];
            printf("in AllocationCalculation:Result=%d,UNIT[%d].XAP_Setp=%f\n",Result,i,UNIT[i].XAP_Setp);
        }
        else
            UNIT[i].XAP_Setp=UNIT[i].AP_RtValue;
    }  
}

void AGC_PowerAllocation(int segment)
{ 
    int i,NewZone,Unit_Comp=0,StartStop;
#ifdef RECORD
    FILE    *fp_Record;
    char filename[50],str[50];
#endif
    float Allocation_Sum,Gap,AP_WorkSP;
    StartStop=0;
    Allocation_Sum=0;
    Gap=0;
    AP_WorkSP=PLANT[segment].AP_WorkSP;
    PLANT[segment].AP_Dev = AP_WorkSP -PLANT[segment].Last_AP_WorkSP;
    if(AP_WorkSP>PLANT[segment].AP_Max)
        AP_WorkSP=PLANT[segment].AP_Max;
    else if(AP_WorkSP<PLANT[segment].AP_Min)
        AP_WorkSP=PLANT[segment].AP_Min;
    
    PLANT[segment].AGC_P_Set=AP_WorkSP-PLANT[segment].p_sum_no_agc;
	
    for(i=1;i<=UNIT_NUMB;i++)
    {
        if(UNIT[i].segment!=segment)continue;
        if ((UNIT[i].AP_Avail==1) && (PLANT[segment].Closed_Loop!=OPEN_LOOP))
		{
            if (UNIT[i].Status==UNIT_IN_GEN)
            {
				if ((UNIT[i].StopAction==0) && (UNIT[i].AP_Fail ==0))
				{
                    if (PLANT[segment].AGC_Generating> 1)
                    {
                        if((UNIT[i].Last_Status!=UNIT_IN_GEN) &&(PLANT[segment].FirstRun!=1))
						{
                            printf("Unit Last Status isn't generating\n");
                            UNIT[i].XAPCompMode = 3;
                            UNIT[i].APCompTime = StopCompTime;
                            StartStop=1; 
                            SetUnitVibMinMax(i);
                            if(UNIT[i].XAPCompMode>0)Unit_Comp=1;  
                            PLANT[segment].Start_AP_Dev=AP_WorkSP-PLANT[segment].p_sum;
                            UNIT[i].StartProcess=1;
                        }
                    }
				}
            }
		}
        if(UNIT[i].StopAction==1)
		{
            UNIT[i].XAP_Setp=APStopMin;
            Unit_Comp=1;
            StartStop=1;
		}
    }
    
    CalculatePowerZone(segment);
    printf("PLANT[%d].AGC_P_Set=%f,AP_Max=%f,AP_Min=%f,UplmtCurrentZone=%f,LowLmtCurrentZone=%f\n",
		segment,PLANT[segment].AGC_P_Set,PLANT[segment].AP_Max,PLANT[segment].AP_Min,
		PLANT[segment].UplmtCurrentZone,PLANT[segment].LowlmtCurrentZone);
    
    AllocationCalculation(segment);
	
    for(i=1;i<=UNIT_NUMB;i++)
    {
		if(UNIT[i].segment!=segment)continue;   
		if((UNIT[i].Status==UNIT_IN_GEN)&&(UNIT[i].AP_Avail==1)&&(StartStop!=1))
		{  
            NewZone = CalcUnitZone(UNIT[i].Type, UNIT[i].XAP_Setp);
            if (UNIT[i].Zone != NewZone)
            {
                printf("NewZone=%d,XAP_Setp=%f,Zone=%d\n",NewZone,UNIT[i].XAP_Setp,UNIT[i].Zone);
                UNIT[i].XAPCompMode = 2;
				UNIT[i].APCompTime = VibCompTime; 
                UNIT[i].Zone = NewZone;
                Unit_Comp=1;
            }
		}
        if((UNIT[i].Status==UNIT_IN_GEN)&&(UNIT[i].AP_Avail==1)&&(UNIT[i].StopAction!=1))
        {
            if(UNIT[i].XAP_Setp>UNIT[i].AP_Max)UNIT[i].XAP_Setp=UNIT[i].AP_Max;
            if((UNIT[i].XAP_Setp<UNIT[i].AP_Min)&&(UNIT[i].AllocationInVib==0))UNIT[i].XAP_Setp=UNIT[i].AP_Min;
        }
		if((UNIT[i].Status==UNIT_IN_GEN)&&(UNIT[i].AP_Avail==1)&&(UNIT[i].StopAction!=1))
		{
            printf("In AGC_PowerAllocation:APCompMode=%d,No.%d Unit XAP_Setp=%f\n",UNIT[i].XAPCompMode,i,UNIT[i].XAP_Setp);
            Allocation_Sum+=UNIT[i].XAP_Setp;
		}
	}	
    Allocation_Sum+=PLANT[segment].p_sum_no_agc;
    Gap=AP_WorkSP-Allocation_Sum;
	if(Unit_Comp==1)
		PLANT[segment].XAGC_Unit_Comp=Unit_Comp; 
#ifdef RECORD
	sprintf( filename, "/home/ems/h9000/his/agc-xjb/%02d%02dAGCRecord.xls",ntime->tm_mon+1,ntime->tm_mday);
	fp_Record=fopen(filename, "a");
	sprintf(str,"\n%f\t%f\t%f",PLANT[segment].AP_WorkSP,Allocation_Sum,Gap);
	fwrite(str,strlen(str),1,fp_Record);
	for(i=1;i<=UNIT_NUMB;i++)
	{
		if(UNIT[i].StopAction!=1)
			sprintf(str,"\t%f",UNIT[i].XAP_Setp);
		else
			sprintf(str,"\t%f",0);
		fwrite(str,strlen(str),1,fp_Record);
	}
	fclose(fp_Record);
#endif
}

void PowerCurve_Module(int segment)
{
    int i,tmp;
    float Value[48];
    USHORT Point_Num,Point_Num1;
    CURVE_DATA curve;
    time_t tim;
    struct tm tmpt;
    DMS_COMMON common_aa;
    char ErrSms[100];
	
    tim=time (NULL);
    tmpt=*localtime (&tim);
    
    if((tmpt.tm_hour<23)||(tmpt.tm_min<59)||(tmpt.tm_sec<55))Move_Curve[segment]=0;
    if((tmpt.tm_hour==23)&&(tmpt.tm_min==59)&&(tmpt.tm_sec>55)&&(Move_Curve[segment]==0))
    { 
        GetCurvePtSegNumByNameStr( DB_NAME_MRFHQX_SD[segment], &Point_Num1);
        GetCurvePtDataByNameStr(DB_NAME_MRFHQX_SD[segment],&curve);
		
		if(ReadEntryByNameStr(DB_NAME_JRFHQX_SD[segment],&common_aa)==-1)
		{
			printf("No point %s in DB!\n",DB_NAME_JRFHQX_SD[segment]);
			sprintf(ErrSms,"No point %s in DB!",DB_NAME_JRFHQX_SD[segment]);
			Err_log(ErrSms);
		}
        for(i=0;i<48;i++){Value[i]=curve.value[i];}
        CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value);
		
        for(i=48;i<96;i++){Value[i-48]=curve.value[i];}
		CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value);
        
        GetCurvePtDataByNameStr(DB_NAME_MRFHQX_XZ[segment],&curve);
		
		if(ReadEntryByNameStr(DB_NAME_JRFHQX_XZ[segment],&common_aa)==-1)
		{
			printf("No point %s in DB!\n",DB_NAME_JRFHQX_XZ[segment]);
			sprintf(ErrSms,"No point %s in DB!",DB_NAME_JRFHQX_XZ[segment]);
			Err_log(ErrSms);
        }
        for(i=0;i<48;i++){Value[i]=curve.value[i];}
		CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value);
        
        for(i=48;i<96;i++){Value[i-48]=curve.value[i];}
		CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value);
		
        Move_Curve[segment]=1;
    }
    //printf("Current Time:%d:%d:%d,%d :%d : %d\n",tmpt.tm_year+1900,tmpt.tm_mon+1,tmpt.tm_mday,tmpt.tm_hour,tmpt.tm_min,tmpt.tm_sec);
    GetCurvePtSegNumByNameStr( DB_NAME_JRFHQX_SD[segment], &Point_Num);
    GetCurvePtDataByNameStr(DB_NAME_JRFHQX_SD[segment],&curve);
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
    
    GetCurvePtSegNumByNameStr( DB_NAME_JRFHQX_XZ[segment], &Point_Num);
    GetCurvePtDataByNameStr(DB_NAME_JRFHQX_XZ[segment],&curve);
    if(PLANT[segment].CurvePointNum!=Point_Num)printf("Curve Point Number error!");
	if((((tmpt.tm_hour<1)&&(tmpt.tm_min<20))||((tmpt.tm_hour==23)&&(tmpt.tm_min>45)))&&(PLANT[segment].FirstRun==0))
	{
		for(i=0;i<Point_Num-1;i++)
			PLANT[segment].SR_Curve[i]=curve.value[i];
	}
	else if((tmpt.tm_hour<1)&&(tmpt.tm_min<20)&&(PLANT[segment].FirstRun==1))
	{
		for(i=0;i<Point_Num-1;i++)
			PLANT[segment].SR_Curve[i]=curve.value[i];
		PLANT[segment].SR_Curve[Point_Num-1]=curve.value[0];
	}
	else
	{
		for(i=0;i<Point_Num;i++)
			PLANT[segment].SR_Curve[i]=curve.value[i];
	}
    
    
    //if(((tmpt.tm_hour>=17)&&(tmpt.tm_hour<23))||((tmpt.tm_hour==23)&&(tmpt.tm_min<=30)))  不作此限制
    {     
		tmp=ReadDB_IND(DB_NAME_Copy_AP_SD[segment]); 
		if((tmp==1)&&(PLANT[segment].APCopyTime==0))
		{ 
			GetCurvePtSegNumByNameStr( DB_NAME_TD_MRFHQX_SD[segment], &Point_Num1);
			GetCurvePtDataByNameStr(DB_NAME_TD_MRFHQX_SD[segment],&curve);
			
			if(ReadEntryByNameStr(DB_NAME_MRFHQX_SD[segment],&common_aa)==-1)
			{
				printf("No point %s in DB!\n",DB_NAME_MRFHQX_SD[segment]);
				sprintf(ErrSms,"No point %s in DB!",DB_NAME_MRFHQX_SD[segment]);
				Err_log(ErrSms);
			}
			for(i=0;i<48;i++){Value[i]=curve.value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value);
			
			for(i=48;i<96;i++){Value[i-48]=curve.value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value);
			
			PLANT[segment].APCopyTime=Time_5s;
		}
		
		tmp=ReadDB_IND(DB_NAME_Copy_AP_XZ[segment]);    
		if((tmp==1)&&(PLANT[segment].SRCopyTime==0))
		{
			GetCurvePtSegNumByNameStr( DB_NAME_TD_MRFHQX_XZ[segment], &Point_Num1);
			GetCurvePtDataByNameStr(DB_NAME_TD_MRFHQX_XZ[segment],&curve);
			
			if(ReadEntryByNameStr(DB_NAME_MRFHQX_XZ[segment],&common_aa)==-1)
			{
				printf("No point %s in DB!\n",DB_NAME_MRFHQX_XZ[segment]);
				sprintf(ErrSms,"No point %s in DB!",DB_NAME_MRFHQX_XZ[segment]);
				Err_log(ErrSms);
			}
			for(i=0;i<48;i++){Value[i]=curve.value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value);
			
			for(i=48;i<96;i++){Value[i-48]=curve.value[i];}
			CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value);
			
			PLANT[segment].SRCopyTime=Time_5s;
		}
    }
	
	tmp=ReadDB_IND(DB_NAME_Copy_JR_AP_SD[segment]); 
	if((tmp==1)&&(PLANT[segment].JR_APCopyTime==0))
	{ 
		GetCurvePtSegNumByNameStr( DB_NAME_TD_JRFHQX_SD[segment], &Point_Num1);
		GetCurvePtDataByNameStr(DB_NAME_TD_JRFHQX_SD[segment],&curve);
		
		if(ReadEntryByNameStr(DB_NAME_JRFHQX_SD[segment],&common_aa)==-1)
		{
			printf("No point %s in DB!\n",DB_NAME_JRFHQX_SD[segment]);
			sprintf(ErrSms,"No point %s in DB!",DB_NAME_JRFHQX_SD[segment]);
			Err_log(ErrSms);
		}
		for(i=0;i<48;i++){Value[i]=curve.value[i];}
		CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value);
		
		for(i=48;i<96;i++){Value[i-48]=curve.value[i];}
		CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value);
		
		PLANT[segment].JR_APCopyTime=Time_5s;
	}
	
	tmp=ReadDB_IND(DB_NAME_Copy_JR_AP_XZ[segment]);    
	if((tmp==1)&&(PLANT[segment].JR_SRCopyTime==0))
	{
		GetCurvePtSegNumByNameStr( DB_NAME_TD_JRFHQX_XZ[segment], &Point_Num1);
		GetCurvePtDataByNameStr(DB_NAME_TD_JRFHQX_XZ[segment],&curve);
		
		if(ReadEntryByNameStr(DB_NAME_JRFHQX_XZ[segment],&common_aa)==-1)
		{
			printf("No point %s in DB!\n",DB_NAME_JRFHQX_XZ[segment]);
			sprintf(ErrSms,"No point %s in DB!",DB_NAME_JRFHQX_XZ[segment]);
			Err_log(ErrSms);
		}
		for(i=0;i<48;i++){Value[i]=curve.value[i];}
		CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,0,Value);
		
		for(i=48;i<96;i++){Value[i-48]=curve.value[i];}
		CurveAllPoint(common_aa.point.stn_id,common_aa.point.dev_id,48,common_aa.point.pt_id,48,Value);
		
		PLANT[segment].JR_SRCopyTime=Time_5s;
	}
}


void ReadAGC_DataFromRTDB(int segment)
{
    int	i,Type,tmp,status;
    float AP_Min,AP_Max,AP_Setpoint,Sum_AP_Min,Sum_AP_Max,XAP_Max;
    int Interval,tmp1,tmp2,PrePos,NextPos,Sel_Bus;
    char ErrSms[100];
    USHORT status1;
    char    pairhostname[20];
    long Second;
	
    PowerCurve_Module(segment); 
    PLANT[segment].Sel_Bus=ReadDB_IND(DB_NAME_SEL_BUS[segment]);
    Sel_Bus=PLANT[segment].Sel_Bus;
    
	if((segment==0)||(segment==1))
		PLANT[segment].FR_ActValue=ReadDB_ANA(DB_NAME_FR_ACTVALUE[Sel_Bus]);
	else
		PLANT[segment].FR_ActValue=ReadDB_ANA(DB_NAME_FR_ACTVALUE[Sel_Bus+2]);
	AGC_UpdateRTDB_ANA(DB_NAME_FR_REACT_VALUE[segment],PLANT[segment].FR_ActValue);
	
    PLANT[segment].AP_MODE=ReadDB_IND(DB_NAME_QC_AGC[segment]);
    PLANT[segment].Closed_Loop=ReadDB_IND(DB_NAME_CLOSED_LOOP[segment]);
    PLANT[segment].Control_Right=ReadDB_IND(DB_NAME_CONTROL_RIGHT[segment]);   
    PLANT[segment].AP_Setting=ReadDB_IND(DB_NAME_AP_Setting[segment]);
    PLANT[segment].AP_Curve=ReadDB_IND(DB_NAME_AP_Curve[segment]);
    PLANT[segment].FR_Regulation=ReadDB_IND(DB_NAME_FR_Regulation[segment]);
    PLANT[segment].FR_Compensation=ReadDB_IND(DB_NAME_FR_COMPENSATION[segment]);
    PLANT[segment].SR_AVAIL_Setting=ReadDB_IND(DB_NAME_SR_AVAIL_SETTING[segment]);
    PLANT[segment].SR_AVAIL_Curve=ReadDB_IND(DB_NAME_SR_AVAIL_CURVE[segment]);
    PLANT[segment].EC_Deadband=ReadDB_ANA(DB_NAME_EC_DEADBAND[segment]);
    PLANT[segment].FR_ChangeMax=ReadDB_ANA(DB_NAME_FR_ChangeMax[segment]);
    PLANT[segment].FR_Deadband=ReadDB_ANA(DB_NAME_FR_DEADBAND[segment]);
    PLANT[segment].FR_CompDeadband=ReadDB_ANA(DB_NAME_FR_CompDeadband[segment]);
    PLANT[segment].SS_Deadband=ReadDB_ANA(DB_NAME_SS_DEADBAND[segment]);
    PLANT[segment].VZ_Deadband=ReadDB_ANA(DB_NAME_VZ_DEADBAND[segment]);
    PLANT[segment].AP_ChangeMax=ReadDB_ANA(DB_NAME_AP_ChangeMax[segment]);
    PLANT[segment].APAnswerYes=ReadDB_IND(DB_NAME_AP_ANSWERYES[segment]);
    PLANT[segment].SSAnswerYes=ReadDB_IND(DB_NAME_SSAnswerYes[segment]);
    PLANT[segment].SSAnswerNo=ReadDB_IND(DB_NAME_SSAnswerNo[segment]);
    PLANT[segment].AP_Deadband=ReadDB_ANA(DB_NAME_AP_Deadband[segment]);
    PLANT[segment].FR_PowGenDB=ReadDB_ANA(DB_NAME_FR_PowGenDB[segment]);
    PLANT[segment].SR_MaxValue=ReadDB_ANA(DB_NAME_SR_MaxValue[segment]);
    PLANT[segment].SR_MaxChange=ReadDB_ANA(DB_NAME_SR_MaxChange[segment]);

	PLANT[segment].AllocationToUnit = ReadDB_IND(DB_NAME_AllocationToUnit[segment]);
#ifdef CASCADE
	PLANT[segment].yggd_pyl = ReadDB_ANA(DB_NAME_YGGD_PYL[segment]);
#endif
	if (PLANT[segment].FirstRun==1)
	{
		PLANT[segment].Last_AllocationToUnit=PLANT[segment].AllocationToUnit;
	}
	if (PLANT[segment].AllocationToUnit!=PLANT[segment].Last_AllocationToUnit)
	{
		PLANT[segment].AP_MODE=0;
		AGC_UpdateRTDB_IND(DB_NAME_QC_AGC[segment],0);
	}
	PLANT[segment].Last_AllocationToUnit=PLANT[segment].AllocationToUnit;

    
    PLANT[segment].Offset_Enable=ReadDB_IND(DB_NAME_Offset_Enable[segment]);
    if(PLANT[segment].AP_Curve!=1)
    {
		PLANT[segment].Offset_Enable=0;
		AGC_UpdateRTDB_IND(DB_NAME_Offset_Enable[segment],0);
    }
    
    PLANT[segment].Offset=ReadDB_ANA(DB_NAME_Offset[segment]);
    if(PLANT[segment].Offset>50)
    {
		PLANT[segment].Offset = 50;
		AGC_UpdateRTDB_ANA(DB_NAME_Offset[segment],PLANT[segment].Offset);
	}
	
	if(PLANT[segment].Offset_Enable==0)
		PLANT[segment].Offset = 0;
	
	PLANT[segment].AP_Faulty=0;
    for(i=1;i<=UNIT_NUMB;i++)
    {   
		if(UNIT[i].segment!=segment)continue;  
		UNIT[i].Status=CHECKOUT;
		tmp=ReadDB_IND(DB_NAME_ONLINE_STA[i]);
		if(tmp==1)
			UNIT[i].Status=UNIT_IN_GEN;  
		tmp=ReadDB_IND(DB_NAME_STOP_STA[i]);
		if(tmp==1)
			UNIT[i].Status=STAND_BY;  
        UNIT[i].Maint_Status=ReadDB_IND(DB_NAME_Maint_Status[i]);
		UNIT[i].AP_RtValue=ReadDB_ANA(DB_NAME_AP_RTVALUE[i]); 
		if(UNIT[i].Maint_Status==1)
            UNIT[i].Status=CHECKOUT;
		tmp=ReadDB_ANA_Chan(DB_NAME_AP_RTVALUE[i]); 
		if(tmp==1)
		{
            UNIT[i].AP_Faulty=1;
			if(UNIT[i].Status==1)
			{
				PLANT[segment].AP_Faulty=1;
				if(AP_Faulty[segment]==0)
				{
               	    sprintf(ErrSms,"%s:%d号机有功通道故障",PLANT[segment].English_Name,i);
               	    BroadCastSms(ErrSms,TRUE);
               	}
            }
		} 
		
		if(UNIT[i].Status!=UNIT_IN_GEN)
            UNIT[i].AP_RtValue=0; 
		UNIT[i].Standby_Time=ReadDB_ANA(DB_NAME_LXBYSJ[i]);
		UNIT[i].Generating_Time=ReadDB_ANA(DB_NAME_LXYXSJ[i]);
		UNIT[i].Privilege=ReadDB_ANA(DB_NAME_PRVILEGE[i]);
        AP_Max=ReadDB_HighLmt(DB_NAME_AP_RTVALUE[i]);
		AP_Min=ReadDB_LowLmt(DB_NAME_AP_RTVALUE[i]);
        Type=UNIT[i].Type;  
        UNIT[i].AP_Min=AGC_Unit_Data[Type].MinPower;
		UNIT[i].AP_Max=AGC_Unit_Data[Type].MaxPower; 
        
        if(AP_Max<UNIT[i].AP_Max)UNIT[i].AP_Max=AP_Max; 
        if(AP_Min>UNIT[i].AP_Min)UNIT[i].AP_Min=AP_Min; 
        if(PLANT[segment].FirstRun==1)
        {
            UNIT[i].Last_AP_Max=UNIT[i].AP_Max;
            UNIT[i].Last_AP_Min=UNIT[i].AP_Min;
        }
		if(fabs(UNIT[i].Last_AP_Max-UNIT[i].AP_Max)>20)
        {
            printf("No.%d AP_Max has changed!\n",i);
            PLANT[segment].Lim_Change=1;
            UNIT[i].Last_AP_Max=UNIT[i].AP_Max;
        }
        if(fabs(UNIT[i].Last_AP_Min-UNIT[i].AP_Min)>20)
        {
            printf("No.%d AP_Min has changed!\n",i);
            PLANT[segment].Lim_Change=1;
            UNIT[i].Last_AP_Min=UNIT[i].AP_Min;
        }
		
		SetUnitVibMinMax(i);
        UNIT[i].AP_Sel=ReadDB_IND(DB_NAME_UNIT_AGCSEL[i]);
        UNIT[i].AP_Permit=ReadDB_IND(DB_NAME_UNIT_Permit[i]);
        UNIT[i].AP_Avail= ReadDB_IND(DB_NAME_AP_CZKT[i]);
		UNIT[i].Starting=ReadDB_IND(DB_NAME_STARTING[i]);
		UNIT[i].Stopping=ReadDB_IND(DB_NAME_STOPPING[i]);
    }   
    AP_Faulty[segment]=PLANT[segment].AP_Faulty;
    Statistics_Unit(segment);  
    
    PLANT[segment].State_SetValue=ReadDB_ANA(DB_NAME_STATE_SETVALUE[segment]);
    PLANT[segment].Cascade_SetValue=ReadDB_ANA(DB_NAME_CASECADE_SETVALUE[segment]);
    PLANT[segment].Operator_SetValue=ReadDB_ANA(DB_NAME_OPERATOR_SETVALUE[segment]);
    
    Sum_AP_Min=0;
    Sum_AP_Max=0;
	
    for(i=1;i<=UNIT_NUMB;i++)
    {
        if(UNIT[i].segment!=segment)continue;
        if(UNIT[i].AP_Avail==1)
        {
            Sum_AP_Max+=UNIT[i].AP_Max;
            if(UNIT[i].Status==UNIT_IN_GEN)
				Sum_AP_Min+=UNIT[i].AP_Min;
        }
        else
        {
            Sum_AP_Max+=UNIT[i].AP_RtValue;
            Sum_AP_Min+=UNIT[i].AP_RtValue;
        }  
    }
    PLANT[segment].AP_Min=ReadDB_ANA(DB_NAME_AP_MIN[segment]);
    XAP_Max=ReadDB_ANA(DB_NAME_AP_MAX[segment]);
	if(PLANT[segment].FirstRun!=1)
	{
		if(XAP_Max<PLANT[segment].AP_WorkSP)
		{
			if(PLANT[segment].XAP_Max>=PLANT[segment].AP_WorkSP)
				XAP_Max=PLANT[segment].XAP_Max;
			else
				XAP_Max=Sum_AP_Max;
			
			sprintf(ErrSms,"%s:设置有功最大值错误!",PLANT[i].English_Name);
			
			if(GetControlState()==1)
				BroadCastSms(ErrSms,TRUE);
			AGC_UpdateRTDB_ANA(DB_NAME_AP_MAX[segment],XAP_Max);
		}
	}
	PLANT[segment].XAP_Max=XAP_Max;
	PLANT[segment].AP_Max=PLANT[segment].XAP_Max;
    
    if(PLANT[segment].AP_Min<Sum_AP_Min)PLANT[segment].AP_Min=Sum_AP_Min;
    if(PLANT[segment].AP_Max>Sum_AP_Max)PLANT[segment].AP_Max=Sum_AP_Max;
    AGC_UpdateRTDB_ANA(DB_NAME_AP_XMIN[segment],PLANT[segment].AP_Min);
    AGC_UpdateRTDB_ANA(DB_NAME_AP_XMAX[segment],PLANT[segment].AP_Max);
	
    if(((PLANT[segment].AP_Setting==1)&&(PLANT[segment].FR_Regulation==1))
		||((PLANT[segment].AP_Setting==1)&&(PLANT[segment].AP_Curve==1))
		||((PLANT[segment].AP_Setting==1)&&(PLANT[segment].FR_Compensation==1))
        ||((PLANT[segment].AP_Curve==1)&&(PLANT[segment].FR_Regulation==1))
		||((PLANT[segment].AP_Curve==1)&&(PLANT[segment].FR_Compensation==1))
		||((PLANT[segment].FR_Regulation==1)&&(PLANT[segment].FR_Compensation==1)))
    {
		PLANT[segment].Mode=5;
        PLANT[segment].AP_Setting=0;
		PLANT[segment].AP_Curve=0;
        PLANT[segment].FR_Regulation=0;
		PLANT[segment].FR_Compensation=0;
        AGC_UpdateRTDB_IND(DB_NAME_AP_Setting[segment],0);
		AGC_UpdateRTDB_IND(DB_NAME_AP_Curve[segment],0);
        AGC_UpdateRTDB_IND(DB_NAME_FR_Regulation[segment],0);
		AGC_UpdateRTDB_IND(DB_NAME_FR_COMPENSATION[segment],0);
    }
    
    if((PLANT[segment].AP_Setting==0)&&(PLANT[segment].AP_Curve==0)&&(PLANT[segment].FR_Regulation==0)
		&&(PLANT[segment].FR_Compensation==0))
    {
		PLANT[segment].Mode=5;
        PLANT[segment].Closed_Loop=0;
        AGC_UpdateRTDB_IND(DB_NAME_CLOSED_LOOP[segment],0);  
        AGC_UpdateRTDB_ANA(DB_NAME_OPERATOR_SETVALUE[segment],PLANT[segment].p_sum);
        AGC_UpdateRTDB_ANA(DB_NAME_STATE_SETVALUE[segment],PLANT[segment].p_sum);
        AGC_UpdateRTDB_ANA(DB_NAME_CASECADE_SETVALUE[segment],PLANT[segment].p_sum);
        AGC_UpdateRTDB_ANA(DB_NAME_FR_SETPOINT[segment],PLANT[segment].FR_ActValue);
    }
    
    if((PLANT[segment].FirstRun==1)||(PLANT[segment].AP_MODE==0)||(PLANT[segment].Mode==5))
    {
		PLANT[segment].Last_Mode=5;
        PLANT[segment].FR_Setpoint=PLANT[segment].FR_ActValue;
        PLANT[segment].XFR_Setpoint=PLANT[segment].FR_ActValue;
        PLANT[segment].YFR_Setpoint=PLANT[segment].FR_ActValue;
        PLANT[segment].FR_WorkSP=PLANT[segment].FR_ActValue; 
        PLANT[segment].AP_Setpoint=PLANT[segment].p_sum;
        PLANT[segment].YAP_Setpoint=PLANT[segment].p_sum;
        PLANT[segment].XAP_Setpoint=PLANT[segment].p_sum;
        PLANT[segment].AP_WorkSP=PLANT[segment].p_sum;
		PLANT[segment].AP_Setpoint=PLANT[segment].p_sum;
    }  
    
    if(PLANT[segment].AP_Setting==1)
    {
		PLANT[segment].Mode=1;
        if(PLANT[segment].SR_AVAIL_Setting==1)
		{
            PLANT[segment].SR_SetPoint=ReadDB_ANA(DB_NAME_SR_SETPOINT[segment]);
			if(PLANT[segment].SR_SetPoint>PLANT[segment].SR_MaxValue)
			{
				PLANT[segment].SR_SetPoint=PLANT[segment].Last_SR_SetPoint;
				AGC_UpdateRTDB_ANA(DB_NAME_SR_SETPOINT[segment],PLANT[segment].SR_SetPoint);
			}
			if(fabs(PLANT[segment].SR_SetPoint-PLANT[segment].Last_SR_SetPoint)<PLANT[segment].SR_MaxChange)
			{
				PLANT[segment].Last_SR_SetPoint=PLANT[segment].SR_SetPoint;
			}
			else
			{
				PLANT[segment].SR_SetPoint=PLANT[segment].Last_SR_SetPoint;
				AGC_UpdateRTDB_ANA(DB_NAME_SR_SETPOINT[segment],PLANT[segment].SR_SetPoint);
			}
		}
		else
            PLANT[segment].SR_SetPoint=0; 
#ifndef CASCADE
        if(PLANT[segment].Control_Right==0)
        {
            PLANT[segment].XAP_Setpoint=PLANT[segment].Operator_SetValue;
            AGC_UpdateRTDB_ANA(DB_NAME_STATE_SETVALUE[segment],PLANT[segment].p_sum);
            AGC_UpdateRTDB_ANA(DB_NAME_CASECADE_SETVALUE[segment],PLANT[segment].p_sum);
        }
        else if(PLANT[segment].Control_Right==1)
        {
            PLANT[segment].XAP_Setpoint=PLANT[segment].Cascade_SetValue;
            AGC_UpdateRTDB_ANA(DB_NAME_OPERATOR_SETVALUE[segment],PLANT[segment].p_sum);
            AGC_UpdateRTDB_ANA(DB_NAME_STATE_SETVALUE[segment],PLANT[segment].p_sum);
        }
        else if(PLANT[segment].Control_Right==2)
        {
            PLANT[segment].XAP_Setpoint=PLANT[segment].State_SetValue;
            AGC_UpdateRTDB_ANA(DB_NAME_OPERATOR_SETVALUE[segment],PLANT[segment].p_sum);
            AGC_UpdateRTDB_ANA(DB_NAME_CASECADE_SETVALUE[segment],PLANT[segment].p_sum);
        }
#else
		if(PLANT[segment].Control_Right==0)
        {
            PLANT[segment].XAP_Setpoint=PLANT[segment].Cascade_SetValue+PLANT[segment].yggd_pyl;
            AGC_UpdateRTDB_ANA(DB_NAME_STATE_SETVALUE[segment],PLANT[segment].p_sum);
        }
        else if(PLANT[segment].Control_Right==1)
        {
            PLANT[segment].XAP_Setpoint=PLANT[segment].State_SetValue+PLANT[segment].yggd_pyl;
            AGC_UpdateRTDB_ANA(DB_NAME_CASECADE_SETVALUE[segment],PLANT[segment].p_sum);
        }
#endif
    }
    else if(PLANT[segment].AP_Curve==1)
    {
		PLANT[segment].Mode=2;
		AGC_UpdateRTDB_IND(DB_NAME_CONTROL_RIGHT[segment],0);
        ReadTime();
        
        Interval=24*60/PLANT[segment].CurvePointNum; 
        tmp1=(ntime->tm_hour*60+ntime->tm_min)/Interval;
        if((ntime->tm_sec>=60-AheadOfSchedule)&&(ntime->tm_sec<60-AheadOfSchedule+2))
        {
            tmp2=(ntime->tm_hour*60+ntime->tm_min)%Interval+1;
            Dispatch[segment]=0;
        }
        else 
        {
            tmp2=(ntime->tm_hour*60+ntime->tm_min)%Interval+1;
            Dispatch[segment]=1;
        }
		
        PrePos=tmp1-1;
        NextPos=tmp1;
        if(PrePos<0)
            PrePos=PLANT[segment].CurvePointNum-1;
        //printf("PrePos=%d,NextPos=%d\n",PrePos,NextPos);
        if(PLANT[segment].SR_AVAIL_Curve==1)
			PLANT[segment].SR_SetPoint=PLANT[segment].SR_Curve[PrePos];
		else
            PLANT[segment].SR_SetPoint=0;
        if((ntime->tm_sec>=60-AheadOfSchedule)&&(tmp2==Interval))
            PrePos=NextPos;
		
        AP_Setpoint=PLANT[segment].Curve[PrePos]+PLANT[segment].Offset+
            (PLANT[segment].Curve[NextPos]-PLANT[segment].Curve[PrePos])*(float)tmp2/(float)Interval;
		
		if(Dispatch[segment]==0)
		{
			if((PLANT[segment].XAP_Setpoint<AP_Setpoint)&&((AP_Setpoint-PLANT[segment].XAP_Setpoint)>=Sch_Inc_Value))
				PLANT[segment].XAP_Setpoint=AP_Setpoint;
			else if((PLANT[segment].XAP_Setpoint>AP_Setpoint)&&((PLANT[segment].XAP_Setpoint-AP_Setpoint)>=Sch_Dec_Value))
				PLANT[segment].XAP_Setpoint=AP_Setpoint;
		}
        if((ntime->tm_sec>=60-AheadOfSchedule)&&(tmp2==Interval))
            PLANT[segment].XAP_Setpoint=AP_Setpoint;
        //printf("Current Time is %d::%d::%d\n",ntime->tm_hour,ntime->tm_min,ntime->tm_sec);printf("***Sch_Inc_Value=%f,Sch_Dec_Value=%f,Dispatch=%d\n",Sch_Inc_Value,Sch_Dec_Value,Dispatch[segment]);
        //printf("AP_Setpoint=%f,PLANT[%d].XAP_Setpoint=%f,YAP_Setpoint=%f\n",AP_Setpoint,segment,PLANT[segment].XAP_Setpoint,PLANT[segment].YAP_Setpoint);
        if((PLANT[segment].Curve[PrePos]<10)||(PLANT[segment].Curve[NextPos]<10))
        {
            printf("曲线有功值前后两点为%f,%f\n",PLANT[segment].Curve[PrePos],PLANT[segment].Curve[NextPos]);
            PLANT[segment].AP_MODE=0;
            AGC_UpdateRTDB_IND(DB_NAME_QC_AGC[segment],0); 
            //sleep(100000);
            sprintf(ErrSms,"%s:曲线有功值过低退AGC!",PLANT[segment].English_Name);
            if(GetControlState()==1)
				BroadCastSms(ErrSms,TRUE);
        }
        
        AGC_UpdateRTDB_ANA(DB_NAME_OPERATOR_SETVALUE[segment],PLANT[segment].p_sum);
        AGC_UpdateRTDB_ANA(DB_NAME_CASECADE_SETVALUE[segment],PLANT[segment].p_sum);
        AGC_UpdateRTDB_ANA(DB_NAME_STATE_SETVALUE[segment],PLANT[segment].p_sum);
        //printf("PrePos=%d,%f,Current=%d,%f\n",PrePos,PLANT[segment].Curve[PrePos],NextPos,PLANT[segment].Curve[NextPos]);
    }
    else
        PLANT[segment].XAP_Setpoint=PLANT[segment].Operator_SetValue;  
	
    if(fabs(PLANT[segment].XAP_Setpoint-PLANT[segment].YAP_Setpoint)>=APSetpDB)
    {
        PLANT[segment].YAP_Setpoint=PLANT[segment].XAP_Setpoint;
        if(PLANT[segment].YAP_Setpoint>PLANT[segment].AP_Max)
        {
            if((PLANT[segment].Num_AP_Avail>0)&&((PLANT[segment].AP_Setting==1)||(PLANT[segment].AP_Curve==1)))
            {
				sprintf(ErrSms,"%s:有功设定越上限!",PLANT[segment].English_Name);
				
				AGC_UpdateRTDB_IND(DB_NAME_Setp_Error[segment],1);
				PLANT[segment].Setp_ErrorTime = Time_5s;
				if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
			}
			PLANT[segment].YAP_Setpoint=PLANT[segment].AP_Setpoint;
			if(PLANT[segment].AP_Setting==1)
			{
#ifdef CASCADE
				if(PLANT[segment].Control_Right==0)
					AGC_UpdateRTDB_ANA(DB_NAME_CASECADE_SETVALUE[segment],PLANT[segment].AP_Setpoint);
				else if(PLANT[segment].Control_Right==1)
					AGC_UpdateRTDB_ANA(DB_NAME_STATE_SETVALUE[segment],PLANT[segment].AP_Setpoint);
#else
				if(PLANT[segment].Control_Right==0)
					AGC_UpdateRTDB_ANA(DB_NAME_OPERATOR_SETVALUE[segment],PLANT[segment].AP_Setpoint);
				else if(PLANT[segment].Control_Right==1)
					AGC_UpdateRTDB_ANA(DB_NAME_CASECADE_SETVALUE[segment],PLANT[segment].AP_Setpoint);
				else if(PLANT[segment].Control_Right==2)
					AGC_UpdateRTDB_ANA(DB_NAME_STATE_SETVALUE[segment],PLANT[segment].AP_Setpoint);
#endif
			}
        }
		else if(PLANT[segment].YAP_Setpoint+0.5<PLANT[segment].AP_Min)
        {
            if((PLANT[segment].Num_AP_Avail>0)&&((PLANT[segment].AP_Setting==1)||(PLANT[segment].AP_Curve==1)))
            {
				sprintf(ErrSms,"%s:有功设定越下限!",PLANT[segment].English_Name);
				
				AGC_UpdateRTDB_IND(DB_NAME_Setp_Error[segment],1);
				PLANT[segment].Setp_ErrorTime = Time_5s;
				if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
            }
            PLANT[segment].YAP_Setpoint=PLANT[segment].AP_Setpoint;
            if(PLANT[segment].AP_Setting==1)
			{
#ifdef CASCADE
				if(PLANT[segment].Control_Right==0)
					AGC_UpdateRTDB_ANA(DB_NAME_CASECADE_SETVALUE[segment],PLANT[segment].AP_Setpoint);
				else if(PLANT[segment].Control_Right==1)
					AGC_UpdateRTDB_ANA(DB_NAME_STATE_SETVALUE[segment],PLANT[segment].AP_Setpoint);
#else
				if(PLANT[segment].Control_Right==0)
					AGC_UpdateRTDB_ANA(DB_NAME_OPERATOR_SETVALUE[segment],PLANT[segment].AP_Setpoint);
				else if(PLANT[segment].Control_Right==1)
					AGC_UpdateRTDB_ANA(DB_NAME_CASECADE_SETVALUE[segment],PLANT[segment].AP_Setpoint);
				else if(PLANT[segment].Control_Right==2)
					AGC_UpdateRTDB_ANA(DB_NAME_STATE_SETVALUE[segment],PLANT[segment].AP_Setpoint);
#endif
			}
        }
		if((fabs(PLANT[segment].YAP_Setpoint-PLANT[segment].AP_Setpoint)>PLANT[segment].AP_ChangeMax)
			&&(PLANT[segment].AP_Setting==1))
        {
            if((PLANT[segment].Num_AP_Avail>0)&&((PLANT[segment].AP_Setting==1)||(PLANT[segment].AP_Curve==1)))
            {
				if(segment==0)
				{
					if(PLANT[segment].Control_Right==0)
					{
						sprintf(ErrSms,"%s:有功设定越变幅!",PLANT[segment].English_Name);			
					}
					else if(PLANT[segment].Control_Right==1)
					{
						sprintf(ErrSms,"%s:梯调有功设定越变幅!",PLANT[segment].English_Name);
					}
					else if(PLANT[segment].Control_Right==2)
					{
						sprintf(ErrSms,"%s:地下国调有功设定越变幅!",PLANT[segment].English_Name);
					}
				}
								
				if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
				AGC_UpdateRTDB_IND(DB_NAME_Setp_Error[segment],1);
				PLANT[segment].Setp_ErrorTime = Time_5s;
            }
            PLANT[segment].YAP_Setpoint=PLANT[segment].AP_Setpoint;
            if(PLANT[segment].AP_Setting==1)
			{
#ifdef CASCADE
				if(PLANT[segment].Control_Right==0)
					AGC_UpdateRTDB_ANA(DB_NAME_CASECADE_SETVALUE[segment],PLANT[segment].AP_Setpoint);
				else if(PLANT[segment].Control_Right==1)
					AGC_UpdateRTDB_ANA(DB_NAME_STATE_SETVALUE[segment],PLANT[segment].AP_Setpoint);
#else
				if(PLANT[segment].Control_Right==0)
					AGC_UpdateRTDB_ANA(DB_NAME_OPERATOR_SETVALUE[segment],PLANT[segment].AP_Setpoint);
				else if(PLANT[segment].Control_Right==1)
					AGC_UpdateRTDB_ANA(DB_NAME_CASECADE_SETVALUE[segment],PLANT[segment].AP_Setpoint);
				else if(PLANT[segment].Control_Right==2)
					AGC_UpdateRTDB_ANA(DB_NAME_STATE_SETVALUE[segment],PLANT[segment].AP_Setpoint);
#endif
			}
        }
		else if((PLANT[segment].AP_Setting==1)||(PLANT[segment].AP_Curve==1))
			PLANT[segment].AP_Setpoint=PLANT[segment].YAP_Setpoint;
        if(PLANT[segment].AP_Curve==1)   //防止负荷曲线方式下越限反复报警
			PLANT[segment].XAP_Setpoint=PLANT[segment].YAP_Setpoint;
    } 
	
    if((PLANT[segment].FR_Regulation==1)||(PLANT[segment].FR_Compensation==1))
    {
		PLANT[segment].Mode=4;
        PLANT[segment].FR_Min=ReadDB_ANA(DB_NAME_FR_Min[segment]);
        PLANT[segment].FR_Max=ReadDB_ANA(DB_NAME_FR_Max[segment]);
		if(PLANT[segment].FR_Min<MinFreq)PLANT[segment].FR_Min=MinFreq;
		if(PLANT[segment].FR_Max>MaxFreq)PLANT[segment].FR_Max=MaxFreq;
        if((PLANT[segment].FR_Max<50)||(PLANT[segment].FR_Min>=PLANT[segment].FR_Max))
        {
            sprintf(ErrSms,"%s频率最大值错误",PLANT[segment].English_Name);
			
            if(GetControlState()==1)
				BroadCastSms(ErrSms,TRUE);
            AGC_UpdateRTDB_IND(DB_NAME_QC_AGC[segment],0);
            PLANT[segment].AP_MODE=0;
        }
        PLANT[segment].XFR_Setpoint=ReadDB_ANA(DB_NAME_FR_SETPOINT[segment]);
		if(fabs(PLANT[segment].XFR_Setpoint-PLANT[segment].YFR_Setpoint)>0.001)
		{
			PLANT[segment].YFR_Setpoint=PLANT[segment].XFR_Setpoint;
			if(PLANT[segment].YFR_Setpoint>PLANT[segment].FR_Max)
			{
				sprintf(ErrSms,"%s频率设定越上限!",PLANT[segment].English_Name);
			
				if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
				PLANT[segment].YFR_Setpoint=PLANT[segment].FR_Setpoint;
				AGC_UpdateRTDB_ANA(DB_NAME_FR_SETPOINT[segment],PLANT[segment].FR_Setpoint);
			}
			else if(PLANT[segment].YFR_Setpoint<PLANT[segment].FR_Min)
			{
				sprintf(ErrSms,"%s频率设定越下限!",PLANT[segment].English_Name);
				
				if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
				PLANT[segment].YFR_Setpoint=PLANT[segment].FR_Setpoint;
				AGC_UpdateRTDB_ANA(DB_NAME_FR_SETPOINT[segment],PLANT[segment].YFR_Setpoint);
			}
			if(fabs(PLANT[segment].YFR_Setpoint-PLANT[segment].FR_Setpoint)>PLANT[segment].FR_ChangeMax)
			{
				sprintf(ErrSms,"%s频率设定越变幅!",PLANT[segment].English_Name);
				
				if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
				PLANT[segment].YFR_Setpoint=PLANT[segment].FR_Setpoint;
				AGC_UpdateRTDB_ANA(DB_NAME_FR_SETPOINT[segment],PLANT[segment].YFR_Setpoint);
			}
			else
				PLANT[segment].FR_Setpoint=PLANT[segment].YFR_Setpoint;
			AGC_UpdateRTDB_ANA(DB_NAME_Real_FrSetpoint[segment],PLANT[segment].FR_Setpoint);
		}
		
		if(PLANT[segment].FR_Compensation!=1)
		{
            AGC_UpdateRTDB_ANA(DB_NAME_OPERATOR_SETVALUE[segment],PLANT[segment].p_sum);
            PLANT[segment].Mode=3;
		}
        AGC_UpdateRTDB_ANA(DB_NAME_STATE_SETVALUE[segment],PLANT[segment].p_sum);
        AGC_UpdateRTDB_ANA(DB_NAME_CASECADE_SETVALUE[segment],PLANT[segment].p_sum);
        //printf("FR_Min=%f,FR_Max=%f\n",PLANT[segment].FR_Min,PLANT[segment].FR_Max);
    } 
    else 
    {
        PLANT[segment].YFR_Setpoint=PLANT[segment].FR_Setpoint=PLANT[segment].FR_WorkSP=PLANT[segment].FR_ActValue;
        PLANT[segment].FR_Min=PLANT[segment].FR_Max=PLANT[segment].FR_ActValue;
        AGC_UpdateRTDB_ANA(DB_NAME_FR_SETPOINT[segment],PLANT[segment].FR_ActValue);
        AGC_UpdateRTDB_ANA(DB_NAME_Real_FrSetpoint[segment],PLANT[segment].FR_ActValue);
    }
	
    Second=time((time_t*)NULL);  
    if(PLANT[segment].FirstRun==1)
		SaveTime[segment]=Second;
    if(PLANT[segment].AP_Curve==1)
    {
		if(abs(SaveTime[segment]-Second)>600)
		{
			PLANT[segment].AP_MODE=0;
			AGC_UpdateRTDB_IND(DB_NAME_QC_AGC[segment],0);
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
    if((ControlState[segment]==2)&&(status==1)&&(PLANT[segment].AP_MODE==1)&&(status!=status1))
    {
		PLANT[segment].AP_MODE=0;
		AGC_UpdateRTDB_IND(DB_NAME_QC_AGC[segment],0);
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
			if(PLANT[segment].AP_MODE==1)
			{
				PLANT[segment].AP_MODE=0;
				AGC_UpdateRTDB_IND(DB_NAME_QC_AGC[segment],0);
				sprintf(ErrSms,"%s:%d号机状态异常退AGC!",PLANT[segment].English_Name,i);
				if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
			}
		}
	}
    if(PLANT[segment].AP_Faulty==1) PLANT[segment].AP_MODE=0;  
    if(PLANT[segment].AP_MODE!=1)
    {  
		PLANT[segment].Mode=5;   
        PLANT[segment].Closed_Loop=0;
        PLANT[segment].Control_Right=0;
        PLANT[segment].FR_Regulation=0;
        PLANT[segment].FR_Compensation=0;
        PLANT[segment].AP_Setting=0;
        PLANT[segment].AP_Curve=0;
        AGC_UpdateRTDB_IND(DB_NAME_CLOSED_LOOP[segment],0);
        AGC_UpdateRTDB_IND(DB_NAME_CONTROL_RIGHT[segment],0);
        AGC_UpdateRTDB_IND(DB_NAME_FR_Regulation[segment],0);
        AGC_UpdateRTDB_IND(DB_NAME_FR_COMPENSATION[segment],0);
        AGC_UpdateRTDB_IND(DB_NAME_AP_Setting[segment],0);
        AGC_UpdateRTDB_IND(DB_NAME_AP_Curve[segment],0);
        
        PLANT[segment].XAP_Setpoint=PLANT[segment].YAP_Setpoint=PLANT[segment].AP_Setpoint=PLANT[segment].p_sum;
        PLANT[segment].AP_WorkSP=PLANT[segment].p_sum;
        AGC_UpdateRTDB_ANA(DB_NAME_OPERATOR_SETVALUE[segment],PLANT[segment].p_sum);
        AGC_UpdateRTDB_ANA(DB_NAME_STATE_SETVALUE[segment],PLANT[segment].p_sum);
        AGC_UpdateRTDB_ANA(DB_NAME_CASECADE_SETVALUE[segment],PLANT[segment].p_sum);
        AGC_UpdateRTDB_ANA(DB_NAME_FR_SETPOINT[segment],PLANT[segment].FR_ActValue);
		// printf("segment=%d QC_AVC exit!All function exit!\n",segment);
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
					sprintf(Str1,"AGCcmd-xjb.%02d%02d*",ntime->tm_year+1900,ntime->tm_mon+1-3-i);
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
			sprintf(Str1,"AGCcmd-xjb.%02d*",ntime->tm_year+1900-1);
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
					sprintf(Str1,"AGCcmd-xjb.%02d%02d*",ntime->tm_year+1900-1,ntime->tm_mon+1+12-3-i);
					strcat(Str,Str1);
					system(Str);
				}
				else 
					break;
			}
		}
        oldday = ntime->tm_mday;
		memset(logfilename,0,90);
		sprintf( logfilename, "/home/ems/h9000/his/agc/AGCcmd-xjb.%02d%02d%02d",ntime->tm_year+1900,ntime->tm_mon+1,ntime->tm_mday);
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

void Load_AGC_Def(void)
{
    int	TypPos,WheadPos,i;
    char strTemp1[20],strTemp2[20],strTemp3[20],strTemp4[20],strTemp5[20];
    char FileName[50];
    char err_str[80]="";
    FILE	*fp;
	
    for (TypPos=1; TypPos<=MAXTYPES; TypPos++) UnitData[TypPos].used = 0;
    for(TypPos=1;TypPos<=MAXTYPES;TypPos++)
    {
        for(WheadPos=1;WheadPos<=MAXDEFPOINTS;WheadPos++)
        {
#ifdef CASCADE
            strcpy(FileName,"/home/ems/h9000/agc-xjb-cas/");
#else
			strcpy(FileName,"/home/ems/h9000/agc-xjb/");
#endif
            sprintf(strTemp1,"AGC_DEF_%d_%d.TXT",TypPos,WheadPos);
            strcat(FileName,strTemp1);
            if((fp=fopen(FileName,"r"))==NULL)
            {
                printf("Fail to open %s\n",FileName);
				sprintf(err_str, "Fail to open %s\n",FileName);
				Err_log(err_str);
                exit(0);
            }
            for(i=1;i<=3;i++)
            {
                fscanf(fp,"%s %s %s ",strTemp1,strTemp2,strTemp3);  
                if((strcmp(strTemp1,"WATERHEAD")==0)&&(strcmp(strTemp2,"=")==0))UnitData[TypPos].WaterHead[WheadPos].WaterHead=(float)atof(strTemp3);
                if((strcmp(strTemp1,"MIN_POWER")==0)&&(strcmp(strTemp2,"=")==0))UnitData[TypPos].WaterHead[WheadPos].MinPower=(float)atof(strTemp3);
                if((strcmp(strTemp1,"MAX_POWER")==0)&&(strcmp(strTemp2,"=")==0))UnitData[TypPos].WaterHead[WheadPos].MaxPower=(float)atof(strTemp3);
            }
            fscanf(fp,"%s %s %s %s %s",strTemp1,strTemp2,strTemp3,strTemp4,strTemp5); 
            if(strcmp(strTemp1,"ROW_POS")!=0||strcmp(strTemp2,"EFFICIENCY")!=0||strcmp(strTemp3,"POWER")!=0||strcmp(strTemp4,"FLOW")!=0||strcmp(strTemp5,"VIBRATION")!=0)
            {
                printf("Read %s error!\n",FileName);
				sprintf(err_str, "Read %s error!\n",FileName);
				Err_log(err_str);
                exit(0);
            }
			
            i=1;
            while(fscanf(fp,"%s %s %s %s %s",strTemp1,strTemp2,strTemp3,strTemp4,strTemp5)!=EOF)
            {
                UnitData[TypPos].WaterHead[WheadPos].EffCurve[i].Eff=(float)atof(strTemp2);
                UnitData[TypPos].WaterHead[WheadPos].EffCurve[i].OutPow=(float)atof(strTemp3);
                UnitData[TypPos].WaterHead[WheadPos].EffCurve[i].Flow=(float)atof(strTemp4);
                UnitData[TypPos].WaterHead[WheadPos].EffCurve[i].VibZone=atoi(strTemp5);
                i+=1;
            }
            fclose(fp);	 	
        }
    }  
}


void Start_Stop_Calculation(int segment)
{
    int i,Start_Priority,Start_Priority1,Stop_Priority,Start_Num,Stop_Num,tmp,Unit_Start,Unit_Stop;
    float Unit_AGC_Max,Unit_AGC_Min,Plant_Max,Plant_Min,Limvar,Unit_Min;
    float AP_Output,AP_Input,AP_Unit,Act_Eff,Eff_Start,Eff_Stop;
    int	Start_Req,Stop_Req,OneInStartAction,OneInStopAction;
    Unit_AGC_Max=0;
    Unit_AGC_Min=0;
    Plant_Max=0;
    Plant_Min=0;
    Start_Priority=10000;
    Start_Priority1=-1;
    Stop_Priority=-1;
    PLANT[segment].Unit_Start=0;
    PLANT[segment].Unit_Stop=0;
    Unit_Start=0;
    Unit_Stop=0;
    AP_Output=0;
    AP_Input=0;
    Act_Eff=0;
    tmp=0;
    Eff_Start=0;
    Limvar=0;
    Unit_Min=0;
    Start_Num=0;
    Stop_Num=0;
    Start_Req=PLANT[segment].Start_Req;
    Stop_Req=PLANT[segment].Stop_Req;
    PLANT[segment].XUnitStrStp =0;
    OneInStartAction=0;
    OneInStopAction=0;
    
    for(i=1;i<=UNIT_NUMB;i++)
        tmp+=(int)UNIT[i].Privilege;
    
    for(i=1;i<=UNIT_NUMB;i++)
    {
        if(UNIT[i].segment!=segment)continue;
        if(UNIT[i].AP_Avail==0)continue; 
		if(UNIT[i].StartAction==1)
			OneInStartAction=1;
		if(UNIT[i].StopAction==1)
			OneInStopAction=1;
        if(UNIT[i].Status==UNIT_IN_GEN)
        {
            Unit_AGC_Max+=UNIT[i].AP_Max;
            Unit_AGC_Min+=UNIT[i].AP_Min;
            AP_Output+=UNIT[i].XAP_Setp;
            AP_Input+=Calculate_Input_Power(i,UNIT[i].XAP_Setp);
        }
        if(tmp==0)/*没有设置开停机优先级*/
        {
            if((UNIT[i].Standby_Time>Start_Priority1)&&(UNIT[i].XAnswerNo==0)
				&&(UNIT[i].Status==STAND_BY)&&(UNIT[i].StartBlockTime==0))
            {
                Start_Priority1=(int)UNIT[i].Standby_Time;
                Start_Num=i;
            }
            if((UNIT[i].Generating_Time>Stop_Priority)&&(UNIT[i].XAnswerNo==0)
				&&(UNIT[i].Status==UNIT_IN_GEN)&&(UNIT[i].StopBlockTime==0))
            {
                Stop_Priority=(int)UNIT[i].Generating_Time;
                Stop_Num=i;
            }
        }
        else  /*设置开停机优先级*/
        {
            if((UNIT[i].Privilege<Start_Priority)&&(UNIT[i].XAnswerNo==0)
				&&(UNIT[i].Status==STAND_BY)&&(UNIT[i].StartBlockTime==0))
            {
                Start_Priority=(int)UNIT[i].Privilege;
                Start_Num=i;
            }
            if((UNIT[i].Privilege>Stop_Priority)&&(UNIT[i].XAnswerNo==0)
				&&(UNIT[i].Status==UNIT_IN_GEN)&&(UNIT[i].StopBlockTime==0))
            {
                Stop_Priority=(int)UNIT[i].Privilege;
                Stop_Num=i;  
            }
        } 
    }
	
    if (AP_Input>1) 
    {
        Act_Eff = (AP_Output / AP_Input) * 100;//求效率
    }
	
    Plant_Max=Unit_AGC_Max+PLANT[segment].p_sum_no_agc;
    Plant_Min=Unit_AGC_Min+PLANT[segment].p_sum_no_agc; 
    if(PLANT[segment].AP_WorkSP>Plant_Max)
    {
        Unit_Start=1; 
		Limvar=Plant_Min+UNIT[Start_Num].AP_Min+PLANT[segment].SS_Deadband;
		if(PLANT[segment].AP_WorkSP-Plant_Max>Max_Power_Gap)
			Limvar=Plant_Min+PLANT[segment].SS_Deadband;
        if(Limvar>=PLANT[segment].AP_WorkSP)
            Unit_Start=0;          
    }  
    if(PLANT[segment].Real_SRValue<PLANT[segment].SR_SetPoint)
		Unit_Start=1;     
    if(OneInStartAction==1)
		Unit_Start=0;       
    Limvar=Plant_Max-UNIT[Stop_Num].AP_Max-PLANT[segment].SS_Deadband;
    if((PLANT[segment].AP_WorkSP<Limvar)&&(PLANT[segment].AP_WorkSP<Plant_Min)&&
        (Plant_Max-UNIT[Stop_Num].AP_Max+Plant_Min>2*PLANT[segment].AP_WorkSP))
        Unit_Stop=1;   
    if(PLANT[segment].AP_WorkSP<0.9*Limvar)
        Unit_Stop=1;
    AP_Unit=AP_Output/(PLANT[segment].AGC_Generating-1);  
    AP_Input=0;
    for(i=1;i<=UNIT_NUMB;i++)
        if((UNIT[i].AP_Avail)&&(UNIT[i].Status==UNIT_IN_GEN)&&(i!=Stop_Num))
            AP_Input+=Calculate_Input_Power(i,AP_Unit);
		if (AP_Input>1) 
		{
			Eff_Stop=(AP_Output / AP_Input) * 100;
		}
		if((PLANT[segment].AP_WorkSP<Limvar)&&(Eff_Stop>Act_Eff))
			Unit_Stop=1;  
		//printf("Eff_Stop=%f,Act_Eff=%f\n",Eff_Stop,Act_Eff);
		
		if( PLANT[segment].Num_Unit_Run==1)  /*最后一台机组手动停*/
			Unit_Stop=0;
		if(OneInStopAction==1)
			Unit_Stop=0;     
		if(PLANT[segment].Real_SRValue<PLANT[segment].SR_SetPoint)
			Unit_Stop=0;
		
		if(Unit_Start==1)
		{
			PLANT[segment].Start_Req=1;
			PLANT[segment].Stop_Req=0;
			PLANT[segment].Unit_Start=Start_Num;
			PLANT[segment].XUnitStrStp =1; 
			UNIT[Start_Num].Start_Req=1;
			if(Start_Num!=0)
			{
				AGC_UpdateRTDB_IND(DB_NAME_Start_Req[Start_Num],1);
			}
			Stop_Num=0;
		}
		else
		{
			PLANT[segment].Start_Req=0;
			Start_Num=0;
		}
		
		if(Unit_Stop==1)
		{
			PLANT[segment].Stop_Req=1;
			PLANT[segment].Start_Req=0;
			PLANT[segment].Unit_Stop=Stop_Num;
			PLANT[segment].XUnitStrStp =1;
			UNIT[Stop_Num].Stop_Req=1;
			if(Stop_Num!=0)
			{
				AGC_UpdateRTDB_IND(DB_NAME_Stop_Req[Stop_Num],1);
			}
			Start_Num=0;
		} 
		else
		{
			PLANT[segment].Stop_Req=0;
			Stop_Num=0;
		}   
		if ((Start_Req != PLANT[segment].Start_Req) ||
			(Stop_Req != PLANT[segment].Stop_Req))
		{   
			for (i=1; i<=UNIT_NUMB; i++) 
			{
				if(UNIT[i].segment!=segment)continue;
				UNIT[i].XAnswerNo = 0;
				if ((i != Start_Num) &&(UNIT[i].StartAction==0))
				{
					UNIT[i].Start_Req = 0;
					AGC_UpdateRTDB_IND(DB_NAME_Start_Req[i],0);
				}
				if ((i != Stop_Num) &&(UNIT[i].StopAction==0))
				{
					UNIT[i].Stop_Req = 0;
					AGC_UpdateRTDB_IND(DB_NAME_Stop_Req[i],0);
				}
			}
		}
}

void Start_Stop_Module(int segment)
{
    int StartNum=0,StopNum=0;
    
    StartNum = PLANT[segment].Unit_Start;
    StopNum = PLANT[segment].Unit_Stop;
	
    if(PLANT[segment].Closed_Loop!=CLOSED_LOOP)
    {  
        if(PLANT[segment].XUnitStrStp==1)
            AGC_UpdateRTDB_IND(DB_NAME_PlantStrStp_Req[segment],1);
        else
            AGC_UpdateRTDB_IND(DB_NAME_PlantStrStp_Req[segment],0);
        /*Answer NO from operator received*/
        if (PLANT[segment].SSAnswerNo == 1)
        {
            /*Reset start request*/
            if ((PLANT[segment].Start_Req == 1) && (StartNum>0)) 
            {
				UNIT[StartNum].Start_Req=0;
				UNIT[StartNum].StartFlag=0;
                UNIT[StartNum].XAnswerNo = 1;
                PLANT[segment].XStrStp_Delay = StartStopDelay;
                PLANT[segment].SSReject_Num+=1;
                PLANT[segment].SSAnswerNoTime=Time_5s;
                printf("%d号机开机请求清除\n",StartNum);
            }
            /*Reset stop request*/
            if ((PLANT[segment].Stop_Req == 1) && (StopNum>0)) 
            {
				UNIT[StopNum].Stop_Req=0;
				UNIT[StopNum].StopFlag=0;
                UNIT[StopNum].XAnswerNo = 1;
                PLANT[segment].XStrStp_Delay = StartStopDelay;
                PLANT[segment].SSReject_Num+=1;
                PLANT[segment].SSAnswerNoTime=Time_5s;
                printf("%d号机停机请求清除\n",StopNum);
            }
        }
        if(PLANT[segment].SSReject_Num>=3)
        {
            PLANT[segment].Start_Req=0;
            PLANT[segment].Stop_Req =0;
            PLANT[segment].Unit_Start=0;
            PLANT[segment].Unit_Stop=0;
            UNIT[StartNum].XAnswerNo = 0;
            PLANT[segment].XUnitStrStp =0;
            PLANT[segment].XStrStp_Delay = 0;
            PLANT[segment].SSReject_Num=0;
            PLANT[segment].SSAnswerNoTime=Time_5s;
            printf("操作员拒绝次数到达三次，所有开停机请求清除!\n");
        }
        /*Answer YES from operator received*/
        if (PLANT[segment].SSAnswerYes ==1) 
        {
            /*Start command*/
            if ((PLANT[segment].Start_Req == 1) && (StartNum>0)) 
            {
                UNIT[StartNum].Start = 1;
				UNIT[StartNum].StartFlag=1;
                UNIT[StartNum].StartAction = 1;
                UNIT[StartNum].StartCmdTime = 0;
                UNIT[StartNum].StartTime=0;
                UNIT[StartNum].StopTime=0;
                UNIT[StartNum].XPowerUp =0;
                UNIT[StartNum].Stop = 0;
                UNIT[StartNum].StopCmdTime = StopCmdTime;
                UNIT[StartNum].StopAction = 0;
                PLANT[segment].XUnitStrStp =0;
				PLANT[segment].XStrStp_Delay = StartStopDelay;
				UNIT[StartNum].CommandTime = 0;
                PLANT[segment].SSAnswerYesTime=Time_5s;
                printf("%d号机开机\n",StartNum);  
				if(AutoStartStop[StartNum]==1)
					AGC_UpdateRTDB_IND(DB_NAME_Start_Cmd[StartNum],1);
            }
            /*Stop command*/
            if ((PLANT[segment].Stop_Req == 1) && (StopNum>0)) 
            {
                UNIT[StopNum].XStop = 1;
				UNIT[StopNum].StopFlag=1;
                UNIT[StopNum].StopAction = 1;
                UNIT[StopNum].ReduceLoad =1;
                UNIT[StopNum].StopCmdTime = StopCmdTime;
                UNIT[StopNum].XPowerUp =0;
                UNIT[StopNum].StartTime=0;
                UNIT[StopNum].StopTime=0;
                UNIT[StopNum].Start = 0;
                UNIT[StopNum].StartAction = 0;
                UNIT[StopNum].StartCmdTime = 0;
                UNIT[StopNum].XAPCompMode=3;
                UNIT[StopNum].APCompTime = StopCompTime;
				PLANT[segment].XStrStp_Delay = StartStopDelay;
                PLANT[segment].XUnitStrStp =0;
                PLANT[segment].SSAnswerYesTime=Time_5s;
                printf("%d号机停机\n",StopNum);
                AGC_PowerAllocation(segment);
            }
        }
    } 
    else 
    {
        /*Start command*/
        if ((PLANT[segment].Start_Req == 1) && (StartNum>0)) 
        { 
            UNIT[StartNum].Start = 1;
            UNIT[StartNum].StartFlag=1;
            UNIT[StartNum].StartAction = 1;
            UNIT[StartNum].StartCmdTime = 0;
            UNIT[StartNum].StartTime=0;
            UNIT[StartNum].StopTime=0;
            UNIT[StartNum].XPowerUp =0;
            UNIT[StartNum].Stop = 0;
            UNIT[StartNum].StopCmdTime = StopCmdTime;
            UNIT[StartNum].StopAction = 0;
            UNIT[StopNum].APCompTime = StopCompTime;
            PLANT[segment].XUnitStrStp =0;
            UNIT[StartNum].CommandTime = 0;
            PLANT[segment].XStrStp_Delay = StartStopDelay;
			if(AutoStartStop[StartNum]==1)
				AGC_UpdateRTDB_IND(DB_NAME_Start_Cmd[StartNum],1);
        }
        /*Stop command*/
        if ((PLANT[segment].Stop_Req == 1) && (StopNum>0)) 
        {
            UNIT[StopNum].XStop = 1;
            UNIT[StopNum].StopFlag=1;
            UNIT[StopNum].StopAction = 1;
            UNIT[StopNum].ReduceLoad =1;
            UNIT[StopNum].StopCmdTime = StopCmdTime;
            UNIT[StopNum].XPowerUp =0;
            UNIT[StopNum].StartTime=0;
            UNIT[StopNum].StopTime=0;
            UNIT[StopNum].Start = 0;
            UNIT[StopNum].StartAction = 0;
            UNIT[StopNum].StartCmdTime = 0;
            UNIT[StopNum].XAPCompMode=3;
            UNIT[StopNum].APCompTime = StopCompTime;
            PLANT[segment].XStrStp_Recalc=0;
            PLANT[segment].XUnitStrStp =0;
            PLANT[segment].XStrStp_Delay = StartStopDelay;
            printf("%d号机停机\n",StopNum);
            AGC_PowerAllocation(segment);
        }
        AGC_UpdateRTDB_IND(DB_NAME_PlantStrStp_Req[segment],0);
    }     
}

float Frequency_Control(int segment)
{
    float Freq_Power,CalcDroop,Power_Add;
    float Freq_Dev,AbsFreq_Dev,MinPow,MaxPow;
    char FreqDevDir;
	
    Power_Add=0;
    Freq_Dev = 0;
    AbsFreq_Dev = 0;
	
    if((PLANT[segment].FR_Compensation==1)||(PLANT[segment].FR_Regulation==1))
    {
        if (PLANT[segment].FR_WorkSP < PLANT[segment].FR_Setpoint) 
            PLANT[segment].FR_WorkSP +=(float)0.0001;
        if (PLANT[segment].FR_WorkSP > PLANT[segment].FR_Setpoint) 
            PLANT[segment].FR_WorkSP -=(float)0.0001;
        
//  		 printf("FR_WorkSP=%f,FR_Setpoint=%f,FR_ActValue=%f\n",PLANT[segment].FR_WorkSP,
//           PLANT[segment].FR_Setpoint,PLANT[segment].FR_ActValue);
        Freq_Dev = PLANT[segment].FR_WorkSP - PLANT[segment].FR_ActValue;
        AbsFreq_Dev =(float)fabs(Freq_Dev);
        PLANT[segment].Dev_Frequency=AbsFreq_Dev;
        if (PLANT[segment].FR_Compensation ==1)
        {
            MinPow = PLANT[segment].AP_Setpoint - PLANT[segment].FR_PowGenDB;
            MaxPow = PLANT[segment].AP_Setpoint + PLANT[segment].FR_PowGenDB;
            if (MinPow < PLANT[segment].AP_Min) MinPow = PLANT[segment].AP_Min;
            if (MaxPow > PLANT[segment].AP_Max) MaxPow = PLANT[segment].AP_Max;
        } 
        else
        {
            MinPow = PLANT[segment].AP_Min;
            MaxPow = PLANT[segment].AP_Max;
        }
        //printf("MinPow=%f,MaxPow=%f\n",MinPow,MaxPow);
        if (AbsFreq_Dev > PLANT[segment].FR_Deadband)
        {
            if (AbsFreq_Dev >  PLANT[segment].FR_CompDeadband)
                PLANT[segment].FR_CTRL_Alarm = 1;
            else
                PLANT[segment].FR_CTRL_Alarm = 0;
			
            if (PLANT[segment].p_sum > PLANT[segment].XAP_FreqCTRL)
                PLANT[segment].XAP_FreqCTRL += 1;
			
            if (PLANT[segment].p_sum < PLANT[segment].XAP_FreqCTRL)
                PLANT[segment].XAP_FreqCTRL -= 1;
			
            CalcDroop = PLANT[segment].XFreqDoopAdaptFact *
				AbsFreq_Dev/ PLANT[segment].FR_WorkSP * Freq_A_Gain;
            if (CalcDroop > 0.5) CalcDroop = 0.5;
            
            FreqDevDir = PLANT[segment].XFreqDevDir;
            if (Freq_Dev>0) FreqDevDir = 1;
            if (Freq_Dev<0) FreqDevDir = -1;
			
            if (PLANT[segment].XFreqDevDir != 0) 
            {
                if (PLANT[segment].XFreqDevDir != FreqDevDir) 
                {
                    if (PLANT[segment].XFreqDoopAdaptFact > 0.2) 
                    {
                        PLANT[segment].XFreqDoopAdaptFact*= 0.75;
                        PLANT[segment].XFreqMaxDev = 0;
                    }
                } 
                else 
                {
                    if (PLANT[segment].XFreqDoopAdaptFact < 1.5) 
                    {
                        if (AbsFreq_Dev >= PLANT[segment].XFreqMaxDev) 
                        {
                            PLANT[segment].XFreqMaxDev = AbsFreq_Dev;
                            if (PLANT[segment].FR_CTRL_Alarm==1) 
                                PLANT[segment].XFreqDoopAdaptFact+=(float)0.01;
                            else 
                                PLANT[segment].XFreqDoopAdaptFact+=(float)0.0025;
                        }
                    }
                }
            }
            PLANT[segment].XFreqDevDir = FreqDevDir;
            Power_Add = CalcDroop * PLANT[segment].p_sum;
            if (Power_Add > MaxPower) Power_Add = MaxPower;
//  			 printf("XAP_FreqCTRL=%f,CalcDroop=%f,XFreqDoopAdaptFact=%f\n",PLANT[segment].XAP_FreqCTRL,
//  			  CalcDroop,PLANT[segment].XFreqDoopAdaptFact);
            
            if (Freq_Dev>0) 
                PLANT[segment].XFreq_Output = PLANT[segment].XAP_FreqCTRL + Power_Add;
            else
                PLANT[segment].XFreq_Output = PLANT[segment].XAP_FreqCTRL - Power_Add;
            if((PLANT[segment].FR_Compensation==1)&&(PLANT[segment].FR_CTRL_Alarm!=1))
                PLANT[segment].XFreq_Output = PLANT[segment].AP_WorkSP;
        } /*if (AbsFreq_Dev > PLANT[segment].FR_Deadband)结束*/
        else 
        {
            PLANT[segment].FR_CTRL_Alarm = 0;
            PLANT[segment].XFreq_Output = PLANT[segment].AP_WorkSP;
        }
		//Limitation of frequency power output
        if(PLANT[segment].XFreq_Output > MaxPow)
            PLANT[segment].XFreq_Output = MaxPow;
		
        if(PLANT[segment].XFreq_Output < MinPow)
            PLANT[segment].XFreq_Output = MinPow;
    } 
    else /*if((PLANT[segment].FR_Compensation==1)||(PLANT[segment].FR_Regulation==1))结束*/
    {
        PLANT[segment].XAP_FreqCTRL = PLANT[segment].p_sum;
        PLANT[segment].XFreq_Output = PLANT[segment].AP_WorkSP;
        PLANT[segment].FR_CTRL_Alarm = 0;
        PLANT[segment].XFreqDevDir = 0;
        PLANT[segment].XFreqDoopAdaptFact = 1.0;
        PLANT[segment].XFreqMaxDev = 0.0;
    }
    Freq_Power = PLANT[segment].XFreq_Output; 
    if(PLANT[segment].FR_Compensation==1)
    {
        if(Freq_Power<PLANT[segment].AP_Setpoint)
            Freq_Power=PLANT[segment].AP_Setpoint;
    }
    // printf("Freq_Power=%f\n",Freq_Power);
    return Freq_Power;
}

void AGC_Module(int segment)
{
    float AP_DeadB,DB_AP_WorkSP,AP_WorkSP,Real_SRValue,Gen_Max;
    int Unit_Comp,i,Unit_Moving,MoveTime,Unit_Start;
	char ErrSms[100];
    Unit_Comp=0;
    Unit_Moving=0;
    Unit_Start=0;
    Gen_Max=0;
    AP_DeadB = PLANT[segment].AP_Deadband; 
	
    if ((PLANT[segment].FR_Compensation == 1)||(PLANT[segment].FR_Regulation == 1) )
    {
        DB_AP_WorkSP = AP_DeadB;
        AP_WorkSP = PLANT[segment].Freq_Power;
    } 
    else 
    {
        AP_WorkSP = PLANT[segment].AP_Setpoint;
        DB_AP_WorkSP = APSetpDB;
    } 

 
	if(fabs(PLANT[segment].AP_WorkSP - AP_WorkSP)>WorkSP_Max_Change)
	{
		PLANT[segment].Mode=5;
		if(PLANT[segment].AP_MODE==1)
		{
			PLANT[segment].AP_MODE=0;
			AGC_UpdateRTDB_IND(DB_NAME_QC_AGC[segment],0);
			sprintf(ErrSms,"%s:工作设定值变化过大!",PLANT[segment].English_Name);
			if(GetControlState()==1)
				BroadCastSms(ErrSms,TRUE);
		}
	}
	
	if(fabs(PLANT[segment].AP_WorkSP - AP_WorkSP) > DB_AP_WorkSP)
	{
		printf("设定值变化且在合理范围内!AP_WorkSP=%f,PLANT[%d].AP_WorkSP=%f\n",AP_WorkSP,
			segment,PLANT[segment].AP_WorkSP);
		PLANT[segment].Last_AP_WorkSP= PLANT[segment].AP_WorkSP;
		PLANT[segment].AP_WorkSP = AP_WorkSP;
		
#ifdef CASCADE			
		if (!PLANT[segment].AllocationToUnit)
		{
			printf("命令下发达:梯调下发%s总有功给定为%f\n",PLANT[segment].Chinese_Name,PLANT[segment].AP_WorkSP);
			SendCommandToPLC(DB_NAME_CasToPlantValue[segment],C_OP_VALUE_SET,PLANT[segment].AP_WorkSP,0);
		}
#endif
		PLANT[segment].SSReject_Num=0;
		AGC_Recalc[segment] = 1;
	} 
    
    for(i=1;i<=UNIT_NUMB;i++)
    {
		if(UNIT[i].segment!=segment)continue;
		if(UNIT[i].AP_Avail!=1)continue;
        if(UNIT[i].XAP_Err_Detect.Moving==1)
            Unit_Moving=1;
        if(UNIT[i].StartAction==1)
            Unit_Start=1;
        if(UNIT[i].Status==UNIT_IN_GEN)
            Gen_Max+=UNIT[i].AP_Max;
    }
    
    Real_SRValue=Gen_Max-PLANT[segment].p_sum_agc;
    PLANT[segment].Real_SRValue=Real_SRValue;
    AGC_UpdateRTDB_ANA(DB_NAME_Real_SRValue[segment],Real_SRValue);
    if((PLANT[segment].SR_AVAIL_Setting==1)||(PLANT[segment].SR_AVAIL_Curve==1))
		if(PLANT[segment].Real_SRValue<PLANT[segment].SR_SetPoint)
		{
			AGC_Recalc[segment]=1;
			printf("实际旋转备用小于旋转备用设定值!\n");
		}
		
		MoveTime=10;
		if((Unit_Moving==1)||(PLANT[segment].XUnitStrStp==1))
			MoveTime=3;
		if(Unit_Start==1)
			MoveTime=1;
		PLANT[segment].AP_Deviation=PLANT[segment].AP_WorkSP-PLANT[segment].p_sum;
		if((fabs(PLANT[segment].AP_Deviation)>AP_DeadB)&&(PLANT[segment].Num_AP_Avail>0))
		{
			if(PLANT[segment].AP_Alm_Time<Time_10m)
			{
				PLANT[segment].AP_Alm_Time+=MoveTime;
			}
			else
			{
				AGC_UpdateRTDB_IND(DB_NAME_AP_Dev_Alm[segment],1);
			}
		}
		else
		{
			AGC_UpdateRTDB_IND(DB_NAME_AP_Dev_Alm[segment],0);
			PLANT[segment].AP_Alm_Time=0;
		}
		
		AGC_UpdateRTDB_ANA(DB_NAME_Real_APSetpoint[segment],PLANT[segment].AP_WorkSP);
		
		if((PLANT[segment].FR_Compensation ==0)&&(PLANT[segment].FR_Regulation ==0)&&(fabs(PLANT[segment].AP_Setpoint-PLANT[segment].AP_WorkSP)>20)&&(PLANT[segment].Closed_Loop!= OPEN_LOOP))
			AGC_UpdateRTDB_IND(DB_NAME_FailToSetp[segment],1);
		else
			AGC_UpdateRTDB_IND(DB_NAME_FailToSetp[segment],0);
		
		if(PLANT[segment].Num_AP_Avail!=PLANT[segment].Last_Num_AP_Avail)
		{	
			printf("联控机组台数发生变化!Last_Num_AP_Avail=%d,Num_AP_Avail=%d\n",PLANT[segment].Last_Num_AP_Avail,PLANT[segment].Num_AP_Avail);
			PLANT[segment].Last_Num_AP_Avail=PLANT[segment].Num_AP_Avail;
			AGC_Recalc[segment] =1;
		}
		if((fabs(PLANT[segment].Last_p_sum_no_agc-PLANT[segment].p_sum_no_agc)>AP_DeadB)&&(fabs(PLANT[segment].Last_p_sum_no_agc-PLANT[segment].p_sum_no_agc)<400))
		{
			printf("非联控机组实发有功值变化!Last_p_sum_no_agc=%f,p_sum_no_agc=%f\n",PLANT[segment].Last_p_sum_no_agc,PLANT[segment].p_sum_no_agc);
			PLANT[segment].Last_p_sum_no_agc=PLANT[segment].p_sum_no_agc;
			printf("AP_DeadB=%f\n",AP_DeadB);
			AGC_Recalc[segment]=1;
		}
		if(PLANT[segment].Last_AGC_Generating!=PLANT[segment].AGC_Generating)
		{
			printf("联控机组发电台数发生变化!Last_AGC_Generating=%d,AGC_Generating=%d\n",PLANT[segment].Last_AGC_Generating,PLANT[segment].AGC_Generating);
			PLANT[segment].Last_AGC_Generating=PLANT[segment].AGC_Generating;
			AGC_Recalc[segment]=1;
		}
		if(PLANT[segment].Lim_Change==1)
		{
			printf("联控机组限值发生变化!\n");
			AGC_Recalc[segment]=1;
			PLANT[segment].Lim_Change=0;
		}
		if((PLANT[segment].Mode!=5)&&(PLANT[segment].Last_Mode==5))
		{
			printf("AGC电厂投入功能%d!\n",PLANT[segment].Mode);
			AGC_Recalc[segment]=1;
		}
		PLANT[segment].Last_Mode=PLANT[segment].Mode;
#ifdef CASCADE
		if (!PLANT[segment].AllocationToUnit)
		{
			AGC_Recalc[segment] = 0;
			return;
		}
#endif		
		for(i=1;i<=UNIT_NUMB;i++)
		{
			if((UNIT[i].XAPCompMode>0)&&(UNIT[i].segment==segment))Unit_Comp=1;
		}
		PLANT[segment].XAGC_Unit_Comp=Unit_Comp;
		if(AGC_Recalc[segment] == 1)
		{
			PLANT[segment].Last_p_sum_no_agc=PLANT[segment].p_sum_no_agc;
			PLANT[segment].XStrStp_Recalc=1;
			AGC_PowerAllocation(segment);
		}
		if((PLANT[segment].XStrStp_Recalc==1)||(PLANT[segment].XUnitStrStp==1))
		{   //printf("XStrStp_Recalc=%d,XUnitStrStp=%d,XStrStp_Delay=%d\n",
			// PLANT[segment].XStrStp_Recalc,PLANT[segment].XUnitStrStp,PLANT[segment].XStrStp_Delay);
			PLANT[segment].XStrStp_Recalc=0;  
			Start_Stop_Calculation(segment);  
			if(PLANT[segment].XStrStp_Delay>0)
				PLANT[segment].XStrStp_Delay--;
			else
				Start_Stop_Module(segment);
		}
		else
			PLANT[segment].XStrStp_Delay=0;
}

void AGC_segment()
{  
    int i;

	for(i=1;i<=4;i++)
          UNIT[i].segment=1;
	for(i=5;i<=8;i++)
          UNIT[i].segment=2;
    
}

//Calculate min. and max. power of unit to avoid crossing of vibration zone
void SetUnitVibMinMax(int UnitPos)
{
    int PowerZone,ZoneCount,UnitType;
    int i1;
    float min_var,max_var,LL,HL,DX,UnitPower,ZoneSize;
    float ZoneLowSize[VIBRATION_NUMB+1];
    float ZoneHighSize[VIBRATION_NUMB+1];
    float ZoneLow[VIBRATION_NUMB+1];
    float ZoneHigh[VIBRATION_NUMB+1];
	
    UnitType = UNIT[UnitPos].Type;
    UnitPower = UNIT[UnitPos].XAP_Setp;  
	
    min_var = 0;
    max_var = AGC_Unit_Data[UnitType].MaxPower;
    ZoneCount = 1;
    ZoneLow[ZoneCount] = 0;
    ZoneLowSize[ZoneCount] = 0;
    ZoneHighSize[ZoneCount] = 0;
    ZoneHigh[ZoneCount] = AGC_Unit_Data[UnitType].MaxPower;
    PowerZone = 0;
	
    if ((UnitType>0) && (UnitType<=MAXTYPES)) 
    {
		for (i1=1; i1<=VIBRATION_NUMB; i1++) 
		{
            if (AGC_Unit_Vibr[UnitType].Vibration[i1].Used==1) 
            {
				DX = AGC_Unit_Vibr[UnitType].Vibration[i1].AP_End -
					AGC_Unit_Vibr[UnitType].Vibration[i1].AP_Begin;
				if (DX > APSetpDB) 
				{
                    ZoneSize = (AGC_Unit_Vibr[UnitType].Vibration[i1].AP_End -
						AGC_Unit_Vibr[UnitType].Vibration[i1].AP_Begin) / 2;
					
					ZoneHigh[ZoneCount] = AGC_Unit_Vibr[UnitType].Vibration[i1].AP_Begin;
					ZoneHighSize[ZoneCount] = ZoneSize;
					
                    ZoneCount++;
                    ZoneLow[ZoneCount] = AGC_Unit_Vibr[UnitType].Vibration[i1].AP_End;
                    ZoneLowSize[ZoneCount] = ZoneSize;
					
                    ZoneHigh[ZoneCount] = AGC_Unit_Data[UnitType].MaxPower;
                    ZoneHighSize[ZoneCount] = 0;
				}
            }
		}
		
		//Find zone where unit is running in
		i1 = 0;
		LL = 0;
		HL = 0;
		while ((i1<ZoneCount) && (PowerZone==0))
		{
            i1++;
            LL = ZoneLow[i1] - ZoneLowSize[i1];
            HL = ZoneHigh[i1] + ZoneHighSize[i1];
            if ((UnitPower>=LL) && (UnitPower<=HL)) 
            {
				PowerZone = i1;
            }
		}
	}
	
    if (PowerZone==0) 
    {
		min_var = UNIT[UnitPos].AP_Min;
		max_var = UNIT[UnitPos].AP_Max;
    } 
    else 
    {
		min_var = ZoneLow[PowerZone];
		max_var = ZoneHigh[PowerZone];
    }
    if (min_var<UNIT[UnitPos].AP_Min) min_var = UNIT[UnitPos].AP_Min;
    if (max_var>UNIT[UnitPos].AP_Max) max_var = UNIT[UnitPos].AP_Max;
	
    UNIT[UnitPos].CurrentLowlmt = min_var;
    UNIT[UnitPos].CurrentUplmt= max_var; 
}

float GetUnitCurrentMin(int UnitPos,float UnitPower)
{
    int PowerZone,ZoneCount,UnitType;
    int i1;
    float min_var,max_var,LL,HL,DX,ZoneSize;
    float ZoneLowSize[VIBRATION_NUMB+1];
    float ZoneHighSize[VIBRATION_NUMB+1];
    float ZoneLow[VIBRATION_NUMB+1];
    float ZoneHigh[VIBRATION_NUMB+1];
	
    UnitType = UNIT[UnitPos].Type;
    min_var = 0;
    max_var = AGC_Unit_Data[UnitType].MaxPower;
    ZoneCount = 1;
    ZoneLow[ZoneCount] = 0;
    ZoneLowSize[ZoneCount] = 0;
    ZoneHighSize[ZoneCount] = 0;
    ZoneHigh[ZoneCount] = AGC_Unit_Data[UnitType].MaxPower;
    PowerZone = 0;
	
    if ((UnitType>0) && (UnitType<=MAXTYPES)) 
    {
		for (i1=1; i1<=VIBRATION_NUMB; i1++) 
		{
            if (AGC_Unit_Vibr[UnitType].Vibration[i1].Used==1) 
            {
				DX = AGC_Unit_Vibr[UnitType].Vibration[i1].AP_End -
					AGC_Unit_Vibr[UnitType].Vibration[i1].AP_Begin;
				if (DX > APSetpDB) 
				{
                    ZoneSize = (AGC_Unit_Vibr[UnitType].Vibration[i1].AP_End -
						AGC_Unit_Vibr[UnitType].Vibration[i1].AP_Begin) / 2;
					
					ZoneHigh[ZoneCount] = AGC_Unit_Vibr[UnitType].Vibration[i1].AP_Begin;
					ZoneHighSize[ZoneCount] = ZoneSize;
					
                    ZoneCount++;
                    ZoneLow[ZoneCount] = AGC_Unit_Vibr[UnitType].Vibration[i1].AP_End;
                    ZoneLowSize[ZoneCount] = ZoneSize;
					
                    ZoneHigh[ZoneCount] = AGC_Unit_Data[UnitType].MaxPower;
                    ZoneHighSize[ZoneCount] = 0;
				}
            }
		}
		
		//Find zone where unit is running in
		i1 = 0;
		LL = 0;
		HL = 0;
		while ((i1<ZoneCount) && (PowerZone==0))
		{
            i1++;
            LL = ZoneLow[i1] - ZoneLowSize[i1];
            HL = ZoneHigh[i1] + ZoneHighSize[i1];
            if ((UnitPower>=LL) && (UnitPower<=HL)) 
            {
				PowerZone = i1;
            }
		}
	} 
	
    if (PowerZone==0) 
    {
		min_var = UNIT[UnitPos].AP_Min;
		max_var = UNIT[UnitPos].AP_Max;
    } 
    else 
    {
		min_var = ZoneLow[PowerZone];
		max_var = ZoneHigh[PowerZone];
    }   
    if (min_var<UNIT[UnitPos].AP_Min) min_var = UNIT[UnitPos].AP_Min;
    if (max_var>UNIT[UnitPos].AP_Max) max_var = UNIT[UnitPos].AP_Max;
	
    return min_var;
}
float GetUnitCurrentMax(int UnitPos,float UnitPower)
{
    int PowerZone,ZoneCount,UnitType;
    int i1;
    float min_var,max_var,LL,HL,DX,ZoneSize;
    float ZoneLowSize[VIBRATION_NUMB+1];
    float ZoneHighSize[VIBRATION_NUMB+1];
    float ZoneLow[VIBRATION_NUMB+1];
    float ZoneHigh[VIBRATION_NUMB+1];
	
    UnitType = UNIT[UnitPos].Type;
    min_var = 0;
    max_var = AGC_Unit_Data[UnitType].MaxPower;
    ZoneCount = 1;
    ZoneLow[ZoneCount] = 0;
    ZoneLowSize[ZoneCount] = 0;
    ZoneHighSize[ZoneCount] = 0;
    ZoneHigh[ZoneCount] = AGC_Unit_Data[UnitType].MaxPower;
    PowerZone = 0;
	
    if ((UnitType>0) && (UnitType<=MAXTYPES)) 
    {
		for (i1=1; i1<=VIBRATION_NUMB; i1++) 
		{
            if (AGC_Unit_Vibr[UnitType].Vibration[i1].Used==1) 
            {
				DX = AGC_Unit_Vibr[UnitType].Vibration[i1].AP_End -
					AGC_Unit_Vibr[UnitType].Vibration[i1].AP_Begin;
				if (DX > APSetpDB) 
				{
                    ZoneSize = (AGC_Unit_Vibr[UnitType].Vibration[i1].AP_End -
						AGC_Unit_Vibr[UnitType].Vibration[i1].AP_Begin) / 2;
					
					ZoneHigh[ZoneCount] = AGC_Unit_Vibr[UnitType].Vibration[i1].AP_Begin;
					ZoneHighSize[ZoneCount] = ZoneSize;
					
                    ZoneCount++;
                    ZoneLow[ZoneCount] = AGC_Unit_Vibr[UnitType].Vibration[i1].AP_End;
                    ZoneLowSize[ZoneCount] = ZoneSize;
					
                    ZoneHigh[ZoneCount] = AGC_Unit_Data[UnitType].MaxPower;
                    ZoneHighSize[ZoneCount] = 0;
				}
            }
		}
		
		//Find zone where unit is running in
		i1 = 0;
		LL = 0;
		HL = 0;
		while ((i1<ZoneCount) && (PowerZone==0))
		{
            i1++;
            LL = ZoneLow[i1] - ZoneLowSize[i1];
            HL = ZoneHigh[i1] + ZoneHighSize[i1];
            if ((UnitPower>=LL) && (UnitPower<=HL)) 
            {
				PowerZone = i1;
            }
		}
	} 
	
    if (PowerZone==0) 
    {
		min_var = UNIT[UnitPos].AP_Min;
		max_var = UNIT[UnitPos].AP_Max;
    } 
    else 
    {
		min_var = ZoneLow[PowerZone];
		max_var = ZoneHigh[PowerZone];
    }   
    if (min_var<UNIT[UnitPos].AP_Min) min_var = UNIT[UnitPos].AP_Min;
    if (max_var>UNIT[UnitPos].AP_Max) max_var = UNIT[UnitPos].AP_Max;
	
    return max_var;
}

void WriteVibrationInfo(void)
{
    int i,i1,i2;
    i1=i2=0;
    for(i=1;i<=VIBRATION_NUMB;i++)
    {
        if(AGC_Unit_Vibr[1].Vibration[i].Used==1)
        {
            i1+=1;
            AGC_UpdateRTDB_ANA(DB_NAME_VIBRATION1_MIN[i1],AGC_Unit_Vibr[1].Vibration[i].AP_Begin);
            AGC_UpdateRTDB_ANA(DB_NAME_VIBRATION1_MAX[i1],AGC_Unit_Vibr[1].Vibration[i].AP_End);
        }
        if(AGC_Unit_Vibr[2].Vibration[i].Used==1)
        {
            i2+=1;
            AGC_UpdateRTDB_ANA(DB_NAME_VIBRATION2_MIN[i2],AGC_Unit_Vibr[2].Vibration[i].AP_Begin);
			AGC_UpdateRTDB_ANA(DB_NAME_VIBRATION2_MAX[i2],AGC_Unit_Vibr[2].Vibration[i].AP_End);
        }
     } 
    
    for(i=1;i<=VIBRATION_NUMB;i++)
    {
        if(i>i1)
        {
            AGC_UpdateRTDB_ANA(DB_NAME_VIBRATION1_MIN[i],0);
            AGC_UpdateRTDB_ANA(DB_NAME_VIBRATION1_MAX[i],0);
        }
        if(i>i2)
        {
            AGC_UpdateRTDB_ANA(DB_NAME_VIBRATION2_MIN[i],0);
            AGC_UpdateRTDB_ANA(DB_NAME_VIBRATION2_MAX[i],0);
        }
    }
    
    for(i=1;i<=MAXTYPES;i++)
    {
        AGC_UpdateRTDB_ANA(DB_NAME_MIN[i],AGC_Unit_Data[i].MinPower);  
        AGC_UpdateRTDB_ANA(DB_NAME_MAX[i],AGC_Unit_Data[i].MaxPower);
    } 
}
void Check_Waterhead(void)
{
    int i,segment;
    float   WaterHead,Sum;
    char ErrSms[100];
    Sum=0;
		
    WaterHead=ReadDB_ANA(DB_NAME_WATERHEAD);
	if((WaterHeadFlag==0)&&(WaterHeadNumber==0))
		SaveWaterHead=WaterHead;
	else if(fabs(SaveWaterHead-WaterHead)<0.5)
	{
		SaveWaterHead=WaterHead;
		AlarmFlag=0;
	}
	else if(fabs(SaveWaterHead-WaterHead)>=0.5)
	{
		if(AlarmFlag==0)
		{
			strcpy(ErrSms,"水头变化过大!");
			if(GetControlState()==1)
				BroadCastSms(ErrSms,TRUE);
			AlarmFlag=1;
		}
		return;
	}
	WaterHeadTempValue[WaterHeadNumber]=WaterHead;
	WaterHeadNumber+=1;
	if(WaterHeadNumber>=10)
	{
		WaterHeadFlag=1;
		WaterHeadNumber=0;
	}
	if(WaterHeadFlag==0)
	{
		for(i=0;i<WaterHeadNumber;i++)
			Sum+=WaterHeadTempValue[i];
		WaterMsg.WaterHead=Sum/WaterHeadNumber;
	}
	else
	{
		for(i=0;i<10;i++)
			Sum+=WaterHeadTempValue[i];
		WaterMsg.WaterHead=(float)(Sum/10.0);  
	}   
    if (WaterMsg.WaterHead<MINHEAD) WaterMsg.WaterHead = MINHEAD;
    if (WaterMsg.WaterHead>MAXHEAD) WaterMsg.WaterHead = MAXHEAD;
    
	if(PLANT[0].FirstRun==1)
	{
		AGC_UpdateRTDB_ANA(DB_NAME_AGC_Whead,WaterMsg.WaterHead);
		SaveFiveMinuteWh=WaterMsg.WaterHead;		
	}
	if(ntime->tm_min%5==0)
	{
		if((SaveFiveMinuteWh!=0)&&(fabs(SaveFiveMinuteWh-WaterMsg.WaterHead)>WH_Value))
		{
			for(segment=0;segment<MAXSEGMENT;segment++)
			{
				if(PLANT[segment].AP_MODE==1)
				{
					PLANT[segment].AP_MODE=0;
					AGC_UpdateRTDB_IND(DB_NAME_QC_AGC[segment],0); 
					strcpy(ErrSms,"水头五分钟变化大退AGC!");
					if(GetControlState()==1)
						BroadCastSms(ErrSms,TRUE);
				}
			}
		}
		AGC_UpdateRTDB_ANA(DB_NAME_AGC_Whead,WaterMsg.WaterHead);
		SaveFiveMinuteWh=WaterMsg.WaterHead;
	}
	
    if (fabs(SaveFiveMinuteWh - WaterMsg.XWaterHead) < 0.3)
        return;  
    WaterMsg.XWaterHead = SaveFiveMinuteWh;
    
    //Calculate actual operation data of units
    for(i=1; i<=MAXTYPES; i++) 
		Calculate_AGC_Curve(i,WaterMsg.XWaterHead);      
	
    WriteVibrationInfo();
    
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
	/* KEYWORD=="AheadOfSchedule" */
	strcpy(keyword, "AheadOfSchedule");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	AheadOfSchedule=atoi(StrTemp1);
	
	/*KEYOWRD=="AutoStartStop"*/
	strcpy(keyword, "AutoStartStop");
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
			AutoStartStop[i]=atoi(StrTemp2);
		}
		else
		{
			strcpy(err_str, "[AutoStartStop] Unit No error!");
			Err_log(err_str);
			exit(0);
		}
		if((AutoStartStop[i]!=0)&&(AutoStartStop[i]!=1))
		{
			strcpy(err_str, "The value of AutoStartStop error!");
			Err_log(err_str);
			exit(0);
		}
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
		if(UNIT[i].Type>2)
		{
			strcpy(err_str, "The type of Unit error!");
			Err_log(err_str);
			exit(0);
		}
	}     
	
	/* KEYWORD=="MinPower" */
	strcpy(keyword, "MinPower");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	MinPower=(float)atof(StrTemp1);
	
	/* KEYWORD=="MaxPower" */
	strcpy(keyword, "MaxPower");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	MaxPower=(float)atof(StrTemp1);
	
	/* KEYWORD=="AP_Step" */
	strcpy(keyword, "AP_Step");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	AP_Step=(float)atof(StrTemp1);
	
	/* KEYWORD=="APCompStepVal" */
	strcpy(keyword, "APCompStepVal");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	APCompStepVal=(float)atof(StrTemp1);
	
	/* KEYWORD=="APStopMin" */
	strcpy(keyword, "APStopMin");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	APStopMin=(float)atof(StrTemp1);
	
	/* KEYWORD=="APStopLimit" */
	strcpy(keyword, "APStopLimit");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	APStopLimit=(float)atof(StrTemp1);
	
	/* KEYWORD=="Unit_AP_MaxDev" */
	strcpy(keyword, "Unit_AP_MaxDev");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	Unit_AP_MaxDev=(float)atof(StrTemp1);
	
	/* KEYWORD=="UnitAPMove" */
	strcpy(keyword, "UnitAPMove");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	UnitAPMove=(float)atof(StrTemp1);
	
	/* KEYWORD=="MINHEAD" */
	strcpy(keyword, "MINHEAD");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	MINHEAD=(float)atof(StrTemp1);
	
	/* KEYWORD=="MAXHEAD" */
	strcpy(keyword, "MAXHEAD");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	MAXHEAD=(float)atof(StrTemp1);
	
	/* KEYWORD=="AP_AlarmFact" */
	strcpy(keyword, "AP_AlarmFact");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	AP_AlarmFact=atoi(StrTemp1);
	
	/* KEYWORD=="FR_AlarmFact" */
	strcpy(keyword, "FR_AlarmFact");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	FR_AlarmFact=atoi(StrTemp1);
	
	/* KEYWORD=="Max_Power_Gap" */
	strcpy(keyword, "Max_Power_Gap");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	Max_Power_Gap=atoi(StrTemp1);
	
	/* KEYWORD=="MinFreq" */
	strcpy(keyword, "MinFreq");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	MinFreq=(float)atof(StrTemp1);
	
	/* KEYWORD=="MaxFreq" */
	strcpy(keyword, "MaxFreq");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	MaxFreq=(float)atof(StrTemp1);
	
	/* KEYWORD=="Freq_A_Gain" */
	strcpy(keyword, "Freq_A_Gain");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	Freq_A_Gain=(float)atof(StrTemp1);
	
	/* KEYWORD=="Sch_Inc_Value" */
	strcpy(keyword, "Sch_Inc_Value");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	Sch_Inc_Value=(float)atof(StrTemp1);
	
	/* KEYWORD=="Sch_Dec_Value" */
	strcpy(keyword, "Sch_Dec_Value");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	Sch_Dec_Value=(float)atof(StrTemp1);
	
	/* KEYWORD=="WH_Value" */
	strcpy(keyword, "WH_Value");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	WH_Value=(float)atof(StrTemp1);
	
	/* KEYWORD=="WorkSP_Max_Change" */
	strcpy(keyword, "WorkSP_Max_Change");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",StrTemp1);
	WorkSP_Max_Change=(float)atof(StrTemp1);
	
	for(i=0;i<MAXSEGMENT;i++)
		PLANT[i].FirstRun=1;
	
	fclose(fp);
}

void ReadDefFile(void)
{
	FILE	*fp;
#ifdef CASCADE
	char filename[40]="/home/ems/h9000/agc-xjb-cas/AGC.DEF";
#else
	char filename[40]="/home/ems/h9000/agc-xjb/AGC.DEF";
#endif
	char err_str[80]="";
	char keyword[30]="";
	char StrTemp[40]="",StrTemp1[40]="";
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
			sprintf(err_str,"No this point %s\n",DB_NAME_UNIT_STATUS[i]);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_RTVALUE[i]);
			Err_log(err_str);
			exit(0);
		}
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
		sprintf(err_str,"No this point %s\n",DB_NAME_WATERHEAD);
		Err_log(err_str);
		exit(0);
	}
	
	
	/* KEYWORD=="QC_AGC" */
	strcpy(keyword, "QC_AGC");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_QC_AGC[i]);
		if(ReadEntryByNameStr(DB_NAME_QC_AGC[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_QC_AGC[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_QC_AGC[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="OPEN_CLOSED_LOOP" */
	strcpy(keyword, "OPEN_CLOSED_LOOP");
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
			sprintf(err_str,"No this point %s\n",DB_NAME_CLOSED_LOOP[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="CONTROL_RIGHT" */
	strcpy(keyword, "CONTROL_RIGHT");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_CONTROL_RIGHT[i]);
		if(ReadEntryByNameStr(DB_NAME_CONTROL_RIGHT[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_CONTROL_RIGHT[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_CONTROL_RIGHT[i]);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_SEL_BUS[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="AP_Setting" */
	strcpy(keyword, "AP_Setting");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AP_Setting[i]);
		if(ReadEntryByNameStr(DB_NAME_AP_Setting[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_AP_Setting[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_Setting[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="AP_Curve" */
	strcpy(keyword, "AP_Curve");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AP_Curve[i]);
		if(ReadEntryByNameStr(DB_NAME_AP_Curve[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_AP_Curve[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_Curve[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="FR_Regulation" */
	strcpy(keyword, "FR_Regulation");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_FR_Regulation[i]);
		if(ReadEntryByNameStr(DB_NAME_FR_Regulation[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_FR_Regulation[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_FR_Regulation[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="FR_COMPENSATION" */
	strcpy(keyword, "FR_COMPENSATION");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_FR_COMPENSATION[i]);
		if(ReadEntryByNameStr(DB_NAME_FR_COMPENSATION[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_FR_COMPENSATION[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_FR_COMPENSATION[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="LXBYSJ" */
	strcpy(keyword, "LXBYSJ");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_LXBYSJ[i]);
		if(ReadEntryByNameStr(DB_NAME_LXBYSJ[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_LXBYSJ[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_LXBYSJ[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="LXYXSJ" */
	strcpy(keyword, "LXYXSJ");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_LXYXSJ[i]);
		if(ReadEntryByNameStr(DB_NAME_LXYXSJ[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_LXYXSJ[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_LXYXSJ[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_PRVILEGE[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="STATE_SETVALUE" */
	strcpy(keyword, "STATE_SETVALUE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_STATE_SETVALUE[i]);
		if(ReadEntryByNameStr(DB_NAME_STATE_SETVALUE[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_STATE_SETVALUE[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_STATE_SETVALUE[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="CASECADE_SETVALUE" */
	strcpy(keyword, "CASECADE_SETVALUE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_CASECADE_SETVALUE[i]);
		if(ReadEntryByNameStr(DB_NAME_CASECADE_SETVALUE[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_CASECADE_SETVALUE[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_CASECADE_SETVALUE[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="COPERATOR_SETVALUE" */
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
			sprintf(err_str,"No this point %s\n",DB_NAME_OPERATOR_SETVALUE[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="UNIT_AGCSEL" */
	strcpy(keyword, "UNIT_AGCSEL");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_UNIT_AGCSEL[i]);
		if(ReadEntryByNameStr(DB_NAME_UNIT_AGCSEL[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_UNIT_AGCSEL[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_UNIT_AGCSEL[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_UNIT_Permit[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="AP_CZKT" */
	strcpy(keyword, "AP_CZKT");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AP_CZKT[i]);
		if(ReadEntryByNameStr(DB_NAME_AP_CZKT[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_AP_CZKT[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_CZKT[i]);
			Err_log(err_str);
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
	for(i=0;i<4;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_FR_ACTVALUE[i]);
		if(ReadEntryByNameStr(DB_NAME_FR_ACTVALUE[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_FR_ACTVALUE[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_FR_ACTVALUE[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="FR_SETPOINT" */
	strcpy(keyword, "FR_SETPOINT");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_FR_SETPOINT[i]);
		if(ReadEntryByNameStr(DB_NAME_FR_SETPOINT[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_FR_SETPOINT[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_FR_SETPOINT[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="SR_SETPOINT" */
	strcpy(keyword, "SR_SETPOINT");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_SR_SETPOINT[i]);
		if(ReadEntryByNameStr(DB_NAME_SR_SETPOINT[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_SR_SETPOINT[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_SR_SETPOINT[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_MIN[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_MAX[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="AP_XMIN" */
	strcpy(keyword, "AP_XMIN");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AP_XMIN[i]);
		if(ReadEntryByNameStr(DB_NAME_AP_XMIN[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_AP_XMIN[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_XMIN[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="AP_XMAX" */
	strcpy(keyword, "AP_XMAX");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AP_XMAX[i]);
		if(ReadEntryByNameStr(DB_NAME_AP_XMAX[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_AP_XMAX[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_XMAX[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_FR_DEADBAND[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_FR_CompDeadband[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_SS_DEADBAND[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_VZ_DEADBAND[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_ChangeMax[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="VIBRATION_ZONE" */
	strcpy(keyword, "VIBRATION_ZONE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=VIBRATION_NUMB;i++)
	{
		fscanf(fp,"%s %s\n",DB_NAME_VIBRATION1_MIN[i],DB_NAME_VIBRATION1_MAX[i]);
		if(ReadEntryByNameStr(DB_NAME_VIBRATION1_MIN[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VIBRATION1_MIN[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_VIBRATION1_MIN[i]);
			Err_log(err_str);
			exit(0);
		}
		if(ReadEntryByNameStr(DB_NAME_VIBRATION1_MAX[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VIBRATION1_MAX[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_VIBRATION1_MAX[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	for(i=1;i<=VIBRATION_NUMB;i++)
	{
		fscanf(fp,"%s %s\n",DB_NAME_VIBRATION2_MIN[i],DB_NAME_VIBRATION2_MAX[i]);
		if(ReadEntryByNameStr(DB_NAME_VIBRATION2_MIN[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VIBRATION2_MIN[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_VIBRATION2_MIN[i]);
			Err_log(err_str);
			exit(0);
		}
		if(ReadEntryByNameStr(DB_NAME_VIBRATION2_MAX[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_VIBRATION2_MAX[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_VIBRATION2_MAX[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	for(i=1;i<=MAXTYPES;i++)
	{
		fscanf(fp,"%s %s\n",DB_NAME_MAX[i],DB_NAME_MIN[i]);
		if(ReadEntryByNameStr(DB_NAME_MAX[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_MAX[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_MAX[i]);
			Err_log(err_str);
			exit(0);
		}
		if(ReadEntryByNameStr(DB_NAME_MIN[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_MIN[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_MIN[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	
	/* KEYWORD=="SR_AVAIL_SETTING" */
	strcpy(keyword, "SR_AVAIL_SETTING");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_SR_AVAIL_SETTING[i]);
		if(ReadEntryByNameStr(DB_NAME_SR_AVAIL_SETTING[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_SR_AVAIL_SETTING[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_SR_AVAIL_SETTING[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="SR_AVAIL_CURVE" */
	strcpy(keyword, "SR_AVAIL_CURVE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_SR_AVAIL_CURVE[i]);
		if(ReadEntryByNameStr(DB_NAME_SR_AVAIL_CURVE[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_SR_AVAIL_CURVE[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_SR_AVAIL_CURVE[i]);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_Maint_Status[i]);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_UNIT_SETPOINT[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="AP_ANSWERYES" */
	strcpy(keyword, "AP_ANSWERYES");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AP_ANSWERYES[i]);
		if(ReadEntryByNameStr(DB_NAME_AP_ANSWERYES[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_AP_ANSWERYES[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_ANSWERYES[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="Start_Req" */
	strcpy(keyword, "Start_Req");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Start_Req[i]);
		if(ReadEntryByNameStr(DB_NAME_Start_Req[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Start_Req[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Start_Req[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="Stop_Req" */
	strcpy(keyword, "Stop_Req");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Stop_Req[i]);
		if(ReadEntryByNameStr(DB_NAME_Stop_Req[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Stop_Req[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Stop_Req[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="Start_Command" */
	strcpy(keyword, "Start_Command");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Start_Cmd[i]);
		if(ReadEntryByNameStr(DB_NAME_Start_Cmd[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Start_Cmd[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Start_Cmd[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="Stop_Command" */
	strcpy(keyword, "Stop_Command");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Stop_Cmd[i]);
		if(ReadEntryByNameStr(DB_NAME_Stop_Cmd[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Stop_Cmd[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Stop_Cmd[i]);
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
	
	/* KEYWORD=="Efficiency" */
	strcpy(keyword, "Efficiency");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Efficiency[i]);
		if(ReadEntryByNameStr(DB_NAME_Efficiency[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Efficiency[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Efficiency[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="Flow" */
	strcpy(keyword, "Flow");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Flow[i]);
		if(ReadEntryByNameStr(DB_NAME_Flow[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Flow[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Flow[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="Unit_Efficiency" */
	strcpy(keyword, "Unit_Efficiency");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Unit_Efficiency[i]);
		if(ReadEntryByNameStr(DB_NAME_Unit_Efficiency[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Unit_Efficiency[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Unit_Efficiency[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="Real_APSetpoint" */
	strcpy(keyword, "Real_APSetpoint");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Real_APSetpoint[i]);
		if(ReadEntryByNameStr(DB_NAME_Real_APSetpoint[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Real_APSetpoint[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Real_APSetpoint[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="Real_FrSetpoint" */
	strcpy(keyword, "Real_FrSetpoint");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Real_FrSetpoint[i]);
		if(ReadEntryByNameStr(DB_NAME_Real_FrSetpoint[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Real_FrSetpoint[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Real_FrSetpoint[i]);
			Err_log(err_str);
			exit(0);
		}
	} 
	
	/* KEYWORD=="AP_RtSum" */
	strcpy(keyword, "AP_RtSum");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AP_RtSum[i]);
		if(ReadEntryByNameStr(DB_NAME_AP_RtSum[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_AP_RtSum[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_RtSum[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="RunInVibration" */
	strcpy(keyword, "RunInVibration");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RunInVibration[i]);
		if(ReadEntryByNameStr(DB_NAME_RunInVibration[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_RunInVibration[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_RunInVibration[i]);
			Err_log(err_str);
			exit(0);
		}
	} 
	
	/* KEYWORD=="PlantStrStp_Req" */
	strcpy(keyword, "PlantStrStp_Req");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_PlantStrStp_Req[i]);
		if(ReadEntryByNameStr(DB_NAME_PlantStrStp_Req[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_PlantStrStp_Req[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_PlantStrStp_Req[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="SSAnswerYes" */
	strcpy(keyword, "SSAnswerYes");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_SSAnswerYes[i]);
		if(ReadEntryByNameStr(DB_NAME_SSAnswerYes[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_SSAnswerYes[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_SSAnswerYes[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="SSAnswerNo" */
	strcpy(keyword, "SSAnswerNo");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_SSAnswerNo[i]);
		if(ReadEntryByNameStr(DB_NAME_SSAnswerNo[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_SSAnswerNo[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_SSAnswerNo[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_Deadband[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="AP_Fail" */
	strcpy(keyword, "AP_Fail");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AP_Fail[i]);
		if(ReadEntryByNameStr(DB_NAME_AP_Fail[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_AP_Fail[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_Fail[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="FR_Fail" */
	strcpy(keyword, "FR_Fail");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_FR_Fail[i]);
		if(ReadEntryByNameStr(DB_NAME_FR_Fail[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_FR_Fail[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_FR_Fail[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_FR_PowGenDB[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="Offset" */
	strcpy(keyword, "Offset");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Offset[i]);
		if(ReadEntryByNameStr(DB_NAME_Offset[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Offset[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Offset[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="Offset_Enable" */
	strcpy(keyword, "Offset_Enable");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Offset_Enable[i]);
		if(ReadEntryByNameStr(DB_NAME_Offset_Enable[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Offset_Enable[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Offset_Enable[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_FR_Min[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_FR_Max[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_EC_DEADBAND[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_FR_ChangeMax[i]);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_ReduceLoad[i]);
			Err_log(err_str);
			exit(0);
		}
	}   
	
	/* KEYWORD=="Real_SRValue" */
	strcpy(keyword, "Real_SRValue");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Real_SRValue[i]);
		if(ReadEntryByNameStr(DB_NAME_Real_SRValue[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Real_SRValue[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Real_SRValue[i]);
			Err_log(err_str);
			exit(0);
		}
	}   
	
	/* KEYWORD=="Unit_AP_Fail" */
	strcpy(keyword, "Unit_AP_Fail");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Unit_AP_Fail[i]);
		if(ReadEntryByNameStr(DB_NAME_Unit_AP_Fail[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Unit_AP_Fail[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Unit_AP_Fail[i]);
			Err_log(err_str);
			exit(0);
		}
	}  
	
	/* KEYWORD=="Curve_Too_Small" */
	strcpy(keyword, "Curve_Too_Small");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Curve_Too_Small[i]);
		if(ReadEntryByNameStr(DB_NAME_Curve_Too_Small[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Curve_Too_Small[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Curve_Too_Small[i]);
			Err_log(err_str);
			exit(0);
		}
	}   
	
	
	/* KEYWORD=="AP_Dev_Alm" */
	strcpy(keyword, "AP_Dev_Alm");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AP_Dev_Alm[i]);
		if(ReadEntryByNameStr(DB_NAME_AP_Dev_Alm[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_AP_Dev_Alm[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_AP_Dev_Alm[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_SR_MaxChange[i]);
			Err_log(err_str);
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
			sprintf(err_str,"No this point %s\n",DB_NAME_SR_MaxValue[i]);
			Err_log(err_str);
			exit(0);
		}
	} 
	
	/* KEYWORD=="Copy_AP_SD" */
	strcpy(keyword, "Copy_AP_SD");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Copy_AP_SD[i]);
		if(ReadEntryByNameStr(DB_NAME_Copy_AP_SD[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Copy_AP_SD[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Copy_AP_SD[i]);
			Err_log(err_str);
			exit(0);
		} 
	} 
	
	/* KEYWORD=="Copy_AP_XZ" */
	strcpy(keyword, "Copy_AP_XZ");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Copy_AP_XZ[i]);
		if(ReadEntryByNameStr(DB_NAME_Copy_AP_XZ[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Copy_AP_XZ[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Copy_AP_XZ[i]);
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
	
	/* KEYWORD=="AGC_Whead" */
	strcpy(keyword, "AGC_Whead");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	fscanf(fp,"%s\n",DB_NAME_AGC_Whead);
	if(ReadEntryByNameStr(DB_NAME_AGC_Whead,&common_aa)==-1)
	{
		printf("No this point %s\n",DB_NAME_AGC_Whead);
		sprintf(err_str,"No this point %s\n",DB_NAME_AGC_Whead);
		Err_log(err_str);
		exit(0);
	}
	
	   /* KEYWORD=="YG_GD" */
	strcpy(keyword, "YG_GD");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_YG_GD[i]);
		if(ReadEntryByNameStr(DB_NAME_YG_GD[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_YG_GD[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_YG_GD[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="Water_Level1" */
	strcpy(keyword, "Water_Level1");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
    for(i=0;i<2;i++)
    {
        fscanf(fp,"%s\n",DB_NAME_Water_Level1[i]);
        if(ReadEntryByNameStr(DB_NAME_Water_Level1[i],&common_aa)==-1)
        {
            printf("No this point %s\n",DB_NAME_Water_Level1[i]);
            sprintf(err_str,"No this point %s\n",DB_NAME_Water_Level1[i]);
			Err_log(err_str);
            exit(0);
        }
    }
	
	/* KEYWORD=="Water_Level2" */
	strcpy(keyword, "Water_Level2");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
    for(i=0;i<2;i++)
    {
        fscanf(fp,"%s\n",DB_NAME_Water_Level2[i]);
        if(ReadEntryByNameStr(DB_NAME_Water_Level2[i],&common_aa)==-1)
        {
            printf("No this point %s\n",DB_NAME_Water_Level2[i]);
            sprintf(err_str,"No this point %s\n",DB_NAME_Water_Level2[i]);
			Err_log(err_str);
            exit(0);
        }
    }
	
	/* KEYWORD=="JRFHQX_SD" */
	strcpy(keyword, "JRFHQX_SD");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
    for(i=0;i<MAXSEGMENT;i++)
    {
		fscanf(fp,"%s\n",DB_NAME_JRFHQX_SD[i]);
		if(ReadEntryByNameStr(DB_NAME_JRFHQX_SD[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_JRFHQX_SD[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_JRFHQX_SD[i]);
			Err_log(err_str);
			exit(0);
		}
    }
	
	/* KEYWORD=="MRFHQX_SD" */
	strcpy(keyword, "MRFHQX_SD");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
    for(i=0;i<MAXSEGMENT;i++)
    {
		fscanf(fp,"%s\n",DB_NAME_MRFHQX_SD[i]);
		if(ReadEntryByNameStr(DB_NAME_MRFHQX_SD[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_MRFHQX_SD[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_MRFHQX_SD[i]);
			Err_log(err_str);
			exit(0);
		}
    }
	
	/* KEYWORD=="JRFHQX_XZ" */
	strcpy(keyword, "JRFHQX_XZ");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
    for(i=0;i<MAXSEGMENT;i++)
    {
		fscanf(fp,"%s\n",DB_NAME_JRFHQX_XZ[i]);
		if(ReadEntryByNameStr(DB_NAME_JRFHQX_XZ[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_JRFHQX_XZ[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_JRFHQX_XZ[i]);
			Err_log(err_str);
			exit(0);
		}
    }
	
	/* KEYWORD=="MRFHQX_XZ" */
	strcpy(keyword, "MRFHQX_XZ");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
    for(i=0;i<MAXSEGMENT;i++)
    {
		fscanf(fp,"%s\n",DB_NAME_MRFHQX_XZ[i]);
		if(ReadEntryByNameStr(DB_NAME_MRFHQX_XZ[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_MRFHQX_XZ[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_MRFHQX_XZ[i]);
			Err_log(err_str);
			exit(0);
		}
    }
	
	/* KEYWORD=="TD_JRFHQX_SD" */
	strcpy(keyword, "TD_JRFHQX_SD");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
    for(i=0;i<MAXSEGMENT;i++)
    {
		fscanf(fp,"%s\n",DB_NAME_TD_JRFHQX_SD[i]);
		if(ReadEntryByNameStr(DB_NAME_TD_JRFHQX_SD[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_TD_JRFHQX_SD[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_TD_JRFHQX_SD[i]);
			Err_log(err_str);
			exit(0);
		}
    }
	
	/* KEYWORD=="TD_MRFHQX_SD" */
	strcpy(keyword, "TD_MRFHQX_SD");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
    for(i=0;i<MAXSEGMENT;i++)
    {
		fscanf(fp,"%s\n",DB_NAME_TD_MRFHQX_SD[i]);
		if(ReadEntryByNameStr(DB_NAME_TD_MRFHQX_SD[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_TD_MRFHQX_SD[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_TD_MRFHQX_SD[i]);
			Err_log(err_str);
			exit(0);
		}
    }
	
	/* KEYWORD=="TD_JRFHQX_XZ" */
	strcpy(keyword, "TD_JRFHQX_XZ");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
    for(i=0;i<MAXSEGMENT;i++)
    {
		fscanf(fp,"%s\n",DB_NAME_TD_JRFHQX_XZ[i]);
		if(ReadEntryByNameStr(DB_NAME_TD_JRFHQX_XZ[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_TD_JRFHQX_XZ[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_TD_JRFHQX_XZ[i]);
			Err_log(err_str);
			exit(0);
		}
    }
	
	/* KEYWORD=="TD_MRFHQX_XZ" */
	strcpy(keyword, "TD_MRFHQX_XZ");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
    for(i=0;i<MAXSEGMENT;i++)
    {
		fscanf(fp,"%s\n",DB_NAME_TD_MRFHQX_XZ[i]);
		if(ReadEntryByNameStr(DB_NAME_TD_MRFHQX_XZ[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_TD_MRFHQX_XZ[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_TD_MRFHQX_XZ[i]);
			Err_log(err_str);
			exit(0);
		}
    }
	
	/* KEYWORD=="Copy_JR_AP_SD" */
	strcpy(keyword, "Copy_JR_AP_SD");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Copy_JR_AP_SD[i]);
		if(ReadEntryByNameStr(DB_NAME_Copy_JR_AP_SD[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Copy_JR_AP_SD[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Copy_JR_AP_SD[i]);
			Err_log(err_str);
			exit(0);
		} 
	} 
	
	/* KEYWORD=="Copy_JR_AP_XZ" */
	strcpy(keyword, "Copy_JR_AP_XZ");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Copy_JR_AP_XZ[i]);
		if(ReadEntryByNameStr(DB_NAME_Copy_JR_AP_XZ[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Copy_JR_AP_XZ[i]);
			sprintf(err_str,"No this point %s\n",DB_NAME_Copy_JR_AP_XZ[i]);
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
	
	
	/* KEYWORD=="FailToSetp" */
	strcpy(keyword, "FailToSetp");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_FailToSetp[i]);
		if(ReadEntryByNameStr(DB_NAME_FailToSetp[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_FailToSetp[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_FailToSetp[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="Gap" */
	strcpy(keyword, "Gap");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Gap[i]);
		if(ReadEntryByNameStr(DB_NAME_Gap[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Gap[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_Gap[i]);
			Err_log(err_str);
			exit(0);
		}
	}
	
	/* KEYWORD=="Setp_Error" */
	strcpy(keyword, "Setp_Error");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Setp_Error[i]);
		if(ReadEntryByNameStr(DB_NAME_Setp_Error[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_Setp_Error[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_Setp_Error[i]);
			Err_log(err_str);
			exit(0);
		}
	}

	/* KEYWORD=="FR_REACT_VALUE" */
	strcpy(keyword, "FR_REACT_VALUE");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_FR_REACT_VALUE[i]);
		if(ReadEntryByNameStr(DB_NAME_FR_REACT_VALUE[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_FR_REACT_VALUE[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_FR_REACT_VALUE[i]);
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

	/* KEYWORD=="YGGD_PYL" */
	strcpy(keyword, "YGGD_PYL");
	if(-1==LocateKeyword(fp, keyword))
	{
		sprintf(err_str, "Fail to find %s keyword", keyword);
		Err_log(err_str);
		exit(0);
	}
	for(i=0;i<MAXSEGMENT;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_YGGD_PYL[i]);
		if(ReadEntryByNameStr(DB_NAME_YGGD_PYL[i],&common_aa)==-1)
		{
			printf("No this point %s\n",DB_NAME_YGGD_PYL[i]);
			sprintf(err_str, "No this point %s\n",DB_NAME_YGGD_PYL[i]);
			Err_log(err_str);
			exit(0);
		}
	}
#endif
	
	fclose(fp);	
}
void InitData()
{
    int i1,Unit_Status,segment;
    float p_sum;
    for(segment=0;segment<MAXSEGMENT;segment++)
    {
		Move_Curve[segment]=1;
		Dispatch[segment]=1;
		SaveTime[segment]=0;
		AGC_Recalc[segment]=0;
		ControlState[segment]=0;
		AP_Faulty[segment]=0;

		p_sum=0;
		PLANT[segment].FirstRun=1;
        for(i1=1;i1<=UNIT_NUMB;i1++)
        {
            if(UNIT[i1].segment!=segment)continue;
            Unit_Status=ReadDB_IND(DB_NAME_UNIT_STATUS[i1]);
            UNIT[i1].Last_Status=Unit_Status;
            if(Unit_Status==UNIT_IN_GEN)
            {
                UNIT[i1].XAP_Setp=ReadDB_ANA(DB_NAME_AP_RTVALUE[i1]);
                p_sum+=UNIT[i1].XAP_Setp; 
            }
            else
                UNIT[i1].XAP_Setp=0;
            UNIT[i1].AP_Setp=UNIT[i1].YAP_Setp=UNIT[i1].ZAP_Setp=UNIT[i1].XAP_Setp;
            UNIT[i1].Zone=CalcUnitZone(UNIT[i1].Type, UNIT[i1].XAP_Setp);
            
            UNIT[i1].StartFlag=0;
            UNIT[i1].StopFlag=0;
            UNIT[i1].AP_Fail=0;
            UNIT[i1].XAPCompMode=0;
            UNIT[i1].ReduceLoad=0;
            UNIT[i1].XStartUpTime=0;
            UNIT[i1].StartBlockTime=0;
            UNIT[i1].StopBlockTime=0;
            UNIT[i1].AP_FailTime=0;
            UNIT[i1].ReduceLoadFailTime=0;
            UNIT[i1].StartCmdTime=0;
            UNIT[i1].StopCmdTime=0;
            UNIT[i1].StopTime=0;
            UNIT[i1].StartTime=0;
			UNIT[i1].AllocationInVib=0;
            AGC_UpdateRTDB_IND(DB_NAME_Unit_AP_Fail[i1],0);
            AGC_UpdateRTDB_IND(DB_NAME_Start_Req[i1],0);
            AGC_UpdateRTDB_IND(DB_NAME_Stop_Req[i1],0);
        }
		
        PLANT[segment].XFreqDoopAdaptFact=1;
        PLANT[segment].XStrStp_Delay=0;
		
        PLANT[segment].AP_WorkSP=p_sum;   
        PLANT[segment].XAP_FreqCTRL=p_sum;
        PLANT[segment].Lim_Change=0;
        PLANT[segment].AP_Setpoint=p_sum;
        PLANT[segment].XAP_Setpoint=p_sum;
        PLANT[segment].YAP_Setpoint=p_sum; 
        PLANT[segment].SR_SetPoint=0;
        PLANT[segment].APCopyTime=0;
        PLANT[segment].SRCopyTime=0;
		PLANT[segment].JR_APCopyTime=0;
        PLANT[segment].JR_SRCopyTime=0;
        PLANT[segment].AP_Alm_Time=0;
        PLANT[segment].FR_Fail_Time=0;
    }	
}

void Unit_Supervision(int segment)
{
    int i1,VibNumber,CompNextStep,Status;
    int INC_Dir,Vib_Zone;
    char ErrSms[100];
    float CompNextVal;

#ifdef CASCADE
	if (!PLANT[segment].AllocationToUnit)
	{
		return;
	}
#endif

    Vib_Zone=0;
    CompNextStep=1;
    if(PLANT[segment].APAnswerYesTime>0)
    {
		PLANT[segment].APAnswerYesTime--;
        if(PLANT[segment].APAnswerYesTime==1)
        {
			PLANT[segment].APAnswerYes=0;
			AGC_UpdateRTDB_IND(DB_NAME_AP_ANSWERYES[segment],0);
		}
	}
	else
	{
		if(PLANT[segment].APAnswerYesTime==0)
		{
			Status=ReadDB_IND(DB_NAME_AP_ANSWERYES[segment]);
			if(Status!=0)
				AGC_UpdateRTDB_IND(DB_NAME_AP_ANSWERYES[segment],0);
		}
	}
    if(PLANT[segment].SSAnswerYesTime>0)
    {
        PLANT[segment].SSAnswerYesTime--;
        if(PLANT[segment].SSAnswerYesTime==1)
        {
			PLANT[segment].SSAnswerYes=0;
			AGC_UpdateRTDB_IND(DB_NAME_SSAnswerYes[segment],0);
		}
	}
	else
	{
		if(PLANT[segment].SSAnswerYesTime==0)
		{
			Status=ReadDB_IND(DB_NAME_SSAnswerYes[segment]);
			if(Status!=0)
				AGC_UpdateRTDB_IND(DB_NAME_SSAnswerYes[segment],0);
		}
	}
    if(PLANT[segment].SSAnswerNoTime>0)
    {
        PLANT[segment].SSAnswerNoTime--;
        if(PLANT[segment].SSAnswerNoTime==1)
        {
			PLANT[segment].SSAnswerNo=0;
			AGC_UpdateRTDB_IND(DB_NAME_SSAnswerNo[segment],0);
		}
	}  
	else
	{
		if(PLANT[segment].SSAnswerNoTime==0)
		{
			Status=ReadDB_IND(DB_NAME_SSAnswerNo[segment]);
			if(Status!=0)
				AGC_UpdateRTDB_IND(DB_NAME_SSAnswerNo[segment],0);
		}
	}
	if(PLANT[segment].APCopyTime>0)
    {
        PLANT[segment].APCopyTime--;
        if(PLANT[segment].APCopyTime==1)
        {
			PLANT[segment].APCopyTime=0;
			AGC_UpdateRTDB_IND(DB_NAME_Copy_AP_SD[segment],0);
		}
	}
	else 
	{
		if(PLANT[segment].APCopyTime==0)
		{
			Status=ReadDB_IND(DB_NAME_Copy_AP_SD[segment]);
			if(Status!=0)
				AGC_UpdateRTDB_IND(DB_NAME_Copy_AP_SD[segment],0);
		}
	}
    if(PLANT[segment].SRCopyTime>0)
    {
        PLANT[segment].SRCopyTime--;
        if(PLANT[segment].SRCopyTime==1)
        {
			PLANT[segment].SRCopyTime=0;
			AGC_UpdateRTDB_IND(DB_NAME_Copy_AP_XZ[segment],0);
		}
	}
	else
	{
		if(PLANT[segment].SRCopyTime==0)
		{
			Status=ReadDB_IND(DB_NAME_Copy_AP_XZ[segment]);
			if(Status!=0)
				AGC_UpdateRTDB_IND(DB_NAME_Copy_AP_XZ[segment],0);
		}
	}
	
	if(PLANT[segment].JR_APCopyTime>0)
    {
        PLANT[segment].JR_APCopyTime--;
        if(PLANT[segment].JR_APCopyTime==1)
        {
			PLANT[segment].JR_APCopyTime=0;
			AGC_UpdateRTDB_IND(DB_NAME_Copy_JR_AP_SD[segment],0);
		}
	}
	else 
	{
		if(PLANT[segment].JR_APCopyTime==0)
		{
			Status=ReadDB_IND(DB_NAME_Copy_JR_AP_SD[segment]);
			if(Status!=0)
				AGC_UpdateRTDB_IND(DB_NAME_Copy_JR_AP_SD[segment],0);
		}
	}
    if(PLANT[segment].JR_SRCopyTime>0)
    {
        PLANT[segment].JR_SRCopyTime--;
        if(PLANT[segment].JR_SRCopyTime==1)
        {
			PLANT[segment].JR_SRCopyTime=0;
			AGC_UpdateRTDB_IND(DB_NAME_Copy_JR_AP_XZ[segment],0);
		}
	}
	else
	{
		if(PLANT[segment].JR_SRCopyTime==0)
		{
			Status=ReadDB_IND(DB_NAME_Copy_JR_AP_XZ[segment]);
			if(Status!=0)
				AGC_UpdateRTDB_IND(DB_NAME_Copy_JR_AP_XZ[segment],0);
		}
	}
	
	if(PLANT[segment].Setp_ErrorTime>0)
	{
		PLANT[segment].Setp_ErrorTime--;
		if(PLANT[segment].Setp_ErrorTime==1)
		{
			PLANT[segment].Setp_ErrorTime=0;
			AGC_UpdateRTDB_IND(DB_NAME_Setp_Error[segment],0);
		}
	}
	else
	{
		if(PLANT[segment].Setp_ErrorTime==0)
		{
			Status=ReadDB_IND(DB_NAME_Setp_Error[segment]);
			if(Status!=0)
				AGC_UpdateRTDB_IND(DB_NAME_Setp_Error[segment],0);
		}
	}
	
    if((PLANT[segment].AP_MODE==1)&&(PLANT[segment].AP_Setting==1))
    {
        if((PLANT[segment].Num_Unit_Run>0)&&(PLANT[segment].XAGC_Unit_Comp==0))
        {
            if(fabs(PLANT[segment].p_sum-PLANT[segment].AP_WorkSP)>AP_AlarmFact*PLANT[segment].AP_Deadband)
            {
                //printf("******segment=%d,%d\n",segment,PLANT[segment].AP_Fail_Time);
                if(PLANT[segment].AP_Fail_Time<VibAlarmTime)
                    PLANT[segment].AP_Fail_Time++;
                else
                {
                    AGC_UpdateRTDB_IND(DB_NAME_AP_Fail[segment],1);
                    //printf("segment %d AP Fail!%s\n",segment,DB_NAME_AP_Fail[segment]);
                }
            }
            else
            {
                PLANT[segment].AP_Fail_Time=0;
                AGC_UpdateRTDB_IND(DB_NAME_AP_Fail[segment],0);
            }
        }
        else
        {
            PLANT[segment].AP_Fail_Time=0;
            AGC_UpdateRTDB_IND(DB_NAME_AP_Fail[segment],0);
        }
    }
    else
    {
        PLANT[segment].AP_Fail_Time=0;
        AGC_UpdateRTDB_IND(DB_NAME_AP_Fail[segment],0);
    }
    if((PLANT[segment].FR_Compensation == 1)||(PLANT[segment].FR_Regulation == 1))
    {   
        if(PLANT[segment].Dev_Frequency>FR_AlarmFact*PLANT[segment].FR_Deadband)
        {    
			// printf("*****segment=%d,Dev_Frequency=%f,FR_Fail_Time=%d\n",segment,PLANT[segment].Dev_Frequency,PLANT[segment].FR_Fail_Time);
            if(PLANT[segment].FR_Fail_Time<Time_5s)
                PLANT[segment].FR_Fail_Time++;
            else
                AGC_UpdateRTDB_IND(DB_NAME_FR_Fail[segment],1);
        }
        else
        {
            PLANT[segment].FR_Fail_Time=0;
            AGC_UpdateRTDB_IND(DB_NAME_FR_Fail[segment],0);
        }
    }
    else
    {
        PLANT[segment].FR_Fail_Time=0;
        AGC_UpdateRTDB_IND(DB_NAME_FR_Fail[segment],0);
    }
    
    for (i1=1; i1<=UNIT_NUMB; i1++) 
    {	
        if(UNIT[i1].segment!=segment)continue;
        //if((UNIT[i1].Start_Req==1)||(UNIT[i1].Stop_Req==1))
        //printf("Unit No.%d,Start_Req=%d,Stop_Req=%d\n",i1,UNIT[i1].Start_Req,UNIT[i1].Stop_Req);
		
		if((fabs(UNIT[i1].Last_AP_RtValue-UNIT[i1].AP_RtValue)>=400)&&(PLANT[segment].FirstRun==0))
		{
			sprintf(ErrSms,"%d号机有功变化异常",i1);
            if(GetControlState()==1)
                BroadCastSms(ErrSms,TRUE);
		}
		UNIT[i1].Last_AP_RtValue=UNIT[i1].AP_RtValue;
        
        if(UNIT[i1].AP_FailTime>0)
        {
			UNIT[i1].AP_FailTime--;
			if(UNIT[i1].AP_FailTime==1)
			{
				UNIT[i1].AP_FailTime=0;
				AGC_UpdateRTDB_IND(DB_NAME_Unit_AP_Fail[i1],0);
			}
		}
		else 
		{
			UNIT[i1].AP_FailTime=0;
			AGC_UpdateRTDB_IND(DB_NAME_Unit_AP_Fail[i1],0);
		}
        
        if(UNIT[i1].ReduceLoad ==1)
        {
            if(UNIT[i1].AP_Avail!=1)
            {
                UNIT[i1].ReduceLoad=2;
                UNIT[i1].ReduceLoadFailTime=Time_5s;
            }
        }
        if(UNIT[i1].ReduceLoadFailTime>0)
        {
            UNIT[i1].ReduceLoadFailTime--;
            if(UNIT[i1].ReduceLoadFailTime==1)
            {
                UNIT[i1].ReduceLoad =0;
                UNIT[i1].ReduceLoadFailTime=0;
            }
        }
        if(UNIT[i1].Status!=UNIT_IN_GEN)
            UNIT[i1].ReduceLoad =0;
        AGC_UpdateRTDB_IND(DB_NAME_ReduceLoad[i1],(short)UNIT[i1].ReduceLoad);
		
		if(UNIT[i1].StartBlockTime>0)
			UNIT[i1].StartBlockTime--;  
		if(UNIT[i1].StopBlockTime>0)
			UNIT[i1].StopBlockTime--;
		
		if ((UNIT[i1].Status!=UNIT_IN_GEN) && (UNIT[i1].Last_Status == UNIT_IN_GEN))
			UNIT[i1].StartBlockTime=StartStopBlockTime;
        /*机组并网发电A unit was started now*/
        if ((UNIT[i1].Status==UNIT_IN_GEN) && (UNIT[i1].Last_Status != UNIT_IN_GEN)) 
        {  
			UNIT[i1].StopBlockTime=StartStopBlockTime;
            UNIT[i1].XStartUpTime = Time_30s;
            UNIT[i1].XPowerUp=1;
            SetUnitVibMinMax(i1);
        }  
        
        UNIT[i1].Last_Status = UNIT[i1].Status;
		
        if (UNIT[i1].XStartUpTime > 0) 
            UNIT[i1].XStartUpTime--;
		
        /*检查机组在振动区或小于最小有功*/
        if (UNIT[i1].Status==UNIT_IN_GEN) 
        {   
            VibNumber = CheckPowerVibZone(UNIT[i1].Type, UNIT[i1].AP_RtValue, APFilterDB);
            if((VibNumber > 0)||((UNIT[i1].AP_RtValue + APFilterDB) < UNIT[i1].AP_Min))
            {
                if (UNIT[i1].XVibDelay < VibAlarmTime)
                    UNIT[i1].XVibDelay++;
                else	
                {
                    UNIT[i1].Vib_Zone = 1;
                    if(UNIT[i1].XVibDelay==VibAlarmTime)
                    {
                        sprintf(ErrSms,"%s:%d号机运行在振动区",PLANT[segment].English_Name,i1);
                        if(GetControlState()==1)
							BroadCastSms(ErrSms,TRUE);
                        UNIT[i1].XVibDelay+=10;
                    }
                    Vib_Zone=1;
                }
            }
            else
            {
                UNIT[i1].XVibDelay = 0;
                UNIT[i1].Vib_Zone = 0;
            }
        } 
        else
        {
            UNIT[i1].XVibDelay = 0;
            UNIT[i1].Vib_Zone = 0;
        }
		
        if (UNIT[i1].XAPCompMode > 0)
        {
            if (UNIT[i1].APCompTime > 0) 
            {
                UNIT[i1].APCompTime--; //printf("No.%d,APCompTime=%d\n",i1,UNIT[i1].APCompTime);
                if (UNIT[i1].StopAction == 0) 
                {
                    if (fabs(UNIT[i1].AP_RtValue - UNIT[i1].XAP_Setp) < APFilterDB)
                    {
                        UNIT[i1].XAPCompMode = 0;
                        UNIT[i1].XPowerUp = 0;
                        UNIT[i1].StartProcess=0;
                        printf("%d 号机实发有功接近给定值\n",i1);
                    }
                }
            }
            else 
            {
                UNIT[i1].XAPCompMode = 0;
                UNIT[i1].XPowerUp = 0;
                UNIT[i1].StartProcess=0;
                printf("%d号机超过补偿时间.\n",i1);
            }
        }
		
        /*复归开机请求和命令*/
        if ((UNIT[i1].Status ==UNIT_IN_GEN) || (UNIT[i1].Starting == 1)) 
        {
            UNIT[i1].Start_Req = 0;
            if (UNIT[i1].CommandTime < CommandTime)
                UNIT[i1].CommandTime++;
            else 
			{
                UNIT[i1].Start = 0;
				AGC_UpdateRTDB_IND(DB_NAME_Start_Req[i1],0);
			}
        }
		
        /*Unit is starting - move of unit*/
        if (UNIT[i1].Starting == 1) 
        {
            UNIT[i1].XAP_Err_Detect.MoveTime = UnitMoveTime;
            UNIT[i1].XAP_Err_Detect.Moving = 1;
        }
        /*Reset start action*/
        if ((UNIT[i1].Status == UNIT_IN_GEN)&&(UNIT[i1].Starting==0)) 
        {
            UNIT[i1].StartAction = 0;
            UNIT[i1].StartTime=0;
            AGC_UpdateRTDB_IND(DB_NAME_Start_Req[i1],0);
			AGC_UpdateRTDB_IND(DB_NAME_Start_Cmd[i1],0);
        }
		
		
        /*监视机组启动Supervision of start*/
        if (UNIT[i1].StartAction == 1) 
        {
            UNIT[i1].StartTime++;
            /*Start failure because of no response to start command*/
            if ((UNIT[i1].Start == 1) && (UNIT[i1].StartTime > ResponseDelay))
            {
                UNIT[i1].XPowerUp = 0;
                UNIT[i1].StartAction = 0;
                UNIT[i1].Start_Req = 0;
                UNIT[i1].Start = 0;
                PLANT[segment].XUnitStrStp =1;
				UNIT[i1].AP_Avail=0;
                UNIT[i1].StartProcess=0;
				AGC_UpdateRTDB_IND(DB_NAME_UNIT_AGCSEL[i1],0);
				AGC_UpdateRTDB_IND(DB_NAME_Start_Cmd[i1],0);
                sprintf(ErrSms,"%s:%d号机开机失败!\n",PLANT[segment].English_Name,i1);
                if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
            }
            /*Start failure because of command not executed in time*/
            if (UNIT[i1].StartTime > MaxStartTime) 
            {
                UNIT[i1].XPowerUp = 0;
                UNIT[i1].StartAction = 0;
                UNIT[i1].Start_Req = 0;
                UNIT[i1].Start = 0;
                UNIT[i1].XAP_Err_Detect.MoveTime = 0;
                UNIT[i1].XAP_Err_Detect.Moving = 0;
                PLANT[segment].XUnitStrStp =1;
                UNIT[i1].StartProcess=0;
				UNIT[i1].AP_Avail=0;
				AGC_UpdateRTDB_IND(DB_NAME_UNIT_AGCSEL[i1],0);
				AGC_UpdateRTDB_IND(DB_NAME_Start_Cmd[i1],0);
                sprintf(ErrSms,"%s:%d号机开机失败!\n",PLANT[segment].English_Name,i1);
                if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
            }
			if ((UNIT[i1].Start ==0)&&(UNIT[i1].Starting==0)&&(UNIT[i1].Status!=UNIT_IN_GEN))
			{
                UNIT[i1].XPowerUp = 0;
                UNIT[i1].StartAction = 0;
                UNIT[i1].Start_Req = 0;
                UNIT[i1].Start = 0;
                UNIT[i1].XAP_Err_Detect.MoveTime = 0;
                UNIT[i1].XAP_Err_Detect.Moving = 0;
                PLANT[segment].XUnitStrStp =1;
                UNIT[i1].StartProcess=0;
				UNIT[i1].AP_Avail=0;
				AGC_UpdateRTDB_IND(DB_NAME_UNIT_AGCSEL[i1],0);
				AGC_UpdateRTDB_IND(DB_NAME_Start_Cmd[i1],0);
                sprintf(ErrSms,"%s:%d号机开机失败!\n",PLANT[segment].English_Name,i1);
                if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
            }
        }
        /*Execute stop Command*/
        if (UNIT[i1].XStop == 1) 
        {
            UNIT[i1].Stop_Req = 0;
            UNIT[i1].ReduceLoad =1;
            if(UNIT[i1].StopCmdTime > 0) 
            {
                UNIT[i1].StopCmdTime--;
                if((UNIT[i1].AP_RtValue < APStopLimit) &&
					(UNIT[i1].StopCmdTime > Time_20s))
                {
                    UNIT[i1].StopCmdTime = Time_20s;
                }
                UNIT[i1].StopTime = 0;  
            } 
            else 
            {  
                UNIT[i1].XStop = 0;
                UNIT[i1].Stop = 1;
                UNIT[i1].XAP_Setp=0;
                UNIT[i1].CommandTime = 0;
				AGC_UpdateRTDB_IND(DB_NAME_Stop_Req[i1],0);
				if(AutoStartStop[i1]==1)
					AGC_UpdateRTDB_IND(DB_NAME_Stop_Cmd[i1],1);
            }
        }
        /*Reset stop request / command*/
        if ((UNIT[i1].Status!=UNIT_IN_GEN) || (UNIT[i1].Stopping == 1)) 
        {
            UNIT[i1].Stop_Req = 0;
			AGC_UpdateRTDB_IND(DB_NAME_Stop_Req[i1],0);
            if (UNIT[i1].CommandTime < CommandTime) 
                UNIT[i1].CommandTime++;
            else 
			{
                UNIT[i1].Stop = 0;
			}
        }
        /*Reset stop action*/
        if (UNIT[i1].Status !=UNIT_IN_GEN)
        {
            UNIT[i1].StopAction = 0;
            UNIT[i1].StopTime = 0;
            UNIT[i1].XAPCompMode = 0;
            UNIT[i1].XStop = 0;
            UNIT[i1].StopCmdTime = StopCmdTime;
            UNIT[i1].AP_Fail = 0;
            UNIT[i1].APCompTime = 0;
			AGC_UpdateRTDB_IND(DB_NAME_Stop_Cmd[i1],0);
        }
		
        if (UNIT[i1].StopAction == 1) 
        {
            UNIT[i1].StopTime++;
            /*Stop failure because of no response to stop command*/
            if ((UNIT[i1].Stop == 1) && (UNIT[i1].StopTime > ResponseDelay))
            {
                UNIT[i1].StopAction = 0;
                UNIT[i1].Stop_Req = 0;
                UNIT[i1].Stop = 0;
                UNIT[i1].XStop = 0;
                UNIT[i1].StopCmdTime = StopCmdTime;
                UNIT[i1].XAPCompMode = 0;
                UNIT[i1].XPowerUp = 0;
                UNIT[i1].APCompTime = 0;
                PLANT[segment].XUnitStrStp =1;
				UNIT[i1].AP_Avail=0;
				AGC_UpdateRTDB_IND(DB_NAME_UNIT_AGCSEL[i1],0);
				AGC_UpdateRTDB_IND(DB_NAME_Stop_Cmd[i1],0);
                sprintf(ErrSms,"%s:%d号机停机失败!\n",PLANT[segment].English_Name,i1);
                if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
            }
            /*Stop failure because of command not executed in time*/
            if (UNIT[i1].StopTime > MaxStopTime)
            {
                UNIT[i1].StopAction = 0;
                UNIT[i1].Stop_Req = 0;
                UNIT[i1].Stop = 0;
                UNIT[i1].XStop = 0;
                UNIT[i1].StopCmdTime = StopCmdTime;
                UNIT[i1].XAPCompMode = 0;
                UNIT[i1].XPowerUp = 0;
                UNIT[i1].APCompTime = 0;
                PLANT[segment].XUnitStrStp =1;
				UNIT[i1].AP_Avail=0;
				AGC_UpdateRTDB_IND(DB_NAME_UNIT_AGCSEL[i1],0);
				AGC_UpdateRTDB_IND(DB_NAME_Stop_Cmd[i1],0);
                sprintf(ErrSms,"%s:%d号机停机失败!\n",PLANT[segment].English_Name,i1);
                if(GetControlState()==1)
					BroadCastSms(ErrSms,TRUE);
            }
        }
		
		
        if (UNIT[i1].AP_Avail==1)
        {
            if ((UNIT[i1].StartAction == 0) && (UNIT[i1].Status !=UNIT_IN_GEN)) 
            {
                UNIT[i1].XAP_Setp = 0;
                UNIT[i1].AP_Setp = 0;
            }
            else /*UNIT[i1].StartAction =1  或UNIT[i1].Status ==UNIT_IN_GEN */
            {
                if (PLANT[segment].Closed_Loop!=OPEN_LOOP) 
                {
                    //Unit setpoint was changed
                    if (fabs(UNIT[i1].AP_Setp - UNIT[i1].XAP_Setp) > APSetpDB)
                    {
                        UNIT[i1].XAP_Setp_Calc = 1;
                        UNIT[i1].XAP_Err_Detect.HL_Value = UNIT[i1].XAP_Setp + Unit_AP_MaxDev;
                        UNIT[i1].XAP_Err_Detect.LL_Value = UNIT[i1].XAP_Setp - Unit_AP_MaxDev;
						
                        if (UNIT[i1].XAP_Setp > UNIT[i1].YAP_Setp) 
                            INC_Dir = 1;
                        else 
                            INC_Dir = 0;
						
                        if ((UNIT[i1].XAP_Err_Detect.MoveTime == 0) ||
							(UNIT[i1].XAP_Err_Detect.INC_Dir != INC_Dir))
                        {
                            UNIT[i1].XAP_Err_Detect.LastValue = UNIT[i1].AP_RtValue;
                        }
                        UNIT[i1].XAP_Err_Detect.MoveTime = UnitMoveTime;
                        UNIT[i1].XAP_Err_Detect.INC_Dir = INC_Dir;
                        UNIT[i1].AP_Setp = UNIT[i1].XAP_Setp;
                        UNIT[i1].XAP_Err_Detect.Enabled = 0;
                    }
                } 
                else /*PLANT[segment].Closed_Loop==OPEN_LOOP*/
                {
                    UNIT[i1].XAP_Err_Detect.Enabled = 0;
                    UNIT[i1].XAPCompMode = 0;
                    UNIT[i1].APCompTime = 0;
                    UNIT[i1].AP_Setp = UNIT[i1].XAP_Setp;
                    UNIT[i1].XAP_Err_Detect.MoveTime = UnitMoveTime;
                }
            } /*UNIT[i1].StartAction =1  或UNIT[i1].Status ==UNIT_IN_GEN 结束*/
            CompNextVal=UnitAPMove;
            if(UNIT[i1].XStartUpTime>0)CompNextVal=APFilterDB;
            if (fabs(UNIT[i1].AP_RtValue - UNIT[i1].YAP_Setp)> CompNextVal)CompNextStep=0;
			
            //Handling of unit control error - active power
            if (UNIT[i1].Status==UNIT_IN_GEN) 
            {
                if ((UNIT[i1].AP_RtValue < UNIT[i1].XAP_Err_Detect.LL_Value) ||
					(UNIT[i1].AP_RtValue > UNIT[i1].XAP_Err_Detect.HL_Value))
                {
                    /*Detection of unit moving*/
                    if (UNIT[i1].XAP_Err_Detect.INC_Dir == 1) 
                    {
                        //Detect unit is increasing
                        if ((UNIT[i1].AP_RtValue - UNIT[i1].XAP_Err_Detect.LastValue) > UnitAPMove)
                        {
                            UNIT[i1].XAP_Err_Detect.LastValue = UNIT[i1].AP_RtValue;
                            if (UNIT[i1].AP_RtValue < UNIT[i1].XAP_Err_Detect.HL_Value)
                            {
                                UNIT[i1].XAP_Err_Detect.MoveTime = UnitMoveTime;
                                UNIT[i1].XAP_Err_Detect.Moving = 1;
                            }
                        }
                    } 
                    else /*UNIT[i1].XAP_Err_Detect.INC_Dir == 0  开始*/
                    {
                        //Detect unit is decreasing
                        if ((UNIT[i1].XAP_Err_Detect.LastValue - UNIT[i1].AP_RtValue) >UnitAPMove)
                        {
                            UNIT[i1].XAP_Err_Detect.LastValue = UNIT[i1].AP_RtValue;
                            if (UNIT[i1].AP_RtValue > UNIT[i1].XAP_Err_Detect.LL_Value) 
                            {
                                UNIT[i1].XAP_Err_Detect.MoveTime = UnitMoveTime;
                                UNIT[i1].XAP_Err_Detect.Moving = 1;
                            }
                        }
                    }  /*UNIT[i1].XAP_Err_Detect.INC_Dir == 0  结束*/
					
                    //Detect active power control failure
                    if ((UNIT[i1].XAP_Err_Detect.Enabled==1) &&
						(PLANT[segment].Closed_Loop!= OPEN_LOOP))
                    {
                        //Error detection timer
                        if (UNIT[i1].XAP_Err_Detect.MoveTime > 0) 
                        {  printf("Unit %d MoveTime=%d\n",i1,UNIT[i1].XAP_Err_Detect.MoveTime);
						UNIT[i1].XAP_Err_Detect.MoveTime--;
						if (UNIT[i1].XAP_Err_Detect.MoveTime == 0)
						{
							UNIT[i1].XAP_Setp_Calc = 0;
							UNIT[i1].XAPCompMode = 0;
						}
                        } 
                        else   /*(UNIT[i1].XAP_Err_Detect.MoveTime <=0)开始*/
                        {
                            if (fabs(UNIT[i1].AP_RtValue - UNIT[i1].AP_Setp) > Unit_AP_MaxDev)
                            {
                                UNIT[i1].AP_Fail = 1;
                                UNIT[i1].XAP_Setp_Calc = 0;
                                UNIT[i1].XAPCompMode = 0;
								UNIT[i1].AP_Avail=0;
								AGC_UpdateRTDB_IND(DB_NAME_UNIT_AGCSEL[i1],0);
                                AGC_UpdateRTDB_IND(DB_NAME_Unit_AP_Fail[i1],1);
                                UNIT[i1].AP_FailTime=Time_10s;
                            }
                        }  /*(UNIT[i1].XAP_Err_Detect.MoveTime <=0)结束*/
                    } /*if ((UNIT[i1].XAP_Err_Detect.Enabled==1) &&(PLANT[segment].Closed_Loop!= OPEN_LOOP))结束*/
                } 
                else  /*UNIT[i1].AP_RtValue在监测上下限范围内*/
                {
                    //Reset active power control failure
                    UNIT[i1].AP_Fail = 0;
					
                    //Indication unit is moving
                    if (UNIT[i1].XAP_Err_Detect.MoveTime > 0) 
                    {
                        UNIT[i1].XAP_Err_Detect.MoveTime--;
                        if (UNIT[i1].XAP_Err_Detect.MoveTime == 0) 
                        {
                            UNIT[i1].XAP_Err_Detect.Moving = 0;
                        }
                    }
                }   /*UNIT[i1].AP_RtValue在监测上下限范围内结束*/
            }/*if (UNIT[i1].Status==UNIT_IN_GEN)  结束*/
        } /*if (UNIT[i1].AP_Avail==1)结束*/
        else 
        {
            //Unit in individual mode - set AGC part of unit to balancing
            UNIT[i1].Start = 0;
            UNIT[i1].Stop = 0;
            UNIT[i1].Start_Req = 0;
            UNIT[i1].Stop_Req = 0;
            UNIT[i1].AP_Fail = 0;
            UNIT[i1].StartTime = 0;
            UNIT[i1].StopTime = 0;
            UNIT[i1].CommandTime = 0;
            UNIT[i1].StartAction = 0;
            UNIT[i1].StopAction = 0;
            UNIT[i1].XStop = 0;
            UNIT[i1].XAnswerNo = 0;
            UNIT[i1].StopCmdTime = StopCmdTime;
            UNIT[i1].StartTime = 0;
            UNIT[i1].XPowerUp = 0;
            UNIT[i1].StartProcess=0;
			UNIT[i1].StartFlag=0;
			UNIT[i1].StopFlag=0;
			
            UNIT[i1].XAPCompMode = 0;
            UNIT[i1].XAP_Setp = UNIT[i1].AP_RtValue;
            UNIT[i1].AP_Setp = UNIT[i1].XAP_Setp;
            UNIT[i1].YAP_Setp= UNIT[i1].XAP_Setp;
            UNIT[i1].ZAP_Setp= UNIT[i1].XAP_Setp;
            UNIT[i1].APCompTime = 0;
            UNIT[i1].ReduceLoad =0;
			
            UNIT[i1].XAP_Err_Detect.HL_Value = UNIT[i1].AP_Setp + Unit_AP_MaxDev;
            UNIT[i1].XAP_Err_Detect.LL_Value = UNIT[i1].AP_Setp - Unit_AP_MaxDev;
			
            UNIT[i1].XAP_Err_Detect.LastValue = UNIT[i1].AP_RtValue;
            UNIT[i1].XAP_Err_Detect.MoveTime = UnitMoveTime;
            UNIT[i1].XAP_Err_Detect.Enabled = 0;
            AGC_UpdateRTDB_ANA(DB_NAME_UNIT_SETPOINT[i1],UNIT[i1].AP_RtValue);
            AGC_UpdateRTDB_IND(DB_NAME_Start_Req[i1],0);
			AGC_UpdateRTDB_IND(DB_NAME_Start_Cmd[i1],0);
			AGC_UpdateRTDB_IND(DB_NAME_Stop_Req[i1],0);
			AGC_UpdateRTDB_IND(DB_NAME_Stop_Cmd[i1],0);
        }
    }
    if((PLANT[segment].XAP_Comp_Time > Time_15s)&&(CompNextStep==1))
    {
        PLANT[segment].XAP_Comp_Time = Time_15s;
        printf("This step is OK,Wait 15 second\n");
    }
    if(Vib_Zone==1)
        AGC_UpdateRTDB_IND(DB_NAME_RunInVibration[segment],1);
    else 
        AGC_UpdateRTDB_IND(DB_NAME_RunInVibration[segment],0);
	
}

void AGC_Distribution(int segment)
{
    int U_COMP,U_JUMP,UC,CompModeMax,UJ,i1,i2,i3;
    int U_COMP_Mem[20]; /*参与补偿机组*/
    int U_JUMP_Mem[20]; /*补偿机组Units starting, stopping, crossing vib. zone*/
    int U_JUCO_Mem[20]; /*与补偿机组对应的常用补偿机组Relation between units (JUMP -> COMP)*/
    int U_CO_Used[20]; 
    float AP_Diff_J,AP_Diff_C,AP_Step1,AP_MinStep,AP_Comp_Step,AP_Step_Dev;
    int CompFunc,AP_CTRL_Fail,Start_Single_Step;
#ifdef CASCADE
	if (!PLANT[segment].AllocationToUnit)
	{
		return;
	}
#endif	
    //Check all segments of power plant
	CompFunc = 0;
	AP_CTRL_Fail = 0;
	Start_Single_Step =0;
	if (PLANT[segment].XAP_DistributeDelay > 0) PLANT[segment].XAP_DistributeDelay--;
	if (PLANT[segment].XAP_Comp_Time > 0) PLANT[segment].XAP_Comp_Time--;
	
	if ((PLANT[segment].XAGC_Unit_Comp == 1) &&
		(PLANT[segment].XAP_DistributeDelay==0))
	{
		CompModeMax = 0;
		AP_Diff_J = 0;
		for (i1=1; i1<=UNIT_NUMB; i1++) 
		{  
			if (UNIT[i1].segment == segment) 
			{
				if ((UNIT[i1].XAPCompMode > 0) &&(UNIT[i1].XAP_Setp_Calc == 1)) 
				{
					if (UNIT[i1].XAPCompMode==3) 
					{/*开停机机组优先级最高*/
						AP_Diff_J = 10000;
						CompModeMax = 3;
					} 
					else 
					{  /*非开停机机组*/
						AP_Step_Dev =(float)fabs(UNIT[i1].AP_Setp - UNIT[i1].YAP_Setp);
						if (AP_Step_Dev > AP_Diff_J) 
						{
							AP_Diff_J = AP_Step_Dev;
							CompModeMax = UNIT[i1].XAPCompMode;
						}
					}
				}
			}
		}/*for (i1=1; i1<=UNIT_NUMB; i1++)结束*/
		
		/** Calculate number of units for compensation function*/
		U_COMP = 0;  /*参与补偿机组台数Number of units available for compensation*/
		U_JUMP = 0;  /*补偿机组台数Number of units starting, stopping, crossing vib. zone*/
		for (i1=1; i1<=UNIT_NUMB; i1++) 
		{
			U_CO_Used[i1] = 0;
			if (UNIT[i1].segment == segment) 
			{
				if (UNIT[i1].AP_Avail== 1)
				{
					if ((UNIT[i1].Status==UNIT_IN_GEN) && (UNIT[i1].XAP_Setp_Calc==1))
					{
						if ((UNIT[i1].XAPCompMode > 0) &&(UNIT[i1].XAPCompMode == CompModeMax))
						{
							U_JUMP++;
							U_JUMP_Mem[U_JUMP] = i1;
							U_JUCO_Mem[U_JUMP] = 0;
						} 
						else 
						{
							U_COMP++;
							U_COMP_Mem[U_COMP] = i1;
							UNIT[i1].XAPCompMode = 0;
						}
					} 
					else 
					{
						UNIT[i1].YAP_Setp = UNIT[i1].AP_Setp;
						UNIT[i1].XAP_Err_Detect.Enabled = 1;
					}
				}
				else 
				{
					UNIT[i1].YAP_Setp = UNIT[i1].AP_Setp;
					UNIT[i1].XAP_Err_Detect.Enabled = 1;
				}
			}
		}/*for (i1=1; i1<=UNIT_NUMB; i1++)结束*/
		
		for (i1=1; i1<=U_JUMP; i1++) 
		{
			UJ = U_JUMP_Mem[i1];
			i2 = 0;
			i3 = 0;
			while ((i2 == 0) && (i3 < U_COMP)) 
			{
				i3++;
				UC = U_COMP_Mem[i3];
				if((UNIT[UJ].XAP_Err_Detect.INC_Dir != UNIT[UC].XAP_Err_Detect.INC_Dir)&&
					(U_CO_Used[UC] == 0))
				{
					i2 = UC;
				}
			}
			
			if (i2 == 0) 
			{
				UNIT[UJ].YAP_Setp = UNIT[UJ].AP_Setp;
				UNIT[UJ].XAP_Err_Detect.Enabled = 1;
				UNIT[UJ].XAP_Setp_Calc = 0;    
				U_JUMP_Mem[i1] = 0;
				U_JUCO_Mem[i1] = 0;
			} 
			else 
			{
				U_JUCO_Mem[i1] = i2;
				U_CO_Used[i2] = 1;
			}
		} /*for (i1=1; i1<=U_JUMP; i1++)结束*/
		
		if ((U_JUMP > 0) && (U_COMP >= U_JUMP)) CompFunc = 1;
	}/*if (OPERATOR[segment].XAGC_Unit_Comp == 1) 结束*/
		
	if (CompFunc == 1) 
	{
		if (PLANT[segment].XAP_Comp_Time == 0) 
		{   
			PLANT[segment].XAP_Comp_Time = APCompStepTime;
			AP_Comp_Step = PLANT[segment].XAP_Comp_Step * (float)0.9;
			
			//*** Compensation Function ********************************************
			
			for (i1=1; i1<=U_JUMP; i1++) 
			{
				UJ = U_JUMP_Mem[i1];
				UC = U_JUCO_Mem[i1];
				if ((UJ > 0) && (UC > 0)) 
				{
					AP_Step1 = APCompStepVal;
					if (UNIT[UJ].XStartUpTime > 0) AP_Step1 = APStopMin;
					AP_MinStep = AP_Step1;
					
					AP_Diff_J =(float)fabs(UNIT[UJ].AP_Setp - UNIT[UJ].YAP_Setp);
					AP_Diff_C = (float)fabs(UNIT[UC].AP_Setp - UNIT[UC].YAP_Setp);
					
					if (AP_Diff_J < AP_Diff_C) 
					{
						if (AP_Diff_J < AP_MinStep) 
						{
							AP_Step1 = AP_Diff_J;
							UNIT[UJ].XAP_Setp_Calc = 0;  
						}
					} 
					else 
					{
						if (AP_Diff_C < AP_MinStep) 
						{
							AP_Step1 = AP_Diff_C;
							UNIT[UC].XAP_Setp_Calc = 0;   
						}
					}
					
					
					/*检查有功补偿模式有功调节失败*/
					if (PLANT[segment].XAP_Comp_Mode==1) 
					{
						AP_Step_Dev =(float)fabs(UNIT[UJ].AP_RtValue - UNIT[UJ].YAP_Setp);
						if ((UNIT[UJ].XAP_Err_Detect.Enabled==0) &&
                            (UNIT[UJ].XPowerUp==0) &&(AP_Step_Dev > AP_Comp_Step))      
						{ 
							printf("****No.%d,%f,%f,%f\n",UJ,UNIT[UJ].AP_RtValue, UNIT[UJ].YAP_Setp,AP_Comp_Step);
							AP_CTRL_Fail = 1;
							UNIT[UJ].AP_Fail = 1;
							UNIT[UJ].XAP_Setp_Calc = 0;
							UNIT[UJ].XAPCompMode = 0;
							UNIT[UJ].XAP_Err_Detect.Moving = 0;
							AGC_UpdateRTDB_IND(DB_NAME_UNIT_AGCSEL[UJ],0);
							AGC_UpdateRTDB_IND(DB_NAME_Unit_AP_Fail[UJ],1);
						}
						
						if (AP_Step_Dev < Unit_AP_MaxDev) 
						{
							UNIT[UJ].XAP_Err_Detect.MoveTime = UnitMoveTime; 
						}
						/*Unit which is compensating new unit*/
						AP_Step_Dev =(float)fabs(UNIT[UC].AP_RtValue - UNIT[UC].YAP_Setp);
						if((UNIT[UC].XAP_Err_Detect.Enabled==0) &&
                            (AP_Step_Dev > AP_Comp_Step))
						{
							AP_CTRL_Fail = 1;
							UNIT[UC].AP_Fail = 1;
							UNIT[UC].XAP_Setp_Calc = 0;
							UNIT[UC].XAPCompMode = 0;
							AGC_UpdateRTDB_IND(DB_NAME_UNIT_AGCSEL[UC],0);
							AGC_UpdateRTDB_IND(DB_NAME_Unit_AP_Fail[UC],1);
							UNIT[i1].AP_FailTime=Time_10s;
						}
					}
					
					if (UNIT[UJ].XPowerUp == 1) 
					{
						UNIT[UJ].XPowerUp = 0;
						UNIT[UJ].YAP_Setp = 0;
						if ((AP_Step1<APStopMin) && (UNIT[UJ].XAP_Err_Detect.INC_Dir==1))
						{
							UNIT[UJ].YAP_Setp = APStopMin - AP_Step1;
						}
					}
					if((UNIT[UJ].StartProcess==1)&&(PLANT[segment].Start_AP_Dev>APFilterDB))
					{
						if (UNIT[UJ].XStartUpTime > 0)
							AP_Step1 = APStopMin;
						else 
							AP_Step1 = PLANT[segment].Start_AP_Dev;
						PLANT[segment].Start_AP_Dev-=AP_Step1;
						Start_Single_Step = 1;
					}
					
					if (AP_CTRL_Fail==0) 
					{
						if(Start_Single_Step==1)
							UNIT[UJ].YAP_Setp += AP_Step1;
						else
						{
							if (UNIT[UJ].XAP_Err_Detect.INC_Dir == 1) 
							{
								UNIT[UJ].YAP_Setp += AP_Step1;
								UNIT[UC].YAP_Setp -= AP_Step1;
							}
							else 
							{
								UNIT[UJ].YAP_Setp -= AP_Step1;
								UNIT[UC].YAP_Setp += AP_Step1;
							}
						} 
					}
					else
					{
						PLANT[segment].XAP_DistributeDelay = Time_15s;
					}
					printf("Cycle=%d,No.%d,StartProcess=%d,YAP_Step_J=%f,YAP_Setp_C=%f,Start_AP_Dev=%f,Start_Single_Step=%d,UJ=%d,UC=%d\n",
						Cycle,UJ,UNIT[UJ].StartProcess,UNIT[UJ].YAP_Setp,UNIT[UC].YAP_Setp,PLANT[segment].Start_AP_Dev,Start_Single_Step,UJ,UC);
					/*Store last compensation step*/
					AP_Comp_Step = AP_Step1;
					if (AP_Comp_Step < Unit_AP_MaxDev) AP_Comp_Step = Unit_AP_MaxDev;
					PLANT[segment].XAP_Comp_Step = AP_Comp_Step;
                    }
                }
            }
        }/*if (CompFunc == 1)结束*/
		else 
		{
            PLANT[segment].XAP_Comp_Step = MaxPower;
            if (PLANT[segment].XAP_DistributeDelay == 0)
            {
                for (i1=1; i1<=UNIT_NUMB; i1++) 
				{
                    if (UNIT[i1].segment == segment) 
                    {
                        if (UNIT[i1].XPowerUp == 1) 
                        {
                            UNIT[i1].XPowerUp = 0;
                            UNIT[i1].XAPCompMode = 0;
                        }
						
                        UNIT[i1].YAP_Setp = UNIT[i1].AP_Setp;
                        UNIT[i1].XAP_Err_Detect.Enabled = 1;
                    }
                }
            }
        }/*else结束*/
        
        PLANT[segment].XAP_Comp_Mode = CompFunc;
		
}

void FillCommandBuf(int segment)
{
    int i,APFlag,Number[UNIT_NUMB+1],num,StartFlag,StopFlag,StartStopNum;
    char	logtxt[200];
	char    Sms[100];
    APFlag=0;
    num=0; 
    StartFlag=0;
    StopFlag=0;
    StartStopNum=0;
#ifdef CASCADE
	if (!PLANT[segment].AllocationToUnit)
	{
		return;
	}
#endif
	
    if(PLANT[segment].Mode==5)
	{
		AGC_Recalc[segment] =0;
		return;
	}
    for (i=1;i<=UNIT_NUMB;i++) 
    {   
        if(UNIT[i].AP_Avail==0)continue;
        if(UNIT[i].segment!=segment)continue;
        
        if((UNIT[i].Start==1)&&(UNIT[i].StartFlag==1)&&(UNIT[i].Status==STAND_BY))
        {
            UNIT[i].StartFlag=0;
            if(AutoStartStop[i]==1)
            {
				SendCommandToPLC(DB_NAME_UNIT_STATUS[i],C_UNIT,0,KJML);   
				printf("No:%d Unit Start!\n",i);    
				StartFlag=1;
				StartStopNum=i;
            }
			else 
			{
				strcpy(Sms,"自动开停机已禁止！");
				if(GetControlState()==1)
					BdSms(Sms);
			}
        }
        if((UNIT[i].Stop==1)&&(UNIT[i].StopFlag==1)&&(UNIT[i].Status==UNIT_IN_GEN))
        {
            UNIT[i].StopFlag=0;
            if(AutoStartStop[i]==1)
            {
				SendCommandToPLC(DB_NAME_UNIT_STATUS[i],C_UNIT,0,TJML);  
				AGC_Sleep(1000);
				SendCommandToPLC(DB_NAME_YG_GD[i],C_APOWER,0,0);         
				AGC_Sleep(1000);
				printf("No:%d Unit Stop!\n",i);
				UNIT[i].XAP_Setp=UNIT[i].YAP_Setp=UNIT[i].ZAP_Setp=0;
				StopFlag=1;
				StartStopNum=i;
            }
			else
			{
				strcpy(Sms,"自动开停机已禁止！");
				if(GetControlState()==1)
					BdSms(Sms);
			}
        } 
		
        AGC_UpdateRTDB_ANA(DB_NAME_UNIT_SETPOINT[i],UNIT[i].YAP_Setp);
		// printf("AGC_Recalc[%d]=%d,YAP_Setp=%f,ZAP_Setp=%f,UNIT[i].Status=%d,UNIT[i].Starting=%d\n",segment,AGC_Recalc[segment],UNIT[i].YAP_Setp,UNIT[i].ZAP_Setp,UNIT[i].Status,UNIT[i].Starting);
        if((fabs(UNIT[i].YAP_Setp-UNIT[i].ZAP_Setp)>=1)&&(UNIT[i].Status==UNIT_IN_GEN)&&
            (UNIT[i].YAP_Setp>10)&&(UNIT[i].Starting==0))
        {    
             //printf("YAP_Setp=%f,ZAP_Setp=%f,PLANT[segment].Closed_Loop=%d\n",UNIT[i].YAP_Setp,UNIT[i].ZAP_Setp,PLANT[segment].Closed_Loop);
            if(( PLANT[segment].Closed_Loop!=OPEN_LOOP)||(PLANT[segment].APAnswerYes==1))
            {   
                num+=1;
                UNIT[i].ZAP_Setp=UNIT[i].YAP_Setp;
                APFlag=1;
                Number[num]=i;
                SendCommandToPLC(DB_NAME_YG_GD[i],C_APOWER,UNIT[i].ZAP_Setp,0);
               // printf("Unit No.%d active Power %f Command Send out!\n",i,UNIT[i].ZAP_Setp);
                if(PLANT[segment].APAnswerYes==1)
                    PLANT[segment].APAnswerYesTime=Time_5s;
            }
        }
    }  
    AGC_Recalc[segment] =0;
    if((StartFlag==1)||(StopFlag==1)||(APFlag==1))
    {
        sprintf(logtxt,"\n%02d月%02d日  %02d:%02d:%02d,segment=%d,AP_Setting=%d,AP_Curve=%d,FR_Regulation=%d,FR_Compensation=%d,Sel_Bus=%d,AP_Setpoint=%f\n",
			ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec,segment,PLANT[segment].AP_Setting,PLANT[segment].AP_Curve,
			PLANT[segment].FR_Regulation,PLANT[segment].FR_Compensation,PLANT[segment].Sel_Bus,
			PLANT[segment].AP_Setpoint);
        WriteLogFile ( logtxt );
    } 
    if(StartFlag==1)
    {
        sprintf(logtxt,"%d月%02d日  %02d:%02d:%02d, No.%d Start Command!\n",ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec,StartStopNum);
        WriteLogFile ( logtxt );
    }
    if(StopFlag==1)
    {
        sprintf(logtxt,"%d月%02d日  %02d:%02d:%02d,No.%d Stop Command!\n",ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec,StartStopNum);
        WriteLogFile ( logtxt );
    }
    
    if(APFlag==1)
    {
        sprintf( logtxt, "%d月%02d日  %02d:%02d:%02d,PLANT[%d].AP_WorkSP=%f,P_Sum=%f\n",
			ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec,segment,PLANT[segment].AP_WorkSP,PLANT[segment].p_sum);
        WriteLogFile ( logtxt );
        for(i=1;i<=num;i++)
        {    
			sprintf(logtxt,"%d月%02d日  %02d:%02d:%02d,No.%d AP_RtValue=%f,AP_Setp is %f!\n",
				ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec,Number[i],UNIT[Number[i]].AP_RtValue,UNIT[Number[i]].ZAP_Setp);
			WriteLogFile ( logtxt );
        }
    }
}
void Compute_Flow_Efficiency(int segment)
{
    int i,FirstUnit,LastUnit;
    float Flow,Efficiency,InputPower,OutPower;
    InputPower=0;
    OutPower=0;
    Flow=0;
 
	FirstUnit=1;
	LastUnit=6;
    
    for(i=FirstUnit;i<=LastUnit;i++)
    {
        if(UNIT[i].Status==UNIT_IN_GEN)
        {   
            Calculate_Unit_Eff_Flow(i, UNIT[i].AP_RtValue);
            //UNIT[i].Efficiency/=100;
            InputPower+=UNIT[i].InputPower;
            OutPower+=UNIT[i].AP_RtValue;
            Flow+=UNIT[i].Flow;
        }
        else
        {
            UNIT[i].Efficiency=0;
            UNIT[i].Flow=0;
        }
        AGC_UpdateRTDB_ANA(DB_NAME_Unit_Efficiency[i],UNIT[i].Efficiency);
    }
    if(InputPower>1)
        Efficiency=OutPower/InputPower*100;
    else 
        Efficiency=0;
    PLANT[segment].Efficiency=Efficiency; 
    PLANT[segment].WaterFlow=Flow;
    AGC_UpdateRTDB_ANA(DB_NAME_Efficiency[segment],Efficiency);
    AGC_UpdateRTDB_ANA(DB_NAME_Flow[segment],Flow);
}

void Quit (int sn)
{
    UnMountDB();
    printf ("AGC exit!(Ctrl_c or kill -2 pid)\n");
    lan_finish(0);
    exit (0);
}
void main(int argc,char *argv[])
{
    int segment; 
	
#ifdef RECORD
	char str[200];
	FILE *fp_Record;	
    char filename[50];
#endif
#ifndef SUN_OS
	WORD	wvr;
	WSADATA	WSAData;  
	wvr=MAKEWORD(1,1);
	
	WSAStartup(wvr, &WSAData);
#endif		
    gethostname(hostname,20);
    printf("hostname =%s\n",hostname);
    
    SetModuleName("agc-xjb");
   // SetErrLogMode(argc,argv);
    CreateLockFile(".agc-xjb.lock");
    signal(SIGINT,Quit);
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
    ReadTime();   
    ReadParameter();
    Load_AGC_Def();
    ReadDefFile();
    AGC_segment();
    Check_Waterhead();  
	
    InitData();
    LogFileChangeDayAndWriteTime();
#ifdef RECORD
	sprintf( filename, "/home/ems/h9000/his/agc/%02d%02dAGCRecord.xls",ntime->tm_mon+1,ntime->tm_mday);
	fp_Record=fopen(filename, "w");
	strcpy(str,"Setpoint\tTotal Power\tDeviation\t");
	fwrite(str,strlen(str),1,fp_Record);
	for(i=1;i<=UNIT_NUMB;i++)
	{
		sprintf(str,"%02dF\t",i);
		fwrite(str,strlen(str),1,fp_Record);
	}
	fclose(fp_Record);
#endif
	
    for (;;)
    {
        ReadTime();
        LogFileChangeDayAndWriteTime();
        
		if(Cycle%5==0)
		{
			AGC_segment();
			Check_Waterhead();  /*根据水头得出机组运行区间*/
		}
		
        for(segment=0;segment<MAXSEGMENT;segment++)
        {   
			if(Cycle%5==0)
				ReadAGC_DataFromRTDB(segment);
			
			PLANT[segment].Freq_Power = Frequency_Control(segment);
			if(Cycle%5==0)
			{
				AGC_Module(segment);
				Unit_Supervision(segment);
				AGC_Distribution(segment);                   
				FillCommandBuf(segment);
				Compute_Flow_Efficiency(segment);
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
        AGC_Sleep(200);
    }
}




