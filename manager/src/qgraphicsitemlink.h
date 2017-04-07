#ifndef QGRAPHICSITEMLINK_H
#define QGRAPHICSITEMLINK_H

#include <QPen>

#include "qabstractgraphicsitem.h"

class QGraphicsItemLink : public QAbstractGraphicsItem
{
public:
	QGraphicsItemLink(QAbstractGraphicsItem* parent=0);
	~QGraphicsItemLink();

public:
	enum {Type=GraphicsItemLink};

public:
	virtual int type() const	{ return Type; }

public:
	void setLine(double x1, double y1, double x2, double y2);
	void setLine(const QPointF& p1, const QPointF& p2);

public:
	virtual void translateItem(double dx, double dy);

public:
	virtual QRectF boundingRect() const;
	QPainterPath shape() const;

public:
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget=0);

private:
	QPointF m_p1, m_p2;

	QPen m_pen;
};

#endif // QGRAPHICSITEMLINK_H
