#include "extern.h"

#include <QApplication>
#include <QStyle>

#include "common.h"

#include "qmessageparser.h"
#include "qmessageprocessor.h"
#include "qmessagefiletransfer.h"
#include "qmessagesystem.h"
#include "qmessagecomputersystem.h"
#include "qmessagenetworkadapter.h"
#include "qmessageprocess.h"
#include "qmessagedisk.h"
#include "qmessagememory.h"
#include "qmessageperformer.h"

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#include "qwmiprocessor.h"
#include "qwmisystem.h"
#include "qwmicomputersystem.h"
#include "qwminetworkadapter.h"
#include "qwmiprocess.h"
#include "qwmidiskdrive.h"
#include "qwmicdromdrive.h"
#include "qwmilogicaldisk.h"
#include "qwmiphysicalmemory.h"
#include "qwmisystemmemory.h"
#include "qwmiperfprocessor.h"
#else
#include "qstatfactory.h"
#include "qstatsystem.h"
#include "qstatcomputersystem.h"
#include "qstatnetworkadapter.h"
#include "qstatprocess.h"
#include "qstatprocessor.h"
#include "qstatperfprocessor.h"
#include "qstatphysicalmemory.h"
#include "qstatsystemmemory.h"
#include "qstatlogicaldisk.h"
#include "qstatdiskdrive.h"
#include "qstatcdromdrive.h"
#endif

#include "qgrouptransferqueue.h"

void registerMessage()
{
//	qRegisterMetaType<QSystemConfigure::SystemInfo>("QSystemConfigure::SystemInfo");
	qRegisterMetaType<QGroupTransferQueue::TransferQueueItem>("QGroupTransferQueue::TransferQueueItem");

	qRegisterMetaType<FileInfo>("FileInfo");
	qRegisterMetaType<QVector<FileInfo> >("QVector<FileInfo>");

	QMessageParser* pParser=QMessageParser::instance();

	pParser->messageRegister(new QMessageProcessor);
	pParser->messageRegister(new QMessageFileTransfer);
	pParser->messageRegister(new QMessageSystem);
	pParser->messageRegister(new QMessageComputerSystem);
	pParser->messageRegister(new QMessageNetworkAdapter);
	pParser->messageRegister(new QMessageProcess);
	pParser->messageRegister(new QMessageDisk);
	pParser->messageRegister(new QMessageMemory);
	pParser->messageRegister(new QMessagePerformer);
}

void registerWmi()
{
	qRegisterMetaType<QVector<ProcessorInfo> >("QVector<ProcessorInfo>");
	qRegisterMetaType<QVector<OperatingSystem> >("QVector<OperatingSystem>");
	qRegisterMetaType<QVector<ComputerSystem> >("QVector<ComputerSystem>");
	qRegisterMetaType<QVector<NetworkAdapter> >("QVector<NetworkAdapter>");
	qRegisterMetaType<QVector<SystemProcess> >("QVector<SystemProcess>");
	qRegisterMetaType<QMap<quint32, quint64> >("QMap<quint32, quint64>");
	qRegisterMetaType<QVector<DiskDrive> >("QVector<DiskDrive>");
	qRegisterMetaType<QVector<CDROMDrive> >("QVector<CDROMDrive>");
	qRegisterMetaType<QVector<LogicalDisk> >("QVector<LogicalDisk>");
	qRegisterMetaType<QVector<PhysicalMemory> >("QVector<PhysicalMemory>");
	qRegisterMetaType<SystemMemory>("SystemMemory");
	qRegisterMetaType<PerfProcessor>("PerfProcessor");

#ifdef Q_OS_WIN
	QWmiFactory* pFactory=QWmiFactory::instance();

	pFactory->registerWmi(new QWmiProcessor);
	pFactory->registerWmi(new QWmiSystem);
	pFactory->registerWmi(new QWmiComputerSystem);
	pFactory->registerWmi(new QWmiNetworkAdapter);
	pFactory->registerWmi(new QWmiProcess);
	pFactory->registerWmi(new QWmiDiskDrive);
	pFactory->registerWmi(new QWmiCDROMDrive);
	pFactory->registerWmi(new QWmiLogicalDisk);
	pFactory->registerWmi(new QWmiPhysicalMemory);
	pFactory->registerWmi(new QWmiSystemMemory);
	pFactory->registerWmi(new QWmiPerfProcessor);
#else
	QStatFactory* pFactory=QStatFactory::instance();

	pFactory->registerStat(new QStatSystem);
	pFactory->registerStat(new QStatComputerSystem);
	pFactory->registerStat(new QStatNetworkAdapter);
	pFactory->registerStat(new QStatProcess);
	pFactory->registerStat(new QStatProcessor);
	pFactory->registerStat(new QStatPerfProcessor);
	pFactory->registerStat(new QStatPhysicalMemory);
	pFactory->registerStat(new QStatSystemMemory);
	pFactory->registerStat(new QStatLogicalDisk);
	pFactory->registerStat(new QStatDiskDrive);
	pFactory->registerStat(new QStatCDROMDrive);
#endif
}

