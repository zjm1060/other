#ifndef	_LCUDATA_H
#define	_LCUDATA_H

#define	 MAX_THR_NUM	10	/* Max thread number of on LCU */
#define  MAX_SCAN_BUF   2048	/* Max word size of plc scan buffer */
#define  PLCCMD_BUF_SIZE	10	/* word size of cmd buf */
#define  MAX_PLCCMD_SEG_NUM	60
#define  MAX_SOEALM_NUM	200
#define  MAX_OBJALM_NUM	100
#define	 SEND_DATA_SIZE	100
#define	 SEND_ANA_SIZE	74	/* max ana point num to be sent to plc */
#define	 SEND_ANA_QLT_WORD_SIZE	((SEND_ANA_SIZE-1)/16+1)  /* word */
#define	 SEND_POL_SIZE	((SEND_DATA_SIZE-SEND_ANA_SIZE-SEND_ANA_QLT_WORD_SIZE-5)*16)	/* bit */
#define	 ANA_QLT_SENDOFF	(SEND_ANA_SIZE + 5)
#define	 POLSENDOFF	(SEND_ANA_SIZE + SEND_ANA_QLT_WORD_SIZE + 5)	 		
#define	 SOE_ALM_LEN	7	/* word length of one soe alarm record */
#define	 OBJ_ALM_LEN	8	/* word length of one obj alarm record */
#define  MAX_SOEALM_BUF	(MAX_SOEALM_NUM*SOE_ALM_LEN*2)
#define  MAX_OBJALM_BUF	(MAX_OBJALM_NUM*OBJ_ALM_LEN*2)
#define  MAX_POL_BUF	5	/* max buffer num of pol added by yyp 06.02.17 */
#define  SV_SIZE	30	/* SET VALUE STRUCT SIZE */
#define  AI_SIZE	2048	/* AI STRUCT SIZE */
#define  AIC_SIZE	2048	
#define  AICOMM_SIZE	2048	
#define  RTD_SIZE	2000	/* RTD STRUCT SIZE */
#define  AIALL_SIZE	AI_SIZE+AIC_SIZE+AICOMM_SIZE+RTD_SIZE	/* AI & DI & PI all-send SIZE */

#define  PI_SIZE	1000	/* PTI STRUCT SIZE */
#define  PICOMM_SIZE	600	/* PTI STRUCT SIZE */
#define  DII_SIZE	3000	/* SOE STRUCT SIZE */
#define  DIN_SIZE	5000	/* DIN STRUCT SIZE */
#define  DI_SIZE	DII_SIZE + DIN_SIZE	/* DIN+DII STRUCT SIZE */
#define  DIC_SIZE	3000
#define  DICOMM_SIZE	5000
#define  MSG_SIZE	1600	/* MSG STRUCT SIZE */
#define  AO_SIZE	1600	/* AO STRUCT SIZE */
#define  DOUT_SIZE	3000	/* DOUT SIZE */
#define  OBJ_SIZE	1600
#define  COMAND_SIZE	80	/* Command receive struct size */
#define  PARAM_SIZE	1600	/* PARAMETER CORRECTION STRUCT SIZE */
#define	 MAX_FLAG_NUM	32000	/* DATA UPDATE  FLAG MAX NUMBER */
#define	 MAX_PLC_FLAG	32000	/* PLC DATA UPDATE  FLAG MAX NUMBER */

