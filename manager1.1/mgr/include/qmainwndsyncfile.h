#ifndef QMAINWNDSYNCFILE_H
#define QMAINWNDSYNCFILE_H

#include <QMainWindow>
#include <QTreeView>
#include <QPushButton>
#include <QSignalMapper>
#include <QAction>
#include <QMenu>
#include <QRunnable>
#include <QUndoStack>
#include <QCloseEvent>

#include "qtreeviewcomparefile.h"
#include "qfilecomparefactory.h"
#include "qreadwritexmlfile.h"

class QMainWndSyncFile : public QMainWindow
{
	Q_OBJECT

public:
	QMainWndSyncFile(const QString& source, int revision, const QString& target, const QString& configure, bool inverse=false, QWidget *parent=0);
	~QMainWndSyncFile();

signals:
	void modified();	//文件修改
	void closing();

protected:
	virtual void closeEvent(QCloseEvent* event);

private slots:
	void horizontalScroll(int value);
	void verticalScroll(int value);
	void sectionResized(int logicalIndex, int oldSize, int newSize);

	void blockSelection(const QModelIndexList& indexes);
	void itemSelection(const QModelIndex& index);
	void mouseRightButtonPress(QPoint pos);

	void actionTriggered(int index);

	void parseFinished(QFileCompareFactory::FileCompareResult result);

	bool onClickedBtnSave();
	void onClickedBtnRefresh();
	void onClickedBtnUseBlock();
	void onClickedBtnUseWholeFile();
	void onClickedBtnPreviousDifference();
	void onClickedBtnNextDifference();
	void onClickedBtnPreviousItemDifference();
	void onClickedBtnNextItemDifference();

	void lockedChanged();

private:
	void setMenuState();

	QVector<int> lockedIndexes() const;
	void saveLockedIndexes() const;

	void setMenuText(QObject* sender);

private:
	QTreeViewCompareFile*	m_pTreeViewSource;
	QTreeViewCompareFile*	m_pTreeViewTarget;

	QPushButton*		m_pButtonSource;
	QPushButton*		m_pButtonTarget;

private:
	enum ActionType
	{
		ActionSave,
		ActionRefresh,
		ActionUndo,
		ActionRedo,
		ActionPreDiff,
		ActionNextDiff,
		ActionPreDiffItem,
		ActionNextDiffItem,
		ActionUseBlock,
		ActionUseWholeFile,
		ActionEnd
	};

	QAction*	m_pActions[ActionEnd];
	QSignalMapper*	m_pSignalMapper;

	QUndoStack*	m_pUndoStack;

	QMenu*		m_pMenu;

private:
	QString		m_strSource;
	QString		m_strTarget;
	int			m_nRevision;

	QString		m_strConfigure;		//配置记录文件

	int			m_nCount;		//文档被修改次数计数
	int			m_nIndex;		//文档被保存时修改的次数
	bool		m_bLoading;		//正在加载文档

private:
	QFileCompareFactory::FileCompareResult m_result;	//比较文件结果
	QPair<QModelIndex, QModelIndex>	m_error;			//保存文件校验错误索引
};

class QMainWndSyncFileRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMainWndSyncFileRunnable(const QString& source, const QString& target,QObject* parent=0);
	virtual ~QMainWndSyncFileRunnable();

signals:
	void finished(QFileCompareFactory::FileCompareResult result);

public:
	virtual void run();

private:
	QString	m_strSourceFile;
	QString m_strTargetFile;
};

#endif // QMAINWNDSYNCFILE_H
