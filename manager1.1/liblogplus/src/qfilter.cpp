#include "qfilter.h"

QDenyAllFilter::QDenyAllFilter ()
{

}

QDenyAllFilter::QDenyAllFilter(const QProperties&)
{

}

QDenyAllFilter::~QDenyAllFilter ()
{

}

QFilter::FilterResult QDenyAllFilter::decide(const QLoggingEvent&) const
{
	return DENY;
}

Q_EXPORT_REGISTER_FILTER_FACTORY(QDenyAllFilter, QDenyAllFilterFactory)

QLogLevelMatchFilter::QLogLevelMatchFilter()
{
	init();
}

QLogLevelMatchFilter::QLogLevelMatchFilter(const QProperties& properties)
{
	init();
	setProperties(properties);
}

QLogLevelMatchFilter::~QLogLevelMatchFilter()
{

}

void QLogLevelMatchFilter::init()
{
	m_bAcceptOnMatch = true;
	m_logLevelToMatch = NOT_SET_LOG_LEVEL;
}

void QLogLevelMatchFilter::setProperties(const QProperties& properties)
{
	QVariant var;
	var=properties.property("AcceptOnMatch");
	if (var.isValid())
	{
		m_bAcceptOnMatch=var.toBool();
	}

	var=properties.property("LogLevel");
	if (var.isValid())
	{
		m_logLevelToMatch=var.toInt();
	}
}

QFilter::FilterResult QLogLevelMatchFilter::decide(const QLoggingEvent& event) const
{
	if (m_bAcceptOnMatch)
	{
		if(m_logLevelToMatch == NOT_SET_LOG_LEVEL)
		{
			return DENY;
		}

		return m_logLevelToMatch == event.level()? ACCEPT : DENY;
	}
	else
	{
		return NEUTRAL;
	}
}

Q_EXPORT_REGISTER_FILTER_FACTORY(QLogLevelMatchFilter, QLogLevelMatchFilterFactory)

QLogLevelRangeFilter::QLogLevelRangeFilter()
{
	init();
}

QLogLevelRangeFilter::QLogLevelRangeFilter(const QProperties& properties)
{
	init();
	setProperties(properties);
}

QLogLevelRangeFilter::~QLogLevelRangeFilter()
{

}

void QLogLevelRangeFilter::init()
{
	m_bAcceptOnMatch = true;
	m_logLevelMin = NOT_SET_LOG_LEVEL;
	m_logLevelMax = NOT_SET_LOG_LEVEL;
}

void QLogLevelRangeFilter::setProperties(const QProperties& properties)
{
	QVariant var;
	var=properties.property("AcceptOnMatch");
	if (var.isValid())
	{
		m_bAcceptOnMatch=var.toBool();
	}

	var=properties.property("LogLevelMin");
	if (var.isValid())
	{
		m_logLevelMin=var.toInt();
	}

	var=properties.property("LogLevelMax");
	if (var.isValid())
	{
		m_logLevelMax=var.toInt();
	}
}

QFilter::FilterResult QLogLevelRangeFilter::decide(const QLoggingEvent& event) const
{
	if(m_bAcceptOnMatch)
	{
		if((m_logLevelMin != NOT_SET_LOG_LEVEL) && (event.level() < m_logLevelMin))
		{
			return DENY;
		}

		if((m_logLevelMax != NOT_SET_LOG_LEVEL) && (event.level() > m_logLevelMax))
		{
			return DENY;
		}

		return ACCEPT;
	}
	else
	{
		return NEUTRAL;
	}
}

Q_EXPORT_REGISTER_FILTER_FACTORY(QLogLevelRangeFilter, QLogLevelRangeFilterFactory)

QStringMatchFilter::QStringMatchFilter()
{
	init();
}

QStringMatchFilter::QStringMatchFilter(const QProperties& properties)
{
	init();
	setProperties(properties);
}

QStringMatchFilter::~QStringMatchFilter()
{

}

void QStringMatchFilter::init()
{
	m_bAcceptOnMatch = true;
}

void QStringMatchFilter::setProperties(const QProperties& properties)
{
	QVariant var;
	var=properties.property("AcceptOnMatch");
	if (var.isValid())
	{
		m_bAcceptOnMatch=var.toBool();
	}

	var=properties.property("StringToMatch");
	if (var.isValid())
	{
		m_stringToMatch=var.toString();
	}
}

QFilter::FilterResult QStringMatchFilter::decide(const QLoggingEvent& event) const
{
	if (m_bAcceptOnMatch)
	{
		const QString& message = event.message();

		if(m_stringToMatch.isEmpty() || message.isEmpty())
		{
			return DENY;
		}

		return message.contains(m_stringToMatch, Qt::CaseInsensitive)? ACCEPT:DENY;
	}
	else
	{
		return NEUTRAL;
	}
}

Q_EXPORT_REGISTER_FILTER_FACTORY(QStringMatchFilter, QStringMatchFilterFactory)
