
#define GWMAIN_C


//#define PROASSET_TEST

#include "cppinc.h"

#include "gwdatastructure.h"

#include "gwexternfct.h"

#include "gwexterndek.h"





#ifdef PROASSET_TEST
char *gettimstr( void );
#endif





int               nGWRun;

sGWProcessCntrl  *pPrcCntrlBas;
sGWProcessCntrl  *pPrcCntrl;

sGWTransTbl      *pTransTblBas;
sGWTransTbl      *pTransTblFst;
sGWTransTbl     **pNamBas;
long int          nMaxSigNum;

sGWTransTbl     **pIMSIdBas;
long int          nMaxIMSIdNum;

sGWTransTbl     **pIMSNamBas;
long int          nMaxIMSNamNum;

sGWTransTbl     **pDC0NamBas;
long int          nMaxDC0NamNum;

sGWTransTbl     **pDC1NamBas;
long int          nMaxDC1NamNum;

sGWTransTbl     **pDC2NamBas;
long int          nMaxDC2NamNum;

sGWTransTbl     **pAppCChnBas;
long int          nMaxAppCChnNum;

sGWSignalValue   *pSigValBas;
sGWSignalValue   *pSigValFstFre;
sGWSignalValue   *pSigValLstFre;
long int          nMaxSigValNum;

sQueue           *pQ_101r;
sQueue           *pQ_101w;
sQueue           *pQ_IMS;
sQueue           *pQ_DC0w;
sQueue           *pQ_DC1w;
sQueue           *pQ_DC2w;
sQueue           *pQ_DCr;
sQueue           *pQ_Prt;
sQueue            sQDat_101r;
sQueue            sQDat_101w;
sQueue            sQDat_IMS;
sQueue            sQDat_DC0w;
sQueue            sQDat_DC1w;
sQueue            sQDat_DC2w;
sQueue            sQDat_DCr;
sQueue            sQDat_Prt;

sSharedMemory    *pM_Data;
sSharedMemory    *pM_Prc;
sSemaphore       *pS_Data;
sSharedMemory     sMDat_Data;
sSharedMemory     sMDat_Prc;
sSemaphore        sSDat_Data;

sGWSigTASE2      *pTASE2_BufR;
sGWSigTASE2       sMemory[256];
sGWSigTASE2       sTASE2_BufR_Data;
sGWSigTASE2      *pTASE2_BufW;
sGWSigTASE2       sTASE2_BufW_Data;

sGWSignalTypeX   *p101_BufR;
sGWSignalTypeX    s101_BufR_Data;
sGWSignalTypeX   *p101_BufW;
sGWSignalTypeX    s101_BufW_Data;

sGWProtocolMsg   *pPrtMsg;
sGWProtocolMsg    sPrtMsgDat;





// signal handler
void catch_sig(int sig_num)
{
  // reset the signal handler for catching signal for the next time
  signal(SIGINT, catch_sig);
  signal(SIGKILL, catch_sig);
  pPrcCntrl->nWork = 0;
}





int main(int argc, char* argv[])

