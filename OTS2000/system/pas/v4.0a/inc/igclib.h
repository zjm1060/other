#define DEBUG

#ifdef		_WIN32
#define		WINDOWS_OS
#else
#define     SUN_OS
#endif


#ifdef SUN_OS
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include <time.h>
#else
#include<fcntl.h>
#include<stdio.h>
#include <time.h>
#endif

#include	"../../../dms/v4.0a/inc/syslog.h"
#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../pas/v4.0a/inc/pdc_def.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include	"../../../sms/v4.0a/inc/user_privilege_def.h"
#include	"../../../dps/v4.0a/inc/fault_table_def.h" 
#include	"../../../oix/v4.0a/inc/alarm_str.h"
#include	"../../../dps/v4.0a/inc/dps_functions_def.h"

#define  IGC_CMDTYPE_C_UNIT 49//机组命令
#define  IGC_CMDTYPE_OPER_C_UNIT_start_NUM 1
#define  IGC_CMDTYPE_OPER_C_UNIT_stop_NUM 44
char *IGC_CMDTYPE_OPER_C_UNIT[]= {	
	"NULL",         //0
	"Start",        //1.   发电
	"Stop",         //2.   停机
	"Reset",        //3.   流程手动复归令
	"Step",         //4.   单步操作令
	"Jump",         //5.   跳转令
	"NoVoltRun",    //6.   空转
	"NoLoadRun",    //7.   空载
	"BlackStart",   //8.   机组黑启动
	"CmpStart",     //9.   机组调相令
	"EmergStop",    //10.  紧急停机
	"Open",         //11.  解列
	"ReadyStart",   //12.  开机准备
	"TransNoLoad",  //13.  主变空载(开机假并网令)
	"ZeroVRun",     //14.  零起升压
	"TranZVRun",    //15.  带主变零起升压
	"ZeroVStop",    //16.  零起升压转备用
	"NULL",         //17.  
	"NULL",         //18.  
	"NULL",         //19.  
	"StartToCool",  //20.  开机至冷却水令
	"StartToSpin",  //21.  开机至空转令
	"StartToNLoad", //22.  开机至空载令
	"StartToOnLin", //23.  开机至并网令
	"StopToNLoad",  //24.  停机至空载令
	"StopToSpin",   //25.  停机至空转令
	"StopToCool",   //26.  停机至冷却水令
	"StopToStand",  //27.  停机至全停令
	"StartStep",    //28.  单步开机启动流程令
	"StopStep",     //29.  单步停机启动流程令
	"EmergStep",    //30.  单步紧急停机启动流程令
	"FastStopStep", //31.  单步快速停机启动流程令
	"PumpStart",    //32.  开泵
	"PumpStop",     //33.  停泵
	"DragStart",    //34.  对拖发电
	"SFCHSStart",   //35.  变频启动高转速抽水
	"SFCLSStart",   //36.  变频启动低转速抽水
	"SFCHSCmp",     //37.  变频启动高转速水泵调相
	"SFCLSCmp",     //38.  变频启动低转速水泵调相
	"DragHSStart",  //39.  对拖启动高转速抽水
	"DragLSStart",  //40.  对拖启动低转速抽水
	"DragHSCmp",    //41.  对拖启动高转速水泵调相
	"DragLSCmp",    //42.  对拖启动低转速水泵调相
	"PumpToGe",     //43.  抽水转发电
	"GenToPump"    //44.  发电转抽水
};


enum IGC_ENUM_CMDTYPE_C_UNIT  {	
		CMDTYPE_C_UNIT_Start =1,     //1.   发电
		CMDTYPE_C_UNIT_Stop,         //2.   停机
		CMDTYPE_C_UNIT_Reset,        //3.   流程手动复归令
		CMDTYPE_C_UNIT_Step,         //4.   单步操作令
		CMDTYPE_C_UNIT_Jump,         //5.   跳转令
		CMDTYPE_C_UNIT_NoVoltRun,    //6.   空转
		CMDTYPE_C_UNIT_NoLoadRun,    //7.   空载
		CMDTYPE_C_UNIT_BlackStart,   //8.   机组黑启动
		CMDTYPE_C_UNIT_CmpStart,     //9.   机组调相令
		CMDTYPE_C_UNIT_EmergStop,    //10.  紧急停机
		CMDTYPE_C_UNIT_Open,         //11.  解列
		CMDTYPE_C_UNIT_ReadyStart,   //12.  开机准备
		CMDTYPE_C_UNIT_TransNoLoad,  //13.  主变空载(开机假并网令)
		CMDTYPE_C_UNIT_ZeroVRun,     //14.  零起升压
		CMDTYPE_C_UNIT_TranZVRun,    //15.  带主变零起升压
		CMDTYPE_C_UNIT_ZeroVStop,    //16.  零起升压转备用
		CMDTYPE_C_UNIT_StartToCool=20,  //20.  开机至冷却水令
		CMDTYPE_C_UNIT_StartToSpin,  //21.  开机至空转令
		CMDTYPE_C_UNIT_StartToNLoad, //22.  开机至空载令
		CMDTYPE_C_UNIT_StartToOnLin, //23.  开机至并网令
		CMDTYPE_C_UNIT_StopToNLoad,  //24.  停机至空载令
		CMDTYPE_C_UNIT_StopToSpin,   //25.  停机至空转令
		CMDTYPE_C_UNIT_StopToCool,   //26.  停机至冷却水令
		CMDTYPE_C_UNIT_StopToStand,  //27.  停机至全停令
		CMDTYPE_C_UNIT_StartStep,    //28.  单步开机启动流程令
		CMDTYPE_C_UNIT_StopStep,     //29.  单步停机启动流程令
		CMDTYPE_C_UNIT_EmergStep,    //30.  单步紧急停机启动流程令
		CMDTYPE_C_UNIT_FastStopStep, //31.  单步快速停机启动流程令
		CMDTYPE_C_UNIT_PumpStart,    //32.  开泵
		CMDTYPE_C_UNIT_PumpStop,     //33.  停泵
		CMDTYPE_C_UNIT_DragStart,    //34.  对拖发电
		CMDTYPE_C_UNIT_SFCHSStart,   //35.  变频启动高转速抽水
		CMDTYPE_C_UNIT_SFCLSStart,   //36.  变频启动低转速抽水
		CMDTYPE_C_UNIT_SFCHSCmp,     //37.  变频启动高转速水泵调相
		CMDTYPE_C_UNIT_SFCLSCmp,     //38.  变频启动低转速水泵调相
		CMDTYPE_C_UNIT_DragHSStart,  //39.  对拖启动高转速抽水
		CMDTYPE_C_UNIT_DragLSStart,  //40.  对拖启动低转速抽水
		CMDTYPE_C_UNIT_DragHSCmp,    //41.  对拖启动高转速水泵调相
		CMDTYPE_C_UNIT_DragLSCmp,    //42.  对拖启动低转速水泵调相
		CMDTYPE_C_UNIT_PumpToGe,     //43.  抽水转发电
		CMDTYPE_C_UNIT_GenToPump     //44.  发电转抽水
};

