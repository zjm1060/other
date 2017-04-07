#include "qsnmp.h"

#include <QDateTime>

#define OID_STRING_LEN 50

#include "qlogger.h"
#include "qutildef.h"
#include "common.h"

int g_nActives;
int snmpAsynResponse(int operation, struct snmp_session *sp, int reqid, struct snmp_pdu *pdu, void *magic)
{
	QSnmp::SnmpAsycOpt* opt= (QSnmp::SnmpAsycOpt*)magic;
	struct variable_list *vars;
	QVector<QVariant> values;

	if (operation == NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE)
	{
		for (vars = pdu->variables; vars; vars = vars->next_variable)
		{
			switch (opt->type)
			{
			case ASN_GAUGE:
				{
					if (vars->type == ASN_GAUGE)
					{
						values.append(QVariant((quint32)*vars->val.integer));
					}
					else
					{
						logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("value #%1 is NOT a gauge! Ack!").arg(values.size()+1));
					}
				}
				break;
			case ASN_INTEGER:
				{
					if (vars->type == ASN_INTEGER)
					{
						values.append(QVariant((int)*vars->val.integer));
						logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1:%2").arg(opt->dest).arg(*vars->val.integer));
					}
					else
					{
						logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("value #%1 is NOT an integer! Ack!").arg(values.size()+1));
					}
				}
				break;
			case ASN_IPADDRESS:
				{
					if (vars->type == ASN_IPADDRESS)
					{
						char address[16];
						u_char *ipAddr = vars->val.string;

						sprintf(address, "%d.%d.%d.%d", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
						values.append(QVariant(address));
					}
					else
					{
						logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("value #%1 is NOT an IP address! Ack!").arg(values.size()+1));
					}
				}
				break;
			case ASN_BIT_STR:
			case ASN_OCTET_STR:
				{
					if (vars->type == opt->type)
					{
						QString strText=QString::fromLatin1((char*)vars->val.bitstring, vars->val_len);
						values.append(QVariant(strText));
					}
					else
					{
						logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("value #%1 is NOT an bit or octet string! Ack!").arg(values.size()+1));
					}
				}
				break;
			case ASN_OBJECT_ID:
				{
					if (vars->type == opt->type)
					{
						QString strObjectID;
						oid* pObjid=vars->val.objid;
						int nObjidLen=vars->val_len/sizeof(oid);
						while (nObjidLen-- > 0)
						{
							strObjectID+=QString("%1.").arg(*(pObjid));

							pObjid++;
						}

						if (strObjectID.endsWith(QChar('.')))
						{
							strObjectID=strObjectID.left(strObjectID.length()-1);
						}
						
 						values.append(QVariant(strObjectID));
					}
					else
					{
						logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("value #%1 is NOT an object id! Ack!").arg(values.size()+1));
					}
				}
				break;
			default:
				break;
			}
		}

		opt->values=values;

		opt->valid=true;
	}
	else
	{
		logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("%1: Timeout!").arg(sp->peername));

		opt->valid=false;
	}

	g_nActives--;

	return 1;
}

QSnmp::QSnmp(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<QVector<QSnmp::SnmpResponse> >("QVector<QSnmp::SnmpResponse>");

	m_pThreadRecvResponse=new QThreadRecvSnmpResponse;
	connect(m_pThreadRecvResponse, SIGNAL(response(QVector<QSnmp::SnmpResponse>)), this, SLOT(response(QVector<QSnmp::SnmpResponse>)));
	connect(m_pThreadRecvResponse, SIGNAL(finished()), this, SLOT(threadQuit()));

	init();

	logplusDebug(LIBTOPOLOGY_LOGGER_STR, "Start SNMP Response threads to find topology graph...");

	m_pThreadRecvResponse->start();
}

QSnmp::~QSnmp()
{
}

void QSnmp::init()
{
	init_snmp("asynchapp");
}

void QSnmp::destroy()
{
	logplusDebug(LIBTOPOLOGY_LOGGER_STR, "Stop SNMP request threads.");

	m_pThreadRecvResponse->setStatus(QThreadRecvSnmpResponse::Running, false);
}

