/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	  	1999-2002, All Rights Reserved                          */
/*									*/
/*		    PROPRIETARY AND CONFIDENTIAL			*/
/*									*/
/* MODULE NAME : mi_size.c						*/
/* PRODUCT(S)  : MI							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR     06    Copyright year update                        */
/* 04/24/02  MDE     05    Changed logging to DEBUG			*/
/* 10/19/01  MDE     04	   Dynamic local object reconfiguration changes	*/
/* 10/19/00  RKR     03    permuted global variable names with mi_	*/
/*			   inc sizes of Extended datatypes		*/
/* 10/04/99  NAV     02    Add IM Message Sizing			*/
/* 03/01/99  MDE     01    New, derived from ms_size.c			*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "mi.h"

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/

#ifdef DEBUG_SISCO
static ST_CHAR *thisFileName = __FILE__;
#endif

/************************************************************************/
/************************************************************************/
/* To allow the user to specify either MMS-EASE or UNKNOWN PDU style,	*/
/* the following definitions & data structures are used.		*/
/* See ms_size.doc for information on how these were derived.		*/

#define MMS_EASE_BUILDER	0
#define UNKNOWN_BUILDER		1

ST_INT mi_req_bld_id  = MMS_EASE_BUILDER;

/* Because the MMS-EASE ASN.1 tools are a bit conservitive, need to 	*/
/* provide for a little extra overhead.					*/
ST_INT mi_size_oh_pad = ASN1_MAX_ELEMENT_OVERHEAD;

ST_LONG mi_med_el_oh[] = 
  {
  4L,			/* MMS-EASE is PDU Builder 			*/
  8L			/* UNKNOWN PDU Builder				*/
  };
ST_LONG mi_small_el_oh[] = 
  {
  2L,			/* MMS-EASE is PDU Builder 			*/
  8L			/* UNKNOWN PDU Builder				*/
  };

#define MED_EL_REQ_OH     mi_med_el_oh[ms_req_bld_id]
#define SMALL_EL_REQ_OH   mi_small_el_oh[ms_req_bld_id]

/* Overhead for various types of MMS ASN.1 data elements		*/
#define UNCONF_PDU_OH	(mi_size_oh_pad + MED_EL_REQ_OH)
#define INFO_REQ_OH	(UNCONF_PDU_OH  + (MED_EL_REQ_OH * 2))

/************************************************************************/
/************************************************************************/
/************************************************************************/

ST_INT mi_dv_asn1_data_size[MI_NUM_MI_TYPES] =
  {
  7,				/* 0  MI_TYPEID_REAL			*/
  4,				/* 1  MI_TYPEID_STATE			*/
  6,				/* 2  MI_TYPEID_DISCRETE		*/
  13,				/* 3  MI_TYPEID_REAL_Q			*/
  4,				/* 4  MI_TYPEID_STATE_Q			*/
  12,				/* 5  MI_TYPEID_DISCRETE_Q		*/
  19,				/* 6  MI_TYPEID_REAL_Q_TIMETAG		*/
  12,				/* 7  MI_TYPEID_STATE_Q_TIMETAG		*/
  18,				/* 8  MI_TYPEID_DISCRETE_Q_TIMETAG	*/
  24,				/* 9  MI_TYPEID_REAL_EXTENDED		*/
  17,				/* 10 MI_TYPEID_STATE_EXTENDED		*/
  23,				/* 11 MI_TYPEID_DISCRETE_EXTENDED	*/
  0,				/* 12 MI_TYPEID_DV_RESERVED_0		*/
  0,				/* 13 MI_TYPEID_DV_RESERVED_1		*/
  0,				/* 14 MI_TYPEID_DV_RESERVED_2		*/
  0,				/* 15 MI_TYPEID_DV_RESERVED_3		*/
  5,				/* 16 MI_TYPEID_SUPPORTED_FEATURES	*/
  34,				/* 17 MI_TYPEID_VISIBLE_STRING_32	*/
  10,				/* 18 MI_TYPEID_TASE2_VERSION		*/
  75,				/* 19 MI_TYPEID_MMS_OBJECT_NAME		*/
  250,				/* 20 MI_TYPEID_DSTS (not correct)	*/
  6,		 		/* 21 MI_TYPEID_GMT_BASED_S		*/	       
  4,				/* 22 MI_TYPEID_INT16			*/
  4				/* 23 MI_TYPEID_DS_COND			*/
  };


