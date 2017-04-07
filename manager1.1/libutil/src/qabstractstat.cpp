#include "qabstractstat.h"

QAbstractStat::QAbstractStat()
{
}

QAbstractStat::~QAbstractStat()
{

}

QVariant QAbstractStat::exec()
{
	QMutexLocker locker(&m_mutex);

	beferQuery();

	query();

	afterQuery();

	return result();
}

void QAbstractStat::beferQuery()
{

}

void QAbstractStat::afterQuery()
{

}

void QAbstractStat::init()
{

}

void QAbstractStat::clear()
{

}

int QAbstractStat::execMethod(int method, const QStringList& arguments)
{
	Q_UNUSED(method);
	Q_UNUSED(arguments);

	return ExecMethodSucceed;
}
