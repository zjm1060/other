#include "qtreeviewcomparefile.h"

#include <QMouseEvent>
#include <QHeaderView>

#include "qabstractfilecompare.h"
#include "qlogger.h"

QTreeViewCompareFile::QTreeViewCompareFile(bool inverse/* =false */, bool lockable/* =false */, QWidget *parent/* =0 */)
	: QTreeView(parent)
	,m_bLockable(lockable)
{
	m_pDelegate=new QCompareFileDelegate(inverse, this);
	setItemDelegate(m_pDelegate);

	setRootIsDecorated(false);
	setItemsExpandable(true);
	setSelectionMode(QAbstractItemView::ContiguousSelection);
	header()->setMovable(false);

	if (m_bLockable)
	{
		header()->setClickable(true);
		connect(header(), SIGNAL(sectionClicked(int)), this, SLOT(sectionClicked(int)));
	}

	connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(doubleClicked(const QModelIndex&)));
}

QTreeViewCompareFile::~QTreeViewCompareFile()
{
}

void QTreeViewCompareFile::setModel(QAbstractItemModel* model)
{
	QItemSelectionModel* pModel = selectionModel();

	QTreeView::setModel(model);

	SAFE_DELETE_CLASS(pModel);

	pModel=selectionModel();
	if (pModel != NULL)
	{
		connect(pModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), 
			this, SLOT(selectionChanged(const QItemSelection&, const QItemSelection&)));
	}
}

void QTreeViewCompareFile::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		return;
	}
	else if (event->button() == Qt::LeftButton && event->modifiers() == Qt::NoModifier && model() != NULL)
	{
		m_indexClicked=indexAt(event->pos());
	}

	QTreeView::mousePressEvent(event);
}

void QTreeViewCompareFile::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		QModelIndex index=indexAt(event->pos());
		
		if (index.isValid())
		{
			selectChangedBlock(index);

			emit mouseRightButtonPress(event->pos());
		}

		return;
	}
	else if (event->button() == Qt::LeftButton && event->modifiers() == Qt::NoModifier && model() != NULL)
	{
		QModelIndex index=indexAt(event->pos());
		if (m_indexClicked.isValid() && index == m_indexClicked && selectionModel()->currentIndex().row() == index.row())
		{
			selectionChanged(QItemSelection(), QItemSelection());
		}
	}

	m_indexClicked=QModelIndex();

	QTreeView::mouseReleaseEvent(event);
}

void QTreeViewCompareFile::selectChangedBlock(const QModelIndex& index)
{
	QAbstractItemModel* pModel=model();
	if (pModel == NULL)
	{
		return;
	}
	
	if (!m_indexes.contains(index) && !m_indexes.contains(pModel->index(index.row(), 0, index.parent())))
	{
		//Ñ¡ÖÐ¿é
		QModelIndexList indexes;
		indexes.append(pModel->index(index.row(), 0, index.parent()));

		QVariant varRow=pModel->data(pModel->index(index.row(), 0, index.parent()), QAbstractFileCompare::RowRole);
		for (int i=index.row()-1; i>=0; i--)
		{
			QModelIndex cIndex=pModel->index(i, 0, index.parent());
			if (pModel->data(cIndex, QAbstractFileCompare::RowRole) == varRow)
			{
				indexes.prepend(cIndex);
			}
			else
			{
				break;
			}
		}

		for (int i=index.row()+1; i<pModel->rowCount(index.parent()); i++)
		{
			QModelIndex cIndex=pModel->index(i, 0, index.parent());
			if (pModel->data(cIndex, QAbstractFileCompare::RowRole) == varRow)
			{
				indexes.append(cIndex);
			}
			else
			{
				break;
			}
		}

		setSelected(indexes);
	}
}

void QTreeViewCompareFile::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
	Q_UNUSED(selected);
	Q_UNUSED(deselected);

	QModelIndexList indexes=selectionModel()->selectedRows();
	if (indexes == m_indexes)
	{
		return;
	}

	setSelected(indexes);
}

