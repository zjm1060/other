#include "qframesystemprocess.h"

#include <QMessageBox>

#include "common.h"
#include "qmessageparser.h"
#include "qdlgnewtask.h"
#include "qnumberdelegate.h"
#include "qreceiverparser.h"
#include "qglobalinfo.h"
#include "qinfomanager.h"

QFrameSystemProcess::QFrameSystemProcess(QClient* client, QWidget *parent)
	: QFrame(parent)
{
	m_pClient=client;

	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	setupUi(this);

	QStringList processesLabels;
	processesLabels<<tr("Name")<<tr("Process ID")<<tr("CPU")<<tr("Session ID")<<tr("Memory Usage")<<tr("User Name")<<tr("Path");

	m_pModel=new QStandardItemModel(0, processesLabels.size(), this);
	m_pModel->setHorizontalHeaderLabels(processesLabels);

	m_pSortModel=new QSortFilterProxyModel;
	m_pSortModel->setDynamicSortFilter(true);
	m_pSortModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	m_pSortModel->setSourceModel(m_pModel);

	QNumberDelegate* pDelegate=new QNumberDelegate(this);

	treeViewProcess->setRootIsDecorated(false);
	treeViewProcess->setAlternatingRowColors(true);
	treeViewProcess->setModel(m_pSortModel);
	treeViewProcess->setSortingEnabled(true);
	treeViewProcess->sortByColumn(0, Qt::AscendingOrder);
	treeViewProcess->setItemDelegate(pDelegate);

 	treeViewProcess->setColumnWidth(0, 150);
 	treeViewProcess->setColumnWidth(1, 100);
 	treeViewProcess->setColumnWidth(2, 50);
 	treeViewProcess->setColumnWidth(3, 100);
 	treeViewProcess->setColumnWidth(4, 100);
 	treeViewProcess->setColumnWidth(5, 150);
 	treeViewProcess->setColumnWidth(6, 300);

	connect(pushButtonNewTask, SIGNAL(clicked()), this, SLOT(createNewTask()));
	connect(pushButtonEndProcess, SIGNAL(clicked()), this, SLOT(terminateTask()));

	treeViewProcess->installEventFilter(this);
}

QFrameSystemProcess::~QFrameSystemProcess()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);

	treeViewProcess->removeEventFilter(this);
}

void QFrameSystemProcess::setSystemProcess(const QVector<SystemProcess>& process, quint64 memory)
{
	m_vecProcess=process;
	m_nPhysicalMemory=memory;

	showSystemProcess();
}

void QFrameSystemProcess::showSystemProcess()
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

	labelNumProcesses->setText(QString::number(m_vecProcess.size()));
	labelCPUUsage->setText(QString::number(dUsage, 'f', 2)+"%");
	labelPhysicalMemory->setText(QString::number(dMemory*100, 'f', 2)+"%");
}

void QFrameSystemProcess::createNewTask()
{
	Q_ASSERT_NO_NULL(m_pClient);

	if (!QGlobalInfo::instance()->hasPrivileges(ProcessPrivilege, CreateProcessPrivilege))
	{
		QMessageBox::warning(this, tr("Warning"), tr("You do not have sufficient permissions to run a new task!"),
			QMessageBox::Ok, QMessageBox::Ok);

		return;
	}

	QDlgNewTask dialog(m_pClient, this);
	dialog.setModal(true);
	if (dialog.exec() != 0)
	{
		QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_SYSTEM_PROCESS);
		if (pMsg != NULL)
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QStringList arguments=dialog.task();

			QString strText;
			QStringListIterator it(arguments);
			while (it.hasNext())
			{
				strText+=it.next()+" ";
			}
			QInfoManager::instance()->append(QInfoManager::Important, tr("Try to create the new task:")+strText);

			stream<<arguments;

			buffer=pMsg->query(QueryCreateProcess, m_nUsrID, buffer);

			m_pClient->sendMessage(buffer, MSG_SYSTEM_PROCESS);
		}
	}
}

void QFrameSystemProcess::terminateTask()
{
	Q_ASSERT_NO_NULL(m_pClient);

	QItemSelectionModel* pModel=treeViewProcess->selectionModel();
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

	if (!QGlobalInfo::instance()->hasPrivileges(ProcessPrivilege, TerminateProcessPrivilege))
	{
		QMessageBox::warning(this, tr("Warning"), tr("You do not have sufficient permissions to terminate the process!"),
			QMessageBox::Ok, QMessageBox::Ok);

		return;
	}

	QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"), tr("Are you sure to terminate the process?"),
		QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);

	if (button == QMessageBox::Ok)
	{
		QString strName=m_pSortModel->data(m_pSortModel->index(treeViewProcess->currentIndex().row(), 0)).toString();
		quint32 nProcessId=m_pSortModel->data(m_pSortModel->index(treeViewProcess->currentIndex().row(), 1)).toUInt();

		QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_SYSTEM_PROCESS);
		if (pMsg != NULL)
		{
			QInfoManager::instance()->append(QInfoManager::Important, tr("Try to terminate the process:%1 pid=%2").arg(strName).arg(nProcessId));

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			
			stream<<nProcessId;

			buffer=pMsg->query(QueryTerminateProcess, -1, buffer);

			m_pClient->sendMessage(buffer, MSG_SYSTEM_PROCESS);
		}
	}
}

void QFrameSystemProcess::recvCreateProcess(const QStringList& command, int result)
{
	QString strCommand;
	foreach (QString argument, command)
	{
		strCommand+=argument+" ";
	}

	if (result != ExecMethodSucceed)
	{
		QInfoManager::instance()->append(QInfoManager::Error, tr("Failed to create the task:%1").arg(strCommand));

		QString strText=tr("Failed to create the process")+" \""+strCommand+"\"!"+"\n"+tr("Please check your command, or you have insufficient privilege!");
		QMessageBox::information(this, tr("Create Process"), strText, QMessageBox::Ok, QMessageBox::Ok);
	}
	else
	{
		QInfoManager::instance()->append(QInfoManager::Important, tr("Create the task successfully:%1").arg(strCommand));
	}
}

void QFrameSystemProcess::recvTerminateProcess(quint32 pid, int result)
{
	if (result != ExecMethodSucceed)
	{
		QInfoManager::instance()->append(QInfoManager::Error, tr("Failed to terminate the process: pid=%1").arg(pid));

		QString strText=tr("Failed to terminate the process ")+" pid="+QString::number(pid)+"!"+"\n"+tr("Please check your privilege!");
		QMessageBox::information(this, tr("Create Process"), strText, QMessageBox::Ok, QMessageBox::Ok);
	}
	else
	{
		QInfoManager::instance()->append(QInfoManager::Important, tr("Terminate the process successfully: pid=%1").arg(pid));
	}
}

bool QFrameSystemProcess::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == treeViewProcess)
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
