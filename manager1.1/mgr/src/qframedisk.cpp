#include "qframedisk.h"

#include "common.h"
#include "qmessageparser.h"

QFrameDisk::QFrameDisk(QWidget *parent)
	: QFrame(parent)
{
	setupUi(this);

	m_pModelDrive=new QStandardItemModel(0, 1, this);
	comboBoxDrive->setModel(m_pModelDrive);

	QStringList listLabels;
	listLabels<<tr("Name")<<tr("File System");
	m_pModelComputer=new QStandardItemModel(0, listLabels.size(), this);
	m_pModelComputer->setHorizontalHeaderLabels(listLabels);

	m_pSortModel=new QSortFilterProxyModel;
	m_pSortModel->setDynamicSortFilter(true);
	m_pSortModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	m_pSortModel->setSourceModel(m_pModelComputer);

	m_pDelegate=new QDiskDelegate(this);
	
	treeView->setRootIsDecorated(false);
	treeView->setAlternatingRowColors(true);
	treeView->setModel(m_pSortModel);
	treeView->setSortingEnabled(true);
	treeView->setItemDelegate(m_pDelegate);
	treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	treeView->setItemsExpandable(true);
	treeView->header()->setStretchLastSection(true);

	treeView->setColumnWidth(QDiskDelegate::NameTitle, 300);
	treeView->setColumnWidth(QDiskDelegate::FileSystemTitle, 230);

	connect(comboBoxDrive, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));

	currentIndexChanged(-1);
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

		m_pModelComputer->itemFromIndex(m_pModelComputer->index(i, QDiskDelegate::NameTitle))->setEditable(false);

		if (!disk.deviceID.isEmpty())
		{
			m_pModelComputer->setData(m_pModelComputer->index(i, QDiskDelegate::NameTitle), disk.description+"("+disk.deviceID+")");
		}
		else
		{
			m_pModelComputer->setData(m_pModelComputer->index(i, QDiskDelegate::NameTitle), disk.description);
		}

		QVariant var;
		var.setValue(disk);

		m_pModelComputer->setData(m_pModelComputer->index(i, QDiskDelegate::NameTitle), var, Qt::UserRole);

		m_pModelComputer->itemFromIndex(m_pModelComputer->index(i, QDiskDelegate::FileSystemTitle))->setEditable(false);
		m_pModelComputer->setData(m_pModelComputer->index(i, QDiskDelegate::FileSystemTitle), disk.size);
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
				labelLogo->setPixmap(findPixmap(":/images/disk.png"));

				QString strSize;
				if (drive.size != INVALID_SIZE_MARK)
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

						label_1->setText(tr("Name:"));
						lineEdit_1->setText(drive.model);

						label_2->setText(tr("Type:"));
						lineEdit_2->setText(drive.description);

						label_3->setText(tr("Manufacturer:"));
						lineEdit_3->setText(drive.manufacturer);

						label_4->setText(tr("Device ID:"));
						lineEdit_4->setText(drive.deviceID);

						label_5->setText(tr("Media Type:"));
						lineEdit_5->setText(strMediaType.remove(QChar('\t'), Qt::CaseInsensitive));

						label_6->setText(tr("Size:"));
						lineEdit_6->setText(strSize);

						label_7->setText(tr("Partitions:"));
						lineEdit_7->setText(drive.partitions==-1? tr("Unknown"):QString::number(drive.partitions));

						label_8->setText(tr("Position:"));
						label_8->show();
						lineEdit_8->setText(strPosition);
						lineEdit_8->show();

						label_9->hide();
						lineEdit_9->hide();

						label_10->hide();
						lineEdit_10->hide();

						textEditDeviceStatus->setText(configManagerErrorCode(drive.configManager));
					}
					break;
				case OS_AIX:
					{
						label_1->setText(tr("Name:"));
						lineEdit_1->setText(drive.model);

						label_2->setText(tr("Type:"));
						lineEdit_2->setText(drive.description);

						label_3->setText(tr("Manufacturer:"));
						lineEdit_3->setText(drive.manufacturer);

						label_4->setText(tr("Device ID:"));
						lineEdit_4->setText(drive.deviceID);

						label_5->setText(tr("Size:"));
						lineEdit_5->setText(strSize);

						label_6->setText(tr("Serial Number:"));
						lineEdit_6->setText(drive.serialNumber);

						label_7->setText(tr("Position:"));
						lineEdit_7->setText(drive.position);

						label_8->hide();
						lineEdit_8->hide();

						label_9->hide();
						lineEdit_9->hide();

						label_10->hide();
						lineEdit_10->hide();

						textEditDeviceStatus->setText(drive.status);
					}
					break;
				}
				
			}
			break;
		case DriveTypeCDROMDrive:
			{
				if (nIndex>=m_vecCDROMDrive.size())
				{
					break;
				}

				const CDROMDrive& drive=m_vecCDROMDrive[nIndex];
				labelLogo->setPixmap(findPixmap(":/images/cdromlarge.png"));

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

						label_1->setText(tr("Name:"));
						lineEdit_1->setText(drive.name);

						label_2->setText(tr("Type:"));
						lineEdit_2->setText(drive.description);

						label_3->setText(tr("Manufacturer:"));
						lineEdit_3->setText(drive.manufacturer);

						label_4->setText(tr("Device ID:"));
						lineEdit_4->setText(drive.deviceID);

						label_5->setText(tr("Media Type:"));
						lineEdit_5->setText(strMediaType.remove(QChar('\t'), Qt::CaseInsensitive));

						label_6->setText(tr("Size:"));
						lineEdit_6->setText(strSize);

						label_7->setText(tr("TransferRate:"));
						lineEdit_7->setText(drive.transferRate==-1? tr("Unknown"):(QString::number(drive.transferRate, 'f', 2))+" KB/s");

						label_8->setText(tr("Volume Name:"));
						label_8->show();
						lineEdit_8->setText(drive.volumeName);
						lineEdit_8->show();

						label_9->setText(tr("SerialNumber:"));
						label_9->show();
						lineEdit_9->setText(drive.volumeSerialNumber);
						lineEdit_9->show();

						label_10->setText(tr("Position:"));
						label_10->show();
						lineEdit_10->setText(strPosition);
						lineEdit_10->show();

						textEditDeviceStatus->setText(configManagerErrorCode(drive.configManager));
					}
					break;
				case OS_AIX:
					{
						label_1->setText(tr("Name:"));
						lineEdit_1->setText(drive.name);

						label_2->setText(tr("Type:"));
						lineEdit_2->setText(drive.description);

						label_3->setText(tr("Manufacturer:"));
						lineEdit_3->setText(drive.manufacturer);

						label_4->setText(tr("Device ID:"));
						lineEdit_4->setText(drive.deviceID);

						label_5->setText(tr("TransferRate:"));
						lineEdit_5->setText(drive.transferRate==-1? tr("Unknown"):(QString::number(drive.transferRate, 'f', 2))+" KB/s");

						label_6->setText(tr("Volume Name:"));
						lineEdit_6->setText(drive.volumeName);

						label_7->setText(tr("SerialNumber:"));
						lineEdit_7->setText(drive.volumeSerialNumber);

						label_8->setText(tr("Position:"));
						label_8->show();
						lineEdit_8->setText(drive.position);
						lineEdit_8->show();

						label_9->hide();
						lineEdit_9->hide();

						label_10->hide();
						lineEdit_10->hide();

						textEditDeviceStatus->setText(drive.status);
					}
					break;
				}
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

		label_1->setText(tr("Name:"));
		lineEdit_1->setText(tr("Unknown"));

		label_2->setText(tr("Type:"));
		lineEdit_2->setText(tr("Unknown"));

		label_3->setText(tr("Manufacturer:"));
		lineEdit_3->setText(tr("Unknown"));

		label_4->setText(tr("Device ID:"));
		lineEdit_4->setText(tr("Unknown"));

		label_5->setText(tr("Media Type:"));
		lineEdit_5->setText(tr("Unknown"));

		label_6->setText(tr("Size:"));
		lineEdit_6->setText(tr("Unknown"));

		label_7->setText(tr("Partitions:"));
		lineEdit_7->setText(tr("Unknown"));

		label_8->setText(tr("Position:"));
		label_8->show();
		lineEdit_8->setText(tr("Unknown"));
		lineEdit_8->show();

		label_9->hide();
		lineEdit_9->hide();

		label_10->hide();
		lineEdit_10->hide();

		textEditDeviceStatus->clear();
	}
}
