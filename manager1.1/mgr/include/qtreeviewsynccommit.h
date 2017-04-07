#ifndef QTREEVIEWSYNCCOMMIT_H
#define QTREEVIEWSYNCCOMMIT_H

#include <QTreeView>
#include <QPoint>
#include <QRunnable>

#include "qutildef.h"
#include "qtreemodel.h"
#include "qdelegatesynccommit.h"
#include "qfilecomparefactory.h"
#include "qreadwritexmlfile.h"

class QTreeViewSyncCommit : public QTreeView
{
	Q_OBJECT

public:
	QTreeViewSyncCommit(QWidget *parent=0);
	~QTreeViewSyncCommit();

public:
	enum TitleLabel
	{
		NameTitle,
		SizeTitle,
		TypeTitle,
		StatusTitle,
		ModifiedTitle,
		TitleLabelEnd
	};

	enum ItemRoleType
	{
		GroupRole,
		FileRole
	};

	enum DiffFileUserData
	{
		ItemRole=Qt::UserRole,	//item角色
		CheckableRole,			//是否可选中
		CheckedRole,			//选中
		StatusRole,				//item状态描述
		UserDataRole			//文件信息
	};

public:
	virtual QString record() const=0;	//记录上次提交的文件信息
	virtual QString commonFolder() const=0;	//默认工作目录

public:
	/**
	*	@Desc	检查差异
	*
	*	@Param	workspace 工作目录，检查差异源目录
	*	@Param	history	历史记录目录，用于和工作目录进行比较，得出差异
	**/
	void check(const QString& workspace, const QString& histroy);

public:
	QVector<QPair<FileInfo, int> > selectedItems(const QModelIndex& parent=QModelIndex());

	bool fetchItem(const QModelIndex& index, QPair<FileInfo, int>& file);

	int rowCount(const QModelIndex& parent=QModelIndex()) const;

	void setItemsCheckable(bool checkable);
	int itemCheckState(int row, const QModelIndex& parent=QModelIndex()) const;

	virtual void setItemCheckState(int row, int state, const QModelIndex& parent=QModelIndex());

	QModelIndexList selectedRows();

	bool hasSelected() const;

	void setRestrictFilterEnabled(bool enable);

public:
	void save();	//保存记录

public:
	void clear();

Q_SIGNALS:
	void refreshKeyPress();

	void refreshModelFinished();
	void mouseRightButtonPressed(QPoint pos);

	void selectedChanged();

private slots:
	void finished(QTreeModel* model);

private:
	void setChildItemCheckState(int row, int state, const QModelIndex & parent=QModelIndex());
	void setParentItemCheckState(const QModelIndex& parent=QModelIndex());

	void setChildItemCheckable(int row, bool checkable, const QModelIndex & parent=QModelIndex());

private:
	void saveRecords(QVector<DiffFile>& files, const QModelIndex & parent=QModelIndex());

protected:
	virtual void checkDiff() = 0;	//子类实现差异检查

	virtual void setCustomItemDelegate();
	virtual void setTreeModel(QTreeModel* model);

	virtual QModelIndex mapToSourceItem(const QModelIndex& index);

	virtual void mousePressEvent(QMouseEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);

protected:
	QTreeModel*	m_pModel;
	QDelegateSyncCommit*	m_pDelegate;

	bool		m_bRestrictFilter;	//是否过滤限制文件

	QString		m_strWorkspace;		//工作目录
	QString		m_strHistory;		//历史备份目录

	bool m_bCheckable;
};

class QTreeViewSyncCommitRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QTreeViewSyncCommitRunnable(const QString& workspace, const QString& history, const QString& record, bool checkable, bool enableRestrict, QObject* parent=0);
	virtual ~QTreeViewSyncCommitRunnable();

signals:
	void finished(QTreeModel* model);

public:
	virtual void run();

protected:
	virtual void check()=0;

	bool traceSubDir(const QString& folder, const QString& suffix="", const QModelIndex& pindex=QModelIndex());		//遍历目录

	bool checkDir(const QString& folder, const QModelIndex& pindex=QModelIndex());		//检查文件夹
	bool checkFile(const QString& name, const QModelIndex& pindex=QModelIndex());	//检查文件

private:
	void checkBackupFiles();

protected:
	QString m_strWorkspace;	//修改目录
	QString m_strHistory;	//历史目录
	QString	m_strRecord;	//上次修改记录文件

	bool m_bCheckable;
	bool m_bRestrictFilter;	//是否过滤限制文件 

	QTreeModel*	m_pModel;

protected:
	QMap<QString, DiffFile>	m_files;		//上次修改记录文件
	QMap<QString, DiffFile>	m_filesBackup;	//历史备份文件
};

typedef QAbstractClassProducer<QTreeViewSyncCommit> QTreeViewSyncCommitProducer;
typedef QSharedObjectPtr<QTreeViewSyncCommitProducer> QTreeViewSyncCommitProducerPtr;
typedef QRegisterClassProducer<QString, QTreeViewSyncCommitProducerPtr> QTreeViewSyncCommitRegister;

template<typename T, class C>
class QRegisterTreeViewSyncCommit
{
public:
	QRegisterTreeViewSyncCommit(T t)
	{
		QTreeViewSyncCommitRegister::instance()->registerProducer(t, QTreeViewSyncCommitProducerPtr(new C));
	}
};

#define REGISTER_TREE_VIEW_SYNC_COMMIT_CLASS(name)	\
	static QRegisterTreeViewSyncCommit<QString, QClassProducer<name, QTreeViewSyncCommitProducer> > static##name##Instance(#name);

#define treeViewSyncCommitDynamicClass(key) \
	QTreeViewSyncCommitRegister::instance()->producer(key)==NULL? NULL:QTreeViewSyncCommitRegister::instance()->producer(key)->produce();

#endif // QTREEVIEWSYNCCOMMIT_H
