#include "qtreeviewupdatefile.h"

#include <QMouseEvent>
#include <QDir>
#include <QApplication>
#include <QAbstractProxyModel>

#include "common.h"
#include "qimessage.h"
#include "qglobalinfo.h"
#include "qlistviewsyncfilerequestserver.h"
#include "qdlginstantupdate.h"

QTreeViewUpdateFile::QTreeViewUpdateFile(QWidget *parent)
	: QTreeView(parent)
	,m_pModel(NULL)
	,m_pDelegate(NULL)
	,m_bRestrictFilter(true)
{
	qRegisterMetaType<QTreeModel*>("QTreeModel*");

	setCustomItemDelegate();
}

QTreeViewUpdateFile::~QTreeViewUpdateFile()
{
	SAFE_DELETE_CLASS(m_pModel);
	SAFE_DELETE_CLASS(m_pDelegate);
}

void QTreeViewUpdateFile::setRestrictFilterEnabled(bool enable)
{
	m_bRestrictFilter=enable;
}

void QTreeViewUpdateFile::setLocalLastModifiedEnabled(bool enable)
{
	m_bLocalLastModified=enable;
}

void QTreeViewUpdateFile::setCustomItemDelegate()
{
	m_pDelegate=new QUpdateFileDelegate(this);
	setItemDelegate(m_pDelegate);
}

void QTreeViewUpdateFile::setTreeModel(QTreeModel* model)
{
	setModel(model);

	SAFE_DELETE_CLASS(m_pModel);
	m_pModel=model;
}

QModelIndexList QTreeViewUpdateFile::manuSelectItems()
{
	QModelIndexList items;

	QItemSelectionModel* pSelectionModel=selectionModel();
	if (pSelectionModel != NULL)
	{
		QModelIndexList indexes=pSelectionModel->selectedIndexes();

		QAbstractProxyModel* pModel=(QAbstractProxyModel*)model();
		if (pModel != NULL)
		{
			foreach (QModelIndex index, indexes)
			{
				items.append(pModel->mapToSource(index));
			}
		}
		else
		{
			items=indexes;
		}
	}

	return items;
}

bool QTreeViewUpdateFile::hasManuSelected() const
{
	return false;
}

QTreeModel*	QTreeViewUpdateFile::copy() const
{
	if (m_pModel == NULL)
	{
		return NULL;
	}

	QTreeModel* pModel=new QTreeModel(m_pModel->columnCount());
	pModel->setHorizontalHeaderLabels(m_pModel->horizontalHeaderLabels());

	copyChildrenItems(m_pModel, QModelIndex(), pModel, QModelIndex());

	return pModel;
}

void QTreeViewUpdateFile::checkItems(int state, const QModelIndex& parent/* =QModelIndex */)
{
	if (m_pModel == NULL)
	{
		return;
	}

	int nRowCount=m_pModel->rowCount(parent);
	if (nRowCount == 0)
	{
		return;
	}

	for (int nRow=0; nRow<nRowCount; nRow++)
	{
		QModelIndex index=m_pModel->index(nRow, NameTitle, parent);
		int nRole=m_pModel->data(index, ItemRole).toInt();
		if (nRole == GroupRole)
		{
			checkItems(state, index);
		}
		else if (nRole == FileRole)
		{
			int nCheck=m_pModel->data(index, CheckedRole).toInt();

			index=m_pModel->index(nRow, StatusTitle, parent);
			int nState=m_pModel->data(index, Qt::DisplayRole).toInt();
			if (state == nState && nCheck != Qt::Checked)
			{
				setItemCheckState(nRow, Qt::Checked, parent);
			}
			else if (state != nState && nCheck == Qt::Checked)
			{
				setItemCheckState(nRow, Qt::Unchecked, parent);
			}
		}
	}
}

