/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	      1998, All Rights Reserved		        	*/
/*									*/
/* MODULE NAME : mics_cfg.h    						*/
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
/* 06/20/02  MDE     03    Removed ICFG naming				*/
/* 05/21/02  MDE     02    Added usr pointers				*/
/* 04/08/02  MDE     01    New file					*/
/************************************************************************/

#ifndef MICS_CFG_INCLUDED
#define MICS_CFG_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/

#include "mi.h"

/************************************************************************/
/* MIS USR Data Structures						*/
/* These structures are used to store the server side information about	*/
/* Data Values & Devices.						*/
/************************************************************************/
/************************************************************************/

typedef struct
  {
  DBL_LNK l;			/* Linked list of data values */
  ST_VOID *usr;
  ST_INT mi_type;
  union 
    {
    MI_REAL_EXTENDED	 re;
    MI_STATE_EXTENDED	 se;
    MI_DISCRETE_EXTENDED de;
    } data;
  MIS_DV_REF dvRef;
  } MISU_DV;
extern MISU_DV *misu_dv_list;

typedef struct
  {
  DBL_LNK l;			/* Linked list of devices */
  ST_VOID *usr;
  ST_INT dev_type;
  ST_LONG selTime;  
  MIS_DEVICE_REF devRef;
  } MISU_DEV;
extern MISU_DEV *misu_dev_list;

/************************************************************************/
/************************************************************************/
/* MIC USR Data Structures						*/
/* These structures are used to store the client side information to	*/
/* be used for creating the Data Sets and Data Set Transfer Sets in	*/
/* remote nodes when a connection has been established.			*/
/************************************************************************/

typedef struct
  {
  DBL_LNK l;			/* Linked list of data values */
  ST_VOID *usr;
  ST_INT mi_type;
  union 			/* Data storage for all DV types */
    {
    MI_REAL 			r;
    MI_STATE 	   		s;
    MI_DISCRETE 		d;
    MI_REAL_Q 	   		rq;
    MI_STATE_Q 	   		sq;
    MI_DISCRETE_Q 		dq;
    MI_REAL_Q_TIMETAG 		rqt;
    MI_STATE_Q_TIMETAG    	sqt;
    MI_DISCRETE_Q_TIMETAG 	dqt;
    MI_REAL_EXTENDED 		re;
    MI_STATE_EXTENDED    	se;
    MI_DISCRETE_EXTENDED 	de;
    } data;
  MIC_DV *mic_dv;
  ST_UINT32 ir_idx;	/* for redundancy support */
  } MICU_DV;

/* Data Set */
typedef struct
  {
  DBL_LNK  l;		       
  ST_VOID *usr;
  ST_CHAR  ds_name[MAX_IDENT_LEN+1];		
  ST_INT   ds_scope;
  ST_INT   num_dv; 
  MIC_DV **mic_dv;
  ST_BOOLEAN mic_ds_ok;
  MIC_DS  *mic_ds;
  } MICU_DS;

/* Data Set Transfer Set */
typedef struct 
  {
  DBL_LNK	 l;
  ST_VOID *usr;
  ST_CHAR	 dsTsName[MAX_IDENT_LEN+1];
  MI_DSTS_DATA   dsts_data;
  MICU_DS	*miu_ds;

  MIC_DSTS *mic_dsts;
  } MICU_DSTS;

/* Devices */
typedef struct
  {
  DBL_LNK l;			/* Linked list of data values */
  ST_VOID *usr;
  ST_INT dev_type;
  ST_BOOLEAN sbo;
  MIC_DEVICE *mic_dev;
  } MICU_DEV;

/* Remotes */
typedef struct
  {
  DBL_LNK l;		       
  ST_VOID *usr;
  ST_CHAR name[MAX_IDENT_LEN+1];
  MICU_DV	*dv_list;	/* List of remote data values 		*/
  MICU_DS   	*ds_list;  	/* List of DS to create at connect time	*/
  MICU_DSTS 	*dsts_list;	/* List of remote DSTS	 		*/
  MICU_DEV	*dev_list ;	/* List of remote devices 		*/

  MI_REMOTE *mi_remote;

  ST_INT     remDvValidation;
  ST_INT     remDvGetConfig;
  ST_BOOLEAN remDvExcludeInvalidFromDs;
  } MIU_REMOTE;

/* List of remotes */
extern MIU_REMOTE *miu_remote_list;

MIU_REMOTE *miuFindRemote (ST_CHAR *remoteName);
MIU_REMOTE *miuFindMiRemote (MI_REMOTE *mi_remote);

MICU_DV *micuFindDv (MIU_REMOTE *mi_icfg_remote, 
		     ST_CHAR *dvName, ST_INT scope);
MICU_DS    *micuFindDs (MIU_REMOTE *miuRemote, ST_CHAR *dsName);
MICU_DSTS  *micuFindDsTs (MIU_REMOTE *miuRemote, ST_CHAR *dsTsName);


/************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* MICS_CFG_INCLUDED */
/************************************************************************/

