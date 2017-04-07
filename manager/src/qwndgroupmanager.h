#ifndef QWNDGROUPMANAGER_H
#define QWNDGROUPMANAGER_H

#include <QMainWindow>
#include <QAction>
#include <QMap>

#include "qclient.h"
#include "qsyncgroupmanager.h"

class QWndGroupManager : public QMainWindow
{
	Q_OBJECT

public:
	QWndGroupManager(QWidget *parent=0);
	~QWndGroupManager();

	enum ActionType
	{
		ActionSynchronization=0,
		ActionEnd
	};

signals:
	void onClosed(QWidget* widget);

private slots:
	void onActionToggled(QAction* action);

protected:
	virtual void closeEvent(QCloseEvent* event);

private:
	void init();

private:
	QActionGroup*	m_pGroup;

	QAction*	m_pAction[ActionEnd];
	QToolBar*	m_pBar;

	QVBoxLayout* m_pLayout;

	QSyncGroupManager*		m_pSyncGroupManager;
};

#endif // QWNDGROUPMANAGER_H
