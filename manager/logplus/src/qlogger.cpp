#include "qlogger.h"

#include "qloggerimplement.h"

QLogger::QLogger()
:m_loggerImplement(0)
{

}

QLogger::QLogger(QLoggerImplement* ptr)
:m_loggerImplement(ptr)
{
	if (m_loggerImplement != NULL)
	{
		m_loggerImplement->addRef();
	}
}

QLogger::QLogger (const QLogger& logger)
:QAppenderAttachable(logger)
,m_loggerImplement(logger.m_loggerImplement)
{
	if (m_loggerImplement != NULL)
	{
		m_loggerImplement->addRef();
	}
}

QLogger::~QLogger()
{
	if (m_loggerImplement != NULL)
	{
		m_loggerImplement->removeRef();
	}
}

QHierarchy& QLogger::defaultHierarchy()
{
	static QHierarchy hierarchy;

	return hierarchy;
}

bool QLogger::exists(const QString& name) 
{
	return defaultHierarchy().exists(name); 
}

QLoggerList QLogger::currentLoggers () 
{
	return defaultHierarchy().currentLoggers();
}

QLogger& QLogger::instance(const QString& name)
{ 
	return defaultHierarchy().loggerInstance(name); 
}

QLogger& QLogger::instance (const QString& name, QLoggerFactoryInterface& factory)
{ 
	return defaultHierarchy().loggerInstance(name, factory); 
}

QLogger& QLogger::root() 
{ 
	return defaultHierarchy().root(); 
}

void QLogger::shutdown() 
{ 
	defaultHierarchy().shutdown(); 
}

QLogger& QLogger::operator = (const QLogger& logger)
{
	QLogger(logger).swap(*this);

	return *this;
}

void QLogger::swap(QLogger& other)
{
	qSwap(m_loggerImplement, other.m_loggerImplement);
}

QLogger QLogger::parent() const 
{
	if (m_loggerImplement->m_parent)
		return QLogger(m_loggerImplement->m_parent.get());
	else
	{
		m_loggerImplement->logLog().error("********* This logger has no parent: " + name());

		return *this;
	}
}

void QLogger::addAppender(QSharedAppenderPtr appender)
{
	m_loggerImplement->addAppender(appender);
}

QSharedAppenderPtrList QLogger::appenders ()
{
	return m_loggerImplement->appenders();
}


QSharedAppenderPtr QLogger::appender(const QString& name)
{
	return m_loggerImplement->appender(name);
}


void QLogger::removeAllAppenders()
{
	m_loggerImplement->removeAllAppenders();
}


void QLogger::removeAppender(QSharedAppenderPtr appender)
{
	m_loggerImplement->removeAppender(appender);
}


void QLogger::removeAppender(const QString& name)
{
	m_loggerImplement->removeAppender(name);
}

void QLogger::assertion(bool assert, const QString& msg) const
{
	if (!assert)
	{
		log(FATAL_LOG_LEVEL, msg);
	}
}

void QLogger::closeNestedAppenders() const
{
	m_loggerImplement->closeNestedAppenders();
}

bool QLogger::isEnabledFor(LogLevel logLevel) const
{
	return m_loggerImplement->isEnabledFor(logLevel);
}

void QLogger::log(LogLevel logLevel, const QString& message, const char* file, int line) const
{
	 m_loggerImplement->log(logLevel, message, file, line);
}

void QLogger::forcedLog(LogLevel logLevel, const QString& message, const char* file, int line) const
{
	m_loggerImplement->forcedLog (logLevel, message, file, line);
}

void QLogger::callAppenders(const QLoggingEvent& event) const
{
	m_loggerImplement->callAppenders (event);
}

LogLevel QLogger::chainedLogLevel() const
{
	return m_loggerImplement->chainedLogLevel();
}

LogLevel QLogger::logLevel() const
{
	return m_loggerImplement->logLevel();
}

void QLogger::setLogLevel(LogLevel logLevel)
{
	m_loggerImplement->setLogLevel(logLevel);
}

QHierarchy& QLogger::hierarchy() const
{ 
	return m_loggerImplement->hierarchy();
}

QString QLogger::name() const
{
	return m_loggerImplement->name();
}

bool QLogger::additivity() const
{
	return m_loggerImplement->additivity();
}

void QLogger::setAdditivity (bool additive)
{ 
	m_loggerImplement->setAdditivity (additive);
}

