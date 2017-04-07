#include "qtreeviewsynccommit.h"

#include <QMouseEvent>
#include <QDir>

#include "common.h"
#include "qimessage.h"
#include "qglobalinfo.h"

QTreeViewSyncCommit::QTreeViewSyncCommit(QWidget *parent)
	: QTreeView(parent)
	,m_pModel(NULL)
	,m_bCheckable(true)
	,m_bRestrictFilter(true)
{
	qRegisterMetaType<QTreeModel*>("QTreeModel*");

	setCustomItemDelegate();
}

QTreeViewSyncCommit::~QTreeViewSyncCommit()
{
	setModel(NULL);
	SAFE_DELETE_CLASS(m_pModel);
}

void QTreeViewSyncCommit::setCustomItemDelegate()
{
	m_pDelegate=new QDelegateSyncCommit(this);
	setItemDelegate(m_pDelegate);
}

bool QTreeViewSyncCommit::hasSelected() const
{
	if (m_pModel == NULL)
	{
		return false;
	}

	int nNumCount=m_pModel->rowCount();
	for (int i=0; i< nNumCount; i++)
	{
		if (itemCheckState(i) != Qt::Unchecked)
		{
			return true;
		}
	}

	return false;
}

void QTreeViewSyncCommit::setRestrictFilterEnabled(bool enable)
{
	m_bRestrictFilter=enable;
}

void QTreeViewSyncCommit::check(const QString& workspace, const QString& histroy)
{
	clear();

	m_strWorkspace=workspace;
	m_strHistory=histroy;

	checkDiff();
}

int QTreeViewSyncCommit::rowCount(const QModelIndex& parent) const
{
	return m_pModel==NULL? 0:m_pModel->rowCount(parent);
}

void QTreeViewSyncCommit::setItemsCheckable(bool checkable)
{
	m_bCheckable=checkable;

	setSortingEnabled(false);

	int nNumRows=rowCount();
	for (int i=0; i<nNumRows; i++)
	{
		setChildItemCheckable(i, checkable);
	}

	setSortingEnabled(true);
}

void QTreeViewSyncCommit::setChildItemCheckable(int row, bool checkable, const QModelIndex& parent)
{
	if (m_pModel == NULL)
	{
		return;
	}

	QModelIndex index=m_pModel->index(row, NameTitle, parent);
	m_pModel->setData(index, checkable, CheckableRole);

	int nNumRow=m_pModel->rowCount(index);
	for (int i=0; i<nNumRow; i++)
	{
		setChildItemCheckable(i, checkable, index);
	}
}

int QTreeViewSyncCommit::itemCheckState(int row, const QModelIndex& parent) const
{
	if (m_pModel == NULL)
	{
		return Qt::Unchecked;
	}

	Q_ASSERT(row>=0 && row < m_pModel->rowCount(parent));

	return m_pModel->data(m_pModel->index(row, NameTitle, parent), CheckedRole).toInt();
}

void QTreeViewSyncCommit::setItemCheckState(int row, int state, const QModelIndex& parent)
{
	if (m_pModel == NULL)
	{
		return;
	}

	Q_ASSERT(row>=0 && row < m_pModel->rowCount(parent));

	setChildItemCheckState(row, state, parent);
	setParentItemCheckState(parent);

	emit selectedChanged();
}

void QTreeViewSyncCommit::setChildItemCheckState(int row, int state, const QModelIndex& parent)
{
	if (m_pModel == NULL)
	{
		return;
	}

	QModelIndex pindex=m_pModel->index(row, NameTitle, parent);

	m_pModel->setData(pindex, state, CheckedRole);

	int nNumRow=m_pModel->rowCount(pindex);
	for (int nRow=0; nRow<nNumRow; nRow++)
	{
		setChildItemCheckState(nRow, state, pindex);
	}
}

