#ifndef QTOPOLOGYPROBE_H
#define QTOPOLOGYPROBE_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QTimer>
#include <QRunnable>

#include "global.h"
#include "topocommon.h"
#include "qicmp.h"
#include "qsnmp.h"
#include "qutildef.h"

class TOPOLOGY_EXPORT QTopologyProbe : public QObject
{
	Q_OBJECT

public:
	QTopologyProbe(QObject *parent=0);
	~QTopologyProbe();

public:
	typedef struct _graphTopoTask
	{
		quint64 puid;	//父节点uid
		int ifIndex;	//父节点接口号

		QString physAddress;	//父节点接口号对应的物理地址
		QVector<quint32>	segments;	//父节点接口号对应的网段
		QHash<quint32, quint64>	nodes;	//返回此任务的节点,key=uid的hash值,quint64=uid
	}GraphTopoTask;

public:
	void probe(const GraphTopoTask& task);

	void addSegments(quint32 address, bool around=false);	//添加address网段,around=true添加附近正负一
	void addSegment(quint32 address);	//添加address网段

	void findAlives(quint32 segment);		//发现活动IP

	void querySysName(quint32 segment);		//发现设备名称
	void queryOidProperty(quint32 segment, int oidtype, unsigned char dataType, int requestType=QSnmp::SnmpAsynGet, int nodeType=AllNode);		//发现设备OID信息
	void querySwitchOidProperty(quint32 segment, int oidtype, unsigned char dataType, int requestType=QSnmp::SnmpAsynGet);
	void queryRouterOidProperty(quint32 segment, int oidtype, unsigned char dataType, int requestType=QSnmp::SnmpAsynGet);

	bool contains(quint32 segment, int nodeType);
	quint64 contains(quint32 segment, const QString& addr);

public:
	void assemble(quint32 segment);	//组装拓扑图,segment:网段
	void assembleRouter(GraphTopoNode& parent, quint32 segment, QList<quint64> uids);	//组装路由器节点,parent=路由器节点，segment=待检测网段,uids=网段中SNMP代理服务器
	void assembleRootSwitch(GraphTopoNode& parent, int ifIndex, QList<quint64>& uids, quint32 segment, QList<quint64>& hosts);	//组装根交换机节点
	void assembleSwitch(GraphTopoNode& sw, int ifIndex, QList<quint64>& uids, quint32 segment, QList<quint64>& hosts);	//组装交换机节点
	void assembleSwitchPerIfIndex(GraphTopoNode& sw, int ifIndex, QList<quint64>& uids, quint32 segment, QList<quint64>& hosts);	//组装交换机每个端口,除了ifIndex，uid=交换机端口，uids=网段中SNMP代理服务器
	void assembleHub(GraphTopoNode& parent, int ifIndex, QList<quint64>& uids, quint32 segment, QList<quint64>& hosts);	//组装Hub

	quint64 addHostToTopo(const QString& physAddr, quint32 segment);
	quint64 addHostToTopo(const QString& physAddr, quint32 segment, GraphNodeInterface& face);

public:
	QString octetStrToString(const QString& str);
	GraphNodeIpNetToMedia ipNetToMediaFromArp(const QString& physAddr);
	const QString macAddrFromIpAddr(const QString& addr) const;
	const QVector<QString> ipAddrFromMacAddr(const QString& mac, quint32 segment=GRAPH_TOPO_INVALID_SEGMENT);

public slots:
	void foundAlives(quint32 segment, QVector<quint32> alives);
	void snmpResponse(int oidType, QVector<QSnmp::SnmpResponse> results);
	void assembled();

public:
	static int ifIndexToPort(const GraphTopoNode& node, int ifIndex);
	static int portToIfIndex(const GraphTopoNode& node, int port);

#ifdef Q_OS_AIX
	void setLocalArpEntry(const ServerInfo& info);
#endif

signals:
	void finished(QTopologyProbe::GraphTopoTask task, GraphTopoNode root, QMap<quint64, GraphTopoNode> nodes);

private:
	void terminateThread();

	void nextAliveTask();
	void foundArpEntry(const QString& mac, const QString& addr);

private:
	GraphTopoTask m_topoTask;

#ifdef Q_OS_AIX
	ServerInfo	m_local;	//本地IP地址与mac对应关系，aix的arp表中无本地信息
#endif

	GraphTopoNode m_root;
	int m_nIfIndex;

	QIcmp*	m_pIcmp;		//ICMP协议
	QSnmp*	m_pSnmp;		//SNMP协议

private:
	QMap<quint32, QVector<quint32> >	m_segments;	//key=网段,value=网段活动IP
	QMap<quint32, QHash<quint32, quint64> >	m_agents;	//每个网段的机器，quin32=网段，Hash<ip地址hash键值， 节点唯一标识>
	QMap<quint64, GraphTopoNode>	m_nodes;	//网络拓扑节点，key=唯一标识，value=节点信息

	QVector<ArpEntry>	m_tableArp;			//本地arp表
	QHash<quint32, int>	m_tableArpMacKey;	//本地arp表,key=mac地址hash值，value=arp表索引
	QHash<quint32, int> m_tableArpAddrKey;	//本地arp表，key=ip地址hash值，value=arp表索引

	QLinkedList<quint32>	m_tasks;
	QPair<quint32, int>		m_task;		//探测活动IP，key=网段，value=探测次数
};

class TOPOLOGY_EXPORT QTopologyAssembleRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QTopologyAssembleRunnable(QTopologyProbe* prober, quint32 segment,  QObject* parent=0);
	virtual ~QTopologyAssembleRunnable();

public:
	virtual void run();

signals:
	void finished();

private:
	QTopologyProbe*		m_pProber;
	quint32 m_nSegment;
};

Q_DECLARE_METATYPE(QTopologyProbe::GraphTopoTask)
Q_DECLARE_METATYPE(GraphTopoNode)

#endif // QTOPOLOGYPROBE_H

