#include "qappender.h"

#include "common.h"
#include "qloglayoutfactory.h"

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
		loglog()->error(err);
		m_bFirstTime = false;
	}
}

void QOnlyOnceErrorHandler::reset()
{
	m_bFirstTime = true;
}

QAppender::QAppender()
:m_strName(""),
m_threshold(NOT_SET_LOG_LEVEL),
m_pErrorHandler(new QOnlyOnceErrorHandler()),
m_bClosed(false)
{
	QLogLayoutFactoryBasePtr pFactory=QLogLayoutFactoryRegistry::instance()->get("QSimpleLogLayout");
	if (pFactory != NULL)
	{
		m_pLayout=pFactory->createObject(QProperties());
	}
}

QAppender::QAppender(const QProperties& properties)
:m_strName(""),
m_threshold(NOT_SET_LOG_LEVEL),
m_pErrorHandler(new QOnlyOnceErrorHandler()),
m_bClosed(false)
{
	QLogLayoutFactoryBasePtr pFactory=QLogLayoutFactoryRegistry::instance()->get("QSimpleLogLayout");
	if (pFactory != NULL)
	{
		m_pLayout=pFactory->createObject(QProperties());
	}

	setProperties(properties);
}

QAppender::~QAppender()
{

}

void QAppender::setProperties(const QProperties& properties)
{
	const QString CONST_STR_LAYOUT="Layout";
	const QString CONST_STR_FILTER="Filter";

	if(properties.exists(CONST_STR_LAYOUT))
	{
		QString factoryName = properties.property(CONST_STR_LAYOUT).toString();
		QLogLayoutFactoryBasePtr pFactory = QLogLayoutFactoryRegistry::instance()->get(factoryName);
		if(pFactory == NULL)
		{
			loglog()->error("Cannot find LayoutFactory: \""+ factoryName+ "\"");
			return;
		}

		QProperties layoutProperties =properties.propertySubset(CONST_STR_LAYOUT);
		try
		{
			QLogLayoutPtr pLayout(pFactory->createObject(layoutProperties));
			if(pLayout == 0)
			{
				loglog()->error("Failed to create appender: %s", factoryName.toLocal8Bit().data());
			}
			else
			{
				m_pLayout = pLayout;
			}
		}
		catch(std::exception& e)
		{
			loglog()->error("Error while creating Layout: %s", e.what());
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
	QStringList filters = properties.property("Filters").toString().split(QChar(' '), QString::SkipEmptyParts);
	QFilterPtr pFilterChain;
	QString filterName, factoryName;
	foreach (QString filter, filters)
	{
		QProperties factoryProperties =properties.propertySubset(filter);

		factoryName = factoryProperties.property(CONST_STR_FILTER).toString();
		QFilterFactoryBasePtr pFactory = QFilterFactoryRegistry::instance()->get(factoryName);

		if(pFactory == 0)
		{
			loglog()->error("Appender::ctor()- Cannot find FilterFactory: " + factoryName);
			continue;
		}

		QProperties filterProperties =factoryProperties.propertySubset(CONST_STR_FILTER);
		QFilterPtr pFilter = pFactory->createObject(filterProperties);
		if(pFilter == 0)
		{
			loglog()->error("Appender::ctor()- Failed to create filter: " + filterName);
		}

		if(pFilterChain == 0)
		{
			pFilterChain = pFilter;
		}
		else
		{
			pFilterChain->appendFilter(pFilter);
		}
	}

	setFilter(pFilterChain);
}

void QAppender::destroy()
{
	// An appender might be closed then destroyed. There is no
	// point in closing twice.
	if(m_bClosed)
	{
		return;
	}

	loglog()->debug("Destroying appender named [%s]...", m_strName.toLocal8Bit().data());

	close();
	m_bClosed= true;
}

void QAppender::doAppend(const QLoggingEvent& event)
{
	QMutexLocker locker(&m_mutex);
	if(m_bClosed)
	{
		loglog()->error("Attempted to append to closed appender named [%s].", m_strName.toLocal8Bit().data());
		return;
	}

	if(!isAsSevereAsThreshold(event.level()))
	{
		return;
	}

	if(QFilter::checkFilter(m_pFilter.get(), event) == QFilter::DENY)
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

const QErrorHandlerPtr& QAppender::errorHandler() const
{
	return m_pErrorHandler;
}

void QAppender::setErrorHandler(const QErrorHandlerPtr& eh)
{
	if(eh == NULL)
	{
		loglog()->warning("You have tried to set a null error-handler.");
		return;
	}

	QMutexLocker locker(&m_mutex);
	m_pErrorHandler = eh;
}

void QAppender::setLayout(const QLogLayoutPtr& layout)
{
	QMutexLocker locker(&m_mutex);
	m_pLayout=layout;
}

const QLogLayoutPtr& QAppender::layout() const
{
	return m_pLayout;
}
