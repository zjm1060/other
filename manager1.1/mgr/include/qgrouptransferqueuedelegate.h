#ifndef QGROUPTRANSFERQUEUEDELEGATE_H
#define QGROUPTRANSFERQUEUEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class QGroupTransferQueueDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QGroupTransferQueueDelegate(QObject* parent=0);
	~QGroupTransferQueueDelegate();

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

#endif // QGROUPTRANSFERQUEUEDELEGATE_H
