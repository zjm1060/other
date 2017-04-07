/***************************************************************************************
FILENAME:	lan_os.c

FUNCTION:	
	
	this file includes all codes in lan module which differ in different operation: 
	SUN and WINDOWS 

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	create the file for V4.0		yyp

****************************************************************************************/
#ifndef	_LAN_OS
#define	_LAN_OS

#ifdef WINDOWS_OS
 #define MSWIN32
#else
 #define SUN
#endif 
	
#ifdef SUN
 #define INADDR_NONE	0xffffffff
 #define SOCKET          int
 #define INVALID_SOCKET  (-1)
 #define closesocket(x)  close(x)
#ifndef  SOCKET_ERROR
 #define SOCKET_ERROR    (-1)
#endif
 typedef struct sockaddr_in SOCKADDR_IN;
#endif

#ifndef SO_REUSEPORT
 #define SO_REUSEPORT 15 
#endif

#ifdef MSWIN32
	#define	OS_LANHEAD_LEN	2560
	#define socklen_t	int
#endif
#ifdef SUN
  	#define	OS_LANHEAD_LEN	10
#endif

#endif
