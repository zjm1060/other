/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*                2001-2002, All Rights Reserved.                       */
/*                                                                      */
/*           PROPRIETARY AND CONFIDENTIAL                               */
/*                                                                      */
/* MODULE NAME : icfg.h	                                                */
/* PRODUCT(S)  : ICCP                                                   */
/*                                                                      */
/* MODULE DESCRIPTION:                                                  */
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev                  Comments                        */
/* --------  ---  ------  --------------------------------------------	*/
/* 06/20/02  RKR     15   Copyright year update                         */
/* 04/24/02  DGE     14	  Added default for TLE.			*/
/* 04/22/02  MDE     13	  Added dsConditionsRequested to client DSTS	*/
/* 04/17/02  MDE     12	  Added remote DV config options  		*/
/* 12/12/01  DGE     11	  Added Default value defines for MaxReqPend,	*/
/*			    MaxIndPend and MaxNest.			*/
/* 12/11/01  DGE     10	  Added Default value defines for CheckBackId,	*/
/*			    SelectTimeout, BufferTime, IntegrityCheck	*/
/*			    and AssocRetryDelay.			*/
/* 12/07/01  DGE     09	  Added NormalValMin/Max for State data types.	*/
/* 12/05/01  DGE     08	  Changed RealQTimeTag to RealQTimetag.		*/
/* 12/03/01  GLB     07	  Changed server infoRef from ST_INT to ST_LONG */
/* 11/28/01  GLB     06	  Added ICFG_LOG_ERR3                           */
/* 11/28/01  GLB     05   Changed discreteVals from ST_LONG to ST_INT32 */
/* 11/21/01  DGE     04   Changed MaxReqPend & MaxIndPend from 32 bit 	*/
/*			    to 16 bit and MaxNest from 32 bit to 8 bit. */
/* 11/19/01  DGE     03   Changed BufferTime, CheckBackId, Integrity	*/
/*			    and Interval from 32 bit to 16 bit.	        */
/* 11/09/01  DGE     02   Added icfgDb declaration and removed the	*/
/*			    NormalSource Enum Value Derived.            */
/* 03/01/01  MDE     01   New module                                    */
/************************************************************************/

#if !defined(ICFG_INCLUDED)
#define ICFG_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "gen_list.h"

/************************************************************************/
            
#define  MAX_DESCRIPTION_LEN  255              
#define  MAX_MAPINFO_LEN      128              

#define  MAX_REMOTE_AR_NAMES    4

#define  MI_MAX_BLT_ID_LEN     32

#define	 Default_MaxReqPend	  5
#define	 Default_MaxIndPend	  5
#define	 Default_MaxNest	  5

#define	 Default_CheckBackId	  0
#define  Default_SelectTimeout   30
#define	 Default_BufferTime	  2
#define  Default_IntegrityCheck   0
#define	 Default_AssocRetryDelay 30
#define  Default_Tle		 60

/************************************************************************/

#define ICFG_LOG_FLOW  	0x0001   
#define ICFG_LOG_NERR 	0x0002   
#define ICFG_LOG_ERR  	0x0004   

extern  ST_UINT  icfg_debug_sel;

/************************************************************************/

  enum icfgDvTypeVal 
    {
    Real 		= 0,
    State		= 1,
    Discrete		= 2,
    RealQ		= 3,
    StateQ		= 4,
    DiscreteQ		= 5,
    RealQTimetag	= 6,
    StateQTimetag	= 7,
    DiscreteQTimetag	= 8,
    RealExtended	= 9,
    StateExtended	= 10,
    DiscreteExtended	= 11
    };


  enum icfgDevTypeVal 
    {
    DevCommand		= 0,
    DevDiscrete		= 1,
    DevReal		= 2 
    };


  enum icfgSourceVal  
    {
    Calculated,
    Entered,
    Estimated,
    Telemetered
    };


  enum icfgScopeVal  
    {
    VCC	= 0,
    ICC	= 1
    };


/*  Values for remDvValidation	*/ 
  enum icfgRemDvValidation
    {
    ValNone		= 0,
    ValNames		= 1,
    ValNamesAndTypes	= 2 
    };

