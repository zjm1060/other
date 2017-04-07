/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*          2000 - 2001, All Rights Reserved                            */
/*                                                                      */
/* MODULE NAME : logcfgx.c                                              */
/* PRODUCT(S)  :                                                        */
/*                                                                      */
/* MODULE DESCRIPTION :    This module processes each value parsed in   */
/*                         the logcfg.xml file .  The logcfg.xml file   */
/*                         assigns Log File Attributes and              */
/*                         Miscellaneous Control Flags as well as       */
/*                         Memory Use Logging and various Log Masks     */
/*                         to enable specific logging required.         */
/*                                                                      */
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :                            */
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev          Comments                                */
/* --------  ---  ------  --------------------------------------------  */
/* 04/25/02  MDE    19    Added MMS REQ, RESP, IND, CONF		*/
/* 03/21/02  MDE    18    Added MILOG_ADDR, MILOG_CFG  			*/
/* 02/27/02  GLB    17    Changed ascii_t_.. to asciiTo..               */
/* 01/10/02  GLB    16    Added sx_pop to implement optional/mandatory  */
/* 11/05/01  GLB    15    Added ICFG_.. logging                         */
/* 08/01/01  RKR    14    Put ifdefs around MI and ADLC header files    */
/* 06/21/01  ASK    13    Added MI Logging functions                    */
/* 05/25/01  GLB    12    Removed getFileSize                           */
/* 05/11/01  JRB    11    Use new sx_parseEx. Del logCfgParse &         */
/*                        LOG_DEC_CTRL (not used for anything useful).  */
/*                        Del LogSystem, _lpIncNextBuf: never used.     */
/*                        Configured "LogFileName" value was NOT saved. */
/*                        Del // comments.                              */
/*                        Only copy configured params from tmp_sLogCtrl */
/*                        (other params may no longer be valid).        */
/* 03/15/01  JRB    10    Added USER_LOG_... masks                      */
/* 03/10/01  GLB    09    Use only system funtions: malloc, calloc, free*/
/* 03/10/01  GLB    08    Added SX_LOG_... masks                        */
/* 01/24/01  JRB    07    Was clearing CLNP_LOG_IND(REQ)                */
/*                        when should have cleared CLSNS_LOG_IND(REQ).  */
/*                        Do NOT set SX_LOG_DEBUG. Only user should.    */
/*                        Use stricmp.                                  */
/* 01/09/01  GLB    06    Change logBufParse to logCfgParse             */
/* 01/04/01  EJV    05    Don't use O_TEXT in open() on non _WIN32 sys. */
/*                        Changed _getFileSize to use stat().           */
/* 02/01/00  GLB    04    Added check for SUIC masks, ACSE_ ...         */
/* 02/01/00  GLB    03    Added log messages for masks not checked      */                         
/* 11/30/00  RKR    02    Commented out adlc_usr.h                      */
/* 09/00/00  GLB    01    Module created from existing log_cfg.c        */
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "mem_chk.h"
#include "smem.h"
#include "mvl_acse.h"
#include "mvl_log.h"

#if defined (ICCP_LITE)
#include "mi_log.h"
#endif

#if defined (ICFG)
#include "icfg.h"
#endif

#include "suicacse.h"

#if (defined (ADLC_SLAVE) || defined (ADLC_MASTER))
#include "adlc_usr.h"
#endif

#include "smp_usr.h"

#include "str_util.h"
#include "slog.h"

#include "sx_defs.h"
#include "sx_log.h"
#include <string.h> 

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <io.h>
#endif

/******** Control bit set/clear macros *********/

#define M_SET_CTRL(a)      {tmp_sLogCtrl.logCtrl |=  (a);}
#define M_CLR_CTRL(a)      {tmp_sLogCtrl.logCtrl &= ~(a);}

#define M_SET_FILE_CTRL(a) {tmp_sLogCtrl.fc.ctrl |=  (a);}
#define M_CLR_FILE_CTRL(a) {tmp_sLogCtrl.fc.ctrl &= ~(a);}

#define M_SET_MEM_CTRL(a)  {tmp_sLogCtrl.mc.ctrl |=  (a);}
#define M_CLR_MEM_CTRL(a)  {tmp_sLogCtrl.mc.ctrl &= ~(a);}

/************************************************************************/
/* Buffers to save file names.                                          */
/************************************************************************/
static ST_CHAR  log_file_name [256];
static ST_CHAR  memlog_file_name [256];
static LOG_CTRL tmp_sLogCtrl;

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of      */
/* __FILE__ strings.                                                    */

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

/************************************************************************/

static ST_VOID _logStart_SFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _sxLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sxLogNerr_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sxLogDec_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sxLogEnc_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sxLogFlow_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _sxLogDebug_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _logControl_SFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _logFileAttributes_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _logFileEnable_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _logFileSize_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _logFileName_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _logMemoryAttributes_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _logMemoryEnable_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _logMemoryItems_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _memoryFileName_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _memoryFileDump_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _miscControlFlags_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _timeEnable_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _destroyOldFile_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _setbuf_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _msgHeader_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _wipe_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _wrap_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _hardFlush_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _headerCr_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _headerFilename_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _headerLogType_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _userLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _userLogClient_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _userLogServer_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _asn1LogMasks_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _asn1LogNerr_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _asn1LogDec_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _asn1LogEnc_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _mmsLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mmsLogReq_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mmsLogResp_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mmsLogInd_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mmsLogConf_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mmsLogNerr_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mmsLogDec_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mmsLogEnc_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mmsLogRt_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mmsLogRtaa_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mmsLogAa_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _mvlLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mvlLogNerr_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mvlLogAcse_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mvlLogAcsedata_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mvluLogFlow_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mvluLogDebug_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _mvluLogTiming_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _acseLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _acseLogEnc_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _acseLogDec_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _acseNerrPrint_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _acseIndPrint_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _acseCnfPrint_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _acseDecPrint_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _acseAbortPrint_EFun (SX_DEC_CTRL *sxDecCtrl);


static ST_VOID _tp4LogMasks_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _tp4LogFlowup_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _tp4LogFlowdown_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _clnpLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _clnpLogNerr_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _clnpLogReq_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _clnpLogInd_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _clnpLogEncDec_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _clnpLogLlcEncDec_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _clsnsLogReq_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _clsnsLogInd_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _adlcLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _adlcLogNerr_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _adlcLogFlow_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _adlcLogIo_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _adlcLogHexio_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _adlcLogSm_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _adlcLogHexsm_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _adlcLogUser_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _adlcLogHexuser_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _miLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _miLogNerr_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _miLogFlow_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _miLogDebug_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _miLogAddr_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _miLogCfg_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _smpLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _smpLogReq_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _smpLogInd_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _icfgLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _icfgLogFlow_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _icfgLogNerr_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _memLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _memLogCalloc_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _memLogMalloc_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _memLogRealloc_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _memLogFree_EFun (SX_DEC_CTRL *sxDecCtrl);

static ST_VOID _memAllocDbgCtrl_SFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _memFillEnable_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _memHeapCheck_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _checkListEnable_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _findNodeEnable_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _trackPrevFree_EFun (SX_DEC_CTRL *sxDecCtrl);
extern ST_BOOLEAN m_track_prev_free;    /* so secret it's not in any header file*/
static ST_VOID _listDebugSel_EFun (SX_DEC_CTRL *sxDecCtrl);
static ST_VOID _noReallocSmaller_EFun (SX_DEC_CTRL *sxDecCtrl);

/************************************************************************/
/************************************************************************/
/************************************************************************/

SX_ELEMENT logStartElements[] = 
{
  {"LOG_CFG",               SX_ELF_CSTART|SX_ELF_CEND, _logStart_SFun}
};

SX_ELEMENT sxLogMaskElements[] = 
{
  {"SX_LOG_NERR",           SX_ELF_CEND|SX_ELF_OPT,    _sxLogNerr_EFun},
  {"SX_LOG_DEC",            SX_ELF_CEND|SX_ELF_OPT,    _sxLogDec_EFun},
  {"SX_LOG_ENC",            SX_ELF_CEND|SX_ELF_OPT,    _sxLogEnc_EFun},
  {"SX_LOG_FLOW",           SX_ELF_CEND|SX_ELF_OPT,    _sxLogFlow_EFun},
  {"SX_LOG_DEBUG",          SX_ELF_CEND|SX_ELF_OPT,    _sxLogDebug_EFun}
};

