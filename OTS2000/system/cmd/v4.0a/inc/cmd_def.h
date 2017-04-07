#define		LO_UNIT_ADD		1
#define		HI_UNIT_ADD		64
#define		LO_BREAK_ADD		65
#define		HI_BREAK_ADD		192
#define		LO_SWITCH_ADD		193
#define		HI_SWITCH_ADD		448
#define		LO_DEVICE_ADD		449
#define		HI_DEVICE_ADD		704
#define		LO_GATE_ADD		705
#define		HI_GATE_ADD		832
#define		LO_TRANS_ADD		833
#define		HI_TRANS_ADD		896
#define		LO_DEV_MODE_ADD		897
#define		HI_DEV_MODE_ADD		1152

#define		USER_UNIT		1
#define		USER_BREAKER		64
#define		USER_SWITCH		128
#define		USER_DEVICE		158 //128
#define		USER_GATE		32
#define		USER_TRANS		16
#define		USER_DEVICE_MODE	256

#define		ST_LCU_ONOFF		3
#define		ST_UNIT			32
#define		ST_BREAKER		17
#define		ST_SWITCH		4
#define		ST_DEVICE		2
#define		ST_APOWER		3
#define		ST_RPOWER		3
#define		ST_TRANS		6
#define		ST_GATE			16
#define		ST_STEP			2
#define		ST_ONOFF_FORCE		3
#define		ST_ANA_FORCE		2
#define		ST_DEV_MODE		3
#define		ST_NET_SWITCH		4
#define		ST_PLC_SWITCH		3
#define		ST_SIGNAL		3

#define		OBJECT_NOT_EXIST	2
#define		CMD_FORMAT_FAULT	3

/* BREAKER */
#define	TRIP_OFF	1
#define	TRIP_ON		2	
#define	IM_Line_ON	3
#define	IIM_Line_ON	4
#define	Line_IM_ON	5
#define Line_IIM_ON	6
#define	Tran_IM_ON	7
#define	Tran_IIM_ON	8
#define	IM_Tran_ON	9
#define	IIM_Tran_ON 	10   
#define	TEST_TRIP_ON	11
#define	CANCEL_SYN	12
#define	LOOP_CLOSE	13
#define	CHG_VLT_CLOSE	14
#define	ZRO_VLT_CLOSE	15
#define TEST_OPEN	16

/*SWITCH*/
#define SWITCH_OPEN	1
#define SWITCH_CLOSE	2
#define SWITCH_TEST_OPEN 	3
#define SWITCH_TEST_CLOSE	4 

/*	UNIT control command type	*/
#define	START_UNIT	1		/*¿ª»ú*/
#define	STOP_UNIT	2		/* Í£»ú */
#define	RESET		3
#define STEP_UNIT	4
#define JUMP_UNIT	5
#define	NO_VOLT_RUN	6		
#define	NO_LOAD_RUN	7		
#define	BLACK_START	8			
#define	CMP_START	9		
#define EMERG_STOP	10
#define UNIT_OPEN	11
#define READY_START	12
#define TRANS_NOLOAD	13
#define ZERO_V_RUN	14
#define TRANS_ZV_RUN	15
#define ZERO_V_STOP	16
#define START_TO_COOL   20
#define START_TO_SPIN   21
#define START_TO_NLOAD  22
#define START_TO_ONLIN  23
#define STOP_TO_NLOAD   24
#define STOP_TO_SPIN    25
#define STOP_TO_COOL    26
#define STOP_TO_STAND   27
#define START_STEP      28
#define STOP_STEP       29
#define EMERG_STEP      30
#define FAST_STOPSTEP   31
#define PUMP_START	32
#define PUMP_STOP	33
#define DRAG_START	34
#define SFCHS_START	35
#define SFCLS_START	36
#define SFCHS_CMP	37
#define SFCLS_CMP	38
#define DRAG_HS_START	39
#define DRAG_LS_START	40
#define DRAG_HS_CMP	41
#define DRAG_LS_CMP	42
#define PUMP_TO_GEN	43
#define GEN_TO_PUMP	44


/*** GATE ***/
#define GATE_OPEN	1
#define GATE_CLOSE	2
#define GATE_STOP	3
#define GATE_UP		4
#define GATE_DOWN	5
#define GATE_FASTOPEN	6
#define GATE_FASTCLOSE	7
#define GATE_STEPUP	8
#define GATE_STEPDOWN	9
#define GATE_FORWARD	10
#define GATE_BACKFORD	11
#define GATE_SETZERO	12
#define GATE_SETFLUX	13
#define GATE_EQUALPRESS 14