{

  char            *args[8];
  bool             bDblI;
  FLAGS            bValue;
  int              nCnt;
  long int         nDblChnNum;
  int              nDblIBit;
  long int         nId;
  long int         nIdx;
  int              nLng;
  long int         nNum;
  int              nPrcId;
  int              nSrc;
  int              nSta;
  int              nSwtch = 0;
  long int         nTim;
  long int         nTimSav = 0;
  int              nValid;
  byte            *pC;
  sGWTransTbl     *pTransTbl;
  sGWTransTbl     *pTransTblDblI;
  double           rValue;
  char             strDC[8];

  struct timeb     timbuf;
  struct tm        timdat;    



#ifdef PROASSET_TEST
  FILE            *pFil;
  pFil = fopen("gwmain.log", "w");
  fprintf(pFil, "%s: GWMAIN - application start\n", gettimstr());
#endif


  pPrtMsg = &sPrtMsgDat;



  // build up base data structures
  // -----------------------------

  pM_Prc = &sMDat_Prc;
  pM_Prc->id = 0;
  pM_Prc->key = SHM_PRCCNTRL_KEY;
  pM_Prc->adr = (unsigned char *)NULL;
  nLng = 64 * sizeof(sGWProcessCntrl);
  pM_Prc->siz = nLng;
  strcpy(pM_Prc->nam, SHM_PRCCNTRL_NAM);
  if (shm_clink(pM_Prc) == -1) {
    // Error
    return(-1);
  }
  pC = pM_Prc->adr;
  while (nLng--) *pC++ = 0x00;
  pPrcCntrlBas = (sGWProcessCntrl *)pM_Prc->adr;
  pPrcCntrl = pPrcCntrlBas + PRC_NUM_GWMAIN;
  pPrcCntrl->nPrc = PRC_NUM_GWMAIN;
  strcpy(pPrcCntrl->strNam, "gwmain");
  pPrcCntrl->nWork = 1;

  // install signal handler
  signal(SIGINT, catch_sig);
  signal(SIGKILL, catch_sig);

  

  // create protocol interface
  // -------------------------

  // create queue
  pQ_Prt = &sQDat_Prt;
  pQ_Prt->q_key = Q_PRT_KEY;
  strcpy(pQ_Prt->q_nam, Q_PRT_NAM);
  pQ_Prt->q_siz = Q_PRT_SIZ;
  if (_q_clink1(pQ_Prt, 1) == -1) {
    // ERROR
    pPrcCntrl->nWork = 0;
    shm_unlink(pM_Prc);
    return(-1);
  }
  prt_write(pQ_Prt, MSG_TYP_INFO, 8, PRC_NUM_GWMAIN, 0,
              "link to protocol queue - ok", NULL, NULL, NULL);

  // start protocol process gwprt
/*
  pPrcCntrl = pPrcCntrlBas + PRC_NUM_GWPRT;
  pPrcCntrl->nPrc = PRC_NUM_GWPRT;
  strcpy(pPrcCntrl->strNam, "gwprt");
  pPrcCntrl->nWork = 1;
#ifdef WINDOWS32
  args[0] = "gwprt";
  args[1] = NULL;
  nSta = _spawnvp(_P_NOWAIT, "E:\\Projects\\AL\\AL001\\Software\\gateway\\gwprt\\Debug\\gwprt.exe", args);
#endif
#ifdef HPUX
  {
    pid_t  pid_gwprt;
    if ((pid_gwprt = fork()) < 0) {
      // ERROR
    } else if (pid_gwprt == 0) { 
      // this is the child process
      if (execlp(PRC_FIL_GWPRT, PRC_FIL_GWPRT, NULL) < 0) {
        // ERROR
      }
    }
  }
#endif
  prt_write(pQ_Prt, MSG_TYP_INFO, 8, PRC_NUM_GWMAIN, 0,
              "start of child process gwprt - ok", NULL, NULL, NULL);
*/



  // read base configuration file
  // ----------------------------

  nSta = gwinittbl();
  if (nSta == - 1) {
    // ERROR - configuration file
    pPrcCntrl = pPrcCntrlBas + PRC_NUM_GWMAIN;
    pPrcCntrl->nWork = 0;
    _q_unlink(pQ_Prt);
    shm_unlink(pM_Prc);
  }


  
  // create queues
  // -------------

  // IMS interface (read)
  pQ_IMS = &sQDat_IMS;
  pQ_IMS->q_key = Q_IMS_KEY;
  strcpy(pQ_IMS->q_nam, Q_IMS_NAM);
  pQ_IMS->q_siz = Q_IMS_SIZ;
  if (_q_clink1(pQ_IMS, 1) == -1) {
    // ERROR
  }
  prt_write(pQ_Prt, MSG_TYP_INFO, 8, PRC_NUM_GWMAIN, 0,
              "link to ims queue - ok", NULL, NULL, NULL);

  // ICCP/TASE.2 interface to DC0 (write)
  pQ_DC0w = &sQDat_DC0w;
  pQ_DC0w->q_key = Q_DC0O_KEY;
  strcpy(pQ_DC0w->q_nam, Q_DC0O_NAM);
  pQ_DC0w->q_siz = Q_DC0O_SIZ;
  if (_q_clink1(pQ_DC0w, 1) == -1) {
    // ERROR
  }

  // ICCP/TASE.2 interface from DC's (read)
  pQ_DCr = &sQDat_DCr;
  pQ_DCr->q_key = Q_DCI_KEY;
  strcpy(pQ_DCr->q_nam, Q_DCI_NAM);
  pQ_DCr->q_siz = Q_DCI_SIZ;
  if (_q_clink1(pQ_DCr, 1) == -1) {
    // ERROR
  }

  // IEC 80670-5-101 interface (write)
  pQ_101w = &sQDat_101w;
  pQ_101w->q_key = Q_101O_KEY;
  strcpy(pQ_101w->q_nam, Q_101O_NAM);
  pQ_101w->q_siz = Q_101O_SIZ;
  if (_q_clink1(pQ_101w, 1) == -1) {
    // ERROR
  }

  // IEC 80670-5-101 interface (read)
  pQ_101r = &sQDat_101r;
  pQ_101r->q_key = Q_101I_KEY;
  strcpy(pQ_101r->q_nam, Q_101I_NAM);
  pQ_101r->q_siz = Q_101I_SIZ;
  if (_q_clink1(pQ_101r, 1) == -1) {
    // ERROR
  }



  // start child processes
  // ---------------------
  //   - interface to application workstation
  //   - IMS interface
  //   - ICP/TASE.2 interface to dispatch centers
  //   - IEC 80670-5-101 interface to application controller
  //   - IEC 80670-5-101 interfaces to dispatch centers

  //   - child process gwiec101
  // --------------------------

  pPrcCntrl = pPrcCntrlBas + PRC_NUM_GWIEC101;
  pPrcCntrl->nPrc = PRC_NUM_GWIEC101;
  strcpy(pPrcCntrl->strNam, "gwiec101");
  pPrcCntrl->nWork = 1;
#ifdef WINDOWS32
  args[0] = "gwims";
  args[1] = NULL;
  nSta = _spawnvp(_P_NOWAIT, "E:\\Projects\\AL\\AL001\\Software\\gateway\\gwims\\Debug\\gwims.exe", args);
#endif
#ifdef HPUX
  if ((nPrcId = fork()) < 0) {
    // ERROR;
  } else if (nPrcId == 0) {  // program executes child process
    if (execlp(PRC_FIL_GWIEC101, PRC_FIL_GWIEC101, NULL) < 0) {
      // ERROR - execlp failed;
    }
  }
  // parent process continues
#endif
  prt_write(pQ_Prt, MSG_TYP_INFO, 8, PRC_NUM_GWIEC101, 0,
              "start of child process gwiec101 - ok", NULL, NULL, NULL);



  nCnt = 10;
  while (nCnt--) sleep(999999);  // sleep 10 seconds



  //   - child process gwdc (ICCP/TASE.2 communication with dispatch centers)
  // ------------------------------------------------------------------------

#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - start TASE.2 interfaces\n", gettimstr());
#endif
  pPrcCntrl = pPrcCntrlBas + PRC_NUM_GWDC0;
  pPrcCntrl->nPrc = PRC_NUM_GWDC0;
  strcpy(pPrcCntrl->strPar0, "0");
  strcpy(pPrcCntrl->strNam, "gwdc");
  pPrcCntrl->nWork = 1;
#ifdef WINDOWS32
  args[0] = "0";
  args[1] = "gwdc";
  args[2] = NULL;
  nSta = _spawnvp(_P_NOWAIT, "E:\\Projects\\AL\\AL001\\Software\\gateway\\gwdc\\Debug\\gwdc.exe", args);
#endif
#ifdef HPUX
  if ((nPrcId = fork()) < 0) {
    // ERROR;
  } else if (nPrcId == 0) {  // program executes child process
    if (execlp(PRC_FIL_GWDC, PRC_FIL_GWDC, "0", PRC_FIL_GWDC, NULL) < 0) {
      // ERROR - execlp failed;
    }
  }
#endif
  // parent process continues
  prt_write(pQ_Prt, MSG_TYP_INFO, 8, PRC_NUM_GWMAIN, 0,
              "start of child process gwdc - ok", NULL, NULL, NULL);



  // setting initial values with bad quality
  // ---------------------------------------

  pTASE2_BufW = &sTASE2_BufW_Data;
  nLng = sizeof(sGWSigTASE2);
  pC = (byte *)pTASE2_BufW;
  while (nLng--) *pC++ = 0x00;
  pTASE2_BufW->nFct = ModifyLocalDataValue;
  pTASE2_BufW->nValBits = 0;
  pTASE2_BufW->nVal = 0;
  pTASE2_BufW->rVal = 0.0;
  pTASE2_BufW->nValidity = 0x01;  //bad quality
  pTASE2_BufW->nCurrentSource = 0;
  pTASE2_BufW->nNormalValue = 0;
  pTASE2_BufW->nTimeStampQuality = 0;
  pTASE2_BufW->nOverflow = 0;
  pTASE2_BufW->sTimeStamp.nTim = 0;
  pTASE2_BufW->sTimeStamp.nMSec = 0;

  pTASE2_BufW->nFct = StopAssociations;
  nLng = sizeof(sGWSigTASE2);
  nSta = _q_write(pQ_DC0w, nLng,
                    (unsigned char *)pTASE2_BufW, 1);
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - stopped all associations\n", gettimstr());
  fprintf(pFil, "%s: GWMAIN - setting inital (invalid) values\n", gettimstr());
#endif
  for (nCnt = 0; nCnt < nMaxSigNum; ++nCnt) {
    pTransTbl = pTransTblBas + nCnt;
    if (!(pTransTbl->nType & (TYPE_MEASUREMENT | TYPE_INDICATION))) continue;
    pTASE2_BufW->nFct = ModifyLocalDataValue;
    if (pTransTbl->nType & TYPE_MEASUREMENT) {
      pTASE2_BufW->nType = TYPE_MEASUREMENT;
      pTASE2_BufW->nValBits = 0x00;
      pTASE2_BufW->nVal = 0;
      pTASE2_BufW->rVal = pTransTbl->rMinVal;    // minimum value
      pTransTbl->rVal = pTransTbl->rMinVal;
      pTransTbl->nValFlg = 0x01;    // invalid
    }
    if (pTransTbl->nType & TYPE_INDICATION) {
      pTASE2_BufW->nType = TYPE_INDICATION;
      pTASE2_BufW->nValBits = 0x01;
      pTASE2_BufW->nVal = 0;
      pTASE2_BufW->rVal = 0.0;
      pTransTbl->bVal = 0x00;    // off
      pTransTbl->nValFlg = 0x01;    // invalid
    }
/*
    if (pTransTbl->bDC0) {
      pTASE2_BufW->nDst = 0x01;
      strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC0Nam);
      nSta = _q_write(pQ_DC0w, sizeof(sGWSigTASE2),
                        (unsigned char *)pTASE2_BufW, 1);
#ifdef PROASSET_TEST_X
  fprintf(pFil, "               '%s' - CDC  '%s'\n", pTASE2_BufW->strDCNam, pTransTbl->strDC0Nam);
#endif
    }
    if (pTransTbl->bDC1) {
      pTASE2_BufW->nDst = 0x02;
      strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC1Nam);
      nSta = _q_write(pQ_DC0w, sizeof(sGWSigTASE2),
                        (unsigned char *)pTASE2_BufW, 1);
#ifdef PROASSET_TEST_X
  fprintf(pFil, "               '%s' - NDC  '%s'\n", pTASE2_BufW->strDCNam, pTransTbl->strDC1Nam);
#endif
    }
    if (pTransTbl->bDC2) {
      pTASE2_BufW->nDst = 0x04;
      strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC2Nam);
      nSta = _q_write(pQ_DC0w, sizeof(sGWSigTASE2),
                        (unsigned char *)pTASE2_BufW, 1);
#ifdef PROASSET_TEST_X
  fprintf(pFil, "               '%s' - CCDC '%s'\n", pTASE2_BufW->strDCNam, pTransTbl->strDC2Nam);
#endif
    }
*/
  }

  pTASE2_BufW->nFct = StartAssociations;
  nLng = sizeof(sGWSigTASE2);
  nSta = _q_write(pQ_DC0w, nLng,
                    (unsigned char *)pTASE2_BufW, 1);
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - started all associations\n", gettimstr());
#endif



  nCnt = 10;
  while (nCnt--) sleep(999999);  // sleep 10 seconds



  //   - child process gwiec101
  // --------------------------
/*
  pPrcCntrl = pPrcCntrlBas + PRC_NUM_GWIEC101;
  pPrcCntrl->nPrc = PRC_NUM_GWIEC101;
  strcpy(pPrcCntrl->strNam, "gwiec101");
  pPrcCntrl->nWork = 1;
#ifdef WINDOWS32
  args[0] = "gwims";
  args[1] = NULL;
  nSta = _spawnvp(_P_NOWAIT, "E:\\Projects\\AL\\AL001\\Software\\gateway\\gwims\\Debug\\gwims.exe", args);
#endif
#ifdef HPUX
  if ((nPrcId = fork()) < 0) {
    // ERROR;
  } else if (nPrcId == 0) {  // program executes child process
    if (execlp(PRC_FIL_GWIEC101, PRC_FIL_GWIEC101, NULL) < 0) {
      // ERROR - execlp failed;
    }
  }
  // parent process continues
#endif
  prt_write(pQ_Prt, MSG_TYP_INFO, 8, PRC_NUM_GWIEC101, 0,
              "start of child process gwiec101 - ok", NULL, NULL, NULL);



  nCnt = 10;
  while (nCnt--) sleep(999999);  // sleep 10 seconds

*/

  //   - child process gwappws (schedule handling)
  // ---------------------------------------------
/*
  pPrcCntrl = pPrcCntrlBas + PRC_NUM_GWAPPWS;
  pPrcCntrl->nPrc = PRC_NUM_GWAPPWS;
  strcpy(pPrcCntrl->strNam, "gwappws");
  pPrcCntrl->nWork = 1;
#ifdef WINDOWS32
  args[0] = "gwappws";
  args[1] = NULL;
  nSta = _spawnvp(_P_NOWAIT, "E:\\Projects\\AL\\AL001\\Software\\gateway\\gwappws\\Debug\\gwappws.exe", args);
#endif
#ifdef HPUX
  if ((nPrcId = fork()) < 0) {
    // ERROR;
  } else if (nPrcId == 0) {  // program executes child process
    if (execlp(PRC_FIL_GWAPPWS, PRC_FIL_GWAPPWS, NULL) < 0) {
      // ERROR - execlp failed;
    }
  }
  // parent process continues
#endif
  prt_write(pQ_Prt, MSG_TYP_INFO, 8, PRC_NUM_GWMAIN, 0,
              "start of child process gwappws - ok", NULL, NULL, NULL);
*/



  //   - child process gwims (get ims data)
  // --------------------------------------

  pPrcCntrl = pPrcCntrlBas + PRC_NUM_GWIMS;
  pPrcCntrl->nPrc = PRC_NUM_GWIMS;
  strcpy(pPrcCntrl->strNam, "gwims");
  pPrcCntrl->nWork = 1;
#ifdef WINDOWS32
  args[0] = "gwims";
  args[1] = NULL;
  nSta = _spawnvp(_P_NOWAIT, "E:\\Projects\\AL\\AL001\\Software\\gateway\\gwims\\Debug\\gwims.exe", args);
#endif
#ifdef HPUX
  if ((nPrcId = fork()) < 0) {
    // ERROR;
  } else if (nPrcId == 0) {  // program executes child process
    if (execlp(PRC_FIL_GWIMS, PRC_FIL_GWIMS, NULL) < 0) {
      // ERROR - execlp failed;
    }
  }
  // parent process continues
#endif
  prt_write(pQ_Prt, MSG_TYP_INFO, 8, PRC_NUM_GWIMS, 0,
              "start of child process gwims - ok", NULL, NULL, NULL);



  nCnt = 10;
  while (nCnt--) sleep(999999);  // sleep 10 seconds



  p101_BufR = &s101_BufR_Data;
  p101_BufW = &s101_BufW_Data;
  pTASE2_BufR = &sTASE2_BufR_Data;
  pTASE2_BufW = &sTASE2_BufW_Data;

  pPrcCntrl = pPrcCntrlBas + PRC_NUM_GWMAIN;   //added 2003.7.15 zy
  pPrcCntrl->nWork = 1;

  // enter gateway working loop
  // --------------------------
  while (pPrcCntrl->nWork) {



/*
//#########################################################################
//## TEST: SoE Message
//#########################################################################
{
    time(&nTim);
    if ((nTim - nTimSav) >= 30) {
      nTimSav = nTim;
      if (nSwtch) nSwtch = 0; else nSwtch = 1;
      nId = gwfindtbl(20, (char *)NULL, 2256);
      if (nId != -1) {
        nLng = sizeof(sGWSigTASE2);
        pC = (byte *)pTASE2_BufW;
        while (nLng--) *pC++ = 0x00;
        bValue = nSwtch;
        nValid = 0;
        pTransTbl->bVal = bValue;
        pTransTbl->nValFlg = nValid;
        bValue = (bValue) ? 0x02 : 0x01;  // = nValBits
        pTASE2_BufW->nFct = ModifyLocalDataValue;
        pTASE2_BufW->nType = TYPE_INDICATION;
        pTASE2_BufW->nValBits = bValue;
        pTASE2_BufW->nValidity = nValid;
        pTASE2_BufW->nVal = INFOMSG_REF_SOE;    // InfoRef SoE message 
        ftime(&timbuf);
        pTASE2_BufW->sTimeStamp.nTim = timbuf.time;
        pTASE2_BufW->sTimeStamp.nMSec = timbuf.millitm;
        pTASE2_BufW->nFct = SendInformationMessage;
        if (pTransTbl->bDC0) {
          pTASE2_BufW->nDst = 0x01;
          strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC0Nam);
          nLng = sizeof(sGWSigTASE2);
          nSta = _q_write(pQ_DC0w, nLng,
                            (unsigned char *)pTASE2_BufW, 1);
        }
        if (pTransTbl->bDC1) {
          pTASE2_BufW->nDst = 0x02;
          nLng = sizeof(sGWSigTASE2);
          strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC1Nam);
          nSta = _q_write(pQ_DC0w, nLng,
                            (unsigned char *)pTASE2_BufW, 1);
        }
        if (pTransTbl->bDC2) {
          pTASE2_BufW->nDst = 0x04;
          nLng = sizeof(sGWSigTASE2);
          strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC2Nam);
          nSta = _q_write(pQ_DC0w, nLng,
                            (unsigned char *)pTASE2_BufW, 1);
        }
      }
    }
}
//#########################################################################
*/



    // read from IEC 101 receive queue
    // -------------------------------
    nLng = sizeof(sGWSignalTypeX);   //added 2003.7.20 zy for check length in queue

    nSta = _q_read(pQ_101r, &nLng, (unsigned char *)p101_BufR, 0);
    if (nSta == 0) {
#ifdef PROASSET_TEST
fprintf(pFil, "%s: GWMAIN - from IEC101 Length %d - Type %d - Source %d - Channel %d\n",
gettimstr(), nLng, p101_BufR->nTyp, p101_BufR->nSrc, p101_BufR->nChnNum);
#endif

      // search signal in translation table
      nId = gwfindtbl(20, (char *)NULL, p101_BufR->nChnNum);
      if (nId == -1) {

        // ERROR;
        nSta = -2;
printf("ERROR: signal %d from IEC101 receive queue not in translation table\n",
  p101_BufR->nChnNum);
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - signal %d received from IEC1010 undefined\n",
    gettimstr(), p101_BufR->nChnNum);
#endif

      } else {

        pTransTbl = *(pAppCChnBas + nId);
printf("GWMAIN - signal '%s' - '%s' - '%s' - '%s'\n",
pTransTbl->strIMSNam, pTransTbl->strDC0Nam, 
pTransTbl->strDC1Nam, pTransTbl->strDC2Nam);
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - signal '%s' - '%s' - '%s' - '%s'\n", gettimstr(),
    pTransTbl->strIMSNam, pTransTbl->strDC0Nam, 
    pTransTbl->strDC1Nam, pTransTbl->strDC2Nam);
#endif

        nLng = sizeof(sGWSigTASE2);
        pC = (byte *)pTASE2_BufW;
        while (nLng--) *pC++ = 0x00;

#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - Quality / Value %02x / %f - ValueType %d'\n", gettimstr(),
    p101_BufR->nQuality, ((p101_BufR->nTyp == 9) ? p101_BufR->uValue.fValue : 0), 
    p101_BufR->nValueType);
#endif
        switch (p101_BufR->nTyp) {

          case 1: // indication
          case 2: // indication with time tag
            bValue = p101_BufR->nQuality & 0x01;
            nValid = (p101_BufR->nQuality & 0x80) ? 1 : 0;
            pTransTbl->bVal = bValue;
            pTransTbl->nValFlg = nValid;
            // comment: DC awaits 0x80 for CLOSE (bValue == 2)
            //                    0x40 for OPEN (bValue == 1)
            //          TASE.2 sends ((nValBits << 6) + (nValidity << 4))
            bValue = (bValue) ? 0x02 : 0x01;  // = nValBits
            pTASE2_BufW->nFct = ModifyLocalDataValue;
            pTASE2_BufW->nType = TYPE_INDICATION;
            pTASE2_BufW->nValBits = bValue;
            pTASE2_BufW->nValidity = nValid;
            if (p101_BufR->nTyp == 2) {
              pTASE2_BufW->nVal = INFOMSG_REF_SOE;    // InfoRef SoE message
              pTASE2_BufW->nFct = SendInformationMessage;
              switch (p101_BufR->nTimeTagType) {
                case 2:
                  timdat.tm_year = p101_BufR->nYear;
                  timdat.tm_mon = p101_BufR->nMonth - 1;
                  timdat.tm_mday = p101_BufR->nDay;
                  timdat.tm_hour = p101_BufR->nHours;
                  timdat.tm_min = p101_BufR->nMinutes;
                  timdat.tm_sec = p101_BufR->nSeconds;
                  timdat.tm_year = p101_BufR->nYear;
                  timdat.tm_wday = p101_BufR->nDayOfWeek - 1;
                  timdat.tm_isdst = 0;
                  pTASE2_BufW->sTimeStamp.nTim = mktime(&timdat);
                  pTASE2_BufW->sTimeStamp.nMSec = p101_BufR->nMs;
                  break;
                case 1:
                  time(&nTim);
                  timdat = *localtime(&nTim);
                  timdat.tm_min = p101_BufR->nMinutes;
                  timdat.tm_sec = p101_BufR->nSeconds;
                  pTASE2_BufW->sTimeStamp.nTim = mktime(&timdat);

/* Modified 2003.7.2 zy for short time tag from IEC 101
                  pTASE2_BufW->sTimeStamp.nTim = 60 * p101_BufR->nMinutes
                                                   + 3600 * p101_BufR->nHours;
*/
                  pTASE2_BufW->sTimeStamp.nMSec = p101_BufR->nMs;
                  break;
                case 0:
                  pTASE2_BufW->sTimeStamp.nTim = 0;
                  pTASE2_BufW->sTimeStamp.nMSec = 0;
                  break;
                default:
                  pTASE2_BufW->sTimeStamp.nTim = 0;
                  pTASE2_BufW->sTimeStamp.nMSec = 0;
                  break;
              } 
              pTASE2_BufW->nFct = SendInformationMessage;
printf("GWMAIN - INDICATION(%d) - Value %02x - TimeStamp %d.%03d\n",
  p101_BufR->nTyp, p101_BufR->nQuality,
  pTASE2_BufW->sTimeStamp.nTim, pTASE2_BufW->sTimeStamp.nMSec);
#ifdef PROASSET_TEST
fprintf(pFil, "%s - GWMAIN - INDICATION(%d) - Value %02x - TimeStamp %d.%03d\n",
  gettimstr(), p101_BufR->nTyp, p101_BufR->nQuality,
  pTASE2_BufW->sTimeStamp.nTim, pTASE2_BufW->sTimeStamp.nMSec);
#endif
            } else {  
printf("GWMAIN - INDICATION(%d) - Value %02x - TimeStamp %d.%03d\n",
  p101_BufR->nTyp, p101_BufR->nQuality);
#ifdef PROASSET_TEST
fprintf(pFil, "%s: GWMAIN - INDICATION(%d) - Value %02x - TimeStamp %d.%03d\n",
  gettimstr(), p101_BufR->nTyp, p101_BufR->nQuality);
#endif
            }
            if (pTransTbl->bDC0) {
/*
              if (pTransTbl->bDCODblI && !pTransTbl->nDC0Bit) {
                pTASE2_BufW->nValBits = ((p101_BufR->nQuality & 0x01) ? 0x01 : 0x02);
              }
*/
              pTASE2_BufW->nDst = 0x01;
              strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC0Nam);
              nLng = sizeof(sGWSigTASE2);
              nSta = _q_write(pQ_DC0w, nLng,
                                (unsigned char *)pTASE2_BufW, 1);
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - indication to CDC  - %02x\n", gettimstr(),
    ((pTASE2_BufW->nValBits << 6) + (pTASE2_BufW->nValidity << 4)));
#endif
            }
            if (pTransTbl->bDC1) {
/*
              if (pTransTbl->bDC1DblI && !pTransTbl->nDC1Bit) {
                pTASE2_BufW->nValBits = ((p101_BufR->nQuality & 0x01) ? 0x01 : 0x02);
              }
*/
              pTASE2_BufW->nDst = 0x02;
              strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC1Nam);
              nLng = sizeof(sGWSigTASE2);
              nSta = _q_write(pQ_DC0w, nLng,
                                (unsigned char *)pTASE2_BufW, 1);
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - indication to NDC - %02x\n", gettimstr(),
    ((pTASE2_BufW->nValBits << 6) + (pTASE2_BufW->nValidity << 4)));