/*  Values for remDvGetConfig	*/ 
  enum icfgRemDvGetConfig
    {
    GetConfigNone 	= 0,
    GetConfigXml 	= 1,
    GetConfigDb		= 2 
    };

/************************************************************************/
/*  Enumerated Data                                                     */

#define  NUM_OF_ENUM_DVTYPES    12    
#define  NUM_OF_ENUM_DEVTYPES    3    
#define	 NUM_OF_ENUM_SRCVALS     4    
#define  NUM_OF_ENUM_SCOPEVALS   2    
#define  NUM_OF_ENUM_DV_VALIDATION 3    
#define  NUM_OF_ENUM_GET_CFG	   3    

/* use this where "0" is a valid enum value */
#define  INVALID_ENUM_VAL	-1   


/* arrays used to output the enumeration type in string format */
/* they must match the previous 'enum' declarations            */
extern ST_CHAR *icfgDvTypeValEnum[];
extern ST_CHAR *icfgDevTypeValEnum[];
extern ST_CHAR *icfgSourceValEnum[];
extern ST_CHAR *icfgScopeValEnum[];
extern ST_CHAR *icfgRemDvValidationEnum[];
extern ST_CHAR *icfgRemDvGetConfigEnum[];
                                       
/************************************************************************/
/* This function performs the XML file parse operation, and makes       */
/* callbacks into the user code to present ICCP objects.                */
/* Returns SD_SUCCESS or a failure code                                 */

ST_RET icfgXml (ST_VOID *userData, ST_CHAR *xmlFileName);
                                       
/************************************************************************/
/* This function performs the Database parse operation, and makes       */
/* callbacks into the user code to present ICCP objects.                */
/* Returns SD_SUCCESS or a failure code                                 */

ST_RET icfgDb  (ST_VOID *userData, ST_CHAR *LccName);

/************************************************************************/
/* Use this to free info passed to the callback routines                */

#define  icfgFree(ptr)chk_free(ptr);


/************************************************************************/
/*  forward declarations                                                */

struct icfg_local_dv;
struct icfg_local_dev;
struct icfg_local_im;
struct icfg_remote_cc;

struct icfg_server_obj_counts;   	
struct icfg_server_dv; 		
struct icfg_server_dev; 		
struct icfg_server_im; 		
struct icfg_server_ds; 		

struct icfg_client_obj_counts;  	        
struct icfg_client_dv;          			
struct icfg_client_dev;      		
struct icfg_client_im;   		
struct icfg_client_ds;     		
struct icfg_client_dsts;        
struct icfg_association;       	


/************************************************************************/
/* LOCAL CONTROL CENTER                                                 */
/* There is a single Local Control Center per configuration, and this	*/
/* represents the ICCP node being configured                            */
/* The start function is called at the start of the parse, and the end	*/
/* function after the parse is complete.                                */

typedef struct icfg_local_cc
  {
  DBL_LNK l;		       
  ST_CHAR  name[MAX_IDENT_LEN+1];		
  ST_LONG  maxDsTs;
  ST_LONG  maxDs;
  ST_LONG  maxMmsPduSize;
  ST_CHAR  description[MAX_DESCRIPTION_LEN+1];

  struct icfg_local_dv   *localDvList;
  struct icfg_local_dev  *localDevList;
  struct icfg_local_im   *localInfoMsgList;
  struct icfg_remote_cc  *remoteControlCenterList;

  } ICFG_LOCAL_CC;

ST_RET u_icfg_StartLocalControlCenter (ST_VOID *usr, ICFG_LOCAL_CC *info);
ST_RET u_icfg_EndLocalControlCenter   (ST_VOID *usr);


/************************************************************************/
/* LOCAL OBJECTS                                                        */
/* These functions are called once per parse, and bracket the           */
/* configuration of local objects.                                      */

ST_RET u_icfg_StartLocalObjects (ST_VOID *usr);
ST_RET u_icfg_EndLocalObjects   (ST_VOID *usr);


/************************************************************************/
/* LOCAL DATA VALUES                                                    */
/* These functions are called to configure local data values.           */