SX_ELEMENT logElements[] = 
{
  {"SxLogMasks",            SX_ELF_CSTART|SX_ELF_OPT,  _sxLogMasks_SFun},
  {"LogControl",            SX_ELF_CSTART,             _logControl_SFun},
  {"UserLogMasks",          SX_ELF_CSTART|SX_ELF_OPT,  _userLogMasks_SFun},
  {"Asn1LogMasks",          SX_ELF_CSTART|SX_ELF_OPT,  _asn1LogMasks_SFun},
  {"MmsLogMasks",           SX_ELF_CSTART|SX_ELF_OPT,  _mmsLogMasks_SFun},
  {"MvlLogMasks",           SX_ELF_CSTART|SX_ELF_OPT,  _mvlLogMasks_SFun},
  {"AcseLogMasks",          SX_ELF_CSTART|SX_ELF_OPT,  _acseLogMasks_SFun},
  {"Tp4LogMasks",           SX_ELF_CSTART|SX_ELF_OPT,  _tp4LogMasks_SFun},
  {"ClnpLogMasks",          SX_ELF_CSTART|SX_ELF_OPT,  _clnpLogMasks_SFun},
  {"AdlcLogMasks",          SX_ELF_CSTART|SX_ELF_OPT,  _adlcLogMasks_SFun},
  {"SmpLogMasks",           SX_ELF_CSTART|SX_ELF_OPT,  _smpLogMasks_SFun},
  {"IcfgLogMasks",          SX_ELF_CSTART|SX_ELF_OPT,  _icfgLogMasks_SFun},
  {"MiLogMasks",            SX_ELF_CSTART|SX_ELF_OPT,  _miLogMasks_SFun},
  {"MemLogMasks",           SX_ELF_CSTART|SX_ELF_OPT,  _memLogMasks_SFun},
  {"MemAllocDbgCtrl",       SX_ELF_CSTART|SX_ELF_OPT,  _memAllocDbgCtrl_SFun}
};

SX_ELEMENT logControlElements[] = 
{
  {"LogFileAttributes",     SX_ELF_CSTART,  _logFileAttributes_SFun},
  {"LogMemoryAttributes",   SX_ELF_CSTART,  _logMemoryAttributes_SFun},
  {"MiscControlFlags",      SX_ELF_CSTART,  _miscControlFlags_SFun}
};

SX_ELEMENT logFileAttributesElements[] = 
{
  {"LogFileEnable",         SX_ELF_CEND,    _logFileEnable_EFun},
  {"LogFileSize",           SX_ELF_CEND,    _logFileSize_EFun},
  {"LogFileName",           SX_ELF_CEND,    _logFileName_EFun}
};

SX_ELEMENT logMemoryAttribElements[] = 
{
  {"LogMemoryEnable",       SX_ELF_CEND,    _logMemoryEnable_EFun},
  {"LogMemoryItems",        SX_ELF_CEND,    _logMemoryItems_EFun},
  {"MemoryFileName",        SX_ELF_CEND,    _memoryFileName_EFun},
  {"MemoryFileDump",        SX_ELF_CEND,    _memoryFileDump_EFun}
};

SX_ELEMENT miscControlFlagsElements[] = 
{
  {"TimeEnable",            SX_ELF_CEND,    _timeEnable_EFun},
  {"DestroyOldFile",        SX_ELF_CEND,    _destroyOldFile_EFun},
  {"Setbuf",                SX_ELF_CEND,    _setbuf_EFun},
  {"MsgHeader",             SX_ELF_CEND,    _msgHeader_EFun},
  {"Wipe",                  SX_ELF_CEND,    _wipe_EFun},
  {"Wrap",                  SX_ELF_CEND,    _wrap_EFun},
  {"HardFlush",             SX_ELF_CEND,    _hardFlush_EFun},
  {"HeaderCr",              SX_ELF_CEND,    _headerCr_EFun},
  {"HeaderFilename",        SX_ELF_CEND,    _headerFilename_EFun},
  {"HeaderLogType",         SX_ELF_CEND,    _headerLogType_EFun}
};

SX_ELEMENT userLogMaskElements[] = 
{
  {"USER_LOG_CLIENT",       SX_ELF_CEND|SX_ELF_OPT,   _userLogClient_EFun},
  {"USER_LOG_SERVER",       SX_ELF_CEND|SX_ELF_OPT,   _userLogServer_EFun}
};

SX_ELEMENT asn1LogMaskElements[] = 
{
  {"ASN1_LOG_NERR",         SX_ELF_CEND|SX_ELF_OPT,    _asn1LogNerr_EFun},
  {"ASN1_LOG_DEC",          SX_ELF_CEND|SX_ELF_OPT,    _asn1LogDec_EFun},
  {"ASN1_LOG_ENC",          SX_ELF_CEND|SX_ELF_OPT,    _asn1LogEnc_EFun}
};

SX_ELEMENT miLogMaskElements[] = 
{
  {"MI_LOG_NERR",           SX_ELF_CEND|SX_ELF_OPT,    _miLogNerr_EFun},
  {"MI_LOG_FLOW",           SX_ELF_CEND|SX_ELF_OPT,    _miLogFlow_EFun},
  {"MI_LOG_DEBUG",          SX_ELF_CEND|SX_ELF_OPT,    _miLogDebug_EFun},
  {"MI_LOG_CFG",            SX_ELF_CEND|SX_ELF_OPT,    _miLogCfg_EFun},
  {"MI_LOG_ADDR",           SX_ELF_CEND|SX_ELF_OPT,    _miLogAddr_EFun}
};

SX_ELEMENT mmsLogMaskElements[] = 
{
  {"MMS_LOG_REQ",           SX_ELF_CEND|SX_ELF_OPT,    _mmsLogReq_EFun},
  {"MMS_LOG_RESP",          SX_ELF_CEND|SX_ELF_OPT,    _mmsLogResp_EFun},
  {"MMS_LOG_IND",           SX_ELF_CEND|SX_ELF_OPT,    _mmsLogInd_EFun},
  {"MMS_LOG_CONF",          SX_ELF_CEND|SX_ELF_OPT,    _mmsLogConf_EFun},
  {"MMS_LOG_NERR",          SX_ELF_CEND|SX_ELF_OPT,    _mmsLogNerr_EFun},
  {"MMS_LOG_DEC",           SX_ELF_CEND|SX_ELF_OPT,    _mmsLogDec_EFun},
  {"MMS_LOG_ENC",           SX_ELF_CEND|SX_ELF_OPT,    _mmsLogEnc_EFun},
  {"MMS_LOG_RT",            SX_ELF_CEND|SX_ELF_OPT,    _mmsLogRt_EFun},
  {"MMS_LOG_RTAA",          SX_ELF_CEND|SX_ELF_OPT,    _mmsLogRtaa_EFun},
  {"MMS_LOG_AA",            SX_ELF_CEND|SX_ELF_OPT,    _mmsLogAa_EFun}
};

SX_ELEMENT mvlLogMaskElements[] = 
{
  {"MVLLOG_NERR",           SX_ELF_CEND|SX_ELF_OPT,    _mvlLogNerr_EFun},
  {"MVLLOG_ACSE",           SX_ELF_CEND|SX_ELF_OPT,    _mvlLogAcse_EFun},
  {"MVLLOG_ACSEDATA",       SX_ELF_CEND|SX_ELF_OPT,    _mvlLogAcsedata_EFun},
  {"MVLULOG_FLOW",          SX_ELF_CEND|SX_ELF_OPT,    _mvluLogFlow_EFun},
  {"MVLULOG_DEBUG",         SX_ELF_CEND|SX_ELF_OPT,    _mvluLogDebug_EFun},
  {"MVLULOG_TIMING",        SX_ELF_CEND|SX_ELF_OPT,    _mvluLogTiming_EFun}
};

SX_ELEMENT acseLogMaskElements[] = 
{
  {"ACSE_LOG_ENC",          SX_ELF_CEND|SX_ELF_OPT,    _acseLogEnc_EFun},
  {"ACSE_LOG_DEC",          SX_ELF_CEND|SX_ELF_OPT,    _acseLogDec_EFun},
  {"ACSE_NERR_PRINT",       SX_ELF_CEND|SX_ELF_OPT,    _acseNerrPrint_EFun},
  {"ACSE_IND_PRINT",        SX_ELF_CEND|SX_ELF_OPT,    _acseIndPrint_EFun},
  {"ACSE_CNF_PRINT",        SX_ELF_CEND|SX_ELF_OPT,    _acseCnfPrint_EFun},
  {"ACSE_DEC_PRINT",        SX_ELF_CEND|SX_ELF_OPT,    _acseDecPrint_EFun},
  {"ACSE_ABORT_PRINT",      SX_ELF_CEND|SX_ELF_OPT,    _acseAbortPrint_EFun}
};

SX_ELEMENT tp4LogMaskElements[] = 
{
  {"TP4_LOG_FLOWUP",        SX_ELF_CEND|SX_ELF_OPT,    _tp4LogFlowup_EFun},
  {"TP4_LOG_FLOWDOWN",      SX_ELF_CEND|SX_ELF_OPT,    _tp4LogFlowdown_EFun}
};

