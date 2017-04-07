#include "qinspectsettingdelegate.h"

#include <QApplication>

#include "common.h"
#include "qtreeviewinspectsetting.h"

QInspectSettingDelegate::QInspectSettingDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

QInspectSettingDelegate::~QInspectSettingDelegate()
{

}

void QInspectSettingDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QAbstractItemModel* pModel=index.model();

	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	int nRole=pModel->data(pModel->index(index.row(), QTreeViewInspectSetting::NameTitle, index.parent()), QTreeViewInspectSetting::ItemRole).toInt();

	if (nRole == QTreeViewInspectSetting::ChildItem)
	{
		QColor color(121, 121, 121);

		opt.palette.setColor(QPalette::Text, color);
	}

	if (index.column() == QTreeViewInspectSetting::NameTitle)
	{
		painter->save();

		QModelIndex cindex=pModel->index(index.row(), QTreeViewInspectSetting::NameTitle, index.parent());
		int nChecked=pModel->data(cindex, QTreeViewInspectSetting::CheckedRole).toInt();

		opt.features|=QStyleOptionViewItemV2::HasCheckIndicator;
		opt.checkState=(Qt::CheckState)nChecked;

		opt.state |=  QStyle::State_Enabled;

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		if (nRole == QTreeViewInspectSetting::TitleItem)
		{
			QRect rect=opt.rect;
			QString strDesc;

			int nWidth=opt.decorationSize.width()+style->pixelMetric(QStyle::PM_ButtonIconSize, 0, widget);
			nWidth+=opt.fontMetrics.width(opt.text);

			int nStatus=pModel->data(cindex, QTreeViewInspectSetting::StatusRole).toInt();
			switch (nStatus)
			{
			case QTreeViewInspectSetting::Connecting:
				{
					opt.palette.setColor(QPalette::Text, QColor(121, 121, 121));
					strDesc="[ "+tr("Connecting...")+" ]";
				}
				break;
			case QTreeViewInspectSetting::Unconnected:
				{
					opt.palette.setColor(QPalette::Text, QColor(100, 0, 0));
					strDesc="[ "+tr("Unconnected")+" ]";
				}
				break;
			case QTreeViewInspectSetting::DataLoading:
				{
					opt.palette.setColor(QPalette::Text, QColor(121, 121, 121));
					strDesc="[ "+tr("Loading...")+" ]";
				}
				break;
			case QTreeViewInspectSetting::DataLoaded:
				{
					opt.palette.setColor(QPalette::Text, QColor(60, 90, 200));
					strDesc="[ "+tr("Loaded")+" ]";
				}
				break;
			case QTreeViewInspectSetting::H9000NotRunning:
				{
					opt.palette.setColor(QPalette::Text, QColor(100, 0, 0));
					strDesc="[ "+tr("H9000 is not running")+" ]";
				}
				break;
			default:
				break;
			}

			rect.adjust(nWidth, 0, 0, 0);

			style->drawItemText(painter, rect, Qt::AlignLeft|Qt::AlignTop, opt.palette, true, strDesc, QPalette::Text);
		}

		painter->restore();
		return;
	}
	
	QStyledItemDelegate::paint(painter, opt, index);
}

QSize QInspectSettingDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
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
