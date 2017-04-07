#ifndef QFILETRANSFERMANAGER_H
#define QFILETRANSFERMANAGER_H

#include <QFrame>
#include <QHBoxLayout>

#include "qbuttonlist.h"
#include "qclient.h"

#include "qframetransferfile.h"

class QFileTransferManager : public QFrame
{
	Q_OBJECT

public:
	QFileTransferManager(QClient* client, QWidget *parent=0);
	~QFileTransferManager();

	enum ButtonId
	{
		ButtonConfigFiles,
		ButtonPictures,
		ButtonDBDefine,
		ButtonDatFiles,
		ButtonIEC104Files,
		ButtonSoundFiles,
		ButtonFileSystem,
		ButtonEnd
	};

private slots:
	void buttonClicked(int id);

private:
	QClient*		m_pClient;

	QHBoxLayout*		m_layout;
	QButtonList*		m_pButtonList;

	QFrameTransferFile*		m_pFrameFileTransfer[ButtonEnd];
};

#endif // QFILETRANSFERMANAGER_H
