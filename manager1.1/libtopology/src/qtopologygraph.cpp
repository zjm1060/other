#include "qtopologygraph.h"

#include <QHostAddress>
#include <QThreadPool>
//#include <QFile>

#include "qlogger.h"
#include "define.h"

#define TOPOLOGY_PROBE_TIME_INTERVAL	8*1000
QTopologyGraph::QTopologyGraph(QObject* parent)
:QObject(parent)
{
	qRegisterMetaType<QList<QTopologyProbe::GraphTopoTask> >("QList<QTopologyProbe::GraphTopoTask>");
	m_graph.root.type=HoldNode;
	m_graph.root.uid=GRAPH_TOPO_ROOT_HOLD_NODE;	//根节点

	m_bStatus[Detecting]=false;
	m_bStatus[Reprobe]=false;

	m_pDetector=new QTopologyProbe;

	connect(m_pDetector, SIGNAL(finished(QTopologyProbe::GraphTopoTask, GraphTopoNode, QMap<quint64, GraphTopoNode>)), 
		this, SLOT(finished(QTopologyProbe::GraphTopoTask, GraphTopoNode, QMap<quint64, GraphTopoNode>)));

	m_timer.setInterval(TOPOLOGY_PROBE_TIME_INTERVAL);
	m_timer.setSingleShot(true);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

QTopologyGraph::~QTopologyGraph()
{
	SAFE_DELETE_CLASS(m_pDetector);
}

QTopologyGraph* QTopologyGraph::instance()
{
	static QTopologyGraph* pInstance=new QTopologyGraph;

	return pInstance;
}

void QTopologyGraph::probe(const ServerInfo& local)
{
#ifndef Q_OS_WIN
	if (getuid() != 0)
	{
		logplusError(LIBTOPOLOGY_LOGGER_STR, "Topology Probe must be running in administrator!");
		exit(0);
		return;
	}
#endif

	if (local.count == 0)
	{
		logplusError(LIBTOPOLOGY_LOGGER_STR, "No find  local IP! Please check your network!!!");
		return;
	}

#ifdef Q_OS_AIX
	if (m_pDetector != NULL)
	{
		m_pDetector->setLocalArpEntry(local);
	}
#endif

	m_bStatus[Reprobe]=true;

	m_tasks.clear();

	int nIfIndex;

	QMapIterator<QString, QVector<Server> > it(local.servers);
	while (it.hasNext())
	{
		it.next();

		const QVector<Server>& servers=it.value();
		QVector<quint32> segments;
		for (int i=0; i<servers.size(); i++)
		{
			segments.append(QHostAddress(servers[i].addr).toIPv4Address());
		}

		if (!segments.isEmpty())
		{
			nIfIndex=-1;
			for (int i=0; i<m_tasks.size(); i++)
			{
				if (it.key() == m_tasks[i].physAddress)
				{
					QTopologyProbe::GraphTopoTask& task=m_tasks[i];
					nIfIndex=nIfIndex;

					for (int j=0; j<segments.size(); j++)
					{
						if (!task.segments.contains(segments[j]))
						{
							task.segments.append(segments[j]);
						}
					}
					break;
				}
			}

			if (nIfIndex == -1)
			{
				nIfIndex=m_tasks.size()+1;

				QTopologyProbe::GraphTopoTask task;
				task.puid=m_graph.root.uid;
				task.ifIndex=nIfIndex;
				task.physAddress=it.key();
				task.segments=segments;

				m_tasks.append(task);
			}
		}
	}

	if (!m_bStatus[Detecting])
	{
		logplusInfo(LIBTOPOLOGY_LOGGER_STR, "Begin to probe network...");

		if (!m_timer.isActive())
		{
			timeout();
		}
	}
}

void QTopologyGraph::nextTask()
{
	logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Subnet probe ifIndex=%1,Physical Address=%2").arg(m_task.ifIndex).arg(m_task.physAddress));

	if (m_pDetector != NULL)
	{
		m_bStatus[Detecting]=true;
		m_pDetector->probe(m_task);
	}
}

void QTopologyGraph::finished(QTopologyProbe::GraphTopoTask task, GraphTopoNode root, QMap<quint64, GraphTopoNode> nodes)
{
// 	//test begin
// 	root.peers.clear();
// 	nodes.clear();
// 
// 	QFile file("D:/home/ems/system/manager/test/topology.txt");
// 	file.open(QIODevice::ReadOnly|QIODevice::Text);
// 	QTextStream stream(&file);
// 	int nFlag=0;
// 	while (!stream.atEnd())
// 	{
// 		QString strText=stream.readLine();
// 		if (strText.startsWith("//"))
// 		{
// 			continue;
// 		}
// 
// 		if (strText.isEmpty())
// 		{
// 			nFlag=1;
// 		}
// 		else
// 		{
// 			QStringList lists=strText.split(QChar('\t'));
// 			if (nFlag == 0)
// 			{
// 				quint64 uid=lists[0].toULongLong(NULL, 16);
// 				int nType=lists[1].toInt();
// 
// 				GraphTopoNode node;
// 				node.uid=uid;
// 				node.type=nType;
// 				nodes.insert(uid, node);
// 			}
// 			else
// 			{
// 				quint64 puid=lists[0].toULongLong(NULL, 16);
// 				int pifIndex=lists[1].toInt();
// 				quint64 uid=lists[2].toULongLong(NULL, 16);
// 				int ifIndex=lists[3].toInt();
// 
// 				if (puid == 0)
// 				{
// 					root.peers[pifIndex]=QPair<quint64, int>(uid, ifIndex);
// 				}
// 				else
// 				{
// 					nodes[puid].peers[pifIndex]=QPair<quint64, int>(uid, ifIndex);
// 				}
// 			}
// 		}
// 	}
// 	//test end

	task.nodes=m_task.nodes;

	QTopologyGraphRunnable* pRunnable=new QTopologyGraphRunnable(m_graph, task, root, nodes);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(assembledFinished(GraphTopo, QTopologyProbe::GraphTopoTask, bool, QList<QTopologyProbe::GraphTopoTask>)), 
		this, SLOT(assembledFinished(GraphTopo, QTopologyProbe::GraphTopoTask, bool, QList<QTopologyProbe::GraphTopoTask>)));

	QThreadPool::globalInstance()->start(pRunnable);
}

