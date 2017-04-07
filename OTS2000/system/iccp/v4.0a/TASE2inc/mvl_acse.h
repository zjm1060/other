/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1986 - 1993, All Rights Reserved		        */
/*									*/
/* MODULE NAME : mvl_acse.h    						*/
/* PRODUCT(S)  : MMSEASE-LITE						*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*	NONE								*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 05/13/02  MDE     17    Added structure element comments		*/
/* 05/03/02  MDE     16    Added mvl_add_bind_ctrl, associated changes	*/
/* 02/25/02  MDE     15    More changes to MVL_CFG_INFO			*/
/* 02/20/02  MDE     14    Changes to new MVL_CFG_INFO, for mult. bind	*/
/* 02/07/02  JRB     13    Define MVL_CFG_INFO & pass to mvl_start_acse.*/
/*			   Move osicfgx proto here, add MVL_CFG_INFO arg*/
/*			   Del global var mvl_local_ar_name.		*/
/* 01/29/02  JRB     12    Replace mvl_init_cl_netinfo funct with	*/
/*			   mvl_init_audt_addr.				*/
/* 11/28/01  MDE     11    mvl_set_num_connections,mvl_set_max_msg_size */
/* 01/05/01  MDE     10    Changes for use over Marben			*/
/* 06/26/00  JRB     09    Del "#define MVL_ACSE". Not used.		*/
/*			   Move "#define COACSE" to mvl_defs.h (avoids	*/
/*			     mvl_defs.h dependence on mvl_acse.h).	*/
/* 03/09/00  MDE     08    Added _mvl_init_conn_ctrl			*/
/* 07/30/99  JRB     07    Chg ma_clbind_ctrl from static to global.	*/
/* 04/13/99  MDE     06    Added '_mvla_initiate_locDe'			*/
/* 03/18/99  MDE     05    Made connection limits runtime settable	*/
/* 11/16/98  MDE     04    Removed mvl_release, mvla_release (obsolete)	*/
/* 07/16/98  MDE     03    Addded 'mvl_dib_entry_to_app_ref'		*/
/* 06/19/98  JRB     02    Add tp_type, etc. to NET_BIND_CTRL.		*/
/* 09/10/97  MDE     01    MMS-LITE V4.0 Release			*/
/************************************************************************/

#ifndef MVL_ACSE_INCLUDED
#define MVL_ACSE_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "suicacse.h"
#include "acse2usr.h"
#include "mvl_defs.h"

/************************************************************************/
/* Some error return codes */
#define REQ_INCOMPLETE  	0x100
#define LOCAL_NAME_NOT_FOUND 	0x101
#define REMOTE_NAME_NOT_FOUND 	0x102

#define MVLE_LOCAL_NAME 	0x101
#define MVLE_REMOTE_NAME 	0x102
#define MVLE_NUM_CONNECTIONS	0x103

/************************************************************************/
/* Bind and Connection control structures				*/

struct dib_entry;   /* Forward declaration */

typedef struct net_bind_ctrl
  {
/* These parameters must be set before using in mvl_start_acse		*/
  ST_CHAR ar_name[MAX_AR_LEN+1];	
  ST_INT num_calling;
  ST_INT num_called;

/* Internal */
  ST_VOID *user_info;
#ifdef LEAN_T
  ST_INT tp_type;
  ST_INT32 copp_bind_id_tcp;
  ST_INT32 copp_bind_id_tp4;
#else
  ST_INT32 copp_bind_id;
#endif
  ST_INT32 clpp_bind_id;
  struct dib_entry *de;
  } NET_BIND_CTRL;

/************************************************************************/
/* MVL configuration information. Filled in by user or configuration	*/
/* and passed to mvl_start_acse.					*/
/************************************************************************/

