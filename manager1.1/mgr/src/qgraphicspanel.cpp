#include "qgraphicspanel.h"

#include "common.h"

QAbstractGrahicsPanelItem::QAbstractGrahicsPanelItem(const QRect& rect, const QBrush& brush, QObject* parent)
:QObject(parent)
,m_rect(rect)
,m_brush(brush)
,m_pen(Qt::NoPen)
,m_dOpacity(1.0)
{

}

QAbstractGrahicsPanelItem::~QAbstractGrahicsPanelItem()
{

}

void QAbstractGrahicsPanelItem::setOpacity(qreal opacity)
{
	m_dOpacity=opacity;
}

void QAbstractGrahicsPanelItem::setBorderPen(const QPen& pen)
{
	m_pen=pen;
}

const QPen& QAbstractGrahicsPanelItem::borderPen() const
{
	return m_pen;
}

void QAbstractGrahicsPanelItem::move(int x, int y)
{
	m_rect.moveTo(x, y);
}

const QRect& QAbstractGrahicsPanelItem::geometry() const
{
	return m_rect;
}

void QAbstractGrahicsPanelItem::setGeometry(const QRect& rect)
{
	m_rect=rect;
}

void QAbstractGrahicsPanelItem::setData(int key, const QVariant& value)
{
	m_mapUserData[key]=value;
}

QVariant QAbstractGrahicsPanelItem::data(int key) const
{
	if (!m_mapUserData.contains(key))
	{
		return QVariant();
	}

	return m_mapUserData[key];
}

void QAbstractGrahicsPanelItem::paint(QPainter* painter)
{
	painter->save();

	painter->setOpacity(m_dOpacity);
	painter->setPen(m_pen);
	painter->setBrush(m_brush);

	painter->drawRoundRect(m_rect, 10, 10);

	painter->restore();
}

#define PANEL_GRID_SPACING	10
QGraphicsPanelItem::QGraphicsPanelItem(const QBrush& brush, QObject *parent)
: QAbstractGrahicsPanelItem(QRect(0, 0, 10, 10), brush, parent)
,m_bFacusIn(false)
{
	
}

QGraphicsPanelItem::~QGraphicsPanelItem()
{

}

void QGraphicsPanelItem::paint(QPainter* painter)
{
	if (m_bFacusIn)
	{
		QAbstractGrahicsPanelItem::paint(painter);
	}

	painter->save();

	painter->drawPixmap(m_rect, m_pixmap);

	painter->restore();
}

void QGraphicsPanelItem::setPixmap(const QPixmap& pixmap)
{
	m_pixmap=pixmap;
}

void QGraphicsPanelItem::setGeometry(const QRect& rect)
{
	QRect r=rect;
	r.adjust(PANEL_GRID_SPACING, PANEL_GRID_SPACING, -PANEL_GRID_SPACING, -PANEL_GRID_SPACING);

	QAbstractGrahicsPanelItem::setGeometry(r);
}

bool QGraphicsPanelItem::setFocus(const QPoint& pt)
{
	m_bFacusIn=m_rect.contains(pt)? true:false;

	return m_bFacusIn;
}

QGraphicsPanel::QGraphicsPanel(const QRect& rect, const QBrush& brush, QObject *parent)
	: QAbstractGrahicsPanelItem(rect, brush, parent)
	,m_bFocusIn(false)
{
	memset(m_pGrid, 0, GRAPHICS_PANEL_ROW_COUNT*GRAPHICS_PANEL_COLUMN_COUNT*sizeof(QGraphicsPanelItem*));

	QPen pen=QPen(QColor(128, 0, 128));
	pen.setWidth(2);

	setBorderPen(pen);
}

QGraphicsPanel::~QGraphicsPanel()
{
	for (int i=0; i<GRAPHICS_PANEL_ROW_COUNT; i++)
	{
		for (int j=0; j<GRAPHICS_PANEL_COLUMN_COUNT; j++)
		{
			SAFE_DELETE_CLASS(m_pGrid[i][j]);
		}
	}
}

void QGraphicsPanel::paint(QPainter* painter)
{
	QAbstractGrahicsPanelItem::paint(painter);

	QRect rect=geometry();
	painter->save();

	qreal dWidth=(qreal)rect.width()/GRAPHICS_PANEL_COLUMN_COUNT;
	qreal dHeight=(qreal)rect.height()/GRAPHICS_PANEL_ROW_COUNT;

	for (int i=0; i<GRAPHICS_PANEL_ROW_COUNT; i++)
	{
		for (int j=0; j<GRAPHICS_PANEL_COLUMN_COUNT; j++)
		{
			if (m_pGrid[i][j] != NULL)
			{
				m_pGrid[i][j]->setGeometry(QRect(rect.left()+(int)(j*dWidth), rect.top()+(int)(i*dHeight), (int)dWidth, (int)dHeight));
				m_pGrid[i][j]->paint(painter);
			}
		}
	}

	painter->restore();
}

void QGraphicsPanel::setItem(int row, int column, QGraphicsPanelItem* item)
{
	Q_ASSERT(row>=0 && row<GRAPHICS_PANEL_ROW_COUNT && column >=0 && column<GRAPHICS_PANEL_COLUMN_COUNT);

	if (m_pGrid[row][column] != NULL)
	{
		SAFE_DELETE_CLASS(m_pGrid[row][column]);
	}

	m_pGrid[row][column]=item;
}

QGraphicsPanelItem* QGraphicsPanel::item(int row, int column)
{
	Q_ASSERT(row>=GRAPHICS_PANEL_HOVER_OFF && row<GRAPHICS_PANEL_ROW_COUNT && column >=GRAPHICS_PANEL_HOVER_OFF && column<GRAPHICS_PANEL_COLUMN_COUNT);

	if (row == GRAPHICS_PANEL_HOVER_OFF || column == GRAPHICS_PANEL_HOVER_OFF)
	{
		return NULL;
	}

	return m_pGrid[row][column];
}

bool QGraphicsPanel::mouseMoveEvent(const QPoint& pt)
{
	if (!m_bFocusIn && !m_rect.contains(pt))
	{
		return false;
	}

	m_bFocusIn=false;
	bool bHoverOn;
	for (int i=0; i<GRAPHICS_PANEL_ROW_COUNT; i++)
	{
		for (int j=0; j<GRAPHICS_PANEL_COLUMN_COUNT; j++)
		{
			if (m_pGrid[i][j] != NULL)
			{
				bHoverOn=m_pGrid[i][j]->setFocus(pt);
				m_bFocusIn|=bHoverOn;

				if (bHoverOn)
				{
					emit hoverOn(item(i, j));
				}
			}
		}
	}

	if (!m_bFocusIn)
	{
		emit hoverOn(item(GRAPHICS_PANEL_HOVER_OFF, GRAPHICS_PANEL_HOVER_OFF));
	}

	return true;
}

bool QGraphicsPanel::mouseDoubleClickEvent(const QPoint& pt)
{
	if (!m_rect.contains(pt))
	{
		return false;
	}

	for (int i=0; i<GRAPHICS_PANEL_ROW_COUNT; i++)
	{
		for (int j=0; j<GRAPHICS_PANEL_COLUMN_COUNT; j++)
		{
			if (m_pGrid[i][j] != NULL)
			{
				if (m_pGrid[i][j]->setFocus(pt))
				{
					emit doubleClick(item(i, j));
				}
			}
		}
	}

	return true;
}