void QTopologyGraph::assembledFinished(GraphTopo graph, QTopologyProbe::GraphTopoTask task, bool change, QList<QTopologyProbe::GraphTopoTask> tasks)
{
	m_graph=graph;

	QMutableListIterator<QTopologyProbe::GraphTopoTask> mit(m_tasks);
	while (mit.hasNext())
	{
		QTopologyProbe::GraphTopoTask& ts=mit.next();
		if (ts.puid == GRAPH_TOPO_ROOT_HOLD_NODE)
		{
			QVector<quint32> segments;
			for (int i=0; i<task.segments.size(); i++)
			{
				segments.append(task.segments[i]&0xffffff00);

				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Task Segment: %1").arg(QHostAddress(task.segments[i]).toString()));
			}

			bool bContains=false;
			for (int i=0; i<ts.segments.size(); i++)
			{
				if (segments.contains(ts.segments[i]&0xffffff00))
				{
					bContains=true;
					break;
				}
			}

			if (bContains)
			{
				for (int i=0; i<ts.segments.size(); i++)
				{
					if (!segments.contains(ts.segments[i]&0xffffff00))
					{
						task.segments.append(ts.segments[i]);
					}
				}

				mit.remove();
			}
		}
	}

	QListIterator<QTopologyProbe::GraphTopoTask> it(tasks);
	while (it.hasNext())
	{
		const QTopologyProbe::GraphTopoTask& newTask=it.next();

		bool bFind=false;
		QMutableListIterator<QTopologyProbe::GraphTopoTask> iter(m_tasks);
		while (iter.hasNext())
		{
			QTopologyProbe::GraphTopoTask& ts=iter.next();
			if (newTask.physAddress == ts.physAddress)
			{
				bFind=true;

				for (int i=0; i<newTask.segments.size(); i++)
				{
					if (!ts.segments.contains(newTask.segments[i]))
					{
						ts.segments.append(newTask.segments[i]);
					}
				}

				break;
			}
		}

		if (!bFind)
		{
			m_tasks.append(tasks);
		}
	}

	if (change)
	{
		emit changed(m_graph);
	}

	if (!m_bStatus[Reprobe])
	{
		m_tasks.append(task);
	}
 
 	m_bStatus[Detecting]=false;
 	m_timer.start();
}

void QTopologyGraph::timeout()
{
	if (m_bStatus[Detecting] || m_tasks.isEmpty())
	{
		return;
	}

	m_bStatus[Reprobe]=false;

	m_task=m_tasks.takeFirst();
	
	nextTask();
}

QTopologyGraphRunnable::QTopologyGraphRunnable(GraphTopo& graph, QTopologyProbe::GraphTopoTask& task, GraphTopoNode& root, 
											   QMap<quint64, GraphTopoNode>& nodes, QObject* parent)
											   :QObject(parent)
											   ,m_graph(graph)
											   ,m_task(task)
											   ,m_root(root)
											   ,m_nodes(nodes)
{

}

QTopologyGraphRunnable::~QTopologyGraphRunnable()
{

}

