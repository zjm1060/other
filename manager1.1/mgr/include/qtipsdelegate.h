#ifndef QTIPSDELEGATE_H
#define QTIPSDELEGATE_H

#include <QItemDelegate>
#include <QColor>
#include <QPixmap>
#include <QPainter>

#include "qreceivertransferfile.h"

class QTipsDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	QTipsDelegate(QObject* parent=0);
	~QTipsDelegate();

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
	QColor	m_color[QReceiverTransferFile::TypeEnd];
	QPixmap	m_pixmap[QReceiverTransferFile::TypeEnd];

	int m_nSize;
};

#endif // QTIPSDELEGATE_H
