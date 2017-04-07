/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*          1996-2002, All Rights Reserved                              */
/*                                                                      */
/* MODULE NAME : mit_util.c						*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*     Some utilities for the MMS-EASE Lite ICCP Test Application	*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  rkr    2      Copyright year update                        */
/* 03/17/99  rkr    1      Created                                      */
/************************************************************************/


#include "mit_main.h"
#include "scrndefs.h"
#include "gvaldefs.h"
#include "fkeydefs.h"
#include "stime.h"

static char *thisFileName = __FILE__;


typedef struct typeConvType 
  {
  char *name;
  ST_INT constant;
  } TYPE_CONV;

static TYPE_CONV typeConvTable[] = 
  {
    { "Data_State", 		Data_State_TYPEID },
    { "Data_StateQ", 		Data_StateQ_TYPEID },
    { "Data_StateQTimeTag",  	Data_StateQTimeTag_TYPEID },
    { "Data_StateExtended", 	Data_StateExtended_TYPEID },
    { "Data_Discrete", 		Data_Discrete_TYPEID },
    { "Data_DiscreteQ", 	Data_DiscreteQ_TYPEID },
    { "Data_DiscreteQTimeTag", 	Data_DiscreteQTimeTag_TYPEID },
    { "Data_DiscreteExtended", 	Data_DiscreteExtended_TYPEID },
    { "Data_Real", 		Data_Real_TYPEID },
    { "Data_RealQ", 		Data_RealQ_TYPEID },
    { "Data_RealQTimeTag", 	Data_RealQTimeTag_TYPEID },
    { "Data_RealExtended", 	Data_RealExtended_TYPEID }
  };
#define NUM_STD_TYPES 		(sizeof (typeConvTable)/sizeof (TYPE_CONV))


ST_VOID do_key_wait_service (void);

/************************************************************************/
/* 			        findRemote				*/
/************************************************************************/

MI_REMOTE *findRemote(char *remoteName)
  {
MIT_REMOTE *pstRemote;
ST_BOOLEAN bFound = SD_FALSE;

  pstRemote = pstRemoteSet;
  while (pstRemote && !bFound)
    {
    if (strcmp(pstRemote->remoteName, remoteName) == 0)
      bFound = SD_TRUE;
    else
      pstRemote = (MIT_REMOTE *) list_get_next
		  ((ST_VOID *)pstRemoteSet, (ST_VOID *)pstRemote);
    }
  if (bFound)
    return (pstRemote->pstMiRemote);
  else
    return (NULL);

  }

/************************************************************************/
/*			      findMitRemote				*/
/************************************************************************/

MIT_REMOTE	*findMitRemote (ST_CHAR *remoteName)
  {
MIT_REMOTE *pstRemote;
ST_BOOLEAN bFound = SD_FALSE;

  pstRemote = pstRemoteSet;
  while (pstRemote && !bFound)
    {
    if (strcmp(pstRemote->remoteName, remoteName) == 0)
      bFound = SD_TRUE;
    else
      pstRemote = (MIT_REMOTE *) list_get_next
		  ((ST_VOID *)pstRemoteSet, (ST_VOID *)pstRemote);
    }
  if (bFound)
    return (pstRemote);
  else
    return (NULL);

  }

/************************************************************************/
/* 			      findMitMicVcc    				*/
/************************************************************************/

MIT_MIC_VCC *findMitMicVcc (ST_CHAR *remoteName)
  {
MIT_MIC_VCC *pstMitMicVcc;
ST_BOOLEAN bFound = SD_FALSE;

  pstMitMicVcc = pstMicVccSet;
  while (pstMitMicVcc && !bFound)
    {
    if (strcmp(pstMitMicVcc->remoteName, remoteName) == 0)
      bFound = SD_TRUE;
    else
      pstMitMicVcc = (MIT_MIC_VCC *) list_get_next
	       ((ST_VOID *)pstMicVccSet, (ST_VOID *)pstMitMicVcc);
    }

  return (pstMitMicVcc);
  }

/************************************************************************/
/* 			      findMitMisVcc    				*/
/************************************************************************/

MIT_MIS_VCC *findMitMisVcc (ST_CHAR *remoteName)
  {
MIT_MIS_VCC *pstMitMisVcc;
ST_BOOLEAN bFound = SD_FALSE;

  pstMitMisVcc = pstMisVccSet;
  while (pstMitMisVcc && !bFound)
    {
    if (strcmp(pstMitMisVcc->remoteName, remoteName) == 0)
      bFound = SD_TRUE;
    else
      pstMitMisVcc = (MIT_MIS_VCC *) list_get_next
	       ((ST_VOID *)pstMisVccSet, (ST_VOID *)pstMitMisVcc);
    }

  return (pstMitMisVcc);
  }

/************************************************************************/
/* 			     findAssociation 				*/
/************************************************************************/

MIT_ASSOC_INFO *findAssociation (char *associationName)
  {
MIT_ASSOC_INFO *pstAI;
ST_BOOLEAN bFound = SD_FALSE;

  pstAI = pstAssocInfoSet;
  while (pstAI && !bFound)
    {
    if (strcmp(pstAI->associationName, associationName) == 0)
      bFound = SD_TRUE;
    else
      pstAI = (MIT_ASSOC_INFO *) list_get_next
	       ((ST_VOID *)pstAssocInfoSet, (ST_VOID *)pstAI);
    }

  return (pstAI);
  }

/************************************************************************/
/* 			       findDataSet	     			*/
/************************************************************************/

