#ifndef QDLGNEWTASK_H
#define QDLGNEWTASK_H

#include <QDialog>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

#include "ui_newtask.h"
#include "extern.h"
#include "qclient.h"
#include "qabstractfiletransfer.h"
#include "qframefilesystem.h"

using namespace Ui;

class QDlgNewTask : public QDialog, public Ui::DialogNewTask, public QAbstractFileTransfer
{
	Q_OBJECT

public:
	QDlgNewTask(QClient* client, QWidget *parent=0);
	~QDlgNewTask();

public:
	QString text();

public:
	virtual void recvRoot(const QString& root);
	virtual void recvPath(const QString& path, int flag, const QVector<FileInfo>& files);
	virtual void recvDrive(const QVector<FileInfo>& drives);
	virtual void recvSymLink(int flag, FileInfo info, const QString& symLink);

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

private:
	void setLocation(const QString& location);

	void queryRoot();
	void queryPath(const QString& strAddr);
	void queryDrives();
	void querySymLink(const QString& symLink);

	void gotoRemoteHome();
	void gotoRemoteLocation(const QString& location);

private:
	QFrameFileSystem*		m_pFrameFileSystem;

	QClient*	m_pClient;
	QString		m_strRoot;
	bool		m_bQueryComputer;	//是否显示在列表中，否则显示在地址下拉框中

	quint32		m_nUsrID;			//用户ID，用于查询目录信息
};

#endif // QDLGNEWTASK_H
