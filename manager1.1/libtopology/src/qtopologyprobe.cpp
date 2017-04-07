#include "qtopologyprobe.h"

#include <QNetworkInterface>
#include <QHostAddress>
#include <QList>
#include <QStringList>
#include <QThreadPool>

#include "topocommon.h"

#include "qlogger.h"

#define MAX_ALIVES_PROBE_TICKS	1

QTopologyProbe::QTopologyProbe(QObject *parent)
: QObject(parent)
,m_pIcmp(NULL)
,m_pSnmp(NULL)
{
	qRegisterMetaType<QTopologyProbe::GraphTopoTask>("QTopologyProbe::GraphTopoTask");
	qRegisterMetaType<GraphTopoNode>("GraphTopoNode");
}

QTopologyProbe::~QTopologyProbe()
{
	terminateThread();

	if (m_pSnmp != NULL)
	{
		m_pSnmp->destroy();
		m_pSnmp=NULL;
	}
}

void QTopologyProbe::terminateThread()
{
	if (m_pIcmp != NULL)
	{
		m_pIcmp->destroy();
		m_pIcmp=NULL;
	}
}

void QTopologyProbe::addSegments(quint32 address, bool around)
{
	if (address == GRAPH_TOPO_INVALID_IP_ADDRESS)
	{
		return;
	}

	quint32 nSegment=address&0xffffff00;
	addSegment(nSegment);

	if (around)
	{
		if ((nSegment & 0xff00) != 0)
		{
			addSegment(nSegment-(1<<8));
		}

		if ((nSegment & 0xff00) != 0xff00)
		{
			addSegment(nSegment+(1<<8));
		}
	}
}

void QTopologyProbe::addSegment(quint32 address)
{
	quint32 nSegment=address&0xffffff00;

	if (!m_segments.contains(nSegment))
	{
		logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Add network segment %1").arg(QHostAddress(nSegment).toString()));

		m_segments.insert(nSegment, QVector<quint32>());

		m_tasks.append(nSegment);

		bool bContains=false;
		for (int i=0; i<m_topoTask.segments.size(); i++)
		{
			if ((m_topoTask.segments[i]&0xffffff00) == nSegment)
			{
				bContains=true;
			}
		}

		if (!bContains)
		{
			m_topoTask.segments.append(address);
		}
	}
}

void QTopologyProbe::assembled()
{
	m_task.first=GRAPH_TOPO_INVALID_SEGMENT;

	nextAliveTask();
}

void QTopologyProbe::nextAliveTask()
{
	if (m_task.first != GRAPH_TOPO_INVALID_SEGMENT)
	{
		logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Begin to assemble the segment %1").arg(QHostAddress(m_task.first).toString()));

		QTopologyAssembleRunnable* pRunnable=new QTopologyAssembleRunnable(this, m_task.first);
		pRunnable->setAutoDelete(true);

		connect(pRunnable, SIGNAL(finished()), this, SLOT(assembled()));

		QThreadPool::globalInstance()->start(pRunnable);

		return;
	}

	if (!m_tasks.isEmpty())
	{
		m_task.first=m_tasks.takeFirst();
		m_task.second=0;

		findAlives(m_task.first);
	}
	else
	{
		emit finished(m_topoTask, m_root, m_nodes);
	}
}

void QTopologyProbe::probe(const GraphTopoTask& task)
{
	Q_ASSERT(task.segments.size() > 0);

	GraphTopoNode root;
	root.type=HoldNode;
	root.uid=GRAPH_TOPO_ROOT_HOLD_NODE;

	m_root=root;
	m_nIfIndex=0;

	m_segments.clear();
	m_tasks.clear();
	m_agents.clear();
	m_nodes.clear();

	m_topoTask=task;
	m_topoTask.nodes.clear();

	if (m_pIcmp == NULL)
	{
		m_pIcmp=new QIcmp(this);
		connect(m_pIcmp, SIGNAL(found(quint32, QVector<quint32>)), this, SLOT(foundAlives(quint32, QVector<quint32>)));
	}

	if (m_pSnmp == NULL)
	{
		m_pSnmp=new QSnmp(this);
		connect(m_pSnmp, SIGNAL(finished(int, QVector<QSnmp::SnmpResponse>)), this, SLOT(snmpResponse(int, QVector<QSnmp::SnmpResponse>)));
	}
 
 	for (int i=0; i<task.segments.size(); i++)
 	{
		addSegments(task.segments[i]);
 	}

	assembled();
}

void QTopologyProbe::findAlives(quint32 segment)
{
	logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Find alive IP in segment %1...").arg(QHostAddress(segment).toString()));

	if (m_pIcmp != NULL)
	{
		m_pIcmp->addTask(segment);
	}
}

#ifdef Q_OS_AIX
void QTopologyProbe::setLocalArpEntry(const ServerInfo& info)
{
	m_local=info;
}
#endif

void QTopologyProbe::foundAlives(quint32 segment, QVector<quint32> alives)
{
	if (segment != m_task.first)
	{
		logplusError(LIBTOPOLOGY_LOGGER_STR, QString("Find invalid alive IP in segment %1!").arg(QHostAddress(segment).toString()));
		return;
	}

	logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Find %1 alives in segment %2.").arg(alives.size()).arg(QHostAddress(segment).toString()));

	if (alives.isEmpty())
	{
		if (++m_task.second < MAX_ALIVES_PROBE_TICKS)
		{
			findAlives(m_task.first);
		}
		else
		{
			assembled();
		}
	}
	else
	{
		m_segments[segment]=alives;

		//记录本机ARP表
		m_tableArpMacKey.clear();
		m_tableArpAddrKey.clear();

		bool bLocal=false;	//只能探测局域网MAC地址

		m_tableArp=getArpTable();

#ifdef Q_OS_AIX
		ArpEntry entry;

		QMapIterator<QString, QVector<Server> > it(m_local.servers);
		while (it.hasNext())
		{
			it.next();

			entry.mac=it.key();

			const QVector<Server>& hosts=it.value();
			for (int i=0; i<hosts.size(); i++)
			{
				entry.address=hosts[i].addr;

				m_tableArp.append(entry);
			}
		}
#endif

		for (int i=0; i<m_tableArp.size(); i++)
		{
			m_tableArpMacKey.insertMulti(qHash(m_tableArp[i].mac), i);
			m_tableArpAddrKey.insertMulti(qHash(m_tableArp[i].address), i);

			logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Found MAC=%1, IP=%2").arg(m_tableArp[i].mac, m_tableArp[i].address));

			if (!bLocal)
			{
				if ((QHostAddress(m_tableArp[i].address).toIPv4Address() & 0xffffff00) == segment)
				{
					bLocal=true;
				}
			}
		}

		if (!bLocal)
		{
			assembled();

			return;
		}

		logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Search management devices in segment %1...").arg(QHostAddress(segment).toString()));
		querySysName(segment);
	}
}

void QTopologyProbe::querySysName(quint32 segment)
{
	Q_ASSERT(m_segments.contains(segment));
	Q_ASSERT_NO_NULL(m_pSnmp);

	const QVector<quint32>& alives=m_segments[segment];
	if (m_pSnmp != NULL)
	{
		m_pSnmp->addTask(alives, QSnmp::SysName, ASN_OCTET_STR);
	}
}

void QTopologyProbe::queryOidProperty(quint32 segment, int oidtype, unsigned char dataType, int requestType, int nodeType)
{
	if (!m_agents.contains(segment))
	{
		return;
	}

	QVector<quint32> addrs;

	const QHash<quint32, quint64>& agents=m_agents[segment];
	QHashIterator<quint32, quint64> it(agents);
	while (it.hasNext())
	{
		it.next();

		if (m_nodes.contains(it.value()))
		{
			const GraphTopoNode& node=m_nodes[it.value()];
			if ((nodeType & node.type) != 0)
			{
				foreach (GraphNodeInterface face, node.interfaces)
				{
					const QVector<QString> addresses=face.addresses;
					for (int i=0; i<addresses.size(); i++)
					{
						quint32 nAddr=QHostAddress(addresses[i]).toIPv4Address();
						if ((nAddr&0xffffff00) == segment)
						{
							addrs.append(nAddr);
						}
					}
				}
			}
		}
	}

	m_pSnmp->addTask(addrs, oidtype, dataType, requestType);
}

