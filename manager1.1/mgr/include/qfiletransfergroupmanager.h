#ifndef QFILETRANSFERGROUPMANAGER_H
#define QFILETRANSFERGROUPMANAGER_H

#include <QFrame>
#include <QHBoxLayout>

#include "qbuttonlist.h"

#include "qframefiletransfergroup.h"

class QFileTransferGroupManager : public QFrame
{
	Q_OBJECT

public:
	QFileTransferGroupManager(QWidget *parent=0);
	~QFileTransferGroupManager();

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

public:
	void setServerList(const QList<QPair<ServerInfo, int> >& servers);

private slots:
	void buttonClicked(int id);

	void addServer(const Server& server);
	void removeServers(const QList<Server>& servers);

private:
	QHBoxLayout*		m_layout;
	QButtonList*		m_pButtonList;

	QList<Server>	m_addedServers;

	bool	m_bChanged;	//是否手动添加了文件接收端

	QFrameFileTransferGroup*	m_pFrameFileTransfer[ButtonEnd];
};

#endif // QFILETRANSFERGROUPMANAGER_H
