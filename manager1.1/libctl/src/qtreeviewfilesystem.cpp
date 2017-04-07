#include "qtreeviewfilesystem.h"

#include <QMouseEvent>

#include "qimessage.h"
#include "qutildef.h"
#include "qglobalinfo.h"
#include "common.h"

QSortFilterFileSystemModel::QSortFilterFileSystemModel(QObject* parent/* =0 */)
:QSortFilterProxyModel(parent)
{

}

QSortFilterFileSystemModel::~QSortFilterFileSystemModel()
{

}

bool QSortFilterFileSystemModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
	QAbstractItemModel*	pModel=sourceModel();

	Q_ASSERT_NO_NULL(pModel);

	QString strName=sourceModel()->data(sourceModel()->index(left.row(), QTreeViewFileSystem::NameTitle), Qt::DisplayRole).toString();
	if (strName == "..")
	{
		return sortOrder()==Qt::AscendingOrder? true:false;
	}

	strName=sourceModel()->data(sourceModel()->index(right.row(), QTreeViewFileSystem::NameTitle), Qt::DisplayRole).toString();
	if (strName == "..")
	{
		return sortOrder()==Qt::AscendingOrder? false:true;
	}

	QVariant l = pModel->data(pModel->index(left.row(), QTreeViewFileSystem::NameTitle), Qt::UserRole);
	QVariant r = pModel->data(pModel->index(right.row(), QTreeViewFileSystem::NameTitle), Qt::UserRole);

	if (l.isValid() && r.isValid())
	{
		FileInfo leftFile=qvariant_cast<FileInfo>(l);
		FileInfo rightFile=qvariant_cast<FileInfo>(r);
		int nLeftType=leftFile.type;
		int nRightType=rightFile.type;

		if ((nLeftType != nRightType) && (((nLeftType&(Dir|HideDir)) == 0) || ((nRightType&(Dir|HideDir)) == 0)))
		{
			switch (nLeftType)
			{
			case Drive:
				return sortOrder()==Qt::AscendingOrder? true:false;
			case CDDVDDrive:
				{
					if (nRightType == Drive)
					{
						return sortOrder()==Qt::AscendingOrder? false:true;
					}
					else
					{
						return sortOrder()==Qt::AscendingOrder? true:false;
					}
				}
				break;
			case Dir:
			case HideDir:
			case SymLinkDir:
			case SymLinkHideDir:
				{
					if (nRightType == Drive || nRightType == CDDVDDrive)
					{
						return sortOrder()==Qt::AscendingOrder? false:true;
					}
					else
					{
						return sortOrder()==Qt::AscendingOrder? true:false;
					}
				}
				break;
			case File:
			case HideFile:
			case SymLinkFile:
			case SymLinkHideFile:
				{
					return sortOrder()==Qt::AscendingOrder? false:true;
				}
				break;
			default:
				break;
			}
		}
	}

	return QSortFilterProxyModel::lessThan(left, right);
}

QTreeViewFileSystem::QTreeViewFileSystem(QWidget *parent)
	: QTreeView(parent)
	,m_bEditable(false)
	,m_pModel(NULL)
{
	m_pSortModel=new QSortFilterFileSystemModel(this);
	m_pSortModel->setDynamicSortFilter(true);
	m_pSortModel->setSortCaseSensitivity(Qt::CaseInsensitive);
//	m_pSortModel->setSourceModel(m_pModel);

	m_pIconDelegate=new QFileIconDelegate(this);
	m_pNumberDelegate=new QNumberDelegate(this);

	setRootIsDecorated(false);
	setAlternatingRowColors(true);
	setModel(m_pSortModel);
	setSortingEnabled(true);
	setSelectionMode(QAbstractItemView::ExtendedSelection);

	setItemDelegateForColumn(NameTitle, m_pIconDelegate);
	setItemDelegateForColumn(SizeTitle, m_pNumberDelegate);

	setItemsExpandable(true);

	sortByColumn(0, Qt::AscendingOrder);
}

