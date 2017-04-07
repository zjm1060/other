/*#define AGC_ON_SERVER*/ /*在JMOPA上运行*/

//#define	WindowsOS
#define	SUNOS
#define	DEBUG
#define	HOST_NAME	"HOSTNAME"

//#define HuaZhongWangDiao	//华中网调
#define PBG	//瀑布沟

#define SCHEDULE    96
#define NAME_SIZE   40
#define MAXSEGMENT	3
#define MAX_UNIT_NUMB	20
#define HEAD_NUMBER    50
#define	DATA_ERR	-1
#define	CHANN_FAIL	2
#define VIBRATION_NUMB	2
#define INFI	3000000
/*以下机组状态定义仅适用于瀑布沟*/
#ifdef PBG
#define UNIT_IN_CHKOUT_AGC 0	//检修态
#define	UNIT_IN_STANDBY_AGC	2	//备用态
#define	UNIT_IN_COMP_AGC	3
#define	UNIT_IN_UNSYN_AGC	4
#define UNIT_IN_GEN_AGC	5	//并网态
#else
#define UNIT_IN_CHKOUT 2	//检修态
#define	UNIT_IN_STANDBY_AGC	0	//备用态
#define	UNIT_IN_COMP_AGC	3
#define	UNIT_IN_UNSYN_AGC	4
#define UNIT_IN_GEN_AGC 1	//并网态
#endif

#define HANGUP	1
#define QUIT	2

#define INCREASE	1
#define DECREASE	2

#define KJML		0xc10	
#define TJML		0xc20	
#define TZFML		0xc40	
#define KZZFDML	        0xc50  
#define FDZKZML		0xc90   
#define YGTJML		0xc00	
#define WGTJML		0xc00	

#define TGLTHM	 0 	/* 调功令 */
#define KJLTHM	 1	/* 开机令 */
#define TJLTHM	 2	/* 停机令 */
#define WCZTHM	 3	/* no operation */

#define NOT 0
#define YES 1
#define FHQX_INTERVAL 15

#define AGC_MODE_AP_Setpoint	1
#define AGC_MODE_AP_Curve		2
#define	AGC_MODE_NONE			3

struct tm  *ntime;
DMS_COMMON common_aa;
int UNIT_NUMB=0;
int move_curve=0;
int src_id=0;
int oldday=0;
char   logfilename[50]="";
int AGC_Period=0;
int refre_time=0;
int water_refresh_Period=0;
float MinWaterhead,MaxWaterhead;
float WaterheadPower[MAX_UNIT_NUMB+1][6][HEAD_NUMBER+1];
int	Type_UnitNo[MAX_UNIT_NUMB+1][MAX_UNIT_NUMB+1]={0};
int ItemNumber,TypeNumber;
float factor[MAX_UNIT_NUMB+1];
int AP_Step[MAX_UNIT_NUMB+1];
int AP_Adj_Step[MAX_UNIT_NUMB+1];
int minor_adjust[MAX_UNIT_NUMB+1];	//小负荷调整的机组优先顺序排序编号
int minor_adjust_p_tmp[MAX_UNIT_NUMB+1];	//小负荷调整的机组临时分配结果
int	ManSetLmt;
int AGC_P_Set_Dev;
int CurveSetFuc;
int	AGC_Loop;

typedef struct
{
	int	   WHeadChanged;
	float    UpWaterLevel;
	float    DownWaterLevel;
	float	 WaterHead;
	float	 XWaterHead;
}Water_Msg;

