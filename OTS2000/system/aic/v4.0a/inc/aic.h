#include    "/home/ems/system/dms/v4.0a/inc/dms_structure_def.h"
#include    "/home/ems/system/dms/v4.0a/inc/dms_data_def.h"
#include    "/home/ems/system/dms/v4.0a/inc/dms_functions.h"

#include <time.h>

DMS_COMMON 	common_aa, common_bb, *common_aaa;

#define	CHANN_FAIL	2
#define DATA_GOOD	0
#define	DATA_ERR	-1
#define DATA_OVER_LMT	-5
#define FLT_E		0.05

#define NAME_SIZE   40
#define MAX_IRRIGATION_GROUP_NUMB	8//每个田间系统的最大轮灌组数8
#define MAX_FIELD_NUMB 5 //最大阀门数（田间系统数327）
#define MAX_IRRIGATION_TURN 4//最大灌溉次数 30
#define MAX_VALVE_NUM_IN_GROUP 10
#define MAX_FIELD_VALVE_NUM	350

/*#define KJML		1	
#define TJML		2*/
#define OPEN		1
#define CLOSE		0

#define WCZTHM	 0	/* no operation */
#define KJLTHM	 1	/* 开始轮灌令 */
#define TJLTHM	 2	/* 停止灌溉令 */


#define IN_STANDBY 0
#define IN_PUMPING 1
#define IN_PUMPED	2

//#define DEBUG

struct tm  *ntime;
DMS_COMMON common_aa;
int IRRIGATION_GROUP_NUMB=0,IRRIGATION_TURN_NUMB=0;
int FIELD_NUM=0;
//int move_curve=0;
//int src_id=0;
int oldmonth=0,oldday=0,oldhour=0,oldmin=0;
char   logfilename[50]="";
int AIC_Period=0;
int	AIC_Loop;



typedef struct
{
    int		Status[MAX_IRRIGATION_TURN+1];            /*轮灌组实时状态 */
    int		AP_Avail;           /*该组是否参与轮灌*/
    int		Pumping_Time[MAX_IRRIGATION_TURN+1];         /*需要灌溉时间，单位：小时*/
	int		Pumped_time[MAX_IRRIGATION_TURN+1];		/*已经灌溉时间*/
	time_t start_time[MAX_IRRIGATION_TURN+1];		/*开始灌溉时间*/
	time_t end_time[MAX_IRRIGATION_TURN+1];		/*关闭灌溉时间*/
    int		Start;			/*开始灌溉*/
    int		Stop;			/*停止灌溉*/
	int		cur_turn;
	char db_name_valve[MAX_VALVE_NUM_IN_GROUP+1][NAME_SIZE];
} T_IRRIGATION_GROUP;     /*轮灌组工况*/  

typedef struct
{
    int		Status[MAX_IRRIGATION_TURN+1];            /*阀门实时状态 */
    int		AP_Avail;           /*该阀门是否参与轮灌*/
    int		Pumping_Time[MAX_IRRIGATION_TURN+1];         /*本次需要灌溉时间，单位：小时*/
	int		Pumped_time[MAX_IRRIGATION_TURN+1];		/*本次已经灌溉时间*/
	time_t start_time[MAX_IRRIGATION_TURN+1];		/*开始灌溉时间*/
	time_t end_time[MAX_IRRIGATION_TURN+1];		/*关闭灌溉时间*/
    int		Start;			/*本次灌溉开始*/
    int		Stop;			/*本次灌溉停止*/	
	int cur_turn;
	char db_name_valve[NAME_SIZE];
} T_IRRIGATION_FIELD_GATE;     /*轮灌阀门工况*/

typedef struct
{
    int		Status;            /*田间系统实时状态 */
    int		AP_Avail;           /*该田间系统是否参与轮灌*/
    int		Pumping_Time[MAX_IRRIGATION_TURN+1];         /*本次需要灌溉时间，单位：小时*/
	int		Pumped_time[MAX_IRRIGATION_TURN+1];		/*本次已经灌溉时间*/
	time_t start_time[MAX_IRRIGATION_TURN+1];		/*开始灌溉时间*/
	time_t end_time[MAX_IRRIGATION_TURN+1];		/*关闭灌溉时间*/
    int		Start;			/*本次灌溉开始*/
    int		Stop;			/*本次灌溉停止*/
	T_IRRIGATION_GROUP Irrigation_Group[MAX_IRRIGATION_GROUP_NUMB+1];//田间系统的轮灌组
	int group_num;
	int Cur_Turn;
} T_IRRIGATION_FIELD;     /*田间系统工况*/

