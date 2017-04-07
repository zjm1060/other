#include "qmibsystemmemory.h"

QMibSystemMemory::QMibSystemMemory()
:QAbstractMib(1000)
{

}

QMibSystemMemory::~QMibSystemMemory()
{

}

void QMibSystemMemory::setResult(const QVariant& var)
{
	if (!var.isValid())
	{
		return;
	}

	m_memory=var.value<SystemMemory>();
}

void QMibSystemMemory::setResult(const QByteArray& buffer)
{
	Q_READ_ONLY_DATA_STREAM(out, buffer);

	out>>m_memory;
}

REGISTER_MIB_INSTANCE(QMibSystemMemory)
