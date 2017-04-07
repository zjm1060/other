#ifndef QDLGINSTANTUPDATE_H
#define QDLGINSTANTUPDATE_H

#include <QDialog>
#include <QStandardItemModel>
#include <QRunnable>
#include <QStyledItemDelegate>

#include "ui_instantupdate.h"
#include "qutildef.h"
#include "qfilecomparefactory.h"

class QDelegateInstantUpdate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QDelegateInstantUpdate(QObject* parent=0);
	~QDelegateInstantUpdate();

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
	void drawItem(QPainter* painter, const QStyleOptionViewItem& option, const QAbstractItemModel* model, const QModelIndex& index) const;
};

class QDlgInstantUpdate : public QDialog
{
	Q_OBJECT

public:
	enum TitleLabel
	{
		ActionTitle,
		PathTitle,
		TitleEnd
	};

	enum UpdateActionType
	{
		CommandAction,
		UpdatingAction,
		AddedAction,
		DeletedAction,
		UpdatedAction,
		ConflictAction,
		ErrorAction,
		CompletedAction
	};

public:
	QDlgInstantUpdate(QWidget *parent = 0);
	~QDlgInstantUpdate();

public:
	void setTasks(const QString& workspace, const QString& history, const QString& configure, const QVector<QPair<DiffFile, FileInfo> >&  files);
	void setRecordFile(const QString& workspace, const QString& hostname, const QString& address);

private slots:
	void message(int flag, QString desc);
	void setProcess(int current, int total);

	void record(DiffFile diff, FileInfo file);

private:
	void logplus(int flag, const QString& desc);

private:
	Ui::DialogInstantUpdate ui;

	QStandardItemModel*		m_pModel;
	QDelegateInstantUpdate*	m_pDelegate;

private:
	QString		m_strConfigure;		//记录配置文件
	QString		m_strWorkspace;		//用于记录更改
	QString		m_strHostName;
	QString		m_strAddress;
};

class QDlgInstantUpdateRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QDlgInstantUpdateRunnable(const QString& workspace, const QString& history, const QString& configure, 
		const QVector<QPair<DiffFile, FileInfo> >&  files, QObject* parent=0);
	virtual ~QDlgInstantUpdateRunnable();

signals:
	void sendMessage(int flag, QString desc="");
	void sendProcess(int current, int total);
	void sendRecord(DiffFile diff, FileInfo file);

public:
	virtual void run();

public slots:
	void onClickedBtnCancel();

private:
	void copy(QAbstractItemModel* smodel, const QModelIndex& psindex, QAbstractItemModel* tmodel, const QModelIndex& ptindex, QVector<int>& indexes);
	bool save(QAbstractFileCompare::FileCompareResult& source, QAbstractFileCompare::FileCompareResult& target);

protected:
	QString m_strWorkspace;	//当前工作目录
	QString m_strHistory;	//提交目录
	QString	m_strConfigure;	//配置文件，读取配置信息
	
	QVector<QPair<DiffFile, FileInfo> >  m_files;	//需要更新文件列表

	bool	m_bCancel;
};

#endif // QDLGINSTANTUPDATE_H