SX_ELEMENT clnpLogMaskElements[] = 
{
  {"CLNP_LOG_NERR",         SX_ELF_CEND|SX_ELF_OPT,    _clnpLogNerr_EFun},
  {"CLNP_LOG_REQ",          SX_ELF_CEND|SX_ELF_OPT,    _clnpLogReq_EFun},
  {"CLNP_LOG_IND",          SX_ELF_CEND|SX_ELF_OPT,    _clnpLogInd_EFun},
  {"CLNP_LOG_ENC_DEC",      SX_ELF_CEND|SX_ELF_OPT,    _clnpLogEncDec_EFun},
  {"CLNP_LOG_LLC_ENC_DEC",  SX_ELF_CEND|SX_ELF_OPT,    _clnpLogLlcEncDec_EFun},
  {"CLSNS_LOG_REQ",         SX_ELF_CEND|SX_ELF_OPT,    _clsnsLogReq_EFun},
  {"CLSNS_LOG_IND",         SX_ELF_CEND|SX_ELF_OPT,    _clsnsLogInd_EFun}
};

SX_ELEMENT adlcLogMaskElements[] = 
{
  {"ADLC_LOG_NERR",         SX_ELF_CEND|SX_ELF_OPT,    _adlcLogNerr_EFun},
  {"ADLC_LOG_FLOW",         SX_ELF_CEND|SX_ELF_OPT,    _adlcLogFlow_EFun},
  {"ADLC_LOG_IO",           SX_ELF_CEND|SX_ELF_OPT,    _adlcLogIo_EFun},
  {"ADLC_LOG_HEXIO",        SX_ELF_CEND|SX_ELF_OPT,    _adlcLogHexio_EFun},
  {"ADLC_LOG_SM",           SX_ELF_CEND|SX_ELF_OPT,    _adlcLogSm_EFun},
  {"ADLC_LOG_HEXSM",        SX_ELF_CEND|SX_ELF_OPT,    _adlcLogHexsm_EFun},
  {"ADLC_LOG_USER",         SX_ELF_CEND|SX_ELF_OPT,    _adlcLogUser_EFun},
  {"ADLC_LOG_HEXUSER",      SX_ELF_CEND|SX_ELF_OPT,    _adlcLogHexuser_EFun}
};


SX_ELEMENT smpLogMaskElements[] = 
{
  {"SMP_LOG_REQ",           SX_ELF_CEND|SX_ELF_OPT,    _smpLogReq_EFun},
  {"SMP_LOG_IND",           SX_ELF_CEND|SX_ELF_OPT,    _smpLogInd_EFun}
};

SX_ELEMENT icfgLogMaskElements[] = 
{
  {"ICFG_LOG_FLOW",         SX_ELF_CEND|SX_ELF_OPT,    _icfgLogFlow_EFun},
  {"ICFG_LOG_NERR",         SX_ELF_CEND|SX_ELF_OPT,    _icfgLogNerr_EFun}
};

SX_ELEMENT memLogMaskElements[] = 
{
  {"MEM_LOG_CALLOC",        SX_ELF_CEND|SX_ELF_OPT,    _memLogCalloc_EFun},
  {"MEM_LOG_MALLOC",        SX_ELF_CEND|SX_ELF_OPT,    _memLogMalloc_EFun},
  {"MEM_LOG_REALLOC",       SX_ELF_CEND|SX_ELF_OPT,    _memLogRealloc_EFun},
  {"MEM_LOG_FREE",          SX_ELF_CEND|SX_ELF_OPT,    _memLogFree_EFun}
};

SX_ELEMENT memAllocElements[] = 
{
  {"MemFillEnable",         SX_ELF_CEND|SX_ELF_OPT,    _memFillEnable_EFun},
  {"MemHeapCheck",          SX_ELF_CEND|SX_ELF_OPT,    _memHeapCheck_EFun},
  {"CheckListEnable",       SX_ELF_CEND|SX_ELF_OPT,    _checkListEnable_EFun},
  {"FindNodeEnable",        SX_ELF_CEND|SX_ELF_OPT,    _findNodeEnable_EFun},
  {"TrackPrevFree",         SX_ELF_CEND|SX_ELF_OPT,    _trackPrevFree_EFun},
  {"ListDebugSel",          SX_ELF_CEND|SX_ELF_OPT,    _listDebugSel_EFun},
  {"NoReallocSmaller",      SX_ELF_CEND|SX_ELF_OPT,    _noReallocSmaller_EFun}
};

/************************************************************************/
/*                                logcfgx                               */
/************************************************************************/

ST_RET logcfgx (ST_CHAR *logFileName)
  {
ST_RET rc;
ST_UINT sx_debug_sel_save;
ST_CHAR *savedFileName;

  savedFileName = sLogCtrl->fc.fileName;

  /* Copy original "sLogCtrl" to temporary struct which may be modified */
  /* by the configuration.                                              */
  /* This prevents logging control changes while there may be logging.  */
  memcpy (&tmp_sLogCtrl, sLogCtrl, sizeof (LOG_CTRL));

  rc = sx_parseEx (logFileName, 
          sizeof (logStartElements)/sizeof(SX_ELEMENT), 
                  logStartElements, NULL);
  if (rc != SD_SUCCESS)
    {
    /* Config failed: Do not modify "sLogCtrl".                         */
    /* Turn on cfg logging and parse file again.                        */
    sx_debug_sel_save = sx_debug_sel;

    sx_debug_sel |= SX_LOG_DEC | SX_LOG_ENC | SX_LOG_FLOW | SX_LOG_DEBUG;
    SXLOG_ERR0 ("Error: Trying again w/debug on ... ");
    rc = sx_parseEx (logFileName, 
          sizeof (logStartElements)/sizeof(SX_ELEMENT), 
                  logStartElements, NULL);

    sx_debug_sel = sx_debug_sel_save;
    }
  else    /* rc == SD_SUCCESS */
    {
    /* Config successful: Save new settings in "sLogCtrl".              */
    /* CRITICAL: only copy elements in tmp_sLogCtrl that may have been  */
    /*           modified by configuration (DON'T copy state, fp, etc.) */
    sLogCtrl->logCtrl         = tmp_sLogCtrl.logCtrl;
    sLogCtrl->fc.ctrl         = tmp_sLogCtrl.fc.ctrl;
    sLogCtrl->fc.fileName     = tmp_sLogCtrl.fc.fileName;
    sLogCtrl->fc.maxSize      = tmp_sLogCtrl.fc.maxSize;
    sLogCtrl->mc.ctrl         = tmp_sLogCtrl.mc.ctrl;
    sLogCtrl->mc.dumpFileName = tmp_sLogCtrl.mc.dumpFileName;
    sLogCtrl->mc.maxItems     = tmp_sLogCtrl.mc.maxItems;

    /* Config can't change "state" or "fp", so if log file was open,    */
    /* and filename changed, close the log file and clear state.        */
    if (sLogCtrl->fc.state & FIL_STATE_OPEN)
      {                 /* log file already opened  */
#ifdef _WIN32   /* Filenames NOT case sensitive on Windows  */
      if (stricmp (savedFileName, sLogCtrl->fc.fileName) != 0)
#else
      if (strcmp (savedFileName, sLogCtrl->fc.fileName) != 0)
#endif
        {   /* NEW log file name different from OLD.    */
        fclose (sLogCtrl->fc.fp);           /* close it */
        sLogCtrl->fc.state &= ~(FIL_STATE_OPEN);    /* clear state  */
        /* NOTE: this msg will log to the new file.     */
        SLOGALWAYS1 ("See the file '%s' for messages logged before or during Logging config",
              savedFileName);
        }
      }

    /* NOTE: this msg will log to the new file.     */
    SLOGALWAYS0 ("Logging config complete");
    }

  return (rc);
  }

/************************************************************************/
/************************************************************************/
/*                           _logStart_SFun                             */
/************************************************************************/

static ST_VOID _logStart_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _logStart_SFun");

  if (sxDecCtrl->reason == SX_ELEMENT_START)
    {
    SX_PUSH (logElements);
    }

  if (sxDecCtrl->reason == SX_ELEMENT_END)
    {
    while (sxDecCtrl->itemStackLevel > 0)
      sx_pop (sxDecCtrl);
    }
  }

/************************************************************************/
/************************************************************************/
/*                           _sxLogMasks_SFun                           */
/************************************************************************/

static ST_VOID _sxLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _sxLogMasks_SFun");
  SX_PUSH (sxLogMaskElements);
  }

/************************************************************************/
/*                           _sxLogNerr_EFun                            */
/************************************************************************/

static ST_VOID _sxLogNerr_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _sxLogNerr_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    sx_debug_sel |= SX_LOG_NERR;
    }
  else if (!stricmp (str, "OFF"))
    {
    sx_debug_sel &= ~SX_LOG_NERR;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: SxLogNerr Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                            _sxLogDec_EFun                            */
/************************************************************************/

static ST_VOID _sxLogDec_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _sxLogDec_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    sx_debug_sel |= SX_LOG_DEC;
    }
  else if (!stricmp (str, "OFF"))
    {
    sx_debug_sel &= ~SX_LOG_DEC;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: SxLogDec Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

 }