typedef struct icfg_local_dv
  {
  DBL_LNK l;		       
  ST_CHAR  name[MAX_IDENT_LEN+1];
  enum     icfgDvTypeVal dvType;
  ST_CHAR  mapInfo[MAX_MAPINFO_LEN+1];
  enum     icfgSourceVal normalSource;
  ST_INT   minPresent;
  ST_INT   maxPresent;
  
  union
    {
    struct 
      {
      ST_INT32 valMin;
      ST_INT32 valMax;	       
      } discreteVals;
    struct 
      {
      ST_FLOAT valMin;
      ST_FLOAT valMax;
      } realVals;
    struct 
      {
      ST_UCHAR valMin;
      ST_UCHAR valMax;	       
      } stateVals;
    } vals;
  ST_CHAR  description[MAX_DESCRIPTION_LEN+1];
  } ICFG_LOCAL_DV;

ST_RET u_icfg_StartLocalDataValues (ST_VOID *usr, ST_INT numDv);
ST_RET u_icfg_LocalDataValue       (ST_VOID *usr, ICFG_LOCAL_DV *info);
ST_RET u_icfg_EndLocalDataValues   (ST_VOID *usr);


/************************************************************************/
/* LOCAL DEVICES                                                        */
/* These functions are called to configure local devices                */

typedef struct icfg_local_dev
  {
  DBL_LNK    l;			
  ST_CHAR    name[MAX_IDENT_LEN-4+1];
  enum	     icfgDevTypeVal devType;
  ST_CHAR    mapInfo[MAX_MAPINFO_LEN+1];
  ST_BOOLEAN sbo; 
  ST_INT16   checkBackId;
  ST_LONG    selTime;  
  ST_BOOLEAN tagEnable; 
  ST_CHAR    description[MAX_DESCRIPTION_LEN+1];
  } ICFG_LOCAL_DEV;

ST_RET u_icfg_StartLocalDevices (ST_VOID *usr, ST_INT numDev);
ST_RET u_icfg_LocalDevice       (ST_VOID *usr, ICFG_LOCAL_DEV *info);
ST_RET u_icfg_EndLocalDevices   (ST_VOID *usr);


/************************************************************************/
/* LOCAL INFORMATION MESSAGES                                           */
/* These functions are called to configure local information messages	*/

typedef struct icfg_local_im
  {
  DBL_LNK l;			
  ST_LONG infoRef;
  ST_CHAR mapInfo[MAX_MAPINFO_LEN+1];
  ST_LONG maxSize; 
  ST_CHAR description[MAX_DESCRIPTION_LEN+1];
  } ICFG_LOCAL_INFOMSG;

ST_INT u_icfg_StartLocalInfoMsgs (ST_VOID *usr, ST_INT numInfoMsg);
ST_INT u_icfg_LocalInfoMsg       (ST_VOID *usr, ICFG_LOCAL_INFOMSG *info);
ST_INT u_icfg_EndLocalInfoMsgs   (ST_VOID *usr);


/************************************************************************/
/************************************************************************/
/* REMOTE CONTROL CENTERS                                               */
/* There will be one or more Remote Control Centers per configuration. 	*/
/* The start function is called when "icfgx" begins parsing Remote      */
/* Control Center configuration information, and the end when all       */
/* Remote Control Center configuration is complete.                     */

ST_RET u_icfg_StartRemoteControlCenters (ST_VOID *usr, ST_INT numRcc);
ST_RET u_icfg_EndRemoteControlCenters   (ST_VOID *usr);


/************************************************************************/
/* REMOTE CONTROL CENTER                                                */
/* These functions are called when we begin/end parsing a particular 	*/
/* Remote Control Center's configuration                                */

/* defines for block supported                                          */
/* Note: these are aligned with ICCP Lite, do not change                */
#define ICFG_BLOCK1	0x80000000
#define ICFG_BLOCK2	0x40000000
#define ICFG_BLOCK3	0x20000000
#define ICFG_BLOCK4	0x10000000
#define ICFG_BLOCK5	0x08000000
#define ICFG_BLOCK6	0x04000000
#define ICFG_BLOCK7	0x02000000
#define ICFG_BLOCK8	0x01000000
#define ICFG_BLOCK9	0x00800000

