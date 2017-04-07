#include "qabstractsync.h"

#include <QDir>
#include <QFileDialog>
#include <QMouseEvent>
#include <QMessageBox>
#include <QLineEdit>

#include "common.h"
#include "extern.h"
#include "qmessagefiletransfer.h"
#include "qcomboboxfilesystem.h"
#include "qtreeviewfilesystem.h"

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#include "qwmiprocess.h"
#else
#include "qstatfactory.h"
#include "qstatprocess.h"
#endif

QAbstractSync::QAbstractSync(QWidget *parent)
	: QFrame(parent),
	m_nPopupMenuOwner(InvalidOwner),
	m_bSync(false)
{
	setupUi(this);

	labelInfoIcon->setPixmap(QPixmap(":/images/error.png"));
	showTipsPixmap(false);

	for (int i=0; i<MachineEnd; i++)
	{
		m_pFrameFileSystem[i]=new QFrameFileSystem(this);
		horizontalLayoutFileSystem->addWidget(m_pFrameFileSystem[i]);
	}

	//本地
	refreshLocalDrives();

	connect(m_pFrameFileSystem[LocalMachine], SIGNAL(clickedToolButton(int)), this, SLOT(clickedToolButtonLocal(int)));
	connect(m_pFrameFileSystem[LocalMachine], SIGNAL(currentIndexChanged(QString)),
		this, SLOT(localLocationCurrentIndexChanged(QString)));

	connect(m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem(), SIGNAL(editTextChanged(const QString&)),
		this, SLOT(localEditTextChangedLocation(const QString&)));
	connect(m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem(), SIGNAL(enterPressed(QString)), 
		this, SLOT(enterPressedLocal(QString)));
	connect(m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem(), SIGNAL(focusStatusChanged(bool)), 
		this, SLOT(localFocusStatusChanged(bool)));

	connect(m_pFrameFileSystem[LocalMachine]->treeViewFileSystem(), SIGNAL(doubleClicked(const QModelIndex &)), 
		this, SLOT(doubleClickedLocalLocation(const QModelIndex&)));
	connect(m_pFrameFileSystem[LocalMachine]->treeViewFileSystem(), SIGNAL(mouseRightButtonPress(QPoint)), 
		this, SLOT(mouseRightButtonPressLocal(QPoint)));
	connect(m_pFrameFileSystem[LocalMachine]->treeViewFileSystem(), SIGNAL(dataChanged(QModelIndex, QString, QString)), 
		this, SLOT(localDataChanged(QModelIndex, QString, QString)));
	
	//远程
	connect(m_pFrameFileSystem[RemoteMachine], SIGNAL(clickedToolButton(int)), this, SLOT(clickedToolButtonRemote(int)));
	connect(m_pFrameFileSystem[RemoteMachine], SIGNAL(currentIndexChanged(QString)),
		this, SLOT(remoteLocationCurrentIndexChanged(QString)));

	connect(m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem(), SIGNAL(editTextChanged(const QString&)),
		this, SLOT(remoteEditTextChangedLocation(const QString&)));
	connect(m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem(), SIGNAL(enterPressed(QString)),
		this, SLOT(enterPressedRemote(QString)));
	connect(m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem(), SIGNAL(focusStatusChanged(bool)),
		this, SLOT(remoteFocusStatusChanged(bool)));

	connect(m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem(), SIGNAL(doubleClicked(const QModelIndex &)), 
		this, SLOT(doubleClickedRemoteLocation(const QModelIndex&)));
	connect(m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem(), SIGNAL(mouseRightButtonPress(QPoint)), 
		this, SLOT(mouseRightButtonPressRemote(QPoint)));
	connect(m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem(), SIGNAL(dataChanged(QModelIndex, QString, QString)), 
		this, SLOT(remoteDataChanged(QModelIndex, QString, QString)));

	m_pViewTransferQueue=new QTreeViewTransferQueue(this);
	m_pViewTransferQueue->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	horizontalLayoutQueue->insertWidget(0, m_pViewTransferQueue);

	connect(m_pViewTransferQueue, SIGNAL(mouseRightButtonPress(QPoint)), this, SLOT(mouseRightButtonPressTransferQueue(QPoint)));

	m_pDelegate=new QTipsDelegate(this);
	listView->setItemDelegate(m_pDelegate);

	m_pModelTips=new QStandardItemModel(0, 1, this);
	listView->setModel(m_pModelTips);

	connect(m_pViewTransferQueue, SIGNAL(synchronize()), this, SLOT(synchronize()));

	//弹出菜单
	m_pMenu=new QMenu(this);
	m_pActions[ActionDownload]=new QAction(QIcon(":/images/menudownload.png"), tr("&Download"), m_pMenu);
	m_pActions[ActionUpload]=new QAction(QIcon(":/images/menuupload.png"), tr("&Upload"), m_pMenu);
	m_pActions[ActionOpen]=new QAction(tr("&Open"), m_pMenu);
	m_pActions[ActionDelete]=new QAction(QIcon(":/images/delete.png"), tr("&Delete"), m_pMenu);
	m_pActions[ActionRefresh]=new QAction(QIcon(":/images/refresh.png"), tr("&Refresh"), m_pMenu);
	m_pActions[ActionNewDirectory]=new QAction(QIcon(":/images/newdirectory.png"), tr("&New Directory"), m_pMenu);
	m_pActions[ActionRename]=new QAction(tr("Rename"), m_pMenu);

	m_pMenu->addAction(m_pActions[ActionDownload]);
	m_pMenu->addAction(m_pActions[ActionUpload]);
	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionOpen]);
	m_pMenu->addAction(m_pActions[ActionDelete]);

	m_pMenuFormat=new QMenu(tr("Format"), m_pMenu);
	m_pActionsFormat[TextFormat]=new QAction(tr("Text"), m_pMenuFormat);
	m_pActionsFormat[BinaryFormat]=new QAction(tr("Binary"), m_pMenuFormat);
	m_pActionsFormat[AutoFormat]=new QAction(tr("Auto"), m_pMenuFormat);

	m_pMenuFormat->addAction(m_pActionsFormat[TextFormat]);
	m_pMenuFormat->addAction(m_pActionsFormat[BinaryFormat]);
	m_pMenuFormat->addSeparator();
	m_pMenuFormat->addAction(m_pActionsFormat[AutoFormat]);

	m_pGroupFormat=new QActionGroup(m_pMenuFormat);
	for (int i=0; i<FormatEnd; i++)
	{
		m_pActionsFormat[i]->setCheckable(true);
		m_pActionsFormat[i]->setData(i);
		m_pGroupFormat->addAction(m_pActionsFormat[i]);
	}
	m_pGroupFormat->setExclusive(true);
	m_pActionsFormat[AutoFormat]->setChecked(true);

	m_pMenu->addSeparator();
	m_pMenu->addMenu(m_pMenuFormat);

	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionNewDirectory]);
	m_pMenu->addAction(m_pActions[ActionRename]);

	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionRefresh]);

	m_pMenuTask=new QMenu(this);
	m_pActions[ActionRetransmit]=new QAction(QIcon(":/images/retransmit.png"), tr("&Retransmit"), m_pMenuTask);
	m_pActions[ActionDeleteTask]=new QAction(QIcon(":/images/delete.png"), tr("&Delete"), m_pMenuTask);
	m_pActions[ActionDeleteAllTask]=new QAction(tr("&Delete All"), m_pMenuTask);

	m_pMenuTask->addAction(m_pActions[ActionRetransmit]);
	m_pMenuTask->addAction(m_pActions[ActionDeleteTask]);
	m_pMenuTask->addSeparator();
	m_pMenuTask->addAction(m_pActions[ActionDeleteAllTask]);

	//菜单触发事件
	m_pSignalMapper=new QSignalMapper(this);
	for (int i=0; i<ActionEnd; i++)
	{
		connect(m_pActions[i], SIGNAL(triggered()), m_pSignalMapper, SLOT(map()));
		m_pSignalMapper->setMapping(m_pActions[i], i);
	}

	connect(m_pSignalMapper, SIGNAL(mapped(int)), this, SLOT(actionTriggered(int)));
}

