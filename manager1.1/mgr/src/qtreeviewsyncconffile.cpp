#include "qtreeviewsyncconffile.h"

#include <QMouseEvent>
#include <QDir>

#include "common.h"
#include "qimessage.h"
#include "qglobalinfo.h"

QSortFilterSyncConfFileModel::QSortFilterSyncConfFileModel(QObject* parent/* =0 */)
:QSortFilterProxyModel(parent)
{

}

QSortFilterSyncConfFileModel::~QSortFilterSyncConfFileModel()
{

}

bool QSortFilterSyncConfFileModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
	QAbstractItemModel*	pModel=sourceModel();

	Q_ASSERT_NO_NULL(pModel);

	if (left.column() == QTreeViewSyncCommit::NameTitle)
	{
		int nLeftStatus=pModel->data(pModel->index(left.row(), QTreeViewSyncCommit::StatusTitle), Qt::DisplayRole).toInt();
		int nRightStatus=pModel->data(pModel->index(right.row(), QTreeViewSyncCommit::StatusTitle), Qt::DisplayRole).toInt();

		if (nLeftStatus != nRightStatus)
		{
			if (nLeftStatus < Unmodified && nRightStatus >= Unmodified)
			{
				return sortOrder()==Qt::AscendingOrder? true:false;
			}
			else if (nRightStatus < Unmodified && nLeftStatus >= Unmodified)
			{
				return sortOrder()==Qt::AscendingOrder? false:true;
			}
		}
	}

	return QSortFilterProxyModel::lessThan(left, right);
}

QTreeViewSyncConfFile::QTreeViewSyncConfFile(QWidget *parent)
	: QTreeViewSyncCommit(parent)
{
	m_pSortModel=new QSortFilterSyncConfFileModel(this);
	m_pSortModel->setDynamicSortFilter(true);
	m_pSortModel->setSortCaseSensitivity(Qt::CaseInsensitive);

	setRootIsDecorated(false);
	setAlternatingRowColors(true);
	setModel(m_pSortModel);
	setSortingEnabled(true);

	sortByColumn(NameTitle, Qt::AscendingOrder);
}

QTreeViewSyncConfFile::~QTreeViewSyncConfFile()
{
	SAFE_DELETE_CLASS(m_pSortModel);
}

void QTreeViewSyncConfFile::setCustomItemDelegate()
{
	m_pDelegate=new QDelegateSortSyncCommit(this);
	setItemDelegate(m_pDelegate);
}

void QTreeViewSyncConfFile::checkDiff()
{
	QSyncConfFileRunnable* pRunnable=new QSyncConfFileRunnable(m_strWorkspace, m_strHistory, record(), true, m_bRestrictFilter);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QTreeModel*)), this, SLOT(finished(QTreeModel*)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

void QTreeViewSyncConfFile::setTreeModel(QTreeModel* model)
{
	m_pSortModel->setSourceModel(model);

	SAFE_DELETE_CLASS(m_pModel);
	m_pModel=model;
}

void QTreeViewSyncConfFile::setItemCheckState(int row, int state, const QModelIndex& parent/* =QModelIndex */)
{
	if (m_pModel == NULL)
	{
		return;
	}

	QTreeViewSyncCommit::setItemCheckState(row, state, parent);

	QModelIndex index=m_pModel->index(row, NameTitle, parent);
	FileInfo file=m_pModel->data(index, UserDataRole).value<FileInfo>();
	if (file.type == Dir || file.type == HideDir)
	{
		QString strPathSuffix=file.name+"/";
		int nNumRows=m_pModel->rowCount(parent);
		for (int i=0; i<nNumRows; i++)
		{
			index=m_pModel->index(i, NameTitle, parent);
			FileInfo info=m_pModel->data(index, UserDataRole).value<FileInfo>();
			if (state == Qt::Unchecked)
			{
				if (info.name.startsWith(strPathSuffix))
				{
					QTreeViewSyncCommit::setItemCheckState(i, state, parent);
				}
			}
			else if (state == Qt::Checked)
			{
				int nStatus=m_pModel->data(m_pModel->index(index.row(), StatusTitle, parent), Qt::DisplayRole).toInt();
				if (nStatus == Modified || nStatus == Added || nStatus == Deleted)
				{
					QTreeViewSyncCommit::setItemCheckState(i, state, parent);
				}
			}
		}
	}
	else if ((file.type & (File|HideFile|SymLink)) != 0)
	{
		if (state == Qt::Checked)
		{
			QString strSuffix=file.name;

			QList<QString> dirs;

			QRegExp rx("[/|\\\\]");
			int nIndex;

			while ((nIndex=rx.lastIndexIn(strSuffix)) != -1)
			{
				strSuffix=strSuffix.left(nIndex);

				if (!strSuffix.isEmpty())
				{
					dirs.append(strSuffix);
				}
			}

			if (!dirs.isEmpty())
			{
				int nNumRows=m_pModel->rowCount(parent);
				for (int i=0; i<nNumRows; i++)
				{
					index=m_pModel->index(i, NameTitle, parent);
					FileInfo info=m_pModel->data(index, UserDataRole).value<FileInfo>();
					if (info.type == Dir || info.type == HideDir)
					{
						QMutableListIterator<QString> it(dirs);
						while (it.hasNext())
						{
							QString strDir=it.next();
							if (strDir == info.name)
							{
								QTreeViewSyncCommit::setItemCheckState(i, state, parent);

								it.remove();
							}
						}
					}
				}
			}
		}
	}
}

QModelIndex QTreeViewSyncConfFile::mapToSourceItem(const QModelIndex& index)
{
	return m_pSortModel==NULL? index:m_pSortModel->mapToSource(index);
}

QSyncConfFileRunnable::QSyncConfFileRunnable(const QString& workspace, const QString& history, 
											 const QString& record, bool checkable, bool enableRestrict, QObject* parent/* =0 */)
:QTreeViewSyncCommitRunnable(workspace, history, record, checkable, enableRestrict, parent)
{

}

QSyncConfFileRunnable::~QSyncConfFileRunnable()
{

}

void QSyncConfFileRunnable::check()
{
	if (m_strWorkspace.isEmpty())
	{
		return;
	}

	traceSubDir(m_strWorkspace);

	if (!m_files.isEmpty())
	{
		QMapIterator<QString, DiffFile> it(m_files);
		while (it.hasNext())
		{
			it.next();

			const DiffFile& file=it.value();

			if (file.status != Deleted)
			{
				if (file.type == Dir || file.type == HideDir)
				{
					checkDir(it.key());
				}
				else if (file.type == File || file.type == HideFile)
				{
					if (m_bRestrictFilter && isRestrictFile(file.path))
					{
						continue;
					}

					checkFile(it.key());
				}
			}
		}
	}
}

REGISTER_TREE_VIEW_SYNC_COMMIT_CLASS(QTreeViewSyncConfFile)
