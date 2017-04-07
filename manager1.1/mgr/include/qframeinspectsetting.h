#ifndef QFRAMEINSPECTSETTING_H
#define QFRAMEINSPECTSETTING_H

#include <QFrame>
#include <QPixmap>

#include "ui_inspectsetting.h"
#include "qlinkbutton.h"
#include "qlistviewcontrolledserver.h"
#include "qtreeviewinspectsetting.h"
#include "qutildef.h"

class QFrameInspectSetting : public QFrame, public Ui::FrameInspectSetting
{
	Q_OBJECT

public:
	QFrameInspectSetting(QWidget* parent=0);
	~QFrameInspectSetting();

public:
	void setServerList(const QList<QPair<ServerInfo, int> >& servers);

signals:
	void inspect(ServerInfo info, QTreeViewInspectSetting::InspectItem items);

private slots:
	void inspect();
	void setInspectEnabled(bool enabled);

	void refresh();

	void currentChanged(const QModelIndex& index);

private:
	QListViewControlledServer*	m_pListViewServers;
	QTreeViewInspectSetting*	m_pTreeViewSetting;

	QLinkButton*	m_pButtonRefresh;
};

#endif // QFRAMEINSPECT_H
