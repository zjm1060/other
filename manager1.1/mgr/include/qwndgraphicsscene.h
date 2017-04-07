#ifndef QWNDGRAPHICSSCENE_H
#define QWNDGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QMap>
#include <QRunnable>

#include "qgraphicspanel.h"
#include "qgraphicspromptpanel.h"
#include "qgraphicspanelhawkeyes.h"
#include "qabstractgraphicsitem.h"
#include "qgraphicsitemlink.h"
#include "qutildef.h"

#include "qwndmanager.h"
#include "qwndgroupmanager.h"
#include "qmainwndprivilege.h"

class QWndGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	QWndGraphicsScene(QObject *parent=0);
	~QWndGraphicsScene();

public slots:
	void hoverOn(QGraphicsPanelItem* item);
	void mouseDoubleClick(QGraphicsPanelItem* item);

	void mouseDoubleClickHawk(QPoint pt);

public slots:
 	void wndClosed(QWidget* widget);

public:
	void logged();

	void clear();

	void setLocalServer(const ServerInfo& info, const QPoint& pt);
	void setRemoteServerList(const QList<ServerInfo>& servers);
	void setTopologyGraph(const GraphTopo& graph);

	void closeEvent();

signals:
	void logout();
	void centerOn(QPointF pt);

protected:
	virtual void drawForeground(QPainter* painter, const QRectF& rect);

	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);

private:
	bool mouseMove(const QPointF& pt);

	QRectF addGraphNodeToScene(const GraphTopoNode& node, int ifIndex, const QPointF& origin, double angle);	//pt=返回值，表示图元中心所在位置
	void translateGraphNode(const GraphTopoNode& node, int ifIndex, double dx, double dy);
	void linkGraphNode(const GraphTopoNode& node, int ifIndex);

private:
	bool canConnected(const ServerInfo& info, Server& server) const;

	void refreshRemoteServerList();
	void refreshGroupManagerServerList(bool bRunning=true);

	QGraphicsPanelItem* findPanelItem(int type);

private slots:
	void modifyPasswordSuccess(const QString usrName, const PrivilegeUser user);
	void modifyPasswordUserNonexistent(const QString usrName);

private:
	enum ToolType
	{
		ToolWorkgroup,
		ToolLocalHost,
		ToolConfigure,
		ToolPrivilege,
		ToolLogout,
		ToolEnd
	};

	enum UsrDataType
	{
		UsrDataToolType,
		UsrDataServerRunning,
		UsrDataServerInfo,
		UsrDataEnd
	};

private:
	QReceiverPrivilege*	m_pReceiver;

	QGraphicsPanel*			m_pToolPanel;
	QGraphicsPromptPanel*	m_pPromptPanel;
	QGraphicsPanelHawkEyes*	m_pHawkPanel;

	quint16		m_nPort;
	QWndManager*		m_pWndManager;
	QWndGroupManager*	m_pWndGroupManager;
	QMainWndPrivilege*	m_pWndPrivilege;

	bool	m_bFocusIn;

	GraphTopo	m_graph;
	QMap<quint64, QPair<QAbstractGraphicsItem*, int> >	m_items;
	QMap<QPair<quint64, quint64>, QGraphicsItemLink*>	m_linkItems;

	QHash<QString, QPair<ServerInfo, int> >	m_remoteServers;		//远程服务器列表
};

#endif // QWNDGRAPHICSSCENE_H