void QTreeViewSyncCommit::setParentItemCheckState(const QModelIndex& parent)
{
	if (m_pModel == NULL || parent == QModelIndex())
	{
		return;
	}

	int nNumRow=m_pModel->rowCount(parent);
	int nChecked=-1;

	for (int nRow=0; nRow<nNumRow; nRow++)
	{
		QModelIndex index=m_pModel->index(nRow, NameTitle, parent);
		int nState=m_pModel->data(index, CheckedRole).toInt();

		if (nState == Qt::PartiallyChecked)
		{
			nChecked=Qt::PartiallyChecked;
			break;
		}
		else
		{
			if (nChecked == -1)
			{
				nChecked=nState;
			}
			else
			{
				if (nChecked != nState)
				{
					nChecked=Qt::PartiallyChecked;
					break;
				}
			}
		}
	}

	if (nChecked != -1)
	{
		m_pModel->setData(m_pModel->index(parent.row(), NameTitle, parent.parent()), nChecked, CheckedRole);
	}

	setParentItemCheckState(parent.parent());
}

void QTreeViewSyncCommit::setTreeModel(QTreeModel* model)
{
	setModel(model);

	SAFE_DELETE_CLASS(m_pModel);
	m_pModel=model;
}

void QTreeViewSyncCommit::finished(QTreeModel* model)
{
	setTreeModel(model);

	setColumnWidth(NameTitle, 260);
	setColumnWidth(SizeTitle, 100);
	setColumnWidth(TypeTitle, 80);
	setColumnWidth(ModifiedTitle, 140);
	setColumnWidth(StatusTitle, 100);

	emit refreshModelFinished();
}

QModelIndex QTreeViewSyncCommit::mapToSourceItem(const QModelIndex& index)
{
	return index;
}

void QTreeViewSyncCommit::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (m_bCheckable)
		{
			QModelIndex index=indexAt(event->pos());
			QModelIndex sindex=mapToSourceItem(index);

			if (index.column() == NameTitle)
			{
				QRect rect=visualRect(index);

				int nCheckWidth=style()->pixelMetric(QStyle::PM_ButtonIconSize, 0, this);

				rect=QRect(rect.left()+4, rect.top(), nCheckWidth, rect.height());
				if (rect.contains(event->pos()))
				{
					int nState =m_pModel->data(m_pModel->index(sindex.row(), NameTitle, sindex.parent()), CheckedRole).toInt();
					if (nState == Qt::PartiallyChecked || nState == Qt::Checked)
					{
						nState=Qt::Unchecked;
					}
					else
					{
						nState=Qt::Checked;
					}

					setItemCheckState(sindex.row(), nState, sindex.parent());
					setExpanded(index, true);

					return;
				}
			}
		}
	}

	QTreeView::mousePressEvent(event);

	if (event->button() == Qt::RightButton)
	{
		emit mouseRightButtonPressed(event->pos());

		event->ignore();
	}
}

void QTreeViewSyncCommit::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_F5:
		{
			emit refreshKeyPress();

			event->ignore();
		}
		return;
	default:
		break;
	}

	QTreeView::keyPressEvent(event);
}

void QTreeViewSyncCommit::clear()
{
	if (m_pModel != NULL)
	{
		m_pModel->setRowCount(0);
	}
}

QModelIndexList QTreeViewSyncCommit::selectedRows()
{
 	QItemSelectionModel* pModel=selectionModel();
 	if (pModel == NULL)
 	{
 		return QModelIndexList();
 	}
 
 	return pModel->selectedRows();
}

QVector<QPair<FileInfo, int> > QTreeViewSyncCommit::selectedItems(const QModelIndex& parent)
{
	QVector<QPair<FileInfo, int> > files;

	int nNumRow=m_pModel->rowCount(parent);
	for (int nRow=0; nRow<nNumRow; nRow++)
	{
		QModelIndex index=m_pModel->index(nRow, NameTitle, parent);
		if (m_pModel->data(index, ItemRole).toInt() == GroupRole)
		{
			if (m_pModel->data(index, CheckedRole).toInt() != Qt::Unchecked)
			{
				files+=selectedItems(index);
			}
		}
		else
		{
			if (m_pModel->data(index, CheckedRole).toInt() == Qt::Checked)
			{
				QPair<FileInfo, int> peer;
				peer.first=qvariant_cast<FileInfo>(m_pModel->data(index, UserDataRole));

				index=m_pModel->index(nRow, StatusTitle, parent);
				peer.second=m_pModel->data(index, Qt::DisplayRole).toInt();
				
				files.append(peer);
			}
		}
	}

	return files;
}