/********************************************************************************************************************************************
* head struct of plc data, consistent the plc data area 
* note: in plc_data, the byte sequence differ from variable and data buffer:
* variable: such as variable "ai_size" in head,  the byte sequence is the same with OS. if in SUN, high first,if in MSWIN32,low first
* recv data buf: such as ana data buf  "ai_en[]", the byte sequence is static: low first  ,high second
* send data buf: such as plc send data buf "send_data[]", the byte sequence is static: high first,low second
*********************************************************************************************************************************************/
struct plc_data_seg_head
{
	unsigned short  objalm_start_addr;		/* 4001989 */
	unsigned short	objalm_max;			/* 4001992 */
	unsigned short	objalm_ptr;			/* 4001999 */	
	unsigned short  obj_size;			/* 4001973 */
	unsigned short  obj_start_addr;			/* 4001973 */
	unsigned short  ai_size;			/* 4001973 */		/* all addr may not be right */
	unsigned short  aic_size;			/* 4001973 */
	unsigned short  aicomm_size;			/* 4001973 */
	unsigned short	ai_off;				/* 4001980 */
	unsigned short	aic_off;			/* 4001980 */
	unsigned short	aicomm_off;			/* 4001980 */	
	unsigned short  ai_start_addr;			/* 4001988 */
	unsigned short  aic_start_addr;			/* 4001988 */
	unsigned short  aicomm_start_addr;		/* 4001988 */			
	unsigned short  rtd_size;			/* 4001973 */
	unsigned short	rtd_off;			/* 4001980 */
	unsigned short  rtd_start_addr;			/* 4001988 */			
	unsigned short	imp_size;			/* 4001974 */
	unsigned short	impcomm_size;			/* 4001974 */	
	unsigned short	imp_off;			/* 4001981 */
	unsigned short	impcomm_off;			/* 4001981 */		
	unsigned short  imp_start_addr;			/* 4001988 */
	unsigned short  impcomm_start_addr;		/* 4001988 */	
	unsigned short  ao_size;			/* 4001975 */
	unsigned short	ao_off;				/* 4001982 */	
	unsigned short  ao_start_addr;			/* 4001988 */	
	unsigned short  soe_size;			/* 4001976 */
	unsigned short	soe_off;			/* 4001984 */
	unsigned short  soe_start_addr;			/* 4001990 */	
	unsigned short  msg_size;			/* 4001978 */
	unsigned short	msg_off;			/* 4001986 */
	unsigned short  msg_start_addr;			/* 4001990 */	
	unsigned short  dout_size;			/* 4001979 */
	unsigned short	dout_off;			/* 4001987 */	
	unsigned short  pol_size;			/* 4001977 */
	unsigned short  polc_size;			/* 4001977 */	
	unsigned short  polcomm_size;			/* 4001977 */	
	unsigned short	pol_off;			/* 4001985 */
	unsigned short	polc_off;			/* 4001985 */	
	unsigned short	polcomm_off;			/* 4001985 */	
	unsigned short  pol_start_addr;			/* 4001990 */
	unsigned short  polc_start_addr;		/* 4001990 */
	unsigned short  polcomm_start_addr;		/* 4001990 */		
	unsigned short	cmd_size;
	unsigned short  cmd_start_addr;			/* 4001990 */				
	unsigned short  soealm_start_addr;		/* 4001989 */
	unsigned short	soealm_max;			/* 4001992 */
	unsigned short	soealm_ptr;			/* 4001999 */
	unsigned short	param_off;			/* 4001999 */
	unsigned short	param_size;			/* 4001999 */
	unsigned short  param_start_addr;		/* 4001990 */
	unsigned short  param_update_flag;		/* 4001990 */	
	unsigned short  send_start_addr;		/* 4001990 */
	unsigned short  cpuflag;			/* 4001990 */			
	unsigned short  host_firstrun;			/* 4001994 */	/* High. host; Low. firstrun */
	unsigned short  timetag_flag;			/* 4001993 */
	unsigned short  ai_sec_ms;			/* 4001995 */
	unsigned short 	ai_hour_min;			/* 4001996 */
	unsigned short 	ai_month_day;			/* 4001997 */
	unsigned short  ai_year;			/* 4001998 */
};

