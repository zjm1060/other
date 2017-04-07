/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	  1993 - 1994, All Rights Reserved.		        */
/*									*/
/*		    PROPRIETARY AND CONFIDENTIAL			*/
/*									*/
/* MODULE NAME : mmsldefs.h						*/
/* PRODUCT(S)  : MMSEASE-LITE						*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*	Functions and variables below this point are used by the	*/
/*	application.							*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 03/22/02  DWL     08    Updated build number				*/
/* 03/12/02  DWL     07    Updated version				*/
/* 02/25/02  MDE     06    Removed mmsl_max_msg_size			*/
/* 10/09/01  JRB     05    Add MMSLITE_* product identification defines.*/
/* 02/20/98  MDE     04    Fixed __cplusplus problem			*/
/* 11/05/97  MDE     03    Changed mmsl_msg_start to ST_UCHAR *		*/
/* 08/24/97  MDE     02    Added 'mmsl_max_msg_size', 'ml_init_buffers' */
/* 11/10/93  DGE     01    Initial MMSEASE-LITE Release.  		*/
/************************************************************************/

#ifndef LMMSDEFS_INCLUDED
#define LMMSDEFS_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/
/*		Defines for Product Identification.			*/
/* Write these parameters on screen and in log file at startup.		*/
/* Use MMSLITE_NAME & MMSLITE_VERSION in MMS Identify response.		*/
/************************************************************************/
#define MMSLITE_COPYRIGHT	"Copyright (c) 1986-2002 SISCO, Inc. All Rights Reserved."
#define MMSLITE_NAME		"MMS-LITE-80X-001"
#define MMSLITE_VERSION		"4.21"
#define MMSLITE_BUILD_NUM	4

/************************************************************************/
/************************************************************************/
/* MMSEASE-LITE specific Global Variables				*/
/************************************************************************/
#define MMS_PCI		0x0001

extern ST_INT    mmsl_enc_buf_size;
extern ST_UCHAR *mmsl_enc_buf;
extern ST_INT    mmsl_dec_info_size;

extern ST_INT16  mmsl_version;
extern ST_UINT32 mmsl_invoke_id;

extern ST_UCHAR  *mmsl_msg_start;
extern ST_INT    mmsl_msg_len;


/************************************************************************/
/* This function initializes the encode/decode buffers and the decode 	*/
/* object limitations, and is normally generated using the 'mbufcalc' 	*/
/* utility program.							*/

ST_VOID ml_init_buffers (ST_VOID);


#ifdef __cplusplus
}
#endif

#endif		/* MMSLDEFS_INCLUDED */


