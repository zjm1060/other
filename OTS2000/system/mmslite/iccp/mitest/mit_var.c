/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*          1996-2002, All Rights Reserved                              */
/*                                                                      */
/* MODULE NAME : mit_var.c                                            	*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*                                                                      */
/* MODULE DESCRIPTION :                                                 */
/*   	Variable handling functions					*/
/*                                                                      */
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev                     Comments                     */
/* --------  ---  ------   -------------------------------------------  */
/* 08/05/02  rkr    21     put some code in u_mic_dv_ind                */
/* 07/25/02  rkr    20     added reorgDs                                */
/* 06/28/02  ask    19     commented out unused displayStateFlags and   */
/*			   iccp_access_to_str, caused warnings under QNX*/
/* 06/26/02  rkr    18     change to u_mis_rd_dv                        */
/* 06/26/02  ask    17     Cleanup for QNX				*/
/* 06/20/02  rkr    16     Copyright year update                        */
/* 06/18/02  rkr    15     Changes for tracking outstanding MMS Reqs	*/
/* 06/11/02  ask    14     use first remote name in getRemoteMitMi*Vcc	*/
/* 06/04/02  rkr    13     fixed state update in iincrementDataValue	*/
/* 03/26/02  ask    12     Added mis_dv_change in modifyLocalDataValue	*/
/* 02/12/02  rkr    11     Worked on createRemoteDataSet		*/
/* 02/11/02  rkr    10     Added numRefDSTS				*/
/* 11/21/01  ASK    9      cleanup for mi lib changes			*/
/* 11/07/01  ask    8      call MIS_DV_REF_TO_DV to update point change */
/*			   table					*/
/* 05/09/01  ask    7      Added change condition to the function	*/
/*			   displayLocalDataValueNames			*/
/* 05/08/01  ask    6      Added modifyLocalDataValueChange		*/
/* 04/12/01  ask    5      Added deleteRemoteDataSet,			*/
/* 			   createRemoteDataSet,				*/
/* 			   getRemoteDataSetElementNames,		*/
/* 			   getRemoteDataValueNames	 		*/
/* 12/15/00  rkr    4      Added logLocalDataSetNames 			*/
/* 08/25/00  rkr    3      Added recovery from failed CreateDS		*/
/* 04/18/00  rkr    2      Added dv_scope to mic_get_dv_attrib 		*/
/* 03/17/99  rkr    1      Created                                      */
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "time.h"
#include "signal.h"

#include "mit_main.h"
#include "str_util.h"

#include "scrndefs.h"
#include "gvaldefs.h"
#include "fkeydefs.h"

/************************************************************************/
/* Global and Local variables						*/
/************************************************************************/

static char *thisFileName = __FILE__;
static ST_BOOLEAN bTimedRead;

/* some defines for determining indication point values of flags	*/
#define IP_VALIDITY_VALID 	0
#define IP_VALIDITY_HELD	1
#define IP_VALIDITY_SUSPECT	2
#define IP_VALIDITY_NOTVALID	3

#define IP_CURR_SRC_TELEMETERED	0
#define IP_CURR_SRC_CALCULATED	1
#define IP_CURR_SRC_ENTERED	2
#define IP_CURR_SRC_ESTIMATED	3

#define IP_NORMAL_VAL_NORMAL	0
#define IP_NORMAL_VAL_ABNORMAL	1

/************************************************************************/
/*      Static Function Declarations                           		*/
/************************************************************************/

static void showStateFlags	(ST_UCHAR stateFlags);
static ST_VOID createDsDone 	(MI_REQ_CTRL *req);
static ST_VOID readDs 		(MIT_ASSOC_INFO *pstMitAssoc,MIT_DATA_SET *pstMitDataSet);
static ST_VOID readDsDone 	(MI_REQ_CTRL *req);
static MIC_DV  **getReadDataValues	(MIC_VCC *vcc, ST_INT *num_dv);
static ST_VOID	getLocalDataValue (ST_CHAR 		*dvName, 
                                   MI_DV_DATA_UNION 	*pData,
                                   ST_INT 		mi_type);
static ST_VOID  readRemoteDataValuesDone (MI_REQ_CTRL *req);
static ST_VOID  writeRemoteDataValuesDone (MI_REQ_CTRL *req);
static ST_VOID  writeRemoteDataSetDone (MI_REQ_CTRL *req);
static ST_VOID  readRemoteDataSetDone (MI_REQ_CTRL *req);
static ST_VOID  getRemoteDataValueAttributesDone (MI_REQ_CTRL *req);
static ST_VOID  getRemoteDataValueNamesDone(MI_REQ_CTRL *req);
static ST_VOID  getRemoteDataSetElementNamesDone(MI_REQ_CTRL *req);
static ST_VOID  createRemoteDataSetDone(MI_REQ_CTRL *req);
static ST_VOID  deleteRemoteDataSetDone(MI_REQ_CTRL *req);

static ST_CHAR	  defaultRemoteName[256];
static ST_BOOLEAN firstRemoteName=SD_TRUE;
static ST_VOID reorgDs (MIT_ASSOC_INFO *pstMitAssoc, MIT_DATA_SET *pstMitDataSet);
static ST_VOID reorgDsDone (MI_REQ_CTRL *req);

/************************************************************************/
/*			getRemoteMitMicVcc				*/
/************************************************************************/

MIT_MIC_VCC	*getRemoteMitMicVcc (ST_CHAR *buffer)
  {
MIT_MIC_VCC	  *pstMitMicVcc;
ST_BOOLEAN	  dataEntered;

  /* Use the first remote until the user supplies one */
  buffer[0]=0;			
  pstMitMicVcc = pstMicVccSet;
  if(pstMitMicVcc->remoteName)
    {
    strcpy(buffer, pstMitMicVcc->remoteName);
    }
  
  if (firstRemoteName==SD_TRUE)
    {
    /* This is the first time an Remote Name is being entered.		*/
    /* When a valid name is entered, it becomes the default.		*/

    printf ("\n Enter Remote Name: (default=%s)",buffer);
    dataEntered = strget (buffer);
    if (dataEntered)
      {
      if ((pstMitMicVcc = findMitMicVcc (buffer))!=NULL)
        {
	strcpy (defaultRemoteName, buffer);
	firstRemoteName=SD_FALSE;
	}			   
      }
    }
  else
    {
    /* There is already an RemoteName being used, it is the 		*/
    /* default supplied to the user and will change when the user 	*/
    /* supplies a different valid name.					*/

    strcpy (buffer, defaultRemoteName);
    printf ("\n Enter Remote Name: (default=%s)",buffer);
    strget (buffer);
    if ((pstMitMicVcc = findMitMicVcc (buffer))!=NULL)
      {
      strcpy (defaultRemoteName, buffer);
      }
    }
  return (pstMitMicVcc);
  }

/************************************************************************/
/*			getRemoteMitMisVcc				*/
/************************************************************************/

MIT_MIS_VCC	*getRemoteMitMisVcc (ST_CHAR *buffer)
  {
MIT_MIS_VCC	  *pstMitMisVcc;
ST_BOOLEAN	  dataEntered;

  /* Use the first remote until the user supplies one */
  buffer[0]=0;			
  pstMitMisVcc = pstMisVccSet;
  if(pstMitMisVcc->remoteName)
    {
    strcpy(buffer, pstMitMisVcc->remoteName);
    }
  
  if (firstRemoteName==SD_TRUE)
    {
    /* This is the first time an Remote Name is being entered.		*/
    /* When a valid name is entered, it becomes the default.		*/

    printf ("\n Enter Remote Name: (default=%s)",buffer);
    dataEntered = strget (buffer);
    if (dataEntered)
      {
      if ((pstMitMisVcc = findMitMisVcc (buffer))!=NULL)
        {
	strcpy (defaultRemoteName, buffer);
	firstRemoteName=SD_FALSE;
	}			   
      }
    }
  else
    {
    /* There is already an RemoteName being used, it is the 		*/
    /* default supplied to the user and will change when the user 	*/
    /* supplies a different valid name.					*/

    strcpy (buffer, defaultRemoteName);
    printf ("\n Enter Remote Name: (default=%s)",buffer);
    strget (buffer);
    if ((pstMitMisVcc = findMitMisVcc (buffer))!=NULL)
      {
      strcpy (defaultRemoteName, buffer);
      }
    }
  return (pstMitMisVcc);
  }

/************************************************************************/
/*                          getReadDataValues				*/
/************************************************************************/

static      MIC_DV   **getReadDataValues (MIC_VCC *vcc, ST_INT *num_dv)
  {
MIC_DV 		**dv;
ST_BOOLEAN 	dataEntered;
ST_CHAR  	dv_name[256];
ST_INT		dv_scope;
MIC_DV		*pstDv;
ST_INT		size;

  *num_dv = 0;
  dv = (MIC_DV **)chk_calloc (1, sizeof(MIC_DV *));
  printf ("\n Enter DataValue Name %d: ", *num_dv+1);
  dataEntered = strget (dv_name);
  printf ("   (0:VCC-Specific, 1:ICC-Specific)");
  printf ("\n Enter DataValue Scope (default = 0) : ");
  dv_scope = 0;
  intget (&dv_scope);
  while (dataEntered)
    {
    pstDv = mic_find_dv (vcc, dv_name, dv_scope); 
    if (pstDv)
      {
      dv[*num_dv] = pstDv;
      (*num_dv)++;
      size = (*num_dv)+1;
      dv = (MIC_DV **)chk_realloc (dv,(size * sizeof(MIC_DV *)));
      }
    else
      {
      printf ("\n DataValue '%s' not found.", dv_name);
      }
    printf ("\n Enter DataValue Name %d: ", *num_dv+1);
    dataEntered = strget (dv_name);
    if (dataEntered)
      {
      printf ("   (0:VCC-Specific, 1:ICC-Specific)");
      printf ("\n Enter DataValue Scope (default = 0) : ");
      dv_scope = 0;
      intget (&dv_scope);
      }
    }
  if (*num_dv==0)		/* we didn't enter any	*/
    {
    chk_free (dv);
    return (NULL);
    }
  else
    {
    return (dv);
    }
  }
    
/************************************************************************/
/*			    readRemoteDataValues			*/
/************************************************************************/

ST_VOID 	readRemoteDataValues	(ST_VOID)
  {
MI_REQ_CTRL *req;
MI_CONN 	*mi_conn;
MIC_DV 		**dv;
ST_CHAR  	buffer[256];
MIT_ASSOC_INFO *pstMitAssoc;
ST_INT 		num_dv;

/* Get the Association Name then find the Association ID			*/

  pstMitAssoc = getAssociationName (buffer);
  if (pstMitAssoc)
    {
    if ((pstMitAssoc->pstMitMicVcc!=NULL)&&
        (pstMitAssoc->pstAssocCtrl->state==MI_ASSOC_STATE_ACTIVE) &&
        (hasReqCapacity(pstMitAssoc)==SD_TRUE))
      {
/* Get the number of DataValues to Read					*/

      dv = getReadDataValues (pstMitAssoc->pstMitMicVcc->pstMicVcc,
                        &num_dv);

/* Issue the Read and set up the user info for the Read Confirm		*/

      if (dv)
        {
        mi_conn = pstMitAssoc->pstAssocCtrl->mi_conn;
        req = mic_read_dv (mi_conn, num_dv, dv);
        chk_free (dv);
        if (req==NULL)
          {
          printf ("\n Read DV failed");
          }
        else
          {
          printf ("\n Read DV issued");
          req->u_req_done = readRemoteDataValuesDone;
          decReqCapacity(pstMitAssoc);
          req->user_info[0]= pstMitAssoc;
          }
        }
      }
    else
      {
      printf ("\n Association Name '%s' not active", buffer);
      pause_msg ("\nPress any key to continue.");
      }
    }
  else
    {
    printf ("\n Association Name '%s' not found", buffer);
    pause_msg ("\nPress any key to continue.");
    }

  (*menu_set_fun)();
  }

/************************************************************************/
/*		       readRemoteDataValuesDone				*/
/************************************************************************/

static ST_VOID  readRemoteDataValuesDone (MI_REQ_CTRL *req)
  {
MIT_ASSOC_INFO *pstMitAssoc;

  /* save this before we free the MI_REQ_CTRL				*/
/*  pstMitAssoc = (Tag_MIT_ASSOC_INFO *)req->user_info[0];
modified 2006.11.24 zy*/
  pstMitAssoc = (struct Tag_MIT_ASSOC_INFO *)req->user_info[0];
  incReqCapacity(pstMitAssoc);
  if (req->result == SD_SUCCESS)
    {
    MIT_LOG_FLOW0 ("ICCP Read DataValue(s) OK");
    mi_free_req_ctrl (req);
    }
  else
    {
    MIT_LOG_ERR1 ("ICCP Read DataValue(s) failed result %d", req->result);
    mi_free_req_ctrl (req);
    }
  }

/************************************************************************/
/*                          getWriteDataValues				*/
/************************************************************************/

static      MIC_WR_DV_CTRL *getWriteDataValues (MIC_VCC *vcc)
  {
ST_BOOLEAN 	dataEntered;
ST_CHAR  	dv_name[256];
ST_INT		dv_scope;
MIC_DV		*pstDv;
ST_INT		num_dv;
ST_INT		dv_type;
MIC_DV_WR_INFO	*pstCurDvWrInfo;
MIC_DV_WR_INFO  *pstDvWrInfo;
MIC_WR_DV_CTRL  *pstDvWrCtrl;

  num_dv = 0;
  pstDvWrInfo = (MIC_DV_WR_INFO *)chk_calloc (1, sizeof(MIC_DV_WR_INFO));
  printf ("\n Enter DataValue Name %d: ", num_dv+1);
  dataEntered = strget (dv_name);
  printf ("   (0:VCC-Specific, 1:ICC-Specific)");
  printf ("\n Enter DataValue Scope (default = 0) : ");
  dv_scope = 0;
  intget (&dv_scope);
  while (dataEntered)
    {
    pstDv = mic_find_dv (vcc, dv_name, dv_scope); 
    if (pstDv)
      {
      /* Calculate the pointer to the current DV_WR_INFO		*/
      pstCurDvWrInfo = pstDvWrInfo + num_dv;
      pstCurDvWrInfo->mic_dv = pstDv;
      pstCurDvWrInfo->wr_data = chk_calloc (1, sizeof(MI_DV_DATA_UNION));
      /* The type of data is masked into the attributes			*/
      dv_type = (pstDv->dv_attrib & MIC_DV_MI_TYPE_MASK);
      getLocalDataValue (dv_name, (MI_DV_DATA_UNION *)pstCurDvWrInfo->wr_data,
                         dv_type);
      num_dv++;
      pstDvWrInfo = (MIC_DV_WR_INFO *)chk_realloc (pstDvWrInfo,
                                  ((num_dv+1) * sizeof(MIC_DV_WR_INFO)));
      }
    else
      {
      printf ("\n DataValue '%s' not found.", dv_name);
      }
    printf ("\n Enter DataValue Name %d: ", num_dv+1);
    dataEntered = strget (dv_name);
    if (dataEntered)
      {
      printf ("   (0:VCC-Specific, 1:ICC-Specific)");
      printf ("\n Enter DataValue Scope (default = 0) : ");
      dv_scope = 0;
      intget (&dv_scope);
      }
    }
  if (num_dv==0)		/* we didn't enter any	*/
    {
    chk_free (pstDvWrInfo);
    return (NULL);
    }
  else
    {
    pstDvWrCtrl = (MIC_WR_DV_CTRL *)chk_calloc (1, sizeof(MIC_WR_DV_CTRL));
    pstDvWrCtrl->num_dv = num_dv;
    pstDvWrCtrl->mic_dv_wr_info = pstDvWrInfo;
    return (pstDvWrCtrl);
    }
  }

/************************************************************************/
/*			    writeRemoteDataValues			*/
/************************************************************************/

