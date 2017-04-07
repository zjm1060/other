#ifndef GLOBAL_H
#define GLOBAL_H

#include <qglobal.h>
#if QT_VERSION < 0x040000
#include <qmodules.h>
#endif

#define GLOBAL_VERSION			0x010000
#define GLOBAL_VERSION_STR		"1.0.0"

#define LOGPLUS_VERSION			0x010000
#define LOGPLUS_VERSION_STR		"1.0.0"

#define NETWORK_VERSION			0x010000
#define NETWORK_VERSION_STR		"1.0.0"

#define TOPOLOGY_VERSION		0x010000
#define TOPOLOGY_VERSION_STR	"1.0.0"

#define MANAGER_VERSION		0x010000
#define MANAGER_VERSION_STR	"1.0.0"

#if defined(Q_WS_WIN) || defined(Q_WS_S60)

#if defined(_MSC_VER) /* MSVC Compiler */
/* template-class specialization 'identifier' is already instantiated */
#pragma warning(disable: 4660)
#endif // _MSC_VER

#ifdef GLOBAL_DLL

#if defined(GLOBAL_MAKEDLL)     // create a extend DLL library 
#define GLOBAL_EXPORT  __declspec(dllexport)
#define GLOBAL_TEMPLATEDLL
#else                        // use a extend DLL library
#define GLOBAL_EXPORT  __declspec(dllimport)
#endif

#endif // GLOBAL_DLL

#ifdef LOGPLUS_DLL

#if defined(LOGPLUS_MAKEDLL)     // create a extend DLL library 
#define LOGPLUS_EXPORT  __declspec(dllexport)
#define LOGPLUS_TEMPLATEDLL
#else                        // use a extend DLL library
#define LOGPLUS_EXPORT  __declspec(dllimport)
#endif

#endif // LOGPLUS_DLL

#ifdef NETWORK_DLL

#if defined(NETWORK_MAKEDLL)     // create a extend DLL library 
#define NETWORK_EXPORT  __declspec(dllexport)
#define NETWORK_TEMPLATEDLL
#else                        // use a extend DLL library
#define NETWORK_EXPORT  __declspec(dllimport)
#endif

#endif // NETWORK_DLL

#ifdef TOPOLOGY_DLL

#if defined(TOPOLOGY_MAKEDLL)     // create a extend DLL library 
#define TOPOLOGY_EXPORT  __declspec(dllexport)
#define TOPOLOGY_TEMPLATEDLL
#else                        // use a extend DLL library
#define TOPOLOGY_EXPORT  __declspec(dllimport)
#endif

#endif // TOPOLOGY_DLL

#endif // Q_WS_WIN || Q_WS_S60


#ifndef GLOBAL_EXPORT
#define GLOBAL_EXPORT
#endif

#ifndef LOGPLUS_EXPORT
#define LOGPLUS_EXPORT
#endif

#ifndef NETWORK_EXPORT
#define NETWORK_EXPORT
#endif

#ifndef TOPOLOGY_EXPORT
#define TOPOLOGY_EXPORT
#endif

#include "macro.h"
#include "define.h"

#endif // GLOBAL_H
