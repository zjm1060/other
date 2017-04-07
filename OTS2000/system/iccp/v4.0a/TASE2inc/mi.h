/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1998 - 2002, All Rights Reserved		        */
/*									*/
/* MODULE NAME : mi.h    						*/
/* PRODUCT(S)  : MI							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 07/26/02  RKR     50    changed MI_NUM_REMOTE_UI from 1 to 2         */
/* 06/20/02  RKR     49    Copyright year update                        */
/* 05/21/02  MDE     48    Added MI_ASSOC_STATE_START_SERVER		*/
/* 05/09/02  MDE     47    Added mi_rebind_remote and associated	*/
/* 05/08/02  MDE     46    Added num_dsts_rpt_pend to MI_CONN  		*/
/* 05/03/02  MDE     45    Added mi_bind_remote				*/
/* 04/17/02  MDE     44    Added mic_reset_all_dv_mismatch, cleanup	*/
/* 04/05/02  MDE     43    Added mi_get_rem_dv && associated		*/
/* 04/05/02  MDE     42    Misc. minor 					*/
/* 03/25/02  MDE     40	   Added matching control parameters		*/
/* 03/21/02  MDE     39	   Removed Remote AR info from MI_REMOTE	*/
/* 02/18/02  MDE     38	   Redundancy support enhancements		*/
/* 02/12/02  MDE     37	   Added mi_find_mvl_cfg_info			*/
/* 02/07/02  MDE     36	   Added mi_abort_all_assoc			*/
/* 02/05/02  MDE     35	   Added mi_find_mvl_cfg_info			*/
/* 01/25/02  MDE     34	   More redundancy control hooks		*/
/* 01/22/02  MDE     33	   Added redundancy control hooks		*/
/* 12/03/01  MDE     32	   Fixed mis_add_device_ex, mis_add_device	*/
/* 11/28/01  MDE     31	   Added and used MIS_DV_INDEX			*/
/* 11/23/01  NAV     30    add _mi_num_active_assoc			*/
/* 11/15/01  MDE     29	   Changed some mic_xxx function argument order	*/
/* 11/15/01  MDE     28	   Changed the ST_UCHAR ???[1] to ST_UCHAR ???	*/
/* 11/15/01  MDE     27	   Reworked the Flags defines and macros	*/
/* 11/07/01  MDE     26	   Added a number of helper functions		*/
/* 10/29/01  MDE     25	   Moved assoc list under remote		*/
/* 10/29/01  MDE     24	   Added configuration logging functions	*/
/* 10/29/01  MDE     23	   Added 'handle_info' to mic_add_device_to_vcc	*/
/* 10/29/01  MDE     22	   Added mic_add_dv_to_vcc_ex	function	*/
/* 10/29/01  MDE     21	   Added mis_find_dv_ex	function		*/
/* 10/29/01  MDE     20	   Added optional name fields to some struct	*/
/* 			   added 'find remote'				*/
/* 10/19/01  MDE     19	   Dynamic local object reconfiguration changes	*/
/* 10/19/01  MDE     18	   Removed limits on DV per DS			*/
/* 10/19/01  MDE     17	   Increased capacity to 1024K DV and DEV	*/
/* 04/13/01  ASK     16	   added mic_delete_ds 				*/
/* 12/14/00  RKR     15    added num of ds to MIS_VCC                   */
/* 12/07/00  RKR     14    added mis_create_dsts_names                  */
/* 12/05/00  RKR     13    changed nested comment			*/
/* 10/03/00  NAV     12    Add MIS_SEND_DENIED_RESP to block5		*/
/* 09/21/00  RKR     11    Fixed nested comments			*/
/* 09/07/00  NAV     10    Changes for time_t intervals			*/
/* 09/01/00  NAV     09    Fix the Tag Flags				*/
/* 07/11/00  NAV     08    Add mic_add_dv_to_vcc_ex			*/
/*			   Add user_info to MIC_DSTS			*/
/*			   Add MIS_SEND_UNAVAIL_RESP			*/
/* 08/21/00  NAV     07    Set some globals 'extern'                    */
/* 11/24/99  MDE     06    Add active_ar_name_index to MI_REMOTE	*/
/* 11/21/99  NAV     05    Add global mi_op_err				*/
/* 09/16/99  NAV     04    Add Block 4 and 5 Support			*/
/* 04/12/99  MDE     03    Cleanup, comments, etc.			*/
/* 04/01/99  MDE     02    Added MI_ASSOC_CTRL and associated code	*/
/* 12/31/98  MDE     01    Created					*/
/************************************************************************/

#ifndef MI_DEFS_INCLUDED
#define MI_DEFS_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "mmsdefs.h"
#include "mms_err.h"
#include "mms_pcon.h"
#include "mvl_acse.h"
#include "mi_log.h"

/************************************************************************/
/************************************************************************/

/* Comment this out to save local DV table size */
#define MI_REDUNDANCY_SUPPORT

/************************************************************************/
/************************************************************************/
/* MI COMMON								*/
/************************************************************************/

#define VCC_SPEC		VMD_SPEC
#define ICC_SPEC		DOM_SPEC
#define MI_UNKNOWN_SCOPE	-1	/* for use in mic_find_dv only 	*/

/************************************************************************/
/* Supported_Features defines - used to set and test block support bits	*/

#define MI_BLOCK1 	0x80	/* First byte */
#define MI_BLOCK2 	0x40
#define MI_BLOCK3 	0x20
#define MI_BLOCK4 	0x10
#define MI_BLOCK5 	0x08
#define MI_BLOCK6 	0x04
#define MI_BLOCK7 	0x02
#define MI_BLOCK8 	0x01
#define MI_BLOCK9 	0x80	/* Second byte */
#define MI_BLOCK10 	0x40	
#define MI_BLOCK11 	0x20
#define MI_BLOCK12 	0x10

/************************************************************************/
/* DATA VALUE BITMASK DEFINES						*/

#define MI_DATA_STATE_MASK			0xC0
/* Some recommended mappings for state values */
#define MI_DATA_STATE_BETWEEN			0x00
#define MI_DATA_STATE_TRIPPED			0x40
#define MI_DATA_STATE_CLOSED			0x80
#define MI_DATA_STATE_OFF			0x40
#define MI_DATA_STATE_ON			0x80
#define MI_DATA_STATE_AUTO			0x40
#define MI_DATA_STATE_MANUAL			0x80
#define MI_DATA_STATE_NORMAL			0x40
#define MI_DATA_STATE_ALARM			0x80
#define MI_DATA_STATE_LOCAL			0x40
#define MI_DATA_STATE_REMOTE			0x80
#define MI_DATA_STATE_RAISE			0x40
#define MI_DATA_STATE_LOWER			0x80
#define MI_DATA_STATE_READY			0x40
#define MI_DATA_STATE_NOTREADY			0x80
#define MI_DATA_STATE_OFFLINE			0x40
#define MI_DATA_STATE_AVAILABLE			0x80


/* Data_Flags and Data_State defines */
#define MI_QFLAG_VALIDITY_MASK			0x30
#define MI_QFLAG_VALIDITY_VALID			0x00
#define MI_QFLAG_VALIDITY_HELD			0x10
#define MI_QFLAG_VALIDITY_SUSPECT		0x20
#define MI_QFLAG_VALIDITY_NOTVALID		0x30

#define MI_QFLAG_CURR_SOURCE_MASK		0x0c
#define MI_QFLAG_CURR_SOURCE_TELEMETERED	0x00
#define MI_QFLAG_CURR_SOURCE_CALCULATED		0x04
#define MI_QFLAG_CURR_SOURCE_ENTERED		0x08
#define MI_QFLAG_CURR_SOURCE_ESTIMATED		0x0c

#define MI_QFLAG_NORMAL_VAL_MASK		0x02
#define MI_QFLAG_NORMAL_VAL_NORMAL		0x00
#define MI_QFLAG_NORMAL_VAL_ABNORMAL		0x02

#define MI_QFLAG_TIMESTAMP_MASK			0x01
#define MI_QFLAG_TIMESTAMP_VALID	       	0x00
#define MI_QFLAG_TIMESTAMP_INVALID	       	0x01

/* For convenience ... */
#define MI_SET_DATA_STATE(f,mask)   {f &= ~MI_DATA_STATE_MASK;        f |= mask;}
#define MI_QSET_VALIDITY(f,mask)    {f &= ~MI_QFLAG_VALIDITY_MASK;    f |= mask;}
#define MI_QSET_CURR_SOURCE(f,mask) {f &= ~MI_QFLAG_CURR_SOURCE_MASK; f |= mask;}
#define MI_QSET_NORMAL_VAL(f,mask)  {f &= ~MI_QFLAG_NORMAL_VAL_MASK;  f |= mask;}
#define MI_QSET_TIMESTAMP(f,mask)   {f &= ~MI_QFLAG_TIMESTAMP_MASK;   f |= mask;}


/* DSConditions defines */
#define MI_DSC_INTERVAL 	0x80
#define MI_DSC_INTEGRITY	0x40
#define MI_DSC_CHANGE		0x20
#define MI_DSC_OPERATOR		0x10
#define MI_DSC_EXTERNAL		0x08

/************************************************************************/
/* ICCP TYPE DEFINES AND TYPEDEFS 					*/
/************************************************************************/
/* Block 1 and Block 2 support						*/
/************************************************************************/

/* mi_type defines, used to select one of the ICCP data types 		*/
#define MI_NUM_MI_TYPES 24

#define MI_TYPEID_REAL 			0
#define MI_TYPEID_STATE			1
#define MI_TYPEID_DISCRETE		2
#define MI_TYPEID_REAL_Q		3
#define MI_TYPEID_STATE_Q		4
#define MI_TYPEID_DISCRETE_Q		5
#define MI_TYPEID_REAL_Q_TIMETAG	6
#define MI_TYPEID_STATE_Q_TIMETAG	7
#define MI_TYPEID_DISCRETE_Q_TIMETAG	8
#define MI_TYPEID_REAL_EXTENDED	        9
#define MI_TYPEID_STATE_EXTENDED       10
#define MI_TYPEID_DISCRETE_EXTENDED    11
#define MI_LAST_DV_TYPEID	       MI_TYPEID_DISCRETE_EXTENDED
#define MI_TYPEID_DV_SPECIAL	       12 /* special variable flag	*/
#define MI_TYPEID_DV_RESERVED_1	       13	
#define MI_TYPEID_DV_RESERVED_2	       14	
#define MI_TYPEID_DV_RESERVED_3	       15	


/* These type ID's are used independent of Data Values and cannot	*/
/* fit into the DV reference mi_type field (4 bits wide)		*/
#define MI_TYPEID_SUPPORTED_FEATURES   16
#define MI_TYPEID_VISIBLE_STRING_32    17
#define MI_TYPEID_TASE2_VERSION	       18
#define MI_TYPEID_MMS_OBJECT_NAME      19
#define MI_TYPEID_DSTS		       20
#define MI_TYPEID_GMT_BASED_S	       21
#define MI_TYPEID_INT16	 	       22
#define MI_TYPEID_DS_COND	       23


