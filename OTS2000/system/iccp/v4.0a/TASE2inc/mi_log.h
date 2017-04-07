/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	  1999-2002, All Rights Reserved.                           */
/*									*/
/* MODULE NAME : mi_log.h 						*/
/* PRODUCT(S)  : MI							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR     11    Copyright year update                        */
/* 06/07/02  MDE     10    Added MI_LOG_NERR3 no-debug			*/
/* 04/05/02  MDE     09    Added MI_LOG_CNERR3				*/
/* 04/05/02  MDE     08    Added MI_LOG_CFG macros			*/
/* 04/03/02  MDE     07    Added MI_LOG_NERR3				*/
/* 03/21/02  MDE     06    Added MILOG_ADDR, MILOG_CFG			*/
/* 11/26/01  MDE     05	   Cleaned up log mask names & macros		*/
/* 11/07/01  MDE     04	   Added CFLOW3 macros				*/
/* 05/25/99  RKR     03    Added CNERR log masks			*/
/* 04/01/99  MDE     02    Added MI_DEBUG macros			*/
/* 03/01/99  MDE     01    New file					*/
/************************************************************************/

#ifndef MI_LOG_INCLUDED
#define MI_LOG_INCLUDED

#include "slog.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/
/* ICCP Lite logging control 						*/

#define MILOG_ERR		0x00000001
#define MILOG_NERR		0x00000002
#define MILOG_FLOW		0x00000004
#define MILOG_DEBUG		0x00000008
#define MILOG_ADDR		0x00000010
#define MILOG_CFG		0x00000020

/* For backwards compatibility */
#define MI_LOG_NERR MILOG_NERR

extern ST_UINT mi_debug_sel;

/************************************************************************/
#ifdef DEBUG_SISCO
/************************************************************************/

/* Log type strings */
extern ST_CHAR *_milog_err_logstr;
extern ST_CHAR *_milog_nerr_logstr;
extern ST_CHAR *_milog_flow_logstr;
extern ST_CHAR *_milog_debug_logstr;
extern ST_CHAR *_milog_addr_logstr;
extern ST_CHAR *_milog_cfg_logstr;

/*********************** ERR Macros *************************************/

#define MI_LOG_ERR0(a) {\
                     if (mi_debug_sel & MILOG_ERR)\
                       _slog (sLogCtrl,_milog_err_logstr,\
		             thisFileName,__LINE__,a);\
                    }

#define MI_LOG_ERR1(a,b) {\
                     if (mi_debug_sel & MILOG_ERR)\
                       _slog (sLogCtrl,_milog_err_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }

#define MI_LOG_ERR2(a,b,c) {\
                     if (mi_debug_sel & MILOG_ERR)\
                       _slog (sLogCtrl,_milog_err_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }

#define MI_LOG_CERR2(a,b,c) {\
                     if (mi_debug_sel & MILOG_ERR)\
                       _slogc (sLogCtrl,a,b,c);\
                    }

/*********************** NERR Macros *************************************/