bool QTreeViewSyncCommit::fetchItem(const QModelIndex& index, QPair<FileInfo, int>& file)
{
	QModelIndex sindex=mapToSourceItem(index);

	QModelIndex cindex=m_pModel->index(sindex.row(), NameTitle, sindex.parent());
	if (m_pModel->data(cindex, ItemRole).toInt() != FileRole)
	{
		return false;
	}
	else
	{
		file.first=qvariant_cast<FileInfo>(m_pModel->data(cindex, UserDataRole));
		if (file.first.type != File && file.first.type != HideFile)
		{
			return false;
		}

		cindex=m_pModel->index(sindex.row(), StatusTitle, sindex.parent());
		file.second=m_pModel->data(cindex, Qt::DisplayRole).toInt();
	}

	return true;
}

void QTreeViewSyncCommit::save()
{
	QVector<DiffFile> files;

	if (m_pModel != NULL)
	{
		saveRecords(files);
	}

	QWriteWorkspaceRecordsToXmlFile writer(appendPath(m_strHistory, record()+".xml"), m_strWorkspace, files);
	writer.write(false);
}

void QTreeViewSyncCommit::saveRecords(QVector<DiffFile>& files, const QModelIndex & parent/* =QModelIndex */)
{
	int nRows=m_pModel->rowCount(parent);
	for (int nRow=0; nRow<nRows; nRow++)
	{
		int nRole=m_pModel->data(m_pModel->index(nRow, NameTitle, parent), ItemRole).toInt();

		if (nRole == FileRole)
		{
			int nStatus=m_pModel->data(m_pModel->index(nRow, StatusTitle, parent), Qt::DisplayRole).toInt();

			QModelIndex index=m_pModel->index(nRow, NameTitle, parent);

			FileInfo file=qvariant_cast<FileInfo>(m_pModel->data(index, UserDataRole));
			QString strFileName=file.name;
			int nChecked=m_pModel->data(index, CheckedRole).toInt();
			if (nChecked == Qt::Checked)
			{
				if (nStatus != Deleted)
				{
					file=entryInfo(QFileInfo(appendPath(m_strWorkspace, file.name)), false);
				}

				nStatus=(nStatus==NonVersioned? Added:nStatus);
			}
			else
			{
				nStatus=(nStatus==Added? NonVersioned:nStatus);
			}

			DiffFile dfFile;
			dfFile.path=strFileName;
			dfFile.type=file.type;
			dfFile.size=file.size;
			dfFile.modified=file.lastModified;
			dfFile.status=nStatus;

			files.append(dfFile);
		}
		else if (nRole == GroupRole)
		{
			saveRecords(files, m_pModel->index(nRow, 0, parent));
		}
	}
}

QTreeViewSyncCommitRunnable::QTreeViewSyncCommitRunnable(const QString& workspace, const QString& history, 
														 const QString& record, bool checkable, bool enableRestrict, QObject* parent/* =0 */)
:QObject(parent)
,m_strWorkspace(workspace)
,m_strHistory(history)
,m_strRecord(record)
,m_bCheckable(checkable)
,m_pModel(NULL)
,m_bRestrictFilter(enableRestrict)
{

}

QTreeViewSyncCommitRunnable::~QTreeViewSyncCommitRunnable()
{

}

void QTreeViewSyncCommitRunnable::run()
{
	if (!m_strHistory.isEmpty())
	{
		QReadWorkspaceRecordsFromXmlFile reader(appendPath(m_strHistory, m_strRecord+".xml"), m_strWorkspace);
		if (reader.read())
		{
			m_files=reader.result();
		}
	}

	QStringList labels;
	labels<<tr("Name")<<tr("Size")<<tr("Type")<<tr("Status")<<tr("Last Modified");

	m_pModel=new QTreeModel(labels.size());
	m_pModel->setHorizontalHeaderLabels(labels);

	checkBackupFiles();

	if (m_pModel != NULL)
	{
		check();
	}

	emit finished(m_pModel);
}



