
#ifndef GWEXTERNDEK_H
#define GWEXTERNDEK_H


#ifndef GWMAIN_C

extern int               nGWRun;

extern sGWProcessCntrl  *pPrcCntrlBas;

extern sGWTransTbl      *pTransTblBas;
extern sGWTransTbl      *pTransTblFst;
extern sGWTransTbl     **pNamBas;
extern long int          nMaxSigNum;

extern sGWTransTbl     **pIMSIdBas;
extern long int          nMaxIMSIdNum;

extern sGWTransTbl     **pIMSNamBas;
extern long int          nMaxIMSNamNum;

extern sGWTransTbl     **pDC0NamBas;
extern long int          nMaxDC0NamNum;

extern sGWTransTbl     **pDC1NamBas;
extern long int          nMaxDC1NamNum;

extern sGWTransTbl     **pDC2NamBas;
extern long int          nMaxDC2NamNum;

extern sGWTransTbl     **pAppCChnBas;
extern long int          nMaxAppCChnNum;

extern sGWSignalValue   *pSigValBas;
extern sGWSignalValue   *pSigValFstFre;
extern sGWSignalValue   *pSigValLstFre;
extern long int          nMaxSigValNum;

extern sSharedMemory    *pM_Data;
extern sSharedMemory    *pM_Prc;
extern sSemaphore       *pS_Data;
extern sSharedMemory     sMDat_Data;
extern sSharedMemory     sMDat_Prc;
extern sSemaphore        sSDat_Data;

#endif

#endif
