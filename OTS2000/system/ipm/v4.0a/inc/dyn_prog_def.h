/*
If the PrgN_ changes,/OIXINC/dyn_prog_def.h should change together 
Modified by zyp on 2006.3.31 
*/
/*Fomat::
#define PrgN_			v3.0	v4.0	*规约中的type_id*
*/

#define	PrgN_NULL						0

#define			PrgN_ALARM_TAB				1	 
#define			PrgN_ALM_BLINK_RESET		2 

#define			PrgN_ANALOG_TAB  			3   
#define			PrgN_CALC_TAB				4	 
#define			PrgN_CANCEL					5	 

#define			PrgN_C_ANA_FORCE			6 
#define			PrgN_C_APOWER				7	 
#define			PrgN_C_BREAKER				8 
#define			PrgN_C_DATA_POLLING			9 
#define			PrgN_C_DEVICE				10	 
#define			PrgN_C_DEV_MOD				11	 
#define			PrgN_C_GATE					12		 

#define			PrgN_C_HOST_SYS				13		 
#define			PrgN_C_LCU_ONOFF			14	 
#define			PrgN_C_LCU_PARAM			15	 

#define			PrgN_C_NET_SWITCH			16	 
#define			PrgN_C_ONOFF_FORCE			17		 
#define			PrgN_C_ONOFF_SET			18 
#define			PrgN_C_RPOWER				19	 

#define			PrgN_C_SIGNAL_RESET			20	 
#define			PrgN_C_SPEED_STEP			21	 
#define			PrgN_C_SWITCH				22	 
#define			PrgN_C_SYS_MODE_SET			23	 

#define			PrgN_C_TRANS				24	 
#define			PrgN_C_UNIT					25	 
#define			PrgN_C_VOLT_STEP			26	 
#define			PrgN_DB_LIST_TAB			27	 
#define			PrgN_DO_ACKCEL				28	 	

#define			PrgN_POINT_TAB				29	 

#define			PrgN_SOFT_COPY				30	 
#define			PrgN_GET_SGZY_HISTORY		31	 


#define			PrgN_GET_TEMP_TREND			32
#define			PrgN_IMPULSE_TAB			33

#define			PrgN_C_IMP_PARAM_SET		34
#define			PrgN_IND_INT_TAB			35
#define			PrgN_IOATTR_SET				36
	
#define			PrgN_IOST_SET				37
#define			PrgN_MAN_CURVE_SET			38
#define			PrgN_MAN_STAT_SET			39
#define			PrgN_MAN_VALUE_SET			40
#define			PrgN_MODFY_PRMT				41
#define			PrgN_NEXT_LCU				42
#define			PrgN_NEXT_POINT				43
#define			PrgN_OP_VALUE_SET			44

#define			PrgN_OUTA_TAB				45	
#define			PrgN_OUTS_TAB				46	
#define			PrgN_PARAM_TAB				47


#define			PrgN_C_PLC_SWITCH			48	
#define			PrgN_IND_POL_TAB			49
#define			PrgN_PREV_LCU				50
#define			PrgN_PREV_POINT				51
#define			PrgN_SGZY_SET				52

#define			PrgN_SYSMSG_TAB				53

#define			PrgN_TEMP_TREND_SET		    54
#define		    PrgN_TIME_SYNC				55
#define			PrgN_PREV_STN				56
#define			PrgN_NEXT_STN				57
#define			PrgN_IO_CTRL_BLOCK			58
#define			PrgN_IO_CTRL_GROUP_BLOCK	59
#define			PrgN_OBJ_TAB	60
#define			PrgN_CURVE_INPUT	61
#define			PrgN_SCENE_REDISP			62
#define			MAX_PROG					63                                               
	