typedef struct
{
    int		AP_MODE;            /*有功联控方式  投入/退出*/
    int		Control_Right;         /* 控制权  厂控/梯调/国调 0为厂控、1为梯调、2为国调*/
    int		Closed_Loop;    /*开环/闭环*/
    int		Mode;           /*AGC调节方式(有功给定/负荷曲线/调频/频率补偿/无功能)*/
    int		AP_Setting;      /*有功给定*/
    int		AP_Curve;        /*负荷曲线*/
    int		FR_Regulation;             /*调频*/
	int		Auto_StartStop;     /*自动开停机方式*/
    int		FR_Compensation;  /*频率补偿模式*/
    int		Sel_Bus;        /*选择母线  母线I/母线II*/
    int		SR_AVAIL_Setting;	/*有功给定旋转备用投入*/
    int		SR_AVAIL_Curve;	   /*负荷曲线旋转备用投入*/
    int		AP_Faulty;			/*有功测量故障*/
    int		XAGC_Unit_Comp;		/*机组处于有功补偿方式下*/
    float	    State_SetValue;		/*国调有功设定值*/
    float	    Cascade_SetValue;	/*梯调有功设定值*/
    float	    Operator_SetValue;	/*操作员设定有功值*/
    float	    p_sum_no_agc;           /* 单控机组有功之和 */
    float	    p_sum_agc;                      /* 联控机组总有功      */
    float	    p_sum;                          /* 全厂总有功       */
	float	ace;	/*给定值与实发值的误差*/
	float	last_ace;	/*上次计算的给定值与实发值的误差*/
    float	    AP_Setpoint;     /*全厂有功给定值*/
    float	    XAP_Setpoint;
    float	    YAP_Setpoint;
	float		MinorAdjP;  /*小容量变化单机分配阙值*/
	int		P_adj;	/*当前给定值与上一次给定的差值*/
    int		Num_AP_Avail;       
    int		Last_Num_AP_Avail;
    int		AGC_Generating;         /* glfs, number of units in agc generation      */
    int		Last_AGC_Generating; 
    int		AGC_Standby;            /* glbs, numb of units in agc standby   */
    int		Generating_No_AGC;      /* unglfs, nmb of units in no agc generation    */
    int		Num_Unit_Run;			/*所有运行机组数*/
    int	    UplmtCurrentZone;   /*不跨越振动区上限*/
    int	    LowlmtCurrentZone;   /*不跨越振动区下限*/
    int	    AP_Deadband;        /*有功调节死区*/
    int	    WaterFlow;		/*发电流量*/
    int		AP_CTRL_Alarm;  /*有功控制报警*/
    int		FR_Set_change; /*频率设定值变化*/
    int		FR_Set_Error;   /*频率设定值错误*/
    int		FR_Faulty;     /*频率测量值故障*/
    int		Start_Req;		/*开机请求*/
    int		Stop_Req;		/*开机请求*/
    int		XUnitStrStp;	/*机组开停机*/
    int		Unit_Start;     /*下一台启动机组*/
    int		Unit_Stop;      /*下一台停运机组*/
    int		SSAnswerYes;		/*开停机确认*/
    int		SSAnswerNo;			/*开停机拒绝*/
    int		APAnswerYes;		/*有功调节确认*/
    int     APAnswerYesTime;
    int     SSAnswerYesTime;
    int     SSAnswerNoTime;
    int		FR_CTRL_Alarm;		/*频率控制生效*/
    int		XStrStp_Recalc;		/*开停机重计算*/
    int		SSReject_Num;		/*开停机拒绝次数*/
    int		XAP_Comp_Mode;
    int		XAP_Comp_Time;
    int     AP_Fail_Time;
    int     FR_Fail_Time;
    int		SR_SetPoint;    /*旋转备用设定值*/
    int     Last_SR_SetPoint;
    int     SR_MaxValue;
    int     SR_MaxChange;
    float		AP_ChangeMax;	/*有功设定值最大变幅*/
    int		FR_ChangeMax;	/*频率设定值最大变幅*/
    float		FR_Setpoint;	/*频率设定值*/
    float		XFR_Setpoint;
    float		YFR_Setpoint;
    float		FR_Deadband;	/*频率死区*/
    float		FR_CompDeadband;	/* 频率补偿死区*/
    float		FR_PowGenDB;     /*调频补偿调节幅度限值*/
    float		FR_ActValue;	/*频率实际值*/  
    float     FR_CompPowBand;   /*max change*/
    float		XAP_FreqCTRL;	/*频率控制有功工作值*/
    float		AP_WorkSP;		/*全厂有功功率设定记录值*/
    int		FR_WorkSP;      /*频率工作设定值*/
    int		Efficiency;		/*效率*/
    int		AGC_P_Set;      /* 成组机组总有功功率当前给定值 */
	int		Last_AGC_P_Set; /* 成组机组总有功功率上次给定值 */
    int		AP_Dev;     /*有功给定值是增加还是减少*/
    int     AP_Deviation;
    int		Dev_Frequency;  /*频率偏差*/
    int		AP_Min;		/*全厂最小总有功*/
    int		AP_Max;     /*全厂最大总有功*/
    int     FR_Min;
    int     FR_Max;
    float		SS_Deadband;	/*开停机死区*/
    int		VZ_Deadband;	/*跨越振动区死区值*/
    float     EC_Deadband;
    int		XFreq_Output;	/*频率控制模式输出功率*/
    int		XFreqDoopAdaptFact; /*频率自适应系数*/
    int		XFreqMaxDev;	/*频率最大偏差*/
    int		XFreqDevDir;	/*频率偏差方向*/
    int		XStrStp_Delay;  /*开停机延时*/
    int		XAP_Comp_Step;	/*有功补偿步长*/
    int		XAP_DistributeDelay;	/*分配延时*/
    int		ACE_over_adj;			/*机组负荷波动，有功ACE发生变化需要微调*/
    int     AP_Alm_Time;
    int     Offset_Enable;
    int     Curve[SCHEDULE];     /**/
    int     SR_Curve[SCHEDULE];
    int     Offset;
    int     ZeroCurveValue;
    int     CurvePointNum;  
    int     FirstRun;
    int		Remote_SingleUnit_Control;
	int		Manual_Set;
} T_PLANT;   

