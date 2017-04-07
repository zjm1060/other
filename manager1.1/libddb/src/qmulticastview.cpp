#include "qmulticastview.h"

#include "qlogger.h"
#include "qddbmanager.h"
#include "qservermanager.h"
#include "qmulticastmsgparser.h"

QMulticastView::QMulticastView(QMulticastSenderPtr& sender, QObject *parent/* =0 */)
	: QObject(parent)
	,m_pSender(sender)
{
}

QMulticastView::~QMulticastView()
{

}

void QMulticastView::queryElectLeader(const QString& from, int lapse)
{
	if (QDdbManager::instance()->electionRole() == QLeaderElection::LeaderRole)
	{
		logplusDebug(LIBDDB_LOGGER_STR, "The local machine is the leader!");

		//如果当前服务器是领导者角色，多播其管理地址
		if (m_pSender != NULL)
		{
			const ServerInfo& info=QServerManager::instance()->localServer();

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

			stream<<lapse<<info;

			QIMulticastMsgPtr pMsg=QMulticastMsgParser::instance()->messageInstance(MULTICAST_MSG_ELECT);
			Q_ASSERT_NO_NULL(pMsg);

			QByteArray buf=pMsg->query(AnswerElectLeader, buffer);
			if (m_pSender != NULL)
			{
				m_pSender->sendDatagram(MULTICAST_MSG_ELECT, from, buf);
			}
		}
	}
}

void QMulticastView::answerElectLeader(const QString& from, int lapse, const ServerInfo& info)
{
	//存在领导者
	if (m_pSender != NULL && m_pSender->uid() != from)
	{
		if (QDdbManager::instance()->electionRole() != QLeaderElection::LeaderRole || 
			QDdbManager::instance()->leaderLapse(QDateTime::currentDateTime()) < lapse)
		{
			QDdbManager::instance()->setElectionRole(QLeaderElection::ManagedRole);

			QDdbManager::instance()->setLeader(info);
		}
		else
		{
			logplusDebug(LIBDDB_LOGGER_STR, QString("Find another invalid leader named %1, the local machine is voted as the leader.").arg(from));

			QDdbManager::instance()->setElectionRole(QLeaderElection::LeaderRole);
			QDdbManager::instance()->setLeader(QServerManager::instance()->localServer());
		}
	}
}

void QMulticastView::queryVote(const QString& from, double performance)
{
	if (m_pSender != NULL && m_pSender->uid() != from)
	{
		if ((qrand()%100)/100.0 < performance)
		{
			QIMulticastMsgPtr pMsg=QMulticastMsgParser::instance()->messageInstance(MULTICAST_MSG_ELECT);
			Q_ASSERT_NO_NULL(pMsg);

			QByteArray buf=pMsg->query(AnswerVote);
			if (m_pSender != NULL)
			{
				m_pSender->sendDatagram(MULTICAST_MSG_ELECT, from, buf);
			}
		}
	}
}

void QMulticastView::answerVote(const QString& from)
{
	logplusDebug(LIBDDB_LOGGER_STR, QString("Rejected to vote the local machine as the leader by %1!").arg(from));

	QDdbManager::instance()->setElectionRole(QLeaderElection::UncheckedRole);
}