void QSnmp::threadQuit()
{
	if (m_pThreadRecvResponse == sender())
	{
		m_pThreadRecvResponse->deleteLater();
		m_pThreadRecvResponse=NULL;
	}

	if (m_pThreadRecvResponse == NULL)
	{
		deleteLater();
	}
}

void QSnmp::response(QVector<QSnmp::SnmpResponse> results)
{
	emit finished(m_task.oidType, results);

	nextTask();
}

void QSnmp::addTask(const QVector<struct in_addr>& addrs, int oidType, unsigned char dataType, int requestType)
{
	SnmpTask task;

	task.oidType=oidType;
	task.dataType=dataType;
	task.type=requestType;

	QVector<struct in_addr> addresses;
	for (int i=0; i<addrs.size();)
	{
		addresses.append(addrs[i]);
		if (((++i)%SNMP_MAX_REQUEST_CONNECTION) == 0 || i==addrs.size())	//由于同时请求SNMP连接数最大为64
		{
			task.addrs.append(addresses);
			addresses.clear();
		}
	}

	m_tasks.append(task);

	nextTask();
}

void QSnmp::addTask(const QVector<quint32>& addrs, int oidType, unsigned char dataType, int requestType)
{
	struct in_addr	addr;
	QVector<struct in_addr> addresses;

	for (int i= 0; i<addrs.size(); i++)
	{
		addr.s_addr = htonl(addrs[i]);

		addresses.append(addr);
	}

	addTask(addresses, oidType, dataType, requestType);
}

void QSnmp::nextTask()
{
	if (m_pThreadRecvResponse == NULL)
	{
		logplusError(LIBTOPOLOGY_LOGGER_STR, "The SNMP request send thread is NULL!!");
		return;
	}

	if (m_tasks.isEmpty() || m_pThreadRecvResponse->status(QThreadRecvSnmpResponse::Tasking))
	{
		return;
	}

	m_task=m_tasks.takeFirst();

	logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Send SNMP quest, oid type=%1").arg(m_task.oidType));
	m_pThreadRecvResponse->addTask(m_task);
}

QThreadRecvSnmpResponse::QThreadRecvSnmpResponse(QObject* parent)
:QThread(parent)
{
	m_oids[QSnmp::SysDescr]=	_strdup("1.3.6.1.2.1.1.1.0");
	m_oids[QSnmp::SysName]=		_strdup("1.3.6.1.2.1.1.5.0");
	m_oids[QSnmp::SysObjectID]=	_strdup("1.3.6.1.2.1.1.2.0");
	m_oids[QSnmp::SysServices]=	_strdup("1.3.6.1.2.1.1.7.0");
	m_oids[QSnmp::IfNumber]=	_strdup("1.3.6.1.2.1.2.1.0");
	m_oids[QSnmp::IfIndex]=	_strdup("1.3.6.1.2.1.2.2.1.1");
	m_oids[QSnmp::IfDescr]=	_strdup("1.3.6.1.2.1.2.2.1.2");
	m_oids[QSnmp::IfType]=	_strdup("1.3.6.1.2.1.2.2.1.3");
	m_oids[QSnmp::IfSpeed]=	_strdup("1.3.6.1.2.1.2.2.1.5");
	m_oids[QSnmp::IfPhysAddress]=	_strdup("1.3.6.1.2.1.2.2.1.6");
	m_oids[QSnmp::IfOperStatus]=	_strdup("1.3.6.1.2.1.2.2.1.8");
	m_oids[QSnmp::IpForwarding]=	_strdup("1.3.6.1.2.1.4.1.0");
	m_oids[QSnmp::IpAdEntAddr]=	_strdup("1.3.6.1.2.1.4.20.1.1");
	m_oids[QSnmp::IpAdEntIfIndex]=	_strdup("1.3.6.1.2.1.4.20.1.2");
	m_oids[QSnmp::IpAdEntNetMask]=	_strdup("1.3.6.1.2.1.4.20.1.3");
	m_oids[QSnmp::IpRouteDest]= _strdup("1.3.6.1.2.1.4.21.1.1");
	m_oids[QSnmp::IpRouteIfIndex]= _strdup("1.3.6.1.2.1.4.21.1.2");
	m_oids[QSnmp::IpRouteNextHop]= _strdup("1.3.6.1.2.1.4.21.1.7");
	m_oids[QSnmp::IpRouteType]= _strdup("1.3.6.1.2.1.4.21.1.8");
	m_oids[QSnmp::IpRouteMask]= _strdup("1.3.6.1.2.1.4.21.1.11");
	m_oids[QSnmp::IpNetToMediaIfIndex]=	_strdup("1.3.6.1.2.1.4.22.1.1");
	m_oids[QSnmp::IpNetToMediaPhysAddress]=	_strdup("1.3.6.1.2.1.4.22.1.2");
	m_oids[QSnmp::IpNetToMediaNetAddress]=	_strdup("1.3.6.1.2.1.4.22.1.3");
	m_oids[QSnmp::Dot1dBaseBridgeAddress]= _strdup("1.3.6.1.2.1.17.1.1.0");
	m_oids[QSnmp::Dot1dBaseNumPorts]= _strdup("1.3.6.1.2.1.17.1.2.0");
	m_oids[QSnmp::Dot1dBaseType]= _strdup("1.3.6.1.2.1.17.1.3.0");
	m_oids[QSnmp::Dot1dBasePort]= _strdup("1.3.6.1.2.1.17.1.4.1.1");
	m_oids[QSnmp::Dot1dBasePortIfIndex]= _strdup("1.3.6.1.2.1.17.1.4.1.2");
	m_oids[QSnmp::Dot1dTpFdbAddress]= _strdup("1.3.6.1.2.1.17.4.3.1.1");
	m_oids[QSnmp::Dot1dTpFdbPort]= _strdup("1.3.6.1.2.1.17.4.3.1.2");
	m_oids[QSnmp::Dot1dTpFdbStatus]= _strdup("1.3.6.1.2.1.17.4.3.1.3");

	setStatus(Running, true);
	setStatus(NewTask, false);
	setStatus(Tasking, false);
}