ST_VOID 	writeRemoteDataValues	(ST_VOID)
  {
MI_REQ_CTRL 	*req;
MI_CONN 	*mi_conn;
ST_CHAR  	buffer[256];
MIT_ASSOC_INFO *pstMitAssoc;
MIC_WR_DV_CTRL 	*mic_wr_dv_ctrl;

/* Get the Association Name then find the Association ID			*/

  pstMitAssoc = getAssociationName (buffer);
  if (pstMitAssoc)
    {
    if ((pstMitAssoc->pstMitMicVcc!=NULL)&&
        (pstMitAssoc->pstAssocCtrl->state==MI_ASSOC_STATE_ACTIVE)&&
        (hasReqCapacity(pstMitAssoc)==SD_TRUE))
      {
      mic_wr_dv_ctrl = getWriteDataValues (pstMitAssoc->pstMitMicVcc->pstMicVcc);
      if (mic_wr_dv_ctrl!=NULL)
        {
        mi_conn = pstMitAssoc->pstAssocCtrl->mi_conn;
        req = mic_write_dv (mi_conn, mic_wr_dv_ctrl);
        if (req==NULL)
          {
          MIT_LOG_FLOW0 ("Write DV failed");
          }
        else
          {
          MIT_LOG_FLOW0 ("Write DV issued");
          req->u_req_done = writeRemoteDataValuesDone;
          decReqCapacity(pstMitAssoc);
          req->user_info[0] = pstMitAssoc;
          }
        }
      }
    else
      {
      printf ("\n Association Name '%s' not active", buffer);
      pause_msg ("Press any key to continue.");
      }
    }
  else
    {
    printf ("\n Association Name '%s' not found", buffer);
    pause_msg ("Press any key to continue.");
    }
  (*menu_set_fun)();
  }

/************************************************************************/
/*			       freeWrDvCtrl				*/
/************************************************************************/

static  ST_VOID	freeWrDvCtrl (MIC_WR_DV_CTRL *mic_wr_dv_ctrl)
  {
ST_INT		num_dv;
MIC_DV_WR_INFO	*pstCurDvWrInfo;
ST_INT		i;

  num_dv = mic_wr_dv_ctrl->num_dv;
  pstCurDvWrInfo = mic_wr_dv_ctrl->mic_dv_wr_info;
  for (i=0; i<num_dv; i++)
    {
    chk_free (pstCurDvWrInfo->wr_data);
    pstCurDvWrInfo++;
    }
  chk_free (mic_wr_dv_ctrl->mic_dv_wr_info);
  chk_free (mic_wr_dv_ctrl);
  }

/************************************************************************/
/*		       getRemoteDataValueAttributes			*/
/************************************************************************/

ST_VOID 	getRemoteDataValueAttributes (ST_VOID)
  {
ST_CHAR		dv_name[255];
MI_REQ_CTRL 	*req;
MI_CONN 	*mi_conn;
ST_CHAR  	buffer[256];
MIT_ASSOC_INFO *pstMitAssoc;
ST_BOOLEAN 	dataEntered;
ST_INT		dv_scope;

  pstMitAssoc = getAssociationName (buffer);
  if (pstMitAssoc)
    {
    if ((pstMitAssoc->pstMitMicVcc!=NULL)&&
        (pstMitAssoc->pstAssocCtrl->state==MI_ASSOC_STATE_ACTIVE)&&
        (hasReqCapacity(pstMitAssoc)==SD_TRUE))
      {
      printf ("\n Enter DataValue Name : ");
      dataEntered = strget (dv_name);
      printf ("   (0:VCC-Specific, 1:ICC-Specific)");
      printf ("\n Enter DataValue Scope (default = 0) : ");
      dv_scope = 0;
      intget (&dv_scope);
      if (dataEntered)
        {
        mi_conn = pstMitAssoc->pstAssocCtrl->mi_conn;
        req = mic_get_dv_attrib (mi_conn, dv_name, dv_scope);
        if (req==NULL)
          {
          MIT_LOG_FLOW0 ("Get DataValue Attrib failed");
          }
        else
          {
          MIT_LOG_FLOW0 ("Get DataValue Attrib issued");
          req->u_req_done = getRemoteDataValueAttributesDone;
          req->user_info[0] = pstMitAssoc;
          decReqCapacity(pstMitAssoc);
          }
        }
      }
    else
      {
      printf ("\n Association Name '%s' not active", buffer);
      pause_msg ("Press any key to continue.");
      }
    }
  else
    {
    printf ("\n Association Name '%s' not found", buffer);
    pause_msg ("Press any key to continue.");
    }
  (*menu_set_fun)();
  }

/************************************************************************/
/*		       getRemoteDataValueAttributesDone 		*/
/************************************************************************/

static ST_VOID  getRemoteDataValueAttributesDone(MI_REQ_CTRL *req)
  {
MIT_ASSOC_INFO	*pstMitAssoc;

  
  pstMitAssoc = (struct Tag_MIT_ASSOC_INFO *)req->user_info[0];
  incReqCapacity(pstMitAssoc);
  /* save this before we free the MI_REQ_CTRL				*/
  if (req->result == SD_SUCCESS)
    {
    printf ("\nICCP Get Data Value Attributes OK");
    mi_free_req_ctrl (req);
    }
  else
    {
    printf ("\nICCP Get Data Value Attributes failed result %d", req->result);
    mi_free_req_ctrl (req);
    }
  }

/************************************************************************/
/*		       getRemoteDataValueNames								 		*/
/************************************************************************/

ST_VOID getRemoteDataValueNames(ST_VOID)
  { 
MI_REQ_CTRL 	*req;
MI_CONN 	*mi_conn;
ST_CHAR  	buffer[256];
ST_CHAR		ca_name[256];
MIT_ASSOC_INFO *pstMitAssoc;
ST_INT		dv_scope;

  pstMitAssoc = getAssociationName (buffer);
  
  if (pstMitAssoc)
    {
    if ((pstMitAssoc->pstMitMicVcc!=NULL)&&
        (pstMitAssoc->pstAssocCtrl->state==MI_ASSOC_STATE_ACTIVE)&&
        (hasReqCapacity(pstMitAssoc)==SD_TRUE))
      {
      printf ("   (0:VCC-DV, 1:ICC-DV, 2:VCC-DS, 3:ICC-DS)");
      printf ("\n Enter DataValue Scope (default = 0) : ");
      dv_scope = 0;
      intget (&dv_scope);
      if ((dv_scope <0) || (dv_scope >3))
        {
        printf ("\n Illegal scope");
        pause_msg ("\n Press any key to continue.");
        return;
        }
  
      printf ("\n Enter an optional DataValue to continue after : ");
      memset(ca_name, 0, 1);
      strget(ca_name);

      mi_conn = pstMitAssoc->pstAssocCtrl->mi_conn;
      
      if(isalpha(ca_name[0]))
	{  
	req = mic_get_namelist (mi_conn, dv_scope, ca_name);
	}
      else
	{
	req = mic_get_namelist (mi_conn, dv_scope, NULL);
	}
      
      if (req == NULL)
        {
        MIT_LOG_FLOW0 ("Get DataValue Names failed");
        }
      else
        {
        MIT_LOG_FLOW0 ("Get DataValue Names issued");
        req->u_req_done = getRemoteDataValueNamesDone;
        req->user_info[0] = pstMitAssoc;
        decReqCapacity(pstMitAssoc);
	}
      }
    else
      {
      printf ("\n Association Name '%s' not active", buffer);
      pause_msg ("\n Press any key to continue.");
      }
    }
  else
    {
    printf ("\n Association Name '%s' not found", buffer);
    pause_msg ("\n Press any key to continue.");
    }

(*menu_set_fun)();
}

/************************************************************************/
/*		       getRemoteDataValueNamesDone 		        */
/************************************************************************/	    
static ST_VOID  getRemoteDataValueNamesDone(MI_REQ_CTRL *req)
  {
  ST_INT i;
  ST_INT line;
  ST_CHAR **str;
  ST_CHAR ans;
  MIT_ASSOC_INFO  *pstMitAssoc;

  str = (ST_CHAR **)(req->o.gnl + 1);
  line = 1;

  pstMitAssoc = (struct Tag_MIT_ASSOC_INFO *)req->user_info[0];
  incReqCapacity(pstMitAssoc);

  /* save this before we free the MI_REQ_CTRL */
  if (req->result == SD_SUCCESS)
    {
    printf ("\nICCP Get Data Value Names OK");
    
    printf ("\nNumber of Names in this PDU: %d", req->o.gnl->num_names);
    printf ("\nName(s):  ");
    
    for(i = 0; i < req->o.gnl->num_names; i++, str++)
      {
      printf("\n %s", *str);
      
      line++;
      if (line == 24)
        {
        printf("\n\n Press any key to continue or 'q' to stop displaying\n");
        ans = get_a_char();

	if(ans == 'q' || ans == 'Q')
	  i = req->o.gnl->num_names;  
           
        line = 1;
        }
      }

    
    pause_msg ("\n Press any key to continue.");
    mi_free_req_ctrl (req);
    }
  else
    {
    printf ("\nICCP Get Data Value Names failed result %d", req->result);
    mi_free_req_ctrl (req);
    }
  
  (*menu_set_fun)();

  }

/************************************************************************/
/*			  getRemoteDataSetElementNames			*/
/************************************************************************/

ST_VOID getRemoteDataSetElementNames(ST_VOID)
  {

MI_REQ_CTRL 	*req;
MI_CONN 	*mi_conn;
ST_BOOLEAN 	dataEntered;
ST_CHAR  	buffer[256];
MIT_ASSOC_INFO *pstMitAssoc;
MIT_MIC_VCC 	*pstMitMicVcc;
ST_INT		dv_scope;
  
  pstMitAssoc = getAssociationName (buffer);
  if ((pstMitAssoc!=NULL)&&
      (pstMitAssoc->pstAssocCtrl->state==MI_ASSOC_STATE_ACTIVE))
    {
    pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
    if (pstMitMicVcc!=NULL)
      {
      printf("\n Enter DataSet Name: ");
      dataEntered = strget (buffer);
      if (dataEntered)
        {
        printf ("   (0:VCC-Specific, 1:ICC-Specific)");
      	printf ("\n Enter DataValue Scope (default = 0) : ");
      	dv_scope = 0;
      	intget (&dv_scope);
          
        mi_conn = pstMitAssoc->pstAssocCtrl->mi_conn;
        req = mic_get_ds_names (mi_conn, buffer, dv_scope); 
        if (req==NULL)
          {
          MIT_LOG_ERR0("Get DS Element Names failed");
          }
        else
          {
          MIT_LOG_FLOW0 ("Get DS Element Names issued");
          req->u_req_done = getRemoteDataSetElementNamesDone;
          }
        }
      }
    else
      {
      printf ("\n Association Name '%s' not found", buffer);
      }
    }
  (*menu_set_fun)();
  }

/************************************************************************/
/*		       getRemoteDataSetElementNamesDone 								*/
/************************************************************************/	    
static ST_VOID  getRemoteDataSetElementNamesDone(MI_REQ_CTRL *req)
  {
  /* save this before we free the MI_REQ_CTRL				*/
  if (req->result == SD_SUCCESS)
    {
    printf ("\nICCP Get Data Set Element Names OK");
    mi_free_req_ctrl (req);
    }
  else
    {
    printf ("\nICCP Get Data Set Element Names failed result %d", req->result);
    mi_free_req_ctrl (req);
    }
  }

/************************************************************************/
/*			  createRemoteDataSet										*/
/************************************************************************/

ST_VOID createRemoteDataSet(ST_VOID)
  {
  MI_CONN 	   *mi_conn;
  MIT_ASSOC_INFO   *pstMitAssoc;
  MIT_MIC_VCC	   *pstMitMicVcc;
  MIT_DATA_SET	   *pstCurMicVccDataSet;    
  MIT_DATA_POINT   *pdv;
  MIT_DATA_POINT   *pdvSav;
  ST_INT  	   ds_scope = 0;
  ST_CHAR 	   ds_name[256];
  ST_INT	   dv_scope = 0;
  ST_CHAR	   buffer[256];
  ST_BOOLEAN	   dataEntered;
  ST_INT	   i;
  ST_INT	   numPoints;
  MIC_VCC	   *mic_vcc;
  MIC_DV           **dv;
  MIT_DATA_POINT   *pstCurDataPoint;
  MIC_DS	   *ds;
  MI_REQ_CTRL	   *req;

  pstMitAssoc = getAssociationName (buffer);

  if (pstMitAssoc!=NULL)
    {
      if((pstMitAssoc->pstAssocCtrl->state==MI_ASSOC_STATE_ACTIVE)&&
          (hasReqCapacity(pstMitAssoc)==SD_TRUE))
        {
        mi_conn = pstMitAssoc->pstAssocCtrl->mi_conn;
        
        printf("\n Enter DataSet Name to be created: ");
        strget (ds_name);

	/* check to see if it exists */

	if ((pstCurMicVccDataSet = findMitDataSet(pstMitAssoc->pstMitMicVcc->pstDataSets,ds_name))!=NULL)
	  {
	  if (pstCurMicVccDataSet->numRefDSTS>0)
	    {
            printf ("\n DataSet Name '%s' in use by %d DSTS", ds_name,pstCurMicVccDataSet->numRefDSTS);
            pause_msg ("\n Press a key to continue");
            (*menu_set_fun)();  
	    return;
	    }
	  }

        
        pstCurMicVccDataSet = (MIT_DATA_SET *)chk_calloc (1, sizeof(MIT_DATA_SET));
        pstCurMicVccDataSet->numPoints = 0;

        
      	strcpy(pstCurMicVccDataSet->dsName, ds_name);
	pstCurMicVccDataSet->dsScope = ICC_SPEC;

	printf("\n Enter Data Value #%d or '0' to stop adding DVs: ", pstCurMicVccDataSet->numPoints + 1);
      	dataEntered = strget(buffer);
      	
      	while(strcmp(buffer, "0") && dataEntered)
      	  { 
          printf ("\n (0:VCC-Specific, 1:ICC-Specific)");
          printf ("\n Enter Data Value Scope (default = %d) : ", dv_scope);
          
          intget (&dv_scope);
	        	  
    	  pdv = (MIT_DATA_POINT *) chk_calloc (1, sizeof(MIT_DATA_POINT));
    	  list_add_last ((ST_VOID **)&pstCurMicVccDataSet->pointList, pdv);

    	  strcpy (pdv->pointName, buffer);
	  pdv->scope = dv_scope;

	  pstCurMicVccDataSet->numPoints++;

      	  printf("\n Enter Data Value #%d or '0' to stop adding DVs: ", pstCurMicVccDataSet->numPoints + 1);
      	  dataEntered = strget(buffer);
      	  }		  
  
#if 1

        mic_vcc = mi_conn->mi_remote->mic_vcc;
        numPoints = pstCurMicVccDataSet->numPoints;
        dv = (MIC_DV **)chk_calloc (numPoints, sizeof(MIC_DV *));
        pstCurDataPoint = pstCurMicVccDataSet->pointList;
/*        display_mic_dv_table (mic_vcc);
*/
        for (i=0; i<numPoints; i++)
          {
          dv[i] = mic_find_dv (mic_vcc, 
                               pstCurDataPoint->pointName, 
        		       pstCurDataPoint->scope);
          if (dv[i]==NULL)
            {
            MIT_LOG_ERR2 ( "DataValue '%s' not found in DataSet '%s'",
                            pstCurDataPoint->pointName,
                            pstCurMicVccDataSet->dsName);
            chk_free (dv);

            /* also free the point list */

    	    pdv = pstCurMicVccDataSet->pointList;
	    while(pdv)
	      {
	      pdvSav = pdv;
	      pdv = (struct Tag_MIT_DATA_POINT *)list_get_next(pstCurMicVccDataSet->pointList,pdv);
              list_unlink(pstCurMicVccDataSet->pointList, pdvSav);
	      chk_free(pdvSav);
	      }
            }
	  else
	    {
      	    list_add_last ((ST_VOID **)&pstMitAssoc->pstMitMicVcc->pstDataSets, pstCurMicVccDataSet);
	    }
        
          pstCurDataPoint = (MIT_DATA_POINT *)list_get_next(pstCurMicVccDataSet->pointList,
         						      pstCurDataPoint);
          }
        
	/* Create a Data Set in the server */
        req = mic_create_ds (mi_conn, 
                             pstCurMicVccDataSet->dsName,
                             pstCurMicVccDataSet->dsScope, 
                             pstCurMicVccDataSet->numPoints, 
        		     dv, 
        		     &ds);
        if (!req)
          {
          MIT_LOG_ERR1 ("Could not create DataSet '%s'", pstCurMicVccDataSet->dsName);
          }
        else
          {
          req->u_req_done = createRemoteDataSetDone;  
          req->user_info[0] = pstMitAssoc;
          req->user_info[1] = pstCurMicVccDataSet;
          decReqCapacity(pstMitAssoc);
          pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
          pstCurMicVccDataSet->pstDS = (ST_VOID *)ds;
          }
        chk_free (dv);
#else
  	pstMitAssoc->pstMitMicVcc->pstNextDataSetToStart = pstCurMicVccDataSet;
  	createDataSets(pstMitAssoc); 
#endif
        
        }
      else
        {
        printf ("\n Association Name '%s' not active", buffer);
        pause_msg ("\n Press a key to continue");
        }
    }
  else
    {
    printf ("\n Association Name '%s' not found", buffer);
    pause_msg ("\n Press a key to continue ");
    } 
    
  (*menu_set_fun)();  
  } 

