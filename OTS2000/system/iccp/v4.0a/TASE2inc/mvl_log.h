/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	  1996, All Rights Reserved.		        	*/
/*									*/
/* MODULE NAME : mvl_log.h 						*/
/* PRODUCT(S)  : Tool							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 07/31/01  MDE     08    Added mvl_conn_filtered_logging		*/
/* 03/28/01  JRB     07    Add ..ACSEDATA3 macro.			*/
/* 03/22/01  GLB     06    Added: MVLULOG_DEBUG, MVLULOG_TIMING         */
/* 12/13/99  JRB     05    Delete ";" from non-debug macros. Get extra	*/
/*			   ";" when expanded (doesn't always compile).	*/
/* 09/13/99  MDE     04    Added SD_CONST modifiers			*/
/* 09/07/99  MDE     03    Added FLOW3, CFLOW3 macros			*/
/* 10/08/98  MDE     02    Migrated to updated SLOG interface		*/
/* 09/10/97  MDE     01    MMS-LITE V4.0 Release			*/
/************************************************************************/

#ifndef MVL_LOG_INCLUDED
#define MVL_LOG_INCLUDED

#include "slog.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/
/* Request/Response logging macros */

#if defined(DEBUG_SISCO) && defined(MLOG_ENABLE)
#define MVL_LOG_REQ(op,info)  mvl_log_req(op,info)
#define MVL_LOG_RESP(op,info) mvl_log_resp(op,info)
#else
#define MVL_LOG_REQ(op,info)
#define MVL_LOG_RESP(op,info)
#endif

/************************************************************************/
/* MMS-EASE Lite logging control 					*/

#define MVLLOG_ERR		0x00000001
#define MVLLOG_NERR		0x00000002
#define MVLLOG_ACSE		0x00000040
#define MVLLOG_ACSEDATA		0x00000080
#define MVLULOG_FLOW		0x00000200
#define MVLULOG_DEBUG		0x00000400
#define MVLULOG_TIMING		0x00000800


/* For backward compatability only */
#define mvl_log_mask mvl_debug_sel
extern ST_UINT mvl_debug_sel;

/* Allows filtering of ACSE data logging by connection */
extern ST_BOOLEAN mvl_conn_filtered_logging;

/************************************************************************/
#ifdef DEBUG_SISCO
/************************************************************************/

/* Log type strings */
extern SD_CONST ST_CHAR *SD_CONST _mvllog_err_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mvllog_nerr_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mvllog_acse_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mvllog_acsedata_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mvlulog_flow_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mvlulog_debug_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mvlulog_timing_logstr;


/*********************** ERR Macros *************************************/

#define MVL_LOG_ERR0(a) {\
                     if (mvl_debug_sel & MVLLOG_ERR)\
                       _slog (sLogCtrl,_mvllog_err_logstr,\
		             thisFileName,__LINE__,a);\
                    }

#define MVL_LOG_ERR1(a,b) {\
                     if (mvl_debug_sel & MVLLOG_ERR)\
                       _slog (sLogCtrl,_mvllog_err_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }

#define MVL_LOG_ERR2(a,b,c) {\
                     if (mvl_debug_sel & MVLLOG_ERR)\
                       _slog (sLogCtrl,_mvllog_err_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }

#define MVL_LOG_CERR2(a,b,c) {\
                     if (mvl_debug_sel & MVLLOG_ERR)\
                       _slogc (sLogCtrl,a,b,c);\
                    }

/*********************** NERR Macros *************************************/

