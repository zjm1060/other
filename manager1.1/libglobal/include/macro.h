#ifndef MCRO_H
#define MCRO_H

#ifdef WIN32
#if _MSC_VER <1400        //vc2003
#define _DO_NOT_HAVE_S
#else
#endif                    //VC2003
#else                     //LINUX
#define _DO_NOT_HAVE_S
#endif                    //WIN32

#ifdef _DO_NOT_HAVE_S

#include <stdio.h>
// #include <stdarg.h>
// #include <stdlib.h>
// inline int sprintf_s(
// 			  char *buffer,
// 			  size_t sizeOfBuffer,
// 			  const char *format ,...)
// {
// 	va_list ap;
// 	va_start(ap, format);
// 	int res = vsprintf(buffer, format, ap);
// 	va_end(ap);
// 	return res;
// }

#define memcpy_s(dest,num,src,count) memcpy(dest,src,count)

#define fprintf_s fprintf

#define _strdate_s(buf,num) _strdate(buf)

#define strcat_s(dest,num,src) strcat(dest,src)

#define fopen_s(pf,name,mode) *pf=fopen(name,mode)

#define strncpy_s(dest,num,src,count) strncpy(dest,src,count)

#define localtime_s(tm,time) *tm=*localtime(time)

#define _strdup strdup

#define _countof(array) (sizeof(array)/sizeof(array[0]))

#define strcpy_s(dest,num,src) strcpy(dest,src)

#define sscanf_s sscanf

// #else
// #define mkdir(pathname,mode) _mkdir(pathname)
#endif

#endif
