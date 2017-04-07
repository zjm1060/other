/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      		1999 - 2002, All Rights Reserved		*/
/*									*/
/* MODULE NAME : sx_defs.h						*/
/* PRODUCT(S)  : 							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/28/02  RWM     36    Added function sx_get_time_ex                */
/* 05/15/02  RWM     35    Added numAlloced3 to sx_dec_ctrl             */
/* 04/17/02  EJV     34    Added 'utc' fun ptr in SXD_ARB_DATA_CTRL	*/
/* 03/05/02  JRB     33    Add more SD_CONST.				*/
/* 02/26/02  EJV     32    strStart: added SD_CONST for rt_head param.	*/
/* 01/21/02  EJV     31    Added useFp field in SX_ENC_CTRL.		*/
/*			   Added sx_start_encodeEx, sx_init_encodeEx.	*/
/*			   Added sx_add_buf, sx_wr_comment funcs.	*/
/* 11/28/01  DSF     30    Added a global flag sxIgnoreNS and a ignoreNS*/
/*			   field to SX_DEC_CTRL to allow the user to	*/
/*			   ignore namespaces in element and atttribute	*/
/*			   names					*/
/* 11/21/01  DGE     29    Removed sx_wr_int16 & sx_wr_int8 functions. 	*/
/* 11/21/01  MDE     28    Moved elemBuf to SX_DEC_CONTROL		*/
/* 11/21/01  DGE     27    Added sx_wr_int16 & sx_wr_int8 functions.  	*/
/* 11/15/01  DGE     26    Added sx_wr_float functions & changed type 	*/
/*			    on sx_wr_double functions to ST_DOUBLE from	*/
/*			    double.					*/
/* 08/22/01  DSF     25    Added elUser used to return the user pointer	*/
/*			   stored in the current element structure	*/
/* 06/25/01  EJV     24    For thread safe operation:			*/
/*			   SX_ENC_CTRL: +sxdPrefix, +sxdStructNestLevel;*/
/*			   SX_DEC_ELEMENT_INFO: +elemBuf;		*/
/*			   Added SX_MAX_ELEM_LEN, SXD_MAX_PREFIX defines*/
/*			   strStart: added rt_head parameter.		*/
/*			   SX_DEC_CTRL: added u_sx_el_start, u_sx_el_end*/
/*			   Added twin funcs: sx_parse_mt, sx_parseEx_mt.*/
/* 05/18/01  RKR     23    Cleaned up some prototypes			*/
/* 05/11/01  JRB     22    Pass fileName, not (FILE *) to sx_parseEx.	*/
/* 04/19/01  RWM     21    Added prototype for sx_format_string         */
/* 03/27/01  MDE     20    Added SXA (local/AXS4 data)			*/
/* 03/21/01  MDE     19    Added arb data handling 			*/
/* 02/28/01  MDE     18    Removed bEmptyTag from SX_ENC_CTRL		*/
/* 02/22/01  MDE     17    Changes to support SXD style selections	*/
/* 02/12/01  RWM     16    Added file pointer and flag to SX_DEC_CTRL   */
/* 01/24/01  EJV     15    Added sx_dec_ctrl forward definition		*/
/* 01/22/01  DWL     14    Created global var sxUseFormatting		*/
/* 01/02/01  DWL     13    Removed UIB_DEFINED check			*/
/* 12/05/00  MDE     12    Added SX_XML_MALFORMED error code		*/
/* 12/04/00  MDE     11    Allow limit defines to be overridden		*/
/* 11/06/00  DWL     10    Modified sx_write_element and sx_end_element	*/
/*                         calls with added parameter			*/
/* 11/02/00  MDE     09    Changed attribute handling a bit		*/
/* 10/30/99  MDE     08    Significant cleanup and new features		*/
/* 09/29/00  DWL     07    Created new error code to allow parse to 	*/
/*			   continue or stop				*/
/* 08/15/00  DWL     06    Removed MAX_MSG_SIZE			        */
/* 08/11/00  DWL     05    Added UIB_DEFINED member		        */
/* 07/20/00  DWL     04    Added formatting member to SX_ENC_CTRL       */
/* 03/21/00  DWL     03    Changed sx_wr_string_el to accept attributes */
/* 03/15/00  DWL     02    Removed redefinition of SX_DEC_CTRL		*/
/* 09/15/99          01    Module created				*/
/************************************************************************/
/************************************************************************/

