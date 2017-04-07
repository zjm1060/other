#ifndef     _OS_TYPE_DEF_H
#define     _OS_TYPE_DEF_H


 
#ifdef		_WIN32
#define		WINDOWS_OS
#else
#define     SUN_OS
#endif


#ifdef		SUN_OS

#ifndef	_REENTRANT
#define _REENTRANT
#endif

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/utime.h>
#include	<fcntl.h> 
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h> 
#include	<sys/mman.h>
#include	<sys/time.h>
#include 	<sys/signal.h>
#include 	<unistd.h>
#include 	<utime.h>
#include 	<strings.h>
#include	<thread.h>
#endif


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
#endif





#endif  /*_OS_TYPE_DEF_H*/

