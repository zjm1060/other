#include "qgraphicspanelhawkeyes.h"

#include "common.h"

QGraphicsPanelHawkEyes::QGraphicsPanelHawkEyes(const QRect& rect, const QBrush& brush, QObject *parent)
	: QAbstractGrahicsPanelItem(rect, brush, parent)
	,m_bFocusIn(false)
{
	QPen pen=QPen(QColor(128, 0, 128));
	pen.setWidth(2);

	setBorderPen(pen);
}

QGraphicsPanelHawkEyes::~QGraphicsPanelHawkEyes()
{
	
}

void QGraphicsPanelHawkEyes::setPoints(const QVector<QPointF>& points)
{
	QMutexLocker locker(&m_mutex);
	m_points=points;
}

void QGraphicsPanelHawkEyes::paint(QRectF rect, QPainter* painter)
{
	QAbstractGrahicsPanelItem::paint(painter);

	painter->save();

	QPen pen(QColor(0, 255, 0));
	pen.setWidth(3);

	painter->setPen(pen);

	for (int i=0; i<m_points.size(); i++)
	{
		QPointF& pt=m_points[i];

		double dx=(pt.x()-rect.left())*m_rect.width()/rect.width()+m_rect.left();
		double dy=(pt.y()-rect.top())*m_rect.height()/rect.height()+m_rect.top();

		painter->drawPoint(QPointF(dx, dy));
	}

	painter->restore();
}

bool QGraphicsPanelHawkEyes::mouseMoveEvent(const QPoint& pt)
{
	if (!m_bFocusIn && !m_rect.contains(pt))
	{
		return false;
	}

	if (!m_bFocusIn)
	{
		emit hoverOn(pt);
	}

	return true;
}

bool QGraphicsPanelHawkEyes::mouseDoubleClickEvent(const QPoint& pt)
{
	if (!m_rect.contains(pt))
	{
		return false;
	}

	emit doubleClick(pt);

	return true;
}
