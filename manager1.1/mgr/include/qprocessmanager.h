#ifndef QPROCESSMANAGER_H
#define QPROCESSMANAGER_H

#include <QFrame>
#include <QHBoxLayout>

#include "qclient.h"
#include "qbuttonlist.h"
#include "qframesystemprocess.h"
#include "qframeh9000process.h"
#include "qabstractreceiver.h"

class QProcessManager : public QFrame, public QAbstractReceiver
{
	Q_OBJECT

	enum ButtonId
	{
		ButtonH9000Process=0,
		ButtonSystemProcess
	};

public:
	QProcessManager(QClient* client, QWidget *parent=0);
	~QProcessManager();

public:
 	void setSystemProcess(const QVector<SystemProcess>& processes, quint64 memory);

public:
 	void querySystemProcess();
	void stopProcessInfo();

public:
	virtual void recvSystemProcess(int flag, const QVector<SystemProcess>& processes, quint64 memory);

private slots:
	void buttonClicked(int id);

private:
	QHBoxLayout*	m_layout;
	QButtonList*	m_pButtonList;

	quint32			m_nUsrID;
	QClient*		m_pClient;

	QFrameSystemProcess*	m_pFrameSystemProcess;
	QFrameH9000Process*		m_pFrameH9000Process;

	bool m_bQuery;
};

#endif // QPROCESSMANAGER_H