void QTreeViewUpdateFile::copyChildrenItems(const QTreeModel* smodel, const QModelIndex & sparent, QTreeModel* tmodel, const QModelIndex & tparent) const
{
	if (smodel == NULL || tmodel == NULL)
	{
		return;
	}

	int nRowCount=smodel->rowCount(sparent);
	if (nRowCount == 0)
	{
		return;
	}

	int nRow=0;

	for (int nIndex=0; nIndex<nRowCount; nIndex++)
	{
		QModelIndex sindex=smodel->index(nRow, NameTitle, sparent);
		int nRole=smodel->data(sindex, ItemRole).toInt();
		if (nRole == GroupRole)
		{
			tmodel->insertRow(nRow, tparent);

			QModelIndex tindex=tmodel->index(nRow, NameTitle, tparent);
			tmodel->setData(tindex, nRole, ItemRole);
			tmodel->setData(tindex, smodel->data(sindex, Qt::DisplayRole), Qt::DisplayRole);

			sindex=smodel->index(nRow, StatusTitle, sparent);
			int nState=smodel->data(sindex, Qt::DisplayRole).toInt();

			tindex=tmodel->index(nRow, StatusTitle, tparent);
			tmodel->setData(tindex, nState, Qt::DisplayRole);

			copyChildrenItems(smodel, sindex, tmodel, tindex);

			nRow++;
		}
		else if (nRole == FileRole)
		{
			FileInfo file=qvariant_cast<FileInfo>(smodel->data(sindex, LocalFileRole));

			sindex=smodel->index(nRow, StatusTitle, sparent);
			int nState=smodel->data(sindex, Qt::DisplayRole).toInt();
			if (nState == Add || nState == Deleted)
			{
				continue;
			}

			tmodel->insertRow(nRow, tparent);

			QModelIndex tindex=tmodel->index(nRow, NameTitle, tparent);
			tmodel->setData(tindex, file.name, Qt::DisplayRole);
			tmodel->setData(tindex, FileRole, ItemRole);
			tmodel->setData(tindex, qVariantFromValue(file), LocalFileRole);
//			tmodel->setData(tindex, bModified? Qt::Checked:Qt::Unchecked, CheckedRole);

			tindex=tmodel->index(nRow, SizeTitle, tparent);
			tmodel->setData(tindex, file.size, Qt::DisplayRole);

			tindex=tmodel->index(nRow, TypeTitle, tparent);
			tmodel->setData(tindex, fileType(file), Qt::DisplayRole);

			tindex=tmodel->index(nRow, UpdatedTitle, tparent);
			tmodel->setData(tindex, file.lastModified, Qt::DisplayRole);

			tindex=tmodel->index(nRow, StatusTitle, tparent);
			tmodel->setData(tindex, nState, Qt::DisplayRole);

			nRow++;
		}
	}
}

QModelIndex QTreeViewUpdateFile::mapToSourceItem(const QModelIndex& index)
{
	return index;
}

QModelIndex QTreeViewUpdateFile::mapFromSourceItem(const QModelIndex& index)
{
	return index;
}

void QTreeViewUpdateFile::setWorkspace(const QString& workspace, const QString& hostname, const QString& address)
{
//	save();
	clear();
	
	m_strWorkspace=workspace;
	m_strHostName=hostname;
	m_strAddress=address;

	check();
}

QVector<QPair<DiffFile, FileInfo> > QTreeViewUpdateFile::selectedItems(const QModelIndex& parent)
{
	QVector<QPair<DiffFile, FileInfo> > files;

	if (m_pModel == NULL)
	{
		return files;
	}

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
				QPair<DiffFile, FileInfo> peer;
				peer.first=qvariant_cast<DiffFile>(m_pModel->data(index, DiffFileRole));
				peer.second=qvariant_cast<FileInfo>(m_pModel->data(index, LocalFileRole));

				files.append(peer);
			}
		}
	}

	return files;
}

bool QTreeViewUpdateFile::hasSelected() const
{
	if (m_pModel == NULL)
	{
		return false;
	}

	int nNumRow=m_pModel->rowCount();
	for (int nRow=0; nRow<nNumRow; nRow++)
	{
		QModelIndex index=m_pModel->index(nRow, NameTitle);
		if (m_pModel->data(index, ItemRole).toInt() == GroupRole)
		{
			if (m_pModel->data(index, CheckedRole).toInt() != Qt::Unchecked)
			{
				return true;
			}
		}
		else
		{
			if (m_pModel->data(index, CheckedRole).toInt() == Qt::Checked)
			{
				return true;
			}
		}
	}

	return false;
}

int QTreeViewUpdateFile::rowCount(const QModelIndex& parent) const
{
	return m_pModel==NULL? 0:m_pModel->rowCount(parent);
}

int QTreeViewUpdateFile::itemRole(int row, const QModelIndex & parent) const
{
	Q_ASSERT(row >=0 && row < m_pModel->rowCount(parent));

	return m_pModel->data(m_pModel->index(row, NameTitle, parent), ItemRole).toInt();
}

int QTreeViewUpdateFile::itemState(int row, const QModelIndex & parent) const
{
	Q_ASSERT(row >=0 && row < m_pModel->rowCount(parent));

	return m_pModel->data(m_pModel->index(row, StatusTitle, parent), Qt::DisplayRole).toInt();
}

void QTreeViewUpdateFile::setItemState(int row, int state, const QModelIndex & parent)
{
	if (m_pModel == NULL)
	{
		return;
	}

	Q_ASSERT(row >=0 && row < m_pModel->rowCount(parent));

	QModelIndex index=m_pModel->index(row, NameTitle, parent);
	DiffFile file=qvariant_cast<DiffFile>(m_pModel->data(index, DiffFileRole));
	file.status=state;
	m_pModel->setData(index, qVariantFromValue(file), DiffFileRole);

	index=m_pModel->index(row, StatusTitle, parent);
	m_pModel->setData(index, state, Qt::DisplayRole);

	setParentItemState(parent);
}

