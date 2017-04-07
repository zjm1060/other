#include "qgraphicsitemrouter.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

QGraphicsItemRouter::QGraphicsItemRouter(QAbstractGraphicsItem* parent)
	: QAbstractGraphicsItem(parent)
{
	m_common.pixmap[StatusOnline]=QPixmap(":/images/route.png");
	m_common.pixmap[StatusOffline]=QPixmap(":/images/route_quit.png");

	setOrigin(0, 0);
	setStatus(StatusOnline);
}

QGraphicsItemRouter::~QGraphicsItemRouter()
{

}

void QGraphicsItemRouter::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);

	painter->save();

	QRectF r=rect();

	painter->drawPixmap(r.left()+(r.width()-m_common.pixmap[m_common.status].width())/2, r.top(), m_common.pixmap[m_common.status]);

	if ((option->state & QStyle::State_Selected) != 0)
	{
		drawBounding(painter);
	}

	painter->restore();
}
