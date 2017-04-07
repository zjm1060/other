#include "qframeprocesspanel.h"

#include <QMessageBox>

#include "ui_processes.h"
#include "common.h"
#include "qnumberdelegate.h"
#include "qipcsocket.h"
#include "qdlgnewtask.h"

QFrameProcessPanel::QFrameProcessPanel(QWidget *parent)
	: QFrame(parent)
{
	m_ui=new Ui_FrameProcess;
	m_ui->setupUi(this);

	QStringList processesLabels;
	processesLabels<<tr("Name")<<tr("Process ID")<<tr("CPU")<<tr("Session ID")<<tr("Memory Usage")<<tr("User Name")<<tr("Path");

	m_pModel=new QStandardItemModel(0, processesLabels.size(), this);
	m_pModel->setHorizontalHeaderLabels(processesLabels);

	m_pSortModel=new QSortFilterProxyModel;
	m_pSortModel->setDynamicSortFilter(true);
	m_pSortModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	m_pSortModel->setSourceModel(m_pModel);

	QNumberDelegate* pDelegate=new QNumberDelegate(this);

	m_ui->treeViewProcess->setRootIsDecorated(false);
	m_ui->treeViewProcess->setAlternatingRowColors(true);
	m_ui->treeViewProcess->setModel(m_pSortModel);
	m_ui->treeViewProcess->setSortingEnabled(true);
	m_ui->treeViewProcess->sortByColumn(0, Qt::AscendingOrder);
	m_ui->treeViewProcess->setItemDelegate(pDelegate);

 	m_ui->treeViewProcess->setColumnWidth(NameTitle, 150);
 	m_ui->treeViewProcess->setColumnWidth(ProcessIDTitle, 100);
 	m_ui->treeViewProcess->setColumnWidth(CpuTitle, 50);
 	m_ui->treeViewProcess->setColumnWidth(SessionIDTitle, 100);
 	m_ui->treeViewProcess->setColumnWidth(MemoryUsedTitle, 100);
 	m_ui->treeViewProcess->setColumnWidth(UserNameTitle, 150);
 	m_ui->treeViewProcess->setColumnWidth(PathTitle, 300);

	connect(m_ui->pushButtonNewTask, SIGNAL(clicked()), this, SLOT(createNewTask()));
	connect(m_ui->pushButtonEndProcess, SIGNAL(clicked()), this, SLOT(terminateTask()));

	m_ui->treeViewProcess->installEventFilter(this);
}

QFrameProcessPanel::~QFrameProcessPanel()
{
	QAbstractItemDelegate* pDelegate=m_ui->treeViewProcess->itemDelegate();
	if (pDelegate != NULL)
	{
		SAFE_DELETE_CLASS(pDelegate);
	}

	m_ui->treeViewProcess->removeEventFilter(this);

	SAFE_DELETE_CLASS(m_ui);
}

void QFrameProcessPanel::setSystemProcess(const QVector<SystemProcess>& process, quint64 memory)
{
	m_vecProcess=process;
	m_nPhysicalMemory=memory;

	showSystemProcess();
}