/************************************************************************/
/*			  createRemoteDataSetDone									*/
/************************************************************************/

static ST_VOID  createRemoteDataSetDone(MI_REQ_CTRL *req)
  {
MIT_ASSOC_INFO	*pstMitAssoc;
MIT_DATA_SET    *pstMitDataSet;

    pstMitAssoc = (struct Tag_MIT_ASSOC_INFO *)req->user_info[0];
    pstMitDataSet = (struct Tag_MIT_DATA_SET *)req->user_info[1];
    incReqCapacity(pstMitAssoc);

    if(req->result == SD_SUCCESS)
      {
      MIT_LOG_FLOW1("\nICCP Create DS '%s' OK",pstMitDataSet->dsName);
      mi_free_req_ctrl (req);
      }
    else
      {
      MIT_LOG_ERR1("\nICCP Create DS '%s' Error",pstMitDataSet->dsName);
      mi_free_req_ctrl (req);
      }
  }

/************************************************************************/
/*			  deleteRemoteDataSet										*/
/************************************************************************/

ST_VOID deleteRemoteDataSet(ST_VOID)
  {
  MI_CONN 		*mi_conn;
  MIC_VCC 		*mic_vcc;
  MI_REQ_CTRL 		*req;
  MIC_DS 		*ds;
  MIT_ASSOC_INFO	*pstMitAssoc;
  ST_CHAR 		ds_name[256];
  ST_CHAR		buffer[256];
  MIT_DATA_SET	*pstMitDataSet;
      
  pstMitAssoc = getAssociationName (buffer);

  if (pstMitAssoc!=NULL)
    {
      if((pstMitAssoc->pstAssocCtrl->state==MI_ASSOC_STATE_ACTIVE)&&
         (hasReqCapacity(pstMitAssoc)==SD_TRUE))
        {
        mi_conn = pstMitAssoc->pstAssocCtrl->mi_conn;
        mic_vcc = mi_conn->mi_remote->mic_vcc;

	printf("\n Enter the DataSet to be deleted: ");
        strget (ds_name);

	pstMitDataSet = findMitDataSet (pstMitAssoc->pstMitMicVcc->pstDataSets, ds_name);
	if (pstMitDataSet && pstMitDataSet->numRefDSTS==0)
	  {
	  ds = (struct _mic_ds *)pstMitDataSet->pstDS;
          req = mic_delete_ds(mi_conn, ds);
		
          if(!req)
	    {
	    printf ("\n Error! Could not delete the dataset");
            MIT_LOG_ERR0("Delete DS failed");
   	    pause_msg ("\n Press a key to continue ");
	    }
        
          else
            {
            MIT_LOG_FLOW0 ("Delete DS issued");
            req->u_req_done = deleteRemoteDataSetDone;
	    pstMitAssoc->pstMitMicVcc->DataSetDeleteing = pstMitDataSet;
            req->user_info[0] = pstMitAssoc;
            decReqCapacity(pstMitAssoc);
	    }
	  }
	else
	  {
          MIT_LOG_ERR0("Delete DS failed");
          MIT_LOG_ERR1("DS Name: %s not found, or in use by DSTS", ds_name);
          printf("\n DS %s not found, unable to delete", ds_name);
    	  pause_msg ("\n Press a key to continue ");
	  }
        }
    
      else
        {
        printf ("\n Association Name '%s' not active", buffer);
        pause_msg ("\n Press a key to continue ");
        }
    }
  
  else
    {
    printf ("\n Association Name '%s' not found", buffer);
    pause_msg ("\n Press a key to continue ");
    }
  
  (*menu_set_fun)();
  }

/************************************************************************/
/*			  deleteRemoteDataSetDone									*/
/************************************************************************/

static ST_VOID  deleteRemoteDataSetDone(MI_REQ_CTRL *req)
  {
MIT_MIC_VCC		*pstMitMicVcc;
MIT_ASSOC_INFO 	*pstMitAssoc;
MIT_DATA_SET	*pstMitDataSet;
MIT_DATA_POINT	*pstCurPoint;
MIT_DATA_POINT	*savePtr;

  pstMitAssoc = (struct Tag_MIT_ASSOC_INFO *)req->user_info[0];
  incReqCapacity(pstMitAssoc);
  if (req->result == SD_SUCCESS)
    {
    printf("\n ICCP Delete DS OK");

    /* get a pointer to the Data set					*/

    pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
    pstMitDataSet = pstMitMicVcc->DataSetDeleteing;

    /* Delete the point list						*/

    pstCurPoint = pstMitDataSet->pointList;
    while (pstCurPoint)
      {
      savePtr = pstCurPoint;
      pstCurPoint = (MIT_DATA_POINT *)list_get_next 
                          (pstMitDataSet->pointList, pstCurPoint);
      list_unlink(&pstMitDataSet->pointList, savePtr);
      chk_free(savePtr);
      }
      /* delete the DataSet and adjust the overhead			*/

    list_unlink(&pstMitMicVcc->pstDataSets, pstMitDataSet);
    chk_free(pstMitDataSet);
    pstMitMicVcc->numDataSets--;
    pstMitMicVcc->numStartedDataSets--;

    mi_free_req_ctrl (req);
    }
  else
    {
    printf ("\nICCP Delete DS Failed result %d", req->result);
    mi_free_req_ctrl (req);
    }
  }

/************************************************************************/
/*			  writeRemoteDataValuesDone			*/
/************************************************************************/

static ST_VOID  writeRemoteDataValuesDone (MI_REQ_CTRL *req)
  {
ST_INT		num_dv;
MIC_DV_WR_INFO	*pstCurDvWrInfo;
ST_INT		i;
MIT_ASSOC_INFO	*pstMitAssoc;

  pstMitAssoc = (struct Tag_MIT_ASSOC_INFO *)req->user_info[0];
  incReqCapacity(pstMitAssoc);
 
  /* save this before we free the MI_REQ_CTRL				*/
  if (req->result == SD_SUCCESS)
    {
    printf ("\nICCP Write DataValues OK");
    num_dv = req->mic_wr_dv_ctrl->num_dv;
    pstCurDvWrInfo = req->mic_wr_dv_ctrl->mic_dv_wr_info;
    printf ("\n\nwriteRemoteDataValueDone: processing Write Results");
    for (i=0; i<num_dv; i++)
      {
      printf ("\n Write Result %d, %d",i+1, pstCurDvWrInfo->result);
      pstCurDvWrInfo++;
      }
    freeWrDvCtrl (req->mic_wr_dv_ctrl);
    mi_free_req_ctrl (req);
    }
  else
    {
    printf ("\nICCP Write DataValues failed result %d", req->result);
    freeWrDvCtrl (req->mic_wr_dv_ctrl);
    mi_free_req_ctrl (req);
    }
  }

/************************************************************************/
/*			     iccp_scope_to_str 				*/
/************************************************************************/

static ST_CHAR *vccStr ="VCC_SPEC";
static ST_CHAR *iccStr ="ICC_SPEC";
static ST_CHAR *unknownStr ="<Unknown>";

ST_CHAR  *iccp_scope_to_str (ST_INT scope)
  {

  if (scope==VCC_SPEC)
    return (vccStr);
  if (scope==ICC_SPEC)
    return (iccStr);
  return (unknownStr);
  }

/************************************************************************/
/*			     iccp_type_to_str				*/
/************************************************************************/
static ST_CHAR *realStr = "MI_REAL";
static ST_CHAR *stateStr = "MI_STATE";
static ST_CHAR *discreteStr = "MI_DISCRETE";
static ST_CHAR *realQStr = "MI_REAL_Q";
static ST_CHAR *stateQStr = "MI_STATE_Q";
static ST_CHAR *discreteQStr = "MI_DISCRETE_Q";
static ST_CHAR *realQTimetagStr = "MI_REAL_Q_TIMETAG";
static ST_CHAR *stateQTimetagStr = "MI_STATE_Q_TIMETAG";
static ST_CHAR *discreteQTimetagStr = "MI_DISCRETE_Q_TIMETAG";
static ST_CHAR *realExtendedStr = "MI_REAL_EXTENDED";
static ST_CHAR *stateExtendedStr = "MI_STATE_EXTENDED";
static ST_CHAR *discreteExtendedStr = "MI_DISCRETE_EXTENDED";
static ST_CHAR *mmsObjectNameStr = "MI_MMS_OBJECT_NAME";
static ST_CHAR *gmtBasedSStr = "MI_GMT_BASED_S";
static ST_CHAR *dsCondStr= "MI_DS_COND";

static ST_CHAR  *iccp_type_to_str (ST_INT type) 
  {

  if (type==MI_TYPEID_REAL)
    return (realStr);
  if (type==MI_TYPEID_STATE)
    return (stateStr);
  if (type==MI_TYPEID_DISCRETE)
    return (discreteStr);
  if (type==MI_TYPEID_REAL_Q)
    return (realQStr);
  if (type==MI_TYPEID_STATE_Q)
    return (stateQStr);
  if (type==MI_TYPEID_DISCRETE_Q)
    return (discreteQStr);
  if (type==MI_TYPEID_REAL_Q_TIMETAG)
    return (realQTimetagStr);
  if (type==MI_TYPEID_STATE_Q_TIMETAG)
    return (stateQTimetagStr);
  if (type==MI_TYPEID_DISCRETE_Q_TIMETAG)
    return (discreteQTimetagStr);
  if (type==MI_TYPEID_REAL_EXTENDED)
    return (realExtendedStr);
  if (type==MI_TYPEID_STATE_EXTENDED)
    return (stateExtendedStr);
  if (type==MI_TYPEID_DISCRETE_EXTENDED)
    return (discreteExtendedStr);
  if (type==MI_TYPEID_MMS_OBJECT_NAME)
    return (mmsObjectNameStr);
  if (type==MI_TYPEID_GMT_BASED_S)
    return (gmtBasedSStr);
  if (type==MI_TYPEID_DS_COND)
    return (dsCondStr);
  return (unknownStr);
  }

/************************************************************************/
/*			    iccp_access_to_str				*/
/************************************************************************
static ST_CHAR	*readableStr ="Readable";
static ST_CHAR	*writableStr ="Writable";
static ST_CHAR  *iccp_access_to_str (ST_INT access) 
  {
  if (access!=0)
    return (writableStr);
  else
    return (readableStr);
  }
*/
/************************************************************************/
/*			   logLocalDataSetNames			        */
/************************************************************************/

ST_VOID		logLocalDataSetNames (ST_VOID)
  {
ST_CHAR  	buffer[256];
MIT_MIS_VCC 	*pstMitMisVcc;
MIS_VCC         *pstMisVcc;
ST_INT          i;
MIS_DS          **ds_ctrl_tbl;

  pstMitMisVcc = getRemoteMitMisVcc (buffer);
  if (pstMitMisVcc)
    {						     
    pstMisVcc = pstMitMisVcc->pstMisVcc;

    /* log VCC_SPEC DataSetNames */
    
    ds_ctrl_tbl = pstMisVcc->vcc_ds_ctrl_tbl;
    MIT_LOG_ALWAYS1 ("%d VCC_SPEC DataSets",pstMisVcc->num_vcc_ds);
    for (i=0; i<pstMisVcc->num_vcc_ds; i++ )
      {
      MIT_LOG_CALWAYS2 ("DataSet '%s', num of DataValues %d",
                        ds_ctrl_tbl[i]->ds_name, ds_ctrl_tbl[i]->num_dv_ref );
      }
    /* log ICC_SPEC DataSetNames */
    
    ds_ctrl_tbl = pstMisVcc->icc_ds_ctrl_tbl;
    MIT_LOG_ALWAYS1 ("%d ICC_SPEC DataSets",pstMisVcc->num_icc_ds);
    for (i=0; i<pstMisVcc->num_icc_ds; i++ )
      {
      MIT_LOG_CALWAYS2 ("DataSet '%s', num of DataValues %d",
                        ds_ctrl_tbl[i]->ds_name, ds_ctrl_tbl[i]->num_dv_ref );
      }
    pause_msg (" DataSet Names logged to file");
    }
  else
    {
    printf ("\n Remote '%s' not found", buffer);
    pause_msg (" ");
    }			
  (*menu_set_fun)();
  }

/************************************************************************/
/*			 displayRemoteDataValueNames			*/
/************************************************************************/

ST_VOID		displayRemoteDataValueNames (ST_VOID)
  {
ST_CHAR  	buffer[256];
MIT_MIC_VCC 	*pstMitMicVcc;
MIT_DVREF_INFO	*pstCurDataValue;
ST_CHAR		*strScope;
ST_CHAR		*strType;
ST_INT		line;

  pstMitMicVcc = getRemoteMitMicVcc (buffer);
  if (pstMitMicVcc)
    {						     
    pstCurDataValue = pstMitMicVcc->pstDvRefSet;
    printf ("\n Remote Domain: '%s' BLTID: '%s'\n",
             pstMitMicVcc->remoteDomainName,
	     pstMitMicVcc->bilateralTableId);
    line = 1;
    while (pstCurDataValue)
      {
      strScope = iccp_scope_to_str(pstCurDataValue->scope);
      strType = iccp_type_to_str(pstCurDataValue->type);
      printf ("\n DvName %s, Scope %s, Type %s",
       	      pstCurDataValue->dvName,
	      strScope,
	      strType);
      line++;
      if (line == 22)
        {
        pause_msg ("\n\n Press any key to continue");
        printf ("\n Remote Domain: '%s' BLTID: '%s'\n",
                pstMitMicVcc->remoteDomainName,
	        pstMitMicVcc->bilateralTableId);
        line =1;
        }
      pstCurDataValue = (struct Tag_MIT_DVREF_INFO *)list_get_next (pstMitMicVcc->pstDvRefSet,
                                       pstCurDataValue);
      }
    pause_msg ("\n\n Press any key to continue");
    }
  else
    {
    printf ("\n Remote '%s' not found", buffer);
    pause_msg (" ");
    }			
  (*menu_set_fun)();
  }

/************************************************************************/
/* 			     readRemoteDataSet				*/
/************************************************************************/

ST_VOID 	readRemoteDataSet	(ST_VOID)
  {
MI_REQ_CTRL 	*req;
MI_CONN 	*mi_conn;
ST_BOOLEAN 	dataEntered;
ST_CHAR  	buffer[256];
MIT_ASSOC_INFO *pstMitAssoc;
MIC_DS 		*ds;

/* Get the Association Name then find the Association ID			*/

  pstMitAssoc = getAssociationName (buffer);
  if (pstMitAssoc)
    {
    if ((pstMitAssoc->pstMitMicVcc!=NULL)&&
        (pstMitAssoc->pstAssocCtrl->state==MI_ASSOC_STATE_ACTIVE)&&
        (hasReqCapacity(pstMitAssoc)==SD_TRUE))
      {
      printf("\n Enter DataSet Name: ");
      dataEntered = strget (buffer);
      if (dataEntered)
        {
        if ((ds = (struct _mic_ds *)findDataSet (pstMitAssoc->pstMitMicVcc->pstDataSets,
                               buffer)) != NULL)
          {
          mi_conn = pstMitAssoc->pstAssocCtrl->mi_conn;
          req = mic_read_ds (mi_conn, ds); 
          if (req==NULL)
            {
            MIT_LOG_ERR0("Read DS failed");
            }
          else
            {
            MIT_LOG_FLOW0 ("Read DS issued");
            req->u_req_done = readRemoteDataSetDone;
            decReqCapacity(pstMitAssoc);
            req->user_info[0] = pstMitAssoc;
            }
          }
        else
          {
          printf ("\n DataSet Name '%s' not found", buffer);
          }
        }
      }
    }
  else
    {
    printf ("\n Association Name '%s' not found", buffer);
    }
  (*menu_set_fun)();
  }

/************************************************************************/
/*			  readRemoteDataSetDone				*/
/************************************************************************/

static ST_VOID  readRemoteDataSetDone (MI_REQ_CTRL *req)
  {
MIT_ASSOC_INFO	*pstMitAssoc;

  pstMitAssoc = (struct Tag_MIT_ASSOC_INFO *)req->user_info[0];
  incReqCapacity(pstMitAssoc);

  /* save this before we free the MI_REQ_CTRL				*/
  if (req->result == SD_SUCCESS)
    {
    MIT_LOG_FLOW0 ("ICCP Read DataSet OK");
    mi_free_req_ctrl (req);
    }
  else
    {
    MIT_LOG_ERR1 ("ICCP Read DataSet failed result %d", req->result);
    mi_free_req_ctrl (req);
    }
  }

