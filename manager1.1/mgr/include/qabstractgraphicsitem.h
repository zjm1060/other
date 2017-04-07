#ifndef QABSTRACTGRAPHICSITEM_H
#define QABSTRACTGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QFont>
#include <QBrush>
#include <QPen>

#include "qutildef.h"

enum GraphicsItemType
{
	GraphicsItemAbstract=QGraphicsItem::UserType,
	GraphicsItemRouter,
	GraphicsItemSwitch,
	GraphicsItemHub,
	GraphicsItemHost,
	GraphicsItemFireWall,
	GraphicsItemLink,
	GraphicsItemEnd
};

class QAbstractGraphicsItem : public QGraphicsItem
{
public:
	QAbstractGraphicsItem(quint64 uid=GRAPH_TOPO_INVALID_NODE, QGraphicsItem* parent=0);
	~QAbstractGraphicsItem();

public:
	enum StatusType
	{
		StatusOnline,
		StatusOffline,
		StatusEnd
	};

	enum TickType
	{
		TickZero,
		TickInc,
		TickTop
	};

	typedef struct _common
	{
		//选中状态
		QBrush	brush;
		QPen	pen;
		qreal	opacity;

		//字体
		QFont font;
		QPen fontPen;
		QString	tag;

		int	tick;				//设备未发现次数

		quint64		uid;		//item代表的网络节点

		StatusType	status;
	}Common;

	enum {Type=GraphicsItemAbstract};

public:
	virtual int type() const	{ return Type; }

public:
	void setStatus(StatusType status);
	virtual void setTickCount(TickType tick);
	void setTickCountAll(TickType tick);

	void setTag(const QString& tag);

	quint64 identifier();

public:
	virtual QRectF rect() const;
	virtual QRectF boundingRect() const;
	virtual QRectF iconBoundingRect() const;

	virtual QRectF boundingRectToScene() const;		//在Scene中item的包围体
	virtual QRectF iconBoundingRectToScene() const;

	void drawBounding(QPainter* painter);

public:
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget=0);

protected:
	Common		m_common;
};

#endif // QABSTRACTGRAPHICSITEM_H
