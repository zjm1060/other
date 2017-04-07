#ifndef QCOMPAREFILEDELEGATE_H
#define QCOMPAREFILEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class QCompareFileDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QCompareFileDelegate(bool inverse=false, QObject* parent=0);
	~QCompareFileDelegate();

	enum ColorRoleType
	{
		AppendBgColor,
		DeleteBgColor,
		ModifyBgColor,
		BlankBgColor,
		AssignBgColor,
		ModifyItemBgColor,
		ModifyItemBorderColor,
		VarifyItemBorderColor,
		ColorRoleEnd
	};

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
	QColor	m_color[ColorRoleEnd];
	bool	m_bInverse;
};

#endif // QCOMPAREFILEDELEGATE_H
