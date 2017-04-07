#ifndef QMULTICASTVIEW_H
#define QMULTICASTVIEW_H

#include <QObject>

#include "global.h"
#include "qmulticastsender.h"
#include "qutildef.h"

class NETWORK_EXPORT QMulticastView : public QObject
{
	Q_OBJECT

public:
	QMulticastView(QMulticastSenderPtr& sender, QObject *parent=0);
	~QMulticastView();

public:
	Q_INVOKABLE void queryElectLeader(const QString& from, int lapse);
	Q_INVOKABLE void answerElectLeader(const QString& from, int lapse, const ServerInfo& info);

	Q_INVOKABLE void queryVote(const QString& from, double performance);
	Q_INVOKABLE void answerVote(const QString& from);

private:
	QMulticastSenderPtr		m_pSender;
};

#endif // QMULTICASTVIEW_H