void QTreeViewSyncCommitRunnable::checkBackupFiles()
{
	m_filesBackup.clear();

	if (m_strHistory.isEmpty())
	{
		return;
	}

	QString strHistory=appendPath(m_strHistory, m_strRecord);

	int nRevision=-1;
	QString strFilename=appendPath(strHistory, QString("%1.xml").arg(m_strRecord));

	QReadCheckRecordFromXmlFile reader(strFilename);
	if (reader.read())
	{
		reader.result(nRevision, m_filesBackup);
	}

	int nCurRevision=nRevision;

	QDir dir(strHistory);
	QStringList folders=dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks, QDir::Time|QDir::Reversed);
	foreach (QString folder, folders)
	{
		bool bOk;
		int nCur=folder.toInt(&bOk);
		if (!bOk || nCur<=nRevision)
		{
			continue;
		}

		bool bNew=(nCur>nCurRevision);

		QReadRequestRecordFormXmlFile reader(appendPath(strHistory, QString("%1/%2.xml").arg(folder).arg(m_strRecord)));
		if (reader.read())
		{
			QVector<DiffFile> files=reader.result();
			for (int i=0; i<files.size(); i++)
			{
				DiffFile& file=files[i];
				if (m_bRestrictFilter && isRestrictFile(file.path))
				{
					continue;
				}

				if (bNew || !m_filesBackup.contains(file.path))
				{
					QString strPath=file.path;
					file.path=QString("%1/%2").arg(folder).arg(file.path);
					m_filesBackup.insert(strPath, file);
				}
			}
		}

		nCurRevision=(nCur>nCurRevision? nCur:nCurRevision);
	}

	if (nCurRevision > nRevision)
	{
		QWriteCheckRecordToXmlFile writer(strFilename, nCurRevision, m_filesBackup.values().toVector());
		writer.write();
	}
}

bool QTreeViewSyncCommitRunnable::traceSubDir(const QString& folder, const QString& suffix, const QModelIndex& pindex)
{
	bool bChecked=false;

	QDir dir(folder);
	QStringList dirs=dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks);
	foreach (QString strDir, dirs)
	{
		QString strFolder=suffix.isEmpty()? strDir:suffix+"/"+strDir;
		bChecked|=checkDir(strFolder, pindex);

		m_files.remove(strFolder);

		bChecked|=traceSubDir(appendPath(folder, strDir), strFolder, pindex);
	}

	QStringList files=dir.entryList(QDir::Files|QDir::NoDotAndDotDot|QDir::NoSymLinks);
	foreach (QString filename, files)
	{
		if (m_bRestrictFilter && isRestrictFile(filename))
		{
			continue;
		}

		QString strFileName=suffix.isEmpty()? filename:suffix+"/"+filename;
		bChecked|=checkFile(strFileName, pindex);

		m_files.remove(strFileName);
	}

	return bChecked;
}