/* Typedefs for the ICCP data types. These types are used to exchange	*/
/* data between MI and the application. The of data being transferred	*/
/* type is identified by a mi_type value, such as 'MI_TYPEID_REAL'	*/

typedef ST_FLOAT MI_REAL;

typedef ST_UCHAR MI_STATE;  /* Bitstring */

typedef ST_INT32 MI_DISCRETE;

typedef struct
  {
  ST_FLOAT Value;
  ST_UCHAR Flags;
  } MI_REAL_Q;

typedef ST_UCHAR MI_STATE_Q;  /* Bitstring */

typedef struct
  {
  ST_INT32 Value;
  ST_UCHAR Flags;
  } MI_DISCRETE_Q;

typedef struct
  {
  ST_FLOAT Value;
  ST_INT32 TimeStamp;
  ST_UCHAR Flags;
  } MI_REAL_Q_TIMETAG;

typedef struct
  {
  ST_INT32 TimeStamp;
  ST_UCHAR Flags;
  } MI_STATE_Q_TIMETAG;

typedef struct
  {
  ST_INT32 Value;
  ST_INT32 TimeStamp;
  ST_UCHAR Flags;
  } MI_DISCRETE_Q_TIMETAG;

typedef struct
  {
  ST_FLOAT Value;
  ST_INT32 TimeStamp;
  ST_UCHAR Flags;
  ST_UINT16 COV;
  } MI_REAL_EXTENDED;

typedef struct
  {
  ST_INT32 TimeStamp;
  ST_UCHAR Flags;
  ST_UINT16 COV;
  } MI_STATE_EXTENDED;

typedef struct
  {
  ST_INT32 Value;
  ST_INT32 TimeStamp;
  ST_UCHAR Flags;
  ST_UINT16 COV;
  } MI_DISCRETE_EXTENDED;


/************************************************************************/
/* TRANSFER SET DATA 							*/
/* This data structure is used by the client to set up a transfer set,	*/
/* and by the server as a server variable and part of the TS control	*/

typedef struct _mi_dsts_data
  {
  struct
    {
    ST_UINT8 scope;
    ST_CHAR DomainName[MAX_IDENT_LEN+1];
    ST_CHAR name[MAX_IDENT_LEN+1];
    } DataSetName;
  time_t StartTime;
  ST_INT16 Interval;
  ST_INT16 TLE;
  ST_INT16 BufferTime;
  ST_INT16 IntegrityCheck;
  ST_UINT8 DSConditionsRequested;
  ST_BOOLEAN BlockData;
  ST_BOOLEAN Critical;
  ST_BOOLEAN RBE;
  ST_BOOLEAN Status;
  ST_INT16 EventCodeRequested;
  } MI_DSTS_DATA;

typedef struct	    
  {
  ST_UINT8 Scope;
  ST_CHAR DomainName[33];
  ST_CHAR Name[33];
  } MI_MMS_OBJECT_NAME;

typedef ST_UCHAR MI_DS_CONDITIONS;   /* Bitstring */
typedef ST_INT16 MI_EC_DETECTED;
typedef ST_INT32 MI_TS_TIMESTAMP;

/************************************************************************/
/* This union is used to allocate temporary storage for all data value 	*/
/* types, including the special transfer objects.			*/

typedef union _mi_dv_data_union
  {
  MI_REAL		r;
  MI_STATE		s;
  MI_DISCRETE		d;
  MI_REAL_Q		rq;
  MI_STATE_Q		sq;
  MI_DISCRETE_Q		dq;
  MI_REAL_Q_TIMETAG	rqt;
  MI_STATE_Q_TIMETAG	sqt;
  MI_DISCRETE_Q_TIMETAG	dqt;
  MI_REAL_EXTENDED	re;
  MI_STATE_EXTENDED	se;
  MI_DISCRETE_EXTENDED	de;

/* Special Transfer Objects */
  MI_MMS_OBJECT_NAME ts_name;
  MI_DS_CONDITIONS   ds_cond;
  MI_EC_DETECTED     ec_detected;
  MI_TS_TIMESTAMP    ts_timestamp;
  } MI_DV_DATA_UNION;

/************************************************************************/
/* Block 4 support							*/
/************************************************************************/

#define MI_NUM_BLOCK4_TYPES   3

#define	MI_TYPEID_IMTRANSET   0
#define MI_TYPEID_INFO_HEADER 1
#define	MI_TYPEID_INFO_BUFF   2


/************************************************************************/
/* Block 5 support							*/
/************************************************************************/

#define MI_NUM_BLOCK5_TYPES 5

#define MI_TYPEID_DEV_COMMAND	0
#define MI_TYPEID_DEV_DISCRETE	1
#define MI_TYPEID_DEV_REAL	2
#define MI_TYPEID_DEV_CHECKBACK	3
#define MI_TYPEID_DEV_TAGVALUE	4

typedef ST_INT16 MI_CONTROL_COMMAND;
typedef ST_FLOAT MI_CONTROL_SETPOINT_REAL;
typedef ST_INT16 MI_CONTROL_SETPOINT_DISCRETE;

typedef ST_INT16 MI_SBO_CHECKBACK;

typedef ST_UCHAR MI_TAG_FLAGS;	/* BitString */
#define MI_TAG_FLAG_NO_TAG	        0x00
#define MI_TAG_FLAG_OPEN_CLOSE_INHIBIT	0x40
#define MI_TAG_FLAG_CLOSE_INHIBIT	0x80
#define MI_TAG_FLAG_ARMED		0x20

typedef ST_CHAR MI_TEXT_STRING[256];

typedef struct
  {
  MI_TAG_FLAGS		TagFlags;
  MI_TEXT_STRING	Reason;
  } MI_TAG_VALUE;

/************************************************************************/
/* This union is used to allocate temporary storage for all device value*/
/* attributes.								*/

typedef union _mi_dev_value_union
  {
  MI_SBO_CHECKBACK		check_back_id;
  MI_CONTROL_COMMAND		cmd;
  MI_CONTROL_SETPOINT_REAL	set_pt_r;
  MI_CONTROL_SETPOINT_DISCRETE	set_pt_d;
  MI_TAG_VALUE			tag_value;
  } MI_DEV_VALUE_UNION;


/************************************************************************/
/* Bilateral_Table_Id length limit - do not change */
#define	MI_MAX_BLT_ID_LEN 32

typedef struct
  {
  ST_UCHAR Supported_Features[2];			/* VCC_SPEC */
  ST_CHAR Bilateral_Table_Id[MI_MAX_BLT_ID_LEN+1];	/* ICC_SPEC */
  struct
    {
    ST_INT16 MajorVersionNumber;
    ST_INT16 MinorVersionNumber;
    } TASE2_Version;					/* VCC_SPEC */
  } MI_ASSOCIATION_ATTRIBUTES;

/************************************************************************/
/* MI REMOTE NODE CONTROL						*/

/* Maximum number remote AE per VCC. Note that this is NOT the max	*/
/* number of connections to a VCC, but rather is the max number of 	*/
/* aliases a remote node may use  					*/
#define MI_MAX_AE_PER_VCC   4

/* The user may want to have more than 2 user_info ptrs to work with	*/
#if !defined (MI_NUM_REMOTE_UI)
#define MI_NUM_REMOTE_UI	2
#endif

typedef struct _mi_remote
  {
  DBL_LNK l;		/* internal use */
  ST_CHAR *name;	/* optional 	*/
  ST_VOID *user_info[MI_NUM_REMOTE_UI];

/* Server Related Elements */
  struct _mis_vcc *mis_vcc; 	/* Server VCC control			*/

/* Client Related Elements */
  struct _mic_vcc *mic_vcc; 	/* Client VCC control			*/

/* Calling Related Elements */
  INIT_INFO locl_init_info;	/* Local Initiate PDU info as sent	*/

  struct _mi_conn *mi_conn_list;
  struct _mi_assoc_ctrl *mi_assoc_list;

#ifdef MI_REDUNDANCY_SUPPORT
/* Redundancy data sync control */
  struct _irDataValueListCtrl *irDvListCtrl;

/* Configuration CRC for this remote */
  ST_INT32 cfgCrc;
  ST_INT32 partnerCfgCrc;

/* State & Stats for this remote */
  ST_BOOLEAN syncActive;
  ST_LONG  dataSyncCount;
#endif
  } MI_REMOTE;
extern MI_REMOTE *mi_remote_list;

MI_REMOTE *mi_create_remote (ST_VOID);
ST_VOID    mi_free_remote   (MI_REMOTE *mi_remote);
MI_REMOTE *mi_find_remote (ST_CHAR *name);

/* Associate AE title's with the remote */
ST_RET mi_add_ae_to_remote (MI_REMOTE *mi_remote, ST_CHAR *ar_name);


/************************************************************************/
/* MI CONNECTION CONTROL						*/
/* The MI_CONN data structure is used to manage ICCP connections	*/

/* The user may want to have more than one user_info ptr to work with	*/
#if !defined (MI_NUM_CONN_UI)
#define MI_NUM_CONN_UI	1
#endif
					    
typedef struct _mi_conn
  {
  DBL_LNK l;			/* Internal use				*/

  ST_CHAR *remote_ar;		/* Remote node AR name			*/
  MI_REMOTE *mi_remote;
  struct _mi_assoc_ctrl *mi_assoc;
  ST_VOID *user_info[MI_NUM_CONN_UI];

/* Internal */
  MVL_NET_INFO *net_info;
  ST_BOOLEAN IM_Transfer_Set;	/* aa spec var for block 4 support*/
  ST_INT num_dsts_rpt_pend;
  } MI_CONN;


/************************************************************************/
/* MI ASSOCIATION CONTROL						*/

/* mi_assoc_ctrl ctrl flags defines - bit masked */
#define MI_ASSOC_CTRL_ENABLE		0x0001
#define MI_ASSOC_CTRL_CALLING		0x0002	/* we are calling */
#define MI_ASSOC_CTRL_CALLED		0x0004	/* we are called  */
#define MI_ASSOC_CTRL_CLIENT		0x0008	/* we are client  */
#define MI_ASSOC_CTRL_SERVER		0x0010	/* we are server  */

/* mi_assoc_ctrl state flags defines */
#define MI_ASSOC_STATE_IDLE	     	0
#define MI_ASSOC_STATE_ACTIVE	     	1
#define MI_ASSOC_STATE_PARAM_CHECK   	2
#define MI_ASSOC_STATE_INIT_REQ	     	3
#define MI_ASSOC_STATE_INIT_IND	     	4
#define MI_ASSOC_STATE_START_CLIENT	5
#define MI_ASSOC_STATE_START_SERVER	6
#define MI_ASSOC_STATE_CONCLUDING    	7
#define MI_ASSOC_STATE_ABORT	     	8
#define MI_ASSOC_STATE_ABORTING	     	9
#define MI_ASSOC_STATE_BACKOFF	       10
#define MI_ASSOC_STATE_INVALID	       11


/* defines for param_check_state */
#define MI_PARAM_UNKNOWN	     	0
#define MI_PARAM_OK		     	1
#define MI_PARAM_MISMATCH	     	2

