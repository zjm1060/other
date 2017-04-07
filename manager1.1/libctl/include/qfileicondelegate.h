#ifndef QFILEICONDELEGATE_H
#define QFILEICONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

#include "libctlglobal.h"

class CTL_EXPORT QFileIconDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QFileIconDelegate(QObject* parent=0);
	~QFileIconDelegate();

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // QFILEICONDELEGATE_H
