/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	2001-2002, All Rights Reserved				*/
/*									*/
/* MODULE NAME : 							*/
/* PRODUCT(S)  : ICCP							*/
/*									*/
/* MODULE DESCRIPTION :							*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*    None.								*/
/*									*/
/* MODIFICATION LOG :			  				*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------  ---------------------------------------------	*/
/* 07/22/02  GLB     04   Remove type on "#endif"                       */
/* 07/07/02  GLB     03   Make unreferenced code okay for porting       */
/* 04/17/02  MDE     02   Cleanup					*/
/* 06/19/01  DGE     01   Module created.				*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "mi.h"
#include "misem.h"
#include "sx_defs.h"
#include "icfg.h"

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

/************************************************************************/
/************************************************************************/

static ST_RET  _mi_enc_local_cc       (SX_ENC_CTRL *sxEncCtrl);
static ST_VOID _mi_enc_local_objs  (SX_ENC_CTRL *sxEncCtrl);
static ST_VOID _mi_enc_remote_ccs     (SX_ENC_CTRL *sxEncCtrl);
static ST_VOID _mi_enc_associations  (SX_ENC_CTRL *sxEncCtrl);
static ST_VOID _mi_enc_server_objs (SX_ENC_CTRL *sxEncCtrl);
static ST_VOID _mi_enc_client_objs (SX_ENC_CTRL *sxEncCtrl);
static ST_VOID _mi_enc_client_dv (SX_ENC_CTRL *sxEncCtrl, 
				  MIC_GROBJ_DV *grobj_dv, SXE_ATTR_PAIR  *pair);

static ST_VOID _get_AttribPairNum (SXE_ATTR_PAIR *pair, ST_CHAR *aName, ST_LONG nValue);
static ST_VOID _get_AttribPairStr (SXE_ATTR_PAIR *pair, ST_CHAR *aName, ST_CHAR *aValue);

typedef struct
  {
  MI_ASSOC_CTRL *mi_assoc; 
  MIC_GROBJ_CTRL *mi_grobj_ctrl; 
  ST_CHAR *lccName;

  MI_REMOTE *mi_remote; 	/* For convenience */
  MIC_VCC *mic_vcc;
  MIS_VCC *mis_vcc;
  } _MI_ENCODE_REMOTE_CTRL;

static ST_CHAR *unassigned = "<Unassigned>";
	       
/************************************************************************/
/************************************************************************/
/*			mic_encode_remote_xml				*/
/************************************************************************/

ST_RET mic_encode_remote_xml (MI_ASSOC_CTRL *mi_assoc, MIC_GROBJ_CTRL *mi_grobj_ctrl, 
			      ST_CHAR *lccName, ST_CHAR *xmlFileName)
  {
ST_RET		    ret;
SX_ENC_CTRL	   *sxEncCtrl;
_MI_ENCODE_REMOTE_CTRL encCtrl;

  /* Start the Encode	*/
  sxEncCtrl = sx_start_encodeEx (xmlFileName);
  if (sxEncCtrl == NULL)
    ret = SD_FAILURE;
  else
    {
    sxEncCtrl->bUseFormatting = SD_TRUE;
    encCtrl.mi_assoc = mi_assoc;
    encCtrl.mi_remote = mi_assoc->mi_remote;
    encCtrl.mic_vcc = mi_assoc->mi_remote->mic_vcc;
    encCtrl.mis_vcc = mi_assoc->mi_remote->mis_vcc;
    encCtrl.mi_grobj_ctrl = mi_grobj_ctrl;
    encCtrl.lccName = lccName;
    sxEncCtrl->usr = &encCtrl;

    /* Encode the body 	*/
    ret = _mi_enc_local_cc (sxEncCtrl);

    /* Complete the Encode	*/
    ret = sxEncCtrl->errCode;
    sx_end_encode (sxEncCtrl);

    if (ret == SD_SUCCESS)
      ;
    }

  return (ret);
  }