QThreadRecvSnmpResponse::~QThreadRecvSnmpResponse()
{
	for (int i=0; i<QSnmp::OidVarsTypeEnd; i++)
	{
		free(m_oids[i]);
	}
}

void QThreadRecvSnmpResponse::setStatus(QThreadRecvSnmpResponse::StatusFlag flag, bool status)
{
	QMutexLocker locker(&m_mutex);
	m_flags[flag]=status;
}

bool QThreadRecvSnmpResponse::status(QThreadRecvSnmpResponse::StatusFlag flag)
{
	bool f;
	QMutexLocker locker(&m_mutex);
	f=m_flags[flag];

	return f;
}

void QThreadRecvSnmpResponse::addTask(const QSnmp::SnmpTask& task)
{
	if (status(Tasking))
	{
		logplusDebug(LIBTOPOLOGY_LOGGER_STR, "QThreadRecvSnmpResponse: A task is running! Add task failed!");
		return;
	}

	logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("QThreadRecvSnmpResponse: Send SNMP quest, oid type=%1").arg(task.oidType));

	m_task=task;

	setStatus(NewTask, true);
}

void QThreadRecvSnmpResponse::run()
{
	while (status(Running))
	{
		if (!status(NewTask))
		{
			msleep(100);
			continue;
		}

		setStatus(NewTask, false);
		setStatus(Tasking, true);

		QVector<QSnmp::SnmpResponse> results;

		while (!m_task.addrs.isEmpty())
		{
			QVector<struct in_addr> addrs=m_task.addrs.takeFirst();

			if (m_task.type == QSnmp::SnmpAsynGet)
			{
				results+=snmpAsynGet(addrs, m_task.oidType, m_task.dataType);
			}
			else
			{
				results+=snmpGetNext(addrs, m_task.oidType, m_task.dataType);
			}
		}

		setStatus(Tasking, false);

		emit response(results);
	}

	logplusDebug(LIBTOPOLOGY_LOGGER_STR, "QThreadRecvSnmpResponse: Thread quit!");
}

