#include "qstatfactory.h"

#include "define.h"

QMutex QStatFactory::m_mutex;
QStatFactory* QStatFactory::m_pInstance=0;

QStatFactory::QStatFactory()
{
}

QStatFactory::~QStatFactory()
{
	foreach (QAbstractStat* pStatistics, m_mapStatistics)
	{
		SAFE_DELETE_CLASS(pStatistics);
	}
}

QStatFactory* QStatFactory::instance()
{
	if (m_pInstance == 0)
	{
		m_mutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=new QStatFactory;
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

void QStatFactory::registerStat(QAbstractStat* statistics)
{
	Q_ASSERT_NO_NULL(statistics);

	if (!m_mapStatistics.contains(statistics->type()))
	{
		m_mapStatistics.insert(statistics->type(), statistics);
	}
}

QAbstractStat* QStatFactory::statInstance(int type)
{
	return m_mapStatistics.value(type, NULL);
}

QVariant QStatFactory::exec(int type)
{
	QAbstractStat* pStatistics=m_mapStatistics.value(type);
	if (pStatistics != NULL)
	{
		return pStatistics->exec();
	}

	return NULL;
}

int QStatFactory::execMethod(int type, int method, const QStringList& arguments)
{
	QAbstractStat* pStatistics=m_mapStatistics.value(type);
	if (pStatistics != NULL)
	{
		return pStatistics->execMethod(method, arguments);
	}

	return QAbstractStat::ExecMethodInvalid;
}
