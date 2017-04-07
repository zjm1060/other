#ifndef QLOGLEVELMANAGER_H
#define QLOGLEVELMANAGER_H

#include "qsharedobject.h"

typedef int LogLevel;

const LogLevel OFF_LOG_LEVEL     = 60000;		//turn off logging

//designates very severe error events that will presumably lead the application to abort.
const LogLevel FATAL_LOG_LEVEL   = 50000;

//designates error events that might still allow the application to continue running.
const LogLevel ERROR_LOG_LEVEL   = 40000;

//designates potentially harmful situations.
const LogLevel WARN_LOG_LEVEL    = 30000;

//designates informational messages  that highlight the progress of the application at coarse-grained level.
const LogLevel INFO_LOG_LEVEL    = 20000;

//designates fine-grained informational events that are most useful to debug an application.
const LogLevel DEBUG_LOG_LEVEL   = 10000;

// The LogLevel is used to "trace" entry and exiting of methods.
const LogLevel TRACE_LOG_LEVEL   = 0;

//The LogLevel is used during configuration to turn on all logging.
const LogLevel ALL_LOG_LEVEL     = TRACE_LOG_LEVEL;

//The LogLevel is used to indicated that no particular LogLevel is desired and that the default should be used.
const LogLevel NOT_SET_LOG_LEVEL = -1;

/** 
* This method type defined the signature of methods that convert LogLevels
* into strings. 
*/
typedef QString (*LogLevelToStringMethod)(LogLevel);

/** 
* This method type defined the signature of methods that convert strings
* into LogLevels. 
*/
typedef LogLevel (*StringToLogLevelMethod)(const QString&);


/**
* This class is used to "manage" LogLevel definitions.  This class is also
* how "derived" LogLevels are created.
*/
class QLogLevelManager;
typedef QSharedObjectPtr<QLogLevelManager> QLogLevelManagerPtr;

class LOGPLUS_EXPORT QLogLevelManager : public QSharedObject
{
public:
	virtual ~QLogLevelManager();

private:
	QLogLevelManager();

public:
	static QLogLevelManagerPtr instance();

public:
	QString toString(LogLevel ll) const;
	LogLevel fromString(const QString& s) const;

	void pushToStringMethod(LogLevelToStringMethod newToString);
	void pushFromStringMethod(StringToLogLevelMethod newFromString);

private:
	static QMutex m_mutex;
	static QLogLevelManagerPtr m_pInstance;

private:
	void* toStringMethods;
	void* fromStringMethods;

	// Disable Copy
	QLogLevelManager(const QLogLevelManager&);
	QLogLevelManager& operator=(const QLogLevelManager&);
};

#endif // QLOGLEVELMANAGER_H
