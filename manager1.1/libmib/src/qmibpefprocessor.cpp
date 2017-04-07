#include "qmibpefprocessor.h"

QMibPefProcessor::QMibPefProcessor()
:QAbstractMib(1000)
{

}

QMibPefProcessor::~QMibPefProcessor()
{

}

void QMibPefProcessor::setResult(const QVariant& var)
{
	if (!var.isValid())
	{
		return;
	}

	m_perfProcessor=var.value<PerfProcessor>();
}

void QMibPefProcessor::setResult(const QByteArray& buffer)
{
	Q_READ_ONLY_DATA_STREAM(out, buffer);

	out>>m_perfProcessor;
}

REGISTER_MIB_INSTANCE(QMibPefProcessor)
