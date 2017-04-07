/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*          1996-2002, All Rights Reserved                              */
/*                                                                      */
/* MODULE NAME : mit_misc.c                                            	*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*                                                                      */
/* MODULE DESCRIPTION :                                                 */
/*   	Functions belonging to the Misc menu				*/
/*                                                                      */
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev                     Comments                     */
/* --------  ---  ------   -------------------------------------------  */
/* 07/16/02  rkr    10     added doModifyAddressMatching		*/
/* 06/26/02  ask    9      Cleanup for QNX				*/
/* 06/20/02  rkr    8      Copyright year update                        */
/* 02/07/02  ask    7      added mis_dv_change calls in updatePtChgTbl  */
/* 01/17/02  ask    6      Ported to Marben				*/
/* 10/25/01  ask    5 	   Updated mem_chk logging masks		*/
/* 10/18/01  rkr    4      fixed comparison in mitConfigurePtChgTbl	*/
/* 06/21/01  rkr    3      Cleanup					*/
/* 06/13/01  rkr    2      Changes for new DataValue test mechanism	*/
/* 03/17/99  rkr    1      Created                                      */
/************************************************************************/

#include "mit_main.h"
#include "scrndefs.h"
#include "gvaldefs.h"
#include "fkeydefs.h"
#include "suicacse.h"

#if defined(_AIX) || defined(__alpha)
#if defined(__alpha)
#include <sys/time.h>
#endif
#include <sys/select.h>
#include <errno.h>
#endif

static char *thisFileName = __FILE__;

/************************************************************************/
/* Global variables							*/
/************************************************************************/

CHANGE_CTRL	    mitChgCtrl;
CHANGE_POINT 	    *PtChgTbl;     /* pointer to the point change table */

/************************************************************************/
/*			    mit_app_dyn_log_fun				*/
/************************************************************************/

ST_VOID mit_app_dyn_log_fun (LOG_CTRL *lc, const ST_CHAR *timeStr, 
			     ST_INT logType, const ST_CHAR *logTypeStr,
                             const ST_CHAR *sourceFile, ST_INT lineNum,
                             ST_INT bufLen, const ST_CHAR *buf)
  {
  printf ("%s\n",buf);
  }

/************************************************************************/
/*			      showMITStatistics				*/
/************************************************************************/

ST_VOID showMITStatistics (ST_VOID)
  {
  }

/************************************************************************/
/*                       get_bmatch					*/
/************************************************************************/

ST_VOID get_bmatch (ST_BOOLEAN *sel_ptr, ST_CHAR *text)
  {
ST_CHAR buf[100];
ST_CHAR q[20];

  sprintf (buf," Enable %s Address Match \0",text);
  if (*sel_ptr) 
    strcat (buf,"(now ON) ?");
  else
    strcat (buf,"(now OFF) ?");

  printf ("%s",buf);
  if (strget (q))
    {
    if (q[0] == 'y' || q[0] == 'Y')
      *sel_ptr = SD_TRUE;
    else
      *sel_ptr = SD_FALSE;
    }
  }

/************************************************************************/
/*                       get_ldebug					*/
/************************************************************************/

ST_VOID get_ldebug (ST_ULONG *sel_ptr, ST_ULONG mask, ST_CHAR *text)
  {
ST_CHAR buf[100];
ST_CHAR q[20];

  sprintf (buf," Enable %s Debug Print \0",text);
  if (*sel_ptr & mask)
    strcat (buf,"(now ON) ?");
  else
    strcat (buf,"(now OFF) ?");

  printf ("%s",buf);
  if (strget (q))
    {
    if (q[0] == 'y' || q[0] == 'Y')
      *sel_ptr |= mask;
    else
      *sel_ptr &= ~mask;
    }
  }

/************************************************************************/
/*                       get_debug					*/
/************************************************************************/

static ST_VOID get_debug (ST_UINT  *sel_ptr, ST_UINT mask, ST_CHAR *text)
  {
ST_CHAR buf[100];
ST_CHAR q[20];

  sprintf (buf," Enable %s Debug Print \0",text);
  if (*sel_ptr & mask)
    strcat (buf,"(now ON) ?");
  else
    strcat (buf,"(now OFF) ?");

  printf ("%s",buf);
  if (strget (q))
    {
    if (q[0] == 'y' || q[0] == 'Y')
      *sel_ptr |= mask;
    else
      *sel_ptr &= ~mask;
    }
  }

/************************************************************************/
/*			     do_debugset				*/
/************************************************************************/

