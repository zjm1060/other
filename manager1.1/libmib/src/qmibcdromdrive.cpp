#include "qmibcdromdrive.h"

QMibCDRomDrive::QMibCDRomDrive()
:QAbstractMib(-1)
{

}

QMibCDRomDrive::~QMibCDRomDrive()
{

}

void QMibCDRomDrive::setResult(const QVariant& var)
{
	if (!var.isValid())
	{
		return;
	}

	QVector<CDROMDrive> drives=var.value<QVector<CDROMDrive> >();

	m_dirves=drives;
}

void QMibCDRomDrive::setResult(const QByteArray& buffer)
{
	Q_READ_ONLY_DATA_STREAM(out, buffer);

	QVector<CDROMDrive> drives;
	out>>drives;

	m_dirves=drives;
}

REGISTER_MIB_INSTANCE(QMibCDRomDrive)
