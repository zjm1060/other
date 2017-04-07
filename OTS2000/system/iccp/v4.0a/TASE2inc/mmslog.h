/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	  1993 - 2001, All Rights Reserved.		        */
/*									*/
/*		    PROPRIETARY AND CONFIDENTIAL			*/
/*									*/
/* MODULE NAME : mmslog.h						*/
/* PRODUCT(S)  : MMSEASE						*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*	This module contains MMS-EASE logging macros used 		*/
/*	internally by MMS-EASE for SLOGGING				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 03/13/02  JRB     05    MLOG_PAUSE* macros do nothing.		*/
/* 10/05/01  EJV     04    Corrected comment.				*/
/* 03/15/01  JRB     03    Added USER log macros.			*/
/* 09/13/99  MDE     02    Added SD_CONST modifiers			*/
/* 10/08/98  MDE     01    Migrated to updated SLOG interface		*/
/* 04/02/97  DTL   7.00    MMSEASE 7.0 release. See MODL70.DOC for	*/
/*			   history.					*/
/************************************************************************/

#ifndef MMSLOG_INCLUDED
#define MMSLOG_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

extern SD_CONST ST_CHAR *SD_CONST _mms_log_dec_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mms_log_enc_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mms_log_acse_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mms_log_llc_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mms_log_ique_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mms_log_rque_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mms_log_ind_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mms_log_conf_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mms_log_vm_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mms_log_err_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mms_log_nerr_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mms_log_pdu_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mms_log_config_logstr;
extern SD_CONST ST_CHAR *SD_CONST _mms_log_always_logstr;

extern SD_CONST ST_CHAR *SD_CONST _user_err_logstr;
extern SD_CONST ST_CHAR *SD_CONST _user_client_logstr;
extern SD_CONST ST_CHAR *SD_CONST _user_server_logstr;

/************************************************************************/
#if defined (DEBUG_SISCO)
/************************************************************************/

/*********************** PAUSE Macros ***********************************/
#define MLOG_PAUSEDEC(msg)
#define MLOG_PAUSEIQUE(msg)
#define MLOG_PAUSERQUE(msg)
#define MLOG_PAUSEPDU(msg)