#endif
            }
            if (pTransTbl->bDC2) {
/*
              if (pTransTbl->bDC2DblI && !pTransTbl->nDC2Bit) {
                pTASE2_BufW->nValBits = ((p101_BufR->nQuality & 0x01) ? 0x01 : 0x02);
              }
*/
              pTASE2_BufW->nDst = 0x04;
              strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC2Nam);
              nLng = sizeof(sGWSigTASE2);
              nSta = _q_write(pQ_DC0w, nLng,
                                  (unsigned char *)pTASE2_BufW, 1);
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - indication to CCDDC - %02x\n", gettimstr(),
    ((pTASE2_BufW->nValBits << 6) + (pTASE2_BufW->nValidity << 4)));
#endif
            }
            break;

          case 9:  // analogue value
printf("GWMAIN - MEASUREMENT - Quality %02x - Value %f\n",
       p101_BufR->nQuality, p101_BufR->uValue.fValue);
            rValue = p101_BufR->uValue.fValue;
            nValid = (p101_BufR->nQuality & 0x80) ? 1 : 0;
            pTransTbl->rVal = rValue;
            pTransTbl->nValFlg = nValid;
            nLng = sizeof(sGWSigTASE2);
            pC = (byte *)pTASE2_BufW;
            while (nLng--) *pC++ = 0x00;
            pTASE2_BufW->nFct = ModifyLocalDataValue;
            pTASE2_BufW->nType = TYPE_MEASUREMENT;
            pTASE2_BufW->rVal = rValue;
            pTASE2_BufW->nValidity = nValid;
            pTASE2_BufW->nCurrentSource = 0;
            pTASE2_BufW->nNormalValue = 0;
            pTASE2_BufW->nTimeStampQuality = 0;
            pTASE2_BufW->nOverflow = 0;
            pTASE2_BufW->sTimeStamp.nTim = 0;
            pTASE2_BufW->sTimeStamp.nMSec = 0;
            if (pTransTbl->bDC0) {
              pTASE2_BufW->nDst = 0x01;
              strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC0Nam);
              nLng = sizeof(sGWSigTASE2);
              nSta = _q_write(pQ_DC0w, nLng,
                                (unsigned char *)pTASE2_BufW, 1);
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - analogue value to CDC  - %f %02x\n", gettimstr(),
    pTASE2_BufW->rVal, pTASE2_BufW->nValidity);
