#ifndef CTL_GLOBAL_H
#define CTL_GLOBAL_H

#define LIBCTL_VERSION			0x010000
#define LIBCTL_VERSION_STR		"1.0.0"
#define LIBCTL_LOGGER_STR		"root.libctl"

#if defined(Q_WS_WIN) || defined(Q_WS_S60)

#if defined(_MSC_VER) /* MSVC Compiler */
/* template-class specialization 'identifier' is already instantiated */
#pragma warning(disable: 4660)
#endif // _MSC_VER

#ifdef CTL_DLL

#if defined(CTL_MAKEDLL)     // create a extend DLL library 
#define CTL_EXPORT  __declspec(dllexport)
#define CTL_TEMPLATEDLL
#else                        // use a extend DLL library
#define CTL_EXPORT  __declspec(dllimport)
#endif

#endif // CTL_DLL

#endif // Q_WS_WIN || Q_WS_S60


#ifndef CTL_EXPORT
#define CTL_EXPORT
#endif

#endif // CTL_GLOBAL_H
