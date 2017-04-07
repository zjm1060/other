#include "qlogicaltopo.h"

#include <QNetworkInterface>
#include <QHostAddress>
#include <QList>
#include <QStringList>

#include "topocommon.h"

#include "qlogger.h"

#define MAX_NUM_REPEAT	3

QLogicalTopo::QLogicalTopo(QObject *parent)
: QObject(parent),
m_pTraceRoute(NULL),
m_pRouteProbe(NULL),
m_pIcmp(NULL),
m_bReprobe(true),
m_nProbeFlag(ProbeFlagInvalid)
{
}

QLogicalTopo::~QLogicalTopo()
{
	terminateThread();

	if (m_pIcmp != NULL)
	{
		m_pIcmp->destroy();
		m_pIcmp=NULL;
	}
}

void QLogicalTopo::terminateThread()
{
	if (m_pRouteProbe != NULL)
	{
		m_pRouteProbe->destroy();
		m_pRouteProbe=NULL;
	}

	if (m_pTraceRoute != NULL)
	{
		m_pTraceRoute->destroy();
		m_pTraceRoute=NULL;
	}
}

void QLogicalTopo::init()
{
	if (!m_segments.isEmpty())
	{
		QList<quint32> keys=m_segments.keys();
		foreach (quint32 key, keys)
		{
			m_tasks.append(key);
		}

		return;
	}

	LOGPLUS_DEBUG_STR(QLogger::root(), "Add local network segments:");

	QList<QHostAddress> addresses=QNetworkInterface::allAddresses();
	QMutableListIterator<QHostAddress> it(addresses);
	while (it.hasNext())
	{
		QHostAddress address=it.next();
		if (address.protocol() != QAbstractSocket::IPv4Protocol || address == QHostAddress::LocalHost)
		{
			it.remove();
		}
	}

	if (!addresses.isEmpty())
	{
		foreach (QHostAddress address, addresses)
		{
			addSegments(address.toIPv4Address(), true);
		}
	}
	else
	{
		LOGPLUS_DEBUG_STR(QLogger::root(), "No find  local IP! Please check your network!!!");

		return;
	}

	LOGPLUS_DEBUG_STR(QLogger::root(), "Add configuration network segments:");

	QString strAddr=parseConfig(getAppParentDir()+INI_FILE, TOPOLOGY_SEGMENTS, "").toString();
	if (!strAddr.isEmpty())
	{
		QStringList addresses=strAddr.split(QChar(','), QString::SkipEmptyParts, Qt::CaseInsensitive);
		if (!addresses.isEmpty())
		{
			LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("Configure %d network segments", addresses.size()));
			foreach (QString address, addresses)
			{
				addSegments(QHostAddress(address).toIPv4Address(), false);
			}
		}
		else
		{
			LOGPLUS_DEBUG_STR(QLogger::root(), "No configure network segments.");
		}
	}
	else
	{
		LOGPLUS_DEBUG_STR(QLogger::root(), "No configure network segments.");
	}
}

void QLogicalTopo::addSegments(quint32 address, bool around)
{
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

void QLogicalTopo::addSegment(quint32 address)
{
	quint32 nSegment=address&0xffffff00;

	if (!m_segments.contains(nSegment))
	{
		LOGPLUS_DEBUG_STR(QLogger::root(),
			qFormatString("Add network segment %s", QHostAddress(nSegment).toString().toLocal8Bit().data()));

		m_segments.insert(nSegment, QVector<quint32>());

		m_tasks.append(nSegment);
	}
}

void QLogicalTopo::nextTask()
{
	if (m_tasks.isEmpty())
	{
		LOGPLUS_DEBUG_STR(QLogger::root(), "Finish to probe network.");

		terminateThread();

		m_bReprobe=false;
		m_nProbeFlag=ProbeAlives;

		nextAliveTask();

		return;
	}

	m_task.first=m_tasks.takeFirst();
	m_task.second=1;

	findAlive(m_task.first);
}

void QLogicalTopo::nextAliveTask()
{
	if (m_tasks.isEmpty())
	{
		if (m_bReprobe)
		{
			probe();
			return;
		}

		emit checkAlives();

		LOGPLUS_DEBUG_STR(QLogger::root(), "Recheck alive ip...");

		QMapIterator<quint32, QVector<quint32> > it(m_segments);
		while (it.hasNext())
		{
			it.next();

			if (!it.value().isEmpty())
			{
				m_tasks.append(it.key());
			}
		}

		LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("Add %d segments to check alives.", m_tasks.size()));
	}

	if (!m_tasks.isEmpty())
	{
		m_task.first=m_tasks.takeFirst();
		m_task.second=1;

		m_pIcmp->addTask(m_task.first);
	}
	else
	{
		m_nProbeFlag=ProbeStop;

		emit disconnected();

		LOGPLUS_DEBUG_STR(QLogger::root(), "Probe Stopped! Please to check your network!");
	}
}