#ifndef SX_DEFS_INCLUDED
#define SX_DEFS_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************/

#include "glbtypes.h"
#include "mms_def2.h"
#include "mms_mp.h"
#include "mms_pvar.h"
#include "mms_vvar.h"

/************************************************************************/
/* This is the amount of overhead required to ensure no overwrite 	*/

#define SX_ENC_BUFFER_OH 	20

/************************************************************************/
/* Capacities */

#if !defined(SX_MAX_XML_NEST)
#define SX_MAX_XML_NEST	        30 
#endif

#if !defined(SX_MAX_STACK_LEVEL)
#define SX_MAX_STACK_LEVEL      25
#endif

#if !defined(SX_MAX_TAG_LEN)
#define SX_MAX_TAG_LEN	       100
#endif

#if !defined(SX_MAX_ATTRIB)
#define SX_MAX_ATTRIB 		20
#endif

#if !defined(SX_MAX_ATTR_NAME)
#define SX_MAX_ATTR_NAME       100
#endif

#if !defined(SX_MAX_ATTR_VALUE)
#define SX_MAX_ATTR_VALUE     1000	
#endif

#if !defined(SX_MAX_ELEM_LEN)
#define SX_MAX_ELEM_LEN       2000
#endif



/************************************************************************/
/* Error codes - stop parsing */
#define SX_USER_ERROR			1
#define SX_STRUCT_NOT_FOUND	    	2
#define SX_REQUIRED_TAG_NOT_FOUND	3
#define SX_DUPLICATE_NOT_ALLOWED	4
#define SX_EMPTY_TAG_NOT_ALLOWED	5
#define SX_XML_NEST_TOO_DEEP	    	6
#define SX_XML_BUFFER_OVER_MAX		7
#define SX_XML_MALFORMED		8

/* Normal errors - continue with parse */
#define SX_ERR_CONVERT		       10

/************************************************************************/
/************************************************************************/
/* XML <-> Local XML Style Selection					*/

/* Note: Style A must have the value 0 */
#define SXD_XML_STYLE_A	0 /* Values as attrib, single element/struct 	*/
#define SXD_XML_STYLE_C	1 /* CDATA value encoding	      		*/
#define SXD_XML_STYLE_V	2 /* Each value has own element, attrib encoded	*/

extern ST_INT sxdDefXmlStyle; 	/* Note: 0 is style A */

/* Examples:								*/
/* A: <Mail pnid="12" zip="48044"/>					*/
/* A: <Alarm tag="BinFull" vs.v="78" vs.s="Good"/>			*/

/* C: <Mail><pnid>12</pnid><zip>48044<zip></Mail>			*/
/* C: <Alarm><tag>BinFull</tag><vs><v>78</v><s>Good</s></vs></Alarm>	*/
									
/* V: <Mail><pnid V="12"/><zip V="48044"/></Mail>			*/
/* V: <Alarm><tag V="BinFull"/><vs><v V="78"/><s V="Good"/></vs></Alarm>*/

/************************************************************************/
/************************************************************************/
extern ST_BOOLEAN sxUseSax;
extern ST_BOOLEAN sxUseFormatting;
extern ST_BOOLEAN sxIgnoreNS;

/* elementFlags defines; bitmasked */
#define SX_ELF_CSTART		0x0001
#define SX_ELF_CEND		0x0002
#define SX_ELF_CSTARTEND	0x0003

#define SX_ELF_RPT		0x0008
#define SX_ELF_OPT		0x0004
#define SX_ELF_OPTRPT		0x000C

#define SX_ELF_EMPTYOK		0x0010

struct sx_dec_ctrl;		/* forward definition */

typedef struct sx_element
  {
  char *tag;
  ST_INT elementFlags;
  void (*funcPtr)(struct sx_dec_ctrl *sxDecCtrl);

  char *user;

/* Runtime elements */
  ST_INT numOcc;
  } SX_ELEMENT;


/************************************************************************/

/* defines for 'reason' */
#define SX_ELEMENT_START	1
#define SX_ELEMENT_END		2

typedef struct sx_element_tbl_ctrl
  {
  ST_INT numItems;
  SX_ELEMENT *itemTbl;
  } SX_ELEMENT_TBL_CTRL;

typedef struct sxe_attr_pair
  {
  ST_CHAR *name;
  ST_CHAR *value;
  } SXE_ATTR_PAIR;

