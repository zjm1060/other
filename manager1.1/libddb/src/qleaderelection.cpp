#include "qleaderelection.h"

#include "common.h"
#include "qlogger.h"
#include "qmulticastmsgparser.h"

QLeaderElection::QLeaderElection(const QString& uid, QMulticastSenderPtr& sender, QObject *parent/* =0 */)
	: QObject(parent)
	,m_uid(uid)
	,m_pSender(sender)
	,m_role(CandidateRole)
{
}

QLeaderElection::~QLeaderElection()
{

}

QLeaderElection::ElectionRole QLeaderElection::electionRole()
{
	QMutexLocker locker(&m_mutex);
	return m_role;
}

void QLeaderElection::setElectionRole(QLeaderElection::ElectionRole role)
{
	QMutexLocker locker(&m_mutex);
	m_role=role;
}

void QLeaderElection::queryElectLeader()
{
	setElectionRole(QLeaderElection::CandidateRole);

	logplusDebug(LIBDDB_LOGGER_STR, "Multicast message to elect a leader.");

	QIMulticastMsgPtr pMsg=QMulticastMsgParser::instance()->messageInstance(MULTICAST_MSG_ELECT);
	Q_ASSERT_NO_NULL(pMsg);

	QByteArray buffer=pMsg->query(QueryElectLeader);
	if (m_pSender != NULL)
	{
		m_pSender->sendDatagram(MULTICAST_MSG_ELECT, "", buffer);
	}
}

void QLeaderElection::queryVote(double performance)
{
	setElectionRole(QLeaderElection::VoteRole);

	Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
	stream<<performance;

	QIMulticastMsgPtr pMsg=QMulticastMsgParser::instance()->messageInstance(MULTICAST_MSG_ELECT);
	Q_ASSERT_NO_NULL(pMsg);

	QByteArray buf=pMsg->query(QueryVote, buffer);
	if (m_pSender != NULL)
	{
		m_pSender->sendDatagram(MULTICAST_MSG_ELECT, "", buf);
	}
}
