#ifndef QINSPECTSETTINGDELEGATE_H
#define QINSPECTSETTINGDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class QInspectSettingDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QInspectSettingDelegate(QObject* parent=0);
	~QInspectSettingDelegate();

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // QINSPECTSETTINGDELEGATE_H
