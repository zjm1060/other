#ifndef QFILTER_H
#define QFILTER_H

#include "qsharedobject.h"
#include "qloggingevent.h"
#include "qproperties.h"

class QFilter;
typedef QSharedObjectPtr<QFilter> QFilterPtr;

class LOGPLUS_EXPORT QFilter : public QSharedObject
{
public:
	QFilter();
	virtual ~QFilter();

public:
	enum FilterResult
	{ 
		DENY,
		NEUTRAL,
		ACCEPT
	};

public:
	static FilterResult checkFilter(const QFilter* filter, const QLoggingEvent& event);

public:
	void appendFilter(QFilterPtr filter);

public:
	virtual FilterResult decide(const QLoggingEvent& event) const = 0;
	virtual void setProperties(const QProperties& properties);

private:
	QFilterPtr next;
};

class LOGPLUS_EXPORT QDenyAllFilter : public QFilter
{
public:
	QDenyAllFilter ();
	QDenyAllFilter (const QProperties&);
	virtual ~QDenyAllFilter();

public:
	virtual FilterResult decide(const QLoggingEvent& event) const;
};

/**
*	@Property AcceptOnMatch	bool
*	@Property LogLevel int
**/
class LOGPLUS_EXPORT QLogLevelMatchFilter : public QFilter
{
public:
	QLogLevelMatchFilter();
	QLogLevelMatchFilter(const QProperties& properties);
	virtual ~QLogLevelMatchFilter();

public:
	virtual FilterResult decide(const QLoggingEvent& event) const;
	virtual void setProperties(const QProperties& properties);

private:
	void init();

private:
	bool m_bAcceptOnMatch;
	LogLevel m_logLevelToMatch;
};

/**
*	@Property AcceptOnMatch	bool
*	@Property LogLevelMin int
*	@Property LogLevelMax int
**/
class LOGPLUS_EXPORT QLogLevelRangeFilter : public QFilter
{
public:
	QLogLevelRangeFilter();
	QLogLevelRangeFilter(const QProperties& properties);
	virtual ~QLogLevelRangeFilter();

public:
	virtual FilterResult decide(const QLoggingEvent& event) const;
	virtual void setProperties(const QProperties& properties);

private:
	void init();

private:
	bool m_bAcceptOnMatch;
	LogLevel m_logLevelMin;
	LogLevel m_logLevelMax;
};

/**
*	@Property AcceptOnMatch bool
*	@Property StringToMatch QString
**/
class LOGPLUS_EXPORT QStringMatchFilter : public QFilter
{
public:
	QStringMatchFilter();
	QStringMatchFilter(const QProperties& properties);
	virtual ~QStringMatchFilter();

public:
	virtual FilterResult decide(const QLoggingEvent& event) const;
	virtual void setProperties(const QProperties& properties);

private:
	void init();

private:
	bool m_bAcceptOnMatch;
	QString m_stringToMatch;
};
#endif // QFILTER_H