typedef struct blt
  {
  ST_CHAR name[MI_MAX_BLT_ID_LEN+1];
  ST_CHAR id[MI_MAX_BLT_ID_LEN+1];
  ST_CHAR localDom[MAX_IDENT_LEN+1];
  ST_CHAR remoteDom[MAX_IDENT_LEN+1];
  ST_LONG shortestInterval; 
  ST_LONG blocks; 
  enum icfgRemDvValidation  remDvValidation;
  enum icfgRemDvGetConfig   remDvGetConfig;
  ST_BOOLEAN remDvExcludeInvalidFromDs;
  ST_CHAR description[MAX_DESCRIPTION_LEN+1];
  } BLT;

typedef struct icfg_remote_cc
  {
  DBL_LNK l;			/* For user */
  ST_CHAR name[MAX_IDENT_LEN+1];
  ST_CHAR description[MAX_DESCRIPTION_LEN+1];
  BLT blt;

  struct icfg_server_obj_counts  *serverObjCounts;   
  struct icfg_server_dv          *serverDvList;			
  struct icfg_server_dev         *serverDevList;		
  struct icfg_server_im          *serverInfoMsgList;			
  struct icfg_server_ds          *serverDsList;			

  struct icfg_client_obj_counts  *clientObjCounts;	
  struct icfg_client_dv          *clientDvList;			
  struct icfg_client_dev         *clientDevList;		
  struct icfg_client_im          *clientInfoMsgList;			
  struct icfg_client_ds          *clientDsList;			
  struct icfg_client_dsts        *clientDsTsList;		
  struct icfg_association        *assocList;				

  } ICFG_REMOTE_CC;

ST_RET u_icfg_StartRemoteControlCenter (ST_VOID *usr, ICFG_REMOTE_CC *info);
ST_RET u_icfg_EndRemoteControlCenter   (ST_VOID *usr);


/************************************************************************/
/* ASSOCIATIONS	                                                        */
/* These functions are called to configure associations to the remote   */
/* defines for Connect role */

/* NOTE: These define values have been aligned with ICCP Lite and 	*/
/* must not be changed without examing the implications carefully	*/
#define ICFG_ROLE_CALLING	0x0002
#define ICFG_ROLE_CALLED	0x0004

/* defines for Service role */
#define ICFG_ROLE_CLIENT	0x0008
#define ICFG_ROLE_SERVER	0x0010

typedef struct icfg_association
  {
  DBL_LNK   l;			
  ST_CHAR   name[MAX_IDENT_LEN+1];
  ST_CHAR   localAr[MAX_AR_LEN+1];
  ST_CHAR   remoteAr[MAX_REMOTE_AR_NAMES][MAX_AR_LEN+1];
  ST_INT    totalRemoteArNames;
  ST_INT    connectRole;
  ST_INT    serviceRole;
  ST_LONG   retryTime;
  ST_LONG   maxMmsPduSize;
  ST_INT16  maxReqPend;
  ST_INT16  maxIndPend;
  ST_INT8   maxNest;
  ST_CHAR   description[MAX_DESCRIPTION_LEN+1];
  } ICFG_ASSOCIATION;

ST_RET u_icfg_StartAssociations (ST_VOID *usr, ST_INT numAssocs); 
ST_RET u_icfg_Association       (ST_VOID *usr, ICFG_ASSOCIATION *info); 
ST_RET u_icfg_EndAssociations   (ST_VOID *usr); 


/************************************************************************/
/* SERVER OBJECTS                                                       */
/* These functions are called once per Remote Control Center, and       */
/* bracket the configuration of local objects to be exposed to the      */
/* remote.                                                              */

typedef struct icfg_server_obj_counts
  {
  DBL_LNK l;			
  ST_LONG numVccDv;
  ST_LONG numVccDev;
  ST_LONG numVccInfoMsg;
  ST_LONG numVccDs;
  ST_LONG numIccDv;
  ST_LONG numIccDev;
  ST_LONG numIccInfoMsg;
  ST_LONG numIccDs;
  } ICFG_SERVER_OBJ_COUNTS;

