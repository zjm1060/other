#ifndef ALARM_GLOBAL_H
#define ALARM_GLOBAL_H

#include "../../libglobal/include/global.h"

#define LIBALARM_VERSION			0x010000
#define LIBALARM_VERSION_STR		"1.0.0"
#define LIBALARM_LOGGER_STR		"root.libalarm"

#if defined(Q_WS_WIN) || defined(Q_WS_S60)

#if defined(_MSC_VER) /* MSVC Compiler */
/* template-class specialization 'identifier' is already instantiated */
#pragma warning(disable: 4660)
#endif // _MSC_VER

#ifdef ALARM_DLL

#if defined(ALARM_MAKEDLL)     // create a extend DLL library 
#define ALARM_EXPORT  __declspec(dllexport)
#define ALARM_TEMPLATEDLL
#else                        // use a extend DLL library
#define ALARM_EXPORT  __declspec(dllimport)
#endif

#endif // ALARM_DLL

#endif // Q_WS_WIN || Q_WS_S60


#ifndef ALARM_EXPORT
#define ALARM_EXPORT
#endif

#endif // ALARM_GLOBAL_H
