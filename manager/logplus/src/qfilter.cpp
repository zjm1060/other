#include "qfilter.h"

QFilter::QFilter()
{

}

QFilter::~QFilter()
{

}

QFilter::FilterResult QFilter::checkFilter(const QFilter* filter, const QLoggingEvent& event)
{
	const QFilter* currentFilter = filter;
	while(currentFilter != NULL)
	{
		FilterResult result = currentFilter->decide(event);
		if(result != NEUTRAL)
		{
			return result;
		}

		currentFilter = currentFilter->next.get();
	}

	return ACCEPT;
}

void QFilter::appendFilter(QFilterPtr filter)
{
	if(next.get() == 0)
	{
		next = filter;
	}
	else
	{
		next->appendFilter(filter);
	}
}

void QFilter::setProperties(const QProperties& properties)
{
	Q_UNUSED(properties);
}

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
	if(m_logLevelToMatch == NOT_SET_LOG_LEVEL)
	{
		return NEUTRAL;
	}

	bool matchOccured = (m_logLevelToMatch == event.level());

	if(matchOccured)
	{
		return (m_bAcceptOnMatch ? ACCEPT : DENY);
	}
	else
	{
		return NEUTRAL;
	}
}

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
	if((m_logLevelMin != NOT_SET_LOG_LEVEL) && (event.level() < m_logLevelMin))
	{
		return DENY;
	}

	if((m_logLevelMax != NOT_SET_LOG_LEVEL) && (event.level() > m_logLevelMax))
	{
		return DENY;
	}

	if(m_bAcceptOnMatch)
	{
		return ACCEPT;
	}
	else
	{
		return NEUTRAL;
	}
}


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
	const QString& message = event.message();

	if(m_stringToMatch.isEmpty() || message.isEmpty())
	{
		return NEUTRAL;
	}

	if(message.contains(m_stringToMatch, Qt::CaseInsensitive))
	{
		return NEUTRAL;
	}
	else
	{
		return (m_bAcceptOnMatch ? ACCEPT : DENY);
	}
}