/************************************************************************/
/*			   getWriteDataSetValues			*/
/************************************************************************/

static MIC_WR_DV_CTRL *getWriteDataSetValues (MIC_VCC	*vcc,
					      MIT_DATA_SET *pstMitDataSet)
  {
MIC_DV		*pstDv;
ST_INT		num_dv;
ST_INT		dv_type;
MIC_DV_WR_INFO	*pstCurDvWrInfo;
MIC_DV_WR_INFO  *pstDvWrInfo;
MIC_WR_DV_CTRL  *pstDvWrCtrl;
MIT_DATA_POINT	*pstCurMitDataPoint;

  num_dv = 0;
  pstDvWrInfo = (MIC_DV_WR_INFO *)chk_calloc (pstMitDataSet->numPoints, 
                                              sizeof(MIC_DV_WR_INFO));
  pstCurMitDataPoint = pstMitDataSet->pointList;
  while (pstCurMitDataPoint && (num_dv < pstMitDataSet->numPoints))
    {

    pstDv = mic_find_dv (vcc, pstCurMitDataPoint->pointName, 
                              pstCurMitDataPoint->scope); 
    if (pstDv)
      {
      /* Calculate the pointer to the current DV_WR_INFO		*/
      pstCurDvWrInfo = pstDvWrInfo + num_dv;
      pstCurDvWrInfo->mic_dv = pstDv;
      pstCurDvWrInfo->wr_data = chk_calloc (1, sizeof(MI_DV_DATA_UNION));
      /* We already have the ICCP type in the MIT_DATA_POINT struct but	*/
      /* getting it this way is just as good.				*/
      dv_type = (pstDv->dv_attrib & MIC_DV_MI_TYPE_MASK);
      printf ("\n (%d) Enter %s data for %s",num_dv+1,
         				     iccp_type_to_str (dv_type),
					     pstCurMitDataPoint->pointName);
      getLocalDataValue (pstCurMitDataPoint->pointName, 
                         (MI_DV_DATA_UNION *)pstCurDvWrInfo->wr_data,
                         dv_type);
      num_dv++;
      }
    else
      {
      printf ("\n DataValue '%s' not found.", pstCurMitDataPoint->pointName);
      }
    pstCurMitDataPoint = (struct Tag_MIT_DATA_POINT *)list_get_next(pstMitDataSet->pointList,
                                       pstCurMitDataPoint);
    }
  if (num_dv==0)		/* we didn't enter any	*/
    {
    chk_free (pstDvWrInfo);
    return (NULL);
    }
  else
    {
    pstDvWrCtrl = (MIC_WR_DV_CTRL *)chk_calloc (1, sizeof(MIC_WR_DV_CTRL));
    pstDvWrCtrl->num_dv = num_dv;
    pstDvWrCtrl->mic_dv_wr_info = pstDvWrInfo;
    return (pstDvWrCtrl);
    }
  }

/************************************************************************/
/*			    writeRemoteDataSet				*/
/************************************************************************/

ST_VOID 	writeRemoteDataSet	(ST_VOID)
  {
MI_REQ_CTRL 	*req;
MI_CONN 	*mi_conn;
ST_BOOLEAN 	dataEntered;
ST_CHAR  	buffer[256];
MIT_ASSOC_INFO *pstMitAssoc;
MIC_WR_DV_CTRL 	*mic_wr_dv_ctrl;
MIT_DATA_SET	*pstMitDataSet;
MIT_MIC_VCC	*pstMitMicVcc;

/* Get the Association Name then find the Association ID			*/

  pstMitAssoc = getAssociationName (buffer);
  if ((pstMitAssoc!=NULL)&&
      (pstMitAssoc->pstAssocCtrl->state==MI_ASSOC_STATE_ACTIVE)&&
      (hasReqCapacity(pstMitAssoc)==SD_TRUE))
    {
    pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
    if (pstMitMicVcc!=NULL)
      {
      printf("\n Enter DataSet Name: ");
      dataEntered = strget (buffer);
      if (dataEntered)
        {
        pstMitDataSet = findMitDataSet (pstMitAssoc->pstMitMicVcc->pstDataSets,
                                        buffer);
        if (pstMitDataSet!=NULL)
          {                
          mic_wr_dv_ctrl = getWriteDataSetValues (pstMitMicVcc->pstMicVcc,
    					          pstMitDataSet);
          if (mic_wr_dv_ctrl!=NULL)
            {
            mi_conn = pstMitAssoc->pstAssocCtrl->mi_conn;
            req = mic_write_ds (mi_conn, (struct _mic_ds *)pstMitDataSet->pstDS, 
 		                mic_wr_dv_ctrl);
            if (req==NULL)
              {
              printf ("\n Write DS failed");
              }
            else
              {
              printf ("\n Write DS issued");
              req->u_req_done = writeRemoteDataSetDone;
              req->user_info[0] = pstMitAssoc;
              decReqCapacity(pstMitAssoc);
              }
            }
          }
        }
      }
    else
      {
      printf ("\n Association Name '%s' not active", buffer);
      pause_msg (" ");
      }
    }
  else
    {
    printf ("\n Association Name '%s' not found", buffer);
    pause_msg (" ");
    }
  (*menu_set_fun)();
  }

/************************************************************************/
/*			  writeRemoteDataSetDone			*/
/************************************************************************/

static ST_VOID  writeRemoteDataSetDone (MI_REQ_CTRL *req)
  {
ST_INT		num_dv;
MIC_DV_WR_INFO	*pstCurDvWrInfo;
ST_INT		i;
MIT_ASSOC_INFO	*pstMitAssoc;

  pstMitAssoc = (struct Tag_MIT_ASSOC_INFO *)req->user_info[0];
  incReqCapacity(pstMitAssoc);

  /* save this before we free the MI_REQ_CTRL				*/
  if (req->result == SD_SUCCESS)
    {
    MIT_LOG_CNF0 ("ICCP Write DataSet OK");
    num_dv = req->mic_wr_dv_ctrl->num_dv;
    pstCurDvWrInfo = req->mic_wr_dv_ctrl->mic_dv_wr_info;
    printf ("\n\nwriteRemoteDataSetDone: processing Write Results");
    for (i=0; i<num_dv; i++)
      {
      MIT_LOG_CCNF2 ("  Write Result %d, %d",i+1, pstCurDvWrInfo->result);
      pstCurDvWrInfo++;
      }
    freeWrDvCtrl (req->mic_wr_dv_ctrl);
    mi_free_req_ctrl (req);
    }
  else
    {
    MIT_LOG_ERR1 ("ICCP Write DataSet failed result %d", req->result);
    freeWrDvCtrl (req->mic_wr_dv_ctrl);
    mi_free_req_ctrl (req);
    }
  }

/************************************************************************/
/* 			       showState				*/
/************************************************************************/

static ST_CHAR *showState(ST_UCHAR state)
  {
static ST_CHAR	showBuffer[1024];
  
  showBuffer[0] =0;
  
  if((state & MI_DATA_STATE_MASK) == MI_DATA_STATE_BETWEEN)
    strcat (showBuffer, "State: BETWEEN ");				  	
  if((state & MI_DATA_STATE_MASK) == MI_DATA_STATE_TRIPPED)	 
    strcat (showBuffer, "State: TRIPPED ");			       	  		
  if((state & MI_DATA_STATE_MASK) == MI_DATA_STATE_CLOSED)			      		  	
    strcat (showBuffer, "State: CLOSED ");				  	
  
  if((state & MI_QFLAG_VALIDITY_MASK) == MI_QFLAG_VALIDITY_VALID)			     	  
    strcat (showBuffer, "Validity: VALID ");		       	  
  if((state & MI_QFLAG_VALIDITY_MASK) == MI_QFLAG_VALIDITY_HELD)				      	
    strcat (showBuffer, "Validity: HELD ");			  
  if((state & MI_QFLAG_VALIDITY_MASK) == MI_QFLAG_VALIDITY_SUSPECT)				  
    strcat (showBuffer, "Validity: SUSPECT ");			  
  if((state & MI_QFLAG_VALIDITY_MASK) == MI_QFLAG_VALIDITY_NOTVALID)				  
    strcat (showBuffer, "Validity: NOTVALID ");			  
  								  
  if((state & MI_QFLAG_CURR_SOURCE_MASK) == MI_QFLAG_CURR_SOURCE_TELEMETERED)		  	  
   strcat (showBuffer, "NormalSource: TELEMETERED ");		  
  if((state & MI_QFLAG_CURR_SOURCE_MASK) == MI_QFLAG_CURR_SOURCE_CALCULATED)			  	
   strcat (showBuffer, "NormalSource: CALCULATED ");		  			
  if((state & MI_QFLAG_CURR_SOURCE_MASK) == MI_QFLAG_CURR_SOURCE_ENTERED)
   strcat (showBuffer, "NormalSource: ENTERED ");		
  if((state & MI_QFLAG_CURR_SOURCE_MASK) == MI_QFLAG_CURR_SOURCE_ESTIMATED)
   strcat (showBuffer, "NormalSource: ESTIMATED ");		
  
  if((state & MI_QFLAG_NORMAL_VAL_MASK) == MI_QFLAG_NORMAL_VAL_NORMAL)		     	
   strcat (showBuffer, "NormalValue: NORMAL ");	     
  if((state & MI_QFLAG_NORMAL_VAL_MASK) == MI_QFLAG_NORMAL_VAL_ABNORMAL)
   strcat (showBuffer, "NormalValue: ABNORMAL ");	
  
  if((state & MI_QFLAG_TIMESTAMP_MASK) == MI_QFLAG_TIMESTAMP_VALID)			     	
   strcat (showBuffer, "TimeStampQuality: VALID ");		 
  if((state & MI_QFLAG_TIMESTAMP_MASK) == MI_QFLAG_TIMESTAMP_INVALID)
   strcat (showBuffer, "TimeStampQuality: INVALID ");	
  
  return (showBuffer);
  }

/************************************************************************/
/* 			       prtState				*/
/************************************************************************/

static void prtState(ST_UCHAR state)
  {
  if((state & MI_DATA_STATE_MASK) == MI_DATA_STATE_BETWEEN)
    printf("State: BETWEEN ");				  	
  if((state & MI_DATA_STATE_MASK) == MI_DATA_STATE_TRIPPED)	 
    printf("State: TRIPPED ");			       	  		
  if((state & MI_DATA_STATE_MASK) == MI_DATA_STATE_CLOSED)			      		  	
    printf("State: CLOSED ");				  	
  
  if((state & MI_QFLAG_VALIDITY_MASK) == MI_QFLAG_VALIDITY_VALID)			     	  
    printf("Validity: VALID ");		       	  
  if((state & MI_QFLAG_VALIDITY_MASK) == MI_QFLAG_VALIDITY_HELD)				      	
    printf("Validity: HELD ");			  
  if((state & MI_QFLAG_VALIDITY_MASK) == MI_QFLAG_VALIDITY_SUSPECT)				  
    printf("Validity: SUSPECT ");			  
  if((state & MI_QFLAG_VALIDITY_MASK) == MI_QFLAG_VALIDITY_NOTVALID)				  
    printf("Validity: NOTVALID ");			  
  								  
  if((state & MI_QFLAG_CURR_SOURCE_MASK) == MI_QFLAG_CURR_SOURCE_TELEMETERED)		  	  
   printf("NormalSource: TELEMETERED ");		  
  if((state & MI_QFLAG_CURR_SOURCE_MASK) == MI_QFLAG_CURR_SOURCE_CALCULATED)			  	
   printf("NormalSource: CALCULATED ");		  			
  if((state & MI_QFLAG_CURR_SOURCE_MASK) == MI_QFLAG_CURR_SOURCE_ENTERED)
   printf("NormalSource: ENTERED ");		
  if((state & MI_QFLAG_CURR_SOURCE_MASK) == MI_QFLAG_CURR_SOURCE_ESTIMATED)
   printf("NormalSource: ESTIMATED ");		
  
  if((state & MI_QFLAG_NORMAL_VAL_MASK) == MI_QFLAG_NORMAL_VAL_NORMAL)		     	
   printf("NormalValue: NORMAL ");	     
  if((state & MI_QFLAG_NORMAL_VAL_MASK) == MI_QFLAG_NORMAL_VAL_ABNORMAL)
   printf("NormalValue: ABNORMAL ");	
  
  if((state & MI_QFLAG_TIMESTAMP_MASK) == MI_QFLAG_TIMESTAMP_VALID)			     	
   printf("TimeStampQuality: VALID ");		 
  if((state & MI_QFLAG_TIMESTAMP_MASK) == MI_QFLAG_TIMESTAMP_INVALID)
   printf("TimeStampQuality: INVALID ");	
  }

/************************************************************************/
/*			    prtLocalDataValue				*/
/************************************************************************/

static ST_VOID	prtLocalDataValue (ST_CHAR 		*dvName, 
                                   MI_DV_DATA_UNION 	*pData,
                                   ST_INT 		mi_type)
  {
MI_REAL 		*dr;
MI_STATE 	   	*ds;
MI_DISCRETE 	   	*dd;
MI_REAL_Q 	   	*drq;
MI_STATE_Q 	   	*dsq;
MI_DISCRETE_Q 	   	*ddq;
MI_REAL_Q_TIMETAG 	*drqt;
MI_STATE_Q_TIMETAG    	*dsqt;
MI_DISCRETE_Q_TIMETAG 	*ddqt;
MI_REAL_EXTENDED 	*dre;
MI_STATE_EXTENDED    	*dse;
MI_DISCRETE_EXTENDED 	*dde;
MI_MMS_OBJECT_NAME 	*ts_name;
MI_TS_TIMESTAMP    	*ts_timestamp;
MI_DS_CONDITIONS   	*ds_cond;
MI_EC_DETECTED     	*ec_detected;
ST_CHAR 		timeBuf[30];
  
  switch (mi_type)
    {
    case MI_TYPEID_REAL:
      dr = &pData->r;
      printf ("\n Value: %f", *dr);
    break;
  
    case MI_TYPEID_STATE:
      ds = &pData->s;
      printf ("\n ");
      prtState(*ds);
      break;
  
    case MI_TYPEID_DISCRETE:
      dd = &pData->d;
      printf ("\n Value: %ld", *dd);
    break;
  
    case MI_TYPEID_REAL_Q:
      drq = &pData->rq;
      printf ("\n Value: %f", drq->Value);
      printf ("\n Flags: %02x", (int) drq->Flags);
    break;
  
    case MI_TYPEID_STATE_Q:
      dsq = &pData->sq;
      printf ("\n ");
      prtState(*dsq);
    break;
  
    case MI_TYPEID_DISCRETE_Q:
      ddq = &pData->dq;
      printf ("\n Value: %ld", ddq->Value);
      printf ("\n Flags: %02x", (int) ddq->Flags);
    break;
  
    case MI_TYPEID_REAL_Q_TIMETAG:
      drqt = &pData->rqt;
      printf ("\n Value: %f", drqt->Value);
      strcpy (timeBuf, ctime(&drqt->TimeStamp));
      timeBuf[24] = 0;
      printf ("\n Timestamp: %s", timeBuf);
      printf ("\n Flags: %02x", (int) drqt->Flags);
    break;
  
    case MI_TYPEID_STATE_Q_TIMETAG:
      dsqt = &pData->sqt;
      strcpy (timeBuf, ctime(&dsqt->TimeStamp));
      timeBuf[24] = 0;
      printf ("\n Timestamp: %s", timeBuf);
      printf ("\n Flags: %02x", (int) dsqt->Flags);
    break;
  
    case MI_TYPEID_DISCRETE_Q_TIMETAG:
      ddqt = &pData->dqt;
      printf ("\n Value: %ld", ddqt->Value);
      strcpy (timeBuf, ctime(&ddqt->TimeStamp));
      timeBuf[24] = 0;
      printf ("\n Timestamp: %s", timeBuf);
      printf ("\n Flags: %02x", (int) ddqt->Flags);
    break;
  
    case MI_TYPEID_REAL_EXTENDED:
      dre = &pData->re;
      printf ("\n Value: %f", dre->Value);
      strcpy (timeBuf, ctime(&dre->TimeStamp));
      timeBuf[24] = 0;
      printf ("\n Timestamp: %s", timeBuf);
      printf ("\n COV_Counter: %d", (int)dre->COV);
      printf ("\n Flags: %02x", (int) dre->Flags);
    break;
  
    case MI_TYPEID_STATE_EXTENDED:
      dse = &pData->se;
      strcpy (timeBuf, ctime(&dse->TimeStamp));
      timeBuf[24] = 0;
      printf ("\n Timestamp: %s", timeBuf);
      printf ("\n COV_Counter: %d", (int)dse->COV);
      printf ("\n Flags: %02x", (int) dse->Flags);
    break;
  
    case MI_TYPEID_DISCRETE_EXTENDED:
      dde = &pData->de;
      printf ("\n Value: %ld", dde->Value);
      strcpy (timeBuf, ctime(&dde->TimeStamp));
      timeBuf[24] = 0;
      printf ("\n Timestamp: %s", timeBuf);
      printf ("\n COV_Counter: %d", (int)dde->COV);
      printf ("\n Flags: %02x", (int) dde->Flags);
    break;

    case MI_TYPEID_MMS_OBJECT_NAME:
      ts_name = &pData->ts_name;
      printf ("\nTransfer Set Name: %s", ts_name->Name);
      if (ts_name->Scope == 1)
        printf (", Domain: %s", ts_name->DomainName);
    break;

    case MI_TYPEID_GMT_BASED_S:
      ts_timestamp = &pData->ts_timestamp;
      printf ("\nTransfer Set Time Stamp: %ld", (long) *ts_timestamp);
    break;

    case MI_TYPEID_INT16:
      ec_detected = &pData->ec_detected;
      printf ("\nEvent Code Detected: 0x%02x", (int) *ec_detected);
    break;

    case MI_TYPEID_DS_COND:
      ds_cond = &pData->ds_cond;
      printf ("\nDSConditions Detected: 0x%04x", (int) *ds_cond);
    break;
  
    default:
      printf ("\nError - Check Me Out!");
    break;
    }
  }

