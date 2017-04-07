
/************************************************************************/
/*	This file created from input file '\mmslite\iccp\mitest\miusrobj.odf'
	Created Mon Dec 09 15:26:40 2002
*/

#include "glbtypes.h"
#include "sysincs.h"
#include "mmsdefs.h"
#include "mms_pvar.h"
#include "mms_vvar.h"
#include "mvl_acse.h"
#include "miusrobj.h"


#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__; 
#endif


/************************************************************************/
MVL_TYPE_CTRL *mvl_type_ctrl;
ST_INT mvl_num_types;
MVL_VMD_CTRL mvl_vmd;

/************************************************************************/

/* Alignment Table:               */
/*  {                             */
/*  0x0000,    ARRSTRT_ALGN   00  */
/*  0x0000,    ARREND_ALGN    01  */
/*  0x0000,    STRSTRT_ALGN   02  */
/*  0x0000,    STREND_ALGN    03  */
/*  0x0000,    INT8_ALGN      04  */
/*  0x0001,    INT16_ALGN     05  */
/*  0x0003,    INT32_ALGN     06  */
/*  0x0007,    INT64_ALGN     07  */
/*  0x0003,    FLOAT_ALGN     08  */
/*  0x0007,    DOUBLE_ALGN    09  */
/*  0x0000,    OCT_ALGN       10  */
/*  0x0000,    BOOL_ALGN      11  */
/*  0x0000,    BCD1_ALGN      12  */
/*  0x0001,    BCD2_ALGN      13  */
/*  0x0003,    BCD4_ALGN      14  */
/*  0x0000,    BIT_ALGN       15  */
/*  0x0000     VIS_ALGN       16  */
/*  };                            */

/************************************************************************/


/*	Common Strings Table	*/
SD_CONST ST_INT numMvlRtNames = 29;
#ifdef USE_RT_TYPE_2
ST_CHAR mvlCompName_ [] = "";
ST_CHAR mvlCompName_Scope [] = "Scope";
ST_CHAR mvlCompName_DomainName [] = "DomainName";
ST_CHAR mvlCompName_Name [] = "Name";
ST_CHAR mvlCompName_GMTBasedS [] = "GMTBasedS";
ST_CHAR mvlCompName_Milliseconds [] = "Milliseconds";
ST_CHAR mvlCompName_DataSetName [] = "DataSetName";
ST_CHAR mvlCompName_StartTime [] = "StartTime";
ST_CHAR mvlCompName_Interval [] = "Interval";
ST_CHAR mvlCompName_TLE [] = "TLE";
ST_CHAR mvlCompName_BufferTime [] = "BufferTime";
ST_CHAR mvlCompName_IntegrityCheck [] = "IntegrityCheck";
ST_CHAR mvlCompName_DSConditionsRequested [] = "DSConditionsRequested";
ST_CHAR mvlCompName_BlockData [] = "BlockData";
ST_CHAR mvlCompName_Critical [] = "Critical";
ST_CHAR mvlCompName_RBE [] = "RBE";
ST_CHAR mvlCompName_Status [] = "Status";
ST_CHAR mvlCompName_EventCodeRequested [] = "EventCodeRequested";
ST_CHAR mvlCompName_MajorVersionNumber [] = "MajorVersionNumber";
ST_CHAR mvlCompName_MinorVersionNumber [] = "MinorVersionNumber";
ST_CHAR mvlCompName_Value [] = "Value";
ST_CHAR mvlCompName_Flags [] = "Flags";
ST_CHAR mvlCompName_TimeStamp [] = "TimeStamp";
ST_CHAR mvlCompName_COV [] = "COV";
ST_CHAR mvlCompName_Reason [] = "Reason";
ST_CHAR mvlCompName_InfoReference [] = "InfoReference";
ST_CHAR mvlCompName_LocalReference [] = "LocalReference";
ST_CHAR mvlCompName_MessageId [] = "MessageId";
ST_CHAR mvlCompName_Size [] = "Size";
#else	/* !USE_RT_TYPE_2	*/
SD_CONST ST_CHAR *SD_CONST mvlRtNames[] =
  {
  "",
  "Scope",
  "DomainName",
  "Name",
  "GMTBasedS",
  "Milliseconds",
  "DataSetName",
  "StartTime",
  "Interval",
  "TLE",
  "BufferTime",
  "IntegrityCheck",
  "DSConditionsRequested",
  "BlockData",
  "Critical",
  "RBE",
  "Status",
  "EventCodeRequested",
  "MajorVersionNumber",
  "MinorVersionNumber",
  "Value",
  "Flags",
  "TimeStamp",
  "COV",
  "Reason",
  "InfoReference",
  "LocalReference",
  "MessageId",
  "Size"
  };

#endif	/* !USE_RT_TYPE_2	*/

/************************************************************************/

#ifdef USE_MVL_TYPEID_STRINGS

/* MVL Type ID String to Type ID Table */
MVL_TYPE_ID_TO_STRING mvlTypeIdToString[] =
  {
    {"MMSIdentifier_TYPEID", 	0},
    {"Boolean_TYPEID", 	1},
    {"Unsigned8_TYPEID", 	2},
    {"Unsigned16_TYPEID", 	3},
    {"Integer32_TYPEID", 	4},
    {"Integer16_TYPEID", 	5},
    {"TimeInterval16_TYPEID", 	5}, /* Integer16 */
    {"Visible_String_32_TYPEID", 	0}, /* MMSIdentifier */
    {"MMSObjectName_TYPEID", 	6},
    {"GMTBasedS_TYPEID", 	4}, /* Integer32 */
    {"GMTBasedM_TYPEID", 	4}, /* Integer32 */
    {"GMTBasedH_TYPEID", 	4}, /* Integer32 */
    {"GMTBasedD_TYPEID", 	4}, /* Integer32 */
    {"TimeStampExtended_TYPEID", 	7},
    {"TimeIntervalD_TYPEID", 	5}, /* Integer16 */
    {"TimeIntervalH_TYPEID", 	5}, /* Integer16 */
    {"TimeIntervalM_TYPEID", 	5}, /* Integer16 */
    {"TimeIntervalS_TYPEID", 	5}, /* Integer16 */
    {"TimeIntervalL16_TYPEID", 	5}, /* Integer16 */
    {"TimeIntervalL32_TYPEID", 	4}, /* Integer32 */
    {"DSConditions_TYPEID", 	8},
    {"DSTransferSet_TYPEID", 	9},
    {"IMTransferSet_TYPEID", 	1}, /* Boolean */
    {"SupportedFeatures_TYPEID", 	10},
    {"TASE2Version_TYPEID", 	11},
    {"Data_Real_TYPEID", 	12},
    {"Data_State_TYPEID", 	13},
    {"Data_Discrete_TYPEID", 	14},
    {"Data_Flags_TYPEID", 	15},
    {"Data_TimeStamp_TYPEID", 	4}, /* Integer32 */
    {"COV_Counter_TYPEID", 	3}, /* Unsigned16 */
    {"Data_RealQ_TYPEID", 	16},
    {"Data_StateQ_TYPEID", 	17},
    {"Data_DiscreteQ_TYPEID", 	18},
    {"Data_RealQTimeTag_TYPEID", 	19},
    {"Data_StateQTimeTag_TYPEID", 	20},
    {"Data_DiscreteQTimeTag_TYPEID", 	21},
    {"Data_RealExtended_TYPEID", 	22},
    {"Data_StateExtended_TYPEID", 	23},
    {"Data_DiscreteExtended_TYPEID", 	24},
    {"Control_Command_TYPEID", 	25},
    {"Control_Setpoint_Real_TYPEID", 	26},
    {"Control_Setpoint_Discrete_TYPEID", 	27},
    {"SBO_CheckBackName_TYPEID", 	28},
    {"TextString_TYPEID", 	29},
    {"TagFlags_TYPEID", 	30},
    {"TagValue_TYPEID", 	31},
    {"Scope_TYPEID", 	5}, /* Integer16 */
    {"User_Data_TYPEID", 	32},
    {"ReferenceNum_TYPEID", 	4}, /* Integer32 */
    {"Number_TYPEID", 	4}, /* Integer32 */
    {"InfoMessHeader_TYPEID", 	33},
    {"InfoBuffXXX_TYPEID", 	34},
    { NULL, -1}
  };