typedef struct sxd_attr_pair
  {
  ST_CHAR name[SX_MAX_ATTR_NAME];
  ST_CHAR value[SX_MAX_ATTR_VALUE];
  } SXD_ATTR_PAIR;

typedef struct sx_dec_element_info
  {
  ST_CHAR tag[SX_MAX_TAG_LEN];		/* start/end			*/
  ST_CHAR *entityStart;			/* start (and simple end)	*/
  ST_UINT attrCount;			/* start (and simple end)	*/
  SXD_ATTR_PAIR attr[SX_MAX_ATTRIB];	/* start (and simple end)	*/
  ST_CHAR *entityEnd;			/* end				*/
  } SX_DEC_ELEMENT_INFO;

typedef struct sx_dec_ctrl 
  {
/* XML being decoded		*/
  ST_CHAR *xmlStart;			
  ST_INT xmlLen;

  ST_BOOLEAN useFp;
  ST_BOOLEAN ignoreNS;
  FILE *fp;
/* Decoded element information 	*/
  ST_INT reason;		   /* SX_ELEMENT_START, SX_ELEMENT_END	*/
  SX_DEC_ELEMENT_INFO sxDecElInfo; 
  ST_CHAR elemBuf[SX_MAX_ELEM_LEN+1];	/* tmp buffer for an element	*/

/* Set to != SD_SUCCESS for error */
  ST_RET errCode; 		/* defined above */

/* Internal use */
  SX_ELEMENT *item;
  ST_INT itemStackLevel;
  SX_ELEMENT_TBL_CTRL items[SX_MAX_STACK_LEVEL];
  ST_BOOLEAN auto_pop[SX_MAX_STACK_LEVEL];

  ST_INT xmlNestLevel;
  SX_ELEMENT *elTbl[SX_MAX_XML_NEST];
  ST_CHAR *xmlPos;

  ST_INT numAlloced; /* Used to keep track of dynamically allocated message buffers */
  ST_INT numAlloced2;
  ST_INT numAlloced3;

/* User fields */
  ST_VOID *usr;
  ST_VOID *elUser;
  
  /* Function pointers below should be used for thread safe XML enc/dec	*/
  /* instead of using the global variables u_sx_el_start, u_sx_el_end.	*/
  /* The initial values for these pointers are passed to the functions	*/
  /* sx_parse_mt or sx_parseEx_mt.					*/
  ST_RET (*u_sx_el_start) (struct sx_dec_ctrl *sxDecCtrl, ST_CHAR *tag);
  ST_RET (*u_sx_el_end) (struct sx_dec_ctrl *sxDecCtrl, ST_CHAR *tag);
  } SX_DEC_CTRL ;

/************************************************************************/

#define SX_PUSH(itemTbl)\
  sx_push (sxDecCtrl, sizeof(itemTbl)/sizeof(SX_ELEMENT), itemTbl, SD_TRUE);

/* Thread unsafe versions of XML parsing				*/
/* They are using the u_sx_el_xxx global function pointers 		*/
ST_RET sx_parse (ST_LONG lMsgLen, ST_CHAR *xml, ST_INT numItems, 
		 SX_ELEMENT *itemTbl, ST_VOID *usr);
ST_RET sx_parseEx (ST_CHAR *fileName, ST_INT numItems, 
		   SX_ELEMENT *itemTbl, ST_VOID *usr);

/* Thread safe versions of XML parsing					*/
/* They are using the u_sx_el_xxx defined in the SX_DEC_CTRL) 		*/
ST_RET sx_parse_mt (ST_LONG lMsgLen, ST_CHAR *xml, ST_INT numItems, 
		 SX_ELEMENT *itemTbl, ST_VOID *usr,
                 ST_RET (*u_sx_el_start_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag),
                 ST_RET (*u_sx_el_end_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag));
ST_RET sx_parseEx_mt (ST_CHAR *fileName, ST_INT numItems, 
                 SX_ELEMENT *itemTbl, ST_VOID *usr,
                 ST_RET (*u_sx_el_start_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag),
                 ST_RET (*u_sx_el_end_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag));

ST_VOID sx_push (SX_DEC_CTRL *sxDecCtrl, ST_INT numItems, 
		 SX_ELEMENT *itemTbl, ST_BOOLEAN auto_pop);
ST_VOID sx_pop (SX_DEC_CTRL *sxDecCtrl);


