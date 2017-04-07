#ifndef PLCSTD
#define PLCSTD

/*#define HPUX
#define ALPHA_UNIX  
#define MSWIN32*/

#define LINUX

#ifdef ALPHA_UNIX
#include        <sys/types.h>
#include        <sys/socket.h>
#include        <sys/errno.h>
#include        <netinet/in.h>
#include        <netdb.h>
#include        <string.h>
#include        <stdio.h>
#include        <signal.h>
#include        <stdlib.h>
#include        <fcntl.h>
#include        <xti.h>
#include        <unistd.h>
#include        <time.h>



#define RECV_FLAGS      0
#define SOCKET          short
#define SOCKET_ERROR    (-1)
#define INVALID_SOCKET  (-1)
#define closesocket(x)  shutdown(x,0)
#define fflush(x)       getchar()

#endif

#ifdef LINUX

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>

#define RECV_FLAGS      (0)
#define SOCKET_ERROR    (-1)
#define SOCKET          int
#define INVALID_SOCKET  (-1)
#define closesocket(x)  close(x)
/*#define _toupper(x)     toupper(x)     old*/

#endif


#ifdef HPUX

/* known to work under HPUX */
/* compile with cc -Aa for ANSI compatible */

#define _HPUX_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>

#define RECV_FLAGS      (0)
#define SOCKET_ERROR    (-1)
#define SOCKET          int
#define INVALID_SOCKET  (-1)
#define closesocket(x)  close(x)
#define BIG_ENDIAN      (1)

#endif


#ifdef MSWIN32

/* known to work under Windows 95 and Windows NT, MSVC 5.0 */

/*
 * note when using MSVC/C++ 5.0 - create workspace as a win32 console app and
 *  change project/settings/link Object/library modules list to include wsock32.lib
 */

#include <windows.h>
#include <winbase.h>
#include <string.h>
#include <stdlib.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#pragma pack(1)

#define RECV_FLAGS  0

#endif


#define		READ_STATUS		0
#define		WRITE_STATUS	1
#define		EXCPT_STATUS	2



#endif

