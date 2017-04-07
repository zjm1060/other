#ifndef QFRAMESYNCFILESHOWLOG_H
#define QFRAMESYNCFILESHOWLOG_H

#include <QFrame>
#include <QRunnable>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

#include "ui_syncfileshowlog.h"
#include "qreadwritexmlfile.h"

class QFrameSyncFileShowLog : public QFrame, public Ui::FrameSyncFileShowLog
{
	Q_OBJECT

public:
	QFrameSyncFileShowLog(const QString& workspace, const QString& record, QWidget *parent=0);
	~QFrameSyncFileShowLog();

	enum LogRecordTitle
	{
		LogRevisionTitle,
		LogActionsTitle,
		LogDateTimeTitle,
		LogWorkspaceTitle
	};

	enum LogFileTitle
	{
		LogFileNameTitle,
		LogFileActionTitle,
		LogFileSizeTitle,
		LogFileTypeTitle,
		LogFileModifiedTitle
	};

private slots:
	void foundRecords(QStandardItemModel* model);
	void foundFiles(QStandardItemModel* model);

	void clicked(const QModelIndex& index);
	void doubleClicked(const QModelIndex& index);

private:
	void init();

private:
	QString		m_strWorkspace;
	QString		m_strRecord;
};

class QListViewLogRecordDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QListViewLogRecordDelegate(QObject* parent=0);
	~QListViewLogRecordDelegate();

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
	QPixmap m_pixmaps[ModifiedStatusEnd];
};

class QListViewLogFileDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QListViewLogFileDelegate(QObject* parent=0);
	~QListViewLogFileDelegate();

public:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

class QSyncFileShowLogRecordsRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QSyncFileShowLogRecordsRunnable(const QString& folder, const QString& record,QObject* parent=0);
	virtual ~QSyncFileShowLogRecordsRunnable();

signals:
	void finished(QStandardItemModel* model);

public:
	virtual void run();

private:
	void setItemData(QStandardItemModel* model, int row, int column, const QVariant& var);

private:
	QString	m_strWorkspace;
	QString m_strRecord;
};

class QSyncFileShowLogFilesRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QSyncFileShowLogFilesRunnable(const QString& filename, const QString& folder,QObject* parent=0);
	virtual ~QSyncFileShowLogFilesRunnable();

signals:
	void finished(QStandardItemModel* model);

public:
	virtual void run();

private:
	void setItemData(QStandardItemModel* model, int row, int column, const QVariant& var);

private:
	QString	m_strFileName;
	QString	m_strWorkspace;
};

class QReadLogRecordsFormXmlFile : public QAbstractReadXmlFile
{
public:
	QReadLogRecordsFormXmlFile(const QString& filename);
	~QReadLogRecordsFormXmlFile();

public:
	QPair<QString, QVector<DiffFile> > result()
	{
		return m_results;
	}

protected:
	virtual void readNode(QXmlStreamReader& reader);

private:
	void readFolderNode(QXmlStreamReader& reader);
	void readFileNode(QXmlStreamReader& reader);

private:
	QPair<QString, QVector<DiffFile> >	m_results;
};


#endif // QFRAMESYNCFILESHOWLOG_H