/* The user may want to have more than one user_info ptr to work with	*/
#if !defined (MI_NUM_ASSOC_UI)
#define MI_NUM_ASSOC_UI	1
#endif
					    
typedef struct _mi_assoc_ctrl
  {
  DBL_LNK l;			/* Internal use				*/
  ST_CHAR *name;		/* optional 	*/

  ST_INT  ctrl_flags;		/* User sets these flags to control 	*/
  ST_INT  state;		/* MI sets flags to reflect state	*/
  ST_INT  param_check_state;	/* MI sets flags to reflect state	*/

  ST_CHAR *local_ar;		/* If NULL, uses default local AR	*/

/* Identify the remote node by one or more AE Title's */
  ST_INT num_remote_ar;		/* Remote ICCP node AR Names		*/
  ST_CHAR *remote_ar[MI_MAX_AE_PER_VCC];
  ST_INT active_ar_name_index;

/* Calling elements - user configures */
  time_t connect_backoff_time;	/* Retry delay time for failed connect	*/
  INIT_INFO def_locl_init_info;	/* Initiate info to send when calling	*/

/* Linkages to other MI control elements */
  MI_REMOTE *mi_remote;		/* Set by MI */
  MI_CONN *mi_conn;		/* Set by MI */

/* MMS Initiate Information */
  INIT_INFO locl_init_info;	/* Local Initiate PDU info as sent	*/
  INIT_INFO rem_init_info;	/* Remote Initiate info as rxd		*/
  ST_INT max_pdu_size;

/* Calling elements, internal use */
  ST_INT remote_ar_sel;
  time_t time_to_initiate;	/* Time we will initiate next		*/

/* User field(s) */
  ST_VOID *user_info[MI_NUM_ASSOC_UI];
  } MI_ASSOC_CTRL;

/* MI_ASSOC_CTRL management functions */
MI_ASSOC_CTRL *mi_create_assoc (MI_REMOTE *mi_remote);
ST_RET mi_add_ae_to_assoc (MI_ASSOC_CTRL *mi_assoc, ST_CHAR *ar_name);
ST_VOID mi_free_assoc (MI_ASSOC_CTRL *mi_assoc);
ST_VOID mi_free_remote_assoc (MI_REMOTE *mi_remote_sel);
MI_ASSOC_CTRL *mi_find_assoc (MI_REMOTE *mi_remote, ST_CHAR *name);

ST_VOID mi_enable_named_assoc (ST_CHAR *remote_name, ST_CHAR *assoc_name);
ST_VOID mi_disable_named_assoc (ST_CHAR *remote_name, ST_CHAR *assoc_name);

ST_VOID mi_enable_remote_assoc (MI_REMOTE *mi_remote);
ST_VOID mi_disable_remote_assoc (MI_REMOTE *mi_remote);

ST_VOID mi_enable_assoc (MI_ASSOC_CTRL *mi_assoc);
ST_VOID mi_disable_assoc (MI_ASSOC_CTRL *mi_assoc);
ST_VOID mi_abort_all_assoc (ST_VOID);

/* User defined MI_ASSOC_CTRL activity indication functions */

ST_VOID u_mi_association_active(MI_ASSOC_CTRL *mi_assoc);

#define MI_ASSOC_INACTIVE_ABORT_IND		0
#define MI_ASSOC_INACTIVE_ABORT_REQ		1
#define MI_ASSOC_INACTIVE_TERMINATE_IND		2
#define MI_ASSOC_INACTIVE_TERMINATE_CONF	3
#define MI_ASSOC_FAILED_ASSOC_REQ		4
ST_VOID u_mi_association_inactive(MI_ASSOC_CTRL *mi_assoc, ST_INT reason);

#define MI_ASSOC_IND_ERR_NOREMOTE		0
#define MI_ASSOC_IND_ERR_NOMISVCC		1
#define MI_ASSOC_IND_ERR_NOASSOC		2
ST_VOID u_mi_association_ind_err (ST_INT reason);

/************************************************************************/
/* General address matching control */
extern ST_BOOLEAN mi_match_allow_missing_ae_elements;
extern ST_BOOLEAN mi_match_allow_extra_ae_elements;

/* Local address matching control */   
extern ST_BOOLEAN mi_match_local_ap_title;
extern ST_BOOLEAN mi_match_local_ae_qualifier;
extern ST_BOOLEAN mi_match_local_ap_invoke;
extern ST_BOOLEAN mi_match_local_ae_invoke;
extern ST_BOOLEAN mi_match_local_psel;
extern ST_BOOLEAN mi_match_local_ssel;
extern ST_BOOLEAN mi_match_local_tsel;
extern ST_BOOLEAN mi_match_local_net_addr;

/* Remote address matching control */
extern ST_BOOLEAN mi_match_remote_ap_title;
extern ST_BOOLEAN mi_match_remote_ae_qualifier;
extern ST_BOOLEAN mi_match_remote_ap_invoke;
extern ST_BOOLEAN mi_match_remote_ae_invoke;
extern ST_BOOLEAN mi_match_remote_psel;
extern ST_BOOLEAN mi_match_remote_ssel;
extern ST_BOOLEAN mi_match_remote_tsel;
extern ST_BOOLEAN mi_match_remote_net_addr;

/************************************************************************/
/* MI REQUEST CONTROL							*/
/* The MI user can have one or more MMS client requests outstanding at 	*/
/* a time, and this control element is used to manage those outstanding */
/* requests								*/

/* The user may want to have more than one user_info ptr to work with	*/
#if !defined (MI_NUM_REQ_UI)
#define MI_NUM_REQ_UI	1
#endif

/* 'result' defines */
#define MI_RESULT_SUCCESS	0
#define MI_RESULT_FAILURE	1
#define MI_RESULT_ERROR_RESP	2
#define MI_RESULT_REJECT	3

typedef struct _mi_req_ctrl
  {
  MI_CONN *mi_conn;
  ST_BOOLEAN done;
  ST_RET result;      	/* SD_SUCCESS or error code			*/
  ST_VOID (*u_req_done) (struct _mi_req_ctrl *req);
  ST_VOID *user_info[MI_NUM_REQ_UI];

/* Output parameters */
  union
    {
    NAMELIST_RESP_INFO *gnl;
    GETVLIST_RESP_INFO *get_ds_names;
    ST_INT mi_type;
    ERR_INFO *err_info;
    REJECT_RESP_INFO *rej_info;
    MI_TAG_VALUE *tag_value;
    MI_SBO_CHECKBACK check_back_id;
    } o;

/* Internal Use */
  MVL_REQ_PEND *mvl_req;
  struct _mic_ds *mic_ds;
  ST_BOOLEAN data_value_read;
  struct _mi_read_req *mi_rd_req;
  struct _mi_write_req *mi_wr_req;
  struct _mic_wr_dv_ctrl *mic_wr_dv_ctrl; 
  struct _mic_device *mic_device;
  struct _mic_grobj_ctrl *mi_grobj_ctrl;

/* Dest reading client startup variables */
  MI_ASSOCIATION_ATTRIBUTES *misv;

  MI_MMS_OBJECT_NAME *next_dsts;
  struct _mic_dsts *mic_dsts;	    
  } MI_REQ_CTRL;

/* The 'u_req_done' function must call 'mi_free_req_ctrl' when it	*/
/* is finished with the confirm information.				*/
ST_VOID mi_free_req_ctrl (MI_REQ_CTRL *req);

/************************************************************************/
/* MI INITIALIZATION/TERMINATION 					*/

ST_RET mi_initialize (ST_VOID);
ST_RET mi_terminate (ST_VOID);
ST_VOID mi_service (ST_VOID);

/************************************************************************/
/************************************************************************/
/* Global MI operation error code, written when error is detected in	*/
/* functions which return pointers. Contains MVL_ERR code.		*/
/************************************************************************/

extern ST_RET mi_op_err;
extern ST_INT mi_wait_service_ms;

/************************************************************************/
/************************************************************************/
/* MIS SPECIFIC								*/
/************************************************************************/

/************************************************************************/
/* MIS VCC CONTROL DATA STRUCTURES					*/
/* Each ICCP Client is assigned to a MIS_VCC. The following data 	*/
/* structures are used to manage the object set available to the client	*/
/************************************************************************/
/* MIS DATA VALUE REFERENCE 						*/
/* This is used in table form for each VCC to select the variables that	*/
/* are to be exposed to the client node.				*/

/* Note: The remaining information in this section is a bit technical 	*/
/* and may be safely ignored by most developers.			*/
/* WARNING: MI currently supports a single DV table!			*/

#define MIS_DV_REF 	ST_UINT32
#define MIS_DV_INDEX 	MIS_DV_REF

/* Design Note: The MIS_DV_REF is used to select a server data value, 	*/
/* and the total storage required for these indexes will be equal to 	*/
/* the average number of DV per VCC times the number of VCC. For this 	*/
/* reason we would like to keep it small. By using a set of defines we 	*/
/* can change the capacity in the future w/o changing code.		*/

/* With a 32 bit reference, we can have up to 0x3FFFFFF DV total	*/
/* but by default we will give the user 8 bits to use, resulting in	*/
/* the following bit usage -						*/

/* 32 bit MIS_DV_REF bit usage, for 32 bit systems */
/* swttttuuuuuuxxxx xxxxxxxxxxxxxxxx							*/
/* swtt ttuu uuuu xxxx xxxx xxxx xxxx xxxx							*/

/* s : Scope (1 == ICC, 0 = VCC)    */
/* w : Write enable		    */
/* t : MIS type			    */
/* u : User Defined		    */
/* y : DV table select		    */
/* x : DV table index		    */

#define MIS_MAX_TOTAL_DV	0x000FFFFF  	/* 1024k total DV 	*/
#define MIS_INDEX_MASK		0x000FFFFF	/* 1024k DV per table	*/
#define MIS_MAX_DV_PER_TBL	MIS_INDEX_MASK
#define MIS_MAX_DV_TBLS		1
#define MIS_TABLE_MASK		0x00000000	/* 1 tables		*/
#define MIS_TABLE_SHIFT		0		/* Shift right for tbl	*/
#define MIS_DVREF_ICC		0x80000000	/* ICC scope bit  	*/
#define MIS_DVREF_WRITE_EN	0x40000000	/* Write Enable bit  	*/
#define MIS_DVREF_TYPE_MASK_A  	0x0000000F	/* After shift 		*/
#define MIS_DVREF_TYPE_MASK_B  	0x3C000000	/* Before shift 	*/
#define MIS_DVREF_TYPE_SHIFT 	26		/* Shift right 		*/
#define MIS_DVREF_USER_MASK  	0x03f00000	
#define MIS_DVREF_USER_SHIFT 	20		/* Shift right for tbl	*/

/************************************************************************/
/* MIS DATA VALUE CONTROL 						*/
/* Data Values are modeled as a shared set of named objects. Each DV	*/
/* has a name and user defined access information; the type and access	*/
/* privledges are determined by the VCC that references the DV.		*/