#define  IGC_CMDTYPE_C_BREAKER 50//断路器
#define  IGC_CMDTYPE_OPER_C_BREAKER_start_NUM 0
#define  IGC_CMDTYPE_OPER_C_BREAKER_stop_NUM 15
char *IGC_CMDTYPE_OPER_C_BREAKER[]= {	
	"Reset",          //0. 故障复归
	"Open",           //1. 分闸
	"Close",          //2. 合闸
	"IM_Line",        //3. I母对线路充电合
	"IIM_Line",       //4. II母对线路充电合
	"Line_IM",        //5. 线路对I母充电合
	"Line_IIM",       //6. 线路对II母充电合
	"Tran_IM",        //7. 变压器对I母充电合
	"Tran_IIM",       //8. 变压器对II母充电合
	"IM_Tran",        //9. I母对变压器充电合
	"IIM_Tran",       //10.II母对变压器充电合
	"TestClose",      //11.试验合
	"CancelSyn",      //12.同期退出
	"LoopClose",      //13.环网合
	"ChgVltCls",      //14.充电合
	"ZroVltCls"       //15.无压合
};

enum IGC_ENUM_CMDTYPE_C_BREAKER  {	
	CMDTYPE_C_BREAKER_Reset=0,        //0. 故障复归
	CMDTYPE_C_BREAKER_Open,           //1. 分闸
	CMDTYPE_C_BREAKER_Close,          //2. 合闸
	CMDTYPE_C_BREAKER_IM_Line,        //3. I母对线路充电合
	CMDTYPE_C_BREAKER_IIM_Line,       //4. II母对线路充电合
	CMDTYPE_C_BREAKER_Line_IM,        //5. 线路对I母充电合
	CMDTYPE_C_BREAKER_Line_IIM,       //6. 线路对II母充电合
	CMDTYPE_C_BREAKER_Tran_IM,        //7. 变压器对I母充电合
	CMDTYPE_C_BREAKER_Tran_IIM,       //8. 变压器对II母充电合
	CMDTYPE_C_BREAKER_IM_Tran,        //9. I母对变压器充电合
	CMDTYPE_C_BREAKER_IIM_Tran,       //10.II母对变压器充电合
	CMDTYPE_C_BREAKER_TestClose,      //11.试验合
	CMDTYPE_C_BREAKER_CancelSyn,      //12.同期退出
	CMDTYPE_C_BREAKER_LoopClose,      //13.环网合
	CMDTYPE_C_BREAKER_ChgVltCls,      //14.充电合
	CMDTYPE_C_BREAKER_ZroVltCls       //15.无压合
};

#define  IGC_CMDTYPE_C_SWITCH 51//隔离开关
#define  IGC_CMDTYPE_OPER_C_SWITCH_start_NUM 0
#define  IGC_CMDTYPE_OPER_C_SWITCH_stop_NUM 4
char *IGC_CMDTYPE_OPER_C_SWITCH[]= {	
	    "Reset",        //0. 故障复归
		"Open",         //1.  分闸
		"Close",        //2.  合闸
		"TestOpen",     //3.  试验分闸
		"TestClose"     //4.  试验合闸
};

enum IGC_ENUM_CMDTYPE_C_SWITCH  {	
		CMDTYPE_C_SWITCH_Reset,        //0. 故障复归
		CMDTYPE_C_SWITCH_Open,         //1.  分闸
		CMDTYPE_C_SWITCH_Close,        //2.  合闸
		CMDTYPE_C_SWITCH_TestOpen,     //3.  试验分闸
		CMDTYPE_C_SWITCH_TestClose     //4.  试验合闸
};



#define  IGC_CMDTYPE_C_APOWER 52//有功功率
#define  IGC_CMDTYPE_OPER_C_APOWER_start_NUM 0
#define  IGC_CMDTYPE_OPER_C_APOWER_stop_NUM 2
char *IGC_CMDTYPE_OPER_C_APOWER[]= {	
	"Power",      //0.  功率调节
	"Openning",   //1.  开度调节
	"Flux"        //2.  流量调节
};

enum IGC_ENUM_CMDTYPE_C_APOWER  {	
	    CMDTYPE_C_APOWER_Power,     //0.  功率调节
		CMDTYPE_C_APOWER_Openning,  //1.  开度调节
		CMDTYPE_C_APOWER_Flux       //2.  流量调节
};



#define  IGC_CMDTYPE_C_RPOWER 53//无功功率
#define  IGC_CMDTYPE_OPER_C_RPOWER_start_NUM 0
#define  IGC_CMDTYPE_OPER_C_RPOWER_stop_NUM 2
char *IGC_CMDTYPE_OPER_C_RPOWER[]= {	
	"Power",      //0.  功率调节
	"Volt",       //1.  电压给定
	"Cos"         //2.  功率因数给定
};