void QTreeViewUpdateFile::itemModified(const QModelIndex& index)
{
	if (m_pModel == NULL)
	{
		return;
	}

	QModelIndex sindex=mapToSourceItem(index);
	QModelIndex cindex=m_pModel->index(sindex.row(), StatusTitle, sindex.parent());
	if (cindex.isValid())
	{
		int nState=m_pModel->data(cindex, Qt::DisplayRole).toInt();

		if (nState == Delete)
		{
			nState=Deleted;
		}
		else if (nState == NonVersioned)
		{
			return;
		}
		else
		{
			nState=Latest;
		}

		setItemState(cindex.row(), nState, cindex.parent());

		//Ð´¼ÇÂ¼ÎÄ¼þ
		QMap<QString, DiffFile> files;

		QString strConfigure=appendPath(getAppParentDir(), QString("%1%2.xml").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH).arg(record()));

		QReadUpdateRecordsFromXmlFile reader(strConfigure, m_strWorkspace, m_strHostName, m_strAddress);
		if (reader.read())
		{
			files=reader.result();
		}

		DiffFile file=qvariant_cast<DiffFile>(m_pModel->data(m_pModel->index(sindex.row(), NameTitle, sindex.parent()), DiffFileRole));
		file.path=parseFileNameFromRevision(file.path);

		if (nState != Deleted)
		{
			FileInfo info=entryInfo(QFileInfo(appendPath(m_strWorkspace, file.path)), false);
			info.name=file.path;

			file.size=info.size;
			file.type=info.type;
			file.modified=info.lastModified;

			m_pModel->setData(m_pModel->index(sindex.row(), NameTitle, sindex.parent()), qVariantFromValue(info), LocalFileRole);
			m_pModel->setData(m_pModel->index(sindex.row(), SizeTitle, sindex.parent()), info.size, Qt::DisplayRole);
			m_pModel->setData(m_pModel->index(sindex.row(), UpdatedTitle, sindex.parent()), info.lastModified, Qt::DisplayRole);

			files.insert(file.path, file);
		}
		else
		{
			files.remove(file.path);
		}

		QWriteUpdateRecordsToXmlFile writer(strConfigure, m_strWorkspace, m_strHostName, m_strAddress, files.values().toVector());
		writer.write(false);

		

		setItemCheckState(sindex.row(), Qt::Unchecked, sindex.parent());

		selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
		update(index);
	}
}

bool QTreeViewUpdateFile::hasModifiedItem() const
{
	return isModified(m_pModel);
}

bool QTreeViewUpdateFile::isModified(const QTreeModel* model)
{
	bool bModified=false;

	int nNumRow=model->rowCount();
	for (int nRow=0; nRow<nNumRow; nRow++)
	{
		QModelIndex index=model->index(nRow, StatusTitle);
		int nState=model->data(index, Qt::DisplayRole).toInt();

		int nRole=model->data(model->index(nRow, QTreeViewUpdateFile::NameTitle), QTreeViewUpdateFile::ItemRole).toInt();
		if (nRole == QTreeViewUpdateFile::FileRole)
		{
			if (nState == Add || nState == Delete || nState == Modify)
			{
				bModified=true;
				break;
			}
		}
		else if (nRole == QTreeViewUpdateFile::GroupRole)
		{
			if (nState == Modified)
			{
				bModified=true;
				break;
			}
		}
	}

	return bModified;
}

int QTreeViewUpdateFile::itemCheckState(int row, const QModelIndex& parent) const
{
	if (m_pModel == NULL)
	{
		return Qt::Unchecked;
	}

	Q_ASSERT(row>=0 && row < m_pModel->rowCount(parent));

	QModelIndex index=m_pModel->index(row, NameTitle, parent);
	return m_pModel->data(index, CheckedRole).toInt();
}

void QTreeViewUpdateFile::setItemCheckState(int row, int state, const QModelIndex& parent)
{
	if (m_pModel == NULL)
	{
		return;
	}

	Q_ASSERT(row>=0 && row < m_pModel->rowCount(parent));

	setChildItemCheckState(row, state, parent);
	setParentItemCheckState(parent);
}

void QTreeViewUpdateFile::setParentItemState(const QModelIndex & parent)
{
	if (parent == QModelIndex())
	{
		return;
	}

	int nNumRow=m_pModel->rowCount(parent);
	int nGroupState=Unmodified;

	for (int nRow=0; nRow<nNumRow; nRow++)
	{
		QModelIndex index=m_pModel->index(nRow, StatusTitle, parent);
		int nState=m_pModel->data(index, Qt::DisplayRole).toInt();

		if (nState == Add || nState == Delete || nState == Modify)
		{
			nGroupState=Modified;
		}
	}

	m_pModel->setData(m_pModel->index(parent.row(), StatusTitle, parent.parent()), nGroupState, Qt::DisplayRole);

	setParentItemState(parent.parent());
}

void QTreeViewUpdateFile::setChildItemCheckState(int row, int state, const QModelIndex& parent)
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

void QTreeViewUpdateFile::setParentItemCheckState(const QModelIndex& parent)
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

