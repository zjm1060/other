/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	  1999-2002, All Rights Reserved.                           */
/*									*/
/* MODULE NAME : mit_log.h 						*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  rkr    6      Copyright year update                        */
/* 06/21/01  rkr    5      Fixed some macros				*/
/* 12/18/00  rkr    4      Added MIT_LOG_ALWAYS                         */
/* 10/19/00  rkr    3      Added MIT_LOG_CFLOW4(a,b,c,d,e)		*/
/* 08/15/00  rkr    2      Added MIT_LOG_CNF1				*/
/* 04/24/99  rkr    1      Created                                      */
/************************************************************************/

#ifndef MIT_LOG_INCLUDED
#define MIT_LOG_INCLUDED

#include "slog.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/
/* MMS-EASE Lite ICCP TASE.2 Extension Demo logging control 						*/

#define MITLOG_ERR		0x00000001
#define MIT_LOG_NERR		0x00000002
#define MITLOG_FLOW		0x00000004
#define MITLOG_CFG		0x00000008
#define MITLOG_IND		0x00000010
#define MITLOG_CNF		0x00000020

extern ST_UINT mit_debug_sel;

/************************************************************************/
#ifdef DEBUG_SISCO
/************************************************************************/

/* Log type strings */
extern ST_CHAR *_mitlog_err_logstr;
extern ST_CHAR *_MIT_LOG_NERR_logstr;
extern ST_CHAR *_mitlog_flow_logstr;
extern ST_CHAR *_mitlog_cfg_logstr;
extern ST_CHAR *_mitlog_ind_logstr;
extern ST_CHAR *_mitlog_cnf_logstr;
extern ST_CHAR *_mitlog_always_logstr;

/*********************** ERR Macros *************************************/

#define MIT_LOG_ERR0(a) {\
                     if (mit_debug_sel & MITLOG_ERR)\
                       _slog (sLogCtrl,_mitlog_err_logstr,\
		             thisFileName,__LINE__,a);\
                    }

#define MIT_LOG_ERR1(a,b) {\
                     if (mit_debug_sel & MITLOG_ERR)\
                       _slog (sLogCtrl,_mitlog_err_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }

#define MIT_LOG_ERR2(a,b,c) {\
                     if (mit_debug_sel & MITLOG_ERR)\
                       _slog (sLogCtrl,_mitlog_err_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }

#define MIT_LOG_CERR2(a,b,c) {\
                     if (mit_debug_sel & MITLOG_ERR)\
                       _slogc (sLogCtrl,a,b,c);\
                    }

/*********************** NERR Macros *************************************/