ST_CHAR *mvl_type_id_to_string (ST_INT type_id)		
  {								
ST_INT i;							
								
  for (i = 0; mvlTypeIdToString[i].name != NULL; ++i)		
    {								
    if (mvlTypeIdToString[i].type_id == type_id)		
      return (mvlTypeIdToString[i].name);			
    }								
  return ("unknown");					
  } 								

ST_INT mvl_string_to_type_id (ST_CHAR *type_id_string)	  
  {								  
ST_INT i;							  
								  
  for (i = 0; mvlTypeIdToString[i].name != NULL; ++i)		  
    {								  
    if (strcmp (mvlTypeIdToString[i].name, type_id_string) == 0)
      return (mvlTypeIdToString[i].type_id);			  
    }								  
  return (-1);						  
  } 								  
#endif

/************************************************************************/

/* MMS OBJECT INITIALIZATION */

ST_VOID mvl_init_type_ctrl (ST_VOID);
static ST_VOID mvl_init_vmd_vars (ST_VOID);
static ST_VOID mvl_init_dom_vars (ST_VOID);
static ST_VOID mvl_init_aa_vars (ST_VOID);
static ST_VOID mvl_init_vmd_varLists (ST_VOID);
static ST_VOID mvl_init_dom_varLists (ST_VOID);
static ST_VOID mvl_init_aa_varLists (ST_VOID);
static ST_VOID mvl_init_journals (ST_VOID);

ST_VOID mvl_init_mms_objs ()
  {
ST_INT i;
static ST_BOOLEAN _mvlInitMmsObjsCalled = SD_FALSE;

/* Make sure called ONLY once (from mvl_start_acse)	*/
  assert (_mvlInitMmsObjsCalled == SD_FALSE);
  _mvlInitMmsObjsCalled = SD_TRUE;

/* Make sure conn_ctrl allocated (by mvl_start_acse)	*/
  assert (mvl_calling_conn_ctrl || mvl_called_conn_ctrl);

/* Set up the AA Control structures */
  for (i = 0; i < mvl_cfg_info->num_called; ++i)
    mvl_called_conn_ctrl[i].aa_objs = (MVL_AA_OBJ_CTRL *) M_CALLOC (MSMEM_STARTUP, 1, sizeof(MVL_AA_OBJ_CTRL));
  for (i = 0; i < mvl_cfg_info->num_calling; ++i)
    mvl_calling_conn_ctrl[i].aa_objs = (MVL_AA_OBJ_CTRL *) M_CALLOC (MSMEM_STARTUP, 1, sizeof(MVL_AA_OBJ_CTRL));

  mvl_init_type_ctrl ();
  mvl_init_vmd_vars ();
  mvl_init_dom_vars ();
  mvl_init_aa_vars ();
  mvl_init_vmd_varLists ();
  mvl_init_dom_varLists ();
  mvl_init_aa_varLists ();
  mvl_init_journals ();
  }
/************************************************************************/

/* VMD WIDE NAMED VARIABLE ASSOCIATION INITIALIZATION */

static ST_VOID mvl_init_vmd_vars ()
  {
MVL_VAR_ASSOC **ppva;

  mvl_vmd.max_num_var_assoc = 0 + MVL_NUM_DYN_VMD_VARS;
  mvl_vmd.num_var_assoc = 0;
  if (mvl_vmd.max_num_var_assoc != 0)
    mvl_vmd.var_assoc_tbl = ppva = (MVL_VAR_ASSOC **) M_CALLOC (MSMEM_STARTUP, 0 + MVL_NUM_DYN_VMD_VARS, sizeof (MVL_VAR_ASSOC *));
  }
/************************************************************************/

/* DOMAIN VARIABLE INITIALIZATION */
static ST_VOID mvl_init_dom_vars ()
  {
  mvl_vmd.num_dom = 0;
  }

/************************************************************************/

/* AA VARIABLE INITIALIZATION */
static ST_VOID mvl_init_aa_vars ()
  {
MVL_AA_OBJ_CTRL *aa;
MVL_VAR_ASSOC **ppva;
ST_INT i;
ST_INT j;

/* Do AA specific variables */
  i = 0;
  for (j = 0; j < mvl_cfg_info->num_called; ++j, ++i)
    {
    aa = (MVL_AA_OBJ_CTRL *) mvl_called_conn_ctrl[j].aa_objs;
    aa->foundry_objects = SD_TRUE;
    aa->max_num_var_assoc = 0 + MVL_NUM_DYN_AA_VARS;
    aa->num_var_assoc = 0;
#if (0 + MVL_NUM_DYN_AA_VARS)
    aa->var_assoc_tbl = ppva = (MVL_VAR_ASSOC **) M_CALLOC (MSMEM_STARTUP, 0 + MVL_NUM_DYN_AA_VARS, sizeof (MVL_VAR_ASSOC *));
#endif
    }
  for (j = 0; j < mvl_cfg_info->num_calling; ++j, ++i)
    {
    aa = (MVL_AA_OBJ_CTRL *) mvl_calling_conn_ctrl[j].aa_objs;
    aa->foundry_objects = SD_TRUE;
    aa->max_num_var_assoc = 0 + MVL_NUM_DYN_AA_VARS;
    aa->num_var_assoc = 0;
#if (0 + MVL_NUM_DYN_AA_VARS)
    aa->var_assoc_tbl = ppva = (MVL_VAR_ASSOC **) M_CALLOC (MSMEM_STARTUP, 0 + MVL_NUM_DYN_AA_VARS, sizeof (MVL_VAR_ASSOC *));
#endif
    }
  }

/************************************************************************/

/* NAMED VARIABLE LIST INITIALIZATION */

static ST_VOID mvl_init_vmd_varLists ()
  {
MVL_NVLIST_CTRL **ppvl;

/* Do VMD wide variable lists */
  mvl_vmd.max_num_nvlist = 0 + MVL_NUM_DYN_VMD_NVLS;
  mvl_vmd.num_nvlist = 0;
  if (mvl_vmd.max_num_nvlist != 0)
    mvl_vmd.nvlist_tbl = ppvl = (MVL_NVLIST_CTRL **) M_CALLOC (MSMEM_STARTUP, 0 + MVL_NUM_DYN_VMD_NVLS, sizeof (MVL_NVLIST_CTRL *));
  }
/************************************************************************/

/* DOMAIN VARIABLE LIST INITIALIZATION */
static ST_VOID mvl_init_dom_varLists ()
  {
  }

/************************************************************************/

/* AA VARIABLE LIST INITIALIZATION */
static ST_VOID mvl_init_aa_varLists ()
  {
MVL_AA_OBJ_CTRL *aa;
MVL_NVLIST_CTRL **ppvl;
ST_INT i;


/* Now do AA specific Variable Lists */
  for (i = 0; i < mvl_cfg_info->num_called; ++i)
    {
    aa = (MVL_AA_OBJ_CTRL *) mvl_called_conn_ctrl[i].aa_objs;
    aa->foundry_objects = SD_TRUE;
    aa->max_num_nvlist = 0 + MVL_NUM_DYN_AA_NVLS;
    aa->num_nvlist = 0;
#if (0 + MVL_NUM_DYN_AA_NVLS)
    aa->nvlist_tbl = ppvl = (MVL_NVLIST_CTRL **) M_CALLOC (MSMEM_STARTUP, 0 + MVL_NUM_DYN_AA_NVLS, sizeof (MVL_NVLIST_CTRL *));
#endif
    }
  for (i = 0; i < mvl_cfg_info->num_calling; ++i)
    {
    aa = (MVL_AA_OBJ_CTRL *) mvl_calling_conn_ctrl[i].aa_objs;
    aa->foundry_objects = SD_TRUE;
    aa->max_num_nvlist = 0 + MVL_NUM_DYN_AA_NVLS;
    aa->num_nvlist = 0;
#if (0 + MVL_NUM_DYN_AA_NVLS)
    aa->nvlist_tbl = ppvl = (MVL_NVLIST_CTRL **) M_CALLOC (MSMEM_STARTUP, 0 + MVL_NUM_DYN_AA_NVLS, sizeof (MVL_NVLIST_CTRL *));
#endif
    }
  }

/************************************************************************/