/* struct of pol package added by yyp 2006.02.17 */
struct pol_data_seg
{
	unsigned short	backup;			
	unsigned short	pol_length;
	unsigned short	pol_timetag_flag;
	unsigned short	pol_sec_ms;
	unsigned short	pol_hour_min;
	unsigned short	pol_month_day;
	unsigned short	pol_year;
	unsigned char	pol_data[DIN_SIZE/16*2];
	unsigned char	dout_data[DOUT_SIZE/16*2];
	unsigned char	pol_qlt[DIN_SIZE/16*2];
	unsigned char	pol_en[DIN_SIZE/16*2];
	unsigned char	dout_qlt[DOUT_SIZE/16*2];
	unsigned char	dout_en[DOUT_SIZE/16*2];
	unsigned char	polc_data[DIC_SIZE/16*2];
	unsigned char	polcomm_data[DIC_SIZE/16*2];
	unsigned char	polcomm_qlt[DIC_SIZE/16*2];
	unsigned char	polcomm_en[DIC_SIZE/16*2];				
};

struct obj_data_seg
{
	unsigned char	status1[2];			
	unsigned char	status2[2];
	unsigned char	param[2];	
};

struct com_data_seg
{
	unsigned short  firstscan;
	unsigned short  ai_size;
	unsigned short	ai_off;				
	unsigned char  	ai_data[AI_SIZE*2];
	unsigned char  	ai_qlt[AI_SIZE/16*2];	
	unsigned char  	ai_en[AI_SIZE/16*2];
	unsigned short	imp_size;			
	unsigned short	imp_off;				
	unsigned char	pi_data[PI_SIZE*4];
	unsigned char	pi_qlt[PI_SIZE/16*2];	
	unsigned char	pi_en[PI_SIZE/16*2];	
	unsigned short	pol_size;			
	unsigned short	pol_off;				
	unsigned char	pol_data[DIN_SIZE/16*2];
	unsigned char	pol_qlt[DIN_SIZE/16*2];
	unsigned char	pol_en[DIN_SIZE/16*2];
};

struct plc_data_seg
{
	struct plc_data_seg_head head;
	unsigned char  	ai_data[AI_SIZE*2];
	unsigned char  	ai_qlt[AI_SIZE/16*2];	
	unsigned char  	ai_en[AI_SIZE/16*2];	
	unsigned char	ao_data[AO_SIZE*2];
	unsigned char	ao_qlt[AO_SIZE/16*2];
	unsigned char	ao_en[AO_SIZE/16*2];
	unsigned char  	aic_data[AIC_SIZE*2];
	unsigned char  	aicomm_data[AICOMM_SIZE*2];
	unsigned char  	aicomm_qlt[AICOMM_SIZE/16*2];	
	unsigned char  	aicomm_en[AICOMM_SIZE/16*2];	
	unsigned char	rtd_data[RTD_SIZE*4];		/* needed to be modified yyp 2.16 */
	unsigned char	rtd_qlt[RTD_SIZE/16*2];	
	unsigned char	rtd_en[RTD_SIZE/16*2];	
	unsigned char	pi_data[PI_SIZE*4];
	unsigned char	pi_qlt[PI_SIZE/16*2];
	
	unsigned char	picomm_data[PI_SIZE*4];
	unsigned char	picomm_qlt[PI_SIZE/16*2];
			
	unsigned char  	soe_data[DII_SIZE/16*2];
	unsigned char  	soe_qlt[DII_SIZE/16*2];	
	unsigned char  	soe_en[DII_SIZE/16*2];	
	unsigned char	msg_data[MSG_SIZE/16*2];	
	
	struct pol_data_seg pol_data[MAX_POL_BUF];

	struct obj_data_seg obj_data[OBJ_SIZE];
	
	unsigned char  	soealm_buf[MAX_SOEALM_BUF]; 

	unsigned char  	objalm_buf[MAX_OBJALM_BUF]; 
	
	unsigned char   param_data[PARAM_SIZE*2];	/* 4008401 */
	