typedef struct
{
    int		AIC_state;            /*AIC  投入/退出*/
   // int		Control_Right;         /* 控制权  厂控/梯调/国调 0为厂控、1为梯调、2为国调*/
    int		Closed_Loop;    /*开环/闭环*/
	int		Field_num;
	T_IRRIGATION_FIELD_GATE Field_Gate[MAX_FIELD_NUMB+1];
	T_IRRIGATION_FIELD Field_Sys[MAX_FIELD_NUMB+1];
} T_PUMP; 

typedef struct
{
    int		Start[MAX_IRRIGATION_TURN+1];     /*下一轮灌组*/
    int		Stop[MAX_IRRIGATION_TURN+1];      /*下一停灌组*/
    int     CurvePointNum;  
	int	IrrigationGroup;//当前需灌溉的轮灌组号
} GROUP_PUMP;   



typedef struct
{
        int     ctrl_type;
        int     p;
}       CMD_DEF;        /*控制调节命令*/

typedef struct
{
        POINTER ptr;
        ANLG_DATA value;
        char   sec;
	    char   min;
}       COMMON_DEF;     /* 通讯数据包 */

T_PUMP PUMP;


char DB_NAME_AIC_Period[NAME_SIZE];
char DB_NAME_Control_Right[NAME_SIZE];
char DB_NAME_AIC_CLOSED_LOOP[NAME_SIZE];
char DB_NAME_QC_AIC[NAME_SIZE];


char DB_NAME_AP_Group_Field[MAX_FIELD_NUMB+1][NAME_SIZE];//田间系统参与轮灌
char DB_NAME_AP_Group_Group[MAX_FIELD_NUMB+1][MAX_IRRIGATION_GROUP_NUMB+1][NAME_SIZE];//轮灌组参与轮灌


char DB_NAME_FIELD_AP_CZKT[MAX_FIELD_NUMB+1][NAME_SIZE];//田间系统成组可控
char DB_NAME_GROUP_AP_CZKT[MAX_FIELD_NUMB+1][MAX_IRRIGATION_GROUP_NUMB+1][NAME_SIZE];//轮灌组成组可控

char DB_NAME_AIC_STATE[NAME_SIZE];//AIC执行/挂起状态


char DB_NAME_Group_StartTime[MAX_FIELD_NUMB+1][MAX_IRRIGATION_GROUP_NUMB+1][MAX_IRRIGATION_TURN+1][NAME_SIZE];
char DB_NAME_Group_EndTime[MAX_FIELD_NUMB+1][MAX_IRRIGATION_GROUP_NUMB+1][MAX_IRRIGATION_TURN+1][NAME_SIZE];
char DB_NAME_Group_PumpTime[MAX_FIELD_NUMB+1][MAX_IRRIGATION_GROUP_NUMB+1][MAX_IRRIGATION_TURN+1][NAME_SIZE];


char DB_NAME_Field_StartTime[MAX_FIELD_NUMB+1][MAX_IRRIGATION_TURN+1][NAME_SIZE];
char DB_NAME_Field_EndTime[MAX_FIELD_NUMB+1][MAX_IRRIGATION_TURN+1][NAME_SIZE];
char DB_NAME_Field_PumpTime[MAX_FIELD_NUMB+1][MAX_IRRIGATION_TURN+1][NAME_SIZE];

char DB_NAME_Field_Control_Command[MAX_FIELD_NUMB+1][NAME_SIZE];
char DB_NAME_IrrigationGroup_Control_Command[MAX_FIELD_NUMB+1][MAX_IRRIGATION_GROUP_NUMB+1][NAME_SIZE];

char DB_NAME_Field_Status[MAX_FIELD_NUMB+1][NAME_SIZE];
char DB_NAME_Group_Status[MAX_FIELD_NUMB+1][MAX_IRRIGATION_GROUP_NUMB+1][NAME_SIZE];


