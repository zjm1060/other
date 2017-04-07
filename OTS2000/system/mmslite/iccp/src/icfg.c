/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.        */
/*              2000 - 2002, All Rights Reserved                        */
/*                                                                      */
/* MODULE NAME : icfg.c							*/
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
/* 06/20/02  RKR    03     Copyright year update                        */
/* 04/17/02  MDE    02	   Added remote DV config options  		*/
/* 11/13/01  DGE    01     Module created                               */
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "str_util.h"
#include "sx_defs.h"
#include "sx_log.h"
#include "gen_list.h"
#include "mem_chk.h"

#include "icfg.h"


/************************************************************************/

#ifdef DEBUG_SISCO
SD_CONST ST_CHAR *SD_CONST _icfg_err_logstr  = "ICFG_LOG_ERR";
SD_CONST ST_CHAR *SD_CONST _icfg_nerr_logstr = "ICFG_LOG_NERR";
SD_CONST ST_CHAR *SD_CONST _icfg_flow_logstr = "ICFG_LOG_FLOW";
#endif

ST_UINT  icfg_debug_sel;


/************************************************************************/
/* arrays used to output the enumeration type in string format		*/
/* they must match the previous 'enum' declarations		        */

ST_CHAR *icfgDvTypeValEnum[]  = 
  {
  "Real",  
  "State",
  "Discrete",
  "RealQ",
  "StateQ",
  "DiscreteQ",
  "RealQTimeTag",
  "StateQTimeTag",
  "DiscreteQTimeTag",
  "RealExtended",
  "StateExtended",
  "DiscreteExtended",
  };    
         
ST_CHAR *icfgDevTypeValEnum[] = 
  {
  "Command",
  "Discrete",  
  "Real"
  };
         
ST_CHAR *icfgSourceValEnum[]  = 
  {
  "Calculated",      
  "Estimated",
  "Entered",
  "Telemetered"
  };
                                       
ST_CHAR *icfgScopeValEnum[] = 
  {
  "VCC",
  "ICC"
  };

ST_CHAR *icfgRemDvValidationEnum[] =
  {
  "None",
  "CompareNames",
  "CompareNamesAndTypes"
  };

ST_CHAR *icfgRemDvGetConfigEnum[] =
  {
  "None",
  "XML",
  "DirectDatabase"
  };