void QTreeViewUpdateFile::setCustomModel(QTreeModel* model)
{
	setTreeModel(model);

	setColumnWidth(NameTitle, 260);
	setColumnWidth(SizeTitle, 100);
	setColumnWidth(TypeTitle, 100);
	setColumnWidth(StatusTitle, 100);
	setColumnWidth(UpdatedTitle, 150);
}

void QTreeViewUpdateFile::finished(QTreeModel* model)
{
	setCustomModel(model);

	emit refreshModelFinished();
}

void QTreeViewUpdateFile::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
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

				emit selectedChanged();

				return;
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

void QTreeViewUpdateFile::keyPressEvent(QKeyEvent* event)
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

void QTreeViewUpdateFile::clear()
{
	if (m_pModel != NULL)
	{
		m_pModel->setRowCount(0);
	}
}

bool QTreeViewUpdateFile::fetchItem(int row, QPair<DiffFile, FileInfo>& peer, const QModelIndex& parent/* =QModelIndex */)
{
	QAbstractItemModel* pModel=model();
	if (pModel == NULL)
	{
		return false;
	}

	QModelIndex index=pModel->index(row, NameTitle, parent);

	return fetchItem(index, peer);
}

bool QTreeViewUpdateFile::fetchItem(const QModelIndex& index, QPair<DiffFile, FileInfo>& peer)
{
	if (m_pModel == NULL)
	{
		return false;
	}

	QModelIndex sindex=mapToSourceItem(index);

	QModelIndex cindex=m_pModel->index(sindex.row(), NameTitle, sindex.parent());
	if (m_pModel->data(cindex, ItemRole).toInt() != FileRole)
	{
		return false;
	}
	else
	{
		peer.first=qvariant_cast<DiffFile>(m_pModel->data(cindex, DiffFileRole));
		peer.second=qvariant_cast<FileInfo>(m_pModel->data(cindex, LocalFileRole));
	}

	return true;
}

// void QTreeViewUpdateFile::save()
// {
// 	if (m_pModel != NULL)
// 	{
// 		save(appendPath(getAppParentDir(), QString("%1%2.xml").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH).arg(record())));
// 	}
//}

// void QTreeViewUpdateFile::save(const QString& filename)
// {
// 	QVector<DiffFile> files;
// 	recordFiles(files);
// 
// 	QWriteUpdateRecordsToXmlFile writer(filename, m_strWorkspace, m_strHostName, m_strAddress, files);
//}

// void QTreeViewUpdateFile::recordFiles(QVector<DiffFile>& files, const QModelIndex & parent)
// {
// 	if (m_pModel == NULL)
// 	{
// 		return;
// 	}
// 
// 	int nRows=m_pModel->rowCount(parent);
// 	for (int nRow=0; nRow<nRows; nRow++)
// 	{
// 		int nRole=m_pModel->data(m_pModel->index(nRow, QTreeViewUpdateFile::NameTitle, parent), QTreeViewUpdateFile::ItemRole).toInt();
// 
// 		if (nRole == QTreeViewUpdateFile::FileRole)
// 		{
// 			int nStatus=m_pModel->data(m_pModel->index(nRow, QTreeViewUpdateFile::StatusTitle, parent), Qt::DisplayRole).toInt();
// 
// 			if (nStatus != NonVersioned && nStatus != Deleted)
// 			{
// 				QModelIndex index=m_pModel->index(nRow, QTreeViewUpdateFile::NameTitle, parent);
// 
// 				DiffFile diff=qvariant_cast<DiffFile>(m_pModel->data(index, QTreeViewUpdateFile::DiffFileRole));
// 				FileInfo info=qvariant_cast<FileInfo>(m_pModel->data(index, QTreeViewUpdateFile::LocalFileRole));
// 
// 				DiffFile file;
// 				file.path=info.name;
// 				file.type=info.type;
// 				file.size=info.type;
// 				file.modified=info.lastModified;
// 				file.status=nStatus;
// 				file.version=diff.version;
// 
// 				files.append(file);
// 			}
// 		}
// 		else if (nRole == QTreeViewUpdateFile::GroupRole)
// 		{
// 			recordFiles(files, m_pModel->index(nRow, QTreeViewUpdateFile::NameTitle, parent));
// 		}
// 	}
// }

QUpdateFileRunnable::QUpdateFileRunnable(const QString& workspace, const QString& catagory, const QString& hostname, const QString& address, 
										 bool restrictFilter, bool localLastModified, bool immediate/* =false */, QObject* parent/* =0 */)
:QObject(parent)
,m_strWorkspace(workspace)
,m_strCatagory(catagory)
,m_strHostName(hostname)
,m_strAddress(address)
,m_bImmediate(immediate)
,m_bRestrictFilter(restrictFilter)
,m_bLocalLastModified(localLastModified)
,m_pModel(NULL)
{

}

QUpdateFileRunnable::~QUpdateFileRunnable()
{

}

void QUpdateFileRunnable::run()
{
	bool bModified=check(m_bImmediate);

	if (!m_bImmediate)
	{
		emit finished(m_pModel);
	}
	else if (bModified)
	{
		emit modified(m_strHostName, m_strAddress);
	}
}

