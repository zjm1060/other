#ifndef QDISKDELEGATE_H
#define QDISKDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

#include "libctlglobal.h"

class CTL_EXPORT QDiskDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QDiskDelegate(QObject* parent=0);
	~QDiskDelegate();

public:
	enum LogicalDiskTitle
	{
		NameTitle,
		FileSystemTitle
	};

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // QDISKDELEGATE_H
