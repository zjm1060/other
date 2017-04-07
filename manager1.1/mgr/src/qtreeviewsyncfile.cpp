#include "qtreeviewsyncfile.h"

#include <QMouseEvent>

#include "qimessage.h"
#include "qglobalinfo.h"

QTreeViewSyncFile::QTreeViewSyncFile(QWidget *parent)
	: QTreeView(parent)
	,m_pModel(NULL)
	,m_bCheckable(true)
{
	setRootIsDecorated(false);
	setAlternatingRowColors(true);
	setSortingEnabled(true);
	setSelectionMode(QAbstractItemView::ExtendedSelection);

	m_pDelegate=new QSyncFileDelegate(this);
	setItemDelegate(m_pDelegate);

	setItemsExpandable(true);

	sortByColumn(0, Qt::AscendingOrder);
}

QTreeViewSyncFile::~QTreeViewSyncFile()
{
	setModel(NULL);
	SAFE_DELETE_CLASS(m_pModel);
}

void QTreeViewSyncFile::setDiffFile(const QList<DiffFile>& files)
{
	QTreeViewSyncFileRunnable* pRunnable=new QTreeViewSyncFileRunnable(files, m_bCheckable);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QStandardItemModel*)), this, SLOT(finished(QStandardItemModel*)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

void QTreeViewSyncFile::setItemTransferStatus(int row, int status)
{
	if (row < 0 || row >= m_pModel->rowCount() )
	{
		return;
	}

	Q_ASSERT(status < StatusEnd);

	m_pModel->setData(m_pModel->index(row, NameTitle), status, UserTransferStatus);

	if (status == Transfering)
	{
		m_pModel->setData(m_pModel->index(row, ProcessTitle), 0);
	}
}

int QTreeViewSyncFile::itemTransferStatus(int row) const
{
	return m_pModel->data(m_pModel->index(row, NameTitle), UserTransferStatus).toInt();
}

int QTreeViewSyncFile::rowCount() const
{
	return m_pModel->rowCount();
}

void QTreeViewSyncFile::addTransferredSize(int row, quint64 size)
{
	quint64 nSize=m_pModel->data(m_pModel->index(row, ProcessTitle)).toULongLong();
	m_pModel->setData(m_pModel->index(row, ProcessTitle), nSize+size);
}

void QTreeViewSyncFile::setItemsCheckable(bool checkable)
{
	m_bCheckable=checkable;

	setSortingEnabled(false);

	int nRowCount=rowCount();
	for (int i=0; i<nRowCount; i++)
	{
		m_pModel->setData(m_pModel->index(i, NameTitle), m_bCheckable, UserCheckable);
	}

	setSortingEnabled(true);
}

bool QTreeViewSyncFile::isItemChecked(int row) const
{
	Q_ASSERT(row < m_pModel->rowCount());

	return m_pModel->data(m_pModel->index(row, NameTitle), UserChecked).toBool();
}

void QTreeViewSyncFile::setItemCheckStatus(int row, bool check)
{
	if (row < m_pModel->rowCount())
	{
		m_pModel->setData(m_pModel->index(row, NameTitle), check, UserChecked);
	}
}

int QTreeViewSyncFile::fetchTask(DiffFile& file)
{
	int row=-1;

	int nRows=m_pModel->rowCount();
	for (int i=0; i<nRows; i++)
	{
		if (itemTransferStatus(i) == Waiting)
		{
			row=i;

			file=m_pModel->data(m_pModel->index(row, QTreeViewSyncFile::NameTitle), UserData).value<DiffFile>();

			setItemTransferStatus(i, Transfering);

			break;
		}
	}

	return row;
}

DiffFile QTreeViewSyncFile::fetchItem(const QModelIndex& index)
{
	return m_pModel->data(m_pModel->index(index.row(), QTreeViewSyncFile::NameTitle), UserData).value<DiffFile>();
}

void QTreeViewSyncFile::finished(QStandardItemModel* model)
{
	if (model != NULL)
	{
		int nRowCount=model->rowCount();
		for (int row=0; row<nRowCount; row++)
		{
			QModelIndex index=model->index(row, NameTitle);

			FileInfo file=qvariant_cast<FileInfo>(model->data(index, UserData));
			model->setData(index, fileIcon(file), Qt::DecorationRole);
		}
	}

	setModel(model);

	setColumnWidth(NameTitle, 200);
	setColumnWidth(SizeTitle, 100);
	setColumnWidth(TypeTitle, 80);
	setColumnWidth(ModifiedTitle, 140);
	setColumnWidth(StatusTitle, 80);
	setColumnWidth(ProcessTitle, 150);

	SAFE_DELETE_CLASS(m_pModel);
	m_pModel=model;

	emit refreshModelFinished();
}

void QTreeViewSyncFile::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (m_bCheckable)
		{
			QModelIndex index=indexAt(event->pos());
			if (index.column() == NameTitle)
			{
				QRect rect=visualRect(index);

				int nCheckWidth=style()->pixelMetric(QStyle::PM_ButtonIconSize, 0, this);

				rect=QRect(rect.left()+4, rect.top(), nCheckWidth, rect.height());
				if (rect.contains(event->pos()))
				{
					bool bChecked=m_pModel->data(index, UserChecked).toBool();
					m_pModel->setData(index, !bChecked, UserChecked);

					return;
				}
			}
		}
	}

	QTreeView::mousePressEvent(event);

	if (event->button() == Qt::RightButton)
	{
		emit mouseRightButtonPressed(event->pos());
	}
}

