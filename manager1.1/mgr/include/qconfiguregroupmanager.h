#ifndef QCONFIGUREGROUPMANAGER_H
#define QCONFIGUREGROUPMANAGER_H

#include <QFrame>
#include <QPixmap>
#include <QHBoxLayout>

#include "qbuttonlist.h"
#include "qutildef.h"
#include "qframebasicsettings.h"
#include "qframesynccommand.h"

class QConfigureGroupManager : public QFrame
{
	Q_OBJECT

public:
	QConfigureGroupManager(QWidget* parent=0);
	~QConfigureGroupManager();

	enum ButtonId
	{
		ButtonBasicSettings,
		ButtonSyncAssociate,
		ButtonEnd
	};

public:
	void setServerList(const QList<QPair<ServerInfo, int> >& servers);

signals:
	void labelSettingChanged(const QMap<QString, QString>& configure);

private slots:
	void buttonClicked(int id);

private:
	QHBoxLayout*	m_pLayout;
	QButtonList*	m_pButtonList;

	QFrameBasicSettings*	m_pFrameSettings;
	QFrameSyncCommand*		m_pFrameCommand;
};

#endif // QCONFIGUREGROUPMANAGER_H