ST_RET u_icfg_StartServerObjects (ST_VOID *usr, ICFG_SERVER_OBJ_COUNTS *info);
ST_RET u_icfg_EndServerObjects   (ST_VOID *usr);


/************************************************************************/
/* SERVER DATA VALUES                                                   */
/* These functions are called to configure local data values to be      */
/* exposed to the remote being configured.                              */

typedef struct icfg_server_dv
  {
  DBL_LNK    l;			
  ST_CHAR    name[MAX_IDENT_LEN+1];
  enum       icfgScopeVal scope;
  enum       icfgDvTypeVal dvType;
  ST_BOOLEAN readOnly;
  ST_CHAR    mapInfo[MAX_MAPINFO_LEN+1];
  } ICFG_SERVER_DV;

ST_RET u_icfg_StartServerDataValues (ST_VOID *usr, ST_INT numDv);
ST_RET u_icfg_ServerDataValue       (ST_VOID *usr, ICFG_SERVER_DV *info);
ST_RET u_icfg_EndServerDataValues   (ST_VOID *usr);


/************************************************************************/
/* SERVER DEVICES                                                       */
/* These functions are called to configure local devices to be exposed	*/
/* to the remote being configured.                                      */

typedef struct icfg_server_dev
  {
  DBL_LNK    l;			
  ST_CHAR    name[MAX_IDENT_LEN-4+1];
  enum       icfgScopeVal scope;
  enum       icfgDevTypeVal devType;
  ST_CHAR    mapInfo[MAX_MAPINFO_LEN+1];
  ST_BOOLEAN tagEnable;
  } ICFG_SERVER_DEV;

ST_RET u_icfg_StartServerDevices (ST_VOID *usr, ST_INT numDev);
ST_RET u_icfg_ServerDevice       (ST_VOID *usr, ICFG_SERVER_DEV *info);
ST_RET u_icfg_EndServerDevices   (ST_VOID *usr);


/************************************************************************/
/* SERVER INFORMATION MESSAGES                                          */
/* These functions are called to configure local information messages	*/
/* to be exposed to the remote being configured.                        */

typedef struct icfg_server_im
  {
  DBL_LNK l;			
  ST_LONG infoRef;
  enum    icfgScopeVal scope;
  ST_CHAR mapInfo[MAX_MAPINFO_LEN+1];
  } ICFG_SERVER_INFOMSG;

ST_RET u_icfg_StartServerInfoMsgs (ST_VOID *usr, ST_INT numInfoMsg);
ST_RET u_icfg_ServerInfoMsg       (ST_VOID *usr, ICFG_SERVER_INFOMSG *info);
ST_RET u_icfg_EndServerInfoMsgs   (ST_VOID *usr);


/************************************************************************/
/* SERVER DATA SETS                                                     */
/* These functions are called to configure local data sets to be        */
/* exposed to the remote being configured.                              */

typedef struct icfg_server_ds_dv
  {
  ST_CHAR name[MAX_IDENT_LEN+1];
  enum    icfgScopeVal scope;
  } ICFG_SERVER_DS_DV;

typedef struct icfg_server_ds
  {		      
  DBL_LNK	     l;			
  ST_CHAR	     name[MAX_IDENT_LEN+1];
  enum		     icfgScopeVal scope;
  ST_BOOLEAN	     transferSetName;
  ST_BOOLEAN	     transferSetTimeStamp;
  ST_BOOLEAN	     dsConditionsDetected;
  ST_BOOLEAN	     eventCodeDetected;
  ST_CHAR	     description[MAX_DESCRIPTION_LEN+1];
  ST_INT	     numDv;
  ICFG_SERVER_DS_DV *dvArray;
  } ICFG_SERVER_DS;

ST_RET u_icfg_StartServerDataSets (ST_VOID *usr, ST_INT numDs);
ST_RET u_icfg_ServerDataSet       (ST_VOID *usr, ICFG_SERVER_DS *info);
ST_RET u_icfg_EndServerDataSets   (ST_VOID *usr); 


/************************************************************************/
/* CLIENT OBJECTS                                                       */
/* These functions are called once per Remote Control Center, and       */
/* bracket the configuration of remote objects to be accessible to      */
/* the local node.                                                      */