enum IGC_ENUM_CMDTYPE_C_RPOWER  {	
	    CMDTYPE_C_RPOWER_Power,      //0.  功率调节
		CMDTYPE_C_RPOWER_Volt,       //1.  电压给定
		CMDTYPE_C_RPOWER_Cos         //2.  功率因数给定	
};


#define  IGC_CMDTYPE_C_DEVICE 54//一般设备
#define  IGC_CMDTYPE_OPER_C_DEVICE_start_NUM 0
#define  IGC_CMDTYPE_OPER_C_DEVICE_stop_NUM 2
char *IGC_CMDTYPE_OPER_C_DEVICE[]= {	
	"Reset",       //0. 故障复归
	"Off",         //1. 设备退出
	"On"           //2. 设备投入
};

enum IGC_ENUM_CMDTYPE_C_DEVICE  {	
    	CMDTYPE_C_DEVICE_Reset,       //0. 故障复归
		CMDTYPE_C_DEVICE_Off,         //1. 设备退出
		CMDTYPE_C_DEVICE_On           //2. 设备投入		
};

#define  IGC_CMDTYPE_C_GATE 55//闸门
#define  IGC_CMDTYPE_OPER_C_GATE_start_NUM 0
#define  IGC_CMDTYPE_OPER_C_GATE_stop_NUM 16
char *IGC_CMDTYPE_OPER_C_GATE[]= {	
	"Reset",       //0. 故障复归
	"Open",        //1. 开闸门
	"Close",       //2. 关闸门
	"Stop",        //3. 停止操作
	"Up",          //4. 提升(高度)
	"Down",        //5. 下降(高度)
	"FastOpen",    //6. 快速开闸门
	"FastClose",   //7. 快速关闸门
	"StepUp",      //8. 步进提升(固定高度)
	"StepDown",    //9. 步进下降(固定高度)
	"Forward",     //10.前移
	"Backward",    //11.后撤
	"SetZero",     //12.控制调零设定
	"SetFlux",     //13.流量设定
	"SmoothPress", //14.手动平压
	"SetOpening",  //15.开度设置
	"SetPos"       //16.高度设置
};

enum IGC_ENUM_CMDTYPE_C_GATE  {		
    	CMDTYPE_C_GATE_Reset,       //0. 故障复归
		CMDTYPE_C_GATE_Open,        //1. 开闸门
		CMDTYPE_C_GATE_Close,       //2. 关闸门
		CMDTYPE_C_GATE_Stop,        //3. 停止操作
		CMDTYPE_C_GATE_Up,          //4. 提升(高度)
		CMDTYPE_C_GATE_Down,        //5. 下降(高度)
		CMDTYPE_C_GATE_FastOpen,    //6. 快速开闸门
		CMDTYPE_C_GATE_FastClose,   //7. 快速关闸门
		CMDTYPE_C_GATE_StepUp,      //8. 步进提升(固定高度)
		CMDTYPE_C_GATE_StepDown,    //9. 步进下降(固定高度)
		CMDTYPE_C_GATE_Forward,     //10.前移
		CMDTYPE_C_GATE_Backward,    //11.后撤
		CMDTYPE_C_GATE_SetZero,     //12.控制调零设定
		CMDTYPE_C_GATE_SetFlux,     //13.流量设定
		CMDTYPE_C_GATE_SmoothPress, //14.手动平压
		CMDTYPE_C_GATE_SetOpening,  //15.开度设置
		CMDTYPE_C_GATE_SetPos       //16.高度设置
};


#define  IGC_CMDTYPE_C_TRANS 56//主变抽头
#define  IGC_CMDTYPE_OPER_C_TRANS_start_NUM 0
#define  IGC_CMDTYPE_OPER_C_TRANS_stop_NUM 5
char *IGC_CMDTYPE_OPER_C_TRANS[]= {	
	"Reset",      //0. 故障复归
	"Raise",      //1. 上升
	"Down",       //2. 下降
	"Stop",       //3. 停止操作
	"RaiseTo",    //4. 上升至(抽头位置)
	"DownTo",     //5. 下降至(抽头位置)
};

enum IGC_ENUM_CMDTYPE_C_TRANS  {		
	    CMDTYPE_C_TRANS_Reset,      //0. 故障复归
		CMDTYPE_C_TRANS_Raise,      //1. 上升
		CMDTYPE_C_TRANS_Down,       //2. 下降
		CMDTYPE_C_TRANS_Stop,       //3. 停止操作
		CMDTYPE_C_TRANS_RaiseTo,    //4. 上升至(抽头位置)
		CMDTYPE_C_TRANS_DownTo,     //5. 下降至(抽头位置)	
};


#define  IGC_CMDTYPE_C_VOLT_STEP 60//单步升压
#define  IGC_CMDTYPE_OPER_C_VOLT_STEP_start_NUM 1
#define  IGC_CMDTYPE_OPER_C_VOLT_STEP_stop_NUM 2
char *IGC_CMDTYPE_OPER_C_VOLT_STEP[]= {	
	"NULL",      //0. 
	"Decrease",  //1. 单步降压
	"Increase",  //2. 单步升压
};
enum IGC_ENUM_CMDTYPE_C_VOLT_STEP  {		
    	CMDTYPE_C_VOLT_STEP_Decrease=1,  //1. 单步降压
		CMDTYPE_C_VOLT_STEP_Increase     //2. 单步升压
};

#define  IGC_CMDTYPE_C_SPEED_STEP 61//单步升频
#define  IGC_CMDTYPE_OPER_C_SPEED_STEP_start_NUM 1
#define  IGC_CMDTYPE_OPER_C_SPEED_STEP_stop_NUM 2
char *IGC_CMDTYPE_OPER_C_SPEED_STEP[]= {	
	"NULL",      //0. 
	"Decrease",  //1. 单步降频
	"Increase"   //2. 单步升频
};

