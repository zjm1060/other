#include "qframetransferfile.h"

#include <QDir>
#include <QMessageBox>

#include "define.h"
#include "common.h"
#include "qmessageparser.h"
#include "qreceiverparser.h"

QFrameTransferFile::QFrameTransferFile(QClient* client, const QString& subPath, QWidget *parent)
	: QAbstractFileTransfer(parent)
	,m_pClient(client)
	,m_bQueryComputer(false)
	,m_bQueryHomePath(true)
{
	m_strSubPath=subPath;

	m_pReceiver=new QReceiverTransferFile(client);

	connect(m_pReceiver, SIGNAL(sendTip(int, const QString)), this, SLOT(appendTip(int, const QString)));
	connect(m_pReceiver, SIGNAL(sendMessageFailed()), this, SLOT(sendMessageFailed()));
	connect(m_pReceiver, SIGNAL(transferTimeout()), this, SLOT(timeout()));

	connect(m_pReceiver, SIGNAL(receiveDrives(const QVector<FileInfo>)), this, SLOT(recvDrives(const QVector<FileInfo>)));
	connect(m_pReceiver, SIGNAL(receiveRoot(const QString)), this, SLOT(recvRoot(const QString)));

	connect(m_pReceiver, SIGNAL(folderCatalog(const QString, const QVector<FileInfo>)),
		this, SLOT(folderCatalog(const QString, const QVector<FileInfo>)));
	connect(m_pReceiver, SIGNAL(folderCatalogFailed(const QString)), this, SLOT(folderCatalogFailed(const QString)));

	connect(m_pReceiver, SIGNAL(renameFailed(const QString, const QString, const QString)), 
		this, SLOT(rename(const QString, const QString, const QString)));
	connect(m_pReceiver, SIGNAL(renamePathLost(const QString, const QString, const QString)), 
		this, SLOT(rename(const QString, const QString, const QString)));
	connect(m_pReceiver, SIGNAL(renameSuccess(const QString, const QString, const QString)), 
		this, SLOT(rename(const QString, const QString, const QString)));

	connect(m_pReceiver, SIGNAL(newDirSuccess(const FileInfo)), this, SLOT(newDirSuccess(const FileInfo)));

	connect(m_pReceiver, SIGNAL(deleteFailed()), this, SLOT(deleteFiles()));
	connect(m_pReceiver, SIGNAL(deleteSuccess()), this, SLOT(deleteFiles()));

	connect(m_pReceiver, SIGNAL(downloadDirSuccess(const QString, const QString, const QVector<FileInfo>, int, int)),
		this, SLOT(downloadDirSuccess(const QString, const QString, const QVector<FileInfo>, int, int)));
	connect(m_pReceiver, SIGNAL(downloadDirFailed()), this, SLOT(downloadDirFailed()));

	connect(m_pReceiver, SIGNAL(downloadFileRenamed(const QString)), this, SLOT(downloadFileRenamed(const QString)));

	connect(m_pReceiver, SIGNAL(downloadFileCreateSuccess(quint64)), this, SLOT(downloadFileCreateSuccess(quint64)));
	connect(m_pReceiver, SIGNAL(downloadFileCreateFailed()), this, SLOT(downloadFileFailed()));
	connect(m_pReceiver, SIGNAL(downloadFileNoExist()), this, SLOT(downloadFileFailed()));
	connect(m_pReceiver, SIGNAL(downloadFileUnreadable()), this, SLOT(downloadFileFailed()));
	connect(m_pReceiver, SIGNAL(downloadFileOpenFail()), this, SLOT(downloadFileFailed()));

	connect(m_pReceiver, SIGNAL(downloadFileWriteSuccess(int)), this, SLOT(downloadFileWriteSuccess(int)));
	connect(m_pReceiver, SIGNAL(downloadFileWriteFailed()), this, SLOT(downloadFileFailed()));
	connect(m_pReceiver, SIGNAL(downloadFileWriteLost()), this, SLOT(downloadFileFailed()));
	connect(m_pReceiver, SIGNAL(downloadFileRenamedFailed()), this, SLOT(downloadFileFailed()));
	connect(m_pReceiver, SIGNAL(downloadFileChangePermissionFailed()), this, SLOT(downloadFileSuccess()));
	connect(m_pReceiver, SIGNAL(downloadFileTransferFinished()), this, SLOT(downloadFileSuccess()));
	connect(m_pReceiver, SIGNAL(downloadFileReadLost()), this, SLOT(downloadFileFailed()));
	connect(m_pReceiver, SIGNAL(downloadFileReadFailed()), this, SLOT(downloadFileFailed()));

	connect(m_pReceiver, SIGNAL(downloadFileSymLinkNoLink()), this, SLOT(downloadFileFailed()));

	connect(m_pReceiver, SIGNAL(uploadDirFailed()), this, SLOT(uploadFailed()));
	connect(m_pReceiver, SIGNAL(uploadDirSuccess(QReceiverTransferFile::TransferQueueItem, const QVector<FileInfo>)), 
		this, SLOT(uploadDirSuccess(QReceiverTransferFile::TransferQueueItem, const QVector<FileInfo>)));
	connect(m_pReceiver, SIGNAL(uploadSymLinkFailed()), this, SLOT(uploadFailed()));
	connect(m_pReceiver, SIGNAL(uploadSymLinkSuccess()), this, SLOT(uploadSuccess()));

	connect(m_pReceiver, SIGNAL(uploadFileRenamed(const QString)), this, SLOT(uploadFileRenamed(const QString)));
	connect(m_pReceiver, SIGNAL(uploadFileCanceled()), this, SLOT(uploadSuccess()));

	connect(m_pReceiver, SIGNAL(uploadFileReadFailed()), this, SLOT(uploadFailed()));
	connect(m_pReceiver, SIGNAL(uploadFileCreateFailed()), this, SLOT(uploadFailed()));
	connect(m_pReceiver, SIGNAL(uploadFileReadLost()), this, SLOT(uploadFailed()));
	connect(m_pReceiver, SIGNAL(uploadFileReadFailed()), this, SLOT(uploadFailed()));
	connect(m_pReceiver, SIGNAL(uploadFileReadSuccess(quint64)), this, SLOT(uploadFileReadSuccess(quint64)));
	connect(m_pReceiver, SIGNAL(uploadFileWriteFailed()), this, SLOT(uploadFailed()));
	connect(m_pReceiver, SIGNAL(uploadFileRenameFailed()), this, SLOT(uploadFailed()));
	connect(m_pReceiver, SIGNAL(uploadFileWriteLost()), this, SLOT(uploadFailed()));
	connect(m_pReceiver, SIGNAL(uploadFileFinished()), this, SLOT(uploadSuccess()));
	connect(m_pReceiver, SIGNAL(uploadFileChangePermissionFailed()), this, SLOT(uploadSuccess()));

	connect(m_pReceiver, SIGNAL(interruptTransferFailed()), this, SLOT(interruptTransfer()));
	connect(m_pReceiver, SIGNAL(interruptTransferSuccess()), this, SLOT(interruptTransfer()));

	remoteDeepRefresh();
}