bool QUpdateFileRunnable::check(bool immediate/* =true */)
{
	if (!m_strAddress.isEmpty())
	{
		QReadUpdateRecordsFromXmlFile reader(appendPath(getAppParentDir(), QString("%1%2.xml").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH).arg(m_strCatagory)), 
			m_strWorkspace, m_strHostName, m_strAddress);
		if (reader.read())
		{
			m_files=reader.result();
		}
	}

	findRequestFiles();

	if (!immediate)
	{
		QStringList labels;
		labels<<tr("Name")<<tr("Size")<<tr("Type")<<tr("Status")<<tr("Updated");

		m_pModel=new QTreeModel(labels.size());
		m_pModel->setHorizontalHeaderLabels(labels);
	}

	bool bModified=readFiles(immediate);

	QWriteUpdateRecordsToXmlFile writer(appendPath(getAppParentDir(), QString("%1%2.xml").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH).arg(m_strCatagory)), 
		m_strWorkspace, m_strHostName, m_strAddress, m_files.values().toVector());
	writer.write(false);

	return bModified;
}

void QUpdateFileRunnable::findRequestFiles()
{
	m_filesRequest.clear();

	if (m_strAddress.isEmpty())
	{
		return;
	}

	QString strWorkspace=appendPath(getAppParentDir(), QString("%1%2/%3/")
		.arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH).arg(m_strHostName.isEmpty()? m_strAddress:m_strAddress+"@"+m_strHostName).arg(m_strCatagory));

	int nRevision=-1;
	QString strFilename=appendPath(strWorkspace, QString("%1.xml").arg(m_strCatagory));
	
	QReadCheckRecordFromXmlFile reader(strFilename);
	if (reader.read())
	{
		reader.result(nRevision, m_filesRequest);
	}

	int nCurRevision=nRevision;

	QDir dir(strWorkspace);
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

		QReadRequestRecordFormXmlFile reader(appendPath(strWorkspace, QString("%1/%2.xml").arg(folder).arg(m_strCatagory)));
		if (reader.read())
		{
			QVector<DiffFile> files=reader.result();
			for (int i=0; i<files.size(); i++)
			{
				DiffFile& file=files[i];
				if (bNew || !m_filesRequest.contains(file.path))
				{
					QString strPath=file.path;
					file.path=QString("%1/%2").arg(folder).arg(file.path);
					file.version=nCur;

					if (file.status != Deleted)
					{
						file.modified=QFileInfo(appendPath(strWorkspace, file.path)).lastModified();
					}
					
					m_filesRequest.insert(strPath, file);
				}
			}

			nCurRevision=(nCur>nCurRevision? nCur:nCurRevision);
		}
	}

	if (nCurRevision > nRevision)
	{
		QWriteCheckRecordToXmlFile writer(strFilename, nCurRevision, m_filesRequest.values().toVector());
		writer.write();
	}
}

bool QUpdateFileRunnable::traceSubDir(const QString& folder, const QString& suffix/* = */, bool immidate/* =true */, const QModelIndex& pindex/* =QModelIndex */)
{
	bool bChecked=false;

	QDir dir(folder);
	QFileInfoList files=dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot|QDir::NoSymLinks);
	QListIterator<QFileInfo> it(files);
	while (it.hasNext())
	{
		const QFileInfo& file=it.next();

		QString strFileName=suffix.isEmpty()? file.fileName():suffix+"/"+file.fileName();

		if (file.isDir())
		{
			bChecked|=readDir(strFileName, immidate, pindex);

			bChecked|=traceSubDir(appendPath(folder, file.fileName()), strFileName, immidate, pindex);
		}
		else if (file.isFile())
		{
			if (m_bRestrictFilter && isRestrictFile(strFileName))
			{
				continue;
			}

			bChecked|=readFile(strFileName, immidate, pindex);
		}
	}

	return bChecked;
}