typedef struct icfg_client_obj_counts
  {
  DBL_LNK l;			/* For user */
  ST_LONG numVccDv;
  ST_LONG numVccDev;
  ST_LONG numVccInfoMsg;
  ST_LONG numIccDv;
  ST_LONG numIccDev;
  ST_LONG numIccInfoMsg;
  ST_LONG numDs;
  ST_LONG numDsTs;
  } ICFG_CLIENT_OBJ_COUNTS;

ST_RET u_icfg_StartClientObjects (ST_VOID *usr, ICFG_CLIENT_OBJ_COUNTS *info);
ST_RET u_icfg_EndClientObjects   (ST_VOID *usr);


/************************************************************************/
/* CLIENT DATA VALUES                                                   */
/* These functions are called to configure remote data values to be 	*/
/* accessible by the local node.                                        */

typedef struct icfg_client_dv
  {
  DBL_LNK     l;			
  ST_CHAR     name[MAX_IDENT_LEN+1];
  enum	      icfgScopeVal scope;
  enum	      icfgDvTypeVal dvType;
  ST_BOOLEAN  readOnly;
  ST_CHAR     mapInfo[MAX_MAPINFO_LEN+1];
  ST_CHAR     description[MAX_DESCRIPTION_LEN+1];
  } ICFG_CLIENT_DV;

ST_RET u_icfg_StartClientDataValues (ST_VOID *usr, ST_INT numDv);
ST_RET u_icfg_ClientDataValue       (ST_VOID *usr, ICFG_CLIENT_DV *info);
ST_RET u_icfg_EndClientDataValues   (ST_VOID *usr);


/************************************************************************/
/* CLIENT DEVICES                                                       */
/* These functions are called to configure remote devices to be         */
/* accessible by the local node.                                        */

typedef struct icfg_client_dev
  {
  DBL_LNK     l;			
  ST_CHAR     name[MAX_IDENT_LEN-4+1];
  enum	      icfgScopeVal scope;
  enum	      icfgDevTypeVal devType;
  ST_CHAR     mapInfo[MAX_MAPINFO_LEN+1];
  ST_BOOLEAN  sbo; 
  ST_INT16    checkBackId;
  ST_BOOLEAN  tagEnable;
  ST_CHAR     description[MAX_DESCRIPTION_LEN+1];
  } ICFG_CLIENT_DEV;

ST_RET u_icfg_StartClientDevices (ST_VOID *usr, ST_INT numDev);
ST_RET u_icfg_ClientDevice       (ST_VOID *usr,ICFG_CLIENT_DEV *info);
ST_RET u_icfg_EndClientDevices   (ST_VOID *usr);


/************************************************************************/
/* CLIENT INFORMATION MESSAGES                                          */
/* These functions are called to configure remote information messgaes  */
/* to be accessible by the local node                                   */

typedef struct icfg_client_im
  {
  DBL_LNK l;			
  ST_LONG infoRef;
  enum    icfgScopeVal scope;
  ST_CHAR mapInfo[MAX_MAPINFO_LEN+1];
  ST_LONG maxSize;
  ST_CHAR description[MAX_DESCRIPTION_LEN+1];
  } ICFG_CLIENT_INFOMSG;

ST_INT u_icfg_StartClientInfoMsgs (ST_VOID *usr, ST_INT numInfoMsg);
ST_INT u_icfg_ClientInfoMsg       (ST_VOID *usr, ICFG_CLIENT_INFOMSG *info);
ST_INT u_icfg_EndClientInfoMsgs   (ST_VOID *usr);


/************************************************************************/
/* CLIENT DATA SETS                                                     */
/* These functions are called to configure data sets to be created      */
/* in the remote node.                                                  */

typedef struct icfg_client_ds_dv
  {
  ST_CHAR name[MAX_IDENT_LEN+1];
  enum    icfgScopeVal scope;
  } ICFG_CLIENT_DS_DV;