enum IGC_ENUM_CMDTYPE_C_SPEED_STEP  {		
	    CMDTYPE_C_SPEED_STEP_Decrease=1,  //1. 单步降频
		CMDTYPE_C_SPEED_STEP_Increase     //2. 单步升频
};

#define  IGC_CMDTYPE_C_ONOFF_FORCE 62//开关量强制
#define  IGC_CMDTYPE_OPER_C_ONOFF_FORCE_start_NUM 0
#define  IGC_CMDTYPE_OPER_C_ONOFF_FORCE_stop_NUM 2
char *IGC_CMDTYPE_OPER_C_ONOFF_FORCE[]= {	
    "UnForce",     //0.解除强置 
	"ForceOff",    //1.强置分
	"ForceOn"      //2.强置合
};

enum IGC_ENUM_CMDTYPE_C_ONOFF_FORCE  {		    
	CMDTYPE_C_ONOFF_FORCE_UnForce,     //0.解除强置 
	CMDTYPE_C_ONOFF_FORCE_ForceOff,    //1.强置分
	CMDTYPE_C_ONOFF_FORCE_ForceOn      //2.强置合
};



#define  IGC_CMDTYPE_C_ANA_FORCE 63//模拟量强制
#define  IGC_CMDTYPE_OPER_C_ANA_FORCE_start_NUM 0
#define  IGC_CMDTYPE_OPER_C_ANA_FORCE_stop_NUM 1
char *IGC_CMDTYPE_OPER_C_ANA_FORCE[]= {	
    "UnForce",     //0.解除强置 
	"Force"        //1.强置模出值
};

enum IGC_ENUM_CMDTYPE_C_ANA_FORCE  {		    
	CMDTYPE_C_ANA_FORCE_UnForce,     //0.解除强置 
	CMDTYPE_C_ANA_FORCE_Force        //1.强置模出值
}; 

#define  IGC_CMDTYPE_C_IO_CTRL_BLOCK 64//IO控制退出命令格式
#define  IGC_CMDTYPE_OPER_C_IO_CTRL_BLOCK_start_NUM 0
#define  IGC_CMDTYPE_OPER_C_IO_CTRL_BLOCK_stop_NUM 1
char *IGC_CMDTYPE_OPER_C_IO_CTRL_BLOCK[]= {	
    "Ctrl_UnBlock",     //0. IO控制恢复 
	"Ctrl_Block"        //1. IO控制退出
};

enum IGC_ENUM_CMDTYPE_C_IO_CTRL_BLOCK  {		    
	CMDTYPE_C_IO_CTRL_BLOCK_Ctrl_UnBlock,     //0. IO控制恢复 
	CMDTYPE_C_IO_CTRL_BLOCK_Ctrl_Block        //1. IO控制退出
};


#define  IGC_CMDTYPE_C_DEV_MODE 57//IO控制退出命令格式
#define  IGC_CMDTYPE_OPER_C_DEV_MODE_start_NUM 0
#define  IGC_CMDTYPE_OPER_C_DEV_MODE_stop_NUM 3
char *IGC_CMDTYPE_OPER_C_DEV_MODE[]= {	
    "Off",       //0. 退出 
	"On",        //1. 投入
	"On2",       //2. 投入运行方式2
	"On3"        //3. 投入运行方式3
};
enum IGC_ENUM_CMDTYPE_C_DEV_MODE  {		    
	CMDTYPE_C_DEV_MODE_Off,     //0. 退出 
	CMDTYPE_C_DEV_MODE_On,      //1. 投入
	CMDTYPE_C_DEV_MODE_On2,     //2. 投入运行方式2
	CMDTYPE_C_DEV_MODE_On3      //3. 投入运行方式3
};

#define  IGC_CMDTYPE_C_SIGNAL_RESET 83//信号复归命令格式
#define  IGC_CMDTYPE_OPER_C_SIGNAL_RESET_start_NUM 1
#define  IGC_CMDTYPE_OPER_C_SIGNAL_RESET_stop_NUM 2
char *IGC_CMDTYPE_OPER_C_SIGNAL_RESET[]= {	
	"NULL",            //0. 
    "FaultReset",      //1. 事故信号复归 
	"FailReset"        //2. 故障信号复归
};

enum IGC_ENUM_CMDTYPE_C_SIGNAL_RESET  {		    
	CMDTYPE_C_SIGNAL_RESET_FaultReset=1,      //1. 事故信号复归 
	CMDTYPE_C_SIGNAL_RESET_FailReset        //2. 故障信号复归
};


#define  IGC_CMDTYPE_C_LCU_ONOFF 48//LCU在线/离线命令
#define  IGC_CMDTYPE_OPER_C_LCU_ONOFF_start_NUM 1
#define  IGC_CMDTYPE_OPER_C_LCU_ONOFF_stop_NUM 2
char *IGC_CMDTYPE_OPER_C_LCU_ONOFF[]= {	
	"NULL",         //0. 
    "Offline",      //1. 设备离线 
	"Online"        //2. 设备在线
};

enum IGC_ENUM_CMDTYPE_C_LCU_ONOFF  {		    
	CMDTYPE_C_LCU_ONOFF_Offline=1,      //1. 设备离线 
	CMDTYPE_C_LCU_ONOFF_Online,         //2. 设备在线
};

#define  IGC_CMDTYPE_TIME_SYNC 47//LCU时钟同步
#define  IGC_CMDTYPE_OPER_TIME_SYNC_start_NUM 0
#define  IGC_CMDTYPE_OPER_TIME_SYNC_stop_NUM 0
char *IGC_CMDTYPE_OPER_TIME_SYNC[]= {	
    "TimeSync"      //0. 同步 
};

enum IGC_ENUM_CMDTYPE_TIME_SYNC  {		    
	CMDTYPE_TIME_SYNC_TimeSync
};



