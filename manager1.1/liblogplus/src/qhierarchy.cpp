#include "qhierarchy.h"

#include "qloggerimplement.h"

const LogLevel QHierarchy::DISABLE_OFF = -1;
const LogLevel QHierarchy::DISABLE_OVERRIDE = -2;

QHierarchy::QHierarchy()
:m_pDefaultFactory(new QDefaultLoggerFactory),
m_nDisableValue(DISABLE_OFF),
m_bEmittedNoAppenderWarning(false),
m_bEmittedNoResourceBundleWarning(false)
{
	QLoggerImplementPtr imp(new QRootLogger(QHierarchyPtr((QHierarchy*)this), DEBUG_LOG_LEVEL));
	m_pRoot = new QLogger(imp);
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
		m_nDisableValue = QLogLevelManager::instance()->fromString(logLevel);
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

QLoggerPtr QHierarchy::loggerInstance(const QString& name)
{
	return loggerInstance(name, m_pDefaultFactory); 
}

QLoggerPtr QHierarchy::loggerInstance(const QString& name, const QLoggerFactoryInterfacePtr& factory)
{
	QMutexLocker locker(&m_mutex);

	return loggerImplementInstance(name, factory);
}

QLoggerPtrList QHierarchy::currentLoggers()
{
	QLoggerPtrList ret;

	initializeLoggerList(ret);

	return ret;
}

bool QHierarchy::isDisabled(int level) 
{ 
	return m_nDisableValue >= level; 
}

QLoggerPtr& QHierarchy::root()
{ 
	return m_pRoot;
}

void QHierarchy::resetConfiguration()
{
	root()->setLogLevel(DEBUG_LOG_LEVEL);
	m_nDisableValue = DISABLE_OFF;

	shutdown();

	QLoggerPtrList loggers = currentLoggers();
	for (int i=0; i<loggers.size(); i++)
	{
		if (loggers[i] != NULL)
		{
			loggers[i]->setLogLevel(NOT_SET_LOG_LEVEL);
			loggers[i]->setAdditivity(true);
		}
	}
}

void QHierarchy::setLoggerFactory(const QLoggerFactoryInterfacePtr& factory) 
{ 
	m_pDefaultFactory = factory; 
}

void QHierarchy::shutdown()
{
	if (m_pRoot != NULL)
	{
		m_pRoot->closeNestedAppenders();
		m_pRoot->removeAllAppenders();
	}

	QLoggerPtrList loggers = currentLoggers();

	for (int i=0; i<loggers.size(); i++)
	{
		if (loggers[i] != NULL)
		{
			loggers[i]->closeNestedAppenders();
			loggers[i]->removeAllAppenders();
		}
	}
}

QLoggerPtr& QHierarchy::loggerImplementInstance(const QString& name, const QLoggerFactoryInterfacePtr& factory)
{
	Q_ASSERT_NO_NULL(factory);

	if (!m_loggerPtrs.contains(name))
	{
		// Need to create a new logger
		QLoggerPtr pLogger = factory->makeNewLoggerInstance(name, QHierarchyPtr((QHierarchy*)this));
		m_loggerPtrs.insert(name, pLogger);

		if (m_provisionNodes.contains(name))
		{
			updateChildren(m_provisionNodes[name], pLogger);
			m_provisionNodes.remove(name);
		}

		updateParents(pLogger);
	}

	return m_loggerPtrs[name];
}

void QHierarchy::initializeLoggerList(QLoggerPtrList& list) const
{
	QLoggerPtrMapIterator it(m_loggerPtrs);
	while (it.hasNext())
	{
		it.next();

		QLoggerPtr pLogger=it.value();
		list.append(pLogger);
	}
}

void QHierarchy::updateParents(const QLoggerPtr& logger)
{
	if (logger == NULL)
	{
		return;
	}

	QString name = logger->name();
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
			if (logger->m_pLoggerImplement != NULL)
			{
				logger->m_pLoggerImplement->m_pParent= m_loggerPtrs[strSub]->m_pLoggerImplement;
			}

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

	if(!bParentFound && logger->m_pLoggerImplement !=NULL && m_pRoot != NULL)
	{
		logger->m_pLoggerImplement->m_pParent = m_pRoot->m_pLoggerImplement;
	}
}


void QHierarchy::updateChildren(QProvisionNode& pn, const QLoggerPtr& logger)
{
	if (logger == NULL)
	{
		return;
	}

	for (int i=0; i<pn.size(); i++)
	{
		if (pn[i] != NULL && logger->m_pLoggerImplement != NULL && pn[i]->m_pLoggerImplement->m_pParent != NULL
			&& !pn[i]->m_pLoggerImplement->m_pParent->name().startsWith(logger->name()))
		{
			logger->m_pLoggerImplement->m_pParent=pn[i]->m_pLoggerImplement->m_pParent;
			pn[i]->m_pLoggerImplement->m_pParent=logger->m_pLoggerImplement;
		}
	}
}
