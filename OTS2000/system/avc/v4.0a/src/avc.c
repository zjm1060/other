#ifdef UNIX
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
#include  <math.h>
#else
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#endif

#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"

#include    "../../../cmd/v4.0a/inc/cmd_def.h"
#include    "../../../dms/v4.0a/inc/dms_std.h"
#include    "../../../dms/v4.0a/inc/dms_structure_def.h"
#include    "../../../dms/v4.0a/inc/os_type_def.h"

#include    "../../../avc/v4.0a/inc/avc.h"


/*以下v3.0 Windows*/
#ifdef v3
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#include	"../../../dms/v3.0a/inc/dmsstd.h"
#include	"../../../dms/v3.0a/inc/dmserr.h"
#include	"../../../dms/v3.0a/inc/dmsdef.h"
#include	"../../../dms/v3.0a/inc/dmscom.h"
#include	"../../../dps/v3.0a/inc/bit_proc_def.h"
#include	"../../../dps/v3.0a/inc/data_type_def.h"
#include	"../../../dps/v3.0a/inc/dbwritedata.h"
#include	"../../../dps/v3.0a/inc/ipc_common.h"
#include	"../../../dps/v3.0a/inc/cmd_def.h"
#include   	 "../../../avc/v3.0a/inc/avc.h"

extern 	Lan_init ();
extern 	lan_out ();
extern 	void Lan_finish(int);
extern void SetAnaStatusBits();
extern int GetHost_Cmd();
#endif


int Number_ANA=0;
ANA_MSG	ml_buf_ANA[64];
LAN_HEAD	lanhd_ANA,lanhd_IND;
int Number_IND=0;
SYS_MSG	ml_buf_IND[64];
int ctrl_flag,lock_flag;


extern void CreateLockFile(char *);
extern int LocateKeyword( FILE *, char *);
//extern int BroadCastSms(char* message, BOOL IsAlm);
/*extern  int     InterLockFileLoad();
extern  int     InterLock_Judge(char *pt);
extern  int     InterLockFileFree();
extern short   GetCurWSCtrlPrvg();
extern short   GetCurServCtrlPrvg();
*/

int BroadCastSms(char* message, BOOL IsAlm)
{
	LAN_HEAD	head;
	time_t     	tt_time;
	char 		smsbuf[SMS_MESSAGE_SIZE+6];
	int			send_bytes, length;

	head.length= SMS_MESSAGE_SIZE+6;
	Swap2Byte(&head.length);
	head.dp_type= BJ_DB_TIME;
	head.dest_id= 0;
	if(-1==GetLocalHostId(&head.src_id))
		return -1;
	head.dest_stn= GetModuleId();
	
	if(IsAlm)
		smsbuf[0]= DPS_SM_ALM;
	else
		smsbuf[0]= DPS_SMS;
	smsbuf[1]=head.src_id;

	time(&tt_time);
	Swap4Byte(&tt_time);
	memcpy(&smsbuf[SMS_MESSAGE_SIZE+2], &tt_time, 4);	

	length= strlen(message);
	for(send_bytes=0; send_bytes<length; send_bytes += SMS_MESSAGE_SIZE)
	{
		strncpy(&smsbuf[2], &message[send_bytes], SMS_MESSAGE_SIZE);
		lan_out((char *)smsbuf, head, 0);
	}
	return 0;
}

void UnMount_handle()
{
	 lan_finish(0);
	 printf ("AVC exit!(Ctrl_c or kill -2 pid)\n");
	 UnMountDB();
	 exit(0);	 
}

void SIGINT_handler (int signal_handler)
{
#ifdef DEBUG
	 printf ( "\nSIGVAL=%d", signal_handler);
#endif
	 lan_finish(0);
	 UnMountDB();
	 exit (0 );
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
	
    fp = fopen(logfilename, "a");
    if ( fp != NULL)
    {
        fwrite( logtxt, strlen( logtxt ),1,fp);
        fclose(fp);
     }
	fclose(fp);
}

/*以下为h9000 v4.0 读写下令部分*/
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
	int stat;
	char err_str[80]="";
	DMS_COMMON common_aa;
	if(ReadEntryByNameStr(PointName,&common_aa)==-1)
	{
		sprintf(err_str,"No this point %s\n",PointName);
		Err_log(err_str);
		return(DATA_ERR);
	}
    /*if(common_aa.var.anlg.value.status.chan_fail==1)
	return(DATA_BAD);
    stat= common_aa.var.anlg.value.status.lmt_state;
    if (stat==ANA_ALM_HI_ALARM||stat==ANA_ALM_HI_WARN)
      return(OVER_HIGH_LMT);*/
    ReturnValue=common_aa.var.anlg.fValue;
    return(ReturnValue);
}

int ReadDB_ANA_Chan(char *PointName) //-1数据库中无此点，0通道正常，1通道故障
{
	DMS_COMMON common_aa;
	int ReturnValue;
	char err_str[80]="";
    if(ReadEntryByNameStr(PointName,&common_aa)==-1)
    {
	 sprintf(err_str,"No this point %s\n",PointName);
	 Err_log(err_str);
	 return(DATA_ERR);
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
          return(DATA_ERR);
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
	if(GetControlState()!=1) return 0;
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

	if(Number_ANA>=0)
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
	if(GetControlState()!=1) return 0;
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
	
	if(Number_IND>=0)
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
	if(GetControlState()!=1) return 0;
	ml_buf.type_id =Type_id;
	ml_buf.stn_id =common_aa.point.stn_id;
	ml_buf.dev_id =common_aa.point.dev_id;
	ml_buf.data_type=common_aa.point.data_type;
	ml_buf.point_id = common_aa.point.pt_id;

	ml_buf.status.opr= Status;
	ml_buf.status.state=0;
	ml_buf.status.src=2;

	ml_buf.data.iValue=data*1000;
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
	lanhd.packet_num =0;
	
	if(GetControlState()==1)
		lan_out ((char *)&ml_buf,lanhd,(unsigned char)0);
	printf("Send Command !!!!!!\n");
	return 0;
}

/*int	LocateKeyword( FILE *fp, char *keyword )
{
	char	str[256];
	char	key[64];

	sprintf(key, "[%s]", keyword);
	rewind( fp );
	while( NULL != fgets(str, sizeof(str), fp) )
	{
		if( strstr(str, key) )
			return 0;
	}
	printf("Cannot find the keyword %s!",keyword);
	return -1;
}
*/

void ReadCurveDefFile()
{
	FILE	*fp;
#ifdef SUN_OS
	char filename[100]="/home/ems/h9000/def/AVC_Curve.DEF";
#else
	char filename[100]="/home/ems/h9000/def/AVC_Curve.DEF";
#endif
	char keyword[30]="";
	char StrTemp[50]="";
	int i,tmp;

	if( (fp=fopen(filename, "r"))==NULL )
	{
		printf("Fail to open %s\n", filename);
		UnMount_handle();
	}

/*	strcpy(keyword,"TodayCurve");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();*/
	fscanf(fp,"%s",StrTemp);
	for(i=1;i<SCHEDULE+1;i++)
	{
		fscanf(fp,"%d %f %f %s %s",&tmp,&Curve_time_begin[i],&Curve_time_end[i],DB_NAME_Curve_High[i],DB_NAME_Curve_Low[i]);
#ifdef DEBUG
		printf("%d\t%f\t%f\t%s\t%s\n",i,Curve_time_begin[i],Curve_time_end[i],DB_NAME_Curve_High[i],DB_NAME_Curve_Low[i]);
#endif
		tmp = ReadDB_ANA_Chan ( DB_NAME_Curve_High[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_Curve_High[i]);
			UnMount_handle();
		}
		tmp = ReadDB_ANA_Chan ( DB_NAME_Curve_Low[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_Curve_Low[i]);
			UnMount_handle();
		}
	}
	fclose(fp);	
}

void ReadAVCDefFile()
{
	FILE	*fp;
#ifdef SUN_OS
	char filename[100]="/home/ems/h9000/def/AVC.DEF";
#else
	char filename[100]="/home/ems/h9000/def/AVC.DEF";
#endif
	char keyword[30]="";
	char StrTemp[50]="",StrTemp1[50]="";
	int i,j;
	int data_type,tmp;

	if( (fp=fopen(filename, "r"))==NULL )
	{
		printf("Fail to open %s\n", filename);
		UnMount_handle();
	}

    strcpy(keyword,"UnitNumber");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",StrTemp);
    UNIT_NUMB=atoi(StrTemp);
	if((UNIT_NUMB>MAX_UNIT_NUMB)||(UNIT_NUMB<=0))
	{
		printf("机组台数定义错误!\n");
		UnMount_handle();
	}
#ifdef DEBUG
	printf("UNIT_NUMB=%d\n",UNIT_NUMB);
#endif   

	strcpy(keyword,"DB_RP_WorkSP");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",StrTemp);
    DB_RP_WorkSP=atof(StrTemp);
#ifdef DEBUG
	printf("DB_RP_WorkSP=%d\n",DB_RP_WorkSP);
#endif

	

    strcpy(keyword,"Voltage_lmt");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s %s\n",StrTemp,StrTemp1);
    Voltage_Max=atoi(StrTemp);
	Voltage_Min=atoi(StrTemp1);
	if(Voltage_Max<Voltage_Min)
	{
		printf("电压限值定义错误!\n");
		UnMount_handle();
	}
#ifdef DEBUG
	printf("Voltage_Max=%d,Voltage_Min=%d\n",Voltage_Max,Voltage_Min);
#endif

	strcpy(keyword,"RP_Step");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s",StrTemp);
		RP_Step[i]=atoi(StrTemp);
#ifdef DEBUG
	printf("RP_Step[%d]=%d\n",i,RP_Step[i]);
#endif
	}
/*
	strcpy(keyword,"DQV");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",StrTemp);
    PLANT.dqv=atoi(StrTemp);
	if((PLANT.dqv>100))
	{
		printf("电压调差系数定义太大\n");
		UnMount_handle();
	}
#ifdef DEBUG
	printf("dqv=%d\n",PLANT.dqv);
#endif 
*/
	strcpy(keyword,"PQ_Enable");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",StrTemp);
    PQ_Enable=atoi(StrTemp);
	if((PQ_Enable!=0)&&(PQ_Enable!=1))
	{
		printf("PQ_Enable定义错误!只能为0或1\n");
		UnMount_handle();
	}
#ifdef DEBUG
	printf("PQ_Enable=%d\n",PQ_Enable);
#endif 

	if(PQ_Enable==1)
	{
		strcpy(keyword,"TypeNumber");
		if(-1==LocateKeyword(fp,keyword))
			UnMount_handle();
		fscanf(fp,"%s\n",StrTemp);
		TypeNumber=atoi(StrTemp);
		if((TypeNumber>UNIT_NUMB)||(TypeNumber<0))
		{
			printf("TypeNumber定义错误!超出正常访问");
			UnMount_handle();
		}
#ifdef DEBUG
	printf("PQ_Enable=%d\n",PQ_Enable);
#endif
	
		strcpy(keyword,"Type_UnitNo");
		if(-1==LocateKeyword(fp,keyword))
			UnMount_handle();
		for(i=1;i<=TypeNumber;i++)
		{
			fscanf(fp,"%s",StrTemp);
			Type_UnitNo[i][0]=atoi(StrTemp);
			for(j=1;j<=Type_UnitNo[i][0];j++)
			{
				fscanf(fp,"%s",StrTemp);
				Type_UnitNo[i][j]=atoi(StrTemp);
					UNIT[Type_UnitNo[i][j]].Type=i;
#ifdef DEBUG
	printf("Type No%d: Unit %d\n",i,Type_UnitNo[i][j]);
#endif
			}
		}

		strcpy(keyword,"TypeKeyword");
		if(-1==LocateKeyword(fp,keyword))
			UnMount_handle();
		for(i=1;i<=TypeNumber;i++)
		{
			fscanf(fp,"%s\n",TypeKeyword[i]);
#ifdef DEBUG
	printf("TypeKeyword=%s\n",TypeKeyword[i]);
#endif
		}

		strcpy(keyword,"ItemNum");
		if(-1==LocateKeyword(fp,keyword))
			UnMount_handle();
		fscanf(fp,"%s\n",StrTemp);
		ItemNum=atoi(StrTemp);
#ifdef DEBUG
	printf("ItemNum=%d\n",ItemNum);
#endif
	}

	strcpy(keyword,"Rt_Status");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_Rt_Status[i]);		
		tmp = ReadDB_ANA_Chan ( DB_NAME_Rt_Status[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_Rt_Status[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_Rt_Status[%d]=%s\n",i,DB_NAME_Rt_Status[i]);
#endif
	}   

	strcpy(keyword,"QC_AVC");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_QC_AVC);
	tmp = ReadDB_ANA_Chan ( DB_NAME_QC_AVC );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_QC_AVC);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_QC_AVC=%s\n",DB_NAME_QC_AVC);