#define  IGC_CMDTYPE_C_HOST_SYS 69//LCU在线/离线命令
#define  IGC_CMDTYPE_OPER_C_HOST_SYS_start_NUM 1
#define  IGC_CMDTYPE_OPER_C_HOST_SYS_stop_NUM 2
char *IGC_CMDTYPE_OPER_C_HOST_SYS[]= {	
 	"NULL",          //0. 
	"MainHost",      //1. 切至主系统 
	"ViceHost"       //2. 切至辅系统
};
enum IGC_ENUM_CMDTYPE_C_HOST_SYS  {		    
	CMDTYPE_C_HOST_SYS_MainHost=1,      //1. 切至主系统 
	CMDTYPE_C_HOST_SYS_ViceHost         //2. 切至辅系统
};


#define  IGC_CMDTYPE_C_PLC_SWITCH 73//PLC双CPU切换命令格式
#define  IGC_CMDTYPE_OPER_C_PLC_SWITCH_start_NUM 1
#define  IGC_CMDTYPE_OPER_C_PLC_SWITCH_stop_NUM 2
char *IGC_CMDTYPE_OPER_C_PLC_SWITCH[]= {	
	"NULL",           //0. 
    "MainCPU",        //1. 切至主系统 
	"ViceCPU"         //2. 切至辅系统
};
enum IGC_ENUM_CMDTYPE_C_PLC_SWITCH  {		    	
	CMDTYPE_C_PLC_SWITCH_MainCPU=1,       //1. 切至主系统 
	CMDTYPE_C_PLC_SWITCH_ViceCPU         //2. 切至辅系统
};


#define  IGC_CMDTYPE_C_NET_SWITCH 70//网络切换命令格式
#define  IGC_CMDTYPE_OPER_C_NET_SWITCH_start_NUM 1
#define  IGC_CMDTYPE_OPER_C_NET_SWITCH_stop_NUM 4
char *IGC_CMDTYPE_OPER_C_NET_SWITCH[]= {	
	"NULL",        //0. 
	"NetA",        //1. 切至A#网 
	"NetB",        //2. 切至B#网
	"NetC",        //2. 切至C#网
	"NetD"         //3. 切至C#网
};
enum IGC_ENUM_CMDTYPE_C_NET_SWITCH  {		    
	
	CMDTYPE_C_NET_SWITCH_NetA=1,       //1. 切至A#网 
	CMDTYPE_C_NET_SWITCH_NetB,         //2. 切至B#网
	CMDTYPE_C_NET_SWITCH_NetC,         //2. 切至C#网
	CMDTYPE_C_NET_SWITCH_NetD          //3. 切至C#网
};


#define  IGC_CMDTYPE_C_OP_VALUE_SET 85//运行参数设置命令格式
#define  IGC_CMDTYPE_OPER_C_OP_VALUE_SET_start_NUM 0
#define  IGC_CMDTYPE_OPER_C_OP_VALUE_SET_stop_NUM 0
char *IGC_CMDTYPE_OPER_C_OP_VALUE_SET[]= {	
	"Real_Value"           //0.实时值 
};
enum IGC_ENUM_CMDTYPE_C_OP_VALUE_SET  {		    
	CMDTYPE_C_OP_VALUE_SET_Real_Value          //0.实时值 
};


#define  IGC_CMDTYPE_C_SYS_MODE_SET 86//系统运行方式
#define  IGC_CMDTYPE_OPER_C_SYS_MODE_SET_start_NUM 0
#define  IGC_CMDTYPE_OPER_C_SYS_MODE_SET_stop_NUM 3
char *IGC_CMDTYPE_OPER_C_SYS_MODE_SET[]= {	
	"Off",       //0.退出 
	"On",        //1.投入
	"On2",       //2.投入方式2
	"On3"        //2.投入方式3
};
enum IGC_ENUM_CMDTYPE_C_SYS_MODE_SET  {		    
	CMDTYPE_C_SYS_MODE_SET_Off,       //0.退出 
	CMDTYPE_C_SYS_MODE_SET_On,        //1.投入
	CMDTYPE_C_SYS_MODE_SET_On2,       //2.投入方式2
	CMDTYPE_C_SYS_MODE_SET_On3        //2.投入方式3
};

#define  IGC_CMDTYPE_C_LCU_PARAM 72//PLC定值操作命令
#define  IGC_CMDTYPE_OPER_C_LCU_PARAM_start_NUM 1
#define  IGC_CMDTYPE_OPER_C_LCU_PARAM_stop_NUM 1
char *IGC_CMDTYPE_OPER_C_LCU_PARAM[]= {	
	"NULL",       //0.退出 
	"ParamDnLoad",//1.下传LCU参数
};
enum IGC_ENUM_CMDTYPE_C_LCU_PARAM  {		    
	CMDTYPE_C_LCU_PARAM_ParamDnLoad=1 //1.下传LCU参数
};


#define  IGC_CMDTYPE_C_DATA_POLLING 71//数据召唤命令
#define  IGC_CMDTYPE_OPER_C_DATA_POLLING_start_NUM 0
#define  IGC_CMDTYPE_OPER_C_DATA_POLLING_stop_NUM 0
char *IGC_CMDTYPE_OPER_C_DATA_POLLING[]= {	
	"DataPolling"      //0.召唤 
};
enum IGC_ENUM_CMDTYPE_C_DATA_POLLING  {		    
	CMDTYPE_C_DATA_POLLING_DataPolling      //0.召唤 
};


