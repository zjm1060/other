#include "qgraphicsitemlink.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

QGraphicsItemLink::QGraphicsItemLink(QAbstractGraphicsItem* parent)
	: QAbstractGraphicsItem(parent),
	m_pen(Qt::yellow)
{
	setStatus(StatusOnline);

	setFlag(ItemIsSelectable, false);

	setOrigin(0, 0);
	setZValue(-1);

	m_pen.setWidth(2);
}

QGraphicsItemLink::~QGraphicsItemLink()
{

}

void QGraphicsItemLink::setLine(double x1, double y1, double x2, double y2)
{
	QPointF p1(x1, y1);
	QPointF p2(x2, y2);

	setLine(p1, p2);
}

void QGraphicsItemLink::setLine(const QPointF& p1, const QPointF& p2)
{
	m_p1=p1;
	m_p2=p2;
}

void QGraphicsItemLink::translateItem(double dx, double dy)
{
	QPointF pt(dx, dy);

	m_p1+=pt;
	m_p2+=pt;
}

QPainterPath QGraphicsItemLink::shape() const
{
	const qreal penWidthZero = qreal(0.00000001);

	QPainterPath path;
	if (m_p1 == m_p2)
		return path;

	path.moveTo(m_p1);
	path.lineTo(m_p2);

	if (path == QPainterPath())
		return path;
	QPainterPathStroker ps;
	ps.setCapStyle(m_pen.capStyle());
	if (m_pen.widthF() <= 0.0)
		ps.setWidth(penWidthZero);
	else
		ps.setWidth(m_pen.widthF());
	ps.setJoinStyle(m_pen.joinStyle());
	ps.setMiterLimit(m_pen.miterLimit());
	QPainterPath p = ps.createStroke(path);
	p.addPath(path);

	return p;
}

QRectF QGraphicsItemLink::boundingRect() const
{
	return shape().controlPointRect();
}

void QGraphicsItemLink::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);
	Q_UNUSED(option)

	painter->save();

	painter->setPen(m_pen);
	painter->drawLine(m_p1, m_p2);

	painter->restore();
}