#ifdef	PROG_DEF
char	prog_names[MAX_PROG][PROG_NAME_LEN]	=
{
	"NULL",	
	"AlarmTab",
	"AlmBlkReSet",
	"AnlgPnt",	
	"CaculTab",
	"Cancel",
	"C_ANA_FORCE",
	"C_APower",
	"C_Brker",
	"C_DATA_POLLING",
	"C_Device",
	"C_Dev_Mode",
	"C_Gate",
	"C_Host_Sys",
	"C_LCU_ONOFF",
	"C_LCU_PARAM",
	"C_Net_Switch",				
	"C_ONOFF_FORCE",
	"C_ONOFF_SET",
	"C_RPower",
	"C_SIGNAL_RESET",
	"C_SpdStep",
	"C_Switch",
	"C_SYS_MODE_SET",	
	"C_TRANS",
	"C_Unit",
	"C_VltStep",		
	"DbTable",
	"DoAckCel",
	"GetPntTab",
	"GetSftCp",
	"GetSgzyHs",
	"GetTrend",
	"ImplPnt",
	"IMP_PARAM_SET",
	"IntrPnt",
	"IOATTR_SET",
	"IOST_SET",
	"MAN_CURVE_SET",
	"MAN_STAT_SET",
	"MAN_VALUE_SET",
	"ModfyPrmt",
	"NextLCU",
	"NextPnt",
	"OP_VALUE_SET",
	"OutaPnt", 
	"OutsPnt",	
	"ParamTab",
	"PLC_Switch",
	"PollPnt",
    "PrevLCU",	
	"PrevPnt",
	"SgzySet",
	"SysmsgPnt", 
	 "TrendSet", 
	"TIME_SYNC",   
	"PREV_STN",
	"NEXT_STN",
	"C_IO_CTRL_BLOCK",
	"C_IO_CTRL_GRP_BLK",
	"OBJ_TAB" ,
	"Curve_Input",
	"SceneRedisp"
};


