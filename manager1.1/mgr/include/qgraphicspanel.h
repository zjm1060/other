#ifndef QGRAPHICSPANEL_H
#define QGRAPHICSPANEL_H

#include <QObject>
#include <QBrush>
#include <QPainter>
#include <QPixmap>
#include <QMap>

class QAbstractGrahicsPanelItem : public QObject
{
	Q_OBJECT

public:
	QAbstractGrahicsPanelItem(const QRect& rect, const QBrush& brush, QObject* parent);
	~QAbstractGrahicsPanelItem();

public:
	virtual void paint(QPainter* painter);

public:
	void setOpacity(qreal opacity);

	void setBorderPen(const QPen& pen);
	const QPen& borderPen() const;

public:
	void setData(int key, const QVariant& value);
	QVariant data(int key) const;

public:
	void move(int x, int y);

	const QRect& geometry() const;
	virtual void setGeometry(const QRect& rect);

protected:
	QRect	m_rect;

	QBrush	m_brush;
	QPen	m_pen;

	qreal	m_dOpacity;

	QMap<int, QVariant>	m_mapUserData;
};

class QGraphicsPanelItem : public QAbstractGrahicsPanelItem
{
	Q_OBJECT

public:
	QGraphicsPanelItem(const QBrush& brush, QObject *parent=0);
	~QGraphicsPanelItem();

public:
	virtual void paint(QPainter* painter);

public:
	void setPixmap(const QPixmap& pixmap);
	bool setFocus(const QPoint& pt);

public:
	virtual void setGeometry(const QRect& rect);

private:
	QPixmap	m_pixmap;

	bool	m_bFacusIn;
};

#define GRAPHICS_PANEL_ROW_COUNT	3
#define GRAPHICS_PANEL_COLUMN_COUNT	4
#define GRAPHICS_PANEL_HOVER_OFF	-1
class QGraphicsPanel : public QAbstractGrahicsPanelItem
{
	Q_OBJECT

public:
	QGraphicsPanel(const QRect& rect, const QBrush& brush, QObject *parent);
	~QGraphicsPanel();

public:
	void paint(QPainter* painter);

public:
	void setItem(int row, int column, QGraphicsPanelItem* item);
	QGraphicsPanelItem* item(int row, int column);

public:
	bool mouseMoveEvent(const QPoint& pt);
	bool mouseDoubleClickEvent(const QPoint& pt);

signals:
	void doubleClick(QGraphicsPanelItem* item);
	void hoverOn(QGraphicsPanelItem* item);

private:
	bool	m_bFocusIn;

	QGraphicsPanelItem*		m_pGrid[GRAPHICS_PANEL_ROW_COUNT][GRAPHICS_PANEL_COLUMN_COUNT];
};

#endif // QGRAPHICSPANEL_H
