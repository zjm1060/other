#ifndef QGRAPHICSITEMROUTER_H
#define QGRAPHICSITEMROUTER_H

#include "qabstractgraphicsitem.h"

class QGraphicsItemRouter : public QAbstractGraphicsItem
{
public:
	QGraphicsItemRouter(QAbstractGraphicsItem* parent=0);
	~QGraphicsItemRouter();

public:
	enum {Type=GraphicsItemRouter};

public:
	virtual int type() const	{ return Type; }

public:
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget=0);
};

#endif // QGRAPHICSITEMROUTER_H
