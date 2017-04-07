#include "qtreeviewupdateconffile.h"

#include "qglobalinfo.h"
#include "qimessage.h"
#include "common.h"

QSortFilterUpdateFileModel::QSortFilterUpdateFileModel(QObject* parent/* =0 */)
:QSortFilterProxyModel(parent)
{

}

QSortFilterUpdateFileModel::~QSortFilterUpdateFileModel()
{

}

int QSortFilterUpdateFileModel::code(int status) const
{
	if (status < Unmodified || status == Delete)
	{
		return 3;
	}
	else if (status == Latest)
	{
		return 2;
	}
	
	return 1;
}

bool QSortFilterUpdateFileModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
	QAbstractItemModel*	pModel=sourceModel();

	Q_ASSERT_NO_NULL(pModel);

	if (left.column() == QTreeViewUpdateFile::NameTitle)
	{
		int nLeftStatus=pModel->data(pModel->index(left.row(), QTreeViewUpdateFile::StatusTitle), Qt::DisplayRole).toInt();
		int nRightStatus=pModel->data(pModel->index(right.row(), QTreeViewUpdateFile::StatusTitle), Qt::DisplayRole).toInt();

		if (nLeftStatus != nRightStatus)
		{
			int nLeftCode=code(nLeftStatus);
			int nRightCode=code(nRightStatus);
			
			if (nLeftCode > nRightCode)
			{
				return sortOrder()==Qt::AscendingOrder? true:false;
			}
			else if (nLeftCode < nRightCode)
			{
				return sortOrder()==Qt::AscendingOrder? false:true;
			}
		}
	}

	return QSortFilterProxyModel::lessThan(left, right);
}

QTreeViewUpdateConfFile::QTreeViewUpdateConfFile(QWidget *parent)
	: QTreeViewUpdateFile(parent)
{
	m_pSortModel=new QSortFilterUpdateFileModel(this);
	m_pSortModel->setDynamicSortFilter(true);
	m_pSortModel->setSortCaseSensitivity(Qt::CaseInsensitive);

	setRootIsDecorated(false);
	setAlternatingRowColors(true);
	setModel(m_pSortModel);
	setSortingEnabled(true);
	setSelectionMode(QAbstractItemView::ExtendedSelection);

	sortByColumn(NameTitle, Qt::AscendingOrder);
}

QTreeViewUpdateConfFile::~QTreeViewUpdateConfFile()
{
	m_pSortModel->setSourceModel(NULL);

	setModel(NULL);
	SAFE_DELETE_CLASS(m_pSortModel);
}

void QTreeViewUpdateConfFile::setCustomItemDelegate()
{
	m_pDelegate=new QDelegateSortUpdateFile(this);
	setItemDelegate(m_pDelegate);
}

void QTreeViewUpdateConfFile::setTreeModel(QTreeModel* model)
{
	m_pSortModel->setSourceModel(model);

	SAFE_DELETE_CLASS(m_pModel);
	m_pModel=model;
}

QModelIndex QTreeViewUpdateConfFile::mapToSourceItem(const QModelIndex& index)
{
	return m_pSortModel==NULL? index:m_pSortModel->mapToSource(index);
}

QModelIndex QTreeViewUpdateConfFile::mapFromSourceItem(const QModelIndex& index)
{
	return m_pSortModel==NULL? index:m_pSortModel->mapFromSource(index);
}

void QTreeViewUpdateConfFile::check()
{	
	QUpdateConfFileRunnable* pRunnable=new QUpdateConfFileRunnable(m_strWorkspace, record(), m_strHostName, m_strAddress, m_bRestrictFilter, m_bLocalLastModified);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QTreeModel*)), this, SLOT(finished(QTreeModel*)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

void QTreeViewUpdateConfFile::checkModified(const QString& workspace, const QString& hostname, const QString& address)
{
	QUpdateConfFileRunnable* pRunnable=new QUpdateConfFileRunnable(workspace, record(), hostname, address, m_bRestrictFilter, true);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(modified(QString, QString)), this, SIGNAL(findModified(QString, QString)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

REGISTER_TREE_VIEW_UPDATE_FILE_CLASS(QTreeViewUpdateConfFile)

QDelegateSortUpdateFile::QDelegateSortUpdateFile(QObject *parent)
: QUpdateFileDelegate(parent)
{
}

QDelegateSortUpdateFile::~QDelegateSortUpdateFile()
{

}

void QDelegateSortUpdateFile::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QAbstractProxyModel* pModel=dynamic_cast<const QAbstractProxyModel*>(index.model());
	if (pModel == NULL)
	{
		return;
	}

	QModelIndex sindex=pModel->mapToSource(index);

	drawItem(painter, option, pModel->sourceModel(), sindex);
}

QUpdateConfFileRunnable::QUpdateConfFileRunnable(const QString& workspace, const QString& catagory, const QString& hostname, const QString& address,
												 bool restrictFilter, bool localLastModified, bool immediate/* =false */, QObject* parent/* =0 */)
:QUpdateFileRunnable(workspace, catagory, hostname, address, restrictFilter,localLastModified, immediate, parent)
{

}

QUpdateConfFileRunnable::~QUpdateConfFileRunnable()
{

}

bool QUpdateConfFileRunnable::readFiles(bool immediate)
{
	bool bModified=false;

	bModified=traceSubDir(m_strWorkspace, "", immediate);

	if (!m_filesRequest.isEmpty())
	{
		QMap<QString, DiffFile> requests=m_filesRequest;
		QMapIterator<QString, DiffFile> it(requests);
		while (it.hasNext())
		{
			it.next();

			const DiffFile& file=it.value();

			if ((file.type & (Dir|HideDir)) != 0)
			{
				readDir(it.key(), immediate);
			}
			else if ((file.type & (File|HideFile)) != 0)
			{
				if (m_bRestrictFilter && isRestrictFile(it.key()))
				{
					continue;
				}

				readFile(it.key(), immediate);
			}
		}
	}

	return bModified;
}

