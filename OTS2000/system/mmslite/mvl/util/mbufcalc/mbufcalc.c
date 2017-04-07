/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	  	1997, All Rights Reserved.		        */
/*									*/
/*		    PROPRIETARY AND CONFIDENTIAL			*/
/*									*/
/* MODULE NAME : mbufcalc.c						*/
/* PRODUCT(S)  : MMSEASE						*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/* This module calculates the mms decode buffer size 			*/
/* This module takes a set of variables that are used as limits on the  */
/* number of objects that we will allocate decode space for. This is	*/
/* used in conjunction with the 'max num objs in the message' 		*/
/* in order to prevent our worst case calcs from making us allocate 	*/
/* excessively large buffers. This will be used with MMS-EASE and 	*/
/* possibly with MMS-LITE in some manner.				*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 10/19/01  JRB     08    Add copyright msg.				*/
/* 09/20/01  JRB     07    ALWAYS alloc mmsl_enc_buf.			*/
/* 05/21/01  MDE     06    SMEM related changes				*/
/* 12/08/98  MDE     05    Print error for problem in mmsop_en.h	*/
/* 10/09/98  MDE     04    Fixed up ml_init unused variable warnings	*/
/* 09/21/98  MDE     03    Minor lint cleanup				*/
/* 10/17/97  MDE     02    Now return 0 for SUCCESS			*/
/* 02/25/97  MDE     01    Created					*/
/************************************************************************/
#include "glbtypes.h"
#include "sysincs.h"
#include "cfg_util.h"
#include "mmsdefs.h"
#include "mms_pvar.h"
#include "mms_pcon.h"
#include "mms_pdom.h"
#include "mms_perr.h"
#include "mms_pevn.h"
#include "mms_pfil.h"
#include "mms_pjou.h"
#include "mms_pocs.h"
#include "mms_pprg.h"
#include "mms_psem.h"
#include "mms_pvar.h"
#include "mms_pvmd.h"
#include "mmsop_en.h"
#include "mvl_defs.h"

/************************************************************************/

ST_RET readMmsOpEn (ST_CHAR *opFile);
ST_VOID write_c_header (FILE *fp);
ST_VOID write_c_trailer (FILE *fp);

/************************************************************************/
ST_UINT16 mmsdec_msglen = 1000;

/************************************************************************/
ST_INT m_cl_read_spec_in_result;
ST_INT m_cl_max_read_var_spec;
ST_INT m_cl_max_read_acc_rslt;
ST_INT m_cl_max_info_var_spec;
ST_INT m_cl_max_info_acc_rslt;
ST_INT m_cl_max_getvla_vars;
ST_INT m_cl_max_write_rslts;
ST_INT m_cl_max_namel_names;
ST_INT m_cl_max_getcl_cap;
ST_INT m_cl_max_getpi_doms;
ST_INT m_cl_max_getdom_cap;
ST_INT m_cl_max_getdom_pi;
ST_INT m_cl_max_initupl_cap;
ST_INT m_cl_max_alarm_summary;
ST_INT m_cl_max_journal_entries;
ST_INT m_cl_max_entry_content;
ST_INT m_cl_max_semaphore_entries;
ST_INT m_cl_max_named_tokens;
ST_INT m_cl_max_file_directory;
ST_INT m_cl_max_event_attributes;
ST_INT m_cl_max_event_enrollment;
ST_INT m_cl_max_enrollment_summary;

ST_INT m_sv_max_loadd_cap;
ST_INT m_sv_max_read_var_spec;
ST_INT m_sv_max_write_var_spec;
ST_INT m_sv_max_write_data;
ST_INT m_sv_max_rqdlnl_cap;
ST_INT m_sv_max_initdnld_cap;
ST_INT m_sv_max_vstr;
ST_INT m_sv_max_file_names;  
ST_INT m_sv_max_obj_name;
ST_INT m_sv_max_prompt_count;
ST_INT m_sv_max_num_vars;
ST_INT m_sv_max_data_count;
ST_INT m_sv_max_entry_content;
ST_INT m_sv_max_data_vars;
ST_INT m_sv_max_jread_vars;

ST_UINT max_dec_size;

/************************************************************************/

/************************************************************************/

static ST_INT my_mms_read_en;
static ST_INT my_mms_write_en;
static ST_INT my_mms_info_en;
static ST_INT my_mms_getnames_en;
static ST_INT my_mms_getcl_en;
static ST_INT my_mms_get_pi_en;
static ST_INT my_mms_getvlist_en;
static ST_INT my_mms_get_dom_en;
static ST_INT my_mms_init_upl_en;
static ST_INT my_mms_geteaa_en;
static ST_INT my_mms_geteea_en;
static ST_INT my_mms_fdir_en;
static ST_INT my_mms_getas_en;
static ST_INT my_mms_jread_en;
static ST_INT my_mms_getaes_en;
static ST_INT my_mms_rep_semstat_en;
static ST_INT my_mms_rep_sempool_en;
static ST_INT my_mms_altee_en;
static ST_INT my_mms_delea_en;
static ST_INT my_mms_delec_en;
static ST_INT my_mms_delee_en;
static ST_INT my_mms_deltype_en;
static ST_INT my_mms_delvar_en;
static ST_INT my_mms_delvlist_en;
static ST_INT my_mms_dwn_load_en;
static ST_INT my_mms_fopen_en;
static ST_INT my_mms_fread_en;
static ST_INT my_mms_geteca_en;
static ST_INT my_mms_getscat_en;
static ST_INT my_mms_gettype_en;
static ST_INT my_mms_getvar_en;
static ST_INT my_mms_ident_en;
static ST_INT my_mms_input_en;
static ST_INT my_mms_jinit_en;
static ST_INT my_mms_jstat_en;
static ST_INT my_mms_repeas_en;
static ST_INT my_mms_repecs_en;
static ST_INT my_mms_repees_en;
static ST_INT my_mms_takectrl_en;
static ST_INT my_mms_up_load_en;
static ST_INT my_mms_cre_pi_en;
static ST_INT my_mms_load_dom_en;
static ST_INT my_mms_rddwn_en;
static ST_INT my_mms_rdupl_en;
static ST_INT my_mms_str_dom_en;
static ST_INT my_mms_term_dwn_en;
static ST_INT my_mms_init_dwn_en;
static ST_INT my_mms_defvlist_en;
static ST_INT my_mms_fdelete_en;
static ST_INT my_mms_frename_en;
static ST_INT my_mms_obtainfile_en;
static ST_INT my_mms_output_en;
static ST_INT my_mms_jwrite_en;
static ST_INT my_mms_ackevnot_en;
static ST_INT my_mms_altecm_en;
static ST_INT my_mms_defec_en;
static ST_INT my_mms_defee_en;
static ST_INT my_mms_defscat_en;
static ST_INT my_mms_define_sem_en;
static ST_INT my_mms_deftype_en;
static ST_INT my_mms_defvar_en;
static ST_INT my_mms_del_dom_en;
static ST_INT my_mms_del_pi_en;
static ST_INT my_mms_delete_sem_en;
static ST_INT my_mms_evnot_en;
static ST_INT my_mms_fclose_en;
static ST_INT my_mms_jcreate_en;
static ST_INT my_mms_jdelete_en;
static ST_INT my_mms_kill_en;
static ST_INT my_mms_relctrl_en;
static ST_INT my_mms_rename_en;
static ST_INT my_mms_reset_en;
static ST_INT my_mms_resume_en;
static ST_INT my_mms_rep_sementry_en;
static ST_INT my_mms_start_en;
static ST_INT my_mms_status_en;
static ST_INT my_mms_stop_en;
static ST_INT my_mms_term_upl_en;
static ST_INT my_mms_trige_en;

typedef struct
  {
  ST_CHAR *str;
  ST_INT *addr;
  } MMS_OP_EN_MAP;

MMS_OP_EN_MAP mmsOpEnMap[] =
  {
//    {"MMS_INIT_EN", &my_mms_init_en},
//    {"MMS_CONCLUDE_EN", &my_mms_conclude_en},
//    {"MMS_CANCEL_EN", &my_mms_cancel_en},
    {"MMS_STATUS_EN", &my_mms_status_en},
//    {"MMS_USTATUS_EN", &my_mms_ustatus_en},
    {"MMS_GETNAMES_EN", &my_mms_getnames_en},
    {"MMS_IDENT_EN", &my_mms_ident_en},
    {"MMS_RENAME_EN", &my_mms_rename_en},
    {"MMS_GETCL_EN", &my_mms_getcl_en},
    {"MMS_INIT_DWN_EN", &my_mms_init_dwn_en},
    {"MMS_DWN_LOAD_EN", &my_mms_dwn_load_en},
    {"MMS_TERM_DWN_EN", &my_mms_term_dwn_en},
    {"MMS_INIT_UPL_EN", &my_mms_init_upl_en},
    {"MMS_UP_LOAD_EN", &my_mms_up_load_en},
    {"MMS_TERM_UPL_EN", &my_mms_term_upl_en},
    {"MMS_RDDWN_EN", &my_mms_rddwn_en},
    {"MMS_RDUPL_EN", &my_mms_rdupl_en},
    {"MMS_LOAD_DOM_EN", &my_mms_load_dom_en},
    {"MMS_STR_DOM_EN", &my_mms_str_dom_en},
    {"MMS_DEL_DOM_EN", &my_mms_del_dom_en},
    {"MMS_GET_DOM_EN", &my_mms_get_dom_en},
    {"MMS_CRE_PI_EN", &my_mms_cre_pi_en},
    {"MMS_DEL_PI_EN", &my_mms_del_pi_en},
    {"MMS_START_EN", &my_mms_start_en},
    {"MMS_STOP_EN", &my_mms_stop_en},
    {"MMS_RESUME_EN", &my_mms_resume_en},
    {"MMS_RESET_EN", &my_mms_reset_en},
    {"MMS_KILL_EN", &my_mms_kill_en},
    {"MMS_GET_PI_EN", &my_mms_get_pi_en},
    {"MMS_READ_EN", &my_mms_read_en},
    {"MMS_WRITE_EN", &my_mms_write_en},
    {"MMS_INFO_EN", &my_mms_info_en},
    {"MMS_GETVAR_EN", &my_mms_getvar_en},
    {"MMS_DEFVAR_EN", &my_mms_defvar_en},
    {"MMS_DEFSCAT_EN", &my_mms_defscat_en},
    {"MMS_GETSCAT_EN", &my_mms_getscat_en},
    {"MMS_DELVAR_EN", &my_mms_delvar_en},
    {"MMS_DEFVLIST_EN", &my_mms_defvlist_en},
    {"MMS_GETVLIST_EN", &my_mms_getvlist_en},
    {"MMS_DELVLIST_EN", &my_mms_delvlist_en},
    {"MMS_DEFTYPE_EN", &my_mms_deftype_en},
    {"MMS_GETTYPE_EN", &my_mms_gettype_en},
    {"MMS_DELTYPE_EN", &my_mms_deltype_en},
    {"MMS_TAKECTRL_EN", &my_mms_takectrl_en},
    {"MMS_RELCTRL_EN", &my_mms_relctrl_en},
    {"MMS_DEFINE_SEM_EN", &my_mms_define_sem_en},
    {"MMS_DELETE_SEM_EN", &my_mms_delete_sem_en},
    {"MMS_REP_SEMSTAT_EN", &my_mms_rep_semstat_en},
    {"MMS_REP_SEMPOOL_EN", &my_mms_rep_sempool_en},
    {"MMS_REP_SEMENTRY_EN", &my_mms_rep_sementry_en},
    {"MMS_JREAD_EN", &my_mms_jread_en},
    {"MMS_JWRITE_EN", &my_mms_jwrite_en},
    {"MMS_JINIT_EN", &my_mms_jinit_en},
    {"MMS_JSTAT_EN", &my_mms_jstat_en},
    {"MMS_JCREATE_EN", &my_mms_jcreate_en},
    {"MMS_JDELETE_EN", &my_mms_jdelete_en},
    {"MMS_DEFEC_EN", &my_mms_defec_en},
    {"MMS_DELEC_EN", &my_mms_delec_en},
    {"MMS_GETECA_EN", &my_mms_geteca_en},
    {"MMS_REPECS_EN", &my_mms_repecs_en},
    {"MMS_ALTECM_EN", &my_mms_altecm_en},
    {"MMS_TRIGE_EN", &my_mms_trige_en},
//    {"MMS_DEFEA_EN", &my_mms_defea_en},
    {"MMS_DELEA_EN", &my_mms_delea_en},
    {"MMS_GETEAA_EN", &my_mms_geteaa_en},
    {"MMS_REPEAS_EN", &my_mms_repeas_en},
    {"MMS_DEFEE_EN", &my_mms_defee_en},
    {"MMS_DELEE_EN", &my_mms_delee_en},
    {"MMS_GETEEA_EN", &my_mms_geteea_en},
    {"MMS_REPEES_EN", &my_mms_repees_en},
    {"MMS_ALTEE_EN", &my_mms_altee_en},
    {"MMS_EVNOT_EN", &my_mms_evnot_en},
    {"MMS_ACKEVNOT_EN", &my_mms_ackevnot_en},
    {"MMS_GETAS_EN", &my_mms_getas_en},
    {"MMS_GETAES_EN", &my_mms_getaes_en},
    {"MMS_INPUT_EN", &my_mms_input_en},
    {"MMS_OUTPUT_EN", &my_mms_output_en},
    {"MMS_OBTAINFILE_EN", &my_mms_obtainfile_en},
    {"MMS_FOPEN_EN", &my_mms_fopen_en},
    {"MMS_FREAD_EN", &my_mms_fread_en},
    {"MMS_FCLOSE_EN", &my_mms_fclose_en},
    {"MMS_FRENAME_EN", &my_mms_frename_en},
    {"MMS_FDELETE_EN", &my_mms_fdelete_en},
    {"MMS_FDIR_EN", &my_mms_fdir_en},
  };
