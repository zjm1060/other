#ifndef QAPPENDER_H
#define QAPPENDER_H

#include "qloglog.h"
#include "qloggingevent.h"
#include "qloglayoutfactory.h"
#include "qfilterfactory.h"

class LOGPLUS_EXPORT QErrorHandler : public QSharedObject
{
public:
	QErrorHandler();
	virtual ~QErrorHandler();
	virtual void error(const QString& err) = 0;
	virtual void reset() = 0;
};

typedef QSharedObjectPtr<QErrorHandler>	QErrorHandlerPtr;

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

	virtual void setErrorHandler(const QErrorHandlerPtr& eh);
	virtual const QErrorHandlerPtr& errorHandler() const;

	virtual void setLayout(const QLogLayoutPtr& layout);
	virtual const QLogLayoutPtr& layout() const;

	void setFilter(const QFilterPtr& f) { m_pFilter = f; }
	const QFilterPtr& filter() const { return m_pFilter; }

	LogLevel threshold() const { return m_threshold; }
	void setThreshold(LogLevel th) { m_threshold = th; }

	bool isAsSevereAsThreshold(LogLevel ll) const
	{
		return ((ll != NOT_SET_LOG_LEVEL) && (ll >= m_threshold));
	}

protected:
	virtual void append(const QLoggingEvent& event) = 0;

protected:
	QLogLayoutPtr m_pLayout;

	QString m_strName;
	LogLevel m_threshold;

	QFilterPtr m_pFilter;

	QErrorHandlerPtr m_pErrorHandler;

	bool m_bClosed;

	QMutex m_mutex;
};

typedef QSharedObjectPtr<QAppender> QAppenderPtr;

#endif // QAPPENDER_H