/* This element is used in table form to provide access to ALL ICCP	*/
/* accessible data values. Individual VCC's reference this table to 	*/
/* restrict the set of points available and to select the DV type. 	*/

/* The 'MIS_DV_ACCESS_INFO' data type define is used to allow the user	*/
/* to map appication data values to ICCP data values. By default this	*/
/* is a void *, but can be changed to another data type if desired.	*/

#if !defined (MIS_DV_ACCESS_INFO)
#define MIS_DV_ACCESS_INFO ST_VOID *
#endif

typedef struct _mis_dv
  {		  
  ST_CHAR *dv_name;
  MIS_DV_ACCESS_INFO access_info;
#ifdef MI_REDUNDANCY_SUPPORT
  MIS_DV_REF partner_dv_ref;	/* should be MIS_DV_INDEX? */
#endif
  } MIS_DV;

extern MIS_DV_INDEX *mis_dv_ctrl_index;	/* Sorted index array for DV	*/	
extern MIS_DV *mis_dv_ctrl; 		/* table of DV			*/
extern ST_INT mis_num_dv_ctrl;		/* Number of DV in tables	*/
extern ST_INT mis_num_sorted_dv_ctrl;	/* Number of sorted DV in tbl	*/
extern ST_INT mis_max_num_dv_ctrl; 	/* Size of table		*/

/************************************************************************/
/* MIS DATASET CONTROL 							*/

/* ds_flags defines */
#define MIS_DS_FLAGS_IN_USE		0x80
#define MIS_DS_FLAGS_WRITE_ENABLE	0x40

typedef struct _mis_ds
  {		  
  ST_CHAR *ds_name;
  ST_UINT8 ds_flags;
  ST_BOOLEAN mms_deletable;
  ST_INT num_dv_ref;		/* Variables in the dataset 	*/
  MIS_DV_REF *mis_dv_ref_tbl;	
  } MIS_DS;

/************************************************************************/
/* MIS TRANSFER SET CONTROL 						*/

/* dsts_flags defines, used internally by MI to manage DSTS elements	*/
#define MIS_DSTS_FLAGS_IN_USE		0x01
#define MIS_DSTS_FIRST_CHANGE		0x02
#define MIS_DSTS_REPORT_UNDERWAY	0x04

typedef struct _mis_dsts
  {		  
  ST_INT16 dsts_flags;
  ST_CHAR *ts_name;
  MI_TS_TIMESTAMP  ts_timestamp;
  MI_DS_CONDITIONS ds_conditions;   /* Bitstring */
  MI_EC_DETECTED   ec_detected;
  MIS_DS *mis_ds;

  ST_DOUBLE last_report_sent;
  ST_DOUBLE last_complete_report_sent;
  ST_DOUBLE first_change_reported;
  ST_UINT8 *changed_dv;
  ST_BOOLEAN saturated;

  ST_INT max_asn1_size;
  MI_DSTS_DATA mi_dsts_data;
  MI_CONN *mi_conn;

  time_t next_interval_rpt;
  time_t next_buffer_rpt;
  time_t next_integrity_rpt;
  } MIS_DSTS;		    

/************************************************************************/
/* MIS DEVICE REFERENCE 						*/
/* This is used in table form for each VCC to select the devices that	*/
/* are to be exposed to the client node.				*/

#define MIS_DEVICE_REF 		ST_UINT32
#define MIS_DEVICE_INDEX 	MIS_DEVICE_REF

/* With a 32 bit reference, we can have up to 0x3FFFFFF Devices total	*/
/* but by default we will give the user 8 bits to use, resulting in	*/
/* the following bit usage -						*/

/* 32 bit MIS_DEVICE_REF bit usage, for 32 bit systems 			*/
/* saaattuuuuuuuuxx xxxxxxxxxxxxxxxx					*/

/* s : Scope (1 = ICC, 0 = VCC)    		*/
/* a : access control		       		*/
/* t : MIS type			    		*/
/* u : User Defined		    		*/
/* x : Device table index		    	*/

#define MIS_MAX_TOTAL_DEVREF	0x0003FFFF  	/* 256k total DEVICE 	*/
#define MIS_MAX_DEVREF_PER_TBL	MIS_INDEX_MASK
#define MIS_MAX_DEVREF_TBLS		1
#define MIS_DEVREF_ICC		0x80000000	/* ICC scope bit  	*/
#define MIS_DEVREF_SELECT	0x40000000	/* Select Enabled	*/
#define MIS_DEVREF_OPERATE	0x20000000	/* Operate Enabled	*/
#define MIS_DEVREF_SETTAG	0x10000000	/* Set Tag Enabled	*/

						/* Type bits		*/
#define MIS_DEVREF_TYPE_MASK_A  0x00000003	/* After shift 		*/
#define MIS_DEVREF_TYPE_MASK_B  0x0C000000	/* Before shift 	*/
#define MIS_DEVREF_TYPE_SHIFT 	26		/* Shift right 		*/

/************************************************************************/
/* MIS DEVICE CONTROL 							*/
/* Devices are modeled as a shared set of named objects. Each Device	*/
/* has a name, tag and state information and user defined access info	*/
/* The type and access privledges are determined by the VCC that 	*/
/* references the device.						*/

#if !defined (MIS_DEVICE_ACCESS_INFO)
#define MIS_DEVICE_ACCESS_INFO ST_VOID *
#endif

#define MIS_DEVICE_STATE_IDLE		0
#define MIS_DEVICE_STATE_SELECTING	1
#define MIS_DEVICE_STATE_ARMED		2
#define MIS_DEVICE_STATE_OPERATING	3
#define MIS_DEVICE_STATE_TAGGED		4
#define MIS_DEVICE_STATE_BUSY		5

typedef struct _mis_device
  {		  
  ST_CHAR 		*device_name;

  ST_INT8		device_state;

  ST_BOOLEAN		sbo;
  MI_SBO_CHECKBACK	check_back_id;
  MI_REMOTE		*selecter;

  ST_BOOLEAN		taggable;
  MI_TAG_VALUE		tag_value;
  MI_REMOTE		*tagger;

  MIS_DEVICE_ACCESS_INFO access_info;
#ifdef MI_REDUNDANCY_SUPPORT
  MIS_DEVICE_REF partner_dev_ref;	/* should be MIS_DV_INDEX? */
#endif
  } MIS_DEVICE;

extern MIS_DEVICE_INDEX *mis_device_ctrl_index; /* sorted index		*/
extern MIS_DEVICE *mis_device_ctrl;	/* table of Device 		*/
extern ST_INT mis_num_device_ctrl;     	/* Number of Devices in table	*/
extern ST_INT mis_max_num_device_ctrl; 	/* Size of table		*/

/************************************************************************/
/* MIS VCC CONTROL 							*/
/* The server creates a VCC for each client. The VCC determines which	*/
/* Data Values are exposed to the client (thus implementing that part 	*/
/* of the bi-lat agreement). It is also used to manage Data Sets and 	*/
/* Transfer Sets, and specifies the client AE's that map to this VCC.	*/
/* also manages the devices						*/

typedef struct _mis_vcc
  {		  
  MI_REMOTE *mi_remote;

/* VCC Specific Data Values */
  ST_INT num_vcc_dv_ref;
  ST_INT max_vcc_dv_ref;
  MIS_DV_REF *vcc_dv_ref_tbl;

/* ICC Specific Data Values */
  ST_CHAR *icc_name;
  ST_INT num_icc_dv_ref;
  ST_INT max_icc_dv_ref;
  MIS_DV_REF *icc_dv_ref_tbl;

/* VCC Specific Data Sets */
  ST_INT max_vcc_ds;
  ST_INT num_vcc_ds;
  MIS_DS **vcc_ds_ctrl_tbl;	/* pointer to table of pointers to DS */

/* ICC Specific Data Sets */
  ST_INT max_icc_ds;
  ST_INT num_icc_ds;
  MIS_DS **icc_ds_ctrl_tbl;	/* pointer to table of pointers to DS */

/* Transfer_set Control */
  ST_INT num_dsts_ctrl;
  MIS_DSTS *mis_dsts_tbl;

  ST_INT min_interval;

/* VCC Specific Devices */
  ST_INT num_vcc_device_ref;
  ST_INT max_vcc_device_ref;
  MIS_DEVICE_REF *vcc_device_ref_tbl;

/* ICC Specific Devices */
  ST_INT num_icc_device_ref;
  ST_INT max_icc_device_ref;
  MIS_DEVICE_REF *icc_device_ref_tbl;

/* Special variable data */
  MI_ASSOCIATION_ATTRIBUTES misv;
  } MIS_VCC;

/************************************************************************/
/************************************************************************/
/* MIS OBJECT HANDLING FUNCTIONS					*/
/************************************************************************/
/* These functions are used to manage server objects, and are typically	*/
/* used at startup and reconfigure time only				*/

ST_RET mis_create_dsts_names(ST_INT max_num_dsts);

/************************************************************************/
/* DV management - add, delete, sort, find. Note that the DV tables 	*/
/* must be stable before the VCCs are created, and the tables must be 	*/
/* sorted after adding or deleting DVs (unless they are added in sorted	*/
/* order.								*/

ST_RET mis_create_dv_tables (ST_INT max_num_dv);
ST_RET mis_resize_dv_tables (ST_INT total_num_dv);
ST_RET mis_free_dv_tables (ST_VOID);
ST_RET mis_add_dv (ST_CHAR *dv_name, MIS_DV_ACCESS_INFO access_info);
ST_RET mis_add_dv_ex (ST_CHAR *dv_name, MIS_DV_ACCESS_INFO access_info, 
		     MIS_DV_INDEX *dv_index_out);
ST_VOID mis_sort_dv_tables (ST_VOID);

ST_RET mis_find_dv (ST_CHAR *dv_name, MIS_DV_INDEX *dv_index_out);

#define MIS_DV_REF_TO_DV(dvRef) (&mis_dv_ctrl[dvRef & MIS_INDEX_MASK])

ST_RET mis_find_vcc_dv (MIS_VCC *mis_vcc, 
			ST_CHAR *dv_name,
                        ST_INT scope,
			MIS_DV **mis_dv_out, 
			MIS_DV_REF *mis_dv_ref_out,
			ST_INT *mi_type_out);

ST_RET mis_objname_to_dv (MIS_VCC *mis_vcc, 
			  OBJECT_NAME *obj, 
			  MIS_DV **mis_dv_out, 
			  MIS_DV_REF *mis_dv_ref_out,
			  ST_INT *mi_type_out);

ST_RET mis_objname_to_ds (MIS_VCC *mis_vcc, 
			  OBJECT_NAME *obj, 
			  MIS_DS **mis_ds_out); 

/************************************************************************/
/* Device management - same rules as DV apply				*/

#define mis_create_device_table mis_create_device_tables
#define mis_free_device_table   mis_free_device_tables