bool QTreeViewSyncCommitRunnable::checkDir(const QString& folder, const QModelIndex& pindex)
{
	bool bChecked=false;

	QString strHistory=appendPath(m_strHistory, m_strRecord);

	QFileInfo file(appendPath(m_strWorkspace, folder));
	if (file.exists())
	{
		if (file.isDir())
		{
			FileInfo info=entryInfo(file, false);
			info.name=folder;
			info.size=0;

			int nStatus=NonVersioned;
			if (m_files.contains(folder))
			{
				const DiffFile& diff=m_files[folder];
				nStatus=(diff.status == NonVersioned? NonVersioned:Unmodified);
			}
			else
			{
				bChecked=true;
				nStatus=Added;
			}

			int nRow=m_pModel->rowCount(pindex);
			m_pModel->insertRow(nRow, pindex);

			QModelIndex index=m_pModel->index(nRow, QTreeViewSyncCommit::NameTitle, pindex);
			m_pModel->setData(index, folder, Qt::DisplayRole);
			m_pModel->setData(index, QTreeViewSyncCommit::FileRole, QTreeViewSyncCommit::ItemRole);
			m_pModel->setData(index, qVariantFromValue(info), QTreeViewSyncCommit::UserDataRole);
			m_pModel->setData(index, bChecked? Qt::Checked:Qt::Unchecked, QTreeViewSyncCommit::CheckedRole);
			m_pModel->setData(index, m_bCheckable, QTreeViewSyncCommit::CheckableRole);

			index=m_pModel->index(nRow, QTreeViewSyncCommit::SizeTitle, pindex);
			m_pModel->setData(index, 0, Qt::DisplayRole);

			index=m_pModel->index(nRow, QTreeViewSyncCommit::TypeTitle, pindex);
			m_pModel->setData(index, fileType(info), Qt::DisplayRole);

			index=m_pModel->index(nRow, QTreeViewSyncCommit::ModifiedTitle, pindex);
			m_pModel->setData(index, info.lastModified, Qt::DisplayRole);

			index=m_pModel->index(nRow, QTreeViewSyncCommit::StatusTitle, pindex);
			m_pModel->setData(index, nStatus, Qt::DisplayRole);
		}
	}
	else
	{
		int nStatus=Deleted;

		FileInfo info;

		if (m_files.contains(folder))
		{
			const DiffFile& diff=m_files[folder];
			bChecked=diff.status == Deleted? false:true;

			info.name=diff.path;
			info.type=diff.type;
			info.lastModified=diff.modified;
		}
		else
		{
			bChecked=true;

			info.name=folder;
			info.type=Dir;
		}

		info.size=0;

		int nRow=m_pModel->rowCount(pindex);
		m_pModel->insertRow(nRow, pindex);

		QModelIndex index=m_pModel->index(nRow, QTreeViewSyncCommit::NameTitle, pindex);
		m_pModel->setData(index, folder, Qt::DisplayRole);
		m_pModel->setData(index, QTreeViewSyncCommit::FileRole, QTreeViewSyncCommit::ItemRole);
		m_pModel->setData(index, qVariantFromValue(info), QTreeViewSyncCommit::UserDataRole);
		m_pModel->setData(index, bChecked? Qt::Checked:Qt::Unchecked, QTreeViewSyncCommit::CheckedRole);
		m_pModel->setData(index, m_bCheckable, QTreeViewSyncCommit::CheckableRole);

		index=m_pModel->index(nRow, QTreeViewSyncCommit::SizeTitle, pindex);
		m_pModel->setData(index, 0, Qt::DisplayRole);

		index=m_pModel->index(nRow, QTreeViewSyncCommit::TypeTitle, pindex);
		m_pModel->setData(index, fileType(info), Qt::DisplayRole);

		index=m_pModel->index(nRow, QTreeViewSyncCommit::StatusTitle, pindex);
		m_pModel->setData(index, nStatus, Qt::DisplayRole);

		index=m_pModel->index(nRow, QTreeViewSyncCommit::ModifiedTitle, pindex);
		m_pModel->setData(index, info.lastModified, Qt::DisplayRole);
	}

	return bChecked;
}