#endif
            }
            if (pTransTbl->bDC1) {
              pTASE2_BufW->nDst = 0x02;
              strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC1Nam);
              nLng = sizeof(sGWSigTASE2);
              nSta = _q_write(pQ_DC0w, nLng,
                                (unsigned char *)pTASE2_BufW, 1);
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - analogue value to NDC  - %f %02x\n", gettimstr(),
    pTASE2_BufW->rVal, pTASE2_BufW->nValidity);
#endif
            }
            if (pTransTbl->bDC2) {
              pTASE2_BufW->nDst = 0x04;
              strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC2Nam);
              nLng = sizeof(sGWSigTASE2);
              nSta = _q_write(pQ_DC0w, nLng,
                                (unsigned char *)pTASE2_BufW, 1);
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - analogue value to CCDC - %f %02x\n", gettimstr(),
    pTASE2_BufW->rVal, pTASE2_BufW->nValidity);
#endif
            }
            break;

          default:
            nSta = -2;
            break;

        }

        if (nSta == 1) {
          // queueu full - overflow
        } else if (nSta == -2) {
          // function not supported
        } else {
          // ERROR;
        }

      }

    } else if (nSta == 1) {
      // no data in queue
      // printf("no data in IEC101 receive queue\n");
    } else {
      // ERROR;
    }



    // read from TASE.2 receive queue
    // ---------------------------------
    nLng = sizeof(sGWSigTASE2);   //added 2003.7.20 zy for check length in queue

    nSta = _q_read(pQ_DCr, &nLng, (unsigned char *)pTASE2_BufR, 0);
    if (nSta == 0) {

      bDblI = 0;
      nDblIBit = 1;
      nId = gwfindtbl(10, pTASE2_BufR->strDCNam, 0);
      if (nId != -1) {
        nSrc = DATASRC_TASE2_DC0;
        pTransTbl = *(pDC0NamBas + nId);
strcpy(strDC, "CDC");
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - signal '%s' from CDC\n", gettimstr(), pTASE2_BufR->strDCNam);
  if (pTASE2_BufR->nFct == 101)
    printf("                      command '%s' - Bits %02x - Chn %d\n",
      pTransTbl->strIMSNam, p101_BufW->nQuality, pTransTbl->nAppCChnNum);
  if (pTASE2_BufR->nFct == 103)
    fprintf(pFil, "                     setpoint '%s' - Value %f - Chn %d\n",
      pTransTbl->strIMSNam, p101_BufW->nQuality, pTransTbl->nAppCChnNum);
#endif
        if (pTransTbl->bDC0DblI) {
          bDblI = 1;
          pTransTblDblI = *(pAppCChnBas + pTransTbl->nDC0RelID);
          nDblIBit = pTransTblDblI->nDC0Bit;
        }
      } else {
        nId = gwfindtbl(11, pTASE2_BufR->strDCNam, 0);
        if (nId != -1) {
          nSrc = DATASRC_TASE2_DC1;
          pTransTbl = *(pDC1NamBas + nId);
strcpy(strDC, "NDC");
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - signal '%s' from NDC\n", gettimstr(), pTASE2_BufR->strDCNam);
  if (pTASE2_BufR->nFct == 101)
    printf("                      command '%s' - Bits %02x - Chn %d\n",
      pTransTbl->strIMSNam, p101_BufW->nQuality, pTransTbl->nAppCChnNum);
  if (pTASE2_BufR->nFct == 103)
    fprintf(pFil, "                     setpoint '%s' - Value %f - Chn %d\n",
      pTransTbl->strIMSNam, p101_BufW->nQuality, pTransTbl->nAppCChnNum);
#endif
          if (pTransTbl->bDC1DblI) {
            bDblI = 1;
            pTransTblDblI = *(pAppCChnBas + pTransTbl->nDC1RelID);
            nDblIBit = pTransTblDblI->nDC1Bit;
          }
        } else {
          nId =  gwfindtbl(12, pTASE2_BufR->strDCNam, 0);
          if (nId != -1) {
            nSrc = DATASRC_TASE2_DC2;
            pTransTbl = *(pDC2NamBas + nId);
strcpy(strDC, "CCDC");
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - signal '%s' from CCDC\n", gettimstr(), pTASE2_BufR->strDCNam);
  if (pTASE2_BufR->nFct == 101)
    printf("                      command '%s' - Bits %02x - Chn %d\n",
      pTransTbl->strIMSNam, p101_BufW->nQuality, pTransTbl->nAppCChnNum);
  if (pTASE2_BufR->nFct == 103)
    fprintf(pFil, "                     setpoint '%s' - Value %f - Chn %d\n",
      pTransTbl->strIMSNam, p101_BufW->nQuality, pTransTbl->nAppCChnNum);
#endif
            if (pTransTbl->bDC2DblI) {
              bDblI = 1;
              pTransTblDblI = *(pAppCChnBas + pTransTbl->nDC1RelID);
              nDblIBit = pTransTblDblI->nDC2Bit;
            }
          }
        }
      }

      if (nId != -1) {

        switch (pTASE2_BufR->nFct) {
          
          case 101:  // command
            nLng = sizeof(sGWSignalTypeX);
            pC = (byte *)p101_BufW;
            while (nLng--) *pC++ = 0x00;
            p101_BufW->nTyp = 46;
            p101_BufW->nSrc = nSrc;
            p101_BufW->nChnNum = pTransTbl->nAppCChnNum;
            p101_BufW->nValueType = 0;
            if (bDblI) {
//              p101_BufW->nChnNum = pTransTbl->nAppCChnNum;

//Modified by zy 2003.7.7 for cmd transfer.
              if((pTASE2_BufR->nValBits & 0x01) == 0)
                  p101_BufW->nChnNum ++ ;
//            p101_BufW->nQuality = pTASE2_BufR->nValBits;
              p101_BufW->nQuality = 0x02;
              p101_BufW->nCauseOfTrans = 6;
              pTransTbl->nVal = p101_BufW->nQuality;
/*
              if (bDblI && !nDblIBit) {
                p101_BufW->nQuality = ((pTASE2_BufR->nValBits & 0x01) ? 0x02 : 0x01);
              }
*/
              pTransTbl->nValFlg = 0;
printf("--GWMAIN - %s input - command '%s' / '%s' - Bits %02x - Chn %d\n",
  strDC, pTASE2_BufR->strDCNam, pTransTbl->strIMSNam,
  p101_BufW->nQuality, p101_BufW->nChnNum);
              nLng = sizeof(sGWSignalTypeX);
              nSta = _q_write(pQ_101w, nLng,
                                (unsigned char *)p101_BufW, 0);
              if (nSta == 1) {
                // queue full - overflow
printf("GWMAIN - IEC 101 out - OVERFLOW\n");
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - OVERFLOW IEC101 queue\n", gettimstr());
#endif
              nSta = 0;
              }
            } else {
//              p101_BufW->nChnNum = pTransTbl->nAppCChnNum;
//              p101_BufW->nQuality = pTASE2_BufR->nValBits;
//Modified by zy 2003.7.7 for cmd transfer.
              if((pTASE2_BufR->nValBits & 0x01) == 0)
                  p101_BufW->nChnNum ++ ;

              p101_BufW->nQuality = 0x02;
              p101_BufW->nCauseOfTrans = 6;
//              pTransTbl->nVal = pTASE2_BufR->nValBits;
              pTransTbl->nVal = p101_BufW->nQuality;
/*
              if (bDblI && !nDblIBit) {
                p101_BufW->nQuality = ((pTASE2_BufR->nValBits & 0x01) ? 0x01 : 0x02);
              }
*/
              pTransTbl->nValFlg = 0;
printf("---GWMAIN - %s input - command '%s' / '%s' - Bits %02x - Chn %d\n",
  strDC, pTASE2_BufR->strDCNam, pTransTbl->strIMSNam,
  p101_BufW->nQuality, p101_BufW->nChnNum);
              nLng = sizeof(sGWSignalTypeX);
              nSta = _q_write(pQ_101w, nLng,
                                (unsigned char *)p101_BufW, 0);
              if (nSta == 1) {
                // queue full - overflow
printf("GWMAIN - IEC 101 out - OVERFLOW\n");
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - OVERFLOW IEC101 queue\n", gettimstr());
#endif
              nSta = 0;
              }
            }
            break;

          case 103:  // setpoint - real
            pTransTbl->rVal = pTASE2_BufR->rVal;
            pTransTbl->nValFlg = 0;
            nLng = sizeof(sGWSignalTypeX);
            pC = (byte *)p101_BufW;
            while (nLng--) *pC++ = 0x00;
            p101_BufW->nTyp = 48;
            p101_BufW->nSrc = nSrc;
            p101_BufW->nChnNum = pTransTbl->nAppCChnNum;
            p101_BufW->nCauseOfTrans = 6;
            p101_BufW->nQuality = 0;
            p101_BufW->nValueType = 1;
            p101_BufW->uValue.fValue = pTASE2_BufR->rVal;
printf("===GWMAIN - %s input - setpoint '%s' / '%s' - Value %f - Chn %d\n",
  strDC, pTASE2_BufR->strDCNam, pTransTbl->strIMSNam,
  p101_BufW->uValue.fValue, pTransTbl->nAppCChnNum);
            nLng = sizeof(sGWSignalTypeX);
            nSta = _q_write(pQ_101w, nLng,
                              (unsigned char *)p101_BufW, 0);
            if (nSta == 1) {
            // queue full - overflow
printf("GWMAIN - IEC 101 out - OVERFLOW\n");
#ifdef PROASSET_TEST
  fprintf(pFil, "%s: GWMAIN - OVERFLOW IEC101 queue\n", gettimstr());
#endif
              nSta = 0;
            }
            break;

          default:
            nSta = -2;
            break;

        }
    
        if (nSta == 1) {
          // queue full - overflow
        } else if (nSta == -2) {
          // function not supported
        } else {
          // ERROR;
        }
      
      } else {
        // no dc signal assigned
      }

    } else if (nSta == 1) {
      // no data in queue
      // printf("no data in IEC101 receive queue\n");
    } else {
      // ERROR;
    }