ST_RET mis_create_device_tables (ST_INT max_num_device);
ST_RET mis_resize_device_tables (ST_INT max_num_device);
ST_RET mis_free_device_tables (ST_VOID);
ST_RET mis_add_device (ST_CHAR *device_name, MI_SBO_CHECKBACK check_back_id,
		       ST_BOOLEAN sbo, ST_BOOLEAN taggable,
		       MIS_DEVICE_ACCESS_INFO access_info);
ST_RET mis_add_device_ex (ST_CHAR *device_name, MI_SBO_CHECKBACK check_back_id,
                       ST_BOOLEAN sbo, ST_BOOLEAN taggable,
		       MIS_DEVICE_ACCESS_INFO access_info, 
		       MIS_DEVICE_INDEX *dev_index_out);

ST_VOID mis_sort_device_table (ST_VOID);

#define MIS_DEV_REF_TO_DEV(devRef) (&mis_device_ctrl[devRef & MIS_INDEX_MASK])
ST_RET mis_find_device (ST_CHAR *device_name, MIS_DEVICE_INDEX *device_index_out);

/************************************************************************/
/* VCC management */

MIS_VCC *mis_create_vcc (MI_REMOTE *mi_remote, 
		       ST_CHAR *bi_lat_tbl_id, ST_CHAR *icc_name, 
  		       ST_INT num_vcc_dv_ref, ST_INT num_vcc_ds, 
  		       ST_INT num_icc_dv_ref, ST_INT num_icc_ds, 
		       ST_INT num_ts_ctrl, ST_INT min_interval);
ST_VOID mis_free_vcc (MIS_VCC *mis_vcc);

/* Add Data Values to the VCC. Note that after DVs are added they must	*/
/* be sorted (unless they are added in sorted order).								*/
ST_RET mis_add_dv_to_vcc (MIS_VCC *mis_vcc, ST_INT dv_scope, 
			   ST_CHAR *dv_name, ST_INT mi_type, 
			   ST_BOOLEAN wr_enable);
ST_RET mis_add_dv_to_vcc_ex (MIS_VCC *mis_vcc, ST_INT dv_scope, 
			   ST_CHAR *dv_name, ST_INT mi_type, 
			   ST_BOOLEAN wr_enable, MIS_DV_REF *dv_ref_out);
ST_VOID mis_sort_vcc_dv (MIS_VCC *vcc);

/* Add Data Sets to the VCC. Note: DS's are sorted as they are added	*/
MIS_DS *mis_add_ds_to_vcc (MIS_VCC *vcc, ST_INT dv_scope, 
			   ST_CHAR *ds_name, ST_INT num_dv);
ST_VOID mis_delete_vcc_ds (MIS_VCC *mis_vcc, MIS_DS *mis_ds, 
			   ST_INT ds_scope);

/* Add Data Value to Data Sets						*/
ST_VOID mis_add_ds_dv (MIS_VCC *mis_vcc, MIS_DS *mis_ds, 
		       MIS_DV_REF mis_dv_ref);

/* Block 5 - device control 						*/
ST_RET mis_enable_device_ctrl (MIS_VCC *mis_vcc, ST_INT num_vcc_dev, 
			       ST_INT num_icc_dev);

ST_RET mis_add_device_to_vcc (MIS_VCC *mis_vcc, ST_INT device_scope, 
			      ST_CHAR *device_name, ST_INT device_type, 
			      ST_BOOLEAN select_access, 
			      ST_BOOLEAN operate_access,
			      ST_BOOLEAN settag_access);
ST_RET mis_add_device_to_vcc_ex (MIS_VCC *mis_vcc, ST_INT device_scope,
                              ST_CHAR *device_name, ST_INT device_type,
			      ST_BOOLEAN select_access,
			      ST_BOOLEAN operate_access,
			      ST_BOOLEAN settag_access, MIS_DEVICE_REF *device_ref_out);

ST_VOID mis_sort_vcc_devices (MIS_VCC *vcc);

ST_RET mis_find_vcc_device (MIS_VCC *mis_vcc, 
			    ST_CHAR *device_name,
                            ST_INT scope,
			    MIS_DEVICE **device_out, 
			    MIS_DEVICE_REF *device_ref_out,
			    ST_INT *mi_type_out);


/************************************************************************/
/* User calls this function to tell us that the Data Value changed	*/
/* This is used by MI in Transfer Set control				*/

ST_VOID mis_dv_change (MIS_DV_REF dv_ref);

/************************************************************************/
/* MIS DATA VALUE READ */

/* rd_reason defines */
#define MIS_RD_REASON_READ_IND	1
#define MIS_RD_REASON_REPORT	2

typedef struct _mis_rd_dv_ctrl
  {
  DBL_LNK l;	      	/* For use by application 			*/
  MIS_VCC *mis_vcc;   	/* VCC for which data is being presented 	*/

			/* Use MIS_DV_REF_TO_DV macro to get MIS_DV *	*/
  MIS_DV_REF mis_dv_ref;/* Additional Data Value information		*/
  ST_INT mi_type;	/* For ease of reference			*/
  ST_VOID *data_buf;   	/* Where to put the data			*/
  ST_INT rd_reason;

  ST_INT16 acc_rslt_tag;  /* ACC_RSLT_SUCCESS or ACC_RSLT_FAILURE	*/
  ST_INT16 failure;	  /* DataAccessError code for failure		*/

/* For internal use by MI */
  MI_DV_DATA_UNION dv_data;
  struct _mis_read_ctrl *mis_rd_ctrl;
  struct _mis_info_rpt_ctrl *mis_info_ctrl;
  MIS_DSTS *mis_dsts;
  ST_INT vl_index;	/* variable list index				*/
  } MIS_RD_DV_CTRL;


typedef struct _mis_read_ctrl
  {
  MIS_VCC *mis_vcc;

  READ_REQ_INFO   *read_req_info;
  ST_INT num_var;		/* Variables being read			*/

  ST_INT num_data_vals;		/* data values being read		*/
  MIS_RD_DV_CTRL  *mis_rd_dv_ctrl_tbl;

  ST_INT num_devices;		/* devices being read			*/
  struct _mis_device_ind_ctrl *mis_device_ctrl_tbl;

  ST_INT num_data_done;		/* Number complete			*/
  MVL_IND_PEND *mvl_ind_ctrl;
  } MIS_READ_CTRL;

/* User defined Data Value Read/Write function. All access to server	*/
/* Data Values takes place through this function. 			*/

ST_VOID u_mis_rd_dv (MIS_RD_DV_CTRL *rd_dv_ctrl);

/* The user calls this function when the data has been put in the dest	*/
/* buffer. The 'rc' parameter is SD_SUCCESS if the data is OK		*/
ST_VOID mis_rd_dv_done (MIS_RD_DV_CTRL *dv_ctrl, ST_RET rc);

/************************************************************************/

/* report_type defines */
#define MIS_REPORT_TYPE_NVL	0
#define MIS_REPORT_TYPE_LOV	1

typedef struct _mis_info_rpt_ctrl
  {
  MIS_VCC *mis_vcc;

  ST_INT num_var;		/* Variables being read			*/
  ST_INT num_included; 
  MIS_RD_DV_CTRL  *mis_rd_dv_ctrl_tbl;

  MIS_DSTS *mis_dsts;

  ST_INT num_data_done;		/* Number complete			*/
  ST_INT report_type;
  MVL_IND_PEND *mvl_ind_ctrl;
  } MIS_INFO_RPT_CTRL;

/************************************************************************/
/* MIS DATA VALUE WRITE 						*/

/* wr_reason defines */
#define MIS_WR_REASON_WRITE_IND	1

typedef struct _mis_wr_dv_ctrl
  {
  DBL_LNK l;	      	/* For use by application 			*/
  MIS_VCC *mis_vcc;   	/* VCC for which data is being presented 	*/

			/* Use MIS_DV_REF_TO_DV macro to get MIS_DV *	*/
  MIS_DV_REF mis_dv_ref;/* Additional Data Value information		*/
  ST_INT mi_type;	/* For ease of reference			*/
  ST_VOID *data_buf;   	/* Where to get the data			*/
  ST_INT wr_reason;

  ST_INT16 resp_tag;
  ST_INT16 failure;	  /* DataAccessError code for failure		*/

  MI_DV_DATA_UNION dv_data;

/* For internal use by MI */
  MIS_DSTS *mis_dsts;
  MVL_IND_PEND *ind_ctrl;
  struct _mis_write_ctrl *mis_wr_ctrl;
  ST_INT vl_index;	/* variable list index				*/
  } MIS_WR_DV_CTRL;


typedef struct _mis_write_ctrl
  {
  MIS_VCC *mis_vcc;

  WRITE_REQ_INFO  *write_req_info;
  ST_INT num_var;			/* Variables being written	*/

  ST_INT num_data_vals;			/* data values being written	*/
  MIS_WR_DV_CTRL  *mis_wr_dv_ctrl_tbl;

  ST_INT num_IM;			/* start/stop IMTransfers	*/
  struct _mis_im_ind_ctrl *mis_im_ctrl_tbl;

  ST_INT num_devices;			/* devices being written	*/
  struct _mis_device_ind_ctrl *mis_device_ctrl_tbl;

  ST_INT num_data_done;			/* Number complete		*/
  MVL_IND_PEND *mvl_ind_ctrl;
  } MIS_WRITE_CTRL;


/* User defined Data Value Read/Write functions. All access to server	*/
/* Data Values takes place through these functions. 			*/

ST_VOID u_mis_wr_dv (MIS_WR_DV_CTRL *wr_dv_ctrl);

/* The user calls this function when the write data data has been 	*/
/* processed. The 'rc' parameter is SD_SUCCESS if the data is OK	*/
ST_VOID mis_wr_dv_done (MIS_WR_DV_CTRL *dv_ctrl, ST_RET rc);

/************************************************************************/
/* MIS DATA SET ACTIVITY INDICATIONS					*/

/* This function is called by MI when a client is creating a data set.	*/
/* No activity is required on our part ...				*/
ST_RET u_mis_create_ds_ind (MIS_VCC *vcc, DEFVLIST_REQ_INFO *req_info);

/* This function is called by MI when a client is deleting a data set.	*/
/* No activity is required on our part ...				*/
ST_RET u_mis_delete_ds_ind (MIS_VCC *vcc, DELVLIST_REQ_INFO *req_info);

/************************************************************************/
/* MIS TRANSFER SET ACTIVITY INDICATIONS 				*/ 

/* activity_type defines */
#define MIS_DSTS_START_TRANSFER 1
#define MIS_DSTS_STOP_TRANSFER 	2
#define MIS_DSTS_BEGIN_REPORT 	3
#define MIS_DSTS_END_REPORT 	4
#define MIS_DSTS_ACK_RCVD 	5
#define MIS_DSTS_NACK_RCVD 	6

ST_RET u_mis_dsts_activity (MIS_VCC *vcc, MIS_DSTS *ts_ctrl, 
			    ST_INT activity_type, 
			    MI_TS_TIMESTAMP ts_timestamp);

/************************************************************************/
/* Server Block 4 Support						*/
/************************************************************************/


#define MI_IM_TRAN_SET_VAR_NAME	"IM_Transfer_Set"
#define MI_IM_INFO_MESS_HEADER	"Info_Mess_Header"
#define MI_IM_INFO_BUFF_NAME	"Info_Buff_"

