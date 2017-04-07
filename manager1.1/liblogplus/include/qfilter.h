#ifndef QFILTER_H
#define QFILTER_H

#include "qfilterfactory.h"

class LOGPLUS_EXPORT QDenyAllFilter : public QFilter
{
public:
	QDenyAllFilter ();
	QDenyAllFilter (const QProperties&);
	virtual ~QDenyAllFilter();

public:
	virtual FilterResult decide(const QLoggingEvent& event) const;
};

typedef QTemplateFactory<QDenyAllFilter, QFilterFactoryBase>			QDenyAllFilterFactory;

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

typedef QTemplateFactory<QLogLevelMatchFilter, QFilterFactoryBase>		QLogLevelMatchFilterFactory;

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

typedef QTemplateFactory<QLogLevelRangeFilter, QFilterFactoryBase>		QLogLevelRangeFilterFactory;

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

typedef QTemplateFactory<QStringMatchFilter, QFilterFactoryBase>		QStringMatchFilterFactory;

#endif // QFILTER_H

