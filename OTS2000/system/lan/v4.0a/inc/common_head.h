#ifdef MSWIN32
#include <signal.h>
#include <winsock.h>
#include <windows.h>       
#include <winbase.h>
#include <stdlib.h>
#include <stdio.h>  
#include <time.h>
#include <string.h>        
#include <memory.h>
#include <process.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#endif

#ifdef SUN
#include 	<sys/types.h>
#include 	<stropts.h>
#include 	<sys/socket.h>
#include 	<sys/errno.h>
#include 	<netinet/in.h>
#include 	<netdb.h>
#include 	<math.h>
#include 	<string.h>                      
#include 	<stdio.h>
#include 	<signal.h>
#include	<stdlib.h>
#include 	<fcntl.h>
#include 	<xti.h>
#include	<unistd.h>
#include 	<sys/shm.h>
#include 	<sys/sem.h>
#include	<time.h>
#include	<errno.h>	
#include	<arpa/inet.h>
#include 	<pthread.h>
#include 	<thread.h>
#endif

#if defined(SUN) && defined(_AIX)
#include	<sys/wait.h>
#elif defined(SUN)
#include	<wait.h>
#endif

