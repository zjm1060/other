
#define CASCADE

#define	UNIT_NUMB		8
#define DATA_ERROR		-1000
#define DATA_BAD		-1001
#define OVER_HIGH_LMT           -1002

#define	MAXPQPOINTS	21
#define MAXTYPES	2
#define MAXSEGMENT      2
#define NAME_SIZE        36

#define OPEN_LOOP   0
#define CLOSE_LOOP  1

//Detection of unit power control failure
typedef struct  
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
	int		Type;		/*机组所属类型*/
	int		RP_Sel;		/*选择无功控制*/
	int		RP_Permit;    /*无功控制允许*/
	int		RP_Avail;	    /*机组无功联控可调*/
	int		Last_RP_Avail;  /*上次机组无功联控状态*/
	int		RP_Faulty;  /*机组无功测量值故障*/
	int		AP_Faulty;  /*机组有功测量值故障*/
	int		segment;        /*机组所属组*/
	int		Status;		/*机组状态*/
	int		Last_Status;
	int		XStatus;
	int             Maint_Status;
	int		RPCompMode;		/*机组补偿模式*/
	int             RPCompTime;
	float           RP_RtValue;	/*实际无功功率*/
	float           AP_RtValue;	/*有功实发值*/
	float           RP_Min;		/*单机最小无功出力*/
	float           RP_Max;		/*单机最大无功出力*/
	float           Last_RP_Min;
	float           Last_RP_Max;
	float           RP_Setp;		/*无功功率设定值*/
	float           XRP_Setp;	
	float           YRP_Setp;
	float           ZRP_Setp;
	int             RP_Setp_Calc;
	float           RP_MaxChange;
	int             RP_Fail;
	int             ReduceLoad;
	int             Last_ReduceLoad;
	int             RP_FailTime;
	T_CTRL_Fail     XRP_Err_Detect;
}T_UNIT;

typedef struct 
{
	int     RP_MODE;        /*AVC联控总开关*/
	int	Last_RP_MODE;
	int     FirstRun;
	int	Mode;		/*AVC调节方式(无功给定1/电压给定2/电压曲线3/电压补偿4/无功能5)*/
	int	Last_Mode;
	int	VO_Regulation;		/*电压调节模式*/
	int     VO_Curve;               /*voltage curve mode*/
	int     RP_Setting;
	int     VO_Compensation;	/*电压补偿模式*/
	int	Sel_Bus;	/*选择I或II母为主母线，0为I母，1为II母*/
	int     Close_Loop;	/*0为开环，1为闭环*/
	int	Last_Close_Loop;
	int	RP_Faulty;	/*无功测量值故障*/
	int     AP_Faulty;  /*有功测量值故障*/
	int     VO_Faulty;	/*电压测量值故障*/
	int     Num_RP_Avail;        /*无功联控可调机组台数*/
	int     Last_Num_Avail;
	int     GenNumb;
	int	VO_CTRL_Alarm;      /*电压控制报警*/
	int	XAVC_Unit_Comp;		/*机组处于功率补偿模式*/
	int     XRP_Comp_Time;
	int	XVoltDevDir;		/*电压偏差方向*/
	int	XRP_Recalc;			/*循环次数计数*/
	int     RPAnswerYes;
	int     RPAnswerYesTime;
	int     Lim_Change;
	int     CompLimPos;
	float   RP_WorkSP;
	float	RP_Setpoint;	/*当前无功设定值*/
	float	XRP_Setpoint;
	float	YRP_Setpoint;
	float	RP_Deadband;		/*无功功率死区*/
	float	XRP_Min;				/*无功最小工作值*/
	float   XRP_Max;				/*无功最大工作值*/
	float	Volt_Power;       /*电压给定或电压曲线方式计算出的所需调节无功值*/
	float	VO_Deadband;	/*电压死区值*/
	float   Comp_Deadband;    
	float	VO_Work;		/*电压工作设定值*/
	float   VO_Deviation;		/*电压偏差*/
	float	VO_RtValue;		/*电压实时值*/
	float	VO_Min;				/*电压最小值*/
	float   VO_Max;				/*电压最大值*/
	float	RP_Min;				/*无功最小值－界面设置*/
	float   RP_Max;				/*无功最大值－界面设置*/
	float	VO_Setpoint;		/*电压设定值*/
	float   XVO_Setpoint;
	float   YVO_Setpoint;
	float	VO_PowGenDB;		/*电压补偿下无功调节幅度限值*/
	float	XRP_VoltCTRL;		/*上次无功控制值*/
	float   XVoltMaxDev;
	float	XVoltDoopAdaptFact;	/*电压调差自适应系数*/
	float   XVolt_Output;
	float   Last_XVolt_Output;
	float   VO_MaxChange;
	float   RP_MaxChange;
	float   RP_Dev;
	float   RP_Deviation;
	float   P_Sum;
	float   P_Sum_AVC;
	float   RP_Sum;
	float   RP_Sum_AVC;
	float   RP_Sum_NotAVC;
	float   Last_RP_Sum_NotAVC;
	int     RP_Alarm_Time;
	int     RP_Fail_Time;
	int     VO_Fail_Time;
	int     RPCopyTime;
	int     JR_RPCopyTime;
	float   Curve[288];
	int     CurvePointNum;
	char    English_Name[20];
	char    Chinese_Name[20];
	int		Control_Right;         /* 控制权  厂控/梯调 0为厂控、1为梯调*/
	int		AllocationToUnit;
	int		Last_AllocationToUnit;
}T_PLANT;

