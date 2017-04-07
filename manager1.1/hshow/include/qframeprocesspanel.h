#ifndef QFRAMEPROCESSPANEL_H
#define QFRAMEPROCESSPANEL_H

#include <QFrame>
#include <QResizeEvent>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

#include "qutildef.h"

class Ui_FrameProcess;

class QFrameProcessPanel : public QFrame
{
	Q_OBJECT

public:
	QFrameProcessPanel(QWidget *parent=0);
	~QFrameProcessPanel();

public:
	enum ProcessTitle
	{
		NameTitle,
		ProcessIDTitle,
		CpuTitle,
		SessionIDTitle,
		MemoryUsedTitle,
		UserNameTitle,
		PathTitle,
		TitleEnd
	};

public:
	void setSystemProcess(const QVector<SystemProcess>& process, quint64 memory);

public slots:
	void createNewTask();
	void terminateTask();

public:
	virtual void recvCreateProcess(const QStringList& command, int result);

signals:
	void terminate(const QString& name, quint32 pid);
	void execute(const QStringList& arguments);

protected:
	bool eventFilter(QObject* obj, QEvent* event);

private:
	void showSystemProcess();

private:
	Ui_FrameProcess*		m_ui;

	QStandardItemModel*		m_pModel;
	QSortFilterProxyModel*	m_pSortModel;

	QVector<SystemProcess>	m_vecProcess;
	quint64	m_nPhysicalMemory;
};

#endif // QFRAMEPROCESSPANEL_H
