#include "qcomparefiledelegate.h"

#include <QApplication>

#include "common.h"
#include "qabstractfilecompare.h"

QCompareFileDelegate::QCompareFileDelegate(bool inverse/* =false */, QObject* parent/* =0 */)
	: QStyledItemDelegate(parent)
	,m_bInverse(inverse)
{
	m_color[AppendBgColor]=QColor(255, 255, 170);
	m_color[DeleteBgColor]=QColor(220, 220, 255);
	m_color[BlankBgColor]=QColor(212, 212, 212);
	m_color[ModifyBgColor]=QColor(255, 200, 100);
	m_color[AssignBgColor]=QColor(255, 0, 0);
	m_color[ModifyItemBgColor]=QColor(221, 235, 255);
	m_color[ModifyItemBorderColor]=QColor(125, 162, 206);
	m_color[VarifyItemBorderColor]=QColor(255, 0, 0);
}

QCompareFileDelegate::~QCompareFileDelegate()
{

}

void QCompareFileDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
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

	int nItemRole=QAbstractFileCompare::CommonItemRole;
	int nRole=pModel->data(pModel->index(index.row(), 0, index.parent()), QAbstractFileCompare::RowRole).toInt();

	if (nRole == QAbstractFileCompare::AppendRowRole)
	{
		painter->save();

		painter->fillRect(opt.rect, QBrush(m_color[AppendBgColor]));

		painter->restore();
	}
	else if (nRole == QAbstractFileCompare::DeleteRowRole)
	{
		painter->save();
		painter->fillRect(opt.rect, QBrush(m_color[DeleteBgColor]));
		painter->restore();

		opt.font.setStrikeOut(!m_bInverse);
	}
	else if (nRole == QAbstractFileCompare::BlankRowRole)
	{
		painter->save();

		painter->fillRect(opt.rect, QBrush(m_color[BlankBgColor]));

		painter->restore();
	}
	else if (nRole == QAbstractFileCompare::ModifyRowRole)
	{
		nItemRole=pModel->data(index, QAbstractFileCompare::ItemRole).toInt();
		if (nItemRole == QAbstractFileCompare::ModifyItemRole || nItemRole == QAbstractFileCompare::DeleteItemRole)
		{
			painter->save();

			painter->fillRect(opt.rect, QBrush(m_color[ModifyItemBgColor]));

			painter->restore();
		}
		else
		{
			painter->save();

			painter->fillRect(opt.rect, QBrush(m_color[ModifyBgColor]));

			painter->restore();
		}

		opt.font.setStrikeOut(m_bInverse? nItemRole == QAbstractFileCompare::ModifyItemRole:nItemRole == QAbstractFileCompare::DeleteItemRole);
	}

	opt.state&=~(QStyle::State_HasFocus|QStyle::State_MouseOver|QStyle::State_Selected);

	QVariant var=pModel->data(index, QAbstractFileCompare::AssignItemRole);
	if (var.isValid() && var.toBool())
	{
		opt.palette.setColor(QPalette::Text, m_color[AssignBgColor]);
		opt.font.setStrikeOut(false);

		if (opt.text.isEmpty())
		{
			opt.text="--";
			opt.displayAlignment=Qt::AlignCenter;
		}
	}
	
	painter->save();
	style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
	painter->restore();

	var=pModel->data(pModel->index(index.row(), 0, index.parent()), QAbstractFileCompare::SelectRowRole);
	if (var.isValid())
	{
		nRole=var.toInt();
		if ((nRole&QAbstractFileCompare::SelectBeginRowRole) != 0)
		{
			painter->save();

			QPen pen(m_color[ModifyItemBorderColor]);
			pen.setWidth(2);
			painter->setPen(pen);
			painter->drawLine(opt.rect.left(), opt.rect.top()+1, opt.rect.right(), opt.rect.top()+1);

			painter->restore();
		}
		
		if ((nRole&QAbstractFileCompare::SelectEndRowRole) != 0)
		{
			painter->save();

			QPen pen(m_color[ModifyItemBorderColor]);
			pen.setWidth(2);
			painter->setPen(pen);
			painter->drawLine(opt.rect.left(), opt.rect.bottom()-1, opt.rect.right(), opt.rect.bottom()-1);

			painter->restore();
		}
	}

	var=pModel->data(index, QAbstractFileCompare::SelectItemRole);
	if (var.isValid() && var.toBool())
	{
		painter->save();

		QPen pen(m_color[ModifyItemBorderColor]);
		pen.setWidth(2);
		painter->setPen(pen);
		painter->drawRect(opt.rect.adjusted(1, 1, -1, -1));

		painter->restore();
	}

	var=pModel->data(index, QAbstractFileCompare::VerifyItemRole);
	if (var.isValid() && var.toBool())
	{
		painter->save();

		QPen pen(m_color[VarifyItemBorderColor]);
		pen.setWidth(2);
		pen.setStyle(Qt::DotLine);
		painter->setPen(pen);
		painter->drawRect(opt.rect.adjusted(1, 1, -1, -1));

		painter->restore();
	}
}

QSize QCompareFileDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QStyledItemDelegate::sizeHint(option, index);
}
