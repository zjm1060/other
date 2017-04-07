#ifndef QFRAMESYSTEMPROCESS_H
#define QFRAMESYSTEMPROCESS_H

#include <QFrame>
#include <QResizeEvent>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

#include "ui_systemprocess.h"
#include "qclient.h"
#include "qabstractreceiver.h"

using namespace Ui;

class QFrameSystemProcess : public QFrame, public Ui::FrameSystemProcess, public QAbstractReceiver
{
	Q_OBJECT

public:
	QFrameSystemProcess(QClient* client, QWidget *parent=0);
	~QFrameSystemProcess();

public:
	void setSystemProcess(const QVector<SystemProcess>& process, quint64 memory);

public slots:
	void createNewTask();
	void terminateTask();

public:
	virtual void recvCreateProcess(const QStringList& command, int result);
	virtual void recvTerminateProcess(quint32 pid, int result);

protected:
	bool eventFilter(QObject* obj, QEvent* event);

private:
	void showSystemProcess();

private:
	QClient*		m_pClient;
	quint32			m_nUsrID;			//用户ID，用于接收创建终止进行信息

	QStandardItemModel*		m_pModel;
	QSortFilterProxyModel*	m_pSortModel;

	QVector<SystemProcess>	m_vecProcess;
	quint64	m_nPhysicalMemory;
};

#endif // QFRAMESYSTEMPROCESS_H
