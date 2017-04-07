
#define GWDC_C

#include "cppinc.h"

#include "gwdatastructure.h"


int prt_write( sQueue *q, int nTyp, int nLvl, int nPrc, int nNum, char *strMsg, void *p0, void *p1, void *p2 );

void gwdcloop( void );
void catch_sig(int sig_num);


sGWProcessCntrl  *pPrcCntrlBas;
sGWProcessCntrl  *pPrcCntrl;

sQueue           *pQ_DCr;
sQueue           *pQ_DCw;
sQueue           *pQ_Prt;
sQueue            sQDat_DCr;
sQueue            sQDat_DCw;
sQueue            sQDat_Prt;

sSharedMemory    *pM_Data;
sSemaphore       *pS_Data;
sSharedMemory     sMDat_Data;
sSemaphore        sSDat_Data;

sGWSigTASE2       sSigTASE2_R;
sGWSigTASE2       sSigTASE2_W;

sGWProtocolMsg   *pPrtMsg;
sGWProtocolMsg    sPrtMsgDat;

char              strPrtBuf[256];




// signal handler
void catch_sig(int sig_num)
{
  // reset the signal handler for catching signal for the next time
  signal(SIGINT, catch_sig);
  signal(SIGKILL, catch_sig);
  pPrcCntrl->nWork = 0;
}





int main( int argc, char* argv[] )

{

  int              nPrcNum;
  sSharedMemory   *pM_Prc;
  sSharedMemory    sMDat_Prc;



  pPrtMsg = &sPrtMsgDat;



  // link to base data structures
  pM_Prc = &sMDat_Prc;
  pM_Prc->id = 0;
  pM_Prc->key = SHM_PRCCNTRL_KEY;
  pM_Prc->adr = (unsigned char *)NULL;
  pM_Prc->siz = 64 * sizeof(sGWProcessCntrl);
  strcpy(pM_Prc->nam, SHM_PRCCNTRL_NAM);
  if (shm_clink(pM_Prc) == -1) {
    // Error
    return(-1);
  }
  pPrcCntrl = (sGWProcessCntrl *)(pM_Prc->adr + PRC_NUM_GWDC0 * sizeof(sGWProcessCntrl));
  pPrcCntrl->nPrc = PRC_NUM_GWDC0;
  strcpy(pPrcCntrl->strNam, "gwdc");
  pPrcCntrl->nWork = 1;



  // install signal handler
  signal(SIGINT, catch_sig);
  signal(SIGKILL, catch_sig);


  
  //-- link to queues --

  //-- protocol interface (write) --
  pQ_Prt = &sQDat_Prt;
  pQ_Prt->q_key = Q_PRT_KEY;
  strcpy(pQ_Prt->q_nam, Q_PRT_NAM);
  pQ_Prt->q_siz = Q_PRT_SIZ;
  if (_q_clink(pQ_Prt) == -1) {
    shm_unlink(pM_Prc);
    return(-1);
  }
  prt_write(pQ_Prt, MSG_TYP_INFO, 0, PRC_NUM_GWDC0, 0,
              "link to protocol queue - ok", NULL, NULL, NULL);

  //-- input queue from gwmain
  pQ_DCr = &sQDat_DCr;
  pQ_DCr->q_key = Q_DC0O_KEY;
  strcpy(pQ_DCr->q_nam, Q_DC0O_NAM);
  pQ_DCr->q_siz = Q_DC0O_SIZ;
  if (_q_clink(pQ_DCr) == -1) {
    // ERROR
    _q_unlink(pQ_Prt);
    shm_unlink(pM_Prc);
    return(-1);
  }

  //-- output queue to gwmain
  pQ_DCw = &sQDat_DCw;
  pQ_DCw->q_key = Q_DCI_KEY;
  strcpy(pQ_DCw->q_nam, Q_DCI_NAM);
  pQ_DCw->q_siz = Q_DCI_SIZ;
  if (_q_clink(pQ_DCw) == -1) {
    // ERROR
    _q_unlink(pQ_DCr);
    _q_unlink(pQ_Prt);
    shm_unlink(pM_Prc);
    return(-1);
  }



  //-- link to shared memory sections and semaphores --
  pM_Data = &sMDat_Data;
  pM_Data->id = 0;
  pM_Data->key = SHM_DATA_KEY;
  pM_Data->adr = (unsigned char *)NULL;
  pM_Data->siz = MAX_SIGNAL_NUM * sizeof(sGWTransTbl);        // size data translation table
  pM_Data->siz += MAX_SIGNAL_NUM * sizeof(sGWTransTbl **);    // size of IMS name pointers
  pM_Data->siz += MAX_SIGNAL_NUM * sizeof(sGWTransTbl **);    // size of IMS id pointers
  pM_Data->siz += MAX_SIGNAL_NUM * sizeof(sGWTransTbl **);    // size of DC0 name pointers
  pM_Data->siz += MAX_SIGNAL_NUM * sizeof(sGWTransTbl **);    // size of DC1 name pointers
  pM_Data->siz += MAX_SIGNAL_NUM * sizeof(sGWTransTbl **);    // size of DC2 name pointers
  pM_Data->siz += MAX_SIGNAL_NUM * sizeof(sGWTransTbl **);    // size of IEC101 pointers
  strcpy(pM_Data->nam, SHM_DATA_NAM);
  if (shm_clink(pM_Data) == -1) {
    // Error - no memory
    _q_unlink(pQ_DCw);
    _q_unlink(pQ_DCr);
    _q_unlink(pQ_Prt);
    shm_unlink(pM_Prc);
    return(-1);
  }
  pS_Data = &sSDat_Data;
  pS_Data->id = 0L;
  pS_Data->key = SEM_DATA_KEY;
  strcpy(pS_Data->nam, SEM_DATA_NAM);
  pS_Data->init_val = 1;
  pS_Data->max_val = 1;
  if ((pS_Data->id = sem_clink(pS_Data)) == -1) {
    // Error - no semaphore
    shm_unlink(pM_Data);
    _q_unlink(pQ_DCw);
    _q_unlink(pQ_DCr);
    _q_unlink(pQ_Prt);
    shm_unlink(pM_Prc);
    return(-1);
  }
  prt_write(pQ_Prt, MSG_TYP_INFO, 8, nPrcNum, 0,
              "link to data shared memory and semaphore - ok", NULL, NULL, NULL);



  while (pPrcCntrl->nWork) {

    gwdcloop();

  }



  sem_del(pS_Data);
  shm_unlink(pM_Data);
  _q_unlink(pQ_DCw);
  _q_unlink(pQ_DCr);
  _q_unlink(pQ_Prt);
  shm_unlink(pM_Prc);

  return(0);

}