/*
//#########################################################################
//## TEST: IMS dataset simulation
//#########################################################################
{
    long int nTim;
    long int nIdx;

    time(&nTim);
    if ((nTim - nTimSav) >= 30) {
      nTimSav = nTim;
      nIdx = gwfindtbl(0, (char *)NULL, 15481);
      if (nIdx != -1) {
        pTransTbl = *(pIMSIdBas + nIdx);
        if (nSwtch) pTransTbl->bVal = 0x01; else pTransTbl->bVal = 0x00;
        pTransTbl->nValFlg = 0;
        pTransTbl->nValProcess = 1;
printf("GWIMS - new indication - '%s' - %02x %02x\n",
  pTransTbl->strIMSNam, pTransTbl->bVal, pTransTbl->nValFlg);
      }
      nIdx = gwfindtbl(0, (char *)NULL, 2000);
      if (nIdx != -1) {
        pTransTbl = *(pIMSIdBas + nIdx);
        if (nSwtch) pTransTbl->rVal = 55.5555; else pTransTbl->rVal = 777.7777;
        pTransTbl->nValFlg = 0;
        pTransTbl->nValProcess = 1;
printf("GWIMS - new measurement - '%s' - %f %02x\n",
  pTransTbl->strIMSNam, pTransTbl->rVal, pTransTbl->nValFlg);
      }
      nIdx = gwfindtbl(0, (char *)NULL, 21331);
      if (nIdx != -1) {
        pTransTbl = *(pIMSIdBas + nIdx);
        if (nSwtch) pTransTbl->bVal = 0x01; else pTransTbl->bVal = 0x00;
        pTransTbl->nValFlg = 0;
        pTransTbl->nValProcess = 1;
printf("GWIMS - new indication - '%s' - %02x %02x\n",
  pTransTbl->strIMSNam, pTransTbl->bVal, pTransTbl->nValFlg);
      }
      if (nSwtch) nSwtch = 0; else nSwtch = 1;
    }
}
//#########################################################################
*/



   // process data from IMS
    // ---------------------
    for (nCnt = 0; nCnt < nMaxIMSNamNum; ++nCnt) {

      pTransTbl = *(pIMSNamBas + nCnt);;

      if (pTransTbl->nValProcess) {

        nLng = sizeof(sGWSignalTypeX);
        pC = (byte *)p101_BufW;
        while (nLng--) *pC++ = 0x00;

        nLng = sizeof(sGWSigTASE2);
        pC = (byte *)pTASE2_BufW;
        while (nLng--) *pC++ = 0x00;

        if (pTransTbl->nType & TYPE_INDICATION) {

          // service IEC101
          p101_BufW->nTyp = 1;
          p101_BufW->nSrc = DATASRC_IMS;
          p101_BufW->nChnNum = pTransTbl->nAppCChnNum;
          p101_BufW->nCauseOfTrans = 3;
          p101_BufW->nQuality = pTransTbl->bVal;
          p101_BufW->nQuality += (pTransTbl->nValFlg << 7);
          p101_BufW->nValueType = 0;
          nLng = sizeof(sGWSignalTypeX);
          nSta = _q_write(pQ_101w, nLng,
                            (unsigned char *)p101_BufW, 1);
printf("GWMAIN - IMS in '%s' / IEC101 out %d - state %02x\n",
  pTransTbl->strIMSNam,  pTransTbl->nAppCChnNum,
  p101_BufW->nQuality);
          if (nSta == 1) {
            // queue full - overflow
printf("GWMAIN - IEC 101 out - OVERFLOW\n");
          }

          // service TASE.2
          pTASE2_BufW->nFct = ModifyLocalDataValue;
          pTASE2_BufW->nType = TYPE_INDICATION;
          pTASE2_BufW->nValBits = ((pTransTbl->bVal) ? 0x02 : 0x01);
          pTASE2_BufW->nValidity = pTransTbl->nValFlg;
          if (pTransTbl->bDC0) {
/*
            if (pTransTbl->bDCODblI && !pTransTbl->nDC0Bit) {
              pTASE2_BufW->bVal = ((pTransTbl->bVal) ? 0x01 : 0x02);
            }
*/
            pTASE2_BufW->nDst = 0x01;
            strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC0Nam);
            nLng = sizeof(sGWSigTASE2);
            nSta = _q_write(pQ_DC0w, nLng,
                              (unsigned char *)pTASE2_BufW, 1);
printf("GWMAIN - IMS in '%s' / CDC TASE.2 out '%s' - state %02x\n",
  pTransTbl->strIMSNam,  pTransTbl->strDC0Nam,
  (pTASE2_BufW->nValBits << 6) + (pTASE2_BufW->nValidity << 4));
          }
          if (pTransTbl->bDC1) {
/*
            if (pTransTbl->bDC1DblI && !pTransTbl->nDC1Bit) {
              pTASE2_BufW->bVal = ((pTransTbl-bVal) ? 0x01 : 0x02);
            }
*/
            pTASE2_BufW->nDst = 0x02;
            strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC1Nam);
            nLng = sizeof(sGWSigTASE2);
            nSta = _q_write(pQ_DC0w, nLng,
                              (unsigned char *)pTASE2_BufW, 1);
printf("GWMAIN - IMS in '%s' / CDC TASE.2 out '%s' - state %02x\n",
  pTransTbl->strIMSNam,  pTransTbl->strDC1Nam,
  (pTASE2_BufW->nValBits << 6) + (pTASE2_BufW->nValidity << 4));
          }
          if (pTransTbl->bDC2) {
/*
            if (pTransTbl->bDC2DblI && !pTransTbl->nDC2Bit) {
              pTASE2_BufW->bVal = ((pTransTbl->bVal) ? 0x01 : 0x02);
            }
*/
            pTASE2_BufW->nDst = 0x04;
            strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC2Nam);
            nLng = sizeof(sGWSigTASE2);
            nSta = _q_write(pQ_DC0w, nLng,
                                (unsigned char *)pTASE2_BufW, 1);
printf("GWMAIN - IMS in '%s' / CDC TASE.2 out '%s' - state %02x\n",
  pTransTbl->strIMSNam,  pTransTbl->strDC2Nam,
  (pTASE2_BufW->nValBits << 6) + (pTASE2_BufW->nValidity << 4));
          }

        }

        if (pTransTbl->nType & TYPE_MEASUREMENT) {

          // service IEC101
          p101_BufW->nTyp = 9;
          p101_BufW->nSrc = DATASRC_IMS;
          p101_BufW->nChnNum = pTransTbl->nAppCChnNum;
          p101_BufW->nCauseOfTrans = 3;
          p101_BufW->nQuality = (pTransTbl->nValFlg << 7);
          p101_BufW->nValueType = 1;
          p101_BufW->uValue.fValue = pTransTbl->rVal;
          nLng = sizeof(sGWSignalTypeX);
          nSta = _q_write(pQ_101w, nLng,
                            (unsigned char *)p101_BufW, 0);
printf("GWMAIN - IMS in '%s' / IEC101 out %d - real %f - %02xd\n",
  pTransTbl->strIMSNam,  pTransTbl->nAppCChnNum,
  p101_BufW->uValue.fValue, p101_BufW->nQuality);
          if (nSta == 1) {
            // queue full - overflow
printf("GWMAIN - IEC 101 out - OVERFLOW\n");
          }
  
          // service TASE.2
          pTASE2_BufW->nFct = ModifyLocalDataValue;
          pTASE2_BufW->nType = TYPE_MEASUREMENT;
          pTASE2_BufW->rVal = pTransTbl->rVal;
          pTASE2_BufW->nValidity = pTransTbl->nValFlg;
          if (pTransTbl->bDC0) {
            pTASE2_BufW->nDst = 0x01;
            strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC0Nam);
            nLng = sizeof(sGWSigTASE2);
            nSta = _q_write(pQ_DC0w, nLng,
                              (unsigned char *)pTASE2_BufW, 1);
printf("GWMAIN - IMS in '%s' / CDC TASE.2 out '%s' - real %f - %02xd\n",
  pTransTbl->strIMSNam,  pTransTbl->strDC0Nam,
  p101_BufW->uValue.fValue, p101_BufW->nQuality);
          }
          if (pTransTbl->bDC1) {
            pTASE2_BufW->nDst = 0x02;
            strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC1Nam);
            nLng = sizeof(sGWSigTASE2);
            nSta = _q_write(pQ_DC0w, nLng,
                              (unsigned char *)pTASE2_BufW, 1);