bool QUpdateFileRunnable::readDir(const QString& folder, bool immediate/* =true */, const QModelIndex& pindex/* =QModelIndex */)
{
	int nStatus=-1;
	bool bModified=false;

	FileInfo file;

	QFileInfo info(appendPath(m_strWorkspace, folder));
	if (info.exists() && info.isDir())
	{
		file=entryInfo(info, false);
		file.name=folder;

		nStatus=NonVersioned;

		if (m_filesRequest.contains(folder))
		{
			const DiffFile& diff=m_filesRequest[folder];
			
			if (diff.status == Deleted)
			{
				bModified=true;
				nStatus=Delete;
			}
			else
			{
				nStatus=Unmodified;
			}
		}
	}
	else
	{
		if (m_filesRequest.contains(folder))
		{
			const DiffFile& diff=m_filesRequest[folder];

			if (diff.status != Deleted)
			{
				file.name=folder;
				file.size=diff.size;
				file.type=diff.type;
				file.isExecutable=false;
				file.lastModified=diff.modified;

				nStatus=Add;

				bModified=true;
			}
		}
	}

	if (nStatus != -1 && !immediate)
	{
		int nRow=m_pModel->rowCount(pindex);
		m_pModel->insertRow(nRow, pindex);

		QModelIndex index=m_pModel->index(nRow, QTreeViewUpdateFile::NameTitle, pindex);
		m_pModel->setData(index, folder, Qt::DisplayRole);
		m_pModel->setData(index, QTreeViewUpdateFile::FileRole, QTreeViewUpdateFile::ItemRole);

		DiffFile df;
		if (m_filesRequest.contains(folder))
		{
			df=m_filesRequest[folder];
			df.status=nStatus;
		}
		else
		{
			df.path=file.name;
			df.size=file.size;
			df.type=file.type;
			df.modified=file.lastModified;
			df.status=nStatus;
			df.version=-1;
		}

		m_pModel->setData(index, qVariantFromValue(df), QTreeViewUpdateFile::DiffFileRole);
		m_pModel->setData(index, qVariantFromValue(file), QTreeViewUpdateFile::LocalFileRole);
		m_pModel->setData(index, bModified? Qt::Checked:Qt::Unchecked, QTreeViewUpdateFile::CheckedRole);

		index=m_pModel->index(nRow, QTreeViewUpdateFile::SizeTitle, pindex);
		m_pModel->setData(index, m_bLocalLastModified? info.size():df.size, Qt::DisplayRole);

		index=m_pModel->index(nRow, QTreeViewUpdateFile::TypeTitle, pindex);
		m_pModel->setData(index, fileType(file), Qt::DisplayRole);

		index=m_pModel->index(nRow, QTreeViewUpdateFile::UpdatedTitle, pindex);
		m_pModel->setData(index, m_bLocalLastModified? info.lastModified():df.modified, Qt::DisplayRole);

		index=m_pModel->index(nRow, QTreeViewUpdateFile::StatusTitle, pindex);
		m_pModel->setData(index, nStatus, Qt::DisplayRole);
	}

	m_filesRequest.remove(folder);

	return bModified;
}

bool QUpdateFileRunnable::readFile(const QString& name, bool immediate/* =true */, const QModelIndex & pindex/* =QModelIndex */)
{
	int nStatus=-1;
	bool bModified=false;

	FileInfo file;

	QFileInfo info(appendPath(m_strWorkspace, name));
	if (info.exists() && info.isFile())
	{
		file=entryInfo(info, false);
		file.name=name;

		nStatus=NonVersioned;

		if (m_filesRequest.contains(name))
		{
			const DiffFile& diff=m_filesRequest[name];
			
			if (diff.status == Deleted)
			{
				bModified=true;
				nStatus=Delete;
			}
			else
			{
				if (m_files.contains(name) && m_files[name].version >= diff.version &&
					file.lastModified.toString(FILE_MODIFIED_DATETIME_FORMAT) <= m_files[name].modified.toString(FILE_MODIFIED_DATETIME_FORMAT))
				{
					nStatus=m_files[name].status;
					bModified=(nStatus == Modified);
				}
				else
				{
					QString strBackup=appendPath(getAppParentDir(), QString("%1%2/%3/").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH)
						.arg(m_strHostName.isEmpty()? m_strAddress:m_strAddress+"@"+m_strHostName).arg(m_strCatagory));

					if (file.size != diff.size || 
						QFileCompareFactory::instance()->isModified(appendPath(m_strWorkspace, name), appendPath(strBackup, diff.path)))
					{
						nStatus=Modified;

						bModified=true;
					}
					else
					{
						nStatus=Unmodified;
					}

					DiffFile df;
					df.path=file.name;
					df.size=file.size;
					df.type=file.type;
					df.modified=file.lastModified;
					df.status=nStatus;
					df.version=diff.version;

					m_files.insert(name, df);
				}
			}
		}
	}
	else
	{
		if (m_filesRequest.contains(name))
		{
			const DiffFile& diff=m_filesRequest[name];

			if (diff.status != Deleted)
			{
				file.name=name;
				file.size=diff.size;
				file.type=diff.type;
				file.isExecutable=false;
				file.lastModified=diff.modified;

				nStatus=Add;

				bModified=true;
			}
		}
	}

	if (nStatus != -1 && !immediate)
	{
		int nRow=m_pModel->rowCount(pindex);
		m_pModel->insertRow(nRow, pindex);

		QModelIndex index=m_pModel->index(nRow, QTreeViewUpdateFile::NameTitle, pindex);
		m_pModel->setData(index, name, Qt::DisplayRole);
		m_pModel->setData(index, QTreeViewUpdateFile::FileRole, QTreeViewUpdateFile::ItemRole);

		DiffFile df;
		if (m_filesRequest.contains(name))
		{
			df=m_filesRequest[name];
			df.status=nStatus;
		}
		else
		{
			df.path=file.name;
			df.size=file.size;
			df.type=file.type;
			df.modified=file.lastModified;
			df.status=nStatus;
			df.version=-1;
		}

		m_pModel->setData(index, qVariantFromValue(df), QTreeViewUpdateFile::DiffFileRole);
		m_pModel->setData(index, qVariantFromValue(file), QTreeViewUpdateFile::LocalFileRole);
		m_pModel->setData(index, bModified? Qt::Checked:Qt::Unchecked, QTreeViewUpdateFile::CheckedRole);

		index=m_pModel->index(nRow, QTreeViewUpdateFile::SizeTitle, pindex);
		m_pModel->setData(index, m_bLocalLastModified? info.size():df.size, Qt::DisplayRole);

		index=m_pModel->index(nRow, QTreeViewUpdateFile::TypeTitle, pindex);
		m_pModel->setData(index, fileType(file), Qt::DisplayRole);

		index=m_pModel->index(nRow, QTreeViewUpdateFile::UpdatedTitle, pindex);
		m_pModel->setData(index, m_bLocalLastModified? info.lastModified():df.modified, Qt::DisplayRole);

		index=m_pModel->index(nRow, QTreeViewUpdateFile::StatusTitle, pindex);
		m_pModel->setData(index, nStatus, Qt::DisplayRole);
	}

	m_filesRequest.remove(name);

	return bModified;
}