QFrameTransferFile::~QFrameTransferFile()
{
}

void QFrameTransferFile::recvRoot(const QString root)
{
	m_strRoot=root;

	m_pFrameFileSystem[RemoteMachine]->setLocation(m_strRoot, false);

	setLocation(RemoteMachine, appendPath(root, m_strSubPath));
}

void QFrameTransferFile::recvDrives(const QVector<FileInfo> drives)
{
	if (m_bQueryComputer)
	{
		m_bQueryComputer=false;

		m_pFrameFileSystem[RemoteMachine]->setFileInfo(drives, QComboBoxFileSystem::Computer);
		QString strLocation=m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->itemText(QComboBoxFileSystem::Computer);
		m_pFrameFileSystem[RemoteMachine]->setLocation(strLocation, false);
	}
	else
	{
		m_pFrameFileSystem[RemoteMachine]->setDrives(drives);

		Q_ASSERT_NO_NULL(m_pReceiver);
		m_pReceiver->queryRoot();
	}
}

void QFrameTransferFile::folderCatalog(const QString path, const QVector<FileInfo> files)
{
	if (m_bQueryHomePath)
	{
		m_bQueryHomePath=false;
	}

	m_pFrameFileSystem[RemoteMachine]->setLocation(path, false);
	m_pFrameFileSystem[RemoteMachine]->setFileInfo(files);
}