void QLogicalTopo::reprobe()
{
	if (!m_bReprobe)
	{
		m_bReprobe=true;

		LOGPLUS_DEBUG_STR(QLogger::root(), "Query to reprobe network.");

		if (m_nProbeFlag == ProbeStop)
		{
			nextAliveTask();
		}
	}
}

void QLogicalTopo::probe()
{
#ifndef Q_OS_WIN
	if (getuid() != 0)
	{
		LOGPLUS_DEBUG_STR(QLogger::root(), "Topology must be running in root user!");
		return;
	}
#endif

	m_routes[ProbeFirstRoutes].clear();
	m_gateways.clear();
	m_tasks.clear();

	m_nProbeFlag=ProbeFirstRoutes;

	LOGPLUS_DEBUG_STR(QLogger::root(), "Begin to probe network...");

	m_pTraceRoute=new QTraceRoute(this);
	connect(m_pTraceRoute, SIGNAL(found(QVector<QPair<quint32, int> >)), 
		this, SLOT(foundRoutes(QVector<QPair<quint32, int> >)));

	if (m_pIcmp == NULL)
	{
		m_pIcmp=new QIcmp(this);
		connect(m_pIcmp, SIGNAL(found(quint32, QVector<quint32>)), this, SLOT(foundAlives(quint32, QVector<quint32>)));
	}
	
	m_pRouteProbe=new QRouteProbe(this);
	connect(m_pRouteProbe, SIGNAL(found(QPair<quint32, quint32>)), 
		this, SLOT(foundGateways(QPair<quint32, quint32>)));

	probeTopo();
}

void QLogicalTopo::probeTopo()
{
	init();	//初始化，获取初始探测网段

	findRoutePath();
}

void QLogicalTopo::findRoutePath()
{
	LOGPLUS_DEBUG_STR(QLogger::root(), "Begin to probe the First Route Path...");

	if (m_nProbeFlag == ProbeFirstRoutes)
	{
		if (!m_segments.isEmpty())
		{
			QList<quint32> segments=m_segments.keys();

			quint32 segment=segments[0]+(1<<16);

			while (true)
			{
				bool bExist=false;
				for (int i=0; i<segments.size(); i++)
				{
					if (segment == segments[i])
					{
						bExist=true;
						break;
					}
				}

				if (!bExist)
				{
					break;
				}

				segment+=1<<8;
			}

			m_task.first=segment;
			m_task.second=1;

			LOGPLUS_DEBUG_STR(QLogger::root(), 
				qFormatString("Use IP %s to find routes...", QHostAddress(m_task.first).toString().toLocal8Bit().data()));

			m_pTraceRoute->addTask(m_task.first);
		}
		else
		{
			LOGPLUS_DEBUG_STR(QLogger::root(), "No segments found! Please to check your network!");

			nextTask();
		}
	}
	else
	{
		nextTask();
	}
}

void QLogicalTopo::foundRoutes(QVector<QPair<quint32, int> > routes)
{
	if (m_nProbeFlag != ProbeFirstRoutes && m_nProbeFlag != ProbeRoutes)
	{
		LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("Invalid probe flag %d!!!", m_nProbeFlag));

		return;
	}

	if (!routes.isEmpty())
	{
		for (int i=0; i<routes.size(); i++)
		{
			if (routes[i].second != i+1)
			{
				LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("No find TTL=%d routes!", i+1));

				routes.resize(i);

				if (m_routes[m_nProbeFlag].isEmpty() || routes.size() > m_routes[m_nProbeFlag].size())
				{
					m_routes[m_nProbeFlag]=routes;
				}

				m_task.second++;
				if (m_task.second > MAX_NUM_REPEAT)
				{
					break;
				}
				else
				{
					m_pTraceRoute->addTask(m_task.first);
					return;
				}
			}

			LOGPLUS_DEBUG_STR(QLogger::root(), 
				qFormatString("Found routes %s", QHostAddress(routes[i].first).toString().toLocal8Bit().data()));
		}

		if (routes.size() > m_routes[m_nProbeFlag].size())
		{
			m_routes[m_nProbeFlag]=routes;
		}
	}
	else
	{
		if (m_routes[m_nProbeFlag].isEmpty() && m_task.second <= MAX_NUM_REPEAT)
		{
			m_task.second++;
			m_pTraceRoute->addTask(m_task.first);

			return;
		}
	}

	routes=m_routes[m_nProbeFlag];

	if (m_nProbeFlag == ProbeFirstRoutes)
	{
		LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("Find %d routes in default route path.", m_routes[ProbeFirstRoutes].size()));

		if (routes.isEmpty())
		{
			routes.append(QPair<quint32, int>(INVALID_IP_ADDRESS, 1));
		}
	}
	else if (m_nProbeFlag == ProbeRoutes)
	{
		LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("Find %d routes!", routes.size()));
	}

	if (!routes.isEmpty())
	{
		for (int i=0; i<routes.size(); i++)
		{
			if (routes[i].first != INVALID_IP_ADDRESS)
			{
				addSegments(routes[i].first, true);		//学习新的网段
			}
		}
	}

	nextTask();

	emit routesProbed(routes);

	if (m_nProbeFlag == ProbeRoutes)
	{
		if (m_gateways.contains(m_segment))
		{
			QPair<quint32, quint32>& gateway=m_gateways[m_segment];
			if (!routes.isEmpty())
			{
				gateway.second=routes[routes.size()-1].first;
			}
			else
			{
				if (gateway.first == gateway.second)
				{
					gateway.second=m_routes[ProbeFirstRoutes][0].first;
				}
			}

			LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("Route %s, Gateway %s, Find %d alives", 
				QHostAddress(gateway.second).toString().toLocal8Bit().data(), 
				QHostAddress(gateway.first).toString().toLocal8Bit().data(), m_segments[m_segment].size()));

			emit alivesProbed(gateway.second, gateway.first, m_segments[m_segment]);
		}
	}
}