void QTopologyGraphRunnable::removeLinkInNodes(GraphTopoNode& node, int ifIndex)
{
	if (node.peers.contains(ifIndex))
	{
		const QPair<quint64, int>& peer=node.peers[ifIndex];
		if (m_graph.nodes.contains(peer.first))
		{
			GraphTopoNode& nd=m_graph.nodes[peer.first].first;

			if (nd.peers.contains(peer.second))
			{
				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Remove Link: %1:%2--->%3:%4")
					.arg(QString::number(nd.uid, 16)).arg(peer.second)
					.arg(QString::number(nd.peers[peer.second].first, 16)).arg(nd.peers[peer.second].second));
				nd.peers.remove(peer.second);

				if (nd.peers.isEmpty() && m_graph.nodes[peer.first].first.type == HubNode)
				{
					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Remove node: uid=%1").arg(QString::number(nd.uid, 16)));
					m_graph.nodes.remove(nd.uid);

					removeNodeInTask(nd.uid);
				}
			}
		}

		logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Remove Link: %1:%2--->%3:%4")
			.arg(QString::number(node.uid, 16)).arg(ifIndex).arg(QString::number(peer.first, 16)).arg(peer.second));

		node.peers.remove(ifIndex);
	}
}

void QTopologyGraphRunnable::removeNodeInTask(quint64 uid)
{
	quint32 nKey=qHash(uid);
	if (m_task.nodes.contains(nKey))
	{
		QMutableHashIterator<quint32, quint64> it(m_task.nodes);
		while (it.hasNext())
		{
			it.next();

			if (it.key() == nKey && uid == it.value())
			{
				it.remove();

				break;
			}
		}
	}
}

