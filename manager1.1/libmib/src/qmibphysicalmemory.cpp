#include "qmibphysicalmemory.h"

QMibPhysicalMemorhy::QMibPhysicalMemorhy()
:QAbstractMib(-1)
{

}

QMibPhysicalMemorhy::~QMibPhysicalMemorhy()
{

}

void QMibPhysicalMemorhy::setResult(const QVariant& var)
{
	if (!var.isValid())
	{
		return;
	}

	m_memories=var.value<QVector<PhysicalMemory> >();
}

void QMibPhysicalMemorhy::setResult(const QByteArray& buffer)
{
	Q_READ_ONLY_DATA_STREAM(out, buffer);

	QVector<PhysicalMemory> memories;
	out>>memories;

	m_memories=memories;
}

REGISTER_MIB_INSTANCE(QMibPhysicalMemorhy)