#endif 

	strcpy(keyword,"AVC_KBH");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_AVC_KBH);
	tmp = ReadDB_ANA_Chan ( DB_NAME_AVC_KBH );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_AVC_KBH);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_AVC_KBH=%s\n",DB_NAME_AVC_KBH);
#endif 

	strcpy(keyword,"Run_State");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_Run_State);	
	tmp = ReadDB_ANA_Chan ( DB_NAME_Run_State );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_Run_State);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_Run_State=%s\n",DB_NAME_Run_State);
#endif 

	strcpy(keyword,"DY_GD");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_DY_GD);
	tmp = ReadDB_ANA_Chan ( DB_NAME_DY_GD );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_DY_GD);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_DY_GD=%s\n",DB_NAME_DY_GD);
#endif 

	strcpy(keyword,"DYQX_GD");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_DYQX_GD);
	tmp = ReadDB_ANA_Chan ( DB_NAME_DYQX_GD );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_DYQX_GD);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_DYQX_GD=%s\n",DB_NAME_DYQX_GD);
#endif

	strcpy(keyword,"WGGD");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_WGGD);
	tmp = ReadDB_ANA_Chan ( DB_NAME_WGGD );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_WGGD);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_DYQX_GD=%s\n",DB_NAME_WGGD);
#endif

	strcpy(keyword,"AVG_ALG");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_AVC_AVGALG);
	tmp = ReadDB_ANA_Chan ( DB_NAME_AVC_AVGALG );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_AVC_AVGALG);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_AVC_AVGALG=%s\n",DB_NAME_AVC_AVGALG);
#endif

	strcpy(keyword,"EQUCOS_ALG");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_AVC_EQUCOSALG);
	tmp = ReadDB_ANA_Chan ( DB_NAME_AVC_EQUCOSALG );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_AVC_EQUCOSALG);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_AVC_EQUCOSALG=%s\n",DB_NAME_AVC_EQUCOSALG);
#endif

	strcpy(keyword,"Control_Right");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_Control_Right);
	tmp = ReadDB_ANA_Chan ( DB_NAME_Control_Right );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_Control_Right);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_Control_Right=%s\n",DB_NAME_Control_Right);
#endif

        strcpy(keyword,"SD_Right_ON");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_SD_Right_ON);
	tmp = ReadDB_ANA_Chan ( DB_NAME_SD_Right_ON );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_SD_Right_ON);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_SD_Right_ON=%s\n",DB_NAME_SD_Right_ON);
#endif
        
 /*   strcpy(keyword,"SD_Right_OFF");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_SD_Right_OFF);
	tmp = ReadDB_ANA_Chan ( DB_NAME_SD_Right_OFF );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_SD_Right_OFF);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_SD_Right_OFF=%s\n",DB_NAME_SD_Right_OFF);
#endif

    strcpy(keyword,"ON_SD_CTRL");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_ON_SD_CTRL);
	tmp = ReadDB_ANA_Chan ( DB_NAME_ON_SD_CTRL );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_ON_SD_CTRL);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_ON_SD_CTRL=%s\n",DB_NAME_ON_SD_CTRL);
#endif
*/	
	strcpy(keyword,"VO_Setpoint");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_VO_Setpoint);
	tmp = ReadDB_ANA_Chan ( DB_NAME_VO_Setpoint );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_VO_Setpoint);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_VO_Setpoint=%s\n",DB_NAME_VO_Setpoint);
#endif 

	strcpy(keyword,"VO_Remote_Setpoint");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_VO_Remote_Setpoint);	
	tmp = ReadDB_ANA_Chan ( DB_NAME_VO_Remote_Setpoint );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_VO_Remote_Setpoint);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_VO_Remote_Setpoint=%s\n",DB_NAME_VO_Remote_Setpoint);
#endif

	strcpy(keyword,"DQV");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_DQV);	
	tmp = ReadDB_ANA_Chan ( DB_NAME_DQV );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_DQV);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_DQV=%s\n",DB_NAME_DQV);
#endif

	strcpy(keyword,"MaxAdj_V");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_V_MAX_CHANGE);	
	tmp = ReadDB_ANA_Chan ( DB_NAME_V_MAX_CHANGE );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_V_MAX_CHANGE);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_DQV=%s\n",DB_NAME_V_MAX_CHANGE);
#endif

	strcpy(keyword,"RP_Setpoint");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_RP_Setpoint);
	tmp = ReadDB_ANA_Chan ( DB_NAME_RP_Setpoint );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_RP_Setpoint);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_RP_Setpoint=%s\n",DB_NAME_RP_Setpoint);
#endif

	strcpy(keyword,"RP_Remote_Set");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_RP_Remote_Set);
	tmp = ReadDB_ANA_Chan ( DB_NAME_RP_Remote_Set );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_RP_Remote_Set);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_RP_Remote_Set=%s\n",DB_NAME_RP_Remote_Set);
#endif

	strcpy(keyword,"VO_DeadBand");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_VO_DeadBand);
	tmp = ReadDB_ANA_Chan ( DB_NAME_VO_DeadBand );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_VO_DeadBand);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_VO_DeadBand=%s\n",DB_NAME_VO_DeadBand);
