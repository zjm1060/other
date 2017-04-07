

//#define RECORD
//#define CASCADE

#define	HOST_NAME	"HOSTNAME"

#define UNIT_NUMB    8
#define NAME_SIZE    36
#define	MAXSEGMENT		3
#define DATA_ERROR		-1000
#define DATA_BAD		-1001
#define OVER_HIGH_LMT           -1002
#define MAXDEFPOINTS   25
#define	MAXAGCPOINTS   35  
#define	MAXTYPES 	2


#define VIBRATION_NUMB 5   /*最大振动区个数*/


#define OPEN_LOOP		0
#define	HALF_OPEN_LOOP	1
#define	CLOSED_LOOP	2


#define TGML        0
#define KJML		23
#define TJML		26	

#define STAND_BY    2
#define CHECKOUT    0

#define BUF_NUM	100

typedef struct
{
	int		WHeadChanged;
	float	 WaterHead;
	float	 XWaterHead;
}Water_Msg;

typedef	struct  
{
  float       HL_Value;
  float       LL_Value;
  float       LastValue;
  int         MoveTime;
  int        INC_Dir;
  int        Moving;
  int        Enabled;
}T_CTRL_Fail;

typedef struct
{
    int		AP_MODE;            /*有功联控方式  投入/退出*/
    int		Last_AP_MODE;
    int		Control_Right;         /* 控制权  厂控/梯调/国调 0为厂控、1为梯调、2为国调*/
    int		Closed_Loop;    /*开环/半开环/闭环*/
    int		Last_Closed_Loop;
    int		Last_Mode;
    int		Mode;           /*AGC调节方式(有功给定1/负荷曲线2/调频3/频率补偿4/无功能5)*/
    int		AP_Setting;      /*有功给定*/
    int		AP_Curve;        /*负荷曲线*/
    int		FR_Regulation;             /*调频*/
    int		FR_Compensation;  /*频率补偿模式*/
    int		Sel_Bus;        /*选择母线  母线I/母线II*/
    int		SR_AVAIL_Setting;	/*有功给定旋转备用投入*/
    int		SR_AVAIL_Curve;	   /*负荷曲线旋转备用投入*/
    int		AP_Faulty;			/*有功测量故障*/
    int		XAGC_Unit_Comp;		/*机组处于有功补偿方式下*/
    float	State_SetValue;		/*国调有功设定值*/
    float	Cascade_SetValue;	/*梯调有功设定值*/
    float	Operator_SetValue;	/*操作员设定有功值*/
    float	p_sum_no_agc;           /* 单控机组有功之和 */
    float	p_sum_agc;                      /* 联控机组总有功      */
    float	Last_p_sum_no_agc;
    float	p_sum;                          /* 全厂总有功       */
    float	AP_Setpoint;     /*全厂有功给定值*/
    float	XAP_Setpoint;
    float	YAP_Setpoint;
    int		Num_AP_Avail;       
    int		Last_Num_AP_Avail;
    int		AGC_Generating;         /* glfs, number of units in agc generation      */
    int		Last_AGC_Generating; 
    int		AGC_Standby;            /* glbs, numb of units in agc standby   */
    int		Generating_No_AGC;      /* unglfs, nmb of units in no agc generation    */
    int		Num_Unit_Run;			/*所有运行机组数*/
    float	UplmtCurrentZone;   /*不跨越振动区上限*/
    float	LowlmtCurrentZone;   /*不跨越振动区下限*/
    float	AP_Deadband;        /*有功调节死区*/
    float	WaterFlow;		/*发电流量*/
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
    int         APAnswerYesTime;
    int         SSAnswerYesTime;
    int         SSAnswerNoTime;
    int		FR_CTRL_Alarm;		/*频率控制生效*/
    int		XStrStp_Recalc;		/*开停机重计算*/
    int		SSReject_Num;		/*开停机拒绝次数*/
    int		XAP_Comp_Mode;
    int		XAP_Comp_Time;
    int         AP_Fail_Time;
    int         FR_Fail_Time;
    float	SR_SetPoint;    /*旋转备用设定值*/
    float       Last_SR_SetPoint;
    float       SR_MaxValue;
    float       SR_MaxChange;
    float	Real_SRValue;
    float	AP_ChangeMax;	/*有功设定值最大变幅*/
    float	FR_ChangeMax;	/*频率设定值最大变幅*/
    float	FR_Setpoint;	/*频率设定值*/
    float	XFR_Setpoint;
    float	YFR_Setpoint;
    float	FR_Deadband;	/*频率死区*/
    float	FR_CompDeadband;	/* 频率补偿死区*/
    float	FR_PowGenDB;     /*调频补偿调节幅度限值*/
    float	Freq_Power;     /*调频计算出的有功给定值*/
    float	FR_ActValue;	/*频率实际值*/  
    float       FR_CompPowBand;   /*max change*/
    float	XAP_FreqCTRL;	/*频率控制有功工作值*/
    float	AP_WorkSP;		/*有功功率工作设定值*/
    float	Last_AP_WorkSP;
    float	FR_WorkSP;      /*频率工作设定值*/
    float	Efficiency;		/*效率*/
    float	AGC_P_Set;      /* pagc */
    float	AP_Dev;     /*有功给定值是增加还是减少*/
    float       Start_AP_Dev;
    float       AP_Deviation;
    float	Dev_Frequency;  /*频率偏差*/
    float	AP_Min;		/*全厂最小总有功*/
    float	AP_Max;     /*全厂最大总有功*/
    float	XAP_Max;
    float       FR_Min;
    float       FR_Max;
    float	SS_Deadband;	/*开停机死区*/
    float	VZ_Deadband;	/*跨越振动区死区值*/
    float       EC_Deadband;
    float	XFreq_Output;	/*频率控制模式输出功率*/
    float	XFreqDoopAdaptFact; /*频率自适应系数*/
    float	XFreqMaxDev;	/*频率最大偏差*/
    int		XFreqDevDir;	/*频率偏差方向*/
    int		XStrStp_Delay;  /*开停机延时*/
    float	XAP_Comp_Step;	/*有功补偿步长*/
    int		XAP_DistributeDelay;	/*分配延时*/
    int		Lim_Change;			/*有功限值发生变化*/
    int         AP_Alm_Time;
    int         Offset_Enable;
    float       Curve[288];     /**/
    float       SR_Curve[288];
    float       Offset;
    int         CurvePointNum;  
    int         FirstRun;
    int         APCopyTime;
    int         SRCopyTime;
    int         JR_APCopyTime;
    int         JR_SRCopyTime;
    int		Setp_ErrorTime;
	char	English_Name[20];
	char    Chinese_Name[20];
#ifdef CASCADE
	float	yggd_pyl;
#endif
	int		AllocationToUnit;
	int		Last_AllocationToUnit;
} T_PLANT;   

