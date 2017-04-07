#ifndef QPANELMENU_H
#define QPANELMENU_H

#include "qabstractpanel.h"
#include "qreadwritexmlfile.h"
#include "qabstractreceiver.h"

class QActionGroup;
class QMenu;

class QPanelMenu : public QAbstractPanel, public QAbstractReceiver
{
	Q_OBJECT

public:
	QPanelMenu(QWidget *parent=0);
	~QPanelMenu();

public:
	virtual int belongsTo()	{ return PanelMenu; }
	virtual int sequence()	{ return 0; }

public:
	void loadConfigure();

	virtual void recvCreateProcess(const QStringList& command, int result);

public slots:
	void onActionToggled(QAction* action);

private:
	void queryExecuteProcess(const QStringList& arguments);

private:
	void loadMenu(QMenu* menu, const PanelMenuConfigure& configure);

private:
	QActionGroup*	m_pGroup;

	quint32		m_nUsrID;
};

#endif // QPANELMENU_H