#define MIT_LOG_NERR0(a) {\
                     if (mit_debug_sel & MIT_LOG_NERR)\
                       _slog (sLogCtrl,_MIT_LOG_NERR_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MIT_LOG_NERR1(a,b) {\
                     if (mit_debug_sel & MIT_LOG_NERR)\
                       _slog (sLogCtrl,_MIT_LOG_NERR_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MIT_LOG_NERR2(a,b,c) {\
                     if (mit_debug_sel & MIT_LOG_NERR)\
                       _slog (sLogCtrl,_MIT_LOG_NERR_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }

/*********************** MIT FLOW Macros *******************************/

#define MIT_LOG_FLOW0(a) {\
                     if (mit_debug_sel & MITLOG_FLOW)\
                       _slog (sLogCtrl,_mitlog_flow_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MIT_LOG_FLOW1(a,b) {\
                     if (mit_debug_sel & MITLOG_FLOW)\
                       _slog (sLogCtrl,_mitlog_flow_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MIT_LOG_FLOW2(a,b,c) {\
                     if (mit_debug_sel & MITLOG_FLOW)\
                       _slog (sLogCtrl,_mitlog_flow_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }

#define MIT_LOG_CFLOW0(a) {\
                     if (mit_debug_sel & MITLOG_FLOW)\
                       _slogc (sLogCtrl,a);\
                    }
#define MIT_LOG_CFLOW1(a,b) {\
                     if (mit_debug_sel & MITLOG_FLOW)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MIT_LOG_CFLOW2(a,b,c) {\
                     if (mit_debug_sel & MITLOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define MIT_LOG_CFLOW4(a,b,c,d,e) {\
                     if (mit_debug_sel & MITLOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c,d,e);\
                    }

/*********************** MIT DEBUG Macros *******************************/

#define MIT_LOG_DEBUG0(a) {\
                     if (mit_debug_sel & MITLOG_DEBUG)\
                       _slog (sLogCtrl,_mitlog_debug_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MIT_LOG_DEBUG1(a,b) {\
                     if (mit_debug_sel & MITLOG_DEBUG)\
                       _slog (sLogCtrl,_mitlog_debug_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MIT_LOG_DEBUG2(a,b,c) {\
                     if (mit_debug_sel & MITLOG_DEBUG)\
                       _slog (sLogCtrl,_mitlog_debug_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }

#define MIT_LOG_CDEBUG0(a) {\
                     if (mit_debug_sel & MITLOG_DEBUG)\
                       _slogc (sLogCtrl,a);\
                    }
#define MIT_LOG_CDEBUG1(a,b) {\
                     if (mit_debug_sel & MITLOG_DEBUG)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MIT_LOG_CDEBUG2(a,b,c) {\
                     if (mit_debug_sel & MITLOG_DEBUG)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
/*********************** MIT CFG Macros *******************************/

#define MIT_LOG_CCFG0(a) {\
                     if (mit_debug_sel & MITLOG_CFG)\
                       _slogc (sLogCtrl,a);\
                    }

#define MIT_LOG_CCFG4(a,b,c,d,e) {\
                     if (mit_debug_sel & MITLOG_CFG)\
                       _slogc (sLogCtrl,a,b,c,d,e);\
                    }

/*********************** MIT INDICATION Macros **************************/

#define MIT_LOG_IND0(a) {\
                     if (mit_debug_sel & MITLOG_IND)\
                       _slog (sLogCtrl,_mitlog_ind_logstr,\
		             thisFileName,__LINE__,a);\
                    }

#define MIT_LOG_CIND0(a) {\
                     if (mit_debug_sel & MITLOG_IND)\
                       _slogc (sLogCtrl,a);\
                    }

#define MIT_LOG_IND1(a,b) {\
                     if (mit_debug_sel & MITLOG_IND)\
                       _slog (sLogCtrl,_mitlog_ind_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }

#define MIT_LOG_CIND1(a,b) {\
                     if (mit_debug_sel & MITLOG_IND)\
                       _slogc (sLogCtrl,a,b);\
                    }

#define MIT_LOG_CIND2(a,b,c) {\
                     if (mit_debug_sel & MITLOG_IND)\
                       _slogc (sLogCtrl,a,b,c);\
                    }

#define MIT_LOG_CIND3(a,b,c,d) {\
                     if (mit_debug_sel & MITLOG_IND)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }
#define MIT_LOG_INDHEX(a,b) {\
                     if (mit_debug_sel & MITLOG_IND)\
		       _slogHex (sLogCtrl,a,b);\
                    }

/*********************** MIT CONFIRMATION Macros ************************/

#define MIT_LOG_CNF0(a) {\
                     if (mit_debug_sel & MITLOG_CNF)\
                       _slog (sLogCtrl,_mitlog_cnf_logstr,\
		             thisFileName,__LINE__,a);\
                    }

#define MIT_LOG_CNF1(a,b) {\
                     if (mit_debug_sel & MITLOG_CNF)\
                       _slog (sLogCtrl,_mitlog_cnf_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MIT_LOG_CCNF2(a,b,c) {\
                     if (mit_debug_sel & MITLOG_CNF)\
                       _slogc (sLogCtrl,a,b,c);\
                    }

/*********************** MIT SLOG ALWAYS Macros *************************/

#define MIT_LOG_CALWAYS2(a,b,c) {\
                       _slogc (sLogCtrl,a,b,c);\
                    }

#define MIT_LOG_ALWAYS1(a,b) {\
                       _slog (sLogCtrl,_mitlog_always_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }


/************************************************************************/
#else
/************************************************************************/

#define MIT_LOG_ERR0(a)		;
#define MIT_LOG_ERR1(a,b)	;
#define MIT_LOG_ERR2(a,b,c)	;
#define MIT_LOG_CERR2(a,b,c)	;
#define MIT_LOG_NERR0(a)	;
#define MIT_LOG_NERR1(a,b)	;
#define MIT_LOG_NERR2(a,b,c)	;
#define MIT_LOG_FLOW0(a)	;
#define MIT_LOG_FLOW1(a,b)	;
#define MIT_LOG_FLOW2(a,b,c)	;
#define MIT_LOG_CFLOW0(a)	;
#define MIT_LOG_CFLOW1(a,b)	;
#define MIT_LOG_CFLOW2(a,b,c)	;
#define MIT_LOG_CFLOW4(a,b,c,d,e) ;
#define MIT_LOG_DEBUG0(a)	;
#define MIT_LOG_DEBUG1(a,b)	;
#define MIT_LOG_DEBUG2(a,b,c)	;
#define MIT_LOG_CDEBUG0(a)	;
#define MIT_LOG_CDEBUG1(a,b)	;
#define MIT_LOG_CDEBUG2(a,b,c)	;
#define MIT_LOG_CCFG0(a) 	;
#define MIT_LOG_CCFG4(a,b,c,d,e);
#define MIT_LOG_IND0(a) 	;
#define MIT_LOG_CIND0(a) 	;
#define MIT_LOG_IND1(a,b) 	;
#define MIT_LOG_CIND1(a,b) 	;
#define MIT_LOG_CIND2(a,b,c) 	;
#define MIT_LOG_CIND3(a,b,c,d) 	;
#define MIT_LOG_INDHEX(a,b)     ;
#define MIT_LOG_CNF0(a) 	;
#define MIT_LOG_CNF1(a,b) 	;
#define MIT_LOG_CCNF2(a,b,c) 	;
#define MIT_LOG_ALWAYS1(a,b)    ;
#define MIT_LOG_CALWAYS2(a,b,c) ;
					
#endif
/************************************************************************/


#ifdef __cplusplus
}
#endif

#endif