QAbstractSync::~QAbstractSync()
{

}

void QAbstractSync::setTitleText(const QString& title)
{
	labelTitle->setText(title);
}

void QAbstractSync::setDefaultDescText(const QString& desc)
{
	m_strDefaultDesc=desc;

	labelDesc->setText(desc);
}

void QAbstractSync::setThemePixmap(const QPixmap& icon)
{
	labelIcon->setPixmap(icon);
}

void QAbstractSync::showTipsPixmap(bool show)
{
	if (show)
	{
		labelInfoIcon->show();
	}
	else
	{
		labelInfoIcon->hide();
	}
}

void QAbstractSync::setDefaultLocalLocation(const QString& location)
{
	m_strLocalHome=location;

	if (m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->topItemType(m_strLocalHome) != -1)
	{
		setLocation(LocalMachine, m_strLocalHome);
	}
	else
	{
		QDir dir(m_strLocalHome);
		if (dir.exists())
		{
			setLocation(LocalMachine, m_strLocalHome);
		}
		else
		{
			setLocation(LocalMachine, getAppParentDir());
		}
	}
}

void QAbstractSync::setLocation(int machine, const QString& location)
{
	if (location.isEmpty())
	{
		return;
	}

	switch (machine)
	{
	case LocalMachine:
		{
			m_pFrameFileSystem[LocalMachine]->setLocation(location);

			refreshLocalFileSystem();
		}
		break;
	case RemoteMachine:
		{
			gotoRemoteLocation(location);
		}
		break;
	default:
		break;
	}
}

