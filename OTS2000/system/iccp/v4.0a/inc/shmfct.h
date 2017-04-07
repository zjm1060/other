
#ifndef SHMFCT_H
#define SHMFCT_H



typedef struct shared_memory {
  long int           id;
  long int           key;
  unsigned char     *adr;
  unsigned long int  siz;
  char               nam[16];
#ifdef WINDOWS32
  HANDLE             pHndl;
#endif
} sSharedMemory;



#define ERR_SHM_CLINK   0x0100
#define ERR_SHM_DEL     0x0200
#define ERR_SHM_UNLINK  0x0300



#ifdef SHMFCT_C

int shm_clink( sSharedMemory *shm );
int shm_del( sSharedMemory *shm );
int shm_unlink_aix( sSharedMemory *shm );

#else

#ifdef __cplusplus
//extern "C" {
#endif

extern int shm_clink( sSharedMemory *shm );
extern int shm_del( sSharedMemory *shm );
extern int shm_unlink_aix( sSharedMemory *shm );

#ifdef __cplusplus
//}
#endif

extern int nErrSemaphore;

#endif



#endif