/************************************************************************/
/*			_mi_enc_local_cc	 				*/
/************************************************************************/
static ST_RET _mi_enc_local_cc (SX_ENC_CTRL *sxEncCtrl)
  {
SXE_ATTR_PAIR	    attr[2];
_MI_ENCODE_REMOTE_CTRL *ec;

  ec = (_MI_ENCODE_REMOTE_CTRL *) sxEncCtrl->usr;
  
  /* Write LocalControlCenter to XML File		  */
  attr[0].name  = "xmlns:xsi";
  attr[0].value = "http://www.w3.org/2001/XMLSchema-instance";
  attr[1].name  = "xsi:noNamespaceSchemaLocation";
  attr[1].value = "None"; 
  sx_start_element (sxEncCtrl, "LocalControlCenter", 2, attr);

  /* Write XML */
  sx_wr_string_el (sxEncCtrl, "Name", ec->lccName);
  sx_wr_long_el (sxEncCtrl, "MaxDsTs", 10);
  sx_wr_long_el (sxEncCtrl, "MaxDataSets", 10);
  sx_wr_long_el (sxEncCtrl, "MaxMmsMsgSize", ec->mi_assoc->def_locl_init_info.max_segsize);

  _mi_enc_local_objs (sxEncCtrl);	  /* LocalObjects	*/
  _mi_enc_remote_ccs (sxEncCtrl);	  /* RemoteControlCenters	*/
  
  sx_end_element (sxEncCtrl);	  /* LocalControlCenter	*/
  return (SD_SUCCESS);
  }


/************************************************************************/
/*			_mi_enc_local_objs	 			*/
/************************************************************************/
static ST_VOID _mi_enc_local_objs (SX_ENC_CTRL *sxEncCtrl)
  {
ST_LONG		     numDVs  = 0;
ST_LONG		     numDEVs = 0;
ST_LONG		     numIMs  = 0;
SXE_ATTR_PAIR	    *pair;
ST_CHAR		     pairName[100];
ST_CHAR		     pairValue[100];
_MI_ENCODE_REMOTE_CTRL *ec;

  ec = (_MI_ENCODE_REMOTE_CTRL *) sxEncCtrl->usr;

  /* allocate storage for attribute pairs	*/
  pair = (SXE_ATTR_PAIR *) chk_calloc (1, sizeof (SXE_ATTR_PAIR));
  pair->name = pairName;
  pair->value = pairValue;

  /* write out the LocalObjects Tag	*/
  sx_start_element (sxEncCtrl, "LocalObjects", 0, NULL);

  /* get the number of Local DataValues, Devices and InfoMsgs	*/
  numDVs = 0;
  numDEVs = 0;
  numIMs = 0;

  _get_AttribPairNum (pair, "Count", numDVs);
  sx_start_element (sxEncCtrl, "LocalDataValues", 1, pair);

#if 0
  while (0)
    {
    sx_start_element (sxEncCtrl, "Ldv", 0, NULL);
    sx_wr_string_el (sxEncCtrl, "Name", "");
    sx_wr_string_el (sxEncCtrl, "DataType", "");
    if (0)
      sx_wr_string_el (sxEncCtrl, "MapInfo", "");
    sx_wr_string_el (sxEncCtrl, "NormalSource", "");
    sx_end_element (sxEncCtrl);	/* Ldv	*/
    }
#endif
  sx_end_element (sxEncCtrl);	/* LocalDataValues	*/

  /* Write database table LocalDevices to XML File	  */
  _get_AttribPairNum (pair, "Count", numDEVs);
  sx_start_element (sxEncCtrl, "LocalDevices", 1, pair);

#if 0
  while (0)
    {
    sx_start_element (sxEncCtrl, "Ldev", 0, NULL);
    sx_wr_string_el (sxEncCtrl, "DeviceName", "");
    sx_wr_string_el (sxEncCtrl, "DeviceType", "");
    if (0)
      sx_wr_string_el (sxEncCtrl, "MapInfo", "");
    if (0)
      sx_wr_string_el (sxEncCtrl, "Sbo", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "Sbo", "N");
    if (0)
      sx_wr_int_el  (sxEncCtrl, "ChkBackId", 0);
    if (0)
      sx_wr_long_el (sxEncCtrl, "SelTime", 1);
    if (0)
      sx_wr_string_el (sxEncCtrl, "TagEn", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "TagEn", "N");
    if (0)
      sx_wr_string_el (sxEncCtrl, "Description", "");
    sx_end_element (sxEncCtrl);	/* Ldev	*/
    }
#endif
  sx_end_element (sxEncCtrl);	/* LocalDevices	*/

  /* Write database table LocalInfoMsgs to XML File	  */
  _get_AttribPairNum (pair, "Count", numIMs);
  sx_start_element (sxEncCtrl, "LocalInfoMsgs", 1, pair);
#if 0
  while (0)
    {
    sx_start_element (sxEncCtrl, "Lim", 0, NULL);
    sx_wr_long_el (sxEncCtrl, "InfoRef", 0);
    if (0)
      sx_wr_string_el (sxEncCtrl, "MapInfo", "");
    sx_wr_long_el (sxEncCtrl, "MaxSize", 0);
    if (0)
      sx_wr_string_el (sxEncCtrl, "Description", "");
    sx_end_element (sxEncCtrl);	/* Lim	*/
    }
#endif
  sx_end_element (sxEncCtrl);	/* LocalInfoMsgs	*/
  sx_end_element (sxEncCtrl);	/* LocalObjects	*/
  chk_free (pair);
  }

