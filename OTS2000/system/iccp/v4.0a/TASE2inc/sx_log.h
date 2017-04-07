/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	  	 1999 - 2001, All Rights Reserved.		*/
/*									*/
/*		    PROPRIETARY AND CONFIDENTIAL			*/
/*									*/
/* MODULE NAME : sx_log.h						*/
/* PRODUCT(S)  : 							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/06/01  GLB     05    Added SXLOG_CFLOW2                           */
/* 04/26/01  EJV     04    Added/rearranged slog macros for !DEBUG_SISCO*/
/* 01/25/01  JRB     03    Added SXLOG_ERR2,3				*/
/* 10/30/99  MDE     02    Added SXLOG_NERR3				*/
/* 09/17/99  MDE     01    New						*/
/************************************************************************/

#ifndef SX_LOG_INCLUDED
#define SX_LOG_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/

#include "slog.h"

/************************************************************************/
/* sx_debug_sel bit assignments						*/

#define SX_LOG_DEC  	0x0001   
#define SX_LOG_ENC  	0x0002   
#define SX_LOG_FLOW  	0x0004   
#define SX_LOG_DEBUG 	0x0008   
#define SX_LOG_ERR  	0x0010   
#define SX_LOG_NERR 	0x0020   

extern ST_UINT sx_debug_sel;


/************************************************************************/
#ifdef DEBUG_SISCO
/************************************************************************/

/* Log type strings */
extern SD_CONST ST_CHAR *SD_CONST _sx_log_dec_logstr;
extern SD_CONST ST_CHAR *SD_CONST _sx_log_enc_logstr;
extern SD_CONST ST_CHAR *SD_CONST _sx_log_flow_logstr;
extern SD_CONST ST_CHAR *SD_CONST _sx_log_debug_logstr;
extern SD_CONST ST_CHAR *SD_CONST _sx_log_debug_logstr;
extern SD_CONST ST_CHAR *SD_CONST _sx_log_err_logstr;
extern SD_CONST ST_CHAR *SD_CONST _sx_log_nerr_logstr;
       

