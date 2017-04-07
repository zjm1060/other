#ifndef QFRAMEREMOTEFILESYSTEM_H
#define QFRAMEREMOTEFILESYSTEM_H

#include <QFrame>

#include "extern.h"
#include "qservermanager.h"
#include "qclientmanager.h"
#include "qabstractfiletransfer.h"
#include "qframefilesystem.h"
#include "qframeremotefilesystem.h"

class QFrameRemoteFileSystem : public QFrame, public QAbstractFileTransfer
{
	Q_OBJECT

public:
	QFrameRemoteFileSystem(const QString& initPath, const QString& subPath, QWidget *parent=0);
	~QFrameRemoteFileSystem();

public:
	enum TipType
	{
		Information,
		Warning,
		Error,
		TipEnd
	};

	typedef struct _transferFileItem 
	{
		QString name;
		quint64 size;
		int type;
		QString typeDesc;
	}TransferFileItem;

public:
	void setClient(const QServerManager::ServerInfo& info);

	QVector<QFrameRemoteFileSystem::TransferFileItem> selectedFileItems();

	QString locationText();
	QString currentLocationText();

	bool canUpload();
	bool canDelete();
	bool canOpen();
	bool canRename();
	bool canNewDir();

	void deepRefresh();

	void doubleClickedLocation();

	void gotoHome();
	void gotoLocation(const QString& location);

public:
	void queryRoot();
	void queryDrives();
	void queryPath(const QString& path);

	void queryRename(const QString& location, int row, const QString& old, const QString& now);
	void queryNewDirectory(const QString& location);
	void queryDelete(const QString& location, const QStringList& files);

public slots:
	void refresh();

	void editTextChangedLocation(const QString& location);

	void newDirectory();
	void delDirectory();
	void rename();

signals:
	void unconnected();		//为建立连接
	void serverQuit();		//服务器退出
	void dirNoExist(const QString& dir);	//请求目录不存在
	void rootDirDeleted();	//程序运行目录不存在

	void enterInvalidChar(bool invalid, const QChar& ch);
	void focusStatusChanged(bool has);
	void mouseRightButtonPress(QPoint pos);

	void tip(int type, const QString& tip);

protected:
	virtual void recvRoot(const QString& root);
	virtual void recvDrive(const QVector<FileInfo>& drives);
	virtual void recvPath(const QString& path, int flag, const QVector<FileInfo>& files);
	virtual void recvNewDirectory(int flag, const FileInfo& file);
	virtual void recvDelete(int flag);
	virtual void recvRename(int row, QString old, QString now, int flag);

private slots:
	void enterPressed(QString location);
	void clickedToolButton(int type);

	void locationCurrentIndexChanged(QString location);
	void doubleClickedLocation(const QModelIndex& index);
	void dataChanged(QModelIndex index, QString old, QString now);

	void mouseRightButtonPressed(QPoint pos);

	void connected(QClient* client);

private:
	QFrameFileSystem*	m_pFrameFileSystem;

	QClient*			m_pClient;
	QClientManager*		m_pClientManager;

	quint32		m_nUsrID;		//用户ID，用于查询目录信息

	QString		m_strRoot;		//程序运行父目录
	QString		m_strSubPath;	//相对于父目录的路径

	QString		m_strInitPath;	//初始路径

	bool		m_bQueryComputer;	//请求获取计算机驱动器信息
};

#endif // QFRAMEREMOTEFILESYSTEM_H
