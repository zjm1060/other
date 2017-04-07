#include "qwndgraphicsscene.h"

#include <QGraphicsSceneMouseEvent>
#include <qmath.h>

#include "qndc.h"
#include "qlogger.h"
#include "qinfomanager.h"
#include "qservermanager.h"
#include "qgraphicsitemrouter.h"
#include "qgraphicsitemswitch.h"
#include "qgraphicsitemhub.h"
#include "qgraphicsitemhost.h"
#include "qgraphicsitemfirewall.h"
#include "qglobalinfo.h"
#include "qdlgmodifypassword.h"

#define GRAPHICS_SCENE_HAWK_EYES_PANEL_HEIGHT	210
#define GRAPHICS_SCENE_HAWK_EYES_PANEL_WIDTH	280

QWndGraphicsScene::QWndGraphicsScene(QObject *parent)
	: QGraphicsScene(parent)
	,m_pWndManager(NULL)
	,m_pWndGroupManager(NULL)
	,m_pWndPrivilege(NULL)
	,m_pHawkPanel(NULL)
	,m_bFocusIn(false)
{
	qRegisterMetaType<QMap<quint64, QPair<QAbstractGraphicsItem*, int> > >("QMap<quint64, QPair<QAbstractGraphicsItem*, int> >");
	qRegisterMetaType<QMap<QPair<quint64, quint64>, QGraphicsItemLink*> >("QMap<QPair<quint64, quint64>, QGraphicsItemLink*>");

	m_nPort=(quint16)parseConfig(getAppParentDir()+INI_FILE, "network/serverPort", 2300).toInt();

#ifdef Q_OS_SOLARIS
	QGraphicsItemRouter::loadPixmap();
	QGraphicsItemSwitch::loadPixmap();
	QGraphicsItemHub::loadPixmap();
	QGraphicsItemHost::loadPixmap();
	QGraphicsItemFireWall::loadPixmap();
#endif

	m_pReceiver=new QReceiverPrivilege;
	connect(m_pReceiver, SIGNAL(modifyPasswordSuccess(const QString, const PrivilegeUser)), 
		this, SLOT(modifyPasswordSuccess(const QString, const PrivilegeUser)));
	connect(m_pReceiver, SIGNAL(modifyPasswordUserNonexistent(const QString)), this, SLOT(modifyPasswordUserNonexistent(const QString)));

	m_pToolPanel=new QGraphicsPanel(QRect(0, 0, 276, 212), QBrush(QColor(0, 64, 128, 64)), this);

	QGraphicsPanelItem* pItem[ToolEnd];
	int nIndex=0;
	for (int i=0; i<ToolEnd; i++)
	{
		pItem[i]=new QGraphicsPanelItem(QBrush(QColor(212, 212, 212, 128)), this);

		switch (i)
		{
		case ToolWorkgroup:
			pItem[i]->setPixmap(findPixmap(":/images/workgroup.png"));
			break;
		case ToolLocalHost:
			{
				pItem[i]->setPixmap(findPixmap(":/images/serverquit.png"));
				pItem[i]->setData(UsrDataServerRunning, false);
			}
			break;
		case ToolConfigure:
			pItem[i]->setPixmap(findPixmap(":/images/configure.png"));
			break;
		case ToolPrivilege:
			break;
		case ToolLogout:
			pItem[i]->setPixmap(findPixmap(":/images/logout.png"));
			break;
		default:
			break;
		}

		pItem[i]->setData(UsrDataToolType, i);

		if (i != ToolLogout)
		{
			m_pToolPanel->setItem(nIndex/GRAPHICS_PANEL_COLUMN_COUNT, nIndex%GRAPHICS_PANEL_COLUMN_COUNT, pItem[i]);
			nIndex++;
		}
		else
		{
			m_pToolPanel->setItem(GRAPHICS_PANEL_ROW_COUNT-1, GRAPHICS_PANEL_COLUMN_COUNT-1, pItem[i]);
		}
	}

	m_pPromptPanel=new QGraphicsPromptPanel(0, 0, 276+40, QBrush(QColor(0, 64, 128, 128)), this);

	connect(m_pToolPanel, SIGNAL(hoverOn(QGraphicsPanelItem*)), this, SLOT(hoverOn(QGraphicsPanelItem*)));
	connect(m_pToolPanel, SIGNAL(doubleClick(QGraphicsPanelItem*)), this, SLOT(mouseDoubleClick(QGraphicsPanelItem*)));

	m_pHawkPanel=new QGraphicsPanelHawkEyes(QRect(0, 0, GRAPHICS_SCENE_HAWK_EYES_PANEL_WIDTH, GRAPHICS_SCENE_HAWK_EYES_PANEL_HEIGHT),
		QBrush(QColor(0, 64, 128, 128)), this);

	connect(m_pHawkPanel, SIGNAL(doubleClick(QPoint)), this, SLOT(mouseDoubleClickHawk(QPoint)));	
}

QWndGraphicsScene::~QWndGraphicsScene()
{
	SAFE_DELETE_CLASS(m_pReceiver);
}

void QWndGraphicsScene::modifyPasswordSuccess(const QString usrName, const PrivilegeUser user)
{
	QInfoManager::instance()->append(QInfoManager::Important, 
		tr("User <%1> password updated successfully.").arg(usrName));

	QGlobalInfo::instance()->setUser(user);
}

void QWndGraphicsScene::modifyPasswordUserNonexistent(const QString usrName)
{
	QInfoManager::instance()->append(QInfoManager::Error, 
		tr("Failed to modify the user <%1> password! The user does not exist!").arg(usrName));
}

