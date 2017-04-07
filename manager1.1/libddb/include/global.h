#ifndef DDB_GLOBAL_H
#define DDB_GLOBAL_H

#include "../../libglobal/include/global.h"

#define LIBDDB_VERSION			0x010000
#define LIBDDB_VERSION_STR		"1.0.0"
#define LIBDDB_LOGGER_STR		"root.libddb"

#if defined(Q_WS_WIN) || defined(Q_WS_S60)

#if defined(_MSC_VER) /* MSVC Compiler */
/* template-class specialization 'identifier' is already instantiated */
#pragma warning(disable: 4660)
#endif // _MSC_VER

#ifdef DDB_DLL

#if defined(DDB_MAKEDLL)     // create a extend DLL library 
#define DDB_EXPORT  __declspec(dllexport)
#define DDB_TEMPLATEDLL
#else                        // use a extend DLL library
#define DDB_EXPORT  __declspec(dllimport)
#endif

#endif // GLOBAL_DLL

#endif // Q_WS_WIN || Q_WS_S60


#ifndef DDB_EXPORT
#define DDB_EXPORT
#endif

#endif // DDB_GLOBAL_H