ST_VOID do_debugset (ST_VOID)
  {

  CLEARSCR;

  printf ("\nICCP-LITE Demo Logging - \n");
  
  get_ldebug ((ST_ULONG *)&mit_debug_sel,MITLOG_ERR,"MIT Unrecoverable Errors");
  get_ldebug ((ST_ULONG *)&mit_debug_sel,MIT_LOG_NERR,"MIT Normal Errors");
  get_ldebug ((ST_ULONG *)&mit_debug_sel,MITLOG_IND,"MIT Indications");
  get_ldebug ((ST_ULONG *)&mit_debug_sel,MITLOG_CNF,"MIT Confirmations");
  get_ldebug ((ST_ULONG *)&mit_debug_sel,MITLOG_CFG,"MIT Configuration");
  get_ldebug ((ST_ULONG *)&mit_debug_sel,MITLOG_FLOW,"MIT Flow");

  get_ldebug ((ST_ULONG *)&mvl_debug_sel,MVLLOG_ACSEDATA,"MVL ACSE Data");
#ifdef MAP30_ACSE
  get_ldebug ((ST_ULONG *)&mms_debug_sel,MMS_LOG_ACSE,"ACSE Interface");
#endif

  get_ldebug ((ST_ULONG *)&mms_debug_sel,MMS_LOG_ENC,"MMS ENCODE");
  get_ldebug ((ST_ULONG *)&mms_debug_sel,MMS_LOG_DEC,"MMS DECODE");

  get_debug (&asn1_debug_sel,ASN1_LOG_ENC,"ASN.1 ENCODE");
  get_debug (&asn1_debug_sel,ASN1_LOG_DEC,"ASN.1 DECODE");

  get_ldebug ((ST_ULONG *)&mms_debug_sel,MMS_LOG_AA,"Alternate Access");
  get_ldebug ((ST_ULONG *)&mms_debug_sel,MMS_LOG_RT,"Runtime Type");
  get_ldebug ((ST_ULONG *)&mms_debug_sel,MMS_LOG_RTAA,"Runtime AA Type");
 
#ifdef LEAN_T
  get_ldebug ((ST_ULONG *)&tp4_debug_sel,TP4_LOG_FLOWUP,"Inbound Transport");
  get_ldebug ((ST_ULONG *)&tp4_debug_sel,TP4_LOG_FLOWDOWN,"Outbound Transport");

  get_ldebug ((ST_ULONG *)&clnp_debug_sel,CLNP_LOG_IND,"Inbound CLNP");
  get_ldebug ((ST_ULONG *)&clnp_debug_sel,CLNP_LOG_REQ,"Outbound CLNP");
#endif

  get_debug ((ST_UINT *) &chk_debug_en,MEM_LOG_ERR,"MEMCHK ERRORS");
  get_debug ((ST_UINT *) &chk_debug_en,MEM_LOG_MALLOC | MEM_LOG_CALLOC | MEM_LOG_REALLOC | MEM_LOG_FREE,
                         "ALL MEMCHK TRANSACTIONS");
  
  printf ("\n");
  if (ask (" Send MMS-EASE  Lite Internal Logging to Screen (Y) ? ",SD_TRUE))
    _slog_dyn_log_fun = mit_app_dyn_log_fun;
  else
    _slog_dyn_log_fun = NULL;
    
  if (ask (" Close Log File (N) ? ",SD_FALSE))
    MLOG_CLOSE_FILE ();

  if (sLogCtrl != NULL)
    {
    if (sLogCtrl->logCtrl & LOG_FILE_EN)
      {
      if (ask (" Stop File Logging (N) ? ",SD_FALSE))
        MLOG_STOP_FILE_LOGGING ();
      }
    else
      {
      if (ask (" Start File Logging (N) ? ",SD_FALSE))
        MLOG_START_FILE_LOGGING ();
      }
    }

  if (sLogCtrl->logCtrl & LOG_MEM_EN)
    {
    if (ask (" Dump Memory Log (N) ? ",SD_FALSE))
      MLOG_DUMP_MEM_LOG ();
    }

  m_heap_check_enable = ask ("\n Enable memory heap check (N) ? ",SD_FALSE);
  m_check_list_enable = ask (" Enable memory list check (N) ? ",SD_FALSE);


/*  (*menu_set_fun)();*/
  }

/************************************************************************/
/* scrnLogOn:  Turn on Screen Logging					*/
/************************************************************************/

ST_VOID scrnLogOn (ST_VOID)
  {
  _slog_dyn_log_fun = mit_app_dyn_log_fun;
  printf ("Log messages to file and stdout\n");

  }

/************************************************************************/
/* scrnLogOff:  Turn off Screen Logging					*/
/************************************************************************/

ST_VOID scrnLogOff (ST_VOID)
  {
  _slog_dyn_log_fun = NULL;
  printf ("Log messages to file only\n");
  
  }

/************************************************************************/
/* showSizes:  								*/
/************************************************************************/