/* JOURNAL INITIALIZATION */
static ST_VOID mvl_init_journals ()
  {
MVL_JOURNAL_CTRL **ppjou;

  /* initialize VMD_SPEC journals */

  mvl_vmd.max_num_jou = 0 + MVL_NUM_DYN_JOUS;
  mvl_vmd.num_jou = 0;
  if (mvl_vmd.max_num_jou != 0)
    mvl_vmd.jou_tbl = ppjou = (MVL_JOURNAL_CTRL **) M_CALLOC (MSMEM_STARTUP, 0 + MVL_NUM_DYN_JOUS, sizeof (MVL_JOURNAL_CTRL *));
  }

/************************************************************************/

ST_VOID mvl_init_type_ctrl ()
  {
static ST_BOOLEAN _mvlInitTypeCtrlCalled = SD_FALSE;

  if (_mvlInitTypeCtrlCalled == SD_TRUE)
    return;
  _mvlInitTypeCtrlCalled = SD_TRUE;

  if (_mvlu_num_dyn_types == 0)
    _mvlu_num_dyn_types = MVLU_NUM_DYN_TYPES;

  mvl_num_types = 35 + _mvlu_num_dyn_types;
  mvl_type_ctrl = (MVL_TYPE_CTRL *) M_CALLOC (MSMEM_STARTUP, 35 + _mvlu_num_dyn_types, sizeof(MVL_TYPE_CTRL));
#if defined USR_SUPPLIED_RT
  u_mvl_start_init_rt_tbl (35, 104);
#endif

/* MMSIdentifier :  */
/*
  mvl_type_ctrl[MMSIdentifier_TYPEID].tdl = 
  "Vstring32";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[MMSIdentifier_TYPEID].asn1_len = 3;
  mvl_type_ctrl[MMSIdentifier_TYPEID].asn1 = "\x8a\x01\xe0";
#endif
  mvl_type_ctrl[MMSIdentifier_TYPEID].num_rt = 1;
  mvl_type_ctrl[MMSIdentifier_TYPEID].data_size = 33;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[MMSIdentifier_TYPEID].rt = u_mvl_get_rt_tbl (MMSIdentifier_TYPEID, 1);
#else
  mvl_type_ctrl[MMSIdentifier_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Boolean :  */
/*
  mvl_type_ctrl[Boolean_TYPEID].tdl = 
  "Bool";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Boolean_TYPEID].asn1_len = 2;
  mvl_type_ctrl[Boolean_TYPEID].asn1 = "\x83\x00";
#endif
  mvl_type_ctrl[Boolean_TYPEID].num_rt = 1;
  mvl_type_ctrl[Boolean_TYPEID].data_size = 1;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Boolean_TYPEID].rt = u_mvl_get_rt_tbl (Boolean_TYPEID, 1);
#else
  mvl_type_ctrl[Boolean_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Unsigned8 :  */
/*
  mvl_type_ctrl[Unsigned8_TYPEID].tdl = 
  "Ubyte";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Unsigned8_TYPEID].asn1_len = 3;
  mvl_type_ctrl[Unsigned8_TYPEID].asn1 = "\x86\x01\x08";
#endif
  mvl_type_ctrl[Unsigned8_TYPEID].num_rt = 1;
  mvl_type_ctrl[Unsigned8_TYPEID].data_size = 1;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Unsigned8_TYPEID].rt = u_mvl_get_rt_tbl (Unsigned8_TYPEID, 1);
#else
  mvl_type_ctrl[Unsigned8_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Unsigned16 :  */
/*
  mvl_type_ctrl[Unsigned16_TYPEID].tdl = 
  "Ushort";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Unsigned16_TYPEID].asn1_len = 3;
  mvl_type_ctrl[Unsigned16_TYPEID].asn1 = "\x86\x01\x10";
#endif
  mvl_type_ctrl[Unsigned16_TYPEID].num_rt = 1;
  mvl_type_ctrl[Unsigned16_TYPEID].data_size = 2;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Unsigned16_TYPEID].rt = u_mvl_get_rt_tbl (Unsigned16_TYPEID, 1);
#else
  mvl_type_ctrl[Unsigned16_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Integer32 :  */
/*
  mvl_type_ctrl[Integer32_TYPEID].tdl = 
  "Long";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Integer32_TYPEID].asn1_len = 3;
  mvl_type_ctrl[Integer32_TYPEID].asn1 = "\x85\x01\x20";
#endif
  mvl_type_ctrl[Integer32_TYPEID].num_rt = 1;
  mvl_type_ctrl[Integer32_TYPEID].data_size = 4;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Integer32_TYPEID].rt = u_mvl_get_rt_tbl (Integer32_TYPEID, 1);
#else
  mvl_type_ctrl[Integer32_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Integer16 :  */
/*
  mvl_type_ctrl[Integer16_TYPEID].tdl = 
  "Short";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Integer16_TYPEID].asn1_len = 3;
  mvl_type_ctrl[Integer16_TYPEID].asn1 = "\x85\x01\x10";
#endif
  mvl_type_ctrl[Integer16_TYPEID].num_rt = 1;
  mvl_type_ctrl[Integer16_TYPEID].data_size = 2;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Integer16_TYPEID].rt = u_mvl_get_rt_tbl (Integer16_TYPEID, 1);
#else
  mvl_type_ctrl[Integer16_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* TimeInterval16 :  */
/* Integer16 is our twin */

/* Visible_String_32 :  */
/* MMSIdentifier is our twin */

/* MMSObjectName :  */
/*
  mvl_type_ctrl[MMSObjectName_TYPEID].tdl = 
  "{(Scope)<Unsigned8>,(DomainName)<MMSIdentifier>,(Name)<MMSIdentifier>}";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[MMSObjectName_TYPEID].asn1_len = 50;
  mvl_type_ctrl[MMSObjectName_TYPEID].asn1 = "\xa2\x30\xa1\x2e\x30\x0c\x80\x05\x53\x63\x6f\
\x70\x65\xa1\x03\x86\x01\x08\x30\x11\x80\x0a\x44\x6f\x6d\x61\x69\x6e\x4e\
\x61\x6d\x65\xa1\x03\x8a\x01\xe0\x30\x0b\x80\x04\x4e\x61\x6d\x65\xa1\x03\
\x8a\x01\xe0";
#endif
  mvl_type_ctrl[MMSObjectName_TYPEID].num_rt = 5;
  mvl_type_ctrl[MMSObjectName_TYPEID].data_size = 67;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[MMSObjectName_TYPEID].rt = u_mvl_get_rt_tbl (MMSObjectName_TYPEID, 5);
#else
  mvl_type_ctrl[MMSObjectName_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* GMTBasedS :  */
/* Integer32 is our twin */

/* GMTBasedM :  */
/* Integer32 is our twin */

/* GMTBasedH :  */
/* Integer32 is our twin */

/* GMTBasedD :  */
/* Integer32 is our twin */

/* TimeStampExtended :  */
/*
  mvl_type_ctrl[TimeStampExtended_TYPEID].tdl = 
  "{(GMTBasedS)<GMTBasedS>,(Milliseconds)<TimeInterval16>}";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[TimeStampExtended_TYPEID].asn1_len = 43;
  mvl_type_ctrl[TimeStampExtended_TYPEID].asn1 = "\xa2\x29\xa1\x27\x30\x10\x80\x09\x47\x4d\x54\
\x42\x61\x73\x65\x64\x53\xa1\x03\x85\x01\x20\x30\x13\x80\x0c\x4d\x69\x6c\
\x6c\x69\x73\x65\x63\x6f\x6e\x64\x73\xa1\x03\x85\x01\x10";
#endif
  mvl_type_ctrl[TimeStampExtended_TYPEID].num_rt = 4;
  mvl_type_ctrl[TimeStampExtended_TYPEID].data_size = 8;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[TimeStampExtended_TYPEID].rt = u_mvl_get_rt_tbl (TimeStampExtended_TYPEID, 4);
#else
  mvl_type_ctrl[TimeStampExtended_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* TimeIntervalD :  */
/* Integer16 is our twin */

/* TimeIntervalH :  */
/* Integer16 is our twin */

/* TimeIntervalM :  */
/* Integer16 is our twin */

/* TimeIntervalS :  */
/* Integer16 is our twin */

/* TimeIntervalL16 :  */
/* Integer16 is our twin */

/* TimeIntervalL32 :  */
/* Integer32 is our twin */

/* DSConditions :  */
/*
  mvl_type_ctrl[DSConditions_TYPEID].tdl = 
  "Bstring5";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[DSConditions_TYPEID].asn1_len = 3;
  mvl_type_ctrl[DSConditions_TYPEID].asn1 = "\x84\x01\x05";
#endif
  mvl_type_ctrl[DSConditions_TYPEID].num_rt = 1;
  mvl_type_ctrl[DSConditions_TYPEID].data_size = 1;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[DSConditions_TYPEID].rt = u_mvl_get_rt_tbl (DSConditions_TYPEID, 1);
#else
  mvl_type_ctrl[DSConditions_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* DSTransferSet :  */
/*
  mvl_type_ctrl[DSTransferSet_TYPEID].tdl = 
  "{(DataSetName)<MMSObjectName>,(StartTime)<GMTBasedS>,(Interval)<TimeInte\
rvalS>,(TLE)<TimeIntervalS>,(BufferTime)<TimeIntervalS>,(IntegrityCheck)\
<TimeIntervalS>,(DSConditionsRequested)<DSConditions>,(BlockData)<Boolea\
n>,(Critical)<Boolean>,(RBE)<Boolean>,(Status)<Boolean>,(EventCodeReques\
ted)<Integer16>}";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[DSTransferSet_TYPEID].asn1_len = 279;
  mvl_type_ctrl[DSTransferSet_TYPEID].asn1 = "\xa2\x82\x01\x13\xa1\x82\x01\x0f\x30\x41\x80\
\x0b\x44\x61\x74\x61\x53\x65\x74\x4e\x61\x6d\x65\xa1\x32\xa2\x30\xa1\x2e\
\x30\x0c\x80\x05\x53\x63\x6f\x70\x65\xa1\x03\x86\x01\x08\x30\x11\x80\x0a\
\x44\x6f\x6d\x61\x69\x6e\x4e\x61\x6d\x65\xa1\x03\x8a\x01\xe0\x30\x0b\x80\
\x04\x4e\x61\x6d\x65\xa1\x03\x8a\x01\xe0\x30\x10\x80\x09\x53\x74\x61\x72\
\x74\x54\x69\x6d\x65\xa1\x03\x85\x01\x20\x30\x0f\x80\x08\x49\x6e\x74\x65\
\x72\x76\x61\x6c\xa1\x03\x85\x01\x10\x30\x0a\x80\x03\x54\x4c\x45\xa1\x03\
\x85\x01\x10\x30\x11\x80\x0a\x42\x75\x66\x66\x65\x72\x54\x69\x6d\x65\xa1\
\x03\x85\x01\x10\x30\x15\x80\x0e\x49\x6e\x74\x65\x67\x72\x69\x74\x79\x43\
\x68\x65\x63\x6b\xa1\x03\x85\x01\x10\x30\x1c\x80\x15\x44\x53\x43\x6f\x6e\
\x64\x69\x74\x69\x6f\x6e\x73\x52\x65\x71\x75\x65\x73\x74\x65\x64\xa1\x03\
\x84\x01\x05\x30\x0f\x80\x09\x42\x6c\x6f\x63\x6b\x44\x61\x74\x61\xa1\x02\
\x83\x00\x30\x0e\x80\x08\x43\x72\x69\x74\x69\x63\x61\x6c\xa1\x02\x83\x00\
\x30\x09\x80\x03\x52\x42\x45\xa1\x02\x83\x00\x30\x0c\x80\x06\x53\x74\x61\
\x74\x75\x73\xa1\x02\x83\x00\x30\x19\x80\x12\x45\x76\x65\x6e\x74\x43\x6f\
\x64\x65\x52\x65\x71\x75\x65\x73\x74\x65\x64\xa1\x03\x85\x01\x10";
#endif
  mvl_type_ctrl[DSTransferSet_TYPEID].num_rt = 18;
  mvl_type_ctrl[DSTransferSet_TYPEID].data_size = 88;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[DSTransferSet_TYPEID].rt = u_mvl_get_rt_tbl (DSTransferSet_TYPEID, 18);
#else
  mvl_type_ctrl[DSTransferSet_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* IMTransferSet :  */
/* Boolean is our twin */

/* SupportedFeatures :  */
/*
  mvl_type_ctrl[SupportedFeatures_TYPEID].tdl = 
  "Bstring12";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[SupportedFeatures_TYPEID].asn1_len = 3;
  mvl_type_ctrl[SupportedFeatures_TYPEID].asn1 = "\x84\x01\x0c";
#endif
  mvl_type_ctrl[SupportedFeatures_TYPEID].num_rt = 1;
  mvl_type_ctrl[SupportedFeatures_TYPEID].data_size = 2;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[SupportedFeatures_TYPEID].rt = u_mvl_get_rt_tbl (SupportedFeatures_TYPEID, 1);
#else
  mvl_type_ctrl[SupportedFeatures_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* TASE2Version :  */
/*
  mvl_type_ctrl[TASE2Version_TYPEID].tdl = 
  "{(MajorVersionNumber)<Integer16>,(MinorVersionNumber)<Integer16>}";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[TASE2Version_TYPEID].asn1_len = 58;
  mvl_type_ctrl[TASE2Version_TYPEID].asn1 = "\xa2\x38\xa1\x36\x30\x19\x80\x12\x4d\x61\x6a\
\x6f\x72\x56\x65\x72\x73\x69\x6f\x6e\x4e\x75\x6d\x62\x65\x72\xa1\x03\x85\
\x01\x10\x30\x19\x80\x12\x4d\x69\x6e\x6f\x72\x56\x65\x72\x73\x69\x6f\x6e\
\x4e\x75\x6d\x62\x65\x72\xa1\x03\x85\x01\x10";
#endif
  mvl_type_ctrl[TASE2Version_TYPEID].num_rt = 4;
  mvl_type_ctrl[TASE2Version_TYPEID].data_size = 4;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[TASE2Version_TYPEID].rt = u_mvl_get_rt_tbl (TASE2Version_TYPEID, 4);
#else
  mvl_type_ctrl[TASE2Version_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Data_Real :  */
/*
  mvl_type_ctrl[Data_Real_TYPEID].tdl = 
  "Float";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Data_Real_TYPEID].asn1_len = 8;
  mvl_type_ctrl[Data_Real_TYPEID].asn1 = "\xa7\x06\x02\x01\x20\x02\x01\x08";
#endif
  mvl_type_ctrl[Data_Real_TYPEID].num_rt = 1;
  mvl_type_ctrl[Data_Real_TYPEID].data_size = 4;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Data_Real_TYPEID].rt = u_mvl_get_rt_tbl (Data_Real_TYPEID, 1);
#else
  mvl_type_ctrl[Data_Real_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Data_State :  */
/*
  mvl_type_ctrl[Data_State_TYPEID].tdl = 
  "Bstring8";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Data_State_TYPEID].asn1_len = 3;
  mvl_type_ctrl[Data_State_TYPEID].asn1 = "\x84\x01\x08";
#endif
  mvl_type_ctrl[Data_State_TYPEID].num_rt = 1;
  mvl_type_ctrl[Data_State_TYPEID].data_size = 1;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Data_State_TYPEID].rt = u_mvl_get_rt_tbl (Data_State_TYPEID, 1);
#else
  mvl_type_ctrl[Data_State_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Data_Discrete :  */
/*
  mvl_type_ctrl[Data_Discrete_TYPEID].tdl = 
  "<Integer32>";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Data_Discrete_TYPEID].asn1_len = 3;
  mvl_type_ctrl[Data_Discrete_TYPEID].asn1 = "\x85\x01\x20";
#endif
  mvl_type_ctrl[Data_Discrete_TYPEID].num_rt = 1;
  mvl_type_ctrl[Data_Discrete_TYPEID].data_size = 4;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Data_Discrete_TYPEID].rt = u_mvl_get_rt_tbl (Data_Discrete_TYPEID, 1);
#else
  mvl_type_ctrl[Data_Discrete_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Data_Flags :  */
/*
  mvl_type_ctrl[Data_Flags_TYPEID].tdl = 
  "Bstring8";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Data_Flags_TYPEID].asn1_len = 3;
  mvl_type_ctrl[Data_Flags_TYPEID].asn1 = "\x84\x01\x08";
#endif
  mvl_type_ctrl[Data_Flags_TYPEID].num_rt = 1;
  mvl_type_ctrl[Data_Flags_TYPEID].data_size = 1;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Data_Flags_TYPEID].rt = u_mvl_get_rt_tbl (Data_Flags_TYPEID, 1);
#else
  mvl_type_ctrl[Data_Flags_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Data_TimeStamp :  */
/* Integer32 is our twin */

/* COV_Counter :  */
/* Unsigned16 is our twin */

/* Data_RealQ :  */
/*
  mvl_type_ctrl[Data_RealQ_TYPEID].tdl = 
  "{(Value)<Data_Real>,(Flags)<Data_Flags>}";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Data_RealQ_TYPEID].asn1_len = 37;
  mvl_type_ctrl[Data_RealQ_TYPEID].asn1 = "\xa2\x23\xa1\x21\x30\x11\x80\x05\x56\x61\x6c\
\x75\x65\xa1\x08\xa7\x06\x02\x01\x20\x02\x01\x08\x30\x0c\x80\x05\x46\x6c\
\x61\x67\x73\xa1\x03\x84\x01\x08";
#endif
  mvl_type_ctrl[Data_RealQ_TYPEID].num_rt = 4;
  mvl_type_ctrl[Data_RealQ_TYPEID].data_size = 8;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Data_RealQ_TYPEID].rt = u_mvl_get_rt_tbl (Data_RealQ_TYPEID, 4);
#else
  mvl_type_ctrl[Data_RealQ_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Data_StateQ :  */
/*
  mvl_type_ctrl[Data_StateQ_TYPEID].tdl = 
  "<Data_State>";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Data_StateQ_TYPEID].asn1_len = 3;
  mvl_type_ctrl[Data_StateQ_TYPEID].asn1 = "\x84\x01\x08";
#endif
  mvl_type_ctrl[Data_StateQ_TYPEID].num_rt = 1;
  mvl_type_ctrl[Data_StateQ_TYPEID].data_size = 1;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Data_StateQ_TYPEID].rt = u_mvl_get_rt_tbl (Data_StateQ_TYPEID, 1);
#else
  mvl_type_ctrl[Data_StateQ_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Data_DiscreteQ :  */
/*
  mvl_type_ctrl[Data_DiscreteQ_TYPEID].tdl = 
  "{(Value)<Data_Discrete>,(Flags)<Data_Flags>}";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Data_DiscreteQ_TYPEID].asn1_len = 32;
  mvl_type_ctrl[Data_DiscreteQ_TYPEID].asn1 = "\xa2\x1e\xa1\x1c\x30\x0c\x80\x05\x56\x61\x6c\
\x75\x65\xa1\x03\x85\x01\x20\x30\x0c\x80\x05\x46\x6c\x61\x67\x73\xa1\x03\
\x84\x01\x08";
#endif
  mvl_type_ctrl[Data_DiscreteQ_TYPEID].num_rt = 4;
  mvl_type_ctrl[Data_DiscreteQ_TYPEID].data_size = 8;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Data_DiscreteQ_TYPEID].rt = u_mvl_get_rt_tbl (Data_DiscreteQ_TYPEID, 4);
#else
  mvl_type_ctrl[Data_DiscreteQ_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Data_RealQTimeTag :  */
/*
  mvl_type_ctrl[Data_RealQTimeTag_TYPEID].tdl = 
  "{(Value)<Data_Real>,(TimeStamp)<Data_TimeStamp>,(Flags)<Data_Flags>}";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Data_RealQTimeTag_TYPEID].asn1_len = 55;
  mvl_type_ctrl[Data_RealQTimeTag_TYPEID].asn1 = "\xa2\x35\xa1\x33\x30\x11\x80\x05\x56\x61\x6c\
\x75\x65\xa1\x08\xa7\x06\x02\x01\x20\x02\x01\x08\x30\x10\x80\x09\x54\x69\
\x6d\x65\x53\x74\x61\x6d\x70\xa1\x03\x85\x01\x20\x30\x0c\x80\x05\x46\x6c\
\x61\x67\x73\xa1\x03\x84\x01\x08";
#endif
  mvl_type_ctrl[Data_RealQTimeTag_TYPEID].num_rt = 5;
  mvl_type_ctrl[Data_RealQTimeTag_TYPEID].data_size = 12;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Data_RealQTimeTag_TYPEID].rt = u_mvl_get_rt_tbl (Data_RealQTimeTag_TYPEID, 5);
#else
  mvl_type_ctrl[Data_RealQTimeTag_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Data_StateQTimeTag :  */
/*
  mvl_type_ctrl[Data_StateQTimeTag_TYPEID].tdl = 
  "{(TimeStamp)<Data_TimeStamp>,(Flags)<Data_Flags>}";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Data_StateQTimeTag_TYPEID].asn1_len = 36;
  mvl_type_ctrl[Data_StateQTimeTag_TYPEID].asn1 = "\xa2\x22\xa1\x20\x30\x10\x80\x09\x54\x69\x6d\
\x65\x53\x74\x61\x6d\x70\xa1\x03\x85\x01\x20\x30\x0c\x80\x05\x46\x6c\x61\
\x67\x73\xa1\x03\x84\x01\x08";
#endif
  mvl_type_ctrl[Data_StateQTimeTag_TYPEID].num_rt = 4;
  mvl_type_ctrl[Data_StateQTimeTag_TYPEID].data_size = 8;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Data_StateQTimeTag_TYPEID].rt = u_mvl_get_rt_tbl (Data_StateQTimeTag_TYPEID, 4);
#else
  mvl_type_ctrl[Data_StateQTimeTag_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Data_DiscreteQTimeTag :  */
/*
  mvl_type_ctrl[Data_DiscreteQTimeTag_TYPEID].tdl = 
  "{(Value)<Data_Discrete>,(TimeStamp)<Data_TimeStamp>,(Flags)<Data_Flags>}\
";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Data_DiscreteQTimeTag_TYPEID].asn1_len = 50;
  mvl_type_ctrl[Data_DiscreteQTimeTag_TYPEID].asn1 = "\xa2\x30\xa1\x2e\x30\x0c\x80\x05\x56\x61\x6c\
\x75\x65\xa1\x03\x85\x01\x20\x30\x10\x80\x09\x54\x69\x6d\x65\x53\x74\x61\
\x6d\x70\xa1\x03\x85\x01\x20\x30\x0c\x80\x05\x46\x6c\x61\x67\x73\xa1\x03\
\x84\x01\x08";
#endif
  mvl_type_ctrl[Data_DiscreteQTimeTag_TYPEID].num_rt = 5;
  mvl_type_ctrl[Data_DiscreteQTimeTag_TYPEID].data_size = 12;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Data_DiscreteQTimeTag_TYPEID].rt = u_mvl_get_rt_tbl (Data_DiscreteQTimeTag_TYPEID, 5);
#else
  mvl_type_ctrl[Data_DiscreteQTimeTag_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Data_RealExtended :  */
/*
  mvl_type_ctrl[Data_RealExtended_TYPEID].tdl = 
  "{(Value)<Data_Real>,(TimeStamp)<Data_TimeStamp>,(Flags)<Data_Flags>,(COV\
)<COV_Counter>}";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Data_RealExtended_TYPEID].asn1_len = 67;
  mvl_type_ctrl[Data_RealExtended_TYPEID].asn1 = "\xa2\x41\xa1\x3f\x30\x11\x80\x05\x56\x61\x6c\
\x75\x65\xa1\x08\xa7\x06\x02\x01\x20\x02\x01\x08\x30\x10\x80\x09\x54\x69\
\x6d\x65\x53\x74\x61\x6d\x70\xa1\x03\x85\x01\x20\x30\x0c\x80\x05\x46\x6c\
\x61\x67\x73\xa1\x03\x84\x01\x08\x30\x0a\x80\x03\x43\x4f\x56\xa1\x03\x86\
\x01\x10";
#endif
  mvl_type_ctrl[Data_RealExtended_TYPEID].num_rt = 6;
  mvl_type_ctrl[Data_RealExtended_TYPEID].data_size = 12;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Data_RealExtended_TYPEID].rt = u_mvl_get_rt_tbl (Data_RealExtended_TYPEID, 6);
#else
  mvl_type_ctrl[Data_RealExtended_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Data_StateExtended :  */
/*
  mvl_type_ctrl[Data_StateExtended_TYPEID].tdl = 
  "{(TimeStamp)<Data_TimeStamp>,(Flags)<Data_State>,(COV)<COV_Counter>}";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Data_StateExtended_TYPEID].asn1_len = 48;
  mvl_type_ctrl[Data_StateExtended_TYPEID].asn1 = "\xa2\x2e\xa1\x2c\x30\x10\x80\x09\x54\x69\x6d\
\x65\x53\x74\x61\x6d\x70\xa1\x03\x85\x01\x20\x30\x0c\x80\x05\x46\x6c\x61\
\x67\x73\xa1\x03\x84\x01\x08\x30\x0a\x80\x03\x43\x4f\x56\xa1\x03\x86\x01\x10";
#endif
  mvl_type_ctrl[Data_StateExtended_TYPEID].num_rt = 5;
  mvl_type_ctrl[Data_StateExtended_TYPEID].data_size = 8;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Data_StateExtended_TYPEID].rt = u_mvl_get_rt_tbl (Data_StateExtended_TYPEID, 5);
#else
  mvl_type_ctrl[Data_StateExtended_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Data_DiscreteExtended :  */
/*
  mvl_type_ctrl[Data_DiscreteExtended_TYPEID].tdl = 
  "{(Value)<Data_Discrete>,(TimeStamp)<Data_TimeStamp>,(Flags)<Data_Flags>,\
(COV)<COV_Counter>}";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Data_DiscreteExtended_TYPEID].asn1_len = 62;
  mvl_type_ctrl[Data_DiscreteExtended_TYPEID].asn1 = "\xa2\x3c\xa1\x3a\x30\x0c\x80\x05\x56\x61\x6c\
\x75\x65\xa1\x03\x85\x01\x20\x30\x10\x80\x09\x54\x69\x6d\x65\x53\x74\x61\
\x6d\x70\xa1\x03\x85\x01\x20\x30\x0c\x80\x05\x46\x6c\x61\x67\x73\xa1\x03\
\x84\x01\x08\x30\x0a\x80\x03\x43\x4f\x56\xa1\x03\x86\x01\x10";
#endif
  mvl_type_ctrl[Data_DiscreteExtended_TYPEID].num_rt = 6;
  mvl_type_ctrl[Data_DiscreteExtended_TYPEID].data_size = 12;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Data_DiscreteExtended_TYPEID].rt = u_mvl_get_rt_tbl (Data_DiscreteExtended_TYPEID, 6);
#else
  mvl_type_ctrl[Data_DiscreteExtended_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Control_Command :  */
/*
  mvl_type_ctrl[Control_Command_TYPEID].tdl = 
  "Short";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Control_Command_TYPEID].asn1_len = 3;
  mvl_type_ctrl[Control_Command_TYPEID].asn1 = "\x85\x01\x10";
#endif
  mvl_type_ctrl[Control_Command_TYPEID].num_rt = 1;
  mvl_type_ctrl[Control_Command_TYPEID].data_size = 2;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Control_Command_TYPEID].rt = u_mvl_get_rt_tbl (Control_Command_TYPEID, 1);
#else
  mvl_type_ctrl[Control_Command_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Control_Setpoint_Real :  */
/*
  mvl_type_ctrl[Control_Setpoint_Real_TYPEID].tdl = 
  "Float";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Control_Setpoint_Real_TYPEID].asn1_len = 8;
  mvl_type_ctrl[Control_Setpoint_Real_TYPEID].asn1 = "\xa7\x06\x02\x01\x20\x02\x01\x08";
#endif
  mvl_type_ctrl[Control_Setpoint_Real_TYPEID].num_rt = 1;
  mvl_type_ctrl[Control_Setpoint_Real_TYPEID].data_size = 4;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Control_Setpoint_Real_TYPEID].rt = u_mvl_get_rt_tbl (Control_Setpoint_Real_TYPEID, 1);
#else
  mvl_type_ctrl[Control_Setpoint_Real_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Control_Setpoint_Discrete :  */
/*
  mvl_type_ctrl[Control_Setpoint_Discrete_TYPEID].tdl = 
  "Short";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[Control_Setpoint_Discrete_TYPEID].asn1_len = 3;
  mvl_type_ctrl[Control_Setpoint_Discrete_TYPEID].asn1 = "\x85\x01\x10";
#endif
  mvl_type_ctrl[Control_Setpoint_Discrete_TYPEID].num_rt = 1;
  mvl_type_ctrl[Control_Setpoint_Discrete_TYPEID].data_size = 2;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[Control_Setpoint_Discrete_TYPEID].rt = u_mvl_get_rt_tbl (Control_Setpoint_Discrete_TYPEID, 1);
#else
  mvl_type_ctrl[Control_Setpoint_Discrete_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* SBO_CheckBackName :  */
/*
  mvl_type_ctrl[SBO_CheckBackName_TYPEID].tdl = 
  "Short";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[SBO_CheckBackName_TYPEID].asn1_len = 3;
  mvl_type_ctrl[SBO_CheckBackName_TYPEID].asn1 = "\x85\x01\x10";
#endif
  mvl_type_ctrl[SBO_CheckBackName_TYPEID].num_rt = 1;
  mvl_type_ctrl[SBO_CheckBackName_TYPEID].data_size = 2;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[SBO_CheckBackName_TYPEID].rt = u_mvl_get_rt_tbl (SBO_CheckBackName_TYPEID, 1);
#else
  mvl_type_ctrl[SBO_CheckBackName_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* TextString :  */
/*
  mvl_type_ctrl[TextString_TYPEID].tdl = 
  "Vstring255";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[TextString_TYPEID].asn1_len = 4;
  mvl_type_ctrl[TextString_TYPEID].asn1 = "\x8a\x02\xff\x01";
#endif
  mvl_type_ctrl[TextString_TYPEID].num_rt = 1;
  mvl_type_ctrl[TextString_TYPEID].data_size = 256;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[TextString_TYPEID].rt = u_mvl_get_rt_tbl (TextString_TYPEID, 1);
#else
  mvl_type_ctrl[TextString_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* TagFlags :  */
/*
  mvl_type_ctrl[TagFlags_TYPEID].tdl = 
  "Bstring3";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[TagFlags_TYPEID].asn1_len = 3;
  mvl_type_ctrl[TagFlags_TYPEID].asn1 = "\x84\x01\x03";
#endif
  mvl_type_ctrl[TagFlags_TYPEID].num_rt = 1;
  mvl_type_ctrl[TagFlags_TYPEID].data_size = 1;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[TagFlags_TYPEID].rt = u_mvl_get_rt_tbl (TagFlags_TYPEID, 1);
#else
  mvl_type_ctrl[TagFlags_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* TagValue :  */
/*
  mvl_type_ctrl[TagValue_TYPEID].tdl = 
  "{(Flags)<TagFlags>,(Reason)<TextString>}";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[TagValue_TYPEID].asn1_len = 34;
  mvl_type_ctrl[TagValue_TYPEID].asn1 = "\xa2\x20\xa1\x1e\x30\x0c\x80\x05\x46\x6c\x61\
\x67\x73\xa1\x03\x84\x01\x03\x30\x0e\x80\x06\x52\x65\x61\x73\x6f\x6e\xa1\
\x04\x8a\x02\xff\x01";
#endif
  mvl_type_ctrl[TagValue_TYPEID].num_rt = 4;
  mvl_type_ctrl[TagValue_TYPEID].data_size = 257;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[TagValue_TYPEID].rt = u_mvl_get_rt_tbl (TagValue_TYPEID, 4);
#else
  mvl_type_ctrl[TagValue_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* Scope :  */
/* Integer16 is our twin */

/* User_Data :  */
/*
  mvl_type_ctrl[User_Data_TYPEID].tdl = 
  "{<Integer16>,Long}";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[User_Data_TYPEID].asn1_len = 18;
  mvl_type_ctrl[User_Data_TYPEID].asn1 =
  "\xa2\x10\xa1\x0e\x30\x05\xa1\x03\x85\x01\x10\x30\x05\xa1\x03\x85\x01\x20";
#endif
  mvl_type_ctrl[User_Data_TYPEID].num_rt = 4;
  mvl_type_ctrl[User_Data_TYPEID].data_size = 8;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[User_Data_TYPEID].rt = u_mvl_get_rt_tbl (User_Data_TYPEID, 4);
#else
  mvl_type_ctrl[User_Data_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* ReferenceNum :  */
/* Integer32 is our twin */

/* Number :  */
/* Integer32 is our twin */

/* InfoMessHeader :  */
/*
  mvl_type_ctrl[InfoMessHeader_TYPEID].tdl = 
  "{(InfoReference)<ReferenceNum>,(LocalReference)<ReferenceNum>,(MessageId\
)<ReferenceNum>,(Size)<Number>}";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[InfoMessHeader_TYPEID].asn1_len = 80;
  mvl_type_ctrl[InfoMessHeader_TYPEID].asn1 = "\xa2\x4e\xa1\x4c\x30\x14\x80\x0d\x49\x6e\x66\
\x6f\x52\x65\x66\x65\x72\x65\x6e\x63\x65\xa1\x03\x85\x01\x20\x30\x15\x80\
\x0e\x4c\x6f\x63\x61\x6c\x52\x65\x66\x65\x72\x65\x6e\x63\x65\xa1\x03\x85\
\x01\x20\x30\x10\x80\x09\x4d\x65\x73\x73\x61\x67\x65\x49\x64\xa1\x03\x85\
\x01\x20\x30\x0b\x80\x04\x53\x69\x7a\x65\xa1\x03\x85\x01\x20";
#endif
  mvl_type_ctrl[InfoMessHeader_TYPEID].num_rt = 6;
  mvl_type_ctrl[InfoMessHeader_TYPEID].data_size = 16;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[InfoMessHeader_TYPEID].rt = u_mvl_get_rt_tbl (InfoMessHeader_TYPEID, 6);
#else
  mvl_type_ctrl[InfoMessHeader_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */

/* InfoBuffXXX :  */
/*
  mvl_type_ctrl[InfoBuffXXX_TYPEID].tdl = 
  "Ostring128";
*/
#if !defined(MVL_DYN_ASN1_TYPES)
  mvl_type_ctrl[InfoBuffXXX_TYPEID].asn1_len = 4;
  mvl_type_ctrl[InfoBuffXXX_TYPEID].asn1 = "\x89\x02\x00\x80";
#endif
  mvl_type_ctrl[InfoBuffXXX_TYPEID].num_rt = 1;
  mvl_type_ctrl[InfoBuffXXX_TYPEID].data_size = 128;
#if defined USR_SUPPLIED_RT
  mvl_type_ctrl[InfoBuffXXX_TYPEID].rt = u_mvl_get_rt_tbl (InfoBuffXXX_TYPEID, 1);
#else
  mvl_type_ctrl[InfoBuffXXX_TYPEID].rt = mvl_rt_tables[rt_table_index++];
#endif /* #if defined USR_SUPPLIED_RT */


#if defined USR_SUPPLIED_RT
  u_mvl_end_init_rt_tbl ();
#endif
  }


