#ifndef  _CALLH9000DLLSPEC_H
#define  _CALLH9000DLLSPEC_H
#if defined(WIN32)

//#pragma comment(lib,"advapi32")
//½â¾öunresolved external symbol regclosekey
#ifdef _CALLH9000DLL
#define _CALLH9000DLL_EXPORT _declspec(dllexport)
#else
#define _CALLH9000DLL_EXPORT _declspec(dllimport)
#endif
#elif defined(__unix)
#define _CALLH9000DLL_EXPORT
#endif
#endif


