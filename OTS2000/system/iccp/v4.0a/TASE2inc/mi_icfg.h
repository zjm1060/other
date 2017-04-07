/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*          2001-2002, All Rights Reserved                            	*/
/*                                                                      */
/* MODULE NAME : 	                                                */
/* PRODUCT(S)  :                                                        */
/*                                                                      */
/*                                                                      */
/* MODULE DESCRIPTION :    		       				*/
/*                                                                      */
/*                                                                      */
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :                            */
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev          Comments                                */
/* --------  ---  ------  --------------------------------------------  */
/* 06/20/02  RKR    08    Copyright year update                         */
/* 05/21/02  MDE    07    Added support for checking service role	*/
/* 02/26/02  MDE    06    Added mi_icfg_log_status			*/
/* 01/25/02  MDE    04    Added MI_ICFG_CTRL 				*/
/* 01/23/02  NAV    03    Add mi_icfg_db_load prototype			*/
/* 11/15/01  MDE    02    Completed changes from pointers to references	*/
/* 10/19/01  MDE    01    Created from mit_xmlc.c        		*/
/************************************************************************/

#ifndef MI_ICFG_INCLUDED
#define MI_ICFG_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/

#include "icfg.h"

/************************************************************************/

/* Main load function */
#define MI_ICFG_MODE_INITIAL	0
#define MI_ICFG_MODE_RELOAD	1
#define MI_ICFG_MODE_VERIFY	2

/* Returns SD_SUCCESS, SD_FAILURE, or ... */
#define MI_ICFG_ERR_REMOTE_NOT_FOUND	2      

ST_RET mi_icfg_load (ST_CHAR *xmlFileName, ST_INT mode, 
		     ST_CHAR *remoteName, ST_VOID *usr);

ST_RET mi_icfg_db_load (ST_CHAR *LccName, ST_INT mode, 
		        ST_CHAR *remoteName, ST_VOID *usr);

#define mi_icfg_free(buf) icfgFree(buf)

/* Free the strings alloc'd during configuration */
ST_VOID mi_icfg_free_local_cc (ST_VOID);
ST_VOID mi_icfg_free_remote_cc (MI_REMOTE *mi_remote);

ST_VOID mi_icfg_log_cfg (ST_VOID);
ST_VOID mi_icfg_log_status (ST_VOID);

/************************************************************************/
/* User must supply these callback functions ... */

ST_RET u_miIcfgProcLocalControlCenter  (ST_VOID *usr, ICFG_LOCAL_CC *icfgLocalCc);  

ST_RET u_miIcfgProcRemoteControlCenter (ST_VOID *usr, MI_REMOTE *mi_remote, 
					ICFG_REMOTE_CC *icfgRemoteCc);

ST_RET u_miIcfgProcAssociation (ST_VOID *usr, MI_ASSOC_CTRL *mi_assoc, 
			        ICFG_ASSOCIATION *assoc);  

ST_RET u_miIcfgProcLocalDv (ST_VOID *usr, MIS_DV_REF misDvRef, 
			    ICFG_LOCAL_DV *localDv, ST_BOOLEAN newDv);

ST_RET u_miIcfgProcLocalDev (ST_VOID *usr, MIS_DEVICE_REF misDevRef, 
			     ICFG_LOCAL_DEV *localDev,  ST_BOOLEAN newDev);

ST_RET u_miIcfgProcLocalIm (ST_VOID *usr, ICFG_LOCAL_INFOMSG *localIm);  

ST_RET u_miIcfgProcServerDv  (ST_VOID *usr, MIS_VCC *mis_vcc, MIS_DV_REF dv_ref, 
			      ICFG_SERVER_DV *serverDv);  

ST_RET u_miIcfgProcServerDev (ST_VOID *usr, MIS_VCC *mis_vcc, MIS_DEVICE_REF dev_ref, 
			      ICFG_SERVER_DEV *serverDev);  

ST_RET u_miIcfgProcServerIM  (ST_VOID *usr, MIS_VCC *mis_vcc, 
			      ICFG_SERVER_INFOMSG *serverIm);  

ST_RET u_miIcfgProcServerDs (ST_VOID *usr, MIS_VCC *mis_vcc, 
			     MIS_DS *mis_ds, ICFG_SERVER_DS *serverDs);

ST_RET u_miIcfgProcClientDv  (ST_VOID *usr, MIC_VCC *mic_vcc, MIC_DV *micDv, 
			      ICFG_CLIENT_DV *clientDv);  

ST_RET u_miIcfgProcClientDev (ST_VOID *usr, MIC_VCC *mic_vcc, 
			      MIC_DEVICE *micDev, ICFG_CLIENT_DEV *clientDev);  

ST_RET u_miIcfgProcClientIM  (ST_VOID *usr, MIC_VCC *mic_vcc, 
			      ICFG_CLIENT_INFOMSG *clientIm);  

ST_RET u_miIcfgProcClientDs  (ST_VOID *usr, MIC_VCC *mic_vcc, 
			      ICFG_CLIENT_DS *clientDs);  

ST_RET u_miIcfgProcClientDsTs (ST_VOID *usr, MIC_VCC *mic_vcc, 
			       ICFG_CLIENT_DSTS *clientDsTs);  


/************************************************************************/

/* Load time control structure, used to maintain config state */
typedef struct
  {
  ST_INT 	  mode;
  ST_CHAR 	 *remote_name;
  ST_VOID 	 *usr;
 
  ICFG_LOCAL_CC  *local_cc;
  ICFG_REMOTE_CC *remote_cc;

  ST_BOOLEAN 	  target_remote;
  ST_BOOLEAN 	  target_remote_found;
  MI_REMOTE 	 *mi_remote;
  MIS_VCC     	 *mis_vcc;
  MIC_VCC     	 *mic_vcc;
  ST_INT 	 	  serviceRole;
  } MI_ICFG_CTRL;

/************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* MI_ICFG_INCLUDED */
/************************************************************************/