/************************************************************************/
/*			  displayLocalDataValue				*/
/************************************************************************/

ST_VOID 	displayLocalDataValue	(ST_VOID)
  {
ST_CHAR  	dvname[256];
ST_BOOLEAN 	dataEntered;
MIS_DV_REF 	dv_ref;
MIS_DV		*pstDv;
MIT_DATA_VALUE	*pstMitDataValue;
ST_VOID		*pData;
ST_INT		ret;

  printf("\n Enter local DataValue Name: ");
  dataEntered = strget (dvname);
  if (dataEntered)
    {
    ret = mis_find_dv (dvname, &dv_ref);
    if (ret==SD_SUCCESS)
      {
      dv_ref = (dv_ref & MIS_INDEX_MASK);
      pstDv = (MIS_DV *)&mis_dv_ctrl[dv_ref];   /* table of ptrs to DV tables	*/
      pstMitDataValue = (struct Tag_MIT_DATA_VALUE *)pstDv->access_info;
      pData = &pstMitDataValue->data;
      prtLocalDataValue (pstMitDataValue->dvName, (struct _mi_dv_data_union *)pData,
                         pstMitDataValue->dvType);
      pause_msg("\n\n Press any key to continue\n");
      }
    else
      {
      printf ("\n\n DataValue '%s', not found ret = %d", dvname, ret);
      }
    }
  (*menu_set_fun)();
  }

/************************************************************************/
/* Functions for assigning keyboard input to standard ICCP data types	*/
/************************************************************************/

static ST_VOID      	getReal (MI_REAL *r)
  {
  printf (" Float Value: ");
  floatget (r);
  }
static ST_VOID      	getState (ST_UCHAR *s)
  {
  printf (" State: ");
  input_hex (s, 1);
  }
static ST_VOID		getDiscrete (MI_DISCRETE *d)
  {
  printf (" Long Value: ");
  longget (d);
  }
static ST_VOID		getRealQ (MI_REAL_Q *rq)
  {
  printf (" Float Value: ");
  floatget (&rq->Value);
  printf (" Flags: ");
  input_hex (&rq->Flags, 1);
  }
static ST_VOID		getStateQ (ST_UCHAR	*sq)
  {
  printf (" State: ");
  input_hex (sq, 1);
  }
static ST_VOID		getDiscreteQ (MI_DISCRETE_Q *dq)
  {
  printf (" Long Value: ");
  longget (&dq->Value);
  printf (" Flags: ");
  input_hex (&dq->Flags, 1);
  }
static ST_VOID		getRealQTimetag (MI_REAL_Q_TIMETAG *rqt)
  {
  printf (" Float Value: ");
  floatget (&rqt->Value);
  printf (" Long Timestamp: ");
  longget (&rqt->TimeStamp);
  printf (" Flags: ");
  input_hex (&rqt->Flags, 1);
  }
static ST_VOID		getStateQTimetag (MI_STATE_Q_TIMETAG *sqt)
  {
  printf (" Long Timestamp: ");
  longget (&sqt->TimeStamp);
  printf (" Flags: ");
  input_hex (&sqt->Flags, 1);
  }
static ST_VOID		getDiscreteQTimetag (MI_DISCRETE_Q_TIMETAG *dqt)
  {
  printf (" Long Value: ");
  longget (&dqt->Value);
  printf (" Long Timestamp: ");
  longget (&dqt->TimeStamp);
  printf (" Flags: ");
  input_hex (&dqt->Flags, 1);
  }
static ST_VOID		getRealExtended (MI_REAL_EXTENDED *re)
  {
ST_INT			tempint;

  printf (" Float Value: ");
  floatget (&re->Value);
  printf (" Long Timestamp: ");
  longget (&re->TimeStamp);
  printf (" Ushort COV_Counter: ");
  intget (&tempint);
  re->COV = (ST_UINT16) tempint;
  printf (" Flags: ");
  input_hex (&re->Flags, 1);
  }
static ST_VOID		getStateExtended (MI_STATE_EXTENDED *se)
  {
ST_INT			tempint;

  printf (" Long Timestamp: ");
  longget (&se->TimeStamp);
  printf (" Flags: ");
  input_hex (&se->Flags, 1);
  printf (" Ushort COV_Counter: ");
  intget (&tempint);
  se->COV = (ST_UINT16) tempint;
  }
static ST_VOID		getDiscreteExtended (MI_DISCRETE_EXTENDED *de)
  {
ST_INT			tempint;

  printf (" Long Value: ");
  longget (&de->Value);
  printf (" Long Timestamp: ");
  longget (&de->TimeStamp);
  printf (" Flags: ");
  input_hex (&de->Flags, 1);
  printf (" Ushort COV_Counter: ");
  intget (&tempint);
  de->COV = (ST_UINT16) tempint;
  }
/************************************************************************/
/*			    getLocalDataValue				*/
/************************************************************************/

static ST_VOID	getLocalDataValue (ST_CHAR 		*dvName, 
                                   MI_DV_DATA_UNION 	*pData,
                                   ST_INT 		mi_type)
  {
  
  switch (mi_type)
    {
    case MI_TYPEID_REAL:
      getReal (&pData->r);
    break;
  
    case MI_TYPEID_STATE:
      getState ((ST_UCHAR *)&pData->s);
    break;
  
    case MI_TYPEID_DISCRETE:
      getDiscrete (&pData->d);
    break;
  
    case MI_TYPEID_REAL_Q:
      getRealQ (&pData->rq);
    break;
  
    case MI_TYPEID_STATE_Q:
      getStateQ ((ST_UCHAR *)&pData->sq);
    break;
  
    case MI_TYPEID_DISCRETE_Q:
      getDiscreteQ (&pData->dq);
    break;
  
    case MI_TYPEID_REAL_Q_TIMETAG:
      getRealQTimetag (&pData->rqt);
    break;
  
    case MI_TYPEID_STATE_Q_TIMETAG:
      getStateQTimetag (&pData->sqt);
    break;
  
    case MI_TYPEID_DISCRETE_Q_TIMETAG:
      getDiscreteQTimetag (&pData->dqt);
    break;
  
    case MI_TYPEID_REAL_EXTENDED:
      getRealExtended (&pData->re);
    break;
  
    case MI_TYPEID_STATE_EXTENDED:
      getStateExtended (&pData->se);
    break;
  
    case MI_TYPEID_DISCRETE_EXTENDED:
      getDiscreteExtended (&pData->de);
    break;

#if 0
    case MI_TYPEID_MMS_OBJECT_NAME:
      ts_name = &pData->ts_name;
      printf ("\nTransfer Set Name: %s", ts_name->Name);
      if (ts_name->Scope == 1)
        printf (", Domain: %s", ts_name->DomainName);
    break;

    case MI_TYPEID_GMT_BASED_S:
      ts_timestamp = &pData->ts_timestamp;
      printf ("\nTransfer Set Time Stamp: %ld", (long) *ts_timestamp);
    break;

    case MI_TYPEID_INT16:
      ec_detected = &pData->ec_detected;
      printf ("\nEvent Code Detected: 0x%02x", (int) *ec_detected);
    break;

    case MI_TYPEID_DS_COND:
      ds_cond = &pData->ds_cond;
      printf ("\nDSConditions Detected: 0x%04x", (int) *ec_detected);
    break;
#endif  
    default:
      printf ("\nError - Check Me Out!");
    break;
    }
  }

/************************************************************************/
/*			  modifyLocalDatavalue				*/
/************************************************************************/

ST_VOID 	modifyLocalDatavalue	(ST_VOID)
  {
ST_CHAR  	dvname[256];
ST_BOOLEAN 	dataEntered;
MIS_DV_REF 	dv_ref;
MIS_DV		*pstDv;
MIT_DATA_VALUE	*pstMitDataValue;
ST_VOID		*pData;
ST_INT		ret;

  printf("\n Enter local DataValue Name: ");
  dataEntered = strget (dvname);
  if (dataEntered)
    {
    ret = mis_find_dv (dvname, &dv_ref);
    if (ret==SD_SUCCESS)
      {
      dv_ref = (dv_ref & MIS_INDEX_MASK);
      pstDv = (MIS_DV *)&mis_dv_ctrl[dv_ref];   /* table of ptrs to DV tables	*/
      pstMitDataValue = (struct Tag_MIT_DATA_VALUE *)pstDv->access_info;
      pData = &pstMitDataValue->data;
      printf ("\n Enter %s data: ", iccp_type_to_str (pstMitDataValue->dvType));
      getLocalDataValue (pstMitDataValue->dvName, (struct _mi_dv_data_union *)pData,
                         pstMitDataValue->dvType);
      mis_dv_change(pstMitDataValue->dvRef);
      }
    else
      {
      printf ("\n\n DataValue '%s', not found ret = %d", dvname, ret);
      }
    }
  (*menu_set_fun)();
  }

/************************************************************************/
/*			 	doForcePtChg				*/
/************************************************************************/

ST_VOID    doForcePtChg (ST_VOID)
  {

MIT_DATA_VALUE	*pstMitDataValue;
MIS_DV_REF    	dv_ref;
MIS_DV	   	*pstDv;
ST_CHAR	   	dvname[256];
ST_INT	   	ret;
  
  printf("\n Enter Local DataValue Name: ");
  strget(dvname);

  ret = mis_find_dv (dvname, &dv_ref);

  if (ret == SD_SUCCESS)
    {
    dv_ref = (dv_ref & MIS_INDEX_MASK);
    pstDv = (MIS_DV *)&mis_dv_ctrl[dv_ref];   /* table of ptrs to DV tables */
    pstMitDataValue = (struct Tag_MIT_DATA_VALUE *)pstDv->access_info;
    
    incrementDataValue (pstMitDataValue->dvType, &pstMitDataValue->data);
    mis_dv_change(dv_ref);
    }
  else
    {
    printf("\n Local DataValue '%s' is not found");
    pause_msg("\n Press any key to continue\n");
    }
    
  (*menu_set_fun)();
  }



/************************************************************************/
/*			 doReconfigurePtChgTbl				*/
/************************************************************************/

ST_VOID		doReconfigurePtChgTbl (ST_VOID)
  {
MIT_DATA_VALUE	*pstMitDataValue;
MIS_DV_REF 	dv_ref;
MIS_DV	   	*pstDv;
ST_CHAR  	dvname[256];
ST_CHAR		change[5];
ST_INT 		ret;
ST_INT          seconds;

/*
ST_INT			i;
time_t			theTime;*/
  
  printf("\n Enter local DataValue Name: ");
  strget (dvname);
  
  ret = mis_find_dv (dvname, &dv_ref);
    
  if (ret == SD_SUCCESS)
    {
    dv_ref = (dv_ref & MIS_INDEX_MASK);
    pstDv = (MIS_DV *) &mis_dv_ctrl[dv_ref];   /* table of ptrs to DV tables	*/
    pstMitDataValue = (struct Tag_MIT_DATA_VALUE *)pstDv->access_info;
	  
    printf ("\n Enter Change type (none, read, time): ");
    strget(change);
      
    if(!stricmp(change, "none"))
      {
      PtChgTbl[pstMitDataValue->ptChgIdx].ptMode = NO_CHANGE;
      }
    
    else if(!stricmp(change, "read"))
      {
      PtChgTbl[pstMitDataValue->ptChgIdx].ptMode = READ_CHANGE;
      }
    
    else if(!stricmp(change, "time"))
      {
      printf ("\n Enter Change Time in seconds: ");
      intget(&seconds);
      if(seconds > 0)
        {
	PtChgTbl[pstMitDataValue->ptChgIdx].ptMode = TIME_CHANGE;
	PtChgTbl[pstMitDataValue->ptChgIdx].deltaTime =	seconds;
	}
      else
        {
        printf("\n Illegal time value");
        }
      }    
    
    else
      {
      printf("\n Unknown Change argument '%s' for Point %s", 
                 change,pstMitDataValue->dvName);
      }
    }  
  else
    {
    printf("\n Data Value '%s' not found. Return code = %d", dvname, ret);
    }	     
													  
  (*menu_set_fun)();
  }
													  
/************************************************************************/
/*			 displayRemoteDataSetNames			*/
/************************************************************************/

ST_VOID		displayRemoteDataSetNames (ST_VOID)
  {
MIT_DATA_SET   *pstCurMicVccDataSet;
ST_CHAR  	buffer[256];
MIT_MIC_VCC 	*pstMitMicVcc;
ST_CHAR		*strScope;
ST_INT		line;

  pstMitMicVcc = getRemoteMitMicVcc (buffer);
  if (pstMitMicVcc)
    {						     
    pstCurMicVccDataSet = pstMitMicVcc->pstDataSets;
    printf ("\n Remote Domain: '%s' BLTID: '%s'\n",
             pstMitMicVcc->remoteDomainName,
	     pstMitMicVcc->bilateralTableId);
    line = 1;
    while (pstCurMicVccDataSet)
      {
      strScope = iccp_scope_to_str(pstCurMicVccDataSet->dsScope);
      printf ("\n DsName %s, Scope %s, NumPoints %d",
	      pstCurMicVccDataSet->dsName,
	      strScope,
	      pstCurMicVccDataSet->numPoints);
      line++;
      if (line == 22)
        {
        pause_msg ("\n\n Press any key to continue");
        printf ("\n Remote Domain: '%s' BLTID: '%s'",
                pstMitMicVcc->remoteDomainName,
	        pstMitMicVcc->bilateralTableId);
        line =1;
        }
      pstCurMicVccDataSet = (struct Tag_MIT_DATA_SET *)list_get_next (pstMitMicVcc->pstDataSets,
                                           pstCurMicVccDataSet);
      }
    pause_msg ("\n\n Press any key to continue");
    }
  else
    {
    printf ("\n Remote '%s' not found", buffer);
    pause_msg (" ");
    }			
  (*menu_set_fun)();
  }

/************************************************************************/
/*			 displayRemoteDataSetAttributes			*/
/************************************************************************/

