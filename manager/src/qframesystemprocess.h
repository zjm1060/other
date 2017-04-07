#ifndef QFRAMESYSTEMPROCESS_H
#define QFRAMESYSTEMPROCESS_H

#include <QFrame>
#include <QResizeEvent>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

#include "ui_systemprocess.h"
#include "extern.h"
#include "qclient.h"

using namespace Ui;

class QFrameSystemProcess : public QFrame, public Ui::FrameSystemProcess
{
	Q_OBJECT

public:
	QFrameSystemProcess(QClient* client, QWidget *parent=0);
	~QFrameSystemProcess();

public:
	void setSystemProcess(QVector<SystemProcess>& process);

public slots:
	void createNewTask();
	void terminateTask();

private:
	void showSystemProcess();

private:
	QClient*		m_pClient;

	QStandardItemModel*		m_pModel;
	QSortFilterProxyModel*	m_pSortModel;
	QVector<SystemProcess>	m_vecProcess;
};

#endif // QFRAMESYSTEMPROCESS_H