QTreeViewFileSystem::~QTreeViewFileSystem()
{
	setModel(NULL);
	SAFE_DELETE_CLASS(m_pModel);
	SAFE_DELETE_CLASS(m_pSortModel);
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

void QTreeViewFileSystem::keyPressEvent(QKeyEvent* event)
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
	case Qt::Key_Backspace:
		{
			if (state() != EditingState || hasFocus())
			{
				emit backspaceKeyPress();

				event->ignore();

				return;
			}
		}
		break;
	case Qt::Key_Return:
	case Qt::Key_Enter:
		{
			QModelIndexList list=selectedRows();
			if (list.size() == 1)
			{
				if (state() != EditingState || hasFocus())
				{
					if (currentIndex().isValid())
					{
						emit doubleClicked(currentIndex());
					}
					event->ignore();

					return;
				}
			}
		}
		break;
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

void QTreeViewFileSystem::insertFileInfoFinished(QStandardItemModel* model)
{
// 	if (model != NULL)
// 	{
// 		int nRow=model->rowCount();
// 		for (int row=0; row<nRow; row++)
// 		{
// 			QModelIndex index=model->index(row, QTreeViewFileSystem::NameTitle);
// 			FileInfo file=qvariant_cast<FileInfo>(model->data(index, Qt::UserRole));
// 
// 			model->setData(index, fileIcon(file), Qt::DecorationRole);
// 		}
// 	}

	if (m_pSortModel == NULL)
	{
		return;
	}

	setSourceModel(model);

	SAFE_DELETE_CLASS(m_pModel);

	m_pModel=model;

	update();
}

void QTreeViewFileSystem::setSourceModel(QStandardItemModel* model)
{
	m_pSortModel->setSourceModel(model);

	setColumnWidth(NameTitle, 200);
	setColumnWidth(SizeTitle, 100);
	setColumnWidth(TypeTitle, 100);
	setColumnWidth(ModifiedTitle, 120);
}

void QTreeViewFileSystem::insertFileInfo(const QFileInfoList& files, bool isDrive)
{
	QTreeViewFileSystemRunnable* pRunnable=new QTreeViewFileSystemRunnable(files, m_bEditable, isDrive);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QStandardItemModel*)), this, SLOT(insertFileInfoFinished(QStandardItemModel*)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

void QTreeViewFileSystem::insertFileInfo(const QVector<FileInfo>& files, bool isDrive)
{
	QTreeViewFileSystemRunnable* pRunnable=new QTreeViewFileSystemRunnable(files, m_bEditable, isDrive);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QStandardItemModel*)), this, SLOT(insertFileInfoFinished(QStandardItemModel*)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

QModelIndex QTreeViewFileSystem::appendFileInfo(const QFileInfo& file, bool isDrive)
{
	return appendFileInfo(entryInfo(file, isDrive), isDrive);
}

QModelIndex QTreeViewFileSystem::appendFileInfo(const FileInfo& file, bool isDrive)
{
	if (m_pSortModel != NULL)
	{
		m_pSortModel->setSourceModel(NULL);
	}

	int nRow=m_pModel->rowCount();
	m_pModel->insertRow(nRow);

	QModelIndex index=m_pModel->index(nRow, QTreeViewFileSystem::NameTitle);

	if (isDrive || file.name == "..")
	{
		m_pModel->itemFromIndex(index)->setEditable(false);
	}
	else
	{
		m_pModel->itemFromIndex(index)->setEditable(m_bEditable);
	}
	m_pModel->setData(index, fileIcon(file), Qt::DecorationRole);
	m_pModel->setData(index, file.name);
	m_pModel->setData(index, qVariantFromValue(file), Qt::UserRole);

	index=m_pModel->index(nRow, QTreeViewFileSystem::SizeTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->itemFromIndex(index)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_pModel->setData(index, fileSizeForNumber(file));

	index=m_pModel->index(nRow, QTreeViewFileSystem::TypeTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->setData(index, fileType(file));

	index=m_pModel->index(nRow, QTreeViewFileSystem::ModifiedTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->setData(index, file.lastModified);

	setSourceModel(m_pModel);

	return m_pModel->index(nRow, QTreeViewFileSystem::NameTitle);
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
	if (row<0 || row>=m_pModel->rowCount())
	{
		return;
	}

	QModelIndex index=m_pModel->index(row, NameTitle);

	m_pModel->setData(index, name);

	FileInfo file=qvariant_cast<FileInfo>(m_pModel->data(index, Qt::UserRole));
	file.name=name;

	m_pModel->setData(index, qVariantFromValue(file), Qt::UserRole);
}

QString QTreeViewFileSystem::filename(const QModelIndex& index)
{
	QString strFileName;
	if (index.isValid())
	{
		strFileName=m_pSortModel->data(m_pSortModel->index(index.row(), NameTitle)).toString();
	}

	return strFileName;
}

int QTreeViewFileSystem::type(const QModelIndex& index)
{
	int nType=-1;

	if (index.isValid())
	{
		FileInfo file=qvariant_cast<FileInfo>(m_pSortModel->data(m_pSortModel->index(index.row(), NameTitle), Qt::UserRole));

		nType=file.type;
	}

	return nType;
}

QString QTreeViewFileSystem::typeDesc(const QModelIndex& index)
{
	QString strTypeDesc;

	if (index.isValid())
	{
		strTypeDesc=m_pSortModel->data(m_pSortModel->index(index.row(), TypeTitle)).toString();
	}

	return strTypeDesc;
}

quint64 QTreeViewFileSystem::size(const QModelIndex& index)
{
	quint64 nSize=0;

	if (index.isValid())
	{
		FileInfo file=qvariant_cast<FileInfo>(m_pSortModel->data(m_pSortModel->index(index.row(), NameTitle), Qt::UserRole));

		nSize=file.size;
	}

	return nSize;
}

FileInfo QTreeViewFileSystem::fileInfo(const QModelIndex& index)
{
	FileInfo file;


	if (index.isValid())
	{
		file=qvariant_cast<FileInfo>(m_pSortModel->data(m_pSortModel->index(index.row(), NameTitle), Qt::UserRole));
	}

	return file;
}

void QTreeViewFileSystem::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
	if (topLeft != bottomRight)
	{
		return;
	}

	QVariant variant=m_pSortModel->data(m_pSortModel->index(topLeft.row(), NameTitle), Qt::UserRole);
	if (!variant.isValid())
	{
		return;
	}

	FileInfo file=qvariant_cast<FileInfo>(variant);

	QString strOld=file.name;
	QString strNew=filename(topLeft);

	if (strOld != strNew)
	{
		emit dataChanged(m_pSortModel->mapToSource(topLeft), strOld, strNew);
	}
}

void QTreeViewFileSystem::setCurrentEdit(const QModelIndex& index)
{
	if (!m_bEditable)
	{
		return;
	}

	QItemSelectionModel* pModel=selectionModel();
	if (pModel != NULL)
	{
		QModelIndex tindex=m_pSortModel->mapFromSource(index);
		pModel->select(tindex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

		edit(tindex);
	}
}

QTreeViewFileSystemRunnable::QTreeViewFileSystemRunnable(const QVector<FileInfo>& files, bool isEditable, bool isDrive/* =false */,QObject* parent/* =0 */)
:QObject(parent)
,m_files_1(files)
,m_bEditable(isEditable)
,m_isDrive(isDrive)
{
}

QTreeViewFileSystemRunnable::QTreeViewFileSystemRunnable(const QFileInfoList& files, bool isEditable, bool isDrive/* =false */,QObject* parent/* =0 */)
:QObject(parent)
,m_files_2(files)
,m_bEditable(isEditable)
,m_isDrive(isDrive)
{
}

QTreeViewFileSystemRunnable::~QTreeViewFileSystemRunnable()
{

}

void QTreeViewFileSystemRunnable::run()
{
	QStringList labels;
	labels<<tr("Name")<<tr("Size")<<tr("Type")<<tr("Last Modified");

	m_pModel=new QStandardItemModel(0, labels.size());
	m_pModel->setHorizontalHeaderLabels(labels);

	if (!m_files_2.isEmpty())
	{
		entryInfoList(m_files_2, m_files_1, m_isDrive);
	}

	if (!m_files_1.isEmpty())
	{	
		int nRow=0;
		for (int i=0; i<m_files_1.size(); i++)
		{
			const FileInfo& file=m_files_1[i];

			if (file.name == ".")
			{
				continue;
			}

			if (isRestrictFile(file.name))
			{
				continue;
			}

			nRow=m_pModel->rowCount();
			m_pModel->insertRow(nRow);

			if (m_isDrive || file.name == "..")
			{
				m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewFileSystem::NameTitle))->setEditable(false);
			}
			else
			{
				m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewFileSystem::NameTitle))->setEditable(m_bEditable);
			}
//			m_pModel->setData(m_pModel->index(nRow, QTreeViewFileSystem::NameTitle), fileIcon(file), Qt::DecorationRole);	//主线程以外不创建图片
			m_pModel->setData(m_pModel->index(nRow, QTreeViewFileSystem::NameTitle), 0, Qt::DecorationRole);
			m_pModel->setData(m_pModel->index(nRow, QTreeViewFileSystem::NameTitle), file.name);
			m_pModel->setData(m_pModel->index(nRow, QTreeViewFileSystem::NameTitle), qVariantFromValue(file), Qt::UserRole);

			m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewFileSystem::SizeTitle))->setEditable(false);
			m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewFileSystem::SizeTitle))->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
			m_pModel->setData(m_pModel->index(nRow, QTreeViewFileSystem::SizeTitle), fileSizeForNumber(file));

			m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewFileSystem::TypeTitle))->setEditable(false);
			m_pModel->setData(m_pModel->index(nRow, QTreeViewFileSystem::TypeTitle), fileType(file));

			m_pModel->itemFromIndex(m_pModel->index(nRow, QTreeViewFileSystem::ModifiedTitle))->setEditable(false);
			m_pModel->setData(m_pModel->index(nRow, QTreeViewFileSystem::ModifiedTitle), file.lastModified);
		}
	}

	emit finished(m_pModel);
}
