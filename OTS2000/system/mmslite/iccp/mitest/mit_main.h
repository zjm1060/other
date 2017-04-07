/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*          1996-2002, All Rights Reserved                              */
/*									*/
/* MODULE NAME : mit_main.h  						*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*									*/
/* MODULE DESCRIPTION : main header file                                */
/*                                                                      */
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev                     Comments                     */
/* --------  ---  ------   -------------------------------------------  */
/* 07/30/02  rkr    25     added dsState and reorgDs                    */
/* 07/19/02  ask    24     cleanup for QNX				*/
/* 07/16/02  rkr    23     added doModifyAddressMatching		*/
/* 06/28/02  ask    22     changes for QNX				*/
/* 06/26/02  ask    21     add set_echo_off proto for QNX		*/
/* 06/20/02  rkr    20     Copyright year update                        */
/* 06/18/02  rkr    19     Changes for tracking outstanding MMS Reqs	*/
/* 06/11/02  ask    18	   removed unused ARlist from MIT_REMOTE struct */
/* 06/11/02  ask    17 	   Updated ICCP_RELEASE to 3.01			*/
/* 02/13/02  ask    16     Changes for mmslite init                     */
/* 02/11/02  rkr    15     Added numRefDSTS				*/
/* 01/14/02  ask    14     Removed references to mit_cfg.c		*/
/* 01/08/02  ask    13     Added global var pstInfoMsgSet 		*/
/* 10/15/01  ask    12     Updated for new ICFG tools			*/
/* 06/21/01  ask    11     Added misc function prototypes               */
/* 06/13/01  rkr    10     Changes for new DataValue test mechanism	*/
/* 06/12/01  rkr    9      Added DataSetDeleteing to MIT_MIC_VCC	*/
/* 05/09/01  ask    8      Changed all MAX_ID_LEN+1 to MAX_ID_LEN 	*/
/*			   Added maxNest to struct MIT_ASSOC_INFO   	*/
/* 05/08/01  ask    7      Added time changing data values as globals	*/
/* 			   Added modifyLocalDataValueChange		*/
/* 05/04/01  ask    6	   Added min & max to struct MIT_DATA_VALUE     */
/* 04/17/01  ask    5  	   Updated ICCP_RELEASE to 2.01			*/
/* 04/12/01  ask    4      Added deleteRemoteDataSet, 			*/
/* 			         createRemoteDataSet, 			*/
/* 			         getRemoteDataSetElementNames,		*/
/*			         getRemoteDataValueNames     		*/
/* 11/14/00  rkr    3      Added logLocalDataSetNames               	*/
/* 07/27/00  rkr    2      Added mitUnconfigure, MIT_INFO_MSG		*/
/* 03/17/99  rkr    1      Created                                      */
/************************************************************************/
#ifndef MIT_MAIN_INCLUDED
#define MIT_MAIN_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


#include "glbtypes.h"
#include "sysincs.h"
#include "time.h"
#include "signal.h"

#include "mvl_acse.h"
#include "mvl_log.h"
#include "miusrobj.h"
#include "mi.h"
#include "mit_log.h"

#define ICCP_RELEASE		"4.2150"
#define MAX_ID_LEN  		33

typedef struct Tag_MIT_DATA_VALUE
  {
  DBL_LNK	l;
  ST_CHAR	dvName[MAX_ID_LEN];
  ST_INT	dvType;
  union {
        ST_DOUBLE	f;
		ST_INT		d;
        }min;
  union {
        ST_DOUBLE	f;
		ST_INT		d;
        }max;
  ST_INT	source;
  ST_INT	ptChgIdx; /* ref into the PtChgTbl, for testing only 	*/
  MIS_DV_REF	dvRef;
  MI_DV_DATA_UNION  data;
  } MIT_DATA_VALUE;

typedef struct Tag_MIT_AR_NAME
  {
  DBL_LNK	l;
  ST_CHAR	arName[MAX_ID_LEN];
  } MIT_AR_NAME;

typedef struct Tag_MIT_REMOTE
   {
  DBL_LNK	l;
  ST_CHAR	remoteName[MAX_ID_LEN];
  MI_REMOTE	*pstMiRemote;  
  ST_LONG 	blocks;
  } MIT_REMOTE;

