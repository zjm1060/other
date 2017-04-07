#include "qfileicondelegate.h"

#include <QApplication>

#include "common.h"
#include "qutildef.h"

QFileIconDelegate::QFileIconDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

QFileIconDelegate::~QFileIconDelegate()
{

}

void QFileIconDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Q_ASSERT(index.isValid());

	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	QVariant value = index.data(Qt::UserRole);
	if (value.isValid() && !value.isNull())
	{
		FileInfo file=qvariant_cast<FileInfo>(value);
		QIcon icon=fileIcon(file);

		opt.icon=icon;
		opt.decorationAlignment=Qt::AlignRight|Qt::AlignVCenter;
		opt.features|=QStyleOptionViewItemV2::HasDecoration;
	}

	const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option);

	const QWidget *widget = (v3!=NULL? v3->widget:NULL);
	QStyle *style = widget ? widget->style() : QApplication::style();

	painter->save();

	style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

	painter->restore();
}

QSize QFileIconDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	int nHeight=style->pixelMetric(QStyle::PM_ListViewIconSize, 0, widget);

	QSize size=QStyledItemDelegate::sizeHint(option, index);
	if (size.height() < nHeight)
	{
		size.setHeight(nHeight+2);
	}

	return size;
}