/************************************************************************/
/*			_mi_enc_remote_ccs	 				*/
/************************************************************************/
static ST_VOID _mi_enc_remote_ccs (SX_ENC_CTRL *sxEncCtrl)
  {
ST_CHAR blocks[50];
ST_LONG			  numRemoteCCs = 0;
ST_LONG			  RemoteCCID = 0;
ST_LONG			  BilatTID = 0;
SXE_ATTR_PAIR		 *pair;
ST_CHAR			  pairName[100];
ST_CHAR			  pairValue[100];
ST_INT i;
ST_CHAR *p;
_MI_ENCODE_REMOTE_CTRL *ec;

  ec = (_MI_ENCODE_REMOTE_CTRL *) sxEncCtrl->usr;

  /* allocate storage for attribute pairs	*/
  pair = (SXE_ATTR_PAIR *) chk_calloc (1, sizeof (SXE_ATTR_PAIR));
  pair->name = pairName;
  pair->value = pairValue;

  /* get count and write out the RemoteControlCenters Tag	*/
  numRemoteCCs = 1;
  _get_AttribPairNum (pair, "Count", numRemoteCCs);
  sx_start_element (sxEncCtrl, "RemoteControlCenters", 1, pair);

  /* Write RemoteControlCenters to the XML File		  */
  for (i = 0; i < 1; ++i)
    {
    RemoteCCID = 0;
    sx_start_element (sxEncCtrl, "RemoteControlCenter", 0, NULL);
    sx_wr_string_el (sxEncCtrl, "Name", ec->mi_remote->name);
    sx_start_element (sxEncCtrl, "BilateralTable", 0, NULL);
    sx_wr_string_el (sxEncCtrl, "Name", "Active");

    p = ec->mic_vcc ? ec->mic_vcc->misv.Bilateral_Table_Id: unassigned;
    sx_wr_string_el (sxEncCtrl, "Id", ec->mic_vcc->misv.Bilateral_Table_Id);

    p = ec->mis_vcc ? ec->mis_vcc->icc_name: unassigned;
    sx_wr_string_el (sxEncCtrl, "LocalDomain", p);

    p = ec->mic_vcc ? ec->mic_vcc->icc_name: unassigned;
    sx_wr_string_el (sxEncCtrl, "RemoteDomain", p);
    sx_wr_long_el (sxEncCtrl, "ShortestInterval", 1);

    strcpy (blocks, "1,2,4,5");
    sx_wr_string_el (sxEncCtrl, "Blocks", blocks);
    sx_end_element (sxEncCtrl);	/* BilateralTable	*/

    /* Encode all Objects for this RemoteControlCenter	  */
    _mi_enc_associations  (sxEncCtrl);
    _mi_enc_server_objs (sxEncCtrl);
    _mi_enc_client_objs (sxEncCtrl);
    sx_end_element (sxEncCtrl);	/* RemoteControlCenter	*/
    }
  sx_end_element (sxEncCtrl);	/* RemoteControlCenters	*/
  chk_free (pair);
  }

