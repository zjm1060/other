
#ifndef GWEXTERNFCT_H
#define GWEXTERNFCT_H



//---- functions - file gwinit.cpp ----
#ifndef GWINITTBL_C
extern int gwinittbl(void);
#endif

//---- functions - file gwvallist.cpp ----
#ifndef GWVALLIST_C
extern int             gwdelsigval(sGWSignalValue *pSigVal);
extern void            gwfreesigval(sGWSignalValue *pSigVal);
extern sGWSignalValue *gwinssigval(sGWSignalValue *pSigValPre);
extern sGWSignalValue *gwnewsigval();
#endif

//---- functions - file gwfindtbl.cpp ----
#ifndef GWFINDTBL_C
extern long int gwfindtbl(int nOpt, char *pNam, long int pNum);
#endif

//---- functions - file gwprtfct.cpp ----
#ifndef GWPRCTFCT_C
extern int prt_init( void );
extern int prt_read( sQueue *q );
extern int prt_write( sQueue *q, int nTyp, int nLvl, int nPrc, int nNum, char *strMsg, void *p0, void *p1, void *p2 );
#endif

#endif