void QAbstractSync::clickedToolButtonLocal(int type)
{
	switch (type)
	{
	case QFrameFileSystem::ButtonHome:
		{
			refreshLocalDrives();

			setDefaultLocalLocation(m_strLocalHome);
		}
		break;
	case QFrameFileSystem::ButtonUp:
		{
			QString strLocation=m_pFrameFileSystem[LocalMachine]->location();
			if (m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->topItemType(strLocation) != -1)
			{
				break;
			}

			enterPressedLocal(m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->parentLocation(strLocation));
		}
		break;
	default:
		break;
	}
}

void QAbstractSync::enterPressedLocal(QString location)
{
	setLocation(LocalMachine, location);
}

void QAbstractSync::localFocusStatusChanged(bool has)
{
	if (has)
	{
		localEditTextChangedLocation(m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->lineEdit()->text());
	}
	else
	{
		showTipsPixmap(false);

		labelDesc->setText(m_strDefaultDesc);
	}
}

void QAbstractSync::remoteFocusStatusChanged(bool has)
{
	if (has)
	{
		remoteEditTextChangedLocation(m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->lineEdit()->text());
	}
	else
	{
		showTipsPixmap(false);

		labelDesc->setText(m_strDefaultDesc);
	}
}

void QAbstractSync::clickedToolButtonRemote(int type)
{
	switch (type)
	{
	case QFrameFileSystem::ButtonHome:
		{
			gotoRemoteHome();
		}
		break;
	case QFrameFileSystem::ButtonUp:
		{
			QString strLocation=m_pFrameFileSystem[RemoteMachine]->location();
			if (m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->topItemType(strLocation) != -1)
			{
				break;
			}

			strLocation=m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->parentLocation(strLocation);
			gotoRemoteLocation(strLocation);
		}
		break;
	default:
		break;
	}
}

void QAbstractSync::enterPressedRemote(QString location)
{
	gotoRemoteLocation(location);
}

void QAbstractSync::localEditTextChangedLocation(const QString& location)
{
	QString strLocation=QDir::fromNativeSeparators(location);

	QChar ch;
	if (!strLocation.isEmpty() && m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->topItemType(strLocation) == -1)
	{
		if (hasInvalidChar(strLocation, &ch))
		{
			showTipsPixmap(true);

			labelDesc->setText(QString(ch)+" "+tr("is an invalid character in your local location."));
		}
		else
		{
			QDir dir(strLocation);
			if (!dir.exists())
			{
				showTipsPixmap(true);

				labelDesc->setText(tr("The location doesn't exist."));
			}
			else
			{
				showTipsPixmap(false);

				labelDesc->setText(tr("Enter the local target folder."));
			}
		}
	}
	else
	{
		showTipsPixmap(false);

		labelDesc->setText(tr("Enter the local target folder."));
	}
}