void QTopologyProbe::querySwitchOidProperty(quint32 segment, int oidtype, unsigned char dataType, int requestType)
{
	queryOidProperty(segment, oidtype, dataType, requestType, SwitchNode|SwitchL3Node);
}

void QTopologyProbe::queryRouterOidProperty(quint32 segment, int oidtype, unsigned char dataType, int requestType)
{
	queryOidProperty(segment, oidtype, dataType, requestType, RouterNode);
}

bool QTopologyProbe::contains(quint32 segment, int nodeType)
{
	if (!m_agents.contains(segment))
	{
		return false;
	}

	const QHash<quint32, quint64>& agents=m_agents[segment];
	QHashIterator<quint32, quint64> it(agents);
	while (it.hasNext())
	{
		it.next();

		if (m_nodes.contains(it.value()))
		{
			const GraphTopoNode& node=m_nodes[it.value()];
			if ((nodeType & node.type) != 0)
			{
				return true;
			}
		}
	}

	return false;
}

quint64 QTopologyProbe::contains(quint32 segment, const QString& addr)
{
	if (!m_agents.contains(segment))
	{
		return GRAPH_TOPO_INVALID_NODE;
	}

	qint32 nKey=qHash(addr);

	//ip地址对应的MAC地址是网络节点的最小MAC地址
	if (m_tableArpAddrKey.contains(nKey))
	{
		QList<int> indexes=m_tableArpAddrKey.values(nKey);
		foreach (int index, indexes)
		{
			Q_ASSERT(index < m_tableArp.size());
			if (m_tableArp[index].address == addr)
			{
				quint64 uid=macStringToUint64(m_tableArp[index].mac);
				if (m_nodes.contains(uid))
				{
					return uid;
				}

				break;
			}
		}
	}

	//ip地址对应的MAC地址不是网络节点的最小MAC地址
	const QHash<quint32, quint64>& agents=m_agents[segment];
	if (agents.contains(nKey))
	{
		QList<quint64> indexes=agents.values(nKey);
		foreach (quint64 index, indexes)
		{
			Q_ASSERT(m_nodes.contains(index));

			const GraphTopoNode& node=m_nodes[index];
			QMapIterator<int, GraphNodeInterface> it(node.interfaces);
			while (it.hasNext())
			{
				it.next();

				const GraphNodeInterface& face=it.value();
				for (int i=0; i<face.addresses.size(); i++)
				{
					if (face.addresses[i] == addr)
					{
						return node.uid;
					}
				}
			}
		}
	}

	return GRAPH_TOPO_INVALID_NODE;
}

const QString QTopologyProbe::macAddrFromIpAddr(const QString& addr) const
{
	quint32 nKey=qHash(addr);

	if (m_tableArpAddrKey.contains(nKey))
	{
		QList<int> indexes=m_tableArpAddrKey.values(nKey);
		foreach (int index, indexes)
		{
			Q_ASSERT(index < m_tableArp.size());
			if (m_tableArp[index].address == addr)
			{
				return m_tableArp[index].mac;
			}
		}
	}
	
	quint32 nAddr=QHostAddress(addr).toIPv4Address();
	//本机ARP信息不位于ARP表
	for (int i=0; i<m_topoTask.segments.size(); i++)
	{
		if (m_topoTask.segments[i] == nAddr)
		{
			return m_topoTask.physAddress;
		}
	}

	return "";
}

const QVector<QString> QTopologyProbe::ipAddrFromMacAddr(const QString& mac, quint32 segment)
{
	QVector<QString> addrs;

	quint32 nKey=qHash(mac);

	if (m_tableArpMacKey.contains(nKey))
	{
		QList<int> indexes=m_tableArpMacKey.values(nKey);
		foreach (int index, indexes)
		{
			Q_ASSERT(index < m_tableArp.size());
			if (m_tableArp[index].mac == mac)
			{
				quint32 nAddr=QHostAddress(m_tableArp[index].address).toIPv4Address();
				if (segment == GRAPH_TOPO_INVALID_SEGMENT || (nAddr&0xffffff00) == segment)
				{
					addrs.append(m_tableArp[index].address);
				}
			}
		}
	}
	else if (mac == m_topoTask.physAddress)
	{
		//本机ARP信息不位于ARP表
		for (int i=0; i<m_topoTask.segments.size(); i++)
		{
			if (segment == GRAPH_TOPO_INVALID_SEGMENT || (m_topoTask.segments[i]&0xffffff00) == segment)
			{
				addrs.append(QHostAddress(m_topoTask.segments[i]).toString());
			}
		}
	}

	return addrs;
}

void QTopologyProbe::foundArpEntry(const QString& mac, const QString& addr)
{
	if (addr.isEmpty() || mac.isEmpty())
	{
		return;
	}

	quint32 nKey=qHash(mac);

	bool bContains=false;
	if (m_tableArpMacKey.contains(nKey))
	{
		QList<int> indexes=m_tableArpMacKey.values(nKey);
		foreach (int index, indexes)
		{
			Q_ASSERT(index < m_tableArp.size());
			if (m_tableArp[index].address == addr)
			{
				bContains=true;
				break;
			}
		}
	}

	if (!bContains)
	{
		ArpEntry entry;
		entry.mac=mac;
		entry.address=addr;
		m_tableArp.append(entry);

		m_tableArpMacKey.insertMulti(qHash(mac), m_tableArp.size()-1);
		m_tableArpAddrKey.insertMulti(qHash(addr), m_tableArp.size()-1);
	}
}