/* APOWER ADJUSTMENT*/
#define POWER_ADJ	0
#define OPENNING_ADJ	1
#define FLUX_ADJ	2

/*RPOWER ADJUSTMENT*/
#define VOLT_ADJ	1
#define COS_ADJ		2

/*VOLT_STEP  SPEED_STEP*/
#define	STEP_DECREASE	1
#define	STEP_INCREASE	2

/* LCU ON/OFF */
#define LCU_OFF_LINE    1
#define LCU_ON_LINE     2

/* HOST_SWITCH PLC_SWITCH */
#define	BE_HOST		1
#define	BE_VICE		2

/* NET SWITCH */
#define	BE_NETA		1
#define	BE_NETB		2
#define	BE_NETC		3
#define	BE_NETD		4

/* SYS_MODE_SET C_DEV_MODE */
#define	SET_TRIP_OFF	0	
#define	SET_TRIP_ON	1
#define	SET_TRIP_ON2	2	
#define	SET_TRIP_ON3	3

/*OP_VALUE_SET*/
#define REAL_VALUE	0
#define PARAM_VALUE	1

/* SIGNAL_RESET */
#define	FAULT_RESET	1
#define FAIL_RESET	2
/*TIME_SYNC*/
#define TIME_SYNC_ON	0

/*C_ONOFF_FORCE*/
#define UNFORCE		0
#define FORCEOFF	1
#define FORCEON		2

/*C_ANA_FORCE*/
#define ANA_FORCE	1

/*IOST_SET*/
#define IOST_TAG_OFF	0
#define IOST_TAG_ON	1
#define	IOST_TAG_ON2	2
#define IOST_TAG_ON3	3

/* MAN_STAT_SET */
#define MAN_SET_OPEN	0
#define MAN_SET_CLOSE	1
#define MAN_SET_TRANSIT	2

/* MAN_CURVE_SET*/
#define CRUVE_REAL_VALUE	0

/* TRANS*/
#define TRANS_RAISE	1
#define TRANS_DOWN	2
#define TRANS_STOP	3
#define TRANS_RAISE_TO	4
#define TRANS_DOWN_TO	5

/* DATA_POLLING*/
#define DATA_POLLING	0

/*LCU_PARAM*/
#define PARAM_POLLING 	1
#define PARAM_DOWNLOAD	2

#define	GATE_IN_OPEN	1
#define	GATE_IN_CLOSE	0

#define	LOCK_ON		0
#define	LOCK_OFF	1


#define	MAN_SET		1
#define	NORMAL_STATUS	0

#define	PROJ_CMD_TYPE_MAX	58	
#define	PROJ_CMD_TYPE_MIN	30	
#define	IMPORTANT_CMD_FROM 	47

#define	IOST_TYPE		1	
#define IOATR_TYPE		2

/* command source	*/
#define LOCAL_CMD_SOURCE	0
#define AGC_CMD_SOURCE		1
#define AVC_CMD_SOUTCE		2
#define CASCADE_CMD_SOURCE      3
#define	PROV_CMD_SOURCE		4
#define NET_CMD_SOURCE  	5
#define ND_CMD_SOURCE		6
#define REGIONAL_CMD_SOURCE	7

#define	UNIT_IN_STANDBY	0	
#define	UNIT_IN_GEN	1
#define	UNIT_IN_CHKOUT	2
#define	UNIT_IN_COMP	3	
#define	UNIT_IN_UNSYN	4

#define	IN_REMOTE_CTRL	1
#define	IN_LOCAL_CTRL 	0

#define IN_DEV_CLOSE	1
#define	IN_DEV_OPEN	0

#define	IN_AUTO_CTRL	1
#define	IN_MANUL_CTRL	0

#define	PLT_CTRL_MANUAL	1
#define	PLT_CTRL_AUTO	2
#define	PLT_CTRL_RMT	4
#define	PLT_CTRL_LOCAL	3
#define	PLT_OPEN	5
#define	PLT_CLOSE	6

#define	IN_CLOSE_LOOP	1
#define	IN_OPEN_LOOP	0

#define	VALID_DATA_NUM	6	


#define ON 1
#define	VOLT_UP_TEST_OK		1
#define	VOLT_UP_TEST_ERR	0


#define	ENABLE 1
#define	DISABLE 0
/*
typedef struct
{
        char    orig_data_type;
        char    usr_id;
}       TWO_CHAR;

typedef union
{
        TWO_CHAR       shchr;
        short		short_data;
}       CHAR_TO_SHORT;          
CHAR_TO_SHORT	cmd_last_short;
*/



#define	IN_HOST		1
#define	IN_VICE		2
#define	IN_EXIT		0	

#define	BE_SOUND	1
#define	NO_SOUND	0
