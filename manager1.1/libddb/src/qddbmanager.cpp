#include "qddbmanager.h"

#include <QDateTime>

#include "qlogger.h"
#include "common.h"
#include "qservermanager.h"
#include "qmessageparser.h"
#include "qreceiverparser.h"
#include "qddbcontroller.h"
#include "qddbwarehouse.h"
#include "qprivilegemanager.h"

#define CANDIDATE_WAIT_TIME_TICK	4
QDdbManager::QDdbManager(QObject *parent)
	: QObject(parent)
	,m_pClient(NULL)
	,m_nCount(0)
	,m_nPortStep(0)
{
	qsrand(QDateTime::currentDateTime().toTime_t());

	QPrivilegeManager::instance()->read();

	m_uid=getHostName();

	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	quint16 nPort=(quint16)parseConfig(getAppParentDir()+INI_FILE, "network/multicastPort", 2536).toUInt();

	m_pSender=new QMulticastSender(m_uid, nPort);

	m_pView=new QMulticastView(m_pSender);
	m_pReceiver=new QMulticastReceiver(m_uid, nPort, m_pView);

	m_pElect=new QLeaderElection(m_uid, m_pSender);

	QSharedControllerPtr pController(new QDDBController);
	m_pClientManager=new QClientManager(pController, this);

	connect(m_pClientManager, SIGNAL(connected(QClient*)), this, SLOT(connected(QClient*)));
	connect(m_pClientManager, SIGNAL(connectFailed(QClient*)), this, SLOT(connectFailed(QClient*)));
	connect(m_pClientManager, SIGNAL(disconnected(QClient*)), this, SLOT(disconnected(QClient*)));

	m_timer.setInterval(1000);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	connect(QPrivilegeManager::instance(), SIGNAL(changed()), this, SLOT(privilegesChanged()));
}

QDdbManager::~QDdbManager()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);

	if (m_pClientManager != NULL)
	{
		m_pClientManager->removeAll();
	}

	SAFE_DELETE_CLASS(m_pReceiver);
	SAFE_DELETE_CLASS(m_pView);

	SAFE_DELETE_CLASS(m_pElect);

	SAFE_DELETE_CLASS(m_pClientManager);
}

QDdbManager* QDdbManager::instance()
{
	static QDdbManager* pInstance=new QDdbManager;

	return pInstance;
}

void QDdbManager::start()
{
	electLeader();
}

QLeaderElection::ElectionRole QDdbManager::electionRole() const
{
	return m_pElect != NULL? m_pElect->electionRole():QLeaderElection::CandidateRole;
}

void QDdbManager::setElectionRole(QLeaderElection::ElectionRole role)
{
	if (m_timer.isActive())
	{
		m_timer.stop();
	}

	if (m_pElect != NULL)
	{
		m_pElect->setElectionRole(role);

		if (role == QLeaderElection::UncheckedRole)
		{
			m_nCount=0;
			m_timer.start();
		}
	}
}

void QDdbManager::electLeader()
{
	if (electionRole() == QLeaderElection::ManagedRole || electionRole() == QLeaderElection::LeaderRole)
	{
		return;
	}

	logplusDebug(LIBDDB_LOGGER_STR, QString("Begin to elect a leader, the local machine named %1...").arg(m_uid));

	QDdbWarehouse::instance()->clear();
	m_pClientManager->removeAll();

	setElectionRole(QLeaderElection::CandidateRole);

	m_nCount=0;
	m_timer.start();
}

void QDdbManager::timeout()
{
	if (m_pElect->electionRole() == QLeaderElection::CandidateRole || m_pElect->electionRole() == QLeaderElection::UncheckedRole)
	{
		if ((++m_nCount) > CANDIDATE_WAIT_TIME_TICK)
		{
			if (m_timer.isActive())
			{
				m_timer.stop();
			}

			logplusDebug(LIBDDB_LOGGER_STR, "Query to vote the local machine as the leader.");

			m_pElect->queryVote((qrand()%100)/100.0);

			m_nCount=0;
			m_timer.start();
		}
	}
	else if (m_pElect->electionRole() == QLeaderElection::VoteRole)
	{
		if ((++m_nCount) > CANDIDATE_WAIT_TIME_TICK)
		{
			if (m_timer.isActive())
			{
				m_timer.stop();
			}

			logplusDebug(LIBDDB_LOGGER_STR, "None rejucted to vote the local machine as the leader!");

			m_pElect->setElectionRole(QLeaderElection::LeaderRole);
			m_time=QDateTime::currentDateTime();

			m_pView->queryElectLeader("", 0);

			setLeader(QServerManager::instance()->localServer());

			m_timer.start();
		}
	}
	else if (m_pElect->electionRole() == QLeaderElection::LeaderRole)
	{
		m_pView->queryElectLeader("", m_time.secsTo(QDateTime::currentDateTime()));
	}
}

void QDdbManager::connected(QClient* client)
{
	Q_ASSERT_NO_NULL(client);
	Q_ASSERT_NO_NULL(m_pElect);

	m_pClient=client;
	m_nPortStep=0;

	logplusDebug(LIBDDB_LOGGER_STR, "Succeed to connect with the leader.");

	Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
	stream<<m_uid;

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_DDB);
	if (pMsg != NULL)
	{
		QByteArray buf=pMsg->query(QueryInformDdbConnection, m_nUsrID, buffer);

		client->sendMessage(buf, MSG_DDB);
	}
}

void QDdbManager::connectFailed(QClient* client)
{
	Q_ASSERT_NO_NULL(m_pSender);

	disconnected(client);
}

void QDdbManager::disconnected(QClient* client)
{
	m_pClient=NULL;

	if (m_pClientManager != NULL)
	{
		m_pClientManager->remove(client);
	}

	logplusDebug(LIBDDB_LOGGER_STR, "Disconnect with the leader, reelect a leader!");

	setElectionRole(QLeaderElection::CandidateRole);
	electLeader();
}

void QDdbManager::setLeader(const ServerInfo& info)
{
	if (electionRole() != QLeaderElection::LeaderRole)
	{
		if (m_timer.isActive())
		{
			m_timer.stop();
		}
	}
	else
	{
		m_pClientManager->removeAll();

		if (!m_timer.isActive())
		{
			m_timer.start();
		}
	}

	QClient* pClient=NULL;
	if (m_pClientManager != NULL)
	{
		pClient=m_pClientManager->addClient(info);
		if (pClient != NULL && !m_pClientManager->isConnected(pClient) && pClient->state() != QAbstractSocket::ConnectingState)
		{
			m_pClientManager->connectToHost(pClient);
		}
	}
}

int QDdbManager::leaderLapse(const QDateTime& datetime)
{
	return m_time.secsTo(datetime);
}

void QDdbManager::recvInformDdbConnection()
{
	queryUploadPrivilegeData(true);
}

void QDdbManager::recvPrivilegeData(int flag, const QMap<QString, PrivilegeGroup>& groups)
{
	QPrivilegeManager::instance()->setSynchronized(true);

	if (flag == PrivilegeDataModified)
	{
		QPrivilegeManager::instance()->updatePrivilegeGroups(groups);
	}
}

void QDdbManager::queryUploadPrivilegeData(bool isRetData)
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_DDB);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
		stream<<QPrivilegeManager::instance()->privilegeData()<<isRetData;

		QByteArray buf=pMsg->query(QueryUploadPrivilegeData, m_nUsrID, buffer);

		if (m_pClient != NULL)
		{
			m_pClient->sendMessage(buf, MSG_DDB);
		}
	}
}

void QDdbManager::privilegesChanged()
{
	queryUploadPrivilegeData(false);
}