#define MVL_LOG_NERR0(a) {\
                     if (mvl_debug_sel & MVLLOG_NERR)\
                       _slog (sLogCtrl,_mvllog_nerr_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MVL_LOG_NERR1(a,b) {\
                     if (mvl_debug_sel & MVLLOG_NERR)\
                       _slog (sLogCtrl,_mvllog_nerr_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MVL_LOG_NERR2(a,b,c) {\
                     if (mvl_debug_sel & MVLLOG_NERR)\
                       _slog (sLogCtrl,_mvllog_nerr_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }

/*********************** ACSE Macros *************************************/

#define MVL_LOG_ACSE0(a) {\
                     if (mvl_debug_sel & MVLLOG_ACSE)\
                       _slog (sLogCtrl,_mvllog_acse_logstr,\
		             thisFileName,__LINE__,a);\
                    }

#define MVL_LOG_ACSE1(a,b) {\
                     if (mvl_debug_sel & MVLLOG_ACSE)\
                       _slog (sLogCtrl,_mvllog_acse_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }

#define MVL_LOG_CACSE0(a) {\
                     if (mvl_debug_sel & MVLLOG_ACSE)\
                       _slogc (sLogCtrl,a);\
                    }

#define MVL_LOG_CACSE1(a,b) {\
                     if (mvl_debug_sel & MVLLOG_ACSE)\
                       _slogc (sLogCtrl,a,b);\
                    }



#define MVL_LOG_ACSEDATA1(a,b) {\
                     if (mvl_debug_sel & MVLLOG_ACSEDATA)\
                       _slog (sLogCtrl,_mvllog_acsedata_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }

#define MVL_LOG_ACSEDATA2(a,b,c) {\
                     if (mvl_debug_sel & MVLLOG_ACSEDATA)\
                       _slog (sLogCtrl,_mvllog_acsedata_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define MVL_LOG_ACSEDATA3(a,b,c,d) {\
                     if (mvl_debug_sel & MVLLOG_ACSEDATA)\
                       _slog (sLogCtrl,_mvllog_acsedata_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }

#define MVL_LOG_ACSEDATAH(a,b) {\
                     if (mvl_debug_sel & MVLLOG_ACSEDATA)\
		       _slogHex (sLogCtrl,a,b);\
                    }

/*********************** MVLU FLOW Macros *******************************/

#define MVLU_LOG_FLOW0(a) {\
                     if (mvl_debug_sel & MVLULOG_FLOW)\
                       _slog (sLogCtrl,_mvlulog_flow_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MVLU_LOG_FLOW1(a,b) {\
                     if (mvl_debug_sel & MVLULOG_FLOW)\
                       _slog (sLogCtrl,_mvlulog_flow_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MVLU_LOG_FLOW2(a,b,c) {\
                     if (mvl_debug_sel & MVLULOG_FLOW)\
                       _slog (sLogCtrl,_mvlulog_flow_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define MVLU_LOG_FLOW3(a,b,c,d) {\
                     if (mvl_debug_sel & MVLULOG_FLOW)\
                       _slog (sLogCtrl,_mvlulog_flow_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }

#define MVLU_LOG_CFLOW0(a) {\
                     if (mvl_debug_sel & MVLULOG_FLOW)\
                       _slogc (sLogCtrl,a);\
                    }
#define MVLU_LOG_CFLOW1(a,b) {\
                     if (mvl_debug_sel & MVLULOG_FLOW)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MVLU_LOG_CFLOW2(a,b,c) {\
                     if (mvl_debug_sel & MVLULOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define MVLU_LOG_CFLOW3(a,b,c,d) {\
                     if (mvl_debug_sel & MVLULOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/*********************** MVLU DEBUG Macros *******************************/

#define MVLU_LOG_DEBUG0(a) {\
                     if (mvl_debug_sel & MVLULOG_DEBUG)\
                       _slog (sLogCtrl,_mvlulog_debug_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MVLU_LOG_DEBUG1(a,b) {\
                     if (mvl_debug_sel & MVLULOG_DEBUG)\
                       _slog (sLogCtrl,_mvlulog_debug_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MVLU_LOG_DEBUG2(a,b,c) {\
                     if (mvl_debug_sel & MVLULOG_DEBUG)\
                       _slog (sLogCtrl,_mvlulog_debug_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define MVLU_LOG_DEBUG3(a,b,c,d) {\
                     if (mvl_debug_sel & MVLULOG_DEBUG)\
                       _slog (sLogCtrl,_mvlulog_debug_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }

#define MVLU_LOG_CDEBUG0(a) {\
                     if (mvl_debug_sel & MVLULOG_DEBUG)\
                       _slogc (sLogCtrl,a);\
                    }
#define MVLU_LOG_CDEBUG1(a,b) {\
                     if (mvl_debug_sel & MVLULOG_DEBUG)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MVLU_LOG_CDEBUG2(a,b,c) {\
                     if (mvl_debug_sel & MVLULOG_DEBUG)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define MVLU_LOG_CDEBUG3(a,b,c,d) {\
                     if (mvl_debug_sel & MVLULOG_DEBUG)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/*********************** MVLU TIMING Macros *******************************/

#define MVLU_LOG_TIMING0(a) {\
                     if (mvl_debug_sel & MVLULOG_TIMING)\
                       _slog (sLogCtrl,_mvlulog_timing_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MVLU_LOG_TIMING1(a,b) {\
                     if (mvl_debug_sel & MVLULOG_TIMING)\
                       _slog (sLogCtrl,_mvlulog_timing_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MVLU_LOG_TIMING2(a,b,c) {\
                     if (mvl_debug_sel & MVLULOG_TIMING)\
                       _slog (sLogCtrl,_mvlulog_timing_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define MVLU_LOG_TIMING3(a,b,c,d) {\
                     if (mvl_debug_sel & MVLULOG_TIMING)\
                       _slog (sLogCtrl,_mvlulog_timing_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }

#define MVLU_LOG_CTIMING0(a) {\
                     if (mvl_debug_sel & MVLULOG_TIMING)\
                       _slogc (sLogCtrl,a);\
                    }
#define MVLU_LOG_CTIMING1(a,b) {\
                     if (mvl_debug_sel & MVLULOG_TIMING)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MVLU_LOG_CTIMING2(a,b,c) {\
                     if (mvl_debug_sel & MVLULOG_TIMING)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define MVLU_LOG_CTIMING3(a,b,c,d) {\
                     if (mvl_debug_sel & MVLULOG_TIMING)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/************************************************************************/
#else
/************************************************************************/

#define MVL_LOG_ERR0(a)
#define MVL_LOG_ERR1(a,b)
#define MVL_LOG_ERR2(a,b,c)
#define MVL_LOG_CERR2(a,b,c)

#define MVL_LOG_NERR0(a)
#define MVL_LOG_NERR1(a,b)
#define MVL_LOG_NERR2(a,b,c)

#define MVL_LOG_ACSE0(a)
#define MVL_LOG_ACSE1(a,b)
#define MVL_LOG_CACSE0(a)
#define MVL_LOG_CACSE1(a,b)

#define MVL_LOG_ACSEDATA1(a,b)
#define MVL_LOG_ACSEDATA2(a,b,c)
#define MVL_LOG_ACSEDATA3(a,b,c,d)
#define MVL_LOG_ACSEDATAH(a,b)

#define MVLU_LOG_FLOW0(a)
#define MVLU_LOG_FLOW1(a,b)
#define MVLU_LOG_FLOW2(a,b,c)
#define MVLU_LOG_FLOW3(a,b,c,d)
#define MVLU_LOG_CFLOW0(a)
#define MVLU_LOG_CFLOW1(a,b)
#define MVLU_LOG_CFLOW2(a,b,c)
#define MVLU_LOG_CFLOW3(a,b,c,d)

#define MVLU_LOG_DEBUG0(a)
#define MVLU_LOG_DEBUG1(a,b)
#define MVLU_LOG_DEBUG2(a,b,c)
#define MVLU_LOG_DEBUG3(a,b,c,d)
#define MVLU_LOG_CDEBUG0(a)
#define MVLU_LOG_CDEBUG1(a,b)
#define MVLU_LOG_CDEBUG2(a,b,c)
#define MVLU_LOG_CDEBUG3(a,b,c,d)

#define MVLU_LOG_TIMING0(a)
#define MVLU_LOG_TIMING1(a,b)
#define MVLU_LOG_TIMING2(a,b,c)
#define MVLU_LOG_TIMING3(a,b,c,d)
#define MVLU_LOG_CTIMING0(a)
#define MVLU_LOG_CTIMING1(a,b)
#define MVLU_LOG_CTIMING2(a,b,c)
#define MVLU_LOG_CTIMING3(a,b,c,d)
					
#endif
/************************************************************************/


#ifdef __cplusplus
}
#endif

#endif