QVector<QSnmp::SnmpResponse> QThreadRecvSnmpResponse::snmpAsynGet(const QVector<struct in_addr>& addrs, int oidType, unsigned char dataType)
{
	struct snmp_session session;
	struct snmp_pdu *req;
	oid oids[MAX_OID_LEN];
	size_t oidlen = MAX_OID_LEN;

	Q_ASSERT(oidType < QSnmp::OidVarsTypeEnd);
	Q_ASSERT(addrs.size() <= 64);

	QVector<QSnmp::SnmpAsycOpt*> opts;

	foreach (struct in_addr addr, addrs)
	{
		QSnmp::SnmpAsycOpt* opt=new QSnmp::SnmpAsycOpt;
		opts.append(opt);

		snmp_sess_init(&session);

		session.peername=_strdup(inet_ntoa(addr));
		session.version=SNMP_VERSION_2c;
		session.community=(u_char*)_strdup("iwhr");
		session.community_len=strlen((char*)session.community);
		session.callback=snmpAsynResponse;
		session.callback_magic=opt;

		opt->dest=_strdup(session.peername);
		opt->oid=oidType;
		opt->type=dataType;
		opt->valid=false;

		if (!(opt->session = snmp_open(&session)))
		{
			logplusError(LIBTOPOLOGY_LOGGER_STR, "snmp_open failed");
			SAFE_DELETE_CLASS(opt);

			free(session.peername);
			free(session.community);

			continue;
		}

		req = snmp_pdu_create(SNMP_MSG_GET);
		read_objid(m_oids[oidType], oids, &oidlen);
		snmp_add_null_var(req, oids, oidlen);
		if (!snmp_send(opt->session, req))
		{
			logplusError(LIBTOPOLOGY_LOGGER_STR, "snmp_send failed");
			snmp_free_pdu(req);
		}
		else
		{
			g_nActives++;
		}

		free(session.peername);
		free(session.community);
	}

	while (g_nActives > 0)
	{
		int fds = 0, block = 1;
		fd_set fdset;
		struct timeval timeout;

		FD_ZERO(&fdset);
		snmp_select_info(&fds, &fdset, &timeout, &block);

		fds = select(fds, &fdset, NULL, NULL, block ? NULL : &timeout);

		if (fds > 0)
		{
			snmp_read(&fdset);
		}
		else if (fds == 0)
		{
			snmp_timeout();
		}
		else
		{
			logplusError(LIBTOPOLOGY_LOGGER_STR, "SNMP: select failed");
			break;
		}
	}

	QVector<QSnmp::SnmpResponse> responses;
	QSnmp::SnmpResponse response;
	for (int i=0; i<opts.size(); i++)
	{
		if (opts[i]->session != NULL) snmp_close(opts[i]->session);
		if (opts[i]->valid)
		{
			response.addr=QString(opts[i]->dest);
			response.oid=opts[i]->oid;
			response.values=opts[i]->values;

			responses.append(response);
		}

		free(opts[i]->dest);
		SAFE_DELETE_CLASS(opts[i]);
	}

	return responses;
}

QVector<QSnmp::SnmpResponse> QThreadRecvSnmpResponse::snmpGetNext(const QVector<struct in_addr>& addrs, int oidType, unsigned char dataType)
{
	QVector<QSnmp::SnmpResponse>	results;
	foreach (struct in_addr addr, addrs)
	{
		QSnmp::SnmpResponse result=snmpGetNext(addr, oidType, dataType);

		results.append(result);
	}

	return results;
}

