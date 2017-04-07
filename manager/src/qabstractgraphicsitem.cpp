#include "qabstractgraphicsitem.h"

#include <QPainter>
#include <qmath.h>

#include "common.h"
#include "qgraphicsitemlink.h"

#define MAX_NUM_COUNT	1

QAbstractGraphicsItem::QAbstractGraphicsItem(QGraphicsItem* parent)
	: QGraphicsItem(parent)
{
	setFlags(ItemIsSelectable);
	setAcceptsHoverEvents(true);

	m_common.font.setPointSize(10);
	setTickCount(TickZero);
}

QAbstractGraphicsItem::~QAbstractGraphicsItem()
{
	QMutableMapIterator<quint32, QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*> > it(m_mapItems);
	while (it.hasNext())
	{
		it.next();

		QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*>& item=it.value();
		SAFE_DELETE_CLASS(item.first);
		SAFE_DELETE_CLASS(item.second);
	}

	m_mapItems.clear();
}

void QAbstractGraphicsItem::setOrigin(const QPointF& origin)
{
	m_common.origin=origin;

	setPos(origin);
}

void QAbstractGraphicsItem::setOrigin(double x, double y)
{
	QPointF origin(x, y);

	setOrigin(origin);
}

QPointF QAbstractGraphicsItem::origin() const
{
	return m_common.origin;
}

void QAbstractGraphicsItem::setStatus(StatusType status)
{
	m_common.status=status;
}

void QAbstractGraphicsItem::setTickCount(TickType tick)
{
	switch (tick)
	{
	case TickZero:
		{
			m_common.tick=0;
		}
		break;
	case TickInc:
		{
			m_common.tick++;
		}
		break;
	case TickTop:
		{
			m_common.tick=MAX_NUM_COUNT+1;
		}
		break;
	default:
		qDebug("QAbstractGraphicsItem: Invalid tick type.");
		break;
	}

	if (m_common.tick == 0)
	{
		m_common.status=StatusOnline;
	}
	else if (m_common.tick > MAX_NUM_COUNT)
	{
		m_common.status=StatusOffline;
	}
}

void QAbstractGraphicsItem::setTickCountAll(TickType tick)
{
	setTickCount(tick);

	QMutableMapIterator<quint32, QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*> > it(m_mapItems);
	while (it.hasNext())
	{
		it.next();

		QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*>& item=it.value();
		item.first->setTickCountAll(tick);
	}
}

void QAbstractGraphicsItem::setTag(const QString& tag)
{
	m_common.tag=tag;
}

void QAbstractGraphicsItem::addItem(quint32 address, QAbstractGraphicsItem* item, QAbstractGraphicsItem* link)
{
	removeItem(address);

	m_mapItems.insert(address, QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*>(item, link));

	return;
}

QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*> QAbstractGraphicsItem::removeItem(quint32 address)
{
	QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*> pair;

	if (m_mapItems.contains(address))
	{
		pair=m_mapItems[address];

		m_mapItems.remove(address);
	}

	return pair;
}

QAbstractGraphicsItem* QAbstractGraphicsItem::item(quint32 address) const
{
	if (!m_mapItems.contains(address))
	{
		return NULL;
	}

	return m_mapItems[address].first;
}

QRectF QAbstractGraphicsItem::rect() const
{
	QFontMetrics metrics(m_common.font);
	int nWidth=metrics.width("255.255.255.255");
	nWidth=nWidth>64?nWidth:64;

	int nHeight=0;

	if (!m_common.pixmap[0].isNull())
	{
		nHeight=m_common.pixmap[0].height();
	}

	if (!m_common.tag.isEmpty())
	{
		QSize size=metrics.size(Qt::TextWordWrap, m_common.tag);
		nWidth=nWidth>size.width()? nWidth:size.width();

		nHeight+=size.height();
	}
	
	return QRectF(-nWidth/2, -nHeight/2, nWidth, nHeight);
}

QRectF QAbstractGraphicsItem::boundingRect() const
{
	QRectF r=rect();

	return r.adjusted(-2, -2, 2, 2);
}

QRectF QAbstractGraphicsItem::boundingVolume() const
{
	QRectF r=boundingRect();
	r.moveTo(origin());

	QRectF rec;

	QMapIterator<quint32, QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*> > it(m_mapItems);
	while (it.hasNext())
	{
		it.next();

		const QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*>& pair=it.value();

		if (pair.first != NULL)
		{
			rec=pair.first->boundingVolume();

			r|=rec;
		}
	}

	return r;
}

void QAbstractGraphicsItem::drawBounding(QPainter* painter)
{
	Q_ASSERT_NO_NULL(painter);

	painter->save();

	QPen pen(Qt::black);

	painter->setPen(pen);
	painter->setBrush(Qt::NoBrush);

	QRectF r=boundingRect();

	qreal dWidth=r.width()/4;
	qreal dHeight=r.height()/4;

	painter->drawLine(r.topLeft(), QPointF(r.left()+dWidth, r.top()));
	painter->drawLine(r.topLeft(), QPointF(r.left(), r.top()+dHeight));

	painter->drawLine(r.topRight(), QPointF(r.right()-dWidth, r.top()));
	painter->drawLine(r.topRight(), QPointF(r.right(), r.top()+dHeight));

	painter->drawLine(r.bottomLeft(), QPointF(r.left()+dWidth, r.bottom()));
	painter->drawLine(r.bottomLeft(), QPointF(r.left(), r.bottom()-dHeight));

	painter->drawLine(r.bottomRight(), QPointF(r.right()-dWidth, r.bottom()));
	painter->drawLine(r.bottomRight(), QPointF(r.right(), r.bottom()-dHeight));

	painter->restore();
}