typedef struct Tag_MIT_DATA_POINT
   {
  DBL_LNK	l;
  ST_CHAR	pointName[MAX_ID_LEN];
  ST_INT	scope; 
  ST_INT	type; 
  } MIT_DATA_POINT;

/* states to determine what is going on with the Data Set */
#define DS_IDLE               0
#define DS_CREATING           1
#define DS_CREATED            2
#define DS_REORGANIZING       3
#define DS_REORGANIZED        4
#define DS_READING            5
#define DS_READ               6
#define DS_FAILED             7

typedef struct Tag_MIT_DATA_SET
   {
  DBL_LNK	l;
  ST_CHAR	dsName[MAX_ID_LEN];
  ST_INT	dsScope; 
  ST_INT	numRefDSTS;
  ST_INT	numPoints;
  ST_BOOLEAN    reorgDs;
  ST_INT        dsState;
  MIT_DATA_POINT	*pointList;
  ST_VOID 	*pstDS;              /* MIC_DS or MIS_DS */
  } MIT_DATA_SET;

typedef struct Tag_MIT_INFO_MSG
   {
  DBL_LNK	l;
  ST_INT	infoRef;
  ST_INT	localRef;
  ST_INT	msgID;
  ST_INT	msgScope; 
  ST_INT	maxSize;
  } MIT_INFO_MSG;

typedef struct Tag_MIT_DEVICE
  {
  DBL_LNK		l;
  ST_CHAR    	devName[MAX_ID_LEN];
  ST_INT16     	chkBackId;
  ST_INT     	scope;
  ST_INT     	type;			/* data type of value associated device */
  time_t     	lastSelectTime;	/* set when device is selected 			*/
  ST_INT	    sboTimeOut;		/* checked when device is operated 		*/
  ST_BOOLEAN 	bSelectable;	/* means device is an SBO device 		*/
  ST_BOOLEAN 	bTaggable;		/* means device is taggable				*/
  MIC_DEVICE 	*pstDevice;
  } MIT_DEVICE;

typedef struct Tag_MIT_DEVICE_REF
  {
  DBL_LNK		l;
  ST_CHAR    	devName[MAX_ID_LEN];
  ST_INT     	scope;
  ST_INT     	type;		
  ST_BOOLEAN 	bSelectable;	
  ST_BOOLEAN 	bOperable;  /* used only for Server Device Ref		*/	
  ST_BOOLEAN 	bTaggable;	
  MIC_DEVICE    *pstDevice;   /* used only for Client Device Ref	*/
  } MIT_DEVICE_REF;

typedef struct Tag_MIT_MIS_VCC
  {
  DBL_LNK	l;
  ST_CHAR	vccName[MAX_ID_LEN];
  ST_CHAR	remoteName[MAX_ID_LEN];
  ST_CHAR	localArName[MAX_ID_LEN];
  ST_CHAR	bilateralTableId[MAX_ID_LEN];
  ST_CHAR	localDomainName[MAX_ID_LEN];
  ST_INT	maxNumVccDvRef; 
  ST_INT	maxNumVccDs;
  ST_INT	maxNumIccDvRef;
  ST_INT	maxNumIccDs;
  ST_INT    maxNumTsCtrl;
  ST_INT	minInterval;
  ST_INT	maxSegsize;
  ST_INT	maxReqCalling;
  ST_INT	maxReqCalled;
  MIT_DATA_SET  *pstDataSets;
  MIT_INFO_MSG  *pstInfoMsgs;
  MIT_DEVICE_REF *pstDeviceSet;
  MIS_VCC	*pstMisVcc;  
  } MIT_MIS_VCC;

typedef struct Tag_MIT_TRANSFER_SET
  {
  DBL_LNK	l;
  ST_CHAR	dsTsName[MAX_ID_LEN];
  ST_INT	startTime;
  ST_INT	interval;
  ST_INT	tLE;
  ST_INT	bufferTime;
  ST_INT	integrityCheck;
  ST_INT	dSConditionsRequested;
  ST_BOOLEAN	blockData;
  ST_BOOLEAN	critical;
  ST_BOOLEAN	rBE;
  ST_BOOLEAN	status;
  ST_CHAR	dataSetName[MAX_ID_LEN];
  MIC_DSTS	*pstDSTS;  
  } MIT_TRANSFER_SET;