FileInfo entryInfo(const QFileInfo& info)
{
	FileInfo fileInfo;

	fileInfo.type=entryInfoFileType(info);
	fileInfo.name=info.fileName();
	fileInfo.size=info.isFile()? info.size():0;
	fileInfo.isReadable=info.isReadable();
	fileInfo.isWritable=info.isWritable();
	fileInfo.isExecutable=info.isExecutable();
	fileInfo.lastModified=info.lastModified();

	return fileInfo;
}

int entryInfoFileType(const QFileInfo& info)
{
	int nFileType=QMessageFileTransfer::File;;

	if (info.isDir())
	{
		if (!info.isHidden())
		{
			nFileType=QMessageFileTransfer::Dir;
		}
		else
		{
			nFileType=QMessageFileTransfer::HideDir;
		}
	}
	else if (info.isFile())
	{
		if (!info.isHidden())
		{
			nFileType=QMessageFileTransfer::File;
		}
		else
		{
			nFileType=QMessageFileTransfer::HideFile;
		}
	}

	if (info.isSymLink())
	{
		if (!info.isHidden())
		{
			nFileType|=QMessageFileTransfer::SymLink;
		}
		else
		{
			nFileType|=QMessageFileTransfer::HideSymLink;
		}
	}

	return nFileType;
}

QString fileType(const QFileInfo& info)
{
	QString strType;
	if (info.isSymLink())
	{
		strType=QObject::tr("Symbol Link");
	}
	else if (info.isDir())
	{
		strType=QObject::tr("File Folder");
	}
	else if (info.isExecutable())
	{
		strType=QObject::tr("Executable");
	}
	else
	{
		strType=info.suffix()+" "+QObject::tr("File");
	}

	return strType;
}

QString fileType(const FileInfo& info)
{
	QString strType;
	
	switch (info.type)
	{
	case QMessageFileTransfer::Drive:
		{
			strType=QObject::tr("Drive");
		}
		break;
	case QMessageFileTransfer::Dir:
	case QMessageFileTransfer::HideDir:
		{
			strType=QObject::tr("File Folder");
		}
		break;
	case QMessageFileTransfer::File:
	case QMessageFileTransfer::HideFile:
		{
			if (info.isExecutable)
			{
				strType=QObject::tr("Executable");
			}
			else
			{
				strType=fileSuffix(info.name)+" "+QObject::tr("File");
			}
		}
		break;
	case QMessageFileTransfer::SymLinkDir:
	case QMessageFileTransfer::SymLinkHideDir:
	case QMessageFileTransfer::SymLinkFile:
	case QMessageFileTransfer::SymLinkHideFile:
		{
			strType=QObject::tr("Symbol Link");
		}
		break;
	default:
		break;
	}

	return strType;
}

QIcon fileIcon(const FileInfo& info)
{
	return fileIcon(info.type);
}

QIcon fileIcon(int type)
{
	QStyle *style = QApplication::style();
	QIcon icon;

	switch (type)
	{
	case QMessageFileTransfer::Drive:
		{
			icon=style->standardIcon(QStyle::SP_DriveHDIcon);
		}
		break;
	case QMessageFileTransfer::Dir:
	case QMessageFileTransfer::HideDir:
		{
			icon=style->standardIcon(QStyle::SP_DirIcon);
		}
		break;
	case QMessageFileTransfer::File:
	case QMessageFileTransfer::HideFile:
		{
			icon=style->standardIcon(QStyle::SP_FileIcon);
		}
		break;
	case QMessageFileTransfer::SymLinkDir:
	case QMessageFileTransfer::SymLinkHideDir:
		{
			icon=style->standardIcon(QStyle::SP_DirLinkIcon);
		}
		break;
	case QMessageFileTransfer::SymLinkFile:
	case QMessageFileTransfer::SymLinkHideFile:
		{
			icon=style->standardIcon(QStyle::SP_FileLinkIcon);
		}
		break;
	case QMessageFileTransfer::CDDVDDrive:
		{
			icon=style->standardIcon(QStyle::SP_DriveDVDIcon);
		}
		break;
	default:
		break;;
	}

	return icon;
}