bool QTopologyGraphRunnable::addSwitchNodeToGraph(quint64 puid, int ifIndex, const GraphTopoNode& sw, QList<QTopologyProbe::GraphTopoTask>& tasks)
{
	bool bChanged=false;

// 	if (m_portUsed.contains(m_task.puid) && m_portUsed[m_task.puid].contains(m_task.ifIndex))
// 	{
// 		return bChanged;
// 	}
// 
// 	m_portUsed[m_task.puid].append(m_task.ifIndex);

	if ((sw.type&(RouterNode|SwitchL3Node)) != 0)
	{
		//获取路由信息，探测拓扑发现任务
		foreach (GraphNodeIpRoute route, sw.ipRoutes)
		{
			if (route.routeType == 4)
			{
				QTopologyProbe::GraphTopoTask task;
				task.puid=sw.uid;
				task.ifIndex=route.ifIndex;
				foreach (GraphNodeInterface face,sw.interfaces)
				{
					if (face.ifIndex == route.ifIndex)
					{
						task.physAddress=face.physAddress;

						task.segments=QVector<quint32>(1, QHostAddress(route.nextHop).toIPv4Address());

						tasks.append(task);

						logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Find a new task: puid=%1, ifIndex=%2, MAC=%3, nextHop=%4")
							.arg(QString::number(task.puid, 16)).arg(task.ifIndex).arg(task.physAddress).arg(QHostAddress(route.nextHop).toString()));

						break;
					}
				}
			}
		}
	}

	//添加节点到拓扑图
	if (!m_graph.nodes.contains(sw.uid))
	{
		//新节点
		bChanged=true;

		if (puid == GRAPH_TOPO_ROOT_HOLD_NODE)
		{
			m_graph.nodes.insert(sw.uid, QPair<GraphTopoNode, int>(sw, 0));
			m_graph.nodes[sw.uid].first.peers.clear();

			int nIfIndex=0;
			while (m_graph.root.peers.contains(++nIfIndex));
			m_graph.root.peers.insert(nIfIndex, QPair<quint64, int>(sw.uid, -1));

			logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Add a new node to root: Node:%1 link to Root node:%2")
				.arg(QString::number(sw.uid, 16)).arg(nIfIndex));

			addChildNodesToGraph(sw.uid, -1, tasks);
		}
		else
		{
			Q_ASSERT(m_graph.nodes.contains(puid));

			int nIndex=-1;
			int nChildIndex=-1;
			GraphTopoNode& pnode=m_graph.nodes[puid].first;
			if (pnode.type != HubNode)
			{
				nIndex=ifIndex;

				if (sw.type != HubNode)
				{
					//父节点和当前节点都是实际物理节点
					m_graph.nodes.insert(sw.uid, QPair<GraphTopoNode, int>(sw, 0));
					m_graph.nodes[sw.uid].first.peers.clear();

					if (pnode.peers.contains(ifIndex))
					{
						QPair<quint64, int> peer=pnode.peers[ifIndex];
						removeLinkInNodes(pnode, ifIndex);

						//将原父节点的子树从父节点分离出来
						int nChild=0;
						while (m_graph.root.peers.contains(++nChild));
						m_graph.root.peers.insert(nChild, QPair<quint64, int>(peer.first, -1));

						logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Move a  node to root: Node:%1 link to root node:%2")
							.arg(QString::number(peer.first, 16)).arg(nChild));
					}
				}
				else
				{
					//当前节点是抽象物理节点
					if (pnode.peers.contains(ifIndex))
					{
						QPair<quint64, int> peer=pnode.peers[ifIndex];
						Q_ASSERT(m_graph.nodes.contains(peer.first));

						GraphTopoNode& nd=m_graph.nodes[peer.first].first;

						if (nd.type != HubNode)
						{
							//原节点是实际物理设备，与当前逻辑设备肯定不同
							removeLinkInNodes(pnode, ifIndex);

							//将原父节点的子树从父节点分离出来
							int nChild=0;
							while (m_graph.root.peers.contains(++nChild));
							m_graph.root.peers.insert(nChild, QPair<quint64, int>(peer.first, -1));

							logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Remove a  node to root: Node:%1 link to root node:%2")
								.arg(QString::number(peer.first, 16)).arg(nChild));

							m_graph.nodes.insert(sw.uid, QPair<GraphTopoNode, int>(sw, 0));
							m_graph.nodes[sw.uid].first.peers.clear();
						}
						else
						{
							//原节点与当前节点都是抽象物理设备，只是uid不同，将原节点uid修改为当前uid
							GraphTopoNode node=nd;
							node.uid=sw.uid;

							logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Change node uid=%1 ---> uid=%2")
								.arg(QString::number(nd.uid, 16)).arg(QString::number(node.uid, 16)));

							pnode.peers[ifIndex]=QPair<quint64, int>(node.uid, peer.second);
							logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Change Link uid=%1:%2 ---> uid=%3:%4 ")
								.arg(QString::number(pnode.uid, 16)).arg(ifIndex).arg(QString::number(node.uid, 16)).arg(peer.second));

							m_graph.nodes.remove(nd.uid);
							m_graph.nodes.insert(node.uid, QPair<GraphTopoNode, int>(node, 0));

							Q_ASSERT(m_graph.nodes.contains(node.uid));
							GraphTopoNode& cur=m_graph.nodes[node.uid].first;
							QMapIterator<int, QPair<quint64, int> > it(cur.peers);
							while (it.hasNext())
							{
								it.next();
								const QPair<quint64, int>& pr=it.value();
								if (pr.first == puid)
								{
									continue;
								}

								Q_ASSERT(m_graph.nodes.contains(pr.first));

								GraphTopoNode& child=m_graph.nodes[pr.first].first;
								child.peers.insert(pr.second, QPair<quint64, int>(node.uid, it.key()));

								logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Change Link uid=%1:%2 ---> uid=%3:%4 ")
									.arg(QString::number(cur.uid, 16)).arg(it.key()).arg(QString::number(child.uid, 16)).arg(pr.second));
							}
						}
					}
					else
					{
						m_graph.nodes.insert(sw.uid, QPair<GraphTopoNode, int>(sw, 0));
						m_graph.nodes[sw.uid].first.peers.clear();
					}
				}
			}
			else
			{
				m_graph.nodes.insert(sw.uid, QPair<GraphTopoNode, int>(sw, 0));
				m_graph.nodes[sw.uid].first.peers.clear();

				nIndex=0;
				while (pnode.peers.contains(++nIndex));
			}

			QMapIterator<int, QPair<quint64, int> > it(sw.peers);
			while (it.hasNext())
			{
				it.next();

				const QPair<quint64, int>& pr=it.value();
				if (pr.first == puid)
				{
					nChildIndex=it.key();
					break;
				}
			}

			if (!pnode.peers.contains(ifIndex))
			{
				if (nChildIndex == -1)
				{
					nChildIndex=0;
					while (sw.peers.contains(++nChildIndex));
				}

				pnode.peers.insert(nIndex, QPair<quint64, int>(sw.uid, nChildIndex));

				Q_ASSERT(m_graph.nodes.contains(sw.uid));
				m_graph.nodes[sw.uid].first.peers.insert(nChildIndex, QPair<quint64, int>(puid, nIndex));

				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Add Node: %1:%2 ---> %3:%4")
					.arg(QString::number(sw.uid, 16)).arg(nChildIndex).arg(QString::number(puid, 16)).arg(nIndex));
			}

			bChanged|=addChildNodesToGraph(sw.uid, nChildIndex, tasks);
		}
	}
	else
	{
		//拓扑图中已包含该节点，根据获取的节点信息修改原节点信息
		GraphTopoNode& node=m_graph.nodes[sw.uid].first;
		setGraphTopoNodeProperty(node, sw);
		m_graph.nodes[sw.uid].second=0;	//表示该设备是活动的

		//检查节点与其父节点的连接关系
		if (puid == GRAPH_TOPO_ROOT_HOLD_NODE)
		{
			bool bFind=false;
			QMapIterator<int, QPair<quint64, int> > iter(m_graph.root.peers);
			while (iter.hasNext())
			{
				iter.next();

				if (sw.uid == iter.value().first)
				{
					bFind=true;

					bChanged|=addChildNodesToGraph(sw.uid, -1, tasks);
					break;
				}
			}

			if (!bFind)
			{
				bChanged=true;

				removeLinkInNodesByTrace(m_graph.root, -1, node);

				int nIfIndex=0;
				while (m_graph.root.peers.contains(++nIfIndex));
				m_graph.root.peers.insert(nIfIndex, QPair<quint64, int>(sw.uid, -1));

				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Remove a  node: Root:%1 link to uid=%2")
					.arg(nIfIndex).arg(QString::number(node.uid, 16)));

				addChildNodesToGraph(sw.uid, -1, tasks);
			}
		}
		else
		{
			Q_ASSERT(m_graph.nodes.contains(puid));
			GraphTopoNode& pnode=m_graph.nodes[puid].first;

			int nIfIndex=-1;
			if (pnode.type != HubNode)
			{
				nIfIndex=ifIndex;

				if (!pnode.peers.contains(nIfIndex))
				{
					bChanged=true;
				}
			}
			else
			{
				QMapIterator<int, QPair<quint64, int> > it(pnode.peers);
				while (it.hasNext())
				{
					it.next();

					const QPair<quint64, int>& peer=it.value();
					if (peer.first == sw.uid)
					{
						nIfIndex=it.key();
					}
				}

				if (nIfIndex == -1)
				{
					nIfIndex=0;
					while (node.peers.contains(++nIfIndex));

					bChanged=true;
				}
			}

			int nChildIndex=-1;
			if (sw.type != HubNode)
			{
				QMapIterator<int, QPair<quint64, int> > it(sw.peers);
				while (it.hasNext())
				{
					it.next();

					const QPair<quint64, int>& peer=it.value();
					if (peer.first == puid)
					{
						nChildIndex=it.key();
					}
				}

				Q_ASSERT(nChildIndex > 0);
				if (!node.peers.contains(nChildIndex))
				{
					bChanged=true;
				}
			}
			else
			{
				QMapIterator<int, QPair<quint64, int> > it(node.peers);
				while (it.hasNext())
				{
					it.next();

					const QPair<quint64, int>& peer=it.value();
					if (peer.first == puid)
					{
						nChildIndex=it.key();
					}
				}

				if (nChildIndex == -1)
				{
					nChildIndex=0;
					while (node.peers.contains(++nChildIndex));

					bChanged=true;
				}
			}

			if (!pnode.peers.contains(nIfIndex) || (pnode.peers.contains(nIfIndex) && pnode.peers[nIfIndex] != QPair<quint64, int>(node.uid, nChildIndex))
				|| !node.peers.contains(nChildIndex) || (node.peers.contains(nChildIndex) && node.peers[nChildIndex] != QPair<quint64, int>(pnode.uid, nIfIndex)))
			{
				bChanged=true;
				removeLinkInNodes(pnode, nIfIndex);
				removeLinkInNodes(node, nChildIndex);

				pnode.peers.insert(nIfIndex, QPair<quint64, int>(node.uid, nChildIndex));
				node.peers.insert(nChildIndex, QPair<quint64, int>(puid, nIfIndex));

				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Add Link: %1:%2 ---> %3:%4")
					.arg(QString::number(pnode.uid, 16)).arg(nIfIndex).arg(QString::number(node.uid, 16)).arg(nChildIndex));
			}

			int nIndex=-1;
			QMapIterator<int, QPair<quint64, int> > it(sw.peers);
			while (it.hasNext())
			{
				it.next();

				const QPair<quint64, int>& peer=it.value();
				if (peer.first == puid)
				{
					nIndex=it.key();
					break;
				}
			}
			bChanged|=addChildNodesToGraph(node.uid, nIndex, tasks);
		}
	}

	return bChanged;
}