typedef struct Tag_MIT_DVREF_INFO
   {
  DBL_LNK	l;
  ST_CHAR	*dvName;
  ST_INT	scope;
  ST_INT	type;
  ST_CHAR	*tag;    
  ST_INT	writable;
  } MIT_DVREF_INFO;


typedef struct Tag_MIT_MIC_VCC
  {
  DBL_LNK	l;
  ST_CHAR	vccName[MAX_ID_LEN];
  ST_CHAR	remoteName[MAX_ID_LEN];
  ST_CHAR	bilateralTableId[MAX_ID_LEN];
  ST_CHAR	remoteDomainName[MAX_ID_LEN];
  ST_INT	maxNumVccDvRef; 
  ST_INT	maxNumVccDs;
  ST_INT	maxNumIccDvRef;
  ST_INT	maxNumIccDs;
  ST_INT    maxNumTsCtrl;
  ST_INT	numDvRefs;
  MIT_DVREF_INFO  *pstDvRefSet;
  ST_INT	numTransferSets;
  ST_INT	numStartedTransferSets;
  MIT_TRANSFER_SET  *pstTransferSets;
  MIT_TRANSFER_SET  *pstNextTransferSetToStart;
  ST_INT	numDataSets;
  ST_INT	numStartedDataSets;
  MIT_DATA_SET  *pstDataSets;
  MIT_DATA_SET  *DataSetStarting;   /* ref to read before starting next */
  MIT_DATA_SET  *DataSetDeleteing;   
  MIT_DATA_SET  *pstNextDataSetToStart;
  MIT_DEVICE_REF*pstDeviceSet;
  MIC_VCC	*pstMicVcc;  
  } MIT_MIC_VCC;

typedef struct Tag_MIT_ASSOC_INFO
  {
  DBL_LNK	  l;
  ST_CHAR	  associationName[MAX_ID_LEN];
  ST_CHAR	  remoteName[MAX_ID_LEN];
  ST_CHAR	  localArName[MAX_ID_LEN];
  MIT_AR_NAME	  *arList;
  ST_INT	  numArNames;
  ST_INT 	  ctrlFlags;
  ST_INT	  maxSegsize;
  ST_INT	  maxReqCalling;
  ST_INT	  maxReqCalled;
  ST_INT	  maxNest;
  ST_INT          numReq;
  MI_ASSOC_CTRL   *pstAssocCtrl;
  MIT_MIC_VCC	  *pstMitMicVcc;   	/* set at connect time */
  ST_INT	  connectBackOffTime;  
  } MIT_ASSOC_INFO;
 

/************************************************************************/
/* Global data structures and functions for points that change based	*/
/* on a time interval							*/
/************************************************************************/

/* values for chgMode */

#define CHG_NO_CHANGE  	0
#define CHG_TIME		1
#define	CHG_READ		2
#define	CHG_MIXED		3
#define	CHG_RANDOM		4

typedef struct	change_ctrl
  {
  ST_INT	chgMode;
  ST_INT	randomDeltaTime;
  time_t	nextRandomChange;
  ST_INT	lowRange;
  ST_INT	highRange;
  } CHANGE_CTRL;

/* values for ptMode */

#define NO_CHANGE		0
#define READ_CHANGE		1
#define	TIME_CHANGE		2

typedef struct	tag_CHANGE_POINT
  {
  ST_INT	deltaTime;
  ST_INT8	ptMode;
  time_t	lastChange;
  time_t	nextChange;
  MIT_DATA_VALUE *pstMitDataValue;
  } CHANGE_POINT;


/************************************************************************/
/* set up global variables						*/
/************************************************************************/

extern MIT_DATA_VALUE 	    *pstDataValueSet;
extern MIT_REMOTE     	    *pstRemoteSet;
extern MIT_INFO_MSG 	    *pstInfoMsgSet;	
extern MIT_ASSOC_INFO	    *pstAssocInfoSet;
extern MIT_MIS_VCC    	    *pstMisVccSet;
extern MIT_MIC_VCC    	    *pstMicVccSet;
extern MIT_DEVICE	    *pstDeviceSet;