ST_VOID showSizes (ST_VOID)
  {
  printf ("\nSome Sizes . . .\n");

  printf ("Size of int = %d\n", sizeof (int));
  printf ("Size of ST_INT = %d\n", sizeof (ST_INT));
  printf ("Size of short int = %d\n", sizeof (short int));
  printf ("Size of ST_INT16 = %d\n", sizeof (ST_INT16));
  printf ("Size of long int = %d\n", sizeof (long int));
  printf ("Size of ST_INT32 = %d\n", sizeof (ST_INT32));

  pause_msg("Press a key to continue!\n");
/*  (*menu_set_fun)(); */
  
  }

/************************************************************************/
/*			   mitConfigurePtChgTbl  			*/
/************************************************************************/

ST_VOID mitConfigurePtChgTbl (ST_VOID)
  {
ST_INT	i;
MIT_DATA_VALUE	*pstCurDataValue;

  PtChgTbl = (CHANGE_POINT *)chk_calloc(numLocalDataValue,
                                        sizeof(CHANGE_POINT));
  mitChgCtrl.chgMode = CHG_NO_CHANGE;
  mitChgCtrl.randomDeltaTime = 1;
  mitChgCtrl.lowRange = 0;
  mitChgCtrl.highRange = (numLocalDataValue -1);
  
  pstCurDataValue = pstDataValueSet;
  i = 0;
  while (pstCurDataValue)
    {
    PtChgTbl[i].ptMode = READ_CHANGE;
    PtChgTbl[i].deltaTime = 1;
    PtChgTbl[i].pstMitDataValue = pstCurDataValue;

    pstCurDataValue->ptChgIdx = i;
    pstCurDataValue = (MIT_DATA_VALUE *)list_get_next 
                          (pstDataValueSet, pstCurDataValue);
    i++;
    }

  if (i != numLocalDataValue)
    MIT_LOG_ERR2 ("Number of nodes on list %d doesn't match table size %d !!!",
                   i+1, numLocalDataValue);

  }

/************************************************************************/
/*			     doReconfigureChgMode			*/
/************************************************************************/

ST_VOID doReconfigureChgMode (ST_VOID)
  {
ST_INT	temp;

  printf ("\nEnter global change mode");
  printf ("\n  (NONE=0, TIME=1, READ=2, MIXED=3, RANDOM = 4)(default = %d): ",
          mitChgCtrl.chgMode);

  temp = mitChgCtrl.chgMode;
  intget(&temp);
  if ((temp>=NO_CHANGE) && (temp<=CHG_RANDOM))
    {
    mitChgCtrl.chgMode = temp;
    if (temp==CHG_RANDOM)
      {
      printf ("\n Enter Random delta time (default=%d)",mitChgCtrl.randomDeltaTime);
      temp = mitChgCtrl.randomDeltaTime;
      intget (&temp);
      if (temp>=1)
        mitChgCtrl.randomDeltaTime = temp;
      }
    
    printf ("\n Current number of DataValues is %d",numLocalDataValue);
    printf ("\n Enter Low Range (default=%d)",mitChgCtrl.lowRange);
    temp = mitChgCtrl.lowRange;
    intget (&temp);
    if (temp>=0 && temp<numLocalDataValue)
      {
      mitChgCtrl.lowRange = temp;
      }
    printf ("\n Enter High Range (default=%d)",mitChgCtrl.highRange);
    temp = mitChgCtrl.highRange;
    intget (&temp);
    if (temp>=0 && temp<numLocalDataValue)
      {
      mitChgCtrl.highRange = temp;
      }
    }
  }

/************************************************************************/
/*                           get_random_range                           */
/* NOTE: RAND_MAX should be defined in stdlib.h.			*/
/************************************************************************/

static ST_INT get_random_range (ST_INT min_val, ST_INT max_val)
  {
ST_INT rshort;
ST_INT val;

  rshort = val = (ST_INT) rand ();
  if (rshort < 0)
    rshort = rshort * -1;
  if (rshort == 0 || rshort == -32768)
    rshort = 1;
  rshort = (rshort % (max_val - min_val)) + min_val;
  if (rshort < min_val  ||  rshort > max_val)
    {
    printf (" Can't generate random number of variables. %d  %d", val, rshort);
    return (min_val);
    }
  return (rshort);
  }  

/************************************************************************/
/*			    doModifyAddressMatching			*/
/************************************************************************/

