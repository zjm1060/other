#ifndef QGRAPHICSITEMSWITCH_H
#define QGRAPHICSITEMSWITCH_H

#include "qabstractgraphicsitem.h"

class QGraphicsItemSwitch : public QAbstractGraphicsItem
{
public:
	QGraphicsItemSwitch(quint64 uid=GRAPH_TOPO_INVALID_NODE, QAbstractGraphicsItem* parent=0);
	~QGraphicsItemSwitch();

public:
	enum {Type=GraphicsItemSwitch};

	enum IconType
	{
		IconOnline,
		IconOffline,
		IconTypeEnd
	};

public:
	virtual int type() const	{ return Type; }

#ifdef Q_OS_SOLARIS
public:
	static void loadPixmap();
#endif

public:
	virtual QRectF rect() const;
	virtual QRectF iconBoundingRect() const;

	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget=0);

public:
	QPixmap pixmap(int type) const;
};

#endif // QGRAPHICSITEMSWITCH_H
