#ifndef QLOGICALTOPO_H
#define QLOGICALTOPO_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QTimer>

#include "global.h"
#include "qtraceroute.h"
#include "qrouteprobe.h"
#include "qicmp.h"

#define INVALID_IP_ADDRESS	-1

class TOPOLOGY_EXPORT QLogicalTopo : public QObject
{
	Q_OBJECT

public:
	QLogicalTopo(QObject *parent=0);
	~QLogicalTopo();

public:
	void probe();
	void reprobe();

	void addSegments(quint32 address, bool around);	//添加address网段,around=true添加附近正负一
	void addSegment(quint32 address);	//添加address网段

	void findRoutePath();				//发现路由路径
	void findGateways(quint32 segment, QVector<quint32> alives);
	void findAlive(quint32 segment);		//发现活动IP

public slots:
	void foundRoutes(QVector<QPair<quint32, int> > routes);
	void foundAlives(quint32 segment, QVector<quint32> alives);
	void foundGateways(QPair<quint32, quint32> gateway);

signals:
	void routesProbed(QVector<QPair<quint32, int> > routes);
	void alivesProbed(quint32 route, quint32 gateway, QVector<quint32> alives);

	void checkAlives();
	void disconnected();

private:
	void init();
	void probeTopo();

	void terminateThread();

	void nextTask();
	void nextAliveTask();

	quint32 firstRoute(quint32 segment);	//获取网段的直连路由器， -1表示不位于首选路由器中
	bool isFirstRoute(quint32 address);		//存在于首选路由器中

private:
	QVector<QPair<quint32, int> > m_routes[2];	//首选路由路径和子网路由路径

	QMap<quint32, QPair<quint32, quint32> >		m_gateways;		//探测子网路径时，用于保存子网与直连路由器对
	quint32 m_segment;							//探测子网路由路径时的子网网段

	QMap<quint32, QVector<quint32> >	m_segments;	//key为网段

	QLinkedList<quint32>	m_tasks;
	QPair<quint32, int>		m_task;

	QTraceRoute*	m_pTraceRoute;
	QRouteProbe*	m_pRouteProbe;
	QIcmp*			m_pIcmp;

	bool			m_bReprobe;		//是否重新探测

private:
	enum ProbeFlag
	{
		ProbeFlagInvalid=-1,
		ProbeFirstRoutes,		//探测首选路径
		ProbeRoutes,			//探测子网路径
		ProbeAlives,
		ProbeStop				//探测停止
	};
	int				m_nProbeFlag;	//探测标志
};

#endif // QLOGICALTOPO_H