void QFrameProcessPanel::showSystemProcess()
{
	m_pModel->setRowCount(m_vecProcess.size());
	
	qreal dUsage=0.0;
	qreal dMemory=0.0;
	for (int i=0; i<m_vecProcess.size(); i++)
	{
		const SystemProcess& process=m_vecProcess[i];

		m_pModel->itemFromIndex(m_pModel->index(i, NameTitle))->setEditable(false);
		m_pModel->setData(m_pModel->index(i, NameTitle), process.name);

		m_pModel->itemFromIndex(m_pModel->index(i, ProcessIDTitle))->setEditable(false);
		if (process.processId != -1)
		{
			m_pModel->setData(m_pModel->index(i, ProcessIDTitle), process.processId);
		}
		
		m_pModel->itemFromIndex(m_pModel->index(i, CpuTitle))->setEditable(false);
		if (process.percentProcessorTime != -1)
		{
			m_pModel->setData(m_pModel->index(i, CpuTitle), process.percentProcessorTime);
		}

		m_pModel->itemFromIndex(m_pModel->index(i, SessionIDTitle))->setEditable(false);
		if (process.sessionId != -1)
		{
			m_pModel->setData(m_pModel->index(i, SessionIDTitle), process.sessionId);
		}

		m_pModel->itemFromIndex(m_pModel->index(i, MemoryUsedTitle))->setEditable(false);
		m_pModel->itemFromIndex(m_pModel->index(i, MemoryUsedTitle))->setTextAlignment(Qt::AlignRight);
		m_pModel->setData(m_pModel->index(i, MemoryUsedTitle), process.workingSetSize);

		m_pModel->itemFromIndex(m_pModel->index(i, UserNameTitle))->setEditable(false);
		m_pModel->setData(m_pModel->index(i, UserNameTitle), process.usrName);

		m_pModel->itemFromIndex(m_pModel->index(i, PathTitle))->setEditable(false);
		m_pModel->setData(m_pModel->index(i, PathTitle), process.executablePath);

		if (process.processId != 0)
		{
			dUsage+=process.percentProcessorTime;
			dMemory+=process.workingSetSize;
		}
	}

	if (m_nPhysicalMemory > 0)
	{
		dMemory/=m_nPhysicalMemory;
	}
	else
	{
		dMemory=0.0;
	}

	m_ui->labelNumProcesses->setText(QString::number(m_vecProcess.size()));
	m_ui->labelCPUUsage->setText(QString::number(dUsage, 'f', 2)+"%");
	m_ui->labelPhysicalMemory->setText(QString::number(dMemory*100, 'f', 2)+"%");
}

void QFrameProcessPanel::createNewTask()
{
	QSharedClientPtr pClient=QIpcSocket::instance()->clientSocket();
	if (pClient == NULL || pClient->state() != QAbstractSocket::ConnectedState)
	{
		QMessageBox::critical(this, tr("Error"), tr("The service process was not running!\nPlease check your network settings."), QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	QDlgNewTask dialog(pClient.get(), this);
	dialog.setModal(true);
	if (dialog.exec() != 0)
	{
		QStringList arguments=dialog.task();

		emit execute(arguments);
	}
}

void QFrameProcessPanel::terminateTask()
{
	QItemSelectionModel* pModel=m_ui->treeViewProcess->selectionModel();
	if (pModel != NULL)
	{
		QModelIndexList list=pModel->selectedRows();
		if (list.size() == 0)
		{
			return;
		}
	}
	else
	{
		return;
	}

	QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"), tr("Are you sure to terminate the process?"),
		QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);

	if (button == QMessageBox::Ok)
	{
		QString strName=m_pSortModel->data(m_pSortModel->index(m_ui->treeViewProcess->currentIndex().row(), 0)).toString();
		quint32 nProcessId=m_pSortModel->data(m_pSortModel->index(m_ui->treeViewProcess->currentIndex().row(), 1)).toUInt();

		emit terminate(strName, nProcessId);
	}
}

void QFrameProcessPanel::recvCreateProcess(const QStringList& command, int result)
{
// 	QString strCommand;
// 	foreach (QString argument, command)
// 	{
// 		strCommand+=argument+" ";
// 	}
// 
// 	if (result != ExecMethodSucceed)
// 	{
// 		QInfoManager::instance()->append(QInfoManager::Error, tr("Failed to create the task:%1").arg(strCommand));
// 
// 		QString strText=tr("Failed to create the process")+" \""+strCommand+"\"!"+"\n"+tr("Please check your command, or you have insufficient privilege!");
// 		QMessageBox::information(this, tr("Create Process"), strText, QMessageBox::Ok, QMessageBox::Ok);
// 	}
// 	else
// 	{
// 		QInfoManager::instance()->append(QInfoManager::Important, tr("Create the task successfully:%1").arg(strCommand));
// 	}
}

bool QFrameProcessPanel::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == m_ui->treeViewProcess)
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent != NULL && keyEvent->key() == Qt::Key_Delete)
			{
				terminateTask();

				return true;
			}
		}
	}

	return false;
}
