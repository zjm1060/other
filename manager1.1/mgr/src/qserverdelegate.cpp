#include "qserverdelegate.h"

#include <QPixmap>
#include <QApplication>

#include "common.h"
#include "qservermanager.h"
#include "qlistviewfilereceiver.h"

static const int g_nSaveIconSize=16;

QServerDelegate::QServerDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
	m_iconButton=QIcon(":/images/save.png");
}

QServerDelegate::~QServerDelegate()
{

}

void QServerDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	painter->save();
	opt.displayAlignment=(Qt::AlignTop|Qt::AlignHCenter);

	style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

//	opt.rect.setHeight(opt.rect.height()-g_nSaveIconSize-6);

	if (index.data(QListViewFileReceiver::ServerSelectRole).toBool())
	{
		QStyleOptionButton styleOption;
		styleOption.features=QStyleOptionButton::DefaultButton;
		styleOption.state|=QStyle::State_Enabled;
		styleOption.rect=toolIconRect(option.rect);
		styleOption.icon=m_iconButton;
		styleOption.iconSize=QSize(g_nSaveIconSize, g_nSaveIconSize);

		style->drawControl(QStyle::CE_PushButton, &styleOption, painter);
	}

	painter->restore();
}

QSize QServerDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QSize size=QStyledItemDelegate::sizeHint(option, index);

	int nHeight=size.height()+g_nSaveIconSize+6;

	return QSize(size.width(), nHeight);
}

QRect QServerDelegate::toolIconRect(const QRect& rect, int index) const
{
	Q_UNUSED(index);
	return QRect(rect.left()+3, rect.bottom()-g_nSaveIconSize-3, g_nSaveIconSize, g_nSaveIconSize);
}
