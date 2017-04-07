#ifndef QLISTVIEWSYNCFILEREQUESTSERVER_H
#define QLISTVIEWSYNCFILEREQUESTSERVER_H

#include <QStandardItemModel>
#include <QMouseEvent>
#include <QPair>
#include <QRunnable>

#include "qlistviewhorizontalicon.h"
#include "qutildef.h"
#include "qreadwritexmlfile.h"
#include "qfilecomparefactory.h"

class QListViewSyncFileRequestServer : public QListViewHorizontalIcon
{
	Q_OBJECT

public:
	QListViewSyncFileRequestServer(QWidget *parent=0);
	~QListViewSyncFileRequestServer();

public:
	enum UserDataRole
	{
		UserDataRole=Qt::UserRole,		//请求同步文件的发送方主机名与IP地址
		StatusRole,						//是否有更新
		SelectedRole					//主机是否被选中
	};

public:
	void loadServers(const QString& category);

public:
	void addServer(const QString& hostname, const QString& address);
	void setServerItemSelectStatus(const QModelIndex& index, bool select);
	void setServerItemModifiedStatus(const QModelIndex& index, bool modified);

	QPair<QString, QString> server(const QModelIndex& index);

	const QModelIndex findServer(const QString& hostname, const QString& address) const;

	const QModelIndex currentItem() const;

public slots:
	void modified(QString hostname, QString address);

	void clicked(const QModelIndex& index);

signals:
	void sendTips(int flag, const QString& message);
	void currentChaged(const QModelIndex& index);

	void loaded(QVector<QPair<QString, QString> > servers);	//加载完所有服务器

protected:
	virtual void mouseMoveEvent(QMouseEvent* event);

private slots:
	void entered(const QModelIndex& index);

private:
	void updateIcon(const QModelIndex& index);

private:
	QStandardItemModel*			m_pModel;

	QModelIndex		m_index;	//当前选中的项

private:
	enum IconType
	{
		HostIcon,
		CheckIcon,
		ModifiedIcon,
		IconTypeEnd
	};

	QPixmap		m_pixmap[IconTypeEnd];
};

class QReadRevisionRecordFormXmlFile : public QAbstractReadXmlFile
{
public:
	QReadRevisionRecordFormXmlFile(const QString& filename, const QString& workspace);
	~QReadRevisionRecordFormXmlFile();

public:
	QMap<QPair<QString, QString>, QVector<DiffFile> > result()
	{
		return m_results;
	}

protected:
	virtual void readNode(QXmlStreamReader& reader);

private:
	void readerWorkspaceNode(QXmlStreamReader& reader);
	void readFolderNode(QXmlStreamReader& reader);
	void readFileNode(QXmlStreamReader& reader);

private:
	QMap<QPair<QString, QString>, QVector<DiffFile> >	m_results;

	QString m_strWorkspace;
	QPair<QString, QString>	m_server;
};

#define REGISTER_TREE_VIEW_SYNC_COMMIT_CLASS(name)	\
	static QRegisterTreeViewSyncCommit<QString, QClassProducer<name, QTreeViewSyncCommitProducer> > static##name##Instance(#name);

#define treeViewSyncCommitDynamicClass(key) \
	QTreeViewSyncCommitRegister::instance()->producer(key)==NULL? NULL:QTreeViewSyncCommitRegister::instance()->producer(key)->produce();

QT_BEGIN_NAMESPACE                                                  
template <>                                                         
struct QMetaTypeId< QPair<QString, QString> >                                          
{                                                                   
	enum { Defined = 1 };                                           
	static int qt_metatype_id()                                     
	{                                                           
		static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0); 
		if (!metatype_id)                                       
			metatype_id = qRegisterMetaType< QPair<QString, QString> >("QPair<QString, QString>");     
		return metatype_id;                                     
	}                                                           
};                                                                  
QT_END_NAMESPACE

#endif // QLISTVIEWSYNCFILEREQUESTSERVER_H
