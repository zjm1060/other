#include "qtreeviewfilesystem.h"

#include <QMouseEvent>

#include "qmessagefiletransfer.h"

QTreeViewFileSystem::QTreeViewFileSystem(QWidget *parent)
	: QTreeView(parent),
	m_bEditable(false)
{
	QStringList labels;
	labels<<tr("Name")<<tr("Size")<<tr("Type")<<tr("Modified");

	m_pModel=new QStandardItemModel(0, labels.size(), this);
	m_pModel->setHorizontalHeaderLabels(labels);

	m_pSortModel=new QSortFilterProxyModel;
	m_pSortModel->setDynamicSortFilter(true);
	m_pSortModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	m_pSortModel->setSourceModel(m_pModel);

	setRootIsDecorated(false);
	setAlternatingRowColors(true);
	setModel(m_pSortModel);
	setSortingEnabled(true);
	setSelectionMode(QAbstractItemView::ExtendedSelection);

	setColumnWidth(0, 150);
	setColumnWidth(1, 50);
	setColumnWidth(2, 80);
	setColumnWidth(3, 120);

	sortByColumn(0, Qt::AscendingOrder);
}

QTreeViewFileSystem::~QTreeViewFileSystem()
{

}

void QTreeViewFileSystem::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (m_bEditable)
		{
			QModelIndexList list=selectedRows();
			if (list.size() == 1)
			{
				QModelIndex index=indexAt(event->pos());
				if (index == list[0])
				{
					edit(index);
				}
			}
		}
	}

	QTreeView::mousePressEvent(event);

	if (event->button() == Qt::RightButton)
	{
		emit mouseRightButtonPress(event->pos());
	}
}

void QTreeViewFileSystem::setEditable(bool editable)
{
	m_bEditable=editable;
}

int QTreeViewFileSystem::sortColumn()
{
	if (m_pSortModel != NULL)
	{
		m_pSortModel->sortColumn();
	}
	
	return 0;
}

void QTreeViewFileSystem::clear()
{
	if (m_pModel != NULL)
	{
		m_pModel->setRowCount(0);
	}
}

QModelIndexList QTreeViewFileSystem::selectedRows()
{
 	QItemSelectionModel* pModel=selectionModel();
 	if (pModel == NULL)
 	{
 		return QModelIndexList();
 	}
 
 	return pModel->selectedRows();
}

void QTreeViewFileSystem::insertFileInfo(const QFileInfoList& files, bool isDrive)
{
	if (m_pModel == NULL)
	{
		return;
	}

	foreach (QFileInfo file, files)
	{
		if (file.fileName() == ".")
		{
			continue;
		}

		appendFileInfo(file, isDrive);
	}
}

void QTreeViewFileSystem::insertFileInfo(const QVector<FileInfo>& files, bool isDrive)
{
	if (m_pModel == NULL)
	{
		return;
	}

	foreach (FileInfo file, files)
	{
		if (file.name == ".")
		{
			continue;
		}

		appendFileInfo(file, isDrive);
	}
}

QModelIndex QTreeViewFileSystem::appendFileInfo(const QFileInfo& file, bool isDrive)
{
	int nRow=m_pModel->rowCount();
	m_pModel->insertRow(nRow);

	if (isDrive || file.fileName() == "..")
	{
		m_pModel->itemFromIndex(m_pModel->index(nRow, 0))->setEditable(false);
	}
	else
	{
		m_pModel->itemFromIndex(m_pModel->index(nRow, 0))->setEditable(m_bEditable);
	}

	QString strName=isDrive? file.filePath():file.fileName();
	m_pModel->setData(m_pModel->index(nRow, 0), m_fileIconProvider.icon(file) , Qt::DecorationRole);
	m_pModel->setData(m_pModel->index(nRow, 0), strName);
	m_pModel->setData(m_pModel->index(nRow, 0), strName, Qt::UserRole);

	m_pModel->itemFromIndex(m_pModel->index(nRow, 1))->setEditable(false);
	m_pModel->itemFromIndex(m_pModel->index(nRow, 1))->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_pModel->setData(m_pModel->index(nRow, 1), fileSize(file.size()));
	m_pModel->setData(m_pModel->index(nRow, 1), file.size(), Qt::UserRole);

	m_pModel->itemFromIndex(m_pModel->index(nRow, 2))->setEditable(false);
	m_pModel->setData(m_pModel->index(nRow, 2), isDrive? tr("Drive"):fileType(file));
	m_pModel->setData(m_pModel->index(nRow, 2), isDrive? QMessageFileTransfer::Drive:entryInfoFileType(file), Qt::UserRole);

	m_pModel->itemFromIndex(m_pModel->index(nRow, 3))->setEditable(false);
	m_pModel->setData(m_pModel->index(nRow, 3), file.lastModified());

	return m_pModel->index(nRow, 0);
}

