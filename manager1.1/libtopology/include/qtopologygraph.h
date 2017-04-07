#ifndef QTOPOLOGYGRAPH_H
#define QTOPOLOGYGRAPH_H
 
#include <QVector>
#include <QList>
#include <QString>

#include "global.h"
#include "topocommon.h"
#include "qtopologyprobe.h"
#include "qutildef.h"

class TOPOLOGY_EXPORT QTopologyGraph : public QObject
{
	Q_OBJECT

public:
	QTopologyGraph(QObject* parent=0);
	~QTopologyGraph();

public:
	static QTopologyGraph* instance();

public:
	void probe(const ServerInfo& local);

public slots:
	void finished(QTopologyProbe::GraphTopoTask task, GraphTopoNode root, QMap<quint64, GraphTopoNode> nodes);
	void assembledFinished(GraphTopo graph, QTopologyProbe::GraphTopoTask task, bool change, QList<QTopologyProbe::GraphTopoTask> tasks);

signals:
	void changed(GraphTopo graph);

private:
	void nextTask(); 

private slots:
	void timeout();

private:
	GraphTopo	m_graph;

	QTopologyProbe*	m_pDetector;

private:
	QList<QTopologyProbe::GraphTopoTask>	m_tasks;
	QTopologyProbe::GraphTopoTask			m_task;

	QTimer	m_timer;	//定时检测网络状态

private:
	enum StatusFlag
	{
		Detecting,
		Reprobe,
		StatusEnd
	};

	bool	m_bStatus[StatusEnd];
};

class TOPOLOGY_EXPORT QTopologyGraphRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QTopologyGraphRunnable(GraphTopo& graph, QTopologyProbe::GraphTopoTask& task, GraphTopoNode& root, QMap<quint64, GraphTopoNode>& nodes, QObject* parent=0);
	virtual ~QTopologyGraphRunnable();

public:
	virtual void run();

signals:
	void assembledFinished(GraphTopo graph, QTopologyProbe::GraphTopoTask task, bool changed, QList<QTopologyProbe::GraphTopoTask> tasks);

private:
	bool addChildNodesToGraph(quint64 puid, int ifIndex, QList<QTopologyProbe::GraphTopoTask>& tasks);	//父节点，ifIndex新获取的节点指向父节点的索引
	bool addSwitchNodeToGraph(quint64 puid, int ifIndex, const GraphTopoNode& sw, QList<QTopologyProbe::GraphTopoTask>& tasks);	//puid,ifIndex为实际拓扑图中的属性，sw为拓扑子图，将sw放到实际拓扑图中

	void removeLinkInNodes(GraphTopoNode& node, int ifIndex);

	bool removeLinkInNodesByTrace(GraphTopoNode& pnode, int ifIndex, GraphTopoNode& node);

	void mergeGraph();
	void mergeTreeToGraph(GraphTopoNode& node, int ifIndex);
	quint64 traceFirstHubNode(GraphTopoNode& node, int ifIndex);
	int traceRootNode(GraphTopoNode& node, int ifIndex);

	void removeNodeInTask(quint64 uid);

private:
	void setGraphTopoNodeProperty(GraphTopoNode& target, const GraphTopoNode& source);

// private:
// 	void test(GraphTopoNode& node, int ifIndex);

private:
	void recordNodesStatus();
	bool nodesStatusChanged();

private:
	GraphTopo	m_graph;
	QTopologyProbe::GraphTopoTask m_task;
	GraphTopoNode m_root;
	QMap<quint64, GraphTopoNode>	m_nodes;

//	QMap<quint64, QVector<int> >	m_portUsed;

	QMap<quint64, int>	m_nodesStatus;	//记录运行前顶点的在离线状态
};

Q_DECLARE_METATYPE(QList<QTopologyProbe::GraphTopoTask>)

#endif // QTOPOLOGYGRAPH_H