/************************************************************************/
/*                            _sxLogEnc_EFun                            */
/************************************************************************/

static ST_VOID _sxLogEnc_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _sxLogEnc_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    sx_debug_sel |= SX_LOG_ENC;
    }
  else if (!stricmp (str, "OFF"))
    {
    sx_debug_sel &= ~SX_LOG_ENC;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: SxLogEnc Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                            _sxLogFlow_EFun                           */
/************************************************************************/

static ST_VOID _sxLogFlow_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _sxLogFlow_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    sx_debug_sel |= SX_LOG_FLOW;
    }
  else if (!stricmp (str, "OFF"))
    {
    sx_debug_sel &= ~SX_LOG_FLOW;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: SxLogFlow Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

 }

/************************************************************************/
/*                           _sxLogDebug_EFun                           */
/************************************************************************/

static ST_VOID _sxLogDebug_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _sxLogDebuf_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    sx_debug_sel |= SX_LOG_DEBUG;
    }
  else if (!stricmp (str, "OFF"))
    {
    sx_debug_sel &= ~SX_LOG_DEBUG;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: SxLogDebug Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/************************************************************************/
/*                           _logControl_SFun                           */
/************************************************************************/

static ST_VOID _logControl_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _logControl_SFun");
  SX_PUSH (logControlElements);
  }

/************************************************************************/
/*                       _logFileAttributes_SFun                        */
/************************************************************************/

static ST_VOID _logFileAttributes_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _logFileAttributes_SFun");
  SX_PUSH (logFileAttributesElements);
  }

/************************************************************************/
/*                         _logFileEnable_EFun                          */
/************************************************************************/

static ST_VOID _logFileEnable_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _logFileEnable_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    M_SET_CTRL (LOG_FILE_EN);
    }
  else if (!stricmp (str, "OFF"))
    {
    M_CLR_CTRL (LOG_FILE_EN);
    }
  else
    {
    SXLOG_ERR0 ("ERROR: LogFileEnable Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                            _logFileSize_EFun                         */
/************************************************************************/

static ST_VOID _logFileSize_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

ST_ULONG uLongInt = 0;

  SXLOG_CFLOW0 ("SX DEC: _logFileSize_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (asciiToUlong(str,&uLongInt) != SD_SUCCESS)
    {
    printf ("\n LogFileSize Data Conversion Error");
    SXLOG_ERR0 ("ERROR: LogFileSize Data Conversion Error");
    sxDecCtrl->errCode = SD_FAILURE;
    }
  else
    {
    tmp_sLogCtrl.fc.maxSize = uLongInt;    /* Size of the circular log file */
    }

  }
 
/************************************************************************/
/*                            _logFileName_EFun                         */
/************************************************************************/

static ST_VOID _logFileName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _logFileName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;


  if (str != NULL)
    {                       
    if (strlen (str) < sizeof (log_file_name))
      {
      strcpy (log_file_name, str);
      tmp_sLogCtrl.fc.fileName = log_file_name;
      }
    else         
      {
      SXLOG_ERR0 ("ERROR: LogFileName Conversion Error");
      sxDecCtrl->errCode = SX_ERR_CONVERT;
      }
    }
  else
    {
    SXLOG_ERR0 ("ERROR: LogFileName Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
  }

/************************************************************************/
/************************************************************************/
/*                        _logMemoryAttributes_SFun                     */
/************************************************************************/

static ST_VOID _logMemoryAttributes_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _logMemoryAttributes_SFun");
  SX_PUSH (logMemoryAttribElements);
  }

/************************************************************************/
/*                      _logMemoryEnable_EFun                           */
/************************************************************************/

static ST_VOID _logMemoryEnable_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _logMemoryEnable_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    M_SET_CTRL (LOG_MEM_EN);
    }
  else if (!stricmp (str, "OFF"))
    {
    M_CLR_CTRL (LOG_MEM_EN);
    }
  else
    {
    SXLOG_ERR0 ("ERROR: LogMemoryEnable Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
  }

/************************************************************************/
/*                            _logMemoryItems_EFun                      */
/************************************************************************/

static ST_VOID _logMemoryItems_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

ST_ULONG uLongInt = 0;

  SXLOG_CFLOW0 ("SX DEC: _logMemoryItems_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (asciiToUlong(str,&uLongInt) != SD_SUCCESS)
    {
    SXLOG_ERR0 ("ERROR: LogMemoryItems Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
  else
    {
    tmp_sLogCtrl.mc.maxItems = uLongInt;    /* Size of the circular log file    */
    }

  }

/************************************************************************/
/*                         _memoryFileName_EFun                         */
/************************************************************************/

static ST_VOID _memoryFileName_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _memoryFileName_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (str != NULL)
    {                       
    if (strlen (str) < sizeof (memlog_file_name))
      {
      strcpy (memlog_file_name, str);
      tmp_sLogCtrl.mc.dumpFileName = memlog_file_name;
      }
    else         
      {
      SXLOG_ERR0 ("ERROR: MemoryFileName Conversion Error");
      sxDecCtrl->errCode = SX_ERR_CONVERT;
      }
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MemoryFileName Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                            _memoryFileDump_EFun                      */
/************************************************************************/

static ST_VOID _memoryFileDump_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _memoryFileDump_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    M_SET_MEM_CTRL (MEM_CTRL_AUTODUMP_EN);
    }
  else if (!stricmp (str, "OFF"))
    {
    M_CLR_MEM_CTRL (MEM_CTRL_AUTODUMP_EN);
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MemoryFileDump Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
  }

/************************************************************************/
/************************************************************************/
/*                         _miscControlFlags_SFun                       */
/************************************************************************/

static ST_VOID _miscControlFlags_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _miscControlFlags_SFun");
  SX_PUSH (miscControlFlagsElements);
  }

/************************************************************************/
/*                          _timeEnable_EFun                            */
/************************************************************************/

static ST_VOID _timeEnable_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

ST_UINT16 uShortInt = 0;

  SXLOG_CFLOW0 ("SX DEC: _timeEnable_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "TimeDate"))
    {
    M_CLR_CTRL (LOG_DIFFTIME_EN);   /* Clear Elapsed time logging   */
    M_SET_CTRL (LOG_TIMEDATE_EN |
                LOG_TIME_EN);       /* Set Time/Date logging    */
    }
  else if (!stricmp (str, "ElapsedTime"))
    {
    M_CLR_CTRL (LOG_TIMEDATE_EN);   /* Clear Time/Date logging  */
    M_SET_CTRL (LOG_DIFFTIME_EN |
                LOG_TIME_EN);       /* Set Elapsed time logging */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: TimeEnable/ElapsedTime Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

 }

/************************************************************************/
/*                           _destroyOldFile_EFun                       */
/************************************************************************/

static ST_VOID _destroyOldFile_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _destroyOldFile_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    M_SET_FILE_CTRL (FIL_CTRL_NO_APPEND);
    }
  else if (!stricmp (str, "OFF"))
    {
    M_CLR_FILE_CTRL (FIL_CTRL_NO_APPEND);
    }
  else
    {
    SXLOG_ERR0 ("ERROR: DestroyOldFile Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                           _setbuf_EFun                               */
/************************************************************************/

static ST_VOID _setbuf_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _setbuf_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    M_SET_FILE_CTRL (FIL_CTRL_SETBUF_EN);  /* Setbuf is requested       */
    }
  else if (!stricmp (str, "OFF"))
    {
    M_CLR_FILE_CTRL (FIL_CTRL_SETBUF_EN);  /* Setbuf is requested       */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: Setbuf Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                           _msgHeader_EFun                            */
/************************************************************************/

static ST_VOID _msgHeader_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _msgHeader_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    M_SET_FILE_CTRL (FIL_CTRL_MSG_HDR_EN);
    M_SET_MEM_CTRL (MEM_CTRL_MSG_HDR_EN);
    }
  else if (!stricmp (str, "OFF"))
    {
    M_CLR_FILE_CTRL (FIL_CTRL_MSG_HDR_EN);
    M_CLR_MEM_CTRL (MEM_CTRL_MSG_HDR_EN);
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MsgHeader Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
  }

/************************************************************************/
/*                             _wipe_EFun                               */
/************************************************************************/

static ST_VOID _wipe_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _wipe_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    M_SET_FILE_CTRL (FIL_CTRL_WIPE_EN);
    }
  else if (!stricmp (str, "OFF"))
    {
    M_CLR_FILE_CTRL (FIL_CTRL_WIPE_EN);
    }
  else
    {
    SXLOG_ERR0 ("ERROR: NoWipe Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                               _wrap_EFun                             */
/************************************************************************/

static ST_VOID _wrap_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _wrap_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    M_SET_FILE_CTRL (FIL_CTRL_WRAP_EN);
    }
  else if (!stricmp (str, "OFF"))
    {
    M_CLR_FILE_CTRL (FIL_CTRL_WRAP_EN);
    }
  else
    {
    SXLOG_ERR0 ("ERROR: wrap Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

}

/************************************************************************/
/*                            _hardFlush_EFun                           */
/************************************************************************/

static ST_VOID _hardFlush_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _hardFlush_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    M_SET_FILE_CTRL (FIL_CTRL_HARD_FLUSH);  /* Hard flush is requested  */
    }
  else if (!stricmp (str, "OFF"))
    {
    M_CLR_FILE_CTRL (FIL_CTRL_HARD_FLUSH);  /* Hard flush is requested  */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: HardFlush Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                           _headerCr_EFun                             */
/************************************************************************/

static ST_VOID _headerCr_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _headerCr_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    M_CLR_CTRL (LOG_NO_HEADER_CR);
    }
  else if (!stricmp (str, "OFF"))
    {
    M_SET_CTRL (LOG_NO_HEADER_CR);
    }
  else
    {
    SXLOG_ERR0 ("ERROR: HeaderCr Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                           _headerFilename_EFun                       */
/************************************************************************/

static ST_VOID _headerFilename_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _suppressFilename_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    M_CLR_CTRL (LOG_FILENAME_SUPPRESS);
    }
  else if (!stricmp (str, "OFF"))
    {
    M_SET_CTRL (LOG_FILENAME_SUPPRESS);
    }
  else
    {
    SXLOG_ERR0 ("ERROR: SuppressFileName Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
  }

/************************************************************************/
/*                            _headerLogType_EFun                       */
/************************************************************************/

static ST_VOID _headerLogType_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _suppressLogType_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    M_CLR_CTRL (LOG_LOGTYPE_SUPPRESS);
    }
  else if (!stricmp (str, "OFF"))
    {
    M_SET_CTRL (LOG_LOGTYPE_SUPPRESS);
    }
  else
    {
    SXLOG_ERR0 ("ERROR: HeaderLogType Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/************************************************************************/
/*                           _userLogMasks_SFun                         */
/************************************************************************/

static ST_VOID _userLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _userLogMasks_SFun");
  SX_PUSH (userLogMaskElements);
  }

/************************************************************************/
/*                         _userLogClient_EFun                          */
/************************************************************************/

static ST_VOID _userLogClient_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _userLogClient_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    user_debug_sel |= USER_LOG_CLIENT;
    }
  else if (!stricmp (str, "OFF"))
    {
    user_debug_sel &= ~USER_LOG_CLIENT;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: UserLogClient Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
  }

/************************************************************************/
/*                            _userLogServer_EFun                       */
/************************************************************************/

static ST_VOID _userLogServer_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _userLogServer_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    user_debug_sel |= USER_LOG_SERVER;
    }
  else if (!stricmp (str, "OFF"))
    {
    user_debug_sel &= ~USER_LOG_SERVER;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: UserLogServer Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
 }

/************************************************************************/
/************************************************************************/
/*                          _asn1LogMasks_SFun                          */
/************************************************************************/

static ST_VOID _asn1LogMasks_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _asn1LogMasks_SFun");
  SX_PUSH (asn1LogMaskElements);
  }

/************************************************************************/
/*                          _asn1LogNerr_EFun                           */
/************************************************************************/

static ST_VOID _asn1LogNerr_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _asn1LogNerr_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    asn1_debug_sel |= ASN1_LOG_NERR;
    }
  else if (!stricmp (str, "OFF"))
    {
    asn1_debug_sel &= ~ASN1_LOG_NERR;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: Asn1LogNerr Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                               _asn1LogDec_EFun                       */
/************************************************************************/

static ST_VOID _asn1LogDec_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _asn1LogDec_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    asn1_debug_sel |= ASN1_LOG_DEC;
    }
  else if (!stricmp (str, "OFF"))
    {
    asn1_debug_sel &= ~ASN1_LOG_DEC;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: Asn1LogDec Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

 }

/************************************************************************/
/*                             _asn1LogEnc_EFun                         */
/************************************************************************/

static ST_VOID _asn1LogEnc_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _asn1LogEnc_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    asn1_debug_sel |= ASN1_LOG_ENC;
    }
  else if (!stricmp (str, "OFF"))
    {
    asn1_debug_sel &= ~ASN1_LOG_ENC;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: Asn1LogEnc Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                          _mmsLogMasks_SFun                           */
/************************************************************************/

static ST_VOID _mmsLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _mmsLogMasks_SFun");
  SX_PUSH (mmsLogMaskElements);
  }

/************************************************************************/
/*                            _mmsLogReq_EFun                          */
/************************************************************************/

static ST_VOID _mmsLogReq_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mmsLogReq_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mms_debug_sel |= MMS_LOG_REQ;
    }
  else if (!stricmp (str, "OFF"))
    {
    mms_debug_sel &= ~MMS_LOG_REQ;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MmsLogReq Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
  }