//P/Q Curve for one voltage
typedef struct {
	float    AP[MAXPQPOINTS+1];  //有功百分比
	float    RP_OE[MAXPQPOINTS+1];  //过励情况无功功率的百分比
	float    RP_UE[MAXPQPOINTS+1];  //欠励情况下无功功率的百分比
}T_P_Q_Curve;

typedef struct {
	float    S_Total;    //视在功率
	T_P_Q_Curve Voltage;
} T_P_Q_Set;

char DB_NAME_QC_AVC[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_RTVALUE[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_WG_GD[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AP_RTVALUE[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_UNIT_STATUS[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_CLOSED_LOOP[MAXSEGMENT][NAME_SIZE];
char DB_NAME_SEL_BUS[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_REGULATION[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_COMPENSATION[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_CURVE[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_SETTING[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_RTVALUE[4][NAME_SIZE];
char DB_NAME_RP_CZKT[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RP_DEADBAND[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_DEADBAND[MAXSEGMENT][NAME_SIZE];
char DB_NAME_COMP_DEADBAND[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_MIN[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_MAX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_XMIN[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_XMAX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_MIN[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_MAX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_PowGenDB[MAXSEGMENT][NAME_SIZE];
char DB_NAME_UNIT_AVCSEL[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RP_AVAIL[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RP_CZKT[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_VO_SETPOINT[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_SETPOINT[MAXSEGMENT][NAME_SIZE];
char DB_NAME_UNIT_SETPOINT[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RP_ANSWERYES[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_MAXCHANGE[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_MAXCHANGE[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_RCTSETPOINT[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_RCTSETPOINT[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_FAIL[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_FAIL[MAXSEGMENT][NAME_SIZE];
char DB_NAME_RP_Sum[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Maint_Status[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_ReduceLoad[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_Unit_RP_Fail[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RP_Dev_Alm[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Copy_JRDYQX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_Copy_MRDYQX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_MLKG[NAME_SIZE];
char DB_NAME_ONLINE_STA[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_STOP_STA[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_OELimit[MAXSEGMENT][NAME_SIZE];
char DB_NAME_UELimit[MAXSEGMENT][NAME_SIZE];
char DB_NAME_JRDYQX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_MRDYQX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_TD_JRDYQX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_TD_MRDYQX[MAXSEGMENT][NAME_SIZE];
char DB_NAME_VO_REACT_VALUE[MAXSEGMENT][NAME_SIZE];
char DB_NAME_WG_GD_CAS[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_CTRL_PRI[UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_GD_VO_SETPOINT[MAXSEGMENT][NAME_SIZE];

char DB_NAME_AllocationToUnit[MAXSEGMENT][NAME_SIZE];

#ifdef CASCADE
char DB_NAME_CasToPlantValue[MAXSEGMENT][NAME_SIZE];
#endif
