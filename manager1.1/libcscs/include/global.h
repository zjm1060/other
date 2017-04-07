#ifndef CSCS_GLOBAL_H
#define CSCS_GLOBAL_H

#include "../../libglobal/include/global.h"

#define LIBCSCS_VERSION			0x010000
#define LIBCSCS_VERSION_STR		"1.0.0"
#define LIBCSCS_LOGGER_STR		"root.libcscs"

#if defined(Q_WS_WIN) || defined(Q_WS_S60)

#if defined(_MSC_VER) /* MSVC Compiler */
/* template-class specialization 'identifier' is already instantiated */
#pragma warning(disable: 4660)
#endif // _MSC_VER

#ifdef CSCS_DLL

#if defined(CSCS_MAKEDLL)     // create a extend DLL library 
#define CSCS_EXPORT  __declspec(dllexport)
#define CSCS_TEMPLATEDLL
#else                        // use a extend DLL library
#define CSCS_EXPORT  __declspec(dllimport)
#endif

#endif // CSCS_DLL

#endif // Q_WS_WIN || Q_WS_S60


#ifndef CSCS_EXPORT
#define CSCS_EXPORT
#endif

#endif // CSCS_GLOBAL_H