/************************************************************************/
/*			_mi_enc_associations				*/
/************************************************************************/
static ST_VOID _mi_enc_associations (SX_ENC_CTRL *sxEncCtrl)
  {
ST_LONG			 numAssociations = 0;
SXE_ATTR_PAIR		*pair;
ST_CHAR			 pairName[100];
ST_CHAR		         pairValue[100];
ST_INT i;
ST_CHAR *p;
_MI_ENCODE_REMOTE_CTRL *ec;
MI_ASSOC_CTRL *mi_assoc; 

  ec = (_MI_ENCODE_REMOTE_CTRL *) sxEncCtrl->usr;
  mi_assoc = ec->mi_assoc;

  /* allocate storage for attribute pairs	*/
  pair = (SXE_ATTR_PAIR *) chk_calloc (1, sizeof (SXE_ATTR_PAIR));
  pair->name = pairName;
  pair->value = pairValue;

  /* get count and write out the Associations Tag	*/
  numAssociations = 1;
  _get_AttribPairNum (pair, "Count", numAssociations);
  sx_start_element (sxEncCtrl, "Associations", 1, pair);

  /* Write Associations to the XML File			  */
  for (i = 0; i < 1; ++i)
    {
    sx_start_element (sxEncCtrl, "Association", 0, NULL);
    sx_wr_string_el (sxEncCtrl, "Name", mi_assoc->name);
    sx_wr_string_el (sxEncCtrl, "LocalAr", mi_assoc->local_ar);
    sx_wr_string_el (sxEncCtrl, "RemoteAr", 
	    mi_assoc->remote_ar[mi_assoc->active_ar_name_index]);

    p = (char *)((mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CALLING) ? "Calling" : "Called");
    sx_wr_string_el (sxEncCtrl, "ConnectRole", p);

    sx_wr_long_el (sxEncCtrl, "AssocRetryTime", mi_assoc->connect_backoff_time);
    if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CLIENT)
      sx_wr_string_el (sxEncCtrl, "ServiceRole", "Client");
    if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_SERVER)
      sx_wr_string_el (sxEncCtrl, "ServiceRole", "Server");

    sx_wr_long_el (sxEncCtrl, "MaxMmsMsgSize", mi_assoc->def_locl_init_info.max_segsize);
    sx_wr_int_el (sxEncCtrl, "MaxReqPend", mi_assoc->def_locl_init_info.maxreq_calling);
    sx_wr_int_el (sxEncCtrl, "MaxIndPend", mi_assoc->def_locl_init_info.maxreq_called);
    sx_wr_int_el (sxEncCtrl, "MaxNest",    mi_assoc->def_locl_init_info.max_nest);
    sx_end_element (sxEncCtrl);	/* Association	*/
    }
  sx_end_element (sxEncCtrl);	/* Associations	*/
  chk_free (pair);
  }

