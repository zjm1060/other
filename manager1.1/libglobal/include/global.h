#ifndef GLOBAL_H
#define GLOBAL_H

#include <qglobal.h>
#if QT_VERSION < 0x040000
#include <qmodules.h>
#endif

#define LIBGLOBAL_VERSION			0x010100
#define LIBGLOBAL_VERSION_STR		"1.1.0"

#define LIBLOGPLUS_VERSION			0x010100
#define LIBLOGPLUS_VERSION_STR		"1.1.0"

#define LIBUTIL_VERSION		0x010000
#define LIBUTIL_VERSION_STR	"1.0.0"
#define LIBUTIL_LOGGER_STR	"root.libutil"

#define LIBMIB_VERSION		0x010000
#define LIBMIB_VERSION_STR	"1.0.0"
#define LIBMIB_LOGGER_STR	"root.libmib"

#define LIBMSG_VERSION			0x010000
#define LIBMSG_VERSION_STR		"1.0.0"
#define LIBMSG_LOGGER_STR		"root.libmsg"

#define LIBNETWORK_VERSION		0x010100
#define LIBNETWORK_VERSION_STR	"1.1.0"
#define LIBNETWORK_LOGGER_STR	"root.libnetwork"

#define LIBTOPOLOGY_VERSION		0x010000
#define LIBTOPOLOGY_VERSION_STR	"1.0.0"
#define LIBTOPOLOGY_LOGGER_STR	"root.libtopology"

#define LIBFC_VERSION			0x010000
#define LIBFC_VERSION_STR		"1.0.0"
#define LIBFC_LOGGER_STR		"root.libfc"

#define MGRSVC_VERSION		0x010000
#define MGRSVC_VERSION_STR	"1.0.0"
#define MGRSVC_LOGGER_STR	"root.mgrsvc"

#define MANAGER_VERSION		0x010100
#define MANAGER_VERSION_STR	"1.1.0"
#define MANAGER_LOGGER_STR	"root.manager"

#define HSHOW_VERSION		0x010000
#define HSHOW_VERSION_STR	"1.0.0"
#define HSHOW_LOGGER_STR	"root.hshow"

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

#ifdef UTIL_DLL

#if defined(UTIL_MAKEDLL)     // create a extend DLL library 
#define UTIL_EXPORT  __declspec(dllexport)
#define UTIL_TEMPLATEDLL
#else                        // use a extend DLL library
#define UTIL_EXPORT  __declspec(dllimport)
#endif

#endif // LOGPLUS_DLL

#ifdef MSG_DLL

#if defined(MSG_MAKEDLL)     // create a extend DLL library 
#define MSG_EXPORT  __declspec(dllexport)
#define MSG_TEMPLATEDLL
#else                        // use a extend DLL library
#define MSG_EXPORT  __declspec(dllimport)
#endif

#endif // MSG_DLL

#ifdef NETWORK_DLL

#if defined(NETWORK_MAKEDLL)     // create a extend DLL library 
#define NETWORK_EXPORT  __declspec(dllexport)
#define NETWORK_TEMPLATEDLL
#else                        // use a extend DLL library
#define NETWORK_EXPORT  __declspec(dllimport)
#endif

#endif // NETWORK_DLL

#ifdef MIB_DLL

#if defined(MIB_MAKEDLL)     // create a extend DLL library 
#define MIB_EXPORT  __declspec(dllexport)
#define MIB_TEMPLATEDLL
#else                        // use a extend DLL library
#define MIB_EXPORT  __declspec(dllimport)
#endif

#endif // UTIL_DLL

#ifdef TOPOLOGY_DLL

#if defined(TOPOLOGY_MAKEDLL)     // create a extend DLL library 
#define TOPOLOGY_EXPORT  __declspec(dllexport)
#define TOPOLOGY_TEMPLATEDLL
#else                        // use a extend DLL library
#define TOPOLOGY_EXPORT  __declspec(dllimport)
#endif

#endif //TOPOLOGY_DLL

#ifdef FC_DLL

#if defined(FC_MAKEDLL)     // create a extend DLL library 
#define FC_EXPORT  __declspec(dllexport)
#define FC_TEMPLATEDLL
#else                        // use a extend DLL library
#define FC_EXPORT  __declspec(dllimport)
#endif

#endif // FC_DLL

#endif // Q_WS_WIN || Q_WS_S60


#ifndef GLOBAL_EXPORT
#define GLOBAL_EXPORT
#endif

#ifndef LOGPLUS_EXPORT
#define LOGPLUS_EXPORT
#endif

#ifndef UTIL_EXPORT
#define UTIL_EXPORT
#endif

#ifndef MIB_EXPORT
#define MIB_EXPORT
#endif

#ifndef MSG_EXPORT
#define MSG_EXPORT
#endif

#ifndef NETWORK_EXPORT
#define NETWORK_EXPORT
#endif

#ifndef TOPOLOGY_EXPORT
#define TOPOLOGY_EXPORT
#endif

#ifndef FC_EXPORT
#define FC_EXPORT
#endif

#include "macro.h"
#include "define.h"

#endif // GLOBAL_H
