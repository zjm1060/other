#include "qfilterfactory.h"

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
		if(result == DENY)
		{
			return result;
		}

		currentFilter = currentFilter->next.get();
	}

	return ACCEPT;
}

void QFilter::appendFilter(QFilterPtr filter)
{
	if(next == 0)
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

QFilterFactoryBase::QFilterFactoryBase()
{

}

QFilterFactoryBase::~QFilterFactoryBase()
{

}

QSharedObjectPtr<QFilterFactoryRegistry > qFilterFactoryRegistryInstance()
{
	return QFilterFactoryRegistry::instance();
}
