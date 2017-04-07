#ifndef QINSPECTMANAGER_H
#define QINSPECTMANAGER_H

#include <QFrame>
#include <QHBoxLayout>

#include "qframeinspectsetting.h"
#include "qframeinspect.h"

class QInspectManager : public QFrame
{
	Q_OBJECT

public:
	QInspectManager(QWidget *parent=0);
	~QInspectManager();

public:
	void setServerList(const QList<QPair<ServerInfo, int> >& servers);

private slots:
	void inspect(ServerInfo info, QTreeViewInspectSetting::InspectItem items);
	void goBack();

private:
	QHBoxLayout*		m_pLayout;

	QFrameInspectSetting*	m_pFrameSetting;
	QFrameInspect*			m_pFrameInspect;
};

#endif // QINSPECTMANAGER_H