typedef struct
{
    int		Status;            /*机组状态 */
    int     Maint_Status;
    int		Last_Status;		/*上次机组状态*/
    int     AP_Sel;
    int     AP_Permit;
    int		AP_Avail;           /*机组是否联控可调*/
    int		Last_AP_Avail;		/*上次机组是否联控可调*/
    float	AP_RtValue;			/*实发有功*/
	float	Last_APValue;		/*上次实发有功*/
    int     InputPower;
    int		AP_Faulty;			/*有功数据质量故障*/
    int		Standby_Time;         /*连续备用时间，单位：分*/
    int		Generating_Time;         /*连续发电时间，单位：分*/
    int		Start;			/*开机*/
    int		Stop;			/*停机*/
    int		XStop;			/*卸载负荷尚未发停机令*/
    int		Start_Req;      /*开机请求*/
    int		Stop_Req;       /*停机请求*/
    int		XAnswerNo;		/*拒绝*/
	int		VibrationNum;
    float		Low_Vibration[VIBRATION_NUMB+1];
	float		Up_Vibration[VIBRATION_NUMB+1];
    float		AP_Min;          /*机组运行下限*/
    float		AP_Max;          /*机组运行上限*/
    float       Last_AP_Min;
    float       Last_AP_Max;
    float       AP_Chang_Max;
    int		CurrentUplmt;                 /*当前不跨越振动区上限*/
    int		CurrentLowlmt;                /*当前不跨越振动区下限*/
    int     Privilege;     /*开停机优先级*/
    int		Type;			/*机组类型*/
    int		XAP_Setp;		/*机组分配的有功值*/
    int		AP_Setp;		/*机组分配的有功值*/
    int		StartCmdTime;		/*开机命令时间*/
    int		StopCmdTime;		/*停机卸载时间*/
    int		StopTime;			/*机组停机监视时间（产生停机失败报警）*/
    int		StopAction;			/*标记为停机机组*/
    int		StartAction;		/*标注机组为启动机组*/
    int		StartProcess;
    int		Starting;			/*正在开机机组，由PLC读状态*/
    int		Stopping;			/*正在停机机组，由PLC读状态*/
    int		ReduceLoad;
    int		StartFlag;
    int		StopFlag;
    int		StartTime;         /*机组开机监视时间（产生开机失败报警）*/
    int		XAPCompMode;		/*有功补偿级别*/
    int		XPowerUp;
    int		Zone;				/*机组所处运行区*/
    int		Vib_Zone;			/*0/1表示不在/在振动区*/
    int		XVibDelay;			/*统计机组在振动区时间*/
    int		XAP_Setp_Calc;		/*机组有新的设定值*/
    int		XStartUpTime;		/*机组并网后计时*/
    int		AP_Fail;			/*有功调节失败*/
    int		XStartBlockTime;	/*启动阻塞时间*/
    int		CommandTime;		/*开机或停机命令保持时间10秒*/
    int		APCompTime;			/*有功补偿时间*/
    int     AP_FailTime;
    int     RduceLoadFailTime;
    int		Flow;				/*流量*/
    int		Efficiency;
    int		RemoteControlable;	/*机组是否远方可控*/
} T_UNIT;     /*机组工况*/

typedef struct
{		
	int     range_p_dn;
	int		range_p_up;
}	P_RANGE;