printf("GWMAIN - IMS in '%s' / NDC TASE.2 out '%s' - real %f - %02xd\n",
  pTransTbl->strIMSNam,  pTransTbl->strDC1Nam,
  p101_BufW->uValue.fValue, p101_BufW->nQuality);
          }
          if (pTransTbl->bDC2) {
            pTASE2_BufW->nDst = 0x04;
            strcpy(pTASE2_BufW->strDCNam, pTransTbl->strDC2Nam);
            nLng = sizeof(sGWSigTASE2);
            nSta = _q_write(pQ_DC0w, nLng,
                                (unsigned char *)pTASE2_BufW, 1);
printf("GWMAIN - IMS in '%s' / CCDC TASE.2 out '%s' - value %f - %02xd\n",
  pTransTbl->strIMSNam,  pTransTbl->strDC2Nam,
  p101_BufW->uValue.fValue, p101_BufW->nQuality);
          }

        }

        pTransTbl->nValProcess = 0;

      }

    }



    sleep(5000);  // 5 milliseconds rest



  }



  // stop gateway programs

  // restart gateway main process


  pPrcCntrl = pPrcCntrlBas + PRC_NUM_GWAPPWS;
  pPrcCntrl->nWork = 0;
  pPrcCntrl = pPrcCntrlBas + PRC_NUM_GWDC0;
  pPrcCntrl->nWork = 0;

  pPrcCntrl = pPrcCntrlBas + PRC_NUM_GWIMS;
  pPrcCntrl->nWork = 0;
  pPrcCntrl = pPrcCntrlBas + PRC_NUM_GWPRT;
  pPrcCntrl->nWork = 0;
  sleep(10000);
  prt_write(pQ_Prt, -1, 8, PRC_NUM_GWMAIN, 0, "", NULL, NULL, NULL);


  shm_unlink(pM_Data);
  _q_unlink(pQ_101w);
  _q_unlink(pQ_101r);
  _q_unlink(pQ_DCr);
  _q_unlink(pQ_DC0w);
  _q_unlink(pQ_IMS);
  shm_unlink(pM_Data);
  sem_del(pS_Data);
/*
  prt_write(pQ_Prt, MSG_TYP_INFO, 0, PRC_NUM_GWCNTRL, 0,
              "end of process", NULL, NULL, NULL);
*/
  _q_unlink(pQ_Prt);
  shm_unlink(pM_Prc);

  return 0;

}





#ifdef PROASSET_TEST
static char *gettimstr( void )
{
  static char          timstr[128];
         struct timeb  timbuf;
         struct tm     timdat;      
         struct tm    *tim;      
  ftime(&timbuf);
  tim = &timdat;
  tim = localtime((const long *)&timbuf.time);
  sprintf(timstr, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday,
            tim->tm_hour, tim->tm_min, tim->tm_sec, timbuf.millitm);
  return(timstr); 
}
#endif