extern CHANGE_CTRL	    mitChgCtrl;
extern CHANGE_POINT 	    *PtChgTbl;  /* pointer to the point change table */
extern ST_INT        	    numLocalDataValue;

extern ST_DOUBLE beginTime, endTime;	/* for elapsed time */
extern ST_VOID (*menu_set_fun)(ST_VOID);     	/* used to reset the menu */

/************************************************************************/
/* functions in mit_misc.c						*/
/*************************************************************************/

ST_VOID 	scrnLogOn  (ST_VOID);
ST_VOID 	scrnLogOff (ST_VOID);
ST_VOID 	showMITStatistics (ST_VOID);
ST_VOID 	do_debugset (ST_VOID);
ST_VOID 	showSizes (ST_VOID);
ST_VOID 	mit_app_dyn_log_fun (LOG_CTRL *lc, const ST_CHAR *timeStr, 
			     			 ST_INT logType, const ST_CHAR *logTypeStr,
                             const ST_CHAR *sourceFile, ST_INT lineNum,
                             ST_INT bufLen, const ST_CHAR *buf);
ST_VOID 	doReconfigureChgMode (ST_VOID);
ST_VOID		mitConfigurePtChgTbl (ST_VOID);
ST_VOID		updatePtChgTbl (ST_VOID);
ST_VOID		updatePt (MIT_DATA_VALUE *pstMitDataValue);
ST_VOID		doModifyAddressMatching (ST_VOID);

/************************************************************************/
/* functions in icfgx.c						*/
/************************************************************************/

ST_RET 		icfgXml 	(ST_VOID *userInfo, ST_CHAR *mitFileName);

/************************************************************************/
/* functions in mit_xmlc.c						*/
/************************************************************************/

ST_RET  mitConfigure_xml (ST_CHAR *xmlFileName, MVL_CFG_INFO *mvlCfg);

/************************************************************************/
/* functions in mit_util.c						*/
/************************************************************************/

MI_REMOTE *findRemote (ST_CHAR *remoteName);
MIT_REMOTE *findMitRemote (ST_CHAR *remoteName);
MIT_ASSOC_INFO *findAssociation(char *associationName);
MIT_MIC_VCC *findMitMicVcc (ST_CHAR *remoteName);
MIT_MIS_VCC *findMitMisVcc (ST_CHAR *remoteName);
ST_VOID *findDataSet (MIT_DATA_SET *pstDataSets, 
                      ST_CHAR *dataSetName);
MIC_DSTS 	*findTransferSet (MIT_TRANSFER_SET *pstTransferSets, 
                                  ST_CHAR *dsTransferSetName);
MIT_TRANSFER_SET *findMitTransferSet (MIT_TRANSFER_SET *pstTransferSets, 
                                      ST_CHAR *dsTransferSetName);
MIT_DATA_SET 	 *findMitDataSet (MIT_DATA_SET  *pstDataSets, ST_CHAR *dsName);

ST_VOID 	startInterval	(ST_VOID);
ST_DOUBLE 	endInterval	(ST_VOID);
ST_VOID 	pause_msg 	(ST_CHAR *str);
ST_VOID 	do_dyn_mem_chk (ST_VOID);

/************************************************************************/
/* functions in mit_ssoc.c						*/
/************************************************************************/

ST_VOID 	startAssociation (MIT_ASSOC_INFO *pstMitAssoc);
ST_VOID 	startAssociations (ST_VOID);
ST_VOID 	stopAssociations  (ST_VOID);
ST_VOID 	showAssocSummary  (ST_VOID);
ST_VOID  	displayConfiguredRemotes (ST_VOID);
MIT_ASSOC_INFO	*getAssociationName ( ST_CHAR *buffer );
ST_RET u_mvl_connect_ind (MVL_NET_INFO *net_info, INIT_INFO *init_ind_info);
ST_VOID		setReqCapacity(MIT_ASSOC_INFO *pstMitAssoc);
ST_BOOLEAN	hasReqCapacity(MIT_ASSOC_INFO *pstMitAssoc);
ST_VOID		incReqCapacity(MIT_ASSOC_INFO *pstMitAssoc);
ST_VOID		decReqCapacity(MIT_ASSOC_INFO *pstMitAssoc);

/************************************************************************/
/* functions in mit_var.c						*/
/************************************************************************/