bool QTopologyGraphRunnable::removeLinkInNodesByTrace(GraphTopoNode& pnode, int ifIndex, GraphTopoNode& node)
{
	Q_ASSERT(node.uid != GRAPH_TOPO_ROOT_HOLD_NODE);

	if (pnode.uid == node.uid)
	{
		removeLinkInNodes(pnode, ifIndex);
		return true;
	}

	QMapIterator<int, QPair<quint64, int> > it(pnode.peers);
	while (it.hasNext())
	{
		it.next();

		if (it.key() == ifIndex)
		{
			continue;
		}

		const QPair<quint64, int>& peer=it.value();
		Q_ASSERT(m_graph.nodes.contains(peer.first));

		GraphTopoNode& nd=m_graph.nodes[peer.first].first;

		if (removeLinkInNodesByTrace(nd, peer.second, node))
		{
			return true;
		}
	}

	return false;
}

void QTopologyGraphRunnable::recordNodesStatus()
{
	QMutableHashIterator<quint32, quint64> it(m_task.nodes);
	while (it.hasNext())
	{
		it.next();

		quint64 uid=it.value();

		if (m_graph.nodes.contains(uid))
		{
			m_nodesStatus.insert(uid, m_graph.nodes[uid].second);
			m_graph.nodes[uid].second=1;
		}
		else
		{
			it.remove();
		}
	}
}