/************************************************************************/
/*			_mi_enc_server_objs				*/
/************************************************************************/
static ST_VOID _mi_enc_server_objs (SX_ENC_CTRL *sxEncCtrl)
  {
ST_LONG		numVccDV  = 0;
ST_LONG		numVccDEV = 0;
ST_LONG		numVccIM  = 0;
ST_LONG		numVccDS  = 0;
ST_LONG		numIccDV  = 0;
ST_LONG		numIccDEV = 0;
ST_LONG		numIccIM  = 0;
ST_LONG		numIccDS  = 0;
ST_LONG		numDSDV   = 0;
SXE_ATTR_PAIR  *pair;
ST_CHAR		pairName[100];
ST_CHAR		pairValue[100];
_MI_ENCODE_REMOTE_CTRL *ec;
#if 0
SXE_ATTR_PAIR   attr[2];
#endif

  ec = (_MI_ENCODE_REMOTE_CTRL *) sxEncCtrl->usr;

  /* allocate storage for attribute pairs	*/
  pair = (SXE_ATTR_PAIR *) chk_calloc (1, sizeof (SXE_ATTR_PAIR));
  pair->name = pairName;
  pair->value = pairValue;

  /* get the number of Server DataValues, Devices and InfoMsgs by scope	*/
  numVccDV =  0;
  numVccDEV = 0;
  numVccIM =  0;
  numVccDS =  0;

  /* set the WHERE Clause to get ICC counts	*/
  numIccDV =  0;
  numIccDEV = 0;
  numIccIM =  0;
  numIccDS =  0;

  /* write out the ServerObjects Tag and counts	*/
  sx_start_element (sxEncCtrl, "ServerObjects", 0, NULL);
  sx_wr_long_el (sxEncCtrl, "NumVccDv", numVccDV);
  sx_wr_long_el (sxEncCtrl, "NumVccDev", numVccDEV);
  sx_wr_long_el (sxEncCtrl, "NumVccInfoMsg", numVccIM);
  sx_wr_long_el (sxEncCtrl, "NumVccDs", numVccDS);
  sx_wr_long_el (sxEncCtrl, "NumIccDv", numIccDV);
  sx_wr_long_el (sxEncCtrl, "NumIccDev", numIccDEV);
  sx_wr_long_el (sxEncCtrl, "NumIccInfoMsg", numIccIM);
  sx_wr_long_el (sxEncCtrl, "NumIccDs", numIccDS);

  
  /* Write database table ServerDataValues to XML File	  */
  _get_AttribPairNum (pair, "Count", numVccDV + numIccDV);
  sx_start_element (sxEncCtrl, "ServerDataValues", 1, pair);
#if 0
  while (0)
    {
    sx_start_element (sxEncCtrl, "Sdv", 0, NULL);
    _get_AttribPairStr (pair, "Scope", "ICC");
    sx_wrx_string_el (sxEncCtrl, "ObjName", "", 1, pair);
    if (0)
      sx_wr_string_el (sxEncCtrl, "DataType", "");
    else
      sx_wr_string_el (sxEncCtrl, "DataType", "");
      sx_wr_string_el (sxEncCtrl, "ReadOnly", "N");
    sx_end_element (sxEncCtrl);	/* Sdv	*/
    }
#endif
  sx_end_element (sxEncCtrl);	/* ServerDataValues	*/

  /* Write database table ServerDevices to XML File	  */
  _get_AttribPairNum (pair, "Count", numVccDEV + numIccDEV);
  sx_start_element (sxEncCtrl, "ServerDevices", 1, pair);
#if 0
  while (0)
    {
    sx_start_element (sxEncCtrl, "Sdev", 0, NULL);
    _get_AttribPairStr (pair, "Scope", "ICC");
    sx_wrx_string_el (sxEncCtrl, "ObjDeviceName", "", 1, pair);
    sx_wr_string_el (sxEncCtrl, "DeviceType", "");
    if (1)
      sx_wr_string_el (sxEncCtrl, "TagEn", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "TagEn", "N");
    sx_end_element (sxEncCtrl);	/* Sdev	*/
    }
#endif
  sx_end_element (sxEncCtrl);	/* ServerDevices	*/

  /* Write database table ServerInfoMsgs to XML File	  */
  _get_AttribPairNum (pair, "Count", numVccIM + numIccIM);
  sx_start_element (sxEncCtrl, "ServerInfoMsgs", 1, pair);
#if 0
  while (0)
    {
    sx_start_element (sxEncCtrl, "Sim", 0, NULL);
    _get_AttribPairStr (pair, "Scope", "ICC");
    sx_wrx_long_el (sxEncCtrl, "ObjInfoRef", 0, 1, pair);
    if (0)
      sx_wr_string_el (sxEncCtrl, "MapInfo", "");
    sx_end_element (sxEncCtrl);	/* Sim	*/
    }
#endif
  sx_end_element (sxEncCtrl);	/* ServerInfoMsgs	*/

  /* Write database table ServerDataSets to XML File	  */
  _get_AttribPairNum (pair, "Count", numVccDS + numIccDS);
  sx_start_element (sxEncCtrl, "ServerDataSets", 1, pair);
#if 0
  while (0)
    {
    sx_start_element (sxEncCtrl, "Sds", 0, NULL);
    _get_AttribPairStr (pair, "Scope", "");
    sx_wrx_string_el (sxEncCtrl, "ObjName", "", 1, pair);
    if (0)
      sx_wr_string_el (sxEncCtrl, "Transfer_Set_Name", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "Transfer_Set_Name", "N");
    if (0)
      sx_wr_string_el (sxEncCtrl, "Transfer_Set_Time_Stamp", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "Transfer_Set_Time_Stamp", "N");
    if (0)
      sx_wr_string_el (sxEncCtrl, "DSConditions_Detected", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "DSConditions_Detected", "N");
    if (0)
      sx_wr_string_el (sxEncCtrl, "Event_Code_Detected", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "Event_Code_Detected", "N");
    if (0)
      sx_wr_string_el (sxEncCtrl, "Description", "");

    /* get the ServerDVsInDSs count	*/
    numDSDV = 0;
    _get_AttribPairNum (pair, "Count", numDSDV);
    sx_start_element (sxEncCtrl, "SdsDvs", 1, pair);

    while (0)
      {
      attr[0].name  = "Scope";
      attr[0].value = "";
      attr[1].name  = "Name";
      attr[1].value = "";
      sx_write_empty_element (sxEncCtrl, "SdsDv", 2, attr);
      }

    sx_end_element (sxEncCtrl);	/* SdsDvs	*/

    sx_end_element (sxEncCtrl);	/* Sds	*/
    }
#endif

  sx_end_element (sxEncCtrl);	/* ServerDataSets	*/
  sx_end_element (sxEncCtrl);	/* ServerObjects	*/
  chk_free (pair);
  }

