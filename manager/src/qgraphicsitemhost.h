#ifndef QGRAPHICSITEMHOST_H
#define QGRAPHICSITEMHOST_H

#include "qabstractgraphicsitem.h"
#include "qwndmanager.h"

class QGraphicsItemHost :public QObject, public QAbstractGraphicsItem
{
	Q_OBJECT

public:
	QGraphicsItemHost(QAbstractGraphicsItem* parent=0);
	~QGraphicsItemHost();

public:
	enum {Type=GraphicsItemHost};

	enum IconType
	{
		IconConnectable,
		IconTypeEnd
	};

	enum UserDataType
	{
		DataHostName,
		DataIPAddress,
		DataPort
	};

public:
	void setFlag(IconType flag, int status);
	int flag(IconType type);

public:
	virtual int type() const	{ return Type; }

	virtual void closeEvent();
	virtual void reconnect();
public:
	virtual QRectF rect() const;

	virtual void setTickCount(TickType tick);

	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget=0);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

private slots:
	void wndClosed(QWidget* widget);

private:
	QPixmap m_pixmap[IconTypeEnd];
	int m_nFlags[IconTypeEnd];		//״̬��־λ

	QWndManager*	m_pWndManager;
};

#endif // QGRAPHICSITEMHOST_H
