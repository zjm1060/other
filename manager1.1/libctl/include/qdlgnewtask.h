#ifndef QDLGNEWTASK_H
#define QDLGNEWTASK_H

#include <QDialog>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

#include "qclient.h"
#include "qabstractreceiver.h"
#include "qframefilesystem.h"

#include "libctlglobal.h"

class Ui_DialogNewTask;

class CTL_EXPORT QDlgNewTask : public QDialog, public QAbstractReceiver
{
	Q_OBJECT

public:
	QDlgNewTask(QClient* client, QWidget *parent=0);
	~QDlgNewTask();

public:
	QStringList task();

public:
	virtual void recvRoot(const QString& root);
	virtual void recvPath(const QString& path, int flag, const QVector<FileInfo>& files);
	virtual void recvDrives(const QVector<FileInfo>& drives);
	virtual void recvSymLink(int flag, const FileInfo& info, const QString& symLink);

protected:
	bool eventFilter(QObject* obj, QEvent* event);

private slots:
	void onToggledExpandButton(bool bExpand);
	void textChangedNewTask(const QString& text);

	void clickedToolButton(int type);
	void locationCurrentIndexChanged(QString location);
	void editLocationTextChanged(const QString& location);
	void enterPressed(QString location);
	void focusStatusChanged(bool has);
	void doubleClickedRemoteLocation(const QModelIndex& index);

	void backspaceKeyPress();
	void refreshKeyPress();

private:
	void setLocation(const QString& location);

	void queryRoot();
	void queryPath(const QString& path);
	void queryDrives();
	void querySymLink(const QString& symLink);

	void gotoRemoteHome();
	void gotoRemoteLocation(const QString& location);

	QStringList parseCommand(const QString& text) const;

private:
	Ui_DialogNewTask*		m_ui;

	QFrameFileSystem*		m_pFrameFileSystem;

	QClient*	m_pClient;
	QString		m_strRoot;
	bool		m_bQueryComputer;	//是否显示在列表中，否则显示在地址下拉框中
	bool		m_bQueryHomePath;	//请求主目录

	quint32		m_nUsrID;			//用户ID，用于查询目录信息
};

#endif // QDLGNEWTASK_H
