#include "qappender.h"

#include "qlogextern.h"

QErrorHandler::QErrorHandler()
{
}

QErrorHandler::~QErrorHandler()
{
}


void QOnlyOnceErrorHandler::error(const QString& err)
{
	if(m_bFirstTime)
	{
		logLog().error(err);
		m_bFirstTime = false;
	}
}

void QOnlyOnceErrorHandler::reset()
{
	m_bFirstTime = true;
}

QAppender::QAppender()
:m_layout(new QSimpleLogLayout()),
m_strName(""),
m_threshold(NOT_SET_LOG_LEVEL),
m_errorHandler(new QOnlyOnceErrorHandler()),
m_bClosed(false)
{

}

QAppender::QAppender(const QProperties& properties)
:m_layout(new QSimpleLogLayout()),
m_strName(""),
m_threshold(NOT_SET_LOG_LEVEL),
m_errorHandler(new QOnlyOnceErrorHandler()),
m_bClosed(false)
{
	setProperties(properties);
}

QAppender::~QAppender()
{

}

void QAppender::setProperties(const QProperties& properties)
{
	if(properties.exists("Layout"))
	{
		QString factoryName = properties.property("Layout").toString();
		QLayoutFactory* factory = QLogLayoutFactoryRegistry::instance()->get(factoryName);
		if(factory == 0)
		{
			logLog().error("Cannot find LayoutFactory: \""+ factoryName+ "\"");
			return;
		}

		QProperties layoutProperties =properties.propertySubset("layout.");
		try
		{
			std::auto_ptr<QLogLayout> newLayout(factory->createObject(layoutProperties).get());
			if(newLayout.get() == 0)
			{
				logLog().error("Failed to create appender: "+ factoryName);
			}
			else
			{
				m_layout = newLayout;
			}
		}
		catch(std::exception& e)
		{
			logLog().error("Error while creating Layout: "+ QString(e.what()));
			return;
		}

	}

	// Support for appender.Threshold in properties configuration file
	if(properties.exists("Threshold"))
	{
		QString tmp = properties.property("Threshold").toString().toUpper();
		m_threshold = QLogLevelManager::instance()->fromString(tmp);
	}

	// Configure the filters
	QProperties filterProps = properties.propertySubset("filters.");
	int filterCount = 0;
	QFilterPtr filterChain;
	QString filterName, factoryName;
	while( filterProps.exists(filterName = QString("%d").arg(++filterCount)) )
	{
		factoryName = filterProps.property(filterName).toString();
		QFilterFactory* factory = QFilterFactoryRegistry::instance()->get(factoryName);

		if(factory == 0)
		{
			logLog().error("Appender::ctor()- Cannot find FilterFactory: " + factoryName);
			continue;
		}

		QFilterPtr filter = QFilterPtr(factory->createObject(filterProps.propertySubset(filterName + ".")).get());
		if(filter.get() == 0)
		{
			logLog().error("Appender::ctor()- Failed to create filter: " + filterName);
		}

		if(filterChain.get() == 0)
		{
			filterChain = filter;
		}
		else
		{
			filterChain->appendFilter(filter);
		}
	}
	setFilter(filterChain);
}

void QAppender::destroy()
{
	logLog().debug("Destroying appender named ["+ m_strName+ "].");

	// An appender might be closed then destroyed. There is no
	// point in closing twice.
	if(m_bClosed)
	{
		return;
	}

	close();
	m_bClosed= true;
}

void QAppender::doAppend(const QLoggingEvent& event)
{
	QMutexLocker locker(&m_mutex);
	if(m_bClosed)
	{
		logLog().error("Attempted to append to closed appender named ["+ m_strName+ "].");
		return;
	}

	if(!isAsSevereAsThreshold(event.level()))
	{
		return;
	}

	if(QFilter::checkFilter(m_filter.get(), event) == QFilter::DENY)
	{
		return;
	}

	append(event);
}

QString QAppender::name()
{
	return m_strName;
}



void QAppender::setName(const QString& name)
{
	m_strName = name;
}

QErrorHandler* QAppender::errorHandler()
{
	return m_errorHandler.get();
}



void QAppender::setErrorHandler(std::auto_ptr<QErrorHandler> eh)
{
	if(eh.get() == NULL)
	{
		logLog().warn("You have tried to set a null error-handler.");
		return;
	}

	QMutexLocker locker(&m_mutex);
	m_errorHandler = eh;
}

void QAppender::setLayout(std::auto_ptr<QLogLayout> layout)
{
	QMutexLocker locker(&m_mutex);
	m_layout=layout;
}

QLogLayout* QAppender::layout()
{
	return m_layout.get();
}
