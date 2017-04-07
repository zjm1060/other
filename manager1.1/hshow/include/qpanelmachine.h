#ifndef QPANELMACHINE_H
#define QPANELMACHINE_H

#include "qabstractpanel.h"
#include "qabstractreceiver.h"

class QToolButton;
class QFloatingDockWidget;
class QFrameSystem;

class QPanelMachine : public QAbstractPanel, public QAbstractReceiver
{
	Q_OBJECT

public:
	QPanelMachine(QWidget *parent=0);
	~QPanelMachine();

public:
	virtual int belongsTo()	{ return PanelSystem; }
	virtual int sequence()	{ return MachinePanel; }

public:
	virtual void recvOperatingSystem(int flag, const OperatingSystem& system);
	virtual void recvComputerSystem(int flag, const ComputerSystem& system);

protected:
	virtual void showWidget(QObject* watched);
	virtual void hideWidget(QObject* watched);

private:
	void queryOperatingSystem();
	void queryComputerSystem();

private:
	QToolButton*	m_pButton;

	QFloatingDockWidget*	m_pDockWidget;
	QFrameSystem*			m_pPanel;

	quint32		m_nUsrID;
};

#endif // QPANELMACHINE_H
