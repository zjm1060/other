#ifndef QAPPENDERATTACHABLE_H
#define QAPPENDERATTACHABLE_H

#include <QVector>

#include "qappender.h"

typedef QVector<QAppenderPtr > QAppenderPtrList;

class LOGPLUS_EXPORT QAppenderAttachable : public QSharedObject
{
public:
	virtual void addAppender(const QAppenderPtr& appender) = 0;

	virtual const QAppenderPtrList& appenders() = 0;
	virtual QAppenderPtr appender(const QString& name) = 0;

	virtual void removeAllAppenders() = 0;
	virtual void removeAppender(const QAppenderPtr& appender) = 0;
	virtual void removeAppender(const QString& name) = 0;
};

typedef QSharedObjectPtr<QAppenderAttachable>	QAppenderAttachablePtr;

class LOGPLUS_EXPORT QAppenderAttachableImplement : public QAppenderAttachable, protected QLogLogUser
{
public:
	QAppenderAttachableImplement();
	virtual ~QAppenderAttachableImplement();

public:
	virtual void addAppender(const QAppenderPtr& appender);

	virtual const QAppenderPtrList& appenders();
	virtual QAppenderPtr appender(const QString& name);

	virtual void removeAllAppenders();
	virtual void removeAppender(const QAppenderPtr& appender);
	virtual void removeAppender(const QString& name);

	int appendLoopOnAppenders(const QLoggingEvent& event);

protected:
	QMutex m_mutex;

protected:
	QAppenderPtrList m_appenders;
};

#endif // QAPPENDERATTACHABLE_H