typedef struct 
  {
  ST_INT32 InfoReference;			      
  ST_INT32 LocalReference;
  ST_INT32 MessageId;
  ST_INT32 Size;
  } MI_INFO_BUF_HEADER;

/************************************************************************/
/* struct used for IM_Transfer_Set write indications			*/

typedef struct _mis_im_ind_ctrl
  {
  ST_VOID *data_buf;	  
  ST_INT data_type;	  /* for encoding/decode			*/

  ST_INT16 acc_rslt_tag;  /* ACC_RSLT_SUCCESS or ACC_RSLT_FAILURE	*/
  ST_INT16 failure;	  /* DataAccessError code for failure		*/

  ST_BOOLEAN enable_transfer;  /* decoded data				*/

  /* internal use	*/
  MIS_WRITE_CTRL *mis_wr_ctrl;
  ST_INT vl_index;	/* variable list index				*/
  } MIS_IM_IND_CTRL;


ST_RET mis_enable_IMTransfers (MIS_VCC *vcc);	/* add block 4 support to*/
						/* the server vcc	*/

ST_RET mis_send_IMTransfer (ST_INT num_conn, MI_CONN **send_to,
			    MI_INFO_BUF_HEADER *buf_header, 
			    ST_INT buf_len, ST_UCHAR *info_buf, ST_INT msg_scope);

/* notification that the transfer set has been enabled by the client	*/
ST_VOID u_mis_start_IM_Transfer (MI_CONN *mi_conn);

/* notification that the transfer set has been disabled by the client	*/
ST_VOID u_mis_stop_IM_Transfer (MI_CONN *mi_conn);

/************************************************************************/
/* Block 5 Support							*/
/************************************************************************/

#define MI_DEVICE_OP_SELECT	0
#define MI_DEVICE_OP_OPERATE	1
#define MI_DEVICE_OP_GETTAG	2
#define MI_DEVICE_OP_SETTAG	3

typedef struct _mis_device_ind_ctrl
  {
  ST_INT op;		/* see MI_DEVICE_OP defines above		*/
  MIS_VCC *mis_vcc;   	/* VCC for which data is being presented 	*/
  MIS_DEVICE *mis_device;	/* Device being referenced		*/
  MIS_DEVICE_REF mis_device_ref;/* Additional Device information	*/
  ST_INT device_type;	/* For ease of reference			*/

  ST_VOID *data_buf;
  ST_INT data_type;	  /* for encoding/decode			*/

  ST_INT16 acc_rslt_tag;  /* ACC_RSLT_SUCCESS or ACC_RSLT_FAILURE	*/
  ST_INT16 failure;	  /* DataAccessError code for failure		*/

/* For internal use by MI */
  MI_DEV_VALUE_UNION device_data;
  MIS_READ_CTRL *mis_rd_ctrl;
  MIS_WRITE_CTRL *mis_wr_ctrl;
  ST_INT vl_index;	/* variable list index				*/
  } MIS_DEVICE_IND_CTRL;

/************************************************************************/
/* user functions that pass up device indications			*/
/* return MIS_SEND_DEFAULT_RESP for the mi to respond or return		*/
/* MIS_USER_SEND_RESP and call appropriate done function		*/
/************************************************************************/

#define MIS_SEND_DEFAULT_RESP	0
#define MIS_USER_SEND_RESP	1
#define MIS_SEND_UNAVAIL_RESP	2
#define MIS_SEND_DENIED_RESP	3

ST_RET u_mis_device_select (MIS_DEVICE_IND_CTRL *dev_ind_ctrl);
ST_RET u_mis_device_operate (MIS_DEVICE_IND_CTRL *dev_ind_ctrl);
ST_RET u_mis_device_settag (MIS_DEVICE_IND_CTRL *dev_ind_ctrl);
ST_RET u_mis_device_gettag (MIS_DEVICE_IND_CTRL *dev_ind_ctrl);

/************************************************************************/
/* done functions to respond to device indications			*/
/************************************************************************/
ST_VOID mis_device_select_done (MIS_DEVICE_IND_CTRL *dev_ind_ctrl, ST_RET rc);
ST_VOID mis_device_operate_done (MIS_DEVICE_IND_CTRL *dev_ind_ctrl, ST_RET rc);
ST_VOID mis_device_gettag_done (MIS_DEVICE_IND_CTRL *dev_ind_ctrl, ST_RET rc);
ST_VOID mis_device_settag_done (MIS_DEVICE_IND_CTRL *dev_ind_ctrl, ST_RET rc);

/************************************************************************/
/* server device support functions					*/
/************************************************************************/

ST_RET mis_device_reset (MIS_DEVICE *pdevice); 	/* transition device	*/
						/* from ARMED to IDLE	*/

ST_RET mis_device_busy (MIS_DEVICE *pdevice); 	/* transition device 	*/
						/* from IDLE to BUSY	*/

/************************************************************************/
/************************************************************************/
/* MIC SPECIFIC								*/
/************************************************************************/
/************************************************************************/
/* MIC DATA VALUE CONTROL */

/* The client constructs a MIC_VCC for each server node it want's to 	*/
/* talk to. Part of the VCC is the Data Values (realized as MIC_DV)	*/
/* which represent that part of the bi-lat table agreement.		*/

/* The 'MIS_DV_ACCESS_INFO' data type define is used to allow the user	*/
/* to map server data values to the application space. By default this	*/
/* is a void *, but can be changed to another data type if desired.	*/

#if !defined (MIC_DV_ACCESS_INFO)
#define MIC_DV_ACCESS_INFO ST_VOID *
#endif

/* dv_attrib defines, Note that we allow extra bits for the type so	*/
/* we have room for the non-data value types too			*/
#define MIC_DV_SCOPE_ICC 	0x0100
#define MIC_DV_MI_TYPE_MASK 	0x00FF
#define MIC_DV_MISMATCH 	0x0200

typedef struct _mic_dv
  {		  
  ST_CHAR *dv_name;
  ST_INT16 dv_attrib;			/* Scope, type */
  MIC_DV_ACCESS_INFO handle_info;
#ifdef MI_REDUNDANCY_SUPPORT
  ST_UINT32 partner_dv_ref;
#endif
  } MIC_DV;

/************************************************************************/
/* MIC DATASET CONTROL */

/* ds_flags defines */
#define MIC_DS_MMS_REQ_PEND		0x40
#define MIC_DS_READY			0x20

typedef struct _mic_ds
  {		  
  ST_CHAR *ds_name;
  ST_INT8 ds_scope;
  ST_INT8 ds_index;
  ST_UINT8 ds_flags;
  ST_INT num_dv; 		/* Variables in the dataset 		*/
  MIC_DV **mic_dv_tbl; 		/* Table of pointers to DV's		*/
  MIC_DV **mic_sorted_dv_tbl; 	/* sorted table of pointers to DV's	*/
  } MIC_DS;

/************************************************************************/
/* TRANSFER SET CONTROL */

/* The user may want to have more than one user_info ptr to work with	*/
#if !defined (MI_NUM_DSTS_UI)
#define MI_NUM_DSTS_UI	1
#endif

/* dsts_flags defines - used internally */
#define MIC_DSTS_FLAGS_IN_USE		0x01

typedef struct _mic_dsts
  {		  
  ST_INT16 dsts_flags;
  MI_MMS_OBJECT_NAME ts_name;	
  MIC_DS *mic_ds;
  MI_DSTS_DATA mi_dsts_data;
  MI_CONN *mi_conn;
  ST_VOID *user_info[MI_NUM_DSTS_UI];
  } MIC_DSTS;

/************************************************************************/
/* Block 5 client devices						*/

#if !defined (MIC_DEVICE_ACCESS_INFO)
#define MIC_DEVICE_ACCESS_INFO ST_VOID *
#endif

#define MIC_DEVICE_SCOPE_ICC 	0x0100
#define MIC_DEVICE_SBO		0x0010
#define MIC_DEVICE_TAGGABLE	0x0020
#define MIC_DEVICE_MI_TYPE_MASK 0x000F

typedef struct _mic_device
  {		  
  ST_CHAR *device_name;
  ST_INT16 device_attrib;	/* Scope, sbo/tag, type */
  MIC_DEVICE_ACCESS_INFO handle_info;
#ifdef MI_REDUNDANCY_SUPPORT
  ST_UINT32 partner_dev_ref;
#endif
  } MIC_DEVICE;


/************************************************************************/
/* MIC VCC CONTROL 							*/
/* The client creates a VCC for each server, which reflects the Data	*/
/* Value points of interest and maintains state information.		*/

#define	MI_MAX_BLT_ID_LEN 32

typedef struct _mic_vcc
  {		  
  MI_REMOTE *mi_remote;

/* VCC Specific Data Values */
  ST_INT   num_vcc_dv;
  ST_INT   max_vcc_dv;
  MIC_DV  **vcc_dv_tbl;

/* ICC Specific Data Values */
  ST_CHAR *icc_name;
  ST_INT   num_icc_dv;
  ST_INT   max_icc_dv;
  MIC_DV  **icc_dv_tbl;

/* VCC Specific Data Sets */
  ST_INT  max_vcc_ds;
  MIC_DS **vcc_ds_ctrl_tbl;	

/* ICC Specific Data Sets */
  ST_INT max_icc_ds;
  MIC_DS **icc_ds_ctrl_tbl;	

/* Transfer_set Control */
  ST_INT num_dsts_ctrl;
  MIC_DSTS *mic_dsts_tbl;

/* VCC Specific Devices */
  ST_INT   num_vcc_devices;
  ST_INT   max_vcc_devices;
  MIC_DEVICE  **vcc_device_tbl;

/* ICC Specific Devices */
  ST_INT   num_icc_devices;
  ST_INT   max_icc_devices;
  MIC_DEVICE  **icc_device_tbl;

/* Special variable data, must match server's values at connect time 	*/
  MI_ASSOCIATION_ATTRIBUTES misv;
  } MIC_VCC;

/************************************************************************/
/* MIC DATA VALUE WRITE 						*/
/* This structure is used when writing Data Values			*/

typedef struct _mic_dv_wr_info
  {
  MIC_DV *mic_dv; 		/* Data Value to Write			*/
  ST_VOID *wr_data;   		/* Data	 				*/
  ST_RET result;
  } MIC_DV_WR_INFO;

typedef struct _mic_wr_dv_ctrl
  {
  ST_INT num_dv;
  MIC_DV_WR_INFO *mic_dv_wr_info;  /* ptr to table of MIC_DV_WR_INFO	*/
  } MIC_WR_DV_CTRL;

/************************************************************************/
/* Special Transfer Set Objects 					*/
/* Used in sending ACK/NACK, and passed to user whene report rcvd	*/

