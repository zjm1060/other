#include "qmibcomputersystem.h"

QMibComputerSystem::QMibComputerSystem()
:QAbstractMib(-1)
{

}

QMibComputerSystem::~QMibComputerSystem()
{

}

void QMibComputerSystem::setResult(const QVariant& var)
{
	m_system=var.value<ComputerSystem>();
}

void QMibComputerSystem::setResult(const QByteArray& buffer)
{
	Q_READ_ONLY_DATA_STREAM(out, buffer);

	out>>m_system;
}

REGISTER_MIB_INSTANCE(QMibComputerSystem)
