#include "qgraphicsitemswitch.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

QGraphicsItemSwitch::QGraphicsItemSwitch(QAbstractGraphicsItem* parent)
	: QAbstractGraphicsItem(parent)
{
	m_common.pixmap[StatusOnline]=QPixmap(":/images/switch.png");
	m_common.pixmap[StatusOffline]=QPixmap(":/images/switch_quit.png");

	setOrigin(0, 0);
	setStatus(StatusOnline);
}

QGraphicsItemSwitch::~QGraphicsItemSwitch()
{

}

void QGraphicsItemSwitch::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();

	QRectF r=rect();

	painter->drawPixmap(r.left()+(r.width()-m_common.pixmap[m_common.status].width())/2, r.top(), m_common.pixmap[m_common.status]);

	QAbstractGraphicsItem::paint(painter, option, widget);

	if ((option->state & QStyle::State_Selected) != 0)
	{
		drawBounding(painter);
	}

	painter->restore();
}
