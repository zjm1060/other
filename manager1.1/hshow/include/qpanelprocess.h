#ifndef QPANELPROCESS_H
#define QPANELPROCESS_H

#include "qabstractpanel.h"
#include "qabstractreceiver.h"

class QToolButton;
class QFrameProcessPanel;

class QPanelProcess : public QAbstractPanel, public QAbstractReceiver
{
	Q_OBJECT

public:
	QPanelProcess(QWidget *parent=0);
	~QPanelProcess();

public:
	virtual int belongsTo()	{ return PanelSystem; }
	virtual int sequence()	{ return ProcessPanel; }

public slots:
	void onClickedBtn();

public:
	virtual void recvSystemProcess(int flag, const QVector<SystemProcess>& processes, quint64 memory);
	virtual void recvTerminateProcess(quint32 pid, int result);
	virtual void recvCreateProcess(const QStringList& command, int result);

protected:
	virtual void showWidget(QObject* watched);
	virtual void hideWidget(QObject* watched);

private:
	void queryProcesses();
	void stopProcesses();

private slots:
	void queryTerminateProcess(const QString& name, quint32 pid);
	void queryExecuteProcess(const QStringList& arguments);

private:
	QToolButton*	m_pButton;

	QFloatingDockWidget*	m_pDockWidget;
	QFrameProcessPanel*	m_pPanel;

	quint32		m_nUsrID;

	bool		m_bQuery;
};

#endif // QPANELPROCESS_H