ST_VOID		displayRemoteDataSetAttributes(ST_VOID)
  {
MIT_DATA_SET    *pstCurMicVccDataSet;
ST_CHAR  	buffer[256];
MIT_MIC_VCC 	*pstMitMicVcc;
ST_CHAR		*strScope;
ST_CHAR		*strType;
ST_INT		line;
MIT_DATA_POINT	*pstCurMitDataPoint;

  pstMitMicVcc = getRemoteMitMicVcc (buffer);
  if (pstMitMicVcc)
    {						     
    pstCurMicVccDataSet = pstMitMicVcc->pstDataSets;
    printf ("\n Remote Domain: %s BLTID(%s)",
             pstMitMicVcc->remoteDomainName,
	     pstMitMicVcc->bilateralTableId);
    line = 1;
    while (pstCurMicVccDataSet)
      {
      strScope = iccp_scope_to_str(pstCurMicVccDataSet->dsScope);
      printf ("\n\n\n\n");
      printf ("\n DsName %s, Scope %s, References %d, NumPoints %d",
	      pstCurMicVccDataSet->dsName,
	      strScope,
              pstCurMicVccDataSet->numRefDSTS,
	      pstCurMicVccDataSet->numPoints);
      pstCurMitDataPoint = pstCurMicVccDataSet->pointList;
      while (pstCurMitDataPoint)
        {
        strScope = iccp_scope_to_str(pstCurMitDataPoint->scope);
        strType = iccp_type_to_str(pstCurMitDataPoint->type);
        printf ("\n DvName %s, Scope %s, Type %s",
	        pstCurMitDataPoint->pointName,
	        strScope,
	        strType);
	line++;
        if (line == 24)
          {
          pause_msg ("\n\n Press any key to continue");
          line =1;
          }
        pstCurMitDataPoint = (struct Tag_MIT_DATA_POINT *)list_get_next (pstCurMicVccDataSet->pointList,
                                           pstCurMitDataPoint);
        }
      pause_msg ("\n\n Press any key to continue");
      pstCurMicVccDataSet = (struct Tag_MIT_DATA_SET *)list_get_next (pstMitMicVcc->pstDataSets,
                                           pstCurMicVccDataSet);
      }
    }
  else
    {
    printf ("\n Remote '%s' not found", buffer);
    pause_msg (" ");
    }			
  (*menu_set_fun)();
  }

/************************************************************************/
/*			 displayLocalDataValueNames			*/
/************************************************************************/

ST_VOID		displayLocalDataValueNames (ST_VOID)
  {
MIT_DATA_VALUE	*pstCurDataValue;
ST_CHAR		*strType;
ST_INT		line;
ST_CHAR    	ans; 

  pstCurDataValue = pstDataValueSet;
  line = 1;
  while (pstCurDataValue)
    {
    strType = iccp_type_to_str(pstCurDataValue->dvType);
    printf ("\n DvName: %s, Type: %s , Change Condition: ",
    	    pstCurDataValue->dvName,
    	    strType);

    switch (PtChgTbl[pstCurDataValue->ptChgIdx].ptMode)
      {
      case NO_CHANGE:
      
           printf("None");
           break;
	       
      case READ_CHANGE:

           printf("Read");
           break;
    
      case TIME_CHANGE:
	
           printf("Time, Seconds: %d", PtChgTbl[pstCurDataValue->ptChgIdx].deltaTime);
           break;
      }

    line++;
    if (line == 24)
      {
      printf("\n\n Press any key to continue or 'q' to stop displaying");
      ans = get_a_char();
         
      line = 1;
      }
    
    pstCurDataValue = (struct Tag_MIT_DATA_VALUE *)list_get_next(pstDataValueSet, pstCurDataValue);
    
    if (ans == 'q' || ans == 'Q') 
      pstCurDataValue = NULL;
  }
  printf ("\n\n %d local DataValue names configured", numLocalDataValue);
  pause_msg("\n\nPress a key to continue!\n");
  (*menu_set_fun)();
  }

/************************************************************************/
/* 			 incrementDataValue				*/
/************************************************************************/

ST_VOID 	incrementDataValue (ST_INT mi_type, MI_DV_DATA_UNION *pData)
  {
ST_RET rc;
time_t	theTime;
ST_UCHAR val;

/*  printf ("\n incrementDataValue entered"); */
  rc = SD_SUCCESS;
  time (&theTime);
  switch (mi_type)
    {
    case MI_TYPEID_REAL:
      pData->r += 41.625;
    break;
  
    case MI_TYPEID_STATE:
	  val = (pData->s & 0xc0) >> 6;
	  val++;
	  val = val % 4;
	  val = val << 6;
          val = val | (pData->s & 0x3F);
          pData->s = val;
    break;
  
    case MI_TYPEID_DISCRETE:
      pData->d += 1082;
    break;
  
    case MI_TYPEID_REAL_Q:
      pData->rq.Value += (ST_FLOAT)62.277;
      pData->rq.Flags += 1;
    break;
  
    case MI_TYPEID_STATE_Q:

      val = (pData->sq & 0xc0) >> 6;
      val++;
      val = val % 4;
      val = val << 6;
      val = val | (pData->sq & 0x3F);
      pData->sq = val;
      
    break;
  
    case MI_TYPEID_DISCRETE_Q:
      pData->dq.Value += 195;
      pData->dq.Flags += 1;
    break;
  
    case MI_TYPEID_REAL_Q_TIMETAG:
      pData->rqt.Value += (ST_FLOAT)103.75;
      pData->rqt.Flags += 1;
      pData->rqt.TimeStamp = theTime;
    break;
  
    case MI_TYPEID_STATE_Q_TIMETAG:
      val = (pData->sqt.Flags & 0xc0) >> 6;
      val++;
      val = val % 4;
      val = val << 6;
      val = val | (pData->sqt.Flags & 0x3F);
      pData->sqt.Flags = val;
      pData->sqt.TimeStamp = theTime;
    break;
  
    case MI_TYPEID_DISCRETE_Q_TIMETAG:
      pData->dqt.Value += 27;
      pData->dqt.Flags += 1; 
      pData->dqt.TimeStamp = theTime;
    break;

  /* extended forms match our local storage, just copy the data	*/
    case MI_TYPEID_REAL_EXTENDED:
      pData->re.Value += (ST_FLOAT)133.33333;
      pData->re.TimeStamp = theTime;
      pData->re.Flags++;
      pData->re.COV += 1200; 
    break;
  
    case MI_TYPEID_STATE_EXTENDED:
      pData->se.TimeStamp = theTime;
      val = (pData->se.Flags & 0xc0) >> 6;
      val++;
      val = val % 4;
      val = val << 6;
      val = val | (pData->se.Flags & 0x3F);
      pData->se.Flags = val;
      pData->se.Flags++; 
      pData->se.COV++;
    break;
  
    case MI_TYPEID_DISCRETE_EXTENDED:
      pData->de.Value++; 
      pData->de.TimeStamp = theTime; 
      pData->de.Flags++;
      pData->de.COV++;
    break;
  
    default:
      printf ("\nError - Check Me Out!");
      rc = SD_FAILURE;
    break;
    }
  }

/************************************************************************/
/* 			     showStateFlags				*/
/************************************************************************/

static void showStateFlags(ST_UCHAR stateFlags)
  {
ST_CHAR valid, currSource, normVal;
char validString[25], srcString[25], normString[25];

  valid = stateFlags & 0x30;
  valid >>= 4;

  currSource = stateFlags & 0x0C;
  currSource >>= 2;

  normVal = stateFlags & 0x2;
  normVal >>= 1;

  switch (valid)
    {
    case IP_VALIDITY_VALID:
      strcpy (validString, "VALID");
      break;

    case IP_VALIDITY_HELD:
      strcpy (validString, "HELD");
      break;

    case IP_VALIDITY_SUSPECT:
      strcpy (validString, "SUSPECT");
      break;

    case IP_VALIDITY_NOTVALID:
      strcpy (validString, "NOT VALID");
      break;
    }

  switch (currSource)
    {
    case IP_CURR_SRC_TELEMETERED:
      strcpy (srcString, "TELEMETERED");
      break;

    case IP_CURR_SRC_CALCULATED:
      strcpy (srcString, "CALCULATED");
      break;

    case IP_CURR_SRC_ENTERED:
      strcpy (srcString, "ENTERED");
      break;

    case IP_CURR_SRC_ESTIMATED:
      strcpy (srcString, "ESTIMATED");
      break;
    }	  

  if (normVal == IP_NORMAL_VAL_NORMAL)
    strcpy (normString, "NORMAL");
  else if (normVal == IP_NORMAL_VAL_ABNORMAL)
    strcpy (normString, "ABNORMAL");

  MIT_LOG_CIND3 ("  {%s, %s, %s} ", validString,srcString,normString);

  }

/************************************************************************/
/************************************************************************/
/* USER DEFINED DATA ACCESS FUNCTIONS 					*/
/************************************************************************/
/************************************************************************/
/*			u_mis_rd_dv 					*/
/************************************************************************/

/* This function is called when MI needs to get Data Value data, either	*/
/* for responding to a read indication or for sending an information	*/
/* report (Data Set Transfer Set).					*/

ST_VOID u_mis_rd_dv (MIS_RD_DV_CTRL *rd_dv_ctrl)
  {
MIS_DV 			*dv;
MI_REAL 		*rDest; /* Destination data pointers		*/
MI_STATE 	   	*sDest;
MI_DISCRETE 	   	*dDest;
MI_REAL_Q 	   	*rqDest;
MI_STATE_Q 	   	*sqDest;
MI_DISCRETE_Q 	   	*dqDest;
MI_REAL_Q_TIMETAG 	*rqtDest;
MI_STATE_Q_TIMETAG    	*sqtDest;
MI_DISCRETE_Q_TIMETAG 	*dqtDest;
MI_REAL_EXTENDED 	*reDest;
MI_STATE_EXTENDED    	*seDest;
MI_DISCRETE_EXTENDED 	*deDest;
MIT_DATA_VALUE		*pstMitDataValue;
MI_DV_DATA_UNION	*pData;
ST_RET 			rc;

/* For our sample, the MIS_DV (Data Value control) has the 		*/
/* 'access_info' element set to point to the data structure containing	*/
/* all information about the point. We use this to fill in the 		*/
/* destination data, depending on the types involved.			*/

  rc = SD_SUCCESS;
  dv = MIS_DV_REF_TO_DV(rd_dv_ctrl->mis_dv_ref);
  pstMitDataValue= (struct Tag_MIT_DATA_VALUE *)dv->access_info;
  if (pstMitDataValue==NULL)
    {
    MIT_LOG_NERR1( "No access info for DataValue '%s'", dv->dv_name);
    mis_rd_dv_done (rd_dv_ctrl, SD_FAILURE);
    return;
    }
  pData = &pstMitDataValue->data; 

/* See if we have to increment the DataPoint before returning the value	*/

  if (mitChgCtrl.chgMode == CHG_READ || mitChgCtrl.chgMode == CHG_MIXED)
    {						    
      updatePt(pstMitDataValue);
    }

  switch (rd_dv_ctrl->mi_type)
    {
 
    case MI_TYPEID_REAL:
      rDest = (float *)rd_dv_ctrl->data_buf;
      *rDest = pData->r;
    break;
  
    case MI_TYPEID_STATE:
      sDest = (unsigned char *)rd_dv_ctrl->data_buf;
      *sDest = pData->s;
    break;
  
    case MI_TYPEID_DISCRETE:
      dDest = (long *)rd_dv_ctrl->data_buf;
      *dDest = pData->d;
    break;
  
    case MI_TYPEID_REAL_Q:
      rqDest = (MI_REAL_Q *)rd_dv_ctrl->data_buf;
      rqDest->Value = pData->rq.Value;
      rqDest->Flags = pData->rq.Flags;
    break;
  
    case MI_TYPEID_STATE_Q:
      sqDest = (unsigned char *)rd_dv_ctrl->data_buf;
      *sqDest = pData->sq;
    break;
  
    case MI_TYPEID_DISCRETE_Q:
      dqDest = (MI_DISCRETE_Q *)rd_dv_ctrl->data_buf;
      dqDest->Value = pData->dq.Value;
      dqDest->Flags = pData->dq.Flags;
    break;
  
    case MI_TYPEID_REAL_Q_TIMETAG:
      rqtDest = (MI_REAL_Q_TIMETAG *)rd_dv_ctrl->data_buf;
      rqtDest->Value = pData->rqt.Value;
      rqtDest->Flags = pData->rqt.Flags;
      rqtDest->TimeStamp = pData->rqt.TimeStamp;
    break;
  
    case MI_TYPEID_STATE_Q_TIMETAG:
      sqtDest = (MI_STATE_Q_TIMETAG *)rd_dv_ctrl->data_buf;
      sqtDest->Flags = pData->sqt.Flags;
      sqtDest->TimeStamp = pData->sqt.TimeStamp;
    break;
  
    case MI_TYPEID_DISCRETE_Q_TIMETAG:
      dqtDest = (MI_DISCRETE_Q_TIMETAG *)rd_dv_ctrl->data_buf;
      dqtDest->Value = pData->dqt.Value;
      dqtDest->Flags = pData->dqt.Flags;
      dqtDest->TimeStamp = pData->dqt.TimeStamp;
    break;

  /* extended forms match our local storage, just copy the data	*/
    case MI_TYPEID_REAL_EXTENDED:
      reDest = (MI_REAL_EXTENDED *)rd_dv_ctrl->data_buf;
      reDest->Value = pData->re.Value;
      reDest->TimeStamp = pData->re.TimeStamp;
      reDest->Flags = pData->re.Flags;
      reDest->COV = pData->re.COV;
    break;
  
    case MI_TYPEID_STATE_EXTENDED:
      seDest = (MI_STATE_EXTENDED *)rd_dv_ctrl->data_buf;
      seDest->TimeStamp = pData->se.TimeStamp;
      seDest->Flags = pData->se.Flags;
      seDest->COV = pData->se.COV;
    break;
  
    case MI_TYPEID_DISCRETE_EXTENDED:
      deDest = (MI_DISCRETE_EXTENDED *)rd_dv_ctrl->data_buf;
      deDest->Value = pData->de.Value;
      deDest->TimeStamp = pData->de.TimeStamp;
      deDest->Flags = pData->de.Flags;
      deDest->COV = pData->de.COV;
    break;

  
    default:
      printf ("\nu_mis_rd_dv: Type Error - Check Me Out!");
      rc = SD_FAILURE;
    break;
    }

/* When the data has been put in the destination, we let MI know. In	*/
/* our sample this happens immediately, but could be done some time 	*/
/* later as apprioriate.						*/
  mis_rd_dv_done (rd_dv_ctrl, rc);
  }


/************************************************************************/
/*			u_mis_wr_dv 					*/
/************************************************************************/

ST_VOID u_mis_wr_dv (MIS_WR_DV_CTRL *wr_dv_ctrl)
  {
MI_REAL 		*rSrc; 		/* Source data pointers		*/
MI_STATE 	   	*sSrc;
MI_DISCRETE 	   	*dSrc;
MI_REAL_Q 	   	*rqSrc;
MI_STATE_Q 	   	*sqSrc;
MI_DISCRETE_Q 	   	*dqSrc;
MI_REAL_Q_TIMETAG 	*rqtSrc;
MI_STATE_Q_TIMETAG    	*sqtSrc;
MI_DISCRETE_Q_TIMETAG 	*dqtSrc;
MI_REAL_EXTENDED 	*reSrc;
MI_STATE_EXTENDED    	*seSrc;
MI_DISCRETE_EXTENDED 	*deSrc;
MIS_DV *dv;
ST_RET rc;
MIT_DATA_VALUE		*pstMitDataValue;
MI_DV_DATA_UNION	*pData;

  rc = SD_SUCCESS;
  dv = MIS_DV_REF_TO_DV(wr_dv_ctrl->mis_dv_ref);
  pstMitDataValue= (struct Tag_MIT_DATA_VALUE *)dv->access_info;
  pData = &pstMitDataValue->data;
  switch (wr_dv_ctrl->mi_type)
    {
    case MI_TYPEID_REAL:
      rSrc = (float *)wr_dv_ctrl->data_buf;
      pData->r = *rSrc;
    break;
  
    case MI_TYPEID_STATE:
      sSrc = (unsigned char *)wr_dv_ctrl->data_buf;
      pData->s = *sSrc;
    break;
  
    case MI_TYPEID_DISCRETE:
      dSrc = (long *)wr_dv_ctrl->data_buf;
      pData->d = *dSrc;
    break;
  
    case MI_TYPEID_REAL_Q:
      rqSrc = (MI_REAL_Q *)wr_dv_ctrl->data_buf;
      pData->rq.Value = rqSrc->Value;
      pData->rq.Flags = rqSrc->Flags;
    break;
  
    case MI_TYPEID_STATE_Q:
      sqSrc = (MI_STATE_Q *)wr_dv_ctrl->data_buf;
      pData->sq = *sqSrc;
    break;
  
    case MI_TYPEID_DISCRETE_Q:
      dqSrc = (MI_DISCRETE_Q *)wr_dv_ctrl->data_buf;
      pData->dq.Value = dqSrc->Value;
      pData->dq.Flags = dqSrc->Flags;
    break;
  
    case MI_TYPEID_REAL_Q_TIMETAG:
      rqtSrc = (MI_REAL_Q_TIMETAG *)wr_dv_ctrl->data_buf;
      pData->rqt.Value = rqtSrc->Value;
      pData->rqt.Flags = rqtSrc->Flags;
      pData->rqt.TimeStamp = rqtSrc->TimeStamp;
    break;
  
    case MI_TYPEID_STATE_Q_TIMETAG:
      sqtSrc = (MI_STATE_Q_TIMETAG *)wr_dv_ctrl->data_buf;
      pData->sqt.Flags = sqtSrc->Flags;
      pData->sqt.TimeStamp = sqtSrc->TimeStamp;
    break;
  
    case MI_TYPEID_DISCRETE_Q_TIMETAG:
      dqtSrc = (MI_DISCRETE_Q_TIMETAG *)wr_dv_ctrl->data_buf;
      pData->dqt.Value = dqtSrc->Value;
      pData->dqt.Flags = dqtSrc->Flags;
      pData->dqt.TimeStamp = dqtSrc->TimeStamp;
    break;

  /* extended forms match our local storage, just copy the data	*/
    case MI_TYPEID_REAL_EXTENDED:
      reSrc = (MI_REAL_EXTENDED *)wr_dv_ctrl->data_buf;
      pData->re.Value = reSrc->Value;
      pData->re.TimeStamp = reSrc->TimeStamp;
      pData->re.Flags = reSrc->Flags;
      pData->re.COV = reSrc->COV;
    break;
  
    case MI_TYPEID_STATE_EXTENDED:
      seSrc = (MI_STATE_EXTENDED *)wr_dv_ctrl->data_buf;
      pData->se.TimeStamp = seSrc->TimeStamp;
      pData->se.Flags = seSrc->Flags;
      pData->se.COV = seSrc->COV;
    break;
  
    case MI_TYPEID_DISCRETE_EXTENDED:
      deSrc = (MI_DISCRETE_EXTENDED *)wr_dv_ctrl->data_buf;
      pData->de.Value = deSrc->Value;
      pData->de.TimeStamp = deSrc->TimeStamp;
      pData->de.Flags = deSrc->Flags;
      pData->de.COV = deSrc->COV;
    break;
  
    default:
      printf ("\nError - Check Me Out!");
      rc = SD_FAILURE;
    break;
    }
  mis_wr_dv_done (wr_dv_ctrl, rc);
  }

