#include "qmibdiskdrive.h"

QMibDiskDrive::QMibDiskDrive()
:QAbstractMib(-1)
{

}

QMibDiskDrive::~QMibDiskDrive()
{

}

void QMibDiskDrive::setResult(const QVariant& var)
{
	if (!var.isValid())
	{
		return;
	}

	QVector<DiskDrive> drives=var.value<QVector<DiskDrive> >();

	m_dirves=drives;
}

void QMibDiskDrive::setResult(const QByteArray& buffer)
{
	Q_READ_ONLY_DATA_STREAM(out, buffer);

	QVector<DiskDrive> drives;
	out>>drives;

	m_dirves=drives;
}

REGISTER_MIB_INSTANCE(QMibDiskDrive)