/************************************************************************/
/*                            _mmsLogResp_EFun                          */
/************************************************************************/

static ST_VOID _mmsLogResp_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mmsLogResp_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mms_debug_sel |= MMS_LOG_RESP;
    }
  else if (!stricmp (str, "OFF"))
    {
    mms_debug_sel &= ~MMS_LOG_RESP;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MmsLogResp Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
  }

/************************************************************************/
/*                            _mmsLogInd_EFun                          */
/************************************************************************/

static ST_VOID _mmsLogInd_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mmsLogInd_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mms_debug_sel |= MMS_LOG_IND;
    }
  else if (!stricmp (str, "OFF"))
    {
    mms_debug_sel &= ~MMS_LOG_IND;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MmsLogInd Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
  }

/************************************************************************/
/*                            _mmsLogConf_EFun                          */
/************************************************************************/

static ST_VOID _mmsLogConf_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mmsLogConf_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mms_debug_sel |= MMS_LOG_CONF;
    }
  else if (!stricmp (str, "OFF"))
    {
    mms_debug_sel &= ~MMS_LOG_CONF;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MmsLogConf Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
  }

/************************************************************************/
/*                            _mmsLogNerr_EFun                          */
/************************************************************************/

static ST_VOID _mmsLogNerr_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mmsLogNerr_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mms_debug_sel |= MMS_LOG_NERR;
    }
  else if (!stricmp (str, "OFF"))
    {
    mms_debug_sel &= ~MMS_LOG_NERR;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MmsLogNerr Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
  }

/************************************************************************/
/*                           _mmsLogDec_EFun                            */
/************************************************************************/

static ST_VOID _mmsLogDec_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mmsLogDec_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mms_debug_sel |= MMS_LOG_DEC;
    }
  else if (!stricmp (str, "OFF"))
    {
    mms_debug_sel &= ~MMS_LOG_DEC;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MmsLogDec Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                            _mmsLogEnc_EFun                           */
/************************************************************************/

static ST_VOID _mmsLogEnc_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mmsLogEnc_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mms_debug_sel |= MMS_LOG_ENC;
    }
  else if (!stricmp (str, "OFF"))
    {
    mms_debug_sel &= ~MMS_LOG_ENC;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MmsLogEnc Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                                _mmsLogRt_EFun                        */
/************************************************************************/

static ST_VOID _mmsLogRt_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mmsLogRt_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mms_debug_sel |= MMS_LOG_RT;   /* set this bit in the bit mask */
    }
  else if (!stricmp (str, "OFF"))
    {
    mms_debug_sel &= ~MMS_LOG_RT;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MmsLogRt Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                           _mmsLogRtaa_EFun                           */
/************************************************************************/

static ST_VOID _mmsLogRtaa_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mmsLogRtaa_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mms_debug_sel |= MMS_LOG_RTAA;
    }
  else if (!stricmp (str, "OFF"))
    {
    mms_debug_sel &= ~MMS_LOG_RTAA;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MmsLogRtaa Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                            _mmsLogAa_EFun                            */
/************************************************************************/

static ST_VOID _mmsLogAa_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mmsLogAa_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mms_debug_sel |= MMS_LOG_AA;
    }
  else if (!stricmp (str, "OFF"))
    {
    mms_debug_sel &= ~MMS_LOG_AA;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MmsLogAa Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                         _mvlLogMasks_SFun                            */
/************************************************************************/

static ST_VOID _mvlLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _mvlLogMasks_SFun");
  SX_PUSH (mvlLogMaskElements);
  }

/************************************************************************/
/*                           _mvlLogNerr_EFun                           */
/************************************************************************/

