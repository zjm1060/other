#ifndef QAPPENDER_H
#define QAPPENDER_H

#include "common.h"
#include "qsharedobject.h"
#include "qloglog.h"
#include "qloggingevent.h"
#include "qloglayout.h"
#include "qfactory.h"
#include "qfilter.h"

class LOGPLUS_EXPORT QErrorHandler
{
public:
	QErrorHandler();
	virtual ~QErrorHandler();
	virtual void error(const QString& err) = 0;
	virtual void reset() = 0;
};

class LOGPLUS_EXPORT QOnlyOnceErrorHandler : public QErrorHandler, protected QLogLogUser
{
public:
	QOnlyOnceErrorHandler() : m_bFirstTime(true){}
	virtual ~QOnlyOnceErrorHandler(){}

public:
	virtual void error(const QString& err);
	virtual void reset();

private:
	bool m_bFirstTime;
};

/**
*	@Property Layout	QLogLayout
**/
class LOGPLUS_EXPORT QAppender : public QSharedObject, protected QLogLogUser
{
public:
	QAppender();
	QAppender(const QProperties& properties);
	virtual ~QAppender();

public:
	void setProperties(const QProperties& properties);
	void destroy();

	virtual void close() = 0;

	void doAppend(const QLoggingEvent& event);

	virtual QString name();
	virtual void setName(const QString& name);

	virtual void setErrorHandler(std::auto_ptr<QErrorHandler> eh);
	virtual QErrorHandler* errorHandler();

	virtual void setLayout(std::auto_ptr<QLogLayout> layout);
	virtual QLogLayout* layout();

	void setFilter(QFilterPtr f) { m_filter = f; }
	QFilterPtr filter() const { return m_filter; }

	LogLevel threshold() const { return m_threshold; }
	void setThreshold(LogLevel th) { m_threshold = th; }

	bool isAsSevereAsThreshold(LogLevel ll) const
	{
		return ((ll != NOT_SET_LOG_LEVEL) && (ll >= m_threshold));
	}

protected:
	virtual void append(const QLoggingEvent& event) = 0;

protected:
	std::auto_ptr<QLogLayout> m_layout;

	QString m_strName;
	LogLevel m_threshold;

	QFilterPtr m_filter;

	std::auto_ptr<QErrorHandler> m_errorHandler;

	bool m_bClosed;
};

typedef QSharedObjectPtr<QAppender> QSharedAppenderPtr;

#endif // QAPPENDER_H