QModelIndex QTreeViewFileSystem::appendFileInfo(const FileInfo& file, bool isDrive)
{
	int nRow=m_pModel->rowCount();
	m_pModel->insertRow(nRow);

	if (isDrive || file.name == "..")
	{
		m_pModel->itemFromIndex(m_pModel->index(nRow, 0))->setEditable(false);
	}
	else
	{
		m_pModel->itemFromIndex(m_pModel->index(nRow, 0))->setEditable(m_bEditable);
	}
	m_pModel->setData(m_pModel->index(nRow, 0), fileIcon(file), Qt::DecorationRole);
	m_pModel->setData(m_pModel->index(nRow, 0), file.name);
	m_pModel->setData(m_pModel->index(nRow, 0), file.name, Qt::UserRole);

	m_pModel->itemFromIndex(m_pModel->index(nRow, 1))->setEditable(false);
	m_pModel->itemFromIndex(m_pModel->index(nRow, 1))->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_pModel->setData(m_pModel->index(nRow, 1), fileSize(file.size));
	m_pModel->setData(m_pModel->index(nRow, 1), file.size, Qt::UserRole);

	m_pModel->itemFromIndex(m_pModel->index(nRow, 2))->setEditable(false);
	m_pModel->setData(m_pModel->index(nRow, 2), fileType(file));
	m_pModel->setData(m_pModel->index(nRow, 2), file.type, Qt::UserRole);

	m_pModel->itemFromIndex(m_pModel->index(nRow, 3))->setEditable(false);
	m_pModel->setData(m_pModel->index(nRow, 3), file.lastModified);

	return m_pModel->index(nRow, 0);
}

void QTreeViewFileSystem::setFileName(const QModelIndex& index, const QString& name)
{
	if (!index.isValid())
	{
		return;
	}

	setFileName(index.row(), name);
}

void QTreeViewFileSystem::setFileName(int row, const QString& name)
{
	m_pModel->setData(m_pModel->index(row, 0), name, Qt::UserRole);
	m_pModel->setData(m_pModel->index(row, 0), name);
}

QString QTreeViewFileSystem::filename(const QModelIndex& index)
{
	QString strFileName;
	if (index.isValid())
	{
		strFileName=m_pSortModel->data(m_pSortModel->index(index.row(), 0)).toString();
	}

	return strFileName;
}

int QTreeViewFileSystem::type(const QModelIndex& index)
{
	int nType=-1;

	if (index.isValid())
	{
		nType=m_pSortModel->data(m_pSortModel->index(index.row(), 2), Qt::UserRole).toInt();
	}

	return nType;
}

QString QTreeViewFileSystem::typeDesc(const QModelIndex& index)
{
	QString strTypeDesc;

	if (index.isValid())
	{
		strTypeDesc=m_pSortModel->data(m_pSortModel->index(index.row(), 2)).toString();
	}

	return strTypeDesc;
}

quint64 QTreeViewFileSystem::size(const QModelIndex& index)
{
	quint64 nSize=0;

	if (index.isValid())
	{
		nSize=m_pSortModel->data(m_pSortModel->index(index.row(), 1), Qt::UserRole).toULongLong();
	}

	return nSize;
}

void QTreeViewFileSystem::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
	if (topLeft != bottomRight)
	{
		return;
	}

	QVariant variant=m_pSortModel->data(m_pSortModel->index(topLeft.row(), 0), Qt::UserRole);
	if (!variant.isValid())
	{
		return;
	}

	QString strOld=variant.toString();
	QString strNew=filename(topLeft);

	if (strOld != strNew)
	{
		emit dataChanged(m_pSortModel->mapToSource(topLeft), strOld, strNew);
	}
}

void QTreeViewFileSystem::setCurrentEdit(const QModelIndex& index)
{
	QItemSelectionModel* pModel=selectionModel();
	if (pModel != NULL)
	{
		pModel->select(m_pSortModel->mapFromSource(index), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

		QModelIndexList list=pModel->selectedRows();
		if (list.size() == 1)
		{
			edit(list[0]);
		}
	}
}
