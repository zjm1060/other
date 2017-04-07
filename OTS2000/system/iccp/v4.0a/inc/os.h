
#define HPUX
#undef WINDOWS32

#ifdef WINDOWS32
#define sleep  Sleep
#endif

#ifdef HPUX
#define NULL  0
#endif