void QTopologyProbe::snmpResponse(int oidType, QVector<QSnmp::SnmpResponse> results)
{
	quint32 nSegment=m_task.first;

	if (!m_segments.contains(nSegment))
	{
		logplusDebug(LIBTOPOLOGY_LOGGER_STR, 
			QString("Response from anywhere which is invalid! An error occurred! Reprobe the subnet."));
		probe(m_topoTask);

		return;
	}

	logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Response from the segment %1.").arg(QHostAddress(nSegment).toString()));

	if (results.size() == 0)
	{
		if (++m_task.second < MAX_ALIVES_PROBE_TICKS)
		{
			findAlives(m_task.first);
		}
		else 
		{
			if (oidType == QSnmp::SysName)
			{
				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Failed to get any snmp information in subnet %1").arg(QHostAddress(nSegment).toString()));
				nextAliveTask();
			}
			else
			{
				logplusDebug(LIBTOPOLOGY_LOGGER_STR, 
					QString("The network configrue has been changed! Reprobe the subnet %1.").arg(QHostAddress(nSegment).toString()));
				probe(m_topoTask);	//网络发生改变，重新探测网络
			}
		}

		return;
	}

	for (int i=0; i<results.size(); i++)
	{
		const QSnmp::SnmpResponse& result=results[i];

		quint64 uid;
		if ((uid=contains(nSegment, result.addr)) == GRAPH_TOPO_INVALID_NODE)
		{
			QString strPhysAddr=macAddrFromIpAddr(result.addr);
			if (strPhysAddr.isEmpty())
			{
				logplusDebug(LIBTOPOLOGY_LOGGER_STR, 
					QString("Failed to find MAC address from IP:%1! An error occurred! Reprobe the subnet.").arg(result.addr));

				probe(m_topoTask);	//探测有误, 重新探测该网段
				return;
			}

			uid=addHostToTopo(strPhysAddr, nSegment);
			if (uid == GRAPH_TOPO_INVALID_NODE)
			{
				continue;
			}

			m_agents[nSegment].insert(qHash(result.addr), uid);
		}

		Q_ASSERT(m_nodes.contains(uid));
		GraphTopoNode& node=m_nodes[uid];

		switch (result.oid)
		{
		case QSnmp::SysName:
			{
				node.name=result.values[0].toString();

				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:SysName=%2").arg(result.addr).arg(result.values[0].toString()));
			}
			break;
		case QSnmp::SysObjectID:
			{
				node.oid=result.values[0].toString();

				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:SysObjectID=%2").arg(result.addr).arg(result.values[0].toString()));
			}
			break;
		case QSnmp::SysDescr:
			{
				node.desc=result.values[0].toString();

				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:SysDescr=%2").arg(result.addr).arg(result.values[0].toString()));
			}
			break;
		case QSnmp::SysServices:
			{
				node.services=result.values[0].toUInt();

				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:SysServices=%2").arg(result.addr).arg(node.services, 0, 16));
			}
			break;
		case QSnmp::IfNumber:
			{
				node.ifNumber=result.values[0].toUInt();

				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:IfNumber=%2").arg(result.addr).arg(node.ifNumber));
			}
			break;
		case QSnmp::IpForwarding:
			{
				node.ipForwarding=result.values[0].toUInt();

				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:IpForwarding=%2").arg(result.addr).arg(node.ipForwarding));

				if (node.desc.contains("firewall", Qt::CaseInsensitive))
				{
					node.type=FirewallNode;

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:Firewall node").arg(result.addr));
				}
				else
				{
					if (node.ifNumber > 1)
					{
						if (node.ipForwarding == 2)
						{
							node.type=SwitchNode;

							logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:Switch node").arg(result.addr));
						}
						else if (node.ipForwarding == 1)
						{
							node.type=RouterNode;

							logplusDebug(LIBTOPOLOGY_LOGGER_STR, 
								QString("%1:Route node(default) or Tree layer switch node, check later.").arg(result.addr));
						}
					}
				}
			}
			break;
		case QSnmp::IfIndex:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					bool bOk;
					int nOid=result.values[j].toString().toInt(&bOk);
					int nInterface=result.values[j+1].toInt();

					if (!bOk)
					{
						nOid=j/2+1;
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IfIndex=%3").arg(result.addr).arg(nOid).arg(nInterface));

					node.interfaces[nOid].ifIndex=nInterface;
				}
			}
			break;
		case QSnmp::IfDescr:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					bool bOk;
					int nOid=result.values[j].toString().toInt(&bOk);
					QString strDesc=result.values[j+1].toString();

					if (!bOk)
					{
						nOid=j/2+1;
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IfDescr=%3").arg(result.addr).arg(nOid).arg(strDesc));

					node.interfaces[nOid].desc=strDesc;
				}
			}
			break;
		case QSnmp::IfType:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					bool bOk;
					int nOid=result.values[j].toString().toInt(&bOk);
					int nType=result.values[j+1].toInt();

					if (!bOk)
					{
						nOid=j/2+1;
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IfType=%3").arg(result.addr).arg(nOid).arg(nType));

					node.interfaces[nOid].type=nType;
				}
			}
			break;
		case QSnmp::IfSpeed:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					bool bOk;
					int nOid=result.values[j].toString().toInt(&bOk);
					quint32 nSpeed=result.values[j+1].toUInt();

					if (!bOk)
					{
						nOid=j/2+1;
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IfSpeed=%3").arg(result.addr).arg(nOid).arg(nSpeed));

					node.interfaces[nOid].speed=nSpeed;
				}
			}
			break;
		case QSnmp::IfPhysAddress:
			{
				quint64 nMinUid=uid;
				quint64 nTemp;
				for (int j=0; j<result.values.size();j+=2)
				{
					bool bOk;
					int nOid=result.values[j].toString().toInt(&bOk);
					QString strMac=octetStrToString(result.values[j+1].toString());

					if (!bOk)
					{
						nOid=j/2+1;
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IfPhysAddress=%3").arg(result.addr).arg(nOid).arg(strMac));

					node.interfaces[nOid].physAddress=strMac;

					if (node.interfaces[nOid].type == 6)
					{
						nTemp=macStringToUint64(strMac);
						if (nTemp != 0 && nTemp<nMinUid)
						{
							nMinUid=nTemp;
						}
					}
				}

				if (nMinUid != uid)
				{
					GraphTopoNode nd=node;
					nd.uid=nMinUid;

					m_nodes.remove(uid);
					m_nodes.insert(nMinUid, nd);

					QHash<quint32, quint64>& agents=m_agents[nSegment];
					QMutableHashIterator<quint32, quint64> it(agents);
					while (it.hasNext())
					{
						it.next();
						if (it.value() == uid)
						{
							quint32 nKey=it.key();
							it.remove();

							agents.insertMulti(nKey, nMinUid);

							break;
						}
					}
				}
			}
			break;
		case QSnmp::IfOperStatus:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					bool bOk;
					int nOid=result.values[j].toString().toInt(&bOk);
					int nStatus=result.values[j+1].toInt();

					if (!bOk)
					{
						nOid=j/2+1;
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IfOperStatus=%3").arg(result.addr).arg(nOid).arg(nStatus));

					node.interfaces[nOid].operStatus=nStatus;
				}
			}
			break;
		case QSnmp::IpAdEntAddr:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					QString strOid=result.values[j].toString();
					QString strAddr=result.values[j+1].toString();

					if (strOid.isEmpty())
					{
						strOid=QString("%1").arg(j/2+1);
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IpAdEntAddr=%3").arg(result.addr).arg(strOid).arg(strAddr));

					node.ipEntAddrs[strOid].address=strAddr;

					addSegments(QHostAddress(strAddr).toIPv4Address());
				}
			}
			break;
		case QSnmp::IpAdEntIfIndex:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					QString strOid=result.values[j].toString();
					int nIndex=result.values[j+1].toInt();

					if (strOid.isEmpty())
					{
						strOid=QString("%1").arg(j/2+1);
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IpAdEntIfIndex=%3").arg(result.addr).arg(strOid).arg(nIndex));

					node.ipEntAddrs[strOid].ifIndex=nIndex;
				}
			}
			break;
		case QSnmp::IpAdEntNetMask:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					QString strOid=result.values[j].toString();
					QString strAddr=result.values[j+1].toString();

					if (strOid.isEmpty())
					{
						strOid=QString("%1").arg(j/2+1);
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IpAdEntNetMask=%3").arg(result.addr).arg(strOid).arg(strAddr));

					node.ipEntAddrs[strOid].netmask=strAddr;
				}
			}
			break;
		case QSnmp::IpRouteDest:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					bool bOk;
					int nOid=result.values[j].toString().toInt(&bOk);
					QString strAddr=result.values[j+1].toString();

					if (!bOk)
					{
						nOid=j/2+1;
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IpRouteDest=%3").arg(result.addr).arg(nOid).arg(strAddr));

					node.ipRoutes[nOid].dest=strAddr;
				}
			}
			break;
		case QSnmp::IpRouteIfIndex:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					bool bOk;
					int nOid=result.values[j].toString().toInt(&bOk);
					int nIndex=result.values[j+1].toInt();

					if (!bOk)
					{
						nOid=j/2+1;
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IpRouteIfIndex=%3").arg(result.addr).arg(nOid).arg(nIndex));

					node.ipRoutes[nOid].ifIndex=nIndex;
				}
			}
			break;
		case QSnmp::IpRouteNextHop:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					bool bOk;
					int nOid=result.values[j].toString().toInt(&bOk);
					QString strAddr=result.values[j+1].toString();

					if (!bOk)
					{
						nOid=j/2+1;
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IpRouteNextHop=%3").arg(result.addr).arg(nOid).arg(strAddr));

					node.ipRoutes[nOid].nextHop=strAddr;
				}
			}
			break;
		case QSnmp::IpRouteType:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					bool bOk;
					int nOid=result.values[j].toString().toInt(&bOk);
					int nType=result.values[j+1].toInt();

					if (!bOk)
					{
						nOid=j/2+1;
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IpRouteType=%3").arg(result.addr).arg(nOid).arg(nType));

					node.ipRoutes[nOid].routeType=nType;
				}
			}
			break;
		case QSnmp::IpRouteMask:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					bool bOk;
					int nOid=result.values[j].toString().toInt(&bOk);
					QString strAddr=result.values[j+1].toString();

					if (!bOk)
					{
						nOid=j/2+1;
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IpRouteMask=%3").arg(result.addr).arg(nOid).arg(strAddr));

					node.ipRoutes[nOid].routeMask=strAddr;
				}
			}
			break;
		case QSnmp::Dot1dBaseBridgeAddress:
			{
				if (!result.values.isEmpty())
				{
					if (node.type == RouterNode)
					{
						node.type=SwitchL3Node;
					}

					node.bridgeBase.address=octetStrToString(result.values[0].toString());

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:Dot1dBaseBridgeAddress=%2").arg(result.addr).arg(node.bridgeBase.address));
				}
			}
			break;
		case QSnmp::Dot1dBaseNumPorts:
			{
				node.bridgeBase.numPorts=result.values[0].toUInt();

				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:Dot1dBaseNumPorts=%2").arg(result.addr).arg(node.bridgeBase.numPorts));
			}
			break;
		case QSnmp::Dot1dBaseType:
			{
				node.bridgeBase.type=result.values[0].toUInt();

				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:Dot1dBaseType=%2").arg(result.addr).arg(node.bridgeBase.type));
			}
			break;
		case QSnmp::Dot1dBasePort:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					int nOid=j/2+1;
					int nPort=result.values[j+1].toInt();

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:index=%2,Dot1dBasePort=%3").arg(result.addr).arg(nOid).arg(nPort));

					node.bridgeBase.portTable[nOid].port=nPort;
				}
			}
			break;
		case QSnmp::Dot1dBasePortIfIndex:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					int nOid=j/2+1;
					int nPort=result.values[j+1].toInt();

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:index=%2,Dot1dBasePort=%3").arg(result.addr).arg(nOid).arg(nPort));

					node.bridgeBase.portTable[nOid].ifIndex=nPort;
				}
			}
			break;
		case QSnmp::Dot1dTpFdbAddress:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					int nOid=j/2+1;
					QString strMac=octetStrToString(result.values[j+1].toString());

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:index=%2,Dot1dTpFdbAddress=%3").arg(result.addr).arg(nOid).arg(strMac));

					node.fdb[nOid].address=strMac;
				}
			}
			break;
		case QSnmp::Dot1dTpFdbPort:
			{
				for (int j=0; j<result.values.size();j+=2)
				{
					int nOid=j/2+1;
					int nPort=result.values[j+1].toInt();

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:index=%2,Dot1dTpFdbPort=%3").arg(result.addr).arg(nOid).arg(nPort));

					node.fdb[nOid].port=nPort;
				}
			}
			break;
		case QSnmp::Dot1dTpFdbStatus:
			{
				for (int j=0; j<result.values.size(); j+=2)
				{
					int nOid=j/2+1;
					int nStatus=result.values[j+1].toInt();

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:index=%2,Dot1dTpFdbStatus=%3").arg(result.addr).arg(nOid).arg(nStatus));

					node.fdb[nOid].status=nStatus;
				}
			}
			break;
		case QSnmp::IpNetToMediaIfIndex:
			{
				for (int j=0; j<result.values.size(); j+=2)
				{
					QString strOid=result.values[j].toString();
					int nIndex=result.values[j+1].toInt();

					if (strOid.isEmpty())
					{
						strOid=QString("%1").arg(j/2+1);
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IpNetToMediaIfIndex=%3").arg(result.addr).arg(strOid).arg(nIndex));

					node.ipMedia[strOid].ifIndex=nIndex;
				}
			}
			break;
		case QSnmp::IpNetToMediaPhysAddress:
			{
				for (int j=0; j<result.values.size(); j+=2)
				{
					QString strOid=result.values[j].toString();
					QString strAddr=octetStrToString(result.values[j+1].toString());

					if (strOid.isEmpty())
					{
						strOid=QString("%1").arg(j/2+1);
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IpNetToMediaPhysAddress=%3").arg(result.addr).arg(strOid).arg(strAddr));

					node.ipMedia[strOid].physAddress=strAddr;
				}
			}
			break;
		case QSnmp::IpNetToMediaNetAddress:
			{
				for (int j=0; j<result.values.size(); j+=2)
				{
					QString strOid=result.values[j].toString();
					QString strAddr=result.values[j+1].toString();

					if (strOid.isEmpty())
					{
						strOid=QString("%1").arg(j/2+1);
					}

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:oid=%2,IpNetToMediaNetAddress=%3").arg(result.addr).arg(strOid).arg(strAddr));

					node.ipMedia[strOid].netAddress=strAddr;
				}

				QMapIterator<QString, GraphNodeIpNetToMedia> it(node.ipMedia);
				while (it.hasNext())
				{
					it.next();

					const GraphNodeIpNetToMedia& media=it.value();
					foundArpEntry(media.physAddress, media.netAddress);
				}
			}
			break;
		default:
			break;
		}
	}

	switch (oidType)
	{
	case QSnmp::SysName:
		{
			queryOidProperty(nSegment, QSnmp::SysObjectID, ASN_OBJECT_ID);
		}
		break;
	case QSnmp::SysObjectID:
		{
			queryOidProperty(nSegment, QSnmp::SysDescr, ASN_OCTET_STR);
		}
		break;
	case QSnmp::SysDescr:
		{
			queryOidProperty(nSegment, QSnmp::SysServices, ASN_INTEGER);
		}
		break;
	case QSnmp::SysServices:
		{
			queryOidProperty(nSegment, QSnmp::IfNumber, ASN_INTEGER);
		}
		break;
	case QSnmp::IfNumber:
		{
			queryOidProperty(nSegment, QSnmp::IpForwarding, ASN_INTEGER);
		}
		break;
	case QSnmp::IpForwarding:
		{
			if (contains(nSegment, SwitchNode|SwitchL3Node|RouterNode))
			{
				queryOidProperty(nSegment, QSnmp::IfIndex, ASN_INTEGER, QSnmp::SnmpGetNext, SwitchNode|SwitchL3Node|RouterNode);
			}
			else
			{
				nextAliveTask();
			}
		}
		break;
	case QSnmp::IfIndex:
		{
			queryOidProperty(nSegment, QSnmp::IfDescr, ASN_OCTET_STR, QSnmp::SnmpGetNext, SwitchNode|SwitchL3Node|RouterNode);
		}
		break;
	case QSnmp::IfDescr:
		{
			queryOidProperty(nSegment, QSnmp::IfType, ASN_INTEGER, QSnmp::SnmpGetNext, SwitchNode|SwitchL3Node|RouterNode);
		}
		break;
	case QSnmp::IfType:
		{
			queryOidProperty(nSegment, QSnmp::IfSpeed, ASN_GAUGE, QSnmp::SnmpGetNext, SwitchNode|SwitchL3Node|RouterNode);
		}
		break;
	case QSnmp::IfSpeed:
		{
			queryOidProperty(nSegment, QSnmp::IfPhysAddress, ASN_OCTET_STR, QSnmp::SnmpGetNext, SwitchNode|SwitchL3Node|RouterNode);
		}
		break;
	case QSnmp::IfPhysAddress:
		{
			queryOidProperty(nSegment, QSnmp::IfOperStatus, ASN_INTEGER, QSnmp::SnmpGetNext, SwitchNode|SwitchL3Node|RouterNode);
		}
		break;
	case QSnmp::IfOperStatus:
		{
			queryOidProperty(nSegment, QSnmp::IpAdEntAddr, ASN_IPADDRESS, QSnmp::SnmpGetNext, SwitchNode|SwitchL3Node|RouterNode);
		}
		break;
	case QSnmp::IpAdEntAddr:
		{
			queryOidProperty(nSegment, QSnmp::IpAdEntIfIndex, ASN_INTEGER, QSnmp::SnmpGetNext, SwitchNode|SwitchL3Node|RouterNode);
		}
		break;
	case QSnmp::IpAdEntIfIndex:
		{
			queryOidProperty(nSegment, QSnmp::IpAdEntNetMask, ASN_IPADDRESS, QSnmp::SnmpGetNext, SwitchNode|SwitchL3Node|RouterNode);
		}
		break;
	case QSnmp::IpAdEntNetMask:
		{
			if (contains(nSegment, RouterNode|SwitchL3Node))
			{
				queryOidProperty(nSegment, QSnmp::IpRouteDest, ASN_IPADDRESS, QSnmp::SnmpGetNext, RouterNode|SwitchL3Node);
			}
			else
			{
				querySwitchOidProperty(nSegment, QSnmp::Dot1dBaseBridgeAddress, ASN_OCTET_STR);
			}
		}
		break;
	case QSnmp::IpRouteDest:
		{
			queryOidProperty(nSegment, QSnmp::IpRouteIfIndex, ASN_INTEGER, QSnmp::SnmpGetNext, RouterNode|SwitchL3Node);
		}
		break;
	case QSnmp::IpRouteIfIndex:
		{
			queryOidProperty(nSegment, QSnmp::IpRouteNextHop, ASN_IPADDRESS, QSnmp::SnmpGetNext, RouterNode|SwitchL3Node);
		}
		break;
	case QSnmp::IpRouteNextHop:
		{
			queryOidProperty(nSegment, QSnmp::IpRouteType, ASN_INTEGER, QSnmp::SnmpGetNext, RouterNode|SwitchL3Node);
		}
		break;
	case QSnmp::IpRouteType:
		{
			queryOidProperty(nSegment, QSnmp::IpRouteMask, ASN_IPADDRESS, QSnmp::SnmpGetNext, RouterNode|SwitchL3Node);
		}
		break;
	case QSnmp::IpRouteMask:
		{
			if (contains(nSegment, RouterNode|SwitchL3Node|SwitchNode))
			{
				queryOidProperty(nSegment, QSnmp::Dot1dBaseBridgeAddress, ASN_OCTET_STR, QSnmp::SnmpAsynGet, RouterNode|SwitchL3Node|SwitchNode);
			}
			else
			{
				queryOidProperty(nSegment, QSnmp::IpNetToMediaIfIndex, ASN_INTEGER, QSnmp::SnmpGetNext, SwitchNode|SwitchL3Node|RouterNode);
			}
		}
		break;
	case QSnmp::Dot1dBaseBridgeAddress:
		{
			if (contains(nSegment, SwitchL3Node|SwitchNode))
			{
				querySwitchOidProperty(nSegment, QSnmp::Dot1dBaseNumPorts, ASN_INTEGER);
			}
			else
			{
				queryOidProperty(nSegment, QSnmp::IpNetToMediaIfIndex, ASN_INTEGER, QSnmp::SnmpGetNext, SwitchNode|SwitchL3Node|RouterNode);
			}
		}
		break;
	case QSnmp::Dot1dBaseNumPorts:
		{
			querySwitchOidProperty(nSegment, QSnmp::Dot1dBaseType, ASN_INTEGER);
		}
		break;
	case QSnmp::Dot1dBaseType:
		{
			querySwitchOidProperty(nSegment, QSnmp::Dot1dBasePort, ASN_INTEGER, QSnmp::SnmpGetNext);
		}
		break;
	case QSnmp::Dot1dBasePort:
		{
			querySwitchOidProperty(nSegment, QSnmp::Dot1dBasePortIfIndex, ASN_INTEGER, QSnmp::SnmpGetNext);
		}
		break;
	case QSnmp::Dot1dBasePortIfIndex:
		{
			querySwitchOidProperty(nSegment, QSnmp::Dot1dTpFdbAddress, ASN_OCTET_STR, QSnmp::SnmpGetNext);
		}
		break;
	case QSnmp::Dot1dTpFdbAddress:
		{
			querySwitchOidProperty(nSegment, QSnmp::Dot1dTpFdbPort, ASN_INTEGER, QSnmp::SnmpGetNext);
		}
		break;
	case QSnmp::Dot1dTpFdbPort:
		{
			querySwitchOidProperty(nSegment, QSnmp::Dot1dTpFdbStatus, ASN_INTEGER, QSnmp::SnmpGetNext);
		}
		break;
	case QSnmp::Dot1dTpFdbStatus:
		{
			queryOidProperty(nSegment, QSnmp::IpNetToMediaIfIndex, ASN_INTEGER, QSnmp::SnmpGetNext, SwitchNode|SwitchL3Node|RouterNode);
		}
		break;
	case QSnmp::IpNetToMediaIfIndex:
		{
			queryOidProperty(nSegment, QSnmp::IpNetToMediaPhysAddress, ASN_OCTET_STR, QSnmp::SnmpGetNext, SwitchNode|SwitchL3Node|RouterNode);
		}
		break;
	case QSnmp::IpNetToMediaPhysAddress:
		{
			queryOidProperty(nSegment, QSnmp::IpNetToMediaNetAddress, ASN_IPADDRESS, QSnmp::SnmpGetNext, SwitchNode|SwitchL3Node|RouterNode);
		}
		break;
	case QSnmp::IpNetToMediaNetAddress:
		{
			nextAliveTask();
		}
		break;
	default:
		break;
	}
}