typedef struct
{
	int     unit_id;
	int		range_id;
	P_RANGE p_range;
}       P_SPACE_1U;        /*1台机组有功分配的解空间*/

typedef struct
{
	int  spa1_id;
	int	 spa2_id;
	P_RANGE p_range;
}       P_SPACE_2U;        /*2台机组有功分配的解空间*/

typedef struct
{	
	int	 spaU2_id;
	int  spaU1_id;
	P_RANGE p_range;
}       P_SPACE_3U;        /*3台机组有功分配的解空间*/

typedef struct
{
	int  spaU3_id;
	int	 spaU1_id;
	P_RANGE p_range;
}       P_SPACE_4U;        /*4台机组有功分配的解空间*/

typedef struct
{
	int  unit_id;
	int  unit_P;
}       P_SPA_Unit_RESULT;        /*用解空间的方法得出的机组有功分配的解*/

typedef struct
{
	P_SPA_Unit_RESULT Unit_Result[5]; //最多容纳四台机组的解
	int	 dx;		//本组解的方差
}       P_SPACE_RESULT;        /*用解空间的方法得出的机组有功分配的解，目前最多三台机组*/

typedef struct
{
        POINTER ptr;
        ANLG_DATA value;
        char   sec;
	    char   min;
}       COMMON_DEF;     /* 通讯数据包 */


Water_Msg Watermsg;
T_PLANT PLANT;
T_UNIT  UNIT[MAX_UNIT_NUMB+1];
P_SPACE_1U result_space1[20];
P_SPACE_2U result_space2[100];
P_SPACE_3U result_space3[100];
P_SPACE_4U result_space4[300];
P_SPACE_RESULT p_space_result,tmp_space_result;
int spnum_1U;
int spnum_2U;
int spnum_3U;
int spnum_4U;