QReadUpdateRecordsFromXmlFile::QReadUpdateRecordsFromXmlFile(const QString& filename, const QString& path, const QString& hostname, const QString& address)
:QAbstractReadXmlFile(filename)
,m_strPath(path)
,m_strHostName(hostname)
,m_strAddress(address)
{

}

QReadUpdateRecordsFromXmlFile::~QReadUpdateRecordsFromXmlFile()
{

}

void QReadUpdateRecordsFromXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "workspace", readWorkspaceNode);
}

void QReadUpdateRecordsFromXmlFile::readWorkspaceNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("path"))
	{
		QString strPath=attributes.value("path").toString();
		if (strPath == m_strPath)
		{
			readNextNode(reader, "folder", readFolder);
			return;
		}
	}

	readItemFinished(reader);
}

void QReadUpdateRecordsFromXmlFile::readFolder(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("hostname") && attributes.hasAttribute("ip"))
	{
		QString strHostName=attributes.value("hostname").toString();
		QString strAddress=attributes.value("ip").toString();
		if (m_strHostName == strHostName && m_strAddress == strAddress)
		{
			readNextNode(reader, "file", readFile);
			return;
		}
	}

	readItemFinished(reader);
}

void QReadUpdateRecordsFromXmlFile::readFile(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "file");

	QXmlStreamAttributes attributes=reader.attributes();
	DiffFile file;

	if (attributes.hasAttribute("path") && attributes.hasAttribute("size") && attributes.hasAttribute("type")
		&& attributes.hasAttribute("modified") && attributes.hasAttribute("status") && attributes.hasAttribute("version"))
	{
		file.path=attributes.value("path").toString();
		if (!file.path.isEmpty())
		{
			file.size=attributes.value("size").toString().toULongLong();
			file.type=attributes.value("type").toString().toInt();
			file.modified=QDateTime::fromString(attributes.value("modified").toString(), FILE_MODIFIED_DATETIME_FORMAT);
			file.status=attributes.value("status").toString().toInt();
			file.version=attributes.value("version").toString().toInt();
			if (file.status >= 0 && file.status < UpdateFlagEnd)
			{
				QString strFileName=parseFileNameFromRevision(file.path);
				m_files.insert(strFileName, file);
			}
		}
	}

	readItemFinished(reader);
}

QWriteUpdateRecordsToXmlFile::QWriteUpdateRecordsToXmlFile(const QString& filename, const QString& path, 
														   const QString& hostname, const QString& address, const QVector<DiffFile>& files)
:QAbstractWriteXmlFile(filename)
,m_strPath(path)
,m_strHostName(hostname)
,m_strAddress(address)
,m_files(files)
{
	write(false);
}

QWriteUpdateRecordsToXmlFile::~QWriteUpdateRecordsToXmlFile()
{

}

bool QWriteUpdateRecordsToXmlFile::check(const QDomElement& node) const
{
	if (node.hasAttribute("path") && node.attribute("path") == m_strPath)
	{
		return true;
	}

	return false;
}

void QWriteUpdateRecordsToXmlFile::save(QDomDocument& doc, QDomElement& parent)
{
	if (!parent.hasAttribute("path"))
	{
		parent.setAttribute("path", m_strPath);
	}

	QDomElement child = parent.firstChildElement("folder");
	while (!child.isNull())
	{
		QDomElement node=child;

		child = child.nextSiblingElement("folder");

		if (node.hasAttribute("hostname") && node.hasAttribute("ip"))
		{
			QString strHostName=node.attribute("hostname");
			QString strAddress=node.attribute("ip");

			if (m_strHostName==strHostName && m_strAddress==strAddress)
			{
				parent.removeChild(node);
			}
		}
	}

	child=doc.createElement("folder");
	child.setAttribute("hostname", m_strHostName);
	child.setAttribute("ip", m_strAddress);
	parent.appendChild(child);

	for (int i=0; i<m_files.size(); i++)
	{
		const DiffFile& file=m_files[i];

		QDomElement node=doc.createElement("file");

		node.setAttribute("path", file.path);
		node.setAttribute("type", file.type);
		node.setAttribute("size", file.size);
		node.setAttribute("modified", file.modified.toString(FILE_MODIFIED_DATETIME_FORMAT));
		node.setAttribute("status", file.status);
		node.setAttribute("version", file.version);

		child.appendChild(node);
	}
}