static ST_VOID _mvlLogNerr_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mvlLogNerr_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mvl_debug_sel |= MVLLOG_NERR;   /* turn on this bit in the bit mask */
    }
  else if (!stricmp (str, "OFF"))
    {
    mvl_debug_sel &= ~MVLLOG_NERR;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MvlLogNerr Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                            _mvlLogAcse_EFun                          */
/************************************************************************/

static ST_VOID _mvlLogAcse_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mvlLogAcse_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mvl_debug_sel |= MVLLOG_ACSE;   /* turn on this bit in the bit mask */
    }
  else if (!stricmp (str, "OFF"))
    {
    mvl_debug_sel &= ~MVLLOG_ACSE;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MvlLogAcse Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                          _mvlLogAcsedata_EFun                        */
/************************************************************************/

static ST_VOID _mvlLogAcsedata_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mvlLogAcsedata_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mvl_debug_sel |= MVLLOG_ACSEDATA;   /* turn on this bit in the bit mask */
    }
  else if (!stricmp (str, "OFF"))
    {
    mvl_debug_sel &= ~MVLLOG_ACSEDATA;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MvlLogAcsedata Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                           _mvlLogFlow_EFun                           */
/************************************************************************/

static ST_VOID _mvluLogFlow_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mvluLogFlow_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mvl_debug_sel |= MVLULOG_FLOW;   /* turn on this bit in the bit mask */
    }
  else if (!stricmp (str, "OFF"))
    {
    mvl_debug_sel &= ~MVLULOG_FLOW;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MvluLogFlow Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                          _mvlLogDebug_EFun                           */
/************************************************************************/

static ST_VOID _mvluLogDebug_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mvluLogDebug_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mvl_debug_sel |= MVLULOG_DEBUG;   /* turn on this bit in the bit mask */
    }
  else if (!stricmp (str, "OFF"))
    {
    mvl_debug_sel &= ~MVLULOG_DEBUG;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MvluLogDebug Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                          _mvlLogTiming_EFun                          */
/************************************************************************/

static ST_VOID _mvluLogTiming_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _mvluLogTiming_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    mvl_debug_sel |= MVLULOG_TIMING;   /* turn on this bit in the bit mask */
    }
  else if (!stricmp (str, "OFF"))
    {
    mvl_debug_sel &= ~MVLULOG_TIMING;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MvluLogTiming Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                             _acseLogMasks_SFun                       */
/************************************************************************/

static ST_VOID _acseLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _acseLogMasks_SFun");
  SX_PUSH (acseLogMaskElements);
  }

/************************************************************************/
/*                           _acseLogEnc_EFun                           */
/************************************************************************/

static ST_VOID _acseLogEnc_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _acseLogEnc_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    acse_debug_sel |= ACSE_LOG_ENC;
    }
  else if (!stricmp (str, "OFF"))
    {
    acse_debug_sel &= ~ACSE_LOG_ENC;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AcseLogEnc Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                             _acseLogDec_EFun                         */
/************************************************************************/

static ST_VOID _acseLogDec_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _acseLogDec_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    acse_debug_sel |= ACSE_LOG_DEC;
    }
  else if (!stricmp (str, "OFF"))
    {
    acse_debug_sel &= ~ACSE_LOG_DEC;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AcseLogDec Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

}

/************************************************************************/
/*                          _acseNerrPrint_EFun                         */
/************************************************************************/

static ST_VOID _acseNerrPrint_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _acseNerrPrint_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if defined(MAP30_ACSE)
  if (!stricmp (str, "ON"))
    {
    s_debug_sel |= ACSE_NERR_PRINT;
    }
  else if (!stricmp (str, "OFF"))
    {
    s_debug_sel &= ~ACSE_NERR_PRINT;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AcseNerrPrint Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ACSE_NERR_PRINT bit mask on or off");
#endif

}

/************************************************************************/
/*                         _acseIndPrint_EFun                           */
/************************************************************************/

static ST_VOID _acseIndPrint_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _acseIndPrint_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if defined(MAP30_ACSE)
  if (!stricmp (str, "ON"))
    {
    s_debug_sel |= ACSE_IND_PRINT;
    }
  else if (!stricmp (str, "OFF"))
    {
    s_debug_sel &= ~ACSE_IND_PRINT;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AcseIndPrint Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ACSE_IND_PRINT bit mask on or off");
#endif

}

/************************************************************************/
/*                              _acseCnfPrint_EFun                      */
/************************************************************************/

static ST_VOID _acseCnfPrint_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _acseCnfPrint_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if defined(MAP30_ACSE)
  if (!stricmp (str, "ON"))
    {
    s_debug_sel |= ACSE_CNF_PRINT;
    }
  else if (!stricmp (str, "OFF"))
    {
    s_debug_sel &= ~ACSE_CNF_PRINT;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AcseCnfPrint Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ACSE_CNF_PRINT bit mask on or off");
#endif

}

/************************************************************************/
/*                           _acseDecPrint_EFun                         */
/************************************************************************/

static ST_VOID _acseDecPrint_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _acseDecPrint_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if defined(MAP30_ACSE)
  if (!stricmp (str, "ON"))
    {
    s_debug_sel |= ACSE_DEC_PRINT;
    }
  else if (!stricmp (str, "OFF"))
    {
    s_debug_sel &= ~ACSE_DEC_PRINT;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AcseDecPrint Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ACSE_DEC_PRINT bit mask on or off");
#endif

}

/************************************************************************/
/*                         _acseAbortPrint_EFun                         */
/************************************************************************/

static ST_VOID _acseAbortPrint_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _acseAbortPrint_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if defined(MAP30_ACSE)
  if (!stricmp (str, "ON"))
    {
    s_debug_sel |= ACSE_ABORT_PRINT;
    }
  else if (!stricmp (str, "OFF"))
    {
    s_debug_sel &= ~ACSE_ABORT_PRINT;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AcseAbortPrint Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ACSE_ABORT_PRINT bit mask on or off");
#endif

}

/************************************************************************/
/*                         _tp4LogMasks_SFun                            */
/************************************************************************/

static ST_VOID _tp4LogMasks_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _tp4LogMasks_SFun");
  SX_PUSH (tp4LogMaskElements);
  }

/************************************************************************/
/*                         _tp4LogFlowup_EFun                           */
/************************************************************************/

static ST_VOID _tp4LogFlowup_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _tp4LogFlowup_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;
#if !defined(MAP30_ACSE)
  if (!stricmp (str, "ON"))
    {
    tp4_debug_sel |= TP4_LOG_FLOWUP;
    }
  else if (!stricmp (str, "OFF"))
    {
    tp4_debug_sel &= ~TP4_LOG_FLOWUP;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: Tp4LogFlowup Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for TP4_LOG_FLOWUP bit mask on or off");
#endif
  }

/************************************************************************/
/*                          _tp4LogFlowdown_EFun                        */
/************************************************************************/

static ST_VOID _tp4LogFlowdown_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _tp4LogFlowdown_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if !defined(MAP30_ACSE)
  if (!stricmp (str, "ON"))
    {
    tp4_debug_sel |= TP4_LOG_FLOWDOWN;
    }
  else if (!stricmp (str, "OFF"))
    {
    tp4_debug_sel &= ~TP4_LOG_FLOWDOWN;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: Tp4LogFlowDown Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for TP4_LOG_FLOWDOWN bit mask on or off");
#endif
 }

/************************************************************************/
/*                           _clnpLogMasks_SFun                         */
/************************************************************************/

static ST_VOID _clnpLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _clnpLogMasks_SFun");
  SX_PUSH (clnpLogMaskElements);
  }

/************************************************************************/
/*                            _clnpLogNerr_EFun                         */
/************************************************************************/

static ST_VOID _clnpLogNerr_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _clnpLogNerr_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if !defined(MAP30_ACSE)
  if (!stricmp (str, "ON"))
    {
    clnp_debug_sel |= CLNP_LOG_NERR;
    }
  else if (!stricmp (str, "OFF"))
    {
    clnp_debug_sel &= ~CLNP_LOG_NERR;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: ClnpLogNerr Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for CLNP_LOG_NERR bit mask on or off");
#endif
  }

/************************************************************************/
/*                              _clnpLogReq_EFun                        */
/************************************************************************/

static ST_VOID _clnpLogReq_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _clnpLogReq_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if !defined(MAP30_ACSE)
  if (!stricmp (str, "ON"))
    {
    clnp_debug_sel |= CLNP_LOG_REQ;
    }
  else if (!stricmp (str, "OFF"))
    {
    clnp_debug_sel &= ~CLNP_LOG_REQ;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: ClnpLogReq Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for CLNP_LOG_REQ bit mask on or off");
#endif
  }

/************************************************************************/
/*                            _clnpLogInd_EFun                          */
/************************************************************************/

static ST_VOID _clnpLogInd_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _clnpLogInd_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if !defined(MAP30_ACSE)
  if (!stricmp (str, "ON"))
    {
    clnp_debug_sel |= CLNP_LOG_IND;
    }
  else if (!stricmp (str, "OFF"))
    {
    clnp_debug_sel &= ~CLNP_LOG_IND;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: ClnpLogInd Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for CLNP_LOG_IND bit mask on or off");
#endif
  }

/************************************************************************/
/*                            _clnpLogEncDec_EFun                       */
/************************************************************************/

static ST_VOID _clnpLogEncDec_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _clnpLogEncdec_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if !defined(MAP30_ACSE)
  if (!stricmp (str, "ON"))
    {
    clnp_debug_sel |= CLNP_LOG_ENC_DEC;
    }
  else if (!stricmp (str, "OFF"))
    {
    clnp_debug_sel &= ~CLNP_LOG_ENC_DEC;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: ClnpLogEncDec Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for CLNP_LOG_ENC_DEC bit mask on or off");
#endif
  }