bool QTopologyGraphRunnable::nodesStatusChanged()
{
	QHashIterator<quint32, quint64> it(m_task.nodes);
	while (it.hasNext())
	{
		it.next();

		quint64 uid=it.value();
		Q_ASSERT(m_graph.nodes.contains(uid));

		if (!m_nodesStatus.contains(uid) || m_nodesStatus[uid] != m_graph.nodes[uid].second)
		{
			return true;
		}
	}

	return false;
}

void QTopologyGraphRunnable::run()
{
	QList<QTopologyProbe::GraphTopoTask> tasks;		//探测到路由器，获取下一跳探测任务
	bool bChanged=false;	//拓扑图是否发生改变

	recordNodesStatus();

	//记录下次探测所要访问的节点
	QMapIterator<quint64, GraphTopoNode> iter(m_nodes);
	while (iter.hasNext())
	{
		iter.next();

		quint32 nHash=qHash(iter.key());
		if (!m_task.nodes.contains(nHash))
		{
			bChanged=true;

			m_task.nodes.insertMulti(nHash, iter.key());
		}
		else
		{
			QList<quint64> uids=m_task.nodes.values(nHash);
			if (!uids.contains(iter.key()))
			{
				bChanged=true;

				m_task.nodes.insertMulti(nHash, iter.key());
			}
		}
	}

	//合并子树
//	m_portUsed.clear();
	mergeGraph();

//	m_portUsed.clear();
	QMapIterator<int, QPair<quint64, int> > it(m_root.peers);
	while (it.hasNext())
	{
		it.next();

		const QPair<quint64, int>& peer=it.value();
		Q_ASSERT(m_nodes.contains(peer.first));

		GraphTopoNode& node=m_nodes[peer.first];
		if ((node.type & (RouterNode|SwitchL3Node|SwitchNode|HubNode)) != 0)
		{
			bChanged|=addSwitchNodeToGraph(m_task.puid, m_task.ifIndex, node, tasks);
		}
	}

	QMutableHashIterator<quint32, quint64> ator(m_task.nodes);
	while (ator.hasNext())
	{
		ator.next();

		quint64 uid=ator.value();

		if (m_graph.nodes.contains(uid))
		{
			GraphTopoNode& node=m_graph.nodes[uid].first;
			if (node.type == HubNode)
			{
				bool bDelete=false;
				if (node.peers.isEmpty())
				{
					bDelete=true;
				}
				else if (node.peers.size() == 1)
				{
					QMapIterator<int, QPair<quint64, int> > iter(node.peers);
					iter.next();
					const QPair<quint64, int>& peer=iter.value();
					int nType=m_graph.nodes[peer.first].first.type;
					if ((nType& (RouterNode|SwitchL3Node|SwitchNode|HubNode)) != 0)
					{
						removeLinkInNodes(node, iter.key());
						bDelete=true;
					}
				}

				if (bDelete)
				{
					bChanged=true;

					m_graph.nodes.remove(uid);
					ator.remove();
				}
			}
		}
		else
		{
			bChanged=true;
			ator.remove();
		}
	}

	if (!bChanged)
	{
		bChanged=nodesStatusChanged();
	}

	QMutableMapIterator<int, QPair<quint64, int> > at(m_graph.root.peers);
	while (at.hasNext())
	{
		at.next();

		QPair<quint64, int>& peer=at.value();
		if (!m_graph.nodes.contains(peer.first))
		{
			removeNodeInTask(peer.first);

			at.remove();

			bChanged=true;
		}
		else
		{
			GraphTopoNode& nd=m_graph.nodes[peer.first].first;
			if ((nd.type&LeafNode) != 0 && !nd.peers.isEmpty())
			{
				removeNodeInTask(peer.first);

				at.remove();

				bChanged=true;
			}
		}
	}

// 	//test begin
// 	QMapIterator<quint64, QPair<GraphTopoNode, int> > temp(m_graph.nodes);
// 	while (temp.hasNext())
// 	{
// 		temp.next();
// 
// 		const GraphTopoNode& tmpNode=temp.value().first;
// 		QMapIterator<int, QPair<quint64, int> > temp1(tmpNode.peers);
// 		while (temp1.hasNext())
// 		{
// 			temp1.next();
// 
// 			const QPair<quint64, int>& peer=temp1.value();
// 
// 			logplusInfo(LIBTOPOLOGY_LOGGER_STR, QString("Node %1:%2 ---> %3:%4")
// 				.arg(QString::number(temp.key(), 16)).arg(temp1.key()).arg(QString::number(peer.first, 16)).arg(peer.second));
// 		}
// 	}
// 	//test end

	emit assembledFinished(m_graph, m_task, bChanged, tasks);
}