void QAbstractSync::remoteEditTextChangedLocation(const QString& location)
{
	QString strLocation=QDir::fromNativeSeparators(location);

	QChar ch;
	if (!strLocation.isEmpty() && 
		m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
	{
		if (hasInvalidChar(strLocation, &ch))
		{
			showTipsPixmap(true);

			labelDesc->setText(QString(ch)+" "+tr("is an invalid character in your remote location."));
		}
		else
		{
			showTipsPixmap(false);

			labelDesc->setText(tr("Enter the remote target folder."));
		}
	}
	else
	{
		showTipsPixmap(false);

		labelDesc->setText(tr("Enter the remote target folder."));
	}
}

void QAbstractSync::synchronize()
{
	if (m_bSync)
	{
		return;
	}

	QTreeViewTransferQueue::TransferQueueItem item;
	int nRow=m_pViewTransferQueue->fetchTask(item);
	if (nRow == -1)
	{
		return;
	}

	m_pModelTips->setRowCount(0);

	m_bSync=true;
	appendTip(QTipsDelegate::Information, tr("Synchronize..."));

	transfer(nRow, item);
}

void QAbstractSync::localLocationCurrentIndexChanged(QString location)
{
	setLocation(LocalMachine, location);
}

void QAbstractSync::remoteLocationCurrentIndexChanged(QString location)
{
	setLocation(RemoteMachine, location);
}

void QAbstractSync::doubleClickedLocalLocation(const QModelIndex& index)
{
	QTreeViewFileSystem* pTreeViewLocalFileSystem=m_pFrameFileSystem[LocalMachine]->treeViewFileSystem();
	QString strFileName=pTreeViewLocalFileSystem->filename(index);
	QString strLocation;
	if (strFileName == "..")
	{
		strLocation=getParentDir(m_pFrameFileSystem[LocalMachine]->location());
	}
	else
	{
		int nType=pTreeViewLocalFileSystem->type(index);

		if (nType == QMessageFileTransfer::File || nType == QMessageFileTransfer::HideFile ||
			nType == QMessageFileTransfer::SymLinkFile || nType == QMessageFileTransfer::SymLinkHideFile)
		{
			QStringList arguments;
			arguments<<appendPath(m_pFrameFileSystem[LocalMachine]->location(), strFileName);

#ifdef Q_OS_WIN
			QWmiFactory::instance()->execMethod(WMI_TYPE_PROCESS, QWmiProcess::MethodCreate, arguments);
#else
			QStatFactory::instance()->execMethod(WMI_TYPE_PROCESS, QStatProcess::MethodCreate, arguments);
#endif
			return;
		}

		if ((nType & QMessageFileTransfer::Dir) != 0 || (nType & QMessageFileTransfer::HideDir) != 0)
		{
			strLocation=appendPath(m_pFrameFileSystem[LocalMachine]->location(), strFileName);
		}
		else if ((nType & QMessageFileTransfer::Drive) != 0)
		{
			strLocation=strFileName;
		}
		
		if (nType == QMessageFileTransfer::SymLinkDir || nType == QMessageFileTransfer::SymLinkHideDir)
		{
			strLocation=QFile::symLinkTarget(strLocation);
		}
	}

	if (!strLocation.isEmpty())
	{
		setLocation(LocalMachine, strLocation);
	}
}

void QAbstractSync::doubleClickedRemoteLocation(const QModelIndex& index)
{
	QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem();
	QString strFileName=pTreeViewFileSystem->filename(index);
	QString strLocation;
	if (strFileName == "..")
	{
		strLocation=getParentDir(m_pFrameFileSystem[RemoteMachine]->location());
	}
	else
	{
		int nType=pTreeViewFileSystem->type(index);

		if (nType == QMessageFileTransfer::Dir || nType == QMessageFileTransfer::HideDir
			|| nType == QMessageFileTransfer::SymLinkDir || nType == QMessageFileTransfer::SymLinkHideDir)
		{
			strLocation=appendPath(m_pFrameFileSystem[RemoteMachine]->location(), strFileName);
		}
		else if ((nType & QMessageFileTransfer::Drive) != 0)
		{
			strLocation=strFileName;
		}
	}

	if (!strLocation.isEmpty())
	{
		setLocation(RemoteMachine, strLocation);
	}
}

void QAbstractSync::mouseRightButtonPressLocal(QPoint pos)
{
	if (m_pFrameFileSystem[LocalMachine] == NULL)
	{
		return;
	}

	QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem[LocalMachine]->treeViewFileSystem();

	QModelIndexList rows=pTreeViewFileSystem->selectedRows();

	QString strRemote=m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->currentLocation();
	QString strLocal=m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->currentLocation();

	m_pActions[ActionUpload]->setVisible(true);
	m_pActions[ActionDownload]->setVisible(false);
	if (!rows.isEmpty())
	{
		if (m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->topItemType(strRemote) != -1)
		{
			m_pActions[ActionUpload]->setEnabled(false);
		}
		else
		{
			if (rows.size() != 1)
			{
				m_pActions[ActionUpload]->setEnabled(true);
			}
			else
			{
				QString strName=pTreeViewFileSystem->filename(rows[0]);
				if (strName == "..")
				{
					m_pActions[ActionUpload]->setEnabled(false);
				}
				else
				{
					m_pActions[ActionUpload]->setEnabled(true);
				}

			}
		}

		if (m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->topItemType(strLocal) != -1)
		{
			m_pActions[ActionDelete]->setEnabled(false);
		}
		else
		{
			if (rows.size() != 1)
			{
				m_pActions[ActionDelete]->setEnabled(true);
			}
			else
			{
				QString strName=pTreeViewFileSystem->filename(rows[0]);
				if (strName == "..")
				{
					m_pActions[ActionDelete]->setEnabled(false);
				}
				else
				{
					m_pActions[ActionDelete]->setEnabled(true);
				}

			}
		}
	}
	else
	{
		m_pActions[ActionUpload]->setEnabled(false);
		m_pActions[ActionDelete]->setEnabled(false);
	}

	if (m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->topItemType(strLocal) != -1)
	{
		m_pActions[ActionNewDirectory]->setEnabled(false);
	}
	else
	{
		m_pActions[ActionNewDirectory]->setEnabled(true);
	}

	if (rows.size() != 1)
	{
		m_pActions[ActionOpen]->setEnabled(false);
	}
	else
	{
		m_pActions[ActionOpen]->setEnabled(true);
	}

	if (rows.size() != 1)
	{
		m_pActions[ActionRename]->setEnabled(false);
	}
	else
	{
		if (m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->topItemType(strLocal) != -1)
		{
			m_pActions[ActionRename]->setEnabled(false);
		}
		else
		{
			QString strName=pTreeViewFileSystem->filename(rows[0]);
			if (strName == "..")
			{
				m_pActions[ActionRename]->setEnabled(false);
			}
			else
			{
				m_pActions[ActionRename]->setEnabled(true);
			}
		}
	}

	m_nPopupMenuOwner=LocalFileSystemOwner;
	m_pMenu->popup(pTreeViewFileSystem->mapToGlobal(pos));
}

void QAbstractSync::mouseRightButtonPressRemote(QPoint pos)
{
	QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QModelIndexList rows=pTreeViewFileSystem->selectedRows();

		QString strLocal=m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->currentLocation();
		QString strRemote=m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->currentLocation();

		m_pActions[ActionUpload]->setVisible(false);
		m_pActions[ActionDownload]->setVisible(true);
		if (!rows.isEmpty())
		{
			if (m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->topItemType(strLocal) != -1)
			{
				m_pActions[ActionDownload]->setEnabled(false);
			}
			else
			{
				if (rows.size() != 1)
				{
					m_pActions[ActionDownload]->setEnabled(true);
				}
				else
				{
					QString strName=pTreeViewFileSystem->filename(rows[0]);
					if (strName == "..")
					{
						m_pActions[ActionDownload]->setEnabled(false);
					}
					else
					{
						m_pActions[ActionDownload]->setEnabled(true);
					}
				}
			}

			if (m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->topItemType(strRemote) != -1)
			{
				m_pActions[ActionDelete]->setEnabled(false);
			}
			else
			{
				if (rows.size() != 1)
				{
					m_pActions[ActionDelete]->setEnabled(true);
				}
				else
				{
					QString strName=pTreeViewFileSystem->filename(rows[0]);
					if (strName == "..")
					{
						m_pActions[ActionDelete]->setEnabled(false);
					}
					else
					{
						m_pActions[ActionDelete]->setEnabled(true);
					}
				}
			}
		}
		else
		{
			m_pActions[ActionDownload]->setEnabled(false);
			m_pActions[ActionDelete]->setEnabled(false);
		}

		if (rows.size() != 1)
		{
			m_pActions[ActionOpen]->setEnabled(false);
			m_pActions[ActionRename]->setEnabled(false);
		}
		else
		{
			int nType=pTreeViewFileSystem->type(rows[0]);
			if ((nType == QMessageFileTransfer::Drive) || (nType & QMessageFileTransfer::Dir) != 0 
				|| (nType & QMessageFileTransfer::HideDir) != 0)
			{
				m_pActions[ActionOpen]->setEnabled(true);
			}
			else
			{
				m_pActions[ActionOpen]->setEnabled(false);
			}
		}

		if (m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->topItemType(strRemote) != -1)
		{
			m_pActions[ActionNewDirectory]->setEnabled(false);
		}
		else
		{
			m_pActions[ActionNewDirectory]->setEnabled(true);
		}

		if (rows.size() != 1)
		{
			m_pActions[ActionRename]->setEnabled(false);
		}
		else
		{
			if (m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->topItemType(strRemote) != -1)
			{
				m_pActions[ActionRename]->setEnabled(false);
			}
			else
			{
				QString strName=pTreeViewFileSystem->filename(rows[0]);
				if (strName == "..")
				{
					m_pActions[ActionRename]->setEnabled(false);
				}
				else
				{
					m_pActions[ActionRename]->setEnabled(true);
				}
			}
		}

		m_nPopupMenuOwner=RemoteFileSystemOwner;
		m_pMenu->popup(pTreeViewFileSystem->mapToGlobal(pos));
	}
}