#define MI_LOG_NERR0(a) {\
                     if (mi_debug_sel & MILOG_NERR)\
                       _slog (sLogCtrl,_milog_nerr_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MI_LOG_NERR1(a,b) {\
                     if (mi_debug_sel & MILOG_NERR)\
                       _slog (sLogCtrl,_milog_nerr_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MI_LOG_NERR2(a,b,c) {\
                     if (mi_debug_sel & MILOG_NERR)\
                       _slog (sLogCtrl,_milog_nerr_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define MI_LOG_NERR3(a,b,c,d) {\
                     if (mi_debug_sel & MILOG_NERR)\
                       _slog (sLogCtrl,_milog_nerr_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }

#define MI_LOG_CNERR0(a) {\
                     if (mi_debug_sel & MILOG_NERR)\
                       _slogc (sLogCtrl,a);\
                    }
#define MI_LOG_CNERR1(a,b) {\
                     if (mi_debug_sel & MILOG_NERR)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MI_LOG_CNERR2(a,b,c) {\
                     if (mi_debug_sel & MILOG_NERR)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define MI_LOG_CNERR3(a,b,c,d) {\
                     if (mi_debug_sel & MILOG_NERR)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }
/*********************** MI FLOW Macros *******************************/

#define MI_LOG_FLOW0(a) {\
                     if (mi_debug_sel & MILOG_FLOW)\
                       _slog (sLogCtrl,_milog_flow_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MI_LOG_FLOW1(a,b) {\
                     if (mi_debug_sel & MILOG_FLOW)\
                       _slog (sLogCtrl,_milog_flow_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MI_LOG_FLOW2(a,b,c) {\
                     if (mi_debug_sel & MILOG_FLOW)\
                       _slog (sLogCtrl,_milog_flow_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }

#define MI_LOG_CFLOW0(a) {\
                     if (mi_debug_sel & MILOG_FLOW)\
                       _slogc (sLogCtrl,a);\
                    }
#define MI_LOG_CFLOW1(a,b) {\
                     if (mi_debug_sel & MILOG_FLOW)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MI_LOG_CFLOW2(a,b,c) {\
                     if (mi_debug_sel & MILOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define MI_LOG_CFLOW3(a,b,c,d) {\
                     if (mi_debug_sel & MILOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/*********************** MI ADDR Macros *******************************/

#define MI_LOG_ADDR0(a) {\
                     if (mi_debug_sel & MILOG_ADDR)\
                       _slog (sLogCtrl,_milog_addr_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MI_LOG_ADDR1(a,b) {\
                     if (mi_debug_sel & MILOG_ADDR)\
                       _slog (sLogCtrl,_milog_addr_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MI_LOG_ADDR2(a,b,c) {\
                     if (mi_debug_sel & MILOG_ADDR)\
                       _slog (sLogCtrl,_milog_addr_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }

#define MI_LOG_CADDR0(a) {\
                     if (mi_debug_sel & MILOG_ADDR)\
                       _slogc (sLogCtrl,a);\
                    }
#define MI_LOG_CADDR1(a,b) {\
                     if (mi_debug_sel & MILOG_ADDR)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MI_LOG_CADDR2(a,b,c) {\
                     if (mi_debug_sel & MILOG_ADDR)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define MI_LOG_CADDR3(a,b,c,d) {\
                     if (mi_debug_sel & MILOG_ADDR)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/*********************** MI CFG Macros *******************************/

#define MI_LOG_CFG0(a) {\
                     if (mi_debug_sel & MILOG_CFG)\
                       _slog (sLogCtrl,_milog_cfg_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MI_LOG_CFG1(a,b) {\
                     if (mi_debug_sel & MILOG_CFG)\
                       _slog (sLogCtrl,_milog_cfg_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MI_LOG_CFG2(a,b,c) {\
                     if (mi_debug_sel & MILOG_CFG)\
                       _slog (sLogCtrl,_milog_cfg_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }

#define MI_LOG_CCFG0(a) {\
                     if (mi_debug_sel & MILOG_CFG)\
                       _slogc (sLogCtrl,a);\
                    }
#define MI_LOG_CCFG1(a,b) {\
                     if (mi_debug_sel & MILOG_CFG)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MI_LOG_CCFG2(a,b,c) {\
                     if (mi_debug_sel & MILOG_CFG)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define MI_LOG_CCFG3(a,b,c,d) {\
                     if (mi_debug_sel & MILOG_CFG)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/*********************** MI DEBUG Macros *******************************/

#define MI_LOG_DEBUG0(a) {\
                     if (mi_debug_sel & MILOG_DEBUG)\
                       _slog (sLogCtrl,_milog_debug_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MI_LOG_DEBUG1(a,b) {\
                     if (mi_debug_sel & MILOG_DEBUG)\
                       _slog (sLogCtrl,_milog_debug_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MI_LOG_DEBUG2(a,b,c) {\
                     if (mi_debug_sel & MILOG_DEBUG)\
                       _slog (sLogCtrl,_milog_debug_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }

#define MI_LOG_CDEBUG0(a) {\
                     if (mi_debug_sel & MILOG_DEBUG)\
                       _slogc (sLogCtrl,a);\
                    }
#define MI_LOG_CDEBUG1(a,b) {\
                     if (mi_debug_sel & MILOG_DEBUG)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MI_LOG_CDEBUG2(a,b,c) {\
                     if (mi_debug_sel & MILOG_DEBUG)\
                       _slogc (sLogCtrl,a,b,c);\
                    }

/************************************************************************/
#else
/************************************************************************/

#define MI_LOG_ERR0(a)		;
#define MI_LOG_ERR1(a,b)	;
#define MI_LOG_ERR2(a,b,c)	;
#define MI_LOG_CERR2(a,b,c)	;
#define MI_LOG_NERR0(a)		;
#define MI_LOG_NERR1(a,b)	;
#define MI_LOG_NERR2(a,b,c)	;
#define MI_LOG_NERR3(a,b,c,d)	;
#define MI_LOG_CNERR0(a)     	;
#define MI_LOG_CNERR1(a,b)	;
#define MI_LOG_CNERR2(a,b,c)	;
#define MI_LOG_CNERR3(a,b,c,d)	;
#define MI_LOG_FLOW0(a)		;
#define MI_LOG_FLOW1(a,b)	;
#define MI_LOG_FLOW2(a,b,c)	;
#define MI_LOG_CFLOW0(a)	;
#define MI_LOG_CFLOW1(a,b)	;
#define MI_LOG_CFLOW2(a,b,c)	;
#define MI_LOG_CFLOW3(a,b,c,d)	;
#define MI_LOG_ADDR0(a)		;
#define MI_LOG_ADDR1(a,b)	;
#define MI_LOG_ADDR2(a,b,c)	;
#define MI_LOG_CADDR0(a)	;
#define MI_LOG_CADDR1(a,b)	;
#define MI_LOG_CADDR2(a,b,c)	;
#define MI_LOG_CADDR3(a,b,c,d)	;
#define MI_LOG_CFG0(a)		;
#define MI_LOG_CFG1(a,b)	;
#define MI_LOG_CFG2(a,b,c)	;
#define MI_LOG_CCFG0(a)	;
#define MI_LOG_CCFG1(a,b)	;
#define MI_LOG_CCFG2(a,b,c)	;
#define MI_LOG_CCFG3(a,b,c,d)	;
#define MI_LOG_DEBUG0(a)	;
#define MI_LOG_DEBUG1(a,b)	;
#define MI_LOG_DEBUG2(a,b,c)	;
#define MI_LOG_CDEBUG0(a)	;
#define MI_LOG_CDEBUG1(a,b)	;
#define MI_LOG_CDEBUG2(a,b,c)	;
					
#endif
/************************************************************************/


#ifdef __cplusplus
}
#endif

#endif

