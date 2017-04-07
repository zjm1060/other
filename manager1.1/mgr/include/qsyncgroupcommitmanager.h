#ifndef QSYNCGROUPCOMMITMANAGER_H
#define QSYNCGROUPCOMMITMANAGER_H

#include <QFrame>
#include <QHBoxLayout>

#include "qbuttonlist.h"
#include "qclient.h"

#include "qframesyncgroupcommitfile.h"

class QSyncGroupCommitManager : public QFrame
{
	Q_OBJECT

public:
	QSyncGroupCommitManager(QWidget *parent=0);
	~QSyncGroupCommitManager();

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

	QFrameSyncGroupCommitFile*		m_pFrame[ButtonEnd];
};

#endif // QSYNCGROUPCOMMITMANAGER_H
