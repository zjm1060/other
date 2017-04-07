#ifndef	_DBWRITE_TYPE_DEF
#define	_DBWRITE_TYPE_DEF


/*****************************************************************
**
**	        	报文类型(dp_type)定义
**
******************************************************************/

#define BJ_DB				1	/***报警***/
#define ALL_DB				2	/***全送***/
#define SERIAL				3	/***串口下行命令***/
#define COMM				4	/***网络下行命令***/
#define COMM_RECORD			5	/***广播命令记录***/
#define COMMB				6	/***转发命令***/
#define RCOMM				7	/***远方通讯***/
#define BJB					8	/***转发报警***/
#define ALLB				9	/***转发全送***/

#define BJ_DB_TIME			11	/***全时标报警***/
#define ALL_DB_TIME			12	/***全时标全送***/
#define SERIAL_TIME			13	/***全时标串口下行命令***/
#define COMM_TIME			14	/***全时标网络下行命令***/
#define COMM_RECORD_TIME	15	/***全时标广播命令记录***/
#define COMMB_TIME			16	/***全时标转发命令***/
#define RCOMM_TIME			17	/***全时标远方通讯***/

#define BJ_DB_SIM			31	/***仿真系统全时标报警***/
#define ALL_DB_SIM			32	/***仿真系统带时标全送***/
#define SERIAL_SIM			33	/***仿真系统带时标串口下行命令***/
#define COMM_SIM			34	/***仿真系统带时标网络下行命令***/
#define COMM_RECORD_SIM		35	/***仿真系统带时标广播命令纪录***/


/*****************************************************************
**
**	        	类型标识(type_id)定义
**
******************************************************************/

#define DPS_ANALOG			1 
#define DPS_IMPULSE			3 
#define DPS_IND_ALM			4
#define DPS_LMS				5
#define DPS_SMS				6
#define DPS_SM_ALM			7
 
#define DPS_CURVE			10
#define DPS_DEV_FB			11
 
#define	DPS_ANA_ALM			13
#define DPS_IMP_ALM    		16
	
#define DPS_ANA_ALL			20
#define DPS_IMP_ALL			21
#define DPS_STATUS_ALL		22
#define DPS_CURVE_ALL		23
#define DPS_DEV_FB_ALL		24
#define DPS_TIME_ALL		25

	
#define	DPS_IOST_SET		29               
#define DPS_IOATTR_SET		30
#define DPS_IO_GROUP_SET	31
#define DPS_MAN_VALUE_SET  	35
#define DPS_MAN_STAT_SET   	36
#define DPS_MAN_CURVE_SET  	37
#define DPS_MAN_TIME_SET  	38   //	added by chi 10.06.07 ; 

#define DPS_ANA_PARAM_SET	42
#define DPS_IMP_PARAM_SET	43
#define DPS_C_ONOFF_SET		44

#define TIME_SYNC       	47
#define C_LCU_ONOFF     	48
#define C_UNIT          	49
#define C_BREAKER       	50
#define C_SWITCH        	51
#define C_APOWER        	52
#define C_RPOWER        	53
#define C_DEVICE        	54
#define C_GATE          	55
#define C_TRANS         	56
#define C_DEV_MODE         	57

#define	C_VOLT_STEP			60
#define	C_SPEED_STEP		61
#define	C_ONOFF_FORCE		62
#define	C_ANA_FORCE			63
#define	C_IO_CTRL_BLOCK		64
#define	C_HOST_SYS			69
#define	C_NET_SWITCH		70
#define	C_DATA_POLLING		71	
#define	C_LCU_PARAM			72
#define	C_PLC_SWITCH		73

#define C_SIGNAL_RESET		83
#define	C_OP_VALUE_SET		85
#define C_SYS_MODE_SET		86




/********************************************************************
**
**           模拟量参数设置种类定义
**
********************************************************************/

#define ANA_PARAM_SET_D_BAND		   	0
#define ANA_PARAM_SET_HI_ALARM     		1
#define ANA_PARAM_SET_HI_WARN      		2
#define ANA_PARAM_SET_LO_WARN      		3
#define ANA_PARAM_SET_LO_ALARM     		4
#define ANA_PARAM_SET_HI_RECOVER	   	5
#define ANA_PARAM_SET_LO_RECOVER	   	6
#define ANA_PARAM_SET_ENG_CVT	   		7 
#define ANA_PARAM_SET_HI_RANGE			8	
#define ANA_PARAM_SET_LO_RANGE			9	
#define ANA_PARAM_SET_ZERO				10	
#define ANA_PARAM_SET_COMPSATE			11	
#define ANA_PARAM_SET_TR_WARN			12	
#define ANA_PARAM_SET_TR_RECOVER		13	
#define ANA_PARAM_SET_HI_CODE			14	
#define ANA_PARAM_SET_LO_CODE			15	




/********************************************************************
**
**           开关量动作次数设置种类定义
**
********************************************************************/

#define IND_PARAM_SET_NORM_OPEN_NUM		1
#define IND_PARAM_SET_NORM_CLOSE_NUM	2
#define	IND_PARAM_SET_FAULT_OPEN_NUM	3
#define	IND_PARAM_SET_FAULT_CLOSE_NUM	4




/********************************************************************
**
**           脉冲量参数设置种类定义
**
********************************************************************/

