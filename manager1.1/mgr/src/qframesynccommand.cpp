#include "qframesynccommand.h"

#include <QMessageBox>

#include "common.h"
#include "qimessage.h"
#include "qfilecontroller.h"
#include "qreceiverparser.h"
#include "qmessageparser.h"
#include "qdlgbrowsefiles.h"

QFrameSyncCommand::QFrameSyncCommand(QWidget *parent/* =0 */)
:QFrame(parent)
,m_pClient(NULL)
{
	m_ui=new Ui::FrameCommand;
	m_ui->setupUi(this);

	QSharedControllerPtr pController(new QFileController);
	m_pClientManager=new QClientManager(pController, this);
	connect(m_pClientManager, SIGNAL(connected(QClient*)), this, SLOT(connected(QClient*)));

	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_ui->pushButtonSave->setEnabled(false);

//	m_ui->labelInfo->setText(tr("When the monitored files are synchronized, the associated programs will run."));

	m_pButtonRefresh=new QLinkButton(tr("Refresh"), this);
	m_ui->horizontalLayoutButtons->addWidget(m_pButtonRefresh);

	connect(m_pButtonRefresh, SIGNAL(clicked()), this, SLOT(onClickedBtnRefresh()));

	m_pListViewServers=new QListViewControlledServer(this);
	m_ui->horizontalLayoutServers->addWidget(m_pListViewServers);

	connect(m_pListViewServers, SIGNAL(currentChanged(const QModelIndex&)), this, SLOT(currentChanged(const QModelIndex&)));

	connect(m_ui->pushButtonSave, SIGNAL(clicked()), this, SLOT(onClickedBtnSave()));

	m_ui->toolButtonAddCommand->setIcon(QIcon(":/images/add.png"));
	m_ui->toolButtonRemoveCommand->setIcon(QIcon(":/images/delete1.png"));
	m_ui->toolButtonMoveUp->setIcon(QIcon(":/images/gopervious.png"));
	m_ui->toolButtonMoveDown->setIcon(QIcon(":/images/gonext.png"));

	m_pTreeViewCommand=new QTreeView(this);
	m_ui->verticalLayoutCommand->addWidget(m_pTreeViewCommand);

	m_pTreeViewCommand->setRootIsDecorated(false);
	m_pTreeViewCommand->setAlternatingRowColors(true);
	m_pTreeViewCommand->setSelectionMode(QAbstractItemView::SingleSelection);

	m_pNumberDelegate=new QNumberDelegate(this);

	m_pTreeViewCommand->setItemDelegateForColumn(SizeTitle, m_pNumberDelegate);

	QStringList labels;
	labels<<tr("Name")<<tr("Running")<<tr("Terminal")<<tr("Size")<<tr("Type")<<tr("Last Modified")<<tr("Path");

	m_pModel=new QStandardItemModel(0, labels.size());
	m_pModel->setHorizontalHeaderLabels(labels);

	m_pTreeViewCommand->setModel(m_pModel);

	m_pTreeViewCommand->setColumnWidth(NameTitle, 200);
	m_pTreeViewCommand->setColumnWidth(SizeTitle, 100);
	m_pTreeViewCommand->setColumnWidth(TypeTitle, 100);
	m_pTreeViewCommand->setColumnWidth(ModifiedTitle, 120);
	m_pTreeViewCommand->setColumnWidth(RunningTitle, 50);
	m_pTreeViewCommand->setColumnWidth(TerminalTitle, 50);
	m_pTreeViewCommand->setColumnWidth(PathTitle, 280);

	connect(m_pTreeViewCommand, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onClickedItem(const QModelIndex&)));

	if (m_pTreeViewCommand->selectionModel() != NULL)
	{
		connect(m_pTreeViewCommand->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
			this, SLOT(currentCommandItemChanged(const QModelIndex&, const QModelIndex&)));
	}

	connect(m_ui->toolButtonAddCommand, SIGNAL(clicked()), this, SLOT(onClickedBtnAddCommand()));
	connect(m_ui->toolButtonRemoveCommand,SIGNAL(clicked()), this, SLOT(onClickedBtnDeleteCommand()));
	connect(m_ui->toolButtonMoveDown, SIGNAL(clicked()), this, SLOT(onClickedBtnMoveDown()));
	connect(m_ui->toolButtonMoveUp, SIGNAL(clicked()), this, SLOT(onClickedBtnMoveUp()));

	m_ui->toolButtonAddFile->setIcon(QIcon(":/images/add.png"));
	m_ui->toolButtonRemoveFile->setIcon(QIcon(":/images/delete1.png"));

	connect(m_ui->toolButtonAddFile, SIGNAL(clicked()), this, SLOT(onClickedBtnAddFiles()));
	connect(m_ui->toolButtonRemoveFile, SIGNAL(clicked()), this, SLOT(onClickedBtnDeleteFiles()));

	m_pTreeViewFiles=new QTreeViewAssociationFiles(this);
	m_ui->verticalLayoutAssociations->addWidget(m_pTreeViewFiles);

	connect(m_pTreeViewFiles, SIGNAL(changed(QVector<SyncMonitorFile>)), this, SLOT(associationFileChanged(QVector<SyncMonitorFile>)));
}