QGraphicsPanelItem* QWndGraphicsScene::findPanelItem(int type)
{
	QGraphicsPanelItem* pItem=NULL;
	for (int row=0; row<GRAPHICS_PANEL_ROW_COUNT; row++)
	{
		for (int column=0; column<GRAPHICS_PANEL_COLUMN_COUNT; column++)
		{
			pItem=m_pToolPanel->item(row, column);
			if (pItem != NULL)
			{
				if (pItem->data(UsrDataToolType).toInt() == type)
				{
					return pItem;
				}
			}
		}
	}

	return NULL;
}

void QWndGraphicsScene::logged()
{
	if (QGlobalInfo::instance()->isLogin())
	{
		QGraphicsPanelItem* pItem=findPanelItem(ToolPrivilege);

		if (pItem != NULL)
		{
			QMap<int, quint32> privileges=QGlobalInfo::instance()->privileges();
			if (checkPrivileges(privileges, PrivilegeRole, AdministratorRole))
			{
				pItem->setPixmap(findPixmap(":/images/privileges.png"));
			}
			else
			{
				pItem->setPixmap(findPixmap(":/images/modifypwd.png"));
			}
		} 
	}
}

void QWndGraphicsScene::clear()
{
	m_items.clear();
	m_linkItems.clear();

	QGraphicsScene::clear();
}

void QWndGraphicsScene::drawForeground(QPainter* painter, const QRectF& rect)
{
	QGraphicsScene::drawForeground(painter, rect);

	if (QGlobalInfo::instance()->isLogin())
	{
		QRect r;
		if (m_pToolPanel!= NULL)
		{
			r=m_pToolPanel->geometry();
			m_pToolPanel->move((int)rect.right()-r.width()-3, (int)rect.top()+3);

			m_pToolPanel->paint(painter);
		}

		if (m_pPromptPanel != NULL)
		{
			QRect rec=m_pPromptPanel->geometry();
			m_pPromptPanel->move((int)rect.right()-rec.width()-3, (int)rect.top()+3+r.height()+10);

			m_pPromptPanel->paint(painter);
		}

		if (m_pHawkPanel != NULL)
		{
			QRect rec=m_pHawkPanel->geometry();
			m_pHawkPanel->move((int)rect.left()+3, (int)rect.bottom()-rec.height()-3);

			m_pHawkPanel->paint(sceneRect(), painter);
		}
	}

	QInfoManager::instance()->paint(painter, (int)rect.left()+30, (int)rect.bottom()-GRAPHICS_SCENE_HAWK_EYES_PANEL_HEIGHT-20);
}

void QWndGraphicsScene::mouseDoubleClickHawk(QPoint pt)
{
	if (m_pHawkPanel != NULL)
	{
		QRectF rect=sceneRect();

		QRect r=m_pHawkPanel->geometry();
		
		double dx=(pt.x()-r.left())*rect.width()/r.width()+rect.left();
		double dy=(pt.y()-r.top())*rect.height()/r.height()+rect.top();

		emit centerOn(QPointF(dx, dy));
	}
}

