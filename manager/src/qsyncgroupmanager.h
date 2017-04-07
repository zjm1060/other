#ifndef QSYNCGROUPMANAGER_H
#define QSYNCGROUPMANAGER_H

#include <QFrame>
#include <QHBoxLayout>

#include "qbuttonlist.h"

#include "qframegroupsync.h"

class QSyncGroupManager : public QFrame
{
	Q_OBJECT

public:
	QSyncGroupManager(QWidget *parent=0);
	~QSyncGroupManager();

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
	QHBoxLayout*		m_layout;
	QButtonList*		m_pButtonList;

	QFrameGroupSync*		m_pFrameSync[ButtonEnd];
};

#endif // QSYNCGROUPMANAGER_H