void QFrameTransferFile::folderCatalogFailed(const QString path)
{
	if (m_bQueryHomePath)
	{
		QString strLocation=m_pFrameFileSystem[RemoteMachine]->location();

		if (strLocation != path)
		{
			queryPath(strLocation);
		}
		else
		{
			if (strLocation != m_strRoot)
			{
				queryPath(m_strRoot);
			}
			else
			{
				Q_ASSERT_NO_NULL(m_pReceiver);

				m_pReceiver->queryRoot();
			}
		}
	}
}

void QFrameTransferFile::uploadFileRenamed(const QString file)
{
	m_pViewTransferQueue->setItemFileName(m_nRow, parseNameFromPath(file));
}

void QFrameTransferFile::downloadDirSuccess(const QString path, const QString savePath, const QVector<FileInfo> files, int direction, int format)
{
	int i=m_nRow+1;
	QReceiverTransferFile::TransferQueueItem task;
	task.direction=direction;
	task.format=format;

	foreach (FileInfo file, files)
	{
		if (file.name == "." || file.name == "..")
		{
			continue;
		}

		task.isTop=false;
		task.name=file.name;
		task.size=file.size;
		task.type=file.type;
		task.typeDesc=fileType(file);
		task.source=path;
		task.target=savePath;

		m_pViewTransferQueue->insertItem(i, task);

		i++;
	}

	m_bStatusTransfer=true;

	fetchTask();
}

void QFrameTransferFile::downloadDirFailed()
{
	m_bStatusTransfer=false;

	fetchTask();
}

void QFrameTransferFile::downloadFileRenamed(const QString file)
{
	m_pViewTransferQueue->setItemFileName(m_nRow, parseNameFromPath(file));
}

void QFrameTransferFile::downloadFileCreateSuccess(quint64 size)
{
	m_pViewTransferQueue->setItemFileSize(m_nRow, size);
}

void QFrameTransferFile::downloadFileFailed()
{
	m_bStatusTransfer=false;

	fetchTask();
}

void QFrameTransferFile::downloadFileSuccess()
{
	m_bStatusTransfer=true;

	fetchTask();
}

void QFrameTransferFile::downloadFileWriteSuccess(int size)
{
	m_pViewTransferQueue->addItemProcess(m_nRow, size);
}

void QFrameTransferFile::uploadFailed()
{
	m_bStatusTransfer=false;

	fetchTask();
}

void QFrameTransferFile::uploadSuccess()
{
	m_bStatusTransfer=true;

	fetchTask();
}

void QFrameTransferFile::uploadDirSuccess(QReceiverTransferFile::TransferQueueItem task, const QVector<FileInfo> files)
{
	int i=m_nRow+1;

	foreach (FileInfo info, files)
	{
		task.name=info.name;
		task.type=info.type;
		task.size=info.size;
		task.typeDesc=fileType(info);

		m_pViewTransferQueue->insertItem(i, task);

		i++;
	}

	m_bStatusTransfer=true;

	fetchTask();
}

void QFrameTransferFile::uploadFileReadSuccess(quint64 size)
{
	m_pViewTransferQueue->addItemProcess(m_nRow, size);
}

void QFrameTransferFile::interruptTransfer()
{
	fetchTask();
}

void QFrameTransferFile::rename(const QString dir, const QString old, const QString now)
{
	Q_UNUSED(old);
	Q_UNUSED(now);

	if (dir == m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->currentLocation())
	{
		queryPath(dir);
	}
}

void QFrameTransferFile::newDirSuccess(const FileInfo file)
{
	QTreeViewFileSystem* pTreeViewFileSystem=m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem();
	QModelIndex index=pTreeViewFileSystem->appendFileInfo(file);

	editItem(RemoteMachine, index);
}

void QFrameTransferFile::deleteFiles()
{
	refresh(RemoteMachine);
}

void QFrameTransferFile::transfer(int row, const QReceiverTransferFile::TransferQueueItem& task)
{
	transferFile(row, task);
}

