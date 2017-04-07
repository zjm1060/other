#include "qfcextern.h"

QFcRegisterInstances* QFcRegisterInstances::instance()
{
	static QFcRegisterInstances* g_pInstances=new QFcRegisterInstances;

	return g_pInstances;
}

void QFcRegisterInstances::addInterface(const QAbstractFileComparePtr& ptr)
{
	if (ptr != NULL)
	{
		m_instances.append(ptr);
	}
}

const QVector<QAbstractFileComparePtr>& QFcRegisterInstances::interfaces() const
{
	return m_instances;
}

void  CreateFileCompareInstance(QVector<QAbstractFileComparePtr>& instances)
{
	instances=QVector<QAbstractFileComparePtr>(QFcRegisterInstances::instance()->interfaces());
}

