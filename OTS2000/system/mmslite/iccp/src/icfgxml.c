/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.        */
/*              2000 - 2002, All Rights Reserved                        */
/*                                                                      */
/* MODULE NAME : icfgXml.c                                              */
/* PRODUCT(S)  : ICCP                                                   */
/*                                                                      */
/* MODULE DESCRIPTION : This routine parses the specified XML           */
/*                      configuration file and places the data          */
/*                      found there into the appropriate places         */
/*                      in the ICCP configuration data structure.       */
/*                                                                      */
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :                            */
/*                                                                      */
/* MODIFICATION LOG                                                     */
/*  Date     Who   Rev          Comments                                */
/* --------  ---  ------   -------------------------------------------  */
/* 07/02/02  GLB    23     Remove unreferenced getFileSize              */
/* 06/20/02  RKR    22     Copyright year update                        */
/* 05/08/02  GLB    21     Discrete/Real min & max can be less than zero*/
/* 04/29/02  GLB    20     Change asciiTo.. to sx_get_...               */
/* 04/26/02  MDE    19     Set default Tle, set DSCondReq bits if 0	*/
/* 04/25/02  MDE    18     Added DSConditions, Tle			*/
/* 04/25/02  GLB    17     Added _icfgGetYesNo                          */
/* 04/17/02  MDE    16     Added remote DV config options  		*/
/* 02/27/02  GLB    15     Changed ascii_to_float to asciiToFloat       */
/* 02/12/02  GLB    14     Set default in appropriate list      	*/
/* 01/22/02  GLB    13     Change mandatory/optional to match DTD       */
/*                         Change order of code to accommodate freeing  */
/* 01/10/02  GLB    12     Added sx_pop                                 */
/*                         Made Association Description optional        */
/*                         Made BilateralTable Description optional     */
/*                         Remove check for Rev 11 default maxes        */
/* 12/27/01  GLB    11	   Added Default values for MaxReqPend,	        */
/*			    MaxIndPend and MaxNest.			*/
/* 12/18/01  GLB    10     If connectRole='called' allow RetryTime      */
/*                          to be entered				*/
/*                         if connectRole = "calling" and no RetryTime  */
/*                          use default value for RetryTime             */                            
/* 12/17/01  GLB    09     If Rbe='N' allow BufferTime and/or Integrity */
/*                          to be entered				*/
/*                         if Rbe='Y' and no BufferTime and/or Integrity*/
/*                          use default values                          */                              
/* 12/16/01  GLB    08     If Sbo='N' allow checkBackId and/or selTime  */
/*                          to be entered				*/
/*                         if Sbo='Y' and no checkBackId and/or selTime */
/*                          use default values                          */                               
/* 12/12/01  GLB    07     Changed ascii_to_.. to asciiTo..             */
/* 12/10/01  GLB    06     Added State min & max                        */
/* 12/09/01  GLB    05     Added ShortestInterval min of 1              */
/* 11/28/01  GLB    04     Changed parsed numbers declatations to match */
/*                          IccpCfg.xsd schema definitions              */
/* 11/15/01  DGE    03	   Changed NormalValueMin and NormalValueMax	*/
/*			    from double to 'float.		        */
/*			   Changed intVals to discreteVals and		*/
/*			    doubleVals to realVals.			*/
/* 11/12/01  DGE    02	   Removed declarations for logging strings and	*/
/*			    enum string arrays and placed them in a	*/
/*			    separate common module icfg.c.		*/
/* 01/18/01  GLB    01     Module created                               */
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h" 
#include "str_util.h"
#include "sx_defs.h"
#include "sx_log.h"
#include "gen_list.h"
#include "mem_chk.h"

#include "icfg.h"

#include <string.h>           
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#ifdef _WIN32
#include <io.h>
#endif


#define  EMPTY_STRING         ""

/* object types */
#define  DV                    0
#define  DEV                   1
#define  IM                    2
#define  DS                    3
#define  DSTS                  4


/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of      */
/* __FILE__ strings.                                                    */

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif



/************************************************************************/
/*  The following subroutines are utilizeded during parsing.            */
/************************************************************************/

