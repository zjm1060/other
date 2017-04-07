
#ifndef QUEUE_H
#define QUEUE_H  1





typedef struct QueueManagement {
  int            q_magic;         /*                                   */
  int            q_dat;           /* Offset Queue-Daten                */
  int            q_dat_end;       /* Offset Queue-Ende                 */
  int            q_dat_siz;       /* Groesse Queue (Datenbereich)      */
  int            q_w;             /* Offset WRITE                      */
  int            q_r;             /* Offset READ                       */
  int            q_tel_num;       /* Anzahl Telegramme in Queue        */
  int            q_byt_num;       /* Anzahl Bytes in Queue             */
  int            q_lnks;          /* Anzahl der Links an Queue         */
} sQueueMngm;

typedef struct Queue {
  long int       q_id;
  long int       q_key;
  char           q_nam[16];       /* Name der Queue                    */
  int            q_siz;           /* Groesse der Queue (mit Management)*/
  unsigned char *q_adr;           /* Pointer Queue                     */
  sQueueMngm    *q_mngm;          /* Pointer Queue-Management-Struktur */
  unsigned char *q_dat;           /* Pointer Queue-Daten               */
  unsigned char *q_dat_end;       /* Pointer Queue-Ende                */
  int            q_dat_siz;       /* Groesse Queue                     */
  sSemaphore     q_sem_main_dat;  /* Semaphore der Queue               */
  sSemaphore    *q_sem_main;      /* Pointer Semaphore der Queue       */
  sSemaphore     q_sem_op_dat;    /* Semaphore der Queue               */
  sSemaphore    *q_sem_op;        /* Pointer Semaphore der Queue       */
} sQueue;



#define Q_MAGIC  0x4afc
#define Q_BLOCK  0x4cfa



#define ERR_Q_CLINK   0x0100
#define ERR_Q_DEL     0x0200
#define ERR_Q_INFO    0x0300
#define ERR_Q_JAM     0x0400
#define ERR_Q_READ    0x0500
#define ERR_Q_RESET   0x0600
#define ERR_Q_UNLINK  0x0700
#define ERR_Q_WRITE   0x0800



/*-----------------------------------------------------------------------*/
/* function prototypes:                                                  */
/*-----------------------------------------------------------------------*/
#ifdef QUEUEFCT_C

int _q_clink( sQueue *q );
int _q_clink1( sQueue *q , int nflag);
int _q_del( sQueue *q );
int _q_jam( sQueue *q, int tel_lng, unsigned char *tel_dat, int wait );
int _q_read( sQueue *q, int *tel_lng, unsigned char *tel_dat, int wait );
int _q_unlink( sQueue *q );
int _q_write( sQueue *q, int tel_lng, unsigned char *tel_dat, int wait );

#else

#ifdef __cplusplus
//extern "C" {
#endif

extern int _q_clink( sQueue *q );
extern int _q_clink1( sQueue *q , int nflag);
extern int _q_del( sQueue *q );
extern int _q_jam( sQueue *q, int tel_lng, unsigned char *tel_dat, int wait );
extern int _q_read( sQueue *q, int *tel_lng, unsigned char *tel_dat, int wait );
extern int _q_unlink( sQueue *q );
extern int _q_write( sQueue *q, int tel_lng, unsigned char *tel_dat, int wait );

#ifdef __cplusplus
//}
#endif

extern int nErrQueue;

#endif






#endif









