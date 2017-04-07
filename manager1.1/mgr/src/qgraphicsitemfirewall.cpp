#include "qgraphicsitemfirewall.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "common.h"

QString g_firwallPixmapKey[QGraphicsItemFireWall::IconTypeEnd]=
{
	":/images/firewall.png",
	":/images/firewall_quit.png"
};

QGraphicsItemFireWall::QGraphicsItemFireWall(quint64 uid, QAbstractGraphicsItem* parent)
	: QAbstractGraphicsItem(uid, parent)
{
	setPos(0, 0);
	setStatus(StatusOnline);
}

QGraphicsItemFireWall::~QGraphicsItemFireWall()
{

}

#ifdef Q_OS_SOLARIS
void QGraphicsItemFireWall::loadPixmap()
{
	for (int i=0; i<QGraphicsItemFireWall::IconTypeEnd; i++)
	{
		findPixmap(g_firwallPixmapKey[i]);
	}
}
#endif

QPixmap QGraphicsItemFireWall::pixmap(int type) const
{
	Q_ASSERT(type < IconTypeEnd);

	return findPixmap(g_firwallPixmapKey[type]);
}

QRectF QGraphicsItemFireWall::rect() const
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

QRectF QGraphicsItemFireWall::iconBoundingRect() const
{
	QRectF r=rect();

	int nHeight=0;
	QPixmap pix=pixmap(m_common.status);
	if (!pix.isNull())
	{
		nHeight=pix.height();
	}

	return QRectF(r.left(), r.top(), r.width(), nHeight);
}

void QGraphicsItemFireWall::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();

	QRectF r=rect();

	QPixmap pix=pixmap(m_common.status);
	painter->drawPixmap(r.left()+(r.width()-pix.width())/2, r.top(), pix);

	QAbstractGraphicsItem::paint(painter, option, widget);

	painter->restore();
}
