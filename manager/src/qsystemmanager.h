#ifndef QSYSTEMMANAGER_H
#define QSYSTEMMANAGER_H

#include <QFrame>
#include <QHBoxLayout>

#include "qbuttonlist.h"
#include "qframesystem.h"
#include "qclient.h"
#include "qframenetwork.h"

class QSystemManager : public QFrame
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
	void setOperatingSystem(QVector<OperatingSystem>& system);
	void setComputerSystem(QVector<ComputerSystem>& system);
	void setNetworkAdapter(QVector<NetworkAdapter>& adapter);

public:
	void queryOperatingSystem();
	void queryComputerSystem();
	void queryNetworkAdapter();

private slots:
	void buttonClicked(int id);

private:
	QHBoxLayout*	m_layout;
	QButtonList*	m_pButtonList;

	QClient*		m_pClient;

	QFrameSystem*	m_pFrameSystem;
	QFrameNetwork*	m_pFrameNetwork;
};

#endif // QSYSTEMMANAGER_H
