#ifndef QGRAPHICSITEMHOST_H
#define QGRAPHICSITEMHOST_H

#include "qabstractgraphicsitem.h"
#include "qwndmanager.h"

class QGraphicsItemHost :public QObject, public QAbstractGraphicsItem
{
	Q_OBJECT

public:
	QGraphicsItemHost(quint64 uid=GRAPH_TOPO_INVALID_NODE, QAbstractGraphicsItem* parent=0);
	~QGraphicsItemHost();

public:
	enum {Type=GraphicsItemHost};

	enum IconType
	{
		IconOnline,
		IconOffline,
		IconConnectable,
		IconDisconnected,
		IconTypeEnd
	};

	enum FunctionFlag	//功能标志
	{
		Connectable,
		FunctionEnd
	};

public:
	void setFlag(int func, bool flag);
	bool flag(int func) const;

	void setServerInfo(const ServerInfo& info);
	const ServerInfo& serverInfo();

#ifdef Q_OS_SOLARIS
public:
	static void loadPixmap();
#endif

public:
	virtual int type() const	{ return Type; }

	virtual void closeWindow();
	virtual void reconnect();

public:
	virtual QRectF rect() const;
	virtual QRectF iconBoundingRect() const;

	virtual void setTickCount(TickType tick);

	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget=0);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

private slots:
	void wndClosed(QWidget* widget);

private:
	QPixmap pixmap(int type) const;

private:
	bool m_bFlags[FunctionEnd];		//功能状态标志位

	QWndManager*	m_pWndManager;

	ServerInfo	m_info;		//服务器信息
};

#endif // QGRAPHICSITEMHOST_H
