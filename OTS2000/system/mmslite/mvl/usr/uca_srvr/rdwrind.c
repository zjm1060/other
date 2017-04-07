/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1986 - 1997, All Rights Reserved		        */
/*									*/
/* MODULE NAME : rdwrind.c    						*/
/* PRODUCT(S)  : MMSEASE-LITE						*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*	NONE								*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/23/06  MDE     15    Added time_t cast				*/
/* 05/18/05  JRB     14    Set failure code in u_no_read_allowed,	*/
/*			   u_no_write_allowed, mvlu_null_wr_ind.	*/
/* 05/18/05  JRB     13    mvlu_null_rd..: return non-zero val for Btime6,*/
/*			    set UTC time fraction to 0.999 seconds.	*/
/* 08/23/04  NAV     12    mvlu_null_rd_ind: chg data on each read.	*/
/* 08/13/04  JRB     11    Del unneeded includes.			*/
/* 07/23/04  JRB     10    Chg mvlu_null_rd_ind to return current time	*/
/*			   for UTC_TIME.				*/
/* 10/08/03  JRB     09    Add mvlu_null_rd(wr)_ind (foundry no longer	*/
/*			   creates them). mvlu_null_rd_ind return	*/
/*			   non-zero dummy data for some data types.	*/
/*			   Add "har_*_ind" to demonstrate array of structs.*/
/* 04/16/03  JRB     08    Add u_int32_offset_rd(wr)_ind_fun.		*/
/* 12/09/02  MDE     07    Now get offset from runtime type		*/
/* 02/27/02  JRB     06    Del // comments.				*/
/* 09/07/99  MDE     05    Upgraded to use new UCA report mechanisms	*/
/* 09/07/99  MDE     04    Migrated to GOMSGE .9			*/
/* 07/12/99  MDE     03    Added more array test code			*/
/* 09/16/98  MDE     02    Added SBO, RPT funs moved to mvlu_rpt.c	*/
/* 09/10/97  MDE     01    MMS-LITE V4.0 Release			*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"

#include "mmsdefs.h"
#include "mms_pvar.h"
#include "mms_vvar.h"

#include "mvl_acse.h"
#include "mvl_log.h"
#include "uca_obj.h"
#include "uca_srvr.h"

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/
/************************************************************************/

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

/************************************************************************/
/************************************************************************/
/************************************************************************/
/* SBO HANDLING								*/
/************************************************************************/
/*			u_mvl_sbo_operate				*/
/************************************************************************/