void QTreeViewSyncFile::keyPressEvent(QKeyEvent* event)
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

void QTreeViewSyncFile::clear()
{
	if (m_pModel != NULL)
	{
		m_pModel->setRowCount(0);
	}
}

QModelIndexList QTreeViewSyncFile::selectedRows()
{
 	QItemSelectionModel* pModel=selectionModel();
 	if (pModel == NULL)
 	{
 		return QModelIndexList();
 	}
 
 	return pModel->selectedRows();
}

QTreeViewSyncFileRunnable::QTreeViewSyncFileRunnable(const QList<DiffFile>& files, bool checkable, QObject* parent/* =0 */)
:QObject(parent)
,m_files(files)
,m_bCheckable(checkable)
{
}

QTreeViewSyncFileRunnable::~QTreeViewSyncFileRunnable()
{

}

void QTreeViewSyncFileRunnable::run()
{
	QStringList labels;
	labels<<tr("Name")<<tr("Size")<<tr("Type")<<tr("Last Modified")<<tr("Status")<<tr("Process");

	m_pModel=new QStandardItemModel(0, labels.size());
	m_pModel->setHorizontalHeaderLabels(labels);

	m_pModel->setRowCount(m_files.size());
	int nRow=0;

	QListIterator<DiffFile> it(m_files);
	while (it.hasNext())
	{
		const DiffFile& file=it.next();

		FileInfo info;
		info.name=file.path;
		info.type=file.type;
		info.isExecutable=false;

		QVariant var;
		var.setValue(file);

		bool bChecked=false;
		if (file.status != QTreeViewSyncFile::NonVersioned)
		{
			bChecked=true;
		}

		m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewSyncFile::NameTitle))->setEditable(false);
//		m_pModel->setData(m_pModel->index(nRow, QTreeViewSyncFile::NameTitle), fileIcon(info), Qt::DecorationRole);
		m_pModel->setData(m_pModel->index(nRow, QTreeViewSyncFile::NameTitle), file.path);
		m_pModel->setData(m_pModel->index(nRow, QTreeViewSyncFile::NameTitle), bChecked, QTreeViewSyncFile::UserChecked);
		m_pModel->setData(m_pModel->index(nRow, QTreeViewSyncFile::NameTitle), m_bCheckable, QTreeViewSyncFile::UserCheckable);
		m_pModel->setData(m_pModel->index(nRow, QTreeViewSyncFile::NameTitle), QTreeViewSyncFile::InvalidStatus, QTreeViewSyncFile::UserTransferStatus);
		m_pModel->setData(m_pModel->index(nRow, QTreeViewSyncFile::NameTitle), var, QTreeViewSyncFile::UserData);

		m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewSyncFile::SizeTitle))->setEditable(false);
		m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewSyncFile::SizeTitle))->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		m_pModel->setData(m_pModel->index(nRow, QTreeViewSyncFile::SizeTitle), file.size);

		m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewSyncFile::TypeTitle))->setEditable(false);
		m_pModel->setData(m_pModel->index(nRow, QTreeViewSyncFile::TypeTitle), fileType(info));

		m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewSyncFile::ModifiedTitle))->setEditable(false);
		m_pModel->setData(m_pModel->index(nRow, QTreeViewSyncFile::ModifiedTitle), file.modified);

		m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewSyncFile::StatusTitle))->setEditable(false);
		m_pModel->setData(m_pModel->index(nRow, QTreeViewSyncFile::StatusTitle), file.status);

		m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewSyncFile::ProcessTitle))->setEditable(false);
		m_pModel->setData(m_pModel->index(nRow, QTreeViewSyncFile::ProcessTitle), (quint64)0);

		nRow++;
	}

	emit finished(m_pModel);
}
