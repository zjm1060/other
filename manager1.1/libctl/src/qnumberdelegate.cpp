#include "qnumberdelegate.h"

#include <QApplication>

#include "common.h"
#include "qutildef.h"

QNumberDelegate::QNumberDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

QNumberDelegate::~QNumberDelegate()
{

}

void QNumberDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Q_ASSERT(index.isValid());

	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	QVariant value = index.data(Qt::DisplayRole);
	if (value.isValid() && !value.isNull())
	{
		if (value.type() == QVariant::Double)
		{
			opt.text = opt.locale.toString(value.toDouble(), 'f', 2);
		}
		else if (value.type() == QVariant::ULongLong)
		{
			quint64 nSize=value.toULongLong();

			if (nSize != INVALID_SIZE_MARK)
			{
				opt.text=fileSize(nSize);
			}
			else
			{
				opt.text="";
			}
		}
	}

	const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option);

	const QWidget *widget = (v3!=NULL? v3->widget:NULL);
	QStyle *style = widget ? widget->style() : QApplication::style();

	painter->save();

	style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

	painter->restore();
}
