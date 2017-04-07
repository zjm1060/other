/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong  2006.4.8 	  
	2007.06.22 : have been SYNC  by chichi;
	LAST UPATE : 2007.06.22 by chichi ; 
	LAST UPATE : 2007.08.06 by chichi ;
 
---------------------------------------------------------------------*/



#ifndef   _GTK_ALARM_TAB_H 
#define   _GTK_ALARM_TAB_H 

#if defined(SUN_OS)
#define	AlarmPrintFile   "/home/ems/h9000/tmp/AlarmPrintFile.tmp"
#endif
/*add by hcl for print*/
#if defined(WINDOWS_OS)
#define	AlarmPrintFile   "\\home\\ems\\h9000\\tmp\\AlarmPrintFile.tmp" 
#endif 
/************************************************************************/
/* 打印写临时文件时 用于对齐 相当于x坐标 
/* 用于表示每一项开始的位置                                              */
/************************************************************************/
#define 	PRINTTITLELOCATION_L0      0
#define 	PRINTTITLELOCATION_L1      18 
#define 	PRINTTITLELOCATION_L2      37
#define 	PRINTTITLELOCATION_L3      60
#define 	PRINTTITLELOCATION_L4      101
#define 	PRINTTITLELOCATION_L5      119
#define 	PRINTTITLELOCATION_L6      129

#define 	PRINTELEMENTLOCATION_L0      0		  /*序号*/
#define 	PRINTELEMENTLOCATION_L1      9		  /*时间*/
#define 	PRINTELEMENTLOCATION_L2      31		  /*逻辑名*/
#define 	PRINTELEMENTLOCATION_L3      56		  /*信息名称*/
#define 	PRINTELEMENTLOCATION_L4      102	  /*实时状态*/
#define 	PRINTELEMENTLOCATION_L5      119      /*meseeage 无标题项*/
#define 	PRINTELEMENTLOCATION_L6      129      /*用户*/

#define PRINTELEMENT       6
#define PRINT_RULER_LEN   138




#define MAX_SEL_RECORD_NUM	100 
#define MAX_DEV_MASK_NUM	10
#define MAX_GROUP_MASK_NUM	50
#define MAX_ATTRIB_NUM		16

#include  "./alarm_str.h"
typedef struct
{
	UCHAR	stn_id ;
	UCHAR	dev_id;
	UINT	stn_mask;
	UCHAR   rec_type;   /*added by chi ;*/
	UCHAR   cmd_type ;  /*added by chi ; */
	UINT	dev_mask[MAX_DEV_MASK_NUM];
	UINT	datatype_mask;
	UINT	type_id_mask;/* added by zyp */
	UINT	fault_mask;  /* added by zyp */
	UINT	group_mask[MAX_GROUP_MASK_NUM];  /* added by dm */
	UINT	attrib_mask[2];					 /* added by dm */
	time_t	start_time;
	time_t	end_time; 
	char	search_str[MAX_SEARCH_NAME_SIZE]; 
    char    LogicName[50] ;
	USHORT  crt_n ; 
    REC_HEAD    *phead;
    REC_ENTRY_UNION    *pRec ;   
	USHORT	is_change;
	USHORT	change_flag;
	USHORT  isNoAck  ;   // added for LJT to select record that is no acknowledged!
} FILTER_T;


enum
{
	eAttribFault,
	eAttribFail,
	eAttribScanEnable,
	eAttribAlarmEnable,
	eAttribAlarmNoAck,
	eAttribCtrlLock,
	eAttribNoUpdate,
	eAttribAlmRepeat,
	eAttribTagState,
	attrib_num
};

enum
{
	COLUMN_CHECKED,
	COLUMN_NAME,
	NUM_COLUMNS
};

enum
{
	eName_Filter,
	eStnId_Filter,
	eCmdType_Filter,
	eStnMask_Filter,
	eFaultMask_Filter,
	eTypeIdMask_Filter,
	eDevMask_Filter,
	eGroupMask_Filter,
	eDataTypeMask_Filter,
	eAttribMask_Filter,
	eStartTime_Filter,
	eEndTime_Filter,
	eLogicName_Filter,
	eLongName_Filter,
	num_columns_Filter
};

typedef struct
{
     UINT       pt ; 
	 USHORT		sel ;
}  INDEX_PT;


typedef struct
{
     UINT       nextpos ; 
	 USHORT		sel[MAX_SEL_RECORD_NUM] ;
}  SEL_PT;

typedef struct
{
	 INDEX_PT 	index_pt[MAX_SHM_RECORD_NUM] ;
	 USHORT	count     ;
	 USHORT	lastcount   ;
	 USHORT header  ;
     USHORT buff_size;
	 
} INDEX;   


typedef struct
{
	 short CurPage;
	 short PageCount;
	 short PageStart;
	 short PageEnd ;
	 short PageMod ; /* mod = recordcound % pagesize  */
         char  isSelected ;
         short SelectStart;
         short SelectEnd;
	 short RecCount ;
	 short PageSize;
         XRECT  mycur ;
} PAGE_INFO;


/*#define	NEW_TO_END*/
#define DSP_ALM_CNT 10
#define  FONT_SIZE_ALARM  "sans 13"  // ((char*)(info[0].event_font))  //  "sans 13"
#define  FONT_SIZE_FAULT_ALM  "sans 15"
#define ALM_PAGE_SIZE     26  /* must be  <=30 for tcp packed length */
		


#define 	ALM_Y0      25
#define         ALM_Y1      60
#define         FAULT_ALM_Y1        15
#define 	ALM_X0      (30-10)
#define 	ALM_X1      (100-20)
#define 	ALM_X2      (280-20)
#define 	ALM_X3      (490 -60-100)
#define 	ALM_X4      (780-100)
#define         ALM_X5      1140
#define         ALM_ROW     35


#define 	FAULT_ALM_X0      30
#define 	FAULT_ALM_X1      280
#define 	FAULT_ALM_X2      580
#define 	FAULT_ALM_X3      780
#define 	FAULT_ALM_X4      880
#define         FAULT_ALM_X5      1140


#define DPS_IO_FAIL			88

#define ALM_REC_REAL			0
#define ALM_REC_HISTRY			1

POINTER		pointer;
extern	char	name_item[POINT_LONGNAME_SIZE]; 

int	alarm_type_sav[MAX_CRT_NUMBER];
int	alm_rec_mode[MAX_CRT_NUMBER];	
 



#define ALM_HEAD_COL	5




#endif

