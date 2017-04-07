#ifndef QGRAPHICSITEMROUTER_H
#define QGRAPHICSITEMROUTER_H

#include "qabstractgraphicsitem.h"

class QGraphicsItemRouter : public QAbstractGraphicsItem
{
public:
	QGraphicsItemRouter(quint64 uid=GRAPH_TOPO_INVALID_NODE, QAbstractGraphicsItem* parent=0);
	~QGraphicsItemRouter();

public:
	enum {Type=GraphicsItemRouter};

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

#endif // QGRAPHICSITEMROUTER_H
