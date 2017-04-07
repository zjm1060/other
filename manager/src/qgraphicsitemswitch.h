#ifndef QGRAPHICSITEMSWITCH_H
#define QGRAPHICSITEMSWITCH_H

#include "qabstractgraphicsitem.h"

class QGraphicsItemSwitch : public QAbstractGraphicsItem
{
public:
	QGraphicsItemSwitch(QAbstractGraphicsItem* parent=0);
	~QGraphicsItemSwitch();

public:
	enum {Type=GraphicsItemSwitch};

public:
	virtual int type() const	{ return Type; }

public:
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget=0);
};

#endif // QGRAPHICSITEMSWITCH_H
