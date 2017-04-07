#include "qgraphicsitemhub.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "common.h"

QString g_hubPixmapKey[QGraphicsItemHub::IconTypeEnd]=
{
	":/images/hub.png",
	":/images/hub_quit.png"
};

QGraphicsItemHub::QGraphicsItemHub(quint64 uid, QAbstractGraphicsItem* parent)
	: QAbstractGraphicsItem(uid, parent)
{
	setPos(0, 0);
	setStatus(StatusOnline);
}

QGraphicsItemHub::~QGraphicsItemHub()
{

}

#ifdef Q_OS_SOLARIS
void QGraphicsItemHub::loadPixmap()
{
	for (int i=0; i<QGraphicsItemHub::IconTypeEnd; i++)
	{
		findPixmap(g_hubPixmapKey[i]);
	}
}
#endif

QPixmap QGraphicsItemHub::pixmap(int type) const
{
	Q_ASSERT(type < IconTypeEnd);

	return findPixmap(g_hubPixmapKey[type]);
}

QRectF QGraphicsItemHub::rect() const
{
	Q_ASSERT(m_common.status < StatusEnd);

	QRectF r=QAbstractGraphicsItem::rect();

	int nWidth=0, nHeight=0;
	QPixmap pix=pixmap(m_common.status);
	if (!pix.isNull())
	{
		nWidth=pix.width();
		nHeight=pix.height();
	}

	nWidth=(nWidth>r.width()? nWidth-r.width():0);

	r.adjust(-nWidth/2, -nHeight/2, nWidth/2, nHeight/2);

	return r;
}

void QGraphicsItemHub::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();

	QRectF r=rect();

	QPixmap pix=pixmap(m_common.status);
	painter->drawPixmap(r.left()+(r.width()-pix.width())/2, r.top(), pix);

	QAbstractGraphicsItem::paint(painter, option, widget);

	painter->restore();
}