void QFrameTransferFile::transferFile(int row, const QReceiverTransferFile::TransferQueueItem& task)
{
	if (row == -1)
	{
		//Í¬²½Íê±Ï
		finished();
		return;
	}

	m_nRow=row;

	if (task.direction == QTreeViewTransferQueue::Download)
	{
		m_nRefreshFlag|=(1<<QTreeViewTransferQueue::Download);
	}
	else if (task.direction == QTreeViewTransferQueue::Upload)
	{
		m_nRefreshFlag|=(1<<QTreeViewTransferQueue::Upload);
	}

	Q_ASSERT_NO_NULL(m_pReceiver);
	m_pReceiver->transfer(task);
}

void QFrameTransferFile::fetchTask()
{
	if (!m_bSync)
	{
		return;
	}

	QReceiverTransferFile::TransferQueueItem task;

	m_pViewTransferQueue->setItemStatus(m_nRow, m_bStatusTransfer? QTreeViewTransferQueue::Finished:QTreeViewTransferQueue::Failed);
	m_nRow=m_pViewTransferQueue->fetchTask(task);
	transferFile(m_nRow, task);
}

void QFrameTransferFile::queryPath(const QString& path)
{
	Q_ASSERT_NO_NULL(m_pReceiver);

	int nSortColumn=m_pFrameFileSystem[RemoteMachine]->treeViewFileSystem()->sortColumn();
	int nSort=SortName;
	switch (nSortColumn)
	{
	case QTreeViewFileSystem::NameTitle:
		nSort=SortName;
		break;
	case QTreeViewFileSystem::SizeTitle:
		nSort=SortSize;
		break;
	case QTreeViewFileSystem::TypeTitle:
		nSort=SortType;
		break;
	case QTreeViewFileSystem::ModifiedTitle:
		nSort=SortTime;
		break;
	default:
		break;
	}

	m_pReceiver->queryPath(path, nSort);
}

void QFrameTransferFile::queryRename(const QString& location, const QString& old, const QString& now)
{
	Q_ASSERT_NO_NULL(m_pReceiver);

	m_pReceiver->queryRename(location, old, now);
}

void QFrameTransferFile::queryNewDirectory(const QString& location)
{
	Q_ASSERT_NO_NULL(m_pReceiver);

	m_pReceiver->queryNewDirectory(location);
}

void QFrameTransferFile::queryDelete(const QString& location, const QVector<QString>& files)
{
	Q_ASSERT_NO_NULL(m_pReceiver);

	m_pReceiver->queryDelete(location, files);
}

void QFrameTransferFile::cancel()
{
	Q_ASSERT_NO_NULL(m_pReceiver);

	m_pReceiver->queryInterruptTransfer();

	finished();
}

void QFrameTransferFile::timeout()
{
	appendTip(QReceiverTransferFile::Error, tr("File transfer timeout."));

	cancel();
}

void QFrameTransferFile::sendMessageFailed()
{
	cancel();
}

void QFrameTransferFile::finished()
{
	Q_UNUSED(m_pReceiver);
	m_pReceiver->finished();

	QAbstractFileTransfer::finished();
	
	appendTip(QReceiverTransferFile::Information, tr("Synchronization finished."));

	if ((m_nRefreshFlag&(1<<QTreeViewTransferQueue::Download)) != 0)
	{
		refresh(LocalMachine);
	}
	else if ((m_nRefreshFlag&(1<<QTreeViewTransferQueue::Upload)) != 0)
	{
		refresh(RemoteMachine);
	}
}

void QFrameTransferFile::gotoRemoteHome()
{
	remoteDeepRefresh();
}

void QFrameTransferFile::gotoRemoteLocation(const QString& location)
{
	int itemType=m_pFrameFileSystem[RemoteMachine]->comboBoxFileSystem()->topItemType(location);
	switch (itemType)
	{
	case QComboBoxFileSystem::OtherItem:
		{
			queryPath(location);
		}
		break;
	case QComboBoxFileSystem::Computer:
		{
			Q_ASSERT_NO_NULL(m_pReceiver);

			m_bQueryComputer=true;
			m_pReceiver->queryDrives();
		}
		break;
	default:
		break;
	}
}

void QFrameTransferFile::remoteDeepRefresh()
{
	Q_ASSERT_NO_NULL(m_pReceiver);

	m_bQueryHomePath=true;
	m_pFrameFileSystem[RemoteMachine]->clear();

	if (!m_bSync)
	{
		m_pModelTips->setRowCount(0);
	}

	m_pReceiver->queryDrives();
}
