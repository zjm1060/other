#ifndef QLOGGER_H
#define QLOGGER_H

#include <QVector>

#include "qappenderattachable.h"
#include "qloggerfactory.h"
#include "qloglevelmanager.h"
#include "qndc.h"
#include "qloggerimplement.h"

class QAppender;
class QHierarchy;
class QLoggerFactoryInterface;

class QLogger;
typedef QSharedObjectPtr<QLogger>	QLoggerPtr;
typedef QVector<QLoggerPtr > QLoggerPtrList;

class LOGPLUS_EXPORT QLogger : public QSharedObject
{
public:
	QLogger();
	QLogger(const QLogger& logger);
	~QLogger();

public:
	static bool exists(const QString& name);

	static QLoggerPtrList currentLoggers();

	static QHierarchyPtr& defaultHierarchy();

	static QLoggerPtr instance(const QString& name);
	static QLoggerPtr instance(const QString& name, const QLoggerFactoryInterfacePtr& factory);

	static QLoggerPtr& root();

	static void shutdown();

public:
	void assertion(bool assert, const QString& msg) const;

	void closeNestedAppenders() const;

	bool isEnabledFor(LogLevel logLevel) const;

	void log(LogLevel logLevel, const QString& message, const char* file=NULL, int line=-1) const;
 
	void forcedLog(LogLevel logLevel, const QString& message, const char* file=NULL, int line=-1) const;

	void callAppenders(const QLoggingEvent& event) const;

	LogLevel chainedLogLevel() const;
	LogLevel logLevel() const;
	void setLogLevel(LogLevel logLevel);

	const QSharedObjectPtr<QHierarchy>& hierarchy() const;

	QString name() const;

	bool additivity() const;
	void setAdditivity(bool additive);

	QLoggerPtr parent() const;

public:
	// AppenderAttachable Methods
	virtual void addAppender(const QAppenderPtr& appender);

	virtual QAppenderPtrList appenders();

	virtual QAppenderPtr appender(const QString& name);

	virtual void removeAllAppenders();

	virtual void removeAppender(const QAppenderPtr& appender);

	virtual void removeAppender(const QString& name);

public:
	QLogger& operator=(const QLogger& logger);

protected:
	void swap(QLogger& logger);

private:
	QLogger(QLoggerImplementPtr& ptr);

protected:
	QLoggerImplementPtr m_pLoggerImplement;

private:
	friend class QLoggerImplement;
	friend class QHierarchy;
	friend class QDefaultLoggerFactory;
};

class LOGPLUS_EXPORT QTraceLogger
{
public:
	QTraceLogger(const QLoggerPtr& logger, const QString& msg, const char* file=NULL, int line=-1) 
		: m_logger(logger), m_strMsg(msg), m_strFile(file), m_nLine(line)
	{
		if(m_logger !=NULL && m_logger->isEnabledFor(TRACE_LOG_LEVEL))
			m_logger->forcedLog(TRACE_LOG_LEVEL, "ENTER: " + m_strMsg, m_strFile, m_nLine); 
	}

	~QTraceLogger()
	{
		if(m_logger !=NULL && m_logger->isEnabledFor(TRACE_LOG_LEVEL))
			m_logger->forcedLog(TRACE_LOG_LEVEL, "EXIT:  " + m_strMsg, m_strFile, m_nLine); 
	}

private:
	QLoggerPtr m_logger;
	QString m_strMsg;
	const char* m_strFile;
	int m_nLine;
};

#define LOGPLUS_MACRO_STR_BODY(logger, log, level)	\
	do	\
	{	\
		if(logger->isEnabledFor(level##_LOG_LEVEL))		\
	{	\
		logger->forcedLog(level##_LOG_LEVEL, log, __FILE__, __LINE__);	\
	}	\
} while (0)

#define LOGPLUS_TRACE_METHOD(logger, log)	QTraceLogger _logplus_trace_logger(logger, log, __FILE__, __LINE__);
#define LOGPLUS_TRACE_STR(logger, log)		LOGPLUS_MACRO_STR_BODY(logger, log, TRACE)
#define LOGPLUS_DEBUG_STR(logger, log)		LOGPLUS_MACRO_STR_BODY(logger, log, DEBUG)
#define LOGPLUS_INFO_STR(logger, log)		LOGPLUS_MACRO_STR_BODY(logger, log, INFO)
#define LOGPLUS_WARN_STR(logger, log)		LOGPLUS_MACRO_STR_BODY(logger, log, WARN)
#define LOGPLUS_ERROR_STR(logger, log)		LOGPLUS_MACRO_STR_BODY(logger, log, ERROR)
#define LOGPLUS_FATAL_STR(logger, log)		LOGPLUS_MACRO_STR_BODY(logger, log, FATAL)

#define logplusTrace(loggerName, log)		LOGPLUS_TRACE_STR(QLogger::instance(loggerName), log)
#define logplusDebug(loggerName, log)		LOGPLUS_DEBUG_STR(QLogger::instance(loggerName), log)
#define logplusInfo(loggerName, log)		LOGPLUS_INFO_STR(QLogger::instance(loggerName), log)
#define logplusWarning(loggerName, log)		LOGPLUS_WARN_STR(QLogger::instance(loggerName), log)
#define logplusError(loggerName, log)		LOGPLUS_ERROR_STR(QLogger::instance(loggerName), log)
#define logplusFatal(loggerName, log)		LOGPLUS_FATAL_STR(QLogger::instance(loggerName), log)

#endif // QLOGGER_H
