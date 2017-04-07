/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Date : 2007.4.8 
    WINDOWS SYN  TO  UNIX  by chi 2007.05.13  

---------------------------------------------------------------------*/



#ifndef   _FIELD_DEF_H 
#define   _FIELD_DEF_H 



#define	BAR_GRPH_UP		0
#define	BAR_GRPH_RIGHT		1
#define	BAR_GRPH_LEFT		2
#define	BAR_GRPH_DOWN		3
#define	BAR_GRPH_UP_C		4
#define BAR_GRPH_RIGHT_C	5
#define	BAR_GRPH_LEFT_C		6
#define	BAR_GRPH_DOWN_C		7		


int	dio_type;

#define	OUTPUT_ONLY		0
#define	IN_OUT_PUT		1
#define	INPUT_ONLY		2


   	
int	qi_loc_type;

#define	QI_LOC_NONE		0
#define	QI_LOC_LEFT		1
#define QI_LOC_RIGHT		2
#define	QI_LOC_UP		3
#define QI_LOC_DOWN		4



int	tag_loc_type;

#define	TAG_NONE		0
#define	TAG_UP	 		1
#define	TAG_DOWN		2
#define	TAG_LEFT		3
#define	TAG_RIGHT		4
#define	TAG_UP_LEFT		5
#define	TAG_UP_RIGHT		6
#define	TAG_DOWN_LEFT		7
#define	TAG_DOWN_RIGHT		8

int	data_class_type;

#define	LONG_NAME		0
#define	LOGIC_NAME		1
#define	RT_VALUE		2
#define	RT_STATE		3
#define	ALM_PROHIBIT		4
#define OUT_OF_SCAN		5
#define	Channel_Fail		6
#define	PT_CALC			7
#define	H_LMT			8
#define	L_LMT			9
#define	HH_LMT			10
#define	LL_LMT			11
#define	HRTN_LMT		12
#define	LRTN_LMT		13
#define	DEAD_BAND		14
#define	ENG_CVTER		15

#define	ALM_ACK			16
#define	SCAN_ADDR		17
#define	CTRL_ADDR		18
#define	FAULT_TAG		19
#define	FAULT_PNT			20	
#define	Unit_Name			21
#define	IO_HDB			22
#define	AUDIO_ALM		23
#define	Man_Set			24
#define	Pt_CTRL			25	
#define	Open_Num		26
#define	Close_Num		27
#define	Hi_Code			28
#define	Lo_Code			29
#define	Pt_LongName		30
#define	Pt_Sgzy			31
#define Soe              	32
#define Dec_Num               33
#define SgOpnNum                34
#define SgClsNum                35
#define DevName			36 
#define	Pt_TrndAlm		37

/* The following added by zyp on 060419*/
#define	HIRANGE			38
#define	LORANGE			39
#define	BASE		        40	
#define	COMPENSATE		41
#define StnName			42 
#define Tr_WARN			43
#define Tr_RECOVER		44
#define Ctrl_lock_state 45
#define Update_State	46
#define Repeat_Alm		47
#define Tag_State		48
#define Io_Group		49
#define Proctype		50
#define Alm_Cba			51
#define Fail_Pnt		52
#define Key_Signal		53
#define Reverse_Pnt		54
#define Double_Pnt		55
#define Virtual_Pnt		56
#define Lcu_Virtual		57
#define Cascade_Pnt		58
#define Hdb_Curve		59
#define Hdb_Period		60
#define Terminal_No		61
#define Unit_No			62
#define Slot_No			63
#define Chanel_No		64
#define Dsp_Filename	65
#define Upate_Time		66
#define Bj_update_time	67
#define Lmt_State	68
#define TrLmt_State	69
#define CmdName         70
#define Condc0          71
#define Condc1          72
#define Ma              73
#define C0              74
#define C1              75
#define A0              76
#define A1              77
#define Swgf_n          78
#define P0              79
#define P1              80
#define Localm          81
#define Lculm           82
#define Resetf          83
#define Actt            84
#define Pulset          85
#define Addr            86
#define O0              87
#define O1              88
#define Orderm          89
#define Posd0i          90
#define Posd1i          91
/* 060419 end*/


int	Impulse_Set_Type;

/********************	Impulse_Set_Type  ****************************************/
#define IMP_ACCUM               0
#define ACCUM                   1	/* added on 2006.8 */
#define PEAK_DAY		2
#define PEAK_MONTH		3
#define PEAK_YEAR		4
#define VALLEY_DAY		5
#define VALLEY_MONTH		6
#define VALLEY_YEAR		7
/**/
#define	NORM_DAY		8
#define	NORM_MONTH		9
#define NORM_YEAR		10	
#define	TOTAL_DAY		11
#define TOTAL_MONTH  		12 	
#define	TOTAL_YEAR		13	
#define	PPEAK_DAY		14
#define PPEAK_MONTH  		15 	
#define	PPEAK_YEAR		16	
#define	DAY_SUM_INI		17/* added on 2006.8 */
#define	MONTH_SUM_INI		18
#define	YEAR_SUM_INI		19
#define CVT_F			20
#define PT_CVT                  21
#define CT_CVT                  22
#define IMP_REAL_NUM                  23
#define IMP_NUM_SAV                  24

#define LAST_DAY                  25
#define LAST_MONTH            26
#define LAST_YEAR            27

#define IMP_CONSTANT             28

/***********************************************************************************/




 
#define	FORM_WGT_NORMAL		0



#define	CREATE_DYN_TYPE		1
#define	MODIFY_DYN_TYPE		0


#define	NORMAL_VALUE		0	
#define	OVER_LMT_W		1
#define	OVER_LMT_A		2

#define REQ_HOURLY      0
#define REQ_DAYLY       1
#define REQ_MONTHLY     2
#define REQ_YEARLY      3

#define NULL_PKVLY      0
#define PEAK            1
#define VALLY           2
#define PPEAK           3              
#define OPN_NUM         4
#define CLS_NUM         5
#define SG_OPN_NUM      6
#define SG_CLS_NUM      7
#define TST_OPN_NUM     8
#define TST_CLS_NUM     9
#define	DAY_MAX		10
#define	AVERG		11
#define SUM		12




#endif 