void QTreeViewCompareFile::clearSelected()
{
	QAbstractItemModel* pModel=model();
	if (pModel == NULL || m_indexes.isEmpty())
	{
		return;
	}

	QModelIndex index=m_indexes.front();
	pModel->setData(pModel->index(index.row(), 0, index.parent()), QAbstractFileCompare::NoSelectRowRole, QAbstractFileCompare::SelectRowRole);
	pModel->setData(index, false, QAbstractFileCompare::SelectItemRole);

	if (m_indexes.size() > 1)
	{
		index=m_indexes.last();

		pModel->setData(pModel->index(index.row(), 0, index.parent()), QAbstractFileCompare::NoSelectRowRole, QAbstractFileCompare::SelectRowRole);
		pModel->setData(index, false, QAbstractFileCompare::SelectItemRole);
	}

	m_indexes.clear();
}

void QTreeViewCompareFile::setSelected(const QModelIndexList& indexes)
{
	selectBlock(indexes);

	emit blockSelection(m_indexes);
}

void QTreeViewCompareFile::doubleClicked(const QModelIndex& index)
{
	selectItem(index);

	emit itemSelection(index);
}

void QTreeViewCompareFile::selectItem(const QModelIndex& index)
{
	clearSelected();

	QAbstractItemModel* pModel=model();
	if (pModel == NULL)
	{
		return;
	}

	pModel->setData(index, true, QAbstractFileCompare::SelectItemRole);

	m_indexes.append(index);

	viewport()->update();
}

void QTreeViewCompareFile::selectBlock(const QModelIndexList& indexes)
{
	clearSelected();

	QAbstractItemModel* pModel=model();
	if (pModel == NULL || indexes.isEmpty())
	{
		viewport()->update();
		return;
	}

	QModelIndex index=indexes.front();
	pModel->setData(index, QAbstractFileCompare::SelectBeginRowRole, QAbstractFileCompare::SelectRowRole);

	index=indexes.last();
	int nRowRole=pModel->data(index, QAbstractFileCompare::SelectRowRole).toInt();
	nRowRole|=QAbstractFileCompare::SelectEndRowRole;
	pModel->setData(index, nRowRole, QAbstractFileCompare::SelectRowRole);

	m_indexes=indexes; 

	viewport()->update();
}

QModelIndexList QTreeViewCompareFile::selectedIndexes() const
{
	return m_indexes;
}

QVector<int> QTreeViewCompareFile::lockedColumns() const
{
	QVector<int> indexes;

	if (m_bLockable && model() != NULL)
	{
		QAbstractItemModel* pModel=model();

		int nNumColumn=pModel->columnCount();
		for (int i=0; i<nNumColumn; i++)
		{
			if (pModel->headerData(i, header()->orientation(), LockRole).toBool())
			{
				indexes.append(i);
			}
		}
	}

	return indexes;
}