#define SXLOG_DEC0(a) {\
                     if (sx_debug_sel & SX_LOG_DEC)\
                       _slog (sLogCtrl,_sx_log_dec_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define SXLOG_DEC1(a,b) {\
                     if (sx_debug_sel & SX_LOG_DEC)\
                       _slog (sLogCtrl,_sx_log_dec_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define SXLOG_CDEC0(a) {\
                     if (sx_debug_sel & SX_LOG_DEC)\
                       _slogc (sLogCtrl,a);\
                    }
#define SXLOG_CDEC1(a,b) {\
                     if (sx_debug_sel & SX_LOG_DEC)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define SXLOG_CDEC3(a,b,c,d) {\
                     if (sx_debug_sel & SX_LOG_DEC)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }
#define SXLOG_CDEC4(a,b,c,d,e) {\
                     if (sx_debug_sel & SX_LOG_DEC)\
                       _slogc (sLogCtrl,a,b,c,d,e);\
                    }
#define SXLOG_DECH(a,b) {\
                     if (sx_debug_sel & SX_LOG_DEC)\
                       _slogHex (sLogCtrl,a,b);\
                    }


#define SXLOG_ENC0(a) {\
                     if (sx_debug_sel & SX_LOG_ENC)\
                       _slog (sLogCtrl,_sx_log_enc_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define SXLOG_ENC1(a,b) {\
                     if (sx_debug_sel & SX_LOG_ENC)\
                       _slog (sLogCtrl,_sx_log_enc_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define SXLOG_CENC0(a) {\
                     if (sx_debug_sel & SX_LOG_ENC)\
                       _slogc (sLogCtrl,a);\
                    }
#define SXLOG_CENC1(a,b) {\
                     if (sx_debug_sel & SX_LOG_ENC)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define SXLOG_ENCH(a,b) {\
                     if (sx_debug_sel & SX_LOG_ENC)\
                       _slogHex (sLogCtrl,a,b);\
                    }


#define SXLOG_FLOW0(a) {\
                     if (sx_debug_sel & SX_LOG_FLOW)\
                       _slog (sLogCtrl,_sx_log_flow_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define SXLOG_FLOW1(a,b) {\
                     if (sx_debug_sel & SX_LOG_FLOW)\
                       _slog (sLogCtrl,_sx_log_flow_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define SXLOG_CFLOW0(a) {\
                     if (sx_debug_sel & SX_LOG_FLOW)\
                       _slogc (sLogCtrl,a);\
                    }
#define SXLOG_CFLOW1(a,b) {\
                     if (sx_debug_sel & SX_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define SXLOG_CFLOW2(a,b,c) {\
                     if (sx_debug_sel & SX_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define SXLOG_CFLOW3(a,b,c,d) {\
                     if (sx_debug_sel & SX_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }
#define SXLOG_CFLOW4(a,b,c,d,e) {\
                     if (sx_debug_sel & SX_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c,d,e);\
                    }
#define SXLOG_FLOWH(a,b) {\
                     if (sx_debug_sel & SX_LOG_FLOW)\
                       _slogHex (sLogCtrl,a,b);\
                    }

#define SXLOG_DEBUG0(a) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slog (sLogCtrl,_sx_log_debug_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define SXLOG_DEBUG1(a,b) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slog (sLogCtrl,_sx_log_debug_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define SXLOG_DEBUG2(a,b,c) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slog (sLogCtrl,_sx_log_debug_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define SXLOG_DEBUG3(a,b,c,d) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slog (sLogCtrl,_sx_log_debug_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }
#define SXLOG_DEBUG4(a,b,c,d,e) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slog (sLogCtrl,_sx_log_debug_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e);\
                    }
#define SXLOG_DEBUG5(a,b,c,d,e,f) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slog (sLogCtrl,_sx_log_debug_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e,f);\
                    }
#define SXLOG_DEBUG6(a,b,c,d,e,f,g) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slog (sLogCtrl,_sx_log_debug_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e,f,g);\
                    }
#define SXLOG_DEBUG7(a,b,c,d,e,f,g,h) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slog (sLogCtrl,_sx_log_debug_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                    }
#define SXLOG_DEBUG8(a,b,c,d,e,f,g,h,i) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slog (sLogCtrl,_sx_log_debug_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e,f,g,h,i);\
                    }
#define SXLOG_CDEBUG0(a) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slogc (sLogCtrl,a);\
                    }
#define SXLOG_CDEBUG1(a,b) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define SXLOG_CDEBUG2(a,b,c) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define SXLOG_CDEBUG3(a,b,c,d) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }
#define SXLOG_CDEBUG4(a,b,c,d,e) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slogc (sLogCtrl,a,b,c,d,e);\
                    }
#define SXLOG_CDEBUG5(a,b,c,d,e,f) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slogc (sLogCtrl,a,b,c,d,e,f);\
                    }
#define SXLOG_CDEBUG6(a,b,c,d,e,f,g) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slogc (sLogCtrl,a,b,c,d,e,f,g);\
                    }
#define SXLOG_CDEBUG7(a,b,c,d,e,f,g,h) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slogc (sLogCtrl,a,b,c,d,e,f,g,h);\
                    }
#define SXLOG_CDEBUG8(a,b,c,d,e,f,g,h,i) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slogc (sLogCtrl,a,b,c,d,e,f,g,h,i);\
                    }
#define SXLOG_DEBUGH(a,b) {\
                     if (sx_debug_sel & SX_LOG_DEBUG)\
                       _slogHex (sLogCtrl,a,b);\
                    }