typedef struct
  {
  ST_INT max_msg_size;	     	/* Max MMS message size			*/
  ST_INT num_calling;	     	/* total number of calling connections	*/
  ST_INT num_called;	     	/* total number of called connections	*/

/* Used for single local address binding (LEAN-T)			*/ 
  ST_CHAR local_ar_name[MAX_AR_LEN+1];

/* Used for multiple local address binding only (MARBEN)		*/ 
#ifdef MAP30_ACSE		/* Select bind mode */
  ST_BOOLEAN use_bind_ctrl;	/* Set to 0 for single local		*/
#endif

/* User sets these only when use_bind_ctrl is SD_TRUE (MARBEN)		*/
  ST_INT         max_bind_ctrl;	/* Max allowed				*/
  ST_INT         num_bind_ctrl;	/* Active bindings			*/
  NET_BIND_CTRL *bind_ctrl;	/* Array of NET_BIND_CTRL		*/

/* Internal use */
  ST_INT calling_used;		/* Number mvl_calling_conn_ctrl bound	*/
  ST_INT called_used;		/* Number mvl_called_conn_ctrl bound	*/
  } MVL_CFG_INFO;

/* Persistant config information; set by MVL during mvl_start_acse 	*/
extern MVL_CFG_INFO *mvl_cfg_info;

/************************************************************************/
/* Global configuration variables					*/

extern ST_CHAR *mvl_local_cl_ar_name;
extern INIT_INFO *mvl_init_resp_info;
extern NET_BIND_CTRL ma_clbind_ctrl;	/* to save clpp_bind info.	*/

#if defined(MAP30_ACSE)
extern ST_INT 	      mvl_num_bind_ctrl;
extern NET_BIND_CTRL *mvl_bind_ctrl;
#endif
ST_RET mvla_set_init_ar (ST_CHAR *ar_name);

/************************************************************************/
/* ACSE interface related MVL ACSE functions				*/

ST_VOID mvl_set_max_msg_size (ST_INT max_msg_size);

ST_RET osicfgx (ST_CHAR *xml_filename, MVL_CFG_INFO *mvlCfg);

ST_RET mvl_start_acse (MVL_CFG_INFO *cfgInfo);
ST_RET mvl_end_acse (ST_VOID);


ST_VOID _mvl_init_conn_ctrl (ST_VOID);

ST_RET mvl_initiate_req (ST_CHAR *remAr,
			INIT_INFO *req_info, 
			INIT_INFO *resp_info,
			MVL_NET_INFO **net_info_out);
ST_RET mvla_initiate_req (ST_CHAR *remAr,
			INIT_INFO *req_info, 
			INIT_INFO *resp_info,
			MVL_NET_INFO **net_info_out,
			MVL_REQ_PEND **req_out);

/* If this variable is not NULL, MVL will use it as the source for the	*/
/* local dib entry for an initiate request.				*/
extern struct dib_entry *_mvla_initiate_locDe;

ST_RET mvl_abort_req (MVL_NET_INFO *cc);

ST_RET mvl_init_audt_addr (AUDT_APDU *audt, ST_CHAR *locArName, ST_CHAR *remArName);
ST_VOID mvl_dib_entry_to_app_ref (struct dib_entry *de, struct app_ref *appRef);
ST_RET mvl_add_bind_ctrl (NET_BIND_CTRL *bc);

/************************************************************************/
/* User defined function pointer to handle disconnect indications	*/

#define MVL_ACSE_RELEASE_IND	1
#define MVL_ACSE_ABORT_IND	2
extern ST_VOID (*u_mvl_disc_ind_fun) (MVL_NET_INFO *cc, ST_INT discType);

/************************************************************************/
/* User defined function pointer to handle connect indications		*/

extern ST_RET (*u_mvl_connect_ind_fun) (MVL_NET_INFO *cc, INIT_INFO *init_info);


/************************************************************************/
#ifdef __cplusplus
}
#endif

#endif 	/* MVL_ACSE_INCLUDED */
/************************************************************************/

