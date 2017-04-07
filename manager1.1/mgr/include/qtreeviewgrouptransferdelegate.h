#ifndef QTREEVIEWGROUPTRANSFERDELEGATE_H
#define QTREEVIEWGROUPTRANSFERDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

#include "qtreeviewgrouptransferqueue.h"

class QTreeViewGroupTransferDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QTreeViewGroupTransferDelegate(QObject* parent=0);
	~QTreeViewGroupTransferDelegate();

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
	QIcon	m_statusIcon[QTreeViewGroupTransferQueue::StatusEnd];
};

#endif // QTREEVIEWGROUPTRANSFERDELEGATE_H