void QTopologyGraphRunnable::mergeGraph()
{
//	m_portUsed[m_root.uid].append(-1);
	mergeTreeToGraph(m_root, -1);
}

void QTopologyGraphRunnable::mergeTreeToGraph(GraphTopoNode& node, int ifIndex)
{
// 	if (m_portUsed.contains(node.uid) && m_portUsed[node.uid].contains(ifIndex))
// 	{
// 		return;
// 	}
// 
// 	m_portUsed[node.uid].append(ifIndex);

	QMutableMapIterator<int, QPair<quint64, int> > it(node.peers);
	while (it.hasNext())
	{
		it.next();

		if (it.key() == ifIndex)
		{
			continue;
		}

		const QPair<quint64, int>& peer=it.value();
		Q_ASSERT(m_nodes.contains(peer.first));

		GraphTopoNode& nd=m_nodes[peer.first];
		if (nd.peers.contains(peer.second))
		{
			if (nd.peers[peer.second].first != node.uid)
			{
				int index=traceRootNode(node, -1);
				int id=traceRootNode(nd, -1);

				if (index != id)
				{
					quint64 uid=traceFirstHubNode(nd, -1);

					if (m_nodes.contains(uid))
					{
						GraphTopoNode& hub=m_nodes[uid];
						Q_ASSERT(hub.type == HubNode);
						int nIfIndex=0;
						while (hub.peers.contains(++nIfIndex));

						node.peers[it.key()]=QPair<quint64, int>(hub.uid, nIfIndex);
						hub.peers[nIfIndex]=QPair<quint64, int>(node.uid, it.key());

						m_root.peers.remove(index);

						logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Combine child-tree %1 to %2. Node %3:%4 link to Hub %5:%6")
							.arg(index).arg(id).arg(QString::number(node.uid, 16)).arg(it.key()).arg(QString::number(hub.uid, 16)).arg(nIfIndex));

						continue;
					}
				}
				else
				{
					it.remove();
				}
			}
		}

		mergeTreeToGraph(nd, peer.second);
	}
}

int QTopologyGraphRunnable::traceRootNode(GraphTopoNode& node, int ifIndex)
{
	QMapIterator<int, QPair<quint64, int> > it(m_root.peers);
	while (it.hasNext())
	{
		it.next();

		const QPair<quint64, int>& peer=it.value();
		if (peer.first == node.uid)
		{
			return it.key();
		}
	}

	int index=-1;

	QMapIterator<int, QPair<quint64, int> > iter(node.peers);
	while (iter.hasNext())
	{
		iter.next();

		if (iter.key() == ifIndex)
		{
			continue;
		}

		const QPair<quint64, int>& peer=iter.value();
		Q_ASSERT(m_nodes.contains(peer.first));
		GraphTopoNode& nd=m_nodes[peer.first];

		index=traceRootNode(nd, peer.second);
		if (index > 0)
		{
			return index;
		}
	}

	return -1;
}

quint64 QTopologyGraphRunnable::traceFirstHubNode(GraphTopoNode& node, int ifIndex)
{
	if (node.type == HubNode)
	{
		return node.uid;
	}

	QMutableMapIterator<int, QPair<quint64, int> > it(node.peers);
	while (it.hasNext())
	{
		it.next();

		if (it.key() == ifIndex)
		{
			continue;
		}

		const QPair<quint64, int>& peer=it.value();
		Q_ASSERT(m_nodes.contains(peer.first));

		GraphTopoNode& nd=m_nodes[peer.first];

		quint64 uid=traceFirstHubNode(nd, peer.second);
		if (uid != GRAPH_TOPO_INVALID_NODE)
		{
			return uid;
		}
	}

	return GRAPH_TOPO_INVALID_NODE;
}

