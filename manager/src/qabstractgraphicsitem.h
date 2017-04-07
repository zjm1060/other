#ifndef QABSTRACTGRAPHICSITEM_H
#define QABSTRACTGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QFont>
#include <QMap>

enum GraphicsItemType
{
	GraphicsItemAbstract=QGraphicsItem::UserType,
	GraphicsItemRouter,
	GraphicsItemSwitch,
	GraphicsItemHub,
	GraphicsItemHost,
	GraphicsItemLink,
	GraphicsItemEnd
};

class QAbstractGraphicsItem : public QGraphicsItem
{
public:
	QAbstractGraphicsItem(QGraphicsItem* parent=0);
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
		QPointF	origin;			//坐标原点
		QRectF	bounding;		//包围体

		QFont font;
		QString	tag;

		int	tick;				//设备未发现次数

		StatusType	status;
		QPixmap	pixmap[StatusEnd];
	}Common;

	enum {Type=GraphicsItemAbstract};

public:
	virtual int type() const	{ return Type; }

public:
	QAbstractGraphicsItem* findServer(quint32 address);		//根据ip，获取主机

	virtual void closeEvent();
	virtual void reconnect();

public:
	void setOrigin(const QPointF& origin);
	void setOrigin(double x, double y);
	QPointF origin() const;

	void setStatus(StatusType status);
	virtual void setTickCount(TickType tick);
	void setTickCountAll(TickType tick);

	void setTag(const QString& tag);

public:
	void addItem(quint32 address, QAbstractGraphicsItem* item, QAbstractGraphicsItem* link=NULL);
	QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*> removeItem(quint32 address);
	QAbstractGraphicsItem* item(quint32 address) const;
	QAbstractGraphicsItem* itemFromSegment(quint32 segment) const;
	QList<quint32>	items() const;

	bool contains(quint32 address) const;

public:
	virtual void translateItem(double dx, double dy);
	virtual double adjust(double angle=0.0);	//调整图元布局，返回包围圆半径

public:
	virtual QRectF rect() const;
	virtual QRectF boundingRect() const;

	virtual QRectF boundingVolume() const;		//包含所有item的包围体

	void drawBounding(QPainter* painter);

public:
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget=0);

protected:
	Common		m_common;

	QMap<quint32, QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*> >	m_mapItems;
};

#endif // QABSTRACTGRAPHICSITEM_H