QUpdateFileDelegate::QUpdateFileDelegate(QObject *parent)
: QStyledItemDelegate(parent)
{
}

QUpdateFileDelegate::~QUpdateFileDelegate()
{

}

void QUpdateFileDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QAbstractItemModel* pModel=index.model();

	drawItem(painter, option, pModel, index);
}

void QUpdateFileDelegate::drawItem(QPainter* painter, const QStyleOptionViewItem& option, const QAbstractItemModel* model, const QModelIndex& index) const
{
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	QColor color(Qt::black);

	QModelIndex cindex=model->index(index.row(), QTreeViewUpdateFile::StatusTitle, index.parent());
	int nStatus=model->data(cindex, Qt::DisplayRole).toInt();
	switch (nStatus)
	{
	case Modify:
		{
			color=QColor(60, 90, 200);
		}
		break;
	case Add:
		{
			color=QColor(128, 60, 180);
		}
		break;
	case Deleted:
		{
			color=QColor(212, 212, 212);
		}
		break;
	case Delete:
		{
			color=QColor(100, 0, 0);
		}
		break;
	case Latest:
		{
			color=QColor(128, 128, 0);
		}
		break;
	default:
		break;
	}

	opt.palette.setColor(QPalette::Text, color);

	cindex=model->index(index.row(), QTreeViewUpdateFile::NameTitle, index.parent());
	int nRole=model->data(cindex, QTreeViewUpdateFile::ItemRole).toInt();
	if (nRole == QTreeViewUpdateFile::FileRole)
	{
		FileInfo file=model->data(cindex, QTreeViewUpdateFile::LocalFileRole).value<FileInfo>();

		if (index.column() == QTreeViewUpdateFile::NameTitle)
		{
			painter->save();

			int nChecked=model->data(cindex, QTreeViewUpdateFile::CheckedRole).toInt();

			opt.features|=QStyleOptionViewItemV2::HasCheckIndicator;
			opt.checkState=(Qt::CheckState)nChecked;

			opt.features|=QStyleOptionViewItemV2::HasDecoration;
			opt.icon=fileIcon(file);

			style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

			painter->restore();
			return;
		}
		else if (index.column() == QTreeViewUpdateFile::SizeTitle)
		{
			opt.text=(file.type == Dir || file.type == HideDir)? "":fileSize(file.size);
			opt.displayAlignment=Qt::AlignVCenter|Qt::AlignRight;

			painter->save();

			style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

			painter->restore();

			return;
		}
		else if (index.column() == QTreeViewUpdateFile::TypeTitle)
		{
			opt.text=fileType(file);
			opt.displayAlignment=Qt::AlignVCenter|Qt::AlignRight;

			painter->save();
			style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
			painter->restore();

			return;
		}
		else if (index.column() == QTreeViewUpdateFile::StatusTitle)
		{
			switch (nStatus)
			{
			case Unmodified:
				opt.text=tr("unmodified");
				break;
			case Modify:
				opt.text=tr("modify");
				break;
			case Add:
				opt.text=tr("add");
				break;
			case Deleted:
				opt.text=tr("deleted");
				break;
			case Delete:
				opt.text=tr("delete");
				break;
			case NonVersioned:
				opt.text=tr("non-versioned");
				break;
			case Latest:
				opt.text=tr("latest");
				break;
			default:
				opt.text="";
				break;
			}

			painter->save();

			style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

			painter->restore();

			return;
		}
	}
	else if (nRole == QTreeViewUpdateFile::GroupRole)
	{
		if (index.column() == QTreeViewUpdateFile::NameTitle)
		{
			painter->save();

			int nChecked=model->data(cindex, QTreeViewUpdateFile::CheckedRole).toInt();

			opt.features|=QStyleOptionViewItemV2::HasCheckIndicator;
			opt.checkState=(Qt::CheckState)nChecked;

			style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

			painter->restore();
			return;
		}
		else if (index.column() == QTreeViewUpdateFile::StatusTitle)
		{
			opt.text="";

			painter->save();
			style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
			painter->restore();

			return;
		}
	}

	QStyledItemDelegate::paint(painter, opt, index);
}

QSize QUpdateFileDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	int nHeight=style->pixelMetric(QStyle::PM_ListViewIconSize, 0, widget)+2;

	QSize size=QStyledItemDelegate::sizeHint(option, index);
	if (size.height() < nHeight)
	{
		size.setHeight(nHeight);
	}

	return size;
}
