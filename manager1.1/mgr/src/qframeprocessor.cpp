#include "qframeprocessor.h"

#include <math.h>

#include "common.h"
#include "qmessageparser.h"

QFrameProcessor::QFrameProcessor(QWidget *parent)
	: QFrame(parent)
{
	setupUi(this);

	m_pGridLayout=new QGridLayout(groupBoxRecords);
	groupBoxRecords->setLayout(m_pGridLayout);

	m_pModel=new QStandardItemModel(0, 1, this);
	comboBoxProcessor->setModel(m_pModel);

	connect(comboBoxProcessor, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
}

QFrameProcessor::~QFrameProcessor()
{

}

void QFrameProcessor::clear()
{
	m_pModel->setRowCount(0);

	if (m_pGridLayout != NULL)
	{
		foreach (QFrameCurve* pFrameCurve, m_mapFrameCurve)
		{
			if (pFrameCurve != NULL)
			{
				pFrameCurve->clearCurves();
			}
		}
	}
}

void QFrameProcessor::setPerfProcessor(const PerfProcessor& processor)
{
	m_perfProcessor=processor;

	showPerfProcessor();
}

void QFrameProcessor::showPerfProcessor()
{
	Q_ASSERT_NO_NULL(m_pGridLayout);

	if (m_mapFrameCurve.size() == 0)
	{
		int row,column;
		int nNumRows=(int)ceil(sqrt((double)m_perfProcessor.core.size()));
		for(int i=0; i<m_perfProcessor.core.size(); i++)
		{
			const PerfProcessorCore& core=m_perfProcessor.core[i];
			row=i%nNumRows;
			column=i/nNumRows;

			QFrameCurve* pFrameCurve=new QFrameCurve(groupBoxRecords);
			m_pGridLayout->addWidget(pFrameCurve, row, column);

			m_mapFrameCurve.insert(core.name, pFrameCurve);
		}
	}

	for(int i=0; i<m_perfProcessor.core.size(); i++)
	{
		const PerfProcessorCore& core=m_perfProcessor.core[i];
		if (!m_mapFrameCurve.contains(core.name))
		{
			continue;
		}

		QFrameCurve* pFrameCurve=m_mapFrameCurve[core.name];
		if (pFrameCurve != NULL)
		{
			if (!pFrameCurve->containsCurve(core.name))
			{
				QString strColorName=QString("framecurve/curveColor%1").arg(i);
				pFrameCurve->addCurve(core.name, parseConfigColor(getAppParentDir()+INI_FILE, strColorName, QColor(0, 255, 0)));
			}
			pFrameCurve->setCurveValue(core.name, core.percentProcessorTime);
		}
	}
}

void QFrameProcessor::setProcessors(const QVector<ProcessorInfo>& processors)
{
	m_vecProcessor=processors;
	m_pModel->setRowCount(0);

	showProcessors();
}

void QFrameProcessor::showProcessors()
{
	for (int nIndex=0; nIndex<m_vecProcessor.size(); nIndex++)
	{
		const ProcessorInfo& processor=m_vecProcessor[nIndex];

		m_pModel->insertRow(nIndex);

		m_pModel->itemFromIndex(m_pModel->index(nIndex, 0))->setEditable(false);
		m_pModel->setData(m_pModel->index(nIndex, 0), QIcon(":/images/cpu.png"), Qt::DecorationRole);
		m_pModel->setData(m_pModel->index(nIndex, 0), processor.deviceID);
		m_pModel->setData(m_pModel->index(nIndex, 0), nIndex, Qt::UserRole);
	}
}

void QFrameProcessor::currentIndexChanged(int index)
{
	if (index >= 0)
	{
		int nIndex=m_pModel->data(m_pModel->index(index, 0), Qt::UserRole).toInt();
		if (nIndex >= m_vecProcessor.size())
		{
			return;
		}

		const ProcessorInfo& processor=m_vecProcessor[nIndex];

		if (processor.manufacturer == "GenuineIntel")
		{
			labelLogo->setPixmap(findPixmap(":/images/intel.png"));
		}
		else if (processor.manufacturer == "AuthenticAMD")
		{
			labelLogo->setPixmap(findPixmap(":/images/amd.png"));
		}
		else
		{
			labelLogo->setPixmap(findPixmap(":/images/processor.png"));
		}

		lineEditProcessorName->setText(processor.name.isEmpty()? tr("Unknown"):processor.name);
		lineEditManufacturer->setText(processor.manufacturer.isEmpty()? tr("Unknown"):processor.manufacturer);
		lineEditAddressWidth->setText(processor.addressWidth==-1? tr("Unknown"):QString::number(processor.addressWidth)+" "+tr("bits"));
		lineEditDataWidth->setText(processor.dataWidth==-1? tr("Unknown"):QString::number(processor.dataWidth)+" "+tr("bits"));
		lineEditMaxClockSpeed->setText(processor.maxClockSpeed==-1? tr("Unknown"):QString::number(processor.maxClockSpeed)+" "+ tr("MHz"));
		lineEditExternalClock->setText(processor.externalClock==-1? tr("Unknown"):QString::number(processor.externalClock)+" "+ tr("MHz"));
		lineEditL2CacheSize->setText(processor.L2CacheSize==-1? tr("Unknown"):sizeToString(processor.L2CacheSize));
		lineEditL2CacheSpeed->setText(processor.L2CacheSpeed==-1? tr("Unknown"):QString::number(processor.L2CacheSpeed)+" "+ tr("MHz"));
		lineEditDescription->setText(processor.description);

		switch (processor.os)
		{
		case OS_WINDOWS:
		case OS_SOLARIS:
			{
				textEditDeviceStatus->setText(configManagerErrorCode(processor.configManager));
			}
			break;
		case OS_AIX:
			{
				textEditDeviceStatus->setText(processor.status);
			}
			break;
		default:
			{
				textEditDeviceStatus->setText("");
			}
			break;
		}
	}
	else
	{
		labelLogo->setPixmap(QPixmap());
		lineEditProcessorName->setText(tr("Unknown"));
		lineEditManufacturer->setText(tr("Unknown"));
		lineEditAddressWidth->setText(tr("Unknown"));
		lineEditDataWidth->setText(tr("Unknown"));
		lineEditMaxClockSpeed->setText(tr("Unknown"));
		lineEditExternalClock->setText(tr("Unknown"));
		lineEditL2CacheSize->setText(tr("Unknown"));
		lineEditL2CacheSpeed->setText(tr("Unknown"));
		lineEditDescription->setText(tr("Unknown"));

		textEditDeviceStatus->clear();
	}
}
