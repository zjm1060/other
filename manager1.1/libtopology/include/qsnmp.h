#ifndef QSNMP_H
#define QSNMP_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QVariant>
#include <QThread>
#include <QLinkedList>

#ifndef NETSNMP_TOOLS_C
#define NETSNMP_TOOLS_C
#endif

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "global.h"
#include "qsharedobject.h"

#define SNMP_MAX_REQUEST_CONNECTION	64

class QThreadRecvSnmpResponse;
class TOPOLOGY_EXPORT QSnmp : public QObject, public QSharedObject
{
	Q_OBJECT

public:
	QSnmp(QObject *parent=0);
	~QSnmp();

public:
	enum OidVarsType
	{
		SysDescr,
		SysName,
		SysObjectID,
		SysServices,
		IfNumber,
		IfIndex,
		IfDescr,
		IfType,
		IfSpeed,
		IfPhysAddress,
		IfOperStatus,
		IpForwarding,
		IpAdEntAddr,
		IpAdEntIfIndex,
		IpAdEntNetMask,
		IpRouteDest,
		IpRouteIfIndex,
		IpRouteNextHop,
		IpRouteType,
		IpRouteMask,
		IpNetToMediaIfIndex,
		IpNetToMediaPhysAddress,
		IpNetToMediaNetAddress,
		Dot1dBaseBridgeAddress,
		Dot1dBaseNumPorts,
		Dot1dBaseType,
		Dot1dBasePort,
		Dot1dBasePortIfIndex,
		Dot1dTpFdbAddress,
		Dot1dTpFdbPort,
		Dot1dTpFdbStatus,
		OidVarsTypeEnd
	};

	typedef struct _snmpAsycOpt
	{
		char* dest;
		int oid;
		unsigned char type;
		struct snmp_session* session;

		bool valid;
		QVector<QVariant>	values;
	}SnmpAsycOpt;

	typedef struct _snmpResponse 
	{
		QString addr;	//IP地址
		int oid;		//oid类型标识
		
		QVector<QVariant>	values;
	}SnmpResponse;

	typedef struct _snmpOpt
	{
		struct in_addr dest;
		char *oid;
		unsigned char type;
		netsnmp_session *session;
	}SnmpOpt;

	enum SnmpRequestType
	{
		SnmpAsynGet,
		SnmpGetNext
	};

	typedef struct _snmpTask
	{
		QLinkedList<QVector<struct in_addr> > addrs;	//任务分组， SNMP每次最多能够同时保持64条连接

		int oidType;
		unsigned char dataType;
		int type;	//0=AsynGet	1=GetNext
	}SnmpTask;

public:
	void destroy();

public:
	void addTask(const QVector<struct in_addr>& addrs, int oidType, unsigned char dataType, int requestType=SnmpAsynGet);
	void addTask(const QVector<quint32>& addrs, int oidType, unsigned char dataType, int requestType=SnmpAsynGet);

public slots:
	void threadQuit();
	void response(QVector<QSnmp::SnmpResponse> results);

signals:
	void finished(int oidType, QVector<QSnmp::SnmpResponse> results);

private:
	void nextTask();

private:
	void init();

private:
	SnmpTask	m_task;
	QLinkedList<SnmpTask>	m_tasks;

	QThreadRecvSnmpResponse*	m_pThreadRecvResponse;
};

class TOPOLOGY_EXPORT QThreadRecvSnmpResponse : public QThread
{
	Q_OBJECT

public:
	QThreadRecvSnmpResponse(QObject* parent=0);
	~QThreadRecvSnmpResponse();

public:
	enum StatusFlag
	{
		Running,
		NewTask,
		Tasking,
		StatusEnd
	};

public:
	void addTask(const QSnmp::SnmpTask&	task);

	void setStatus(StatusFlag flag, bool status);
	bool status(StatusFlag flag);

public:
	void run();

signals:
	void response(QVector<QSnmp::SnmpResponse> result);

private:
	QVector<QSnmp::SnmpResponse> snmpAsynGet(const QVector<struct in_addr>& addrs, int oidType, unsigned char dataType);	//注意：最多同时连接64个SNMP连接
	QVector<QSnmp::SnmpResponse> snmpGetNext(const QVector<struct in_addr>& addrs, int oidType, unsigned char dataType);
	QSnmp::SnmpResponse snmpGetNext(struct in_addr& addr, int oidType, unsigned char dataType);

private:
	QSnmp::SnmpTask	m_task;

	QVector<QSnmp::SnmpResponse>	m_result;

private:
	QMutex	m_mutex;
	mutable bool m_flags[StatusEnd];

private:
	char* m_oids[QSnmp::OidVarsTypeEnd];
};

Q_DECLARE_METATYPE(struct in_addr);
Q_DECLARE_METATYPE(QSnmp::SnmpResponse);
Q_DECLARE_METATYPE(QVector<QSnmp::SnmpResponse>);

int snmpAsynResponse(int operation, struct snmp_session *sp, int reqid, struct snmp_pdu *pdu, void *magic);

#endif // QSERVERMANAGER_H

