#include "qframeh9000process.h"

#include <QMessageBox>

#include "qmessageparser.h"
#include "qdlgnewtask.h"
#include "qnumberdelegate.h"

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

	QNumberDelegate* pNumberDelegate=new QNumberDelegate(this);

	treeViewProcess->setRootIsDecorated(false);
	treeViewProcess->setAlternatingRowColors(true);
	treeViewProcess->setModel(m_pSortModel);
	treeViewProcess->setSortingEnabled(true);
	treeViewProcess->sortByColumn(0, Qt::AscendingOrder);
	treeViewProcess->setItemDelegate(pNumberDelegate);

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

void QFrameH9000Process::setProcess(QVector<SystemProcess>& process, quint64 memory)
{
	m_vecProcess=process;
	m_nPhysicalMemory=memory;

	showProcess();
}

void QFrameH9000Process::showProcess()
{
	m_pModel->setRowCount(m_vecProcess.size());
	
	qreal dUsage=0.0;
	qreal dMemory=0.0;
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
			m_pModel->setData(m_pModel->index(i, 2), process.percentProcessorTime);
		}

		m_pModel->itemFromIndex(m_pModel->index(i, 3))->setEditable(false);
		if (process.sessionId != -1)
		{
			m_pModel->setData(m_pModel->index(i, 3), process.sessionId);
		}

		m_pModel->itemFromIndex(m_pModel->index(i, 4))->setEditable(false);
		m_pModel->itemFromIndex(m_pModel->index(i, 4))->setTextAlignment(Qt::AlignRight);
		m_pModel->setData(m_pModel->index(i, 4), process.workingSetSize);

		m_pModel->itemFromIndex(m_pModel->index(i, 5))->setEditable(false);
		m_pModel->setData(m_pModel->index(i, 5), process.usrName);

		m_pModel->itemFromIndex(m_pModel->index(i, 6))->setEditable(false);
		m_pModel->setData(m_pModel->index(i, 6), process.executablePath);

		dUsage+=process.percentProcessorTime;
		dMemory+=process.workingSetSize;
	}

	if (m_nPhysicalMemory > 0)
	{
		dMemory/=m_nPhysicalMemory;
	}
	else
	{
		dMemory=0.0;
	}

	labelNumProcesses->setText(QString::number(m_vecProcess.size()));
	labelCPUUsage->setText(QString::number(dUsage, 'f', 2)+"%");
	labelPhysicalMemory->setText(QString::number(dMemory*100, 'f', 2)+"%");
}