QFrameSyncCommand::~QFrameSyncCommand()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);

	onClickedBtnSave();

	m_pClientManager->removeAll();
	SAFE_DELETE_CLASS(m_pClientManager);

	m_pTreeViewCommand->setModel(NULL);
	SAFE_DELETE_CLASS(m_pModel);
	SAFE_DELETE_CLASS(m_pNumberDelegate);
}

void QFrameSyncCommand::setServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	m_pListViewServers->refreshServerList(servers);
}

void QFrameSyncCommand::currentChanged(const QModelIndex& index)
{
	if (!index.isValid() || m_pListViewServers == NULL)
	{
		return;
	}

//	enquireSaveChanged();

	ServerInfo info=m_pListViewServers->serverInfoFromIndex(index);
	setClient(info);
}

void QFrameSyncCommand::setClient(const ServerInfo& info)
{
	if (m_server == info)
	{
		return;
	}

	if (m_pClient != NULL)
	{
		m_pClientManager->remove(m_pClient);
		m_pClient=NULL;
	}

	QClient* pClient=m_pClientManager->addClient(info);
	m_pClientManager->connectToHost(pClient);

	m_server=info;

	m_pModel->setRowCount(0);
	m_pTreeViewFiles->clear();
}

void QFrameSyncCommand::connected(QClient* client)
{
	m_pClient=client;
	if (m_pClient != NULL)
	{
		queryCommandSetting();
	}
}

void QFrameSyncCommand::queryCommandSetting()
{
	m_pModel->setRowCount(0);
	m_pTreeViewFiles->clear();

	if (m_pClient == NULL)
	{
		emit unconnected();
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_CONFIGURE);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QuerySyncAssociationConfigure, m_nUsrID);
		bool bRtn=m_pClient->sendMessage(buffer, MSG_CONFIGURE);
		if (!bRtn)
		{
			emit serverQuit();
		}
		else
		{
			setMenuState(true);
		}
	}
}

void QFrameSyncCommand::queryWriteCommandSetting(const SyncCommands& commands)
{
	if (m_pClient == NULL)
	{
		emit unconnected();
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_CONFIGURE);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
		stream<<commands;

		buffer=pMsg->query(QueryWriteSyncAssociateionConfig, m_nUsrID, buffer);
		bool bRtn=m_pClient->sendMessage(buffer, MSG_CONFIGURE);
		if (!bRtn)
		{
			emit serverQuit();
		}
	}
}

void QFrameSyncCommand::setMenuState(bool loading)
{
	if (loading)
	{
		m_ui->toolButtonAddCommand->setEnabled(false);
		m_ui->toolButtonRemoveCommand->setEnabled(false);
		m_ui->toolButtonMoveDown->setEnabled(false);
		m_ui->toolButtonMoveUp->setEnabled(false);
		
		m_ui->toolButtonAddFile->setEnabled(false);
		m_ui->toolButtonRemoveFile->setEnabled(false);
	}
	else
	{
		if (m_pModel == NULL)
		{
			return;
		}
		
		m_ui->toolButtonAddCommand->setEnabled(true);

		QModelIndex index=m_pTreeViewCommand->currentIndex();

		if (index.isValid())
		{
			if (m_pModel->data(m_pModel->index(index.row(), NameTitle), ItemRole) != CommandRole)
			{
				m_ui->toolButtonRemoveCommand->setEnabled(true);
			}
			else
			{
				m_ui->toolButtonRemoveCommand->setEnabled(false);
			}

			m_ui->toolButtonMoveUp->setEnabled(index.row() > 0);
			m_ui->toolButtonMoveDown->setEnabled(index.row() >= 0 && index.row() < m_pModel->rowCount()-1);

			m_ui->toolButtonAddFile->setEnabled(true);
			m_ui->toolButtonRemoveFile->setEnabled(m_pTreeViewFiles->selectedRows().isEmpty()? false:true);
		}
		else
		{
			m_ui->toolButtonRemoveCommand->setEnabled(false);
			m_ui->toolButtonMoveUp->setEnabled(false);
			m_ui->toolButtonMoveDown->setEnabled(false);

			m_ui->toolButtonAddFile->setEnabled(false);
			m_ui->toolButtonRemoveFile->setEnabled(false);
		}
	}
}

