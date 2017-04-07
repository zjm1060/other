
#ifndef     _OS_TYPE_DEF_H
#define     _OS_TYPE_DEF_H


 
#ifdef		_WIN32
#define		WINDOWS_OS
#else
#define     SUN_OS
#endif




#ifdef		SUN_OS

#ifndef		_REENTRANT
#define		_REENTRANT
#endif

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h> 
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h> 
#include	<sys/mman.h>
#include	<sys/time.h>
#include 	<sys/signal.h>
#include 	<unistd.h>
#include 	<time.h>
#include 	<utime.h>
#include 	<strings.h>

#ifdef		_BIG_ENDIAN
#define		_BIG_ENDIAN_
#endif

#endif /*SUN_OS*/


#ifdef		WINDOWS_OS
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/utime.h>
#include	<fcntl.h> 
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h> 
#include	<windows.h>
#include	<string.h>
#include	<time.h>
#include	<tchar.h>
#include	<io.h>
#include	<direct.h>
#endif /*WINDOWS_OS*/



#ifdef WINDOWS_OS
	#define	TASK_SUFFIX_NAME	".exe"
#elif defined(_AIX)
	#define	TASK_SUFFIX_NAME	".axe"
#elif defined(SUN_OS) 
#ifdef _BIG_ENDIAN_  //SPARC
	#define	TASK_SUFFIX_NAME	".dxe"
#else // X86
	#define	TASK_SUFFIX_NAME	".xxe"
#endif
#else
	#define	TASK_SUFFIX_NAME	".dxe"
#endif



#endif  /*_OS_TYPE_DEF_H*/