/************************************************************************/
/*                             _clnpLogLlcEncDec_EFun                   */
/************************************************************************/

static ST_VOID _clnpLogLlcEncDec_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _clnpLogLlcEncdec_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if !defined(MAP30_ACSE)
  if (!stricmp (str, "ON"))
    {
    clnp_debug_sel |= CLNP_LOG_LLC_ENC_DEC;
    }
  else if (!stricmp (str, "OFF"))
    {
    clnp_debug_sel &= ~CLNP_LOG_LLC_ENC_DEC;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: ClnpLogLlcEncDec Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for TP4_LOG_LLC_ENC_DEC bit mask on or off");
#endif
  }

/************************************************************************/
/*                          _clsnsLogReq_EFun                           */
/************************************************************************/

static ST_VOID _clsnsLogReq_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _clsnsLogReq_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if !defined(MAP30_ACSE)
  if (!stricmp (str, "ON"))
    {
    clnp_debug_sel |= CLSNS_LOG_REQ;
    }
  else if (!stricmp (str, "OFF"))
    {
    clnp_debug_sel &= ~CLSNS_LOG_REQ;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: clsnsLogReq Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for CLNP_LOG_REQ bit mask on or off");
#endif
  }

/************************************************************************/
/*                           _clsnsLogInd_EFun                          */
/************************************************************************/

static ST_VOID _clsnsLogInd_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _clsnsLogInd_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if !defined(MAP30_ACSE)
  if (!stricmp (str, "ON"))
    {
    clnp_debug_sel |= CLSNS_LOG_IND;
    }
  else if (!stricmp (str, "OFF"))
    {
    clnp_debug_sel &= ~CLSNS_LOG_IND;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: ClsnsLogInd Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for CLNP_LOG_IND bit mask on or off");
#endif
  }

/************************************************************************/
/*                         _adlcLogMasks_SFun                           */
/************************************************************************/

static ST_VOID _adlcLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl)
  {                        
  SXLOG_CFLOW0 ("SX DEC: _adlcLogMasks_SFun");
  SX_PUSH (adlcLogMaskElements);
  }

/************************************************************************/
/*                           _adlcLogNerr_EFun                          */
/************************************************************************/

static ST_VOID _adlcLogNerr_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _adlcLogNerr_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if (defined (ADLC_SLAVE) || defined (ADLC_MASTER))

  if (!stricmp (str, "ON"))
    {
    adlc_debug_sel |= ADLC_MASK_LOG_NERR;
    }
  else if (!stricmp (str, "OFF"))
    {
    adlc_debug_sel &= ~ADLC_MASK_LOG_NERR;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AdlcLogNerr Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ADLC_MASK_LOG_NERR bit mask on or off");
#endif
  }
  
/************************************************************************/
/*                           _adlcLogFlow_EFun                          */
/************************************************************************/

static ST_VOID _adlcLogFlow_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _adlcLogFlow_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if (defined (ADLC_SLAVE) || defined (ADLC_MASTER))

  if (!stricmp (str, "ON"))
    {
    adlc_debug_sel |= ADLC_MASK_LOG_FLOW;
    }
  else if (!stricmp (str, "OFF"))
    {
    adlc_debug_sel &= ~ADLC_MASK_LOG_FLOW;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AdlcLogFlow Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ADLC_MASK_LOG_FLOW bit mask on or off");
#endif
  }

/************************************************************************/
/*                           _adlcLogIo_EFun                            */
/************************************************************************/

static ST_VOID _adlcLogIo_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _adlcLogIo_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if (defined (ADLC_SLAVE) || defined (ADLC_MASTER))

  if (!stricmp (str, "ON"))
    {
    adlc_debug_sel |= ADLC_MASK_LOG_IO;
    }
  else if (!stricmp (str, "OFF"))
    {
    adlc_debug_sel &= ~ADLC_MASK_LOG_IO;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AdlcLogIo Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ADLC_MASK_LOG_IO bit mask on or off");
#endif
  }

/************************************************************************/
/*                         _adlcLogHexio_EFun                           */
/************************************************************************/

static ST_VOID _adlcLogHexio_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _adlcLogHexio_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if (defined (ADLC_SLAVE) || defined (ADLC_MASTER))

  if (!stricmp (str, "ON"))
    {
    adlc_debug_sel |= ADLC_MASK_LOG_HEXIO;
    }
  else if (!stricmp (str, "OFF"))
    {
    adlc_debug_sel &= ~ADLC_MASK_LOG_HEXIO;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AdlcLogHexio Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ADLC_MASK_LOG_HEXIO bit mask on or off");
#endif
  }

/************************************************************************/
/*                            _adlcLogSm_EFun                           */
/************************************************************************/

static ST_VOID _adlcLogSm_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _adlcLogSm_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if (defined (ADLC_SLAVE) || defined (ADLC_MASTER))

  if (!stricmp (str, "ON"))
    {
    adlc_debug_sel |= ADLC_MASK_LOG_SM;
    }
  else if (!stricmp (str, "OFF"))
    {
    adlc_debug_sel &= ~ADLC_MASK_LOG_SM;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AdlcLogSm Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ADLC_MASK_LOG_SM bit mask on or off");
#endif
  }

/************************************************************************/
/*                         _adlcsLogHexsm_EFun                          */
/************************************************************************/

static ST_VOID _adlcLogHexsm_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _adlcLogReq_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if (defined (ADLC_SLAVE) || defined (ADLC_MASTER))

  if (!stricmp (str, "ON"))
    {
    adlc_debug_sel |= ADLC_MASK_LOG_HEXSM;
    }
  else if (!stricmp (str, "OFF"))
    {
    adlc_debug_sel &= ~ADLC_MASK_LOG_HEXSM;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AdlcLogHexsm Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ADLC_MASK_LOG_HEXSM bit mask on or off");
#endif
  }

/************************************************************************/
/*                           _adlcsLogUser_EFun                         */
/************************************************************************/

static ST_VOID _adlcLogUser_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _adlcsLogUser_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if (defined (ADLC_SLAVE) || defined (ADLC_MASTER))

  if (!stricmp (str, "ON"))
    {
    adlc_debug_sel |= ADLC_MASK_LOG_USER;
    }
  else if (!stricmp (str, "OFF"))
    {
    adlc_debug_sel &= ~ADLC_MASK_LOG_USER;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AdlcLogUser Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ADLC_MASK_LOG_USER bit mask on or off");
#endif
  }

/************************************************************************/
/*                            _adlcsLogHexuser_EFun                     */
/************************************************************************/

static ST_VOID _adlcLogHexuser_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _adlcsLogHexuser_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if (defined (ADLC_SLAVE) || defined (ADLC_MASTER))

  if (!stricmp (str, "ON"))
    {
    adlc_debug_sel |= ADLC_MASK_LOG_HEXUSER;
    }
  else if (!stricmp (str, "OFF"))
    {
    adlc_debug_sel &= ~ADLC_MASK_LOG_HEXUSER;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: AdlcLogHexUser Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ADLC_MASK_LOG_HEXUSER bit mask on or off");
#endif
  }

/************************************************************************/
/*                               _miLogMasks_SFun                       */
/************************************************************************/

static ST_VOID _miLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _miLogMasks_SFun");
  SX_PUSH (miLogMaskElements);
  }

/************************************************************************/
/*                              _miLogNerr_EFun                         */
/************************************************************************/

static ST_VOID _miLogNerr_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _miLogNerr_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if defined (ICCP_LITE)
  if (!stricmp (str, "ON"))
    {
    mi_debug_sel |= MI_LOG_NERR;
    }
  else if (!stricmp (str, "OFF"))
    {
    mi_debug_sel &= ~MI_LOG_NERR;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MiLogNerr Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
#endif

  }

/************************************************************************/
/*                              _miLogFlow_EFun                         */
/************************************************************************/

static ST_VOID _miLogFlow_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
  ST_BOOLEAN rc;
  ST_CHAR *str;
  ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _miLogFlow_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if defined (ICCP_LITE)
  if (!stricmp (str, "ON"))
    {
    mi_debug_sel |= MILOG_FLOW;   /* turn on this bit in the bit mask */
    }
  else if (!stricmp (str, "OFF"))
    {
    mi_debug_sel &= ~MILOG_FLOW;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MiLogFlow Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
#endif

  }

/************************************************************************/
/*                              _miLogDebug_EFun                        */
/************************************************************************/

static ST_VOID _miLogDebug_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
  ST_BOOLEAN rc;
  ST_CHAR *str;
  ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _miLogDebug_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if defined (ICCP_LITE)
  if (!stricmp (str, "ON"))
    {
    mi_debug_sel |= MILOG_DEBUG;   /* turn on this bit in the bit mask */
    }
  else if (!stricmp (str, "OFF"))
    {
    mi_debug_sel &= ~MILOG_DEBUG;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MiLogDebug Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
#endif

  }