bool QWndGraphicsScene::mouseMove(const QPointF& pt)
{
	QGraphicsItem* item=(QGraphicsItem*)itemAt(pt);
	if (item == NULL)
	{
		m_pPromptPanel->setHtmlText("");

		if (m_bFocusIn)
		{
			m_bFocusIn=false;
			return true;
		}
		else
		{
			return false;
		}
	}

	QAbstractGraphicsItem* pItem=(QAbstractGraphicsItem*)item;
	if (pItem != NULL)
	{
		quint64 uid=pItem->identifier();
		if (uid != GRAPH_TOPO_INVALID_NODE && m_graph.nodes.contains(uid))
		{
			QString strHtmlText;

			const GraphTopoNode& node=m_graph.nodes[uid].first;
			switch (node.type)
			{
			case RouterNode:
			case SwitchL3Node:
			case SwitchNode:
				{
					if (node.type == RouterNode)
					{
						strHtmlText="<font color=#A0A0A0>"+tr("Router")+"</font>";
					}
					else if (node.type == SwitchL3Node)
					{
						strHtmlText="<font color=#A0A0A0>"+tr("Layer 3 Switch")+"</font>";
					}
					else if (node.type == SwitchNode)
					{
						strHtmlText="<font color=#A0A0A0>"+tr("Switch")+"</font>";
					}

					if (!node.name.isEmpty())
					{
						strHtmlText+="<br/><br/><font color=#A0A0A0>"+tr("Name:")+"</font>"
							+"<font color=#D4D400>	"+node.name+"</font>";
					}

					QString strImagePath=getAppParentDir()+"/ini/images/";

					strHtmlText+="<dl><font color=#A0A0A0>"+tr("Manage Address:")+"</font>";

					QMap<QString, QVector<QPair<int, quint32> > > lights;
					QMapIterator<int, GraphNodeInterface> it(node.interfaces);
					while (it.hasNext())
					{
						it.next();

						const GraphNodeInterface& face=it.value();

						if (face.addresses.size() > 0)
						{
							strHtmlText+="<dt><font color=#A0A0A0>-"+tr("MAC: ")+face.physAddress+"</font></dt>";

							for (int i=0; i<face.addresses.size(); i++)
							{
								strHtmlText+="<dd><font color=#A0A0A0>"+QString("- %1").arg(face.addresses[i])+"</font></dd>";
							}
						}

						if (face.type == 6)
						{
							QStringList split=face.desc.split(QChar('/'));
							if (!split.isEmpty())
							{
								lights[split.first()].append(QPair<int, quint32>(face.operStatus, face.speed));
							}
						}
					}
					strHtmlText+="</dl>";

					strHtmlText+="<dl><font color=#A0A0A0>"+tr("Interfaces:")+"</font>";
					QString strImage;
					QMapIterator<QString, QVector<QPair<int, quint32> > > iter(lights);
					while (iter.hasNext())
					{
						iter.next();

						QVector<QPair<int, quint32> > vecLights=iter.value();

						strHtmlText+="<dd><font color=#A0A0A0>"+QString("- %1").arg(iter.key())+"</font></dd>";

						int nIndex=0;
						while (nIndex < ((int)ceil(vecLights.size()/24.0))*2)
						{
							QString strInterfaces;
							if (nIndex != 0 && (nIndex%2)==0)
							{
								strInterfaces="<dt><font color=#A0A0A0>-------------------------</font></dt>";
							}

							strInterfaces+="<dt>";

							for (int i=(nIndex/2)*24+(nIndex%2); i<(nIndex/2+1)*24 && i<vecLights.size() ; i+=2)
							{
								if (vecLights[i].first == 1)
								{
									if (vecLights[i].second <= 10000000)
									{
										strImage="<img src="+strImagePath+"light_10M.png />";
									}
									else if (vecLights[i].second <= 100000000)
									{
										strImage="<img src="+strImagePath+"light_100M.png />";
									}
									else
									{
										strImage="<img src="+strImagePath+"light_1000M.png />";
									}
								}
								else
								{
									strImage="<img src="+strImagePath+"light_down.png />";
								}

								if ((i-(nIndex/2)*24-(nIndex%2))>0 && (i-(nIndex/2)*24-(nIndex%2))%12==0)
								{
									strInterfaces+=" ";
								}

								strInterfaces+=strImage;
							}

							strInterfaces+="</dt>";

							strHtmlText+=strInterfaces;

							nIndex++;
						}

						strHtmlText+="</dl>";
					}

					strHtmlText+="<font color=#606060>"+node.desc+"</font>";
				}
				break;
			case FirewallNode:
				{
					strHtmlText="<font color=#A0A0A0>"+tr("Firewall")+"</font>";
					
					if (!node.name.isEmpty())
					{
						strHtmlText+="<br/><br/><font color=#A0A0A0>"+tr("Name:")+"</font>"
							+"<font color=#D4D400>	"+node.name+"</font>";
					}

					strHtmlText+="<dl><font color=#A0A0A0>"+tr("Network:")+"</font>";
					QMapIterator<int, GraphNodeInterface> it(node.interfaces);
					while (it.hasNext())
					{
						it.next();

						const GraphNodeInterface& face=it.value();

						strHtmlText+="<dt><font color=#A0A0A0>-"+tr("MAC: ")+face.physAddress+"</font></dt>";

						for (int i=0; i<face.addresses.size(); i++)
						{
							strHtmlText+="<dd><font color=#A0A0A0>"+QString("- %1").arg(face.addresses[i])+"</font></dd>";
						}
					}
					strHtmlText+="</dl>";

					strHtmlText+="<font color=#606060>"+node.desc+"</font>";
				}
				break;
			case HubNode:
				{
					strHtmlText="<font color=#A0A0A0>"+tr("Hub")+"</font>"
						+"<br/><br/><font color=#606060>"+tr("This is a non-managed switch.")+"</font>";
				}
				break;
			case HostNode:
				{
					strHtmlText="<font color=#A0A0A0>"+tr("Host")+"</font>";

					if (((QGraphicsItemHost*)pItem)->flag(QGraphicsItemHost::Connectable))
					{
						const ServerInfo& info=((QGraphicsItemHost*)pItem)->serverInfo();

						if (!info.hostName.isEmpty())
						{
							strHtmlText+="<br/><br/><font color=#A0A0A0>"+tr("Host Name:")+"</font>"
								+"<font color=#D4D400>	"+info.hostName+"</font>";
						}

						if (!info.usrName.isEmpty())
						{
							strHtmlText+="<br/><font color=#A0A0A0>"+tr("User Name:")+"</font>"
								+"<font color=#D4D400>	"+info.usrName+"</font>";
						}

						if (info.count > 0)
						{
							strHtmlText+="<dl><font color=#A0A0A0>"+tr("Network:")+"</font>";
							QMapIterator<QString, QVector<Server> > it(info.servers);
							while (it.hasNext())
							{
								it.next();

								strHtmlText+="<dt><font color=#A0A0A0>-"+tr("MAC: ")+it.key()+"</font></dt>";

								const QVector<Server>& servers=it.value();
								foreach (Server server, servers)
								{
									strHtmlText+="<dd><font color=#A0A0A0>"+QString("- %1:%2").arg(server.addr).arg(server.port)+"</font></dd>";
								}
							}
							strHtmlText+="</dl>";
						}
						else
						{
							strHtmlText+="<br/><br/>";
						}
					}
					else
					{
						if (!node.name.isEmpty())
						{
							strHtmlText+="<br/><br/><font color=#A0A0A0>"+tr("Host Name:")+"</font>"
								+"<font color=#D4D400>	"+node.name+"</font>";
						}

						strHtmlText+="<dl><font color=#A0A0A0>"+tr("Network:")+"</font>";
						QMapIterator<int, GraphNodeInterface> it(node.interfaces);
						while (it.hasNext())
						{
							it.next();

							const GraphNodeInterface& face=it.value();

							strHtmlText+="<dt><font color=#A0A0A0>-"+tr("MAC: ")+face.physAddress+"</font></dt>";

							for (int i=0; i<face.addresses.size(); i++)
							{
								strHtmlText+="<dd><font color=#A0A0A0>"+QString("- %1").arg(face.addresses[i])+"</font></dd>";
							}
						}
						strHtmlText+="</dl>";
					}

					strHtmlText+="<font color=#606060>"+tr("To check host configuration information and operational status.")+"</font>";
				}
				break;
			default:
				return false;
			}

			m_bFocusIn=true;
			m_pPromptPanel->setHtmlText(strHtmlText);

			return true;
		}
		else
		{
			m_pPromptPanel->setHtmlText("");

			if (m_bFocusIn)
			{
				m_bFocusIn=false;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

void QWndGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	if (QGlobalInfo::instance()->isLogin())
	{
		if (m_pToolPanel->mouseMoveEvent(mouseEvent->scenePos().toPoint()))
		{
			invalidate();

			return;
		}

		if (mouseMove(mouseEvent->scenePos()))
		{
			invalidate();
		}

		if (m_pHawkPanel->mouseMoveEvent(mouseEvent->scenePos().toPoint()))
		{
			invalidate();
		}
	}

	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void QWndGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	if (QGlobalInfo::instance()->isLogin())
	{
		if (m_pToolPanel->mouseDoubleClickEvent(mouseEvent->scenePos().toPoint()))
		{
			return;
		}

		if (m_pHawkPanel->mouseDoubleClickEvent(mouseEvent->scenePos().toPoint()))
		{
			return;
		}
	}

	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void QWndGraphicsScene::hoverOn(QGraphicsPanelItem* item)
{
	int nIndex=(item==NULL? -1:item->data(UsrDataToolType).toInt());

	QString strHtmlText;

	switch (nIndex)
	{
	case ToolWorkgroup:
		{
			strHtmlText="<font color=#A0A0A0>"+tr("Workgroup")+"</font>"
				+"<br/><br/><font color=#606060>"+tr("To achieve one-to-many communication.")+"</font>";
		}
		break;
	case ToolLocalHost:
		{
			strHtmlText="<font color=#A0A0A0>"+tr("Local Host")+"</font>";

			ServerInfo info;
			info.count=0;
			if (item != NULL)
			{
				if (item->data(UsrDataServerRunning).toBool())
				{
					QVariant var=item->data(UsrDataServerInfo);
					if (var.isValid())
					{
						info=var.value<ServerInfo>();
					}
				}
			}

			if (!info.hostName.isEmpty())
			{
				strHtmlText+="<br/><br/><font color=#A0A0A0>"+tr("Host Name:")+"</font>"
					+"<font color=#D4D400>	"+info.hostName+"</font>";
			}

			if (!info.usrName.isEmpty())
			{
				strHtmlText+="<br/><font color=#A0A0A0>"+tr("User Name:")+"</font>"
					+"<font color=#D4D400>	"+info.usrName+"</font>";
			}
			
			if (info.count > 0)
			{
				strHtmlText+="<dl><font color=#A0A0A0>"+tr("Network:")+"</font>";
				QMapIterator<QString, QVector<Server> > it(info.servers);
				while (it.hasNext())
				{
					it.next();

					strHtmlText+="<dt><font color=#A0A0A0>- "+tr("MAC: ")+it.key()+"</font></dt>";

					const QVector<Server>& servers=it.value();
					foreach (Server server, servers)
					{
						strHtmlText+="<dd><font color=#A0A0A0>"+QString("- %1:%2").arg(server.addr).arg(server.port)+"</font></dd>";
					}
				}
				strHtmlText+="</dl>";
			}
			else
			{
				strHtmlText+="<br/><br/>";
			}

			strHtmlText+="<font color=#606060>"+tr("To check host configuration information and operational status.")+"</font>";
		}
		break;
	case ToolConfigure:
		{
			strHtmlText="<font color=#A0A0A0>"+tr("Configure")+"</font>"
				+"<br/><br/><font color=#606060>"+tr("To configure the runtime environment.")+"</font>";
		}
		break;
	case ToolPrivilege:
		{
			strHtmlText="<font color=#A0A0A0>"+tr("Authority Management")+"</font>";

			PrivilegeUser user=QGlobalInfo::instance()->user();

			strHtmlText+="<br/><br/><font color=#A0A0A0>"+tr("Login User:")+"</font>"
				+"<font color=#D4D400>	"+user.name+"</font>"+"<br/><br/><font color=#606060>";

			if (checkPrivileges(user.privileges, PrivilegeRole, AdministratorRole))
			{
				strHtmlText+=tr("Administrator can manage accounts and can assign permissions to other users.");
			}
			else
			{
				strHtmlText+=tr("To modify the login password.");
			}
				
			strHtmlText+="</font>";
		}
		break;
	case ToolLogout:
		{
			strHtmlText="<font color=#A0A0A0>"+tr("Logout")+"</font>"
				+"<br/><br/><font color=#606060>"+tr("Logout when you leave.")+"</font>";
		}
		break;
	default:break;
	}

	m_pPromptPanel->setHtmlText(strHtmlText);
}

void QWndGraphicsScene::setTopologyGraph(const GraphTopo& graph)
{
	QGraphicsPanelItem* pItem=findPanelItem(ToolLocalHost);
	
	if (pItem != NULL)
	{
		if (!pItem->data(UsrDataServerRunning).toBool())
		{
			return;
		}
	}

	setSceneRect(QRectF());

	m_graph=graph;

	QMapIterator<QPair<quint64, quint64>, QGraphicsItemLink*> iterFore(m_linkItems);
	while (iterFore.hasNext())
	{
		iterFore.next();
		Q_ASSERT_NO_NULL(iterFore.value());

		removeItem(iterFore.value());
	}
	m_linkItems.clear();

	QMutableMapIterator<quint64, QPair<QAbstractGraphicsItem*, int> > itFore(m_items);
	while (itFore.hasNext())
	{
		itFore.next();

		QPair<QAbstractGraphicsItem*, int>& peer=itFore.value();
		if (!m_graph.nodes.contains(itFore.key()))
		{
			removeItem(peer.first);
			itFore.remove();
		}
	}

	QMap<quint64, QPair<QAbstractGraphicsItem*, int> > items=m_items;

	addGraphNodeToScene(m_graph.root, -1, QPointF(0.0, 0.0), 0.0);
	linkGraphNode(m_graph.root, -1);

	QVector<QPointF> points;

	QMapIterator<quint64, QPair<QAbstractGraphicsItem*, int> > it(m_items);
	while (it.hasNext())
	{
		it.next();

		QAbstractGraphicsItem* ptr=it.value().first;
		Q_ASSERT_NO_NULL(ptr);

		if (!items.contains(it.key()))
		{
			addItem(ptr);
		}

		points.append(ptr->scenePos());
	}

	QMapIterator<QPair<quint64, quint64>, QGraphicsItemLink*> iter(m_linkItems);
	while (iter.hasNext())
	{
		iter.next();
		Q_ASSERT_NO_NULL(iter.value());

		addItem(iter.value());
	}

	refreshRemoteServerList();

	QRectF rect=sceneRect();
	double dWidth=rect.width();
	double dHeight=rect.width()*GRAPHICS_SCENE_HAWK_EYES_PANEL_HEIGHT/GRAPHICS_SCENE_HAWK_EYES_PANEL_WIDTH;
	if (dHeight < rect.height())
	{
		dWidth=rect.height()*GRAPHICS_SCENE_HAWK_EYES_PANEL_WIDTH/GRAPHICS_SCENE_HAWK_EYES_PANEL_HEIGHT;
		dHeight=rect.height();
	}

	setSceneRect(rect.left()-(dWidth-rect.width())/2, rect.top()-(dHeight-rect.height())/2, dWidth, dHeight);

	m_pHawkPanel->setPoints(points);

	invalidate();
}

void QWndGraphicsScene::setLocalServer(const ServerInfo& info, const QPoint& pt)
{
	QGraphicsPanelItem* pItem=findPanelItem(ToolLocalHost);

	if (pItem != NULL)
	{
		QGlobalInfo::instance()->setLocal(info);

		QVariant var;
		var.setValue(info);

		pItem->setData(UsrDataServerInfo, var);

		if (info.count > 0)
		{
			pItem->setData(UsrDataServerRunning, true);
			pItem->setPixmap(findPixmap(":/images/server.png"));
		}
		else
		{
			pItem->setData(UsrDataServerRunning, false);
			pItem->setPixmap(findPixmap(":/images/serverquit.png"));

			//服务器down
			QMutableMapIterator<quint64, QPair<QAbstractGraphicsItem*, int> >	it(m_items);
			while (it.hasNext())
			{
				it.next();

				QPair<QAbstractGraphicsItem*, int>& peer=it.value();
				if (peer.first != NULL)
				{
					peer.first->setTickCount(QAbstractGraphicsItem::TickTop);
				}
			}
		}

		if (info.count == 0 || m_pToolPanel->mouseMoveEvent(pt))
		{
			invalidate();
		}
	}
}

void QWndGraphicsScene::setRemoteServerList(const QList<ServerInfo>& servers)
{
	bool bRunning=false;	//服务器端正在运行

	QGraphicsPanelItem* pItem=findPanelItem(ToolLocalHost);

	if (pItem != NULL)
	{
		if (pItem->data(UsrDataServerRunning).toBool())
		{
			bRunning=true;
		}
		else
		{
			bRunning=false;
		}
	}

	QMutableHashIterator<QString, QPair<ServerInfo, int> > it(m_remoteServers);
	while (it.hasNext())
	{
		it.next();

		QPair<ServerInfo, int>& peer=it.value();
		peer.second++;
	}

	if (bRunning)
	{
		QListIterator<ServerInfo> iter(servers);
		while (iter.hasNext())
		{
			const ServerInfo& server=iter.next();

			bool bExisted=false;
			if (m_remoteServers.contains(server.hostName))
			{
				QHash<QString, QPair<ServerInfo, int> >::iterator it=m_remoteServers.find(server.hostName);
				while (it != m_remoteServers.end() && it.key() == server.hostName)
				{
					if (server == it.value().first)
					{
						bExisted=true;

						it.value().second=0;

						break;
					}

					it++;
				}
			}

			if (!bExisted)
			{
				m_remoteServers.insertMulti(server.hostName, QPair<ServerInfo, int>(server, 0));
			}
		}
	}

	it.toFront();
	while (it.hasNext())
	{
		it.next();

		QPair<ServerInfo, int>& peer=it.value();

		if (peer.first.count > 0)
		{
			QList<QString> physAddrs=peer.first.servers.keys();
			foreach (QString strPhysAddr, physAddrs)
			{
				quint64 uid=macStringToUint64(strPhysAddr);
				if (m_items.contains(uid))
				{
					const GraphTopoNode& node=m_graph.nodes[uid].first;
					Q_ASSERT(m_graph.nodes[uid].first.type == HostNode);
					QGraphicsItemHost* pItem=(QGraphicsItemHost*)m_items[uid].first;
					if (pItem != NULL)
					{
						if (peer.second == 0)
						{
							pItem->setFlag(QGraphicsItemHost::Connectable, true);
							pItem->setServerInfo(peer.first);

							QString strTag;
							
							QMapIterator<int, GraphNodeInterface> it(node.interfaces);
							while (it.hasNext())
							{
								it.next();

								const GraphNodeInterface& face=it.value();
								for (int i=0; i<face.addresses.size(); i++)
								{
									if (!strTag.isEmpty())
									{
										strTag+="\n";
									}

									strTag+=face.addresses[i];
								}
							}

							if (!peer.first.hostName.isEmpty())
							{
								if (!strTag.isEmpty())
								{
									strTag+="\n";
								}

								strTag+=peer.first.hostName;
							}

							pItem->setTag(strTag);
						}
						else
						{
							pItem->setFlag(QGraphicsItemHost::Connectable, false);
						}
					}
				}
			}
		}

// 		if (peer.second > 0)
// 		{
// 			it.remove();
// 		}
	}

	refreshGroupManagerServerList(bRunning);

	refreshRemoteServerList();

	invalidate();
}

void QWndGraphicsScene::refreshRemoteServerList()
{
	QHashIterator<QString, QPair<ServerInfo, int> > it(m_remoteServers);
	while (it.hasNext())
	{
		it.next();

		const QPair<ServerInfo, int>& peer=it.value();
		if (peer.first.count > 0)
		{
			QList<QString> physAddrs=peer.first.servers.keys();
			foreach (QString strPhysAddr, physAddrs)
			{
				quint64 uid=macStringToUint64(strPhysAddr);
				if (m_items.contains(uid))
				{
					const GraphTopoNode& node=m_graph.nodes[uid].first;

					Q_ASSERT(node.type == HostNode);
					QString strTag;
					
					QMapIterator<int, GraphNodeInterface> iter(node.interfaces);
					while (iter.hasNext())
					{
						iter.next();
						const GraphNodeInterface& face=iter.value();
						for (int i=0; i<face.addresses.size(); i++)
						{
							strTag+=face.addresses[i]+"\n";
						}
					}

					strTag+=peer.first.hostName;

					QGraphicsItemHost* pItem=(QGraphicsItemHost*)m_items[uid].first;
					if (pItem != NULL)
					{
						if (peer.second == 0)
						{
							pItem->setFlag(QGraphicsItemHost::Connectable, true);
							pItem->setServerInfo(peer.first);
						}
						else
						{
							pItem->setFlag(QGraphicsItemHost::Connectable, false);
						}

						pItem->setTag(strTag);
					}
				}
			}
		}
	}
}

void QWndGraphicsScene::refreshGroupManagerServerList(bool bRunning/* =true */)
{
	QList<QPair<ServerInfo, int> > servers;
	QHashIterator<QString, QPair<ServerInfo, int> > it(m_remoteServers);
	while (it.hasNext())
	{
		it.next();

		servers.append(it.value());
	}

	QGlobalInfo::instance()->setServerList(servers);

	if (m_pWndGroupManager == NULL)
	{
		return;
	}

	if (bRunning)
	{
		m_pWndGroupManager->setServerList(servers);
	}
	else
	{
		m_pWndGroupManager->setServerList(QList<QPair<ServerInfo, int> >());
	}
}

void QWndGraphicsScene::mouseDoubleClick(QGraphicsPanelItem* item)
{
	if (item == NULL)
	{
		return;
	}

	int nIndex=item->data(UsrDataToolType).toInt();

	switch (nIndex)
	{
	case ToolWorkgroup:
		{
	 		if (m_pWndGroupManager == NULL)
	 		{
	 			m_pWndGroupManager=new QWndGroupManager;

				refreshGroupManagerServerList();
	 
	 			connect(m_pWndGroupManager, SIGNAL(onClosed(QWidget*)), this, SLOT(wndClosed(QWidget*)));
	 			m_pWndGroupManager->show();
	 		}
	 		else
	 		{
	 			m_pWndGroupManager->raise();
	 			m_pWndGroupManager->activateWindow();
	 		}
		}
		break;
	case ToolLocalHost:
		{
			if (m_pWndManager == NULL)
			{
				ServerInfo info;
				info.count=0;

				if (item != NULL)
				{
					if (!item->data(UsrDataServerRunning).toBool())
					{
						return;
					}

					QVariant var=item->data(UsrDataServerInfo);
					if (var.isValid())
					{
						info=var.value<ServerInfo>();
					}
				}

				Server server;
				if (info.count > 0)
				{
					if (!canConnected(info, server))
					{
						QInfoManager::instance()->append(QInfoManager::Error, tr("Connect to local host failed!!!"));
						return;
					}
				}
				else
				{
					server.addr=QHostAddress(QHostAddress::LocalHost).toString();
					server.port=m_nPort;
				}
				
				m_pWndManager=new QWndManager(info.hostName+"-"+info.usrName, server.addr, server.port);

				connect(m_pWndManager, SIGNAL(onClosed(QWidget*)), this, SLOT(wndClosed(QWidget*)));

				m_pWndManager->show();
			}
			else
			{
				m_pWndManager->raise();
				m_pWndManager->activateWindow();
			}
		}
		break;
	case ToolConfigure:
		{
		}
		break;
	case ToolPrivilege:
		{
			QMap<int, quint32> privileges=QGlobalInfo::instance()->privileges();
			if (checkPrivileges(privileges, PrivilegeRole, AdministratorRole))
			{
				//有权限管理帐号
				if (m_pWndPrivilege == NULL)
				{
					m_pWndPrivilege=new QMainWndPrivilege;

					connect(m_pWndPrivilege, SIGNAL(onClosed(QWidget*)), this, SLOT(wndClosed(QWidget*)));
					m_pWndPrivilege->show();
				}
				else
				{
					m_pWndPrivilege->raise();
					m_pWndPrivilege->activateWindow();
				}
			}
			else
			{
				QDlgModifyPassword dialog(findPixmap(":/images/user_icon.png"));
				dialog.setUser(QGlobalInfo::instance()->user());
				dialog.setModal(true);
				if (dialog.exec() != 0)
				{
					PrivilegeUser usr=dialog.user();

					m_pReceiver->queryModifyUserPassword(usr.name, usr.passwd);
				}
			}
		}
		break;
	case ToolLogout:
		{
			if (m_pWndManager != NULL)
			{
				m_pWndManager->close();
			}

			if (m_pWndGroupManager != NULL)
			{
				m_pWndGroupManager->close();
			}

			if (m_pWndPrivilege != NULL)
			{
				m_pWndPrivilege->close();
			}

			emit logout();
		}
		break;
	default:
		break;
	}
}

 bool QWndGraphicsScene::canConnected(const ServerInfo& info, Server& server) const
 {
 	bool bCanConnected=false;
 	foreach (QVector<Server> servers, info.servers)
 	{
 		foreach (Server s, servers)
 		{
 			if (QServerManager::canConnected(info, s))
 			{
 				server=s;
 
 				bCanConnected=true;
 				break;
 			}
 		}
 
 		if (bCanConnected)
 		{
 			break;
 		}
 	}
 
 	return bCanConnected;
 }

void QWndGraphicsScene::wndClosed(QWidget* widget)
{
	if (widget == NULL)
	{
		return;
	}

	if (widget == m_pWndManager)
	{
		SAFE_DELETE_CLASS(m_pWndManager);
	}
	else if (widget == m_pWndGroupManager)
	{
		SAFE_DELETE_CLASS(m_pWndGroupManager);
	}
	else if (widget == m_pWndPrivilege)
	{
		SAFE_DELETE_CLASS(m_pWndPrivilege);
	}
}

void QWndGraphicsScene::closeEvent()
{
	if (m_pWndManager != NULL)
	{
		m_pWndManager->close();
	}

	if (m_pWndGroupManager != NULL)
	{
		m_pWndGroupManager->close();
	}

	if (m_pWndPrivilege != NULL)
	{
		m_pWndPrivilege->close();
	}

	QMapIterator<quint64, QPair<QAbstractGraphicsItem*, int> >	it(m_items);
	while (it.hasNext())
	{
		it.next();

		if (m_graph.nodes.contains(it.key()) && m_graph.nodes[it.key()].first.type == HostNode)
		{
			QGraphicsItemHost* pItem=(QGraphicsItemHost*)it.value().first;
			if (pItem != NULL)
			{
				pItem->closeWindow();
			}
		}
	}

	QNdc::instance()->pop();
}

void QWndGraphicsScene::linkGraphNode(const GraphTopoNode& node, int ifIndex)
{
	QGraphicsItemLink* pLink=NULL;

	quint64 uid;

	QMapIterator<int, QPair<quint64, int> > it(node.peers);
	while (it.hasNext())
	{
		it.next();

		if (it.key() == ifIndex)
		{
			continue;
		}

		uid=it.value().first;
		if (!m_graph.nodes.contains(uid))
		{
			continue;
		}
		Q_ASSERT (m_graph.nodes.contains(uid));

		if (node.type != HoldNode)
		{
			if (!m_linkItems.contains(QPair<quint64, quint64>(node.uid, uid)) && !m_linkItems.contains(QPair<quint64, quint64>(uid, node.uid)))
			{
				pLink=new QGraphicsItemLink;

				m_linkItems.insert(QPair<quint64, quint64>(node.uid, uid), pLink);

				Q_ASSERT(m_items.contains(node.uid));
				Q_ASSERT(m_items.contains(uid));
				pLink->setLine(m_items[node.uid].first->iconBoundingRectToScene().center(), m_items[uid].first->iconBoundingRectToScene().center());
			}
		}

		if ((node.type&LeafNode) == 0)
		{
			linkGraphNode(m_graph.nodes[uid].first, node.peers[it.key()].second);
		}
	}
}

QRectF QWndGraphicsScene::addGraphNodeToScene(const GraphTopoNode& node, int ifIndex, const QPointF& origin, double angle)
{
	QAbstractGraphicsItem* pItem=NULL;
	QRectF r;

//	logplusInfo(MANAGER_LOGGER_STR, QString("Add node to scene uid=%1, ifIndex=%2").arg(QString::number(node.uid, 16)).arg(ifIndex));

	if (m_items.contains(node.uid))
	{
		pItem=m_items[node.uid].first;
	}

	if (node.type != HoldNode && !m_items.contains(node.uid))
	{
		switch (node.type)
		{
		case RouterNode:
			{
				if (pItem == NULL)
				{
					pItem=new QGraphicsItemRouter(node.uid);
				}
				
				pItem->setTag(node.name);
			}
			break;
		case SwitchNode:
		case SwitchL3Node:
			{
				if (pItem == NULL)
				{
					pItem=new QGraphicsItemSwitch(node.uid);
				}

				QString strTag;
				
				QMapIterator<int, GraphNodeInterface> it(node.interfaces);
				while (it.hasNext())
				{
					it.next();

					const GraphNodeInterface& face=it.value();
					for (int i=0; i<face.addresses.size(); i++)
					{
						if (!strTag.isEmpty())
						{
							strTag+="\n";
						}

						strTag=face.addresses[i];
					}
				}

				if (!node.name.isEmpty())
				{
					if (!strTag.isEmpty())
					{
						strTag+="\n";
					}

					strTag+=node.name;
				}

				pItem->setTag(strTag);
			}
			break;
		case HubNode:
			{
				if (pItem == NULL)
				{
					pItem=new QGraphicsItemHub(node.uid);
				}
			}
			break;
		case HostNode:
			{
				if (pItem == NULL)
				{
					pItem=new QGraphicsItemHost(node.uid);
				}

				QString strTag;
				
				QMapIterator<int, GraphNodeInterface> it(node.interfaces);
				while (it.hasNext())
				{
					it.next();

					const GraphNodeInterface& face=it.value();
					for (int i=0; i<face.addresses.size(); i++)
					{
						if (!strTag.isEmpty())
						{
							strTag+="\n";
						}

						strTag+=face.addresses[i];
					}
				}

				if (!node.name.isEmpty())
				{
					if (!strTag.isEmpty())
					{
						strTag+="\n";
					}

					strTag+=node.name;
				}

				pItem->setTag(strTag);
			}
			break;
		case FirewallNode:
			{
				if (pItem == NULL)
				{
					pItem=new QGraphicsItemFireWall(node.uid);
				}

				QString strTag;

				QMapIterator<int, GraphNodeInterface> it(node.interfaces);
				while (it.hasNext())
				{
					it.next();

					const GraphNodeInterface& face=it.value();
					for (int i=0; i<face.addresses.size(); i++)
					{
						if (!strTag.isEmpty())
						{
							strTag+="\n";
						}

						strTag=face.addresses[i];
					}
				}

				if (!node.name.isEmpty())
				{
					if (!strTag.isEmpty())
					{
						strTag+="\n";
					}

					strTag+=node.name;
				}

				pItem->setTag(strTag);
			}
			break;
		default:
			Q_ASSERT(true);
			break;
		}

		if (pItem != NULL)
		{
			m_items.insert(node.uid, QPair<QAbstractGraphicsItem*, int>(pItem, m_graph.nodes[node.uid].second));
		}
	}

	if (node.type != HoldNode && pItem == NULL)
	{
		Q_ASSERT(m_items.contains(node.uid));

		pItem=m_items[node.uid].first;
	}

	if (pItem != NULL)
	{
		pItem->resetTransform();
		pItem->setPos(origin);
		r=pItem->boundingRect();

		if (m_graph.nodes[node.uid].second > 0)
		{
			pItem->setTickCount(QAbstractGraphicsItem::TickTop);
		}
		else
		{
			pItem->setTickCount(QAbstractGraphicsItem::TickZero);
		}
	}

	double dRadius;
	if (node.type == HoldNode)
	{
		dRadius=10;
		r=QRectF(origin.x()-dRadius/2, origin.y()-dRadius/2, dRadius, dRadius);
	}
	else
	{
		dRadius=qSqrt(r.width()*r.width()+r.height()*r.height())/2;
	}

	if ((node.type&LeafNode) != 0 || node.peers.isEmpty())
	{
		return r;
	}

	int nNumItem=node.peers.size()>3? node.peers.size():3;
	double delta=2*MATH_PI/nNumItem;
	int nStep=qCeil(nNumItem/8.0);

	double dAngleStart=angle+MATH_PI;

	QVector<int> keys=node.peers.keys().toVector();

	QVector<QPair<QPointF, double> > circles;		//圆包围体
	QPair<QPointF, double> pair(origin, dRadius);
	circles.append(pair);

	int nType=0;	//nType=0 遍历所有叶节点， nType=1遍历所有根节点

	while (nType <= 1)
	{
		int nTimes=0;	//第几次遍历，由nStep决定

		double dr;
		double dAngle=dAngleStart;
		quint64 uid;
		while (nTimes < nStep)
		{
			for (int i=nTimes; i<keys.size(); i+=nStep)
			{
				if (keys[i] == ifIndex)
				{
					continue;
				}

				uid=node.peers[keys[i]].first;
				Q_ASSERT (m_graph.nodes.contains(uid));

				dAngle+=delta;

				if (nType == 0)
				{
					if ((m_graph.nodes[uid].first.type&LeafNode) == 0)
					{
						continue;
					}
				}
				else if (nType == 1)
				{
					if ((m_graph.nodes[uid].first.type&LeafNode) != 0)
					{
						continue;
					}
				}

				QRectF rect=addGraphNodeToScene(m_graph.nodes[uid].first, node.peers[keys[i]].second, origin, dAngle);
				dr=qSqrt(rect.width()*rect.width()+rect.height()*rect.height())/2;

				double dx=10.0*cos(dAngle);
				double dy=10.0*sin(dAngle);

				QPointF pt=rect.center();
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

				translateGraphNode(m_graph.nodes[uid].first, node.peers[keys[i]].second, pt.x()-origin.x(), pt.y()-origin.y());

				r|=QRectF(pt.x()-dr/2, pt.y()-dr/2, dr, dr).translated(pt.x()-origin.x(), pt.y()-origin.y());
			}

			nTimes++;
		}

		nType++;
	}

	return r;
}

void QWndGraphicsScene::translateGraphNode(const GraphTopoNode& node, int ifIndex, double dx, double dy)
{
	Q_ASSERT (m_items.contains(node.uid));

	QAbstractGraphicsItem* pItem=m_items[node.uid].first;
	if (pItem != NULL)
	{
		pItem->translate(dx, dy);
	}

	if (((node.type&(HoldNode|RouterNode|SwitchL3Node|SwitchNode|HubNode)) == 0) || node.peers.isEmpty())
	{
		return ;
	}

	QMapIterator<int, QPair<quint64, int> > it(node.peers);
	while (it.hasNext())
	{
		it.next();

		if (it.key() != ifIndex)
		{
			const QPair<quint64, int>& peer=it.value();
			Q_ASSERT (m_graph.nodes.contains(peer.first));

			translateGraphNode(m_graph.nodes[peer.first].first, peer.second, dx, dy);
		}
	}
}
