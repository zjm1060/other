#include "qtreeviewassociationfiles.h"

#include <QMouseEvent>
#include <QApplication>

#include "qimessage.h"
#include "qutildef.h"
#include "qglobalinfo.h"

QTreeViewAssociationFiles::QTreeViewAssociationFiles(QWidget *parent)
	: QTreeView(parent)
	,m_pModel(NULL)
	,m_bExclusive(false)
{
	m_pSortModel=new QSortFilterProxyModel(this);
	m_pSortModel->setDynamicSortFilter(true);
	m_pSortModel->setSortCaseSensitivity(Qt::CaseInsensitive);

	setRootIsDecorated(false);
	setAlternatingRowColors(true);
	setModel(m_pSortModel);
	setSortingEnabled(true);
	setSelectionMode(QAbstractItemView::ExtendedSelection);

	m_pDelegate=new QTreeViewAssociationFilesDelegate(this);
	setItemDelegate(m_pDelegate);

	setItemsExpandable(true);

	QStringList labels;
	labels<<tr("Name")<<tr("Size")<<tr("Type")<<tr("Last Modified")<<tr("Path");

	m_pModel=new QStandardItemModel(0, labels.size());
	m_pModel->setHorizontalHeaderLabels(labels);

	connect(m_pModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));

	m_pSortModel->setSourceModel(m_pModel);

	setColumnWidth(NameTitle, 200);
	setColumnWidth(SizeTitle, 100);
	setColumnWidth(TypeTitle, 100);
	setColumnWidth(ModifiedTitle, 120);
	setColumnWidth(PathTitle, 250);

	sortByColumn(0, Qt::AscendingOrder);
}

QTreeViewAssociationFiles::~QTreeViewAssociationFiles()
{
	setModel(NULL);
	SAFE_DELETE_CLASS(m_pModel);
	SAFE_DELETE_CLASS(m_pSortModel);
	SAFE_DELETE_CLASS(m_pDelegate);
}

void QTreeViewAssociationFiles::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		emit mouseRightButtonPress(event->pos());
	}

	QTreeView::mousePressEvent(event);
}

void QTreeViewAssociationFiles::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Delete:
		{
			QModelIndexList list=selectedRows();
			if (list.size() > 0)
			{
				emit deleteKeyPress();

				event->ignore();

				return;
			}
		}
		break;;
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

int QTreeViewAssociationFiles::sortColumn()
{
	if (m_pSortModel != NULL)
	{
		m_pSortModel->sortColumn();
	}
	
	return 0;
}

void QTreeViewAssociationFiles::clear()
{
	if (m_pModel != NULL)
	{
		m_pModel->setRowCount(0);
	}
}

QModelIndexList QTreeViewAssociationFiles::selectedRows()
{
 	QItemSelectionModel* pModel=selectionModel();
 	if (pModel == NULL)
 	{
 		return QModelIndexList();
 	}
 
	QModelIndexList indexes=pModel->selectedRows();
	QModelIndexList items;
	foreach (QModelIndex index, indexes)
	{
		items.append(m_pSortModel->mapToSource(index));
	}

 	return items;
}

void QTreeViewAssociationFiles::removeRows(const QModelIndexList& items)
{
	QListIterator<QModelIndex> it(items);
	it.toBack();
	while (it.hasPrevious())
	{
		const QModelIndex& index=it.previous();
		m_pModel->removeRow(index.row(), index.parent());
	}

	QVector<SyncMonitorFile> files=fileInfos();

	emit changed(files);
}

void QTreeViewAssociationFiles::insertFiles(const QVector<SyncMonitorFile>& files)
{
	disconnect(m_pModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));
	m_pModel->setRowCount(0);

	for (int i=0; i<files.size(); i++)
	{
		appendFileInfo(files[i]);
	}

	connect(m_pModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));
}

void QTreeViewAssociationFiles::itemChanged(QStandardItem* item)
{
	if (item == NULL || m_pModel ==NULL)
	{
		return;
	}

	if (m_bExclusive)
	{
		if (item->checkState() == Qt::Checked)
		{
			QModelIndex index=item->index();
			int nRowCount=m_pModel->rowCount();
			for (int nRow=0; nRow<nRowCount; nRow++)
			{
				if (nRow == index.row())
				{
					continue;
				}

				QModelIndex currentIndex=m_pModel->index(nRow, QTreeViewAssociationFiles::NameTitle);
				if (m_pModel->itemFromIndex(currentIndex)->checkState() == Qt::Checked)
				{
					m_pModel->itemFromIndex(currentIndex)->setCheckState(Qt::Unchecked);
				}
			}
		}
	}

	QVector<SyncMonitorFile> files=fileInfos();

	emit changed(files);
}

