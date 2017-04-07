#include "qframememory.h"

#include "common.h"

QFrameMemory::QFrameMemory(QWidget *parent)
	: QFrame(parent)
{
	setupUi(this);

	m_pFrameCurve=new QFrameCurve(groupBoxMemoryRecords);
	m_pLayout=new QVBoxLayout(groupBoxMemoryRecords);
	groupBoxMemoryRecords->setLayout(m_pLayout);
	m_pLayout->addWidget(m_pFrameCurve);

	m_pModelPhysicalMemory=new QStandardItemModel(0, 1, this);
	comboBoxMemoryDevice->setModel(m_pModelPhysicalMemory);

	connect(comboBoxMemoryDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
}

QFrameMemory::~QFrameMemory()
{

}

void QFrameMemory::clear()
{
	m_pModelPhysicalMemory->setRowCount(0);

	if (m_pFrameCurve != NULL)
	{
		m_pFrameCurve->clearCurves();
	}
}

void QFrameMemory::setMemoryDevice(const QVector<PhysicalMemory>& memory)
{
	m_vecPhysicalMemory=memory;
	m_pModelPhysicalMemory->setRowCount(0);

	showPhysicalMemory();

	comboBoxMemoryDevice->setCurrentIndex(0);
}

void QFrameMemory::showPhysicalMemory()
{
	for (int nIndex=0; nIndex<m_vecPhysicalMemory.size(); nIndex++)
	{
		const PhysicalMemory& memory=m_vecPhysicalMemory[nIndex];

		m_pModelPhysicalMemory->insertRow(nIndex);

		m_pModelPhysicalMemory->itemFromIndex(m_pModelPhysicalMemory->index(nIndex, 0))->setEditable(false);
		m_pModelPhysicalMemory->setData(m_pModelPhysicalMemory->index(nIndex, 0), QIcon(":/images/memory.png"), Qt::DecorationRole);
		m_pModelPhysicalMemory->setData(m_pModelPhysicalMemory->index(nIndex, 0), memory.name);
		m_pModelPhysicalMemory->setData(m_pModelPhysicalMemory->index(nIndex, 0), nIndex, Qt::UserRole);
	}
}

void QFrameMemory::setSystemMemory(const SystemMemory& memory)
{
	m_memory=memory;

	showSystemMemory();
}

void QFrameMemory::showSystemMemory()
{
	if (m_memory.totalVisibleMemorySize != -1)
	{
		labelTotal->setText(formatNumberString(QString::number(m_memory.totalVisibleMemorySize)));
	}
	else
	{
		labelTotal->setText("");
	}
	
	if (m_memory.freePhysicalMemory != -1)
	{
		labelFree->setText(formatNumberString(QString::number(m_memory.freePhysicalMemory)));
	}
	else
	{
		labelFree->setText("");
	}

	if (m_memory.systemCache != -1)
	{
		labelSystemCache->setText(formatNumberString(QString::number(m_memory.systemCache)));
	}
	else
	{
		labelSystemCache->setText("");
	}

	if (m_memory.totalVirtualMemorySize != -1)
	{
		labelVirtualTotal->setText(formatNumberString(QString::number(m_memory.totalVirtualMemorySize)));
	}
	else
	{
		labelVirtualTotal->setText("");
	}

	if (m_memory.freeVirtualMemory !=-1)
	{
		labelVirtualFree->setText(formatNumberString(QString::number(m_memory.freeVirtualMemory)));
	}
	else
	{
		labelVirtualFree->setText("");
	}
	
	if (m_memory.poolPaged != -1 && m_memory.poolNonpaged != -1)
	{
		labelCoreTotal->setText(formatNumberString(QString::number(m_memory.poolPaged+m_memory.poolNonpaged)));
	}
	else
	{
		labelCoreTotal->setText("");
	}

	if (m_memory.poolPaged != -1)
	{
		labelPaged->setText(formatNumberString(QString::number(m_memory.poolPaged)));
	}
	else
	{
		labelPaged->setText("");
	}

	if (m_memory.poolNonpaged != -1)
	{
		labelUnpaged->setText(formatNumberString(QString::number(m_memory.poolNonpaged)));
	}
	else
	{
		labelUnpaged->setText("");
	}

	if (m_memory.sizeUsageInPagingFiles != -1)
	{
		labelUsageTotal->setText(formatNumberString(QString::number(m_memory.sizeUsageInPagingFiles)));
	}
	else
	{
		labelUsageTotal->setText("");
	}

	if (m_memory.sizeStoredInPagingFiles != -1)
	{
		labelMemoryLimit->setText(formatNumberString(QString::number(m_memory.sizeStoredInPagingFiles)));
	}
	else
	{
		labelMemoryLimit->setText("");
	}

	if (m_memory.sizeUsageInPagingFilesPeak != -1)
	{
		labelMemoryPeak->setText(formatNumberString(QString::number(m_memory.sizeUsageInPagingFilesPeak)));
	}
	else
	{
		labelMemoryPeak->setText("");
	}

	if (m_pFrameCurve != NULL)
	{
		if (!m_pFrameCurve->containsCurve(tr("PF Usage")))
		{
			QString strColorName=QString("framecurve/curveColor%1").arg(0);
			m_pFrameCurve->addCurve(tr("PF Usage"), parseConfigColor(getAppParentDir()+INI_FILE, strColorName, QColor(0, 255, 0)));
			m_pFrameCurve->setMaxValue(m_memory.sizeStoredInPagingFiles);
		}

		m_pFrameCurve->setCurveValue(tr("PF Usage"), m_memory.sizeUsageInPagingFiles);
	}
}

void QFrameMemory::currentIndexChanged(int index)
{
	if (index >= 0)
	{
		int nIndex=m_pModelPhysicalMemory->data(m_pModelPhysicalMemory->index(index, 0), Qt::UserRole).toInt();
		if (nIndex >= m_vecPhysicalMemory.size())
		{
			return;
		}

		const PhysicalMemory& memory=m_vecPhysicalMemory[nIndex];
		labelLogo->setPixmap(findPixmap(":/images/ddr.png"));

		QString strCapacity;
		if (memory.capacity != -1)
		{
			strCapacity=sizeToString(memory.capacity);
		}
		else
		{
			strCapacity=tr("Unknown");
		}

		QString strDataWidth;
		if (memory.dataWidth == (quint16)-1)
		{
			strDataWidth=tr("Unknown");
		}
		else if (memory.dataWidth == 0)
		{
			strDataWidth=tr("Error Correction Bits");
		}
		else
		{
			strDataWidth=QString::number(memory.dataWidth)+" "+tr("bits");
		}

		lineEditDescription->setText(memory.description);
		lineEditVersion->setText(memory.version.isEmpty()? tr("Unknown"):memory.version);
		lineEditManufacturer->setText(memory.manufacturer.isEmpty()? tr("Unknown"):memory.manufacturer);
		lineEditMemoryType->setText(memory.memoryType);
		lineEditCapacity->setText(strCapacity);
		lineEditDataWidth->setText(strDataWidth);
		lineEditSpeed->setText((memory.speed == -1 || memory.speed == 0)? tr("Unknown"):(QString::number(memory.speed)+" "+tr("ns")));
		lineEditTag->setText(memory.tag);
		lineEditSerialNumber->setText(memory.serialNumber.isEmpty()? tr("Unknown"):memory.serialNumber);
	}
	else
	{
		labelLogo->setPixmap(QPixmap());

		lineEditDescription->setText(tr("Unknown"));
		lineEditVersion->setText(tr("Unknown"));
		lineEditManufacturer->setText(tr("Unknown"));
		lineEditMemoryType->setText(tr("Unknown"));
		lineEditCapacity->setText(tr("Unknown"));
		lineEditDataWidth->setText(tr("Unknown"));
		lineEditSpeed->setText(tr("Unknown"));
		lineEditTag->setText(tr("Unknown"));
		lineEditSerialNumber->setText(tr("Unknown"));
	}
}
