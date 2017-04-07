#include "qlogger.h"

#include "qhierarchy.h"

QLogger::QLogger()
{

}

QLogger::QLogger(QLoggerImplementPtr& ptr)
:m_pLoggerImplement(ptr)
{
}

QLogger::QLogger (const QLogger& logger)
:m_pLoggerImplement(logger.m_pLoggerImplement)
{
}

QLogger::~QLogger()
{
}

QHierarchyPtr& QLogger::defaultHierarchy()
{
	static QHierarchyPtr pHierarchy(new QHierarchy);

	return pHierarchy;
}

bool QLogger::exists(const QString& name) 
{
	return defaultHierarchy()->exists(name); 
}

QLoggerPtrList QLogger::currentLoggers () 
{
	return defaultHierarchy()->currentLoggers();
}

QLoggerPtr QLogger::instance(const QString& name)
{ 
	return defaultHierarchy()->loggerInstance(name); 
}

QLoggerPtr QLogger::instance (const QString& name, const QLoggerFactoryInterfacePtr& factory)
{ 
	return defaultHierarchy()->loggerInstance(name, factory); 
}

QLoggerPtr& QLogger::root() 
{ 
	return defaultHierarchy()->root(); 
}

void QLogger::shutdown() 
{ 
	defaultHierarchy()->shutdown(); 
}

QLogger& QLogger::operator = (const QLogger& logger)
{
	QLogger(logger).swap(*this);

	return *this;
}

void QLogger::swap(QLogger& other)
{
	qSwap(m_pLoggerImplement, other.m_pLoggerImplement);
}

QLoggerPtr QLogger::parent() const 
{
	if (m_pLoggerImplement->m_pParent)
		return QLoggerPtr(new QLogger(m_pLoggerImplement->m_pParent));
	else
	{
		m_pLoggerImplement->loglog()->error("********* This logger has no parent: " + name());

		return QLoggerPtr((QLogger*)this);
	}
}

void QLogger::addAppender(const QAppenderPtr& appender)
{
	m_pLoggerImplement->addAppender(appender);
}

QAppenderPtrList QLogger::appenders ()
{
	return m_pLoggerImplement->appenders();
}


QAppenderPtr QLogger::appender(const QString& name)
{
	return m_pLoggerImplement->appender(name);
}


void QLogger::removeAllAppenders()
{
	m_pLoggerImplement->removeAllAppenders();
}


void QLogger::removeAppender(const QAppenderPtr& appender)
{
	m_pLoggerImplement->removeAppender(appender);
}


void QLogger::removeAppender(const QString& name)
{
	m_pLoggerImplement->removeAppender(name);
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
	m_pLoggerImplement->closeNestedAppenders();
}

bool QLogger::isEnabledFor(LogLevel logLevel) const
{
	return m_pLoggerImplement->isEnabledFor(logLevel);
}

void QLogger::log(LogLevel logLevel, const QString& message, const char* file, int line) const
{
	 m_pLoggerImplement->log(logLevel, message, file, line);
}

void QLogger::forcedLog(LogLevel logLevel, const QString& message, const char* file, int line) const
{
	m_pLoggerImplement->forcedLog (logLevel, message, file, line);
}

void QLogger::callAppenders(const QLoggingEvent& event) const
{
	m_pLoggerImplement->callAppenders (event);
}

LogLevel QLogger::chainedLogLevel() const
{
	return m_pLoggerImplement->chainedLogLevel();
}

LogLevel QLogger::logLevel() const
{
	return m_pLoggerImplement->logLevel();
}

void QLogger::setLogLevel(LogLevel logLevel)
{
	m_pLoggerImplement->setLogLevel(logLevel);
}

const QHierarchyPtr& QLogger::hierarchy() const
{ 
	return m_pLoggerImplement->hierarchy();
}

QString QLogger::name() const
{
	return m_pLoggerImplement->name();
}

bool QLogger::additivity() const
{
	return m_pLoggerImplement->additivity();
}

void QLogger::setAdditivity (bool additive)
{ 
	m_pLoggerImplement->setAdditivity (additive);
}