void QLogicalTopo::findAlive(quint32 segment)
{
	LOGPLUS_DEBUG_STR(QLogger::root(), 
		qFormatString("Find alive IP in segment %s...", QHostAddress(segment).toString().toLocal8Bit().data()));

	m_pIcmp->addTask(segment);
}

void QLogicalTopo::foundAlives(quint32 segment, QVector<quint32> alives)
{
	m_segments[segment]=alives;

	LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("Find %d alive IPs in subnet %s", alives.size(), 
		QHostAddress(segment).toString().toLocal8Bit().data()));

	if (m_nProbeFlag == ProbeAlives)
	{
		if (!alives.isEmpty())
		{
			QPair<quint32, quint32> gateway;
			if (m_gateways.contains(segment))
			{
				gateway=m_gateways[segment];
			}
			else
			{
				gateway.second=INVALID_IP_ADDRESS;
				gateway.first=segment&0xff;
			}

			emit alivesProbed(gateway.second, gateway.first, alives);
		}

		nextAliveTask();
	}
	else
	{
		if (alives.isEmpty())
		{
			nextTask();
			return;
		}

		findGateways(segment, alives);		//获得路由器远端接口地址
	}
}

void QLogicalTopo::findGateways(quint32 segment, QVector<quint32> alives)
{
	LOGPLUS_DEBUG_STR(QLogger::root(), 
		qFormatString("Find gateway for segment %s...", QHostAddress(segment).toString().toLocal8Bit().data()));

	quint32 gateway=firstRoute(segment);
	if (gateway != INVALID_IP_ADDRESS)
	{
		LOGPLUS_DEBUG_STR(QLogger::root(), 
			qFormatString("Find gateway %s!", QHostAddress(gateway).toString().toLocal8Bit().data()));

		emit alivesProbed(gateway, gateway, alives);

		nextTask();
	}
	else
	{
		m_pRouteProbe->addTask(alives);
	}
}

quint32 QLogicalTopo::firstRoute(quint32 segment)
{
	quint32 gateway=INVALID_IP_ADDRESS;
	for (int i=0; i<m_routes[ProbeFirstRoutes].size(); i++)
	{
		if (segment == (m_routes[ProbeFirstRoutes].at(i).first & 0xffffff00))
		{
			gateway=m_routes[ProbeFirstRoutes].at(i).first;
			break;
		}
	}
	
	return gateway;
}

bool QLogicalTopo::isFirstRoute(quint32 address)
{
	bool bRet=false;
	for (int i=0; i<m_routes[ProbeFirstRoutes].size(); i++)
	{
		if (address == m_routes[ProbeFirstRoutes][i].first)
		{
			bRet=true;
			break;
		}
	}

	return bRet;
}

void QLogicalTopo::foundGateways(QPair<quint32, quint32> gateway)
{
	LOGPLUS_DEBUG_STR(QLogger::root(), 
		qFormatString("Find gateway %s, route %s", QHostAddress(gateway.first).toString().toLocal8Bit().data(),
		QHostAddress(gateway.second).toString().toLocal8Bit().data()));

	m_segment=gateway.first & 0xffffff00;

	if (gateway.first != gateway.second && !isFirstRoute(gateway.second))
	{
		m_nProbeFlag=ProbeRoutes;

		m_gateways[m_segment]=gateway;
		m_routes[m_nProbeFlag].clear();

		m_task.first=gateway.second;
		m_task.second=1;

		m_pTraceRoute->addTask(m_task.first);
	}
	else
	{
		QVector<quint32>& alives=m_segments[gateway.first & 0xffffff00];

		if (gateway.first == gateway.second)
		{
			if (!m_routes->isEmpty())
			{
				gateway.second=m_routes[ProbeFirstRoutes][0].first;
			}
			else
			{
				gateway.second=INVALID_IP_ADDRESS;
			}
		}

		m_gateways[m_segment]=gateway;

		emit alivesProbed(gateway.second, gateway.first, alives);

		nextTask();
	}
}
