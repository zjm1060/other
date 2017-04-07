/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1998 - 2002, All Rights Reserved		        */
/*									*/
/* MODULE NAME : mitypeid.h    						*/
/* PRODUCT(S)  : MI							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR     03    Copyright year update                        */
/* 10/10/99  NAV     02    Add block 4 and 5 support			*/
/* 04/05/99  MDE     01    Created					*/
/************************************************************************/

/* This file is to be included in one and only one user source file,	*/
/* after the Foundry output header file. 				*/
#if !defined(Data_DiscreteExtended_TYPEID)
#pragma message ("Error: ICCP Lite applications ODF files must include 'iccp_obj.odf'")
#endif

ST_INT mi_type_to_type_id_tbl[MI_NUM_MI_TYPES] =
  {
  Data_Real_TYPEID,		/* 0  MI_TYPEID_REAL			*/
  Data_State_TYPEID,		/* 1  MI_TYPEID_STATE			*/
  Data_Discrete_TYPEID,		/* 2  MI_TYPEID_DISCRETE		*/
  Data_RealQ_TYPEID,		/* 3  MI_TYPEID_REAL_Q			*/
  Data_StateQ_TYPEID,		/* 4  MI_TYPEID_STATE_Q			*/
  Data_DiscreteQ_TYPEID,	/* 5  MI_TYPEID_DISCRETE_Q		*/
  Data_RealQTimeTag_TYPEID,	/* 6  MI_TYPEID_REAL_Q_TIMETAG		*/
  Data_StateQTimeTag_TYPEID,	/* 7  MI_TYPEID_STATE_Q_TIMETAG		*/
  Data_DiscreteQTimeTag_TYPEID,	/* 8  MI_TYPEID_DISCRETE_Q_TIMETAG	*/
  Data_RealExtended_TYPEID,	/* 9  MI_TYPEID_REAL_EXTENDED		*/
  Data_StateExtended_TYPEID,	/* 10 MI_TYPEID_STATE_EXTENDED		*/
  Data_DiscreteExtended_TYPEID,	/* 11 MI_TYPEID_DISCRETE_EXTENDED	*/
  -1,				/* 12 MI_TYPEID_DV_RESERVED_0		*/
  -1,				/* 13 MI_TYPEID_DV_RESERVED_1		*/
  -1,				/* 14 MI_TYPEID_DV_RESERVED_2		*/
  -1,				/* 15 MI_TYPEID_DV_RESERVED_3		*/
  SupportedFeatures_TYPEID,	/* 16 MI_TYPEID_SUPPORTED_FEATURES	*/
  Visible_String_32_TYPEID,	/* 17 MI_TYPEID_VISIBLE_STRING_32	*/
  TASE2Version_TYPEID,		/* 18 MI_TYPEID_TASE2_VERSION		*/
  MMSObjectName_TYPEID,		/* 19 MI_TYPEID_MMS_OBJECT_NAME		*/
  DSTransferSet_TYPEID,		/* 20 MI_TYPEID_DSTS			*/
  GMTBasedS_TYPEID, 		/* 21 MI_TYPEID_GMT_BASED_S		*/	       
  Integer16_TYPEID,		/* 22 MI_TYPEID_INT16			*/
  DSConditions_TYPEID		/* 23 MI_TYPEID_DS_COND			*/
  };

ST_INT mi_block4_type_id_tbl[MI_NUM_BLOCK4_TYPES] =
  {
  IMTransferSet_TYPEID,		/*0 MI_TYPEID_IMTRANSET			*/
  InfoMessHeader_TYPEID,	/*1 MI_TYPEID_INFO_HEADER		*/
  InfoBuffXXX_TYPEID		/*2 MI_TYPEID_INFO_BUFF			*/
  };

ST_INT mi_block5_type_id_tbl[MI_NUM_BLOCK5_TYPES] =
  {
  Control_Command_TYPEID,		/*0 MI_TYPEID_DEV_COMMAND	*/
  Control_Setpoint_Discrete_TYPEID,	/*1 MI_TYPEID_DEV_DISCRETE	*/
  Control_Setpoint_Real_TYPEID,		/*2 MI_TYPEID_DEV_REAL		*/
  SBO_CheckBackName_TYPEID,		/*3 MI_TYPEID_DEV_CHECKBACK	*/
  TagValue_TYPEID			/*4 MI_TYPEID_DEV_TAGVALUE	*/
  };