QString fileSize(quint64 size)
{
	QString strSize;

	if (size > 0)
	{
		if (size > 1024)
		{
			strSize=formatNumberString(QString::number((size+1023)/1024))+" K";
		}
		else
		{
			strSize=formatNumberString(QString::number(size))+" B";
		}
	}

	return strSize;
}

QString configManagerErrorCode(quint32 configManager)
{
	QString strCode;

	switch (configManager)
	{
	case 1:
		strCode=QObject::tr("Device is not configured correctly.");
		break;
	case 2:
		strCode=QObject::tr("Windows cannot load the driver for this device.");
		break;
	case 3:
		strCode=QObject::tr("Driver for this device might be corrupted, or the system may be low on memory or other resources.");
		break;
	case 4:
		strCode=QObject::tr("Device is not working properly. One of its drivers or the registry might be corrupted.");
		break;
	case 5:
		strCode=QObject::tr("Driver for the device requires a resource that Windows cannot manage.");
		break;
	case 6:
		strCode=QObject::tr("Boot configuration for the device conflicts with other devices.");
		break;
	case 7:
		strCode=QObject::tr("Cannot filter.");
		break;
	case 8:
		strCode=QObject::tr("Driver loader for the device is missing.");
		break;
	case 9:
		strCode=QObject::tr("Device is not working properly; the controlling firmware is incorrectly reporting the resources for the device.");
		break;
	case 10:
		strCode=QObject::tr("Device cannot start.");
		break;
	case 11:
		strCode=QObject::tr("Device failed.");
		break;
	case 12:
		strCode=QObject::tr("Device cannot find enough free resources to use.");
		break;
	case 13:
		strCode=QObject::tr("Windows cannot verify the device's resources.");
		break;
	case 14:
		strCode=QObject::tr("Device cannot work properly until the computer is restarted.");
		break;
	case 15:
		strCode=QObject::tr("Device is not working properly due to a possible re-enumeration problem.");
		break;
	case 16:
		strCode=QObject::tr("Windows cannot identify all of the resources that the device uses.");
		break;
	case 17:
		strCode=QObject::tr("Device is requesting an unknown resource type.");
		break;
	case 18:
		strCode=QObject::tr("Device drivers need to be reinstalled.");
		break;
	case 19:
		strCode=QObject::tr("Failure using the VxD loader.");
		break;
	case 20:
		strCode=QObject::tr("Registry might be corrupted.");
		break;
	case 21:
		strCode=QObject::tr("System failure. If changing the device driver is ineffective, see the hardware documentation. Windows is removing the device.");
		break;
	case 22:
		strCode=QObject::tr("Device is disabled.");
		break;
	case 23:
		strCode=QObject::tr("System failure. If changing the device driver is ineffective, see the hardware documentation.");
		break;
	case 24:
		strCode=QObject::tr("Device is not present, not working properly, or does not have all of its drivers installed.");
		break;
	case 25:
		strCode=QObject::tr("Windows is still setting up the device.");
		break;
	case 26:
		strCode=QObject::tr("Windows is still setting up the device.");
		break;
	case 27:
		strCode=QObject::tr("Device does not have valid log configuration.");
		break;
	case 28:
		strCode=QObject::tr("Device drivers are not installed.");
		break;
	case 29:
		strCode=QObject::tr("Device is disabled; the device firmware did not provide the required resources.");
		break;
	case 30:
		strCode=QObject::tr("Device is using an IRQ resource that another device is using.");
		break;
	case 31:
		strCode=QObject::tr("Device is not working properly; Windows cannot load the required device drivers.");
		break;
	default:
		strCode=QObject::tr("Device is working properly.");
		break;
	}

	return strCode;
}

QDataStream& operator<<(QDataStream& out, const ProcessorInfo& processor)
{
	out<<processor.os;

	switch (processor.os)
	{
	case OS_WINDOWS:
	case OS_SOLARIS:
		{
			out<<processor.configManager<<processor.core<<processor.addressWidth<<processor.dataWidth
				<<processor.description<<processor.deviceID<<processor.externalClock<<processor.L2CacheSize
				<<processor.L2CacheSpeed<<processor.manufacturer<<processor.maxClockSpeed<<processor.name;
		}
		break;
	case OS_AIX:
		{
			out<<processor.status<<processor.core<<processor.addressWidth<<processor.dataWidth
				<<processor.description<<processor.deviceID<<processor.externalClock<<processor.L2CacheSize
				<<processor.L2CacheSpeed<<processor.manufacturer<<processor.maxClockSpeed<<processor.name;
		}
		break;
	default:
		qDebug("Unknow os!!!");
		break;
	}

	return out;
}

