#ifndef QTIPSDELEGATE_H
#define QTIPSDELEGATE_H

#include <QItemDelegate>
#include <QColor>
#include <QPixmap>
#include <QPainter>

class QTipsDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	QTipsDelegate(QObject* parent=0);
	~QTipsDelegate();

	enum TipType
	{
		Information=0,
		Warning,
		Error,
		Download,
		Upload,
		Success,
		Unknown,
		TypeEnd
	};

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
	QColor	m_color[TypeEnd];
	QPixmap	m_pixmap[TypeEnd];

	int m_nSize;
};

#endif // QTIPSDELEGATE_H