#define	IMP_PARAM_SET_IMP_ACCUM			0
#define	IMP_PARAM_SET_D_ACCUM			1
#define IMP_PARAM_SET_PEAK_DAY			2		
#define IMP_PARAM_SET_PEAK_MONTH		3
#define IMP_PARAM_SET_PEAK_YEAR			4
#define IMP_PARAM_SET_VALLEY_DAY		5
#define IMP_PARAM_SET_VALLEY_MONTH		6
#define IMP_PARAM_SET_VALLEY_YEAR		7
#define	IMP_PARAM_SET_NORM_DAY			8
#define IMP_PARAM_SET_NORM_MONTH		9
#define	IMP_PARAM_SET_NORM_YEAR			10
#define	IMP_PARAM_SET_SUM_DAY			11
#define	IMP_PARAM_SET_SUM_MONTH			12
#define	IMP_PARAM_SET_SUM_YEAR			13
#define IMP_PARAM_SET_PPEAK_DAY			14  
#define IMP_PARAM_SET_PPEAK_MONTH		15  
#define IMP_PARAM_SET_PPEAK_YEAR		16  
#define IMP_PARAM_SET_INIT_DAY			17  
#define IMP_PARAM_SET_INIT_MONTH		18  
#define IMP_PARAM_SET_INIT_YEAR			19  
#define	IMP_PARAM_SET_CVT_F				20
#define	IMP_PARAM_SET_PT				21
#define	IMP_PARAM_SET_CT				22
#define	IMP_PARAM_SET_CONSTANT			23
#define	IMP_PARAM_SET_BASE_ACCUM		24




/********************************************************************
**
**           IOST设置种类定义
**
********************************************************************/

#define	IOST_SET_SCAN_STATE		1
#define	IOST_SET_ALM_STATE		2
#define IOST_SET_ALM_NOACK		3		
#define IOST_SET_CTRL_LOCK		4
#define IOST_SET_NO_UPDATE		5
#define IOST_SET_ALM_REPEAT		6
#define IOST_SET_TAG_STATE		7




/********************************************************************
**
**           IOATTR设置种类定义
**
********************************************************************/

#define IOATTR_SET_HDB_PERIOD		0
#define	IOATTR_SET_PROCTYPE			1
#define	IOATTR_SET_ALM_CBA			2
#define IOATTR_SET_STATISTIC		3		
#define IOATTR_SET_FAULT_PNT		4
#define IOATTR_SET_FAIL_PNT			5
#define IOATTR_SET_CTRL_PNT			6
#define IOATTR_SET_SOE_PNT			7
#define IOATTR_SET_TREND_ALM		8
#define IOATTR_SET_KEY_SIGNAL		9
#define IOATTR_SET_FAULT_REC		10
#define IOATTR_SET_AUD_ALM			11
#define IOATTR_SET_NORM_CLOSE		12
#define IOATTR_SET_DOUBLE_PNT		13
#define IOATTR_SET_SYS_VIRTUAL		14
#define IOATTR_SET_LCU_VIRTUAL		15
#define IOATTR_SET_CASCADE_PNT		16
#define IOATTR_SET_HDB_REC			17
#define IOATTR_SET_HDB_CURVE		18
#define IOATTR_SET_ON_ALM			19
#define IOATTR_SET_OFF_ALM			20
#define IOATTR_SET_NORM_STATE		21




/********************************************************************
**
**           模拟量报警状态定义
**
********************************************************************/

#define ANA_ALM_NORMAL 			0
#define ANA_ALM_HI_ALARM     	1
#define ANA_ALM_HI_WARN      	2
#define ANA_ALM_LO_WARN      	3
#define ANA_ALM_LO_ALARM     	4
#define ANA_ALM_HI_RECOVER	   	5
#define ANA_ALM_LO_RECOVER	   	6
#define ANA_ALM_HH_RECOVER	   	7
#define ANA_ALM_LL_RECOVER	   	8

#define ANA_ALM_TR_WARN			1	
#define ANA_ALM_TR_RECOVER		2	




/********************************************************************
**
**           模拟量报警类型定义
**
********************************************************************/

#define ANA_ALM_TYPE_NORM		0
#define ANA_ALM_TYPE_LMT 		1
#define ANA_ALM_TYPE_TR     	2
#define ANA_ALM_TYPE_CHANFAIL  	3
#define ANA_ALM_TYPE_CTRLBLOCK 	4
#define ANA_ALM_TYPE_OVERFLOW	5


/********************************************************************
**
**           开关量报警类型定义
**
********************************************************************/

#define IND_ALM_TYPE_NORM		0
#define IND_ALM_TYPE_CHANGE		1
#define IND_ALM_TYPE_MANFORCE  	2
#define IND_ALM_TYPE_CHANFAIL  	3

/********************************************************************
**
**           脉冲量报警类型定义
**
********************************************************************/

#define IMP_ALM_TYPE_NORM 		0
#define IMP_ALM_TYPE_MANFORCE  	1
#define IMP_ALM_TYPE_CHANFAIL  	2



/********************************************************************
**
**           脉冲量时间类型定义
**
********************************************************************/

#define IMP_VALLEY   	0
#define IMP_PEAK        1
#define IMP_NORMAL      2
#define IMP_PPEAK 		3		 



#endif /*_DBWRITE_TYPE_DEF*/