QString QTopologyProbe::octetStrToString(const QString& str)
{
//	char strMac[18];
	QByteArray buffer=str.toLatin1();
	char* pBuf=buffer.data();

// #ifndef Q_OS_AIX
// 	sprintf(strMac, "%02X:%02X:%02X:%02X:%02X:%02X", 
// 		*(unsigned char*)pBuf, *(unsigned char*)(pBuf+1), *(unsigned char*)(pBuf+2), 
// 		*(unsigned char*)(pBuf+3), *(unsigned char*)(pBuf+4), *(unsigned char*)(pBuf+5));
// #else
// 	sprintf(strMac, "%hhX:%hhX:%hhX:%hhX:%hhX:%hhX", 
// 		*(unsigned char*)pBuf, *(unsigned char*)(pBuf+1), *(unsigned char*)(pBuf+2), 
// 		*(unsigned char*)(pBuf+3), *(unsigned char*)(pBuf+4), *(unsigned char*)(pBuf+5));
//#endif

//	return strMac;

	QString strMacAddress;
	for (int i=0; i<6; i++)
	{
		strMacAddress+=(i==0? QString("%1")
			.arg((unsigned char)*(pBuf+i), 2, 16, QLatin1Char('0')):QString(":%1").arg((unsigned char)*(pBuf+i), 2, 16, QLatin1Char('0')));
	}

	return strMacAddress.toUpper();
}

