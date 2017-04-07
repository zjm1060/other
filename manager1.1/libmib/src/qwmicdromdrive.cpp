#include "qwmicdromdrive.h"

QWmiCDROMDrive::QWmiCDROMDrive(QObject *parent)
: QObject(parent)
{
}

QWmiCDROMDrive::~QWmiCDROMDrive()
{
}

void QWmiCDROMDrive::beferQuery() 
{
	m_vecCDROMDrive.clear(); 
}

QStringList QWmiCDROMDrive::query()
{
	QStringList listQuery;
	listQuery<<"SELECT * FROM Win32_CDROMDrive";

	return listQuery;
}

void QWmiCDROMDrive::record(IWbemClassObject* object, IWbemServices* service, int index)
{
	Q_UNUSED(service);
	Q_UNUSED(index);

	Q_ASSERT_NO_NULL(object);

	CDROMDrive drive;

	drive.os=OS_WINDOWS;

	drive.configManager=findUInt32(object, L"ConfigManagerErrorCode");
	drive.description=findString(object, L"Description");
	drive.deviceID=findString(object, L"DeviceID");
	drive.drive=findString(object, L"Drive");
	drive.manufacturer=findString(object, L"Manufacturer");
	drive.mediaType=findString(object, L"MediaType");
	drive.numberOfMediaSupported=findUInt32(object, L"NumberOfMediaSupported");
	drive.size=findUInt64(object, L"Size");
	drive.SCSIBus=findUInt32(object, L"SCSIBus");
	drive.SCSILogicalUnit=findUInt32(object, L"SCSILogicalUnit");
	drive.SCSIPort=findUInt16(object, L"SCSIPort");
	drive.SCSITargetId=findUInt16(object, L"SCSITargetId");
	drive.transferRate=findReal(object, L"TransferRate");
	drive.volumeName=findString(object, L"VolumeName");
	drive.volumeSerialNumber=findString(object, L"VolumeSerialNumber");
	drive.name=findString(object, L"Name");

	m_vecCDROMDrive.append(drive);
}

REGISTER_WMI_INSTANCE(QWmiCDROMDrive)
