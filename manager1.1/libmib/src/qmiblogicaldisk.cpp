#include "qmiblogicaldisk.h"

#include "common.h"
#include "qalarmfactory.h"

QMibLogicalDisk::QMibLogicalDisk()
:QAbstractMib(1000)
{

}

QMibLogicalDisk::~QMibLogicalDisk()
{

}

void QMibLogicalDisk::setResult(const QVariant& var)
{
	if (!var.isValid())
	{
		return;
	}

	QVector<LogicalDisk> disks=var.value<QVector<LogicalDisk> >();

	m_disks=disks;
}

void QMibLogicalDisk::setResult(const QByteArray& buffer)
{
	Q_READ_ONLY_DATA_STREAM(out, buffer);

	QVector<LogicalDisk> disks;
	out>>disks;

	m_disks=disks;

	QAlarmFactory::instance()->alarm(qVariantFromValue(disks));
}

REGISTER_MIB_INSTANCE(QMibLogicalDisk)