ST_VOID  u_mvl_sbo_operate (MVL_SBO_CTRL *sboSelect, 
			    MVLU_WR_VA_CTRL *mvluWrVaCtrl)
  {
MVL_VAR_ASSOC *va;
ST_CHAR *src;

  /* The select is in place, go ahead and operate ... 			*/
  va = mvluWrVaCtrl->wrVaCtrl->va;
  src = mvluWrVaCtrl->primData;

  mvlu_wr_prim_done (mvluWrVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/* COMMON READ/WRITE INDICATION HANDLERS				*/
/************************************************************************/
/*			u_string_offset_rd_ind_fun 	   		*/
/************************************************************************/
/* This common read indication function assumes that the base VA's	*/
/* user_info parameter points to the base of a data structure and the	*/
/* MVLU Read Control primRef element is the offset from that pointer to	*/
/* the string to return. Easy. Really.					*/

ST_VOID u_string_offset_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_CHAR *dest;
MVL_VAR_ASSOC *baseVa;
ST_CHAR *strBase;
ST_INT strOffset;

  baseVa = mvluRdVaCtrl->rdVaCtrl->va->base_va;
  strBase = (ST_CHAR *) baseVa->user_info;
  strOffset = (ST_INT) mvluRdVaCtrl->rt->mvluTypeInfo.offSet;

  dest = mvluRdVaCtrl->primData;
  strcpy (dest, strBase + strOffset);
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_uint8_offset_rd_ind_fun 			*/
/************************************************************************/
/* This common read indication function assumes that the base VA's	*/
/* user_info parameter points to the base of a data structure and the	*/
/* MVLU Read Control primRef element is the offset from that pointer to	*/
/* the ST_UINT8 to return. 						*/

ST_VOID u_uint8_offset_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_UINT8 *dest;
MVL_VAR_ASSOC *baseVa;
ST_UINT8 *baseAddr;
ST_INT valOffset;

  baseVa = mvluRdVaCtrl->rdVaCtrl->va->base_va;
  baseAddr = (ST_UINT8 *) baseVa->user_info;
  valOffset = (ST_INT) mvluRdVaCtrl->rt->mvluTypeInfo.offSet;

  dest = (ST_UINT8 *) mvluRdVaCtrl->primData;
  *dest = *(baseAddr + valOffset);
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_int32_offset_rd_ind_fun			*/
/************************************************************************/
ST_VOID u_int32_offset_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_INT32 *dest;
MVL_VAR_ASSOC *baseVa;
ST_INT32 *baseAddr;
ST_INT valOffset;

  baseVa = mvluRdVaCtrl->rdVaCtrl->va->base_va;
  baseAddr = (ST_INT32 *) baseVa->data;
  assert (baseAddr);
  valOffset = (ST_INT) mvluRdVaCtrl->rt->mvluTypeInfo.offSet;

  dest = (ST_INT32 *) mvluRdVaCtrl->primData;
  *dest = *(baseAddr + valOffset);
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_string_table_rd_ind_fun  			*/
/************************************************************************/
/* This common read indication function assumes that the base VA's	*/
/* user_info parameter points to a table of pointers to strings, and	*/
/* the MVLU Read Control primRef element is the index into that table	*/
/* to the string to return. 						*/

ST_VOID u_string_table_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_CHAR *dest;
MVL_VAR_ASSOC *baseVa;
ST_CHAR **strTbl;
ST_INT strIndex;

  baseVa = mvluRdVaCtrl->rdVaCtrl->va->base_va;
  strTbl = (ST_CHAR **) baseVa->user_info;
  strIndex = (ST_INT) mvluRdVaCtrl->primRef;

  dest = mvluRdVaCtrl->primData;
  strcpy (dest, strTbl[strIndex]);
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/* 		'NO READ ALLOWED' READ HANDLER				*/
/************************************************************************/

ST_VOID u_no_read_allowed (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
  mvluRdVaCtrl->rdVaCtrl->failure = ARE_OBJ_ACCESS_DENIED;	/* send in rsp*/
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_FAILURE);
  }


/************************************************************************/
/* 		'NO WRITE ALLOWED' WRITE HANDLER			*/
/************************************************************************/

ST_VOID u_no_write_allowed (MVLU_WR_VA_CTRL *mvluWrVaCtrl)
  {
  mvluWrVaCtrl->wrVaCtrl->failure = ARE_OBJ_ACCESS_DENIED;	/* send in rsp*/
  mvlu_wr_prim_done (mvluWrVaCtrl, SD_FAILURE);
  }


/************************************************************************/
/************************************************************************/
/* 'LIVE' REPORT VARIABLES 						*/
/************************************************************************/
/*			u_pbro_sg_pu_phsi_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "BRO$SP$Pu$Phsi" */

ST_VOID u_pbro_sg_pu_phsi_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_INT16 *dest;

  dest = (ST_INT16 *) mvluRdVaCtrl->primData;
  *dest = Pbro_SG_Pu_Phsi_val++;
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_pbro_sg_pu_neuti_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "BRO$SP$Pu$Neuti" */

ST_VOID u_pbro_sg_pu_neuti_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_INT16 *dest;

  dest = (ST_INT16 *) mvluRdVaCtrl->primData;
  *dest = Pbro_SG_Pu_neutrali_val++;
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_pbro_sg_pu_hzi_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "BRO$SP$Pu$Hzi" */

ST_VOID u_pbro_sg_pu_hzi_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_UINT16 *dest;

  dest = (ST_UINT16 *) mvluRdVaCtrl->primData;
  *dest = Pbro_SG_Pu_hzi_val++;
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/* ARRAY SAMPLES							*/
/************************************************************************/

static ARR_TYPE_TDEF arrTypeVar =
  {
  1,		/* a     */
  2,		/* b     */
    {		/* c     */
    69,
    70,
    71,
    72,
    73
    },
    {		/* d     */
      {		
      100,	/* da[0] */
      101,	/* db[0] */
      },
      {		
      102,	/* da[1] */
      103,	/* db[1] */
      },
    }
  };

static SRCCONFIG_TDEF arr2TypeVar =
  {
    {		/* [0] 		*/
    "srcName1",	/*   SrcName 	*/
    12,		/*   SrcPhsCT	*/
    13,		/*   SrcGndCT	*/
    14,		/*   SrcPhsVT	*/
    },
    {		/* [1] 		*/
    "srcName2",	/*   SrcName 	*/
    22,		/*   SrcPhsCT	*/
    23,		/*   SrcGndCT	*/
    24,		/*   SrcPhsVT	*/
    },
    {		/* [2] 		*/
    "srcName3",	/*   SrcName 	*/
    32,	    	/*   SrcPhsCT	*/
    33,		/*   SrcGndCT	*/
    34,		/*   SrcPhsVT	*/
    },
    {		/* [3] 		*/
    "srcName4",	/*   SrcName 	*/
    42,	        /*   SrcPhsCT	*/
    43,		/*   SrcGndCT	*/
    44,		/*   SrcPhsVT	*/
    },
    {		/* [4] 		*/
    "srcName5",	/*   SrcName 	*/
    52,		/*   SrcPhsCT	*/
    53,		/*   SrcGndCT	*/
    54,		/*   SrcPhsVT	*/
    },
    {		/* [5] 		*/
    "srcName6",	/*   SrcName 	*/
    62,		/*   SrcPhsCT	*/
    63,		/*   SrcGndCT	*/
    64,		/*   SrcPhsVT	*/
    },
  };

/************************************************************************/
/*			u_arr_type_a_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "ARR_TYPE$a" */

ST_VOID u_arr_type_a_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_INT16 *dest;

  dest = (ST_INT16 *) mvluRdVaCtrl->primData;
  *dest = arrTypeVar.a;
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_arr_type_b_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "ARR_TYPE$b" */

ST_VOID u_arr_type_b_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_INT32 *dest;

  dest = (ST_INT32 *) mvluRdVaCtrl->primData;
  *dest = arrTypeVar.b;
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_arr_type_c_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "ARR_TYPE$c" */

ST_VOID u_arr_type_c_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_INT16 *dest;
ST_RTINT curr_index;

  curr_index = mvluRdVaCtrl->rdVaCtrl->va->arrCtrl.curr_index;
  dest = (ST_INT16 *) mvluRdVaCtrl->primData;
  *dest = arrTypeVar.c[curr_index];
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }


/************************************************************************/
/*			u_arr_type_d_da_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "ARR_TYPE$d$da" */

ST_VOID u_arr_type_d_da_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_INT16 *dest;
ST_RTINT curr_index;

  curr_index = mvluRdVaCtrl->rdVaCtrl->va->arrCtrl.curr_index;
  dest = (ST_INT16 *) mvluRdVaCtrl->primData;
  *dest = arrTypeVar.d[curr_index].da;
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_arr_type_d_db_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "ARR_TYPE$d$db" */

ST_VOID u_arr_type_d_db_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_INT32 *dest;
ST_RTINT curr_index;

  curr_index = mvluRdVaCtrl->rdVaCtrl->va->arrCtrl.curr_index;
  dest = (ST_INT32 *) mvluRdVaCtrl->primData;
  *dest = arrTypeVar.d[curr_index].db;
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*			u_int32_offset_wr_ind_fun			*/
/************************************************************************/
ST_VOID u_int32_offset_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl)
  {
ST_INT32 *src;
ST_INT32 *dest;
MVL_VAR_ASSOC *baseVa;
ST_INT32 *baseAddr;
ST_INT valOffset;
ST_RET rc = SD_SUCCESS;

  baseVa = mvluWrVaCtrl->wrVaCtrl->va->base_va;
  baseAddr = (ST_INT32 *) baseVa->data;
  assert (baseAddr);
  valOffset = (ST_INT) mvluWrVaCtrl->rt->mvluTypeInfo.offSet;

  src = (ST_INT32 *) mvluWrVaCtrl->primData;
  dest = baseAddr + valOffset;

  *dest = *src;

  mvlu_wr_prim_done (mvluWrVaCtrl, rc);
  }