ST_RET sx_get_entity (SX_DEC_CTRL *sxDecCtrl, 
		      ST_CHAR *destBuf, ST_INT destLen,
		      ST_INT *lenOut);
ST_RET sx_get_int (SX_DEC_CTRL *sxDecCtrl, ST_INT *out_ptr);
ST_RET sx_get_double (SX_DEC_CTRL *sxDecCtrl, ST_DOUBLE *out_ptr);
ST_RET sx_get_uchar (SX_DEC_CTRL *sxDecCtrl, ST_UCHAR *out_ptr);
ST_RET sx_get_int16 (SX_DEC_CTRL *sxDecCtrl, ST_INT16 *out_ptr);
ST_RET sx_get_long (SX_DEC_CTRL *sxDecCtrl, ST_LONG *out_ptr);
ST_RET sx_get_ulong (SX_DEC_CTRL *sxDecCtrl, ST_UINT32 *out_ptr);
ST_RET sx_get_bool (SX_DEC_CTRL *sxDecCtrl, ST_BOOLEAN *out_ptr);
ST_RET sx_get_time (SX_DEC_CTRL *sxDecCtrl, time_t *out_ptr);
ST_RET sx_get_time_ex (SX_DEC_CTRL *sxDecCtrl, time_t *out_ptr, long *microseconds);
ST_RET sx_get_uint (SX_DEC_CTRL *sxDecCtrl, ST_UINT *out_ptr);
ST_RET sx_get_alloc_string (SX_DEC_CTRL *sxDecCtrl, ST_CHAR **strOut);
ST_RET sx_get_string_ptr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR **strOut, ST_INT *lenOut);
ST_RET sx_get_string (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *dest, ST_INT *lenOut);
ST_VOID sx_format_string (ST_CHAR *dest, ST_CHAR *src);
ST_RET sx_get_bitstring (SX_DEC_CTRL *sxDecCtrl, ST_INT *dest, ST_INT *lenOut);
ST_RET sx_get_element_contents (SX_DEC_CTRL *sxDecCtrl, 
		      ST_CHAR *destBuf, ST_INT destLen,
		      ST_INT *lenOut);
ST_RET sx_find_element_contents (SX_DEC_CTRL *sxDecCtrl, 
		      ST_CHAR **elStartOut, ST_INT *lenOut);

ST_RET sx_get_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *value, ST_CHAR *name);
ST_RET sx_get_attr_ptr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR **ptrOut, ST_CHAR *name);
ST_RET sx_get_bool_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_BOOLEAN *out_ptr);
ST_RET sx_get_int_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_INT *out_ptr);
ST_RET sx_get_double_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_DOUBLE *out_ptr);
ST_RET sx_get_uchar_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_UCHAR *out_ptr);
ST_RET sx_get_int16_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_INT16 *out_ptr);
ST_RET sx_get_long_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_LONG *out_ptr);
ST_RET sx_get_ulong_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_UINT32 *out_ptr);
ST_RET sx_get_uint_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_UINT *out_ptr);

#define sxd_decode_data_el(elName,xml,xmlLen,xmlUsed,vdp,typeId,elPres,checkDt)\
 _sxd_decode_data_el(elName,xml,xmlLen,xmlUsed,vdp,typeId,elPres,sxdDefXmlStyle,checkDt)


ST_RET _sxd_decode_data_el (ST_CHAR *elName, 
			   ST_CHAR *xml, ST_INT xmlLen, ST_INT *xmlUsed, 
			   ST_VOID *vdp, ST_INT typeId, 
			   ST_BOOLEAN *elPres, ST_INT sxdXmlStyle, ST_BOOLEAN checkDt);

ST_RET sxd_decode_rtdata_el (SD_CONST ST_CHAR *elName, 
			   ST_CHAR *xml, ST_INT xmlLen, ST_INT *xmlUsed, 
			   ST_VOID *vdp, SD_CONST RUNTIME_TYPE *rt_head, 
			   ST_INT rt_num, ST_BOOLEAN *elPres, 
			   ST_INT sxdXmlStyle, ST_BOOLEAN checkDt);

extern ST_RET (*u_sx_el_start) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag);
extern ST_RET (*u_sx_el_end) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag);

ST_VOID sxStartElement (SX_DEC_CTRL *sxDecCtrl);
ST_VOID sxEndElement (SX_DEC_CTRL *sxDecCtrl);
ST_RET sx_get_value (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *format_string, ST_VOID *out_ptr);
ST_RET sx_get_attr_value (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, 
			  ST_CHAR *format_string, ST_VOID *out_ptr);