typedef struct _mic_dsts_sto
  {		  
  ST_BOOLEAN ts_name_pres;
  ST_BOOLEAN ts_timestamp_pres;
  ST_BOOLEAN ds_cond_pres;
  ST_BOOLEAN ec_detected_pres;

  MI_MMS_OBJECT_NAME ts_name;
  MI_TS_TIMESTAMP    ts_timestamp;
  MI_DS_CONDITIONS   ds_cond;
  MI_EC_DETECTED     ec_detected;
  } MIC_DSTS_STO;

/************************************************************************/
/* MIC OBJECT MANAGEMENT						*/

/* VCC Management */
MIC_VCC *mic_create_vcc (MI_REMOTE *mi_remote, 
		       ST_CHAR *bi_lat_tbl_id, ST_CHAR *icc_name, 
  		       ST_UINT num_vcc_dv, ST_INT num_vcc_ds, 
  		       ST_UINT num_icc_dv, ST_INT num_icc_ds, 
		       ST_UINT num_ts_ctrl);
ST_VOID mic_free_vcc (MIC_VCC *mic_vcc);

MIC_DV *mic_add_dv_to_vcc (MIC_VCC *mic_vcc, ST_INT dv_scope, 
			      ST_CHAR *dv_name, ST_INT mi_type, 
			      MIC_DV_ACCESS_INFO handle_info);
ST_VOID mic_sort_vcc_dv (MIC_VCC *vcc);


ST_VOID mic_enable_device_ctrl (MIC_VCC *vcc, ST_INT num_vcc_dev,
			        ST_INT num_icc_dev);

MIC_DEVICE *mic_add_device_to_vcc (MIC_VCC *mic_vcc, ST_INT device_scope,
                                   ST_CHAR *device_name, ST_INT device_type,
			           ST_BOOLEAN selectable,
			           ST_BOOLEAN taggable, 
				   MIC_DEVICE_ACCESS_INFO handle_info);
ST_VOID mic_sort_vcc_devices (MIC_VCC *vcc);
MIC_DEVICE *mic_find_device (MIC_VCC *vcc, ST_CHAR *name, ST_INT scope); 

/************************************************************************/
/* MIC DATA VALUE							*/

/* Find a MIC_DV given the VCC, name, and scope	*/
MIC_DV *mic_find_dv (MIC_VCC *vcc, ST_CHAR *dv_name, ST_INT dv_scope); 
MIC_DV *mic_find_dv_in_ds (MIC_DS *mic_ds, ST_CHAR *dv_name);

/************************************************************************/
/* MIC DATA SET & TRANSFER SET MANAGEMENT				*/

/* Create a Data Set in the server */
MI_REQ_CTRL *mic_create_ds (MI_CONN *mi_conn, ST_CHAR *ds_name, 
			    ST_INT ds_scope, ST_INT num_dv, 
			    MIC_DV **mic_dv, MIC_DS **mic_ds_out);

/* Create the MIC_DS locally only (for staticaly defined NVL in server 	*/
ST_INT mic_define_ds (MI_REMOTE *mi_remote, ST_CHAR *ds_name, 
			    ST_INT ds_scope, ST_INT num_dv, 
			    MIC_DV **dv, MIC_DS **ds_out);

/* Delete a remote Data Set */
MI_REQ_CTRL *mic_delete_ds(MI_CONN *mi_conn, MIC_DS *mic_ds);

MIC_DS *mic_find_ds (MIC_VCC *mic_vcc, OBJECT_NAME *ds_obj);
MIC_DSTS *mic_find_dsts (MIC_VCC *mic_vcc, MIC_DS *mic_ds);

/* Start Transfer */
MI_REQ_CTRL *mic_start_dsts (MI_CONN *mi_conn, MIC_DS *mic_ds, 
			     MI_DSTS_DATA *mi_dsts_data, MIC_DSTS **mic_dsts_out);

/* Stop Transfer */
MI_REQ_CTRL *mic_stop_dsts (MI_CONN *mi_conn, MIC_DSTS *mic_dsts);

/* ACK/NACK Transfer */
ST_RET mic_ack_dsts (MI_CONN *mi_conn, MIC_DSTS *mic_dsts, 
		     MIC_DSTS_STO *mic_dsts_sto, ST_BOOLEAN nack);

/************************************************************************/
/* MIC READ/WRITE DATA VALUES AND DATA SETS				*/

/* Read */
MI_REQ_CTRL *mic_read_dv (MI_CONN *mi_conn, ST_INT num_dv, MIC_DV **dv);
MI_REQ_CTRL *mic_read_ds (MI_CONN *mi_conn, MIC_DS *ds); 

/* Write */
MI_REQ_CTRL *mic_write_dv (MI_CONN *mi_conn, MIC_WR_DV_CTRL *mic_wr_dv_ctrl);

MI_REQ_CTRL *mic_write_ds (MI_CONN *mi_conn, MIC_DS *ds, 
			    MIC_WR_DV_CTRL *mic_wr_dv_ctrl);

/************************************************************************/
/* MIC DATA VALUE DATA HANDLING						*/

/* DV indication Start/End user functions. These functions are used to 	*/
/* notify the user of the begin and end of a series of calls to		*/
/* 'u_mic_dv_ind'. A series of calls is made for a read confirm, and 	*/
/* for a DS Transfer.							*/

/* reason defines */
#define MIC_DV_IND_REASON_READ		1
#define MIC_DV_IND_REASON_REPORT	2
 

ST_VOID u_mic_dv_ind_start (MI_CONN *mi_conn, ST_INT reason, 
			    MIC_DSTS *mic_dsts);
ST_VOID u_mic_dv_ind_end   (MI_CONN *mi_conn, ST_INT reason,
			    MIC_DSTS *mic_dsts, MIC_DSTS_STO *mic_dsts_sto);

/* User defined Data Value Indication function, called when data is	*/
/* received via Read confirm or Information Report indication. 		*/

ST_VOID u_mic_dv_ind (MI_CONN *mi_conn, MIC_DV *dv, ST_RET result,
		      ST_VOID *data_buf, ST_INT mi_type);

/************************************************************************/
/* MIC GET NAMELIST							*/

#define MIC_OBJ_CLASS_VCC_DV	0
#define MIC_OBJ_CLASS_ICC_DV	1
#define MIC_OBJ_CLASS_VCC_DS	2
#define MIC_OBJ_CLASS_ICC_DS	3

MI_REQ_CTRL *mic_get_namelist (MI_CONN *mi_conn, 
			       ST_INT mi_obj_class, ST_CHAR *ca_name);

/* MIC GET DS NAMES							*/
MI_REQ_CTRL *mic_get_ds_names (MI_CONN *mi_conn, ST_CHAR *ds_name, 
			       ST_INT ds_scope);

/* MIC GET DV ATTRIBUTES						*/
MI_REQ_CTRL *mic_get_dv_attrib (MI_CONN *mi_conn, ST_CHAR *dv_name, 
				ST_INT dv_scope);


/************************************************************************/
/* Block 4 Support - Client						*/
/************************************************************************/

ST_RET mic_enable_IMTransfers (MIC_VCC *vcc);	/* add block 4 support to*/
						/* the client vcc	*/

MI_REQ_CTRL *mic_start_IMTransfer (MI_CONN *mi_conn);

MI_REQ_CTRL *mic_stop_IMTransfer (MI_CONN *mi_conn);

ST_VOID u_mic_receive_IMTransfer (MI_CONN *mi_conn, 
				  MI_INFO_BUF_HEADER *buf_header, 
			          ST_UCHAR *info_buf, ST_INT msg_scope);

/************************************************************************/
/* Block 5 Support - Client						*/
/************************************************************************/

MI_REQ_CTRL *mic_device_select  (MI_CONN *mi_conn, MIC_DEVICE *mic_device);
MI_REQ_CTRL *mic_device_operate (MI_CONN *mi_conn, MIC_DEVICE *mic_device, 
		   		 MI_DEV_VALUE_UNION *device_value);
MI_REQ_CTRL *mic_device_settag  (MI_CONN *mi_conn, MIC_DEVICE *mic_device,
		   		 MI_TAG_VALUE *tag_value);
MI_REQ_CTRL *mic_device_gettag  (MI_CONN *mi_conn, MIC_DEVICE *mic_device);

/* For responses to the above requests,					*/
/* the MI_REQ_CTRL result member will contain one of the following	*/
#define MIC_RESULT_SUCCESS		0
#define MIC_RESULT_OBJ_NON_EXISTENT	1
#define MIC_RESULT_ACCESS_DENIED	2
#define MIC_RESULT_HARDWARE_FAULT	3
#define MIC_RESULT_TEMP_UNAVAIL		4
#define MIC_RESULT_OTHER_MMS_ERR	5

/************************************************************************/
/************************************************************************/

ST_VOID mi_find_mvl_cfg_info (MVL_CFG_INFO *mvl_cfg_info);

/* Advanced: Use when it is necessary to add connection capacity to an	*/
/* existing local AR, or to add a new local AR. This can be used only 	*/
/* after MVL startup and requires unused connections to be requested	*/
/* during MVL startup. Typically used to add a remote while on-line	*/
ST_RET  mi_bind_remote (MI_REMOTE *mi_remote);
ST_VOID mi_free_remote_bind_info (MVL_CFG_INFO *mvl_cfg_info);
ST_VOID mi_get_remote_bind_info (MI_REMOTE *mi_remote, MVL_CFG_INFO *mvlCfgInfoOut);
ST_RET  mi_rebind_remote (MI_REMOTE *mi_remote, MVL_CFG_INFO *prevBindInfo);

/************************************************************************/
/* INTERNAL */
/************************************************************************/

extern ST_DOUBLE _mi_curr_ms_time;
extern time_t _mi_curr_sec_time;

typedef struct _mi_read_var_spec
  {
  ST_CHAR *name;
  ST_CHAR *domain;	
  ST_INT   scope;
  ST_INT   mi_type;
  ST_VOID *dest;
  ST_RET  result;
  } MI_READ_VAR_SPEC;

typedef struct _mi_read_req
  {
  ST_BOOLEAN nvl_flag;
  OBJECT_NAME nvl_name;	

  ST_INT num_var;
  MI_READ_VAR_SPEC *vs;
  ST_VOID (*u_req_done) (struct _mi_read_req *read_req);
  ST_RET result;
  ST_BOOLEAN done;
  
  MI_REQ_CTRL *mi_req;
  } MI_READ_REQ;

/************************************************************************/

typedef struct _mi_write_var_spec
  {
  ST_CHAR *name;
  ST_CHAR *domain;	
  ST_INT   scope;
  ST_INT   mi_type;
  ST_VOID *src;
  ST_RET  result;
  } MI_WRITE_VAR_SPEC;

typedef struct _mi_write_req
  {
  ST_BOOLEAN nvl_flag;
  OBJECT_NAME nvl_name;	

  ST_INT num_var;
  MI_WRITE_VAR_SPEC *vs;
  ST_VOID (*u_req_done) (struct _mi_write_req *write_req);
  ST_RET result;
  ST_BOOLEAN done;
  
  MI_REQ_CTRL *mi_req;
  } MI_WRITE_REQ;

ST_RET _mic_write (MI_CONN *mi_conn, MI_WRITE_REQ *mi_wr_req, 
	           MVL_REQ_PEND **mvl_req_out);