void QAbstractGraphicsItem::translateItem(double dx, double dy)
{
	QPointF pt=origin();
	pt+=QPointF(dx, dy);

	setOrigin(pt);

	QMutableMapIterator<quint32, QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*> > it(m_mapItems);
	QAbstractGraphicsItem* pItem;
	while (it.hasNext())
	{
		it.next();

		pItem=it.value().first;
		if (pItem != NULL)
		{
			pItem->translateItem(dx, dy);
		}

		pItem=it.value().second;
		if (pItem != NULL)
		{
			pItem->translateItem(dx, dy);
		}
	}
}

double QAbstractGraphicsItem::adjust(double angle)
{
	QRectF r=boundingRect();
	double dRadius=qSqrt(r.width()*r.width()+r.height()*r.height())/2;

	if (m_mapItems.isEmpty())
	{
		return dRadius;
	}

	QList<quint32>	keys=m_mapItems.keys();

	QAbstractGraphicsItem* pItem;
	QGraphicsItemLink* pLink;

	int nNumItem=keys.size()>2? (keys.size()+1):3;
	double delta=2*MATH_PI/nNumItem;
	int nStep=qCeil(nNumItem/8.0);

	double dAngleStart=angle+MATH_PI+delta;

	QVector<QPair<QPointF, double> > circles;		//圆包围体
	QPair<QPointF, double> pair(origin(), dRadius);
	circles.append(pair);

	int nTimes=0;	//第几次遍历，由nStep决定

	double dr;
	double dAngle;
	while (nTimes < nStep)
	{
		for (int i=nTimes; i<keys.size(); i+=nStep)
		{
			pItem=m_mapItems[keys[i]].first;
			if (pItem == NULL)
			{
				continue;
			}

			dAngle=dAngleStart+delta*i;
			dr=pItem->adjust(dAngle);

			double dx=5.0*cos(dAngle);
			double dy=5.0*sin(dAngle);

			QPointF pt=origin();
			QPointF pos;
			for (int j=0; j<circles.size(); j++)
			{
				double dLen=circles[j].second+dr;
				pos=circles[j].first;
				while (qSqrt((pt.x()-pos.x())*(pt.x()-pos.x())+(pt.y()-pos.y())*(pt.y()-pos.y())) < dLen)
				{
					pt+=QPointF(dx, dy);
				}
			}

			circles.append(QPair<QPointF, double>(pt, dr));

			pos=pItem->origin();
			pItem->translateItem(pt.x()-pos.x(), pt.y()-pos.y());

			pt=origin();
			pos=pItem->origin();
			dr+=qSqrt((pos.x()-pt.x())*(pos.x()-pt.x())+(pos.y()-pt.y())*(pos.y()-pt.y()));
			dRadius=(dRadius > dr? dRadius:dr);

			pLink=qgraphicsitem_cast<QGraphicsItemLink*>(m_mapItems[keys[i]].second);
			if (pLink != NULL)
			{
				pLink->setLine(pt, pos);
			}
		}

		nTimes++;
	}

	return dRadius;
}

QAbstractGraphicsItem* QAbstractGraphicsItem::itemFromSegment(quint32 segment) const
{
	QMapIterator<quint32, QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*> > it(m_mapItems);

	QAbstractGraphicsItem* pItem=NULL;
	while (it.hasNext())
	{
		it.next();

		if ((it.key()&0xffffff00) == (segment%0xffffff00))
		{
			pItem=m_mapItems[it.key()].first;
			break;
		}
	}

	return pItem;
}

bool QAbstractGraphicsItem::contains(quint32 address) const
{
	return m_mapItems.contains(address);
}

QList<quint32> QAbstractGraphicsItem::items() const
{
	return m_mapItems.keys();
}

void QAbstractGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	if (!m_common.tag.isEmpty())
	{
		QRectF r=rect();
		painter->setFont(m_common.font);
		
		r.setTop(r.top()+m_common.pixmap[m_common.status].height());
		painter->drawText(r, Qt::AlignHCenter|Qt::AlignTop, m_common.tag);
	}
}

QAbstractGraphicsItem* QAbstractGraphicsItem::findServer(quint32 address)
{
	QAbstractGraphicsItem* pItem=NULL;

	QAbstractGraphicsItem* pTemp;
	if (type() == GraphicsItemAbstract || type() == GraphicsItemRouter)
	{
		QMapIterator<quint32, QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*> > it(m_mapItems);
		while (it.hasNext())
		{
			it.next();

			pTemp=it.value().first;
			if (pTemp->type() == GraphicsItemRouter)
			{
				pItem=pTemp->findServer(address);
			}
			else if (pTemp->type() == GraphicsItemSwitch)
			{
				if ((it.key()&0xffffff00) == (address&0xffffff00))
				{
					pItem=pTemp->item(address);
				}
			}

			if (pItem != NULL)
			{
				break;
			}
		}
	}

	return pItem;
}

void QAbstractGraphicsItem::closeEvent()
{
	QMapIterator<quint32, QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*> > it(m_mapItems);
	while (it.hasNext())
	{
		it.next();

		if (it.value().first != NULL)
		{
			it.value().first->closeEvent();
		}
	}
}

void QAbstractGraphicsItem::reconnect()
{
	QMapIterator<quint32, QPair<QAbstractGraphicsItem*, QAbstractGraphicsItem*> > it(m_mapItems);
	while (it.hasNext())
	{
		it.next();

		if (it.value().first != NULL)
		{
			it.value().first->reconnect();
		}
	}
}