typedef struct icfg_client_ds
  {
  DBL_LNK	     l;			
  ST_CHAR	     name[MAX_IDENT_LEN+1];
  ST_BOOLEAN	     transferSetName;
  ST_BOOLEAN	     transferSetTimeStamp;
  ST_BOOLEAN	     dsConditionsDetected;
  ST_BOOLEAN	     eventCodeDetected;
  ST_CHAR	     description[MAX_DESCRIPTION_LEN+1];
  ST_INT	     numDv;
  ICFG_CLIENT_DS_DV *dvArray;
  } ICFG_CLIENT_DS;

ST_INT u_icfg_StartClientDataSets (ST_VOID *usr, ST_INT numDs);
ST_INT u_icfg_ClientDataSet       (ST_VOID *usr, ICFG_CLIENT_DS *info);
ST_INT u_icfg_EndClientDataSets   (ST_VOID *usr); 


/************************************************************************/
/* CLIENT DSTS                                                          */
/* These functions are called to configure remote DSTS to be creaded    */
/* in the remote node.                                                  */

/* Defines for dsConditionsRequested	                                */
/* Note: these are aligned with ICCP Lite and the spec; do not change!  */
#define ICFG_DSC_INTERVAL 	0x80
#define ICFG_DSC_INTEGRITY	0x40
#define ICFG_DSC_CHANGE		0x20
#define ICFG_DSC_OPERATOR	0x10
#define ICFG_DSC_EXTERNAL	0x08

typedef struct icfg_client_dsts
  {
  DBL_LNK     l;			
  ST_CHAR     dsName[MAX_IDENT_LEN+1];
  ST_CHAR     assocName[MAX_IDENT_LEN+1];
  ST_INT16    interval;
  ST_BOOLEAN  rbe;
  ST_INT16    bufferTime;
  ST_INT16    integrity;
  time_t      startTime;
  ST_INT16    tle;
  ST_UCHAR    dsConditionsRequested;
  ST_BOOLEAN  critical;
  ST_CHAR     description[MAX_DESCRIPTION_LEN+1];
  } ICFG_CLIENT_DSTS;

ST_RET u_icfg_StartClientDSTSs (ST_VOID *usr, ST_INT numDsTs); 
ST_RET u_icfg_ClientDSTS       (ST_VOID *usr, ICFG_CLIENT_DSTS *info);
ST_RET u_icfg_EndClientDSTSs   (ST_VOID *usr); 


/************************************************************************/
/************************************************************************/
/* INTERNAL                                                             */
/************************************************************************/
#ifdef DEBUG_SISCO
/************************************************************************/

/* Log type strings */
extern SD_CONST ST_CHAR *SD_CONST _icfg_flow_logstr;
extern SD_CONST ST_CHAR *SD_CONST _icfg_nerr_logstr;
extern SD_CONST ST_CHAR *SD_CONST _icfg_err_logstr;
extern SD_CONST ST_CHAR *SD_CONST _icfg_cerr_logstr;

/*********************** ERR Macros *************************************/

#define ICFG_LOG_ERR0(a) {\
                     if (icfg_debug_sel & ICFG_LOG_ERR)\
                       _slog (sLogCtrl,_icfg_err_logstr,\
                             thisFileName,__LINE__,a);\
                    }

#define ICFG_LOG_ERR1(a,b) {\
                     if (icfg_debug_sel & ICFG_LOG_ERR)\
                       _slog (sLogCtrl,_icfg_err_logstr,\
                             thisFileName,__LINE__,a,b);\
                    }

#define ICFG_LOG_ERR2(a,b,c) {\
                     if (icfg_debug_sel & ICFG_LOG_ERR)\
                       _slog (sLogCtrl,_icfg_err_logstr,\
                             thisFileName,__LINE__,a,b,c);\
                    }

#define ICFG_LOG_ERR3(a,b,c,d) {\
                     if (icfg_debug_sel & ICFG_LOG_ERR)\
                       _slog (sLogCtrl,_icfg_err_logstr,\
                             thisFileName,__LINE__,a,b,c,d);\
                    }

#define ICFG_LOG_CERR0(a) {\
                     if (icfg_debug_sel & ICFG_LOG_ERR)\
                       _slogc (sLogCtrl,a);\
                    }