#define  IGC_CMDTYPE_ANA_PARAM_SET 42//模拟量/整定值参数设置命令格式
#define  IGC_CMDTYPE_OPER_ANA_PARAM_SET_start_NUM 0
#define  IGC_CMDTYPE_OPER_ANA_PARAM_SET_stop_NUM 15
char *IGC_CMDTYPE_OPER_ANA_PARAM_SET[]= {	
	    "D_BAND",  //	0	死区值			
		"HH_LMT",  //	1	上上限值	
		"H_LMT",   //	2	上限值			
		"L_LMT",   //	3	下限值			
		"LL_LMT",  //	4	下下限值		
		"HR_LMT",  //	5	上复限死区值		
		"LR_LMT",  //	6	下复限死区值		
		"ENG_CVT", //	7	工程量转换系数	
		"H_RANG",  //	8	上量程			
		"L_RANG",  //	9	下量程			
		"ZERO",    //  10	零点值(基值)		
		"COMPENS", //  11	补偿值			
		"TR_LMT",  //  12	趋势越限值			
		"RT_LMT",  //  13	趋势复限死区值		
		"H_CODE",  //  14	原码上量程			
		"L_CODE"   //  15	原码下量程			
};
enum IGC_ENUM_CMDTYPE_ANA_PARAM_SET  {		    
		CMDTYPE_ANA_PARAM_SET_D_BAND,   //	0	死区值			
		CMDTYPE_ANA_PARAM_SET_HH_LMT,   //	1	上上限值	
		CMDTYPE_ANA_PARAM_SET_H_LMT,    //	2	上限值			
		CMDTYPE_ANA_PARAM_SET_L_LMT,    //	3	下限值			
		CMDTYPE_ANA_PARAM_SET_LL_LMT,   //	4	下下限值		
		CMDTYPE_ANA_PARAM_SET_HR_LMT,   //	5	上复限死区值		
		CMDTYPE_ANA_PARAM_SET_LR_LMT,   //	6	下复限死区值		
		CMDTYPE_ANA_PARAM_SET_ENG_CVT,  //	7	工程量转换系数	
		CMDTYPE_ANA_PARAM_SET_H_RANG,   //	8	上量程			
		CMDTYPE_ANA_PARAM_SET_L_RANG,   //	9	下量程			
		CMDTYPE_ANA_PARAM_SET_ZERO,     //	10	零点值(基值)		
		CMDTYPE_ANA_PARAM_SET_COMPENS,  //	11	补偿值			
		CMDTYPE_ANA_PARAM_SET_TR_LMT,   //	12	趋势越限值			
		CMDTYPE_ANA_PARAM_SET_RT_LMT,   //	13	趋势复限死区值		
		CMDTYPE_ANA_PARAM_SET_H_CODE,   //  14	原码上量程			
		CMDTYPE_ANA_PARAM_SET_L_CODE,   //	15	原码下量程	
};


#define  IGC_CMDTYPE_IMP_PARAM_SET 43//脉冲量参数设置命令
#define  IGC_CMDTYPE_OPER_IMP_PARAM_SET_start_NUM 0
#define  IGC_CMDTYPE_OPER_IMP_PARAM_SET_stop_NUM 24
char *IGC_CMDTYPE_OPER_IMP_PARAM_SET[]= {	
    	"IMP_ACCUM",       //(整型数)	    0	脉冲累加值    		
		"D_ACCUM",         //(浮点数)		1	电度表读数			
		"D_DAY_PEAK",      //(浮点数)		2	日峰值			
		"D_MONTH_PEAK",    //(浮点数)		3	月峰值			
		"D_YEAR_PEAK",     //(浮点数)		4	年峰值			
		"D_DAY_VALLY",     //(浮点数)		5	日谷值			
		"D_MONTH_VALLY",   //(浮点数)		6	月谷值			
		"D_YEAR_VALLY",    //(浮点数)		7	年谷值			
		"D_DAY_AVE",       //(浮点数)		8	日平值			
		"D_MONTH_AVE",     //(浮点数)		9	月平值			
		"D_YEAR_AVE",      //(浮点数)		10	年平值			
		"D_DAY_SUM",       //(浮点数)		11	日总值			
		"D_MONTH_SUM",     //(浮点数)		12	月总值			
		"D_YEAR_SUM",      //(浮点数)		13	年总值			
		"D_DAY_PPEAK",     //(浮点数)	    (V3.0)		14	日尖峰值			
		"D_MONTH_PPEAK",   //(浮点数)   	(V3.0)		15	月尖峰值			
		"D_YEAR_PPEAK",    //(浮点数)	    (V3.0)		16	年尖峰值			
		"D_DAY_SUM_INI",   //(浮点数)	    (V4.0)		17	日总初值			
		"D_MONTH_SUM_INI", //(浮点数)   	(V4.0)		18	月总初值			
		"D_YEAR_SUM_INI",  //(浮点数)	    (V4.0)		19	年总初值			
		"IMP_ENG_CVT",     //(浮点数)		20	工程量转换系数		
		"PT_CVT",          //(浮点数)		(V4.0)		21	PT变比系数			
		"CT_CVT",          //(浮点数)		(V4.0)		22	CT变比系数			
		"CONSTANT",        //(整型数)		(V4.0)		23	脉冲计数归零翻转值	
		"IMP_BASE"         //(整型数)		(V4.0)		24	脉冲基值			
};
enum IGC_ENUM_CMDTYPE_IMP_PARAM_SET  {		    
		CMDTYPE_IMP_PARAM_SET_IMP_ACCUM,       //(整型数)	    0	脉冲累加值    		
		CMDTYPE_IMP_PARAM_SET_D_ACCUM,         //(浮点数)		1	电度表读数			
		CMDTYPE_IMP_PARAM_SET_D_DAY_PEAK,      //(浮点数)		2	日峰值			
		CMDTYPE_IMP_PARAM_SET_D_MONTH_PEAK,    //(浮点数)		3	月峰值			
		CMDTYPE_IMP_PARAM_SET_D_YEAR_PEAK,     //(浮点数)		4	年峰值			
		CMDTYPE_IMP_PARAM_SET_D_DAY_VALLY,     //(浮点数)		5	日谷值			
		CMDTYPE_IMP_PARAM_SET_D_MONTH_VALLY,   //(浮点数)		6	月谷值			
		CMDTYPE_IMP_PARAM_SET_D_YEAR_VALLY,    //(浮点数)		7	年谷值			
		CMDTYPE_IMP_PARAM_SET_D_DAY_AVE,       //(浮点数)		8	日平值			
		CMDTYPE_IMP_PARAM_SET_D_MONTH_AVE,     //(浮点数)		9	月平值			
		CMDTYPE_IMP_PARAM_SET_D_YEAR_AVE,      //(浮点数)		10	年平值			
		CMDTYPE_IMP_PARAM_SET_D_DAY_SUM,       //(浮点数)		11	日总值			
		CMDTYPE_IMP_PARAM_SET_D_MONTH_SUM,     //(浮点数)		12	月总值			
		CMDTYPE_IMP_PARAM_SET_D_YEAR_SUM,      //(浮点数)		13	年总值			
		CMDTYPE_IMP_PARAM_SET_D_DAY_PPEAK,     //(浮点数)	    (V3.0)		14	日尖峰值			
		CMDTYPE_IMP_PARAM_SET_D_MONTH_PPEAK,   //(浮点数)   	(V3.0)		15	月尖峰值			
		CMDTYPE_IMP_PARAM_SET_D_YEAR_PPEAK,    //(浮点数)	    (V3.0)		16	年尖峰值			
		CMDTYPE_IMP_PARAM_SET_D_DAY_SUM_INI,   //(浮点数)	    (V4.0)		17	日总初值			
		CMDTYPE_IMP_PARAM_SET_D_MONTH_SUM_INI, //(浮点数)   	(V4.0)		18	月总初值			
		CMDTYPE_IMP_PARAM_SET_D_YEAR_SUM_INI,  //(浮点数)	    (V4.0)		19	年总初值			
		CMDTYPE_IMP_PARAM_SET_IMP_ENG_CVT,     //(浮点数)		20	工程量转换系数		
		CMDTYPE_IMP_PARAM_SET_PT_CVT,          //(浮点数)		(V4.0)		21	PT变比系数			
		CMDTYPE_IMP_PARAM_SET_CT_CVT,          //(浮点数)		(V4.0)		22	CT变比系数			
		CMDTYPE_IMP_PARAM_SET_CONSTANT,        //(整型数)		(V4.0)		23	脉冲计数归零翻转值	
		CMDTYPE_IMP_PARAM_SET_IMP_BASE         //(整型数)		(V4.0)		24	脉冲基值
};

