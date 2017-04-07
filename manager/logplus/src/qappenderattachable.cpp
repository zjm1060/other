#include "qappenderattachable.h"

QAppenderAttachableImplement::QAppenderAttachableImplement()
{

}

QAppenderAttachableImplement::~QAppenderAttachableImplement()
{

}

void QAppenderAttachableImplement::addAppender(QSharedAppenderPtr appender)
{
	QMutexLocker locker(&m_mutex);

	if (appender == NULL)
	{
		logLog().warn("Tried to add NULL appender!");
		return;
	}

	for (int i=0; i<m_appenders.size(); i++)
	{
		if (m_appenders[i] == appender)
		{
			return;
		}
	}

	m_appenders.append(appender);
}

QSharedAppenderPtrList QAppenderAttachableImplement::appenders()
{
	QMutexLocker locker(&m_mutex);

	return m_appenders;
}

QSharedAppenderPtr QAppenderAttachableImplement::appender(const QString& name)
{
	QMutexLocker locker(&m_mutex);

	for (int i=0; i<m_appenders.size(); i++)
	{
		if (m_appenders[i]->name() == name)
		{
			return m_appenders[i];
		}
	}

	return QSharedAppenderPtr(NULL);
}

void QAppenderAttachableImplement::removeAllAppenders()
{
	QMutexLocker locker(&m_mutex);

	m_appenders.clear();
}

void QAppenderAttachableImplement::removeAppender(QSharedAppenderPtr appender)
{
	if(appender == NULL)
	{
		logLog().warn("Tried to remove NULL appender");
		return;
	}

	QMutexLocker locker(&m_mutex);

	for (int i=0; i<m_appenders.size(); i++)
	{
		if (m_appenders[i] == appender)
		{
			m_appenders.remove(i);
			break;
		}
	}
}

void QAppenderAttachableImplement::removeAppender(const QString& name)
{
	removeAppender(appender(name));
}

int QAppenderAttachableImplement::appendLoopOnAppenders(const QLoggingEvent& event)
{
	int count = 0;

	QMutexLocker locker(&m_mutex);
	for (int i=0; i<m_appenders.size(); i++)
	{
		++count;
		m_appenders[i]->doAppend(event);
	}
	
	return count;
}