/************************************************************************/

/* Start ICCP client on a called or calling connection. This function	*/
/* reads and verifies the bilateral table and version variables.	*/
MI_REQ_CTRL *_mic_start (MI_CONN *mi_conn);
ST_RET _mis_start (MI_CONN *mi_conn);

ST_INT _mis_dsts_service (ST_DOUBLE *next_service);
ST_VOID _mi_assoc_service (ST_VOID);
ST_VOID _mis_reset_conn_dsts (MI_CONN *mi_conn);

extern ST_INT mi_type_to_type_id_tbl[MI_NUM_MI_TYPES];
extern ST_INT mi_block4_type_id_tbl[MI_NUM_BLOCK4_TYPES];
extern ST_INT mi_block5_type_id_tbl[MI_NUM_BLOCK5_TYPES];

MI_REQ_CTRL *_mi_get_req_ctrl (MI_CONN *mi_conn, ST_VOID *mvl_req);
ST_VOID _mi_req_done (MI_REQ_CTRL *mi_req);


ST_VOID _mi_set_bit (ST_UCHAR *bit_array, ST_INT bit_num);
ST_INT _mi_get_type_id (ST_INT mi_type);
ST_INT _mi_get_mi_type (ST_INT type_id);

ST_INT _mi_get_block4_type_id (ST_INT mi_type);
ST_INT _mi_get_block4_mi_type (ST_INT type_id);

ST_INT _mi_get_block5_type_id (ST_INT mi_type);
ST_INT _mi_get_block5_mi_type (ST_INT type_id);

ST_VOID _mi_set_named_assoc_state (ST_CHAR *remote_name, ST_CHAR *assoc_name, ST_BOOLEAN enable);
ST_VOID _mi_set_remote_assoc_state (MI_REMOTE *mi_remote, ST_BOOLEAN enable);

ST_INT _mi_num_active_assoc (MI_REMOTE *mi_remote);

ST_VOID mic_clean_assoc (MI_ASSOC_CTRL *mi_assoc);
ST_VOID mis_clean_assoc (MI_ASSOC_CTRL *mi_assoc);
ST_VOID _mi_set_next_rem_ar (MI_ASSOC_CTRL *mi_assoc);

ST_INT _mis_calc_info_rpt_size (MIS_INFO_RPT_CTRL *mis_info_ctrl);
MIS_INFO_RPT_CTRL *_mis_get_info_rpt_ctrl (MIS_VCC *mis_vcc, 
					   MIS_DSTS *mis_dsts);
ST_INT _mis_calc_info_msg_size (ST_INT buf_len, 
				ST_INT msg_scope,
				MI_CONN *mi_conn);

ST_INT _mic_find_avail_ds (MIC_VCC *mic_vcc, ST_INT ds_scope);

ST_VOID _mic_process_read_conf (MVL_REQ_PEND *mvl_req);
ST_VOID _mis_process_read_ind (MVL_IND_PEND *mvl_ind_ctrl);
ST_VOID _mis_process_write_ind (MVL_IND_PEND *mvl_ind_ctrl);
ST_VOID _mic_process_info_ind (MVL_COMM_EVENT *event);
ST_VOID _mis_process_info_ind (MVL_COMM_EVENT *event);
ST_VOID _mis_process_getnam_ind (MVL_IND_PEND *mvl_ind_ctrl);
ST_VOID _mis_process_getvaa_ind (MVL_IND_PEND *mvl_ind_ctrl);
ST_VOID _mis_process_getvla_ind (MVL_IND_PEND *mvl_ind_ctrl);
ST_VOID _mis_process_defvl_ind (MVL_IND_PEND *mvl_ind_ctrl);
ST_VOID _mis_process_delvlist_ind (MVL_IND_PEND *mvl_ind_ctrl);
ST_VOID _mis_send_read_resp (MIS_READ_CTRL *mis_rd_ctrl);
ST_VOID _mis_send_write_resp (MIS_WRITE_CTRL *mis_wr_ctrl);

/************************************************************************/
/* some block 4 support functions:					*/
/************************************************************************/

ST_VOID _mis_start_stop_im_transfer (MIS_IM_IND_CTRL *im_ind_ctrl,
				     VAR_ACC_DATA  *va_data);
ST_VOID _mic_process_info_message (MVL_COMM_EVENT *event);

/************************************************************************/
/* some block 5 support functions:					*/
/************************************************************************/

ST_VOID _mis_device_read_ind (MIS_DEVICE_IND_CTRL *mis_dev_ind);
ST_VOID _mis_device_write_ind (MIS_DEVICE_IND_CTRL *mis_dev_ind,
			       VAR_ACC_DATA  *va_data);
ST_RET _mi_get_device_asn1_data (ST_VOID *data, ST_INT type_id,
                                 ST_UCHAR *asn1_dest_buffer,
				 ST_INT asn1_buffer_len,
				 ST_INT *asn1_len_out);

/************************************************************************/
/* MMS Error Codes:							*/
/************************************************************************/
#define MMS_ECLASS_DEFINITION	2

#define MMS_ECODE_OBJ_UNDEFINED		1
#define MMS_ECODE_OBJ_EXISTS		5
#define MMS_ECODE_OBJ_ATTR_INCONSISTENT	6


#define MMS_ECLASS_RESOURCE	3

#define MMS_ECODE_MEM_UNAVAIL		1


#define MMS_ECLASS_ACCESS	7

#define MMS_ECODE_OBJ_ACCESS_UNSUPPORT  1
#define MMS_ECODE_OBJ_NONEXISTENT	2
#define MMS_ECODE_OBJ_ACCESS_DENIED	3

/************************************************************************/

ST_VOID mis_log_dv_tables (ST_VOID);
ST_VOID mis_log_dev_tables (ST_VOID);
ST_VOID mis_log_vcc_dv_tables (MIS_VCC *mis_vcc);
ST_VOID mis_log_vcc_device_tables (MIS_VCC *mis_vcc);
ST_VOID mic_log_vcc_dv_tables (MIC_VCC *mic_vcc);
ST_VOID mic_log_vcc_device_tables (MIC_VCC *mic_vcc);
ST_VOID mi_log_assoc (MI_ASSOC_CTRL *mi_assoc);

/************************************************************************/

typedef struct
  {
  ST_CHAR *name;
  ST_INT mi_type;
  ST_INT scope;
  } MIS_SPECIAL_VAR_ATTRIB;

extern MIS_SPECIAL_VAR_ATTRIB _mis_special_vars[];
extern ST_INT _mi_num_special_vars; 

/************************************************************************/
/************************************************************************/

typedef struct 
  {
  DBL_LNK  l;			
  ST_CHAR  name[MAX_IDENT_LEN+1];
  ST_INT   scope;
  ST_INT   mi_type;
  } MIC_GROBJ_DV;

typedef struct 
  {
  DBL_LNK  l;			
  ST_CHAR  name[MAX_IDENT_LEN+1];
  ST_INT   scope;
  } MIC_GROBJ_DS;

/* State defines, internal */
#define  _MIC_GROBJ_STATE_VCCDV		1
#define  _MIC_GROBJ_STATE_ICCDV		2
#define  _MIC_GROBJ_STATE_VCC_VA       	3
#define  _MIC_GROBJ_STATE_ICC_VA       	4
#define  _MIC_GROBJ_STATE_VCCDS		5
#define  _MIC_GROBJ_STATE_ICCDS		6
#define  _MIC_GROBJ_STATE_VCC_DSA	7
#define  _MIC_GROBJ_STATE_ICC_DSA	8

typedef struct _mic_grobj_ctrl
  {
  ST_INT vcc_dv_count;		/* Result: list of remote VCC DV's	*/
  MIC_GROBJ_DV *vcc_dv_list;	

  ST_INT icc_dv_count;		/* Result: list of remote ICC DV's	*/
  MIC_GROBJ_DV *icc_dv_list;	

  ST_INT vcc_ds_count;		/* Result: list of remote VCC DS's	*/
  MIC_GROBJ_DS *vcc_ds_list;	

  ST_INT icc_ds_count;		/* Result: list of remote ICC DS's	*/
  MIC_GROBJ_DS *icc_ds_list;	

/* Internal */
  ST_BOOLEAN get_attrib;
  ST_INT state;
  ST_INT mms_class;
  MIC_GROBJ_DV *curr_dv;
  MIC_GROBJ_DS *curr_ds;
  } MIC_GROBJ_CTRL;

MI_REQ_CTRL *mic_get_rem_dv (MI_CONN *mi_conn,  MIC_GROBJ_CTRL *mi_grobj_ctrl, 
 			     ST_BOOLEAN get_attrib);
ST_VOID mic_free_grobj_ctrl (MIC_GROBJ_CTRL *mi_grobj_ctrl);
ST_RET mic_check_dv_cfg (MI_REMOTE *mi_remote, MIC_GROBJ_CTRL *mi_grobj_ctrl);
ST_VOID mic_reset_all_dv_mismatch (MI_REMOTE *mi_remote);
ST_RET mic_encode_remote_xml (MI_ASSOC_CTRL *mi_assoc, MIC_GROBJ_CTRL *mi_grobj_ctrl, 
			      ST_CHAR *lccName, ST_CHAR *xmlFileName);

/* Internal */
ST_INT  _mi_asn1type_to_mi_type (ST_INT asn1_len, ST_UCHAR *asn1);
ST_VOID _mi_cmp_rt_types (RUNTIME_TYPE *rt1, ST_INT num_rt1, 
			  RUNTIME_TYPE *rt2, ST_INT num_rt2,
			  ST_BOOLEAN *matchOut,
			  ST_BOOLEAN *exactMatchOut);

MVL_REQ_PEND *_mic_send_namelist (MI_CONN *mi_conn, 
			       ST_INT mi_obj_class, ST_CHAR *ca_name);
ST_BOOLEAN _mi_is_special_var (ST_CHAR *name);
ST_CHAR *mi_type_to_string (ST_INT mi_type);

/************************************************************************/
/************************************************************************/
#if 0	/* Correct but obsolete defines, use the MI_QFLAG_XXX defines	*/
#define MI_DFM_VALIDITY_HI 		0x20
#define MI_DFM_VALIDITY_LO 		0x10
#define MI_DFM_CURRENT_SOURCE_HI 	0x08
#define MI_DFM_CURRENT_SOURCE_LO 	0x04
#define MI_DFM_NORMAL_VALUE		0x02
#define MI_DFM_TIME_STAMP_QUALITY	0x01

#define MI_DSM_STATE_HI 		0x80
#define MI_DSM_STATE_LO 		0x40
#define MI_DSM_VALIDITY_HI 		0x20
#define MI_DSM_VALIDITY_LO 		0x10
#define MI_DSM_CURRENT_SOURCE_HI 	0x08
#define MI_DSM_CURRENT_SOURCE_LO 	0x04
#define MI_DSM_NORMAL_VALUE		0x02
#define MI_DSM_TIME_STAMP_QUALITY	0x01
#endif

/************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* MVL_DEFS_INCLUDED */
/************************************************************************/