QCompareFileUndoCommand* QTreeViewCompareFile::copy(QTreeViewCompareFile* treeview)
{
	if (treeview == NULL)
	{
		return NULL;
	}

	QAbstractItemModel* smodel=treeview->model();
	QAbstractItemModel* tmodel=model();

	if (smodel == NULL || tmodel == NULL)
	{
		return NULL;
	}

	QCompareFileUndoCommand* pUndoCommand=new QCompareFileUndoCommand(tmodel);

	const QModelIndexList& indexes=treeview->m_indexes;
	QListIterator<QModelIndex> it(indexes);
	while (it.hasNext())
	{
		const QModelIndex& index=it.next();

		QModelIndexList path=treePath(index);
		QModelIndex tindex;

		QListIterator<QModelIndex> iter(path);
		while (iter.hasNext())
		{
			const QModelIndex& cindex=iter.next();

			tindex=tmodel->index(cindex.row(), cindex.column(), tindex);
		}

		if (index.column() == 0 && (smodel->data(index, QAbstractFileCompare::RowRole).toInt() != QAbstractFileCompare::CommonRowRole) && 
			!smodel->data(index, QAbstractFileCompare::SelectItemRole).toBool())
		{
			int nNumColumn=smodel->columnCount(index.parent());
			for (int i=0; i<nNumColumn; i++)
			{
				if (!m_bLockable || !tmodel->headerData(i, header()->orientation(), LockRole).toBool())
				{
					int nRowRole=tmodel->data(tmodel->index(tindex.row(), 0, tindex.parent()), QAbstractFileCompare::RowRole).toInt();
					if (nRowRole != QAbstractFileCompare::ModifyRowRole ||
						tmodel->data(tmodel->index(tindex.row(), i, tindex.parent()), QAbstractFileCompare::ItemRole).toInt() != QAbstractFileCompare::CommonItemRole)
					{
						QModelIndex cindex=tmodel->index(tindex.row(), 0, tindex.parent());
						pUndoCommand->appendCommand(cindex, tmodel->data(cindex, QAbstractFileCompare::AssignRowRole),
							smodel->data(smodel->index(index.row(), 0, index.parent()), QAbstractFileCompare::RowRole), QAbstractFileCompare::AssignRowRole);

						cindex=tmodel->index(tindex.row(), i, tindex.parent());
						pUndoCommand->appendCommand(cindex, tmodel->data(cindex), smodel->data(smodel->index(index.row(), i, index.parent())));
						pUndoCommand->appendCommand(cindex, tmodel->data(cindex, QAbstractFileCompare::AssignItemRole), true, QAbstractFileCompare::AssignItemRole);

						logplusInfo(MANAGER_LOGGER_STR, QString("Line %1, Col %2: %3 --> %4").arg(cindex.row())
							.arg(cindex.column()).arg(tmodel->data(cindex).toString()).arg(smodel->data(smodel->index(index.row(), i, index.parent())).toString()));
					}
				}
			}
		}
		else if (!m_bLockable || !tmodel->headerData(index.column(), header()->orientation(), LockRole).toBool())
		{
			int nRowRole=tmodel->data(tmodel->index(tindex.row(), 0, tindex.parent()), QAbstractFileCompare::RowRole).toInt();
			if (nRowRole != QAbstractFileCompare::CommonRowRole && (nRowRole != QAbstractFileCompare::ModifyRowRole ||
				tmodel->data(tindex, QAbstractFileCompare::ItemRole).toInt() != QAbstractFileCompare::CommonItemRole))
			{
				QModelIndex cindex=tmodel->index(tindex.row(), 0, tindex.parent());
				pUndoCommand->appendCommand(cindex, tmodel->data(cindex, QAbstractFileCompare::AssignRowRole), 
					smodel->data(smodel->index(index.row(), 0, index.parent()), QAbstractFileCompare::RowRole), QAbstractFileCompare::AssignRowRole);

				pUndoCommand->appendCommand(tindex, tmodel->data(tindex), smodel->data(index));
				pUndoCommand->appendCommand(tindex, tmodel->data(tindex, QAbstractFileCompare::AssignItemRole), true, QAbstractFileCompare::AssignItemRole);

				logplusInfo(MANAGER_LOGGER_STR, QString("Line %1, Col %2: %3 --> %4").arg(tindex.row())
					.arg(tindex.column()).arg(tmodel->data(tindex).toString()).arg(smodel->data(index).toString()));
			}
		}
	}

	pUndoCommand->redo();

	viewport()->update();

	return pUndoCommand;
}

QCompareFileUndoCommand* QTreeViewCompareFile::copyWhole(QTreeViewCompareFile* treeview)
{
	if (treeview == NULL)
	{
		return NULL;
	}

	QAbstractItemModel* smodel=treeview->model();
	QAbstractItemModel* tmodel=model();

	if (smodel == NULL || tmodel == NULL)
	{
		return NULL;
	}

	QCompareFileUndoCommand* pUndoCommand=new QCompareFileUndoCommand(tmodel);

	copy(smodel, QModelIndex(), tmodel, QModelIndex(), pUndoCommand);

	pUndoCommand->redo();

	viewport()->update();

	return pUndoCommand;
}

