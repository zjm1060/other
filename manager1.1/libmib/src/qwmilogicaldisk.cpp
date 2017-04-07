#include "qwmilogicaldisk.h"

QWmiLogicalDisk::QWmiLogicalDisk(QObject *parent)
: QObject(parent)
{
}

QWmiLogicalDisk::~QWmiLogicalDisk()
{
}

void QWmiLogicalDisk::beferQuery() 
{
	m_vecLogicalDisk.clear(); 
}

QStringList QWmiLogicalDisk::query()
{
	QStringList listQuery;
	listQuery<<"SELECT * FROM Win32_LogicalDisk";

	return listQuery;
}

void QWmiLogicalDisk::record(IWbemClassObject* object, IWbemServices* service, int index)
{
	Q_UNUSED(service);
	Q_UNUSED(index);

	Q_ASSERT_NO_NULL(object);

	LogicalDisk disk;

	disk.description=findString(object, L"Description");
	disk.deviceID=findString(object, L"DeviceID");
	disk.driveType=findUInt32(object, L"DriveType");

	switch (disk.driveType)
	{
	case 1:
		disk.driveTypeDesc=tr("No Root Directory");
		break;
	case 2:
		disk.driveTypeDesc=tr("Removable Disk");
		break;
	case 3:
		disk.driveTypeDesc=tr("Local Disk");
		break;
	case 4:
		disk.driveTypeDesc=tr("Network Drive");
		break;
	case 5:
		disk.driveTypeDesc=tr("Compact Disc");
		break;
	case 6:
		disk.driveTypeDesc=tr("RAM Disk");
		break;
	default:
		disk.driveTypeDesc=tr("Unknown");
		break;
	}

	disk.fileSystem=findString(object, L"FileSystem");
	disk.freeSpace=findUInt64(object, L"FreeSpace");
	disk.size=findUInt64(object, L"Size");

	m_vecLogicalDisk.append(disk);
}

REGISTER_WMI_INSTANCE(QWmiLogicalDisk)