/************************************************************************/
/*                              _miLogAddr_EFun                        */
/************************************************************************/

static ST_VOID _miLogAddr_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
  ST_BOOLEAN rc;
  ST_CHAR *str;
  ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _miLogAddr_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if defined (ICCP_LITE)
  if (!stricmp (str, "ON"))
    {
    mi_debug_sel |= MILOG_ADDR;   /* turn on this bit in the bit mask */
    }
  else if (!stricmp (str, "OFF"))
    {
    mi_debug_sel &= ~MILOG_ADDR;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MiLogAddr Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
#endif

  }


/************************************************************************/
/*                              _miLogCfg_EFun 	                       */
/************************************************************************/

static ST_VOID _miLogCfg_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
  ST_BOOLEAN rc;
  ST_CHAR *str;
  ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _miLogCfg_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if defined (ICCP_LITE)
  if (!stricmp (str, "ON"))
    {
    mi_debug_sel |= MILOG_CFG;   /* turn on this bit in the bit mask */
    }
  else if (!stricmp (str, "OFF"))
    {
    mi_debug_sel &= ~MILOG_CFG;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MiLogCfg Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }
#endif

  }

/************************************************************************/
/*                          _smpLogMasks_SFun                           */
/************************************************************************/

static ST_VOID _smpLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _smpLogMasks_SFun");
  SX_PUSH (smpLogMaskElements);
  }

/************************************************************************/
/*                             _smpLogReq_EFun                          */
/************************************************************************/

static ST_VOID _smpLogReq_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _smpLogReq_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if defined (UCA_SMP)

  if (!stricmp (str, "ON"))
    {
    smp_debug_sel |= SMP_LOG_REQ;
    }
  else if (!stricmp (str, "OFF"))
    {
    smp_debug_sel &= ~SMP_LOG_REQ;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: SmpLogReq Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for SMP_LOG_REQ bit mask on or off");
#endif
  }

/************************************************************************/
/*                            _smpLogInd_EFun                           */
/************************************************************************/

static ST_VOID _smpLogInd_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _smpLogInd_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if defined (UCA_SMP)

  if (!stricmp (str, "ON"))
    {
    smp_debug_sel |= SMP_LOG_IND;
    }
  else if (!stricmp (str, "OFF"))
    {
    smp_debug_sel &= ~SMP_LOG_IND;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: SmpLogInd Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for SMP_LOG_IND bit mask on or off");
#endif
  }

/************************************************************************/
/*          _icfgLogMasks_SFun                                          */
/************************************************************************/

static ST_VOID _icfgLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _icfgLogMasks_SFun");
  SX_PUSH (icfgLogMaskElements);
  }

/************************************************************************/
/*                      _icfgLogFlow_EFun                               */
/************************************************************************/

static ST_VOID _icfgLogFlow_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _icfgLogFlow_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if defined (ICFG)

  if (!stricmp (str, "ON"))
    {
    icfg_debug_sel |= ICFG_LOG_FLOW;
    }
  else if (!stricmp (str, "OFF"))
    {
    icfg_debug_sel &= ~ICFG_LOG_FLOW;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: IcfgLogFlow Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ICFG_LOG_FLOW bit mask on or off");
#endif
  }

/************************************************************************/
/*                      _icfgLogNerr_EFun                               */
/************************************************************************/

static ST_VOID _icfgLogNerr_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _icfgLogNerr_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

#if defined (ICFG)

  if (!stricmp (str, "ON"))
    {
    icfg_debug_sel |= ICFG_LOG_NERR;
    }
  else if (!stricmp (str, "OFF"))
    {
    icfg_debug_sel &= ~ICFG_LOG_NERR;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: IcfgLogNerr Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

#else
  SXLOG_CFLOW0 ("SX DEC: ignoring check for ICFG_LOG_NERR bit mask on or off");
#endif
  }

/************************************************************************/
/*                         _memLogMasks_SFun                            */
/************************************************************************/

static ST_VOID _memLogMasks_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _memLogMasks_SFun");
  SX_PUSH (memLogMaskElements);
  }

/************************************************************************/
/*                          _memLogCalloc_EFun                          */
/************************************************************************/

static ST_VOID _memLogCalloc_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _memLogCalloc_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    chk_debug_en |= MEM_LOG_CALLOC;
    }
  else if (!stricmp (str, "OFF"))
    {
    chk_debug_en &= ~MEM_LOG_CALLOC;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MemLogCalloc Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                               _memLogMalloc_EFun                     */
/************************************************************************/

static ST_VOID _memLogMalloc_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _memLogMalloc_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    chk_debug_en |= MEM_LOG_MALLOC;
    }
  else if (!stricmp (str, "OFF"))
    {
    chk_debug_en &= ~MEM_LOG_MALLOC;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MemLogMalloc Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                              _memLogRealloc_EFun                     */
/************************************************************************/

static ST_VOID _memLogRealloc_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _memLogRealloc_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    chk_debug_en |= MEM_LOG_REALLOC;
    }
  else if (!stricmp (str, "OFF"))
    {
    chk_debug_en &= ~MEM_LOG_REALLOC;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MemLogRealloc Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                               _memLogFree_EFun                       */
/************************************************************************/

static ST_VOID _memLogFree_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _memLogFree_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    chk_debug_en |= MEM_LOG_FREE;
    }
  else if (!stricmp (str, "OFF"))
    {
    chk_debug_en &= ~MEM_LOG_FREE;  /* turn off this bit in the bit mask */
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MemLogFree Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                           _memAllocDbgCtrl_SFun                      */
/************************************************************************/

static ST_VOID _memAllocDbgCtrl_SFun (SX_DEC_CTRL *sxDecCtrl)
  {
  SXLOG_CFLOW0 ("SX DEC: _memAllocDbgCtrl_SFun");
  SX_PUSH (memAllocElements);
  }


/************************************************************************/
/*                             _memFillEnable_EFun                      */
/************************************************************************/

static ST_VOID _memFillEnable_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _memFillEnable_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    m_fill_en = SD_TRUE;
    }
  else if (!stricmp (str, "OFF"))
    {
    m_fill_en = SD_FALSE;  
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MemFillEnable Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                          _memHeapCheck_EFun                          */
/************************************************************************/

static ST_VOID _memHeapCheck_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _memHeapCheck_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    m_heap_check_enable = SD_TRUE;
    }
  else if (!stricmp (str, "OFF"))
    {
    m_heap_check_enable = SD_FALSE;
    }
  else
    {
    SXLOG_ERR0 ("ERROR: MemHeapCheck Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                            _checkListEnable_EFun                     */
/************************************************************************/

static ST_VOID _checkListEnable_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _checkListEnable_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    m_check_list_enable = SD_TRUE;
    }
  else if (!stricmp (str, "OFF"))
    {
    m_check_list_enable = SD_FALSE;
    }
  else
    {
    SXLOG_ERR0 ("ERROR: CheckListEnable Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*          _findNodeEnable_EFun                */
/************************************************************************/

static ST_VOID _findNodeEnable_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _memLogFree_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    m_find_node_enable = SD_TRUE;
    }
  else if (!stricmp (str, "OFF"))
    {
    m_find_node_enable = SD_FALSE;
    }
  else
    {
    SXLOG_ERR0 ("ERROR: FindNodeEnable Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                           _trackPrevFree_EFun                        */
/************************************************************************/

static ST_VOID _trackPrevFree_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _memLogFree_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    m_track_prev_free = SD_TRUE;     
    }
  else if (!stricmp (str, "OFF"))
    {
    m_track_prev_free = SD_FALSE;
    }
  else
    {
    SXLOG_ERR0 ("ERROR: TrackPrevFree Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                            _listDebugSel_EFun                        */
/************************************************************************/

static ST_VOID _listDebugSel_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _listDebugSel_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    list_debug_sel = SD_TRUE;
    }
  else if (!stricmp (str, "OFF"))
    {
    list_debug_sel = SD_FALSE;
    }
  else
    {
    SXLOG_ERR0 ("ERROR: ListDEbugSel Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }

/************************************************************************/
/*                             _noReallocSmaller_EFun                   */
/************************************************************************/

static ST_VOID _noReallocSmaller_EFun (SX_DEC_CTRL *sxDecCtrl)
  {
ST_BOOLEAN rc;
ST_CHAR *str;
ST_INT strLen;

  SXLOG_CFLOW0 ("SX DEC: _noReallocSmaller_EFun");
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return;

  if (!stricmp (str, "ON"))
    {
    chk_debug_en |= MEM_LOG_FREE;
    }
  else if (!stricmp (str, "OFF"))
    {
    m_no_realloc_smaller = SD_FALSE;
    }
  else
    {
    SXLOG_ERR0 ("ERROR: NoReallocSmaller Data Conversion Error");
    sxDecCtrl->errCode = SX_ERR_CONVERT;
    }

  }


