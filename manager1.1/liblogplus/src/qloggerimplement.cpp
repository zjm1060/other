#include "qloggerimplement.h"

#include "qhierarchy.h"

#include <stdexcept>

QLoggerImplement::QLoggerImplement(const QString& name, const QHierarchyPtr& hierarchy)
: m_strName(name),
m_logLevel(NOT_SET_LOG_LEVEL),
m_pParent(NULL),
m_bAdditive(true), 
m_pHierarchy(hierarchy)
{
}

QLoggerImplement::~QLoggerImplement()
{

}

void QLoggerImplement::callAppenders(const QLoggingEvent& event)
{
	int writes = 0;
	for(QLoggerImplement* c = this; c != NULL; c=c->m_pParent.get())
	{
		writes += ((QAppenderAttachableImplement*)c)->appendLoopOnAppenders(event);
		if(!c->m_bAdditive)
		{
			break;
		}
	}

	// No appenders in hierarchy, warn user only once.
	if(!m_pHierarchy->m_bEmittedNoAppenderWarning && writes == 0)
	{
		loglog()->error("No appenders could be found for logger ("+ name()+").");
		loglog()->error("Please initialize the logplus system properly.");
		m_pHierarchy->m_bEmittedNoAppenderWarning = true;
	}
}

void QLoggerImplement::closeNestedAppenders()
{
	QAppenderPtrList appenders = QAppenderAttachableImplement::appenders();
	for (int i=0; i<appenders.size(); i++)
	{
		appenders[i]->close();
	}
}

bool QLoggerImplement::isEnabledFor(LogLevel logLevel)
{
	if(m_pHierarchy->m_nDisableValue >= logLevel)
	{
		return false;
	}

	return logLevel >= chainedLogLevel();
}

void QLoggerImplement::log(LogLevel logLevel, const QString& message, const char* file, int line)
{
	if(isEnabledFor(logLevel))
	{
		forcedLog(logLevel, message, file, line);
	}
}

LogLevel QLoggerImplement::chainedLogLevel()
{
	for(const QLoggerImplement *c=this; c != NULL; c=c->m_pParent.get())
	{
		if(c->m_logLevel != NOT_SET_LOG_LEVEL)
		{
			return c->m_logLevel;
		}
	}

	loglog()->error("LoggerImpl::getChainedLogLevel()- No valid LogLevel found");
	throw std::runtime_error("No valid LogLevel found");
}

const QHierarchyPtr& QLoggerImplement::hierarchy() const
{ 
	return m_pHierarchy; 
}

bool QLoggerImplement::additivity() const
{ 
	return m_bAdditive; 
}

void QLoggerImplement::setAdditivity(bool additive)
{
	this->m_bAdditive = additive;
}

void QLoggerImplement::forcedLog(LogLevel logLevel, const QString& message, const char* file, int line)
{
	callAppenders(QLoggingEvent(this->name(), logLevel, message, file, line));
}


QRootLogger::QRootLogger(const QHierarchyPtr& hierarchy, LogLevel logLevel)
: QLoggerImplement("root", hierarchy)
{
	setLogLevel(logLevel);
}

QRootLogger::~QRootLogger()
{

};

LogLevel QRootLogger::chainedLogLevel() const
{
	return m_logLevel;
}

void QRootLogger::setLogLevel(LogLevel logLevel)
{
	if(logLevel == NOT_SET_LOG_LEVEL)
	{
		loglog()->error("You have tried to set NOT_SET_LOG_LEVEL to root.");
	}
	else
	{
		QLoggerImplement::setLogLevel(logLevel);
	}
}