#define  IGC_CMDTYPE_C_ONOFF_SET 44//开关量动作次数统命令
#define  IGC_CMDTYPE_OPER_C_ONOFF_SET_start_NUM 1
#define  IGC_CMDTYPE_OPER_C_ONOFF_SET_stop_NUM 4
char *IGC_CMDTYPE_OPER_C_ONOFF_SET[]= {	
    	"NULL"
		"NORMAL_OPEN", //	    1	正常分闸次数		
		"NORMAL CLOSE",//	    2	正常合闸次数				
		"FAULT OPEN",  //		3	事故分闸次数		
		"FAULT CLOSE", // 		4	事故合闸次数				
};
enum IGC_ENUM_CMDTYPE_C_ONOFF_SET  {		    
		CMDTYPE_C_ONOFF_SETNORMAL_OPEN=1, //	1	正常分闸次数		
		CMDTYPE_C_ONOFF_SETNORMAL_CLOSE, //	    2	正常合闸次数				
		CMDTYPE_C_ONOFF_SETFAULT_OPEN,   //		3	事故分闸次数		
		CMDTYPE_C_ONOFF_SETFAULT_CLOSE   //     4	事故合闸次数	
};


typedef struct
{	
	int second;     /* seconds after the minute - [0,59] */
	int minute;     /* minutes after the hour - [0,59] */
	int hour;       /* hours since midnight - [0,23] */
	int day;        /* day of the month - [1,31] */
	int month;      /* months since January - [1,12] */
	int year;    
	
}IGC_datetime; 

typedef struct
{
	float value;
	int   int_value;
	int   chan_fail;
	int   ok_flag;
	IGC_datetime update_time;
	IGC_datetime change_time;
}H9000_IGC_data;


typedef struct
{
	int   value;
	int   chan_fail;
	int   ok_flag;
	IGC_datetime update_time;
	IGC_datetime change_time;
}H9000_IGC_IND_data;

typedef struct
{
	float value;
	float D_BAND;    //死区值
	float HH_LMT;    //上上限
	float H_LMT;     //上限		
	float L_LMT;     //下限
	float LL_LMT;    //下下限
	float HR_LMT;    //上复限
	float LR_LMT;    //下复限
	float ENG_CVT;	 //工程转换系统	
	float HI_RANGE;  //上量程
	float LO_RANGE;	 //下量程
	float ZERO;      //0值
	float COMPENSATE;//补偿值
	float TR_LIM;    //趋势限值
	float RT_LIM;    //趋势复限死区值
	int   int_value;
	int   chan_fail;
	int   ok_flag;

	IGC_datetime update_time;
	IGC_datetime change_time;
}H9000_IGC_ANA_data;

typedef struct
{
	int		max_no;      //最大值点号
	int		min_no;      //最小值点号
	int		max_rate_no; //最大变化值点号
	int		min_rate_no; //最小变化值点号
	float	max_value;   //最大值
	float	min_value;   //最小值
	float	avg_value;   //平均值
	float	std_value;   //方差值
	float   max_rate;    //最大变化率
	float   min_rate;    //最小变化率
  
	int   chan_fail;
	int   ok_flag;
	
	IGC_datetime update_time;
 
}H9000_IGC_CHARA_data;


typedef struct
{
	int     ok_flag;
	char	sn_str[12];
	char	time_str[64];
	char	repeatAlm[12];
	char	logicname[52];
	char	longname[112];
	char	state_str[128];
//	char	cmd_src[64];
//	char	cmd_result[64];
	char	hostname[10];
	char	usrname[8];
	char	message[256];
	IGC_datetime igc_datetime;
} H9000_IGC_alarm_record;

extern PRIVILEGE_DB		privilege_db;
extern int LoadUserPrivilegeDatabase( );
char igc_cmd_control_point[128][POINT_NAME_SIZE+12];
 