typedef struct
{
    int		Status;            /*机组状态 */
    int		XStatus;
    int         Maint_Status;
    int		Last_Status;		/*上次机组状态*/
    int         AP_Sel;
    int         AP_Permit;
    int		AP_Avail;           /*机组是否联控可调*/
    int		Last_AP_Avail;		/*上次机组是否联控可调*/
    float	AP_RtValue;				/*实发有功*/
    float	Last_AP_RtValue;
    float       InputPower;
    int		AP_Faulty;			/*有功数据质量故障*/
    float	Standby_Time;         /*连续备用时间，单位：分*/
    float	Generating_Time;         /*连续发电时间，单位：分*/
    int		Start;			/*开机*/
    int		Stop;			/*停机*/
    int		XStop;			/*卸载负荷尚未发停机令*/
    int		Start_Req;      /*开机请求*/
    int		Stop_Req;       /*停机请求*/
    int		XAnswerNo;		/*拒绝*/
    int		segment;          /*机组所属电厂*/
    float	AP_Min;          /*机组运行下限*/
    float	AP_Max;          /*机组运行上限*/
    float       Last_AP_Min;
    float       Last_AP_Max;
    float	CurrentUplmt;                 /*当前不跨越振动区上限*/
    float	CurrentLowlmt;                /*当前不跨越振动区下限*/
    float       Privilege;     /*开停机优先级*/
    int		Type;			/*机组类型*/
    float	XAP_Setp;		/*机组分配的有功值*/
    float	AP_Setp;		/*机组分配的有功值*/
    float	YAP_Setp;
    float	ZAP_Setp;
    int		StartCmdTime;		/*开机命令时间*/
    int		StopCmdTime;		/*停机卸载时间*/
    int		StopTime;			/*机组停机监视时间（产生停机失败报警）*/
    int		StopAction;			/*标记为停机机组*/
    int		StartAction;		/*标注机组为启动机组*/
    int         StartProcess;
    int		Starting;			/*正在开机机组，由PLC读状态*/
    int		Stopping;			/*正在停机机组，由PLC读状态*/
    int         ReduceLoad;
    int		StartFlag;
    int		StopFlag;
    int		StartTime;         /*机组开机监视时间（产生开机失败报警）*/
    int		XAPCompMode;		/*有功补偿级别*/
    int		XPowerUp;
    int		Zone;				/*机组所处运行区*/
    int		Vib_Zone;			/*0/1表示不在/在振动区*/
    int		AllocationInVib;    /*分配有功在振动区*/
    int		XVibDelay;			/*统计机组在振动区时间*/
    int		XAP_Setp_Calc;		/*机组有新的设定值*/
    int		XStartUpTime;		/*机组并网后计时*/
    int		AP_Fail;			/*有功调节失败*/
    int		XStartBlockTime;	/*启动阻塞时间*/
    int		CommandTime;		/*开机或停机命令保持时间10秒*/
    int		APCompTime;			/*有功补偿时间*/
    int		StartBlockTime;     /*开机闭锁时间*/
    int		StopBlockTime;     /*停机闭锁时间*/
    int         AP_FailTime;
    int         ReduceLoadFailTime;
    float	Flow;				/*流量*/
    float       Efficiency;
    T_CTRL_Fail     XAP_Err_Detect;     /*监测有功调节*/
} T_UNIT;     /*机组工况*/



