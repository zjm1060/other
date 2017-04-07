#include "qtreeviewgrouptransferdelegate.h"

#include <QApplication>
#include <QStyle>

#include "common.h"
#include "qimessage.h"
#include "qfilecomparefactory.h"

QTreeViewGroupTransferDelegate::QTreeViewGroupTransferDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
	m_statusIcon[QTreeViewGroupTransferQueue::Transfering]=QIcon(":/images/goup.png");
	m_statusIcon[QTreeViewGroupTransferQueue::BackingUp]=QIcon(":/images/backup.png");
	m_statusIcon[QTreeViewGroupTransferQueue::Failed]=QIcon(":/images/error1.png");
}

QTreeViewGroupTransferDelegate::~QTreeViewGroupTransferDelegate()
{

}

void QTreeViewGroupTransferDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QAbstractItemModel*	pModel=index.model();
	if (pModel == NULL)
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

	QModelIndex cindex=pModel->index(index.row(), QTreeViewGroupTransferQueue::NameTitle, index.parent());

	int nRole=pModel->data(cindex, QTreeViewGroupTransferQueue::TransferRole).toInt();
	if (nRole == QTreeViewGroupTransferQueue::FileRole)
	{
		QColor color(Qt::black);
		int nStatus=pModel->data(pModel->index(index.row(), QTreeViewGroupTransferQueue::StatusTitle, index.parent()), Qt::DisplayRole).toInt();
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

		if (index.column() == QTreeViewGroupTransferQueue::NameTitle)
		{
			QIcon icon;

			QTreeViewGroupTransferQueue::TransferQueueFileItem item=
				qvariant_cast<QTreeViewGroupTransferQueue::TransferQueueFileItem>(pModel->data(index, QTreeViewGroupTransferQueue::TransferDataRole));

			FileInfo info;
			info.name=item.file.name;
			info.type=item.file.type;

			int nTransferStatus=pModel->data(index, QTreeViewGroupTransferQueue::TransferStatusRole).toInt();
			icon=addIconToIcon(m_statusIcon[nTransferStatus], fileIcon(info), Qt::BottomRightCorner);

			opt.features|=QStyleOptionViewItemV2::HasDecoration;
			opt.icon=icon;
		}
		else if (index.column() == QTreeViewGroupTransferQueue::StatusTitle)
		{
//			initStyleOption(&opt, index);

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
				opt.text=tr("non-versioned");
				break;
			default:
				break;
			}
		}
		else if (index.column() == QTreeViewGroupTransferQueue::ProcessTitle)
		{
			QStyleOptionProgressBar progressBarOption;
			progressBarOption.rect = option.rect.adjusted(1, 1, -1, -1);
			progressBarOption.minimum = 0;
			progressBarOption.maximum = 10000;
			progressBarOption.progress = 0;
			progressBarOption.textVisible = true;
			progressBarOption.textAlignment=Qt::AlignCenter;

			if (pModel != NULL)
			{
				int nStatus=pModel->data(cindex, QTreeViewGroupTransferQueue::TransferStatusRole).toInt();
				if (nStatus == QTreeViewGroupTransferQueue::Waiting)
				{
					progressBarOption.text = tr("Waiting...");
				}
				else if (nStatus == QTreeViewGroupTransferQueue::Transfering)
				{
					QTreeViewGroupTransferQueue::TransferQueueFileItem item=
						pModel->data(cindex, QTreeViewGroupTransferQueue::TransferDataRole).value<QTreeViewGroupTransferQueue::TransferQueueFileItem>();

					if (item.file.type == File || item.file.type == HideFile)
					{
						quint64 nSize=pModel->data(pModel->index(index.row(), QTreeViewGroupTransferQueue::ProcessTitle, index.parent()),
							QTreeViewGroupTransferQueue::TransferProcessRole).toULongLong();

						if (item.file.size > 0)
						{
							progressBarOption.progress=(int)((nSize<=item.file.size? nSize:item.file.size)*10000.0/item.file.size);
						}
						else
						{
							progressBarOption.progress=0;
						}
						
						progressBarOption.text=QString::number(progressBarOption.progress/100.0, 'f', 2)+"%";
					}
					else
					{
						progressBarOption.text = tr("Transfering...");
					}
				}
				else if (nStatus == QTreeViewGroupTransferQueue::Failed)
				{
					progressBarOption.text = tr("Failed");
				}

				painter->save();

				style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
				style->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);

				painter->restore();

				return;
			}
		}

		painter->save();

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		painter->restore();

		return;
	}
	else if (nRole == QTreeViewGroupTransferQueue::BackUpRole)
	{
		opt.palette.setColor(QPalette::Text, QColor(Qt::black));

		if (index.column() == QTreeViewGroupTransferQueue::NameTitle)
		{
			opt.features|=QStyleOptionViewItemV2::HasDecoration;
			opt.icon=m_statusIcon[QTreeViewGroupTransferQueue::BackingUp];
		}
		else if (index.column() == QTreeViewGroupTransferQueue::ProcessTitle)
		{
			QStyleOptionProgressBar progressBarOption;
			progressBarOption.rect = option.rect.adjusted(1, 1, -1, -1);
			progressBarOption.minimum = 0;
			progressBarOption.maximum = 10000;
			progressBarOption.progress = 0;
			progressBarOption.textVisible = true;
			progressBarOption.textAlignment=Qt::AlignCenter;

			if (pModel != NULL)
			{
				int nStatus=pModel->data(cindex, QTreeViewGroupTransferQueue::TransferStatusRole).toInt();
				if (nStatus == QTreeViewGroupTransferQueue::Waiting)
				{
					progressBarOption.text = tr("Waiting...");
				}
				else if (nStatus == QTreeViewGroupTransferQueue::BackingUp)
				{
					QTreeViewGroupTransferQueue::TransferQueueBackupItem item=
						pModel->data(cindex, QTreeViewGroupTransferQueue::TransferDataRole).value<QTreeViewGroupTransferQueue::TransferQueueBackupItem>();

					quint64 nSize=pModel->data(pModel->index(index.row(), QTreeViewGroupTransferQueue::ProcessTitle, index.parent()),
						QTreeViewGroupTransferQueue::TransferProcessRole).toULongLong();

					if (item.files.size() > 0)
					{
						progressBarOption.progress=(int)((nSize<=item.files.size()? nSize:item.files.size())*10000.0/item.files.size());
					}
					else
					{
						progressBarOption.progress=0;
					}
					
					progressBarOption.text=QString::number(progressBarOption.progress/100.0, 'f', 2)+"%";
				}
				else if (nStatus == QTreeViewGroupTransferQueue::Failed)
				{
					progressBarOption.text = tr("Failed");
				}

				painter->save();

				style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
				style->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);

				painter->restore();

				return;
			}
		}

		painter->save();

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		painter->restore();

		return;
	}
	else if (nRole == QTreeViewGroupTransferQueue::CommandRole)
	{
		if (index.column() == QTreeViewGroupTransferQueue::NameTitle)
		{
			opt.features|=QStyleOptionViewItemV2::HasDecoration;
			opt.icon=QIcon(":/images/command.png");
		}
		else if (index.column() == QTreeViewGroupTransferQueue::ProcessTitle)
		{
			int nStatus=pModel->data(cindex, QTreeViewGroupTransferQueue::TransferStatusRole).toInt();
			if (nStatus == QTreeViewGroupTransferQueue::Waiting)
			{
				opt.text = tr("Waiting...");
			}
			else if (nStatus == QTreeViewGroupTransferQueue::Skip)
			{
				opt.text=tr("Skip");
			}
			else if (nStatus == QTreeViewGroupTransferQueue::Failed)
			{
				opt.text = tr("Failed");
			}

			opt.decorationAlignment=Qt::AlignCenter;
		}

		painter->save();

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		painter->restore();

		return;
	}
	else if (nRole == QTreeViewGroupTransferQueue::ServerRole)
	{
		QIcon icon;

		int nTransferStatus=pModel->data(index, QTreeViewGroupTransferQueue::TransferStatusRole).toInt();
		switch (nTransferStatus)
		{
		case QTreeViewGroupTransferQueue::Waiting:
			break;
		case QTreeViewGroupTransferQueue::Connecting:
			icon=QIcon(":/images/wait.png");
			break;
		case QTreeViewGroupTransferQueue::Connected:
			icon=QIcon(":/images/tick.png");
			break;
		case QTreeViewGroupTransferQueue::Unconneced:
			icon=QIcon(":/images/error1.png");
			break;
		default:
			break;
		}

		opt.features|=QStyleOptionViewItemV2::HasDecoration;
		opt.icon=icon;

		painter->save();

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		painter->restore();

		return;
	}
	
	QStyledItemDelegate::paint(painter, opt, index);
}

QSize QTreeViewGroupTransferDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
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