/************************************************************************/
/* RUNTIME TYPE DATA */


#if !defined USR_SUPPLIED_RT

/************************************************************************/
/* MMSIdentifier :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_0[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_VISIBLE_STRING,		/* el_tag				*/
    33,				/* el_size				*/
    33,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      -32,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Boolean :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_1[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_BOOL,		/* el_tag				*/
    1,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      1,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Unsigned8 :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_2[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_UNSIGNED,		/* el_tag				*/
    1,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      1,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Unsigned16 :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_3[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_UNSIGNED,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Integer32 :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_4[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Integer16 :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_5[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* MMSObjectName :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_6[5] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    67,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      3,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_UNSIGNED,		/* el_tag				*/
    1,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      1,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Scope, 	/* comp_name_ptr 'Scope'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_SCOPE_STRING_INDEX, 	/* name_index 'Scope'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_VISIBLE_STRING,		/* el_tag				*/
    33,				/* el_size				*/
    33,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      -32,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_DomainName, 	/* comp_name_ptr 'DomainName'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_DOMAINNAME_STRING_INDEX, 	/* name_index 'DomainName'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_VISIBLE_STRING,		/* el_tag				*/
    33,				/* el_size				*/
    33,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      -32,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Name, 	/* comp_name_ptr 'Name'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_NAME_STRING_INDEX, 	/* name_index 'Name'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[4] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      3,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* TimeStampExtended :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_7[4] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    8,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_GMTBasedS, 	/* comp_name_ptr 'GMTBasedS'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_GMTBASEDS_STRING_INDEX, 	/* name_index 'GMTBasedS'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Milliseconds, 	/* comp_name_ptr 'Milliseconds'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_MILLISECONDS_STRING_INDEX, 	/* name_index 'Milliseconds'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* DSConditions :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_8[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    1,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      5,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* DSTransferSet :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_9[18] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    88,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      16,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    67,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      3,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_DataSetName, 	/* comp_name_ptr 'DataSetName'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_DATASETNAME_STRING_INDEX, 	/* name_index 'DataSetName'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_UNSIGNED,		/* el_tag				*/
    1,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      1,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Scope, 	/* comp_name_ptr 'Scope'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_SCOPE_STRING_INDEX, 	/* name_index 'Scope'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_VISIBLE_STRING,		/* el_tag				*/
    33,				/* el_size				*/
    33,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      -32,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_DomainName, 	/* comp_name_ptr 'DomainName'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_DOMAINNAME_STRING_INDEX, 	/* name_index 'DomainName'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[4] init data ...			*/
    RT_VISIBLE_STRING,		/* el_tag				*/
    33,				/* el_size				*/
    33,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      -32,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Name, 	/* comp_name_ptr 'Name'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_NAME_STRING_INDEX, 	/* name_index 'Name'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[5] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    1,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      3,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[6] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_StartTime, 	/* comp_name_ptr 'StartTime'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_STARTTIME_STRING_INDEX, 	/* name_index 'StartTime'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[7] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Interval, 	/* comp_name_ptr 'Interval'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_INTERVAL_STRING_INDEX, 	/* name_index 'Interval'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[8] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_TLE, 	/* comp_name_ptr 'TLE'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_TLE_STRING_INDEX, 	/* name_index 'TLE'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[9] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_BufferTime, 	/* comp_name_ptr 'BufferTime'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_BUFFERTIME_STRING_INDEX, 	/* name_index 'BufferTime'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[10] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_IntegrityCheck, 	/* comp_name_ptr 'IntegrityCheck'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_INTEGRITYCHECK_STRING_INDEX, 	/* name_index 'IntegrityCheck'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[11] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    1,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      5,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_DSConditionsRequested, 	/* comp_name_ptr 'DSConditionsRequested'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_DSCONDITIONSREQUESTED_STRING_INDEX, 	/* name_index 'DSConditionsRequested'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[12] init data ...			*/
    RT_BOOL,		/* el_tag				*/
    1,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      1,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_BlockData, 	/* comp_name_ptr 'BlockData'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_BLOCKDATA_STRING_INDEX, 	/* name_index 'BlockData'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[13] init data ...			*/
    RT_BOOL,		/* el_tag				*/
    1,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      1,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Critical, 	/* comp_name_ptr 'Critical'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_CRITICAL_STRING_INDEX, 	/* name_index 'Critical'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[14] init data ...			*/
    RT_BOOL,		/* el_tag				*/
    1,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      1,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_RBE, 	/* comp_name_ptr 'RBE'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_RBE_STRING_INDEX, 	/* name_index 'RBE'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[15] init data ...			*/
    RT_BOOL,		/* el_tag				*/
    2,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      1,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Status, 	/* comp_name_ptr 'Status'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_STATUS_STRING_INDEX, 	/* name_index 'Status'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[16] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_EventCodeRequested, 	/* comp_name_ptr 'EventCodeRequested'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_EVENTCODEREQUESTED_STRING_INDEX, 	/* name_index 'EventCodeRequested'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[17] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      16,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* SupportedFeatures :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_10[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      12,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* TASE2Version :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_11[4] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_MajorVersionNumber, 	/* comp_name_ptr 'MajorVersionNumber'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_MAJORVERSIONNUMBER_STRING_INDEX, 	/* name_index 'MajorVersionNumber'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_MinorVersionNumber, 	/* comp_name_ptr 'MinorVersionNumber'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_MINORVERSIONNUMBER_STRING_INDEX, 	/* name_index 'MinorVersionNumber'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Data_Real :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_12[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_FLOATING_POINT,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Data_State :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_13[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    1,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      8,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Data_Discrete :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_14[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Data_Flags :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_15[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    1,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      8,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Data_RealQ :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_16[4] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    8,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_FLOATING_POINT,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Value, 	/* comp_name_ptr 'Value'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_VALUE_STRING_INDEX, 	/* name_index 'Value'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    4,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      8,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Flags, 	/* comp_name_ptr 'Flags'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_FLAGS_STRING_INDEX, 	/* name_index 'Flags'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Data_StateQ :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_17[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    1,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      8,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Data_DiscreteQ :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_18[4] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    8,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Value, 	/* comp_name_ptr 'Value'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_VALUE_STRING_INDEX, 	/* name_index 'Value'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    4,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      8,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Flags, 	/* comp_name_ptr 'Flags'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_FLAGS_STRING_INDEX, 	/* name_index 'Flags'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Data_RealQTimeTag :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_19[5] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    12,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      3,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_FLOATING_POINT,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Value, 	/* comp_name_ptr 'Value'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_VALUE_STRING_INDEX, 	/* name_index 'Value'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_TimeStamp, 	/* comp_name_ptr 'TimeStamp'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_TIMESTAMP_STRING_INDEX, 	/* name_index 'TimeStamp'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    4,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      8,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Flags, 	/* comp_name_ptr 'Flags'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_FLAGS_STRING_INDEX, 	/* name_index 'Flags'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[4] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      3,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Data_StateQTimeTag :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_20[4] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    8,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_TimeStamp, 	/* comp_name_ptr 'TimeStamp'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_TIMESTAMP_STRING_INDEX, 	/* name_index 'TimeStamp'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    4,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      8,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Flags, 	/* comp_name_ptr 'Flags'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_FLAGS_STRING_INDEX, 	/* name_index 'Flags'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Data_DiscreteQTimeTag :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_21[5] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    12,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      3,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Value, 	/* comp_name_ptr 'Value'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_VALUE_STRING_INDEX, 	/* name_index 'Value'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_TimeStamp, 	/* comp_name_ptr 'TimeStamp'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_TIMESTAMP_STRING_INDEX, 	/* name_index 'TimeStamp'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    4,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      8,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Flags, 	/* comp_name_ptr 'Flags'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_FLAGS_STRING_INDEX, 	/* name_index 'Flags'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[4] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      3,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Data_RealExtended :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_22[6] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    12,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_FLOATING_POINT,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Value, 	/* comp_name_ptr 'Value'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_VALUE_STRING_INDEX, 	/* name_index 'Value'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_TimeStamp, 	/* comp_name_ptr 'TimeStamp'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_TIMESTAMP_STRING_INDEX, 	/* name_index 'TimeStamp'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    2,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      8,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Flags, 	/* comp_name_ptr 'Flags'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_FLAGS_STRING_INDEX, 	/* name_index 'Flags'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[4] init data ...			*/
    RT_UNSIGNED,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_COV, 	/* comp_name_ptr 'COV'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_COV_STRING_INDEX, 	/* name_index 'COV'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[5] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Data_StateExtended :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_23[5] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    8,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      3,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_TimeStamp, 	/* comp_name_ptr 'TimeStamp'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_TIMESTAMP_STRING_INDEX, 	/* name_index 'TimeStamp'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    2,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      8,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Flags, 	/* comp_name_ptr 'Flags'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_FLAGS_STRING_INDEX, 	/* name_index 'Flags'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_UNSIGNED,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_COV, 	/* comp_name_ptr 'COV'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_COV_STRING_INDEX, 	/* name_index 'COV'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[4] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      3,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Data_DiscreteExtended :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_24[6] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    12,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Value, 	/* comp_name_ptr 'Value'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_VALUE_STRING_INDEX, 	/* name_index 'Value'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_TimeStamp, 	/* comp_name_ptr 'TimeStamp'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_TIMESTAMP_STRING_INDEX, 	/* name_index 'TimeStamp'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    2,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      8,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Flags, 	/* comp_name_ptr 'Flags'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_FLAGS_STRING_INDEX, 	/* name_index 'Flags'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[4] init data ...			*/
    RT_UNSIGNED,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_COV, 	/* comp_name_ptr 'COV'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_COV_STRING_INDEX, 	/* name_index 'COV'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[5] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Control_Command :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_25[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Control_Setpoint_Real :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_26[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_FLOATING_POINT,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* Control_Setpoint_Discrete :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_27[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* SBO_CheckBackName :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_28[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    2,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* TextString :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_29[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_VISIBLE_STRING,		/* el_tag				*/
    256,				/* el_size				*/
    256,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      -255,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* TagFlags :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_30[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    1,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      3,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* TagValue :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_31[4] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    257,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_BIT_STRING,		/* el_tag				*/
    1,				/* el_size				*/
    1,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      3,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Flags, 	/* comp_name_ptr 'Flags'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_FLAGS_STRING_INDEX, 	/* name_index 'Flags'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_VISIBLE_STRING,		/* el_tag				*/
    256,				/* el_size				*/
    256,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      -255,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Reason, 	/* comp_name_ptr 'Reason'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_REASON_STRING_INDEX, 	/* name_index 'Reason'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* User_Data :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_32[4] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    8,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    2,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      2,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* InfoMessHeader :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_33[6] =
  {
    {				/* rt[0] init data ...			*/
    RT_STR_START,		/* el_tag				*/
    0,				/* el_size				*/
    16,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[1] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_InfoReference, 	/* comp_name_ptr 'InfoReference'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_INFOREFERENCE_STRING_INDEX, 	/* name_index 'InfoReference'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[2] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_LocalReference, 	/* comp_name_ptr 'LocalReference'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_LOCALREFERENCE_STRING_INDEX, 	/* name_index 'LocalReference'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[3] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_MessageId, 	/* comp_name_ptr 'MessageId'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_MESSAGEID_STRING_INDEX, 	/* name_index 'MessageId'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[4] init data ...			*/
    RT_INTEGER,		/* el_tag				*/
    4,				/* el_size				*/
    4,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    mvlCompName_Size, 	/* comp_name_ptr 'Size'				*/
#else	/* !USE_RT_TYPE_2	*/
    FO_SIZE_STRING_INDEX, 	/* name_index 'Size'				*/
#endif	/* !USE_RT_TYPE_2	*/
    },
    {				/* rt[5] init data ...			*/
    RT_STR_END ,		/* el_tag				*/
    0,				/* el_size				*/
    0,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      4,			/*   num_rt_blks				*/
      0			/*   pad					*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/
/* InfoBuffXXX :  */

SD_CONST static RUNTIME_TYPE mvl_rt_table_34[1] =
  {
    {				/* rt[0] init data ...			*/
    RT_OCTET_STRING,		/* el_tag				*/
    128,				/* el_size				*/
    128,				/* offset_to_last			*/
      {				/* u					*/
      {				/* p					*/
      128,			/*   el_len				*/
      0			/*   pad				*/
      }				/* end 'p'				*/
      },			/* end 'u'				*/
#ifdef USE_RT_TYPE_2
    NULL,				/* comp_name_ptr 				*/
#else	/* !USE_RT_TYPE_2	*/
    0,				/* name_index 				*/
#endif	/* !USE_RT_TYPE_2	*/
    }
  };

/************************************************************************/


ST_INT rt_table_index;

SD_CONST RUNTIME_TYPE * SD_CONST mvl_rt_tables[] =
  {
  mvl_rt_table_0,
  mvl_rt_table_1,
  mvl_rt_table_2,
  mvl_rt_table_3,
  mvl_rt_table_4,
  mvl_rt_table_5,
  mvl_rt_table_6,
  mvl_rt_table_7,
  mvl_rt_table_8,
  mvl_rt_table_9,
  mvl_rt_table_10,
  mvl_rt_table_11,
  mvl_rt_table_12,
  mvl_rt_table_13,
  mvl_rt_table_14,
  mvl_rt_table_15,
  mvl_rt_table_16,
  mvl_rt_table_17,
  mvl_rt_table_18,
  mvl_rt_table_19,
  mvl_rt_table_20,
  mvl_rt_table_21,
  mvl_rt_table_22,
  mvl_rt_table_23,
  mvl_rt_table_24,
  mvl_rt_table_25,
  mvl_rt_table_26,
  mvl_rt_table_27,
  mvl_rt_table_28,
  mvl_rt_table_29,
  mvl_rt_table_30,
  mvl_rt_table_31,
  mvl_rt_table_32,
  mvl_rt_table_33,
  mvl_rt_table_34
  };


#endif /* #if defined USR_SUPPLIED_RT */

