#include "qdelegatesynccommit.h"

#include <QApplication>
#include <QAbstractProxyModel>

#include "common.h"
#include "qimessage.h"
#include "qtreeviewsynccommit.h"

QDelegateSyncCommit::QDelegateSyncCommit(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

QDelegateSyncCommit::~QDelegateSyncCommit()
{

}

void QDelegateSyncCommit::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QAbstractItemModel* pModel=index.model();

	drawItem(painter, option, pModel, index);
}

void QDelegateSyncCommit::drawItem(QPainter* painter, const QStyleOptionViewItem& option, const QAbstractItemModel* model, const QModelIndex& index) const
{
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	int nRole=model->data(model->index(index.row(), QTreeViewSyncCommit::NameTitle, index.parent()), QTreeViewSyncCommit::ItemRole).toInt();

	if (nRole == QTreeViewSyncCommit::FileRole)
	{
		QColor color(Qt::black);

		FileInfo file=model->data(model->index(index.row(), QTreeViewSyncCommit::NameTitle, index.parent()),
			QTreeViewSyncCommit::UserDataRole).value<FileInfo>();

		int nStatus=model->data(model->index(index.row(), QTreeViewSyncCommit::StatusTitle, index.parent()), Qt::DisplayRole).toInt();
		switch (nStatus)
		{
		case Modified:
			{
				color=QColor(60, 90, 200);
			}
			break;
		case Added:
			{
				color=QColor(128, 60, 180);
			}
			break;
		case Deleted:
			{
				color=QColor(100, 0, 0);
			}
			break;
		default:
			break;
		}

		opt.palette.setColor(QPalette::Text, color);

		if (index.column() == QTreeViewSyncCommit::NameTitle)
		{
			painter->save();

			QModelIndex cindex=model->index(index.row(), QTreeViewSyncCommit::NameTitle, index.parent());
			int nChecked=model->data(cindex, QTreeViewSyncCommit::CheckedRole).toInt();

			opt.features|=QStyleOptionViewItemV2::HasCheckIndicator;
			opt.checkState=(Qt::CheckState)nChecked;

			opt.features|=QStyleOptionViewItemV2::HasDecoration;
			opt.icon=fileIcon(file);

			bool bCheckable=model->data(cindex, QTreeViewSyncCommit::CheckableRole).toBool();
			if (bCheckable)
			{
				opt.state |=  QStyle::State_Enabled;
			}
			else
			{
				opt.state &=  ~QStyle::State_Enabled;
			}

			style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

			painter->restore();
			return;
		}
		else if (index.column() == QTreeViewSyncCommit::SizeTitle)
		{
			initStyleOption(&opt, index);

			if (file.type == File || file.type == HideFile)
			{
				opt.text=fileSize(file.size);
			}
			else
			{
				opt.text="";
			}
			
			opt.displayAlignment=Qt::AlignVCenter|Qt::AlignRight;

			painter->save();

			style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

			painter->restore();

			return;
		}
		else if (index.column() == QTreeViewSyncCommit::StatusTitle)
		{
			initStyleOption(&opt, index);

			switch (nStatus)
			{
			case Unmodified:
				opt.text=tr("unmodified");
				break;
			case Modified:
				opt.text=tr("modified");
				break;
			case Added:
				opt.text=tr("added");
				break;
			case Deleted:
				opt.text=tr("deleted");
				break;
			case NonVersioned:
				opt.text=tr("uncommitted");
				break;
			default:
				break;
			}

			painter->save();

			style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

			painter->restore();

			return;
		}
	}
	else if (nRole == QTreeViewSyncCommit::GroupRole)
	{
		if (index.column() == QTreeViewSyncCommit::NameTitle)
		{
			painter->save();

			QModelIndex cindex=model->index(index.row(), QTreeViewSyncCommit::NameTitle, index.parent());
			int nChecked=model->data(cindex, QTreeViewSyncCommit::CheckedRole).toInt();

			opt.features|=QStyleOptionViewItemV2::HasCheckIndicator;
			opt.checkState=(Qt::CheckState)nChecked;

			bool bCheckable=model->data(cindex, QTreeViewSyncCommit::CheckableRole).toBool();
			if (bCheckable)
			{
				opt.state |=  QStyle::State_Enabled;
			}
			else
			{
				opt.state &=  ~QStyle::State_Enabled;
			}

			style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

			painter->restore();
			return;
		}
	}

	QStyledItemDelegate::paint(painter, opt, index);
}

QSize QDelegateSyncCommit::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
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

QDelegateSortSyncCommit::QDelegateSortSyncCommit(QObject *parent)
: QDelegateSyncCommit(parent)
{
}

QDelegateSortSyncCommit::~QDelegateSortSyncCommit()
{

}

void QDelegateSortSyncCommit::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QAbstractProxyModel* pModel=dynamic_cast<const QAbstractProxyModel*>(index.model());
	if (pModel == NULL)
	{
		return;
	}

	QModelIndex sindex=pModel->mapToSource(index);

	drawItem(painter, option, pModel->sourceModel(), sindex);
}