void QFrameSyncCommand::recvSyncAssociationConfigure(SyncCommands commands)
{
	setCommands(commands);

	setMenuState(false);
}

void QFrameSyncCommand::recvWriteSyncAssociationConfigure(int flag)
{
	switch (flag)
	{
	case QueryWriteConfigureSuccess:
		{
			QMessageBox::information(this, tr("Information"), tr("Successfully saved!"), QMessageBox::Ok, QMessageBox::Ok);
		}
		break;
	case QueryWriteConfigureFail:
		{
			QMessageBox::critical(this, tr("Error"), tr("Save failed!"), QMessageBox::Ok, QMessageBox::Ok);

			dataChanged();
		}
		break;
	default:
		Q_ASSERT(false);
		break;
	}
}

void QFrameSyncCommand::setCommands(const SyncCommands& commands)
{
	disconnect(m_pModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));

	m_pModel->setRowCount(0);
	int nIndex=0, nCount=commands.programs.size()+commands.commands.size();
	if (nCount > 0)
	{
		while (nCount > 0 )
		{
			if (commands.programs.contains(nIndex))
			{
				nCount--;

				const SyncProgram& program=commands.programs[nIndex];
				addProgramItem(program);
			}
			else if (commands.commands.contains(nIndex))
			{
				nCount--;

				const SyncCommand& command=commands.commands[nIndex];

				addCommandItem(command);
			}

			nIndex++;
		}
	}
	else
	{
		addDefaultCommand();
	}

	connect(m_pModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));
}

SyncCommands QFrameSyncCommand::commands() const
{
	SyncCommands commands;

	if ( m_pModel != NULL)
	{
		SyncProgram program;
		SyncCommand command;

		QModelIndex index;
		int nRowCount=m_pModel->rowCount();
		for (int nRow=0; nRow<nRowCount; nRow++)
		{
			index=m_pModel->index(nRow, NameTitle);
			int nRole=m_pModel->data(index, ItemRole).toInt();
			if (nRole == ProgramRole)
			{
				program.program=qvariant_cast<FileInfo>(m_pModel->data(index, FileRole));;
				program.files=qvariant_cast<QVector<SyncMonitorFile> >(m_pModel->data(index, AssociateRole));
				program.enabled=(m_pModel->itemFromIndex(index)->checkState() == Qt::Checked);

				index=m_pModel->index(nRow, PathTitle);
				program.path=m_pModel->data(index).toString();

				index=m_pModel->index(nRow, RunningTitle);
				program.running=(m_pModel->itemFromIndex(index)->checkState() == Qt::Checked);

				index=m_pModel->index(nRow, TerminalTitle);
				program.terminal=(m_pModel->itemFromIndex(index)->checkState() == Qt::Checked);

				commands.programs.insert(nRow, program);
			}
			else if (nRole == CommandRole)
			{
				command.command=m_pModel->data(index).toString();
				command.enabled=(m_pModel->itemFromIndex(index)->checkState() == Qt::Checked);
				command.files=qvariant_cast<QVector<SyncMonitorFile> >(m_pModel->data(index, AssociateRole));
				for (int i=0; i<command.files.size(); i++)
				{
					if (command.files[i].enabled)
					{
						command.params.append(appendPath(command.files[i].path, command.files[i].file.name));
					}
				}

				commands.commands.insert(nRow, command);
			}
		}
	}

	return commands;
}

void QFrameSyncCommand::refresh()
{
	queryCommandSetting();
}

