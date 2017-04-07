#ifndef STR_UTIL_INCLUDED
#define STR_UTIL_INCLUDED
/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*	(c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*	2000 - 2002 All Rights Reserved					*/
/*									*/
/* MODULE NAME : str_util.h						*/
/* PRODUCT(S)  : General Use						*/
/*									*/
/* MODULE DESCRIPTION :							*/
/*	General purpose string manipulation functions.			*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev     Comments					*/
/* --------  ---  ------   -------------------------------------------  */
/* 02/28/02  EJV    11     Added asciiToFloat, asciiToDouble.		*/
/* 02/25/02  EJV    10     Replaced ascii_to_ macros with asciiToxxx fun*/
/*			   Added asciiToUlong, asciiToUint16,		*/
/*			    asciiToUint8, asciiToSint, asciiToUint	*/
/*			   hex_to_ascii_str: added param hex_no_spaces.	*/
/* 12/09/01  GLB    05     Added asciiToSlong, asciiToSint16,           */
/*                          asciiToSint8, asciiToUchar                  */
/* 09/10/01  GLB    08     Added ascii_to_uint & ascii_to_slongint      */
/* 08/06/01  GLB    07     Added ascii_to_sint                          */
/* 06/06/01  GLB    06     Added ascii_to_double & ascii_to_float       */
/* 05/21/01  MDE    05     Added bitstring_to_ascii_str			*/
/* 01/19/01  EJV    04     UNIX: added strcmpi, stricmp, strnicmp protos*/
/* 01/05/01  EJV    03     Corrected ascii_to_ushortint, AIX returned 0.*/
/*			   ascii_to_ulongint: changed "%ul" to "%lu".	*/
/* 11/02/00  MDE    02     Added _hex_no_spaces				*/
/* 02/03/00  JRB    01     Created					*/
/************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

ST_VOID hex_to_ascii_str (
		ST_CHAR *astr,		/* ascii string			*/
		ST_UCHAR *hstr,		/* hex string			*/
                ST_UINT hlen,		/* len of hex string		*/
		ST_BOOLEAN hex_no_spaces);

ST_RET ascii_to_hex_str (
		ST_UCHAR *hstr,		/* hex string			*/
                ST_UINT *hlen_out,	/* ptr to hex len to be set	*/
                ST_UINT hlen_max,	/* maximum hex len to allow.	*/
		ST_CHAR *astr);		/* ascii string			*/

ST_VOID bitstring_to_ascii_str (ST_CHAR *astr, ST_UCHAR *bstr,
                		ST_UINT numBits);


ST_RET asciiToSlong   (ST_CHAR  *astr,	 			              
                       ST_LONG  *sLong);   
ST_RET asciiToUlong   (ST_CHAR *astr,
                       ST_ULONG *uLong);

ST_RET asciiToSint    (ST_CHAR *astr,
                       ST_INT *sInt);
ST_RET asciiToUint    (ST_CHAR *astr,
                       ST_UINT *uInt);

ST_RET asciiToSint16  (ST_CHAR  *astr,	 			              
                       ST_INT16 *sInt16);   
ST_RET asciiToUint16  (ST_CHAR *astr,
                       ST_UINT16 *uInt16);

ST_RET asciiToSint8   (ST_CHAR  *astr,	 			              
                       ST_INT8  *sInt8);   
ST_RET asciiToUint8   (ST_CHAR *astr,
                       ST_UINT8 *uInt8);

ST_RET asciiToUchar   (ST_CHAR  *astr,	 			              
                       ST_UCHAR *uChar);   

ST_RET asciiToFloat   (ST_CHAR  *astr,	 			              
                       ST_FLOAT *floatNum);   
ST_RET asciiToDouble   (ST_CHAR *astr,
                       ST_DOUBLE *doubleNum);


#if defined(PSOS_SYSTEM) || defined(VXWORKS) || defined(_AIX) || \
    defined(sun) || defined(__alpha) || defined(__hpux)
ST_INT strcmpi (ST_CHAR *,ST_CHAR *);
ST_INT stricmp (ST_CHAR *,ST_CHAR *);
ST_INT strnicmp (ST_CHAR *,ST_CHAR *,ST_INT n);
#endif

#ifdef __cplusplus
}
#endif

#endif	/* !STR_UTIL_INCLUDED	*/

