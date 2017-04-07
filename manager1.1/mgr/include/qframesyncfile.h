#ifndef QFRAMESYNCFILE_H
#define QFRAMESYNCFILE_H

#include <QFile>
#include <QTimer>
#include <QList>
#include <QRunnable>
#include <QXmlStreamReader>

#include "ui_syncfile.h"
#include "qclient.h"
#include "qabstractreceiver.h"
#include "qtreeviewsyncfile.h"

class QFrameSyncFile : public QFrame, public Ui::FrameSyncFile, public QAbstractReceiver
{
	Q_OBJECT

public:
	QFrameSyncFile(QClient* client, QWidget *parent=0);
	~QFrameSyncFile();

public:
	void setTitleText(const QString& title);
	void setDefaultDescText(const QString& desc);

	void setThemePixmap(const QPixmap& pixmap);
	void showTipsPixmap(bool show);

public:
	void refreshFileSystem();

public:
	void queryRoot();
	void queryUploadDir(int type, const QString& savePath);
	void queryUploadSymlink(int type, const QString& file, const QString& savePath);
	void queryUploadFile(const QString& file, int format, const QString&savePath);
	void queryInterruptTransfer();

protected:
	virtual void recvRoot(const QString& root);
	virtual void recvUploadDirAndSymlink(int flag);
	virtual void recvUploadFile(int queryFlag, int flag, quint64 size);
	virtual void recvInterruptTransfer(int flag);

protected:
	void transferFile(int row, const DiffFile& task);
	void uploads();

	virtual void cancel();
	virtual void finished();

private slots:
	void timeout();

	void readCommonFolderFinished(QList<QString> folders);
	void diffFolderFinished(QList<DiffFile> files);

	void currentIndexChanged(int index);
	void refreshModelFinished();

	void onClickedBtnOk();
	void onClickedBtnCancel();
	void onClickedBtnRefresh();
	void onClickedBtnAll();
	void onClickedBtnInverse();

private:
	void fetchTask();
	int findFileFormat(const QString& name, int format) const;

	void syncFinished();

protected:
	QClient*	m_pClient;
	quint32		m_nUsrID;		//用户ID，用于查询目录信息
	quint32		m_nDynamicID;	//用户ID，用于文件传输

private:
	QString		m_strDefaultDesc;		//默认提示信息描述

	QTreeViewSyncFile*	m_pTreeView;

	QString		m_strPath;		//本地上传目录
	QString		m_strSavePath;	//远端保存目录

	int			m_nRow;			//当前同步文件位于同步列表行号
	DiffFile	m_task;		//任务
	bool		m_bStatusTransfer;	//传输状态， true传输完成，false传输失败

private:
	QTimer		m_timer;		//掉线传输超时
	int			m_nTick;
};

class QSyncFileReadCommonFolderRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QSyncFileReadCommonFolderRunnable(const QString& file,QObject* parent=0);
	virtual ~QSyncFileReadCommonFolderRunnable();

signals:
	void finished(QList<QString> folders);

public:
	virtual void run();

private:
	void readRoot(QXmlStreamReader& reader);
	void readFolder(QXmlStreamReader& reader);
	void readUnknownElement(QXmlStreamReader& reader);

private:
	QString	m_strFile;

	QList<QString>	m_folders;
};

class QSyncFileReadFolderDiffRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QSyncFileReadFolderDiffRunnable(const QString& file, const QString& attr,QObject* parent=0);
	virtual ~QSyncFileReadFolderDiffRunnable();

signals:
	void finished(QList<DiffFile> files);

public:
	virtual void run();

private:
	void readRoot(QXmlStreamReader& reader);
	void readFolder(QXmlStreamReader& reader);
	void readFile(QXmlStreamReader& reader);
	void readUnknownElement(QXmlStreamReader& reader);

private:
	QString	m_strFile;
	QString m_strAttr;

	QMap<QString, DiffFile>	m_files;
};

#endif // QFRAMESYNCFILE_H