ST_RET     createDataSets (MIT_ASSOC_INFO *pstMitAssoc);
ST_RET 	   createDs (MI_CONN *mi_conn, MIT_DATA_SET *pstMitDataSet,
			  MIT_ASSOC_INFO *pstMitAssoc);
ST_CHAR	   *iccp_scope_to_str (ST_INT scope);
ST_VOID    display_mic_dv_table (MIC_VCC *mic_vcc);
ST_RET     createTransferSets (MIT_ASSOC_INFO *pstMitAssoc);
ST_RET 	   startTransferSet (MI_CONN *mi_conn, 
                             MIT_TRANSFER_SET *pstMitTransferSet,
	                     MIT_ASSOC_INFO *pstMitAssoc);
ST_RET     createTransferSet (MIT_ASSOC_INFO *pstMitAssoc,
		  	          MIT_TRANSFER_SET *pstCurTransferSet);
ST_VOID    displayTransferSetNames (ST_VOID);
ST_VOID    readRemoteDataValues	(ST_VOID);
ST_VOID    writeRemoteDataValues	(ST_VOID);
ST_VOID	   displayRemoteDataValueNames (ST_VOID);
ST_VOID    readRemoteDataSet	(ST_VOID);
ST_VOID    writeRemoteDataSet	(ST_VOID);
ST_VOID	   displayRemoteDataSetNames (ST_VOID);
ST_VOID    displayLocalDataValue	(ST_VOID);
ST_VOID    modifyLocalDatavalue	(ST_VOID);
ST_VOID	   displayLocalDataValueNames (ST_VOID);
ST_VOID	   displayRemoteDataSetAttributes(ST_VOID);
MIT_MIC_VCC	*getRemoteMitMicVcc (ST_CHAR *buffer);
MIT_MIS_VCC	*getRemoteMitMisVcc (ST_CHAR *buffer);
ST_VOID    getRemoteDataValueAttributes (ST_VOID);
ST_VOID	   logLocalDataSetNames (ST_VOID);
ST_VOID    getRemoteDataValueNames(ST_VOID);
ST_VOID    getRemoteDataSetElementNames(ST_VOID);
ST_VOID    createRemoteDataSet(ST_VOID);
ST_VOID    deleteRemoteDataSet(ST_VOID);
ST_VOID    doReconfigurePtChgTbl (ST_VOID);
ST_VOID    doForcePtChg (ST_VOID);


ST_VOID usr_mis_service (ST_VOID);

ST_VOID    incrementDataValue (ST_INT mi_type, MI_DV_DATA_UNION *pData);

/************************************************************************/
/* functions in mit_tran.c						*/
/************************************************************************/

ST_VOID    startTransferSets (ST_VOID);
ST_VOID    stopTransferSets (ST_VOID);
ST_VOID	   displayRemoteTransferSetNames(ST_VOID);
ST_VOID	   displayRemoteTransferSetAttributes (ST_VOID);

/************************************************************************/
/* functions in mit_imsg.c						*/
/************************************************************************/

ST_VOID	   enableInformationMessages(MIT_ASSOC_INFO*);
ST_VOID    startInformationMessages (ST_VOID);
ST_VOID    stopInformationMessages (ST_VOID);
ST_VOID    sendInformationMessage (ST_VOID);
ST_VOID	   displayRemoteInfoMsgs(ST_VOID);

/************************************************************************/
/* functions in mit_devc.c						*/
/************************************************************************/

ST_VOID    selectDevice (ST_VOID);
ST_VOID    operateDevice (ST_VOID);
ST_VOID    setDeviceTag (ST_VOID);
ST_VOID    getDeviceTag (ST_VOID);
ST_VOID    displayLocalDevices (ST_VOID);
ST_VOID    displayRemoteDevices (ST_VOID);

/************************************************************************/
/* misc functions 	  						*/
/************************************************************************/

ST_VOID get_ldebug (ST_ULONG *sel_ptr, ST_ULONG mask, ST_CHAR *text);

#if !defined(_WIN32)
ST_VOID	set_echo_off (ST_VOID);
#endif

#ifdef _WIN32
int gettimeofday( struct timeval *, void * );   
#endif

#endif		/* end #ifndef MIT_MAIN_INCLUDED		*/



