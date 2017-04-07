#ifndef DEA_GLOBAL_H
#define DEA_GLOBAL_H

#include "../../libglobal/include/global.h"

#define LIBDEA_VERSION			0x010000
#define LIBDEA_VERSION_STR		"1.0.0"
#define LIBDEA_LOGGER_STR		"root.libdea"

#if defined(Q_WS_WIN) || defined(Q_WS_S60)

#if defined(_MSC_VER) /* MSVC Compiler */
/* template-class specialization 'identifier' is already instantiated */
#pragma warning(disable: 4660)
#endif // _MSC_VER

#ifdef DEA_DLL

#if defined(DEA_MAKEDLL)     // create a extend DLL library 
#define DEA_EXPORT  __declspec(dllexport)
#define DEA_TEMPLATEDLL
#else                        // use a extend DLL library
#define DEA_EXPORT  __declspec(dllimport)
#endif

#endif // DEA_DLL

#endif // Q_WS_WIN || Q_WS_S60


#ifndef DEA_EXPORT
#define DEA_EXPORT
#endif

#endif // DEA_GLOBAL_H