/*效率曲线一点*/
typedef struct {
  float         OutPow;        /*输出功率*/
  float         Eff;           /*效率efficiency*/
  float         Flow;          /*流量Water Flow*/
  unsigned char VibZone;       /*振动区信息Vibration zone */
                               
}T_EffRec;

/*某一水头效率曲线结构*/
typedef struct {
  float    WaterHead;                 /*水头*/
  float    MinPower;                  /*该水头下最小出力*/
  float    MaxPower;                  /*该水头下最大出力*/
  T_EffRec EffCurve[MAXDEFPOINTS+1];  /*25点效率曲线值*/
}T_EffCurve;

//Definition of one type of unit
typedef struct  {
  int       used;
  T_EffCurve WaterHead[MAXDEFPOINTS+1];
}T_EffSet;

//Calculated efficiency record for actual water head
typedef struct  {
  float         OutPow;        /*输出功率*/
  float         Eff;           /*效率Efficency*/
  float         Flow;          /*流量Water Flow*/
  float         InpPow;        /*输入功率 Input Power*/
  float         dInp_dOut;     /*Slope of Input Power devided by Output Power*/
  unsigned char VibZone;       /*振动区信息Vibration zone */
                               
}T_AGCRec;

//Calculated efficiency curve for actual water head
typedef struct  {
  float    WaterHead;                 /*水头*/
  float    MinPower;                  /*该水头下最小出力*/
  float    MaxPower;                  /*该水头下最大出力*/
  T_AGCRec EffCurve[MAXAGCPOINTS+1];  /*35点效率曲线值*/
}T_AGCCurve;

//Set for one vibration zone
typedef struct {
  int     Used;
  float    AP_Begin;                  //Active power at begin of zone
  float    AP_End;                    //Active power at end of zone
  float    AP_Size;                   //Size of zone
}T_VibSet ;

//Vibration zone for actual water head
typedef struct {
  int     Used;
  T_VibSet Vibration[VIBRATION_NUMB+1];
} T_AGCVibration;


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



