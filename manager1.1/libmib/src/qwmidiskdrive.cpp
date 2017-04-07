#include "qwmidiskdrive.h"

QWmiDiskDrive::QWmiDiskDrive(QObject *parent)
: QObject(parent)
{
}

QWmiDiskDrive::~QWmiDiskDrive()
{
}

void QWmiDiskDrive::beferQuery() 
{
	m_vecDiskDrive.clear(); 
}

QStringList QWmiDiskDrive::query()
{
	QStringList listQuery;
	listQuery<<"SELECT * FROM Win32_DiskDrive";

	return listQuery;
}

void QWmiDiskDrive::record(IWbemClassObject* object, IWbemServices* service, int index)
{
	Q_UNUSED(service);
	Q_UNUSED(index);

	Q_ASSERT_NO_NULL(object);

	DiskDrive diskDrive;

	diskDrive.os=OS_WINDOWS;
	diskDrive.configManager=findUInt32(object, L"ConfigManagerErrorCode");
	diskDrive.description=findString(object, L"Description");
	diskDrive.deviceID=findString(object, L"DeviceID");
	diskDrive.interfaceType=findString(object, L"InterfaceType");
	diskDrive.manufacturer=findString(object, L"Manufacturer");
	diskDrive.mediaType=findString(object, L"MediaType");
	diskDrive.model=findString(object, L"Model");
	diskDrive.numberOfMediaSupported=findUInt32(object, L"NumberOfMediaSupported");
	diskDrive.partitions=findUInt32(object, L"Partitions");
	diskDrive.size=findUInt64(object, L"Size");
	diskDrive.SCSIBus=findUInt32(object, L"SCSIBus");
	diskDrive.SCSILogicalUnit=findUInt16(object, L"SCSILogicalUnit");
	diskDrive.SCSIPort=findUInt16(object, L"SCSIPort");
	diskDrive.SCSITargetId=findUInt32(object, L"SCSITargetId");

	m_vecDiskDrive.append(diskDrive);
}

REGISTER_WMI_INSTANCE(QWmiDiskDrive)

