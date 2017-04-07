/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	  1986 - 2001, All Rights Reserved.		        */
/*									*/
/* MODULE NAME : mms_err.h						*/
/* PRODUCT(S)  : MMSEASE, MMSEASE-LITE					*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*	This module contains structure & function definitions used for	*/
/*	decoding and encoding an error response.			*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 12/20/01  JRB     02	   Convert to use ASN1R.			*/
/* 08/14/97  RKR     01    Format changes				*/
/* 04/02/97  DTL   7.00    MMSEASE 7.0 release. See MODL70.DOC for	*/
/*			   history.					*/
/************************************************************************/

#ifndef MMS_ERR_INCLUDED
#define MMS_ERR_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "mms_mp.h"

/************************************************************************/
/* Additional Error Response Information -				*/
/*   This information is set whenever there is optional additional	*/
/*   error response information.					*/
/************************************************************************/

struct adtnl_err_resp_info
  {
  ST_BOOLEAN mod_pos_pres;/* Confirmed-ErrorPDU only: modifier pos pres	*/
  ST_INT32   mod_pos;	/* Confirmed-ErrorPDU only: modifier position   */
  ST_BOOLEAN info_pres;	/* global additional info present indicator	*/
  ST_BOOLEAN code_pres;	/* additional code present indicator		*/
  ST_INT32   code;	/* additional code				*/
  ST_BOOLEAN descr_pres;/* additional description present indicator	*/
  ST_CHAR    *descr; 	/* pointer to the additional description	*/
  ST_BOOLEAN ssi_pres;	/* service specific info present indicator	*/
  ST_INT16   service;	/* number indicating the service (0-10)		*/
  ST_UINT32  ss_error_val;	/* service specific error value, when service 	*/
			/*   is 0-7, or 9 (IS)				*/
  			/* service specific object name, when service	*/
			/*   is 8 (IS-Define Event Enrollment)		*/
  OBJECT_NAME ss_error_oname; 
  			/* service specific additional service error, 	*/
			/*   when service is 10 (IS-Companion Standard)	*/
  ST_INT ss_error_len;	/* service specific additional service length	*/
  ST_UCHAR *ss_error_data;/* service specific additional service data ptr	*/
  };
typedef struct adtnl_err_resp_info ADTNL_ERR_RESP_INFO;


/************************************************************************/
/* RECEIVED ERROR RESPONSE INFORMATION					*/
/************************************************************************/
/* The structure below is filled in when an error message is received,	*/
/* before the u_xxxx_conf function is invoked.				*/
/************************************************************************/

struct err_info
  {
  ST_INT16            eclass; 	/* error class			*/
  ST_INT16  	      code;   	/* error code			*/
  ADTNL_ERR_RESP_INFO adtnl;	/* additional error information */
  };
typedef struct err_info	ERR_INFO;

/************************************************************************/
/* Error Response Decode Function -					*/
/*   This routine is called to decode an error response PDU.		*/
/************************************************************************/

ST_VOID mms_err_rsp (ASN1_DEC_CTXT *aCtx);

/************************************************************************/
/* SEND ERROR RESPONSE INFORMATION					*/
/************************************************************************/
/* The structure below is filled in when an error response is to be	*/
/* sent that includes the optional additional error information.	*/
/*   Note : this information must be set prior to issuing the call to	*/
/*	    mp_err_resp, if it is to be included in the response.	*/
/************************************************************************/

extern ADTNL_ERR_RESP_INFO  adtnl_err_info;

/************************************************************************/
/* Error Response Encode & Send Functions -				*/
/*   These routines are called to build and send error response PDU's,  */
/*   one for the case of a Cancel indication, the other for all other	*/
/*   cases. The parameters are : a pointer to the request information,	*/
/*   the error class, and the error code.				*/
/************************************************************************/

#ifndef MMS_LITE
ST_RET  mp_err_resp     (MMSREQ_IND *ind, ST_INT16 err_class, ST_INT16 code);
ST_RET  mp_init_err     (MMSREQ_IND *ind, ST_INT16 err_class, ST_INT16 code);
ST_RET  mp_conclude_err (MMSREQ_IND *ind, ST_INT16 err_class, ST_INT16 code); 
ST_RET  mp_cancel_err   (MMSREQ_IND *ind, ST_INT16 err_class, ST_INT16 code);
#else
ST_RET  mpl_err_resp     (ST_INT op, ST_UINT32 invoke, ST_INT16 err_class, ST_INT16 code);
ST_RET  mpl_init_err     (ST_INT16 err_class, ST_INT16 code); 
ST_RET  mpl_conclude_err (ST_INT16 err_class, ST_INT16  code); 
ST_RET  mpl_cancel_err   (ST_UINT32 invoke, ST_INT16 err_class, ST_INT16 code);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* MMS_ERR_INCLUDED */