#define ICFG_LOG_CERR1(a,b) {\
                     if (icfg_debug_sel & ICFG_LOG_ERR)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define ICFG_LOG_CERR2(a,b,c) {\
                     if (icfg_debug_sel & ICFG_LOG_ERR)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define ICFG_LOG_CERR3(a,b,c,d) {\
                     if (icfg_debug_sel & ICFG_LOG_ERR)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/*********************** NERR Macros *************************************/

#define ICFG_LOG_NERR0(a) {\
                     if (icfg_debug_sel & ICFG_LOG_NERR)\
                       _slog (sLogCtrl,_icfg_nerr_logstr,\
                             thisFileName,__LINE__,a);\
                    }
#define ICFG_LOG_NERR1(a,b) {\
                     if (icfg_debug_sel & ICFG_LOG_NERR)\
                       _slog (sLogCtrl,_icfg_nerr_logstr,\
                             thisFileName,__LINE__,a,b);\
                    }
#define ICFG_LOG_NERR2(a,b,c) {\
                     if (icfg_debug_sel & ICFG_LOG_NERR)\
                       _slog (sLogCtrl,_icfg_nerr_logstr,\
                             thisFileName,__LINE__,a,b,c);\
                    }

/*********************** FLOW Macros *******************************/

#define ICFG_LOG_FLOW0(a) {\
                     if (icfg_debug_sel & ICFG_LOG_FLOW)\
                       _slog (sLogCtrl,_icfg_flow_logstr,\
                             thisFileName,__LINE__,a);\
                    }
#define ICFG_LOG_FLOW1(a,b) {\
                     if (icfg_debug_sel & ICFG_LOG_FLOW)\
                       _slog (sLogCtrl,_icfg_flow_logstr,\
                             thisFileName,__LINE__,a,b);\
                    }
#define ICFG_LOG_FLOW2(a,b,c) {\
                     if (icfg_debug_sel & ICFG_LOG_FLOW)\
                       _slog (sLogCtrl,_icfg_flow_logstr,\
                             thisFileName,__LINE__,a,b,c);\
                    }
#define ICFG_LOG_FLOW3(a,b,c,d) {\
                     if (icfg_debug_sel & ICFG_LOG_FLOW)\
                       _slog (sLogCtrl,_icfg_flow_logstr,\
                             thisFileName,__LINE__,a,b,c,d);\
                    }

#define ICFG_LOG_CFLOW0(a) {\
                     if (icfg_debug_sel & ICFG_LOG_FLOW)\
                       _slogc (sLogCtrl,a);\
                    }
#define ICFG_LOG_CFLOW1(a,b) {\
                     if (icfg_debug_sel & ICFG_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define ICFG_LOG_CFLOW2(a,b,c) {\
                     if (icfg_debug_sel & ICFG_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define ICFG_LOG_CFLOW3(a,b,c,d) {\
                     if (icfg_debug_sel & ICFG_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }
/************************************************************************/
#else /* Not debug */
/************************************************************************/

#define ICFG_LOG_ERR0(a)
#define ICFG_LOG_ERR1(a,b)
#define ICFG_LOG_ERR2(a,b,c)
#define ICFG_LOG_ERR3(a,b,c,d)
#define ICFG_LOG_CERR0(a)
#define ICFG_LOG_CERR1(a,b)
#define ICFG_LOG_CERR2(a,b,c)
#define ICFG_LOG_NERR0(a)
#define ICFG_LOG_NERR1(a,b)
#define ICFG_LOG_NERR2(a,b,c)
#define ICFG_LOG_FLOW0(a)
#define ICFG_LOG_FLOW1(a,b)
#define ICFG_LOG_FLOW2(a,b,c)
#define ICFG_LOG_FLOW3(a,b,c,d)
#define ICFG_LOG_CFLOW0(a)
#define ICFG_LOG_CFLOW1(a,b)
#define ICFG_LOG_CFLOW2(a,b,c)
#define ICFG_LOG_CFLOW3(a,b,c,d)

#endif

/************************************************************************/

#ifdef __cplusplus
}
#endif

/************************************************************************/
#endif  /* File not already included */
/************************************************************************/



