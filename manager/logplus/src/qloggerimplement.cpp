#include "qloggerimplement.h"

#include <stdexcept>

QLoggerImplement::QLoggerImplement(const QString& name, QHierarchy& hierarchy)
: m_strName(name),
m_logLevel(NOT_SET_LOG_LEVEL),
m_parent(NULL),
m_bAdditive(true), 
m_hierarchy(hierarchy)
{
}

QLoggerImplement::~QLoggerImplement()
{

}

void QLoggerImplement::callAppenders(const QLoggingEvent& event)
{
	int writes = 0;
	for(QLoggerImplement* c = this; c != NULL; c=c->m_parent.get())
	{
		writes += ((QAppenderAttachableImplement*)c)->appendLoopOnAppenders(event);
		if(!c->m_bAdditive)
		{
			break;
		}
	}

	// No appenders in hierarchy, warn user only once.
	if(!m_hierarchy.m_bEmittedNoAppenderWarning && writes == 0)
	{
		logLog().error("No appenders could be found for logger ("+ name()+").");
		logLog().error("Please initialize the log4cplus system properly.");
		m_hierarchy.m_bEmittedNoAppenderWarning = true;
	}
}

void QLoggerImplement::closeNestedAppenders()
{
	QSharedAppenderPtrList appenders = QAppenderAttachableImplement::appenders();
	for (int i=0; i<appenders.size(); i++)
	{
		appenders[i]->close();
	}
}

bool QLoggerImplement::isEnabledFor(LogLevel logLevel) const
{
	if(m_hierarchy.m_nDisableValue >= logLevel)
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

LogLevel QLoggerImplement::chainedLogLevel() const
{
	for(const QLoggerImplement *c=this; c != NULL; c=c->m_parent.get())
	{
		if(c->m_logLevel != NOT_SET_LOG_LEVEL)
		{
			return c->m_logLevel;
		}
	}

	logLog().error("LoggerImpl::getChainedLogLevel()- No valid LogLevel found");
	throw std::runtime_error("No valid LogLevel found");
}

QHierarchy& QLoggerImplement::hierarchy() const
{ 
	return m_hierarchy; 
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


QRootLogger::QRootLogger(QHierarchy& hierarchy, LogLevel logLevel)
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
		logLog().error("You have tried to set NOT_SET_LOG_LEVEL to root.");
	}
	else
	{
		QLoggerImplement::setLogLevel(logLevel);
	}
}
