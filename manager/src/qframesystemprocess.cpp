#include "qframesystemprocess.h"

#include <QMessageBox>

#include "common.h"
#include "qmessageparser.h"
#include "qmessageprocess.h"
#include "qdlgnewtask.h"

QFrameSystemProcess::QFrameSystemProcess(QClient* client, QWidget *parent)
	: QFrame(parent)
{
	m_pClient=client;

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

	connect(pushButtonNewTask, SIGNAL(clicked()), this, SLOT(createNewTask()));
	connect(pushButtonEndProcess, SIGNAL(clicked()), this, SLOT(terminateTask()));
}

QFrameSystemProcess::~QFrameSystemProcess()
{

}

void QFrameSystemProcess::setSystemProcess(QVector<SystemProcess>& process)
{
	m_vecProcess=process;

	showSystemProcess();
}

void QFrameSystemProcess::showSystemProcess()
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

		if (process.processId != 0)
		{
			dUsage+=process.percentProcessorTime;
		}
	}

	labelProcesses->setText(tr("Processes: ")+QString::number(m_vecProcess.size()));
	labelCpuUsage->setText(tr("CPU Usage: ")+QString::number(dUsage, 'f', 2)+"%");
}

void QFrameSystemProcess::createNewTask()
{
	Q_ASSERT_NO_NULL(m_pClient);

	QDlgNewTask dialog(m_pClient, this);
	dialog.setModal(true);
	if (dialog.exec() != 0)
	{
		QString strText=dialog.text();

		QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageProcess::Type);
		if (pMsg != NULL)
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);
			stream<<strText;

			buffer=pMsg->query(QMessageProcess::QueryCreateProcess, -1, buffer);

			m_pClient->sendMessage(buffer, QMessageProcess::Type);
		}
	}
}

void QFrameSystemProcess::terminateTask()
{
	Q_ASSERT_NO_NULL(m_pClient);

	QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"), tr("Are you sure to terminate the process?"),
		QMessageBox::No | QMessageBox::Yes, QMessageBox::No);

	if (button == QMessageBox::Yes)
	{
		quint32 nProcessId=m_pSortModel->data(m_pSortModel->index(treeViewProcess->currentIndex().row(), 1)).toUInt();

		QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageProcess::Type);
		if (pMsg != NULL)
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);
			stream<<nProcessId;

			buffer=pMsg->query(QMessageProcess::QueryTerminateProcess, -1, buffer);

			m_pClient->sendMessage(buffer, QMessageProcess::Type);
		}
	}
}