QSnmp::SnmpResponse QThreadRecvSnmpResponse::snmpGetNext(struct in_addr& addr, int oidType, unsigned char dataType)
{
	struct snmp_pdu *pdu;
	struct snmp_pdu *response;
	struct variable_list *vars= NULL;
	oid name[MAX_OID_LEN];
	size_t name_length = MAX_OID_LEN;
	oid root[MAX_OID_LEN];
	size_t rootlen = MAX_OID_LEN;
	int status = 0;
	bool running = true;
	int count = 0;
	char oidString[OID_STRING_LEN+1];
	QString strOid;

	struct snmp_session session;

	QSnmp::SnmpOpt opt;

	QSnmp::SnmpResponse result;

	Q_ASSERT(oidType < QSnmp::OidVarsTypeEnd);

	snmp_sess_init(&session);

	session.peername=_strdup(inet_ntoa(addr));
	session.version=SNMP_VERSION_2c;
	session.community=(u_char*)_strdup("iwhr");
	session.community_len = strlen((char *)session.community);

	result.addr=session.peername;
	result.oid=oidType;

	if (!(opt.session = snmp_open(&session)))
	{
		logplusError(LIBTOPOLOGY_LOGGER_STR, "snmp_open");
		return result;
	}

	opt.dest=addr;
	opt.oid=m_oids[oidType];
	opt.type=dataType;

	read_objid(m_oids[oidType], name, &name_length);
	read_objid(m_oids[oidType], root, &rootlen);

	while (running)
	{
		pdu = snmp_pdu_create(SNMP_MSG_GETNEXT);
		snmp_add_null_var(pdu, name, name_length);

		status = snmp_synch_response(opt.session, pdu, &response); 

		if (status == STAT_SUCCESS)
		{
			if (response->errstat == SNMP_ERR_NOERROR)
			{
				for (vars = response->variables; vars; vars = vars->next_variable)
				{
					if ((vars->name_length < rootlen) || (memcmp(root, vars->name, rootlen * sizeof(oid)) != 0))
					{
						running = false;
						continue;
					}

					snprint_objid(oidString, OID_STRING_LEN, vars->name, name_length);
					strOid=oidString;
					int nIndex=strOid.indexOf(QChar('.'));
					if (nIndex != -1)
					{
						strOid=strOid.right(strOid.length()-nIndex-1);
					}
					else
					{
						strOid="";
					}

					if (vars->type != SNMP_ENDOFMIBVIEW && vars->type != SNMP_NOSUCHOBJECT && vars->type != SNMP_NOSUCHINSTANCE)
					{
						result.values.append(strOid);
						switch (opt.type)
						{
						case ASN_GAUGE:
							{
								if (vars->type == ASN_GAUGE)
								{
									QVariant var=(quint32)(*vars->val.integer);
									result.values.append(var);

									count++;
								}
								else
								{
									logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("value #%1 is NOT a gauge! Ack!").arg(count));
								}
							}
							break;
						case ASN_INTEGER:
							{
								if (vars->type == ASN_INTEGER)
								{
									QVariant var=(quint32)(*vars->val.integer);
									result.values.append(var);

									count++;
								}
								else
								{
									logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("value #%1 is NOT an integer! Ack!").arg(count));
								}
							}
							break;
						case ASN_IPADDRESS:
							{
								if (vars->type == ASN_IPADDRESS)
								{
									char address[16];
									u_char *ipAddr = vars->val.string;

									sprintf(address, "%d.%d.%d.%d", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);

									result.values.append(QString(address));

									count++;
								}
								else
								{
									logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("value #%1 is NOT an IP address! Ack!").arg(count));
								}
							}
							break;
						case ASN_BIT_STR:
						case ASN_OCTET_STR:
							{
								if (vars->type == dataType)
								{
									QString strText=QString::fromLatin1((char*)vars->val.bitstring, vars->val_len);
									result.values.append(strText);

									count++;
								}
								else
								{
									logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("value #%1 is NOT an bit or octet string! Ack!").arg(count));
								}
							}
							break;
						default:
							break;
						}
					}
					else 
					{
						running = false;
						continue;
					}

					memmove((char *) name, (char *) vars->name, vars->name_length * sizeof(oid));
					name_length = vars->name_length;
				}
			}
			else
			{
				running = false;
				logplusError(LIBTOPOLOGY_LOGGER_STR, QString("Error in packet.Reason: %1").arg(snmp_errstring (response->errstat)));
				if (response->errstat == SNMP_ERR_NOSUCHNAME)
				{
					logplusError(LIBTOPOLOGY_LOGGER_STR, "The request for this object identifier failed: ");
					if (vars)
					{
						fprint_objid(stderr, vars->name, vars->name_length);
					}
				}
			}
		}
		else if (status == STAT_TIMEOUT)
		{
			struct snmp_session *sptr = opt.session; 
			logplusDebug(LIBTOPOLOGY_LOGGER_STR, QString("Timeout: No Response from %1").arg(sptr->peername));
			running = false;
			count = -1;
		}
		else
		{
			logplusError(LIBTOPOLOGY_LOGGER_STR, "snmpwalk");
			running = false;
			count = -1;
		}


		if (response)
		{
			snmp_free_pdu(response);
		}
	}

	if (opt.session != NULL) snmp_close((netsnmp_session*)opt.session);
	free(session.peername);
	free(session.community);

	return result;
}

