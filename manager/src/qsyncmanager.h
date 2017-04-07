#ifndef QSYNCMANAGER_H
#define QSYNCMANAGER_H

#include <QFrame>
#include <QHBoxLayout>

#include "qbuttonlist.h"
#include "qclient.h"

#include "qframesyncfile.h"

class QSyncManager : public QFrame
{
	Q_OBJECT

public:
	QSyncManager(QClient* client, QWidget *parent=0);
	~QSyncManager();

	enum ButtonId
	{
		ButtonConfigureFile=0,
		ButtonPicture,
		ButtonDatabase,
		ButtonInterlock,
		ButtonCalculate,
		ButtonVoice,
		ButtonFileSystem,
		ButtonEnd
	};

private slots:
	void buttonClicked(int id);

private:
	QClient*		m_pClient;

	QHBoxLayout*		m_layout;
	QButtonList*		m_pButtonList;

	QFrameSyncFile*		m_pFrameSync[ButtonEnd];
};

#endif // QSYNCMANAGER_H