void QFrameSyncCommand::addDefaultCommand()
{
	addCommandItem(H9000_FUNCTION_UPDATE_DATABASE_FILE);
}

void QFrameSyncCommand::addCommandItem(const QString& command, const QVector<SyncMonitorFile>& files)
{
	SyncCommand cmd;
	cmd.command=command;
	cmd.enabled=true;
	cmd.files=files;

	addCommandItem(cmd);
}

void QFrameSyncCommand::addCommandItem(const SyncCommand& command)
{
	if (m_pModel == NULL)
	{
		return;
	}

	QModelIndex index;

	int nRowCount=m_pModel->rowCount();
	for (int nRow=0; nRow<nRowCount; nRow++)
	{
		index=m_pModel->index(nRow, NameTitle);
		int nRole=m_pModel->data(index, ItemRole).toInt();
		if (nRole == CommandRole)
		{
			QString strCommand=m_pModel->data(index).toString();

			if (command.command == strCommand)
			{
				return;
			}
		}
	}

	m_pModel->insertRow(nRowCount);

	index=m_pModel->index(nRowCount, NameTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->itemFromIndex(index)->setCheckable(true);
	m_pModel->itemFromIndex(index)->setCheckState(command.enabled ? Qt::Checked:Qt::Unchecked);
	m_pModel->setData(index, CommandRole, ItemRole);
	m_pModel->setData(index, QIcon(":/images/command.png"), Qt::DecorationRole);
	m_pModel->setData(index, command.command);
	m_pModel->setData(index, qVariantFromValue(command.files), AssociateRole);

	index=m_pModel->index(nRowCount, SizeTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->itemFromIndex(index)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

	index=m_pModel->index(nRowCount, TypeTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->setData(index, tr("Command"));

	index=m_pModel->index(nRowCount, ModifiedTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);

	index=m_pModel->index(nRowCount, PathTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);

	index=m_pModel->index(nRowCount, RunningTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);

	index=m_pModel->index(nRowCount, TerminalTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
}

void QFrameSyncCommand::currentCommandItemChanged(const QModelIndex& current, const QModelIndex& previous)
{
	Q_UNUSED(previous);
	Q_UNUSED(current);

	setMenuState(false);
}

void QFrameSyncCommand::fileItemsSelectedChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
	Q_UNUSED(selected);
	Q_UNUSED(deselected);

	setMenuState(false);
}

void QFrameSyncCommand::onClickedItem(const QModelIndex& index)
{
	if (m_pTreeViewFiles == NULL)
	{
		return;
	}

	QItemSelectionModel* pSelectionModel=m_pTreeViewFiles->selectionModel();
	if (pSelectionModel != NULL)
	{
		disconnect(pSelectionModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
			this, SLOT(fileItemsSelectedChanged(const QItemSelection&, const QItemSelection&)));
	}

	m_pTreeViewFiles->clear();

	QModelIndex currentIndex=m_pModel->index(index.row(), NameTitle, index.parent());
	QVariant var=m_pModel->data(currentIndex, AssociateRole);
	if (var.isValid())
	{
		if (m_pModel->data(currentIndex, ItemRole) != CommandRole)
		{
			m_pTreeViewFiles->setExclusive(false);
		}
		else
		{
			m_pTreeViewFiles->setExclusive(true);
		}

		QVector<SyncMonitorFile> files=qvariant_cast<QVector<SyncMonitorFile> >(var);
		m_pTreeViewFiles->insertFiles(files);
	}

	if (pSelectionModel != NULL)
	{
		connect(pSelectionModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
			this, SLOT(fileItemsSelectedChanged(const QItemSelection&, const QItemSelection&)));
	}

	setMenuState(false);
}

void QFrameSyncCommand::itemChanged(QStandardItem* item)
{
	Q_UNUSED(item);

	dataChanged();
}

bool QFrameSyncCommand::enquireSaveChanged()
{
	bool bSaved=false;

	if (m_ui->pushButtonSave->isEnabled())
	{
		QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"),
			tr("The configures has changed! Need to save?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

		if (button == QMessageBox::Yes)
		{
			onClickedBtnSave();

			bSaved=true;
		}
	}

	return bSaved;
}

void QFrameSyncCommand::onClickedBtnRefresh()
{
	if (!enquireSaveChanged())
	{
		queryCommandSetting();
	}
}

void QFrameSyncCommand::associationFileChanged(QVector<SyncMonitorFile> files)
{
	if (m_pTreeViewCommand == NULL || m_pModel == NULL)
	{
		return;
	}

	QModelIndex index=m_pTreeViewCommand->currentIndex();
	if (index.isValid())
	{
		m_pModel->setData(m_pModel->index(index.row(), NameTitle, index.parent()), qVariantFromValue(files), AssociateRole);
	}

	setMenuState(false);
}

void QFrameSyncCommand::onClickedBtnSave()
{
	if (m_pModel != NULL)
	{
		SyncCommands coms=commands();
		queryWriteCommandSetting(coms);
	}
	else
	{
		recvWriteSyncAssociationConfigure(QueryWriteConfigureFail);
	}

	m_ui->pushButtonSave->setEnabled(false);
}

void QFrameSyncCommand::dataChanged()
{
	m_ui->pushButtonSave->setEnabled(true);
}

void QFrameSyncCommand::onClickedBtnAddCommand()
{
	if (m_server.count <= 0)
	{
		return;
	}

	QDlgBrowseFiles dialog(m_server, m_strWorkspaceCommand, "bin", this);
	dialog.setFilters(QStringList(), QDir::AllDirs|QDir::Files|QDir::Executable);
	dialog.setModal(true);
	if (dialog.exec() != 0)
	{
		QVector<FileInfo> files=dialog.selectedFiles();

		if (!files.isEmpty())
		{
			SyncProgram program;

			m_strWorkspaceCommand=dialog.text();

			program.path=m_strWorkspaceCommand;
			if (program.path.isEmpty())
			{
				return;
			}

			program.enabled=true;
			program.running=false;
			program.terminal=false;

			for (int i=0; i<files.size(); i++)
			{
				if (isFile(files[i].type))
				{
					program.program=files[i];
					addProgramItem(program);
				}
			}

			dataChanged();
		}
	}
}

void QFrameSyncCommand::addProgramItem(const SyncProgram& program)
{
	if (m_pModel == NULL)
	{
		return;
	}

	QModelIndex index;

	int nRowCount=m_pModel->rowCount();
	for (int nRow=0; nRow<nRowCount; nRow++)
	{
		index=m_pModel->index(nRow, NameTitle);
		int nRole=m_pModel->data(index, ItemRole).toInt();
		if (nRole == ProgramRole)
		{
			QString strPath=m_pModel->data(m_pModel->index(nRow, PathTitle)).toString();
			QString strProgram=m_pModel->data(index).toString();

			if (appendPath(program.path, program.program.name) == appendPath(strPath, strProgram))
			{
				return;
			}
		}
	}

	m_pModel->insertRow(nRowCount);

	index=m_pModel->index(nRowCount, NameTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->itemFromIndex(index)->setCheckable(true);
	m_pModel->itemFromIndex(index)->setCheckState(program.enabled? Qt::Checked:Qt::Unchecked);
	m_pModel->setData(index, ProgramRole, ItemRole);
	m_pModel->setData(index, fileIcon(program.program), Qt::DecorationRole);
	m_pModel->setData(index, program.program.name);
	m_pModel->setData(index, qVariantFromValue(program.program), FileRole);
	m_pModel->setData(index, qVariantFromValue(program.files), AssociateRole);

	index=m_pModel->index(nRowCount, SizeTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->itemFromIndex(index)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_pModel->setData(index, fileSizeForNumber(program.program));

	index=m_pModel->index(nRowCount, TypeTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->setData(index, fileType(program.program));

	index=m_pModel->index(nRowCount, ModifiedTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->setData(index, program.program.lastModified);

	index=m_pModel->index(nRowCount, PathTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->setData(index, program.path, Qt::DisplayRole);

	index=m_pModel->index(nRowCount, RunningTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->itemFromIndex(index)->setCheckable(true);
	m_pModel->itemFromIndex(index)->setCheckState(program.running? Qt::Checked:Qt::Unchecked);

	index=m_pModel->index(nRowCount, TerminalTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->itemFromIndex(index)->setCheckable(true);
	m_pModel->itemFromIndex(index)->setCheckState(program.terminal? Qt::Checked:Qt::Unchecked);
}

QModelIndexList QFrameSyncCommand::selectedRows() const
{
	QItemSelectionModel* pModel=NULL;

	if (m_pTreeViewCommand != NULL)
	{
		pModel=m_pTreeViewCommand->selectionModel();
	}

	if (pModel == NULL)
	{
		return QModelIndexList();
	}

	return pModel->selectedRows();
}

void QFrameSyncCommand::onClickedBtnDeleteCommand()
{
	if (m_pModel == NULL)
	{
		return;
	}

	QModelIndexList indexes=selectedRows();
	if (indexes.isEmpty())
	{
		return;
	}

	QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"),
		tr("Will delete the command! Are you sure to delete?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::No);

	if (button == QMessageBox::Yes)
	{
		QListIterator<QModelIndex> it(indexes);
		it.toBack();
		while (it.hasPrevious())
		{
			const QModelIndex& index=it.previous();
			if (m_pModel->data(index, ItemRole).toInt() != CommandRole)
			{
				m_pModel->removeRow(index.row(), index.parent());
			}
		}

		dataChanged();
	}
}

void QFrameSyncCommand::interchangeRows(int firstRow, int secondRow)
{
	if (m_pModel == NULL)
	{
		return;
	}

	if (firstRow<0 || firstRow>=m_pModel->rowCount() || secondRow<0 || secondRow>=m_pModel->rowCount())
	{
		return;
	}

	int nColCount=m_pModel->columnCount();
	QStandardItem *pFirstItem, *pSecondItem;
	for (int nCol=0; nCol<nColCount; nCol++)
	{
		pFirstItem=m_pModel->takeItem(firstRow, nCol);
		pSecondItem=m_pModel->takeItem(secondRow, nCol);

		m_pModel->setItem(firstRow, nCol, pSecondItem);
		m_pModel->setItem(secondRow, nCol, pFirstItem);
	}
}

void QFrameSyncCommand::onClickedBtnMoveUp()
{
	if (m_pModel == NULL || m_pTreeViewCommand == NULL)
	{
		return;
	}

	QModelIndexList indexes=selectedRows();
	if (indexes.size() != 1)
	{
		return;
	}

	QModelIndex index=indexes.first();
	if (index.row() == 0)
	{
		return;
	}

	interchangeRows(index.row()-1, index.row());

	m_pTreeViewCommand->setCurrentIndex(m_pModel->index(index.row()-1, NameTitle));

	dataChanged();
}

void QFrameSyncCommand::onClickedBtnMoveDown()
{
	if (m_pModel == NULL || m_pTreeViewCommand == NULL)
	{
		return;
	}

	QModelIndexList indexes=selectedRows();
	if (indexes.size() != 1)
	{
		return;
	}

	QModelIndex index=indexes.first();
	if (index.row() == m_pModel->rowCount()-1)
	{
		return;
	}

	interchangeRows(index.row(), index.row()+1);

	m_pTreeViewCommand->setCurrentIndex(m_pModel->index(index.row()+1, NameTitle));

	dataChanged();
}

void QFrameSyncCommand::onClickedBtnAddFiles()
{
	if (m_server.count <= 0 || m_pTreeViewFiles == NULL)
	{
		return;
	}

	QDlgBrowseFiles dialog(m_server, m_strWorkspace, "", this);
	dialog.setModal(true);
	if (dialog.exec() != 0)
	{
		m_strWorkspace=dialog.text();

		QVector<FileInfo> files=dialog.selectedFiles();

		if (!files.isEmpty())
		{
			SyncMonitorFile file;

			if (m_strWorkspace.isEmpty())
			{
				return;
			}

			file.enabled=true;
			file.path=m_strWorkspace;
			
			for (int i=0; i<files.size(); i++)
			{
				if (isFile(files[i].type))
				{
					file.file=files[i];
					m_pTreeViewFiles->appendFileInfo(file);
				}
			}

			dataChanged();
		}
	}
}

void QFrameSyncCommand::onClickedBtnDeleteFiles()
{
	if (m_pTreeViewFiles == NULL)
	{
		return;
	}

	QModelIndexList indexes=m_pTreeViewFiles->selectedRows();
	if (indexes.isEmpty())
	{
		return;
	}

	QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"),
		tr("Will delete the association files! Are you sure to delete?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::No);

	if (button == QMessageBox::Yes)
	{
		m_pTreeViewFiles->removeRows(indexes);

		dataChanged();
	}
}