void QTopologyProbe::assemble(quint32 segment)
{
	if (segment == GRAPH_TOPO_INVALID_SEGMENT)
	{
		return;
	}

	logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Assemble topology graph in subnet %1...").arg(QHostAddress(segment).toString()));

	if (m_agents.contains(segment))
	{
		//查找路由器
		bool bFind=false;
		QList<quint64> uids;
		quint64 uid=GRAPH_TOPO_INVALID_NODE;
		const QHash<quint32, quint64> agents=m_agents[segment];
		foreach (quint64 agent, agents)
		{
			Q_ASSERT(m_nodes.contains(agent));
			const GraphTopoNode& node=m_nodes[agent];
			if (node.type == RouterNode && !bFind)
			{
				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Find a router name=%1").arg(node.name));
				
				m_root.peers.insert(++m_nIfIndex, QPair<quint64, int>(node.uid, 0));

				uid=node.uid;

				bFind=true;
			}
			else
			{
				uids.append(node.uid);
			}
		}

		if (bFind)
		{
			Q_ASSERT(m_nodes.contains(uid));

			logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Find a router uid=%1 in subnet %2...").arg(m_nodes[uid].name).arg(QHostAddress(segment).toString()));
			assembleRouter(m_nodes[uid], segment, uids);

			return;
		}
	}

	logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Don't find a router in subnet %1...").arg(QHostAddress(segment).toString()));

	if (m_agents.contains(segment))
	{
		//不存在路由器，查找根交换机
		const QHash<quint32, quint64> agents=m_agents[segment];
		quint64 uid=GRAPH_TOPO_INVALID_NODE;
		bool bFindSwitch=false;
		bool bFindRootSwitch=false;
		foreach (quint64 agent, agents)
		{
			Q_ASSERT(m_nodes.contains(agent));
			const GraphTopoNode& node=m_nodes[agent];
			if (node.type != SwitchNode && node.type != SwitchL3Node)
			{
				continue;
			}

			bFindSwitch=true;

			if (!bFindRootSwitch)
			{
				uid=node.uid;
			}

			foreach (GraphNodeFdb fdb, node.fdb)
			{
				if (fdb.address == m_topoTask.physAddress)
				{
					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Find a root Switch name=%1").arg(node.name));

					m_root.peers.insert(++m_nIfIndex, QPair<quint64, int>(node.uid, 0));

					bFindRootSwitch=true;
					uid=node.uid;

					break;
				}
			}

			if (bFindRootSwitch)
			{
				break;
			}
		}

		if (bFindSwitch)
		{
			if (!bFindRootSwitch)
			{
				const GraphTopoNode& node=m_nodes[uid];
				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Failed to find a root switch, Select one of them as root switch ,named=%1").arg(node.name));

				m_root.peers.insert(++m_nIfIndex, QPair<quint64, int>(node.uid, 0));
			}

			Q_ASSERT(uid != GRAPH_TOPO_INVALID_NODE);
			Q_ASSERT(m_nodes.contains(uid));

			QList<quint64> uids;
			QList<quint64> hosts;
			quint64 id;

			QVector<quint32> alives=m_segments[segment];
			for (int j=0; j<alives.size(); j++)
			{
				QString strAddr=QHostAddress(alives[j]).toString();
				QString strPhysAddr=macAddrFromIpAddr(strAddr);
				if (strPhysAddr.isEmpty())
				{
					logplusDebug(LIBTOPOLOGY_LOGGER_STR, 
						QString("Failed to find MAC address from IP:%1! An error occurred! Reprobe the subnet.").arg(strAddr));
					probe(m_topoTask);	//探测有误
					return;
				}

				id=addHostToTopo(strPhysAddr, segment);

				if (id != GRAPH_TOPO_INVALID_NODE && id != uid)
				{
					Q_ASSERT(m_nodes.contains(id));

					GraphTopoNode& node=m_nodes[id];
					if ((node.type&(HoldNode|RouterNode|SwitchNode|SwitchL3Node)) == 0)
					{
						hosts.append(node.uid);
					}
					else if ((node.type&(RouterNode|SwitchNode|SwitchL3Node)) != 0)
					{
						uids.append(node.uid);
					}
				}
			}

			assembleSwitchPerIfIndex(m_nodes[uid], -1, uids, segment, hosts);

			return;
		}
	}

	//不存在可管理的交换设备，按逻辑连接设备
	logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Assemble hub in subnet %1...").arg(QHostAddress(segment).toString()));
	Q_ASSERT(m_segments.contains(segment));
	QList<quint64> uids;
	QList<quint64> hosts;

	QVector<quint32> alives=m_segments[segment];
	for (int j=0; j<alives.size(); j++)
	{
		QString strAddr=QHostAddress(alives[j]).toString();
		QString strPhysAddr=macAddrFromIpAddr(strAddr);
		if (strPhysAddr.isEmpty())
		{
			if (alives.size() != 1)
			{
				logplusError(LIBTOPOLOGY_LOGGER_STR, 
					QString("Failed to find MAC address from IP:%1! An error occurred! Reprobe the subnet.").arg(strAddr));

				probe(m_topoTask);	//探测有误
			}
			else
			{
				assembled();	//网段仅存在本机，可能未接入网络，获取不到ARP表
			}
			return;
		}

		quint64 uid=addHostToTopo(strPhysAddr, segment);

		hosts.append(uid);
	}

	assembleHub(m_root, ++m_nIfIndex, uids, segment, hosts);

	return;
}