ST_INT numOpMap = sizeof(mmsOpEnMap)/sizeof(MMS_OP_EN_MAP);

/************************************************************************/
static ST_VOID cfg_mmsdec_msglen (ST_VOID);
static ST_VOID cfg_m_cl_read_spec_in_result (ST_VOID);
static ST_VOID cfg_m_cl_max_read_var_spec (ST_VOID);
static ST_VOID cfg_m_cl_max_read_acc_rslt (ST_VOID);
static ST_VOID cfg_m_cl_max_info_var_spec (ST_VOID);
static ST_VOID cfg_m_cl_max_info_acc_rslt (ST_VOID);
static ST_VOID cfg_m_cl_max_getvla_vars (ST_VOID);
static ST_VOID cfg_m_cl_max_write_rslts (ST_VOID);
static ST_VOID cfg_m_cl_max_namel_names (ST_VOID);
static ST_VOID cfg_m_cl_max_getcl_cap (ST_VOID);
static ST_VOID cfg_m_cl_max_getpi_doms (ST_VOID);
static ST_VOID cfg_m_cl_max_getdom_cap (ST_VOID);
static ST_VOID cfg_m_cl_max_getdom_pi (ST_VOID);
static ST_VOID cfg_m_cl_max_initupl_cap (ST_VOID);
static ST_VOID cfg_m_cl_max_alarm_summary (ST_VOID);
static ST_VOID cfg_m_cl_max_journal_entries (ST_VOID);
static ST_VOID cfg_m_cl_max_entry_content (ST_VOID);
static ST_VOID cfg_m_cl_max_semaphore_entries (ST_VOID);
static ST_VOID cfg_m_cl_max_named_tokens (ST_VOID);
static ST_VOID cfg_m_cl_max_file_directory (ST_VOID);
static ST_VOID cfg_m_cl_max_event_attributes (ST_VOID);
static ST_VOID cfg_m_cl_max_event_enrollment (ST_VOID);
static ST_VOID cfg_m_cl_max_enrollment_summary (ST_VOID);

static ST_VOID cfg_m_sv_max_loadd_cap (ST_VOID);
static ST_VOID cfg_m_sv_max_read_var_spec (ST_VOID);
static ST_VOID cfg_m_sv_max_write_var_spec (ST_VOID);
static ST_VOID cfg_m_sv_max_write_data (ST_VOID);
static ST_VOID cfg_m_sv_max_rqdlnl_cap (ST_VOID);
static ST_VOID cfg_m_sv_max_initdnld_cap (ST_VOID);
static ST_VOID cfg_m_sv_max_vstr (ST_VOID);
static ST_VOID cfg_m_sv_max_file_names (ST_VOID);
static ST_VOID cfg_m_sv_max_obj_name (ST_VOID);
static ST_VOID cfg_m_sv_max_prompt_count (ST_VOID);
static ST_VOID cfg_m_sv_max_num_vars (ST_VOID);
static ST_VOID cfg_m_sv_max_data_count (ST_VOID);
static ST_VOID cfg_m_sv_max_entry_content (ST_VOID);
static ST_VOID cfg_m_sv_max_data_vars (ST_VOID);
static ST_VOID cfg_m_sv_max_jread_vars (ST_VOID);

static ST_VOID cfg_ind_mode (ST_VOID);
static ST_VOID cfg_conf_mode (ST_VOID);
static ST_VOID cfg_max_req_outstanding (ST_VOID);
static ST_VOID cfg_uca_support (ST_VOID);
static ST_VOID cfg_uca_data_mode (ST_VOID);
static ST_VOID cfg_uca_array_vars (ST_VOID);
static ST_VOID cfg_uca_arr_alt_acc (ST_VOID);
static ST_VOID cfg_client_alt_acc (ST_VOID);

static ST_VOID cfg_begin (ST_VOID);
static ST_VOID cfg_end (ST_VOID);

static ST_VOID _write_smemcfg (ST_VOID);

struct cfg_kw_el kwtblDecodeSizeAttr [] = 
  {
    {"Begin",			    cfg_begin,           		NULL},
    {"mmsdec_msglen",  		    cfg_mmsdec_msglen,			NULL},
    {"m_cl_read_spec_in_result",    cfg_m_cl_read_spec_in_result,	NULL},
    {"m_cl_max_read_var_spec",	    cfg_m_cl_max_read_var_spec,		NULL},
    {"m_cl_max_read_acc_rslt",	    cfg_m_cl_max_read_acc_rslt,		NULL},
    {"m_cl_max_info_var_spec",	    cfg_m_cl_max_info_var_spec,		NULL},
    {"m_cl_max_info_acc_rslt",	    cfg_m_cl_max_info_acc_rslt,		NULL},
    {"m_cl_max_getvla_vars",	    cfg_m_cl_max_getvla_vars,		NULL},
    {"m_cl_max_write_rslts",	    cfg_m_cl_max_write_rslts,		NULL},
    {"m_cl_max_namel_names",	    cfg_m_cl_max_namel_names,		NULL},
    {"m_cl_max_getcl_cap",	    cfg_m_cl_max_getcl_cap,		NULL},
    {"m_cl_max_getpi_doms",	    cfg_m_cl_max_getpi_doms,		NULL},
    {"m_cl_max_getdom_cap",	    cfg_m_cl_max_getdom_cap,		NULL},
    {"m_cl_max_getdom_pi",	    cfg_m_cl_max_getdom_pi,		NULL},
    {"m_cl_max_initupl_cap",	    cfg_m_cl_max_initupl_cap,		NULL},
    {"m_cl_max_alarm_summary",	    cfg_m_cl_max_alarm_summary,		NULL},
    {"m_cl_max_journal_entries",    cfg_m_cl_max_journal_entries,	NULL},
    {"m_cl_max_entry_content",	    cfg_m_cl_max_entry_content,		NULL},
    {"m_cl_max_semaphore_entries",  cfg_m_cl_max_semaphore_entries,	NULL},
    {"m_cl_max_named_tokens",	    cfg_m_cl_max_named_tokens,		NULL},
    {"m_cl_max_file_directory",	    cfg_m_cl_max_file_directory,	NULL},
    {"m_cl_max_event_attributes",   cfg_m_cl_max_event_attributes,	NULL},
    {"m_cl_max_event_enrollment",   cfg_m_cl_max_event_enrollment,	NULL},
    {"m_cl_max_enrollment_summary", cfg_m_cl_max_enrollment_summary,	NULL},

    {"m_sv_max_loadd_cap",	    cfg_m_sv_max_loadd_cap,		NULL},
    {"m_sv_max_read_var_spec",	    cfg_m_sv_max_read_var_spec,		NULL},
    {"m_sv_max_write_var_spec",	    cfg_m_sv_max_write_var_spec,	NULL},
    {"m_sv_max_write_data",	    cfg_m_sv_max_write_data,		NULL},
    {"m_sv_max_rqdlnl_cap",	    cfg_m_sv_max_rqdlnl_cap,		NULL},
    {"m_sv_max_initdnld_cap",	    cfg_m_sv_max_initdnld_cap,		NULL},
    {"m_sv_max_vstr",		    cfg_m_sv_max_vstr,			NULL},
    {"m_sv_max_file_names",	    cfg_m_sv_max_file_names,		NULL},  
    {"m_sv_max_obj_name",	    cfg_m_sv_max_obj_name,		NULL},
    {"m_sv_max_prompt_count",	    cfg_m_sv_max_prompt_count,		NULL},
    {"m_sv_max_num_vars",	    cfg_m_sv_max_num_vars,		NULL},
    {"m_sv_max_data_count",	    cfg_m_sv_max_data_count,		NULL},
    {"m_sv_max_entry_content",	    cfg_m_sv_max_entry_content,		NULL},
    {"m_sv_max_data_vars",	    cfg_m_sv_max_data_vars,		NULL},
    {"m_sv_max_jread_vars",	    cfg_m_sv_max_jread_vars,		NULL},
    {"ind_mode",		    cfg_ind_mode, 			NULL},
    {"conf_mode",		    cfg_conf_mode, 			NULL},
    {"max_req_outstanding",	    cfg_max_req_outstanding, 		NULL},
    {"uca_support",		    cfg_uca_support, 			NULL},
    {"uca_data_mode",		    cfg_uca_data_mode, 			NULL},
    {"uca_array_vars",		    cfg_uca_array_vars, 		NULL},
    {"uca_arr_alt_acc",		    cfg_uca_arr_alt_acc, 		NULL},
    {"client_alt_acc",		    cfg_client_alt_acc, 		NULL},
    {"end",	  		    cfg_end,				NULL},
    { NULL, NULL, NULL}
  };


/************************************************************************/

/************************************************************************/

ST_INT max_num_vars;
ST_INT max_rslts;
ST_INT max_obj_name; /* ike needs attention */
ST_INT max_vstr;
ST_INT max_modifiers;
ST_INT max_fnames;
ST_INT max_vars;
ST_INT max_entries;
ST_INT max_data_vars;
ST_INT max_capabilities;
ST_INT max_data_count;
ST_INT max_ev_enrolls;
ST_INT max_pi_names;
ST_INT max_dir_ents;
ST_INT max_prompt_count;
ST_INT max_summaries;
ST_INT max_jou_ent;
ST_INT max_ec_vars;
ST_INT max_enroll_sums;
ST_INT max_sem_entries;
ST_INT max_named_tokens;
ST_INT max_acc_rslt;
ST_INT max_domain_names;


#define SMEM_ITEM_UNDEFINED	-1
#define IND_MODE_IMMEDIATE	1
#define IND_MODE_ASYNC		2
#define CONF_MODE_IMMEDIATE	1
#define CONF_MODE_ASYNC		2
#define UCA_DATA_MODE_STATIC	1
#define UCA_DATA_MODE_DYNAMIC	2

ST_INT ind_mode	 		= SMEM_ITEM_UNDEFINED;
ST_INT conf_mode	 	= SMEM_ITEM_UNDEFINED;
ST_INT max_req_outstanding	= SMEM_ITEM_UNDEFINED;
ST_INT uca_support	 	= SMEM_ITEM_UNDEFINED;
ST_INT uca_data_mode	 	= SMEM_ITEM_UNDEFINED;
ST_INT uca_array_vars	 	= SMEM_ITEM_UNDEFINED;
ST_INT uca_arr_alt_acc	 	= SMEM_ITEM_UNDEFINED;
ST_INT client_alt_acc	 	= SMEM_ITEM_UNDEFINED;

/************************************************************************/
/*				main					*/
/* This calculates the decode buffer for every MMS service and the max	*/
/* buffer for decoding mms pdu						*/
/* It is divided into small blocks where every block calculating decode	*/
/* buffer for a particular service					*/
/************************************************************************/

ST_UINT req_size;
ST_UINT rsp_size;

ST_VOID printUsage (ST_VOID);

ST_CHAR outFileName[200] = "ml_init.c";
ST_CHAR cfgFileName[200] = "mbufcalc.cfg";
ST_CHAR mmsopFileName[200] = "mmsop_en.h";

ST_BOOLEAN smem_support = SD_FALSE;