ST_VOID sx_load_characters (ST_CHAR *lineBuf, ST_BOOLEAN *eof, FILE *fp, ST_INT numToRead);



/************************************************************************/
/************************************************************************/
/************************************************************************/

/* max prefix length in SXD_XML_STYLE_A */
#define  SXD_MAX_PREFIX	100

typedef struct sx_enc_ctrl 
  {
  ST_INT xmlBufLen;
  ST_CHAR *xmlBuf;
  ST_CHAR *xmlBufEnd;

  ST_BOOLEAN useFp;
  FILE *fp;

  ST_CHAR *nextWritePos;

  ST_INT currNestLevel;
  ST_CHAR tags[SX_MAX_STACK_LEVEL][SX_MAX_TAG_LEN+1];
  ST_RET errCode;

  ST_BOOLEAN bUseFormatting;
  ST_BOOLEAN bOneLineEl;

/* Used for data encoding */
  ST_BOOLEAN encDt;
  ST_CHAR *outerElement;
  ST_INT sxdXmlStyle;	
  ST_CHAR sxdPrefix[SXD_MAX_PREFIX+1];	/* only for SXD_XML_STYLE_A */
  ST_INT sxdStructNestLevel;		/* only for SXD_XML_STYLE_A */

  ST_VOID *usr;
  } SX_ENC_CTRL;

/* Macro for getting current encoded length */
#define SX_ENC_LEN(__sxEncCtrl)  ((__sxEncCtrl)->nextWritePos - (__sxEncCtrl)->xmlBuf)

/************************************************************************/
/************************************************************************/

SX_ENC_CTRL *sx_start_encode (ST_CHAR *xmlBuf, ST_INT xmlBufLen);
SX_ENC_CTRL *sx_start_encodeEx (ST_CHAR *fileName);
ST_VOID sx_end_encode (SX_ENC_CTRL *sxEncCtrl);
ST_VOID sx_init_encode (SX_ENC_CTRL *sxEncCtrl, 
			ST_CHAR *xmlBuf, ST_INT xmlBufLen);
ST_RET  sx_init_encodeEx (SX_ENC_CTRL *sxEncCtrl, ST_CHAR *fileName);

#define sx_start_element(sxEncCtrl,tag,numAttr,attr)\
              sx_write_element(sxEncCtrl,tag,numAttr,attr,SD_FALSE,SD_FALSE);

#define sx_write_empty_element(sxEncCtrl,tag,numAttr,attr)\
              sx_write_element(sxEncCtrl,tag,numAttr,attr,SD_FALSE,SD_TRUE);

ST_VOID sx_end_element (SX_ENC_CTRL *sxEncCtrl);
ST_VOID sx_write_element (SX_ENC_CTRL *sxEncCtrl, SD_CONST ST_CHAR *tag, 
			   ST_INT numAttr, SXE_ATTR_PAIR *attr, 
			   ST_BOOLEAN end, ST_BOOLEAN empty);

/* Defines for 'no attribute' cases */
#define sx_wr_string_el(sxEncCtrl,tag,str)\
		sx_wrx_string_el(sxEncCtrl,tag,str,0,NULL)

#define sx_wr_int_el(sxEncCtrl,tag,val)\
		sx_wrx_int_el(sxEncCtrl,tag,val,0,NULL)

#define sx_wr_uint_el(sxEncCtrl,tag,val)\
		sx_wrx_uint_el(sxEncCtrl,tag,val,0,NULL)

#define sx_wr_ulong_el(sxEncCtrl,tag,val)\
		sx_wrx_ulong_el(sxEncCtrl,tag,val,0,NULL)

#define sx_wr_float_el(sxEncCtrl,tag,val)\
		sx_wrx_float_el(sxEncCtrl,tag,val,0,NULL)

#define sx_wr_double_el(sxEncCtrl,tag,val)\
		sx_wrx_double_el(sxEncCtrl,tag,val,0,NULL)

#define sx_wr_long_el(sxEncCtrl,tag,val)\
		sx_wrx_long_el(sxEncCtrl,tag,val,0,NULL)

#define sx_wr_bool_el(sxEncCtrl,tag,val)\
		sx_wrx_bool_el(sxEncCtrl,tag,val,0,NULL)

