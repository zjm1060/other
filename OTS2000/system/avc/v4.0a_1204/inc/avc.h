//#define SUN_OS
#define WINDOWS
#define	HOST_NAME	"HOSTNAME"

#define SCHEDULE    24
#define NAME_SIZE   40
#define MAX_UNIT_NUMB	20
#define	DATA_ERR	-1
#define	CHANN_FAIL	2
#define SIGN_MASK	0x80
#define NOT 0
#define YES 1
#define MAX_ITEM_NUMB	30

#define WGTJML		0xc00
#define TGLTHM	 0 	/* 调功令 */

#define DEBUG
#define PBG

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

struct tm  *ntime;
DMS_COMMON common_aa;
int UNIT_NUMB=0;
char   logfilename[50]="";
int move_curve=0;
int src_id=0;
int oldday=0;
int PQ_Enable=0;
int AVC_Loop=0;
int Voltage_Min,Voltage_Max;
float AP_RtValue[MAX_UNIT_NUMB+1];
int RP_Step[MAX_UNIT_NUMB]={0};
float DB_RP_WorkSP;
float MaxAdj_V;

int TypeNumber=0;
int Type_UnitNo[MAX_UNIT_NUMB+1][MAX_UNIT_NUMB+1]={0};
int ItemNum=0;
float AP_Power[MAX_UNIT_NUMB]={0};
float PQCurve[MAX_UNIT_NUMB+1][MAX_ITEM_NUMB][4];
char TypeKeyword[MAX_UNIT_NUMB+1][50];

typedef struct
{
        POINTER ptr;
        ANLG_DATA value;
        char   sec;
	    char   min;
}       COMMON_DEF;     /* 通讯数据包 */

typedef struct
{
    int			Type;		/*机组所属类型*/
	int			RP_Sel;		/*选择无功控制*/
	int			RP_Permit;    /*无功控制允许*/
	int			RP_Avail;	    /*机组无功联控可调*/
	int			Last_RP_Avail;  /*上次机组无功联控状态*/
	int			RP_Faulty;  /*机组无功测量值故障*/
	int			AP_Faulty;  /*机组有功测量值故障*/
	int			Status;		/*机组状态*/
    int			Maint_Status;    
	int			Privilege;	/*机组调节优先级*/
	int			RPCompMode;		/*机组补偿模式*/
    int         RPCompTime;
    int                 Lock_increase;    /*电压、电流等越限不可增无功*/
    int                 Lock_decrease;   /*电压低不可减无功*/
	float       RP_RtValue;	/*实际无功功率*/
	float       AP_RtValue;	/*有功实发值*/
	float       RP_Min;		/*单机最小无功出力*/
	float       RP_Max;		/*单机最大无功出力*/
    float       Last_RP_Min;
    float       Last_RP_Max;
	float       RP_Setp;		/*无功功率设定值*/
	float       XRP_Setp;	
    float       YRP_Setp;
    float       ZRP_Setp;
    int         RP_Setp_Calc;
    float       RP_MaxChange;
    int         RP_Fail;
    float				I_max;		/*定子电流限制*/
    float				LC_I_max;	/*转子电流限制*/
    float				U_max;		/*机端电压上限限制*/
    float				U_min;		/*机端电压下限限制*/
	int			Increase_Prohibit;
	int			Decrease_Prohibit;
}T_UNIT;

typedef struct 
{
	int     RP_MODE;        /*AVC联控总开关*/
    int     FirstRun;
    int		Mode;		/*AVC调节方式(无功给定1/电压给定2/电压曲线3/电压补偿4/无功能5)*/
	int		Control_Right;
    int		Last_Mode;
	int		AVC_Period;
    int     VO_Curve;               /*voltage curve mode*/
	int		RP_Setting;
    int     VO_Setting;
	int		VO_Regulation;		/*电压调节模式*/
	int     VO_Compensation;	/*电压补偿模式*/
    int     Close_Loop;	/*0为开环，1为闭环*/
	int		RP_Faulty;	/*无功测量值故障*/
	int     AP_Faulty;  /*有功测量值故障*/
	int     VO_Faulty;	/*电压测量值故障*/
	int     Num_RP_Avail;        /*无功联控可调机组台数*/
    int     Last_Num_Avail;
    int     GenNumb;
	int		XRP_Recalc;			/*循环次数计数*/
    int     Lim_Change;
    int     ALG_AVG;	/*无功平均分配方式*/
    int     ALG_EQUCOS;	/*等功率因数分配方式*/
    int                 Lock_RP_increase;    /*电压、电流等越限不可增无功*/
    int                 Lock_RP_decrease;   /*电压低不可减无功*/
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
    float   XVolt_Output;
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
	int		dqv;
}T_PLANT;


char DB_NAME_Curve_Low[SCHEDULE+1][NAME_SIZE];
char DB_NAME_Curve_High[SCHEDULE+1][NAME_SIZE];
//char DB_NAME_Tomorrow_Curve[SCHEDULE+1][NAME_SIZE];
char DB_NAME_Rt_Status[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_QC_AVC[NAME_SIZE];
char DB_NAME_AVC_KBH[NAME_SIZE];
char DB_NAME_Run_State[NAME_SIZE];
char DB_NAME_DY_GD[NAME_SIZE];
char DB_NAME_DYQX_GD[NAME_SIZE];
char DB_NAME_WGGD[NAME_SIZE];
char DB_NAME_AVC_AVGALG[NAME_SIZE];
char DB_NAME_AVC_EQUCOSALG[NAME_SIZE];
char DB_NAME_Control_Right[NAME_SIZE];
char DB_NAME_SD_Right_ON[NAME_SIZE];
char DB_NAME_SD_Right_OFF[NAME_SIZE];
char DB_NAME_ON_SD_CTRL[NAME_SIZE];
char DB_NAME_VO_Setpoint[NAME_SIZE];
char DB_NAME_VO_Remote_Setpoint[NAME_SIZE];
char DB_NAME_VO_DeadBand[NAME_SIZE];
char DB_NAME_DQV[NAME_SIZE];
char DB_NAME_V_MAX_CHANGE[NAME_SIZE];
char DB_NAME_VO_RtValue[7][NAME_SIZE];
char DB_NAME_AP_RtValue[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RP_RtValue[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RP_OptiValue[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RP_SetValue[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RP_Setpoint[NAME_SIZE];
char DB_NAME_RP_Remote_Set[NAME_SIZE];
char DB_NAME_RP_CZ[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RP_CZKT[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RP_LMT[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_RP_LLMT[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AVC_Period[NAME_SIZE];
char DB_NAME_WG_UP_LOCK[NAME_SIZE];
char DB_NAME_WG_DN_LOCK[NAME_SIZE];
char DB_NAME_AVC_UMAX[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AVC_UMIN[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AVC_IMAX[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_LC_I_MAX[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AVC_UAB[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AVC_I[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_AVC_LCI[MAX_UNIT_NUMB+1][NAME_SIZE];
char DB_NAME_Prvilege[MAX_UNIT_NUMB+1][NAME_SIZE];

T_UNIT  UNIT[MAX_UNIT_NUMB];
T_PLANT PLANT;
char    hostname[20];
float Curve_time_begin[SCHEDULE+1],Curve_time_end[SCHEDULE+1];