#ifndef QSYNCFILEDELEGATE_H
#define QSYNCFILEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class QSyncFileDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QSyncFileDelegate(QObject* parent=0);
	~QSyncFileDelegate();

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // QSYNCFILEDELEGATE_H