QDataStream& operator>>(QDataStream& in, ProcessorInfo& processor)
{
	in>>processor.os;

	switch (processor.os)
	{
	case OS_WINDOWS:
	case OS_SOLARIS:
		{
			in>>processor.configManager>>processor.core>>processor.addressWidth>>processor.dataWidth
				>>processor.description>>processor.deviceID>>processor.externalClock>>processor.L2CacheSize
				>>processor.L2CacheSpeed>>processor.manufacturer>>processor.maxClockSpeed>>processor.name;
		}
		break;
	case OS_AIX:
		{
			in>>processor.status>>processor.core>>processor.addressWidth>>processor.dataWidth
				>>processor.description>>processor.deviceID>>processor.externalClock>>processor.L2CacheSize
				>>processor.L2CacheSpeed>>processor.manufacturer>>processor.maxClockSpeed>>processor.name;
		}
		break;
	default:
		qDebug("Unknow os!!!");
		break;
	}

	return in;
}

QDataStream& operator<<(QDataStream& out, const DiskDrive& disk)
{
	out<<disk.os;

	switch (disk.os)
	{
	case OS_WINDOWS:
	case OS_SOLARIS:
		{
			out<<disk.configManager<<disk.description<<disk.deviceID<<disk.interfaceType
				<<disk.manufacturer<<disk.partitions<<disk.size<<disk.mediaType
				<<disk.model<<disk.numberOfMediaSupported<<disk.SCSIBus<<disk.SCSILogicalUnit
				<<disk.SCSITargetId<<disk.SCSIPort;
		}
		break;
	case OS_AIX:
		{
			out<<disk.status<<disk.serialNumber<<disk.position<<disk.description<<disk.deviceID<<disk.interfaceType
				<<disk.manufacturer<<disk.size<<disk.model;
		}
		break;
	default:
		qDebug("Unknow os!!!");
		break;
	}

	return out;
}

QDataStream& operator>>(QDataStream& in, DiskDrive& disk)
{
	in>>disk.os;

	switch (disk.os)
	{
	case OS_WINDOWS:
	case OS_SOLARIS:
		{
			in>>disk.configManager>>disk.description>>disk.deviceID>>disk.interfaceType
				>>disk.manufacturer>>disk.partitions>>disk.size>>disk.mediaType
				>>disk.model>>disk.numberOfMediaSupported>>disk.SCSIBus>>disk.SCSILogicalUnit
				>>disk.SCSITargetId>>disk.SCSIPort;
		}
		break;
	case OS_AIX:
		{
			in>>disk.status>>disk.serialNumber>>disk.position>>disk.description>>disk.deviceID>>disk.interfaceType
				>>disk.manufacturer>>disk.size>>disk.model;
		}
		break;
	default:
		qDebug("Unknow system!!!");
		break;
	}

	return in;
}

QDataStream& operator<<(QDataStream& out, const CDROMDrive& disk)
{
	out<<disk.os;

	switch (disk.os)
	{
	case OS_WINDOWS:
	case OS_SOLARIS:
		{
			out<<disk.configManager<<disk.description<<disk.deviceID<<disk.drive
				<<disk.manufacturer<<disk.mediaType<<disk.numberOfMediaSupported
				<<disk.SCSIBus<<disk.SCSILogicalUnit<<disk.SCSITargetId<<disk.SCSIPort
				<<disk.size<<disk.transferRate<<disk.volumeName<<disk.volumeSerialNumber<<disk.name;
		}
		break;
	case OS_AIX:
		{
			out<<disk.status<<disk.position<<disk.description<<disk.deviceID<<disk.manufacturer<<disk.transferRate
				<<disk.volumeName<<disk.volumeSerialNumber<<disk.name;
		}
		break;
	default:
		qDebug("Unknow os!!!");
		break;
	}

	return out;
}

QDataStream& operator>>(QDataStream& in, CDROMDrive& disk)
{
	in>>disk.os;

	switch (disk.os)
	{
	case OS_WINDOWS:
	case OS_SOLARIS:
		{
			in>>disk.configManager>>disk.description>>disk.deviceID>>disk.drive
				>>disk.manufacturer>>disk.mediaType>>disk.numberOfMediaSupported
				>>disk.SCSIBus>>disk.SCSILogicalUnit>>disk.SCSITargetId>>disk.SCSIPort
				>>disk.size>>disk.transferRate>>disk.volumeName>>disk.volumeSerialNumber>>disk.name;
		}
		break;
	case OS_AIX:
		{
			in>>disk.status>>disk.position>>disk.description>>disk.deviceID>>disk.manufacturer>>disk.transferRate
				>>disk.volumeName>>disk.volumeSerialNumber>>disk.name;
		}
		break;
	default:
		qDebug("Unknow system!!!");
		break;
	}

	return in;
}
