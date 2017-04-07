#include "qmibsystem.h"

QMibSystem::QMibSystem()
:QAbstractMib(-1)
{

}

QMibSystem::~QMibSystem()
{

}

void QMibSystem::setResult(const QVariant& var)
{
	QVector<OperatingSystem> systems=var.value<QVector<OperatingSystem> >();
	if (!systems.isEmpty())
	{
		m_system=systems[0];
	}
}

void QMibSystem::setResult(const QByteArray& buffer)
{
	Q_READ_ONLY_DATA_STREAM(out, buffer);

	out>>m_system;
}

REGISTER_MIB_INSTANCE(QMibSystem)