void QTreeViewCompareFile::copy(QAbstractItemModel* smodel, const QModelIndex& psindex, QAbstractItemModel* tmodel, const QModelIndex& ptindex,
								QCompareFileUndoCommand* command)
{
	int nNumRows=smodel->rowCount(psindex);
	if (nNumRows == 0)
	{
		return;
	}

	for (int i=0; i<nNumRows; i++)
	{
		QModelIndex sindex=smodel->index(i, 0, psindex);
		QModelIndex tindex=tmodel->index(i, 0, ptindex);

		command->appendCommand(tindex, tmodel->data(tindex, QAbstractFileCompare::AssignRowRole), 
			smodel->data(sindex, QAbstractFileCompare::RowRole), QAbstractFileCompare::AssignRowRole);

		int nNumCols=smodel->columnCount(psindex);
		for (int j=0; j<nNumCols; j++)
		{
			if (!m_bLockable || !tmodel->headerData(j, header()->orientation(), LockRole).toBool())
			{
				sindex=smodel->index(i, j, psindex);
				tindex=tmodel->index(i, j, ptindex);

				int nRowRole=tmodel->data(tmodel->index(i, 0, psindex), QAbstractFileCompare::RowRole).toInt();
				if ((nRowRole != QAbstractFileCompare::CommonRowRole && nRowRole != QAbstractFileCompare::ModifyRowRole) || 
					(nRowRole == QAbstractFileCompare::ModifyRowRole && tmodel->data(tindex, QAbstractFileCompare::ItemRole).toInt() != QAbstractFileCompare::CommonItemRole))
				{
					if (command != NULL)
					{
						command->appendCommand(tindex, tmodel->data(tindex), smodel->data(sindex));
						command->appendCommand(tindex, tmodel->data(tindex, QAbstractFileCompare::AssignItemRole), true, QAbstractFileCompare::AssignItemRole);
					}
				}

				copy(smodel, sindex, tmodel, tindex, command);
			}
		}
	}
}

bool QTreeViewCompareFile::nextDifference(bool move/* =true */)
{
	if (model() == NULL)
	{
		return false;
	}

	QModelIndex index;
	if (!m_indexes.isEmpty())
	{
		index=m_indexes.last();
		while (index.isValid() && index.row()+1 == model()->rowCount(index.parent()))
		{
			index=index.parent();
		}

		if (!index.isValid())
		{
			return false;
		}

		index=model()->index(index.row()+1, 0, index.parent());
	}
	else
	{
		if (model()->rowCount() > 0)
		{
			index=model()->index(0, 0);
		}
		else
		{
			return false;
		}
	}

	return findNextDifference(index, move);
}

bool QTreeViewCompareFile::findNextDifference(const QModelIndex& index, bool move/* =true */)
{
	if (model() == NULL)
	{
		return false;
	}

	QModelIndex cindex=index;

	while (cindex.isValid())
	{
		int nRowRole=model()->data(cindex, QAbstractFileCompare::RowRole).toInt();
		if (nRowRole != QAbstractFileCompare::CommonRowRole)
		{
			if (move)
			{
				selectChangedBlock(cindex);

				scrollTo(m_indexes.last());

				emit blockSelection(m_indexes);
			}

			return true;
		}

		if (cindex.row() < model()->rowCount(cindex.parent())-1)
		{
			cindex=model()->index(cindex.row()+1, 0, cindex.parent());
		}
		else
		{
			cindex=cindex.parent();
		}
	}

	return false;
}

bool QTreeViewCompareFile::previousDifference(bool move/* =true */)
{
	if (model() == NULL)
	{
		return false;
	}

	QModelIndex index;
	if (!m_indexes.isEmpty())
	{
		index=m_indexes.front();
		while (index.isValid() && index.row() == 0)
		{
			index=index.parent();
		}

		if (!index.isValid())
		{
			return false;
		}

		index=model()->index(index.row()-1, 0, index.parent());
	}
	else
	{
		return false;
	}

	return findPreviousDifference(index, move);
}

bool QTreeViewCompareFile::findPreviousDifference(const QModelIndex& index, bool move/* =true */)
{
	if (model() == NULL)
	{
		return false;
	}

	QModelIndex cindex=index;

	while (cindex.isValid())
	{
		int nRowRole=model()->data(cindex, QAbstractFileCompare::RowRole).toInt();
		if (nRowRole != QAbstractFileCompare::CommonRowRole)
		{
			if (move)
			{
				selectChangedBlock(cindex);

				scrollTo(m_indexes.front());

				emit blockSelection(m_indexes);
			}

			return true;
		}

		if (cindex.row() > 0)
		{
			cindex=model()->index(cindex.row()-1, 0, cindex.parent());
		}
		else
		{
			cindex=cindex.parent();
		}
	}

	return false;
}