/************************************************************************/
/*			u_arr_type_a_wr_ind_fun					*/
/************************************************************************/
/* UCA variable name: "ARR_TYPE$a" */

ST_VOID u_arr_type_a_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl)
  {
ST_CHAR *src;

/* Get the pointer to the source for this primitive data element   */
  src = mvluWrVaCtrl->primData;
  arrTypeVar.a = *src;
  mvlu_wr_prim_done (mvluWrVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_arr_type_b_wr_ind_fun					*/
/************************************************************************/
/* UCA variable name: "ARR_TYPE$b" */

ST_VOID u_arr_type_b_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl)
  {
ST_CHAR *src;

/* Get the pointer to the source for this primitive data element   */
  src = mvluWrVaCtrl->primData;
  arrTypeVar.b = *src;
  mvlu_wr_prim_done (mvluWrVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_arr_type_c_wr_ind_fun					*/
/************************************************************************/
/* UCA variable name: "ARR_TYPE$c" */

ST_VOID u_arr_type_c_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl)
  {
ST_INT16 *src;
ST_RTINT curr_index;

  curr_index = mvluWrVaCtrl->wrVaCtrl->va->arrCtrl.curr_index;
  src = (ST_INT16 *) mvluWrVaCtrl->primData;
  arrTypeVar.c[curr_index] = *src;
  mvlu_wr_prim_done (mvluWrVaCtrl, SD_SUCCESS);
  }


/************************************************************************/
/*			u_arr_type_d_da_wr_ind_fun					*/
/************************************************************************/
/* UCA variable name: "ARR_TYPE$d$da" */

ST_VOID u_arr_type_d_da_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl)
  {
ST_INT16 *src;
ST_RTINT curr_index;

  curr_index = mvluWrVaCtrl->wrVaCtrl->va->arrCtrl.curr_index;
  src = (ST_INT16 *) mvluWrVaCtrl->primData;
  arrTypeVar.d[curr_index].da = *src;
  mvlu_wr_prim_done (mvluWrVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_arr_type_d_db_wr_ind_fun					*/
/************************************************************************/
/* UCA variable name: "ARR_TYPE$d$db" */

ST_VOID u_arr_type_d_db_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl)
  {
ST_INT32 *src;
ST_RTINT curr_index;

  curr_index = mvluWrVaCtrl->wrVaCtrl->va->arrCtrl.curr_index;
  src = (ST_INT32 *) mvluWrVaCtrl->primData;
  arrTypeVar.d[curr_index].db = *src;
  mvlu_wr_prim_done (mvluWrVaCtrl, SD_SUCCESS);
  }


/************************************************************************/
/************************************************************************/
/************************************************************************/
/*			u_srcconfig_srcname_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "SRCCONFIG$SrcName" */

ST_VOID u_srcconfig_srcname_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_RTINT curr_index;
ST_CHAR *dest;

/* This is an element of an array, get the index ... */
  curr_index = mvluRdVaCtrl->rdVaCtrl->va->arrCtrl.curr_index;
  dest = mvluRdVaCtrl->primData;
  strcpy (dest, arr2TypeVar[curr_index].SrcName);

/* When the data buffer has valid read data, send the response */
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_srcconfig_srcphsct_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "SRCCONFIG$SrcPhsCT" */

ST_VOID u_srcconfig_srcphsct_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_RTINT curr_index;
ST_UINT16 *dest;

/* This is an element of an array, get the index ... */
  curr_index = mvluRdVaCtrl->rdVaCtrl->va->arrCtrl.curr_index;

/* We will just return some dummy data for test purposes */
  dest = (ST_UINT16 *) mvluRdVaCtrl->primData;
  *dest = arr2TypeVar[curr_index].SrcPhsCT;

/* When the data buffer has valid read data, send the response */
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_srcconfig_srcgndct_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "SRCCONFIG$SrcGndCT" */

ST_VOID u_srcconfig_srcgndct_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_RTINT curr_index;
ST_UINT16 *dest;

/* This is an element of an array, get the index ... */
  curr_index = mvluRdVaCtrl->rdVaCtrl->va->arrCtrl.curr_index;

/* We will just return some dummy data for test purposes */
  dest = (ST_UINT16 *) mvluRdVaCtrl->primData;
  *dest = arr2TypeVar[curr_index].SrcGndCT;

/* When the data buffer has valid read data, send the response */
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_srcconfig_srcphsvt_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "SRCCONFIG$SrcPhsVT" */

ST_VOID u_srcconfig_srcphsvt_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_RTINT curr_index;
ST_UINT16 *dest;

/* This is an element of an array, get the index ... */
  curr_index = mvluRdVaCtrl->rdVaCtrl->va->arrCtrl.curr_index;

/* We will just return some dummy data for test purposes */
  dest = (ST_UINT16 *) mvluRdVaCtrl->primData;
  *dest = arr2TypeVar[curr_index].SrcPhsVT;

/* When the data buffer has valid read data, send the response */
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }


/************************************************************************/
/************************************************************************/
/*			u_srcconfig_srcname_wr_ind_fun					*/
/************************************************************************/
/* UCA variable name: "SRCCONFIG$SrcName" */

ST_VOID u_srcconfig_srcname_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl)
  {
ST_RTINT curr_index;
ST_CHAR *src;

/* This is an element of an array, get the index ... */
  curr_index = mvluWrVaCtrl->wrVaCtrl->va->arrCtrl.curr_index;

/* Get the pointer to the source for this primitive data element   */
  src = mvluWrVaCtrl->primData;
  strcpy (arr2TypeVar[curr_index].SrcName, src);
  mvlu_wr_prim_done (mvluWrVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_srcconfig_srcphsct_wr_ind_fun					*/
/************************************************************************/
/* UCA variable name: "SRCCONFIG$SrcPhsCT" */

ST_VOID u_srcconfig_srcphsct_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl)
  {
ST_RTINT curr_index;
ST_CHAR *src;

/* This is an element of an array, get the index ... */
  curr_index = mvluWrVaCtrl->wrVaCtrl->va->arrCtrl.curr_index;

/* Get the pointer to the source for this primitive data element   */
  src = mvluWrVaCtrl->primData;
  arr2TypeVar[curr_index].SrcPhsCT = *src;
  mvlu_wr_prim_done (mvluWrVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_srcconfig_srcgndct_wr_ind_fun					*/
/************************************************************************/
/* UCA variable name: "SRCCONFIG$SrcGndCT" */

ST_VOID u_srcconfig_srcgndct_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl)
  {
ST_RTINT curr_index;
ST_CHAR *src;

/* This is an element of an array, get the index ... */
  curr_index = mvluWrVaCtrl->wrVaCtrl->va->arrCtrl.curr_index;

/* Get the pointer to the source for this primitive data element   */
  src = mvluWrVaCtrl->primData;
  arr2TypeVar[curr_index].SrcGndCT = *src;
  mvlu_wr_prim_done (mvluWrVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_srcconfig_srcphsvt_wr_ind_fun					*/
/************************************************************************/
/* UCA variable name: "SRCCONFIG$SrcPhsVT" */

ST_VOID u_srcconfig_srcphsvt_wr_ind_fun (MVLU_WR_VA_CTRL *mvluWrVaCtrl)
  {
ST_RTINT curr_index;
ST_CHAR *src;

/* This is an element of an array, get the index ... */
  curr_index = mvluWrVaCtrl->wrVaCtrl->va->arrCtrl.curr_index;

/* Get the pointer to the source for this primitive data element   */
  src = mvluWrVaCtrl->primData;
  arr2TypeVar[curr_index].SrcPhsVT = *src;
  mvlu_wr_prim_done (mvluWrVaCtrl, SD_SUCCESS);
  }











/************************************************************************/
/*			u_pbro_st_out_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "PBRO$ST$Out" */

ST_VOID u_pbro_st_out_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_BOOLEAN *dest;

  dest = (ST_BOOLEAN *) mvluRdVaCtrl->primData;
  *dest = pbro_st.Out;
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_pbro_st_tar_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "PBRO$ST$Tar" */

ST_VOID u_pbro_st_tar_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_UINT8 *dest;

  dest = (ST_UINT8 *) mvluRdVaCtrl->primData;
  *dest = pbro_st.Tar;
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_pbro_st_fctds_b2_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "PBRO$ST$FctDS$b2" */

ST_VOID u_pbro_st_fctds_b2_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_UINT8 *dest;

  dest = (ST_UINT8 *) mvluRdVaCtrl->primData;
  *dest = (ST_UINT8) pbro_st.FctDS.b2;
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_pbro_st_fctds_q_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "PBRO$ST$FctDS$q" */

ST_VOID u_pbro_st_fctds_q_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_VOID *dest;

  dest = mvluRdVaCtrl->primData;
  memcpy (dest, &pbro_st.FctDS.q, sizeof (pbro_st.FctDS.q));
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_pbro_st_fctds_t_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "PBRO$ST$FctDS$t" */

ST_VOID u_pbro_st_fctds_t_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_UINT32 *dest;

  dest = (ST_UINT32 *) mvluRdVaCtrl->primData;
  *dest = pbro_st.FctDS.t_ms;
  ++dest;
  *dest = pbro_st.FctDS.t_days;
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_pbro_st_auxin1_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "PBRO$ST$AuxIn1" */

ST_VOID u_pbro_st_auxin1_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_VOID *dest;

  dest = mvluRdVaCtrl->primData;
  memcpy (dest, &pbro_st.AuxIn1, sizeof (pbro_st.AuxIn1));
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			u_pbro_st_pugrp_rd_ind_fun					*/
/************************************************************************/
/* UCA variable name: "PBRO$ST$PuGrp" */

ST_VOID u_pbro_st_pugrp_rd_ind_fun (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_UINT8 *dest;

  dest = (ST_UINT8 *) mvluRdVaCtrl->primData;
  *dest = pbro_st.PuGrp;
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			har_mag_i_rd_ind				*/
/* This sample function just returns the array index as the data for each read.*/
/************************************************************************/
ST_VOID har_mag_i_rd_ind (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_RTINT curr_index;
ST_INT *dest;

  /* This is an element of an array, get the index ... */
  curr_index = mvluRdVaCtrl->rdVaCtrl->va->arrCtrl.curr_index;

  dest = (ST_INT *) mvluRdVaCtrl->primData;
  *dest = curr_index;	/* dummy data. Return array index	*/
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }
/************************************************************************/
/*			har_mag_f_rd_ind				*/
/* This sample function just returns the array index as the data for each read.*/
/************************************************************************/
ST_VOID har_mag_f_rd_ind (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_RTINT curr_index;
ST_FLOAT *dest;

  /* This is an element of an array, get the index ... */
  curr_index = mvluRdVaCtrl->rdVaCtrl->va->arrCtrl.curr_index;

  dest = (ST_FLOAT *) mvluRdVaCtrl->primData;
  *dest = (ST_FLOAT)(curr_index * 1.0);	/* dummy data. Return array index converted to float	*/
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }
/************************************************************************/
/*			har_ang_i_rd_ind				*/
/* This sample function just returns the array index as the data for each read.*/
/************************************************************************/
ST_VOID har_ang_i_rd_ind (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_RTINT curr_index;
ST_INT *dest;

  /* This is an element of an array, get the index ... */
  curr_index = mvluRdVaCtrl->rdVaCtrl->va->arrCtrl.curr_index;

  dest = (ST_INT *) mvluRdVaCtrl->primData;
  *dest = curr_index * 2;	/* dummy data. Return array index * 2	*/
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }
/************************************************************************/
/*			har_ang_f_rd_ind				*/
/* This sample function just returns the array index as the data for each read.*/
/************************************************************************/
ST_VOID har_ang_f_rd_ind (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
ST_RTINT curr_index;
ST_FLOAT *dest;

  /* This is an element of an array, get the index ... */
  curr_index = mvluRdVaCtrl->rdVaCtrl->va->arrCtrl.curr_index;

  dest = (ST_FLOAT *) mvluRdVaCtrl->primData;
  *dest = (ST_FLOAT)(curr_index * 2.0);	/* dummy data. Return array index * 2 converted to float*/
  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			mvlu_null_rd_ind				*/
/* Default read indication function. Just return some kind of dummy data*/
/* that is appropriate for the element type (mvluRdVaCtrl->rt->el_tag).	*/ 
/************************************************************************/
static ST_BOOLEAN   gBool = SD_FALSE;
static ST_INT8	    gInt8 = 0;
static ST_INT16	    gInt16 = 0;
static ST_INT32	    gInt32 = 0;
static ST_UINT8	    gUint8 = 0;
static ST_UINT16    gUint16 = 0;
static ST_UINT32    gUint32 = 0;
static ST_FLOAT	    gFloat = 0.0;
static ST_DOUBLE    gDouble = 0.0;
static ST_UINT16    gRptQual[] = 
		      {
		      0x0000, 0x0008, 0x0020, 0x0028, 0x4000, 0x4008,
		      0xc100, 0xc108, 0xc040, 0xc048, 0xd000, 0xd008,
		      0xc080, 0xc088, 0xfff0, 0xfff8
		      };

#define MAX_QUAL  (sizeof (gRptQual) / sizeof (ST_UINT16))

static int curQual = 0;
static ST_DOUBLE gNextQualChange = 0.0;

ST_VOID mvlu_null_rd_ind (MVLU_RD_VA_CTRL *mvluRdVaCtrl)
  {
MMS_UTC_TIME UtcTimeNow;
ST_CHAR *compName;

  switch (mvluRdVaCtrl->rt->el_tag)
    {
    case RT_BOOL :
      *(ST_BOOLEAN *) mvluRdVaCtrl->primData = gBool;
      gBool = (gBool) ? SD_FALSE : SD_TRUE;
    break;

    case RT_BIT_STRING :
      /* Set all bits to 0.					*/
      /* For variable-length bitstring, this also sets len = 0	*/
      memset (mvluRdVaCtrl->primData, 0, mvluRdVaCtrl->rt->el_size);

      compName = ms_comp_name_find (mvluRdVaCtrl->rt);
      if (strcmp (compName, "q") == 0)
	{
	if (mvluRdVaCtrl->rt->el_size >=4)
	  {
	  ST_DOUBLE timeNow;
	  ST_INT16 *pInt = (ST_INT16 *)mvluRdVaCtrl->primData;
	  *pInt = 13;
          mvluRdVaCtrl->primData[2] = gRptQual[curQual] >> 8;	/* high byte	*/
          mvluRdVaCtrl->primData[3] = gRptQual[curQual] & 0xFF;	/* low byte	*/
	  timeNow =  sGetMsTime ();
	  if (timeNow > gNextQualChange)
	    {
	    gNextQualChange = timeNow + 2000.0;
	    curQual ++;
	    curQual %= MAX_QUAL;
	    }
	  }
	}
    break;

    case RT_INTEGER :
      switch (mvluRdVaCtrl->rt->u.p.el_len)	/* determine length		*/
        {
        case 1 :
          *(ST_INT8 *) mvluRdVaCtrl->primData = gInt8++;
        break;

        case 2 :
          *(ST_INT16 *) mvluRdVaCtrl->primData = gInt16++;
        break;

        case 4 :
          *(ST_INT32 *) mvluRdVaCtrl->primData = gInt32++;
        break;

#ifdef INT64_SUPPORT
        case 8 :
          *(ST_INT64 *) mvluRdVaCtrl->primData = -8;
        break;
#endif
        }
    break;

    case RT_UNSIGNED :
      switch (mvluRdVaCtrl->rt->u.p.el_len)	/* determine length		*/
        {
        case 1 :
          *(ST_UINT8 *) mvluRdVaCtrl->primData = gUint8++;
        break;

        case 2 :
          *(ST_UINT16 *) mvluRdVaCtrl->primData = gUint16++;
        break;

        case 4 :
          *(ST_UINT32 *) mvluRdVaCtrl->primData = gUint32++;
        break;

#ifdef INT64_SUPPORT
        case 8 :
          *(ST_UINT64 *) mvluRdVaCtrl->primData = 8;
        break;
#endif
        }
    break;

    case RT_FLOATING_POINT :
      if (mvluRdVaCtrl->rt->u.p.el_len != sizeof (ST_FLOAT))
        {
        *(ST_DOUBLE *) mvluRdVaCtrl->primData = gDouble;
	gDouble += 1.1;
        }
      else
        {
        *(ST_FLOAT *) mvluRdVaCtrl->primData = gFloat;
	gFloat += (ST_FLOAT) 1.1;
        }
    break;

    case RT_OCTET_STRING :
      memset (mvluRdVaCtrl->primData, 0, mvluRdVaCtrl->rt->el_size);
    break;

    case RT_VISIBLE_STRING :
      /* copy as much of dummy string as possible	*/
      strncpy (mvluRdVaCtrl->primData, "dummy_vstring_33bytes_34567890123", mvluRdVaCtrl->rt->el_size);
      mvluRdVaCtrl->primData[mvluRdVaCtrl->rt->el_size-1] = '\0';	/* make sure NULL-terminated*/
    break;

    case RT_GENERAL_TIME :
    case RT_BCD :
      memset (mvluRdVaCtrl->primData, 0, mvluRdVaCtrl->rt->el_size);
    break;

    case RT_BINARY_TIME :
      if (mvluRdVaCtrl->rt->u.p.el_len==6)
        {	/* for Btime6, set constant time.	*/
        MMS_BTIME6 *btime6=(MMS_BTIME6 *)mvluRdVaCtrl->primData;
        btime6->day = 0;
        btime6->ms = 1999;
        }
      else
        memset (mvluRdVaCtrl->primData, 0, mvluRdVaCtrl->rt->el_size);
    break;

    case RT_UTC_TIME :
      UtcTimeNow.secs = (ST_UINT32) time(NULL);	/* Update seconds in UTC time	*/
      UtcTimeNow.fraction = (ST_UINT32)(999.0 / 1000.0 * (ST_DOUBLE)0x01000000); /* 999 milliseconds*/
      UtcTimeNow.qflags =  0x0E;	/* dummy value	*/
					/* leave fraction,qflags alone*/
      memcpy (mvluRdVaCtrl->primData, &UtcTimeNow, sizeof (MMS_UTC_TIME));
    break;

    case RT_UTF8_STRING :
      /* Just return empty (0 length) string.	*/
      memset (mvluRdVaCtrl->primData, 0, mvluRdVaCtrl->rt->el_size);
    break;

    default :			/* should not be any other tag	*/
      MVL_LOG_ERR1 ("Invalid tag: %d", (int) mvluRdVaCtrl->rt->el_tag);
    break;
    }

  mvlu_rd_prim_done (mvluRdVaCtrl, SD_SUCCESS);
  }

/************************************************************************/
/*			mvlu_null_wr_ind				*/
/************************************************************************/

ST_VOID mvlu_null_wr_ind (MVLU_WR_VA_CTRL *mvluWrVaCtrl)
  {
  mvluWrVaCtrl->wrVaCtrl->failure = ARE_OBJ_ACCESS_DENIED;	/* send in rsp*/
  mvlu_wr_prim_done (mvluWrVaCtrl, SD_FAILURE);
  }
