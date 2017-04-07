#ifndef QGRAPHICSPANELHAWKEYES_H
#define QGRAPHICSPANELHAWKEYES_H

#include <QObject>
#include <QBrush>
#include <QPainter>
#include <QPixmap>
#include <QVector>
#include <QMutex>

#include "qgraphicspanel.h"

class QGraphicsPanelHawkEyes : public QAbstractGrahicsPanelItem
{
	Q_OBJECT

public:
	QGraphicsPanelHawkEyes(const QRect& rect, const QBrush& brush, QObject *parent);
	~QGraphicsPanelHawkEyes();

public:
	void paint(QRectF rect, QPainter* painter);

	void setPoints(const QVector<QPointF>& points);

public:
	bool mouseMoveEvent(const QPoint& pt);
	bool mouseDoubleClickEvent(const QPoint& pt);

signals:
	void doubleClick(QPoint pt);
	void hoverOn(QPoint pt);

private:
	bool	m_bFocusIn;

	QMutex	m_mutex;
	QVector<QPointF>	m_points;
};

#endif // QGRAPHICSPANELHAWKEYES_H
