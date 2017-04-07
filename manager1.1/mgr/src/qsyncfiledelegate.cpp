#include "qsyncfiledelegate.h"

#include <QApplication>

#include "common.h"
#include "qimessage.h"
#include "qtreeviewsyncfile.h"

QSyncFileDelegate::QSyncFileDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

QSyncFileDelegate::~QSyncFileDelegate()
{

}

void QSyncFileDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QColor color(Qt::black);
	const QAbstractItemModel* pModel=index.model();

	DiffFile file=
		pModel->data(pModel->index(index.row(), QTreeViewSyncFile::NameTitle), QTreeViewSyncFile::UserData).value<DiffFile>();
	switch (file.status)
	{
	case QTreeViewSyncFile::Modified:
		{
			color=QColor(60, 90, 200);
		}
		break;
	case QTreeViewSyncFile::Added:
		{
			color=QColor(128, 60, 180);
		}
		break;
	case QTreeViewSyncFile::Deleted:
		{
			color=QColor(100, 0, 0);
		}
		break;
	default:
		break;
	}

	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	opt.palette.setColor(QPalette::Text, color);

	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	if (index.column() == QTreeViewSyncFile::NameTitle)
	{
		painter->save();

		int nCheckWidth=style->pixelMetric(QStyle::PM_ButtonIconSize, 0, widget);

 		opt.decorationAlignment=Qt::AlignRight|Qt::AlignVCenter;
 		opt.decorationSize+=QSize(nCheckWidth, 0);
 		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		//»æÖÆcheckbox
		bool bChecked=pModel->data(pModel->index(index.row(), QTreeViewSyncFile::NameTitle), QTreeViewSyncFile::UserChecked).toBool();

		QStyleOptionButton checkButtonOption;

		checkButtonOption.rect=QRect(opt.rect.left()+4, opt.rect.top(), nCheckWidth, opt.rect.height());

		bool bCheckable=pModel->data(pModel->index(index.row(), QTreeViewSyncFile::NameTitle), QTreeViewSyncFile::UserCheckable).toBool();
		checkButtonOption.state=opt.state;
		if (bCheckable)
		{
			checkButtonOption.state |=  QStyle::State_Enabled;
		}
		else
		{
			checkButtonOption.state &=  ~QStyle::State_Enabled;
		}
		
		checkButtonOption.state |= bChecked? QStyle::State_On : QStyle::State_Off;

		style->drawControl(QStyle::CE_CheckBox, &checkButtonOption, painter, widget);

		painter->restore();
		return;
	}
	else if (index.column() == QTreeViewSyncFile::SizeTitle)
	{
		initStyleOption(&opt, index);

		opt.text=fileSize(file.size);

		painter->save();

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		painter->restore();

		return;
	}
	else if (index.column() == QTreeViewSyncFile::StatusTitle)
	{
		initStyleOption(&opt, index);

		switch (file.status)
		{
		case QTreeViewSyncFile::Unmodified:
			opt.text=tr("unmodified");
			break;
		case QTreeViewSyncFile::Modified:
			opt.text=tr("modified");
			break;
		case QTreeViewSyncFile::Added:
			opt.text=tr("added");
			break;
		case QTreeViewSyncFile::Deleted:
			opt.text=tr("deleted");
			break;
		case QTreeViewSyncFile::NonVersioned:
			opt.text=tr("non-versioned");
			break;
		default:
			break;
		}

		painter->save();

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		painter->restore();

		return;
	}
	else if (index.column() == QTreeViewSyncFile::ProcessTitle)
	{
		painter->save();

		opt.text="";
		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		if (pModel != NULL)
		{
			int nStatus=pModel->data(pModel->index(index.row(), QTreeViewSyncFile::NameTitle), QTreeViewSyncFile::UserTransferStatus).toInt();
			if (nStatus != QTreeViewSyncFile::InvalidStatus)
			{
				QStyleOptionProgressBar progressBarOption;
				progressBarOption.rect = option.rect.adjusted(3, 3, -3, -3);
				progressBarOption.minimum = 0;
				progressBarOption.maximum = 10000;
				progressBarOption.progress = 0;
				progressBarOption.textVisible = true;
				progressBarOption.textAlignment=Qt::AlignCenter;

				if (nStatus == QTreeViewSyncFile::Waiting)
				{
					progressBarOption.text = tr("Waiting...");
				}
				else if (nStatus == QTreeViewSyncFile::Transfering)
				{
	 				if (file.type == File || file.type == HideFile)
	 				{
	 					quint64 nSize=pModel->data(pModel->index(index.row(), QTreeViewSyncFile::ProcessTitle)).toULongLong();
	 
	 					progressBarOption.progress=(int)(nSize*10000.0/file.size);
	 					progressBarOption.text=QString::number(progressBarOption.progress/100.0, 'f', 2)+"%";
	 				}
	 				else
	 				{
	 					progressBarOption.text = tr("Transfering...");
	 				}
				}
				else if (nStatus == QTreeViewSyncFile::Failed)
				{
					progressBarOption.text = tr("Failed");
				}
				else
				{
					progressBarOption.progress=progressBarOption.maximum;
					progressBarOption.text = tr("Finished");
				}

				style->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
			}

			painter->restore();

			return;
		}
	}
	
	QStyledItemDelegate::paint(painter, opt, index);
}

QSize QSyncFileDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QStyledItemDelegate::sizeHint(option, index);
}
