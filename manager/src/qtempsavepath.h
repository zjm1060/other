#ifndef QTEMPSAVEPATH_H
#define QTEMPSAVEPATH_H

#include <QAbstractItemModel>

#include "extern.h"
#include "qservermanager.h"
#include "qclientmanager.h"
#include "qabstractfiletransfer.h"
#include "qframefilesystem.h"

class QTempSavePath : public QObject, public QAbstractFileTransfer
{
	Q_OBJECT

public:
	QTempSavePath(QAbstractItemModel* model, const QModelIndex& index, int kind, 
		const QString& subPath, QObject *parent=0);
	~QTempSavePath();

public:
	void setClient(const QServerManager::ServerInfo& info);

public:
	void queryRoot();
	void queryPath(const QString& path);

protected:
	virtual void recvRoot(const QString& root);
	virtual void recvPath(const QString& path, int flag, const QVector<FileInfo>& files);

private slots:
	void connected(QClient* client);
	void disconnected(QClient* client);

private:
	QAbstractItemModel*	m_pModel;
	QModelIndex			m_index;

	QClient*			m_pClient;
	QClientManager*		m_pClientManager;

	quint32		m_nUsrID;		//用户ID，用于查询目录信息

	int			m_nKind;		//路径分类
	QString		m_strDefault;	//默认路径

	QString		m_strRoot;		//程序运行父目录
	QString		m_strSubPath;	//相对于父目录的路径
};

#endif // QTEMPSAVEPATH_H