/*********************** DECODE Macros **********************************/
#define MLOG_DEC0(a)  {\
                     if (mms_debug_sel & MMS_LOG_DEC)\
                       _slog (sLogCtrl,_mms_log_dec_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MLOG_DEC1(a,b) {\
                     if (mms_debug_sel & MMS_LOG_DEC)\
                       _slog (sLogCtrl,_mms_log_dec_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MLOG_DEC2(a,b,c) {\
                     if (mms_debug_sel & MMS_LOG_DEC)\
                       _slog (sLogCtrl,_mms_log_dec_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define MLOG_CDEC0(a) {\
                     if (mms_debug_sel & MMS_LOG_DEC)\
                       _slogc (sLogCtrl,a);\
                    }
#define MLOG_CDEC1(a,b) {\
                     if (mms_debug_sel & MMS_LOG_DEC)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MLOG_CDEC2(a,b,c) {\
                     if (mms_debug_sel & MMS_LOG_DEC)\
                       _slogc (sLogCtrl,a,b,c);\
                    }

/*********************** ENCODE Macros **********************************/
#define MLOG_ENC0(a) {\
                     if (mms_debug_sel & MMS_LOG_ENC)\
                       _slog (sLogCtrl,_mms_log_enc_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MLOG_ENC1(a,b) {\
                     if (mms_debug_sel & MMS_LOG_ENC)\
                       _slog (sLogCtrl,_mms_log_enc_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MLOG_ENC2(a,b,c) {\
                     if (mms_debug_sel & MMS_LOG_ENC)\
                       _slog (sLogCtrl,_mms_log_enc_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define MLOG_CENC1(a,b) {\
                     if (mms_debug_sel & MMS_LOG_ENC)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MLOG_ENCH(a,b) {\
                     if (mms_debug_sel & MMS_LOG_ENC)\
                       _slogHex (sLogCtrl,a,b);\
                    }

#define MLOG_PAUSEENC(msg)

/*********************** ACSE Macros ***********************************/
#define MLOG_ACSE0(a) {\
                     if (mms_debug_sel & MMS_LOG_ACSE)\
                       _slog (sLogCtrl,_mms_log_acse_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MLOG_ACSE1(a,b) {\
                     if (mms_debug_sel & MMS_LOG_ACSE)\
                       _slog (sLogCtrl,_mms_log_acse_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MLOG_ACSE2(a,b,c) {\
                     if (mms_debug_sel & MMS_LOG_ACSE)\
                       _slog (sLogCtrl,_mms_log_acse_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define MLOG_CACSE0(a) {\
                     if (mms_debug_sel & MMS_LOG_ACSE)\
                       _slogc (sLogCtrl,a);\
                    }
#define MLOG_CACSE1(a,b) {\
                     if (mms_debug_sel & MMS_LOG_ACSE)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MLOG_CACSE2(a,b,c) {\
                     if (mms_debug_sel & MMS_LOG_ACSE)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define MLOG_CACSE3(a,b,c,d) {\
                     if (mms_debug_sel & MMS_LOG_ACSE)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }
#define MLOG_ACSEH(a,b) {\
                     if (mms_debug_sel & MMS_LOG_ACSE)\
                       _slogHex (sLogCtrl,a,b);\
                    }


/*********************** LLC Macros **************************************/
#define MLOG_LLC0(a) {\
                     if (mms_debug_sel & MMS_LOG_LLC)\
                       _slog (sLogCtrl,_mms_log_llc_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MLOG_LLC1(a,b) {\
                     if (mms_debug_sel & MMS_LOG_LLC)\
                       _slog (sLogCtrl,_mms_log_llc_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MLOG_LLC2(a,b,c) {\
                     if (mms_debug_sel & MMS_LOG_LLC)\
                       _slog (sLogCtrl,_mms_log_llc_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define MLOG_LLC3(a,b,c,d) {\
                     if (mms_debug_sel & MMS_LOG_LLC)\
                       _slog (sLogCtrl,_mms_log_llc_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }

#define MLOG_CLLC0(a) {\
                     if (mms_debug_sel & MMS_LOG_LLC)\
                       _slogc (sLogCtrl,a);\
                    }
#define MLOG_CLLC1(a,b) {\
                     if (mms_debug_sel & MMS_LOG_LLC)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MLOG_CLLC2(a,b,c) {\
                     if (mms_debug_sel & MMS_LOG_LLC)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define MLOG_CLLC3(a,b,c,d) {\
                     if (mms_debug_sel & MMS_LOG_LLC)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

#define MLOG_LLCH(a,b) {\
                     if (mms_debug_sel & MMS_LOG_LLC)\
                       _slogHex (sLogCtrl,a,b);\
                    }

/*********************** QUE Macros **************************************/
#define MLOG_IQUE0(a) {\
                     if (mms_debug_sel & MMS_LOG_IQUE)\
                       _slog (sLogCtrl,_mms_log_ique_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MLOG_CIQUE0(a) {\
                     if (mms_debug_sel & MMS_LOG_IQUE)\
                       _slogc (sLogCtrl,a);\
                    }
#define MLOG_CIQUE1(a,b) {\
                     if (mms_debug_sel & MMS_LOG_IQUE)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MLOG_CIQUE4(a,b,c,d,e) {\
                     if (mms_debug_sel & MMS_LOG_IQUE)\
                       _slogc (sLogCtrl,a,b,c,d,e);\
                    }
#define MLOG_RQUE0(a) {\
                     if (mms_debug_sel & MMS_LOG_RQUE)\
                       _slog (sLogCtrl,_mms_log_rque_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MLOG_CRQUE0(a) {\
                     if (mms_debug_sel & MMS_LOG_RQUE)\
                       _slogc (sLogCtrl,a);\
                    }
#define MLOG_CRQUE1(a,b) {\
                     if (mms_debug_sel & MMS_LOG_RQUE)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MLOG_CRQUE4(a,b,c,d,e) {\
                     if (mms_debug_sel & MMS_LOG_RQUE)\
                       _slogc (sLogCtrl,a,b,c,d,e);\
                    }

/*********************** REQ/IND/RESP/CONF Macros **************************/

#define MLOG_IND0(a) {\
                     if (mms_debug_sel & MMS_LOG_IND)\
                       _slog (sLogCtrl,_mms_log_ind_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MLOG_INDH(a,b) {\
                     if (mms_debug_sel & MMS_LOG_IND)\
                       _slogHex (sLogCtrl,a,b);\
                    }
#define MLOG_CONF0(a) {\
                     if (mms_debug_sel & MMS_LOG_CONF)\
                       _slog (sLogCtrl,_mms_log_conf_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MLOG_CONFH(a,b) {\
                     if (mms_debug_sel & MMS_LOG_CONF)\
                       _slogHex (sLogCtrl,a,b);\
                    }
#define MLOG_REQ0(a) {\
                     if (mms_debug_sel & MMS_LOG_REQ)\
                       _slog (sLogCtrl,_mms_log_req_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MLOG_REQH(a,b) {\
                     if (mms_debug_sel & MMS_LOG_REQ)\
                       _slogHex (sLogCtrl,a,b);\
                    }
#define MLOG_RESP0(a) {\
                     if (mms_debug_sel & MMS_LOG_RESP)\
                       _slog (sLogCtrl,_mms_log_resp_logstr,\
		             thisFileName,__LINE__,a);\
                    }

#define MLOG_RESPH(a,b) {\
                     if (mms_debug_sel & MMS_LOG_RESP)\
                       _slogHex (sLogCtrl,a,b);\
                    }

/*********************** VM Macros **************************************/
#define MLOG_VM0(a) {\
                     if (mms_debug_sel & MMS_LOG_VM)\
                       _slog (sLogCtrl,_mms_log_vm_logstr,\
		             thisFileName,__LINE__,a);\
                    }

/*********************** ERR Macros **************************************/
#define MLOG_ERR0(a) {\
                     if (mms_debug_sel & MMS_LOG_ERR)\
                       _slog (sLogCtrl,_mms_log_err_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MLOG_ERR1(a,b) {\
                     if (mms_debug_sel & MMS_LOG_ERR)\
                       _slog (sLogCtrl,_mms_log_err_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MLOG_ERR2(a,b,c) {\
                     if (mms_debug_sel & MMS_LOG_ERR)\
                       _slog (sLogCtrl,_mms_log_err_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define MLOG_ERR3(a,b,c,d) {\
                     if (mms_debug_sel & MMS_LOG_ERR)\
                       _slog (sLogCtrl,_mms_log_err_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }
#define MLOG_ERR4(a,b,c,d,e) {\
                     if (mms_debug_sel & MMS_LOG_ERR)\
                       _slog (sLogCtrl,_mms_log_err_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e);\
                    }

/*********************** NERR Macros *************************************/
#define MLOG_NERR0(a) {\
                     if (mms_debug_sel & MMS_LOG_NERR)\
                       _slog (sLogCtrl,_mms_log_nerr_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MLOG_NERR1(a,b) {\
                     if (mms_debug_sel & MMS_LOG_NERR)\
                       _slog (sLogCtrl,_mms_log_nerr_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MLOG_NERR2(a,b,c) {\
                     if (mms_debug_sel & MMS_LOG_NERR)\
                       _slog (sLogCtrl,_mms_log_nerr_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define MLOG_CNERR3(a,b,c,d) {\
                     if (mms_debug_sel & MMS_LOG_NERR)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/*********************** PDU Macros **************************************/
#define MLOG_PDU4(a,b,c,d,e) {\
                     if (mms_debug_sel & MMS_LOG_PDU)\
                       _slog (sLogCtrl,_mms_log_pdu_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e);\
                    }
#define MLOG_PDUH(a,b) {\
                     if (mms_debug_sel & MMS_LOG_PDU)\
                       _slogHex (sLogCtrl,a,b);\
                    }


/*********************** ALWAYS  Macros *********************************/
						 
#define MLOG_ALWAYS0(a) {\
                       _slog (sLogCtrl,_mms_log_always_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define MLOG_ALWAYS1(a,b) {\
                       _slog (sLogCtrl,_mms_log_always_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define MLOG_ALWAYS2(a,b,c) {\
                       _slog (sLogCtrl,_mms_log_always_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define MLOG_ALWAYS3(a,b,c,d) {\
                       _slog (sLogCtrl,_mms_log_always_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }
#define MLOG_CALWAYS0(a) {\
                       _slogc (sLogCtrl,a);\
                    }
#define MLOG_CALWAYS1(a,b) {\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MLOG_CALWAYS2(a,b,c) {\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define MLOG_CALWAYS3(a,b,c,d) {\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }
#define MLOG_CALWAYS4(a,b,c,d,e) {\
                       _slogc (sLogCtrl,a,b,c,d,e);\
                    }
#define MLOG_CALWAYS5(a,b,c,d,e,f) {\
                       _slogc (sLogCtrl,a,b,c,d,e,f);\
                    }

#define MLOG_ALWAYSH(a,b) {\
                       _slogHex (sLogCtrl,a,b);\
                    }

/*********************** USER ERR Macros********************************/

#define USER_LOG_ERR0(a) {\
                     if (user_debug_sel & USER_LOG_ERR)\
                       _slog (sLogCtrl,_user_err_logstr,\
		             thisFileName,__LINE__,a);\
                    }

#define USER_LOG_ERR1(a,b) {\
                     if (user_debug_sel & USER_LOG_ERR)\
                       _slog (sLogCtrl,_user_err_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }

#define USER_LOG_ERR2(a,b,c) {\
                     if (user_debug_sel & USER_LOG_ERR)\
                       _slog (sLogCtrl,_user_err_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }

#define USER_LOG_ERR3(a,b,c,d) {\
                     if (user_debug_sel & USER_LOG_ERR)\
                       _slog (sLogCtrl,_user_err_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }

#define USER_LOG_ERR4(a,b,c,d,e) {\
                     if (user_debug_sel & USER_LOG_ERR)\
                       _slog (sLogCtrl,_user_err_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e);\
                    }

#define USER_LOG_ERR5(a,b,c,d,e,f) {\
                     if (user_debug_sel & USER_LOG_ERR)\
                       _slog (sLogCtrl,_user_err_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e,f);\
                    }

#define USER_LOG_ERR6(a,b,c,d,e,f,g) {\
                     if (user_debug_sel & USER_LOG_ERR)\
                       _slog (sLogCtrl,_user_err_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e,f,g);\
                    }

#define USER_LOG_ERR7(a,b,c,d,e,f,g,h) {\
                     if (user_debug_sel & USER_LOG_ERR)\
                       _slog (sLogCtrl,_user_err_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                    }

/*********************** USER Client Macros ****************************/

#define USER_LOG_CLIENT0(a) {\
                     if (user_debug_sel & USER_LOG_CLIENT)\
                       _slog (sLogCtrl,_user_client_logstr,\
		             thisFileName,__LINE__,a);\
                    }

#define USER_LOG_CLIENT1(a,b) {\
                     if (user_debug_sel & USER_LOG_CLIENT)\
                       _slog (sLogCtrl,_user_client_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }

#define USER_LOG_CLIENT2(a,b,c) {\
                     if (user_debug_sel & USER_LOG_CLIENT)\
                       _slog (sLogCtrl,_user_client_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }

#define USER_LOG_CLIENT3(a,b,c,d) {\
                     if (user_debug_sel & USER_LOG_CLIENT)\
                       _slog (sLogCtrl,_user_client_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }

#define USER_LOG_CLIENT4(a,b,c,d,e) {\
                     if (user_debug_sel & USER_LOG_CLIENT)\
                       _slog (sLogCtrl,_user_client_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e);\
                    }

#define USER_LOG_CLIENT5(a,b,c,d,e,f) {\
                     if (user_debug_sel & USER_LOG_CLIENT)\
                       _slog (sLogCtrl,_user_client_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e,f);\
                    }

#define USER_LOG_CLIENT6(a,b,c,d,e,f,g) {\
                     if (user_debug_sel & USER_LOG_CLIENT)\
                       _slog (sLogCtrl,_user_client_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e,f,g);\
                    }

#define USER_LOG_CLIENT7(a,b,c,d,e,f,g,h) {\
                     if (user_debug_sel & USER_LOG_CLIENT)\
                       _slog (sLogCtrl,_user_client_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                    }


#define USER_LOG_CLIENT_HEX(a,b)      {\
				   if (user_debug_sel & USER_LOG_CLIENT)\
				     _slogHex (sLogCtrl,a,b);\
			           }			

/*********************** USER Server Macros ****************************/

#define USER_LOG_SERVER0(a) {\
                     if (user_debug_sel & USER_LOG_SERVER)\
                       _slog (sLogCtrl,_user_server_logstr,\
		             thisFileName,__LINE__,a);\
                    }

#define USER_LOG_SERVER1(a,b) {\
                     if (user_debug_sel & USER_LOG_SERVER)\
                       _slog (sLogCtrl,_user_server_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }

#define USER_LOG_SERVER2(a,b,c) {\
                     if (user_debug_sel & USER_LOG_SERVER)\
                       _slog (sLogCtrl,_user_server_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }

#define USER_LOG_SERVER3(a,b,c,d) {\
                     if (user_debug_sel & USER_LOG_SERVER)\
                       _slog (sLogCtrl,_user_server_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }

#define USER_LOG_SERVER4(a,b,c,d,e) {\
                     if (user_debug_sel & USER_LOG_SERVER)\
                       _slog (sLogCtrl,_user_server_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e);\
                    }

#define USER_LOG_SERVER5(a,b,c,d,e,f) {\
                     if (user_debug_sel & USER_LOG_SERVER)\
                       _slog (sLogCtrl,_user_server_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e,f);\
                    }

#define USER_LOG_SERVER6(a,b,c,d,e,f,g) {\
                     if (user_debug_sel & USER_LOG_SERVER)\
                       _slog (sLogCtrl,_user_server_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e,f,g);\
                    }

#define USER_LOG_SERVER7(a,b,c,d,e,f,g,h) {\
                     if (user_debug_sel & USER_LOG_SERVER)\
                       _slog (sLogCtrl,_user_server_logstr,\
		             thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                    }


#define USER_LOG_SERVER_HEX(a,b)      {\
				   if (user_debug_sel & USER_LOG_SERVER)\
				     _slogHex (sLogCtrl,a,b);\
			           }			

/************************************************************************/
#else
/************************************************************************/


#define MLOG_PAUSEDEC(msg)
#define MLOG_PAUSEIQUE(msg)
#define MLOG_PAUSERQUE(msg)
#define MLOG_PAUSEPDU(msg)
#define MLOG_DEC0(a)
#define MLOG_DEC1(a,b)
#define MLOG_DEC2(a,b,c)
#define MLOG_CDEC0(a)
#define MLOG_CDEC1(a,b)
#define MLOG_CDEC2(a,b,c)
#define MLOG_ENC0(a)
#define MLOG_ENC1(a,b)
#define MLOG_ENC2(a,b,c)
#define MLOG_CENC1(a,b)
#define MLOG_ENCH(a,b)
#define MLOG_PAUSEENC(msg)
#define MLOG_ACSE0(a)
#define MLOG_ACSE1(a,b)
#define MLOG_ACSE2(a,b,c)
#define MLOG_CACSE0(a)
#define MLOG_CACSE1(a,b)
#define MLOG_CACSE2(a,b,c)
#define MLOG_CACSE3(a,b,c,d)
#define MLOG_ACSEH(a,b)
#define MLOG_LLC0(a)
#define MLOG_LLC1(a,b)
#define MLOG_LLC2(a,b,c)
#define MLOG_LLC3(a,b,c,d)
#define MLOG_LLCH(a,b)
#define MLOG_CLLC0(a);
#define MLOG_CLLC1(a,b);
#define MLOG_CLLC2(a,b,c);
#define MLOG_CLLC3(a,b,c,d);
#define MLOG_IQUE0(a)
#define MLOG_CIQUE0(a)
#define MLOG_CIQUE1(a,b)
#define MLOG_CIQUE4(a,b,c,d,e)
#define MLOG_RQUE0(a)
#define MLOG_CRQUE0(a)
#define MLOG_CRQUE1(a,b)
#define MLOG_CRQUE4(a,b,c,d,e)
#define MLOG_IND0(a)
#define MLOG_INDH(a,b)
#define MLOG_CONF0(a)
#define MLOG_CONFH(a,b)
#define MLOG_REQ0(a)
#define MLOG_REQH(a,b)
#define MLOG_RESP0(a)
#define MLOG_RESPH(a,b)
#define MLOG_VM0(a)
#define MLOG_ERR0(a)
#define MLOG_ERR1(a,b)
#define MLOG_ERR2(a,b,c)
#define MLOG_ERR3(a,b,c,d)
#define MLOG_ERR4(a,b,c,d,e)
#define MLOG_NERR0(a)
#define MLOG_NERR1(a,b)
#define MLOG_NERR2(a,b,c)
#define MLOG_CNERR3(a,b,c,d)
#define MLOG_PDU4(a,b,c,d,e)
#define MLOG_PDUH(a,b)
#define MLOG_ALWAYS0(a)
#define MLOG_ALWAYS1(a,b)
#define MLOG_ALWAYS2(a,b,c)
#define MLOG_CALWAYS0(a)
#define MLOG_CALWAYS1(a,b)
#define MLOG_CALWAYS2(a,b,c)
#define MLOG_CALWAYS3(a,b,c,d)
#define MLOG_CALWAYS4(a,b,c,d,e)
#define MLOG_CALWAYS5(a,b,c,d,e,f)
#define MLOG_ALWAYSH(a,b)

#define USER_LOG_ERR0(a)
#define USER_LOG_ERR1(a,b)
#define USER_LOG_ERR2(a,b,c)
#define USER_LOG_ERR3(a,b,c,d)
#define USER_LOG_ERR4(a,b,c,d,e)
#define USER_LOG_ERR5(a,b,c,d,e,f)
#define USER_LOG_ERR6(a,b,c,d,e,f,g)
#define USER_LOG_ERR7(a,b,c,d,e,f,g,h)

#define USER_LOG_CLIENT0(a)
#define USER_LOG_CLIENT1(a,b)
#define USER_LOG_CLIENT2(a,b,c)
#define USER_LOG_CLIENT3(a,b,c,d)
#define USER_LOG_CLIENT4(a,b,c,d,e)
#define USER_LOG_CLIENT5(a,b,c,d,e,f)
#define USER_LOG_CLIENT6(a,b,c,d,e,f,g)
#define USER_LOG_CLIENT7(a,b,c,d,e,f,g,h)

#define USER_LOG_CLIENT_HEX(a,b)      

					
#define USER_LOG_SERVER0(a)
#define USER_LOG_SERVER1(a,b)
#define USER_LOG_SERVER2(a,b,c)
#define USER_LOG_SERVER3(a,b,c,d)
#define USER_LOG_SERVER4(a,b,c,d,e)
#define USER_LOG_SERVER5(a,b,c,d,e,f)
#define USER_LOG_SERVER6(a,b,c,d,e,f,g)
#define USER_LOG_SERVER7(a,b,c,d,e,f,g,h)

#define USER_LOG_SERVER_HEX(a,b)      
/************************************************************************/
#endif		/* End of DEBUG_SISCO					*/
/************************************************************************/

#ifdef __cplusplus
}
#endif

/************************************************************************/
#endif		/* End of MMS_LOG_INCLUDED				*/
/************************************************************************/