	unsigned short  param_update_flag;	/* for dpp  */ 
	unsigned short 	ana_update_flag;		
	unsigned short 	rtd_update_flag;
	unsigned short 	imp_update_flag;
	unsigned short 	obj_update_flag;	
	unsigned short 	sysmsg_update_flag;
	unsigned short 	pol_seg_ptr;
	unsigned short	soealm_ptr;
	unsigned short	objalm_ptr;
	unsigned short  firstscan;
	unsigned short  endpp;
	
	unsigned short com_port_no;	/*for communication data */
	struct com_data_seg com_data[16];
	
	
	unsigned char  ai_all_send_flag; /*for polling cmd in cmd.lib*/
	unsigned char  ao_all_send_flag;
	unsigned char  imp_all_send_flag;
	unsigned char  pol_all_send_flag;
	unsigned char  dout_all_send_flag;
	unsigned char  dii_all_send_flag;
	unsigned char  msg_all_send_flag;	
	unsigned char  param_all_send_flag;
	unsigned char  obj_all_send_flag;
	
	unsigned char  com_ai_all_send_flag;
	unsigned char  com_imp_all_send_flag;
	unsigned char  com_pol_all_send_flag;
	
	unsigned short	head_thr_valid; 
	unsigned char	stnid;
	unsigned char 	devid;	
};

struct plc_data 
{
	short			valid;
	struct plc_data_seg	seg[MAX_PLCDATA_SEG_NUM];
};

/* seg of plc_send struct */	/* modified  */
struct plc_send_seg
{
	unsigned short  send_update_flag;	/* 4001820 */
	unsigned short  ai_size;		/* 4001821 */
	unsigned short	pol_size;		/* 4001822 */
	unsigned short  aiqlt_base;		/* 4001899 */
	unsigned short	pol_base;		/* 4001904 */
	char  	ana_data[SEND_ANA_SIZE*2];
	char	ana_qlt[SEND_ANA_QLT_WORD_SIZE*2];
	char	pol_data[SEND_POL_SIZE/16*2];
};

/* struct of data shared memory,to be sent to plc */
struct plc_send 
{
	struct plc_send_seg  seg[MAX_STN_NUM+1][MAX_LCU_NUM+1];
};

struct send_cmd
{
	unsigned char cmd_flag;	
	unsigned char type_id;
	unsigned char stn_id;
	unsigned char lcu_id;
	unsigned char src_id;
	unsigned char cmd_src_id;
	unsigned short cmd_w[7];
};

struct plc_cmd_seg 
{
	unsigned char stnid;
	unsigned char devid;
	union
	{
		unsigned short	data[PLCCMD_BUF_SIZE];
		struct send_cmd	cmd;
	}cmd_buf;
};

struct plc_cmd
{
        short head;
        struct plc_cmd_seg seg[MAX_PLCCMD_SEG_NUM]; 
};



#define	PLC_DATA_SIZE	(sizeof(struct plc_data))
#define	PLC_CMD_SIZE    (sizeof(struct plc_cmd))
#define	PLC_SEND_SIZE   (sizeof(struct plc_send))
#define	PLCHEADLEN	(sizeof(struct plc_data_seg_head))	/* PLC original data head size(WORD)  */

#define REGSEG          8

#define HEADBASE_MB	1940		/* read from plc data base addr */
#define ANABASE_MB	2000		/* ana data base addr */
#define SWBASE_MB	10001		/* sw map data base addr */ 
#define SWBASE_PRE	10001		/* sw map data base addr for premium yyp 07.12.20 */ 
#define OBJBASE_MB	4000
#define CMDBASE_MB	1680		/* cmd data base addr */
#define CMDSIZE_MB	14
#define SOEALMMAX_MB	200
#define SOEALMBASE_MB	7000	/* soe alarm data base addr */
#define PARAMBASE_MB	8401	/* Param data base addr */
#define SENDBASE_MB		1820	/* send to plc data base addr */
#define OBJALMMAX_MB	100
#define OBJALMBASE_MB	5000	/* obj alarm data base addr */
#define PLCRUNADDR_MB	1920	/*  run flag base addr */
#define IPCRUNADDR_MB	1921	/* plc run flag base addr */
#define SENDANAQLTBASE_MB	1899	
#define SENDPOLBASE_MB	1904