#define SXLOG_ERR0(a) {\
                     if (sx_debug_sel & SX_LOG_ERR)\
                       _slog (sLogCtrl,_sx_log_err_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define SXLOG_ERR1(a,b) {\
                     if (sx_debug_sel & SX_LOG_ERR)\
                       _slog (sLogCtrl,_sx_log_err_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define SXLOG_ERR2(a,b,c) {\
                     if (sx_debug_sel & SX_LOG_ERR)\
                       _slog (sLogCtrl,_sx_log_err_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define SXLOG_ERR3(a,b,c,d) {\
                     if (sx_debug_sel & SX_LOG_ERR)\
                       _slog (sLogCtrl,_sx_log_err_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }
#define SXLOG_NERR0(a) {\
                     if (sx_debug_sel & SX_LOG_NERR)\
                       _slog (sLogCtrl,_sx_log_nerr_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define SXLOG_NERR1(a,b) {\
                     if (sx_debug_sel & SX_LOG_NERR)\
                       _slog (sLogCtrl,_sx_log_nerr_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define SXLOG_NERR2(a,b,c) {\
                     if (sx_debug_sel & SX_LOG_NERR)\
                       _slog (sLogCtrl,_sx_log_nerr_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define SXLOG_NERR3(a,b,c,d) {\
                     if (sx_debug_sel & SX_LOG_NERR)\
                       _slog (sLogCtrl,_sx_log_nerr_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }
#define SXLOG_NERRH(a,b) {\
                     if (sx_debug_sel & SX_LOG_NERR)\
                       _slogHex (sLogCtrl,a,b);\
                    }

#else
/* DEBUG_SISCO not defined */

#define SXLOG_DECLF()
#define SXLOG_PAUSEDEC(msg)

#define SXLOG_DEC0(a)
#define SXLOG_DEC1(a,b)
#define SXLOG_CDEC0(a)
#define SXLOG_CDEC1(a,b)
#define SXLOG_CDEC3(a,b,c,d)
#define SXLOG_CDEC4(a,b,c,d,e)
#define SXLOG_DECH(a,b)
#define SXLOG_ENC0(a)
#define SXLOG_ENC1(a,b)
#define SXLOG_CENC0(a)
#define SXLOG_CENC1(a,b)
#define SXLOG_ENCH(a,b)
#define SXLOG_FLOW0(a)
#define SXLOG_FLOW1(a,b)
#define SXLOG_CFLOW0(a)
#define SXLOG_CFLOW1(a,b)
#define SXLOG_CFLOW2(a,b,c)
#define SXLOG_CFLOW3(a,b,c,d)
#define SXLOG_CFLOW4(a,b,c,d,e)
#define SXLOG_FLOWH(a,b)
#define SXLOG_DEBUG0(a)
#define SXLOG_DEBUG1(a,b)
#define SXLOG_DEBUG2(a,b,c)
#define SXLOG_DEBUG3(a,b,c,d)
#define SXLOG_DEBUG4(a,b,c,d,e)
#define SXLOG_DEBUG5(a,b,c,d,e,f)
#define SXLOG_DEBUG6(a,b,c,d,e,f,g)
#define SXLOG_DEBUG7(a,b,c,d,e,f,g,h)
#define SXLOG_DEBUG8(a,b,c,d,e,f,g,h,i)
#define SXLOG_CDEBUG0(a)
#define SXLOG_CDEBUG1(a,b)
#define SXLOG_CDEBUG2(a,b,c)
#define SXLOG_CDEBUG3(a,b,c,d)
#define SXLOG_CDEBUG4(a,b,c,d,e)
#define SXLOG_CDEBUG5(a,b,c,d,e,f)
#define SXLOG_CDEBUG6(a,b,c,d,e,f,g)
#define SXLOG_CDEBUG7(a,b,c,d,e,f,g,h)
#define SXLOG_CDEBUG8(a,b,c,d,e,f,g,h,i)
#define SXLOG_DEBUGH(a,b)
#define SXLOG_ERR0(a)
#define SXLOG_ERR1(a,b)
#define SXLOG_ERR2(a,b,c)
#define SXLOG_ERR3(a,b,c,d)
#define SXLOG_NERR0(a)
#define SXLOG_NERR1(a,b)
#define SXLOG_NERR2(a,b,c)
#define SXLOG_NERR3(a,b,c,d)
#define SXLOG_NERRH(a,b)

/************************************************************************/
#endif		/* End of DEBUG_SISCO					*/
/************************************************************************/

#ifdef __cplusplus
}
#endif


/************************************************************************/
#endif		/* End of SX_LOG_INCLUDED				*/
/************************************************************************/
