#ifndef QAPPENDERATTACHABLE_H
#define QAPPENDERATTACHABLE_H

#include <QVector>

#include "qappender.h"

typedef QVector<QSharedAppenderPtr> QSharedAppenderPtrList;

class LOGPLUS_EXPORT QAppenderAttachable
{
public:
	virtual void addAppender(QSharedAppenderPtr appender) = 0;

	virtual QSharedAppenderPtrList appenders() = 0;
	virtual QSharedAppenderPtr appender(const QString& name) = 0;

	virtual void removeAllAppenders() = 0;
	virtual void removeAppender(QSharedAppenderPtr appender) = 0;
	virtual void removeAppender(const QString& name) = 0;
};


class LOGPLUS_EXPORT QAppenderAttachableImplement : public QAppenderAttachable, protected QLogLogUser
{
public:
	QAppenderAttachableImplement();
	virtual ~QAppenderAttachableImplement();

public:
	virtual void addAppender(QSharedAppenderPtr appender);

	virtual QSharedAppenderPtrList appenders();
	virtual QSharedAppenderPtr appender(const QString& name);

	virtual void removeAllAppenders();
	virtual void removeAppender(QSharedAppenderPtr appender);
	virtual void removeAppender(const QString& name);

	int appendLoopOnAppenders(const QLoggingEvent& event);

protected:
	QMutex m_mutex;

protected:
	QSharedAppenderPtrList m_appenders;
};

#endif // QAPPENDERATTACHABLE_H