void QTopologyProbe::assembleHub(GraphTopoNode& parent, int ifIndex, QList<quint64>& uids, quint32 segment, QList<quint64>& hosts)
{
	if (!uids.isEmpty())
	{
		//组装可管理设备
		assembleRootSwitch(parent, ifIndex, uids, segment, hosts);
	}
	else if (!hosts.isEmpty())
	{
		//仅有主机
		GraphTopoNode hub;
		hub.type=HubNode;
		hub.uid=GRAPH_TOPO_INVALID_NODE;
		int nIfIndex=0;

		foreach (quint64 uid, hosts)
		{
			GraphNodeInterface face;
			uid=addHostToTopo(uint64ToMacString(uid), segment, face);

			if (uid == GRAPH_TOPO_INVALID_NODE)
			{
				continue;
			}

			Q_ASSERT(m_nodes.contains(uid));
			hub.uid=(hub.uid<uid? hub.uid:uid);

			hub.peers.insert(++nIfIndex, QPair<quint64, int>(uid, face.ifIndex));
		}

		hub.uid|=(((quint64)hub.peers.size()) << 48);

		if (!m_nodes.contains(hub.uid))
		{
			m_nodes.insert(hub.uid, hub);
		}
		else
		{
			Q_ASSERT(m_nodes.contains(hub.uid));
			GraphTopoNode& node=m_nodes[hub.uid];

			QMapIterator<int, QPair<quint64, int> > at(hub.peers);
			while (at.hasNext())
			{
				at.next();
				const QPair<quint64, int>& peer=at.value();

				bool bFind=false;
				QMapIterator<int, QPair<quint64, int> > ator(node.peers);
				while (ator.hasNext())
				{
					ator.next();

					if (ator.value() == peer)
					{
						bFind=true;
						break;
					}
				}

				nIfIndex=0;
				if (!bFind)
				{
					while (node.peers.contains(++nIfIndex));
					node.peers.insert(nIfIndex, peer);
				}
			}
		}

		Q_ASSERT(m_nodes.contains(hub.uid));
		GraphTopoNode& node=m_nodes[hub.uid];

		QMapIterator<int, QPair<quint64, int> > at(node.peers);
		while (at.hasNext())
		{
			at.next();

			const QPair<quint64, int>& peer=at.value();
			Q_ASSERT(m_nodes.contains(peer.first));

			logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Hub uid=%1 link to Host %2 MAC=%3")
				.arg(QString::number(node.uid, 16)).arg(ipAddrFromMacAddr(m_nodes[peer.first].interfaces[peer.second].physAddress, segment)[0])
				.arg(m_nodes[peer.first].interfaces[peer.second].physAddress));

			GraphTopoNode& nd=m_nodes[peer.first];

 			if (!nd.peers.contains(peer.second) || !m_nodes.contains(nd.peers[peer.second].first))
 			{
 				nd.peers.insert(peer.second, QPair<quint64, int>(node.uid, at.key()));	
 			}
		}

		parent.peers.insert(ifIndex, QPair<quint64, int>(node.uid, ++nIfIndex));

		if (parent.type != HoldNode)
		{
			node.peers.insert(nIfIndex, QPair<quint64, int>(parent.uid, ifIndex));

			QString strDevice;
			if (parent.type == RouterNode)
			{
				strDevice="Router";
			}
			else if (parent.type == SwitchNode)
			{
				strDevice="Switch";
			}
			else if (parent.type == SwitchL3Node)
			{
				strDevice="Switch L3";
			}

			logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1 %2:%3 link to Hub %4:%5").arg(strDevice).arg(parent.name).arg(ifIndex)
				.arg(QString::number(node.uid, 16)).arg(QString::number(nIfIndex)));
		}
	}
}

void QTopologyProbe::assembleRouter(GraphTopoNode& parent, quint32 segment, QList<quint64> uids)
{
	Q_ASSERT(parent.type==RouterNode);

	QMapIterator<QString, GraphNodeIpEntAddr> it(parent.ipEntAddrs);
	while (it.hasNext())
	{
		it.next();

		const GraphNodeIpEntAddr& ent=it.value();
		quint32 nSegment=QHostAddress(ent.address).toIPv4Address()&0xffffff00;

		if (segment == nSegment)
		{
			QList<quint64> hosts;

			QVector<quint32> alives=m_segments[nSegment];
			quint64 uid;
			for (int j=0; j<alives.size(); j++)
			{
				QString strAddr=QHostAddress(alives[j]).toString();
				QString strPhysAddr=macAddrFromIpAddr(strAddr);
				if (strPhysAddr.isEmpty())
				{
					logplusDebug(LIBTOPOLOGY_LOGGER_STR, 
						QString("Failed to find MAC address from IP:%1! An error occurred! Reprobe the subnet.").arg(strAddr));
					probe(m_topoTask);	//探测有误
					return;
				}

				GraphNodeInterface face;
				uid=addHostToTopo(strPhysAddr, segment, face);

				if (uid != GRAPH_TOPO_INVALID_NODE)
				{
					Q_ASSERT(m_nodes.contains(uid));

					GraphTopoNode& node=m_nodes[uid];
					if ((node.type&(HoldNode|RouterNode|SwitchNode|SwitchL3Node)) != 0)
					{
						hosts.append(node.uid);
					}
				}
			}

			assembleRootSwitch(parent, ent.ifIndex, uids, nSegment, hosts);
		}
		else
		{
			addSegments(nSegment);
		}
	}
}