void QAbstractSync::mouseRightButtonPressTransferQueue(QPoint pos)
{
	if (m_pViewTransferQueue == NULL)
	{
		return;
	}

	QModelIndexList list=m_pViewTransferQueue->selectedRows();
	if (list.isEmpty())
	{
		m_pActions[ActionRetransmit]->setEnabled(false);
		m_pActions[ActionDeleteTask]->setEnabled(false);
	}
	else
	{
		bool bRetransmit=false;
		foreach (QModelIndex index, list)
		{
			if (m_pViewTransferQueue->itemStatus(index.row()) == QTreeViewTransferQueue::Failed)
			{
				bRetransmit=true;
				break;
			}
		}

		m_pActions[ActionRetransmit]->setEnabled(bRetransmit);
		m_pActions[ActionDeleteTask]->setEnabled(true);
	}

	if (m_pViewTransferQueue->taskCount() > 0)
	{
		m_pActions[ActionDeleteAllTask]->setEnabled(true);
	}
	else
	{
		m_pActions[ActionDeleteAllTask]->setEnabled(false);
	}

	m_nPopupMenuOwner=TransferQueueOwner;
	m_pMenuTask->popup(m_pViewTransferQueue->mapToGlobal(pos));
}

void QAbstractSync::localDataChanged(QModelIndex index, QString old, QString now)
{
	QString strLocation=m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->currentLocation();
	if (m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->topItemType(strLocation) != -1)
	{
		m_pFrameFileSystem[LocalMachine]->treeViewFileSystem()->setFileName(index, old);
	}
	else
	{
		QDir dir(appendPath(strLocation, old));
		if (dir.rename(appendPath(strLocation, old), appendPath(strLocation, now)))
		{
			m_pFrameFileSystem[LocalMachine]->treeViewFileSystem()->setFileName(index, now);

			refresh(LocalMachine);
		}
		else
		{
			m_pFrameFileSystem[LocalMachine]->treeViewFileSystem()->setFileName(index, old);
		}
	}
}