ST_VOID doModifyAddressMatching (ST_VOID)
  {

  CLEARSCR;

  printf ("\n MI Address Matching - \n");
  
  get_bmatch (&mi_match_allow_missing_ae_elements, "Allow missing AE elements");
  get_bmatch (&mi_match_allow_extra_ae_elements, "Allow extra AE elements");
  get_bmatch (&mi_match_local_ap_title, "Local AP Title");
  get_bmatch (&mi_match_local_ae_qualifier, "Local AE Qualifier");
  get_bmatch (&mi_match_local_ap_invoke, "Local AP Invoke ID");
  get_bmatch (&mi_match_local_ae_invoke, "Local AE Invoke ID");
  get_bmatch (&mi_match_local_psel, "Local P-Selector");
  get_bmatch (&mi_match_local_ssel, "Local S-Selector");
  get_bmatch (&mi_match_local_tsel, "Local T-Selector");
  /*get_bmatch (&mi_match_local_net_addr, "Local NSAP or IP");*/

  get_bmatch (&mi_match_remote_ap_title, "Remote AP Title");
  get_bmatch (&mi_match_remote_ae_qualifier, "Remote AE Qualifier");
  get_bmatch (&mi_match_remote_ap_invoke, "Remote AP Invoke ID");
  get_bmatch (&mi_match_remote_ae_invoke, "Remote AE Invoke ID");
  get_bmatch (&mi_match_remote_psel, "Remote P-Selector");
  get_bmatch (&mi_match_remote_ssel, "Remote S-Selector");
  get_bmatch (&mi_match_remote_tsel, "Remote T-Selector");
  /*get_bmatch (&mi_match_remote_net_addr, "Remote NSAP or IP");*/
  }

/************************************************************************/
/*			    updatePtChgTbl				*/
/************************************************************************/

ST_VOID    updatePtChgTbl (ST_VOID)
  {
time_t		theTime;
MIT_DATA_VALUE	*pstMitDataValue;
ST_INT		idx;
ST_INT	range;
ST_INT	numPoints;
ST_INT	i;

  switch(mitChgCtrl.chgMode)
    {
    case CHG_TIME:
      time (&theTime);
      idx = mitChgCtrl.lowRange;
      while (idx <= mitChgCtrl.highRange)
        {
	if (PtChgTbl[idx].ptMode != NO_CHANGE)
	  {
          if (theTime >= PtChgTbl[idx].nextChange)
            {
            pstMitDataValue = PtChgTbl[idx].pstMitDataValue;
            incrementDataValue (pstMitDataValue->dvType, &pstMitDataValue->data);
            mis_dv_change(pstMitDataValue->dvRef);
            PtChgTbl[idx].lastChange = theTime;
            PtChgTbl[idx].nextChange = theTime + PtChgTbl[idx].deltaTime;
            }						      
	  }
	idx++;
	}
      break;

    case CHG_MIXED:
      time (&theTime);
      idx = mitChgCtrl.lowRange;
      while (idx <= mitChgCtrl.highRange)
        {
	if (PtChgTbl[idx].ptMode == READ_CHANGE)
	  {
          if (theTime >= PtChgTbl[idx].nextChange)
            {
            pstMitDataValue = PtChgTbl[idx].pstMitDataValue;
            incrementDataValue (pstMitDataValue->dvType, &pstMitDataValue->data);
            mis_dv_change(pstMitDataValue->dvRef);
            PtChgTbl[idx].lastChange = theTime;
            PtChgTbl[idx].nextChange = theTime + PtChgTbl[idx].deltaTime;
            }						      
	  }
	idx++;
	}
      break;

    case CHG_RANDOM:
      time (&theTime);
      if (( theTime >= mitChgCtrl.nextRandomChange) &&
          ( mitChgCtrl.lowRange < mitChgCtrl.highRange))
        {
        range = mitChgCtrl.highRange - mitChgCtrl.lowRange;
	/* figure out how many points in the window to update		*/
        numPoints = get_random_range (0, range);

	/* for the number of random updates to perform select points	*/
	/* randomly in the window and increment them			*/

	for (i=0; i<numPoints; i++)
	  {
	  idx = get_random_range (mitChgCtrl.lowRange, mitChgCtrl.highRange);
   	  if (PtChgTbl[idx].ptMode != NO_CHANGE)
            {
            pstMitDataValue = PtChgTbl[idx].pstMitDataValue;
            incrementDataValue (pstMitDataValue->dvType, &pstMitDataValue->data);
            mis_dv_change(pstMitDataValue->dvRef);
  	    }
	  } /* end for all points */
        mitChgCtrl.nextRandomChange = theTime + mitChgCtrl.randomDeltaTime;
	}
      break;

    case CHG_NO_CHANGE:
    case CHG_READ:
    default:
      break;
    }

  }


/************************************************************************/
/*		       		updatePt				*/
/************************************************************************/

ST_VOID		updatePt (MIT_DATA_VALUE *pstMitDataValue)
  {
ST_INT	idx;

  idx = pstMitDataValue->ptChgIdx;

  if ((PtChgTbl[idx].ptMode != NO_CHANGE) &&
      (idx >= mitChgCtrl.lowRange) &&
      (idx <= mitChgCtrl.highRange))
    {
    incrementDataValue (pstMitDataValue->dvType, 
                        &pstMitDataValue->data);
    mis_dv_change(pstMitDataValue->dvRef);
    }

  }
