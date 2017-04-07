#ifndef QTRANSFERQUEUEDELEGATE_H
#define QTRANSFERQUEUEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class QTransferQueueDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QTransferQueueDelegate(QObject* parent=0);
	~QTransferQueueDelegate();

public:
	enum TitleLabel
	{
		NameTitle,
		TypeTitle,
		ProcessTitle,
		TargetTitle
	};

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // QTRANSFERQUEUEDELEGATE_H