ST_INT _mis_calc_info_rpt_size (MIS_INFO_RPT_CTRL *mis_info_ctrl)
  {
MIS_DV_REF *ds_dv_ref;	
MIS_DV *mis_dv;
MIS_DS *mis_ds;
MIS_DSTS *mis_dsts;
MIS_RD_DV_CTRL *mis_rd_dv_ctrl;
ST_INT mi_type;
ST_INT size;
ST_INT icc_name_size;
ST_INT i;
ST_UCHAR bit_mask;
ST_UCHAR *byte_ptr;


  mis_dsts = mis_info_ctrl->mis_dsts;
  mis_ds = mis_dsts->mis_ds;
  icc_name_size = strlen (mis_info_ctrl->mis_vcc->icc_name);
  MI_LOG_DEBUG0 ("Calculating information report size");
  
  size = mi_size_oh_pad; /* ASN.1 encoder padding */
  size += MED_EL_REQ_OH; /* PDU class == unconfirmed   */
  size += MED_EL_REQ_OH; /* info rpt outer constructor */
  MI_LOG_CDEBUG1 ("  Added overhead: %d", size);

/* If the variable specification is a named list, add in the name	*/
  if (mis_info_ctrl->report_type == MIS_REPORT_TYPE_NVL)
    {		/* assume always domain scope */
    size += SMALL_EL_REQ_OH;		/* objname constructor */
    size += SMALL_EL_REQ_OH;		/* domain */
    size += icc_name_size;
    size += SMALL_EL_REQ_OH;		/* ds name */
    size += strlen (mis_ds->ds_name);
    MI_LOG_CDEBUG1 ("  Added NVL name: %d", size);
    }
  else
    {
    size += MED_EL_REQ_OH; 			/* seq of seq */
    MI_LOG_CDEBUG1 ("  Added LOV seq of seq: %d", size);
    }

  size += MED_EL_REQ_OH;	/* seq of data constructor */
  MI_LOG_CDEBUG1 ("  Added seq of data cstr overhead: %d", size);

  ds_dv_ref = mis_ds->mis_dv_ref_tbl;
  bit_mask = 0x80;
  byte_ptr = mis_dsts->changed_dv;
  mis_rd_dv_ctrl =  mis_info_ctrl->mis_rd_dv_ctrl_tbl;
  for (i = 0; i < mis_ds->num_dv_ref; ++i, ++mis_rd_dv_ctrl)
    {
    if ((*byte_ptr & bit_mask) ||
        (mis_rd_dv_ctrl->mi_type > MI_LAST_DV_TYPEID))
      {

    /* If a list of variables, add the size of the va spec for this var  */
      if (mis_info_ctrl->report_type == MIS_REPORT_TYPE_LOV)
        {
        size += SMALL_EL_REQ_OH;	/* seq cstr */
        size += SMALL_EL_REQ_OH;	/* object name cstr */

        if ((mis_rd_dv_ctrl->mis_dv_ref) & MIS_DVREF_ICC)
          {
          size += SMALL_EL_REQ_OH * 2;
          size += icc_name_size;
          }
        size += SMALL_EL_REQ_OH;
        mis_dv = MIS_DV_REF_TO_DV (mis_rd_dv_ctrl->mis_dv_ref);
        size += strlen (mis_dv->dv_name);
        MI_LOG_CDEBUG2 ("  Added DV %d var spec: %d", i, size);
        }

    /* Add the data size */
      mi_type = mis_rd_dv_ctrl->mi_type;
      if (mi_type != MI_TYPEID_MMS_OBJECT_NAME)
        size += mi_dv_asn1_data_size[mi_type];
      else
        {
        size += 9;  /* tags and scope value */
        size += icc_name_size;
        size += strlen (mis_dsts->ts_name);
        }
      MI_LOG_CDEBUG2 ("  Added DV %d data: %d", i, size);
      }

    if (bit_mask == 0x01)
      {
      bit_mask = 0x80;
      ++byte_ptr;
      }
    else
      bit_mask = bit_mask >> 1;
    }
  return (size);
  }

/************************************************************************/
/* _mis_calc_info_msg_size:  determine size of information message	*/
/************************************************************************/
#define MI_EST_BUF_HEADER_ASN1 26

ST_INT _mis_calc_info_msg_size (ST_INT buf_len, 
				ST_INT msg_scope,
				MI_CONN *mi_conn)

  {				
ST_INT size;
ST_INT icc_name_size;
ST_CHAR temp[33];

  icc_name_size = strlen (mi_conn->mi_remote->mis_vcc->icc_name);
  MI_LOG_DEBUG0 ("Calculating information message size");
  
  size = mi_size_oh_pad; /* ASN.1 encoder padding */
  size += MED_EL_REQ_OH; /* PDU class == unconfirmed   */
  size += MED_EL_REQ_OH; /* info rpt outer constructor */
  MI_LOG_CDEBUG1 ("  Added overhead: %d", size);

  size += MED_EL_REQ_OH; 			/* seq of seq */
  MI_LOG_CDEBUG1 ("  Added LOV seq of seq: %d", size);

  size += MED_EL_REQ_OH;	/* seq of data constructor */
  MI_LOG_CDEBUG1 ("  Added seq of data cstr overhead: %d", size);

  /* add the size of the va spec for the Info_Mess_Header var	*/
  size += SMALL_EL_REQ_OH;	/* seq cstr */
  size += SMALL_EL_REQ_OH;	/* object name cstr */

  if (msg_scope == DOM_SPEC)
    {
    size += SMALL_EL_REQ_OH * 2;
    size += icc_name_size;
    }
  size += SMALL_EL_REQ_OH;
  size += strlen (MI_IM_INFO_MESS_HEADER);

  /* Add the data size */
  size += MI_EST_BUF_HEADER_ASN1;

  /* now take care of Info_Buff_XXXX				*/
  /* add the size of the va spec for the Info_Mess_Header var	*/
  size += SMALL_EL_REQ_OH;	/* seq cstr */
  size += SMALL_EL_REQ_OH;	/* object name cstr */

  if (msg_scope == DOM_SPEC)
    {
    size += SMALL_EL_REQ_OH * 2;
    size += icc_name_size;
    }
  sprintf (temp, "%s%d", MI_IM_INFO_BUFF_NAME, buf_len);
  size += SMALL_EL_REQ_OH;
  size += strlen (temp);

  /* Add the data size */
  size += (buf_len + 2);

  return (size);
  }