ST_VOID *findDataSet (MIT_DATA_SET *pstDataSets, ST_CHAR *dataSetName)
  {
MIT_DATA_SET	*pstCurDataSet;
ST_BOOLEAN bFound = SD_FALSE;
MIC_DS		*pstMicDs;

  pstMicDs = NULL;
  pstCurDataSet = pstDataSets;
  while (pstCurDataSet && !bFound)
    {
    if (strcmp (pstCurDataSet->dsName, dataSetName) == 0)
      {
      bFound = SD_TRUE;
/*      pstMicDs = (_mic_ds *)pstCurDataSet->pstDS;
modified 2006.11.24 zy*/
      pstMicDs = (struct _mic_ds *)pstCurDataSet->pstDS;
      }
    else
      pstCurDataSet = (MIT_DATA_SET *) list_get_next 
                 ((ST_VOID *)pstDataSets, (ST_VOID *)pstCurDataSet);
    }
  return (pstMicDs);
  }								      

/************************************************************************/
/* 			       findTransferSet	     			*/
/************************************************************************/

MIC_DSTS 	*findTransferSet (MIT_TRANSFER_SET *pstTransferSets, 
                                  ST_CHAR *dsTransferSetName)
  {
MIT_TRANSFER_SET	*pstCurTransferSet;
ST_BOOLEAN 		bFound = SD_FALSE;
MIC_DSTS 		*pstMicDsTs;

  pstMicDsTs = NULL;
  pstCurTransferSet = pstTransferSets;
  while (pstCurTransferSet && !bFound)
    {
    if (strcmp (pstCurTransferSet->dsTsName, dsTransferSetName) == 0)
      {
      bFound = SD_TRUE;
      pstMicDsTs = pstCurTransferSet->pstDSTS;
      }
    else
      pstCurTransferSet = (MIT_TRANSFER_SET *) list_get_next 
                 ((ST_VOID *)pstTransferSets, (ST_VOID *)pstCurTransferSet);
    }
  return (pstMicDsTs);
  }								      

/************************************************************************/
/* 			     findMitTransferSet	     			*/
/************************************************************************/

MIT_TRANSFER_SET *findMitTransferSet (MIT_TRANSFER_SET *pstTransferSets, 
                                      ST_CHAR *dsTransferSetName)
  {
MIT_TRANSFER_SET	*pstCurTransferSet;
ST_BOOLEAN 		bFound = SD_FALSE;

  pstCurTransferSet = pstTransferSets;
  while (pstCurTransferSet && !bFound)
    {
    if (strcmp (pstCurTransferSet->dsTsName, dsTransferSetName) == 0)
      {
      bFound = SD_TRUE;
      }
    else
      pstCurTransferSet = (MIT_TRANSFER_SET *) list_get_next 
                 ((ST_VOID *)pstTransferSets, (ST_VOID *)pstCurTransferSet);
    }
  return (pstCurTransferSet);
  }								      

/************************************************************************/
/*		                findMitDataSet				*/
/************************************************************************/

MIT_DATA_SET		*findMitDataSet (MIT_DATA_SET  *pstDataSets, ST_CHAR *dsName)
  {
MIT_DATA_SET		*pstCurDataSet;
ST_BOOLEAN 		bFound = SD_FALSE;

  pstCurDataSet = pstDataSets;
  while (pstCurDataSet && !bFound)
    {
    if (strcmp (pstCurDataSet->dsName, dsName) == 0)
      {
      bFound = SD_TRUE;
      }
    else
      pstCurDataSet = (MIT_DATA_SET *) list_get_next 
                 ((ST_VOID *)pstDataSets, (ST_VOID *)pstCurDataSet);
    }
  return (pstCurDataSet);
  }

/************************************************************************/
/*			pause_msg					*/
/* Function to display a message and wait for a key hit 		*/
/************************************************************************/

ST_VOID pause_msg (ST_CHAR *str)
  {
#ifdef _WIN32
  flush_keys ();
  printf ("\n %s",str);
  do_key_wait_service ();
  flush_keys ();
#else
  flush_keys ();
  printf ("\n %s",str);
  while (!kbhit())
    (*servefun) ();			/* execute background task	*/
  flush_keys ();
#endif
  }

/************************************************************************/
/* startInterval:  set current time for use in elapsed time calc	*/
/************************************************************************/

void startInterval( void )
  {
  stimeInit();
  beginTime = sGetMsTime ();
  }

/************************************************************************/
/* endInterval:  return elapsed time for the interval			*/
/************************************************************************/

double endInterval( void )
  {
  endTime = sGetMsTime ();
  return (endTime - beginTime);
  }


/************************************************************************/
/*			do_dyn_mem_chk					*/
/* Check the status of the dynamic memory pointers.			*/
/************************************************************************/

ST_VOID do_dyn_mem_chk (ST_VOID)
  {

  if (ask ("\n Statistics Summary Only (Y) ? ",SD_TRUE))
    {
    printf ("\n\n Total number of allocated pointers:   %d",
  		chk_curr_mem_count);
    printf ("\n Total amount of memory allocated:     %ld bytes ",
  		chk_curr_mem_size);
    printf ("\n Maximum number of pointers allocated: %d",
  		chk_max_dyn_mem_ptrs);
    printf ("\n Maximum amount of memory allocated:   %ld bytes",
  		chk_max_mem_allocated);
    wait_key ();
    }
  else
    dyn_mem_ptr_status ();

  (*menu_set_fun) ();
  }
  