bool QTreeViewCompareFile::nextItemDifference(bool move/* =true */)
{
	if (model() == NULL)
	{
		return false;
	}

	QModelIndex index;
	if (!m_indexes.isEmpty())
	{
		index=m_indexes.front();
		
		if (m_indexes.size() == 1)
		{
			int nRowRole=model()->data(model()->index(index.row(), 0, index.parent()), QAbstractFileCompare::RowRole).toInt();
			if (nRowRole != QAbstractFileCompare::ModifyRowRole || index.column()+1 == model()->columnCount(index.parent()))
			{
				while (index.isValid() && index.row()+1 == model()->rowCount(index.parent()))
				{
					index=index.parent();
				}

				if (!index.isValid())
				{
					return false;
				}

				index=model()->index(index.row()+1, 0, index.parent());
			}
			else
			{
				index=model()->index(index.row(), index.column()+1, index.parent());
			}
		}
	}
	else
	{
		if (model()->rowCount() > 0)
		{
			index=model()->index(0, 0);
		}
		else
		{
			return false;
		}
	}

	return findNextItemDifference(index, move);
}

bool QTreeViewCompareFile::findNextItemDifference(const QModelIndex& index, bool move/* =true */)
{
	if (model() == NULL)
	{
		return false;
	}

	QModelIndex cindex=index;

	while (cindex.isValid())
	{
		int nRowRole=model()->data(model()->index(cindex.row(), 0, cindex.parent()), QAbstractFileCompare::RowRole).toInt();
		if (nRowRole != QAbstractFileCompare::CommonRowRole && nRowRole != QAbstractFileCompare::ModifyRowRole)
		{
			if (move)
			{
				QModelIndexList indexes;
				indexes<<model()->index(cindex.row(), 0, cindex.parent());
				selectBlock(indexes);

				scrollTo(indexes.front());

				emit blockSelection(m_indexes);
			}

			return true;
		}
		else if (nRowRole == QAbstractFileCompare::ModifyRowRole)
		{
			int nItemRole=model()->data(cindex, QAbstractFileCompare::ItemRole).toInt();
			if (nItemRole != QAbstractFileCompare::CommonItemRole)
			{
				if (move)
				{
					doubleClicked(cindex);
					scrollTo(cindex);
				}

				return true;
			}

			if (cindex.column()+1 < model()->columnCount(cindex.parent()))
			{
				cindex=model()->index(cindex.row(), cindex.column()+1, cindex.parent());

				continue;
			}
		}

		if (cindex.row() < model()->rowCount(cindex.parent())-1)
		{
			cindex=model()->index(cindex.row()+1, 0, cindex.parent());
		}
		else
		{
			cindex=cindex.parent();
		}
	}

	return false;
}

bool QTreeViewCompareFile::previousItemDifference(bool move/* =true */)
{
	if (model() == NULL)
	{
		return false;
	}

	QModelIndex index;
	if (!m_indexes.isEmpty())
	{
		index=m_indexes.front();

		if (m_indexes.size() == 1)
		{
			int nRowRole=model()->data(model()->index(index.row(), 0, index.parent()), QAbstractFileCompare::RowRole).toInt();
			if (nRowRole != QAbstractFileCompare::ModifyRowRole || index.column() == 0)
			{
				while (index.isValid() && index.row() == 0)
				{
					index=index.parent();
				}

				if (!index.isValid())
				{
					return false;
				}

				index=model()->index(index.row()-1, model()->columnCount(index.parent())-1, index.parent());
			}
			else
			{
				index=model()->index(index.row(), index.column()-1, index.parent());
			}
		}
		else
		{
			while (index.isValid() && index.row() == 0)
			{
				index=index.parent();
			}

			if (!index.isValid())
			{
				return false;
			}

			index=model()->index(index.row()-1, model()->columnCount(index.parent())-1, index.parent());
		}
	}
	else
	{
		return false;
	}

	return findPreviousItemDifference(index, move);
}