int main (int argc, char *argv[])
  {
ST_INT size1;
ST_INT size2;
ST_RET ret;
FILE *fp;
ST_INT i;


  printf ("\n                     MMS-EASE Lite Mbufcalc"
          "\n(c) Copyright Systems Integration Specialists Company, Inc.,"
          "\n                 1997-2001, All Rights Reserved\n");

  if (argc > 4)
    {
    printUsage ();
    exit (2);
    }

  for (i = 1; i < argc; ++i)
    {
    if (!strncmp (argv[i], "-c", 2))
      strcpy (cfgFileName, &argv[i][2]);
    else if (!strncmp (argv[i], "-m", 2))
      strcpy (mmsopFileName, &argv[i][2]);
    else if (!strcmp (argv[i], "-smem"))
      smem_support = SD_TRUE;
    else if (!strncmp (argv[i], "-", 1))
      {
      printUsage ();
      exit (2);
      }
    else
      strcpy (outFileName, argv[i]);
    }


  ret = readMmsOpEn (mmsopFileName);
  if (ret != SD_SUCCESS)
    exit (1);

  ret = cfg_process_file (cfgFileName, kwtblDecodeSizeAttr);
  if (ret != SD_SUCCESS)
    {
    if (cfg_curr_line == 0)
      printf ("\nConfig Error, Unable to open %s", cfgFileName);
    else  
      printf ("\nConfig Error, Check Line %d in %s", 
					cfg_curr_line, cfgFileName);
    exit (1);
    }

  fp = fopen (outFileName, "w");
  if (fp == NULL)
    {
    printf ("\n  Error: Could not open output file '%s'", outFileName);
    exit (2);
    }
  write_c_header (fp);
  max_dec_size = 0;

/* ****** CLIENT: rq_read.c */
if (my_mms_read_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Read */");
  fprintf (fp, "\n/*   m_cl_read_spec_in_result = %d */",m_cl_read_spec_in_result);
  fprintf (fp, "\n/*   m_cl_max_read_var_spec = %d */",m_cl_max_read_var_spec);
  fprintf (fp, "\n/*   m_cl_max_read_acc_rslt = %d */",m_cl_max_read_acc_rslt);

  if (m_cl_read_spec_in_result)
    {
    max_vars     = mmsdec_msglen / 4;
    if (m_cl_max_read_var_spec && (max_vars > m_cl_max_read_var_spec))
      max_vars = m_cl_max_read_var_spec;
    rsp_size = sizeof(READ_RESP_INFO) + (sizeof(VARIABLE_LIST) * max_vars);

    fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
    fprintf (fp, "\n/*   Allows for %d Variables */", max_vars);
    fprintf (fp, "\n/*   Need %d per variable */", sizeof(VARIABLE_LIST));

    fprintf (fp, "\n  m_cl_read_spec_in_result = SD_TRUE;");
    fprintf (fp, "\n  m_cl_max_read_var_spec = %d;", max_vars);
    max_acc_rslt = (rsp_size - sizeof(READ_RESP_INFO))/sizeof(ACCESS_RESULT);
    fprintf (fp, "\n  m_cl_max_read_acc_rslt = %d;", max_acc_rslt);
    }
  else
    {
    max_acc_rslt = mmsdec_msglen / 3;
    if (m_cl_max_read_acc_rslt && (max_acc_rslt > m_cl_max_read_acc_rslt))
      max_acc_rslt = m_cl_max_read_acc_rslt;
    rsp_size = sizeof(READ_RESP_INFO) + (sizeof(ACCESS_RESULT) * max_acc_rslt);

    fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
    fprintf (fp, "\n/*   Allows for %d Access Results */", max_acc_rslt);
    fprintf (fp, "\n/*   Need %d per access result */", sizeof(ACCESS_RESULT));
    fprintf (fp, "\n  m_cl_read_spec_in_result = SD_FALSE;");

    max_vars = (rsp_size - sizeof(READ_RESP_INFO))/sizeof(VARIABLE_LIST);
    fprintf (fp, "\n  m_cl_max_read_var_spec = %d;", max_vars);
    fprintf (fp, "\n  m_cl_max_read_acc_rslt = %d;", max_acc_rslt);
    }
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_write.c */
if (my_mms_write_en & REQ_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* CLIENT: Write (rq_write.c) */");
  fprintf (fp, "\n/*   m_cl_max_write_rslts = %d */",m_cl_max_write_rslts);

  max_rslts = mmsdec_msglen / 2;
  if (m_cl_max_write_rslts && (max_rslts > m_cl_max_write_rslts))
    max_rslts = m_cl_max_write_rslts;
  rsp_size  = sizeof(WRITE_RESP_INFO) + (sizeof(WRITE_RESULT) * max_rslts);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u*/", rsp_size);
  fprintf (fp, "\n/*   Allows for %d write results */", max_rslts);
  fprintf (fp, "\n/*   Need %d per write result */", sizeof(WRITE_RESULT));
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;

  fprintf (fp, "\n  m_cl_max_write_rslts = %d;", max_rslts);
  }


/* ****** CLIENT: rs_info.c */
if (my_mms_info_en & RESP_EN)			  
  {
/* remove : here are two control variables and we are using algorithm 2 */
/* i.e. rsp_size = max(size1,size2)					*/
  fprintf (fp, "\n\n/* CLIENT: Info Report (rs_info.c) */");
  fprintf (fp, "\n/*   m_cl_max_info_var_spec = %d */",m_cl_max_info_var_spec);
  fprintf (fp, "\n/*   m_cl_max_info_acc_rslt = %d */",m_cl_max_info_acc_rslt);

/* First assume a list of variables (each var has a data) */
  max_vars	 = mmsdec_msglen / 7;
  if (m_cl_max_info_var_spec && (max_vars > m_cl_max_info_var_spec))
    max_vars = m_cl_max_info_var_spec;

  size1 = sizeof(INFO_REQ_INFO) + 
          (max_vars * (sizeof(VARIABLE_LIST) + sizeof(ACCESS_RESULT)));

/* Now assume a named variable list name */
  max_acc_rslt = mmsdec_msglen / 3;
  if (m_cl_max_info_acc_rslt && (max_acc_rslt > m_cl_max_info_acc_rslt))
    max_acc_rslt = m_cl_max_info_acc_rslt;
  size2 = sizeof(INFO_REQ_INFO) + (sizeof(ACCESS_RESULT) * max_acc_rslt);

  req_size = size1 > size2 ? size1 : size2;
  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Variables (list of variables) size1: %d */", 
						max_vars, size1);
  fprintf (fp, "\n/*   Need %d per variable */", sizeof(VARIABLE_LIST));
  fprintf (fp, "\n/*   Allows for %d Data (named variable list) size2: %d */", 
						max_acc_rslt, size2);
  fprintf (fp, "\n/*   Need %d per data */", sizeof(ACCESS_RESULT));
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  fprintf (fp, "\n  m_cl_max_info_acc_rslt = %d;", max_acc_rslt);
  fprintf (fp, "\n  m_cl_max_info_var_spec = %d;", max_vars);
  }


/* ****** CLIENT: rq_namel.c */
if (my_mms_getnames_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Get Name List */");
  fprintf (fp, "\n/*   m_cl_max_namel_names = %d */",m_cl_max_namel_names);

  max_vstr = mmsdec_msglen/2;
  if (m_cl_max_namel_names && (max_vstr > m_cl_max_namel_names))
    max_vstr = m_cl_max_namel_names;
  rsp_size = sizeof(NAMELIST_RESP_INFO) + (sizeof(ST_CHAR *) * max_vstr);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  fprintf (fp, "\n/*   Allows for %d Names */", max_vstr);
  fprintf (fp, "\n/*   Need %d per Name */", sizeof(ST_CHAR *));
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  fprintf (fp, "\n  m_cl_max_namel_names = %d;", max_vstr);
  }

/* ****** CLIENT: rq_getcl.c */
if (my_mms_getcl_en & REQ_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* CLIENT: Get Capability List */");
  fprintf (fp, "\n/*   m_cl_max_getcl_cap = %d */",m_cl_max_getcl_cap);

  max_capabilities = mmsdec_msglen/2;
  if (m_cl_max_getcl_cap && (max_capabilities > m_cl_max_getcl_cap))
    max_capabilities = m_cl_max_getcl_cap;
  rsp_size = sizeof(GETCL_RESP_INFO)+ (max_capabilities * sizeof(ST_CHAR *));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  fprintf (fp, "\n/*   Allows for %d Capabilities */", max_capabilities);
  fprintf (fp, "\n/*   Need %d per Capability */", sizeof(ST_CHAR *));
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  fprintf (fp, "\n  m_cl_max_getcl_cap = %d;", max_capabilities);
  }

/* ****** CLIENT: rq_getpi.c */
if (my_mms_get_pi_en & REQ_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* CLIENT: Get PI Attributes */");
  fprintf (fp, "\n/*   m_cl_max_getpi_doms = %d */",m_cl_max_getpi_doms);

  max_domain_names = mmsdec_msglen/2;
  if (m_cl_max_getpi_doms && (max_domain_names > m_cl_max_getpi_doms))
    max_domain_names = m_cl_max_getpi_doms;
  rsp_size = sizeof(GETPI_RESP_INFO) + (max_domain_names * sizeof(ST_CHAR *));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  fprintf (fp, "\n/*   Allows for %d Domain Names */", max_domain_names);
  fprintf (fp, "\n/*   Need %d per Domain */", sizeof(ST_CHAR *));
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  fprintf (fp, "\n  m_cl_max_getpi_doms = %d;", max_domain_names);
  }

/* ****** CLIENT: rq_getvl.c */
if (my_mms_getvlist_en & REQ_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* CLIENT: Get Variable List Attributes */");
  fprintf (fp, "\n/*   m_cl_max_getvla_vars = %d */",m_cl_max_getvla_vars);

  max_vars     = mmsdec_msglen / 4;
  if (m_cl_max_getvla_vars && (max_vars > m_cl_max_getvla_vars))
    max_vars = m_cl_max_getvla_vars;
  rsp_size = sizeof(GETVLIST_RESP_INFO) + 
			(sizeof(VARIABLE_LIST) * max_vars);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  fprintf (fp, "\n/*   Allows for %d Variables */", max_vars);
  fprintf (fp, "\n/*   Need %d per variable */", sizeof(VARIABLE_LIST));
  if (rsp_size > max_dec_size)
  fprintf (fp, "\n  m_cl_max_getvla_vars = %d;", max_vars);
  }

/* ****** CLIENT: rq_getdo.c */
if (my_mms_get_dom_en & REQ_EN)
  {
/* remove : the following service also has two control variables	*/
/* we are using algorithm1 i.e. each control var multiplied by its struct */
  fprintf (fp, "\n\n/* CLIENT: Get Domain Attributes */");
  fprintf (fp, "\n/*   m_cl_max_getdom_cap = %d */",m_cl_max_getdom_cap);
  fprintf (fp, "\n/*   m_cl_max_getdom_pi = %d */",m_cl_max_getdom_pi);

  max_capabilities = mmsdec_msglen/2;
  max_pi_names	   = mmsdec_msglen/2;
  if (m_cl_max_getdom_cap && (max_capabilities > m_cl_max_getdom_cap))
    max_capabilities = m_cl_max_getdom_cap;
  if (m_cl_max_getdom_pi && (max_pi_names > m_cl_max_getdom_pi))
    max_pi_names = m_cl_max_getdom_pi;
  rsp_size = sizeof(GETDOM_RESP_INFO) +
		 (max_capabilities * sizeof(ST_CHAR *)) +
		 (max_pi_names     * sizeof(ST_CHAR *));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  fprintf (fp, "\n/*   Allows for %d Capabilities */", max_capabilities);
  fprintf (fp, "\n/*   Allows for %d PI Names */", max_pi_names);
  fprintf (fp, "\n/*   Need %d per Capability */", sizeof(ST_CHAR *));
  fprintf (fp, "\n/*   Need %d per PI */", sizeof(ST_CHAR *));
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  fprintf (fp, "\n  m_cl_max_getdom_cap = %d;", max_capabilities);
  fprintf (fp, "\n  m_cl_max_getdom_pi = %d;", max_pi_names);
  }
 
/* ****** CLIENT: rq_initu.c */
if (my_mms_init_upl_en & REQ_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* CLIENT: Initiate Upload */");
  fprintf (fp, "\n/*   m_cl_max_initupl_cap = %d */",m_cl_max_initupl_cap);

  max_capabilities = mmsdec_msglen/2;
  if (m_cl_max_initupl_cap && (max_capabilities > m_cl_max_initupl_cap))
    max_capabilities = m_cl_max_initupl_cap;
  rsp_size = sizeof(INITUPL_RESP_INFO)
		+ (max_capabilities * (sizeof(ST_CHAR *)));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  fprintf (fp, "\n/*   Allows for %d Capabilities */", max_capabilities);
  fprintf (fp, "\n/*   Need %d per Capability */", sizeof(ST_CHAR *));
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  fprintf (fp, "\n  m_cl_max_initupl_cap = %d;", max_capabilities);
  }
 
/* ****** CLIENT: rq_getea.c */
if (my_mms_geteaa_en & REQ_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* CLIENT: Get Event Attributes */");
  fprintf (fp, "\n/*   m_cl_max_event_attributes = %d */",m_cl_max_event_attributes);

  max_modifiers = mmsdec_msglen / 2;
  if (m_cl_max_event_attributes && (max_modifiers > m_cl_max_event_attributes))
    max_modifiers = m_cl_max_event_attributes;
  rsp_size = sizeof(GETEAA_RESP_INFO) + (sizeof(MODIFIER) * max_modifiers);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  fprintf (fp, "\n/*   Allows for %d Modifiers */", max_modifiers);
  fprintf (fp, "\n/*   Need %d per modifier */", sizeof(MODIFIER));
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  fprintf (fp, "\n  m_cl_max_event_attributes = %d;", max_modifiers);
  }
         
/* ****** CLIENT: rq_getee.c */
if (my_mms_geteea_en & REQ_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* CLIENT: Get Event Enrollment */");
  fprintf (fp, "\n/*   m_cl_max_event_enrollment = %d */",m_cl_max_event_enrollment);

  max_ev_enrolls = mmsdec_msglen / 2;
  if (m_cl_max_event_enrollment && (max_ev_enrolls > m_cl_max_event_enrollment))
    max_ev_enrolls = m_cl_max_event_enrollment;
  rsp_size = sizeof(GETEEA_RESP_INFO)
		+ (sizeof(EVENT_ENROLLMENT) * max_ev_enrolls);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  fprintf (fp, "\n/*   Allows for %d Event Enrollments */", max_ev_enrolls);
  fprintf (fp, "\n/*   Need %d per Event Enrollment */", sizeof(EVENT_ENROLLMENT));
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  fprintf (fp, "\n  m_cl_max_event_enrollment = %d;", max_ev_enrolls);
  }
  
/* ****** CLIENT: rq_fdir.c */
if (my_mms_fdir_en & REQ_EN)
  {
/* remove : we are ok here but double check if we need max_fnames	*/
/* 	    to be multiplied by sizeof(FILE_NAME)			*/
  fprintf (fp, "\n\n/* CLIENT: File Directory */");
  fprintf (fp, "\n/*   m_cl_max_file_directory = %d */",m_cl_max_file_directory);

  max_dir_ents = mmsdec_msglen/8;
  if (m_cl_max_file_directory && (max_dir_ents > m_cl_max_file_directory))
    max_dir_ents = m_cl_max_file_directory;
  rsp_size    = sizeof(FDIR_RESP_INFO) +
		  (max_dir_ents * sizeof(FDIR_DIR_ENT)) +
		  (max_dir_ents * sizeof(FILE_NAME));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  fprintf (fp, "\n/*   Allows for %d Directory Entries */", max_dir_ents);
  fprintf (fp, "\n/*   Need %d per Directory Entry */", 
				sizeof(FDIR_DIR_ENT) + sizeof(FILE_NAME));
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  fprintf (fp, "\n  m_cl_max_file_directory = %d;", max_dir_ents);
  }
      
/* ****** CLIENT: rq_getas.c */
if (my_mms_getas_en & REQ_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* CLIENT: Get Alarm Summary */");
  fprintf (fp, "\n/*   m_cl_max_alarm_summary = %d */",m_cl_max_alarm_summary);

  max_summaries = mmsdec_msglen/8;
  if (m_cl_max_alarm_summary && (max_summaries > m_cl_max_alarm_summary))
    max_summaries = m_cl_max_alarm_summary;
  rsp_size = sizeof(GETAS_RESP_INFO) + 
		 (max_summaries * (sizeof(ALARM_SUMMARY)));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  fprintf (fp, "\n/*   Allows for %d Summaries */", max_summaries);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  fprintf (fp, "\n  m_cl_max_alarm_summary = %d;", max_summaries);
  }
 
/* ****** CLIENT: rq_jread.c */
if (my_mms_jread_en & REQ_EN)
  {
/* remove : here are again two control variables			*/
/* we are using algorithm1 i.e. each control var multiplied by its struct */
  fprintf (fp, "\n\n/* CLIENT: Journal Read */");
  fprintf (fp, "\n/*   m_cl_max_journal_entries = %d */",m_cl_max_journal_entries);
  fprintf (fp, "\n/*   m_cl_max_entry_content = %d */",m_cl_max_entry_content);

  max_jou_ent  = mmsdec_msglen/10;
  max_ec_vars  = mmsdec_msglen/4;
  if (m_cl_max_journal_entries && (max_jou_ent > m_cl_max_journal_entries))
    max_jou_ent = m_cl_max_journal_entries;
  if (m_cl_max_entry_content && (max_ec_vars > m_cl_max_entry_content))
    max_ec_vars = m_cl_max_entry_content;
  rsp_size = sizeof(JREAD_RESP_INFO) +
	       (sizeof(JOURNAL_ENTRY) * max_jou_ent) +
  	       (sizeof(VAR_INFO)      * max_ec_vars);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  fprintf (fp, "\n/*   Allows for %d Journal Entries */", max_jou_ent);
  fprintf (fp, "\n/*   Allows for %d EC Vars */", max_ec_vars);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  fprintf (fp, "\n  m_cl_max_journal_entries = %d;", max_jou_ent);
  fprintf (fp, "\n  m_cl_max_entry_content = %d;", max_ec_vars);
  }

/* ****** CLIENT: rq_getae.c */
if (my_mms_getaes_en & REQ_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* CLIENT: Get Alarm Enrollment Summary Response */");
  fprintf (fp, "\n/*   m_cl_max_enrollment_summary = %d */",m_cl_max_enrollment_summary);

  max_enroll_sums = mmsdec_msglen/8;
  if (m_cl_max_enrollment_summary && (max_enroll_sums > m_cl_max_enrollment_summary))
    max_enroll_sums = m_cl_max_enrollment_summary;
  rsp_size = sizeof(GETAES_RESP_INFO) +
		(max_enroll_sums * (sizeof(ALARM_ENROLL_SUMMARY)));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  fprintf (fp, "\n/*   Allows for %d Enrollment Summaries */", max_enroll_sums);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  fprintf (fp, "\n  m_cl_max_enrollment_summary = %d;", max_enroll_sums);
  }

/* ****** CLIENT: rq_rsent.c */
if (my_mms_rep_semstat_en & REQ_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* CLIENT: Report Semaphore Entry */");
  fprintf (fp, "\n/*   m_cl_max_semaphore_entries = %d */",m_cl_max_semaphore_entries);

  max_sem_entries = mmsdec_msglen / 6;
  if (m_cl_max_semaphore_entries && (max_sem_entries > m_cl_max_semaphore_entries))
    max_sem_entries = m_cl_max_semaphore_entries;
  rsp_size = sizeof(RSENTRY_RESP_INFO)
		+ (sizeof(SEMAPHORE_ENTRY) * max_sem_entries);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  fprintf (fp, "\n/*   Allows for %d Semaphore Entries */", max_sem_entries);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  fprintf (fp, "\n  m_cl_max_semaphore_entries = %d;", max_sem_entries);
  }

/* ****** CLIENT: rq_rspoo.c */
if (my_mms_rep_sempool_en & REQ_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* CLIENT: Report Pool Semaphore Status */");
  fprintf (fp, "\n/*   m_cl_max_named_tokens = %d */",m_cl_max_named_tokens);

  max_named_tokens = mmsdec_msglen / 2;
  if (m_cl_max_named_tokens && (max_named_tokens > m_cl_max_named_tokens))
    max_named_tokens = m_cl_max_named_tokens;
  rsp_size = sizeof(RSPOOL_RESP_INFO) + (sizeof(TOKEN_ID) * max_named_tokens);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  fprintf (fp, "\n/*   Allows for %d Tokens */", max_named_tokens);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  fprintf (fp, "\n  m_cl_max_named_tokens = %d;", max_named_tokens);
  }

/* ****** CLIENT: rq_altee.c */
if (my_mms_altee_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Alter Event Enrollment Response */");

  rsp_size = sizeof (ALTEE_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_delea.c */
if (my_mms_delea_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Delete Event Action Response */");

  rsp_size = sizeof (DELEA_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_delec.c */
if (my_mms_delec_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Delete Event Condition Response */");

  rsp_size = sizeof (DELEC_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_delee.c */
if (my_mms_delee_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Delete Event Enrollment Response */");

  rsp_size = sizeof (DELEE_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_delty.c */
if (my_mms_deltype_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Delete Type Response */");

  rsp_size = sizeof (DELTYPE_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }


/* ****** CLIENT: rq_delva.c */
if (my_mms_delvar_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Delete Variable Access Response */");

  rsp_size = sizeof (DELVAR_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_delvl.c */
if (my_mms_delvlist_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Named Variable List Response */");

  rsp_size = sizeof (DELVLIST_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }


/* ****** CLIENT: rq_downl.c */
if (my_mms_dwn_load_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Download Segment Response */");

  rsp_size = sizeof (DOWNLOAD_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }


/* ****** CLIENT: rq_fopen.c */
if (my_mms_fopen_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: File Open Response */");

  rsp_size = sizeof (FOPEN_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_fread.c */
if (my_mms_fread_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: File Read Response */");

  rsp_size = sizeof (FREAD_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }


/* ****** CLIENT: rq_getec.c */
if (my_mms_geteca_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Get Event Condition Attributes Response */");

  rsp_size = sizeof (GETECA_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_getsc.c */
if (my_mms_getscat_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Get Scattered Access Attributes Response */");

  rsp_size = sizeof (GETSCAT_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_getty.c */
if (my_mms_gettype_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Get Named Type Attributes Response */");

  rsp_size = sizeof (GETTYPE_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_getva.c */
if (my_mms_getvar_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Get Variable Access Attributes Response */");

  rsp_size = sizeof (GETVAR_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_ident.c */
if (my_mms_ident_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Identify Response */");

  rsp_size = sizeof (IDENT_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_input.c */
if (my_mms_input_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Input Response */");

  rsp_size = sizeof (INPUT_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_jinit.c */
if (my_mms_jinit_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Initialize Journal Response */");

  rsp_size = sizeof (JINIT_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_jstat.c */
if (my_mms_jstat_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Report journal status Response */");

  rsp_size = sizeof (JSTAT_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_repea.c */
if (my_mms_repeas_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Report Event Action status Response */");

  rsp_size = sizeof (REPEAS_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_repec.c */
if (my_mms_repecs_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Report Event Condition status Response */");

  rsp_size = sizeof (REPECS_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_repee.c */
if (my_mms_repees_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Report Event Enrollment status Response */");

  rsp_size = sizeof (REPEES_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_rssta.c */
if (my_mms_rep_semstat_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Semaphore Status Response */");

  rsp_size = sizeof (RSSTAT_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_takec.c */
if (my_mms_takectrl_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Take Control Response */");

  rsp_size = sizeof (TAKECTRL_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }

/* ****** CLIENT: rq_uploa.c */
if (my_mms_up_load_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* CLIENT: Upload Segment Response */");

  rsp_size = sizeof (UPLOAD_RESP_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", rsp_size);
  if (rsp_size > max_dec_size)
    max_dec_size = rsp_size;
  }
  

/************************************************************************/
/************************************************************************/
/* SERVER OPERATIONS */
/************************************************************************/

/* ****** SERVER: rs_read.c */
if (my_mms_read_en & RESP_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* SERVER: Read */");
  fprintf (fp, "\n/*   m_sv_max_read_var_spec = %d */",m_sv_max_read_var_spec);

  max_vars     = mmsdec_msglen / 4;
  if (m_sv_max_read_var_spec && (max_vars > m_sv_max_read_var_spec))
    max_vars = m_sv_max_read_var_spec;
  req_size = sizeof(READ_REQ_INFO) + (sizeof(VARIABLE_LIST) * max_vars);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Variables (list of variables) */", max_vars);
  fprintf (fp, "\n/*   Need %d per variable */", sizeof(VARIABLE_LIST));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_read_var_spec = %d;", max_vars);
  }

/* ****** SERVER: rs_write.c */
if (my_mms_write_en & RESP_EN)
  {
/* remove : here are two control variables and we are using algorithm 2 */
/* i.e. rsp_size = max(size1,size2)					*/
  fprintf (fp, "\n\n/* SERVER: Write */");
  fprintf (fp, "\n/*   m_sv_max_write_var_spec = %d */",m_sv_max_write_var_spec);
  fprintf (fp, "\n/*   m_sv_max_write_data = %d */", m_sv_max_write_data);

/* First assume a list of variables (each var has a data) */
  max_vars	 = mmsdec_msglen / 7;
  if (m_sv_max_write_var_spec && (max_vars > m_sv_max_write_var_spec))
    max_vars = m_sv_max_write_var_spec;

  size1 = sizeof(WRITE_REQ_INFO) + 
          ((sizeof(VARIABLE_LIST) + sizeof(VAR_ACC_DATA)) * max_vars);

/* Now assume a named variable list name */
  max_data_count = mmsdec_msglen / 3;
  if (m_sv_max_write_data && (max_data_count > m_sv_max_write_data))
    max_data_count = m_sv_max_write_data;
  size2 = sizeof(WRITE_REQ_INFO) + sizeof(VARIABLE_LIST) + 
           (sizeof(VAR_ACC_DATA) * max_data_count);

  req_size = size1 > size2 ? size1 : size2;
  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Variables (list of variables) size1: %d */", 
						max_vars, size1);
  fprintf (fp, "\n/*   Need %d per variable */", sizeof(VARIABLE_LIST));
  fprintf (fp, "\n/*   Allows for %d Data (named variable list) size2: %d */", 
						max_data_count, size2);
  fprintf (fp, "\n/*   Need %d per data */", sizeof(VAR_ACC_DATA));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_write_var_spec = %d;", max_vars);
  fprintf (fp, "\n  m_sv_max_write_data = %d;", max_data_count);
  }



/* ****** SERVER: rs_crepi.c */
if (my_mms_cre_pi_en & RESP_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* SERVER: Create Program Invokation (rs_crepi.c) */");
  fprintf (fp, "\n/*   m_sv_max_vstr = %d */",m_sv_max_vstr);

  max_vstr = mmsdec_msglen/2;
  if (m_sv_max_vstr && (max_vstr > m_sv_max_vstr))
    max_vstr = m_sv_max_vstr;
  req_size = sizeof(CREPI_REQ_INFO) + (sizeof(ST_CHAR *)*max_vstr);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Vstrings */", max_vstr);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_vstr = %d;", max_vstr);
  }

/* ****** SERVER: rs_loadd.c */
if (my_mms_load_dom_en & RESP_EN)
  {
/* remove : max_capabilities not used. check in rq_fdir.c */
  fprintf (fp, "\n\n/* SERVER: Load Domain */");
  fprintf (fp, "\n/*   m_sv_max_loadd_cap = %d */",m_sv_max_loadd_cap);

  max_capabilities = (mmsdec_msglen/2);
  max_fnames = (mmsdec_msglen/2);
  if (m_sv_max_loadd_cap && (max_capabilities > m_sv_max_loadd_cap))
    max_capabilities = m_sv_max_loadd_cap;
  if (m_sv_max_file_names && (max_fnames > m_sv_max_file_names))
    max_fnames = m_sv_max_file_names;
  req_size = sizeof(LOADDOM_REQ_INFO) + 
	     	(max_fnames * sizeof(FILE_NAME)) +
  		(max_capabilities * sizeof(ST_CHAR *));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Capabilities */", max_capabilities);
  fprintf (fp, "\n/*   Allows for %d File Names */", max_fnames);
  fprintf (fp, "\n/*   Need %d per Capability */", sizeof(ST_CHAR *));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_loadd_cap = %d;", max_capabilities);
  fprintf (fp, "\n  m_sv_max_file_names = %d;", max_fnames);
  }

/* ****** SERVER: rs_rddwn.c */
if (my_mms_rddwn_en & RESP_EN)
  {
/* remove : max_capabilities not used. check in rq_fdir.c */
  fprintf (fp, "\n\n/* SERVER: Request Download */");
  fprintf (fp, "\n/*   m_sv_max_rqdlnl_cap = %d */",m_sv_max_rqdlnl_cap);

  max_capabilities = (mmsdec_msglen/2);
  max_fnames = (mmsdec_msglen/2);
  if (m_sv_max_rqdlnl_cap && (max_capabilities > m_sv_max_rqdlnl_cap))
    max_capabilities = m_sv_max_rqdlnl_cap;
  if (m_sv_max_file_names && (max_fnames > m_sv_max_file_names))
    max_fnames = m_sv_max_file_names;
  req_size = sizeof(RDDWN_REQ_INFO) + 
	     	(max_fnames * sizeof(FILE_NAME)) +
  		(max_capabilities * sizeof(ST_CHAR *));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Capabilities */", max_capabilities);
  fprintf (fp, "\n/*   Allows for %d File Names */", max_fnames);
  fprintf (fp, "\n/*   Need %d per Capability */", sizeof(ST_CHAR *));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_rqdlnl_cap = %d;", max_capabilities);
  fprintf (fp, "\n  m_sv_max_file_names = %d;", max_fnames);
  }

/* ****** SERVER: rs_rdupl.c */
if (my_mms_rdupl_en & RESP_EN)
  {
/* remove : max_fnames used again */
  fprintf (fp, "\n\n/* SERVER: Request Upload (rs_rdupl.c) */");
  fprintf (fp, "\n/*   m_sv_max_file_names = %d */",m_sv_max_file_names);

  max_fnames = (mmsdec_msglen/2);
  if (m_sv_max_file_names && (max_fnames > m_sv_max_file_names))
    max_fnames = m_sv_max_file_names;
  req_size = sizeof(RDUPL_REQ_INFO) + (max_fnames * sizeof(FILE_NAME));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d File Names */", max_fnames);
  fprintf (fp, "\n/*   Need %d per File Name */",  sizeof(FILE_NAME));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_file_names = %d;", max_fnames);
  }

/* ****** SERVER: rs_store.c */
if (my_mms_str_dom_en & RESP_EN)
  {
/* remove : max_fnames used again */
  fprintf (fp, "\n\n/* SERVER: Store Domain */");
  fprintf (fp, "\n/*   m_sv_max_file_names = %d */",m_sv_max_file_names);

  max_fnames = (mmsdec_msglen/2);
  if (m_sv_max_file_names && (max_fnames > m_sv_max_file_names))
    max_fnames = m_sv_max_file_names;
  req_size = sizeof(STOREDOM_REQ_INFO)	+ (max_fnames * sizeof(FILE_NAME));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d File Names */", max_fnames);
  fprintf (fp, "\n/*   Need %d per File Name */",  sizeof(FILE_NAME));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_file_names = %d;", max_fnames);
  }

/* ****** SERVER: rs_termd.c */
if (my_mms_term_dwn_en & RESP_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* SERVER: Terminate Download */");
  req_size = (sizeof(TERMDOWN_REQ_INFO) + sizeof(ERR_INFO));
  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_delty.c */
if (my_mms_deltype_en & RESP_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* SERVER: Delete Type */");
  fprintf (fp, "\n/*   m_sv_max_obj_name = %d */",m_sv_max_obj_name);

  max_obj_name = mmsdec_msglen / 2;
  if (m_sv_max_obj_name && (max_obj_name > m_sv_max_obj_name))
    max_obj_name = m_sv_max_obj_name;
  req_size = sizeof(DELTYPE_REQ_INFO) + (sizeof(OBJECT_NAME) * max_obj_name);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Object Names */", max_obj_name);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_obj_name = %d;", max_obj_name);
  }

/* ****** SERVER: rs_fdir.c */
if (my_mms_fdir_en & RESP_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* SERVER: File Directory */");
  fprintf (fp, "\n/*   m_sv_max_file_names = %d */",m_sv_max_file_names);

  max_fnames = (mmsdec_msglen/2);
  if (m_sv_max_file_names && (max_fnames > m_sv_max_file_names))
    max_fnames = m_sv_max_file_names;
  req_size = sizeof(FDIR_REQ_INFO) + (max_fnames * sizeof(FILE_NAME));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d File Names */", max_fnames);
  fprintf (fp, "\n/*   Need %d per File Name */",  sizeof(FILE_NAME));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_file_names = %d;", max_fnames);
  }

/* ****** SERVER: rs_fopen.c */
if (my_mms_fopen_en & RESP_EN)
  {
/* remove : max_fnames used again */
  fprintf (fp, "\n\n/* SERVER: File Open */");
  fprintf (fp, "\n/*   m_sv_max_file_names = %d */",m_sv_max_file_names);

  max_fnames = (mmsdec_msglen/2);
  if (m_sv_max_file_names && (max_fnames > m_sv_max_file_names))
    max_fnames = m_sv_max_file_names;
  req_size = sizeof(FOPEN_REQ_INFO) + (max_fnames * (sizeof(FILE_NAME)));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d File Names */", max_fnames);
  fprintf (fp, "\n/*   Need %d per File Name */",  sizeof(FILE_NAME));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_file_names = %d;", max_fnames);
  }

/* ****** SERVER: rs_initd.c */
if (my_mms_init_dwn_en & RESP_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* SERVER: Initiate Download */");
  fprintf (fp, "\n/*   m_sv_max_initdnld_cap = %d */",m_sv_max_initdnld_cap);

  max_capabilities = (mmsdec_msglen/2);
  if (m_sv_max_initdnld_cap && (max_capabilities > m_sv_max_initdnld_cap))
    max_capabilities = m_sv_max_initdnld_cap;
  req_size = sizeof(INITDOWN_REQ_INFO)	+ (max_capabilities * sizeof(ST_CHAR *));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Capabilities */", max_capabilities);
  fprintf (fp, "\n/*   Need %d per Capability */", sizeof(ST_CHAR *));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_initdnld_cap = %d;", max_capabilities);
  }

/* ****** SERVER: rs_input.c */
if (my_mms_input_en & RESP_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* SERVER: Input */");
  fprintf (fp, "\n/*   m_sv_max_prompt_count = %d */",m_sv_max_prompt_count);

  max_prompt_count = mmsdec_msglen / 2;
  if (m_sv_max_prompt_count && (max_prompt_count > m_sv_max_prompt_count))
    max_prompt_count = m_sv_max_prompt_count;
  req_size = sizeof(INPUT_REQ_INFO) + (sizeof(ST_CHAR *) * max_prompt_count);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Prompts */", max_prompt_count);
  fprintf (fp, "\n/*   Need %d per Prompt */", sizeof(ST_CHAR *));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_prompt_count = %d;", max_prompt_count);
  }

/* ****** SERVER: rs_defvl.c */
if (my_mms_defvlist_en & RESP_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* SERVER: Define Variable List (rs_defvl.c) */");
  fprintf (fp, "\n/*   m_sv_max_num_vars = %d */",m_sv_max_num_vars);

  max_num_vars = mmsdec_msglen / 2;
  if (m_sv_max_num_vars && (max_num_vars > m_sv_max_num_vars))
    max_num_vars = m_sv_max_num_vars;
  req_size = sizeof(DEFVLIST_REQ_INFO) +
		(sizeof(VARIABLE_LIST) * max_num_vars);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d variables in the list to be defined */", max_num_vars);
  fprintf (fp, "\n/*   Need %d per Variable in the list */", sizeof(VARIABLE_LIST));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_num_vars = %d;", max_num_vars);
  }
  
/* ****** SERVER: rs_fdele.c */
if (my_mms_fdelete_en & RESP_EN)
  {
/* remove : max_fnames used again */
  fprintf (fp, "\n\n/* SERVER: File Delete (rs_fdele.c) */");
  fprintf (fp, "\n/*   m_sv_max_file_names = %d */",m_sv_max_file_names);

  max_fnames = (mmsdec_msglen/2);
  if (m_sv_max_file_names && (max_fnames > m_sv_max_file_names))
    max_fnames = m_sv_max_file_names;
  req_size = sizeof(FDELETE_REQ_INFO) + (max_fnames * sizeof(FILE_NAME));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d File Names */", max_fnames);
  fprintf (fp, "\n/*   Need %d per File Name */",  sizeof(FILE_NAME));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_file_names = %d;", max_fnames);
  }

/* ****** SERVER: rs_frena.c */
if (my_mms_frename_en & RESP_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* SERVER: File Rename (rs_frena.c) */");
  fprintf (fp, "\n/*   m_sv_max_file_names = %d */",m_sv_max_file_names);

  max_fnames = (mmsdec_msglen/2);
  if (m_sv_max_file_names && (max_fnames > m_sv_max_file_names))
    max_fnames = m_sv_max_file_names;
  req_size = sizeof(FRENAME_REQ_INFO) + (max_fnames * sizeof(FILE_NAME));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d File Names */", max_fnames);
  fprintf (fp, "\n/*   Need %d per File Name */",  sizeof(FILE_NAME));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_file_names = %d;", max_fnames);
  }

/* ****** SERVER: rs_obtfi.c */
if (my_mms_obtainfile_en & RESP_EN)
  {
/* remove : max_fnames used again */
  fprintf (fp, "\n\n/* SERVER: Obtain File (rs_obtfi.c) */");
  fprintf (fp, "\n/*   m_sv_max_file_names = %d */",m_sv_max_file_names);

  max_fnames = (mmsdec_msglen/2);
  if (m_sv_max_file_names && (max_fnames > m_sv_max_file_names))
    max_fnames = m_sv_max_file_names;
  req_size = sizeof(OBTFILE_REQ_INFO) + (max_fnames * sizeof(FILE_NAME));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d File Names */", max_fnames);
  fprintf (fp, "\n/*   Need %d per File Name */",  sizeof(FILE_NAME));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_file_names = %d;", max_fnames);
  }

/* ****** SERVER: rs_outpu.c */
if (my_mms_output_en & RESP_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* SERVER: Output */");
  fprintf (fp, "\n/*   m_sv_max_data_count = %d */",m_sv_max_data_count);

  max_data_count = mmsdec_msglen / 2;
  if (m_sv_max_data_count && (max_data_count > m_sv_max_data_count))
    max_data_count = m_sv_max_data_count;
  req_size = sizeof(OUTPUT_REQ_INFO)  + (sizeof(ST_CHAR *) * max_data_count);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Output Data */", max_data_count);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_data_count = %d;", max_data_count);
  }


/* ****** SERVER: rs_delea.c */
if (my_mms_delea_en & RESP_EN)
  {
/* remove : max_obj_name used again */
  fprintf (fp, "\n\n/* SERVER: Delete Event Action (rs_delea.c) */");
  fprintf (fp, "\n/*   m_sv_max_obj_name = %d */",m_sv_max_obj_name);

  max_obj_name = mmsdec_msglen / 2;
  if (m_sv_max_obj_name && (max_obj_name > m_sv_max_obj_name))
    max_obj_name = m_sv_max_obj_name;
  req_size = sizeof(DELEA_REQ_INFO) + (sizeof(OBJECT_NAME) * max_obj_name);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Object Names */", max_obj_name);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_obj_name = %d;", max_obj_name);
  }

/* ****** SERVER: rs_delec.c */
if (my_mms_delec_en & RESP_EN)
  {
/* remove : max_obj_name used again */
  fprintf (fp, "\n\n/* SERVER: Delete Event Condition (rs_delec.c) */");
  fprintf (fp, "\n/*   m_sv_max_obj_name = %d */",m_sv_max_obj_name);

  max_obj_name = mmsdec_msglen / 2;
  if (m_sv_max_obj_name && (max_obj_name > m_sv_max_obj_name))
    max_obj_name = m_sv_max_obj_name;
  req_size = sizeof(DELEC_REQ_INFO) + (sizeof(OBJECT_NAME) * max_obj_name);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Object Names */", max_obj_name);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_obj_name = %d;", max_obj_name);
  }

/* ****** SERVER: rs_delee.c */
if (my_mms_delee_en & RESP_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* SERVER: Delete Event Enrollment (rs_delee.c) */");
  fprintf (fp, "\n/*   m_sv_max_obj_name = %d */",m_sv_max_obj_name);

  max_obj_name = mmsdec_msglen / 2;
  if (m_sv_max_obj_name && (max_obj_name > m_sv_max_obj_name))
    max_obj_name = m_sv_max_obj_name;
  req_size = sizeof(DELEE_REQ_INFO) + (sizeof(OBJECT_NAME) * max_obj_name);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Object Names */", max_obj_name);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_obj_name = %d;", max_obj_name);
  }

/* ****** SERVER: rs_jwrit.c */
if (my_mms_jwrite_en & RESP_EN)
  {
/* remove : here are again two control variables			*/
/* we are using algorithm1 i.e. each control var multiplied by its struct */
  fprintf (fp, "\n\n/* SERVER: Journal Write */");
  fprintf (fp, "\n/*   m_sv_max_entry_content = %d */",m_sv_max_entry_content);
  fprintf (fp, "\n/*   m_sv_max_data_vars = %d */",m_sv_max_data_vars);

  max_entries	= (mmsdec_msglen/4);
  max_data_vars = (mmsdec_msglen/6);
  if (m_sv_max_entry_content && (max_entries > m_sv_max_entry_content))
    max_entries = m_sv_max_entry_content;
  if (m_sv_max_data_vars && (max_data_vars > m_sv_max_data_vars))
    max_data_vars = m_sv_max_data_vars;
  req_size = sizeof(JWRITE_REQ_INFO)
		+ (max_entries * sizeof(ENTRY_CONTENT))
		+ (max_data_vars * sizeof(VAR_INFO));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_entry_content = %d;", max_entries);
  fprintf (fp, "\n  m_sv_max_data_vars = %d;", max_data_vars);
  }

/* ****** SERVER: rs_jread.c */
if (my_mms_jread_en & RESP_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* SERVER: Journal Read */");
  fprintf (fp, "\n/*   m_sv_max_jread_vars = %d */",m_sv_max_jread_vars);

  max_vars = (mmsdec_msglen/2);
  if (m_sv_max_jread_vars && (max_vars > m_sv_max_jread_vars))
    max_vars = m_sv_max_jread_vars;
  req_size = sizeof(JREAD_REQ_INFO) + (max_vars * sizeof(ST_CHAR *));

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Variables */", max_vars);
  fprintf (fp, "\n/*   Need %d per variable */", sizeof(VARIABLE_LIST));
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_jread_vars = %d;", max_vars);
  }

/* ****** SERVER: rs_getee.c */
if (my_mms_geteea_en & RESP_EN)
  {
/* remove : done */
  fprintf (fp, "\n\n/* SERVER: Get Event Enrollment */");
  fprintf (fp, "\n/*   m_sv_max_obj_name = %d */",m_sv_max_obj_name);

  max_obj_name = mmsdec_msglen / 2;
  if (m_sv_max_obj_name && (max_obj_name > m_sv_max_obj_name))
    max_obj_name = m_sv_max_obj_name;
  req_size = sizeof(GETEEA_REQ_INFO) + (sizeof(OBJECT_NAME) * max_obj_name);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  fprintf (fp, "\n/*   Allows for %d Object Names */", max_obj_name);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  fprintf (fp, "\n  m_sv_max_obj_name = %d;", max_obj_name);
  }

/* ****** SERVER: rs_ackev.c */
if (my_mms_ackevnot_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Acknowledge Event Notification Request */");

  req_size = sizeof (ACKEVNOT_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_altec.c */
if (my_mms_altecm_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Alter Event Condition Monitoring Request */");

  req_size = sizeof (ALTECM_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_altee.c */
if (my_mms_altee_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Alter Event Enrollment Request */");

  req_size = sizeof (ALTEE_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_defec.c */
if (my_mms_defec_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Define Event Condition Request */");

  req_size = sizeof (DEFEC_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_defee.c */
if (my_mms_defee_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Define Event Enrollment Request */");

  req_size = sizeof (DEFEE_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_defsc.c */
if (my_mms_defscat_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Define Scattered Access Request */");

  req_size = sizeof (DEFSCAT_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_defse.c */
if (my_mms_define_sem_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Define Semaphore Request */");

  req_size = sizeof (DEFSEM_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_defty.c */
if (my_mms_deftype_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Define Named Type Request */");

  req_size = sizeof (DEFTYPE_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_defva.c */
if (my_mms_defvar_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Define Named Variable Request */");

  req_size = sizeof (DEFVAR_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_deldo.c */
if (my_mms_del_dom_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Delete Domain Request */");

  req_size = sizeof (DELDOM_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_delpi.c */
if (my_mms_del_pi_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Delete Program Invocation Request */");

  req_size = sizeof (DELPI_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_delse.c */
if (my_mms_delete_sem_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Delete Semaphore Request */");

  req_size = sizeof (DELSEM_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_downl.c */
if (my_mms_dwn_load_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Download Segment Request */");

  req_size = sizeof (DOWNLOAD_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_evnot.c */
if (my_mms_evnot_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Event Notification Request */");

  req_size = sizeof (EVNOT_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_fclos.c */
if (my_mms_fclose_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: File Close Request */");

  req_size = sizeof (FCLOSE_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_fread.c */
if (my_mms_fread_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: File Read Request */");

  req_size = sizeof (FREAD_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_getae.c */
if (my_mms_getaes_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Get Alarm Enrollment Summary Request */");

  req_size = sizeof (GETAES_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_getas.c */
if (my_mms_getas_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Get Alarm Summary Request */");

  req_size = sizeof (GETAS_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_getcl.c */
if (my_mms_getcl_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Get Capability List Request */");

  req_size = sizeof (GETCL_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_getdo.c */
if (my_mms_get_dom_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Get Domain Attribute Request */");

  req_size = sizeof (GETDOM_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_getea.c */
if (my_mms_geteaa_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Get Event Action Attributes Request */");

  req_size = sizeof (GETEAA_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_getec.c */
if (my_mms_geteca_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Get Event Condition Attributes Request */");

  req_size = sizeof (GETECA_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_getpi.c */
if (my_mms_get_pi_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Get Program Invocation Attribute Request */");

  req_size = sizeof (GETPI_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_getsc.c */
if (my_mms_getscat_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Get Scattered Access Attributes Request */");

  req_size = sizeof (GETSCAT_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_getty.c */
if (my_mms_gettype_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Get Named Type Attributes Request */");

  req_size = sizeof (GETTYPE_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_getva.c */
if (my_mms_getvar_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Get Variable Access Attributes Request */");

  req_size = sizeof (GETVAR_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_getvl.c */
if (my_mms_getvlist_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Get Named Variable List Attributes Request */");

  req_size = sizeof (GETVLIST_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_initu.c */
if (my_mms_init_upl_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Initiate Upload Sequence Request */");

  req_size = sizeof (INITUPL_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_jcrea.c */
if (my_mms_jcreate_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Create Journal Request */");

  req_size = sizeof (JCREATE_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_jdele.c */
if (my_mms_jdelete_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Delete Journal Request */");

  req_size = sizeof (JDELETE_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_jinit.c */
if (my_mms_jinit_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Initialize Journal Request */");

  req_size = sizeof (JINIT_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_jstat.c */
if (my_mms_jstat_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Report Journal Status Request */");

  req_size = sizeof (JSTAT_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_kill.c */
if (my_mms_kill_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Kill Request */");

  req_size = sizeof (KILL_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_namel.c */
if (my_mms_getnames_en & REQ_EN)
  {
  fprintf (fp, "\n\n/* SERVER: GetNameList Request */");

  req_size = sizeof (NAMELIST_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_relct.c */
if (my_mms_relctrl_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Relinquish Control Request */");

  req_size = sizeof (RELCTRL_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_renam.c */
if (my_mms_rename_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Rename Request */");

  req_size = sizeof (RENAME_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_repea.c */
if (my_mms_repeas_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Report Event Action Status Request */");

  req_size = sizeof (REPEAS_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_repec.c */
if (my_mms_repecs_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Report Event Condition Status Request */");

  req_size = sizeof (REPECS_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_repee.c */
if (my_mms_repees_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Report Event Enrollment Status Request */");

  req_size = sizeof (REPEES_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_reset.c */
if (my_mms_reset_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Reset Request */");

  req_size = sizeof (RESET_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_resum.c */
if (my_mms_resume_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Resume Request */");

  req_size = sizeof (RESUME_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_rsent.c */
if (my_mms_rep_sementry_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Report Semaphore Entry Status Request */");

  req_size = sizeof (RSENTRY_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_poo.c */
if (my_mms_rep_sempool_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Report Pool Semaphore Status Request */");

  req_size = sizeof (RSPOOL_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_rssta.c */
if (my_mms_rep_semstat_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Report Semaphore Status Request */");

  req_size = sizeof (RSSTAT_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_start.c */
if (my_mms_start_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Start Request */");

  req_size = sizeof (START_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_stat.c */
if (my_mms_status_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Decode status Request */");

  req_size = sizeof (STATUS_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_stop.c */
if (my_mms_stop_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Stop Request */");

  req_size = sizeof (STOP_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_termu.c */
if (my_mms_term_upl_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Terminate Upload Sequence Request */");

  req_size = sizeof (TERMUPL_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_trige.c */
if (my_mms_trige_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Trigger Event Request */");

  req_size = sizeof (TRIGE_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

/* ****** SERVER: rs_uploa.c */
if (my_mms_up_load_en & RESP_EN)
  {
  fprintf (fp, "\n\n/* SERVER: Upload Segment Request */");

  req_size = sizeof (UPLOAD_REQ_INFO);

  fprintf (fp, "\n/*   Maximum required decode buffer size : %u */", req_size);
  if (req_size > max_dec_size)
    max_dec_size = req_size;
  }

  write_c_trailer (fp);
  fclose (fp);

  _write_smemcfg ();
  return (0);
  }


/************************************************************************/
/************************************************************************/

static ST_VOID cfg_mmsdec_msglen (ST_VOID)
  {
  cfg_get_ushort (&mmsdec_msglen);
  }
static ST_VOID cfg_m_cl_read_spec_in_result (ST_VOID)
  {
  cfg_get_int (&m_cl_read_spec_in_result);
  }
static ST_VOID cfg_m_cl_max_read_var_spec (ST_VOID)
  {
  cfg_get_int (&m_cl_max_read_var_spec);
  }

static ST_VOID cfg_m_cl_max_read_acc_rslt (ST_VOID)
  {
  cfg_get_int (&m_cl_max_read_acc_rslt);
  }

static ST_VOID cfg_m_cl_max_info_var_spec (ST_VOID)
  {
  cfg_get_int (&m_cl_max_info_var_spec);
  }

static ST_VOID cfg_m_cl_max_info_acc_rslt (ST_VOID)
  {
  cfg_get_int (&m_cl_max_info_acc_rslt);
  }

static ST_VOID cfg_m_cl_max_getvla_vars (ST_VOID)
  {
  cfg_get_int (&m_cl_max_getvla_vars);
  }

static ST_VOID cfg_m_cl_max_write_rslts (ST_VOID)
  {
  cfg_get_int (&m_cl_max_write_rslts);
  }

static ST_VOID cfg_m_cl_max_namel_names (ST_VOID)
  {
  cfg_get_int (&m_cl_max_namel_names);
  }

static ST_VOID cfg_m_cl_max_getcl_cap (ST_VOID)
  {
  cfg_get_int (&m_cl_max_getcl_cap);
  }

static ST_VOID cfg_m_cl_max_getpi_doms (ST_VOID)
  {
  cfg_get_int (&m_cl_max_getpi_doms);
  }

static ST_VOID cfg_m_cl_max_getdom_cap (ST_VOID)
  {
  cfg_get_int (&m_cl_max_getdom_cap);
  }

static ST_VOID cfg_m_cl_max_getdom_pi (ST_VOID)
  {
  cfg_get_int (&m_cl_max_getdom_pi);
  }

static ST_VOID cfg_m_cl_max_initupl_cap (ST_VOID)
  {
  cfg_get_int (&m_cl_max_initupl_cap);
  }

static ST_VOID cfg_m_cl_max_alarm_summary (ST_VOID)
  {
  cfg_get_int (&m_cl_max_alarm_summary);
  }

static ST_VOID cfg_m_cl_max_journal_entries (ST_VOID)
  {
  cfg_get_int (&m_cl_max_journal_entries);
  }

static ST_VOID cfg_m_cl_max_entry_content (ST_VOID)
  {
  cfg_get_int (&m_cl_max_entry_content);
  }

static ST_VOID cfg_m_cl_max_semaphore_entries (ST_VOID)
  {
  cfg_get_int (&m_cl_max_semaphore_entries);
  }

static ST_VOID cfg_m_cl_max_named_tokens (ST_VOID)
  {
  cfg_get_int (&m_cl_max_named_tokens);
  }

static ST_VOID cfg_m_cl_max_file_directory (ST_VOID)
  {
  cfg_get_int (&m_cl_max_file_directory);
  }

static ST_VOID cfg_m_cl_max_event_attributes (ST_VOID)
  {
  cfg_get_int (&m_cl_max_event_attributes);
  }

static ST_VOID cfg_m_cl_max_event_enrollment (ST_VOID)
  {
  cfg_get_int (&m_cl_max_event_enrollment);
  }

static ST_VOID cfg_m_cl_max_enrollment_summary (ST_VOID)
  {
  cfg_get_int (&m_cl_max_enrollment_summary);
  }

static ST_VOID cfg_m_sv_max_loadd_cap (ST_VOID)
  {
  cfg_get_int (&m_sv_max_loadd_cap);
  }

static ST_VOID cfg_m_sv_max_read_var_spec (ST_VOID)
  {
  cfg_get_int (&m_sv_max_read_var_spec);
  }

static ST_VOID cfg_m_sv_max_write_var_spec (ST_VOID)
  {
  cfg_get_int (&m_sv_max_write_var_spec);
  }

static ST_VOID cfg_m_sv_max_write_data (ST_VOID)
  {
  cfg_get_int (&m_sv_max_write_data);
  }

static ST_VOID cfg_m_sv_max_rqdlnl_cap (ST_VOID)
  {
  cfg_get_int (&m_sv_max_rqdlnl_cap);
  }

static ST_VOID cfg_m_sv_max_initdnld_cap (ST_VOID)
  {
  cfg_get_int (&m_sv_max_initdnld_cap);
  }
static ST_VOID cfg_m_sv_max_vstr (ST_VOID)
  {
  cfg_get_int (&m_sv_max_vstr);
  }

static ST_VOID cfg_m_sv_max_file_names (ST_VOID)
  {
  cfg_get_int (&m_sv_max_file_names);
  }

static ST_VOID cfg_m_sv_max_obj_name (ST_VOID)
  {
  cfg_get_int (&m_sv_max_obj_name);
  }

static ST_VOID cfg_m_sv_max_prompt_count (ST_VOID)
  {
  cfg_get_int (&m_sv_max_prompt_count);
  }

static ST_VOID cfg_m_sv_max_num_vars (ST_VOID)
  {
  cfg_get_int (&m_sv_max_num_vars);
  }

static ST_VOID cfg_m_sv_max_data_count (ST_VOID)
  {
  cfg_get_int (&m_sv_max_data_count);
  }

static ST_VOID cfg_m_sv_max_entry_content (ST_VOID)
  {
  cfg_get_int (&m_sv_max_entry_content);
  }

static ST_VOID cfg_m_sv_max_data_vars (ST_VOID)
  {
  cfg_get_int (&m_sv_max_data_vars);
  }

static ST_VOID cfg_m_sv_max_jread_vars (ST_VOID)
  {
  cfg_get_int (&m_sv_max_jread_vars);
  }


static ST_VOID cfg_ind_mode (ST_VOID)
  {
ST_CHAR *str;

  str = cfg_get_string_ptr ();
  if (str != NULL)
    {
    if (!strcmp (str, "immediate"))
      ind_mode = IND_MODE_IMMEDIATE;
    else if (!strcmp (str, "asynchronous"))
      ind_mode = IND_MODE_ASYNC;
    }
  }

static ST_VOID cfg_conf_mode (ST_VOID)
  {
ST_CHAR *str;

  str = cfg_get_string_ptr ();
  if (str != NULL)
    {
    if (!strcmp (str, "immediate"))
      conf_mode = CONF_MODE_IMMEDIATE;
    else if (!strcmp (str, "asynchronous"))
      conf_mode = CONF_MODE_ASYNC;
    }
  }

static ST_VOID cfg_max_req_outstanding (ST_VOID)
  {
  cfg_get_int (&max_req_outstanding);
  }

static ST_VOID cfg_uca_support (ST_VOID)
  {
ST_CHAR *str;

  str = cfg_get_string_ptr ();
  if (str != NULL)
    {
    if (!strcmp (str, "yes"))
      uca_support = SD_TRUE;
    else if (!strcmp (str, "no"))
      uca_support = SD_FALSE;
    }
  }

static ST_VOID cfg_uca_data_mode (ST_VOID)
  {
ST_CHAR *str;

  str = cfg_get_string_ptr ();
  if (str != NULL)
    {
    if (!strcmp (str, "static"))
      uca_data_mode = UCA_DATA_MODE_STATIC;
    else if (!strcmp (str, "dynamic"))
      uca_data_mode = UCA_DATA_MODE_DYNAMIC;
    }
  }

static ST_VOID cfg_uca_array_vars (ST_VOID)
  {
ST_CHAR *str;

  str = cfg_get_string_ptr ();
  if (str != NULL)
    {
    if (!strcmp (str, "yes"))
      uca_array_vars = SD_TRUE;
    else if (!strcmp (str, "no"))
      uca_array_vars = SD_FALSE;
    }
  }

static ST_VOID cfg_uca_arr_alt_acc (ST_VOID)
  {
ST_CHAR *str;

  str = cfg_get_string_ptr ();
  if (str != NULL)
    {
    if (!strcmp (str, "yes"))
      uca_arr_alt_acc = SD_TRUE;
    else if (!strcmp (str, "no"))
      uca_arr_alt_acc = SD_FALSE;
    }
  }
static ST_VOID cfg_client_alt_acc (ST_VOID)
  {
ST_CHAR *str;

  str = cfg_get_string_ptr ();
  if (str != NULL)
    {
    if (!strcmp (str, "yes"))
      client_alt_acc = SD_TRUE;
    else if (!strcmp (str, "no"))
      client_alt_acc = SD_FALSE;
    }
  }

static ST_VOID cfg_begin (ST_VOID)
  {
  }
static ST_VOID cfg_end (ST_VOID) 
  {
  cfg_set_endfile_ok ();	/* this is the end of configuration	*/
  }

/************************************************************************/
/************************************************************************/
/************************************************************************/
/*      write_c_header                                                  */
/************************************************************************/

ST_VOID write_c_header (FILE *fp)
  {
time_t rightnow;
ST_INT i;

  rightnow = time (NULL);
  fprintf (fp, "/*\tThis file created %s", ctime (&rightnow));
  fprintf (fp, "*/\n\n");
  fprintf (fp, "#include \"glbtypes.h\"\n");
  fprintf (fp, "#include \"sysincs.h\"\n");
  fprintf (fp, "#include \"mmsdefs.h\"\n");
  fprintf (fp, "#include \"mmsldefs.h\"\n");
  fprintf (fp, "#include \"mvl_defs.h\"\n");

  fprintf (fp, "\n#ifdef DEBUG_SISCO");
  fprintf (fp, "\nSD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__; ");
  fprintf (fp, "\n#endif");
  fprintf (fp, "\n\n");

  fprintf (fp, "\n\n/* MMSOP_EN.H SERVICE ENABLE DEFINES:\t*/");
  for (i = 0; i < numOpMap; ++i)
    {
    if (*mmsOpEnMap[i].addr == REQ_RESP_EN)
      fprintf (fp, "\n/* #define %-19.19s \tREQ_RESP_EN\t*/",mmsOpEnMap[i].str);
    else if (*mmsOpEnMap[i].addr == REQ_RESP_DIS)
      fprintf (fp, "\n/* #define %-19.19s \tREQ_RESP_DIS\t*/",mmsOpEnMap[i].str);
    else if (*mmsOpEnMap[i].addr == REQ_EN)
      fprintf (fp, "\n/* #define %-19.19s \tREQ_EN\t*/",mmsOpEnMap[i].str);
    else if (*mmsOpEnMap[i].addr == RESP_EN)
      fprintf (fp, "\n/* #define %-19.19s \tRESP_EN\t*/",mmsOpEnMap[i].str);
    }

  fprintf (fp, "\n\nST_VOID ml_init_buffers ()\n  {");
  /* Dummy } to fool dumb brace-matchers and match previous brace. */
  fprintf (fp, "\n#if !defined(MVL_DYN_MEM)");
  fprintf (fp, "\nST_INT op_enc_buf_size;");
  fprintf (fp, "\nST_INT size1;");
  fprintf (fp, "\n#endif");
  }



/************************************************************************/
/*      write_c_trailer                                                 */
/************************************************************************/

ST_VOID write_c_trailer (FILE *fp)
  {
  fprintf (fp, "\n\n  mmsl_max_msg_size = %d;", mmsdec_msglen);
  fprintf (fp, "\n  mmsl_enc_buf_size = %d + ASN1_MAX_ELEMENT_OVERHEAD;", 
						mmsdec_msglen);

  fprintf (fp, "\n  mmsl_dec_info_size = %u;", max_dec_size);

  fprintf (fp, "\n  mmsl_enc_buf = M_MALLOC (MSMEM_STARTUP, mmsl_enc_buf_size);");

  fprintf (fp, "\n#if !defined(MVL_DYN_MEM)");
  fprintf (fp, "\n  _mms_dec_info = chk_calloc (1, mmsl_dec_info_size);");
  fprintf (fp, "\n  mvl_asn1_enc_buf = chk_calloc (1, mmsl_max_msg_size);");
  fprintf (fp, "\n  mvl_local_write_buf = chk_calloc (1, MAX_LOCAL_DATA_SIZE);");

  fprintf (fp, "\n\n/* Here we calculate the required op specific buffer size	*/");
  fprintf (fp, "\n  op_enc_buf_size = 100;");
  fprintf (fp, "\n");
if (my_mms_read_en & RESP_EN)
  {
  fprintf (fp, "\n/* READ RESPONSE */");
  fprintf (fp, "\n  size1 = sizeof (READ_RESP_INFO) + ");
  fprintf (fp, "\n          (MVL_MAX_READ_RSLTS * sizeof (ACCESS_RESULT));");
  fprintf (fp, "\n  op_enc_buf_size = max (size1, op_enc_buf_size);");
  fprintf (fp, "\n");
  }
if (my_mms_write_en & RESP_EN)
  {
  fprintf (fp, "\n/* WRITE RESPONSE */");
  fprintf (fp, "\n  size1 = sizeof(WRITE_RESP_INFO) + ");
  fprintf (fp, "\n	  (MVL_MAX_WRITE_RSLTS * sizeof (WRITE_RESULT));");
  fprintf (fp, "\n  op_enc_buf_size = max (size1, op_enc_buf_size);");
  fprintf (fp, "\n");
  }
if (my_mms_info_en & REQ_EN)			  
  {
  fprintf (fp, "\n  /* INFO REPORT REQUEST */");
  fprintf (fp, "\n  size1 = sizeof(INFO_REQ_INFO) + (MVL_MAX_INFO_RSLTS * sizeof(ACCESS_RESULT));");
  fprintf (fp, "\n  size1 += MVL_MAX_INFO_VARS * sizeof(VARIABLE_LIST);");
  fprintf (fp, "\n  op_enc_buf_size = max (size1, op_enc_buf_size);");
  fprintf (fp, "\n");
  }
if (my_mms_getnames_en & RESP_EN)
  {
  fprintf (fp, "\n/* GET NAMELIST RESPONSE */");
  fprintf (fp, "\n  size1 = sizeof(NAMELIST_RESP_INFO) + ");
  fprintf (fp, "\n	  (MVL_MAX_GNL_NAMES * sizeof(ST_CHAR *));");
  fprintf (fp, "\n  op_enc_buf_size = max (size1, op_enc_buf_size);");
  fprintf (fp, "\n");
  }
if (my_mms_getvlist_en & RESP_EN)
  {
  fprintf (fp, "\n/* GET VLA RESPONSE */");
  fprintf (fp, "\n  size1 = sizeof(GETVLIST_RESP_INFO) + ");
  fprintf (fp, "\n	  (MVL_MAX_VLIST_VARS * sizeof(VARIABLE_LIST));");
  fprintf (fp, "\n  op_enc_buf_size = max (size1, op_enc_buf_size);");
  fprintf (fp, "\n");
  }
if (my_mms_fdir_en & RESP_EN)
  {
  fprintf (fp, "\n/* FILE DIRECTORY RESPONSE */");
  fprintf (fp, "\n  size1 = sizeof (FDIR_RESP_INFO) + ");
  fprintf (fp, "\n	  (MVL_MAX_FDIR_FNAMES * (sizeof (FDIR_DIR_ENT) + sizeof (FILE_NAME)));");
  fprintf (fp, "\n  op_enc_buf_size = max (size1, op_enc_buf_size);");
  fprintf (fp, "\n");
  }
  fprintf (fp, "\n  mvl_op_enc_buf = chk_calloc (1, op_enc_buf_size);");
  fprintf (fp, "\n#endif");

  /* Dummy { to fool dumb brace-matchers and match previous brace. */
  fprintf (fp, "\n  }\n\n");
  }


/************************************************************************/
/************************************************************************/
ST_VOID printUsage ()
  {
  printf ("\n\nUSAGE: mbufcalc [-smem -mmmsop_en] [-ccfgfile] [outfile]\n");
  }
 



/************************************************************************/
/************************************************************************/

#define MAX_OP_LINE	200

ST_RET readMmsOpEn (ST_CHAR *opFile)
  {
FILE *fp;
ST_CHAR lineBuf[MAX_OP_LINE];
ST_INT i;

  fp = fopen (opFile, "r");
  if (!fp)
    {
    printf ("\n Error: Could not open %s", opFile);
    return (SD_FAILURE);
    }

  while (fgets (lineBuf, MAX_OP_LINE, fp) != NULL)
    {
    for (i = 0; i < numOpMap; ++i)
      {
      if (strncmp (lineBuf, "#define", 7) != 0)
        continue;

      if (strstr (lineBuf, mmsOpEnMap[i].str) != NULL)
        {
        if (strstr (lineBuf, "REQ_RESP_EN") != NULL)
          {
          *mmsOpEnMap[i].addr = REQ_RESP_EN;
          }
        else if (strstr (lineBuf, "REQ_RESP_DIS") != NULL)
          {
          *mmsOpEnMap[i].addr = REQ_RESP_DIS;
          }
        else if (strstr (lineBuf, "REQ_EN") != NULL)
          {
          *mmsOpEnMap[i].addr = REQ_EN;
          }
        else if (strstr (lineBuf, "RESP_EN") != NULL)
          {
          *mmsOpEnMap[i].addr = RESP_EN;
          }
        else
          {
          printf ("\n Warning: Operation enable problem, line %d", i+1);
          printf ("\n lineBuf = %s, mmsOpEnMap[i].str = ", lineBuf, 
							    mmsOpEnMap[i].str);
          return (SD_FAILURE);
          }
        break;
        } /* if we found an op enable match */
      } /* Search for op enable define */
    }  /* While getting lines from the file */

  fclose (fp);
  return (SD_SUCCESS);
  }


/************************************************************************/
/************************************************************************/

static ST_VOID _write_smemCtxt (FILE *fp, ST_CHAR *contextName, ST_CHAR *poolName,
				ST_INT bufferSize, ST_INT numBuffers,
				ST_INT autoClone, ST_INT maxClones, ST_BOOLEAN noSize);

ST_CHAR smemOutFileName[200] = "smemcfg.xml";

ST_CHAR *smemCfgOpen =
  "<SmemCfg xmlns=\"urn:sisco-schemas:daf\" xmlns:xsi=\"http://www.w3.org/1999/XMLSchema-instance\" xsi:schemaLocation=\"urn:sisco-schemas:daf C:/mmslite/mvl/usr/uca_srvr/smemcfg.xsd\">";

ST_CHAR *smemSeperator = "\n<!-- ######### -->";

static ST_VOID _write_smemcfg ()
  {
FILE *fp;

  fp = fopen (smemOutFileName, "w");
  if (fp == NULL)
    {
    printf ("\n  Error: Could not open smem cfg file '%s'", smemOutFileName);
    exit (2);
    }

  fprintf (fp, "%s", smemCfgOpen);

  _write_smemCtxt (fp, "MSMEM_GEN", "GEN", 0, 0, SD_FALSE, 0, SD_TRUE);

  _write_smemCtxt (fp, "MSMEM_DEC_OS_INFO", "DEC_OS_INFO",
				max_dec_size, 1, SD_FALSE, 0, SD_FALSE);

//  _write_smemCtxt (fp, "MSMEM_ENC_OS_INFO", "ENC_OS_INFO",
//				ST_CHAR *bufferSize, ST_CHAR *numBuffers,
//				SD_FALSE, 0);

  _write_smemCtxt (fp, "MSMEM_WR_DATA_DEC_BUF", "WR_DATA_DEC_BUF",
				0, 0, SD_FALSE, 0, SD_TRUE);

  _write_smemCtxt (fp, "MSMEM_ASN1_DATA_ENC", "ASN1_DATA_ENC",
				mmsdec_msglen, 1, SD_FALSE, 0, SD_FALSE);

  _write_smemCtxt (fp, "MSMEM_PDU_ENC", "PDU_ENC",
				mmsdec_msglen + ASN1_MAX_ELEMENT_OVERHEAD,
				1, SD_FALSE, 0, SD_FALSE);


  if (ind_mode == IND_MODE_IMMEDIATE &&  conf_mode == CONF_MODE_IMMEDIATE)
    {
    _write_smemCtxt (fp, "MSMEM_COM_EVENT", "COM_EVENT", 
				0, 1, SD_FALSE, 0, SD_TRUE);
    _write_smemCtxt (fp, "MSMEM_RXPDU", "RXPDU",
				mmsdec_msglen, 1, SD_FALSE, 0, SD_FALSE);
    }
  else 
    {
    _write_smemCtxt (fp, "MSMEM_COM_EVENT", "COM_EVENT", 
				0, 0, SD_FALSE, 0, SD_TRUE);
    _write_smemCtxt (fp, "MSMEM_RXPDU", "RXPDU",
				0, 0, SD_FALSE, 0, SD_TRUE);
    }

  _write_smemCtxt (fp, "MSMEM_NETINFO", "NETINFO", 
				0, 1, SD_FALSE, 0, SD_TRUE);

  if (max_req_outstanding != SMEM_ITEM_UNDEFINED)
    {
    _write_smemCtxt (fp, "MSMEM_REQ_CTRL", "REQ_CTRL",
				sizeof (MVL_REQ_PEND), max_req_outstanding,
				SD_FALSE, 0, SD_FALSE);
    }
  else
    {
    _write_smemCtxt (fp, "MSMEM_REQ_CTRL", "REQ_CTRL",
				0, 0, SD_FALSE, 0, SD_TRUE);
    }

  if (ind_mode == IND_MODE_IMMEDIATE)
    {
    _write_smemCtxt (fp, "MSMEM_IND_CTRL", "IND_CTRL",
				sizeof (MVL_IND_PEND), 1,
				SD_FALSE, 0, SD_FALSE);
    }
  else
    {
    _write_smemCtxt (fp, "MSMEM_IND_CTRL", "IND_CTRL",
				sizeof (MVL_IND_PEND), 0,
				SD_FALSE, 0, SD_FALSE);
    }

  if (uca_support == SD_TRUE)
    {
    _write_smemCtxt (fp, "MSMEM_MVLU_VA", "MVLU_VA",
				sizeof (MVL_VAR_ASSOC) + (MAX_IDENT_LEN + 1),
				1, SD_FALSE, 0, SD_FALSE);

    _write_smemCtxt (fp, "MSMEM_MVLU_VA_CTRL", "MVLU_VA_CTRL",
		max (sizeof(MVLU_RD_VA_CTRL), sizeof(MVLU_WR_VA_CTRL)), 
		1,SD_FALSE, 0, SD_FALSE);


    _write_smemCtxt (fp, "MSMEM_MVLU_VA_DATA", "MVLU_VA_DATA",
				0, 0, SD_FALSE, 0, SD_TRUE);
    }

    if (uca_arr_alt_acc == SD_TRUE)
      {
      _write_smemCtxt (fp, "MSMEM_MVLU_AA", "MVLU_AA",
  				0, 0, SD_FALSE, 0, SD_TRUE);
 

    _write_smemCtxt (fp, "MSMEM_MVLU_GNL", "MVLU_GNL",
				(MAX_IDENT_LEN + 1) * MVL_MAX_GNL_NAMES,
				1, SD_FALSE, 0, SD_FALSE);

    _write_smemCtxt (fp, "MSMEM_DYN_RT", "DYN_RT",
				0, 0, SD_FALSE, 0, SD_TRUE);
    }

  if (client_alt_acc == SD_TRUE)
    {
    _write_smemCtxt (fp, "MSMEM_AA_ENCODE", "AA_ENCODE",
				0, 0, SD_FALSE, 0, SD_TRUE);
    }

  _write_smemCtxt (fp, "MSMEM_ACSE_CONN", "ACSE_CONN",
				0, 0, SD_FALSE, 0, SD_TRUE);
  _write_smemCtxt (fp, "MSMEM_ACSE_DATA", "ACSE_DATA",
				0, 0, SD_FALSE, 0, SD_TRUE);
  _write_smemCtxt (fp, "MSMEM_COSP_CN", "COSP_CN",
				0, 0, SD_FALSE, 0, SD_TRUE);
  _write_smemCtxt (fp, "MSMEM_N_UNITDATA", "N_UNITDATA",
				0, 0, SD_FALSE, 0, SD_TRUE);
  _write_smemCtxt (fp, "MSMEM_SOCK_INFO", "SOCK_INFO",
				0, 0, SD_FALSE, 0, SD_TRUE);
  _write_smemCtxt (fp, "MSMEM_SPDU_TX", "SPDU_TX",
				0, 0, SD_FALSE, 0, SD_TRUE);
  _write_smemCtxt (fp, "MSMEM_STARTUP", "STARTUP",
				0, 0, SD_FALSE, 0, SD_TRUE);
  _write_smemCtxt (fp, "MSMEM_TPKT", "TPKT",
				0, 0, SD_FALSE, 0, SD_TRUE);

  fprintf (fp, "</SmemCfg>", smemCfgOpen);
  }

/************************************************************************/

static ST_VOID _write_smemCtxt (FILE *fp, ST_CHAR *contextName, ST_CHAR *poolName,
				ST_INT bufferSize, ST_INT numBuffers,
				ST_INT autoClone, ST_INT maxClones, ST_BOOLEAN noSize)
  {
  fprintf (fp, "\n%s", smemSeperator);
  fprintf (fp, "\n  <SmemContext>");
  fprintf (fp, "\n    <ContextName>%s</ContextName>", contextName);
  fprintf (fp, "\n    <SmemPools>");
  fprintf (fp, "\n      <Pool>");
  fprintf (fp, "\n        <PoolName>%s</PoolName>", poolName);
  if (noSize)
    {
    }
  else
    {
    fprintf (fp, "\n      <BufferSize>%d</BufferSize>", bufferSize);
    fprintf (fp, "\n      <NumBuffers>%d</NumBuffers>", numBuffers);
    fprintf (fp, "\n      <AutoClone>%s</AutoClone>", autoClone ? "yes" : "no");
    fprintf (fp, "\n      <MaxClones>%d</MaxClones>", maxClones);
    }
  fprintf (fp, "\n      </Pool>");
  fprintf (fp, "\n   </SmemPools>");
  fprintf (fp, "\n</SmemContext>");
  }

