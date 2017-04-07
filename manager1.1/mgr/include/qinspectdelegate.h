#ifndef QINSPECTDELEGATE_H
#define QINSPECTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class QInspectDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QInspectDelegate(QObject* parent=0);
	~QInspectDelegate();

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
	QString textWrapped(const QString& text, int width, const QFontMetrics& metrics) const;
};

#endif // QINSPECTDELEGATE_H
