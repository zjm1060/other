#ifndef QSYSTEMMANAGER_H
#define QSYSTEMMANAGER_H

#include <QFrame>
#include <QHBoxLayout>

#include "qbuttonlist.h"
#include "qframesystem.h"
#include "qclient.h"
#include "qframenetwork.h"
#include "qabstractreceiver.h"

class QSystemManager : public QFrame, public QAbstractReceiver
{
	Q_OBJECT

	enum ButtonId
	{
		ButtonSystem=0,
		ButtonNetWork
	};

public:
	QSystemManager(QClient* client, QWidget *parent=0);
	~QSystemManager();

public:
	void reconnected();

public:
	void setOperatingSystem(const OperatingSystem& system);
	void setComputerSystem(const ComputerSystem& system);
	void setNetworkAdapter(const QVector<NetworkAdapter>& adapter);

public:
	void queryOperatingSystem();
	void queryComputerSystem();
	void queryNetworkAdapter();

public:
	virtual void recvOperatingSystem(int flag, const OperatingSystem& system);
	virtual void recvComputerSystem(int flag, const ComputerSystem& system);
	virtual void recvNetworkAdapter(int flag, const QVector<NetworkAdapter>& adapters);

private slots:
	void buttonClicked(int id);

private:
	QHBoxLayout*	m_layout;
	QButtonList*	m_pButtonList;

	quint32			m_nUsrID;
	QClient*		m_pClient;

	QFrameSystem*	m_pFrameSystem;
	QFrameNetwork*	m_pFrameNetwork;
};

#endif // QSYSTEMMANAGER_H