static ST_VOID _localControlCenter_SEFun   (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _localCcName_EFun           (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _localMaxDsTs_EFun          (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _localMaxDataSets_EFun      (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _localMaxMmsMsgSize_EFun    (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _localNumDv_EFun            (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _localDescription_EFun      (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _localObjects_SEFun         (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _localDataValues_SEFun      (SX_DEC_CTRL *sxDecCtrl);
                                            
static ST_VOID _ldv_SERFun                 (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _ldvName_EFun               (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldvDvType_EFun             (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldvMapInfo_EFun            (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldvNormalSource_EFun       (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldvRealLimits_EFun         (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldvDiscreteLimits_EFun     (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldvStateLimits_EFun        (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldvDescription_EFun        (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _ldvRealNValMin_EFun        (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldvRealNValMax_EFun        (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _ldvDiscreteNValMin_EFun    (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldvDiscreteNValMax_EFun    (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _ldvStateNValMin_EFun       (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldvStateNValMax_EFun       (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _localDevices_SEFun         (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _ldev_SERFun                (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _ldevDeviceName_EFun        (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldevDevType_EFun           (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldevMapInfo_EFun           (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldevSbo_EFun               (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldevSelTime_EFun           (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldevCheckBackId_EFun       (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldevTagEn_EFun             (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _ldevDescription_EFun       (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _localInfoMsgs_SEFun        (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _lim_SERFun                 (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _limInfoRef_EFun            (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _limMapInfo_EFun            (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _limMaxSize_EFun            (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _limDescription_EFun        (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _remoteControlCenters_SEFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _remoteControlCenter_SERFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _remoteCcName_EFun          (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _bilateralTable_SEFun       (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _localDomain_EFun           (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _remoteDomain_EFun          (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _shortestInterval_EFun      (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _blocks_EFun                (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _remDvValidation_EFun       (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _remDvGetConfig_EFun        (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _remDvExcludeInvalidFromDs_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _remoteDescription_EFun     (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _associations_SEFun         (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _bltId_EFun                 (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _bltName_EFun               (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _bltDescription_EFun        (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _association_SERFun         (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _assocName_EFun             (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _localAr_EFun               (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _remoteAr_ERFun             (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _connectRole_ERFun          (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _serviceRole_ERFun          (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _assocRetryTime_EFun        (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _remoteMaxMmsMsgSize_EFun   (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _maxReqPend_EFun            (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _maxIndPend_EFun            (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _maxNest_EFun               (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _assocDescription_EFun      (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _serverObjects_SEFun        (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _serverNumVccDv_EFun        (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _serverNumVccDev_EFun       (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _serverNumVccInfoMsg_EFun   (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _serverNumVccDs_EFun        (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _serverNumIccDv_EFun        (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _serverNumIccDev_EFun       (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _serverNumIccInfoMsg_EFun   (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _serverNumIccDs_EFun        (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _serverDataValues_SEFun     (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _sdv_SERFun                 (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _sdvObjName_EFun            (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sdvDvType_EFun             (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sdvReadOnly_EFun           (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sdvMapInfo_EFun            (SX_DEC_CTRL *sxDecCtrl);
                      
static ST_VOID _serverDevices_SEFun        (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _sdev_SERFun                (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _sdevObjDeviceName_EFun     (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sdevDevType_EFun           (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sdevMapInfo_EFun           (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sdevTagEn_EFun             (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _serverInfoMsgs_SEFun       (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _sim_SERFun                 (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _simObjInfoRef_EFun         (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _simMapInfo_EFun            (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _serverDataSets_SEFun       (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _sds_SERFun                 (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _sdsObjName_EFun            (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sdsTransferSetName_EFun       (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sdsTransferSetTimeStamp_EFun  (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sdsDsConditionsDetected_EFun  (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sdsEventCodeDetected_EFun     (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sdsDvs_SEFun               (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sdsDescription_EFun        (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _sDsDv_ERFun                (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _clientObjects_SEFun        (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _clientNumVccDv_EFun        (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _clientNumVccDev_EFun       (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _clientNumVccInfoMsg_EFun   (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _clientNumIccDv_EFun        (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _clientNumIccDev_EFun       (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _clientNumIccInfoMsg_EFun   (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _clientNumDs_EFun           (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _clientNumDsTs_EFun         (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _clientDataValues_SEFun     (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _cdv_SERFun                 (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _cdvObjName_EFun            (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdvDvType_EFun             (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdvReadOnly_EFun           (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdvMapInfo_EFun            (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdvDescription_EFun        (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _clientDevices_SEFun        (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _cdev_SERFun                (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _cdevObjDeviceName_EFun     (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdevDevType_EFun           (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdevMapInfo_EFun           (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdevSbo_EFun               (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdevCheckBackId_EFun       (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdevTagEn_EFun             (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdevDescription_EFun       (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _clientInfoMsgs_SEFun       (SX_DEC_CTRL *sxDecCtrl);
                                                                                 
static ST_VOID _cim_SERFun                 (SX_DEC_CTRL *sxDecCtrl);
                                             
static ST_VOID _cimObjInfoRef_EFun         (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cimMapInfo_EFun            (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cimMaxSize_EFun            (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cimDescription_EFun        (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _clientDataSets_SEFun       (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _cds_SERFun                 (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _cdsName_EFun               (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdsTransferSetName_EFun       (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdsTransferSetTimeStamp_EFun  (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdsDsConditionsDetected_EFun  (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdsEventCodeDetected_EFun     (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdsDescription_EFun        (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _cdsDvs_SEFun               (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _cdsDv_ERFun                (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _clientDSTSs_SEFun          (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _cdsts_SERFun               (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _cdstsDsName_EFun           (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdstsAssociationName_EFun  (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdstsInterval_EFun         (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdstsRbe_EFun              (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdstsBufferTime_EFun       (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdstsIntegrity_EFun        (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdstsStartTime_EFun        (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdstsCritical_EFun         (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdstsTle_EFun		   (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdstsDsCondInterval_EFun   (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdstsDsCondIntegrity_EFun  (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdstsDsCondChange_EFun	   (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdstsDsCondOperator_EFun   (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdstsDsCondExternal_EFun   (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _cdstsDescription_EFun      (SX_DEC_CTRL *sxDecCtrl);


/************************************************************************/
/*  determine if "yes" or "no" was entered                              */
/************************************************************************/

static ST_VOID _icfgGetYesNo (SX_DEC_CTRL *sxDecCtrl, ST_BOOLEAN *dest, 
		      ST_CHAR *text);

/************************************************************************/
/*  obtain value of element attribute "Count"                           */
/************************************************************************/

static ST_RET  _getListCount (SX_DEC_CTRL *sxDecCtrl, ST_INT *countOut);

/************************************************************************/
/*  parts of ICCP cfg. data structure used to store data during parsing */
/************************************************************************/

static ICFG_LOCAL_CC          *icfgLocalCc; 

static ICFG_LOCAL_DV          *localDvList;
static ICFG_LOCAL_DEV         *localDevList;
static ICFG_LOCAL_INFOMSG     *localInfoMsgList;
static ICFG_REMOTE_CC         *remoteCcList;
static ICFG_ASSOCIATION       *assocList;

static ICFG_SERVER_OBJ_COUNTS *serverObjCounts; 
static ICFG_SERVER_OBJ_COUNTS sServerObjCounts; 
static ICFG_SERVER_DV         *serverDvList;
static ICFG_SERVER_DEV        *serverDevList;
static ICFG_SERVER_INFOMSG    *serverInfoMsgList;
static ICFG_SERVER_DS         *serverDsList;
static ICFG_SERVER_DS_DV      *serverDsDvList;

static ICFG_CLIENT_OBJ_COUNTS *clientObjCounts; 
static ICFG_CLIENT_OBJ_COUNTS cClientObjCounts; 
static ICFG_CLIENT_DV         *clientDvList;
static ICFG_CLIENT_DEV        *clientDevList;
static ICFG_CLIENT_INFOMSG    *clientInfoMsgList;
static ICFG_CLIENT_DS         *clientDsList;
static ICFG_CLIENT_DS_DV      *clientDsDvList;
static ICFG_CLIENT_DSTS       *clientDsTsList;

/************************************************************************/
/* for the user to pass needed information to the callback routines     */
/************************************************************************/

  ST_VOID *userInfo;

/************************************************************************/
/* These variables are used to verify that the actual count specified   */
/* at the beginning of ServerObjects and ClientObjects sections is the  */
/* same as the calculated count these variables will contain            */                                            
/************************************************************************/

  ST_INT  numVccDv;
  ST_INT  numVccDev;
  ST_INT  numVccInfoMsg;
  ST_INT  numVccDs;

  ST_INT  numIccDv;
  ST_INT  numIccDev;
  ST_INT  numIccInfoMsg;
  ST_INT  numIccDs;

/************************************************************************/
/* These variables are used to verify that the "Count" specified for:   */
/* RemoteControlCenters, Associations, ServerDataValues, ServerDevices, */
/* ServerInfoMsgs, ServerDataSets, SdsDvs, ClientDataValues,            */
/* ClientDevices, ClientInfoMsgs, ClientDataSets,                       */
/* ClientDataSetTransferSets and CdsDvs is the same as the calculated   */
/* count these variables will contain                                   */
/************************************************************************/

  ST_INT  actualCount;
  ST_INT  rCCActualCount;
  ST_INT  numRcc;   
  ST_INT  numAssoc;
  ST_INT  numDv;
  ST_INT  numDev;   
  ST_INT  numInfoMsg;
  ST_INT  numDs;   
  ST_INT  sdsDvsActualCount;
  ST_INT  sdsDvsMaxCount;
  ST_INT  cdsDvsActualCount;
  ST_INT  cdsDvsMaxCount;
  ST_INT  numDsTs;

/************************************************************************/
/*  Used to determine if info message should be printed                 */
/************************************************************************/

  ST_INT  chkBackIdEntered;
  ST_INT  selTimeEntered;
  ST_INT  bufferTimeEntered;
  ST_INT  integrityCheckEntered;
  ST_INT  assocRetryDelayEntered;

/************************************************************************/
/*  Elements found in the XML file to be parsed                         */
/************************************************************************/

SX_ELEMENT iCfgStartElement[] = 
{
  {"LocalControlCenter",    SX_ELF_CSTART | SX_ELF_CEND,                       _localControlCenter_SEFun}
};
  
SX_ELEMENT localCtrlCtrElements[] = 
{
  {"Name",                      SX_ELF_CEND,                   _localCcName_EFun},
  {"MaxDsTs",                   SX_ELF_CEND,                   _localMaxDsTs_EFun},
  {"MaxDataSets",               SX_ELF_CEND,                   _localMaxDataSets_EFun},
  {"MaxMmsMsgSize",             SX_ELF_CEND,                   _localMaxMmsMsgSize_EFun},
  {"Description",               SX_ELF_CEND | SX_ELF_OPT,      _localDescription_EFun},
  {"LocalObjects",              SX_ELF_CSTART | SX_ELF_CEND,   _localObjects_SEFun},
  {"RemoteControlCenters",      SX_ELF_CSTART | SX_ELF_CEND,   _remoteControlCenters_SEFun}
};

SX_ELEMENT localObjectsElements[] = 
{
  {"LocalDataValues",           SX_ELF_CSTART | SX_ELF_CEND,   _localDataValues_SEFun},
  {"LocalDevices",              SX_ELF_CSTART | SX_ELF_CEND,   _localDevices_SEFun},
  {"LocalInfoMsgs",             SX_ELF_CSTART | SX_ELF_CEND,   _localInfoMsgs_SEFun}
};
									
SX_ELEMENT ldvElement[] = 
{
  {"Ldv",                       SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPTRPT,   _ldv_SERFun}
};

SX_ELEMENT ldvElements[] = 
{
  {"Name",                      SX_ELF_CEND,                   _ldvName_EFun},
  {"DataType",                  SX_ELF_CEND,                   _ldvDvType_EFun},
  {"MapInfo",                   SX_ELF_CEND | SX_ELF_OPT,      _ldvMapInfo_EFun},
  {"NormalSource",              SX_ELF_CEND,                   _ldvNormalSource_EFun},
  {"RealLimits",                SX_ELF_CSTART | SX_ELF_OPT,    _ldvRealLimits_EFun},
  {"DiscreteLimits",            SX_ELF_CSTART | SX_ELF_OPT,    _ldvDiscreteLimits_EFun},
  {"StateLimits",               SX_ELF_CSTART | SX_ELF_OPT,    _ldvStateLimits_EFun},
  {"Description",               SX_ELF_CEND | SX_ELF_OPT,      _ldvDescription_EFun}
};

SX_ELEMENT ldvRealLimitElements[] = 
{
  {"RealNValMin",               SX_ELF_CEND | SX_ELF_OPT,      _ldvRealNValMin_EFun},
  {"RealNValMax",               SX_ELF_CEND | SX_ELF_OPT,      _ldvRealNValMax_EFun}
};

SX_ELEMENT ldvDiscreteLimitElements[] = 
{
  {"DiscreteNValMin",           SX_ELF_CEND | SX_ELF_OPT,      _ldvDiscreteNValMin_EFun},
  {"DiscreteNValMax",           SX_ELF_CEND | SX_ELF_OPT,      _ldvDiscreteNValMax_EFun}
};

SX_ELEMENT ldvStateLimitElements[] = 
{
  {"StateNValMin",              SX_ELF_CEND | SX_ELF_OPT,      _ldvStateNValMin_EFun},
  {"StateNValMax",              SX_ELF_CEND | SX_ELF_OPT,      _ldvStateNValMax_EFun}
};

SX_ELEMENT ldevElement[] = 
{
  {"Ldev",                      SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPTRPT,   _ldev_SERFun}
};

SX_ELEMENT ldevElements[] = 
{                           
  {"DeviceName",                SX_ELF_CEND,                   _ldevDeviceName_EFun},
  {"DeviceType",                SX_ELF_CEND,                   _ldevDevType_EFun},
  {"MapInfo",                   SX_ELF_CEND | SX_ELF_OPT,      _ldevMapInfo_EFun},
  {"Sbo",                       SX_ELF_CEND,                   _ldevSbo_EFun},
  {"ChkBackId",                 SX_ELF_CEND | SX_ELF_OPT,      _ldevCheckBackId_EFun},
  {"SelTime",                   SX_ELF_CEND | SX_ELF_OPT,      _ldevSelTime_EFun},
  {"TagEn",                     SX_ELF_CEND,                   _ldevTagEn_EFun},
  {"Description",               SX_ELF_CEND | SX_ELF_OPT,      _ldevDescription_EFun}
};

SX_ELEMENT limElement[] = 
{                            
 {"Lim",                        SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPTRPT,   _lim_SERFun}
};									      
SX_ELEMENT limElements[] = 
{
  {"InfoRef",                   SX_ELF_CEND,                   _limInfoRef_EFun},
  {"MapInfo",                   SX_ELF_CEND | SX_ELF_OPT,      _limMapInfo_EFun},
  {"MaxSize",                   SX_ELF_CEND,                   _limMaxSize_EFun},
  {"Description",               SX_ELF_CEND | SX_ELF_OPT,      _limDescription_EFun}
};                                                            

SX_ELEMENT remoteCtrlCtrElement[] = 
{
  {"RemoteControlCenter",       SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPTRPT,   _remoteControlCenter_SERFun}
};

SX_ELEMENT remoteCtrlCtrElements[] = 
{
  {"Name",                      SX_ELF_CEND,                   _remoteCcName_EFun},
  {"Description",               SX_ELF_CEND | SX_ELF_OPT,      _remoteDescription_EFun},
  {"BilateralTable",            SX_ELF_CSTART | SX_ELF_CEND,   _bilateralTable_SEFun},
  {"Associations",              SX_ELF_CSTART | SX_ELF_CEND,   _associations_SEFun},
  {"ServerObjects",             SX_ELF_CSTART | SX_ELF_CEND,   _serverObjects_SEFun},
  {"ClientObjects",             SX_ELF_CSTART | SX_ELF_CEND,   _clientObjects_SEFun}
};


SX_ELEMENT bilateralTableElements[] = 
{
  {"Id",                        SX_ELF_CEND,                   _bltId_EFun},
  {"Name",                      SX_ELF_CEND,                   _bltName_EFun},
  {"LocalDomain",               SX_ELF_CEND | SX_ELF_OPT,      _localDomain_EFun},
  {"RemoteDomain",              SX_ELF_CEND | SX_ELF_OPT,      _remoteDomain_EFun},
  {"ShortestInterval",          SX_ELF_CEND,                   _shortestInterval_EFun},
  {"Blocks",                    SX_ELF_CEND,                   _blocks_EFun},
  {"RemDvValidation",           SX_ELF_CEND | SX_ELF_OPT,      _remDvValidation_EFun},
  {"RemDvGetConfig",            SX_ELF_CEND | SX_ELF_OPT,      _remDvGetConfig_EFun},
  {"RemDvExcludeInvalidFromDs", SX_ELF_CEND | SX_ELF_OPT,      _remDvExcludeInvalidFromDs_EFun},
  {"Description",               SX_ELF_CEND | SX_ELF_OPT,      _bltDescription_EFun},
};


SX_ELEMENT associationsElements[] = 
{
  {"Association",               SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPTRPT,   _association_SERFun}
};

SX_ELEMENT associationElements[] = 
{
  {"Name",                      SX_ELF_CEND,                   _assocName_EFun},
  {"LocalAr",                   SX_ELF_CEND,                   _localAr_EFun},
  {"RemoteAr",                  SX_ELF_CEND | SX_ELF_RPT,      _remoteAr_ERFun},
  {"ConnectRole",               SX_ELF_CEND,                   _connectRole_ERFun},
  {"ServiceRole",               SX_ELF_CEND | SX_ELF_RPT,      _serviceRole_ERFun},
  {"AssocRetryTime",            SX_ELF_CEND | SX_ELF_OPT,      _assocRetryTime_EFun},
  {"MaxMmsMsgSize",             SX_ELF_CEND,                   _remoteMaxMmsMsgSize_EFun},
  {"MaxReqPend",                SX_ELF_CEND,                   _maxReqPend_EFun},
  {"MaxIndPend",                SX_ELF_CEND,                   _maxIndPend_EFun},
  {"MaxNest",                   SX_ELF_CEND,                   _maxNest_EFun},
  {"Description",               SX_ELF_CEND | SX_ELF_OPT,      _assocDescription_EFun}
};

SX_ELEMENT serverObjsElements[] = 
{
  {"NumVccDv",                  SX_ELF_CEND,                   _serverNumVccDv_EFun},
  {"NumVccDev",                 SX_ELF_CEND,                   _serverNumVccDev_EFun},
  {"NumVccInfoMsg",             SX_ELF_CEND,                   _serverNumVccInfoMsg_EFun},
  {"NumVccDs",                  SX_ELF_CEND,                   _serverNumVccDs_EFun},
  {"NumIccDv",                  SX_ELF_CEND,                   _serverNumIccDv_EFun},
  {"NumIccDev",                 SX_ELF_CEND,                   _serverNumIccDev_EFun},
  {"NumIccInfoMsg",             SX_ELF_CEND,                   _serverNumIccInfoMsg_EFun},
  {"NumIccDs",                  SX_ELF_CEND,                   _serverNumIccDs_EFun},
  {"ServerDataValues",          SX_ELF_CSTART | SX_ELF_CEND,   _serverDataValues_SEFun},
  {"ServerDevices",             SX_ELF_CSTART | SX_ELF_CEND,   _serverDevices_SEFun},
  {"ServerInfoMsgs",            SX_ELF_CSTART | SX_ELF_CEND,   _serverInfoMsgs_SEFun},
  {"ServerDataSets",            SX_ELF_CSTART | SX_ELF_CEND,   _serverDataSets_SEFun}
};

SX_ELEMENT sdvElement[] = 
{
  {"Sdv",                       SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPTRPT,   _sdv_SERFun}
};

SX_ELEMENT sdvElements[] = 
{
  {"ObjName",                   SX_ELF_CEND,                   _sdvObjName_EFun},
  {"DataType",                  SX_ELF_CEND | SX_ELF_OPT,      _sdvDvType_EFun},
  {"ReadOnly",                  SX_ELF_CEND,                   _sdvReadOnly_EFun},
  {"MapInfo",                   SX_ELF_CEND | SX_ELF_OPT,      _sdvMapInfo_EFun}
};

SX_ELEMENT sdevElement[] = 
{
  {"Sdev",                      SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPTRPT,   _sdev_SERFun}
};

SX_ELEMENT sdevElements[] = 
{
  {"ObjDeviceName",             SX_ELF_CEND,                   _sdevObjDeviceName_EFun},
  {"DeviceType",                SX_ELF_CEND,                   _sdevDevType_EFun},
  {"MapInfo",                   SX_ELF_CEND | SX_ELF_OPT,      _sdevMapInfo_EFun},
  {"TagEn",                     SX_ELF_CEND,                   _sdevTagEn_EFun}
};

SX_ELEMENT simElement[] = 
{
  {"Sim",                       SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPTRPT,   _sim_SERFun}
};

SX_ELEMENT simElements[] = 
{
  {"ObjInfoRef",                SX_ELF_CEND,                   _simObjInfoRef_EFun},
  {"MapInfo",                   SX_ELF_CEND | SX_ELF_OPT,      _simMapInfo_EFun}
};

SX_ELEMENT sdsElement[] = 
{
  {"Sds",                       SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPTRPT,   _sds_SERFun}
};

SX_ELEMENT sdsElements[] = 
{
  {"ObjName",                   SX_ELF_CEND,                   _sdsObjName_EFun},
  {"Transfer_Set_Name",		SX_ELF_CEND,		       _sdsTransferSetName_EFun},
  {"Transfer_Set_Time_Stamp",   SX_ELF_CEND,		       _sdsTransferSetTimeStamp_EFun},
  {"DSConditions_Detected",	SX_ELF_CEND,		       _sdsDsConditionsDetected_EFun},
  {"Event_Code_Detected",	SX_ELF_CEND,		       _sdsEventCodeDetected_EFun},
  {"Description",               SX_ELF_CEND | SX_ELF_OPT,      _sdsDescription_EFun},
  {"SdsDvs",                    SX_ELF_CSTART | SX_ELF_CEND,   _sdsDvs_SEFun}
};

SX_ELEMENT sdsDvsElement[] = 
{
  {"SdsDv",                     SX_ELF_CEND | SX_ELF_OPTRPT,                   _sDsDv_ERFun}
};                                                            

SX_ELEMENT clientObjsElements[] = 
{
  {"NumVccDv",                  SX_ELF_CEND,                   _clientNumVccDv_EFun},
  {"NumVccDev",                 SX_ELF_CEND,                   _clientNumVccDev_EFun},
  {"NumVccInfoMsg",             SX_ELF_CEND,                   _clientNumVccInfoMsg_EFun},
  {"NumIccDv",                  SX_ELF_CEND,                   _clientNumIccDv_EFun},
  {"NumIccDev",                 SX_ELF_CEND,                   _clientNumIccDev_EFun},
  {"NumIccInfoMsg",             SX_ELF_CEND,                   _clientNumIccInfoMsg_EFun},
  {"NumDs",                     SX_ELF_CEND,                   _clientNumDs_EFun},
  {"NumDsTs",                   SX_ELF_CEND,                   _clientNumDsTs_EFun},
  {"ClientDataValues",          SX_ELF_CSTART | SX_ELF_CEND,   _clientDataValues_SEFun},
  {"ClientDevices",             SX_ELF_CSTART | SX_ELF_CEND,   _clientDevices_SEFun},
  {"ClientInfoMsgs",            SX_ELF_CSTART | SX_ELF_CEND,   _clientInfoMsgs_SEFun},
  {"ClientDataSets",            SX_ELF_CSTART | SX_ELF_CEND,   _clientDataSets_SEFun},
  {"ClientDataSetTransferSets", SX_ELF_CSTART | SX_ELF_CEND,   _clientDSTSs_SEFun}
};

SX_ELEMENT cdvElement[] = 
{
  {"Cdv",                       SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPTRPT,   _cdv_SERFun}
};

SX_ELEMENT cdvElements[] = 
{
  {"ObjName",                   SX_ELF_CEND,                   _cdvObjName_EFun},
  {"DataType",                  SX_ELF_CEND,                   _cdvDvType_EFun},
  {"ReadOnly",                  SX_ELF_CEND,                   _cdvReadOnly_EFun},
  {"MapInfo",                   SX_ELF_CEND | SX_ELF_OPT,      _cdvMapInfo_EFun},
  {"Description",               SX_ELF_CEND | SX_ELF_OPT,      _cdvDescription_EFun}
};

SX_ELEMENT cdevElement[] = 
{
  {"Cdev",                      SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPTRPT,   _cdev_SERFun}
};

SX_ELEMENT cdevElements[] = 
{
  {"ObjDeviceName",             SX_ELF_CEND,                   _cdevObjDeviceName_EFun},
  {"DeviceType",                SX_ELF_CEND,                   _cdevDevType_EFun},
  {"MapInfo",                   SX_ELF_CEND | SX_ELF_OPT,      _cdevMapInfo_EFun},
  {"Sbo",                       SX_ELF_CEND,                   _cdevSbo_EFun},
  {"ChkBackId",                 SX_ELF_CEND | SX_ELF_OPT,      _cdevCheckBackId_EFun},
  {"TagEn",                     SX_ELF_CEND,                   _cdevTagEn_EFun},
  {"Description",               SX_ELF_CEND | SX_ELF_OPT,      _cdevDescription_EFun}
};

SX_ELEMENT cimElement[] = 
{
  {"Cim",                       SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPTRPT,   _cim_SERFun}
};

SX_ELEMENT cimElements[] = 
{
  {"ObjInfoRef",                SX_ELF_CEND,                   _cimObjInfoRef_EFun},
  {"MapInfo",                   SX_ELF_CEND | SX_ELF_OPT,      _cimMapInfo_EFun},
  {"MaxSize",                   SX_ELF_CEND,                   _cimMaxSize_EFun},
  {"Description",               SX_ELF_CEND | SX_ELF_OPT,      _cimDescription_EFun}
};

SX_ELEMENT cdsElement[] = 
{
  {"Cds",                       SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPTRPT,   _cds_SERFun}
};

SX_ELEMENT cdsElements[] = 
{
  {"Name",                      SX_ELF_CEND,                   _cdsName_EFun},
  {"Transfer_Set_Name",			SX_ELF_CEND,				   _cdsTransferSetName_EFun},
  {"Transfer_Set_Time_Stamp",	SX_ELF_CEND,				   _cdsTransferSetTimeStamp_EFun},
  {"DSConditions_Detected",		SX_ELF_CEND,				   _cdsDsConditionsDetected_EFun},
  {"Event_Code_Detected",	    SX_ELF_CEND,				   _cdsEventCodeDetected_EFun},
  {"Description",               SX_ELF_CEND | SX_ELF_OPT,      _cdsDescription_EFun},
  {"CdsDvs",                    SX_ELF_CSTART | SX_ELF_CEND,   _cdsDvs_SEFun}
};                             

SX_ELEMENT cdsDvElement[] = 
{
  {"CdsDv",                     SX_ELF_CEND | SX_ELF_OPTRPT,                   _cdsDv_ERFun}
};

SX_ELEMENT cdstsElement[] = 
{
  {"Cdsts",                     SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPTRPT,   _cdsts_SERFun}
};										
SX_ELEMENT cdstsElements[] = 
{
  {"DsName",                    SX_ELF_CEND,                   _cdstsDsName_EFun},
  {"AssocName",                 SX_ELF_CEND,                   _cdstsAssociationName_EFun},
  {"Interval",                  SX_ELF_CEND,                   _cdstsInterval_EFun},
  {"Rbe",                       SX_ELF_CEND,                   _cdstsRbe_EFun},
  {"BufferTime",                SX_ELF_CEND | SX_ELF_OPT,      _cdstsBufferTime_EFun},
  {"Integrity",                 SX_ELF_CEND | SX_ELF_OPT,      _cdstsIntegrity_EFun},
  {"StartTime",                 SX_ELF_CEND,                   _cdstsStartTime_EFun},
  {"Critical",                  SX_ELF_CEND,                   _cdstsCritical_EFun},
  {"Tle",                 	SX_ELF_CEND | SX_ELF_OPT,      _cdstsTle_EFun},
  {"DsCondInterval",            SX_ELF_CEND | SX_ELF_OPT,      _cdstsDsCondInterval_EFun},
  {"DsCondIntegrity",           SX_ELF_CEND | SX_ELF_OPT,      _cdstsDsCondIntegrity_EFun},
  {"DsCondChange",              SX_ELF_CEND | SX_ELF_OPT,      _cdstsDsCondChange_EFun},
  {"DsCondOperator",            SX_ELF_CEND | SX_ELF_OPT,      _cdstsDsCondOperator_EFun},
  {"DsCondExternal",            SX_ELF_CEND | SX_ELF_OPT,      _cdstsDsCondExternal_EFun},
  {"Description",               SX_ELF_CEND | SX_ELF_OPT,      _cdstsDescription_EFun}
};


/************************************************************************/
/* Beginning of actual parse                                            */
/************************************************************************/

ST_RET icfgXml (ST_VOID *userData, ST_CHAR *xmlFileName)
  {
ST_RET rc;

  /* save in a global variable the user information for callback routines */
  userInfo = userData;

  /* perform the actual parse */	     
  rc = sx_parseEx (xmlFileName, 
                  sizeof (iCfgStartElement)/sizeof(SX_ELEMENT), iCfgStartElement,
                  icfgLocalCc); 


  if (rc != SD_SUCCESS)
    {
    ICFG_LOG_ERR1 ("ERROR: parsing failed, return code: '%d'", rc);
    }

  return (rc);
  }


/************************************************************************/
/************************************************************************/
/*                       _localControlCenter_SEFun                      */
/************************************************************************/

static ST_VOID _localControlCenter_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;			

  ICFG_LOG_FLOW0 ("SX DEC: _localControlCenter_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (localCtrlCtrElements);

    icfgLocalCc = (ICFG_LOCAL_CC *) chk_calloc (1,sizeof(ICFG_LOCAL_CC));
    }

  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    ret = u_icfg_EndLocalControlCenter (userInfo); 
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndLocalControlCenter returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    while (sxDecCtrl->itemStackLevel > 0)
      sx_pop (sxDecCtrl);
    }

  }


/************************************************************************/
/*                              _localCcName_EFun                       */
/************************************************************************/

static ST_VOID _localCcName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _localCcName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          
  
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: LocalCcName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: LocalCcName too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (icfgLocalCc->name,str);
    }

  }


/************************************************************************/
/*                           _localMaxDsTs_EFun                         */
/************************************************************************/

static ST_VOID _localMaxDsTs_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET    rc;

  ICFG_LOG_FLOW0 ("SX DEC: _localMaxDsTs_EFun");
  rc = sx_get_long (sxDecCtrl, &icfgLocalCc->maxDsTs);
  if (rc != SD_SUCCESS || icfgLocalCc->maxDsTs < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: LocalMaxDsTs Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }


/************************************************************************/
/*                        _localMaxDataSets_EFun                        */
/************************************************************************/

static ST_VOID _localMaxDataSets_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET    rc;

  ICFG_LOG_FLOW0 ("SX DEC: _localMaxDataSets_EFun");
  rc = sx_get_long (sxDecCtrl, &icfgLocalCc->maxDs);
  if (rc != SD_SUCCESS || icfgLocalCc->maxDs < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: LocalMaxDs Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }


/************************************************************************/
/*                         _localMaxMmsMsgSize_EFun                     */
/************************************************************************/

static ST_VOID _localMaxMmsMsgSize_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET    rc;

  ICFG_LOG_FLOW0 ("SX DEC: _localMaxMmsPduSize_EFun");
  rc = sx_get_long (sxDecCtrl, &icfgLocalCc->maxMmsPduSize);
  if (rc != SD_SUCCESS || icfgLocalCc->maxMmsPduSize < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: LocalMaxMmsPduSize Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }


/************************************************************************/
/*                           _localDescription_EFun                     */
/************************************************************************/

static ST_VOID _localDescription_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _localDescription_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) > MAX_DESCRIPTION_LEN + 1)
    {
    strncpy (icfgLocalCc->description, str, MAX_DESCRIPTION_LEN + 1);
    }
  else
    {
    strcpy (icfgLocalCc->description, str);
    }

  }


/************************************************************************/
/************************************************************************/
/*                        _localObjects_SEFun                           */
/************************************************************************/

static ST_VOID _localObjects_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _localObjects_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (localObjectsElements);

    ret = u_icfg_StartLocalControlCenter(userInfo, icfgLocalCc);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartLocalControlCenter returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    ret = u_icfg_StartLocalObjects (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartLocalObjects returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    ret = u_icfg_EndLocalObjects (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndLocalObjects returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                         _localDataValues_SEFun                       */
/************************************************************************/

static ST_VOID _localDataValues_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _localDataValues_SEFun");
     
  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (ldvElement);

    actualCount = 0;
    if (_getListCount (sxDecCtrl, &numDv) != SD_SUCCESS)
      return;
        
    ret = u_icfg_StartLocalDataValues (userInfo, numDv);  
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartLocalDataValues returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }                                
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    if (actualCount < numDv)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Ldv Count of %d is less than LocalDataValues 'Count' of %d ", actualCount, numDv);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    
    ret = u_icfg_EndLocalDataValues (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndLocalDataValues returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }

  
/************************************************************************/
/*                             _ldv_SERFun                              */
/************************************************************************/

static ST_VOID _ldv_SERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _ldv_SERFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (ldvElements);

    actualCount++;
    if (actualCount > numDv)
      {		      
      ICFG_LOG_ERR2 ("ERROR: Calculated Ldv count of %d exceeds LocalDataValues 'Count' of %d ", actualCount, numDv);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    localDvList = (ICFG_LOCAL_DV *) chk_calloc (1,sizeof(ICFG_LOCAL_DV));
      
    /* initialize variables */
    localDvList->dvType = (enum icfgDvTypeVal)INVALID_ENUM_VAL;       /* set this to something else because "0" is a valid enum value */
    localDvList->normalSource = (enum icfgSourceVal)INVALID_ENUM_VAL;
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    ret = u_icfg_LocalDataValue (userInfo, localDvList);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_LocalDataValue returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                               _ldvName_EFun                          */
/************************************************************************/

static ST_VOID _ldvName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _ldvName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: LdvName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: LdvName too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (localDvList->name,str);
    }

  }


/************************************************************************/
/*                               _ldvDvType_EFun                        */
/************************************************************************/

static ST_VOID _ldvDvType_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT      validEnumVal;
ST_INT      i;

  ICFG_LOG_FLOW0 ("SX DEC: _ldvDvType_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             
           
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: LdvDvType required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  /* make sure this is a valid enumeration value */
  i = 0;
  validEnumVal = SD_FALSE;
  while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_DVTYPES))
    {
    if (!strcmp(icfgDvTypeValEnum[i],str))                 
      {
      /* found enumerated name, it is valid               */
      validEnumVal = SD_TRUE;

      localDvList->dvType = (enum icfgDvTypeVal)i;    
      }
     else
      i++;
    }

  if (validEnumVal == SD_FALSE)
    {
    ICFG_LOG_ERR1    ("ERROR: Invalid Ldv DataType: %s",str);
    sxDecCtrl->errCode = SX_USER_ERROR;

    ICFG_LOG_CERR0   ("       Valid values are: ");
    for (i=0; i < (NUM_OF_ENUM_DVTYPES); i++)
      {
      ICFG_LOG_CERR1  ("           %s", icfgDvTypeValEnum[i]);
      }
    }

  }


/************************************************************************/
/*                         _ldvMapInfo_EFun                            */
/************************************************************************/

static ST_VOID _ldvMapInfo_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _ldvMapInfo_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             
             
  if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: LdvMapInfo too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (localDvList->mapInfo,str);
    }

  }


/************************************************************************/
/*                          _ldvNormalSource_EFun                       */
/************************************************************************/

static ST_VOID _ldvNormalSource_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT      validEnumVal;
ST_INT      i;

  ICFG_LOG_FLOW0 ("SX DEC: _ldvNormalSource_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: LdvNormalSource required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  /* make sure this is a valid enumeration value */
  i = 0;
  validEnumVal = SD_FALSE;
  while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_SRCVALS))
    {
    if (!strcmp(icfgSourceValEnum[i],str))                 
      {
      /* found enumerated name, it is valid               */
      validEnumVal = SD_TRUE;

      localDvList->normalSource = (enum icfgSourceVal)i;  
      }
    else
      i++;
    }

   if (validEnumVal == SD_FALSE)
     {
     ICFG_LOG_ERR1    ("ERROR: Invalid Ldv NormalSource: %s",str);
     sxDecCtrl->errCode = SX_USER_ERROR;

     ICFG_LOG_CERR0   ("       Valid values are: ");
     for (i=0; i < (NUM_OF_ENUM_SRCVALS); i++)
       {
       ICFG_LOG_CERR1 ("           %s", icfgSourceValEnum[i]);
       }
     }

  }


/************************************************************************/
/*                          _ldvRealLimits_EFun                       */
/************************************************************************/

static ST_VOID _ldvRealLimits_EFun (SX_DEC_CTRL *sxDecCtrl)
  {

  ICFG_LOG_FLOW0 ("SX DEC: _ldvRealLimits_EFun");
  SX_PUSH (ldvRealLimitElements);

  }


/************************************************************************/
/*                        _ldvDiscreteLimits_EFun                     */
/************************************************************************/

static ST_VOID _ldvDiscreteLimits_EFun (SX_DEC_CTRL *sxDecCtrl)
  {

  ICFG_LOG_FLOW0 ("SX DEC: _ldvDiscretLimits_EFun");
  SX_PUSH (ldvDiscreteLimitElements);

  }


/************************************************************************/
/*                        _ldvStateLimits_EFun                          */
/************************************************************************/

static ST_VOID _ldvStateLimits_EFun (SX_DEC_CTRL *sxDecCtrl)
  {

  ICFG_LOG_FLOW0 ("SX DEC: _ldvStateLimits_EFun");
  SX_PUSH (ldvStateLimitElements);

  }


/************************************************************************/
/*                       _ldvDescription_EFun                           */
/************************************************************************/

static ST_VOID _ldvDescription_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _ldvDescription_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) > MAX_DESCRIPTION_LEN + 1)
    {
    strncpy (localDvList->description, str, MAX_DESCRIPTION_LEN + 1);
    }
  else
    {
    strcpy (localDvList->description,str);
    }

  }


/************************************************************************/
/*                          _ldvRealNValMin_EFun                        */
/************************************************************************/

static ST_VOID _ldvRealNValMin_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_DOUBLE   dblVal;
ST_BOOLEAN  rc;


  ICFG_LOG_FLOW0 ("SX DEC: _ldvRealNValMin_EFun");

  if (localDvList->dvType != INVALID_ENUM_VAL)    /* was a valid DvType entered? */
    {
    localDvList->minPresent = SD_TRUE;
             
    if (!strcmp(icfgDvTypeValEnum[localDvList->dvType],"Real") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"RealQ") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"RealQTimeTag") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"RealExtended") )               
        {
                                      
        /* if icfgDvTypeVal is Discrete convert to float */

        rc = sx_get_double (sxDecCtrl, &dblVal);
        if (rc != SD_SUCCESS)
          {
          ICFG_LOG_ERR0 ("ERROR: LdvRealValMin Conversion Error");
          sxDecCtrl->errCode = SX_USER_ERROR;
          }
	else
	  localDvList->vals.realVals.valMin = (ST_FLOAT)dblVal;
       }
    }
  else
    {
    ICFG_LOG_ERR0 ("ERROR: Invalid Enumeration value found for DvType in LdvRealNValMin");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }


/************************************************************************/
/*                        _ldvRealNValMax_EFun                      */
/************************************************************************/

static ST_VOID _ldvRealNValMax_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_DOUBLE   dblVal;
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _ldvRealNValMax_EFun");

  if (localDvList->dvType != INVALID_ENUM_VAL)             /* was a valid DvType entered? */
    {
    localDvList->maxPresent = SD_TRUE;

    if (!strcmp(icfgDvTypeValEnum[localDvList->dvType],"Real") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"RealQ") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"RealQTimeTag") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"RealExtended") )               
        {                                     
        /* if icfgDvTypeVal is Discrete convert to float */
  
        rc = sx_get_double (sxDecCtrl, &dblVal);
        if (rc != SD_SUCCESS)
          {
          ICFG_LOG_ERR0 ("ERROR: LdvRealValMin Conversion Error");
          sxDecCtrl->errCode = SX_USER_ERROR;
          }
	else
	  localDvList->vals.realVals.valMax = (ST_FLOAT)dblVal;
        }
    }

  }

  
/************************************************************************/
/*                          _ldvDiscreteNValMin_EFun                        */
/************************************************************************/

static ST_VOID _ldvDiscreteNValMin_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _ldvDiscreteNValMin_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             

  if (localDvList->dvType != INVALID_ENUM_VAL)    /* was a valid DvType entered? */
    {
    localDvList->minPresent = SD_TRUE;
             
    if (!strcmp(icfgDvTypeValEnum[localDvList->dvType],"Discrete") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"DiscreteQ") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"DiscreteQTimeTag") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"DiscreteExtended") )                
      {                               

      /* if icfgDvTypeVal is Discrete convert to signed integer */

      rc = sx_get_long (sxDecCtrl, &localDvList->vals.discreteVals.valMin);
      if (rc != SD_SUCCESS)
        {
        ICFG_LOG_ERR0 ("ERROR: LdvDiscreteNValMin Conversion Error");
        sxDecCtrl->errCode = SX_USER_ERROR;
        }
      }
    }
  else
    {
    ICFG_LOG_ERR0 ("ERROR: Invalid Enumeration value found for DvType in LdvDiscreteNValMin");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }


/************************************************************************/
/*                          _ldvDiscreteNValMax_EFun                    */
/************************************************************************/

static ST_VOID _ldvDiscreteNValMax_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _ldvDiscreteNValMax_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (localDvList->dvType != INVALID_ENUM_VAL)             /* was a valid DvType entered? */
    {
    localDvList->maxPresent = SD_TRUE;

    if (!strcmp(icfgDvTypeValEnum[localDvList->dvType],"Discrete") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"DiscreteQ") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"DiscreteQTimeTag") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"DiscreteExtended") )                
      {                               
      /* if icfgDvTypeVal is Discrete convert to integer */
  
      rc = sx_get_long (sxDecCtrl, &localDvList->vals.discreteVals.valMax);
      if (rc != SD_SUCCESS)
        {
        ICFG_LOG_ERR0 ("ERROR: LdvDiscreteNValMax Conversion Error");
        sxDecCtrl->errCode = SX_USER_ERROR;
        }
      }
    }

  }

  
/************************************************************************/
/*                          _ldvStateNValMin_EFun                       */
/************************************************************************/

static ST_VOID _ldvStateNValMin_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _ldvStateNValMin_EFun");

  if (localDvList->dvType != INVALID_ENUM_VAL)    /* was a valid DvType entered? */
    {
    localDvList->minPresent = SD_TRUE;
             
    if (!strcmp(icfgDvTypeValEnum[localDvList->dvType],"State") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"StateQ") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"StateQTimeTag") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"StateExtended") )                
      {                               

      /* if icfgDvTypeVal is State convert to unsigned char */

      rc = sx_get_uchar (sxDecCtrl, &localDvList->vals.stateVals.valMin);
      if (rc != SD_SUCCESS)
        {
        ICFG_LOG_ERR0 ("ERROR: LdvStateValMin Conversion Error");
        sxDecCtrl->errCode = SX_USER_ERROR;
        }
      else if (localDvList->vals.stateVals.valMin < '0' || localDvList->vals.stateVals.valMin > '3')
        {
        ICFG_LOG_ERR0 ("ERROR: LdvStateValMin out of range 0-3");
        sxDecCtrl->errCode = SX_USER_ERROR;
        }

      }
    }
  else
    {
    ICFG_LOG_ERR0 ("ERROR: Invalid Enumeration value found for DvType in LdvStateNValMin");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }


/************************************************************************/
/*                          _ldvStateNValMax_EFun                    */
/************************************************************************/

static ST_VOID _ldvStateNValMax_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _ldvStateNValMax_EFun");

  if (localDvList->dvType != INVALID_ENUM_VAL)             /* was a valid DvType entered? */
    {
    localDvList->maxPresent = SD_TRUE;

    if (!strcmp(icfgDvTypeValEnum[localDvList->dvType],"State") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"StateQ") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"StateQTimeTag") ||
        !strcmp(icfgDvTypeValEnum[localDvList->dvType],"StateExtended") )                
      {                               
      /* if icfgDvTypeVal is State convert to unsigned char */
  
      rc = sx_get_uchar (sxDecCtrl, &localDvList->vals.stateVals.valMax);
      if (rc != SD_SUCCESS)
        {
        ICFG_LOG_ERR0 ("ERROR: LdvStateValMax Conversion Error");
        sxDecCtrl->errCode = SX_USER_ERROR;
        }
      else if (localDvList->vals.stateVals.valMax < '0' || localDvList->vals.stateVals.valMax > '3')
        {
        ICFG_LOG_ERR0 ("ERROR: LdvStateValMax out of range 0-3");
        sxDecCtrl->errCode = SX_USER_ERROR;
        }

      }
    }

  }

  
/************************************************************************/
/*                          _localDevices_SEFun                         */
/************************************************************************/

static ST_VOID _localDevices_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _localDevices_SEFun");
             
   if (sxDecCtrl->reason == SX_ELEMENT_START)
    {           
    SX_PUSH (ldevElement);

    actualCount = 0;
    if (_getListCount (sxDecCtrl, &numDev) != SD_SUCCESS)
      return;

    ret = u_icfg_StartLocalDevices (userInfo, numDev);    
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartLocalDevices returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {                     
    if (actualCount < numDev)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Ldev Count value of %d is less than LocalDevices 'Count' of %d ", actualCount, numDev);
      sxDecCtrl->errCode = SX_USER_ERROR;
      } 

    ret = u_icfg_EndLocalDevices (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndLocalDevices returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }

  
/************************************************************************/
/*                             _ldev_SERFun                             */
/************************************************************************/

static ST_VOID _ldev_SERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _ldev_SERFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (ldevElements);

    actualCount++;
    if (actualCount > numDev)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Ldv count of %d exceeds LocalDevices 'Count' of %d ", actualCount, numDev);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    localDevList = (ICFG_LOCAL_DEV *) chk_calloc (1,sizeof(ICFG_LOCAL_DEV));

    chkBackIdEntered = SD_FALSE;
    localDevList->checkBackId = Default_CheckBackId;

    selTimeEntered =SD_FALSE;
    localDevList->selTime = Default_SelectTimeout;

    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    if ( (localDevList->sbo == SD_TRUE) && (chkBackIdEntered == SD_FALSE) )
      {
      ICFG_LOG_FLOW1 ("INFO: SBO='Y' but CheckBackId was not entered.  Used default: %d", Default_CheckBackId);
      }

    if ( (localDevList->sbo == SD_FALSE) && (chkBackIdEntered == SD_TRUE) )
      {
      ICFG_LOG_FLOW1 ("INFO: SBO='N' but CheckBackId: %d was entered.", localDevList->checkBackId);
      }
     
    if ( (localDevList->sbo == SD_TRUE) && (selTimeEntered == SD_FALSE) )
      {
      ICFG_LOG_FLOW1 ("INFO: SBO='Y' but SelTime was not entered.  Used default: %d", Default_SelectTimeout);
      }

    if ( (localDevList->sbo == SD_FALSE) && (selTimeEntered == SD_TRUE) )
      {
      ICFG_LOG_FLOW1 ("INFO: SBO='N' but SelTime: %d was entered.", localDevList->checkBackId);
      }
     
    ret = u_icfg_LocalDevice  (userInfo, localDevList);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_LocalDevice returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    }		    

  }


/************************************************************************/
/*                          _ldevDeviceName_EFun                        */
/************************************************************************/


static ST_VOID _ldevDeviceName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _ldevDeviceName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)  
    {
    ICFG_LOG_ERR0 ("ERROR: LdevName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_IDENT_LEN - 4 + 1)     /*   " - 4 " allows room for _SBO or _TAG   */
    {
    ICFG_LOG_ERR1 ("ERROR: Name too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (localDevList->name,str);
    }

  }


/************************************************************************/
/*                         _ldevDevType_EFun                            */
/************************************************************************/

static ST_VOID _ldevDevType_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT      validEnumVal;
ST_INT      i;

  ICFG_LOG_FLOW0 ("SX DEC: _ldevDevType_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             
    
  if (strlen(str) == 0)  
    {
    ICFG_LOG_ERR0 ("ERROR: LdevDevType required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  localDevList->devType = (enum icfgDevTypeVal)INVALID_ENUM_VAL;
  /* make sure this is a valid enumeration value */
  i = 0;
  validEnumVal = SD_FALSE;
  while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_DEVTYPES))
    {
    if (!strcmp(icfgDevTypeValEnum[i],str))                
      {
      /* found enumerated name, it is valid               */
      validEnumVal = SD_TRUE;

      localDevList->devType = (enum icfgDevTypeVal)i;    
      }
     else
      i++;
    }

  if (validEnumVal == SD_FALSE)
    {
    ICFG_LOG_ERR1    ("ERROR: Invalid Ldev DeviceType: %s",str);
    sxDecCtrl->errCode = SX_USER_ERROR;

    ICFG_LOG_CERR0   ("       Valid values are: ");
    for (i=0; i < (NUM_OF_ENUM_DEVTYPES); i++)
      {
      ICFG_LOG_CERR1 ("           %s", icfgDevTypeValEnum[i]);
      }
    }

  }


/************************************************************************/
/*                          _ldevMapInfo_EFun                           */
/************************************************************************/

static ST_VOID _ldevMapInfo_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _ldevMapInfo_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             
             
  if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: LdevMapInfo too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (localDevList->mapInfo,str);
    }

  }


/************************************************************************/
/*                             _ldevSbo_EFun                            */
/************************************************************************/

static ST_VOID _ldevSbo_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _ldevSbo_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)  
    {
    ICFG_LOG_ERR0 ("ERROR: LdevSbo required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else 
    _icfgGetYesNo (sxDecCtrl, &localDevList->sbo, "LdevSbo");

  }


/************************************************************************/
/*                       _ldevCcheckBackId_EFun                         */
/************************************************************************/

static ST_VOID _ldevCheckBackId_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _ldevCheckBackId_EFun");
  rc = sx_get_int16 (sxDecCtrl, &localDevList->checkBackId);
  if (rc != SD_SUCCESS || localDevList->checkBackId < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: localDevList->checkBackId Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    chkBackIdEntered = SD_TRUE;

  }


/************************************************************************/
/*                              _ldevSelTime_EFun                      */
/************************************************************************/

static ST_VOID _ldevSelTime_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _ldevSelTime_EFun");
  rc = sx_get_long (sxDecCtrl, &localDevList->selTime);
  if (rc != SD_SUCCESS || localDevList->selTime < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: LDevSelTime Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    selTimeEntered = SD_TRUE;
  }


/************************************************************************/
/*                         _ldevTagEn_EFun                              */
/************************************************************************/

static ST_VOID _ldevTagEn_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _ldevTagEn_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)  
    {
    ICFG_LOG_ERR0 ("ERROR: LdevTagEn required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &localDevList->tagEnable, "LdevTagEnable");

  }


/************************************************************************/
/*                       _ldevDescription_EFun                          */
/************************************************************************/


static ST_VOID _ldevDescription_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _ldevDescription_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) > MAX_DESCRIPTION_LEN + 1)
    {
    strncpy (localDevList->description, str, MAX_DESCRIPTION_LEN + 1);
    }
  else
    {
    strcpy (localDevList->description,str);
    }

  }


/************************************************************************/
/*                        _localInfoMsgs_SEFun                          */
/************************************************************************/

static ST_VOID _localInfoMsgs_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _localInfoMsgs_SEFun");
             
  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (limElement);

    actualCount = 0;
    if (_getListCount (sxDecCtrl, &numInfoMsg) != SD_SUCCESS)
      return;

    ret = u_icfg_StartLocalInfoMsgs     (userInfo, numInfoMsg);     
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartLocalInfoMsgs returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    if (actualCount < numInfoMsg)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Lim count of %d is less than LocalInfoMsgs 'Count' of %d ", actualCount, numInfoMsg);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    ret = u_icfg_EndLocalInfoMsgs (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndLocalInfoMsgs returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

 }

  
/************************************************************************/
/*                              _lim_SERFun                             */
/************************************************************************/

static ST_VOID _lim_SERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;
 
  ICFG_LOG_FLOW0 ("SX DEC: _lim_SERFun");
             
  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (limElements);

    actualCount++;
    if (actualCount > numInfoMsg)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Lim count of %d exceeds LocalInfoMsgs 'Count' of %d ", actualCount, numInfoMsg);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
      localInfoMsgList = (ICFG_LOCAL_INFOMSG *) chk_calloc (1,sizeof(ICFG_LOCAL_INFOMSG));
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    ret = u_icfg_LocalInfoMsg (userInfo, localInfoMsgList);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_LocalInfoMsgs returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }

  
/************************************************************************/
/*                            _limInfoRef_EFun                          */
/************************************************************************/

static ST_VOID _limInfoRef_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _limInfoRef_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                        

  if (strlen(str) == 0)  
    {
    ICFG_LOG_ERR0 ("ERROR: LimInfoRef required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {    
    rc = sx_get_long (sxDecCtrl, &localInfoMsgList->infoRef);
    if (rc != SD_SUCCESS || localInfoMsgList->infoRef < 0)
      {
      ICFG_LOG_ERR0 ("ERROR: LimInfoRef Conversion Error");
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }                                                     


/************************************************************************/
/*                          _limMapInfo_EFun                            */
/************************************************************************/


static ST_VOID _limMapInfo_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _limMapInfo_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             
             
  if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: LimMapInfo too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (localInfoMsgList->mapInfo,str);
    }

  }


/************************************************************************/
/*                          _limMaxSize_EFun                            */
/************************************************************************/

static ST_VOID _limMaxSize_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _limMaxSize_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;     
   
  if (strlen(str) == 0)  
    {
    ICFG_LOG_ERR0 ("ERROR: LimMaxSize required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    rc = sx_get_long (sxDecCtrl, &localInfoMsgList->maxSize);
    if (rc != SD_SUCCESS || localInfoMsgList->maxSize < 0)
      {
      ICFG_LOG_ERR0 ("ERROR: LimMaxSize Conversion Error");
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                          _limDescription_EFun                        */
/************************************************************************/


static ST_VOID _limDescription_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _limDescription_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) > MAX_DESCRIPTION_LEN + 1)
    {
    strncpy (localInfoMsgList->description, str, MAX_DESCRIPTION_LEN + 1);
    }
  else
    {
    strcpy (localInfoMsgList->description,str);
    }

  }


/************************************************************************/
/************************************************************************/
/*                       _remoteControlCenters_SEFun                    */
/************************************************************************/

static ST_VOID _remoteControlCenters_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _remoteControlCenters_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (remoteCtrlCtrElement);

    rCCActualCount = 0;
    if (_getListCount (sxDecCtrl, &numRcc) != SD_SUCCESS)
      return;

    ret = u_icfg_StartRemoteControlCenters (userInfo, numRcc);   
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartRemoteControlCenters returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    remoteCcList = NULL;
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    if (rCCActualCount < numRcc)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated RemoteControlCenter count of %d is less than RemoteControlCenters 'Count' of %d ", rCCActualCount, numRcc);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    ret = u_icfg_EndRemoteControlCenters (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndRemoteControlCenters returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                    _remoteControlCenter_SERFun                       */
/************************************************************************/

static ST_VOID _remoteControlCenter_SERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _remoteControlCenter_SERFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (remoteCtrlCtrElements);

    rCCActualCount++;
    if (rCCActualCount > numRcc)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated RemoteControlCenter count of %d exceeds RemoteControlCenters 'Count' value of %d ", rCCActualCount, numRcc);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    remoteCcList = (ICFG_REMOTE_CC *) chk_calloc (1,sizeof(ICFG_REMOTE_CC));

    /* Set defaults */
    remoteCcList->blt.remDvValidation = ValNames;
    remoteCcList->blt.remDvGetConfig = GetConfigNone;
    remoteCcList->blt.remDvExcludeInvalidFromDs = SD_TRUE;
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    ret = u_icfg_EndRemoteControlCenter (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndRemoteControlCenter returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                            _remoteCcName_EFun                        */
/************************************************************************/

static ST_VOID _remoteCcName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _remoteCcName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: RemoteCcName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: RemoteCcName too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (remoteCcList->name,str);
    }

  }


/************************************************************************/
/*                      _remoteDescription_EFun                         */
/************************************************************************/

static ST_VOID _remoteDescription_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _remoteDescription_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) > MAX_DESCRIPTION_LEN + 1)
    {
    strncpy (remoteCcList->description, str, MAX_DESCRIPTION_LEN + 1);
    }
  else
    {
    strcpy (remoteCcList->description, str);
    }

  }


/************************************************************************/
/*                         _bilateralTable_SEFun                        */
/************************************************************************/

static ST_VOID _bilateralTable_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {

  ICFG_LOG_FLOW0 ("SX DEC: _bilateralTable_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (bilateralTableElements);
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {

    }

  }


/************************************************************************/
/*                             _bltName_EFun                            */
/************************************************************************/

static ST_VOID _bltName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _bltName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: BltName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MI_MAX_BLT_ID_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: BltName too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (remoteCcList->blt.name,str);
    }

  }


/************************************************************************/
/*                             _bltId_EFun                              */
/************************************************************************/

static ST_VOID _bltId_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _bltId_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: BltId required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MI_MAX_BLT_ID_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: BltId too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (remoteCcList->blt.id,str);
    }

  }


/************************************************************************/
/*                           _localDomain_EFun                          */
/************************************************************************/

static ST_VOID _localDomain_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _localDomain_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;               

  if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: LocalDomain too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy ( remoteCcList->blt.localDom,str);
    }
             
  }


/************************************************************************/
/*                            _remoteDomain_EFun                        */
/************************************************************************/

static ST_VOID _remoteDomain_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _remoteDomain_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: RemoteDomain too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (remoteCcList->blt.remoteDom,str);
    }

  }
 

/************************************************************************/
/*                      _shortestInterval_EFun                          */
/************************************************************************/

static ST_VOID _shortestInterval_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_LONG     sLong;

  ICFG_LOG_FLOW0 ("SX DEC: _shortestInterval_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  sLong = 0;
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ShortestInterval required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if ( asciiToSlong(str, &sLong) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: ShortestInterval Conversion Error on value: %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (sLong < 1)
    {
    ICFG_LOG_ERR1 ("ERROR: ShortestInterval: %s is less than minimum: 1", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    { 
    remoteCcList->blt.shortestInterval = sLong;
    }

  }


/************************************************************************/
/*                              _blocks_EFun                            */
/************************************************************************/

static ST_VOID _blocks_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_INT      done;
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_LONG     sLong;
ST_CHAR    *nptr, *tptr, *ptr;
ST_INT      n;

  ICFG_LOG_FLOW0 ("SX DEC: _blocks_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: Blocks required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  nptr = (ST_CHAR *) chk_calloc (1, strlen (str) + 1);

  if (str == EMPTY_STRING) return;
 
  ptr = str;
  done = SD_FALSE;

  while (done == SD_FALSE)
    {
  
    if (strchr(ptr,','))
          tptr = strchr(ptr,',');
        else
        {
          tptr = strchr(ptr,'\0');
          done = SD_TRUE;
        }
    n = tptr - ptr + 1;
    strncpy (nptr, ptr, n);
    nptr [n-1] = '\0';
    if (done == SD_FALSE)
          strcpy (ptr, tptr + 1);  /* look at next number in input text */

    if ( asciiToSlong(nptr, &sLong) || strchr(nptr,'-'))
      {
      ICFG_LOG_ERR1 ("ERROR: Blocks Conversion Error on value: %s ", str);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    else
      {

      switch (sLong)
        {
        case 1: 
          remoteCcList->blt.blocks  |=  ICFG_BLOCK1;  
          break; 
        case 2: 
          remoteCcList->blt.blocks  |=  ICFG_BLOCK2;  
          break; 
        case 3: 
          remoteCcList->blt.blocks  |=  ICFG_BLOCK3;  
          break; 
        case 4: 
           remoteCcList->blt.blocks |=  ICFG_BLOCK4;  
           break; 
        case 5: 
          remoteCcList->blt.blocks  |=  ICFG_BLOCK5;  
          break; 
        case 6: 
          remoteCcList->blt.blocks  |=  ICFG_BLOCK6;  
          break; 
        case 7: 
          remoteCcList->blt.blocks  |=  ICFG_BLOCK7;  
          break; 
        case 8: 
          remoteCcList->blt.blocks  |=  ICFG_BLOCK8;  
          break; 
        case 9: 
          remoteCcList->blt.blocks  |=  ICFG_BLOCK9;  
          break; 
        default: 
          break;
        } /* switch */

      } /* else */
    } /* while */

  chk_free (nptr);
  }

/************************************************************************/
/*			_remDvValidation_EFun				*/
/************************************************************************/

static ST_VOID _remDvValidation_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _remDvValidation_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (!strcmp (str, "None"))
    remoteCcList->blt.remDvValidation = ValNone;
  else if (!strcmp (str, "CompareNames"))                
    remoteCcList->blt.remDvValidation = ValNames;
  else if (!strcmp (str, "CompareNamesAndTypes"))                
    remoteCcList->blt.remDvValidation = ValNamesAndTypes;
  else
    {
    ICFG_LOG_ERR1 ("ERROR: Rem DV Validation Data Conversion Error on value of %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }

/************************************************************************/
/*			_remDvGetConfig_EFun				*/
/************************************************************************/

static ST_VOID _remDvGetConfig_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _remDvGetConfig_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (!strcmp (str, "None"))
    remoteCcList->blt.remDvGetConfig = GetConfigNone;
  else if (!strcmp (str, "XML"))                
    remoteCcList->blt.remDvGetConfig = GetConfigXml;
  else if (!strcmp (str, "DirectDatabase"))
    remoteCcList->blt.remDvGetConfig = GetConfigDb;
  else
    {
    ICFG_LOG_ERR1 ("ERROR: Get DV config Data Conversion Error on value of %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }

/************************************************************************/
/*			_remDvExcludeInvalidFromDs_EFun			*/
/************************************************************************/

static ST_VOID _remDvExcludeInvalidFromDs_EFun (SX_DEC_CTRL *sxDecCtrl)
  {

  ICFG_LOG_FLOW0 ("SX DEC: _remDvExcludeInvalidFromDs_EFun");

  _icfgGetYesNo (sxDecCtrl, &remoteCcList->blt.remDvExcludeInvalidFromDs, 
                 "remoteCcBltRemDvExcludeInvalidFromDs");

  }


/************************************************************************/
/*                      _BltDescription_EFun                         */
/************************************************************************/

static ST_VOID _bltDescription_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _bltDescription_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) > MAX_DESCRIPTION_LEN + 1)
    {
    strncpy (remoteCcList->blt.description, str, MAX_DESCRIPTION_LEN + 1);
    }
  else
    {
    strcpy (remoteCcList->blt.description, str);
    }

  }


/************************************************************************/
/*                          _associations_SEFun                         */
/************************************************************************/

static ST_VOID _associations_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _associations_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (associationsElements);

    actualCount = 0;
    if (_getListCount (sxDecCtrl, &numAssoc) != SD_SUCCESS)
      return;

    ret = u_icfg_StartRemoteControlCenter (userInfo, remoteCcList);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartRemoteControlCenter returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }	 

    assocList = NULL;
    ret = u_icfg_StartAssociations  (userInfo, numAssoc);    
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartAssociations returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    if (actualCount < numAssoc)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Association count of %d is less than Associations 'Count' of %d ", actualCount, numAssoc);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    ret = u_icfg_EndAssociations (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndAssociations returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                             _association_SERFun                      */
/************************************************************************/

static ST_VOID _association_SERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _association_SERFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (associationElements);

    actualCount++;
    if (actualCount > numAssoc)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Association count of %d exceeds Associations 'Count' value of %d ", actualCount, numAssoc);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    assocList = (ICFG_ASSOCIATION *) chk_calloc (1,sizeof(ICFG_ASSOCIATION));

    assocList->connectRole = 0;
    assocList->serviceRole = 0;
    assocList->maxReqPend  = Default_MaxReqPend;
    assocList->maxIndPend  = Default_MaxIndPend;
    assocList->maxNest     = Default_MaxNest;

    assocRetryDelayEntered = SD_FALSE;
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    if ( (assocList->connectRole == ICFG_ROLE_CALLING) && (assocRetryDelayEntered == SD_FALSE) )
      {
      ICFG_LOG_FLOW1 ("INFO: ConnectRole='calling' but AssocRetryTime was not entered.  Used default: %d", Default_AssocRetryDelay);
      }

    if ( (assocList->connectRole != ICFG_ROLE_CALLING) && (assocRetryDelayEntered == SD_TRUE) )
      {
      ICFG_LOG_FLOW1 ("INFO: ConnectRole='called' but AssocRetryTime: %d was entered.", assocList->retryTime);
      }

    ret = u_icfg_Association (userInfo, assocList);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_Association returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                            _assocName_EFun                           */
/************************************************************************/


static ST_VOID _assocName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _assocName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: AssocName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: AssocName too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (assocList->name,str);
    }

  }


/************************************************************************/
/*                            _localAr_EFun                             */
/************************************************************************/

static ST_VOID _localAr_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _localAr_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;     
             
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: LocalAr required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_AR_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: LocalAr too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (assocList->localAr,str);
    }

  }


/************************************************************************/
/*                          _remoteAr_ERFun                             */
/************************************************************************/

static ST_VOID _remoteAr_ERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _remoteAr_ERFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;     

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: RemoteArName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_AR_LEN + 1) 
    {
    ICFG_LOG_ERR1 ("ERROR: RemoteArName too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (assocList->totalRemoteArNames == MAX_REMOTE_AR_NAMES)
    {
    ICFG_LOG_ERR0 ("ERROR: Too Many RemoteArNames ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (assocList->remoteAr[assocList->totalRemoteArNames],str);
    assocList->totalRemoteArNames++;
    }
                     
  }


/************************************************************************/
/*                          _connectRole_ERFun                          */
/************************************************************************/

static ST_VOID _connectRole_ERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _connectRole_ERFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ConnectRole required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  if (!strcmpi (str, "Called"))
    {
    assocList->connectRole |= ICFG_ROLE_CALLED;
    }
  else if (!strcmpi (str, "Calling"))
    {
    assocList->connectRole |= ICFG_ROLE_CALLING;
    }
  else
    {
    ICFG_LOG_ERR1 ("ERROR: Connect Role Data Conversion Error on value: %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }


/************************************************************************/
/*                          _serviceRole_ERFun                          */
/************************************************************************/

static ST_VOID _serviceRole_ERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _serviceRole_ERFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ServiceRole required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  if (!strcmpi (str, "Client"))
    {
    assocList->serviceRole |= ICFG_ROLE_CLIENT;
    }
  else if (!strcmpi (str, "Server"))
    {
    assocList->serviceRole |= ICFG_ROLE_SERVER;
    }
  else
    {
    ICFG_LOG_ERR1 ("ERROR: Service Role Data Conversion Error on value: %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }


/************************************************************************/
/*                           _assocRetryTime_EFun                       */
/************************************************************************/

static ST_VOID _assocRetryTime_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_LONG     sLong;

  ICFG_LOG_FLOW0 ("SX DEC: _assocRetryTime_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                     

  sLong = 0;
  if ( asciiToSlong(str, &sLong) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: AssocRetryTime Conversion Error on value: %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    { /* valid short integer value */
    if (assocList != NULL)
      assocList->retryTime = sLong;
    assocRetryDelayEntered = SD_TRUE;
    }
     
  }


/************************************************************************/
/*                       _remoteMaxMmsMsgSize_EFun                      */
/************************************************************************/

static ST_VOID _remoteMaxMmsMsgSize_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET    rc;
ST_CHAR  *str;
ST_INT    strLen;
ST_LONG   sLong;

  ICFG_LOG_FLOW0 ("SX DEC: _remoteMaxMmsMsgSize_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  sLong = 0;
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: RemoteMaxMmsMsgSize required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if ( asciiToSlong(str, &sLong) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: RemoteMaxMmsMsgSize Data Conversion Error on value: %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    { /* valid short integer value */
    if (assocList != NULL)
      assocList->maxMmsPduSize = sLong;
    }

  }


/************************************************************************/
/*                         _maxReqPend_EFun                             */
/************************************************************************/

static ST_VOID _maxReqPend_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT16    sInt16;

  ICFG_LOG_FLOW0 ("SX DEC: _maxReqPend_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             

  sInt16 = 0;
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: MaxReqPend required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if ( asciiToSint16(str, &sInt16) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: MaxReqPend Conversion Error on value: %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    { 
    if (assocList != NULL)
      assocList->maxReqPend = sInt16;
    }
             
  }


/************************************************************************/
/*                          _maxIndPend_EFun                            */
/************************************************************************/

static ST_VOID _maxIndPend_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT16    sInt16;

  ICFG_LOG_FLOW0 ("SX DEC: _maxIndPend_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                         

  sInt16 = 0;
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: MaxIndPend required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if ( asciiToSint16(str, &sInt16) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: MaxIndPend Conversion Error on value: %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    { 
    if (assocList != NULL)
       assocList->maxIndPend = sInt16;
    }

  }


/************************************************************************/
/*                            _maxNest_EFun                             */
/************************************************************************/

static ST_VOID _maxNest_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT8     sInt8;

  ICFG_LOG_FLOW0 ("SX DEC: _maxNest_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             

  sInt8 = 0;
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: Assoc MaxNest required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (asciiToSint8(str, &sInt8) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: Assoc MaxNest Conversion Error on value: %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    if (assocList != NULL)
      assocList->maxNest = sInt8;
    }
             
  }


/************************************************************************/
/*                     _assocDescription_EFun                           */
/************************************************************************/


static ST_VOID _assocDescription_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _assocDescription_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) > MAX_DESCRIPTION_LEN + 1)
    {
    strncpy (assocList->description, str, MAX_DESCRIPTION_LEN + 1);
    }
  else
    {
    strcpy (assocList->description,str);
    }

  }


/************************************************************************/
/************************************************************************/
/*                      _serverObjects_SEFun                            */
/************************************************************************/

static ST_VOID _serverObjects_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _serverObjects_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (serverObjsElements);

    serverObjCounts = NULL;
    serverObjCounts = (ICFG_SERVER_OBJ_COUNTS *) chk_calloc (1,sizeof(ICFG_SERVER_OBJ_COUNTS));
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    ret = u_icfg_EndServerObjects (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndServerObjects returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                         _serverNumVccDv_EFun                         */
/************************************************************************/

static ST_VOID _serverNumVccDv_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _serverNumVccDv_EFun");
  rc = sx_get_long (sxDecCtrl, &serverObjCounts->numVccDv);
  if (rc != SD_SUCCESS || serverObjCounts->numVccDv < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ServerNumVccDv Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }


/************************************************************************/                                   
/*                            _serverNumVccDev_EFun                     */
/************************************************************************/

static ST_VOID _serverNumVccDev_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _serverNumVccDev_EFun");
  rc = sx_get_long (sxDecCtrl, &serverObjCounts->numVccDev);
  if (rc != SD_SUCCESS || serverObjCounts->numVccDev < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ServerNumVccDev Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
    
  }   


/************************************************************************/
/*                          _serverVccNumInfoMsg_EFun                   */
/************************************************************************/

static ST_VOID _serverNumVccInfoMsg_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _serverNumVccInfoMsg_EFun");
  rc = sx_get_long (sxDecCtrl, &serverObjCounts->numVccInfoMsg);
  if (rc != SD_SUCCESS || serverObjCounts->numVccInfoMsg < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ServerNumVccInfoMsg Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }


/************************************************************************/
/*                         _serverNumVccDs_EFun                         */
/************************************************************************/

static ST_VOID _serverNumVccDs_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _serverNumVccDs_EFun");
  rc = sx_get_long (sxDecCtrl, &serverObjCounts->numVccDs);
  if (rc != SD_SUCCESS || serverObjCounts->numVccDs < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ServerNumVccDs Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
     
  }


/************************************************************************/
/*                           _serverNumIccDv_EFun                       */
/************************************************************************/

static ST_VOID _serverNumIccDv_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _serverNumIccDv_EFun");
  rc = sx_get_long (sxDecCtrl, &serverObjCounts->numIccDv);
  if (rc != SD_SUCCESS || serverObjCounts->numIccDv < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ServerNumIccDv Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
     
  }


/************************************************************************/                                   
/*                        _serverNumIccDev_EFun                         */
/************************************************************************/

static ST_VOID _serverNumIccDev_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _serverNumIccDev_EFun");
  rc = sx_get_long (sxDecCtrl, &serverObjCounts->numIccDev);
  if (rc != SD_SUCCESS || serverObjCounts->numIccDev < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ServerNumIccDev Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }   


/************************************************************************/
/*                        _serverNumIccInfoMsg_EFun                     */
/************************************************************************/

static ST_VOID _serverNumIccInfoMsg_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _serverNumIccInfoMsg_EFun");
  rc = sx_get_long (sxDecCtrl, &serverObjCounts->numIccInfoMsg);
  if (rc != SD_SUCCESS || serverObjCounts->numIccInfoMsg < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ServerNumIccInfoMsg Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }


/************************************************************************/
/*                           _serverNumIccDs_EFun                       */
/************************************************************************/

static ST_VOID _serverNumIccDs_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_RET      ret;

  ICFG_LOG_FLOW0 ("SX DEC: _serverNumIccDs_EFun");
  rc = sx_get_long (sxDecCtrl, &serverObjCounts->numIccDs);
  if (rc != SD_SUCCESS || serverObjCounts->numIccDs < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ServerNumIccDs Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }


  /* save all counts in case "serverObjCounts" is freed in callback routines */
  memcpy (&sServerObjCounts, serverObjCounts, sizeof(ICFG_SERVER_OBJ_COUNTS));

  /* this would normally be done in serverObjects_SEFun but is done here so  */
  /* u_icfg_StartServerObjs may be called with valid data                    */

  ret = u_icfg_StartServerObjects (userInfo, serverObjCounts);
  if (ret != SD_SUCCESS)
    {
    ICFG_LOG_ERR1 ("ERROR: u_icfg_StartServerObjects returns: %d", ret);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
            
  }


/************************************************************************/
/*                        _serverDataValues_SEFun                       */
/************************************************************************/

static ST_VOID _serverDataValues_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _serverDataValues_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (sdvElement);

    actualCount = 0;
    if (_getListCount (sxDecCtrl, &numDv) != SD_SUCCESS)
      return;

    numVccDv = 0;
    numIccDv = 0;
    serverDvList = NULL;

    ret = u_icfg_StartServerDataValues (userInfo, numDv);  
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartServerDataValues returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {

    if (actualCount < numDv)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Sdv Count of %d is less than ServerDataValues 'Count' of %d ", actualCount, numDv);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    /* validate input VCC parameters */
    if (numVccDv != sServerObjCounts.numVccDv)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Server NumVccDv of %d does not equal Server 'NumVccDv' of %d ",
                  numVccDv, sServerObjCounts.numVccDv);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    /* validate input ICC parameters */
    else if (numIccDv != sServerObjCounts.numIccDv)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Server NumIccDv of %d does not equal Server 'NumIccDv' of %d ",
                  numIccDv, sServerObjCounts.numIccDv);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    ret = u_icfg_EndServerDataValues (userInfo);
      if (ret != SD_SUCCESS)
        {
        ICFG_LOG_ERR1 ("ERROR: u_icfg_EndServerDataValues returns: %d", ret);
        sxDecCtrl->errCode = SX_USER_ERROR;
        }
    }

  }


/************************************************************************/
/*                             _sdv_SERFun                              */
/************************************************************************/

static ST_VOID _sdv_SERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _sdv_SERFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (sdvElements);
    actualCount++;
    if (actualCount > numDv)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Sdv count of %d exceeds ServerDataValues 'Count' value of %d ", actualCount, numDv);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    serverDvList = (ICFG_SERVER_DV *) chk_calloc (1,sizeof(ICFG_SERVER_DV));

    /* initialize variables */
    serverDvList->dvType = (enum icfgDvTypeVal)INVALID_ENUM_VAL;       /* set this to something else because "0" is a valid enum value */
    }


  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    if (serverDvList->dvType == INVALID_ENUM_VAL)
      {
      ICFG_LOG_ERR1 ("ERROR: ServerDvType is invalid enum value: %d", serverDvList->dvType);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }


    ret = u_icfg_ServerDataValue (userInfo, serverDvList);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_ServerDataValue returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                           _sdvObjName_EFun                              */
/************************************************************************/

static ST_VOID _sdvObjName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_INT      i;
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT      validEnumVal;

  ICFG_LOG_FLOW0 ("SX DEC: _sdvObjName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                         

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: SdvObjName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: SdvObjName too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: SdvObjName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else         
    {
    /* make sure this is a valid enumeration value */
    i = 0;
    validEnumVal = SD_FALSE;
    while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_SCOPEVALS))
      {
      if (!strcmpi (icfgScopeValEnum[i], sxDecCtrl->sxDecElInfo.attr[0].value))
        {

        /* found Scope name, it is valid               */
        validEnumVal = SD_TRUE;
        serverDvList->scope = (enum icfgScopeVal)i;    

        strcpy (serverDvList->name,str);

        if ( !strcmpi (sxDecCtrl->sxDecElInfo.attr[0].value,"VCC") )   numVccDv++;
        if ( !strcmpi (sxDecCtrl->sxDecElInfo.attr[0].value,"ICC") )   numIccDv++;
        }
    else
      i++;
    }

    if (validEnumVal == SD_FALSE)
      {
      ICFG_LOG_ERR1     ("ERROR: Invalid Sdv Scope: %s",sxDecCtrl->sxDecElInfo.attr[0].value);
      sxDecCtrl->errCode = SX_USER_ERROR;

      ICFG_LOG_CERR0   ("       Valid values are: ");
      for (i=0; i < (NUM_OF_ENUM_SCOPEVALS); i++)
        {
        ICFG_LOG_CERR1 ("           %s", icfgScopeValEnum[i]);
        }
      }
    } /* else */

  }


/************************************************************************/
/*                               _sdvDvType_EFun                           */
/************************************************************************/


static ST_VOID _sdvDvType_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_INT      i;
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT      validEnumVal;

  ICFG_LOG_FLOW0 ("SX DEC: _sdvDvType_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             
             
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: SdvDvType required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  /* make sure this is a valid enumeration value */
  i = 0;
  validEnumVal = SD_FALSE;
  while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_DVTYPES))
    {
    if (!strcmp(icfgDvTypeValEnum[i],str))                 
      {
      /* found enumerated name, it is valid               */
      validEnumVal = SD_TRUE;

      serverDvList->dvType = (enum icfgDvTypeVal)i;    
      }
     else
      i++;
    }

  if (validEnumVal == SD_FALSE)
    {
    ICFG_LOG_ERR1 ("ERROR: Invalid Sdv DataType: %s",str);
    sxDecCtrl->errCode = SX_USER_ERROR;

    ICFG_LOG_CERR0   ("       Valid values are: ");
    for (i=0; i < (NUM_OF_ENUM_DVTYPES); i++)
      {
      ICFG_LOG_CERR1 ("           %s", icfgDvTypeValEnum[i]);
      }
    }

  }


/************************************************************************/
/*                              _sdvReadOnly_EFun                          */
/************************************************************************/

static ST_VOID _sdvReadOnly_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _sdvReadOnly_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                        

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: SdvReadOnly required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &serverDvList->readOnly, "SdvReadOnly");

  }


/************************************************************************/
/*                       _sdvMapInfo_EFun                               */
/************************************************************************/


static ST_VOID _sdvMapInfo_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _sdvMapInfo_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             
             
  if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: SdvMapInfo too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (serverDvList->mapInfo,str);
    }

  }


/************************************************************************/
/*                      _serverDevices_SEFun                            */
/************************************************************************/

static ST_VOID _serverDevices_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _serverDevices_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (sdevElement);

    actualCount = 0;
    if (_getListCount (sxDecCtrl, &numDev) != SD_SUCCESS)
      return;

    numVccDev = 0;
    numIccDev = 0;

    serverDevList = NULL;
    ret = u_icfg_StartServerDevices     (userInfo, numDev);    
      if (ret != SD_SUCCESS)
        {
        ICFG_LOG_ERR1 ("ERROR: u_icfg_StartServerDevices returns: %d", ret);
        sxDecCtrl->errCode = SX_USER_ERROR;
        }
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {

    if (actualCount < numDev)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Sdv count of %d is less than Server 'NumDev' of %d ", actualCount, numDev);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
        
    /* validate input VCC parameters */
    if (numVccDev != sServerObjCounts.numVccDev)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Server NumVccDev count of %d does not equal Server 'NumVccDev' of %d ",
                  numVccDev, sServerObjCounts.numVccDev);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    /* validate input ICC parameters */
    else if (numIccDev != sServerObjCounts.numIccDev)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Server NumIccDev count of %d does not equal Server 'NumIccDev' of %d ",
                  numIccDev, sServerObjCounts.numIccDev);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    ret = u_icfg_EndServerDevices (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndServerDevices returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    }

  }


/************************************************************************/
/*                            _sdev_SERFun                              */
/************************************************************************/

static ST_VOID _sdev_SERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _sdev_SERFun");
             
  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (sdevElements);

    actualCount++;
    if (actualCount > numDev)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Sdev count of %d exceeds ServerDevices 'Count' value of %d ", actualCount, numDev);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    serverDevList = (ICFG_SERVER_DEV *) chk_calloc (1,sizeof(ICFG_SERVER_DEV));
    }

  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    ret = u_icfg_ServerDevice (userInfo, serverDevList);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_ServerDevice returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                          _sdevObjDeviceName_EFun                     */
/************************************************************************/

static ST_VOID _sdevObjDeviceName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_INT      i;
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT      validEnumVal;

  ICFG_LOG_FLOW0 ("SX DEC: _sdevObjDeviceName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                         

  if (strlen(str) == 0)   
    {
    ICFG_LOG_ERR0 ("ERROR: SdevObjDeviceName required but not present");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_IDENT_LEN - 4 + 1)           /*   " - 4 " allows room for _SBO or _TAG   */
    {
    ICFG_LOG_ERR1 ("ERROR: SdevObjDeviceName too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else         
    {
    /* make sure this is a valid enumeration value */
    i = 0;
    validEnumVal = SD_FALSE;
    while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_SCOPEVALS))
      {
      if (!strcmpi (icfgScopeValEnum[i], sxDecCtrl->sxDecElInfo.attr[0].value))
        {
        /* found Scope name, it is valid               */
        validEnumVal = SD_TRUE;
        serverDevList->scope = (enum icfgScopeVal)i;    

        strcpy (serverDevList->name,str);

        if ( !strcmpi (sxDecCtrl->sxDecElInfo.attr[0].value,"VCC") )   numVccDev++;
        if ( !strcmpi (sxDecCtrl->sxDecElInfo.attr[0].value,"ICC") )   numIccDev++;
        }
      else
        i++;
      }

      if (validEnumVal == SD_FALSE)
      {
      ICFG_LOG_ERR1     ("ERROR: Invalid Sdev Scope: %s",sxDecCtrl->sxDecElInfo.attr[0].value);
      sxDecCtrl->errCode = SX_USER_ERROR;

      ICFG_LOG_CERR0   ("       Valid values are: ");
      for (i=0; i < (NUM_OF_ENUM_SCOPEVALS); i++)
        {
        ICFG_LOG_CERR2 ("           %s",i+1,icfgScopeValEnum[i]);
        }
      }

    } /* else */

  }


/************************************************************************/
/*                         _sdevDevType_EFun                            */
/************************************************************************/


static ST_VOID _sdevDevType_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_INT      i;
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT      validEnumVal;

  ICFG_LOG_FLOW0 ("SX DEC: _sdevDevType_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             
    
  if (strlen(str) == 0)   
    {
    ICFG_LOG_ERR0 ("ERROR: SdevDevType required but not present");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  serverDevList->devType = (enum icfgDevTypeVal)INVALID_ENUM_VAL;
  /* make sure this is a valid enumeration value */
  i = 0;
  validEnumVal = SD_FALSE;
  while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_DEVTYPES))
    {
    if (!strcmp(icfgDevTypeValEnum[i],str))                
      {
      /* found enumerated name, it is valid               */
      validEnumVal = SD_TRUE;

      serverDevList->devType = (enum icfgDevTypeVal)i;    
      }
     else
      i++;
    }

  if (validEnumVal == SD_FALSE)
    {
    ICFG_LOG_ERR1     ("ERROR: Invalid Sdev DeviceType: %s",str);
    sxDecCtrl->errCode = SX_USER_ERROR;

    ICFG_LOG_CERR0   ("       Valid values are: ");
    for (i=0; i < (NUM_OF_ENUM_DEVTYPES); i++)
      {
      ICFG_LOG_CERR2 ("           %s",i+1,icfgDevTypeValEnum[i]);
      }
    }

  }


/************************************************************************/
/*                        _sdevMapInfo_EFun                             */
/************************************************************************/


static ST_VOID _sdevMapInfo_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _sdevMapInfo_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             
             
  if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: SdevMapInfo too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (serverDevList->mapInfo,str);
    }

  }


/************************************************************************/
/*                         _sdevTagEn_EFun                              */
/************************************************************************/

                           
static ST_VOID _sdevTagEn_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _sdevTagEn_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)   
    {
    ICFG_LOG_ERR0 ("ERROR: SdevTagEn required but not present");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &serverDevList->tagEnable, "SdevtagEnable");

  }


/************************************************************************/
/*                       _serverInfoMsgs_SEFun                          */
/************************************************************************/

static ST_VOID _serverInfoMsgs_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _serverInfoMsgs_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (simElement);

    actualCount = 0;
    if (_getListCount (sxDecCtrl, &numInfoMsg) != SD_SUCCESS)
      return;

    numVccInfoMsg = 0;
    numIccInfoMsg = 0;
    serverInfoMsgList = NULL;

    ret = u_icfg_StartServerInfoMsgs (userInfo, numInfoMsg);   
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartServerInfoMsgs returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    if (actualCount < numInfoMsg)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Sim count of %d is less than ServerInfoMsgs 'Count' of %d ", actualCount, numInfoMsg);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    /* validate input VCC parameters */
    if (numVccInfoMsg != sServerObjCounts.numVccInfoMsg)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Server NumVccInfoMsg count of %d does not equal Server 'NumVccInfoMsg' of %d ",
                  numVccInfoMsg, sServerObjCounts.numVccInfoMsg);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    /* validate input ICC parameters */
    else if (numIccInfoMsg != sServerObjCounts.numIccInfoMsg)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Server NumIccInfoMsg count of %d does not equal Server 'NumIccInfoMsg' of %d ",
                  numIccInfoMsg, sServerObjCounts.numIccInfoMsg);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    ret = u_icfg_EndServerInfoMsgs (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndServerInfoMsgs returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                             _sim_SERFun                              */
/************************************************************************/

static ST_VOID _sim_SERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _sim_SERFun");
             
  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (simElements);

    actualCount++;
    if (actualCount > numInfoMsg)
      {
      ICFG_LOG_ERR2 ("ERROR:  Calculated Sim count of %d exceeds ServerInfoMsgs 'Count' value of %d ", actualCount, numInfoMsg);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    serverInfoMsgList = (ICFG_SERVER_INFOMSG *) chk_calloc (1,sizeof(ICFG_SERVER_INFOMSG));
    }

  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    ret = u_icfg_ServerInfoMsg (userInfo, serverInfoMsgList);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_ServerInfoMsg returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }

  
/************************************************************************/
/*                         _simObjInfoRef_EFun                          */
/************************************************************************/

static ST_VOID _simObjInfoRef_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_INT      i;
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_LONG     sLong;
ST_INT      validEnumVal;

  ICFG_LOG_FLOW0 ("SX DEC: _simObjInfoRef_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                        
  
  sLong = 0;
  if (strlen(str) == 0)   
    {
    ICFG_LOG_ERR0 ("ERROR: SdevObjInfoRef required but not present");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if ( asciiToSlong(str, &sLong) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: SimObjInfoRef Data Conversion Error on value: %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    { 
    serverInfoMsgList->infoRef = sLong;
    }

    /* make sure this is a valid enumeration value */
    i = 0;
    validEnumVal = SD_FALSE;
    while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_SCOPEVALS))
      {
      if (!strcmpi (icfgScopeValEnum[i],sxDecCtrl->sxDecElInfo.attr[0].value))
        {
        /* found Scope name, it is valid               */
        validEnumVal = SD_TRUE;
        serverInfoMsgList->scope = (enum icfgScopeVal)i;    

        if ( !strcmpi (sxDecCtrl->sxDecElInfo.attr[0].value,"VCC") )   numVccInfoMsg++;
        if ( !strcmpi (sxDecCtrl->sxDecElInfo.attr[0].value,"ICC") )   numIccInfoMsg++;
        }
      else
        i++;
      }

    if (validEnumVal == SD_FALSE)
      {
      ICFG_LOG_ERR1     ("ERROR: Invalid Sim Scope: %s",sxDecCtrl->sxDecElInfo.attr[0].value);
      sxDecCtrl->errCode = SX_USER_ERROR;

      ICFG_LOG_CERR0   ("       Valid values are: ");
      for (i=0; i < (NUM_OF_ENUM_SCOPEVALS); i++)
        {
        ICFG_LOG_CERR1 ("           %s", icfgScopeValEnum[i]);
        }
      }
    
  }

                        
/************************************************************************/
/*                          _simMapInfo_EFun                            */
/************************************************************************/


static ST_VOID _simMapInfo_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _simMapInfo_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             
             
  if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: SimMapInfo too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (serverInfoMsgList->mapInfo,str);
    }

  }


/************************************************************************/
/*                         _serverDataSets_SEFun                        */
/************************************************************************/

static ST_VOID _serverDataSets_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;
        
  ICFG_LOG_FLOW0 ("SX DEC: _serverDataSets_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (sdsElement);

    actualCount = 0;
    if (_getListCount (sxDecCtrl, &numDs) != SD_SUCCESS)
      return;

    numVccDs = 0;
    numIccDs = 0;
    serverDsList = NULL;

    ret = u_icfg_StartServerDataSets (userInfo, numDs);   
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartServerDataSets returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    if (actualCount < numDs)
      {
      ICFG_LOG_ERR2 ("ERROR: Count value of %d is less than Server Data Sets count of %d ",actualCount, numDs);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    /* validate input VCC parameters */
   if (sServerObjCounts.numVccDs < numVccDs)
      {
      ICFG_LOG_ERR2 ("ERROR: Input Server NumVccDs count of %d is less than calculated Server NumVccDs of %d",
                  sServerObjCounts.numVccDs, numVccDs);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    /* validate input ICC parameters */
    else if (sServerObjCounts.numIccDs < numIccDs)
      {
      ICFG_LOG_ERR2 ("ERROR: Input Server NumIccDs count of %d is less than calculated Server NumIccDs of %d ",
                 sServerObjCounts.numIccDs,  numIccDs);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    ret = u_icfg_EndServerDataSets (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndServerDataSets returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                              _sds_SERFun                             */
/************************************************************************/

static ST_VOID _sds_SERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _sds_SERFun");
             
  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (sdsElements);

    actualCount++;
    if (actualCount > numDs)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Sds count of %d exceeds ServerDataSets 'Count' value of %d ", actualCount, numDs);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    serverDsList = (ICFG_SERVER_DS *) chk_calloc (1,sizeof(ICFG_SERVER_DS));
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    serverDsList->numDv = sdsDvsMaxCount;

    ret = u_icfg_ServerDataSet (userInfo, serverDsList);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_ServerDataSet returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }

  
/************************************************************************/
/*                           _sdsObjName_EFun                           */
/************************************************************************/

static ST_VOID _sdsObjName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_INT      i;
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT      validEnumVal;

  ICFG_LOG_FLOW0 ("SX DEC: _sdsObjName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                         

  if (strlen(str) == 0)   
    {
    ICFG_LOG_ERR0 ("ERROR: SdsObjName required but not present");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: SdsObjName too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: SdsObjName required but not present");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else         
    {
    /* make sure this is a valid enumeration value */
    i = 0;
    validEnumVal = SD_FALSE;
    while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_SCOPEVALS))
      {
      if (!strcmpi (icfgScopeValEnum[i], sxDecCtrl->sxDecElInfo.attr[0].value))
        {

        /* found Scope name, it is valid               */
        validEnumVal = SD_TRUE;
        serverDsList->scope = (enum icfgScopeVal)i;    

        strcpy (serverDsList->name,str);

        if ( !strcmpi (sxDecCtrl->sxDecElInfo.attr[0].value,"VCC") )   numVccDs++;
        if ( !strcmpi (sxDecCtrl->sxDecElInfo.attr[0].value,"ICC") )   numIccDs++;
        }
      else
        i++;
      }

      if (validEnumVal == SD_FALSE)
        {
        ICFG_LOG_ERR1     ("ERROR: Invalid Sds Scope: %s", sxDecCtrl->sxDecElInfo.attr[0].value);
        sxDecCtrl->errCode = SX_USER_ERROR;

        ICFG_LOG_CERR0   ("       Valid values are: ");
        for (i=0; i < (NUM_OF_ENUM_SCOPEVALS); i++)
          {
          ICFG_LOG_CERR1 ("           %s", icfgScopeValEnum[i]);
          }
      }

    } /* else */

  }


/************************************************************************/
/*                        _sdsTransferSetName_EFun                      */
/************************************************************************/


static ST_VOID _sdsTransferSetName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _sdsTransferSetName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)          
    {
    ICFG_LOG_ERR0 ("ERROR: CdsTransferSetName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &serverDsList->transferSetName, "SdsTransferSetName");

  }


/************************************************************************/
/*                      _sdsTransferSetTimeStamp_EFun                    */
/************************************************************************/


static ST_VOID _sdsTransferSetTimeStamp_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _sdsTransferSetTimeStamp_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)          
    {
    ICFG_LOG_ERR0 ("ERROR: CdsTransferSetTimeStamp required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &serverDsList->transferSetTimeStamp, "SdsTransferSetTimeStamp");
  }


/************************************************************************/
/*                       _sdsDsConditionsDetected_EFun                  */
/************************************************************************/


static ST_VOID _sdsDsConditionsDetected_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _sdsDsConditionsDetected_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)          
    {
    ICFG_LOG_ERR0 ("ERROR: CdsDsConditionsDetected required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &serverDsList->dsConditionsDetected, "SdsDsConditionsDetected");

  }


/************************************************************************/
/*                   _sdsEventCodeDetected_EFun                         */
/************************************************************************/


static ST_VOID _sdsEventCodeDetected_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _sdsEventCodeDetected_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)          
    {
    ICFG_LOG_ERR0 ("ERROR: CdsEventCodeDetected required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &serverDsList->eventCodeDetected, "SdsEventCodeDetected");

  }


/************************************************************************/
/*                           _sdsDescription_EFun                       */
/************************************************************************/


static ST_VOID _sdsDescription_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _sdsDescription_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) > MAX_DESCRIPTION_LEN + 1)
    {
    strncpy (serverDsList->description, str, MAX_DESCRIPTION_LEN + 1);
    }
  else
    {
    strcpy (serverDsList->description,str);
    }

  }


/************************************************************************/
/*                             _sdsDvs_SEFun                            */
/************************************************************************/

static ST_VOID _sdsDvs_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {

  ICFG_LOG_FLOW0 ("SX DEC: _sdsDvs_SEFun");
             
   if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH ( sdsDvsElement);

    sdsDvsActualCount = 0; 
    if (_getListCount (sxDecCtrl, &sdsDvsMaxCount) != SD_SUCCESS)
      return;
    else
      serverDsList->numDv = sdsDvsMaxCount;
    if (sdsDvsMaxCount != 0)
      {
      if (serverDsList != NULL)
        {
/*        serverDsList = (icfg_server_ds *)chk_realloc (serverDsList, (sizeof(ICFG_SERVER_DS) + (sdsDvsMaxCount * sizeof(ICFG_SERVER_DS_DV))) );
modified 2006.11.23 zy*/
        serverDsList = (struct icfg_client_ds *)chk_realloc (serverDsList, (sizeof(ICFG_SERVER_DS) + (sdsDvsMaxCount * sizeof(ICFG_SERVER_DS_DV))) );
        serverDsList->dvArray = (ICFG_SERVER_DS_DV *) (serverDsList + 1);
        }
      }                                              
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    if (sdsDvsActualCount < sdsDvsMaxCount)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated SDsDv Count of %d is less than SdsDvs 'Count' of %d ", sdsDvsActualCount, sdsDvsMaxCount);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }

  
/************************************************************************/
/*                             _sDsDv_ERFun                             */
/************************************************************************/

static ST_VOID _sDsDv_ERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_INT      i;
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT      validEnumVal;

  ICFG_LOG_FLOW0 ("SX DEC: _sDsDv_ERFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (sdsDvsActualCount > sdsDvsMaxCount)
    {
    ICFG_LOG_ERR2 ("ERROR: Calculated SdsDv count of %d exceeds SDsDvs 'Count' value of %d ", sdsDvsActualCount, sdsDvsMaxCount);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    if (strlen(sxDecCtrl->sxDecElInfo.attr[1].value) == 0)   
      {
      ICFG_LOG_ERR0 ("ERROR: SdsDvName required but not present");
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    else if ((serverDsList != NULL)  && (sdsDvsMaxCount != 0))
        {
        strcpy (serverDsList->dvArray[sdsDvsActualCount].name,sxDecCtrl->sxDecElInfo.attr[1].value);
        }

    /* make sure input scope is a valid enumeration value */
    i = 0;
    validEnumVal = SD_FALSE;
    while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_SCOPEVALS))
      {

      if (!strcmpi (icfgScopeValEnum[i],sxDecCtrl->sxDecElInfo.attr[0].value))
        {
        /* found Scope name, it is valid               */
        validEnumVal = SD_TRUE;
        if ((serverDsList != NULL) && (sdsDvsMaxCount != 0))
                serverDsList->dvArray[sdsDvsActualCount].scope = (enum icfgScopeVal)i;
        }
      else
        i++;

      }

    if (validEnumVal == SD_FALSE)
      {
      ICFG_LOG_ERR1     ("ERROR: Invalid SdsDv Scope: %s",sxDecCtrl->sxDecElInfo.attr[0].value);
      sxDecCtrl->errCode = SX_USER_ERROR;

      ICFG_LOG_CERR0   ("       Valid values are: ");
      for (i=0; i < (NUM_OF_ENUM_SCOPEVALS); i++)
        {
        ICFG_LOG_CERR1 ("           %s", icfgScopeValEnum[i]);
        }
      }

    sdsDvsActualCount++;
    } /* else */
   
  }


/************************************************************************/
/*                            _clientObjects_SEFun                      */
/************************************************************************/

static ST_VOID _clientObjects_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _clientObjects_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (clientObjsElements);

    clientObjCounts = NULL;
    clientObjCounts = (ICFG_CLIENT_OBJ_COUNTS *) chk_calloc (1,sizeof(ICFG_CLIENT_OBJ_COUNTS));
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {

    ret = u_icfg_EndClientObjects (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndClientObjects returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    }

  }


/************************************************************************/
/*                        _clientNumVccDv_EFun                          */
/************************************************************************/


static ST_VOID _clientNumVccDv_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _clientNumVccDv_EFun");
  rc = sx_get_long (sxDecCtrl, &clientObjCounts->numVccDv);
  if (rc != SD_SUCCESS || clientObjCounts->numVccDv < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ClientNumVccDv Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }


/************************************************************************/                                   
/*                          _clientNumVccDev_EFun                       */
/************************************************************************/

                          
static ST_VOID _clientNumVccDev_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _clientNumVccDev_EFun");
  rc = sx_get_long (sxDecCtrl, &clientObjCounts->numVccDev);
  if (rc != SD_SUCCESS || clientObjCounts->numVccDev < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ClientNumVccDev Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
    
  }   


/************************************************************************/
/*                       _clientNumVccInfoMsg_EFun                      */
/************************************************************************/

static ST_VOID _clientNumVccInfoMsg_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _clientNumVccInfoMsg_EFun");
  rc = sx_get_long (sxDecCtrl, &clientObjCounts->numVccInfoMsg);
  if (rc != SD_SUCCESS || clientObjCounts->numVccInfoMsg < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ClientNumVccInfoMsg Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }


/************************************************************************/
/*                     _clientNumIccDv_EFun                             */
/************************************************************************/


static ST_VOID _clientNumIccDv_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _clientNumIccDv_EFun");
  rc = sx_get_long (sxDecCtrl, &clientObjCounts->numIccDv);
  if (rc != SD_SUCCESS || clientObjCounts->numIccDv < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ClientNumIccDv Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
     
  }


/************************************************************************/                                   
/*                          _clientNmIccDev_EFun                        */
/************************************************************************/

                          
static ST_VOID _clientNumIccDev_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _clientNumIccDev_EFun");
  rc = sx_get_long (sxDecCtrl, &clientObjCounts->numIccDev);
  if (rc != SD_SUCCESS || clientObjCounts->numIccDev < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ClientNumIccDev Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
    
  }   


/************************************************************************/
/*                       _clientNumIccInfoMsg_EFun                      */
/************************************************************************/

static ST_VOID _clientNumIccInfoMsg_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _clientNumIccInfoMsg_EFun");
  rc = sx_get_long (sxDecCtrl, &clientObjCounts->numIccInfoMsg);
  if (rc != SD_SUCCESS || clientObjCounts->numIccInfoMsg < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ClientNumIccInfoMsg Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }


/************************************************************************/
/*                            _clientNumDs_EFun                         */
/************************************************************************/

static ST_VOID _clientNumDs_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;

  ICFG_LOG_FLOW0 ("SX DEC: _clientNumDs_EFun");
  rc = sx_get_long (sxDecCtrl, &clientObjCounts->numDs);
  if (rc != SD_SUCCESS || clientObjCounts->numDs < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ClientNumDs Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
             
  }


/************************************************************************/
/*                            _clientNumDsTs_EFun                       */
/************************************************************************/

static ST_VOID _clientNumDsTs_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_RET      ret;

  ICFG_LOG_FLOW0 ("SX DEC: _clientNumDsTs_EFun");
  rc = sx_get_long (sxDecCtrl, &clientObjCounts->numDsTs);
  if (rc != SD_SUCCESS || clientObjCounts->numDsTs < 0)
    {
    ICFG_LOG_ERR0 ("ERROR: ClientNumDsTs Conversion Error");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  /* save all counts in case "clientObjCounts" is freed in callback routines */
  memcpy (&cClientObjCounts, clientObjCounts, sizeof(ICFG_CLIENT_OBJ_COUNTS));

  /* this would normally be done in clientObjects_SEFun but is done here so  */
  /* u_icfg_StartClientObjectss may be called with valid data                */

  ret = u_icfg_StartClientObjects (userInfo, clientObjCounts);
  if (ret != SD_SUCCESS)
    {
    ICFG_LOG_ERR1 ("ERROR: u_icfg_StartClientObjects returns: %d", ret);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
   
  }


/************************************************************************/
/*                        _clientDataValues_SEFun                       */
/************************************************************************/

static ST_VOID _clientDataValues_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _clientDataValues_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (cdvElement);

    actualCount = 0;
    if (_getListCount (sxDecCtrl, &numDv) != SD_SUCCESS)
      return;

    numVccDv = 0;
    numIccDv = 0;

    clientDvList = NULL;
    ret = u_icfg_StartClientDataValues (userInfo, numDv);   
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartClientDataValues returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {

    if (actualCount < numDv)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Cdv count of %d is less than ClientDataValues 'Count' of %d ", actualCount, numDv);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    /* validate input VCC parameters */
    if (numVccDv != cClientObjCounts.numVccDv)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Client NumVccDv count of %d does not equal Client 'NumVccDv' of %d ",
                  numVccDv, cClientObjCounts.numVccDv);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    /* validate input ICC parameters */
    else if (numIccDv != cClientObjCounts.numIccDv)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Client NumIccDv count of %d does not equal Client 'NumIccDv' of %d ",
                  numIccDv, cClientObjCounts.numIccDv);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    ret = u_icfg_EndClientDataValues (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndClientDataValues returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                              _cdv_SERFun                             */
/************************************************************************/

static ST_VOID _cdv_SERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _cdv_SERFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (cdvElements);

    actualCount++;
    if (actualCount > numDv)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Cdv count of %d exceeds ClientDataValues 'Count' of %d ", actualCount, numDv);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    clientDvList = (ICFG_CLIENT_DV *) chk_calloc (1,sizeof(ICFG_CLIENT_DV));

    /* initialize */
    clientDvList->dvType = (enum icfgDvTypeVal)INVALID_ENUM_VAL;       /* set this to something else because "0" is a valid enum value */
    }

  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    ret = u_icfg_ClientDataValue (userInfo, clientDvList);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_ClientDataValue returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                           _cdvObjName_EFun                           */
/************************************************************************/


static ST_VOID _cdvObjName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_INT      i;
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT      validEnumVal;

  ICFG_LOG_FLOW0 ("SX DEC: _cdvObjName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                         

  if (strlen(str) == 0)   
    {
    ICFG_LOG_ERR0 ("ERROR: CdvObjName required but not present");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: cdvObjName too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: cdvObjName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else         
    {
    /* make sure this is a valid enumeration value */
    i = 0;
    validEnumVal = SD_FALSE;
    while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_SCOPEVALS))
      {
      if (!strcmpi (icfgScopeValEnum[i], sxDecCtrl->sxDecElInfo.attr[0].value))
        {

        /* found Scope name, it is valid               */
        validEnumVal = SD_TRUE;
        clientDvList->scope = (enum icfgScopeVal)i;    

        strcpy (clientDvList->name,str);

        if ( !strcmpi (sxDecCtrl->sxDecElInfo.attr[0].value,"VCC") )   numVccDv++;
        if ( !strcmpi (sxDecCtrl->sxDecElInfo.attr[0].value,"ICC") )   numIccDv++;

        }
      else
        i++;
      }

      if (validEnumVal == SD_FALSE)
        {
        ICFG_LOG_ERR1     ("ERROR: Invalid Cdv Scope: %s",sxDecCtrl->sxDecElInfo.attr[0].value);
        sxDecCtrl->errCode = SX_USER_ERROR;

        ICFG_LOG_CERR0   ("       Valid values are: ");
        for (i=0; i < (NUM_OF_ENUM_SCOPEVALS); i++)
          {
          ICFG_LOG_CERR1 ("           %s", icfgScopeValEnum[i]);
          }
      }
    } /* else */

  }


/************************************************************************/
/*                            _cdvDvType_EFun                           */
/************************************************************************/


static ST_VOID _cdvDvType_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_INT      i;
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT      validEnumVal;

  ICFG_LOG_FLOW0 ("SX DEC: _cdvDvType_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             
             
  if (strlen(str) == 0)   
    {
    ICFG_LOG_ERR0 ("ERROR: CdvDvType required but not present");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  /* make sure this is a valid enumeration value */
  i = 0;
  validEnumVal = SD_FALSE;
  while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_DVTYPES))
    {
    if (!strcmp(icfgDvTypeValEnum[i],str))                 
      {
      /* found enumerated name, it is valid               */
      validEnumVal = SD_TRUE;

      clientDvList->dvType = (enum icfgDvTypeVal)i;    
      }
     else
      i++;
    }

  if (validEnumVal == SD_FALSE)
    {
    ICFG_LOG_ERR1 ("ERROR: Invalid Cdv DataType: %s",str);
    sxDecCtrl->errCode = SX_USER_ERROR;

    ICFG_LOG_CERR0   ("       Valid values are: ");
    for (i=0; i < (NUM_OF_ENUM_DVTYPES); i++)
      {
      ICFG_LOG_CERR1 ("           %s", icfgDvTypeValEnum[i]);
      }
    }

  }


/************************************************************************/
/*                            _cdvReadOnly_EFun                            */
/************************************************************************/


static ST_VOID _cdvReadOnly_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdvReadOnly_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                        

  if (strlen(str) == 0)   
    {
    ICFG_LOG_ERR0 ("ERROR: CdvReadOnly required but not present");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &clientDvList->readOnly, "CdvReadOnly");

  }


/************************************************************************/
/*                           _cdvMapInfo_EFun                              */
/************************************************************************/


static ST_VOID _cdvMapInfo_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdvMapInfo_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             
             
  if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: CdvMapInfo too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (clientDvList->mapInfo,str);
    }

  }


/************************************************************************/
/*                         _cdvDescription_EFun                         */
/************************************************************************/


static ST_VOID _cdvDescription_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdvDescription_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) > MAX_DESCRIPTION_LEN + 1)
    {
    strncpy (clientDvList->description, str, MAX_DESCRIPTION_LEN + 1);
    }
  else
    {
    strcpy (clientDvList->description,str);
    }

  }


/************************************************************************/
/*                        _clientDevices_SEFun                          */
/************************************************************************/

static ST_VOID _clientDevices_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _clientDevices_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (cdevElement);

    actualCount = 0;
    if (_getListCount (sxDecCtrl, &numDev) != SD_SUCCESS)
      return;

    numVccDev = 0;
    numIccDev = 0;
    clientDevList = NULL;

    ret = u_icfg_StartClientDevices     (userInfo, numDev);   
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartClientDevices returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {

    if (actualCount < numDev)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Cdev count of %d is less than ClientDevices 'Count' of %d ", actualCount, numDev);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    /* validate input VCC parameters */
    if (numVccDev != cClientObjCounts.numVccDev)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Client NumVccDev count of %d does not equal Client 'NumVccDev' of %d ",
                  numVccDev, cClientObjCounts.numVccDev);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    /* validate input ICC parameters */
    else if (numIccDev != cClientObjCounts.numIccDev)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Client NumIccDev count of %d does not equal client 'NumIccDev' of %d ",
                  numIccDev, cClientObjCounts.numIccDev);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    ret = u_icfg_EndClientDevices (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndClientDevices returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                            _cdev_SERFun                              */
/************************************************************************/

static ST_VOID _cdev_SERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _cdev_SERFun");
             
  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (cdevElements);

    actualCount++;
    if (actualCount > numDev)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Cdev count of %d exceeds ClientDevices 'Count' value of %d ", actualCount, numDev);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    clientDevList = (ICFG_CLIENT_DEV *) chk_calloc (1,sizeof(ICFG_CLIENT_DEV));

    chkBackIdEntered = SD_FALSE;
    clientDevList->checkBackId = Default_CheckBackId;

    }

  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    if ( (clientDevList->sbo == SD_TRUE) && (chkBackIdEntered == SD_FALSE) )
      {
      ICFG_LOG_FLOW1 ("INFO: SBO='Y' but CheckBackId was not entered.  Used default: %d", Default_CheckBackId);
      }

    if ( (clientDevList->sbo == SD_FALSE) && (chkBackIdEntered == SD_TRUE) )
      {
      ICFG_LOG_FLOW1 ("INFO: SBO='N' but CheckBackId: %d was entered.", clientDevList->checkBackId);
      }
     
    ret = u_icfg_ClientDevice (userInfo, clientDevList);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_ClientDevice returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    }

  }

  
/************************************************************************/
/*                      _cdevObjDeviceName_EFun                         */
/************************************************************************/


static ST_VOID _cdevObjDeviceName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_INT      i;
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT      validEnumVal;

  ICFG_LOG_FLOW0 ("SX DEC: _cdevObjDeviceName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                         

  if (strlen(str) == 0)          
    {
    ICFG_LOG_ERR0 ("ERROR: CdevObjDeviceName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_IDENT_LEN - 4 + 1)           /*   " - 4 " allows room for _SBO or _TAG   */
    {
    ICFG_LOG_ERR1 ("ERROR: CdevObjDeviceName too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else         
    {
    /* make sure this is a valid enumeration value */
    i = 0;
    validEnumVal = SD_FALSE;
    while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_SCOPEVALS))
      {
      if (!strcmpi (icfgScopeValEnum[i], sxDecCtrl->sxDecElInfo.attr[0].value))
        {
        /* found Scope name, it is valid               */
        validEnumVal = SD_TRUE;
        clientDevList->scope = (enum icfgScopeVal)i;    

        strcpy (clientDevList->name,str);

        if ( !strcmpi (sxDecCtrl->sxDecElInfo.attr[0].value,"VCC") )   numVccDev++;
        if ( !strcmpi (sxDecCtrl->sxDecElInfo.attr[0].value,"ICC") )   numIccDev++;
        }
      else
        i++;
      }

      if (validEnumVal == SD_FALSE)
      {
      ICFG_LOG_ERR1     ("ERROR: Invalid Cdev Scope: %s",sxDecCtrl->sxDecElInfo.attr[0].value);
      sxDecCtrl->errCode = SX_USER_ERROR;

      ICFG_LOG_CERR0   ("       Valid values are: ");
      for (i=0; i < (NUM_OF_ENUM_SCOPEVALS); i++)
        {
        ICFG_LOG_CERR1 ("           %s", icfgScopeValEnum[i]);
        }
      }
    } /* else */

  }


/************************************************************************/
/*                       _cdevDevType_EFun                              */
/************************************************************************/


static ST_VOID _cdevDevType_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_INT      i;
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT      validEnumVal;

  ICFG_LOG_FLOW0 ("SX DEC: _cdevDevType_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             

  if (strlen(str) == 0)          
    {
    ICFG_LOG_ERR0 ("ERROR: CdevDevType required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
    
  clientDevList->devType = (enum icfgDevTypeVal)INVALID_ENUM_VAL;
  /* make sure this is a valid enumeration value */
  i = 0;
  validEnumVal = SD_FALSE;
  while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_DEVTYPES))
    {
    if (!strcmp(icfgDevTypeValEnum[i],str))                
      {
      /* found enumerated name, it is valid               */
      validEnumVal = SD_TRUE;

      clientDevList->devType = (enum icfgDevTypeVal)i;    
      }
     else
      i++;
    }

  if (validEnumVal == SD_FALSE)
    {
    ICFG_LOG_ERR1     ("ERROR: Invalid Cdev DeviceType: %s",str);
    sxDecCtrl->errCode = SX_USER_ERROR;

    ICFG_LOG_CERR0   ("       Valid values are: ");
    for (i=0; i < (NUM_OF_ENUM_DEVTYPES); i++)
      {
      ICFG_LOG_CERR1 ("           %s", icfgDevTypeValEnum[i]);
      }
    }

  }


/************************************************************************/
/*                         _cdevMapInfo_EFun                             */
/************************************************************************/


static ST_VOID _cdevMapInfo_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdevMapInfo_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             
             
  if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: CdevMapInfo too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (clientDevList->mapInfo,str);
    }

  }


/************************************************************************/
/*                            _cdevSbo_EFun                             */
/************************************************************************/


static ST_VOID _cdevSbo_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdevSbo_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)          
    {
    ICFG_LOG_ERR0 ("ERROR: CdevSbo required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &clientDevList->sbo, "CdevSbo");

  }


/************************************************************************/
/*                           _cdevCeckBackId_EFun                       */
/************************************************************************/


static ST_VOID _cdevCheckBackId_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT16     sInt16;

  ICFG_LOG_FLOW0 ("SX DEC: _cdevCheckBackId_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  sInt16 = 0;
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: CdevCheckBackId required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if ( asciiToSint16(str, &sInt16) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: CdevCheckBackId Conversion Error on value of %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    { /* valid value */
    if (clientDevList != NULL)
       clientDevList->checkBackId = sInt16;
	chkBackIdEntered = SD_TRUE;
    }

  }


/************************************************************************/
/*                         _cdevTagEn_EFun                              */
/************************************************************************/


static ST_VOID _cdevTagEn_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdevTagEn_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)          
    {
    ICFG_LOG_ERR0 ("ERROR: CdevTagEn required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &clientDevList->tagEnable, "CdevTagEnable");

  }


/************************************************************************/
/*                       _cdevDescription_EFun                          */
/************************************************************************/


static ST_VOID _cdevDescription_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdevDescription_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) > MAX_DESCRIPTION_LEN + 1)
    {
    strncpy (clientDevList->description, str, MAX_DESCRIPTION_LEN + 1);
    }
  else
    {
    strcpy (clientDevList->description,str);
    }

  }


/************************************************************************/
/*                         _clientInfoMsgs_SEFun                        */
/************************************************************************/

static ST_VOID _clientInfoMsgs_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _clientInfoMsgs_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (cimElement);

    actualCount = 0;
    if (_getListCount (sxDecCtrl, &numInfoMsg) != SD_SUCCESS)
      return;

    numVccInfoMsg = 0;
    numIccInfoMsg = 0;
    clientInfoMsgList = NULL;

    ret = u_icfg_StartClientInfoMsgs (userInfo, numInfoMsg);  
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartClientInfoMsgs returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {

    if (actualCount < numInfoMsg)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Cim count of %d exceeds ClientInfoMsgs 'Count' of %d ", actualCount, numInfoMsg);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    /* validate input VCC parameters */
    if (numVccInfoMsg != cClientObjCounts.numVccInfoMsg)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Client NumVccInfoMsgs of %d does not equal Client 'NumVccInfoMsg' of %d ",
                  numVccInfoMsg, cClientObjCounts.numVccInfoMsg);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    /* validate input ICC parameters */
    else if (numIccInfoMsg != cClientObjCounts.numIccInfoMsg)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Client NumIccInfoMsgs of %d does not equal input Client 'NumIccInfoMsg' of %d ",
                  numIccInfoMsg, cClientObjCounts.numIccInfoMsg);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    ret = u_icfg_EndClientInfoMsgs (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndCLientInfoMsgs returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                               _cim_SERFun                            */
/************************************************************************/

static ST_VOID _cim_SERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _cim_SERFun");
         
  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (cimElements);

    actualCount++;
    if (actualCount > numInfoMsg)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Client Cim count of %d exceeds ClientInfoMsgs 'Count' of %d ", actualCount, numInfoMsg);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    clientInfoMsgList = (ICFG_CLIENT_INFOMSG *) chk_calloc (1,sizeof(ICFG_CLIENT_INFOMSG));
    }

  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    ret = u_icfg_ClientInfoMsg (userInfo, clientInfoMsgList);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_ClientInfoMsg returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }

  
/************************************************************************/
/*                          _cimObjInfoRef_EFun                         */
/************************************************************************/


static ST_VOID _cimObjInfoRef_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_INT      i;
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_LONG     sLong;
ST_INT      validEnumVal;

  ICFG_LOG_FLOW0 ("SX DEC: _cimObjInfoRef_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                        
  
  sLong = 0;
  if (strlen(str) == 0)          
    {
    ICFG_LOG_ERR0 ("ERROR: CimObjInfoRef required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if ( asciiToSlong(str, &sLong) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: CimObjInfoRef Data Conversion Error on value: %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    { 
    clientInfoMsgList->infoRef = sLong;
    }

    /* make sure this is a valid enumeration value */
    i = 0;
    validEnumVal = SD_FALSE;
    while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_SCOPEVALS))
      {
      if (!strcmpi (icfgScopeValEnum[i],sxDecCtrl->sxDecElInfo.attr[0].value))
        {
        /* found Scope name, it is valid               */
        validEnumVal = SD_TRUE;
        clientInfoMsgList->scope = (enum icfgScopeVal)i;    

        if ( !strcmpi (sxDecCtrl->sxDecElInfo.attr[0].value,"VCC") )   numVccInfoMsg++;
        if ( !strcmpi (sxDecCtrl->sxDecElInfo.attr[0].value,"ICC") )   numIccInfoMsg++;
        }
      else
        i++;
      }

    if (validEnumVal == SD_FALSE)
      {
      ICFG_LOG_ERR1     ("ERROR: Invalid Cim Scope: %s",sxDecCtrl->sxDecElInfo.attr[0].value);
      sxDecCtrl->errCode = SX_USER_ERROR;

      ICFG_LOG_CERR0   ("       Valid values are: ");
      for (i=0; i < (NUM_OF_ENUM_SCOPEVALS); i++)
        {
        ICFG_LOG_CERR1 ("           %s", icfgScopeValEnum[i]);
        }
      }
    
  }

                        
/************************************************************************/
/*                          _cimMapInfo_EFun                            */
/************************************************************************/


static ST_VOID _cimMapInfo_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cimMapInfo_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             
             
  if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: CimMapInfo too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (clientInfoMsgList->mapInfo,str);
    }

  }


/************************************************************************/
/*                           _cimMaxSize_EFun                           */
/************************************************************************/

static ST_VOID _cimMaxSize_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_LONG     sLong;

  ICFG_LOG_FLOW0 ("SX DEC: _cimMaxSize_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;     
   
  sLong = 0;
  if (strlen(str) == 0)          
    {
    ICFG_LOG_ERR0 ("ERROR: CimMaxSize required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if ( asciiToSlong(str, &sLong) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: CimMaxSize Conversion Error on value: %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    { 
    clientInfoMsgList->maxSize = sLong;
    }

  }


/************************************************************************/
/*                          _cimDescription_EFun                        */
/************************************************************************/


static ST_VOID _cimDescription_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cimDescription_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) > MAX_DESCRIPTION_LEN + 1)
    {
    strncpy (clientInfoMsgList->description, str, MAX_DESCRIPTION_LEN + 1);
    }
  else
    {
    strcpy (clientInfoMsgList->description,str);
    }

  }


/************************************************************************/
/*                         _clientDataSets_SEFun                        */
/************************************************************************/

static ST_VOID _clientDataSets_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _clientDataSets_SEFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (cdsElement);

    actualCount = 0;
    if (_getListCount (sxDecCtrl, &numDs) != SD_SUCCESS)
      return;
    
    clientDsList = NULL;
    ret = u_icfg_StartClientDataSets (userInfo, numDs);   
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartClientDataSets returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {

    if (actualCount < numDs)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Cds Count of %d is less than ClientDataSets 'Count' of %d ", actualCount, numDs);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    else if (cClientObjCounts.numDs < actualCount)
      {
      ICFG_LOG_ERR2 ("ERROR: Client 'NumDs' of %d is less than calculated Cds Count of %d  ",
                  cClientObjCounts.numDs, actualCount);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    ret = u_icfg_EndClientDataSets (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndCLientDataSets returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    }
                             
  }


/************************************************************************/
/*                              _cds_SERFun                             */
/************************************************************************/

static ST_VOID _cds_SERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _cds_SERFun");
             
   if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (cdsElements);

    actualCount++;
    if (actualCount > numDs)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated Cds count of %d exceeds ClientDataSets 'Count' of %d ", actualCount, numDs);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    clientDsList = (ICFG_CLIENT_DS *) chk_calloc (1,sizeof(ICFG_CLIENT_DS));
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    clientDsList->numDv = cdsDvsMaxCount;

    ret = u_icfg_ClientDataSet (userInfo, clientDsList);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_ClientDataSet returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    clientDsDvList = NULL;
    }

  }

  
/************************************************************************/
/*                                  _cdsName_EFun                          */
/************************************************************************/

static ST_VOID _cdsName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdsName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: CdsName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: CdsName too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (clientDsList->name,str);
    }

  }


/************************************************************************/
/*                        _cdsTransferSetName_EFun                      */
/************************************************************************/


static ST_VOID _cdsTransferSetName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdsTransferSetName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)          
    {
    ICFG_LOG_ERR0 ("ERROR: CdsTransferSetName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &clientDsList->transferSetName, "CdsTransferSetName");

  }


/************************************************************************/
/*                      _cdsDsConditionDetected_EFun                    */
/************************************************************************/


static ST_VOID _cdsTransferSetTimeStamp_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdsTransferSetTimeStamp_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)          
    {
    ICFG_LOG_ERR0 ("ERROR: CdsTransferSetTimeStamp required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &clientDsList->transferSetTimeStamp, "CdsTransferSetTimeStamp");

  }


/************************************************************************/
/*                       _cdsDsConditionsDetected_EFun                  */
/************************************************************************/


static ST_VOID _cdsDsConditionsDetected_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdsDsConditionsDetected_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)          
    {
    ICFG_LOG_ERR0 ("ERROR: CdsDsConditionsDetected required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &clientDsList->dsConditionsDetected, "CdsDsConditionsDetected");

  }


/************************************************************************/
/*                   _cdsEventCodeDetected_EFun                         */
/************************************************************************/


static ST_VOID _cdsEventCodeDetected_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdsEventCodeDetected_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)          
    {
    ICFG_LOG_ERR0 ("ERROR: CdsEventCodeDetected required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &clientDsList->eventCodeDetected, "CdsEventCodeDetected");

  }


/************************************************************************/
/*                            _cdsDescription_EFun                      */
/************************************************************************/


static ST_VOID _cdsDescription_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdsDescription_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) > MAX_DESCRIPTION_LEN + 1)
    {
    strncpy (clientDsList->description, str, MAX_DESCRIPTION_LEN + 1);
    }
  else
    {
    strcpy (clientDsList->description,str);
    }

  }


/************************************************************************/
/*                                _cdsDvs_SEFun                         */
/************************************************************************/

static ST_VOID _cdsDvs_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {

  ICFG_LOG_FLOW0 ("SX DEC: _cdsDvs_SEFun");
             
  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (cdsDvElement);

    cdsDvsActualCount = 0;
    if (_getListCount (sxDecCtrl, &cdsDvsMaxCount) != SD_SUCCESS)
      return;

    if ((clientDsList != NULL) && (cdsDvsMaxCount != 0))
      {
      if (clientDsList != NULL)
        {
/*        clientDsList = (icfg_client_ds *)chk_realloc (clientDsList, (sizeof(ICFG_CLIENT_DS) + (cdsDvsMaxCount * sizeof(ICFG_CLIENT_DS_DV))) );
modified 2003.11.23 zy*/
        clientDsList = (struct icfg_client_ds *)chk_realloc (clientDsList, (sizeof(ICFG_CLIENT_DS) + (cdsDvsMaxCount * sizeof(ICFG_CLIENT_DS_DV))) );
        clientDsList->dvArray = (ICFG_CLIENT_DS_DV *) (clientDsList + 1);
        }
      }
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    if (cdsDvsActualCount < cdsDvsMaxCount)
      {
      ICFG_LOG_ERR2 ("ERROR: CDsDvs 'Count' of %d is less than calculated CDsDv count of %d ", cdsDvsMaxCount, cdsDvsActualCount);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }

  
/************************************************************************/
/*                              _cdsDv_ERFun                            */
/************************************************************************/

static ST_VOID _cdsDv_ERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_INT      i;
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT      validEnumVal;

  ICFG_LOG_FLOW0 ("SX DEC: _cdsDv_ERFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (cdsDvsActualCount > cdsDvsMaxCount-1)
    {
    ICFG_LOG_ERR2 ("ERROR: Calculated CDsDv of %d exceeds CDsDvs 'Count' value of %d ", cdsDvsActualCount, cdsDvsMaxCount);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    if (strlen(sxDecCtrl->sxDecElInfo.attr[1].value) == 0)
      {
      ICFG_LOG_ERR0 ("ERROR: CdsDvName required but not present ");
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    else if (clientDsList != NULL)
      {
      strcpy (clientDsList->dvArray[cdsDvsActualCount].name,sxDecCtrl->sxDecElInfo.attr[1].value);
      }

    /* make sure inpput scope is a valid enumeration value */
    i = 0;
    validEnumVal = SD_FALSE;
    while ((validEnumVal == SD_FALSE) && (i < NUM_OF_ENUM_SCOPEVALS))
      { 
      if (!strcmpi (icfgScopeValEnum[i],sxDecCtrl->sxDecElInfo.attr[0].value))
        {
        /* found Scope name, it is valid               */
        validEnumVal = SD_TRUE;
        if ((clientDsList != NULL) && (cdsDvsMaxCount != 0))
          clientDsList->dvArray[cdsDvsActualCount].scope = (enum icfgScopeVal)i;
        }
      else
        i++;
      }

    if (validEnumVal == SD_FALSE)
      {
      ICFG_LOG_ERR1     ("ERROR: Invalid CdsDv Scope: %s",sxDecCtrl->sxDecElInfo.attr[0].value);
      sxDecCtrl->errCode = SX_USER_ERROR;

      ICFG_LOG_CERR0   ("       Valid values are: ");
      for (i=0; i < (NUM_OF_ENUM_SCOPEVALS); i++)
        {
        ICFG_LOG_CERR1 ("           %s", icfgScopeValEnum[i]);
        }
      }
   
    cdsDvsActualCount++;
    }  /* else */

  }


/************************************************************************/
/*                          _clientDSTSs_SEFun                          */
/************************************************************************/

static ST_VOID _clientDSTSs_SEFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _clientDSTSs_SEFun");

   if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (cdstsElement);

    actualCount = 0;
    if (_getListCount (sxDecCtrl, &numDsTs) != SD_SUCCESS)
      return;
 
    clientDsTsList = NULL;

    ret = u_icfg_StartClientDSTSs (userInfo, numDsTs); 
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_StartClientDSTSs returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    if (actualCount < numDsTs)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated CDsTs count of %d is less than  ClientDataSetTransferSets 'Count' of %d ", numDsTs, actualCount);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    else if (actualCount != cClientObjCounts.numDsTs)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated CDsTs of %d not equal Client 'NumDsTs' of %d  ",
                  actualCount, cClientObjCounts.numDsTs);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    ret = u_icfg_EndClientDSTSs (userInfo);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_EndClientDSTSs returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }
    }

  }


/************************************************************************/
/*                              _cdsts_SERFun                           */
/************************************************************************/

static ST_VOID _cdsts_SERFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_RET  ret;

  ICFG_LOG_FLOW0 ("SX DEC: _cdsts_SERFun");
             
  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (cdstsElements);

    actualCount++;
    if (actualCount > numDsTs)
      {
      ICFG_LOG_ERR2 ("ERROR: Calculated CDsTs of %d exceeds ClientDataSetTransferSets 'Count' value of %d ", actualCount, numDsTs);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }

    clientDsTsList = (ICFG_CLIENT_DSTS *) chk_calloc (1,sizeof(ICFG_CLIENT_DSTS));

    bufferTimeEntered = SD_FALSE;
    clientDsTsList->bufferTime = Default_BufferTime;

    integrityCheckEntered =SD_FALSE;
    clientDsTsList->integrity = Default_IntegrityCheck;
    clientDsTsList->tle = Default_Tle;
    }
    
  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    if ( (clientDsTsList->rbe == SD_TRUE) && (bufferTimeEntered == SD_FALSE) )
      {
      ICFG_LOG_FLOW1 ("INFO: RBE='Y' but BufferTime was not entered.  Used default: %d", Default_BufferTime);
      }

    if ( (clientDsTsList->rbe == SD_FALSE) && (bufferTimeEntered == SD_TRUE) )
      {
      ICFG_LOG_FLOW1 ("INFO: RBE='N' but BufferTime: %d was entered.", clientDsTsList->bufferTime);
      }
     
    if ( (clientDsTsList->rbe == SD_TRUE) && (integrityCheckEntered == SD_FALSE) )
      {
      ICFG_LOG_FLOW1 ("INFO: RBE='Y' but Integrity was not entered.  Used default: %d", Default_IntegrityCheck);
      }

    if ( (clientDsTsList->rbe == SD_FALSE) && (integrityCheckEntered == SD_TRUE) )
      {
      ICFG_LOG_FLOW1 ("INFO: RBE='N' but Integrity: %d was entered.", clientDsTsList->integrity);
      }

  /* Help the user get the bits right */
    if (clientDsTsList->dsConditionsRequested == 0)
      {
      ICFG_LOG_NERR0 ("Warning: No DS Conditions Requested");
  
    /* We will make our best guess ... */
      if (clientDsTsList->integrity > 0)
        {
        ICFG_LOG_NERR0 ("  Turning on Integrity");
        clientDsTsList->dsConditionsRequested |= ICFG_DSC_INTEGRITY;
        }
      if (clientDsTsList->interval > 0)
        {
        ICFG_LOG_NERR0 ("  Turning on Interval");
        clientDsTsList->dsConditionsRequested |= ICFG_DSC_INTERVAL;
        }
      if (clientDsTsList->rbe)
        {
        ICFG_LOG_NERR0 ("  Turning on Change");
        clientDsTsList->dsConditionsRequested |= ICFG_DSC_CHANGE;
        }
      }
     
    ret = u_icfg_ClientDSTS (userInfo, clientDsTsList);
    if (ret != SD_SUCCESS)
      {
      ICFG_LOG_ERR1 ("ERROR: u_icfg_ClientDSTS returns: %d", ret);
      sxDecCtrl->errCode = SX_USER_ERROR;
      }


    }

  }

  
/************************************************************************/
/*                           _cdstsDsName_EFun                          */
/************************************************************************/

static ST_VOID _cdstsDsName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdstsDsName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: CdstsDsName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: cdstsDsName too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (clientDsTsList->dsName,str);
    }

  }


/************************************************************************/
/*                        _cdstsAssociationName_EFun                    */
/************************************************************************/

static ST_VOID _cdstsAssociationName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdstsAssociationName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: CdstaAssociationName required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (strlen(str) > MAX_IDENT_LEN + 1)
    {
    ICFG_LOG_ERR1 ("ERROR: CdstsAssociationName too long : %s", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    {
    strcpy (clientDsTsList->assocName,str);
    }

  }


/************************************************************************/
/*                          _cdstsInterval_EFun                         */
/************************************************************************/

static ST_VOID _cdstsInterval_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT16     sInt16;

  ICFG_LOG_FLOW0 ("SX DEC: _cdstsInterval_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             

  sInt16 = 0;
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: CdstsInterval required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if ( asciiToSint16(str, &sInt16) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: CdstsInterval Data Conversion Error on value: %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    { 
    clientDsTsList->interval = sInt16;
    }
             
  }


/************************************************************************/
/*                              _cdstsRbe_EFun                          */
/************************************************************************/

static ST_VOID _cdstsRbe_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdstsInterval_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: CdstsInterval required but not present ");
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    _icfgGetYesNo (sxDecCtrl, &clientDsTsList->rbe, "CdstsRbe");

  }


/************************************************************************/
/*                          _cdstsBufferTime_EFun                       */
/************************************************************************/

static ST_VOID _cdstsBufferTime_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT16    sInt16;

  ICFG_LOG_FLOW0 ("SX DEC: _cdstsBufferTime_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             

  sInt16 = 0;
  if ( asciiToSint16(str, &sInt16) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: CdstsBufferTime Data Conversion Error on value: %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    { /* valid value */
    if (clientDsTsList != NULL)
      clientDsTsList->bufferTime = sInt16;
	bufferTimeEntered = SD_TRUE;
    }
             
  }


/************************************************************************/
/*                           _cdstsIntegrity_EFun                           */
/************************************************************************/

static ST_VOID _cdstsIntegrity_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT16    sInt16;

  ICFG_LOG_FLOW0 ("SX DEC: _cdstsIntegrity_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             

  sInt16 = 0;
  if ( asciiToSint16(str, &sInt16) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: CdstsIntegrity Data Conversion Error on value: %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    { 
    if (clientDsTsList != NULL)
      clientDsTsList->integrity = sInt16;
      integrityCheckEntered = SD_TRUE;
    }
             
  }


/************************************************************************/
/*                            _cdstsStartTime_EFun                      */
/************************************************************************/

static ST_VOID _cdstsStartTime_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_LONG     sLong;

  ICFG_LOG_FLOW0 ("SX DEC: _cdstsStartTime_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  sLong = 0;
  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR0 ("ERROR: CdstaStartTime required but not present ");
    sxDecCtrl->errCode =
		
		SX_USER_ERROR;
    }
  else if ( asciiToSlong(str, &sLong) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: CdstsStartTime Data Conversion Error on value: %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    { 
    if (clientDsTsList != NULL)
      clientDsTsList->startTime = sLong;
    }

  }


/************************************************************************/
/*                            _cdstsCritical_EFun                       */
/************************************************************************/

static ST_VOID _cdstsCritical_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
  ICFG_LOG_FLOW0 ("SX DEC: _cdstsCritical_EFun");

  _icfgGetYesNo (sxDecCtrl, &clientDsTsList->critical, "CdstsCritical");

  }


/************************************************************************/
/*			_cdstsTle_EFun					*/
/************************************************************************/

static ST_VOID _cdstsTle_EFun		   (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
ST_INT16    sInt16;

  ICFG_LOG_FLOW0 ("SX DEC: _cdstsTle_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;             

  sInt16 = 0;
  if ( asciiToSint16(str, &sInt16) || strchr(str,'-'))
    {
    ICFG_LOG_ERR1 ("ERROR: TLE Data Conversion Error on value: %s ", str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else
    clientDsTsList->tle = sInt16;

  }

/************************************************************************/
/*			_cdstsDsCondInterval_EFun			*/
/************************************************************************/

static ST_VOID _cdstsDsCondInterval_EFun   (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN b;

  ICFG_LOG_FLOW0 ("SX DEC: _cdstsDsCondInterval_EFun");

  _icfgGetYesNo (sxDecCtrl, &b, "DsCondInterval"); 
  if (b == SD_TRUE)
    clientDsTsList->dsConditionsRequested |= ICFG_DSC_INTERVAL;

  }

/************************************************************************/
/*			_cdstsDsCondIntegrity_EFun			*/
/************************************************************************/

static ST_VOID _cdstsDsCondIntegrity_EFun  (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN b;

  ICFG_LOG_FLOW0 ("SX DEC: _cdstsDsCondIntegrity_EFun");

  _icfgGetYesNo (sxDecCtrl, &b, "DsCondIntegrity"); 
  if (b == SD_TRUE)
    clientDsTsList->dsConditionsRequested |= ICFG_DSC_INTEGRITY;

  }

/************************************************************************/
/*			_cdstsDsCondChange_EFun				*/
/************************************************************************/

static ST_VOID _cdstsDsCondChange_EFun	   (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN b;

  ICFG_LOG_FLOW0 ("SX DEC: _cdstsDsCondChange_EFun");

  _icfgGetYesNo (sxDecCtrl, &b, "DsCondChange"); 
  if (b == SD_TRUE)
    clientDsTsList->dsConditionsRequested |= ICFG_DSC_CHANGE;

  }

/************************************************************************/
/*			_cdstsDsCondOperator_EFun			*/
/************************************************************************/

static ST_VOID _cdstsDsCondOperator_EFun   (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN b;

  ICFG_LOG_FLOW0 ("SX DEC: _cdstsDsCondOperator_EFun");

  _icfgGetYesNo (sxDecCtrl, &b, "DsCondOperator"); 
  if (b == SD_TRUE)
    clientDsTsList->dsConditionsRequested |= ICFG_DSC_OPERATOR;

  }

/************************************************************************/
/*			_cdstsDsCondExternal_EFun			*/
/************************************************************************/

static ST_VOID _cdstsDsCondExternal_EFun   (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN b;

  ICFG_LOG_FLOW0 ("SX DEC: _cdstsDsCondExternal_EFun");

  _icfgGetYesNo (sxDecCtrl, &b, "DsCondExternal"); 
  if (b == SD_TRUE)
    clientDsTsList->dsConditionsRequested |= ICFG_DSC_EXTERNAL;

  }

/************************************************************************/
/*                           _cdstsDescription_EFun                     */
/************************************************************************/

static ST_VOID _cdstsDescription_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;

  ICFG_LOG_FLOW0 ("SX DEC: _cdstsDescription_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) > MAX_DESCRIPTION_LEN + 1)
    {
    strncpy (clientDsTsList->description, str, MAX_DESCRIPTION_LEN + 1);
    }
  else
    {
    strcpy (clientDsTsList->description,str);
    }

  }


/************************************************************************/
/************************************************************************/
/************************************************************************/
/*			icfgGetYesNo					*/
/************************************************************************/

static ST_VOID _icfgGetYesNo (SX_DEC_CTRL *sxDecCtrl, ST_BOOLEAN *dest, 
			      ST_CHAR *text)
  {
ST_BOOLEAN  rc;
ST_CHAR    *str;
ST_INT      strLen;
                
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;                          

  if (strlen(str) == 0)
    {
    ICFG_LOG_ERR1 ("ERROR: %s required but not present", text);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }
  else if (!strcmpi (str, "Y"))
    *dest = SD_TRUE;
  else if (!strcmpi (str, "N"))
    *dest = SD_FALSE;
  else
    {
    ICFG_LOG_ERR2 ("ERROR: %s Data Conversion Error on value: %s ", text, str);
    sxDecCtrl->errCode = SX_USER_ERROR;
    }

  }

/************************************************************************/
/*                            getListCount                              */
/************************************************************************/

ST_RET _getListCount (SX_DEC_CTRL *sxDecCtrl, ST_INT *countOut)
  {
ST_INT               count;
SX_DEC_ELEMENT_INFO *sxDecElInfo;

  sxDecElInfo = &sxDecCtrl->sxDecElInfo;
  if (sxDecElInfo->attrCount == 0 ||
      strcmpi (sxDecElInfo->attr[0].name, "Count") != 0)
    {
    ICFG_LOG_ERR0 ("SX_DEC: No count attribute present");
    sxDecCtrl->errCode = SD_FAILURE;
    return (SD_FAILURE);
    }
  count = atoi (sxDecElInfo->attr[0].value);
  *countOut = count;
  return (SD_SUCCESS);

  }


/************************************************************************/