char DB_NAME_QC_AGC[MAXSEGMENT][NAME_SIZE];
char DB_NAME_CLOSED_LOOP[MAXSEGMENT][NAME_SIZE];
char DB_NAME_CONTROL_RIGHT[MAXSEGMENT][NAME_SIZE];
char DB_NAME_SEL_BUS[MAXSEGMENT][NAME_SIZE];
char DB_NAME_AP_Setting[MAXSEGMENT][NAME_SIZE];
char DB_NAME_AP_Curve[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_Regulation[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_COMPENSATION[MAXSEGMENT][NAME_SIZE];
char DB_NAME_AP_RTVALUE[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_YG_GD[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_UNIT_STATUS[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_LXBYSJ[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_LXYXSJ[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_PRVILEGE[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_STATE_SETVALUE[MAXSEGMENT][NAME_SIZE];
char DB_NAME_CASECADE_SETVALUE[MAXSEGMENT][NAME_SIZE];
char DB_NAME_OPERATOR_SETVALUE[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_ACTVALUE[4][NAME_SIZE];
char DB_NAME_FR_SETPOINT[MAXSEGMENT][NAME_SIZE];
char DB_NAME_SR_SETPOINT[MAXSEGMENT][NAME_SIZE];
char DB_NAME_AP_MIN[MAXSEGMENT][NAME_SIZE];
char DB_NAME_AP_MAX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_AP_XMIN[MAXSEGMENT][NAME_SIZE];
char DB_NAME_AP_XMAX[MAXSEGMENT][NAME_SIZE]; //act value
char DB_NAME_FR_DEADBAND[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_CompDeadband[MAXSEGMENT][NAME_SIZE];
char DB_NAME_SS_DEADBAND[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_VZ_DEADBAND[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AP_ChangeMax[MAXSEGMENT][NAME_SIZE];
char DB_NAME_WATERHEAD[NAME_SIZE];
char DB_NAME_STARTING[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_STOPPING[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_SR_AVAIL_SETTING[MAXSEGMENT][NAME_SIZE];
char DB_NAME_SR_AVAIL_CURVE[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Maint_Status[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_UNIT_AGCSEL[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_UNIT_Permit[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AP_CZKT[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_UNIT_SETPOINT[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AP_ANSWERYES[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Start_Req[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_Stop_Req[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_Start_Cmd[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_Stop_Cmd[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_ReduceLoad[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_Efficiency[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Flow[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Real_APSetpoint[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Real_FrSetpoint[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Unit_Efficiency[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AP_RtSum[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RunInVibration[MAXSEGMENT][NAME_SIZE];
char DB_NAME_PlantStrStp_Req[MAXSEGMENT][NAME_SIZE];
char DB_NAME_SSAnswerYes[MAXSEGMENT][NAME_SIZE];
char DB_NAME_SSAnswerNo[MAXSEGMENT][NAME_SIZE];
char DB_NAME_AP_Deadband[MAXSEGMENT][NAME_SIZE];
char DB_NAME_AP_Fail[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_Fail[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_PowGenDB[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Offset_Enable[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Offset[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_Min[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_Max[MAXSEGMENT][NAME_SIZE];
char DB_NAME_EC_DEADBAND[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_ChangeMax[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Real_SRValue[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Unit_AP_Fail[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_Curve_Too_Small[MAXSEGMENT][NAME_SIZE];
char DB_NAME_APSetpoint_Err[MAXSEGMENT][NAME_SIZE];
char DB_NAME_AP_Dev_Alm[MAXSEGMENT][NAME_SIZE];
char DB_NAME_SR_MaxChange[MAXSEGMENT][NAME_SIZE];
char DB_NAME_SR_MaxValue[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Copy_AP_SD[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Copy_AP_XZ[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Copy_JR_AP_SD[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Copy_JR_AP_XZ[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VIBRATION1_MIN[VIBRATION_NUMB+1][NAME_SIZE];
char DB_NAME_VIBRATION1_MAX[VIBRATION_NUMB+1][NAME_SIZE];
char DB_NAME_VIBRATION2_MIN[VIBRATION_NUMB+1][NAME_SIZE];
char DB_NAME_VIBRATION2_MAX[VIBRATION_NUMB+1][NAME_SIZE];
char DB_NAME_VIBRATION3_MIN[VIBRATION_NUMB+1][NAME_SIZE];
char DB_NAME_VIBRATION3_MAX[VIBRATION_NUMB+1][NAME_SIZE];
char DB_NAME_MIN[MAXTYPES+1][NAME_SIZE];
char DB_NAME_MAX[MAXTYPES+1][NAME_SIZE];
char DB_NAME_MLKG[NAME_SIZE];
char DB_NAME_ONLINE_STA[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_STOP_STA[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AGC_Whead[NAME_SIZE];
char DB_NAME_Water_Level1[2][NAME_SIZE];
char DB_NAME_Water_Level2[2][NAME_SIZE];
char DB_NAME_JRFHQX_SD[MAXSEGMENT][NAME_SIZE];
char DB_NAME_MRFHQX_SD[MAXSEGMENT][NAME_SIZE];
char DB_NAME_JRFHQX_XZ[MAXSEGMENT][NAME_SIZE];
char DB_NAME_MRFHQX_XZ[MAXSEGMENT][NAME_SIZE];
char DB_NAME_TD_JRFHQX_SD[MAXSEGMENT][NAME_SIZE];
char DB_NAME_TD_JRFHQX_XZ[MAXSEGMENT][NAME_SIZE];
char DB_NAME_TD_MRFHQX_SD[MAXSEGMENT][NAME_SIZE];
char DB_NAME_TD_MRFHQX_XZ[MAXSEGMENT][NAME_SIZE];
char DB_NAME_TD_JRDYQX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_TD_MRDYQX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FailToSetp[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Gap[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Setp_Error[MAXSEGMENT][NAME_SIZE];
char DB_NAME_FR_REACT_VALUE[MAXSEGMENT][NAME_SIZE];

char DB_NAME_AllocationToUnit[MAXSEGMENT][NAME_SIZE];
#ifdef CASCADE
char DB_NAME_CasToPlantValue[MAXSEGMENT][NAME_SIZE];
char DB_NAME_YGGD_PYL[MAXSEGMENT][NAME_SIZE];
#endif

static float factor[]={0,1,1,1,1,1,1,1,1}; 


