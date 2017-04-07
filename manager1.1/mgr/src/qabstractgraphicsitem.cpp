#include "qabstractgraphicsitem.h"

#include <QPainter>
#include <qmath.h>
#include <QStyleOptionGraphicsItem>

#include "common.h"

#define MAX_NUM_COUNT	3

QAbstractGraphicsItem::QAbstractGraphicsItem(quint64 uid, QGraphicsItem* parent)
	: QGraphicsItem(parent)
{
	m_common.brush=QBrush(QColor(212, 212, 212, 128));
	m_common.pen=QPen(Qt::NoPen);
	m_common.opacity=1.0;

	m_common.fontPen=QPen(Qt::white);

	m_common.uid=uid;

	setFlags(ItemIsSelectable);
	setAcceptsHoverEvents(true);

	m_common.font.setPointSize(10);
	setTickCount(TickZero);
}

QAbstractGraphicsItem::~QAbstractGraphicsItem()
{
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
}

void QAbstractGraphicsItem::setTag(const QString& tag)
{
	m_common.tag=tag;
}

quint64 QAbstractGraphicsItem::identifier()
{
	return m_common.uid;
}

QRectF QAbstractGraphicsItem::rect() const
{
	QFontMetrics metrics(m_common.font);
	int nWidth=metrics.width("255.255.255.255");
	nWidth=nWidth>64?nWidth:64;

	int nHeight=0;

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

	return r.adjusted(-3, -3, 3, 3);
}

QRectF QAbstractGraphicsItem::boundingRectToScene() const
{
	QRectF r=boundingRect();

	return mapRectToScene(r);
}

QRectF QAbstractGraphicsItem::iconBoundingRect() const
{
	return boundingRect();
}

QRectF QAbstractGraphicsItem::iconBoundingRectToScene() const
{
	return mapRectToScene(iconBoundingRect());
}

void QAbstractGraphicsItem::drawBounding(QPainter* painter)
{
	Q_ASSERT_NO_NULL(painter);

	painter->save();

	QRectF r=boundingRect();

	painter->setOpacity(m_common.opacity);
	painter->setPen(m_common.pen);
	painter->setBrush(m_common.brush);

	painter->drawRoundRect(r, 10, 10);

	painter->restore();
}

void QAbstractGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);

	if (!m_common.tag.isEmpty())
	{
		painter->save();

		QRectF r=rect();
		painter->setFont(m_common.font);
		painter->setPen(m_common.fontPen);

		QFontMetrics metrics(m_common.font);
		QSize size=metrics.size(Qt::TextWordWrap, m_common.tag);
		
		r.setTop(r.bottom()-size.height());
		painter->drawText(r, Qt::AlignHCenter|Qt::AlignTop, m_common.tag);

		painter->restore();
	}

	if ((option->state & QStyle::State_Selected) != 0 || (option->state & QStyle::State_MouseOver) != 0)
	{
		drawBounding(painter);
	}
}