#define HEADBASE_SIE	0		/* read from plc data base addr */
#define ANABASE_SIE		0		/* ana data base addr */
#define SWBASE_SIE		0
#define OBJBASE_SIE		0
#define CMDBASE_SIE		0		/* cmd data base addr */
#define CMDSIZE_SIE		12
#define SOEALMBASE_SIE	0
#define SOEALMMAX_SIE	100
#define PARAMBASE_SIE	0
#define SENDBASE_SIE	2
#define OBJALMMAX_SIE	100
#define OBJALMBASE_SIE	0		/* obj alarm data base addr */
#define PLCRUNADDR_SIE	0		/* plc run flag base addr */
#define IPCRUNADDR_SIE	0		/* plc run flag base addr */
#define SENDANAQLTBASE_SIE	79	
#define SENDPOLBASE_SIE	84 	 


#define HEADBASE_GE	1940		/* read from plc data base addr */
#define ANABASE_GE	2000		/* ana data base addr */
#define SWBASE_GE	2300		/* sw map data base addr */ 
#define OBJBASE_GE	3100
#define CMDBASE_GE	1720		/* cmd data base addr */
#define CMDSIZE_GE	1
#define SOEALMMAX_GE	60
#define SOEALMBASE_GE	2500	/* soe alarm data base addr */
#define PARAMBASE_GE	3000	/* Param data base addr */
#define SENDBASE_GE	1820	/* send to plc data base addr */
#define OBJALMMAX_GE	100
#define OBJALMBASE_GE	3300	/* obj alarm data base addr */
#define PLCRUNADDR_GE	1920	/* plc run flag base addr */
#define IPCRUNADDR_GE	1921	/* plc run flag base addr */
#define SENDANAQLTBASE_GE	1899	
#define SENDPOLBASE_GE	1904

#define HEADBASE_AB	1940		/* read from plc data base addr */
#define ANABASE_AB	2000		/* ana data base addr */
#define SWBASE_AB	10001		/* sw map data base addr */ 
#define OBJBASE_AB	4000
#define CMDBASE_AB	1680		/* cmd data base addr */
#define CMDSIZE_AB	14
#define SOEALMMAX_AB	200
#define SOEALMBASE_AB	7000	/* soe alarm data base addr */
#define PARAMBASE_AB	8401	/* Param data base addr */
#define SENDBASE_AB		1820	/* send to plc data base addr */
#define OBJALMMAX_AB	100
#define OBJALMBASE_AB	5000	/* obj alarm data base addr */
#define PLCRUNADDR_AB	1920	/* plc run flag base addr */
#define IPCRUNADDR_AB	1921	/* plc run flag base addr */
#define SENDANAQLTBASE_AB	1899	
#define SENDPOLBASE_AB	1904

#define	RECVPDUHEAD	9			/*  pdu head of modbus added by yyp 06.01.20		*/
#define	PARAMFLAGPLC 	514		/* the param update flag: by plc yyp 06.02.17		*/
#define	PARAMFLAGSCADA 	257		/* the param update flag: by scada yyp 06.02.17		*/
#define	PARAMFLAGNO  	0		/* the param update flag: no yyp 06.02.17			*/
#define	SENDFLAGPLC 	0		/* the sending data update flag: by plc yyp 06.02.17 */
#define	SENDFLAGSCADA 	257		/* the sending data update flag: by scada yyp 06.02.17  */
#define	SENDFLAGNO  	0		/* the sending data update flag: no yyp 06.02.17	*/


#endif /*_LCUDATA_H*/
