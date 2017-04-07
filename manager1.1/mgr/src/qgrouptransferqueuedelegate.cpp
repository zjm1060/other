#include "qgrouptransferqueuedelegate.h"

#include <QApplication>

#include "qimessage.h"
#include "common.h"
#include "qgrouptransferqueue.h"

QGroupTransferQueueDelegate::QGroupTransferQueueDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

QGroupTransferQueueDelegate::~QGroupTransferQueueDelegate()
{

}

void QGroupTransferQueueDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
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
			if (nStatus == QGroupTransferQueue::Waiting)
			{
				progressBarOption.text = tr("Waiting...");
			}
			else if (nStatus == QGroupTransferQueue::Transfering)
			{
				QGroupTransferQueue::TransferQueueItem item=
					pModel->data(pModel->index(index.row(), NameTitle), Qt::UserRole).value<QGroupTransferQueue::TransferQueueItem>();

				if (item.file.type == File || item.file.type == HideFile)
				{
					quint64 nSize=pModel->data(pModel->index(index.row(), ProcessTitle), Qt::UserRole).toULongLong();

					progressBarOption.progress=(int)(nSize*10000.0/item.file.size);
					progressBarOption.text=QString::number(progressBarOption.progress/100.0, 'f', 2)+"%";
				}
				else
				{
					progressBarOption.text = tr("Transfering...");
				}
			}
			else if (nStatus == QGroupTransferQueue::Failed)
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

QSize QGroupTransferQueueDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QStyledItemDelegate::sizeHint(option, index);
}