/************************************************************************/
/*			_mi_enc_client_objs				*/
/************************************************************************/
static ST_VOID _mi_enc_client_objs (SX_ENC_CTRL *sxEncCtrl)
  {
ST_LONG		numVccDV;
ST_LONG		numVccDEV;
ST_LONG		numVccIM;
ST_LONG		numIccDV;
ST_LONG		numIccDEV;
ST_LONG		numIccIM;
ST_LONG		numDS;
ST_LONG		numDSDV = 0;
ST_LONG		numDSTS;
SXE_ATTR_PAIR  *pair;
ST_CHAR		pairName[100];
ST_CHAR	        pairValue[100];
_MI_ENCODE_REMOTE_CTRL *ec;
MIC_GROBJ_CTRL *mi_grobj_ctrl;
MIC_GROBJ_DV *grobj_dv;
#if 0
SXE_ATTR_PAIR   attr[2];
#endif

  ec = (_MI_ENCODE_REMOTE_CTRL *) sxEncCtrl->usr;
  mi_grobj_ctrl = ec->mi_grobj_ctrl;

  /* allocate storage for attribute pairs	*/
  pair = (SXE_ATTR_PAIR *) chk_calloc (1, sizeof (SXE_ATTR_PAIR));
  pair->name = pairName;
  pair->value = pairValue;

  numVccDV = 0;
  grobj_dv = mi_grobj_ctrl->vcc_dv_list;
  while (grobj_dv != NULL)
    {
    if (grobj_dv->mi_type <= NUM_OF_ENUM_DVTYPES)
      ++numVccDV;
    grobj_dv = (MIC_GROBJ_DV *)list_get_next (mi_grobj_ctrl->vcc_dv_list, grobj_dv);
    }
  numIccDV = 0;
  grobj_dv = mi_grobj_ctrl->icc_dv_list;
  while (grobj_dv != NULL)
    {
    if (grobj_dv->mi_type <= NUM_OF_ENUM_DVTYPES)
      ++numIccDV;
    grobj_dv = (MIC_GROBJ_DV *)list_get_next (mi_grobj_ctrl->icc_dv_list, grobj_dv);
    }


  numVccDEV = 0;
  numVccIM = 0;
  numIccDEV = 0;
  numIccIM = 0;
  numDS = 0;
  numDSTS = 0;

   /* write out the ClientObjects Tag and counts	*/
  sx_start_element (sxEncCtrl, "ClientObjects", 0, NULL);
  sx_wr_long_el (sxEncCtrl, "NumVccDv", numVccDV);
  sx_wr_long_el (sxEncCtrl, "NumVccDev", numVccDEV);
  sx_wr_long_el (sxEncCtrl, "NumVccInfoMsg", numVccIM);
  sx_wr_long_el (sxEncCtrl, "NumIccDv", numIccDV);
  sx_wr_long_el (sxEncCtrl, "NumIccDev", numIccDEV);
  sx_wr_long_el (sxEncCtrl, "NumIccInfoMsg", numIccIM);
  sx_wr_long_el (sxEncCtrl, "NumDs", numDS);
  sx_wr_long_el (sxEncCtrl, "NumDsTs", numDSTS);

  /* Write database table ClientDataValues to XML File	  */
  _get_AttribPairNum (pair, "Count", numVccDV + numIccDV);
  sx_start_element (sxEncCtrl, "ClientDataValues", 1, pair);
  grobj_dv = mi_grobj_ctrl->vcc_dv_list;
  while (grobj_dv != NULL)
    {
    _mi_enc_client_dv (sxEncCtrl, grobj_dv, pair);
    grobj_dv = (MIC_GROBJ_DV *)list_get_next (mi_grobj_ctrl->vcc_dv_list, grobj_dv);
    }
  grobj_dv = mi_grobj_ctrl->icc_dv_list;
  while (grobj_dv != NULL)
    {
    _mi_enc_client_dv (sxEncCtrl, grobj_dv, pair);
    grobj_dv = (MIC_GROBJ_DV *)list_get_next (mi_grobj_ctrl->icc_dv_list, grobj_dv);
    }
  sx_end_element (sxEncCtrl);	/* ClientDataValues	*/

  /* Write database table ClientDevices to XML File	  */
  _get_AttribPairNum (pair, "Count", numVccDEV + numIccDEV);
  sx_start_element (sxEncCtrl, "ClientDevices", 1, pair);
#if 0
  while (0)
    {
    sx_start_element (sxEncCtrl, "Cdev", 0, NULL);
    _get_AttribPairStr (pair, "Scope", "ICC");
    sx_wrx_string_el (sxEncCtrl, "ObjDeviceName", "", 1, pair);
    sx_wr_string_el (sxEncCtrl, "DeviceType", "");

    if (0)
      sx_wr_string_el (sxEncCtrl, "MapInfo", "");
    if (0)
      sx_wr_string_el (sxEncCtrl, "Sbo", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "Sbo", "N");
    if (0)
      sx_wr_int_el (sxEncCtrl, "ChkBackId", 0);
    if (0)
      sx_wr_string_el (sxEncCtrl, "TagEn", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "TagEn", "N");
    
    if (0)
      sx_wr_string_el (sxEncCtrl, "Description", "");

    sx_end_element (sxEncCtrl);	/* Cdev	*/
    }
#endif
  sx_end_element (sxEncCtrl);	/* ClientDevices	*/

  /* Write database table ClientInfoMsgs to XML File	  */
  _get_AttribPairNum (pair, "Count", numVccIM + numIccIM);
  sx_start_element (sxEncCtrl, "ClientInfoMsgs", 1, pair);
#if 0
  while (0)
    {
    sx_start_element (sxEncCtrl, "Cim", 0, NULL);
    _get_AttribPairStr (pair, "Scope", "");
    sx_wrx_long_el (sxEncCtrl, "ObjInfoRef", 0, 1, pair);
    if (0)
      sx_wr_string_el (sxEncCtrl, "MapInfo", "");
    sx_wr_long_el (sxEncCtrl, "MaxSize", 0);
    if (0)
      sx_wr_string_el (sxEncCtrl, "Description", "");

    sx_end_element (sxEncCtrl);	/* Cim	*/
    }
#endif
  sx_end_element (sxEncCtrl);	/* ClientInfoMsgs	*/

  /* Write database table ClientDataSets to XML File	  */
  _get_AttribPairNum (pair, "Count", numDS);
  sx_start_element (sxEncCtrl, "ClientDataSets", 1, pair);
#if 0
  while (0)
    {
    sx_start_element (sxEncCtrl, "Cds", 0, NULL);
    sx_wr_string_el (sxEncCtrl, "Name", "");
    if (0)
      sx_wr_string_el (sxEncCtrl, "Transfer_Set_Name", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "Transfer_Set_Name", "N");
    if (0)
      sx_wr_string_el (sxEncCtrl, "Transfer_Set_Time_Stamp", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "Transfer_Set_Time_Stamp", "N");
    if (0)
      sx_wr_string_el (sxEncCtrl, "DSConditions_Detected", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "DSConditions_Detected", "N");
    if (0)
      sx_wr_string_el (sxEncCtrl, "Event_Code_Detected", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "Event_Code_Detected", "N");
    if (0)
      sx_wr_string_el (sxEncCtrl, "Description", "");

    /* get the ClientDVsInDSs count	*/
    _get_AttribPairNum (pair, "Count", numDSDV);
    sx_start_element (sxEncCtrl, "CdsDvs", 1, pair);
    while (0)
      {
      attr[0].name  = "Scope";
      attr[0].value = "";
      attr[1].name  = "Name";
      attr[1].value = "";
      sx_write_empty_element (sxEncCtrl, "CdsDv", 2, attr);
      }
    sx_end_element (sxEncCtrl);	/* CdsDvs	*/
    sx_end_element (sxEncCtrl);	/* Cds	*/
    }
#endif
  sx_end_element (sxEncCtrl);	/* ClientDataSets	*/

  /* Write database table ClientDataSetTransferSets to XML File	  */
  _get_AttribPairNum (pair, "Count", numDSTS);
  sx_start_element (sxEncCtrl, "ClientDataSetTransferSets", 1, pair);
#if 0
  while (0)
    {
    sx_start_element (sxEncCtrl, "Cdsts", 0, NULL);
    sx_wr_string_el (sxEncCtrl, "DsName", "");
    sx_wr_string_el (sxEncCtrl, "AssocName", "");
    sx_wr_int_el (sxEncCtrl, "Interval", 1);

    if (0)
      sx_wr_string_el (sxEncCtrl, "Rbe", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "Rbe", "N");
    if (0)
      sx_wr_int_el (sxEncCtrl, "BufferTime", 1);
    if (0)
      sx_wr_int_el (sxEncCtrl, "Integrity",  1);
    sx_wr_long_el (sxEncCtrl, "StartTime", 1);
    if (0)
      sx_wr_string_el (sxEncCtrl, "Critical", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "Critical", "N");
    if (0)
      sx_wr_string_el (sxEncCtrl, "Description", "");

    sx_end_element (sxEncCtrl);	/* Cdsts	*/
    }
#endif

  sx_end_element (sxEncCtrl);	/* ClientDataSetTransferSets	*/
  sx_end_element (sxEncCtrl);	/* ClientObjects	*/
  chk_free (pair);
  }


