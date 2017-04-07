#include "qloglevelmanager.h"

#include <QObject>

#define _ALL_STRING		QObject::tr("ALL")
#define _TRACE_STRING	QObject::tr("TRACE")
#define _DEBUG_STRING	QObject::tr("DEBUG")
#define _INFO_STRING	QObject::tr("INFO")
#define _WARN_STRING	QObject::tr("WARNING")
#define _ERROR_STRING	QObject::tr("ERROR")
#define _FATAL_STRING	QObject::tr("FATAL")
#define _OFF_STRING		QObject::tr("OFF")
#define _NOTSET_STRING	QObject::tr("NOTSET")
#define _UNKNOWN_STRING QObject::tr("UNKNOWN")

#define GET_TO_STRING_NODE static_cast<ToStringNode*>(this->toStringMethods)
#define GET_FROM_STRING_NODE static_cast<FromStringNode*>(this->fromStringMethods)

class ToStringNode
{
public:
	ToStringNode(LogLevelToStringMethod m) : method(m), next(0) {}

	LogLevelToStringMethod method;
	ToStringNode* next;
};


class FromStringNode
{
public:
	FromStringNode(StringToLogLevelMethod m) : method(m), next(0) {}

	StringToLogLevelMethod method;
	FromStringNode* next;
};


static QString defaultLogLevelToStringMethod(LogLevel logLevel)
{
	switch(logLevel)
	{
	case OFF_LOG_LEVEL:     return _OFF_STRING;
	case FATAL_LOG_LEVEL:   return _FATAL_STRING;
	case ERROR_LOG_LEVEL:   return _ERROR_STRING;
	case WARN_LOG_LEVEL:    return _WARN_STRING;
	case INFO_LOG_LEVEL:    return _INFO_STRING;
	case DEBUG_LOG_LEVEL:   return _DEBUG_STRING;
	case TRACE_LOG_LEVEL:   return _TRACE_STRING;
	case NOT_SET_LOG_LEVEL: return _NOTSET_STRING;
	};

	return QString();
}


static LogLevel defaultStringToLogLevelMethod(const QString& arg)
{
	QString s = arg.toUpper();

	if(s == _ALL_STRING)   return ALL_LOG_LEVEL;
	if(s == _TRACE_STRING) return TRACE_LOG_LEVEL;
	if(s == _DEBUG_STRING) return DEBUG_LOG_LEVEL;
	if(s == _INFO_STRING)  return INFO_LOG_LEVEL;
	if(s == _WARN_STRING)  return WARN_LOG_LEVEL;
	if(s == _ERROR_STRING) return ERROR_LOG_LEVEL;
	if(s == _FATAL_STRING) return FATAL_LOG_LEVEL;
	if(s == _OFF_STRING)   return OFF_LOG_LEVEL;

	return NOT_SET_LOG_LEVEL;
}

QMutex QLogLevelManager::m_gMutex;
QSharedObjectPtr<QLogLevelManager> QLogLevelManager::m_pInstance;

QLogLevelManager::QLogLevelManager()
: toStringMethods(new ToStringNode(defaultLogLevelToStringMethod)),
fromStringMethods(new FromStringNode(defaultStringToLogLevelMethod))
{

}

QLogLevelManager::~QLogLevelManager()
{
	ToStringNode* toStringTmp = GET_TO_STRING_NODE;
	while(toStringTmp != NULL)
	{
		ToStringNode* tmp = toStringTmp;
		toStringTmp = toStringTmp->next;
		delete tmp;
	}

	FromStringNode* fromStringTmp = GET_FROM_STRING_NODE;
	while(fromStringTmp != NULL)
	{
		FromStringNode* tmp = fromStringTmp;
		fromStringTmp = fromStringTmp->next;
		delete tmp;
	}
}

QSharedObjectPtr<QLogLevelManager> QLogLevelManager::instance()
{
	if (m_pInstance.get() == 0)
	{
		m_gMutex.lock();
		if (m_pInstance.get() == 0)
		{
			m_pInstance=QSharedObjectPtr<QLogLevelManager>(new QLogLevelManager);
		}
		m_gMutex.unlock();
	}

	return m_pInstance;
}

QString QLogLevelManager::toString(LogLevel logLevel) const
{
	ToStringNode* toStringTmp = GET_TO_STRING_NODE;
	while(toStringTmp != NULL)
	{
		QString ret = toStringTmp->method(logLevel);
		if(!ret.isEmpty())
		{
			return ret;
		}

		toStringTmp = toStringTmp->next;
	}

	return _UNKNOWN_STRING;
}

LogLevel QLogLevelManager::fromString(const QString& s) const
{
	FromStringNode* fromStringTmp = GET_FROM_STRING_NODE;
	while(fromStringTmp != NULL)
	{
		LogLevel ret = fromStringTmp->method(s);
		if(ret != NOT_SET_LOG_LEVEL)
		{
			return ret;
		}

		fromStringTmp = fromStringTmp->next;
	}

	return NOT_SET_LOG_LEVEL;
}

void QLogLevelManager::pushToStringMethod(LogLevelToStringMethod newToString)
{
	ToStringNode* toStringTmp = GET_TO_STRING_NODE;
	while(1)
	{
		if(toStringTmp->next != NULL)
		{
			toStringTmp = toStringTmp->next;
		}
		else
		{
			toStringTmp->next = new ToStringNode(newToString);
			break;
		}
	}
}



void QLogLevelManager::pushFromStringMethod(StringToLogLevelMethod newFromString)
{
	FromStringNode* fromStringTmp = GET_FROM_STRING_NODE;
	while(1)
	{
		if(fromStringTmp->next != NULL)
		{
			fromStringTmp = fromStringTmp->next;
		}
		else
		{
			fromStringTmp->next = new FromStringNode(newFromString);
			break;
		}
	}
}
