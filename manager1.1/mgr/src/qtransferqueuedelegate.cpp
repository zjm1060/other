#include "qtransferqueuedelegate.h"

#include <QApplication>

#include "qimessage.h"
#include "common.h"
#include "qreceivertransferfile.h"
#include "qtreeviewtransferqueue.h"

QTransferQueueDelegate::QTransferQueueDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

QTransferQueueDelegate::~QTransferQueueDelegate()
{

}

void QTransferQueueDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.column() == ProcessTitle)
	{
		QStyleOptionProgressBar progressBarOption;
		progressBarOption.rect = option.rect;
		progressBarOption.minimum = 0;
		progressBarOption.maximum = 10000;
		progressBarOption.progress = 0;
		progressBarOption.textVisible = true;
		progressBarOption.textAlignment=Qt::AlignCenter;

		const QAbstractItemModel*	pModel=index.model();

		if (pModel != NULL)
		{
			int nStatus=pModel->data(pModel->index(index.row(), NameTitle), Qt::UserRole+1).toInt();
			if (nStatus == QTreeViewTransferQueue::Waiting)
			{
				progressBarOption.text = tr("Waiting...");
			}
			else if (nStatus == QTreeViewTransferQueue::Transfering)
			{
				QReceiverTransferFile::TransferQueueItem item=
					pModel->data(pModel->index(index.row(), NameTitle), Qt::UserRole).value<QReceiverTransferFile::TransferQueueItem>();

				if (item.type == File || item.type == HideFile)
				{
					quint64 nSize=pModel->data(pModel->index(index.row(), ProcessTitle), Qt::UserRole).toULongLong();

					progressBarOption.progress=(int)(nSize*10000.0/item.size);
					progressBarOption.text=QString::number(progressBarOption.progress/100.0, 'f', 2)+"%";
				}
				else
				{
					progressBarOption.text = tr("Transfering...");
				}
			}
			else if (nStatus == QTreeViewTransferQueue::Failed)
			{
				progressBarOption.text = tr("Failed");
			}

			painter->save();

			QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);

			painter->restore();

			return;
		}
	}
	
	QStyledItemDelegate::paint(painter, option, index);
}

QSize QTransferQueueDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QStyledItemDelegate::sizeHint(option, index);
}