QModelIndex QTreeViewAssociationFiles::appendFileInfo(const SyncMonitorFile& file)
{
	if (file.file.name.isEmpty() || file.file.name == "." || file.file.name == "..")
	{
		return QModelIndex();
	}

	int nRowCount=m_pModel->rowCount();
	int nRow=0;
	for (; nRow<nRowCount; nRow++)
	{
		QString strName=m_pModel->data(m_pModel->index(nRow, QTreeViewAssociationFiles::NameTitle)).toString();
		QString strPath=m_pModel->data(m_pModel->index(nRow, QTreeViewAssociationFiles::PathTitle)).toString();
		if (strName == file.file.name && strPath == file.path)
		{
			break;
		}
	}

	if (nRow >= nRowCount)
	{
		m_pModel->insertRow(nRow);
	}

	QModelIndex index=m_pModel->index(nRow, QTreeViewAssociationFiles::NameTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->itemFromIndex(index)->setCheckable(true);
	m_pModel->itemFromIndex(index)->setCheckState(file.enabled ? Qt::Checked:Qt::Unchecked);
	m_pModel->setData(index, 0, Qt::DecorationRole);
	m_pModel->setData(index, file.file.name);
	m_pModel->setData(index, qVariantFromValue(file), QTreeViewAssociationFiles::FileDataRole);

	index=m_pModel->index(nRow, QTreeViewAssociationFiles::SizeTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->itemFromIndex(index)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_pModel->setData(index, fileSizeForNumber(file.file));

	index=m_pModel->index(nRow, QTreeViewAssociationFiles::TypeTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->setData(index, fileType(file.file));

	m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewAssociationFiles::ModifiedTitle))->setEditable(false);
	m_pModel->setData(m_pModel->index(nRow, QTreeViewAssociationFiles::ModifiedTitle), file.file.lastModified);

	m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewAssociationFiles::PathTitle))->setEditable(false);
	m_pModel->setData(m_pModel->index(nRow, QTreeViewAssociationFiles::PathTitle), file.path);

	if (file.enabled && m_bExclusive)
	{
		for (int i=0; i<nRow; i++)
		{
			index=m_pModel->index(i, QTreeViewAssociationFiles::NameTitle);
			if (m_pModel->itemFromIndex(index)->checkState() == Qt::Checked)
			{
				m_pModel->itemFromIndex(index)->setCheckState(Qt::Unchecked);
			}
		}
	}

	return m_pModel->index(nRow, QTreeViewAssociationFiles::NameTitle);
}

SyncMonitorFile QTreeViewAssociationFiles::fileInfo(const QModelIndex& index) const
{
	SyncMonitorFile file;

	if (index.isValid())
	{
		file=qvariant_cast<SyncMonitorFile>(m_pSortModel->data(m_pSortModel->index(index.row(), NameTitle), QTreeViewAssociationFiles::FileDataRole));
	}

	return file;
}

QVector<SyncMonitorFile> QTreeViewAssociationFiles::fileInfos() const
{
	QVector<SyncMonitorFile> files;
	if (m_pModel != NULL)
	{
		int nRowCount=m_pModel->rowCount();
		SyncMonitorFile file;
		for (int nRow=0; nRow<nRowCount; nRow++)
		{
			QModelIndex index=m_pModel->index(nRow, NameTitle);
			file=qvariant_cast<SyncMonitorFile>(m_pModel->data(index, QTreeViewAssociationFiles::FileDataRole));
			file.enabled=(m_pModel->itemFromIndex(index)->checkState() == Qt::Checked);
			
			files.append(file);
		}
	}

	return files;
}

void QTreeViewAssociationFiles::setExclusive(bool exclusive)
{
	m_bExclusive=exclusive;
}

QTreeViewAssociationFilesDelegate::QTreeViewAssociationFilesDelegate(QObject *parent)
: QStyledItemDelegate(parent)
{
}

QTreeViewAssociationFilesDelegate::~QTreeViewAssociationFilesDelegate()
{

}

void QTreeViewAssociationFilesDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QAbstractItemModel* pModel=index.model();
	if (pModel != NULL)
	{
		drawItem(painter, option, ((QSortFilterProxyModel*)pModel)->sourceModel(), ((QSortFilterProxyModel*)pModel)->mapToSource(index));
	}
}

void QTreeViewAssociationFilesDelegate::drawItem(QPainter* painter, const QStyleOptionViewItem& option, const QAbstractItemModel* model, const QModelIndex& index) const
{
	if (model == NULL)
	{
		return;
	}

	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	QModelIndex cindex=model->index(index.row(), QTreeViewAssociationFiles::NameTitle, index.parent());
	SyncMonitorFile file=model->data(cindex, QTreeViewAssociationFiles::FileDataRole).value<SyncMonitorFile>();

	if (index.column() == QTreeViewAssociationFiles::NameTitle)
	{
		painter->save();

		opt.icon=fileIcon(file.file);

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		painter->restore();
		return;
	}
	else if (index.column() == QTreeViewAssociationFiles::SizeTitle)
	{
		opt.text=(file.file.type == Dir || file.file.type == HideDir)? "":fileSize(file.file.size);
		opt.displayAlignment=Qt::AlignVCenter|Qt::AlignRight;

		painter->save();

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		painter->restore();

		return;
	}
	else if (index.column() == QTreeViewAssociationFiles::TypeTitle)
	{
		opt.text=fileType(file.file);
		opt.displayAlignment=Qt::AlignVCenter|Qt::AlignRight;

		painter->save();
		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
		painter->restore();

		return;
	}

	QStyledItemDelegate::paint(painter, opt, index);
}

QSize QTreeViewAssociationFilesDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
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
