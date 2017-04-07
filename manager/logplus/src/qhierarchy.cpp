#include "qhierarchy.h"

#include "qloggerimplement.h"

const LogLevel QHierarchy::DISABLE_OFF = -1;
const LogLevel QHierarchy::DISABLE_OVERRIDE = -2;

QHierarchy::QHierarchy()
:m_defaultFactory(new QDefaultLoggerFactory()),
m_root(NULL),
m_nDisableValue(DISABLE_OFF),
m_bEmittedNoAppenderWarning(false),
m_bEmittedNoResourceBundleWarning(false)
{
	m_root = QLogger(new QRootLogger(*this, DEBUG_LOG_LEVEL));
}

QHierarchy::~QHierarchy()
{
	shutdown();
}

void QHierarchy::clear() 
{
	QMutexLocker locker(&m_mutex);

	m_provisionNodes.clear();
	m_loggerPtrs.clear();
}

bool QHierarchy::exists(const QString& name)
{
	QMutexLocker locker(&m_mutex);

	return m_loggerPtrs.contains(name);
}

void QHierarchy::disable(const QString& logLevel)
{
	if(m_nDisableValue != DISABLE_OVERRIDE)
	{
		m_nDisableValue = QLogLevelManager::instance().get()->fromString(logLevel);
	}
}

void QHierarchy::disable(LogLevel logLevel) 
{
	if(m_nDisableValue != DISABLE_OVERRIDE)
	{
		m_nDisableValue = logLevel;
	}
}

void QHierarchy::disableAll() 
{ 
	disable(FATAL_LOG_LEVEL);
}

void QHierarchy::disableDebug() 
{ 
	disable(DEBUG_LOG_LEVEL);
}

void QHierarchy::disableInfo() 
{ 
	disable(INFO_LOG_LEVEL);
}

void QHierarchy::enableAll() 
{ 
	m_nDisableValue = DISABLE_OFF; 
}

QLogger& QHierarchy::loggerInstance(const QString& name)
{
	return loggerInstance(name, *m_defaultFactory); 
}

QLogger& QHierarchy::loggerInstance(const QString& name, QLoggerFactoryInterface& factory)
{
	QMutexLocker locker(&m_mutex);

	return loggerImplementInstance(name, factory);
}

QLoggerList QHierarchy::currentLoggers()
{
	QLoggerList ret;

	QMutexLocker locker(&m_mutex);
	initializeLoggerList(ret);

	return ret;
}

bool QHierarchy::isDisabled(int level) 
{ 
	return m_nDisableValue >= level; 
}

QLogger& QHierarchy::root()
{ 
	return m_root;
}

void QHierarchy::resetConfiguration()
{
	root().setLogLevel(DEBUG_LOG_LEVEL);
	m_nDisableValue = DISABLE_OFF;

	shutdown();

	QLoggerList loggers = currentLoggers();
	for (int i=0; i<loggers.size(); i++)
	{
		loggers[i].setLogLevel(NOT_SET_LOG_LEVEL);
		loggers[i].setAdditivity(true);
	}
}

void QHierarchy::setLoggerFactory(std::auto_ptr<QLoggerFactoryInterface> factory) 
{ 
	m_defaultFactory = factory; 
}

void QHierarchy::shutdown()
{
	QLoggerList loggers = currentLoggers();

	m_root.closeNestedAppenders();
	m_root.removeAllAppenders();

	for (int i=0; i<loggers.size(); i++)
	{
		loggers[i].closeNestedAppenders();
		loggers[i].removeAllAppenders();
	}
}

QLogger& QHierarchy::loggerImplementInstance(const QString& name, QLoggerFactoryInterface& factory)
{
	if (!m_loggerPtrs.contains(name))
	{
		// Need to create a new logger
		QLogger logger = factory.makeNewLoggerInstance(name, *this);
		m_loggerPtrs.insert(name, logger);

		if (m_provisionNodes.contains(name))
		{
			updateChildren(m_provisionNodes[name], logger);
			m_provisionNodes.remove(name);
		}

		updateParents(logger);
	}

	return m_loggerPtrs[name];
}

void QHierarchy::initializeLoggerList(QLoggerList& list) const
{
	QLoggerMapIterator it(m_loggerPtrs);
	while (it.hasNext())
	{
		it.next();

		list.append(it.value());
	}
}

void QHierarchy::updateParents(QLogger logger)
{
	QString name = logger.name();
	bool bParentFound = false;

	// if name = "w.x.y.z", loop thourgh "w.x.y", "w.x" and "w", but not "w.x.y.z"
	int index=name.length();
	QString strSub;
	while ((index=name.lastIndexOf(QChar('.'), index-1)) != -1)
	{
		strSub=name.left(index);

		if (m_loggerPtrs.contains(strSub))
		{
			bParentFound=true;
			logger.m_loggerImplement->m_parent = m_loggerPtrs[strSub].m_loggerImplement;

			break;
		}
		else
		{
			if (m_provisionNodes.contains(strSub))
			{
				m_provisionNodes[strSub].append(logger);
			}
			else
			{
				QProvisionNode node;
				node.append(logger);
				m_provisionNodes.insert(strSub, node);
			}
		}
	}

	if(!bParentFound)
	{
		logger.m_loggerImplement->m_parent = m_root.m_loggerImplement;
	}
}


void QHierarchy::updateChildren(QProvisionNode& pn,QLogger logger)
{
	for (int i=0; i<pn.size(); i++)
	{
		if (!pn[i].m_loggerImplement->m_parent->name().startsWith(logger.name()))
		{
			logger.m_loggerImplement->m_parent=pn[i].m_loggerImplement->m_parent;
			pn[i].m_loggerImplement->m_parent=logger.m_loggerImplement;
		}
	}
}
