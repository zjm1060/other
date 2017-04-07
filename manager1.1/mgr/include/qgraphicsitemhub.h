#ifndef QGRAPHICSITEMHUB_H
#define QGRAPHICSITEMHUB_H

#include "qabstractgraphicsitem.h"

class QGraphicsItemHub : public QAbstractGraphicsItem
{
public:
	QGraphicsItemHub(quint64 uid=GRAPH_TOPO_INVALID_NODE, QAbstractGraphicsItem* parent=0);
	~QGraphicsItemHub();

public:
	enum {Type=GraphicsItemHub};

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
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget=0);

public:
	QPixmap pixmap(int type) const;
};

#endif // QGRAPHICSITEMHUB_H