void QAbstractSync::remoteDataChanged(QModelIndex index, QString old, QString now)
{
	QString strLocation=m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->currentLocation();
	if (m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->topItemType(strLocation) != -1)
	{
		m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem()->setFileName(index, old);
	}
	else
	{
		queryRename(strLocation, index.row(), old, now);
	}
}

void QAbstractSync::localNewDirectory()
{
	QString strLocation=m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->currentLocation();
	if (m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->topItemType(strLocation) != -1)
	{
		return;
	}
	else
	{
		QDir dir(strLocation);
		QString strPath;

		int i=1;
		while (true)
		{
			strPath=appendPath(strLocation, QString(tr("New Directory %1")).arg(i));
			if (!dir.exists(strPath))
			{
				break;
			}

			i++;
		}

		QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem[LocalMachine]->treeViewFileSystem();
		if (dir.mkpath(strPath))
		{
			QModelIndex index=pTreeViewFileSystem->appendFileInfo(QFileInfo(strPath), false);
			pTreeViewFileSystem->setCurrentEdit(index);
		}
		else
		{
			appendTip(QTipsDelegate::Error, tr("Local: New directory failed! May be insufficient privileges!"));
		}
	}
}

void QAbstractSync::remoteNewDirectory()
{
	QString strLocation=m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->currentLocation();
	if (m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->topItemType(strLocation) != -1)
	{
		return;
	}
	else
	{
		queryNewDirectory(strLocation);
	}
}