#define sx_wr_time_el(sxEncCtrl,tag,val)\
		sx_wrx_time_el(sxEncCtrl,tag,val,0,NULL)

#define sx_wr_cdata_el(sxEncCtrl,tag,str, len)\
		sx_wrx_cdata_el(sxEncCtrl,tag,str,len,0,NULL)

/* Functions to write elements and element contents */
ST_VOID sx_wr_string (SX_ENC_CTRL *sxEncCtrl, ST_CHAR *str);
ST_VOID sx_wrx_string_el (SX_ENC_CTRL *sxEncCtrl, SD_CONST ST_CHAR *tag, ST_CHAR *str, 
			 ST_INT numAttr, SXE_ATTR_PAIR *attr);

ST_VOID sx_wr_bitstring (SX_ENC_CTRL *sxEncCtrl, ST_INT numBits, 
						 ST_UINT8 *bitStr);
ST_VOID sx_wrx_bitstring_el (SX_ENC_CTRL *sxEncCtrl, ST_CHAR *tag, 
			 ST_INT numBits, ST_UINT8 *bitStr,
			 ST_INT numAttr, SXE_ATTR_PAIR *attr);

ST_VOID sx_wr_int (SX_ENC_CTRL *sxEncCtrl, ST_INT val);
ST_VOID sx_wrx_int_el (SX_ENC_CTRL *sxEncCtrl, ST_CHAR *tag, ST_INT val,
			 ST_INT numAttr, SXE_ATTR_PAIR *attr);

ST_VOID sx_wr_uint (SX_ENC_CTRL *sxEncCtrl, ST_UINT val);
ST_VOID sx_wrx_uint_el (SX_ENC_CTRL *sxEncCtrl, ST_CHAR *tag, ST_UINT val,
			 ST_INT numAttr, SXE_ATTR_PAIR *attr);

ST_VOID sx_wr_ulong (SX_ENC_CTRL *sxEncCtrl, ST_ULONG val);
ST_VOID sx_wrx_ulong_el (SX_ENC_CTRL *sxEncCtrl, ST_CHAR *tag, ST_ULONG val,
			 ST_INT numAttr, SXE_ATTR_PAIR *attr);

ST_VOID sx_wr_float (SX_ENC_CTRL *sxEncCtrl, ST_FLOAT val);
ST_VOID sx_wrx_float_el (SX_ENC_CTRL *sxEncCtrl, ST_CHAR *tag, ST_FLOAT val,
			 ST_INT numAttr, SXE_ATTR_PAIR *attr);

ST_VOID sx_wr_double (SX_ENC_CTRL *sxEncCtrl, double val);
ST_VOID sx_wrx_double_el (SX_ENC_CTRL *sxEncCtrl, ST_CHAR *tag, double val,
			 ST_INT numAttr, SXE_ATTR_PAIR *attr);

ST_VOID sx_wr_long (SX_ENC_CTRL *sxEncCtrl, ST_LONG val);
ST_VOID sx_wrx_long_el (SX_ENC_CTRL *sxEncCtrl, ST_CHAR *tag, ST_LONG val,
			 ST_INT numAttr, SXE_ATTR_PAIR *attr);

ST_VOID sx_wr_bool (SX_ENC_CTRL *sxEncCtrl, ST_BOOLEAN val);
ST_VOID sx_wrx_bool_el (SX_ENC_CTRL *sxEncCtrl, ST_CHAR *tag, ST_BOOLEAN val,
			 ST_INT numAttr, SXE_ATTR_PAIR *attr);

ST_VOID sx_wr_time (SX_ENC_CTRL *sxEncCtrl, time_t val);
ST_VOID sx_wrx_time_el (SX_ENC_CTRL *sxEncCtrl, ST_CHAR *tag, time_t val,
			 ST_INT numAttr, SXE_ATTR_PAIR *attr);

ST_VOID sx_wr_cdata (SX_ENC_CTRL *sxEncCtrl, ST_CHAR *str, ST_INT len);
ST_VOID sx_wrx_cdata_el (SX_ENC_CTRL *sxEncCtrl, ST_CHAR *tag, 
			ST_CHAR *str, ST_INT len,
			ST_INT numAttr, SXE_ATTR_PAIR *attr);

#define sxd_wr_data(ec,typeId,elName,data,encodeDt,elPres)\
  _sxd_wr_data(ec,typeId,elName,data,sxdDefXmlStyle,encodeDt,elPres)\