char igc_user_login_name[20];
SHM_RECORD	 *shm_rec_addr;
FAULT_RECORD_TABLE *shm_fault_addr;
int  if_point_is_Doublept_By_pointid(unsigned short stnid,unsigned short devid,unsigned short datatype_id,unsigned short ptid);
int set_pt_to_doublePt_by_pointid(unsigned short stnid,unsigned short devid,unsigned short datatype_id,unsigned short ptid,unsigned short stnid_P,unsigned short devid_P,unsigned short datatype_id_P,unsigned short ptid_P,int isdoublept);
int  get_iogroup_by_pointid(unsigned short stnid,unsigned short devid,unsigned short datatype_id,unsigned short ptid);


int IGC_SEND_H9000_SMG(char * smg);

int  igc_InitStrTextDefine();

H9000_IGC_alarm_record igc_search_one_record_by_logicalname(char * logicalname,int Forward_num,int Forward_index);
H9000_IGC_alarm_record igc_usetime_search_one_record_by_logicalname(char * logicalname,int Forward_num,int Forward_index,IGC_datetime start_time);
H9000_IGC_alarm_record igc_search_one_alarm_by_logicalname(char * logicalname,int Forward_num);

 

int IGC_GetCurentHostCtrlPrvg(int Itype);
int igc_GetPointIdFromRec(REC_ENTRY_UNION *rec, POINTER *pt, int *alm_type);
int trans_cmdtype_to_priv(int cmdtype);

int IGC_Init_RECORD_IPC();
int igc_userlogin(char * user,char * pass);
int igc_userunlogin();
int  set_igc_cmd_control_point(int cmd_type,char * logicalname);
int  clear_igc_cmd_control_point(int cmd_type);
int igc_SendCmd(int cmdtype,int operate_type,int sim_cmd,char * logicalname,float setvalue,char * interlockname_user);

int   igc_WSAStartup();
int   igc_lan_out_init();
//void  igc_SetModuleName(char *module);
int   igc_SetErrLogMode(int argc, char *argv[]);
//void  igc_CreateLockFile(char *filename );
void  igc_Err_log( char *info_buf );

int  igc_Judge_interlock();
int  igc_MountDB();
int  igc_UnMountDB();
int  igc_InterLockFileLoad();
void igc_InterLockFree();
int  get_CLK_TCK();
void sleep_second_igclib(int second);
void sleep_minisecond_igclib(int minisecond);
IGC_datetime get_lacal_datetime_now();
IGC_datetime get_datetime_by_time_t(time_t ltime);

long total_time_seconds(IGC_datetime start_time,IGC_datetime end_time);

H9000_IGC_data get_data_value_by_logicalname(char * logicalname);
H9000_IGC_data get_data_value_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,unsigned int POINT_ID);
void get_data_value_by_pointid_IN(POINTER point,H9000_IGC_data * R_data);

H9000_IGC_CHARA_data  get_chara_data_value_by_logicalname(char * logicalname);
H9000_IGC_CHARA_data get_chara_data_value_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,unsigned int POINT_ID);
void get_chara_data_value_by_pointid_IN(POINTER point,H9000_IGC_CHARA_data * R_data);

 
H9000_IGC_IND_data get_ind_data_value_by_logicalname(char * logicalname);
H9000_IGC_IND_data get_ind_data_value_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,unsigned int POINT_ID);
void get_ind_data_value_by_pointid_IN(POINTER point,H9000_IGC_IND_data * R_data);

 
H9000_IGC_ANA_data get_ana_data_value_by_logicalname(char * logicalname);
H9000_IGC_ANA_data get_ana_data_value_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,unsigned int POINT_ID);
void get_ana_data_value_by_pointid_IN(POINTER point,H9000_IGC_ANA_data * R_data);

 
void MultiPointPack_set_ind(unsigned char type_id,unsigned char stn,unsigned char dev,unsigned char type,unsigned short first,unsigned short end,unsigned short status,int data,int smsfalg);

void MultiPointPack_ALM(unsigned char typeid,unsigned char stn,unsigned char dev,unsigned char type,unsigned short first,unsigned short end,unsigned short status,int data,int almfag,int smsfalg);
int ind_alm_by_logicalname(char * logicalname,int alm_status,int stand_by,int MAN_SET,int CHAN_FAIL,int alarm_type,int sendsms_falg);
int ana_alm_by_logicalname(char * logicalname,float set_value,int alm_status,int trend_alarm_status,int data_overflow,int CHAN_FAIL,int alarm_type,int sendsms_falg);//status (0.复归 1.越上上限 2.越上限 3.越下限 4.越下下限 5.复上限 6.复下限 7.复上上限 8.复下下限....)
int ind_alm_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,
					   int alm_status,int stand_by,int MAN_SET,int CHAN_FAIL,int alarm_type,unsigned int START_POINT_ID,unsigned int END_POINT_ID,int sendsms_falg);
int ana_alm_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,
					   float set_value,int alm_status,int trend_alarm_status,int data_overflow,int CHAN_FAIL,int alarm_type,unsigned int START_POINT_ID,unsigned int END_POINT_ID,int sendsms_falg);

int set_ind_value_by_logicalname(char * logicalname,int set_value,int stand_by,int MAN_SET,int CHAN_FAIL,int sendsms_falg);
int set_ana_value_by_logicalname(char * logicalname,float set_value,int stand_by,int MAN_SET,int CHAN_FAIL,int data_overflow,int sendsms_falg);
int set_int_value_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,unsigned int START_POINT_ID,unsigned int END_POINT_ID,
							 int set_value,int stand_by,int MAN_SET,int CHAN_FAIL,int sendsms_falg);
int set_ana_value_by_pointid(unsigned int station_ID,unsigned int  DEVICE_ID,unsigned int  TTPE_ID,unsigned int START_POINT_ID,unsigned int END_POINT_ID,
							 float set_value,int stand_by,int MAN_SET,int CHAN_FAIL,int data_overflow,int sendsms_falg);