void QTopologyProbe::assembleRootSwitch(GraphTopoNode& parent, int ifIndex, QList<quint64>& uids, quint32 segment, QList<quint64>& hosts)
{
	Q_ASSERT(parent.type != HoldNode && parent.type != HubNode);

	QHash<quint32, QString> physAddresses;
	foreach (GraphNodeInterface face, parent.interfaces)
	{
		if (face.operStatus == 1)
		{
			physAddresses.insertMulti(qHash(face.physAddress), face.physAddress);
		}
	}

	//检查是否连接可管理交换机
	QMutableListIterator<quint64> it(uids);
	while (it.hasNext())
	{
		quint64 uid=it.next();
		Q_ASSERT(m_nodes.contains(uid));

		GraphTopoNode& node=m_nodes[uid];
		if (node.type != SwitchNode && node.type != SwitchL3Node)
		{
			continue;
		}

		QMap<int, bool> ports;

		QMapIterator<int, GraphNodeFdb> iter(node.fdb);
		while (iter.hasNext())
		{
			iter.next();

			const GraphNodeFdb& fdb=iter.value();
			if (fdb.status == 3)
			{
				if (!ports.contains(fdb.port))
				{
					ports.insert(fdb.port, true);
				}

				if (ports[fdb.port])
				{
					if (!physAddresses.contains(qHash(fdb.address)))
					{
						ports[fdb.port]=false;
					}
					else
					{
						QList<QString> addrs=physAddresses.values(qHash(fdb.address));
						if (!addrs.contains(fdb.address))
						{
							ports[fdb.port]=false;
						}
					}
				}
			}
		}

		QMapIterator<int, bool> at(ports);
		while (at.hasNext())
		{
			at.next();

			if (at.value())
			{
				//父节点端口连接可管理交换机
				node.peers.insert(at.key(), QPair<quint64, int>(parent.uid, ifIndex));
				parent.peers.insert(ifIndex, QPair<quint64, int>(node.uid, at.key()));

				QString strDevice;
				if (parent.type == RouterNode)
				{
					strDevice="Router";
				}
				else if (parent.type == SwitchNode)
				{
					strDevice="Switch";
				}
				else if (parent.type == SwitchL3Node)
				{
					strDevice="Switch L3";
				}
				logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1 %2:%3 link to Switch %4:%5")
					.arg(strDevice).arg(parent.name).arg(ifIndex).arg(node.name).arg(at.key()));

				it.remove();

				assembleSwitchPerIfIndex(node, at.key(), uids, segment, hosts);

				return;
			}
		}
	}

	//父节点端口连接hub
	assembleHub(parent, ifIndex, uids, segment, hosts);
}

void QTopologyProbe::assembleSwitchPerIfIndex(GraphTopoNode& sw, int ifIndex, QList<quint64>& uids, quint32 segment, QList<quint64>& hosts)
{
	Q_ASSERT(sw.type == SwitchNode || sw.type == SwitchL3Node);

	logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Assemble Switch=%1, ifIndex=%2").arg(QString::number(sw.uid, 16)).arg(ifIndex));

	if (sw.type == SwitchL3Node)
	{
		QMapIterator<QString, GraphNodeIpEntAddr> it(sw.ipEntAddrs);
		while (it.hasNext())
		{
			it.next();

			const GraphNodeIpEntAddr& ent=it.value();
			quint32 nSegment=QHostAddress(ent.address).toIPv4Address()&0xffffff00;

			if (segment != nSegment)
			{
				addSegments(nSegment);
			}
		}
	}

	QVector<int> ifIndexes;
	foreach (GraphNodeInterface face, sw.interfaces)
	{
		if (face.operStatus == 1 && face.ifIndex != ifIndex)
		{
			if (sw.type == SwitchL3Node && sw.ipRoutes.contains(face.ifIndex))
			{
				continue;
			}

			ifIndexes.append(face.ifIndex);
		}
	}

	for (int i=0; i<ifIndexes.size(); i++)
	{
		if (!sw.peers.contains(ifIndexes[i]))
		{
			int port=ifIndexToPort(sw, ifIndexes[i]);
			if (port == -1)
			{
				continue;		//vlan口
			}

			QList<quint64> lstHosts;
			foreach (GraphNodeFdb fdb, sw.fdb)
			{
				if (fdb.status == 3 && fdb.port==port)
				{
					quint64 uid=addHostToTopo(fdb.address, segment);
					if (uid != GRAPH_TOPO_INVALID_NODE)
					{
						if ((uids.contains(uid) && m_nodes[uid].type == SwitchNode))
						{
							continue;
						}

						lstHosts.append(uid);

						hosts.removeOne(uid);
					}
				}
			}

			assembleSwitch(sw, ifIndexes[i], uids, segment, lstHosts);
		}
	}
}

quint64 QTopologyProbe::addHostToTopo(const QString& physAddr, quint32 segment)
{
	GraphNodeInterface face;

	return addHostToTopo(physAddr, segment, face);
}

quint64 QTopologyProbe::addHostToTopo(const QString& physAddr, quint32 segment, GraphNodeInterface & face)
{
	QVector<QString> addrs=ipAddrFromMacAddr(physAddr);

	QString strAddr;
	for (int i=0; i<addrs.size(); i++)
	{
		quint32 nAddr=QHostAddress(addrs[i]).toIPv4Address();
		addSegments(nAddr);

		if ((strAddr.isEmpty() && (nAddr&0xffffff00)==segment) || m_agents.contains(nAddr&0xffffff00))
		{
			strAddr=addrs[i];
		}
	}

	if (strAddr.isEmpty())
	{
		return GRAPH_TOPO_INVALID_NODE;
	}

	quint64 uid;

	face.addresses=addrs;
	face.physAddress=physAddr;
	face.type=1;
	face.operStatus=1;

	if ((uid=contains(segment, strAddr)) == GRAPH_TOPO_INVALID_NODE)
	{
		uid=macStringToUint64(physAddr);

		if (!m_nodes.contains(uid))
		{
			GraphTopoNode hostNode;
			hostNode.type=HostNode;
			hostNode.uid=uid;
			
			m_nodes.insert(uid, hostNode);

			logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Add node %1 to topolopy.").arg(QString::number(uid, 16)));
		}
	}

	bool bFind=false;

	GraphTopoNode& node=m_nodes[uid];
	QMutableMapIterator<int, GraphNodeInterface> it(node.interfaces);
	while (it.hasNext())
	{
		it.next();

		GraphNodeInterface& f=it.value();
		if (f.physAddress == physAddr)
		{
			face.ifIndex=f.ifIndex;

			bFind=true;

			for (int i=0; i<face.addresses.size(); i++)
			{
				if (!f.addresses.contains(face.addresses[i]))
				{
					f.addresses.append(face.addresses[i]);
				}
			}
		}
	}

	if (!bFind)
	{
		face.ifIndex=m_nodes[uid].interfaces.size()+1;
		m_nodes[uid].interfaces.insert(face.ifIndex, face);
	}

	return uid;
}