bool QTopologyGraphRunnable::addChildNodesToGraph(quint64 puid, int ifIndex, QList<QTopologyProbe::GraphTopoTask>& tasks)
{
	bool bChanged=false;

	Q_ASSERT(m_nodes.contains(puid));
	Q_ASSERT(m_graph.nodes.contains(puid));

	GraphTopoNode& pnode=m_graph.nodes[puid].first;
	const GraphTopoNode& node=m_nodes[puid];
	QMapIterator<int, QPair<quint64, int> > it(node.peers);
	while (it.hasNext())
	{
		it.next();

		if (it.key() == ifIndex)
		{
			continue;
		}

		const QPair<quint64, int>& peer=it.value();
		
		logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Node %1:%2 link to node %3:%4")
			.arg(QString::number(peer.first, 16)).arg(peer.second).arg(QString::number(puid, 16)).arg(ifIndex));
		Q_ASSERT(m_nodes.contains(peer.first));
		const GraphTopoNode& nd=m_nodes[peer.first];

		int nIfIndex=-1;
		if (node.type != HubNode)
		{
			nIfIndex=it.key();
		}
		else
		{
			QMapIterator<int, QPair<quint64, int> > iter(pnode.peers);
			while (iter.hasNext())
			{
				iter.next();

				const QPair<quint64, int>& peer=iter.value();
				if (peer.first == nd.uid)
				{
					nIfIndex=iter.key();

					break;
				}
			}

			if (nIfIndex == -1)
			{
				nIfIndex=0;
				while (pnode.peers.contains(++nIfIndex));

				bChanged=true;
			}
		}

		if ((nd.type&LeafNode) == 0)
		{
			bChanged|=addSwitchNodeToGraph(puid, nIfIndex, nd, tasks);
		}
		else
		{
			//添加主机节点到拓扑图中
			if (!m_graph.nodes.contains(nd.uid))
			{
				bChanged=true;

				m_graph.nodes.insert(nd.uid, QPair<GraphTopoNode, int>(nd, 0));
				m_graph.nodes[nd.uid].first.peers.clear();

				removeLinkInNodes(pnode, nIfIndex);

				pnode.peers.insert(nIfIndex, peer);
				m_graph.nodes[nd.uid].first.peers.insert(peer.second, QPair<quint64, int>(puid, nIfIndex));

				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Add node %1:%2 ---> %3:%4")
					.arg(QString::number(nd.uid, 16)).arg(peer.second).arg(QString::number(pnode.uid, 16)).arg(nIfIndex));
			}
			else
			{
				GraphTopoNode& child=m_graph.nodes[nd.uid].first;
				setGraphTopoNodeProperty(child, nd);
				m_graph.nodes[nd.uid].second=0;

				int nChildIfIndex=-1;
				if (child.type != HubNode)
				{
					nChildIfIndex=peer.second;
				}
				else
				{
					QMapIterator<int, QPair<quint64, int> > iter(child.peers);
					while (iter.hasNext())
					{
						iter.next();

						const QPair<quint64, int>& peer=iter.value();
						if (peer.first == puid)
						{
							nChildIfIndex=iter.key();

							break;
						}
					}

					if (nChildIfIndex == -1)
					{
						nChildIfIndex=0;
						while (child.peers.contains(++nChildIfIndex));

						bChanged=true;
					}
				}

				if (!pnode.peers.contains(nIfIndex) || (pnode.peers.contains(nIfIndex) && pnode.peers[nIfIndex] != QPair<quint64, int>(child.uid, nChildIfIndex))
					|| !child.peers.contains(nChildIfIndex) || (child.peers.contains(nChildIfIndex) && child.peers[nChildIfIndex] != QPair<quint64, int>(pnode.uid, nIfIndex)))
				{
					bChanged=true;
					removeLinkInNodes(child, nChildIfIndex);
					removeLinkInNodes(pnode, nIfIndex);

					pnode.peers.insert(nIfIndex, QPair<quint64, int>(child.uid, nChildIfIndex));
					child.peers.insert(nChildIfIndex, QPair<quint64, int>(puid, nIfIndex));

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Add Link: %1:%2 ---> %3:%4")
						.arg(QString::number(pnode.uid, 16)).arg(nIfIndex).arg(QString::number(child.uid, 16)).arg(nChildIfIndex));
				}
			}
		}
	}

	return bChanged;
}

void QTopologyGraphRunnable::setGraphTopoNodeProperty(GraphTopoNode& target, const GraphTopoNode& source)
{
	target.name=source.name;
	target.oid=source.oid;
	target.desc=source.desc;
	target.services=source.services;
	target.ifNumber=source.ifNumber;
	target.ipForwarding=source.ipForwarding;
	target.ipEntAddrs=source.ipEntAddrs;
	target.ipRoutes=source.ipRoutes;
	target.ipMedia=source.ipMedia;
	target.interfaces=source.interfaces;
	target.bridgeBase=source.bridgeBase;
	target.fdb=source.fdb;
}