char *Cmd_Mode[][50] = {	
{"NULL"},/*NULL*/
{"NULL"},/*AlarmTab 1*/
{"NULL"},/*AlmBlkReset 2 */
{"NULL"},/*AnlgPnt 3 */
{"NULL"},/*CaculTab 4 */
{"NULL"},/*Cancel 5*/
/* C_ANA_FORCE 6*/ {"UnForce","Force" },

{"Power","Opening","Flux"},/*APOWER  7*/
/* C_BREAKER 8 */	{"Reset" ,"TestOpen","Open","Close","IM_Line","IIM_Line","Line_IM","Line_IIM","Tran_IM","Tran_IIM","IM_Tran","IIM_Tran","TestClose","CancelSyn","LoopClose","ChgVltCls","ZroVltCls"},
{"DataPolling"},/*DATA_POLLING  9*/
/* C_DEVICE 10 */	{"Reset","Off","On" },
/* C_DEV_MOD 11*/	{"Off","On","On2","On3" },
/* C_GATE 12*/		{"Reset","Open","Close","Stop","Up","Down","FastOpen","FastClose","StepUp",	
								"StepDown","Forward","Backward","SetZero","SetFlux","EqualPress"} ,
/* C_HOST_SYS 13*/  {"MainHost","ViceHost" },
{"Offline","Online"},/*LCU_ONOFF   14*/
/* C_LCU_Parm 15*/  {"ParamPolling","ParamDownLoad" },
/* C_NET_SWITCH 16*/{"NetA","NetB","NetC","NetD","NetE","NetF", },
	/* C_ONOFF_FORCE 17*/{"UnForce","ForceOff","ForceOn" },
{"NULL","Open_Num","Close_Num","SgOpnNum","SgClsNum"},/*ONOFF_SET 18*/
{"Power","Volt","Cos"},/*RPower  19*/
{"NULL","FaultReset","FailReset"},/*Signal_Reset 20*/
/* C_SPEED_STEP 21*/{"Decrease","Increase" },
/* C_SWITCH 22 */	{"Reset","Open","Close","TestOpen","TestClose" },
/* C_SYS_MOD_SET 23*/{"Off","On","On2","On3"},
/* C_TRANS 24*/		{"Reset","Raise","Down","Stop","RaiseTo","DownTo" },

	{"Start","Stop","Reset","Step","Jump", "NoVoltRun","NoLoadRun","BlackStart",         /*  C_Unit  */
			                 "CmpStart","EmergStop","Open","ReadyStart","TransNoLoad","ZeroVRun",
							 "TranZVRun","ZeroVStop","PumpStart","PumpStop","DragStart","GenToPump",
							 "SFCHSStart","SFCLSStart","SFCHSCmp","SFCLSCmp","DragHSStart" ,
							 "DragLSStart","DragHSCmp","DragLSCmp","PumpToGe" ,
							 "StartToCool" , "StartToSpin" , "StartToNLoad" , "StartToOnLin" , "StopToNLoad" , 
							 "StopToSpin","StopToCool","StopToStand","StartStep","StopStep","EmergStep",
							 "FastStopStep"  }, 	/*C_UNIT  25*/
		
		/* C_VOLT_STEP 26 */{"Decrease","Increase" },
{"NULL"},/*DbTab  27*/
{"NULL"},/*DoAckCel  28*/
{"NULL"},/*GetPntTab  29 */
{"NULL"},/*GetSftCp  30 */
{"NULL"},/*GetSgzyHs  31 */
{"NULL"},/*GetTrend  32 */
{"NULL"},/*GetImplPnt  33 */	
{"NULL"},/*IMP_PARAM_SET  34*/
{"NULL"},/*IntrPnt  35*/	
{"NULL","Statistic_Yes","Statistic_No","Failpnt_Yes","Failpnt_No","Faultpnt_Yes","Faultpnt_No","Trendalm_Yes","Trendalm_No",
	"Keysignal_Yes","Keysignal_No",
	"Audioalm_Yes","Audioalm_No","Cascadepnt_Yes","Cascadepnt_No","Hdbrec_Yes","Hdbrec_No","Hdbcurve_Yes",
	"Hdbcurve_No","Hdb_period0","Hdb_period1","Hdb_period2","Hdb_period3","Hdb_period4","Hdb_period5"},/*IOATTR_SET  36*/
{"NULL","Scan_state_Enable","Scan_state_Disable","Alm_state_Enable","Alm_state_Disable","Alm_ack_state",
"Tag_state_Enable","Tag_state_Disable"},/*IOST_SET 37 */
{"Real_Value"},					/*MAN_CURVE_SET 38 */
{"Open","Close","Transit"},		/*MAN_STAT_SET 39 */
{"Real_Value","Param_Value"},	/*MAN_VALUE_SET 40 */
{"NULL"},						/*ModfyPrmt 41*/
{"NULL"},						/*NEXT_LCU 42*/
{"NULL"},						/*NEXT_PNT  43*/
/* C_OP_Val_SET 44*/{"RealValue","ParamValue"  },
{"NULL"},/*OutaPnt  45 */
{"NULL"},/*OutsPnt  46*/
{"NULL"},/*ParamTab  47*/
		/* C_PLC_SWITCH 48*/{"MainCPU","ViceCPU"  },
{"NULL"},/*PollPnt 49*/
{"NULL"},/*PrevLCU 50*/
{"NULL"},/*PrevPnt  51*/
{"NULL"},/*SgzySet  52*/
{"NULL"},/*SysmsgPnt 53*/
{"NULL"},/*TrendSet  54*/
{"TimeSync"},/*TIME_SYNC  55*/	
{"NULL"},	/*PREV_STN  56 */
{"NULL"},	/*NEXT_STN  57  */
{"UnBlock", "Block"}	,  /*NEXT_STN  58  */
{"NULL"} ,	/*NEXT_STN  59  */
{"NULL"} ,	/*NEXT_STN  60  */
{"NULL"} ,
{"scene"}												
} ;


#else
extern	char	prog_names[MAX_PROG][PROG_NAME_LEN];
extern  char	*Cmd_Mode[][50];
#endif