bool QTreeViewCompareFile::findPreviousItemDifference(const QModelIndex& index, bool move/* =true */)
{
	if (model() == NULL)
	{
		return false;
	}

	QModelIndex cindex=index;
	while (cindex.isValid())
	{
		int nRowRole=model()->data(model()->index(cindex.row(), 0, cindex.parent()), QAbstractFileCompare::RowRole).toInt();
		if (nRowRole != QAbstractFileCompare::CommonRowRole && nRowRole != QAbstractFileCompare::ModifyRowRole)
		{
			if (move)
			{
				QModelIndexList indexes;
				indexes<<model()->index(cindex.row(), 0, cindex.parent());
				selectBlock(indexes);

				scrollTo(indexes.front());

				emit blockSelection(m_indexes);
			}

			return true;
		}
		else if (nRowRole == QAbstractFileCompare::ModifyRowRole)
		{
			int nItemRole=model()->data(cindex, QAbstractFileCompare::ItemRole).toInt();
			if (nItemRole != QAbstractFileCompare::CommonItemRole)
			{
				if (move)
				{
					doubleClicked(cindex);
					scrollTo(cindex);
				}

				return true;
			}

			if (cindex.column() > 0)
			{
				cindex=model()->index(cindex.row(), cindex.column()-1, cindex.parent());
				continue;
			}
		}

		if (cindex.row() > 0)
		{
			cindex=model()->index(cindex.row()-1, model()->columnCount(cindex.parent())-1, cindex.parent());
		}
		else
		{
			cindex=cindex.parent();
		}
	}

	return false;
}

void QTreeViewCompareFile::sectionClicked(int logicalIndex)
{
	QAbstractItemModel* pModel=model();
	if (pModel != NULL)
	{
		QVariant var=pModel->headerData(logicalIndex, header()->orientation(), LockRole);
		if (!var.isValid() || !var.toBool())
		{
			pModel->setHeaderData(logicalIndex, header()->orientation(), QIcon(":images/lock_closed.png"), Qt::DecorationRole);
			pModel->setHeaderData(logicalIndex, header()->orientation(), true, LockRole);
		}
		else
		{
			pModel->setHeaderData(logicalIndex, header()->orientation(), QIcon(), Qt::DecorationRole);
			pModel->setHeaderData(logicalIndex, header()->orientation(), false, LockRole);
		}

		emit lockedChanged();
	}
}

QModelIndexList QTreeViewCompareFile::treePath(const QModelIndex& index)
{
	QModelIndexList indexes;
	indexes.append(index);

	QModelIndex pindex=index;
	do 
	{
		pindex=pindex.parent();
		if (pindex.isValid())
		{
			indexes.prepend(pindex);
		}
		else
		{
			break;
		}
	} while (true);

	return indexes;
}

QCompareFileUndoCommand::QCompareFileUndoCommand(QAbstractItemModel* model, QUndoCommand* parent/* =0 */)
:QUndoCommand(parent)
,m_pModel(model)
{

}

QCompareFileUndoCommand::~QCompareFileUndoCommand()
{

}

void QCompareFileUndoCommand::appendCommand(const QModelIndex& index, const QVariant& svalue, const QVariant& tvalue, int role/* =Qt::EditRole */)
{
	UndoCommand command;

	command.index=index;
	command.role=role;
	command.svalue=svalue;
	command.tvalue=tvalue;

	m_commands.append(command);
}

void QCompareFileUndoCommand::undo()
{
	if (m_pModel == NULL)
	{
		return;
	}

	for (int i=m_commands.size()-1; i>=0; i--)
	{
		const UndoCommand& command=m_commands[i];

		m_pModel->setData(command.index, command.svalue, command.role);
	}
}

void QCompareFileUndoCommand::redo()
{
	if (m_pModel == NULL)
	{
		return;
	}

	for (int i=0; i<m_commands.size(); i++)
	{
		const UndoCommand& command=m_commands[i];

		m_pModel->setData(command.index, command.tvalue, command.role);
	}
}
