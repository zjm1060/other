#ifndef QSYNCGROUPUPDATEMANAGER_H
#define QSYNCGROUPUPDATEMANAGER_H

#include <QFrame>
#include <QHBoxLayout>

#include "qbuttonlist.h"
#include "qclient.h"

#include "qframesyncgroupupdate.h"

class QSyncGroupUpdateManager : public QFrame
{
	Q_OBJECT

public:
	QSyncGroupUpdateManager(QWidget *parent=0);
	~QSyncGroupUpdateManager();

	enum ButtonId
	{
		ButtonConfigFiles,
		ButtonPictures,
		ButtonDBDefine,
		ButtonDatFiles,
		ButtonXLDIEC104Files,
		ButtonXJBIEC104Files,
		ButtonSoundFiles,
		ButtonEnd
	};

public:
	void setServerList(const QList<QPair<ServerInfo, int> >& servers);

private slots:
	void buttonClicked(int id);

private:
	QHBoxLayout*		m_layout;
	QButtonList*		m_pButtonList;

	QFrameSyncGroupUpdate*		m_pFrame[ButtonEnd];
};

#endif // QSYNCGROUPUPDATEMANAGER_H
