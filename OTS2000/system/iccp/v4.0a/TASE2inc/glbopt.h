#ifndef GLBOPT_INCLUDED
#define GLBOPT_INCLUDED
/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*	(c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*	2002-2002 All Rights Reserved					*/
/*									*/
/* MODULE NAME : glbopt.h						*/
/* PRODUCT(S)  : MMSEASE-LITE						*/
/*									*/
/* MODULE DESCRIPTION :							*/
/*	Global compiler options to be used by all SISCO source modules.	*/
/*	This file may be used to enable optional features in		*/
/*	MMSEASE-LITE. Usually this is easier than adding an		*/
/*	option to several makefiles. If this file is modified, all	*/
/*	source code should be recompiled.				*/
/*									*/
/*	This file must be included by "glbtypes.h" so that all SISCO	*/
/*	source modules will indirectly include it.			*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev     Comments					*/
/* --------  ---  ------   -------------------------------------------  */
/* 02/27/02  JRB     01    Created					*/
/************************************************************************/

#if !defined (MMS_LITE)
#define MMS_LITE
#endif

/*
 * NOTE: Insert additional compiler options here.
 */

#endif	/* !GLBOPT_INCLUDED	*/

