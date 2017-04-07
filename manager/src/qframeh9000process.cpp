#include "qframeh9000process.h"

#include <QMessageBox>

#include "common.h"
#include "qmessageparser.h"
#include "qmessageprocess.h"
#include "qdlgnewtask.h"

QFrameH9000Process::QFrameH9000Process(QWidget *parent)
	: QFrame(parent)
{
	setupUi(this);

	QStringList processesLabels;
	processesLabels<<tr("Name")<<tr("Process ID")<<tr("CPU")<<tr("Session ID")<<tr("Memory Usage")<<tr("User Name")<<tr("Path");

	m_pModel=new QStandardItemModel(0, processesLabels.size(), this);
	m_pModel->setHorizontalHeaderLabels(processesLabels);

	m_pSortModel=new QSortFilterProxyModel;
	m_pSortModel->setDynamicSortFilter(true);
	m_pSortModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	m_pSortModel->setSourceModel(m_pModel);

	treeViewProcess->setRootIsDecorated(false);
	treeViewProcess->setAlternatingRowColors(true);
	treeViewProcess->setModel(m_pSortModel);
	treeViewProcess->setSortingEnabled(true);

 	treeViewProcess->setColumnWidth(0, 150);
 	treeViewProcess->setColumnWidth(1, 100);
 	treeViewProcess->setColumnWidth(2, 50);
 	treeViewProcess->setColumnWidth(3, 100);
 	treeViewProcess->setColumnWidth(4, 100);
 	treeViewProcess->setColumnWidth(5, 150);
 	treeViewProcess->setColumnWidth(6, 300);
}

QFrameH9000Process::~QFrameH9000Process()
{

}

void QFrameH9000Process::setProcess(QVector<SystemProcess>& process)
{
	m_vecProcess=process;

	showProcess();
}

void QFrameH9000Process::showProcess()
{
	m_pModel->setRowCount(m_vecProcess.size());
	
	qreal dUsage=0.0;
	for (int i=0; i<m_vecProcess.size(); i++)
	{
		const SystemProcess& process=m_vecProcess[i];

		m_pModel->itemFromIndex(m_pModel->index(i, 0))->setEditable(false);
		m_pModel->setData(m_pModel->index(i, 0), process.name);

		m_pModel->itemFromIndex(m_pModel->index(i, 1))->setEditable(false);
		if (process.processId != -1)
		{
			m_pModel->setData(m_pModel->index(i, 1), process.processId);
		}
		
		m_pModel->itemFromIndex(m_pModel->index(i, 2))->setEditable(false);
		if (process.percentProcessorTime != -1)
		{
			m_pModel->setData(m_pModel->index(i, 2), QString::number(process.percentProcessorTime, 'f', 2));
		}

		m_pModel->itemFromIndex(m_pModel->index(i, 3))->setEditable(false);
		if (process.sessionId != -1)
		{
			m_pModel->setData(m_pModel->index(i, 3), process.sessionId);
		}

		m_pModel->itemFromIndex(m_pModel->index(i, 4))->setEditable(false);
		m_pModel->itemFromIndex(m_pModel->index(i, 4))->setTextAlignment(Qt::AlignRight);
		if (process.workingSetSize != -1)
		{
			if (process.workingSetSize > 1024)
			{
				m_pModel->setData(m_pModel->index(i, 4), formatNumberString(QString::number((process.workingSetSize+1023)/1024))+" K");
			}
			else
			{
				m_pModel->setData(m_pModel->index(i, 4), formatNumberString(QString::number(process.workingSetSize))+" B");
			}
		}

		m_pModel->itemFromIndex(m_pModel->index(i, 5))->setEditable(false);
		m_pModel->setData(m_pModel->index(i, 5), process.usrName);

		m_pModel->itemFromIndex(m_pModel->index(i, 6))->setEditable(false);
		m_pModel->setData(m_pModel->index(i, 6), process.executablePath);

		dUsage+=process.percentProcessorTime;
	}

	labelProcesses->setText(tr("Processes: ")+QString::number(m_vecProcess.size()));
	labelCpuUsage->setText(tr("CPU Usage: ")+QString::number(dUsage, 'f', 2)+"%");
}