void QAbstractSync::localDelDirectory()
{
	QString strLocation=m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->currentLocation();
	if (m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->topItemType(strLocation) != -1)
	{
		return;
	}
	else
	{
		QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"),
			tr("Will permanently delete these files!\nAre you sure to delete?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::No);

		if (button == QMessageBox::Yes)
		{
			QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem[LocalMachine]->treeViewFileSystem();
			QModelIndexList list=pTreeViewFileSystem->selectedRows();

			QString strName;
			bool bSuccess=true;
			foreach (QModelIndex index, list)
			{
				strName=appendPath(strLocation, pTreeViewFileSystem->filename(index));

				QFileInfo info(strName);
				if (info.isSymLink() || info.isFile())
				{
					bSuccess&=QFile::remove(strName);
				}
				else if (info.isDir())
				{
					bSuccess&=QMessageFileTransferRunnable::deleteDir(strName);
				}
			}

			if (!bSuccess)
			{
				appendTip(QTipsDelegate::Error, 
					tr("Local: Delete file(s) or file folder(s) failed! May be in use, or insufficient privileges!"));
			}

			refresh(LocalMachine);
		}
	}
}

void QAbstractSync::remoteDelDirectory()
{
	QString strLocation=m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->currentLocation();
	if (m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->topItemType(strLocation) != -1)
	{
		return;
	}
	else
	{
		QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"),
			tr("Will permanently delete these files!\nAre you sure to delete?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::No);

		if (button == QMessageBox::Yes)
		{
			QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem();
			QModelIndexList list=pTreeViewFileSystem->selectedRows();

			QStringList files;
			foreach (QModelIndex index, list)
			{
				files<<pTreeViewFileSystem->filename(index);
			}

			queryDelete(strLocation, files);
		}
	}
}

void QAbstractSync::refreshLocalDrives()
{
	QFileInfoList drives=QDir::drives();

	m_pFrameFileSystem[LocalMachine]->setDrives(drives);
}

void QAbstractSync::refreshLocalFileSystem()
{
	QString strLocation=m_pFrameFileSystem[LocalMachine]->location();

	QFileInfoList files;

	QComboBoxFileSystem* pComboBoxFileSystem=m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem();
	int nItemType;
	if ((nItemType=pComboBoxFileSystem->topItemType(strLocation)) != -1)
	{
		switch (nItemType)
		{
		case QComboBoxFileSystem::Computer:
			{
				files=QDir::drives();
			}
			break;
// 		case QComboBoxFileSystem::Trash:
// 			break;
		default:
			break;
		}
	}
	else
	{
		QDir dir(strLocation);
		if (!dir.exists())
		{
			strLocation=getAppParentDir();
			m_pFrameFileSystem[LocalMachine]->setLocation(strLocation);

			dir.setPath(strLocation);
		}

		files=dir.entryInfoList(QDir::AllEntries);
	}

	m_pFrameFileSystem[LocalMachine]->setFileInfo(files, nItemType);
}

void QAbstractSync::appendTip(int flag, const QString& tip)
{
	if (m_pModelTips == NULL)
	{
		return;
	}

	int nRow=m_pModelTips->rowCount();
	m_pModelTips->insertRow(nRow);

	m_pModelTips->itemFromIndex(m_pModelTips->index(nRow, 0))->setEditable(false);
	m_pModelTips->setData(m_pModelTips->index(nRow, 0), flag, Qt::UserRole);
	m_pModelTips->setData(m_pModelTips->index(nRow, 0), tip);

	QItemSelectionModel* pModel=listView->selectionModel();
	if (pModel != NULL && !pModel->hasSelection())
	{
		listView->scrollTo(m_pModelTips->index(nRow, 0));
	}
}

void QAbstractSync::finished()
{
	m_bSync=false;
}

void QAbstractSync::actionTriggered(int index)
{
	switch (index)
	{
	case ActionDownload:
	case ActionUpload:
		{
			QTreeViewFileSystem* pFileSystem;

			QTreeViewTransferQueue::TransferQueueItem item;

			item.isTop=false;
			if (index == ActionDownload)
			{
				item.source=m_pFrameFileSystem[RemoteMachine]->location();
				if (m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->topItemType(item.source) != -1)
				{
					item.isTop=true;
				}

				item.target=m_pFrameFileSystem[LocalMachine]->location();
				if (m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->topItemType(item.target) != -1)
				{
					return;
				}

				item.direction=QTreeViewTransferQueue::Download;

				pFileSystem=m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem();
			}
			else
			{
				item.target=m_pFrameFileSystem[RemoteMachine]->location();
				if (m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->topItemType(item.target) != -1)
				{
					return;
				}

				item.source=m_pFrameFileSystem[LocalMachine]->location();
				if (m_pFrameFileSystem[LocalMachine]->comboBoxFileSystem()->topItemType(item.source) != -1)
				{
					item.isTop=true;
				}

				item.direction=QTreeViewTransferQueue::Upload;

				pFileSystem=m_pFrameFileSystem[LocalMachine]->treeViewFileSystem();
			}

			if (pFileSystem == NULL)
			{
				break;
			}

			QModelIndexList rows=pFileSystem->selectedRows();;

			if (rows.size() <= 0)
			{
				break;
			}

			item.format=m_pGroupFormat->checkedAction()->data().toInt();

			QString strName;
			foreach (QModelIndex index, rows)
			{
				strName=pFileSystem->filename(index);
				if (strName == "..")
				{
					continue;
				}

				item.size=pFileSystem->size(index);
				item.type=pFileSystem->type(index);
				item.typeDesc=pFileSystem->typeDesc(index);
				item.name=strName;

				m_pViewTransferQueue->appendItem(item);
			}
		}
		break;
	case ActionOpen:
		{
			if (m_nPopupMenuOwner == LocalFileSystemOwner)
			{
				QTreeViewFileSystem* pFileSystem=m_pFrameFileSystem[LocalMachine]->treeViewFileSystem();
				QModelIndexList rows=pFileSystem->selectedRows();;

				if (rows.size() != 1)
				{
					break;
				}

				doubleClickedLocalLocation(rows[0]);
			}
			else if (m_nPopupMenuOwner == RemoteFileSystemOwner)
			{
				QTreeViewFileSystem* pFileSystem=m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem();
				QModelIndexList rows=pFileSystem->selectedRows();;

				if (rows.size() != 1)
				{
					break;
				}

				doubleClickedRemoteLocation(rows[0]);
			}
		}
		break;
	case ActionDelete:
		{
			if (m_nPopupMenuOwner == LocalFileSystemOwner)
			{
				localDelDirectory();
			}
			else if (m_nPopupMenuOwner == RemoteFileSystemOwner)
			{
				remoteDelDirectory();
			}
		}
		break;
	case ActionRefresh:
		{
			if (m_nPopupMenuOwner == LocalFileSystemOwner)
			{
				refresh(LocalMachine);
			}
			else if (m_nPopupMenuOwner == RemoteFileSystemOwner)
			{
				refresh(RemoteMachine);
			}
		}
		break;
	case ActionNewDirectory:
		{
			if (m_nPopupMenuOwner == LocalFileSystemOwner)
			{
				localNewDirectory();
			}
			else if (m_nPopupMenuOwner == RemoteFileSystemOwner)
			{
				remoteNewDirectory();
			}
		}
		break;
	case ActionRename:
		{
			QModelIndexList list;
			QTreeViewFileSystem* pTreeViewFileSystem=NULL;
			if (m_nPopupMenuOwner == LocalFileSystemOwner)
			{
				pTreeViewFileSystem=m_pFrameFileSystem[LocalMachine]->treeViewFileSystem();
			}
			else if (m_nPopupMenuOwner == RemoteFileSystemOwner)
			{
				pTreeViewFileSystem=m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem();
			}

			if (pTreeViewFileSystem == NULL)
			{
				return;
			}

			list=pTreeViewFileSystem->selectedRows();
			if (list.size() == 1)
			{
				pTreeViewFileSystem->edit(list[0]);
			}
		}
		break;
	case ActionRetransmit:
		{
			QModelIndexList list=m_pViewTransferQueue->selectedRows();
			if (!list.isEmpty())
			{
				appendTip(QTipsDelegate::Information, tr("Begin to retransmit..."));
				foreach (QModelIndex index, list)
				{
					if (m_pViewTransferQueue->itemStatus(index.row()) == QTreeViewTransferQueue::Failed)
					{
						m_pViewTransferQueue->setItemStatus(index.row(), QTreeViewTransferQueue::Waiting);
					}
				}
			}
		}
		break;
	case ActionDeleteTask:
		{
			appendTip(QTipsDelegate::Information, tr("Delete tasks..."));
			QModelIndexList list=m_pViewTransferQueue->selectedRows();
			while (!list.isEmpty())
			{
				if (m_pViewTransferQueue->itemStatus(list[0].row()) == QTreeViewTransferQueue::Transfering)
				{
					cancel();
				}

				m_pViewTransferQueue->removeItem(list[0].row());

				list=m_pViewTransferQueue->selectedRows();
			}

			m_pViewTransferQueue->startSynchronize();
		}
		break;
	case ActionDeleteAllTask:
		{
			appendTip(QTipsDelegate::Information, tr("Delete all tasks..."));

			cancel();
			m_pViewTransferQueue->clear();
		}
		break;
	default:
		break;
	}
}

void QAbstractSync::refresh(int machine)
{
	if (machine == LocalMachine)
	{
		localLocationCurrentIndexChanged(m_pFrameFileSystem[machine]->comboBoxFileSystem()->currentLocation());
	}
	else if (machine == RemoteMachine)
	{
		remoteLocationCurrentIndexChanged(m_pFrameFileSystem[machine]->comboBoxFileSystem()->currentLocation());
	}
}
