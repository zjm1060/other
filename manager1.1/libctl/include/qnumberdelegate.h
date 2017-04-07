#ifndef QNUMBEREDELEGATE_H
#define QNUMBEREDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

#include "libctlglobal.h"

class CTL_EXPORT QNumberDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QNumberDelegate(QObject* parent=0);
	~QNumberDelegate();

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // QNUMBEREDELEGATE_H