#endif 

	strcpy(keyword,"VO_RtValue");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=6;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_VO_RtValue[i]);		
		tmp = ReadDB_ANA_Chan ( DB_NAME_VO_RtValue[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_VO_RtValue[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_VO_RtValue[%d]=%s\n",i,DB_NAME_VO_RtValue[i]);
#endif
	}   

	strcpy(keyword,"AP_RtValue");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AP_RtValue[i]);
		tmp = ReadDB_ANA_Chan ( DB_NAME_AP_RtValue[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_AP_RtValue[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_AP_RtValue[%d]=%s\n",i,DB_NAME_AP_RtValue[i]);
#endif
	}   

	strcpy(keyword,"RP_RtValue");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_RtValue[i]);
		tmp = ReadDB_ANA_Chan ( DB_NAME_RP_RtValue[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_RP_RtValue[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_RP_RtValue[%d]=%s\n",i,DB_NAME_RP_RtValue[i]);
#endif
	} 

	strcpy(keyword,"RP_OptiValue");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_OptiValue[i]);
		tmp = ReadDB_ANA_Chan ( DB_NAME_RP_OptiValue[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_RP_OptiValue[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_RP_OptiValue[%d]=%s\n",i,DB_NAME_RP_OptiValue[i]);
#endif
	} 
	
	strcpy(keyword,"RP_SetValue");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_SetValue[i]);
		tmp = ReadDB_ANA_Chan ( DB_NAME_RP_SetValue[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_RP_SetValue[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_RP_SetValue[%d]=%s\n",i,DB_NAME_RP_SetValue[i]);
#endif
	} 


	strcpy(keyword,"RP_CZ");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_CZ[i]);
		tmp = ReadDB_ANA_Chan ( DB_NAME_RP_CZ[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_RP_CZ[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_RP_CZ[%d]=%s\n",i,DB_NAME_RP_CZ[i]);
#endif
	} 

	strcpy(keyword,"RP_CZKT");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_CZKT[i]);		
		tmp = ReadDB_ANA_Chan ( DB_NAME_RP_CZKT[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_RP_CZKT[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_RP_CZKT[%d]=%s\n",i,DB_NAME_RP_CZKT[i]);
#endif
	} 

	strcpy(keyword,"RP_LMT");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_LMT[i]);		
		tmp = ReadDB_ANA_Chan ( DB_NAME_RP_LMT[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_RP_LMT[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_RP_LMT[%d]=%s\n",i,DB_NAME_RP_LMT[i]);
#endif
	} 

	strcpy(keyword,"RP_LLMT");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_RP_LLMT[i]);		
		tmp = ReadDB_ANA_Chan ( DB_NAME_RP_LLMT[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_RP_LLMT[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_RP_LLMT[%d]=%s\n",i,DB_NAME_RP_LLMT[i]);
#endif
	} 

	strcpy(keyword,"WG_UP_LOCK");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_WG_UP_LOCK);
	tmp = ReadDB_ANA_Chan ( DB_NAME_WG_UP_LOCK );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_WG_UP_LOCK);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_WG_UP_LOCK=%s\n",DB_NAME_WG_UP_LOCK);
#endif 

	strcpy(keyword,"WG_DN_LOCK");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	fscanf(fp,"%s\n",DB_NAME_WG_DN_LOCK);
	tmp = ReadDB_ANA_Chan ( DB_NAME_WG_DN_LOCK );
	if ( tmp == DATA_ERR )
	{
		printf ( "AVC: %s is not found in Database!\n", DB_NAME_WG_DN_LOCK);
		UnMount_handle();
	}
#ifdef DEBUG
	printf("DB_NAME_WG_DN_LOCK=%s\n",DB_NAME_WG_DN_LOCK);
#endif 

	strcpy(keyword,"Unit_Uab_Max");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AVC_UMAX[i]);		
		tmp = ReadDB_ANA_Chan ( DB_NAME_AVC_UMAX[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_AVC_UMAX[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_AVC_UMAX[%d]=%s\n",i,DB_NAME_AVC_UMAX[i]);
#endif
	} 

	strcpy(keyword,"Unit_Uab_Min");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AVC_UMIN[i]);		
		tmp = ReadDB_ANA_Chan ( DB_NAME_AVC_UMIN[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_AVC_UMIN[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_AVC_UMIN[%d]=%s\n",i,DB_NAME_AVC_UMIN[i]);
#endif
	} 

	strcpy(keyword,"Unit_Uab");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AVC_UAB[i]);		
		tmp = ReadDB_ANA_Chan ( DB_NAME_AVC_UAB[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_AVC_UAB[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_AVC_UAB[%d]=%s\n",i,DB_NAME_AVC_UAB[i]);
#endif
	} 

	strcpy(keyword,"Unit_I_Max");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AVC_IMAX[i]);		
		tmp = ReadDB_ANA_Chan ( DB_NAME_AVC_IMAX[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_AVC_IMAX[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_AVC_IMAX[%d]=%s\n",i,DB_NAME_AVC_IMAX[i]);
#endif
	} 

	strcpy(keyword,"Unit_I");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AVC_I[i]);		
		tmp = ReadDB_ANA_Chan ( DB_NAME_AVC_I[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_AVC_I[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_AVC_I[%d]=%s\n",i,DB_NAME_AVC_I[i]);
#endif
	} 

	strcpy(keyword,"Unit_LCI_Max");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_LC_I_MAX[i]);		
		tmp = ReadDB_ANA_Chan ( DB_NAME_LC_I_MAX[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_LC_I_MAX[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_LC_I_MAX[%d]=%s\n",i,DB_NAME_LC_I_MAX[i]);
#endif
	} 
	
	strcpy(keyword,"Unit_LCI");
	if(-1==LocateKeyword(fp,keyword))
		UnMount_handle();
	for(i=1;i<=UNIT_NUMB;i++)
	{
		fscanf(fp,"%s\n",DB_NAME_AVC_LCI[i]);		
		tmp = ReadDB_ANA_Chan ( DB_NAME_AVC_LCI[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_AVC_LCI[i]);
			UnMount_handle();
		}
#ifdef DEBUG
	printf("DB_NAME_AVC_LCI[%d]=%s\n",i,DB_NAME_AVC_LCI[i]);
#endif
	} 

	fclose(fp);

}

void ReadPQDefFile(void)
{
    FILE	*fp;
#ifdef SUN_OS
	char filename[100]="/home/ems/h9000/def/PQ_DEF.DEF";
#else
	char filename[100]="/home/ems/h9000/def/PQ_DEF.DEF";
#endif
	char keyword[30]="";
	char strTemp1[50]="",strTemp2[50]="",strTemp3[50]="",strTemp4[50]="";
	int i,j;

	if( (fp=fopen(filename, "r"))==NULL )
	{
		printf("Fail to open %s\n", filename);
		UnMount_handle();
	}
 
	for(i=1;i<=TypeNumber;i++)
	{
		strcpy(keyword,TypeKeyword[i]);
		if(-1==LocateKeyword(fp,keyword))
			UnMount_handle();
        fscanf(fp,"%s %s %s ",strTemp1,strTemp2,strTemp3);  
        if((strcmp(strTemp1,"APP_POWER")==0)&&(strcmp(strTemp2,"=")==0))
            AP_Power[i]=(float)atof(strTemp3);
	 	
        fscanf(fp,"%s %s %s %s ",strTemp1,strTemp2,strTemp3,strTemp4); 
        if(strcmp(strTemp1,"ROW_POS")!=0||strcmp(strTemp2,"AP_FACT")!=0||strcmp(strTemp3,"RP_OE_FACT")!=0||strcmp(strTemp4,"RP_UE_FACT")!=0)
        {
            printf("Read %s error!\n",filename);
            UnMount_handle();
        }
			 
        for(j=1;j<=ItemNum;j++)
        {
            fscanf(fp,"%s %s %s %s ",strTemp1,strTemp2,strTemp3,strTemp4);
            PQCurve[i][j][1]=(float)atof(strTemp2);
            PQCurve[i][j][2]=(float)atof(strTemp3);
            PQCurve[i][j][3]=(float)atof(strTemp4);
#ifdef DEBUG
     printf("%d,%f,%f,%f\n",j,PQCurve[i][j][1],PQCurve[i][j][2],PQCurve[i][j][3]);
#endif
        }	 
    }  
	fclose(fp);
}

float Unit_OELimit(int UnitNo, float ActPower)
{
    int i1,i2,UType;
    float f1,f2,f3;
    float AP_Fact,Interpol_fact;
    float RP_MaxFact,RP_MaxLim;

    RP_MaxLim = 0;
    UType = UNIT[UnitNo].Type;

    if (AP_Power[UType] < 0) {printf("Type %d S_Total error!\n",UType);exit(0);}
    AP_Fact = ActPower / AP_Power[UType];
    if (AP_Fact>1) AP_Fact = 1;
    if (AP_Fact<0) AP_Fact = 0;

    i1 = 1;
    i2 = 0;
     while ((i1<ItemNum) && (i2==0)) {
        f1 = PQCurve[UType][i1][1];
		f2 = PQCurve[UType][i1+1][1];
        if ((AP_Fact<=f1) && (AP_Fact>=f2)) {
          i2 = i1;
          f3 = f2 - f1;
        }
        i1++;
      }

      if ((i2>0) && (f3<0)) { 
      Interpol_fact = (AP_Fact-f1) / f3;
      f1 = PQCurve[UType][i2][2];
      f2 = PQCurve[UType][i2+1][2];
      RP_MaxFact = f1 + ((f2 - f1) * Interpol_fact); 
      RP_MaxLim = RP_MaxFact * AP_Power[UType];
    }
    return(RP_MaxLim); 
}

float Unit_UELimit(int UnitNo, float ActPower)
{
	int i1,i2,UType;
    float f1,f2,f3;
    float AP_Fact,Interpol_fact;
    float RP_MaxFact,RP_MinLim;

    RP_MinLim = 0;
    UType = UNIT[UnitNo].Type;

	if (AP_Power[UType] < 0) {printf("Type %d S_Total error!\n",UType);exit(0);}
    AP_Fact = ActPower / AP_Power[UType];
    if (AP_Fact>1) AP_Fact = 1;
    if (AP_Fact<0) AP_Fact = 0;

	i1 = 1;
    i2 = 0;
     while ((i1<ItemNum) && (i2==0)) {
        f1 = PQCurve[UType][i1][1];
		f2 = PQCurve[UType][i1+1][1];
        if ((AP_Fact<=f1) && (AP_Fact>=f2)) {
          i2 = i1;
          f3 = f2 - f1;
		  break;
        }
        i1++;
      }

	 if ((i2>0) && (f3<0)) { 
      Interpol_fact = (AP_Fact-f1) / f3;
      f1 = PQCurve[UType][i2][3];
		f2 = PQCurve[UType][i2+1][3];
      
      RP_MaxFact = f1 + ((f2 - f1) * Interpol_fact); 
      RP_MinLim = RP_MaxFact * AP_Power[UType];
    }
    return(RP_MinLim); 
}

void InitData()
{
	int i;
    for(i=1;i<=UNIT_NUMB;i++)
    {
        UNIT[i].RP_RtValue=ReadDB_ANA(DB_NAME_RP_RtValue[i]);
        UNIT[i].XRP_Setp=UNIT[i].RP_RtValue;
        UNIT[i].ZRP_Setp=UNIT[i].YRP_Setp=UNIT[i].RP_Setp=UNIT[i].RP_RtValue;
		//removed by yyp 2012.12.04
		//UNIT[i].Privilege=(int)ReadDB_ANA(DB_NAME_Prvilege[i]);
		PLANT.FirstRun=1;
    }
    ctrl_flag=0;
	lock_flag=0;
    AVC_UpdateRTDB_ANA (DB_NAME_VO_Setpoint,PLANT.VO_RtValue); 
    AVC_UpdateRTDB_ANA (DB_NAME_VO_Remote_Setpoint,PLANT.VO_RtValue);
	AVC_UpdateRTDB_IND (DB_NAME_QC_AVC,NOT);
    AVC_UpdateRTDB_IND (DB_NAME_Control_Right,NOT);
    AVC_UpdateRTDB_IND (DB_NAME_AVC_KBH,NOT);
}

void InitialAVC_Data_On_Screen ()
{
	int	i,data_type,tmp;

	for ( i= 1; i<= UNIT_NUMB; i++ )  
	{
		//removed by yyp 2012.12.04
		//UNIT[i].Privilege=(int)ReadDB_ANA(DB_NAME_Prvilege[i]);
		tmp = ReadDB_ANA_Chan ( DB_NAME_RP_RtValue[i] );
		if ( tmp == DATA_ERR )
		{
			printf ( "AVC: %s is not found in Database!\n", DB_NAME_RP_RtValue[i]);
		}
		AVC_UpdateRTDB_ANA (DB_NAME_RP_OptiValue[i],UNIT[i].RP_RtValue); 
	}
}

void   LogFileChangeDayAndWriteTime()
{
	char   logfilename[50];
	FILE *fp;

	if( oldday != ntime->tm_mday )
	{
		oldday = ntime->tm_mday;
		sprintf(logfilename, "/home/ems/h9000/his/avc/v4.0a/log/AVCcmd.%02d",ntime->tm_mday);
	    fp=fopen(logfilename, "w");
	    fclose(fp);
	}
}

/*
void MoveVol_Curve_At_Zero ()
{
	int     i, hour, host_assist;
	float p_tomorrow;
	int VoltageValue;
	short rt_status;

	ReadTime ();
	hour = ntime->tm_hour;
	if( hour > 0 ) move_curve = NOT;

	host_assist = GetControlState();       
	if(( hour == 0 )&&( move_curve == NOT )&&( GetControlState()==1))
	{
		move_curve = YES;
		for(i=1;i<=SCHEDULE;i++)
		{
			p_tomorrow = ReadDB_ANA (DB_NAME_Tomorrow_Curve[i]);
			AVC_UpdateRTDB_ANA (DB_NAME_Today_Curve[i] ,p_tomorrow);
		}
	}
}*/

float GetCurveValue()
{
	float value;
	float lmt_h,lmt_l,f_time;
	int i;
	ReadTime ();
	f_time=(float)(ntime->tm_hour)+(float)(ntime->tm_min/60.0);
	for(i=1;i<=SCHEDULE;i++)
	{
		if((f_time>Curve_time_begin[i])&&(f_time<=Curve_time_end[i]))
		{
			lmt_h=ReadDB_ANA(DB_NAME_Curve_High[i]);
			lmt_l=ReadDB_ANA(DB_NAME_Curve_Low[i]);
			value=(lmt_h+lmt_l)/2;
		}
	}
	return(value);
}

void Statistics_Unit()	
{
    int i,sbo_flag,rt_status;
//	char	sbo_db_name[80];

    PLANT.P_Sum_AVC=0;
    PLANT.RP_Sum=0;
    PLANT.RP_Sum_AVC=0;
    PLANT.RP_Sum_NotAVC=0;
    PLANT.Num_RP_Avail=0;
    PLANT.GenNumb=0;
    PLANT.P_Sum=0;
	
    for (i=1; i<=UNIT_NUMB;i++)
    {
/*		strcpy ( sbo_db_name, DB_NAME_RP_CZKT[ i ] );
		strcat ( sbo_db_name, "_YES" );
		sbo_flag = InterLock_Judge ( sbo_db_name );*/
		sbo_flag = ReadDB_IND(DB_NAME_RP_CZKT[i]);
		if((sbo_flag==1)&&(UNIT[i].Status==UNIT_IN_GEN_AGC))
			UNIT[i].RP_Avail=1;
		else
			UNIT[i].RP_Avail=0;

/*		strcpy ( sbo_db_name, DB_NAME_RP_CZKT[ i ] );
		strcat ( sbo_db_name, "_NO" );
		sbo_flag = InterLock_Judge ( sbo_db_name );
		if(sbo_flag==1)
		{
			UNIT[i].RP_Avail=0;			
			AVC_UpdateRTDB_IND (DB_NAME_RP_CZ[i] ,NOT);
		}
*/
		UNIT[i].Last_RP_Avail=UNIT[i].RP_Avail;
        
		if(UNIT[i].Status==UNIT_IN_GEN_AGC)
        {
            PLANT.GenNumb+=1;
            PLANT.RP_Sum+=UNIT[i].RP_RtValue;
            PLANT.P_Sum+=UNIT[i].AP_RtValue; 
        }
       
        if(UNIT[i].RP_Avail==1 )
        {
            UNIT[i].RP_Avail=1;
            PLANT.RP_Sum_AVC+=UNIT[i].RP_RtValue;
				PLANT.P_Sum_AVC+=UNIT[i].AP_RtValue;
            PLANT.Num_RP_Avail+=1;
        }
        else
            PLANT.RP_Sum_NotAVC+=UNIT[i].RP_RtValue; 
		
        AVC_UpdateRTDB_IND (DB_NAME_RP_CZKT[i] ,UNIT[i].RP_Avail);
        if(UNIT[i].RP_Avail==0)
		{
			AVC_UpdateRTDB_ANA (DB_NAME_RP_OptiValue[i] ,UNIT[i].RP_RtValue);
		}

/*		strcpy ( sbo_db_name, DB_NAME_RP_RtValue[ i ] );
		strcat ( sbo_db_name, "_Increase" );
		UNIT[i].Increase_Prohibit = InterLock_Judge ( sbo_db_name );
		if(UNIT[i].Increase_Prohibit==1)
			printf("No.%d机增无功闭锁\n",i);

		strcpy ( sbo_db_name, DB_NAME_RP_RtValue[ i ] );
		strcat ( sbo_db_name, "_Decrease" );
		UNIT[i].Decrease_Prohibit = InterLock_Judge ( sbo_db_name );
		if(UNIT[i].Decrease_Prohibit==1)
			printf("No.%d机减无功闭锁\n",i);*/

		if((UNIT[i].RP_Avail==1)&&(UNIT[i].RP_RtValue==DATA_ERR))
		{
			PLANT.RP_MODE=0;
			PLANT.Mode=5;
			AVC_UpdateRTDB_IND (DB_NAME_QC_AVC,NOT);
		}

    }
	if(PLANT.Num_RP_Avail==0)
		printf("无机组成组可调！\n");
}

void CheckVoRTValue()
{
	int i,tmp;
	float vo_err[7],VO_tmp;
	for(i=1;i<=6;i++)   //母线电压的个数为3个Uab、Ubc、Uca
	{
		vo_err[i]=0;
		VO_tmp=ReadDB_ANA(DB_NAME_VO_RtValue[i]);
		tmp=ReadDB_ANA_Chan(DB_NAME_VO_RtValue[i]);
		if((VO_tmp<Voltage_Min)||(VO_tmp>Voltage_Max)||(tmp == 1))
			vo_err[i]=1;
	}
	if(((vo_err[1] == 1)&&(vo_err[2] == 1)&&(vo_err[3] == 1))
		||((vo_err[4] == 1)&&(vo_err[5] == 1)&&(vo_err[6] == 1)) ) //母线电压通道故障，AVC退出
	{
		PLANT.RP_MODE=0;
		PLANT.Mode=5;
		AVC_UpdateRTDB_IND (DB_NAME_QC_AVC,NOT);
	}
}

void ReadAVC_DataFromRTDB()
{
	int i,tmp;
    int ctrl_right,ctrl_sdtr,ctrl_sdtc;
	float RP_Min,RP_Max;

//	MoveVol_Curve_At_Zero();
	PLANT.RP_MODE=ReadDB_IND(DB_NAME_QC_AVC);
	PLANT.Close_Loop=ReadDB_IND(DB_NAME_AVC_KBH);	
	PLANT.RP_Setting=ReadDB_IND(DB_NAME_WGGD);
	PLANT.VO_Setting=ReadDB_IND(DB_NAME_DY_GD);
	PLANT.VO_Curve=ReadDB_IND(DB_NAME_DYQX_GD);
	PLANT.Control_Right=ReadDB_IND(DB_NAME_Control_Right);  //除湖南以外直接读控制点控制权即可
	MaxAdj_V=ReadDB_ANA(DB_NAME_V_MAX_CHANGE);
	PLANT.ALG_AVG=ReadDB_IND(DB_NAME_AVC_AVGALG);	//瀑布沟默认采取平均分配方式，0为平均分配，1为等功率因数分配
	PLANT.ALG_EQUCOS=ReadDB_IND(DB_NAME_AVC_EQUCOSALG); 
/*以下控制权为湖南省调特殊*/   
/*	ctrl_right=ReadDB_IND(DB_NAME_Control_Right);
	ctrl_sdtr=ReadDB_IND(DB_NAME_SD_Right_ON);
	ctrl_sdtc=ReadDB_IND(DB_NAME_SD_Right_OFF);
	printf("ctrl_right=%d,ctrl_sdtr=%d,ctrl_sdtc=%d\n",ctrl_right,ctrl_sdtr,ctrl_sdtc);
	if((ctrl_sdtr==1)&&(ctrl_sdtc==1))
	{
	    AVC_UpdateRTDB_IND (DB_NAME_Control_Right,NOT);
	    ctrl_flag=0;
	    ctrl_right=ctrl_sdtr=ctrl_sdtc=0;
	    AVC_UpdateRTDB_IND (DB_NAME_SD_Right_OFF,NOT);
	    AVC_UpdateRTDB_IND (DB_NAME_SD_Right_ON,NOT);
	    AVC_UpdateRTDB_IND (DB_NAME_ON_SD_CTRL,NOT);
	}
	else if(((ctrl_right==0)||(ctrl_sdtc==1)))
	{
	    PLANT.Control_Right=0;
	    ctrl_flag=0;
	    AVC_UpdateRTDB_IND (DB_NAME_SD_Right_OFF,NOT);
	    AVC_UpdateRTDB_IND (DB_NAME_Control_Right,NOT);
	    AVC_UpdateRTDB_IND (DB_NAME_SD_Right_ON,NOT);
	    AVC_UpdateRTDB_IND (DB_NAME_ON_SD_CTRL,NOT);
	}	
	else if((ctrl_right==1)&&(ctrl_sdtr==1)&&(ctrl_flag==0))
	{
	    PLANT.Control_Right=1;
	    ctrl_flag=1;
	    AVC_UpdateRTDB_IND (DB_NAME_SD_Right_ON,NOT);
	    AVC_UpdateRTDB_IND (DB_NAME_ON_SD_CTRL,YES);
	}
	PLANT.Control_Right=ctrl_flag;
*/
/*以上控制权为湖南省调特殊*/

	if((PLANT.ALG_AVG==1)&&(PLANT.ALG_EQUCOS==1))
	{
		PLANT.ALG_AVG=0;
		PLANT.ALG_EQUCOS=0;
		AVC_UpdateRTDB_IND(DB_NAME_AVC_AVGALG,NOT);
		AVC_UpdateRTDB_IND(DB_NAME_AVC_EQUCOSALG,NOT);
	}
	if((PLANT.VO_Curve==1)&&(PLANT.VO_Setting==1))
	{
		PLANT.VO_Curve=0;
		PLANT.VO_Setting=0;
		AVC_UpdateRTDB_IND(DB_NAME_DY_GD,NOT);
		AVC_UpdateRTDB_IND(DB_NAME_DYQX_GD,NOT);
	}

	for(i=1;i<=6;i++)   //母线电压的个数为3个Uab、Ubc、Uca
	{
		PLANT.VO_RtValue=ReadDB_ANA(DB_NAME_VO_RtValue[i]);
		tmp=ReadDB_ANA_Chan(DB_NAME_VO_RtValue[i]);
		if((PLANT.VO_RtValue<Voltage_Min)||(PLANT.VO_RtValue>Voltage_Max)||(tmp == DATA_ERR))
			continue;
		else
			break;
	}
	if(tmp == DATA_ERR)  //母线电压通道故障，AVC退出
	{
		PLANT.RP_MODE=0;
		PLANT.Mode=5;
		AVC_UpdateRTDB_IND (DB_NAME_QC_AVC,NOT);
	}
#ifdef PBG
	CheckVoRTValue();		//瀑布沟要求单母运行退出AVC
#endif
	PLANT.VO_Setpoint=PLANT.VO_RtValue;
	PLANT.dqv=(int)ReadDB_ANA(DB_NAME_DQV);
	PLANT.Mode=5;
	if(PLANT.VO_Curve==1)
	{
//		PLANT.VO_Setpoint=ReadDB_ANA(DB_NAME_Today_Curve[ntime->tm_hour+1]);
		PLANT.VO_Setpoint=GetCurveValue();	//for PBG,2012-04-12
		AVC_UpdateRTDB_ANA(DB_NAME_VO_Remote_Setpoint,PLANT.VO_Setpoint);
		AVC_UpdateRTDB_ANA(DB_NAME_VO_Setpoint,PLANT.VO_Setpoint);
		AVC_UpdateRTDB_IND  ( DB_NAME_Control_Right, NOT );
		PLANT.Mode=3;
	}
	if(PLANT.VO_Setting==1)
	{
		if(PLANT.Control_Right==0)
		{
			PLANT.VO_Setpoint=ReadDB_ANA(DB_NAME_VO_Setpoint);
			AVC_UpdateRTDB_ANA(DB_NAME_VO_Remote_Setpoint,PLANT.VO_Setpoint);
		}
		else if(PLANT.Control_Right==1)
		{
			PLANT.VO_Setpoint=ReadDB_ANA(DB_NAME_VO_Remote_Setpoint);
			AVC_UpdateRTDB_ANA(DB_NAME_VO_Setpoint,PLANT.VO_Setpoint);
		}
		PLANT.Mode=2;
	}
	if(PLANT.RP_Setting==1)
	{
		PLANT.RP_Setpoint=ReadDB_ANA(DB_NAME_RP_Setpoint);
		PLANT.Mode=1;
	}
	PLANT.VO_Deadband=ReadDB_ANA(DB_NAME_VO_DeadBand);

	if(((PLANT.VO_Curve==1)&&(PLANT.VO_Setting==1))||((PLANT.VO_Curve==1)&&(PLANT.RP_Setting==1))
		||((PLANT.VO_Setting==1)&&(PLANT.RP_Setting==1)))
	{
		PLANT.Mode=5;
		
		AVC_UpdateRTDB_IND (DB_NAME_DY_GD,NOT);
		AVC_UpdateRTDB_IND (DB_NAME_DYQX_GD,NOT);
		AVC_UpdateRTDB_IND (DB_NAME_WGGD,NOT);
	}
	
	PLANT.RP_Min=0;
	PLANT.RP_Max=0;
	for(i=1;i<=UNIT_NUMB;i++)
	{
		UNIT[i].AP_RtValue=ReadDB_ANA(DB_NAME_AP_RtValue[i]);
		UNIT[i].RP_RtValue=ReadDB_ANA(DB_NAME_RP_RtValue[i]);
		UNIT[i].RP_Max=ReadDB_ANA(DB_NAME_RP_LMT[i]);
		UNIT[i].RP_Min=ReadDB_ANA(DB_NAME_RP_LLMT[i]);
		//removed by yyp 2012.12.04
		//UNIT[i].Privilege=(int)ReadDB_ANA(DB_NAME_Prvilege[i]);
		if(PQ_Enable)
		{
			if(UNIT[i].Status!=UNIT_IN_GEN_AGC)
			{
				RP_Min=0;
				RP_Max=0;
			}
			else
			{
				RP_Min=Unit_UELimit(i,UNIT[i].AP_RtValue);
				RP_Max=Unit_OELimit(i,UNIT[i].AP_RtValue);
			}
//			if(RP_Min>UNIT[i].RP_Min)
			UNIT[i].RP_Min=RP_Min;
			AVC_UpdateRTDB_ANA(DB_NAME_RP_LLMT[i],UNIT[i].RP_Min);
//			if(RP_Max<UNIT[i].RP_Max)
			UNIT[i].RP_Max=RP_Max;
			AVC_UpdateRTDB_ANA(DB_NAME_RP_LMT[i],UNIT[i].RP_Max);                        
		}
		UNIT[i].Status=ReadDB_IND(DB_NAME_Rt_Status[i]);
		if(UNIT[i].Status!=UNIT_IN_GEN_AGC)
		{
			UNIT[i].AP_RtValue=0;
			UNIT[i].RP_RtValue=0;
		}
		if(UNIT[i].Status==UNIT_IN_GEN_AGC)
		{
			PLANT.RP_Min+=UNIT[i].RP_Min;
			PLANT.RP_Max+=UNIT[i].RP_Max;
		}
	}
	Statistics_Unit();
	if((PLANT.FirstRun==1)||(PLANT.Mode==5))
	{
		PLANT.VO_Work=PLANT.VO_RtValue;
		PLANT.RP_WorkSP=PLANT.RP_Sum;	
		PLANT.Last_Num_Avail=PLANT.Num_RP_Avail;
	}
	if(PLANT.RP_MODE==0)
	{
		PLANT.Mode=5;
		AVC_UpdateRTDB_IND (DB_NAME_AVC_KBH,NOT);
		AVC_UpdateRTDB_IND (DB_NAME_Control_Right,NOT);
/*		AVC_UpdateRTDB_IND (DB_NAME_DY_GD,NOT);
		AVC_UpdateRTDB_IND (DB_NAME_DYQX_GD,NOT);
		AVC_UpdateRTDB_IND (DB_NAME_WGGD,NOT);
		for(i=1;i<=UNIT_NUMB;i++)
			AVC_UpdateRTDB_IND (DB_NAME_RP_CZ[i],NOT);*/
	}

}

void AVC_Calculation(int AVC_Unit_Run, float AP_Joint, float RP_Joint)	//AVC_Unit_Run=AVC可控机组数，AP_Joint=AVC可控机组的有功之和，RP_Joint=根据电压差计算出的要进行分配的AVC机组总无功
{
    int NumJCRun,NumAVC,UnitPos,i1,i2,Prohibit;
    int EqDis,LimVar,AVC_COS_PHI;
    float ReactVar,UnitSetp,SetpJCAP,SetpJCRP;
    float RP_JC_Add,UnitAVCSetp[MAX_UNIT_NUMB+1],temp;
    int UnitAVC[MAX_UNIT_NUMB+1];
    int Limited[MAX_UNIT_NUMB+1]={0};
    int	UnitNum[MAX_UNIT_NUMB+1]={0};
    float UnitRPValue[MAX_UNIT_NUMB+1],jzmp[MAX_UNIT_NUMB+1];
    int Num1,Num2,ComputeNum;
	char err_str[80];

    NumJCRun = AVC_Unit_Run;
    SetpJCAP = AP_Joint;	//SetpJCRP为尚未分配无功的机组的有功之和变量
    SetpJCRP = RP_Joint;	//SetpJCRP为尚未分配的无功功率变量
    RP_JC_Add = RP_Joint;	
    NumAVC = 0;
    i2 = 0;
    LimVar = 1;
    AVC_COS_PHI = 0;
    Prohibit=0;
    PLANT.RP_Dev=RP_Joint-PLANT.RP_Sum_AVC;													//全厂无功目标值和实发值之差(PLANT.RP_Dev)

	while ((LimVar == 1) && (i2 < UNIT_NUMB) && (NumJCRun > 0)) 		//对计算次数进行循环，条件：1、标志LimVar=1；2、循环次数<机组台数；3、尚未分配无功的AVC可控机组>0
	{
        i2++;
        LimVar = 0;		//标志LimVar暂时置0        
        
		//if (SetpJCAP < 10)                        //条件：如果机组实发有功过小，即AVC可控机组的有功之和<10MW
		if(PLANT.ALG_AVG==1)
        {
           EqDis = 1;				//EqDis = 1表示将所有无功平均分配各台机组
           if (NumJCRun > 0)   
              ReactVar = SetpJCRP / NumJCRun; 	//此处的ReactVar变量代表每台机组的平均无功值
           else 
              ReactVar = 0;
        }
        else
        {
            EqDis = 0;			//EqDis = 0表示机组间将按照有功功率比例的原则分配无功
            ReactVar = SetpJCRP / SetpJCAP;   //此处的ReactVar变量代表无功和有功的比例系数
        }

        for (i1=1; i1<=UNIT_NUMB; i1++) 		//对所有机组循环
        {
            if(UNIT[i1].Status!=UNIT_IN_GEN_AGC) continue;		//机组必须在发电态，否则跳到下个循环

			if (UNIT[i1].RP_Avail==1)			//条件：AVC成组可控
			{
				if(Limited[i1] ==1)  continue;	
                
				if (EqDis == 1)
					UnitSetp = (float)floor(ReactVar); 			//将所有无功全分配给当前机组，floor函数为取整数部分
				else
					UnitSetp = (float)floor(UNIT[i1].AP_RtValue * ReactVar);  //将所有无功全按等功率因数分给当前机组，floor函数为取整数部分
               
				printf("unit%d RP_Min=%f,RP_Max=%f\n",i1,UNIT[i1].RP_Min,UNIT[i1].RP_Max);

				/*分配给当前机组的无功功率越可调下限时*/
                if (UnitSetp < UNIT[i1].RP_Min) 
                {
                   UnitSetp = UNIT[i1].RP_Min;	//当前机组分配的无功等于可调下限
                   LimVar = 1;		//标志LimVar=1，while条件1，下台机组继续计算
                   Limited[i1] = 1;
                   NumJCRun--;	//尚未分配无功的机组台数减1
                   SetpJCRP = SetpJCRP - UnitSetp;		//SetpJCRP变量减去当前机组分配的无功
                   SetpJCAP = SetpJCAP - UNIT[i1].AP_RtValue;	//SetpJCAP变量减去当前机组有功实发值
                }
                /*分配给当前机组的无功功率越可调上限时*/
                if (UnitSetp > UNIT[i1].RP_Max) 
                {
                   UnitSetp = UNIT[i1].RP_Max;	//当前机组分配的无功等于可调上限
                   LimVar = 1;		//标志LimVar=1，while条件1，下台机组继续计算
                   Limited[i1] = 1;
                   NumJCRun--;	//尚未分配无功的机组台数减1
                   SetpJCRP = SetpJCRP - UnitSetp;		//SetpJCRP变量减去当前机组分配的无功
                   SetpJCAP = SetpJCAP - UNIT[i1].AP_RtValue;	//SetpJCAP变量减去当前机组有功实发值
                }

                if (i2 == 1) 	//以下几行只在while的首次循环中执行一次，避免重复计算
                {
                   if (NumAVC <=UNIT_NUMB) 
                   {
                      NumAVC+=1;	//NumAVC每次加一
                      UnitAVC[NumAVC] = i1;	//UnitAVC[i]依次等于每台AVC成组可调机组的机组编号
                   }
                }/*以上几行使得NumAVC等于AVC成组可调的所有机组台数，UnitAVC[i]依次等于每台AVC成组可调机组的机组编号*/
                
                UnitAVCSetp[i1] = UnitSetp;		//当前机组所分配的无功等于临时变量UnitSetp
			} 
            else 			//条件：AVC成组不可控时分配该机组的无功为当前实发无功
			{
              if (UNIT[i1].Status == UNIT_IN_GEN_AGC) 
                 jzmp[i1] = UNIT[i1].RP_RtValue;
			} 
        } 
	}

    i2 = 0;
    for (i1=1; i1<=NumAVC; i1++) 
    {
		UnitPos = UnitAVC[i1]; 
		if (fabs(UnitAVCSetp[UnitPos] - UNIT[UnitPos].RP_Setp) >= RP_Step[UnitPos]) 
			i2++;
		if((UNIT[UnitPos].Increase_Prohibit==1)||(UNIT[UnitPos].Decrease_Prohibit==1))
			Prohibit=1;	/*注意h9000v4.0不用闭锁文件判断无功增减闭锁，而是在程序Check_Lmt()函数中判断*/
    }	/*以上几行用于计算分配无功后，调节步长大于定义文件AVC.DEF中定义的机组无功调节步长死区RP_Step的机组台数*/
    
	if ((i2==NumAVC)||(PLANT.Last_Num_Avail!=PLANT.Num_RP_Avail))		//如果所有机组都超过死区或AVC成组机组台数变化，标志AVC_COS_PHI置1
		AVC_COS_PHI = 1;
	PLANT.Last_Num_Avail=PLANT.Num_RP_Avail;	//PLANT.Last_Num_Avail为上次AVC成组机组台数
    if ((AVC_COS_PHI == 1)&&(Prohibit==0))	//如果所有机组当前的无功分配值调节变化量均在无功调节步长死区以外，且无上下调节闭锁
    {
        for (i1=1; i1<=NumAVC; i1++) 		//以目前计算结果分配每台机组
        {
             UnitPos = UnitAVC[i1];
             jzmp[UnitPos] = UnitAVCSetp[UnitPos];  
        }
    }
    else /*以下是重点的分配算法*/
    { 
        RP_JC_Add = RP_Joint; 	//RP_JC_Add变量=剩余的待分配无功
		for(i1=1;i1<=UNIT_NUMB;i1++)
             Limited[i1]=0;
        for (i1=1; i1<=NumAVC; i1++) 
        {
            UnitPos = UnitAVC[i1];
            if(fabs(UnitAVCSetp[UnitPos] - UNIT[UnitPos].RP_Setp) >= RP_Step[UnitPos]) //如果当前机组的无功分配值调节变化量均在无功调节步长死区以外
                jzmp[UnitPos] = UnitAVCSetp[UnitPos];		//以目前计算结果分配当前机组
            else
                jzmp[UnitPos] = UNIT[UnitPos].RP_Setp;		//否则以上次计算结果分配当前机组
                
    /*机组有增减闭锁时以上次计算结果分配当前机组*/
			if((PLANT.RP_Dev>0)&&(UNIT[UnitPos].Increase_Prohibit==1))
			{
				jzmp[UnitPos] = UNIT[UnitPos].RP_Setp;
				Limited[UnitPos]=1;
				printf("No.%d 增有功闭锁\n",UnitPos);
			}
			if((PLANT.RP_Dev<0)&&(UNIT[UnitPos].Decrease_Prohibit==1))
			{
				jzmp[UnitPos] = UNIT[UnitPos].RP_Setp;
				Limited[UnitPos]=1;
				printf("No.%d 减有功闭锁\n",UnitPos);
			}
            RP_JC_Add-= jzmp[UnitPos];		//重新定义RP_JC_Add变量的剩余待分配无功
        }
        
/*计算到此时各台机组的无功分配都在数组jzmp[i]中，未并网机组分配为0，并网且AVC成组机组分配为上面计算结果，并网但AVC未成组机组分配为当前实发无功*/
        
        ComputeNum=0;  	//ComputeNum为循环计算次数计数变量，
        /*以下为最多10次的循环计算，逐步减小剩余待分配无功*/
		while((RP_JC_Add!=0)&&(ComputeNum<10))
		{
            ComputeNum+=1;
            for(i1=1;i1<=UNIT_NUMB;i1++)
            {
				if (UNIT[i1].RP_Avail!=1) continue;
				if(PLANT.ALG_EQUCOS==1)	//等功率因数方式下选取功率因数为分配顺序条件
				{
					if(UNIT[i1].AP_RtValue<1) UNIT[i1].AP_RtValue=1;
					UnitRPValue[i1]=jzmp[i1]/UNIT[i1].AP_RtValue;  //当前计算结果的（无功/有功）的比例值
				}
				else	//平均分配方式下选取机组有功为分配顺序条件（因为有功值较小的分配裕度较大）
				{
					UnitRPValue[i1]=UNIT[i1].AP_RtValue;  
				}
            }
            Num2=0;
			for(i1=1;i1<=UNIT_NUMB;i1++)
			{
				temp=32767.0;
				Num1=0;
				if ((UNIT[i1].RP_Avail!=1)||(Limited[i1]==1)||(UNIT[i1].Status!=UNIT_IN_GEN_AGC)) continue;	//对于并网但AVC未成组机组、并网成组但有增减无功闭锁的机组保持当前分配结果不变
                /*以下循环语句找出当前计算结果的功率因数最小的机组*/
				for(i2=1;i2<=UNIT_NUMB;i2++)
				{
					if ((UNIT[i2].RP_Avail!=1)||(Limited[i2]==1)||(UNIT[i2].Status!=UNIT_IN_GEN_AGC)) continue;
					if(UnitRPValue[i2]<temp)
					{
						temp=UnitRPValue[i2];
						Num1=i2;
					}
				}
                /*以下将功率因数从小到大排列的机组的编号依次放入UnitNum[i]数组*/
				if(Num1!=0)
				{
					Num2+=1;
					UnitRPValue[Num1]=32767.0;  //将已选的计算结果的功率因数最小的机组功率因数设成一个大值，就不会在下次i2循环中再被选中
					UnitNum[Num2]=Num1;  
				}
			} 
         
            if(RP_JC_Add>0)	//如果剩余未分配无功>0，则下一个待分配机组是功率因数最小的，反之则是功率因数最大的那台
                UnitPos=UnitNum[1];
            else
                UnitPos=UnitNum[Num2]; 
            temp=jzmp[UnitPos];
			if(RP_JC_Add>RP_Step[UnitPos])	//如果剩余未分配无功大于目标机组步长死区值，要给目标机组增无功，但一次分配不完，需要继续循环
            {
                /*如果目标机组在当前结果下再增加一个步长死区值仍未越上限，则将其分配值增加一个步长死区值，否则分配到上限*/
                if(jzmp[UnitPos]+RP_Step[UnitPos]>UNIT[UnitPos].RP_Max)
                {
                    jzmp[UnitPos]=UNIT[UnitPos].RP_Max;
                    Limited[UnitPos]=1;
                }
                else
                    jzmp[UnitPos]+=RP_Step[UnitPos];
                RP_JC_Add-=jzmp[UnitPos]-temp; 	//重新定义RP_JC_Add变量的剩余待分配无功，此时尚未分配完无功
			}
            else if(RP_JC_Add<-RP_Step[UnitPos])	//如果剩余未分配无功小于一个负的目标机组步长死区值，要给目标机组减无功
            {
                /*如果目标机组在当前结果下再减小一个步长死区值仍未越下限，则将其分配值减小一个步长死区值，否则分配到下限*/
                if(jzmp[UnitPos]-RP_Step[UnitPos]<UNIT[UnitPos].RP_Min)
                {
                    jzmp[UnitPos]=UNIT[UnitPos].RP_Min;
                    Limited[UnitPos]=1;
                }
                else
                    jzmp[UnitPos]-=RP_Step[UnitPos];
                RP_JC_Add-=jzmp[UnitPos]-temp;	//重新定义RP_JC_Add变量的剩余待分配无功，此时尚未分配完无功
            }
            /*注意以上两种情况都属于一次分配给一台机组分不完，需要继续while循环*/
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
            /*剩余未分配无功差值不大，如不超过无功上下限值就一次分给目标机组，否则继续分，直到退出while循环*/
		}        
    }  
    /*此时形成最终分配结果，存放在jzmp[i]数组内*/   
	temp=0;
	for (i1=1; i1<=UNIT_NUMB; i1++) 		//以目前计算结果分配每台机组的总给定值
	{
		if (UNIT[i1].RP_Avail==1)
			temp+=jzmp[i1];
	}
	if(fabs(temp-RP_Joint)>=DB_RP_WorkSP)
	{
		sprintf(err_str,"AVC:无功容量不足%dMvar",(int)(RP_Joint-temp));
		BroadCastSms(err_str, TRUE);
	}
    for(i1=1;i1<=UNIT_NUMB;i1++)
    {
        AP_RtValue[i1]=UNIT[i1].AP_RtValue;		//记录下当前的机组实发值，用于下次比较
        	
	    UNIT[i1].XRP_Setp=jzmp[i1];		//将每台机组的最终分配结果赋给变量UNIT[i].XRP_Setp，作为优化值或给定值
	    /*未并网机组分配为0，并网但AVC未成组机组最终分配为当前实发无功*/
		if((UNIT[i1].RP_Avail==0)&&(UNIT[i1].Status==UNIT_IN_GEN_AGC))
            UNIT[i1].XRP_Setp=UNIT[i1].RP_RtValue;
        if(UNIT[i1].Status!=UNIT_IN_GEN_AGC)
            UNIT[i1].XRP_Setp=0;
        printf("Unit No.%d XRP_Setp=%f\n",i1,UNIT[i1].XRP_Setp);
    }
}


/*
void SendAVC_CommandPeerToPeer (char *db_name,int data_type,int data,int status)
{
	LANH_PTR	wltx;
	COMMON_DEF 	kztj;
	int  	tmp;
	short 	host_assist;

     printf("db_name=%s\n",db_name);
#ifdef  AVC_ON_SERVER
        host_assist = GetCurServCtrlPrvg();     
#else
        host_assist = GetCurWSCtrlPrvg();      
#endif
        
//	if (( host_assist != IN_HOST ) ||(PLANT.Close_Loop!=1)) 	
//		return;   
 
	kztj.ptr.data_type = data_type;
	tmp = ReadDB_1 ( db_name );
	kztj.ptr.dev_id = common_aa.entry.anlg.variable.ptr.dev_id;
	kztj.ptr.point_id = common_aa.entry.anlg.variable.ptr.point_id;
	if(data<0)
		status=status|SIGN_MASK;
	kztj.value.status = status;
	kztj.value.data = abs(data);
	kztj.sec = common_aa.entry.anlg.variable.ptr.data_type;	

	printf ( "AVC:PeerToPeer: kztj.ptr.dev_id =%d", kztj.ptr.dev_id );
	printf ( "  data_type =%d", kztj.ptr.data_type );
	printf ( "  point_id =%d", kztj.ptr.point_id );
	printf ( "  status =%x", kztj.value.status );
	printf ( "  data =%d\n", kztj.value.data );
#ifdef SUN_OS
	Swap2Byte(&kztj.ptr.point_id);
	Swap2Byte(&kztj.value.status);
	Swap2Byte(&kztj.value.data);
#endif


	wltx.length = sizeof ( COMMON_DEF );
	wltx.dp_type = COMM;
	wltx.dest_id = kztj.ptr.dev_id;
	wltx.src_id = src_id;
#ifdef SUN_OS
	Swap2Byte(&wltx.length);
#endif
#ifdef SUN_OS
		usleep(200000);
#else
        Sleep(200);
#endif
	lan_out ( &kztj, wltx );    
}
*/
void FillCommandBuf()
{
	int i,flag;
	char	logtxt[100];
	flag=0;
   
	if(PLANT.Mode==5)
	{
		printf("没有功能或无机组加入AVC!\n");
		return;
	}

	for (i=1;i<=UNIT_NUMB;i++) 
	{   
		if(UNIT[i].RP_Avail==0)continue;
 //printf("&&&&&&&&&&&%s = %f\n",DB_NAME_RP_OptiValue[i],UNIT[i].XRP_Setp);
		AVC_UpdateRTDB_ANA (DB_NAME_RP_OptiValue[i],UNIT[i].XRP_Setp); 
		if(PLANT.Close_Loop==0)
		{
			UNIT[i].RP_Setp=UNIT[i].XRP_Setp;
		}
		if((abs(UNIT[i].RP_Setp-UNIT[i].XRP_Setp)>=1)&&(UNIT[i].Status==UNIT_IN_GEN_AGC)&&(PLANT.Close_Loop==1))
		{ 
			UNIT[i].RP_Setp=UNIT[i].XRP_Setp;
			SendCommandToPLC(DB_NAME_RP_SetValue[i],C_RPOWER,UNIT[i].RP_Setp,0); 
			flag=1;
			sprintf(logtxt, "%d月%02d日  %02d:%02d:%02d \n",ntime->tm_mon+1,ntime->tm_mday, ntime->tm_hour, ntime->tm_min, ntime->tm_sec );
			WriteLogFile(logtxt);
			sprintf(logtxt,"No.Unit %d RP_Setp=%d Active Power Adjust Command send out\n",i,UNIT[i].RP_Setp);
			WriteLogFile(logtxt);
		} 
/*		if((UNIT[i].RP_Setp==UNIT[i].XRP_Setp)&&(fabs(UNIT[i].RP_Setp-UNIT[i].RP_RtValue)>10)     //对于瀑布沟，机组有功偏移大于10MW，重新下令
			&&(UNIT[i].Status==UNIT_IN_GEN_AGC)&&(PLANT.Close_Loop==1))
		{
			SendCommandToPLC(DB_NAME_RP_SetValue[i],C_RPOWER,UNIT[i].RP_Setp,0); 
		}*/
	}  
	if(flag==1)
	{
		for(i=1;i<=UNIT_NUMB;i++)
		{
			sprintf(logtxt,"Unit No.%d Status=%d,RP_Avail=%d,Active Power=%d,RP_Min=%d,AP_Max=%d,RP_Setp=%d\n",
				i,UNIT[i].Status,UNIT[i].RP_Avail,UNIT[i].RP_RtValue,UNIT[i].RP_Min,UNIT[i].RP_Max,UNIT[i].RP_Setp);
			WriteLogFile(logtxt);
		}
		sprintf(logtxt,"PLANT.Mode=%d,APSetpoint=%d PLANT.Control_Right=%d\n\n\n",PLANT.Mode,PLANT.RP_Setpoint,PLANT.Control_Right);
		WriteLogFile(logtxt);
	}
}

int Check_Lmt()
{ 
	int i,tmp_uplock,tmp_dnlock,unit_avail;
	float rt_I,rt_lc_I,rt_Uab,plant_maxRP,plant_minRP;
	char err_str[80];
	
	tmp_uplock=0;
	tmp_dnlock=0;
	unit_avail=0;
	plant_maxRP=0;
	plant_minRP=0;
	for(i=1;i<=UNIT_NUMB;i++)
	{
		/*读转子电流、机端电压限值*/
		UNIT[i].LC_I_max=ReadDB_ANA(DB_NAME_LC_I_MAX[i]);		
		UNIT[i].U_max=ReadDB_ANA(DB_NAME_AVC_UMAX[i]);	
		UNIT[i].U_min=ReadDB_ANA(DB_NAME_AVC_UMIN[i]);     
		UNIT[i].I_max=ReadDB_ANA(DB_NAME_AVC_IMAX[i]);	
		/*读转子电流、机端电压实时值*/	
		rt_lc_I=ReadDB_ANA(DB_NAME_AVC_LCI[i]);		
		rt_Uab=ReadDB_ANA(DB_NAME_AVC_UAB[i]);	
		rt_I=ReadDB_ANA(DB_NAME_AVC_I[i]);
		if(UNIT[i].RP_Avail==1)
		{
			unit_avail++;
			if(((rt_lc_I>=UNIT[i].LC_I_max)||(rt_Uab>=UNIT[i].U_max)||(rt_I>=UNIT[i].I_max))
			 &&(PLANT.VO_Setpoint-PLANT.VO_RtValue>0))		/*检查转子电流、机端电压上限，母线增电压闭锁，(定子电流检查取消）*/
			{			
				UNIT[i].RP_Max=UNIT[i].RP_RtValue;				
				tmp_uplock++;
				printf("Unit %d 定子电流、转子电流、机端电压越上限！\n",i);
				if(UNIT[i].Increase_Prohibit==0)
				{
					sprintf(err_str,"AVC:%dF增磁闭锁",i);
					BroadCastSms(err_str, TRUE);
					UNIT[i].Increase_Prohibit=1;
				}
			}
			else
				UNIT[i].Increase_Prohibit=0;
			if((rt_Uab<=UNIT[i].U_min)&&(PLANT.VO_Setpoint-PLANT.VO_RtValue<0))	/*检查机端电压下限，母线减电压闭锁*/
			{
				UNIT[i].RP_Min=UNIT[i].RP_RtValue;
				tmp_dnlock++;
				printf("Unit %d 机端电压越下限！%f\n",i,UNIT[i].RP_Min);  
				if(UNIT[i].Decrease_Prohibit==0)
				{
					sprintf(err_str,"AVC:%dF减磁闭锁",i);
					BroadCastSms(err_str, TRUE);
					UNIT[i].Decrease_Prohibit=1;
				}
			}  
			else
				UNIT[i].Decrease_Prohibit=0;
		}
		if(UNIT[i].Status==UNIT_IN_GEN_AGC)
		{
			plant_maxRP+=UNIT[i].RP_Max;
			plant_minRP+=UNIT[i].RP_Min;
		}
	}
	if(fabs(PLANT.VO_Setpoint-PLANT.VO_RtValue)>MaxAdj_V)          /*检查电压给定步长闭锁,AVC退出*/
    {
		printf("电压调节步长越限%6.2f kV，AVC挂起!\n",MaxAdj_V);
		if(lock_flag==0)
		{
			sprintf(err_str,"AVC:电压调节%3.1fkV越限",(PLANT.VO_Setpoint-PLANT.VO_RtValue));
			BroadCastSms(err_str, TRUE);
		}
		lock_flag=1;
		return(1);
	}
	if(tmp_uplock==unit_avail)
	{		
		printf("所有机组定子电流、转子电流、机端电压越限，AVC无功增挂起！\n");
		if(PLANT.Lock_RP_increase==0)
		{
			sprintf(err_str,"AVC:全厂增电压闭锁!",i);
			BroadCastSms(err_str, TRUE);
		}
		PLANT.Lock_RP_increase=1;
		return(1);
	}
	else
		PLANT.Lock_RP_increase=0;
	if(tmp_dnlock==unit_avail)
	{		
		printf("所有机组机端电压越下限，AVC无功减挂起！\n");
		if(PLANT.Lock_RP_decrease==0)
		{
			sprintf(err_str,"AVC:全厂减电压闭锁!",i);
			BroadCastSms(err_str, TRUE);
		}
		PLANT.Lock_RP_decrease=1;
		return(1);
	}
	else
		PLANT.Lock_RP_decrease=0;
	if((PLANT.VO_Setpoint>Voltage_Max)||(PLANT.VO_Setpoint<Voltage_Min))
	{
		printf("给定电压越过定义文件规定的上下限%d~%dKv,AVC挂起\n！\n",Voltage_Min,Voltage_Max);
		return(1);
	}
	if(plant_maxRP<PLANT.RP_WorkSP)
	{
		PLANT.Lock_RP_increase=1;
		printf("无功目标值%6.2f超过所有机组无功容量%6.2f，AVC无功增挂起！\n",PLANT.RP_WorkSP,plant_maxRP);
		return(2);
	}
	if(plant_minRP>PLANT.RP_WorkSP)
	{
		PLANT.Lock_RP_decrease=1;
		printf("无功目标值%6.2f低于所有机组进相容量%6.2f，AVC无功减挂起！\n",PLANT.RP_WorkSP,plant_minRP);
		return(2);
	}
	
	return(0);
}

void AVC_Module()
{
	float RP_Joint,RP_WorkSP,Act_DB_RP_WorkSP;
	int AVC_Recalc=0,i,tmp;
    
	if((PLANT.VO_Setting==1)||(PLANT.VO_Curve==1))
	{
		if(fabs(PLANT.VO_Setpoint-PLANT.VO_RtValue)<=PLANT.VO_Deadband)
			RP_WorkSP=PLANT.RP_Sum;
		else
			RP_WorkSP=PLANT.dqv*(PLANT.VO_Setpoint-PLANT.VO_RtValue)+PLANT.RP_Sum;
		Act_DB_RP_WorkSP=DB_RP_WorkSP;
	}
	else if(PLANT.RP_Setting==1)
	{
		RP_WorkSP=PLANT.RP_Setpoint;
		Act_DB_RP_WorkSP=1;	
	}
	else
	{
		RP_WorkSP=PLANT.RP_Sum;
		Act_DB_RP_WorkSP=DB_RP_WorkSP;
	}
	printf("RP_WorkSP=%f,PLANT.RP_WorkSP=%f,Act_DB_RP_WorkSP=%f\n",RP_WorkSP,PLANT.RP_WorkSP,Act_DB_RP_WorkSP);
	if (fabs(PLANT.RP_WorkSP - RP_WorkSP) >= Act_DB_RP_WorkSP) 
	{
		printf("The change value of reactive power work value has passed deadband!\n");
		PLANT.RP_WorkSP = RP_WorkSP;  
		AVC_Recalc = 1;
	}
    if(PLANT.Last_Num_Avail!=PLANT.Num_RP_Avail)
    {
        printf("The Number of Unit in Joint Control has changed!\n"); 
        AVC_Recalc = 1;
    }
    PLANT.RP_Deviation=PLANT.RP_WorkSP-PLANT.RP_Sum;
/*    
    if(fabs(PLANT.Last_RP_Sum_NotAVC-PLANT.RP_Sum_NotAVC)>10)
    {
        PLANT.Last_RP_Sum_NotAVC=PLANT.RP_Sum_NotAVC;
        AVC_Recalc = 1;
    }
   */
    if((PLANT.Mode!=5)&&(PLANT.Last_Mode==5))
    {
    	printf("AVC电厂投入功能%d!\n",PLANT.Mode);	
	    AVC_Recalc=1;
    }
	for(i=1;i<=UNIT_NUMB;i++)
	{
		if(UNIT[i].RP_Avail==1)
		{
			if((UNIT[i].RP_RtValue>UNIT[i].RP_Max)||(UNIT[i].RP_RtValue<UNIT[i].RP_Min)
				||(UNIT[i].RP_Setp>UNIT[i].RP_Max)||(UNIT[i].RP_Setp<UNIT[i].RP_Min))
				AVC_Recalc=1;
		}
	}
	PLANT.Last_Mode=PLANT.Mode;
/*    for(i=1;i<=UNIT_NUMB;i++)
    {
        if(fabs(UNIT[i].AP_RtValue-AP_RtValue[i])>=100)
        {
            printf("No.%d 有功实发值变化超过100MW\n",i);
            AVC_Recalc=1;
        }
    }        
 */   
    tmp=Check_Lmt();       /*检查机组定子电流、转子电流、机端电压、给定电压越限*/    
    
    if(tmp==0)  
    {        
        AVC_UpdateRTDB_IND (DB_NAME_WG_UP_LOCK ,NOT);
        AVC_UpdateRTDB_IND (DB_NAME_WG_DN_LOCK ,NOT);                
    }
    else if(tmp==1)
    {
        AVC_Recalc=0;
		PLANT.RP_WorkSP=PLANT.RP_Sum;
    }
    else if(tmp==2)
    {
        AVC_Recalc=1;
    }
    if(PLANT.Lock_RP_increase==1)
        AVC_UpdateRTDB_IND (DB_NAME_WG_UP_LOCK ,YES);
    if(PLANT.Lock_RP_decrease==1)
        AVC_UpdateRTDB_IND (DB_NAME_WG_DN_LOCK ,YES);
    
    RP_Joint = PLANT.RP_WorkSP - PLANT.RP_Sum_NotAVC;
	printf("AVC_Recalc=%d,RP_Joint=%5.2f,PLANT.RP_Sum_AVC=%5.2f\n",AVC_Recalc,RP_Joint,PLANT.RP_Sum_AVC);    
	if((AVC_Recalc==1)&&(fabs(RP_Joint-PLANT.RP_Sum_AVC)>Act_DB_RP_WorkSP))
	{
		printf("AVC Recalc: RP_Joint=%f,PLANT.P_Sum_AVC=%f,PLANT.Num_RP_Avail=%d\n",RP_Joint,PLANT.P_Sum_AVC,PLANT.Num_RP_Avail);

		AVC_Calculation(PLANT.Num_RP_Avail,PLANT.P_Sum_AVC,RP_Joint);
		FillCommandBuf();
	}
//	FillCommandBuf();
}

void main(int argc,char *argv[])
{
	int sbo_flag,rt_status,tmp;

#ifdef WINDOWS
	WORD	wvr,wVersionRequested;
	WSADATA	WSAData;  
	wvr=MAKEWORD(1,1); 	
	if ((rt_status = WSAStartup(wvr, &WSAData)) != 0)
	{
		printf("### AvC---main() WSAStartup error!\n");
		Sleep(2000);
	}
#endif
	gethostname(hostname,20);
	printf("hostname =%s\n",hostname);
	
#ifdef SUN_OS	
	SetModuleName("AVC");
	SetErrLogMode(argc,argv);  
	CreateLockFile(".avc.lock");
	signal( SIGINT, SIGINT_handler);
#endif  
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

	ReadCurveDefFile();
	ReadAVCDefFile();
	if(PQ_Enable)	ReadPQDefFile();
	ReadAVC_DataFromRTDB();
	InitData();

//	src_id = GetHost_Cmd ();

	signal( SIGINT, SIGINT_handler);

	ReadTime ();
    oldday = ntime->tm_mday;
#ifdef SUN_OS
	sprintf(logfilename, "/home/ems/h9000/his/avc/v4.0a/log/AVCcmd.%02d",ntime->tm_mday);
#else
	sprintf(logfilename, "/home/ems/h9000/his/avc/v4.0a/log/AVCcmd.%02d",ntime->tm_mday);
#endif
    
	InitialAVC_Data_On_Screen();

	for (;;)
	{
		sbo_flag = ReadDB_IND(DB_NAME_Run_State);
		tmp = GetControlState();	//判断主机的主用或备用
		ReadTime ();
		LogFileChangeDayAndWriteTime();
		ReadAVC_DataFromRTDB();

		if((sbo_flag==1)&&(tmp==1))
		{
			AVC_Module();
		}
		else if(tmp==1)
		{			
//			AVC_UpdateRTDB_IND (DB_NAME_Run_State,NOT);
			AVC_UpdateRTDB_ANA (DB_NAME_VO_Setpoint,PLANT.VO_RtValue); 
			AVC_UpdateRTDB_ANA (DB_NAME_VO_Remote_Setpoint,PLANT.VO_RtValue);
			InitialAVC_Data_On_Screen ();	
		}
		PLANT.FirstRun=0;
		AVC_Loop++;
		if(AVC_Loop>10000)
		AVC_Loop=1;
#ifdef SUN_OS
    sleep(4);
#else
	Sleep(4000);
#endif
	}
}
