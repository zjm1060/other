#include "qdiskdelegate.h"

#include <QApplication>

#include "common.h"
#include "qutildef.h"

QDiskDelegate::QDiskDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

QDiskDelegate::~QDiskDelegate()
{

}

void QDiskDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QAbstractItemModel* pModel=index.model();
	LogicalDisk disk=pModel->data(pModel->index(index.row(), NameTitle), Qt::UserRole).value<LogicalDisk>();

	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	painter->save();

	opt.text="";
	style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

	if (index.column() == NameTitle)
	{
		int nMetric=style->pixelMetric(QStyle::PM_IconViewIconSize, 0, widget);

		QIcon icon=style->standardIcon((disk.driveType==5? QStyle::SP_DriveDVDIcon:QStyle::SP_DriveHDIcon), &option, widget);

		QRect rect=option.rect;
		rect.adjust(2, 0, 0, 0);

		style->drawItemPixmap(painter, rect, Qt::AlignLeft|Qt::AlignVCenter, icon.pixmap(nMetric, nMetric, QIcon::Normal, QIcon::On));

		rect.adjust(nMetric+6, 5, 0, 0);
		QString strDesc=pModel->data(pModel->index(index.row(), NameTitle)).toString();
	
#ifndef Q_OS_WIN
		if ((opt.state & QStyle::State_Selected) != 0)
		{
			opt.palette.setColor(QPalette::Text, opt.palette.color(QPalette::Active, QPalette::HighlightedText));
		}
#endif
		
		style->drawItemText(painter, rect, Qt::AlignLeft|Qt::AlignTop, opt.palette, true, strDesc, QPalette::Text);

		if (disk.size != INVALID_SIZE_MARK)
		{
			int nFontHeight=opt.fontMetrics.height();

			QStyleOptionProgressBar progressBarOption;
			rect.adjust(0, nFontHeight+5, -10, -5);

			progressBarOption.rect = rect;
			progressBarOption.minimum = 0;
			progressBarOption.maximum = 10000;
			progressBarOption.progress = 0;
			progressBarOption.textVisible = true;
			progressBarOption.textAlignment=Qt::AlignCenter;

			progressBarOption.progress=(int)((disk.size-disk.freeSpace)*10000.0/disk.size);
			progressBarOption.text=QString::number(progressBarOption.progress/100.0, 'f', 2)+"%";

			style->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
		}
	}
	else if (index.column() == FileSystemTitle)
	{
		QRect rect=option.rect;
		rect.adjust(2, 5, 0, 0);
		QString strDesc=disk.driveTypeDesc+"   "+disk.fileSystem;

#ifndef Q_OS_WIN
		if ((opt.state & QStyle::State_Selected) != 0)
		{
			opt.palette.setColor(QPalette::Text, opt.palette.color(QPalette::Active, QPalette::HighlightedText));
		}
#endif
		style->drawItemText(painter, rect, Qt::AlignLeft|Qt::AlignTop, opt.palette, true, strDesc, QPalette::Text);

		if (disk.size != INVALID_SIZE_MARK)
		{
			int nFontHeight=opt.fontMetrics.height();
			rect.adjust(0, nFontHeight+1, 0, -1);
			strDesc=sizeToString(disk.freeSpace)+tr(" available,")+tr("a total of ")+sizeToString(disk.size);
			opt.palette.setColor(QPalette::Text, QColor(96, 96, 96));
			style->drawItemText(painter, rect, Qt::AlignLeft|Qt::AlignVCenter, opt.palette, true, strDesc, QPalette::Text);
		}
	}

	painter->restore();
}

QSize QDiskDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Q_UNUSED(index);

	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	int nHeight=style->pixelMetric(QStyle::PM_IconViewIconSize, 0, widget);
	nHeight=nHeight>48? nHeight:48;

	return QSize(option.rect.width(), nHeight);
}