bool QTreeViewSyncCommitRunnable::checkFile(const QString& name, const QModelIndex& pindex)
{
	bool bChecked=false;

	QString strHistory=appendPath(m_strHistory, m_strRecord);

	QFileInfo file(appendPath(m_strWorkspace, name));
	if (file.exists())
	{
		if (file.isFile())
		{
			FileInfo info=entryInfo(file, false);
			info.name=name;

			int nStatus=NonVersioned;
			if (m_files.contains(name))
			{
				const DiffFile& diff=m_files[name];
				if (diff.size != info.size || ((diff.modified.toString(FILE_MODIFIED_DATETIME_FORMAT) != info.lastModified.toString(FILE_MODIFIED_DATETIME_FORMAT) //进行字符串比较是由于文件最后修改日期包含毫秒级
					) && (!m_filesBackup.contains(name) || 
					QFileCompareFactory::instance()->isModified(appendPath(m_strWorkspace, name), appendPath(strHistory, m_filesBackup[name].path)))))
				{
					bChecked=true;
					nStatus=Modified;
				}
				else
				{
					nStatus=(diff.status == NonVersioned? NonVersioned:Unmodified);
				}

				info.lastModified=diff.modified;
				info.size=diff.size;
				info.type=diff.type;
			}
			else
			{
				bChecked=true;
				nStatus=Added;
			}

			int nRow=m_pModel->rowCount(pindex);
			m_pModel->insertRow(nRow, pindex);

			QModelIndex index=m_pModel->index(nRow, QTreeViewSyncCommit::NameTitle, pindex);
			m_pModel->setData(index, name, Qt::DisplayRole);
			m_pModel->setData(index, QTreeViewSyncCommit::FileRole, QTreeViewSyncCommit::ItemRole);
			m_pModel->setData(index, qVariantFromValue(info), QTreeViewSyncCommit::UserDataRole);
			m_pModel->setData(index, bChecked? Qt::Checked:Qt::Unchecked, QTreeViewSyncCommit::CheckedRole);
			m_pModel->setData(index, m_bCheckable, QTreeViewSyncCommit::CheckableRole);

			index=m_pModel->index(nRow, QTreeViewSyncCommit::SizeTitle, pindex);
			m_pModel->setData(index, info.size, Qt::DisplayRole);

			index=m_pModel->index(nRow, QTreeViewSyncCommit::TypeTitle, pindex);
			m_pModel->setData(index, fileType(info), Qt::DisplayRole);

			index=m_pModel->index(nRow, QTreeViewSyncCommit::ModifiedTitle, pindex);
			m_pModel->setData(index, info.lastModified, Qt::DisplayRole);

			index=m_pModel->index(nRow, QTreeViewSyncCommit::StatusTitle, pindex);
			m_pModel->setData(index, nStatus, Qt::DisplayRole);
		}
	}
	else
	{
		int nStatus=Deleted;

		FileInfo info;

		if (m_files.contains(name))
		{
			const DiffFile& diff=m_files[name];
			bChecked=diff.status == Deleted? false:true;

			info.name=diff.path;
			info.type=diff.type;
			info.size=diff.size;
			info.isExecutable=false;
		}
		else
		{
			bChecked=true;

			info.name=name;
			info.type=File;
			info.size=0;
			info.isExecutable=false;
		}

		info.lastModified=QDateTime::currentDateTime();

		int nRow=m_pModel->rowCount(pindex);
		m_pModel->insertRow(nRow, pindex);

		QModelIndex index=m_pModel->index(nRow, QTreeViewSyncCommit::NameTitle, pindex);
		m_pModel->setData(index, name, Qt::DisplayRole);
		m_pModel->setData(index, QTreeViewSyncCommit::FileRole, QTreeViewSyncCommit::ItemRole);
		m_pModel->setData(index, qVariantFromValue(info), QTreeViewSyncCommit::UserDataRole);
		m_pModel->setData(index, bChecked? Qt::Checked:Qt::Unchecked, QTreeViewSyncCommit::CheckedRole);
		m_pModel->setData(index, m_bCheckable, QTreeViewSyncCommit::CheckableRole);

		index=m_pModel->index(nRow, QTreeViewSyncCommit::SizeTitle, pindex);
		m_pModel->setData(index, info.size, Qt::DisplayRole);

		index=m_pModel->index(nRow, QTreeViewSyncCommit::TypeTitle, pindex);
		m_pModel->setData(index, fileType(info), Qt::DisplayRole);

		index=m_pModel->index(nRow, QTreeViewSyncCommit::StatusTitle, pindex);
		m_pModel->setData(index, nStatus, Qt::DisplayRole);

		index=m_pModel->index(nRow, QTreeViewSyncCommit::ModifiedTitle, pindex);
		m_pModel->setData(index, info.lastModified, Qt::DisplayRole);
	}

	return bChecked;
}
