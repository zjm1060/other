/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1986 - 1997, All Rights Reserved		        */
/*									*/
/* MODULE NAME : rtload.c    						*/
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
/* 08/12/98  MDE     02    Now just read binary image from file 	*/
/* 02/24/98  MDE     01    New						*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "time.h"
#include "signal.h"
#include "mmsdefs.h"
#include "mms_pvar.h"
#include "mms_vvar.h"
#include "mms_pvmd.h"
#include "mms_err.h"
#include "mms_pcon.h"
#include "asn1defs.h"

#include "mvl_acse.h"
#include "mvl_log.h"

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/
/************************************************************************/

#ifdef DEBUG_SISCO
static ST_CHAR *thisFileName = __FILE__;
#endif

/************************************************************************/

#define OUTPUT_FILE_NAME "rt_data.bin"

static FILE *fp;

/************************************************************************/
ST_VOID u_mvl_start_init_rt_tbl (ST_INT numTypes, ST_INT totalNumRt)
  {
  fp = fopen (OUTPUT_FILE_NAME, "rb");
  }


/************************************************************************/

RUNTIME_TYPE *u_mvl_get_rt_tbl (ST_INT typeId, ST_INT numRt)
  {
RUNTIME_TYPE *rt;
ST_INT num_read;

  rt = chk_calloc (numRt, sizeof(RUNTIME_TYPE));
  num_read = fread (rt, sizeof(RUNTIME_TYPE), numRt, fp);
  return (rt);  
  }

/************************************************************************/
ST_VOID u_mvl_end_init_rt_tbl (ST_VOID)
  {
  fclose (fp);
  }
  