void QTopologyProbe::assembleSwitch(GraphTopoNode& sw, int ifIndex, QList<quint64>& uids, quint32 segment, QList<quint64>& hosts)
{
	int port=ifIndexToPort(sw, ifIndex);
	if (port == -1)
	{
		return;		//vlan口
	}

	QVector<QString> physAddrs;
	foreach (GraphNodeFdb fdb, sw.fdb)
	{
		if (fdb.status == 3 && fdb.port==port)
		{
			quint64 uid=addHostToTopo(fdb.address, segment);
			if (uid != GRAPH_TOPO_INVALID_NODE)
			{
				physAddrs.append(fdb.address);
			}
		}
	}

	if (physAddrs.isEmpty())
	{
		//未学习到邻接表
		return;
	}

	//端口包含一个或多个MAC地址
	bool bContains=false;	//true=至少连接一个可管理交换机
	QVectorIterator<QString> iter(physAddrs);
	while (iter.hasNext())
	{
		const QString& strPhysAddr=iter.next();

		quint64 uid=macStringToUint64(strPhysAddr);
		if (m_nodes.contains(uid) && (m_nodes[uid].type & (SwitchNode|SwitchL3Node)) != 0)
		{
			bContains=true;

			break;
		}
	}

	if (!bContains)
	{
		if (physAddrs.size() == 1)
		{
			//端口为交换机与一个主机相连
			GraphNodeInterface face;
			quint64 uid=addHostToTopo(physAddrs[0], segment, face);
			
			if (uid == GRAPH_TOPO_INVALID_NODE)
			{
				return;
			}

			hosts.removeOne(uid);

			Q_ASSERT(m_nodes.contains(uid));
			GraphTopoNode& node=m_nodes[uid];

			sw.peers.insert(ifIndex, QPair<quint64, int>(uid, face.ifIndex));
			node.peers.insert(face.ifIndex, QPair<quint64, int>(sw.uid, ifIndex));

			logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Switch %1:%2 link to Host %3 MAC=%4").arg(sw.name).arg(ifIndex)
				.arg(ipAddrFromMacAddr(physAddrs[0], segment)[0]).arg(physAddrs[0]));
		}
		else
		{
			//端口为交换机与Hub相连
			assembleHub(sw, ifIndex, uids, segment, hosts);
		}

		return;
	}

	//端口为交换机与交换机直连,或经Hub相连

	//经Hub间接连接时使用
	GraphTopoNode hub;
	hub.type=HubNode;
	hub.uid=GRAPH_TOPO_INVALID_NODE;
	int nIfIndex=0;

	QHash<quint32, QString> hashPhysAddrs;
	foreach (GraphNodeInterface face, sw.interfaces)
	{
		hashPhysAddrs.insertMulti(qHash(face.physAddress), face.physAddress);
	}

	QMutableListIterator<quint64> it(uids);
	while (it.hasNext())
	{
		quint64 index=it.next();
		Q_ASSERT(m_nodes.contains(index));
		GraphTopoNode& node=m_nodes[index];
		if (node.type != SwitchNode && node.type != SwitchL3Node)
		{
			continue;
		}

		QHash<quint32, QString> hashDownAddrs;	//node 所有下行端口MAC地址
		foreach (GraphNodeInterface face, node.interfaces)
		{
			if (face.operStatus != 1 || !physAddrs.contains(face.physAddress))
			{
				continue;
			}

			if (hashDownAddrs.isEmpty())
			{
				//计算node交换机所有下行端口包含的MAC地址集合
				QMap<int, QVector<QString> > addrsInPort;
				foreach (GraphNodeFdb fdb, node.fdb)
				{
					if (fdb.status == 3)
					{
						addrsInPort[fdb.port].append(fdb.address);
					}
				}

				foreach (QVector<QString> macs, addrsInPort)
				{
					bool isDownPort=true;
					for (int i=0; i<macs.size(); i++)
					{
						quint32 hashKey=qHash(macs[i]);
						if (hashPhysAddrs.contains(hashKey))
						{
							if (hashPhysAddrs.values(hashKey).contains(macs[i]))
							{
								isDownPort=false;
								break;
							}
						}
					}

					if (isDownPort)
					{
						for (int i=0; i<macs.size(); i++)
						{
							quint32 hashKey=qHash(macs[i]);
							if (!hashDownAddrs.contains(hashKey) || !hashDownAddrs.values(hashKey).contains(macs[i]))
							{
								hashDownAddrs.insertMulti(hashKey, macs[i]);
							}
						}
					}
				}
			}

			bool bDirect=true;	//判断交换机是否相连
			foreach (QString mac, hashDownAddrs)
			{
				if (!physAddrs.contains(mac))
				{
					bDirect=false;
					break;
				}
			}

			if (bDirect)	//直接连接或间接连接
			{
				it.remove();

				if (physAddrs.size() == hashDownAddrs.size()+1)
				{
					//直连
					sw.peers.insert(ifIndex, QPair<quint64, int>(node.uid, face.ifIndex));
					node.peers.insert(face.ifIndex, QPair<quint64, int>(sw.uid, ifIndex));

					logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Switch %1:%2 link to Switch %3:%4").arg(sw.name).arg(ifIndex)
						.arg(node.name).arg(face.ifIndex));

					assembleSwitchPerIfIndex(node, face.ifIndex, uids, segment, hosts);

					return;
				}
				else
				{
					//间接连接
					foreach (GraphNodeInterface face, node.interfaces)
					{
						quint64 uid=macStringToUint64(face.physAddress);
						if (hub.uid > uid)
						{
							hub.uid=uid;
						}
					}

					hub.peers.insert(++nIfIndex, QPair<quint64, int>(node.uid, face.ifIndex));

					assembleSwitchPerIfIndex(node, face.ifIndex, uids, segment, hosts);

					break;
				}
			}
		}
	}

	//当存在剩余未连接的主机时，连接到Hub上
	if (!hosts.isEmpty())
	{
		foreach (quint64 uid, hosts)
		{
			GraphNodeInterface face;
			uid=addHostToTopo(uint64ToMacString(uid), segment, face);

			if (uid == GRAPH_TOPO_INVALID_NODE)
			{
				continue;
			}

			Q_ASSERT(m_nodes.contains(uid));
			GraphTopoNode& node=m_nodes[uid];
			if (!node.peers.contains(face.ifIndex) || !m_nodes.contains(node.peers[face.ifIndex].first) 
				|| (m_nodes[node.peers[face.ifIndex].first].type&(HoldNode|HubNode)) != 0)
			{
				hub.uid=(hub.uid<uid? hub.uid:uid);

				hub.peers.insert(++nIfIndex, QPair<quint64, int>(uid, face.ifIndex));
			}
		}
	}

	hub.uid|=(((quint64)hub.peers.size()) << 48);

	if (!m_nodes.contains(hub.uid))
	{
		m_nodes.insert(hub.uid, hub);
	}
	else
	{
		Q_ASSERT(m_nodes.contains(hub.uid));
		GraphTopoNode& node=m_nodes[hub.uid];

		QMapIterator<int, QPair<quint64, int> > at(hub.peers);
		while (at.hasNext())
		{
			at.next();
			const QPair<quint64, int>& peer=at.value();

			bool bFind=false;
			QMapIterator<int, QPair<quint64, int> > ator(node.peers);
			while (ator.hasNext())
			{
				ator.next();

				if (ator.value() == peer)
				{
					bFind=true;
					break;
				}
			}

			nIfIndex=0;
			if (!bFind)
			{
				while (node.peers.contains(++nIfIndex));
				node.peers.insert(nIfIndex, peer);
			}
		}
	}

	Q_ASSERT(m_nodes.contains(hub.uid));
	GraphTopoNode& node=m_nodes[hub.uid];
	
	QMapIterator<int, QPair<quint64, int> > at(node.peers);
	while (at.hasNext())
	{
		at.next();

		const QPair<quint64, int>& peer=at.value();
		Q_ASSERT(m_nodes.contains(peer.first));

		logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Hub uid=%1 link to Host %2 MAC=%3")
			.arg(QString::number(node.uid, 16)).arg(ipAddrFromMacAddr(m_nodes[peer.first].interfaces[peer.second].physAddress, segment)[0])
			.arg(m_nodes[peer.first].interfaces[peer.second].physAddress));

		GraphTopoNode& nd=m_nodes[peer.first];
		nd.peers.insert(peer.second, QPair<quint64, int>(node.uid, at.key()));
	}

	sw.peers.insert(ifIndex, QPair<quint64, int>(node.uid, ++nIfIndex));
	node.peers.insert(nIfIndex, QPair<quint64, int>(sw.uid, ifIndex));
	logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Switch %1:%2 link to Hub %3:%4").arg(sw.name).arg(ifIndex)
		.arg(QString::number(node.uid, 16)).arg(QString::number(nIfIndex)));
}

int QTopologyProbe::ifIndexToPort(const GraphTopoNode& node, int ifIndex)
{
	foreach (GraphNodeBridgeBasePort basePort, node.bridgeBase.portTable)
	{
		if (basePort.ifIndex == ifIndex)
		{
			return basePort.port;
		}
	}

	return -1;
}

int QTopologyProbe::portToIfIndex(const GraphTopoNode& node, int port)
{
	foreach (GraphNodeBridgeBasePort basePort, node.bridgeBase.portTable)
	{
		if (basePort.port == port)
		{
			return basePort.ifIndex;
		}
	}

	return -1;
}

GraphNodeIpNetToMedia QTopologyProbe::ipNetToMediaFromArp(const QString& physAddr)
{
	GraphNodeIpNetToMedia media;
	media.ifIndex=-1;

	QList<int> indexes=m_tableArpMacKey.values(qHash(physAddr));
	foreach (int index, indexes)
	{
		Q_ASSERT(index<m_tableArp.size());
		const ArpEntry& entry=m_tableArp[index];

		if (entry.mac == physAddr)
		{
			if (m_segments.contains(QHostAddress(entry.address).toIPv4Address() & 0xffffff00))
			{
				media.ifIndex=1;
				media.physAddress=entry.mac;
				media.netAddress=entry.address;
			}
			
			break;
		}
	}

	return media;
}

QTopologyAssembleRunnable::QTopologyAssembleRunnable(QTopologyProbe* prober, quint32 segment, QObject* parent)
:QObject(parent)
,m_pProber(prober)
,m_nSegment(segment)
{

}

QTopologyAssembleRunnable::~QTopologyAssembleRunnable()
{

}

void QTopologyAssembleRunnable::run()
{
	if (m_pProber != NULL)
	{
		m_pProber->assemble(m_nSegment);

		emit finished();
	}
}
