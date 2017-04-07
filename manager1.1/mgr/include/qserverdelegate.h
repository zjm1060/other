#ifndef QSERVERDELEGATE_H
#define QSERVERDELEGATE_H

#include <QStyledItemDelegate>
#include <QColor>
#include <QPainter>

class QServerDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QServerDelegate(QObject* parent=0);
	~QServerDelegate();

public:
	QRect toolIconRect(const QRect& rect, int index=0) const;

public:
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
	QIcon	m_iconButton;
};

#endif // QSERVERDELEGATE_H