char DB_NAME_Today_Curve[SCHEDULE+1][NAME_SIZE];
char DB_NAME_Tomorrow_Curve[SCHEDULE+1][NAME_SIZE];
char DB_NAME_AP_RtValue[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_Optimization_Value[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_Control_Command[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RT_P_Curve[NAME_SIZE];
char DB_NAME_AGC_Period[NAME_SIZE];
char DB_NAME_Control_Right[NAME_SIZE];
char DB_NAME_AGC_CLOSED_LOOP[NAME_SIZE];
char DB_NAME_QC_AGC[NAME_SIZE];
char DB_NAME_AP_Setting[NAME_SIZE];
char DB_NAME_AP_Curve[NAME_SIZE];
char DB_NAME_AP_ChangeMax[NAME_SIZE];
char DB_NAME_Rt_Status[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AP_Group[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AP_CZKT[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AP_RtValue[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AP_SetValue[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_BREAKER_Status[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AGC_STATE[NAME_SIZE];
char DB_NAME_Prvilege[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AP_Setpoint[NAME_SIZE];
char DB_NAME_Upstream_Level[NAME_SIZE];
char DB_NAME_Downstream_Level[NAME_SIZE];
char DB_NAME_Water_Head[NAME_SIZE];
char DB_NAME_Manual_Set[NAME_SIZE];
char DB_NAME_Remote_Setpoint[NAME_SIZE];
char DB_NAME_Low_Vibration[MAX_UNIT_NUMB+1][VIBRATION_NUMB+1][NAME_SIZE];
char DB_NAME_Up_Vibration[MAX_UNIT_NUMB+1][VIBRATION_NUMB+1][NAME_SIZE];
char DB_NAME_AP_Min[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AP_Max[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_StartStopDeadband[NAME_SIZE];
char DB_NAME_DB_AP_WorkSP[NAME_SIZE];
char DB_NAME_P_UPLMT[NAME_SIZE];
char DB_NAME_P_DOWNLMT[NAME_SIZE];
char DB_NAME_RemoteAGCOnLine[NAME_SIZE];
char DB_NAME_SD_YK[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_SD_PSet[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AGCYG_STEP[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_Remote_OverPLmtALM[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_Remote_OverBCLmtALM[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_QCUNRUN_UPLMT1[NAME_SIZE];    /*全厂第一有功禁运上下限，added by 瀑布沟 for FT 2010-918*/
char DB_NAME_QCUNRUN_DNLMT1[NAME_SIZE];
char DB_NAME_QCUNRUN_UPLMT2[NAME_SIZE];    /*全厂第二有功禁运上下限，added by 瀑布沟 for FT 2010-918*/
char DB_NAME_QCUNRUN_DNLMT2[NAME_SIZE];
char DB_NAME_ALM_Plant_UN_RUN[NAME_SIZE];	/*全厂有功给定在全厂禁运区内报警*/
char DB_NAME_ALM_ChangeMax[NAME_SIZE];	/*全厂有功给定超过单步调整步长报警*/
char DB_NAME_ALM_StatusErr[NAME_SIZE];	/*全厂有功给定超过单步调整步长报警*/
char DB_NAME_ALM_YFKErr[NAME_SIZE];	/*全厂AGC切远方条件不满足报警*/
char DB_NAME_AGC_FRUPLMT[NAME_SIZE]; /*增有功闭锁频率上限值*/
char DB_NAME_AGC_FRDOWNLMT[NAME_SIZE]; /*增有功闭锁频率上限值*/
char DB_NAME_AGC_FRLOW_ERR[NAME_SIZE]; /*AGC频率低减负荷闭锁*/
char DB_NAME_AGC_FRHIGH_ERR[NAME_SIZE]; /*AGC频率高增负荷闭锁*/
char DB_NAME_AGC_RTFR[NAME_SIZE]; /*当前母线频率值*/
char DB_NAME_AGC_FRLOW_ERR[NAME_SIZE];	/*AGC频率低减负荷闭锁*/
char DB_NAME_AGC_FRHIGH_ERR[NAME_SIZE];	/*AGC频率高增负荷闭锁*/
char DB_NAME_DD104_DOWN[NAME_SIZE];	/*与网调104通讯中断*/
char DB_NAME_WATERINFO_ERR[NAME_SIZE]; /*AGC水情数据错误报警*/
char DB_NAME_AGC_PLOW_ERR[NAME_SIZE];	/*与网调104通讯中断*/
char DB_NAME_AGC_POVER_ERR[NAME_SIZE]; /*AGC水情数据错误报警*/
char DB_NAME_AGC_LMT_DB_ALM[NAME_SIZE]; /*AGC因跨越振动区而不满足给定值分配时报警点*/
char DB_NAME_AGC_COMP_DERECT[NAME_SIZE];	/*AGC因跨越振动区而不满足给定值分配时建议增减负荷的方向，1为增，2为减*/
char DB_NAME_AGC_LMT_DB_ADVICE[NAME_SIZE]; /*AGC因跨越振动区而不满足给定值分配时建议手动增减的负荷值*/
char DB_NAME_ALM_P_GETERR[NAME_SIZE]; /*AGC全厂有功采集值突变报警*/
char DB_NAME_WATER_REFR_TIME[NAME_SIZE]; /*人工设置水位刷新时间*/
char DB_NAME_MINOR_ADJ_P[NAME_SIZE]; /*小容量变化单机分配阙值*/

//float min_jz_uplmt,sum_jz_downlmt;	/*发电成组机组的最小振动上限和发电机组的振动下限值之和(旧的全厂振动区算法)*/

/*for Schedule.o*/
/*
#define BUF_NUM	100
typedef struct{
  int		  RowNum;
  int         Position;    //Position of row
  int         Hour;        //Hour of time
  int         Minute;      //Minute of time
  double       APower;      //Active power
  double       Voltage;     //Voltage
  double       Spinn;       //Spinning reserve
  double        Error;       //Error in line detected
}T_Schedule;

int LIMITS_MAX_ACTIVE_POWER;
int LIMITS_MIN_ACTIVE_POWER;
int LIMITS_MAX_SPINN_RESERVE;
int LIMITS_MIN_SPINN_RESERVE;
int LIMITS_MAX_VOLTAGE;
int LIMITS_MIN_VOLTAGE;
char DB_NAME_TD_JRFHQX_SD[MAXSEGMENT][NAME_SIZE];
char DB_NAME_TD_JRFHQX_XZ[MAXSEGMENT][NAME_SIZE];
char DB_NAME_TD_MRFHQX_SD[MAXSEGMENT][NAME_SIZE];
char DB_NAME_TD_MRFHQX_XZ[MAXSEGMENT][NAME_SIZE];
char DB_NAME_TD_JRDYQX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_TD_MRDYQX[MAXSEGMENT][NAME_SIZE];
*/