/************************************************************************/
/************************************************************************/
/* MIS COMMUNICATIONS ACTIVITY INDICATION				*/
/* These functions are called by MI to tell the application about 	*/
/* server communications activity. Typically, no action is required ...	*/
/************************************************************************/
/************************************************************************/
/* DATA SET ACTIVITY							*/
/************************************************************************/
/************************************************************************/
/*			u_mis_create_ds_ind 				*/
/************************************************************************/

/* This function is called by MI when a client is creating a data set.	*/
/* No activity is required on our part ...				*/

ST_RET u_mis_create_ds_ind (MIS_VCC *vcc, DEFVLIST_REQ_INFO *req_info)
  {
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_mis_delete_ds_ind 				*/
/************************************************************************/

/* This function is called by MI when a client is deleting a data set.	*/
/* No activity is required on our part ...				*/

ST_RET u_mis_delete_ds_ind (MIS_VCC *vcc, DELVLIST_REQ_INFO *req_info)
  {
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/* TRANSFER SET ACTIVITY						*/
/************************************************************************/
/*			u_mis_dsts_activity 				*/
/************************************************************************/

/* This function is called by MI when there is activity associated with	*/
/* a Data Set Transfer Set. No activity is required on our part ...	*/

ST_RET u_mis_dsts_activity (MIS_VCC *vcc, MIS_DSTS *misDsts, 
			    ST_INT activity_type, 
			    MI_TS_TIMESTAMP ts_timestamp)
  {
ST_CHAR *tsName;

  tsName = misDsts->ts_name;
  switch (activity_type)
    {
    case MIS_DSTS_START_TRANSFER :
      MIT_LOG_IND1 (" Start Transfer received for DSTS '%s'", tsName);
    break;

    case MIS_DSTS_STOP_TRANSFER :
      MIT_LOG_IND1 (" Stop Transfer received for DSTS '%s'", tsName);

    break;

    case MIS_DSTS_BEGIN_REPORT :
      MIT_LOG_IND1 (" Begin Report received for DSTS '%s'", tsName);
    break;

    case MIS_DSTS_END_REPORT :
      MIT_LOG_IND1 (" End Report received for DSTS '%s'", tsName);
    break;

    case MIS_DSTS_ACK_RCVD : 
      MIT_LOG_IND1 (" ACK received for DSTS '%s'", tsName);
    break;

    case MIS_DSTS_NACK_RCVD :
      MIT_LOG_IND1 (" NACK received for DSTS '%s'", tsName);
    break;

    default:
    break;
    }
  return (SD_SUCCESS);
  }
/************************************************************************/
/* 			  displayStateFlags				*/
/************************************************************************

static ST_VOID displayStateFlags (ST_UCHAR stateFlags)
  {
ST_UCHAR 	valid;
ST_UCHAR	currSource;
ST_UCHAR	normVal;
ST_CHAR		validString[25];
ST_CHAR		srcString[25];
ST_CHAR		normString[25];

  valid = stateFlags & 0x30;
  valid >>= 4;

  currSource = stateFlags & 0x0C;
  currSource >>= 2;

  normVal = stateFlags & 0x2;
  normVal >>= 1;

  switch (valid)
    {
    case IP_VALIDITY_VALID:
      strcpy (validString, "VALID");
      break;

    case IP_VALIDITY_HELD:
      strcpy (validString, "HELD");
      break;

    case IP_VALIDITY_SUSPECT:
      strcpy (validString, "SUSPECT");
      break;

    case IP_VALIDITY_NOTVALID:
      strcpy (validString, "NOT VALID");
      break;
    }

  switch (currSource)
    {
    case IP_CURR_SRC_TELEMETERED:
      strcpy (srcString, "TELEMETERED");
      break;

    case IP_CURR_SRC_CALCULATED:
      strcpy (srcString, "CALCULATED");
      break;

    case IP_CURR_SRC_ENTERED:
      strcpy (srcString, "ENTERED");
      break;

    case IP_CURR_SRC_ESTIMATED:
      strcpy (srcString, "ESTIMATED");
      break;
    }	  

  if (normVal == IP_NORMAL_VAL_NORMAL)
    strcpy (normString, "NORMAL");
  else if (normVal == IP_NORMAL_VAL_ABNORMAL)
    strcpy (normString, "ABNORMAL");

  printf ("   %s %s %s \n", validString, srcString,normString);

  }
*/

/************************************************************************/
/*			u_mic_dv_ind 					*/
/************************************************************************/

/* This function is called once for each Data Value received via Read	*/
/* confirm or Information Report indication.				*/

/* The application can do what it pleases with the data; in the case of	*/
/* our sample we will simply print it. The 'handle_info' is a 'tag'	*/
/* string to use in the print.						*/

ST_VOID u_mic_dv_ind (MI_CONN *mi_conn, MIC_DV *dv, ST_RET result,
		      ST_VOID *data_buf, ST_INT mi_type)
  {
ST_CHAR *tag;
MI_REAL 		*dr;
MI_STATE 	   	*ds;
MI_DISCRETE 	   	*dd;
MI_REAL_Q 	   	*drq;
MI_STATE_Q 	   	*dsq;
MI_DISCRETE_Q 	   	*ddq;
MI_REAL_Q_TIMETAG 	*drqt;
MI_STATE_Q_TIMETAG    	*dsqt;
MI_DISCRETE_Q_TIMETAG 	*ddqt;
MI_REAL_EXTENDED 	*dre;
MI_STATE_EXTENDED    	*dse;
MI_DISCRETE_EXTENDED 	*dde;
MI_MMS_OBJECT_NAME 	*ts_name;
MI_TS_TIMESTAMP    	*ts_timestamp;
MI_DS_CONDITIONS   	*ds_cond;
MI_EC_DETECTED     	*ec_detected;
ST_CHAR timeBuf[30];

  if (result!=SD_SUCCESS)
    {
    MIT_LOG_CIND1 ("%s : DataAccess error", dv->dv_name);
    return;
    }


  tag = dv->dv_name;
  switch (mi_type)
    {
    case MI_TYPEID_REAL:
      dr = (MI_REAL *)data_buf;
      MIT_LOG_CIND2 ("%s Value: %f", tag, *dr);
    break;
  
    case MI_TYPEID_STATE:
      ds = (unsigned char *)data_buf;
      MIT_LOG_CIND3 ("%s Flags: %02x { %s}", tag, (int) *ds, showState(*ds));
    break;
  
    case MI_TYPEID_DISCRETE:
      dd = (long *)data_buf;
      MIT_LOG_CIND2 ("%s Value: %ld", tag, *dd);
    break;
  
    case MI_TYPEID_REAL_Q:
      drq = (MI_REAL_Q *)data_buf;
      MIT_LOG_CIND2 ("%s Value: %f", tag, drq->Value);
      MIT_LOG_CIND2 ("%s Flags: %02x", tag, (int) drq->Flags);
      showStateFlags (drq->Flags);
    break;
  
    case MI_TYPEID_STATE_Q:
      dsq = (MI_STATE_Q *)data_buf;
      MIT_LOG_CIND3 ("%s Flags: %02x { %s}", tag, (int) *dsq, showState(*dsq));
    break;
  
    case MI_TYPEID_DISCRETE_Q:
      ddq = (MI_DISCRETE_Q *)data_buf;
      MIT_LOG_CIND2 ("%s Value: %ld", tag, ddq->Value);
      MIT_LOG_CIND2 ("%s Flags: %02x", tag, (int) ddq->Flags);
      showStateFlags (ddq->Flags);
    break;
  
    case MI_TYPEID_REAL_Q_TIMETAG:
      drqt = (MI_REAL_Q_TIMETAG *)data_buf;
      MIT_LOG_CIND2 ("%s Value: %f", tag, drqt->Value);
      strcpy (timeBuf, ctime(&drqt->TimeStamp));
      timeBuf[24] = 0;
      MIT_LOG_CIND2 ("%s Timestamp: %s", tag, timeBuf);
      MIT_LOG_CIND2 ("%s Flags: %02x", tag, (int) drqt->Flags);
      showStateFlags (drqt->Flags);
    break;
  
    case MI_TYPEID_STATE_Q_TIMETAG:
      dsqt = (MI_STATE_Q_TIMETAG *)data_buf;
      strcpy (timeBuf, ctime(&dsqt->TimeStamp));
      timeBuf[24] = 0;
      MIT_LOG_CIND2 ("%s Timestamp: %s", tag, timeBuf);
      MIT_LOG_CIND2 ("%s Flags: %02x", tag, (int) dsqt->Flags);
      showStateFlags (dsqt->Flags);
    break;
  
    case MI_TYPEID_DISCRETE_Q_TIMETAG:
      ddqt = (MI_DISCRETE_Q_TIMETAG *)data_buf;
      MIT_LOG_CIND2 ("%s Value: %ld", tag, ddqt->Value);
      strcpy (timeBuf, ctime(&ddqt->TimeStamp));
      timeBuf[24] = 0;
      MIT_LOG_CIND2 ("%s Timestamp: %s", tag, timeBuf);
      MIT_LOG_CIND2 ("%s Flags: %02x", tag, (int) ddqt->Flags);
      showStateFlags (ddqt->Flags);
    break;
  
    case MI_TYPEID_REAL_EXTENDED:
      dre = (MI_REAL_EXTENDED *)data_buf;
      MIT_LOG_CIND2 ("%s Value: %f", tag, dre->Value);
      strcpy (timeBuf, ctime(&dre->TimeStamp));
      timeBuf[24] = 0;
      MIT_LOG_CIND2 ("%s Timestamp: %s", tag, timeBuf);
      MIT_LOG_CIND2 ("%s COV_Counter: %d", tag, (int)dre->COV);
      MIT_LOG_CIND2 ("%s Flags: %02x", tag, (int) dre->Flags);
    break;
  
    case MI_TYPEID_STATE_EXTENDED:
      dse = (MI_STATE_EXTENDED *)data_buf;
      strcpy (timeBuf, ctime(&dse->TimeStamp));
      timeBuf[24] = 0;
      MIT_LOG_CIND2 ("%s Timestamp: %s", tag, timeBuf);
      MIT_LOG_CIND2 ("%s COV_Counter: %d", tag, (int)dse->COV);
      MIT_LOG_CIND2 ("%s Flags: %02x", tag, (int) dse->Flags);
    break;
  
    case MI_TYPEID_DISCRETE_EXTENDED:
      dde = (MI_DISCRETE_EXTENDED *)data_buf;
      MIT_LOG_CIND2 ("%s Value: %ld", tag, dde->Value);
      strcpy (timeBuf, ctime(&dde->TimeStamp));
      timeBuf[24] = 0;
      MIT_LOG_CIND2 ("%s Timestamp: %s", tag, timeBuf);
      MIT_LOG_CIND2 ("%s COV_Counter: %d", tag, (int)dde->COV);
      MIT_LOG_CIND2 ("%s Flags: %02x", tag, (int) dde->Flags);
    break;

    case MI_TYPEID_MMS_OBJECT_NAME:
      ts_name = (MI_MMS_OBJECT_NAME *)data_buf;
      MIT_LOG_CIND1 ("Transfer Set Name: %s", ts_name->Name);
      if (ts_name->Scope == 1)
        MIT_LOG_CIND1 ("Domain: %s", ts_name->DomainName);
    break;

    case MI_TYPEID_GMT_BASED_S:
      ts_timestamp = (long *)data_buf;
      MIT_LOG_CIND1 ("Transfer Set Time Stamp: %ld", (long) *ts_timestamp);
    break;

    case MI_TYPEID_INT16:
      ec_detected = (short *)data_buf;
      MIT_LOG_CIND1 ("Event Code Detected: 0x%02x", (int) *ec_detected);
    break;

    case MI_TYPEID_DS_COND:
      ds_cond = (unsigned char *)data_buf;
      MIT_LOG_CIND1 ("DSConditions Detected: 0x%04x", (int) *ds_cond);
    break;
  
    default:
      MIT_LOG_ERR0 ("Unexpected Type - Check Me Out!");
    break;
    }
  }

/************************************************************************/
/*			u_mic_dv_ind_end   				*/
/************************************************************************/

/* This function is called when all calls to 'u_mic_dv_ind' have been 	*/
/* made for the MMS PDU.						*/

ST_VOID u_mic_dv_ind_end   (MI_CONN *mi_conn, ST_INT reason,
			    MIC_DSTS *mic_dsts, MIC_DSTS_STO *sto)
  {
ST_RET rc;
static ST_INT ackCount = 0;

  if (reason == MIC_DV_IND_REASON_READ)
    MIT_LOG_CIND0 ("Read response DV indications complete");
  if (reason == MIC_DV_IND_REASON_REPORT)
    {
    MIT_LOG_CIND0 ("Report DV indications complete");
    if (mic_dsts->mi_dsts_data.Critical)
      {
      /* send back a NACK every 3 Reports just for test purposes */
      ++ackCount;
      if (ackCount % 3)
        rc = mic_ack_dsts (mi_conn, mic_dsts, sto, SD_FALSE); /*ACK*/
      else
        rc = mic_ack_dsts (mi_conn, mic_dsts, sto, SD_TRUE);/*NACK*/

      if (rc != SD_SUCCESS)
        MIT_LOG_ERR1 ("Report ACK error %d", rc);
      }
    }
  }

/************************************************************************/
/*			u_mic_dv_ind_start 				*/
/************************************************************************/

/* This function is called when a set of new Data Value values is 	*/
/* received, and before MI starts calling 'u_mic_dv_ind'. After all 	*/
/* calls to 'u_mic_dv_ind' have been made for the MMS PDU, 		*/
/* 'u_mic_dv_ind_end' will be called.					*/

ST_VOID u_mic_dv_ind_start (MI_CONN *mi_conn, ST_INT reason,
			    MIC_DSTS *mic_dsts)
  {
  if (reason == MIC_DV_IND_REASON_READ)
    MIT_LOG_IND0 ("Read response DV indications starting ...");
  if (reason == MIC_DV_IND_REASON_REPORT)
    MIT_LOG_IND0 ("Report DV indications starting ...");
  }

/************************************************************************/
/*		    	   createDataSets				*/
/************************************************************************/

ST_RET     createDataSets (MIT_ASSOC_INFO *pstMitAssoc)
  {
MIT_MIC_VCC	*pstMitMicVcc;
ST_RET		ret = SD_FAILURE;
ST_BOOLEAN	startedADs;

/* See which DataSet we have to start.  If we started them all then	*/

  pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
  startedADs = SD_FALSE;
  if (pstMitMicVcc)
    {
    /* See if we have any to start, if not return SD_FAILURE		*/
    ret = SD_FAILURE;

    while ((pstMitMicVcc->pstNextDataSetToStart) && (ret == SD_FAILURE) &&
           (hasReqCapacity(pstMitAssoc)==SD_TRUE))
      {
      if ((pstMitMicVcc->pstNextDataSetToStart->dsState == DS_IDLE) ||
          (pstMitMicVcc->pstNextDataSetToStart->dsState == DS_REORGANIZED)) 
        {
        MIT_LOG_FLOW1 ("Creating DataSet '%s'", pstMitMicVcc->pstNextDataSetToStart->dsName);
        ret = createDs (pstMitAssoc->pstAssocCtrl->mi_conn, 
                        pstMitMicVcc->pstNextDataSetToStart,
	  	        pstMitAssoc);
        startedADs = SD_TRUE;
        pstMitMicVcc->numStartedDataSets++;
        }
      pstMitMicVcc->pstNextDataSetToStart = (struct Tag_MIT_DATA_SET *)list_get_next(
      			   pstMitMicVcc->pstDataSets,
			   pstMitMicVcc->pstNextDataSetToStart);
      }	
      /* it is possible to call this function and have it not start a DS */
      /* and get to the end of the list. That means there may be some    */
      /* DataSets that were created and some that were not not.          */
      /* Start creating TransferSets for the DataSets that exist.        */
      if ((pstMitMicVcc->pstNextDataSetToStart==NULL)&&
          (startedADs==SD_FALSE))
        {
        ret = createTransferSets (pstMitAssoc);
        }
    }
  return (ret);
  }

/************************************************************************/
/*			      display_mic_dv_table			*/
/************************************************************************/

ST_VOID 	display_mic_dv_table (MIC_VCC *mic_vcc)
  {
ST_INT	i;
MIC_DV	*dvp;

  printf ("\n VCC_SPEC Table");
  for(i=0; i<mic_vcc->num_vcc_dv; i++)
    {
    dvp = mic_vcc->vcc_dv_tbl[i];
    printf( "\n Name '%s', Scope %d, Access info %p", dvp->dv_name, dvp->dv_attrib, dvp->handle_info);
    }
  printf ("\n ICC_SPEC Table");
  for(i=0; i<mic_vcc->num_icc_dv; i++)
    {
    dvp = mic_vcc->icc_dv_tbl[i];
    printf( "\n Name '%s', Scope %d, Access info %p", dvp->dv_name, dvp->dv_attrib, dvp->handle_info);
    }
  }

/************************************************************************/
/*			        createDs 				*/
/************************************************************************/

ST_RET createDs (MI_CONN *mi_conn, MIT_DATA_SET *pstMitDataSet,
		 MIT_ASSOC_INFO *pstMitAssoc)
  {
MI_REQ_CTRL 	*req;
MIC_DV 		**dv;
MIC_DS 		*ds;
MIC_VCC 	*mic_vcc;
ST_RET		ret;
MIT_MIC_VCC	*pstMitMicVcc;
MIT_DATA_POINT	*pstCurDataPoint;
ST_INT		numPoints;
ST_INT		i;

  mic_vcc = mi_conn->mi_remote-> mic_vcc;
  numPoints = pstMitDataSet->numPoints;
  if (numPoints==0)
    {
    MIT_LOG_ERR1 ("0 DataValues configured for DataSet '%s' skipping", pstMitDataSet->dsName);
    return(SD_FAILURE);
    }
  dv = (MIC_DV **)chk_calloc (numPoints, sizeof(MIC_DV *));
  pstCurDataPoint = pstMitDataSet->pointList;
//  display_mic_dv_table (mic_vcc);
  for (i=0; i<numPoints; i++)
    {
    dv[i] = mic_find_dv (mic_vcc, 
                         pstCurDataPoint->pointName, 
			 pstCurDataPoint->scope);
    if (dv[i]==NULL)
      {
      MIT_LOG_ERR2 ( "DataValue '%s' not found in DataSet '%s'",
               pstCurDataPoint->pointName,
               pstMitDataSet->dsName);
      chk_free (dv);
      pstMitDataSet->dsScope = DS_FAILED;
      return (SD_FAILURE);
      }

    pstCurDataPoint = (MIT_DATA_POINT *)list_get_next(pstMitDataSet->pointList,
   						      pstCurDataPoint);
    }

/* Create a Data Set in the server */
  req = mic_create_ds (mi_conn, 
                       pstMitDataSet->dsName,
                       pstMitDataSet->dsScope, 
                       pstMitDataSet->numPoints, 
		       dv, 
		       &ds);
  if (!req)
    {
    MIT_LOG_ERR1 ("Could not create DataSet '%s'", pstMitDataSet->dsName);
    ret = SD_FAILURE;
    }
  else
    {
    req->u_req_done = createDsDone;
    req->user_info[0] = pstMitAssoc;
    req->user_info[1] = pstMitDataSet;
    pstMitDataSet->dsScope = DS_CREATING;
    decReqCapacity(pstMitAssoc);
    pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
    pstMitDataSet->pstDS = ds;
    ret = SD_SUCCESS;
    }
  chk_free (dv);
  return (ret);
  }

/************************************************************************/
/*			createDsDone 					*/
/************************************************************************/

static ST_VOID createDsDone (MI_REQ_CTRL *req)
  {
MIT_ASSOC_INFO 	*pstMitAssoc;
MIT_MIC_VCC     *pstMitMicVcc;
MIT_DATA_SET	*pstMitDataSet;

  /* save this before we free the MI_REQ_CTRL */
  pstMitAssoc = (struct Tag_MIT_ASSOC_INFO *)req->user_info[0];
  pstMitDataSet = (struct Tag_MIT_DATA_SET *)req->user_info[1];
  incReqCapacity(pstMitAssoc);

  if (req->result == SD_SUCCESS)
    {
    MIT_LOG_CNF1 ("ICCP Create DS '%s' OK time to Read it",pstMitDataSet->dsName);
    pstMitDataSet->dsState = DS_CREATED;
    mi_free_req_ctrl (req);
    /* Read the Current Data Set */
    readDs (pstMitAssoc,pstMitDataSet);
    }
  else
    {
    MIT_LOG_ERR2 ("ICCP Create DS '%s' Error %d", pstMitDataSet->dsName, req->result);
    mi_free_req_ctrl (req);
    pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
    if (pstMitDataSet->reorgDs == SD_FALSE)
      {
      pstMitDataSet->reorgDs = SD_TRUE;
      MIT_LOG_FLOW1 ("Attempting to Reorganize DS %s", pstMitDataSet->dsName);
      reorgDs (pstMitAssoc,pstMitDataSet);
      }
    else /* we already tried to sort this one out and the server      */
      {  /* won't let us so on to the next                            */
      MIT_LOG_FLOW1 ("Reorganization failed for DS %s going to next DS", pstMitDataSet->dsName);
      pstMitDataSet->dsState = DS_FAILED;
      createDataSets (pstMitAssoc);
      }
     
    }

  }

/************************************************************************/
/*			        readDs	 				*/
/************************************************************************/

static ST_VOID readDs (MIT_ASSOC_INFO *pstMitAssoc,MIT_DATA_SET *pstMitDataSet)
  {
MIT_MIC_VCC	*pstMitMicVcc;
MI_REQ_CTRL 	*req;
MI_CONN 	*mi_conn;
MIC_DS 		*ds;

  mi_conn = pstMitAssoc->pstAssocCtrl->mi_conn;
  pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
  ds = (struct _mic_ds *)pstMitDataSet->pstDS;

  req = mic_read_ds (mi_conn, ds); 
  if (!req)
    {
    MIT_LOG_ERR1 ("Could not Issue read for DataSet %s",
                   pstMitDataSet->dsName);
    }
  else
    {
    MIT_LOG_FLOW1 ("Issued DataSet Read for %s", 
                    pstMitDataSet->dsName);
    req->u_req_done =  readDsDone;
    req->user_info[0] = pstMitAssoc;
    req->user_info[1] = pstMitDataSet;
    pstMitDataSet->dsState = DS_READING;
    decReqCapacity(pstMitAssoc);
    }
  }

/************************************************************************/
/*			readDsDone					*/
/************************************************************************/

static ST_VOID readDsDone (MI_REQ_CTRL *req)
  {
MIT_MIC_VCC		*pstMitMicVcc;
MIT_ASSOC_INFO 		*pstMitAssoc;
MIT_DATA_SET		*pstMitDataSet;
ST_RET			ret;

  /* save this before we free the MI_REQ_CTRL */
  pstMitAssoc = (struct Tag_MIT_ASSOC_INFO *)req->user_info[0];
  pstMitDataSet = (struct Tag_MIT_DATA_SET *)req->user_info[1];
  incReqCapacity(pstMitAssoc);
  if (req->result == SD_SUCCESS)
    {
    MIT_LOG_CNF1 ("ICCP Initial DS '%s' Read OK",pstMitDataSet->dsName);
    pstMitDataSet->dsState = DS_READ;
    mi_free_req_ctrl (req);
    }
  else
    {
    MIT_LOG_ERR2 ("ICCP Read DS '%s' Error %d", pstMitDataSet->dsName,
                   req->result);
    mi_free_req_ctrl (req);
    }

  /* Retrieve the pointer of the MIT_MIC_VCC the DataSet was created for */
  /* If there is a next DataSet to start then we will do that, otherwise */
  /* we check to see if there are TransferSets configured, start any 	 */
  /* DSTS after all the DS are created.					 */

  pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
  if (pstMitMicVcc)
    {
    if (pstMitMicVcc->pstNextDataSetToStart)
      {
      ret = createDataSets (pstMitAssoc);
      }
    else
      {
      MIT_LOG_FLOW2 ("All DataSets created for Remote %s, next create %d Transfer Sets",
                      pstMitMicVcc->remoteName,pstMitMicVcc->numTransferSets);
      ret = createTransferSets (pstMitAssoc);
      }
    }
  }

/************************************************************************/
/*			        reorgDs	 				*/
/************************************************************************/

static ST_VOID reorgDs (MIT_ASSOC_INFO *pstMitAssoc, MIT_DATA_SET *pstMitDataSet)
  {
MIT_MIC_VCC	*pstMitMicVcc;
MI_REQ_CTRL 	*req;
MI_CONN 	*mi_conn;
MIT_DATA_POINT	*pstCurPoint;
ST_INT		i;
ST_INT		num_dv;
MIC_DV		**dv;


  mi_conn = pstMitAssoc->pstAssocCtrl->mi_conn;
  pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
  
  /* have to whip out a read to see which ones don't exist.    */
  /* pray that they all fit in a PDU.			       */

  num_dv = pstMitDataSet->numPoints;
  dv = (MIC_DV **)chk_calloc(num_dv, sizeof(MIC_DV *));

  pstCurPoint = pstMitDataSet->pointList;
  for(i=0;i<num_dv; i++)
    {
    /* We found them during configuration so they should still be here */
    dv[i] = mic_find_dv (pstMitMicVcc->pstMicVcc, 
                         pstCurPoint->pointName, 
 			             pstCurPoint->scope); 
    pstCurPoint = (MIT_DATA_POINT *)list_get_next 
                          (pstMitDataSet->pointList, pstCurPoint);
    }

  req = mic_read_dv (mi_conn, num_dv, dv);
  chk_free(dv);
  if (!req)
    {
    MIT_LOG_ERR1 ("Could not issue read for Point list belonging to %s",
                   pstMitDataSet->dsName);
    }
  else
    {
    MIT_LOG_FLOW1 ("Issued read for Point list belonging to %s", 
                    pstMitDataSet->dsName);
    req->u_req_done =  reorgDsDone;
    decReqCapacity(pstMitAssoc);
    req->user_info[0] = pstMitAssoc;
    req->user_info[1] = pstMitDataSet;
    pstMitDataSet->dsState = DS_REORGANIZING;
    }
  }


/************************************************************************/
/*			   specTSVar					*/
/************************************************************************/

ST_BOOLEAN  specTSVar( ST_CHAR *pointName)
  {
  if (!strcmp(pointName,"DSConditions_Detected") ||
      !strcmp(pointName,"Transfer_Set_Name") ||
      !strcmp(pointName,"Transfer_Set_Time_Stamp"))
    return(SD_TRUE);
  else
    return(SD_FALSE);
  }

/************************************************************************/
/*			reorgDsDone					*/
/************************************************************************/

static ST_VOID reorgDsDone (MI_REQ_CTRL *req)
  {
MIT_MIC_VCC		*pstMitMicVcc;
MIT_ASSOC_INFO 	*pstMitAssoc;
READ_RESP_INFO  *read_info;
ACCESS_RESULT   *ar_ptr;
ST_INT 		num_data;
ST_INT 		i;
MIT_DATA_SET	*pstMitDataSet;
MIT_DATA_POINT	*pstCurPoint;
MIT_DATA_POINT	*savePtr;
ST_BOOLEAN	remove;
ST_BOOLEAN	validPointDiscovered;
MVL_REQ_PEND	*mvl_req;
ST_RET		ret;

  /* save this before we free the MI_REQ_CTRL				*/
  pstMitAssoc = (struct Tag_MIT_ASSOC_INFO *)req->user_info[0];
  pstMitDataSet = (struct Tag_MIT_DATA_SET *)req->user_info[1];
  incReqCapacity(pstMitAssoc);
  if (req->result == SD_SUCCESS)
    {
    MIT_LOG_CNF1 ("ICCP Reorg DS '%s' Read OK", pstMitDataSet->dsName);
    /* after all the call backs are over with, check the access results	*/


    mvl_req = req->mvl_req;
    read_info = (READ_RESP_INFO *) mvl_req->event->u.mms.dec_rslt.data_ptr;
    ar_ptr    = read_info->acc_rslt_list;
    num_data = mvl_req->u.rd.num_data;


    pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
    pstCurPoint = pstMitDataSet->pointList;
    validPointDiscovered = SD_FALSE;

    for (i = 0; i < num_data; ++i, ar_ptr++)
      {      /* check for successfull read of this variable */

      remove=SD_FALSE;
      if ((ar_ptr->acc_rslt_tag == ACC_RSLT_FAILURE) &&
       /*   (ar_ptr->failure == ARE_OBJ_NONEXISTENT ) && */
          (!specTSVar(pstCurPoint->pointName)))
        {
	/* it failed to return a good result, so check it's name to	*/
	/* make sure it's not a special variable			*/
	remove=SD_TRUE;

        }
      else
        {
        validPointDiscovered = SD_TRUE;
	}
      savePtr = pstCurPoint;
      pstCurPoint = (MIT_DATA_POINT *)list_get_next 
                          (pstMitDataSet->pointList, pstCurPoint);
      if (remove)  /* NONEXISTENT variables are tossed out of the DS	*/
        {
        pstMitDataSet->numPoints--;
	list_unlink(&pstMitDataSet->pointList, savePtr);
	chk_free(savePtr);
	}
      } /* for each variable						*/

    if (validPointDiscovered == SD_FALSE)
      {
      /* delete any special variables left in the set 			*/
      pstCurPoint = pstMitDataSet->pointList;
      for (i=0; i<pstMitDataSet->numPoints; i++)
        {
        savePtr = pstCurPoint;
        pstCurPoint = (MIT_DATA_POINT *)list_get_next 
                          (pstMitDataSet->pointList, pstCurPoint);
	list_unlink(&pstMitDataSet->pointList, savePtr);
	chk_free(savePtr);
	}
      /* delete the DataSet and adjust the overhead			*/

      list_unlink(&pstMitMicVcc->pstDataSets, pstMitDataSet);
      chk_free(pstMitDataSet);
      pstMitMicVcc->numDataSets--;
      pstMitMicVcc->numStartedDataSets--;
      }

    }
  else
    {
    MIT_LOG_ERR1 ("ICCP Reorg Read DS Error %d", req->result);
    }

  mi_free_req_ctrl (req);

  /* If there are any valid points left in the DataSet then start it up */
  /* otherwise we check to see if there are TransferSets configured, 	*/
  /* start any DSTS after all the DS are created.			*/

  pstMitMicVcc = pstMitAssoc->pstMitMicVcc;

  if (pstMitMicVcc)
    {
    if (validPointDiscovered == SD_TRUE)
      {
      pstMitDataSet->dsState = DS_REORGANIZED; 
      pstMitMicVcc->pstNextDataSetToStart = pstMitDataSet;
      }


    if (pstMitMicVcc->pstNextDataSetToStart)
      {
      ret = createDataSets (pstMitAssoc);
      }
    else
      {
      MIT_LOG_FLOW2 ("All DataSets created for Remote %s, next create %d Transfer Sets",
                      pstMitMicVcc->remoteName,pstMitMicVcc->numTransferSets);
      ret = createTransferSets (pstMitAssoc);
      }
    }
  }