ST_RET _sxd_wr_data (SX_ENC_CTRL *sxEncCtrl, ST_INT typeId,
		   ST_CHAR *elName, ST_VOID *data, ST_INT sxdXmlStyle,
		   ST_BOOLEAN encodeDt, ST_BOOLEAN *elPres);

ST_RET sxd_wr_rtdata (SX_ENC_CTRL *sxEncCtrl, RUNTIME_TYPE *rt, ST_INT numRt, 
		   ST_CHAR *elName, ST_VOID *data, ST_INT sxdXmlStyle,
		   ST_BOOLEAN encodeDt, ST_BOOLEAN *elPres);

ST_VOID sx_add_string (SX_ENC_CTRL *sxEncCtrl, SD_CONST ST_CHAR *_src, 
				ST_CHAR **writePosIo);
ST_RET  sx_add_buf (SX_ENC_CTRL *sxEncCtrl, SD_CONST ST_CHAR *_src, ST_INT _srcLen,
                    ST_CHAR **writePosIo);
ST_VOID sx_wr_comment (SX_ENC_CTRL *sxEncCtrl, ST_CHAR *str);


/************************************************************************/
/************************************************************************/
/* Arbitrary data processing, for use in traversing the RUNTIME_TYPE	*/

typedef struct 
  {
  ST_RET (*arrStart) (ST_VOID *usr, RUNTIME_TYPE *rt);
  ST_RET (*arrEnd)   (ST_VOID *usr, RUNTIME_TYPE *rt);
  ST_RET (*strStart) (ST_VOID *usr, RUNTIME_TYPE *rt, SD_CONST RUNTIME_TYPE *rt_head);
  ST_RET (*strEnd)   (ST_VOID *usr, RUNTIME_TYPE *rt);
  ST_RET (*int8)     (ST_VOID *usr, ST_INT8    *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*int16)    (ST_VOID *usr, ST_INT16   *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*int32)    (ST_VOID *usr, ST_INT32   *data_dest, RUNTIME_TYPE *rt);
#ifdef INT64_SUPPORT
  ST_RET (*int64)    (ST_VOID *usr, ST_INT64   *data_dest, RUNTIME_TYPE *rt);
#endif
  ST_RET (*uint8)    (ST_VOID *usr, ST_UINT8   *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*uint16)   (ST_VOID *usr, ST_UINT16  *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*uint32)   (ST_VOID *usr, ST_UINT32  *data_dest, RUNTIME_TYPE *rt);
#ifdef INT64_SUPPORT
  ST_RET (*uint64)   (ST_VOID *usr, ST_UINT64  *data_dest, RUNTIME_TYPE *rt);
#endif
  ST_RET (*flt)      (ST_VOID *usr, ST_FLOAT   *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*dbl)      (ST_VOID *usr, ST_DOUBLE  *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*oct)      (ST_VOID *usr, ST_UCHAR   *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*booln)    (ST_VOID *usr, ST_BOOLEAN *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*bcd1)     (ST_VOID *usr, ST_INT8    *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*bcd2)     (ST_VOID *usr, ST_INT16   *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*bcd4)     (ST_VOID *usr, ST_INT32   *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*bs)       (ST_VOID *usr, ST_UCHAR   *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*vis)      (ST_VOID *usr, ST_CHAR    *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*bt4)      (ST_VOID *usr, ST_INT32   *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*bt6)      (ST_VOID *usr, ST_INT32   *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*gt)       (ST_VOID *usr, time_t     *data_dest, RUNTIME_TYPE *rt);
  ST_RET (*utc)      (ST_VOID *usr, MMS_UTC_TIME *data_dest, RUNTIME_TYPE *rt);
  } SXD_ARB_DATA_CTRL;

ST_RET sxd_process_arb_data (ST_CHAR *datptr, SD_CONST RUNTIME_TYPE *rt_head, 
			     ST_INT rt_num, ST_VOID *usr, 
			     SXD_ARB_DATA_CTRL *ac, ST_BOOLEAN *elPres);

ST_RET sxaTextToLocal (ST_CHAR *pSource, ST_VOID *pDest, ST_INT numRt, RUNTIME_TYPE *rtHead);
ST_CHAR *sxaLocalToText (ST_VOID *dataPtr, RUNTIME_TYPE *rtHead, ST_INT numRt);


/************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* SX_DEFS_INCLUDED */

