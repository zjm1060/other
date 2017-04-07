#include "qmibprocessor.h"

QMibProcessor::QMibProcessor()
:QAbstractMib(-1)
{

}

QMibProcessor::~QMibProcessor()
{

}

void QMibProcessor::setResult(const QVariant& var)
{
	if (!var.isValid())
	{
		return;
	}

	m_processors=var.value<QVector<ProcessorInfo> >();
}

void QMibProcessor::setResult(const QByteArray& buffer)
{
	Q_READ_ONLY_DATA_STREAM(out, buffer);

	QVector<ProcessorInfo> processors;
	out>>processors;

	m_processors=processors;
}

REGISTER_MIB_INSTANCE(QMibProcessor)
