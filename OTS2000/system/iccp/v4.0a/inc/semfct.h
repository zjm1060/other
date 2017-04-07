
#ifndef SEMFCT_H
#define SEMFCT_H



typedef struct Semaphore {
  long int  id;
  long int  key;
  char      nam[16];
  long int  init_val;
  long int  max_val;
#ifdef WINDOWS32
  LPSECURITY_ATTRIBUTES  lpSemaphoreAttributes;
  HANDLE                 pHndl;
#endif
} sSemaphore;




#define ERR_SEM_CLINK   0x0100
#define ERR_SEM_DEL     0x0200
#define ERR_SEM_SIGNAL  0x0300
#define ERR_SEM_WAIT    0x0400



#ifdef SEMFCT_C

int sem_clink( sSemaphore *sem );
int sem_del( sSemaphore *sem );
int sem_signal( sSemaphore *sem );
int sem_wait( sSemaphore *sem, int no_wait );

#else

#ifdef __cplusplus
//extern "C" {
#endif

extern int sem_clink( sSemaphore *sem );
extern int sem_del( sSemaphore *sem );
extern int sem_signal( sSemaphore *sem );
extern int sem_wait( sSemaphore *sem, int no_wait );

#ifdef __cplusplus
//}
#endif

extern int nErrSemaphore;

#endif



#endif