/************************************************************************/
/*			_mi_enc_client_dv				*/
/************************************************************************/

static ST_VOID _mi_enc_client_dv (SX_ENC_CTRL *sxEncCtrl, 
				  MIC_GROBJ_DV *grobj_dv, SXE_ATTR_PAIR  *pair)
  {
ST_CHAR *p;

  if (grobj_dv->mi_type <= NUM_OF_ENUM_DVTYPES)
    {
    sx_start_element (sxEncCtrl, "Cdv", 0, NULL);
    _get_AttribPairStr (pair, "Scope", (char *)((grobj_dv->scope == ICC_SPEC) ? "ICC" : "VCC"));
    sx_wrx_string_el (sxEncCtrl, "ObjName", grobj_dv->name, 1, pair);
    if (grobj_dv->mi_type >= 0)
      p = icfgDvTypeValEnum[grobj_dv->mi_type];
    else
      p = "Real";
    sx_wr_string_el (sxEncCtrl, "DataType", p);

#if 0
    if (0)
      sx_wr_string_el (sxEncCtrl, "MapInfo", "");
    if (0)
      sx_wr_string_el (sxEncCtrl, "ReadOnly", "Y");
    else
      sx_wr_string_el (sxEncCtrl, "ReadOnly", "N");
    if (0)
      sx_wr_string_el (sxEncCtrl, "Description", "");
#endif
    sx_end_element (sxEncCtrl);	/* Cdv	*/
    }
  }

/************************************************************************/
/************************************************************************/
/*			_get_AttribPairNum				*/
/************************************************************************/
static ST_VOID _get_AttribPairNum (SXE_ATTR_PAIR *pair, ST_CHAR *aName, ST_LONG nValue)
  {
  strcpy (pair->name, aName);
/*
  ltoa (nValue, pair->value, 10);
*/
/*  strcpy(pair->value, ltoa(nValue));
*/
  sprintf(pair->value,"%d",nValue);
  }

/************************************************************************/
/*			_get_AttribPairStr				*/
/************************************************************************/
static ST_VOID _get_AttribPairStr (SXE_ATTR_PAIR *pair, ST_CHAR *aName, 
				   ST_CHAR *aValue)
  {
  strcpy (pair->name, aName);
  strcpy (pair->value, aValue);
  }

