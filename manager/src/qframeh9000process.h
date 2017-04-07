#ifndef QFRAMEH9000PROCESS_H
#define QFRAMEH9000PROCESS_H

#include <QFrame>
#include <QResizeEvent>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

#include "ui_h9000process.h"
#include "extern.h"

using namespace Ui;

class QFrameH9000Process : public QFrame, public Ui::FrameH9000Process
{
	Q_OBJECT

public:
	QFrameH9000Process(QWidget *parent=0);
	~QFrameH9000Process();

public:
	void setProcess(QVector<SystemProcess>& process);

private:
	void showProcess();

private:
	QStandardItemModel*		m_pModel;
	QSortFilterProxyModel*	m_pSortModel;
	QVector<SystemProcess>	m_vecProcess;
};

#endif // QFRAMEH9000PROCESS_H
