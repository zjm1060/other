#include "qframedisk.h"

#include "common.h"

#ifdef Q_OS_WIN
#include "qabstractwmi.h"
#endif

QFrameDisk::QFrameDisk(QWidget *parent)
	: QFrame(parent)
{
	setupUi(this);

	m_pModelDrive=new QStandardItemModel(0, 1, this);
	comboBoxDrive->setModel(m_pModelDrive);

	QStringList listLabels;
	listLabels<<tr("Name")<<tr("Type")<<tr("File System")<<tr("Free Space")<<tr("Size");
	m_pModelComputer=new QStandardItemModel(0, listLabels.size(), this);
	m_pModelComputer->setHorizontalHeaderLabels(listLabels);

	m_pSortModel=new QSortFilterProxyModel;
	m_pSortModel->setDynamicSortFilter(true);
	m_pSortModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	m_pSortModel->setSourceModel(m_pModelComputer);
	
	treeView->setRootIsDecorated(false);
	treeView->setAlternatingRowColors(true);
	treeView->setModel(m_pSortModel);
	treeView->setSortingEnabled(true);

	treeView->setColumnWidth(0, 200);
	treeView->setColumnWidth(1, 100);
	treeView->setColumnWidth(2, 100);
	treeView->setColumnWidth(3, 100);
	treeView->setColumnWidth(4, 100);

	connect(comboBoxDrive, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
}

QFrameDisk::~QFrameDisk()
{

}

void QFrameDisk::clear()
{
	m_bClear=true;
	m_pModelDrive->setRowCount(0);
}

void QFrameDisk::setDiskDrive(const QVector<DiskDrive>& diskDrives)
{
	clear();

	m_vecDiskDrive=diskDrives;

	showDiskDrive();

	comboBoxDrive->setCurrentIndex(0);
}

void QFrameDisk::showDiskDrive()
{
	for (int nIndex=0; nIndex<m_vecDiskDrive.size(); nIndex++)
	{
		const DiskDrive& drive=m_vecDiskDrive[nIndex];
		m_pModelDrive->insertRow(nIndex);

		m_pModelDrive->itemFromIndex(m_pModelDrive->index(nIndex, 0))->setEditable(false);
		m_pModelDrive->setData(m_pModelDrive->index(nIndex, 0), m_iconProvider.icon(QFileIconProvider::Drive), Qt::DecorationRole);
		m_pModelDrive->setData(m_pModelDrive->index(nIndex, 0), drive.model);
		m_pModelDrive->setData(m_pModelDrive->index(nIndex, 0), DriveTypeDiskDrive, Qt::UserRole);
		m_pModelDrive->setData(m_pModelDrive->index(nIndex, 0), nIndex, Qt::UserRole+1);
	}
}

void QFrameDisk::setCDROMDrive(const QVector<CDROMDrive>& drives)
{
	if (!m_bClear)
	{
		return;
	}

	m_bClear=false;
	m_vecCDROMDrive=drives;

	showCDROMDrive();
}

void QFrameDisk::showCDROMDrive()
{
	int nIndex=m_pModelDrive->rowCount();
	for (int i=0; i<m_vecCDROMDrive.size(); i++)
	{
		const CDROMDrive& drive=m_vecCDROMDrive[i];
		m_pModelDrive->insertRow(nIndex);

		m_pModelDrive->itemFromIndex(m_pModelDrive->index(nIndex, 0))->setEditable(false);
		m_pModelDrive->setData(m_pModelDrive->index(nIndex, 0), QIcon(":/images/cdromsmall.png"), Qt::DecorationRole);
		m_pModelDrive->setData(m_pModelDrive->index(nIndex, 0), drive.name);
		m_pModelDrive->setData(m_pModelDrive->index(nIndex, 0), DriveTypeCDROMDrive, Qt::UserRole);
		m_pModelDrive->setData(m_pModelDrive->index(nIndex, 0), i, Qt::UserRole+1);

		nIndex++;
	}
}

void QFrameDisk::setLogicalDisk(const QVector<LogicalDisk>& disk)
{
	m_pModelComputer->setRowCount(0);
	m_vecLogicalDisk=disk;

	showLogicalDisk();
}

void QFrameDisk::showLogicalDisk()
{
	for (int i=0; i<m_vecLogicalDisk.size(); i++)
	{
		const LogicalDisk& disk=m_vecLogicalDisk[i];

		m_pModelComputer->insertRow(i);

		m_pModelComputer->itemFromIndex(m_pModelComputer->index(i, 0))->setEditable(false);

		if (!disk.deviceID.isEmpty())
		{
			m_pModelComputer->setData(m_pModelComputer->index(i, 0), disk.description+"("+disk.deviceID+")");
		}
		else
		{
			m_pModelComputer->setData(m_pModelComputer->index(i, 0), disk.description);
		}

		switch (disk.driveType)
		{
		case 5:
			m_pModelComputer->setData(m_pModelComputer->index(i, 0), QIcon(":/images/cdromsmall.png"), Qt::DecorationRole);
			break;
		default:
			m_pModelComputer->setData(m_pModelComputer->index(i, 0), m_iconProvider.icon(QFileIconProvider::Drive), Qt::DecorationRole);
			break;
		}
		
		m_pModelComputer->itemFromIndex(m_pModelComputer->index(i, 1))->setEditable(false);
		m_pModelComputer->setData(m_pModelComputer->index(i, 1), disk.driveTypeDesc);

		m_pModelComputer->itemFromIndex(m_pModelComputer->index(i, 2))->setEditable(false);
		m_pModelComputer->setData(m_pModelComputer->index(i, 2), disk.fileSystem);

		m_pModelComputer->itemFromIndex(m_pModelComputer->index(i, 3))->setEditable(false);
		m_pModelComputer->itemFromIndex(m_pModelComputer->index(i, 3))->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		m_pModelComputer->setData(m_pModelComputer->index(i, 3), sizeToString(disk.freeSpace));

		m_pModelComputer->itemFromIndex(m_pModelComputer->index(i, 4))->setEditable(false);
		m_pModelComputer->itemFromIndex(m_pModelComputer->index(i, 4))->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		m_pModelComputer->setData(m_pModelComputer->index(i, 4), sizeToString(disk.size));
	}
}

void QFrameDisk::currentIndexChanged(int index)
{
	if (index >= 0)
	{
		int nType=m_pModelDrive->data(m_pModelDrive->index(index, 0), Qt::UserRole).toInt();
		int nIndex=m_pModelDrive->data(m_pModelDrive->index(index, 0), Qt::UserRole+1).toInt();

		switch (nType)
		{
		case DriveTypeDiskDrive:
			{
				if (nIndex >= m_vecDiskDrive.size())
				{
					break;
				}

				const DiskDrive& drive=m_vecDiskDrive[nIndex];
				labelLogo->setPixmap(QPixmap(":/images/disk.png"));

				QString strSize;
				if (drive.size != -1)
				{
					strSize=sizeToString(drive.size);
				}
				else
				{
					strSize=tr("Unknown");
				}

				QString strMediaType=drive.mediaType;

				QString strDesc;
				switch (drive.os)
				{
				case OS_WINDOWS:
				case OS_SOLARIS:
					{
						QString strPosition;
						if (drive.SCSIBus == -1 || drive.SCSITargetId == -1 || drive.SCSILogicalUnit == -1 || drive.SCSIPort == -1)
						{
							strPosition=tr("Unknown");
						}
						else
						{
							strPosition=QString(tr("Bus Number %1, Target ID %2, LUN %3, Port %4")
								.arg(drive.SCSIBus).arg(drive.SCSITargetId).arg(drive.SCSILogicalUnit).arg(drive.SCSIPort));
						}

						strDesc=tr("Name:\t")+drive.model
							+"\n"+tr("Type:\t")+drive.description
							+"\n"+tr("Manufacturer:\t")+drive.manufacturer
							+"\n"+tr("Device ID:\t")+drive.deviceID
							+"\n"+tr("Media Type:\t")+strMediaType.remove(QChar('\t'), Qt::CaseInsensitive)
							+"\n"+tr("Size:\t")+strSize
							+"\n"+tr("Partitions:\t")+(drive.partitions==-1? tr("Unknown"):QString::number(drive.partitions))
							+"\n"+tr("Position:\t")+strPosition;

						textEditDeviceStatus->setText(configManagerErrorCode(drive.configManager));
					}
					break;
				case OS_AIX:
					{
						strDesc=tr("Name:\t")+drive.model
							+"\n"+tr("Type:\t")+drive.description
							+"\n"+tr("Manufacturer:\t")+drive.manufacturer
							+"\n"+tr("Device ID:\t")+drive.deviceID
							+"\n"+tr("Size:\t")+strSize
							+"\n"+tr("Serial Number:\t")+drive.serialNumber
							+"\n"+tr("Position:\t")+drive.position;

						textEditDeviceStatus->setText(drive.status);
					}
					break;
				}

				labelDescription->setText(strDesc);

				
			}
			break;
		case DriveTypeCDROMDrive:
			{
				if (nIndex>=m_vecCDROMDrive.size())
				{
					break;
				}

				const CDROMDrive& drive=m_vecCDROMDrive[nIndex];
				labelLogo->setPixmap(QPixmap(":/images/cdromlarge.png"));

				QString strDesc;
				switch (drive.os)
				{
				case OS_WINDOWS:
				case OS_SOLARIS:
					{
						QString strSize;
						if (drive.size != -1)
						{
							strSize=sizeToString(drive.size);
						}
						else
						{
							strSize=tr("Unknown");
						}

						QString strMediaType=drive.mediaType;
						QString strPosition;
						if (drive.SCSIBus == -1 || drive.SCSITargetId == -1 || drive.SCSILogicalUnit == -1 || drive.SCSIPort == -1)
						{
							strPosition=tr("Unknown");
						}
						else
						{
							strPosition=QString(tr("Bus Number %1, Target ID %2, LUN %3, Port %4")
								.arg(drive.SCSIBus).arg(drive.SCSITargetId).arg(drive.SCSILogicalUnit).arg(drive.SCSIPort));
						}

						strDesc=tr("Name:\t")+drive.name
							+"\n"+tr("Type:\t")+drive.description
							+"\n"+tr("Manufacturer:\t")+drive.manufacturer
							+"\n"+tr("Device ID:\t")+drive.deviceID
							+"\n"+tr("Media Type:\t")+strMediaType.remove(QChar('\t'), Qt::CaseInsensitive)
							+"\n"+tr("Size:\t")+strSize
							+"\n"+tr("TransferRate:\t")+
							(drive.transferRate==-1? tr("Unknown"):(QString::number(drive.transferRate, 'f', 2))+" KB/s")
							+"\n"+tr("Volume Name:\t")+drive.volumeName
							+"\n"+tr("SerialNumber:\t")+drive.volumeSerialNumber
							+"\n"+tr("Position:\t")+strPosition;

						textEditDeviceStatus->setText(configManagerErrorCode(drive.configManager));
					}
					break;
				case OS_AIX:
					{
						strDesc=tr("Name:\t")+drive.name
							+"\n"+tr("Type:\t")+drive.description
							+"\n"+tr("Manufacturer:\t")+drive.manufacturer
							+"\n"+tr("Device ID:\t")+drive.deviceID
							+"\n"+tr("TransferRate:\t")+
							(drive.transferRate==-1? tr("Unknown"):(QString::number(drive.transferRate, 'f', 2))+" KB/s")
							+"\n"+tr("Volume Name:\t")+drive.volumeName
							+"\n"+tr("SerialNumber:\t")+drive.volumeSerialNumber
							+"\n"+tr("Position:\t")+drive.position;

						textEditDeviceStatus->setText(drive.status);
					}
					break;
				}

				labelDescription->setText(strDesc);
			}
			break;
		default:
			Q_ASSERT(true);
			break;
		}
	}
	else
	{
		labelLogo->setPixmap(QPixmap());
		labelDescription->setText("");
		textEditDeviceStatus->clear();
	}
}
