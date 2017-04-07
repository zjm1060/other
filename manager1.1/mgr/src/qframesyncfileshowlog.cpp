#include "qframesyncfileshowlog.h"

#include <QDir>
#include <QStylePainter>
#include <QMessageBox>

#include "common.h"
#include "qimessage.h"
#include "qglobalinfo.h"
#include "qlistviewsyncfileserver.h"
#include "qmainwndsyncfile.h"
#include "qtreeviewupdatefile.h"

#define SYNC_FILE_SHOW_LOG_ICON_SIZE 16
#define SYNC_FILE_SHOW_LOG_STATE_ICON_SIZE 10

QFrameSyncFileShowLog::QFrameSyncFileShowLog(const QString& workspace, const QString& record, QWidget *parent/* =0 */)
	: QFrame(parent)
	,m_strWorkspace(workspace)
	,m_strRecord(record)
{
	setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose, true);

	treeViewLogRecords->setItemDelegate(new QListViewLogRecordDelegate(this));
	treeViewLogRecords->setSelectionMode(QAbstractItemView::SingleSelection);
	treeViewLogRecords->setSortingEnabled(true);
	treeViewLogRecords->sortByColumn(LogRevisionTitle, Qt::DescendingOrder);
	treeViewLogRecords->setRootIsDecorated(false);

	connect(treeViewLogRecords, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));

	treeViewLogFiles->setItemDelegate(new QListViewLogFileDelegate(this));
	treeViewLogFiles->setSelectionMode(QAbstractItemView::SingleSelection);
	treeViewLogFiles->setRootIsDecorated(false);

	connect(treeViewLogFiles, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(doubleClicked(const QModelIndex&)));

	init();
}

QFrameSyncFileShowLog::~QFrameSyncFileShowLog()
{
	QAbstractItemModel* pModel=treeViewLogRecords->model();
	treeViewLogRecords->setModel(NULL);
	SAFE_DELETE_CLASS(pModel);

	QAbstractItemDelegate* pDelegate=treeViewLogRecords->itemDelegate();
	if (pDelegate != NULL)
	{
		SAFE_DELETE_CLASS(pDelegate);
	}

	pModel=treeViewLogFiles->model();
	treeViewLogFiles->setModel(NULL);
	SAFE_DELETE_CLASS(pModel);

	pDelegate=treeViewLogFiles->itemDelegate();
	if (pDelegate != NULL)
	{
		SAFE_DELETE_CLASS(pDelegate);
	}
}

void QFrameSyncFileShowLog::init()
{
	QSyncFileShowLogRecordsRunnable* pRunnable=new QSyncFileShowLogRecordsRunnable(appendPath(m_strWorkspace, m_strRecord), m_strRecord);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QStandardItemModel*)), this, SLOT(foundRecords(QStandardItemModel*)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

void QFrameSyncFileShowLog::clicked(const QModelIndex& index)
{
	if (!index.isValid())
	{
		return;
	}

	int nRevision=-1;

	QAbstractItemModel* pModel=treeViewLogRecords->model();
	nRevision=pModel->data(pModel->index(index.row(), LogRevisionTitle)).toInt();
	QString strFolder=pModel->data(pModel->index(index.row(), LogWorkspaceTitle)).toString();

	QSyncFileShowLogFilesRunnable* pRunnable=new QSyncFileShowLogFilesRunnable(
		appendPath(m_strWorkspace, QString("%1/%2/%3.xml").arg(m_strRecord).arg(nRevision).arg(m_strRecord)), strFolder);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QStandardItemModel*)), this, SLOT(foundFiles(QStandardItemModel*)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

void QFrameSyncFileShowLog::doubleClicked(const QModelIndex& index)
{
	int nRevision=-1;

	QAbstractItemModel* pModel=treeViewLogRecords->model();
	if (pModel == NULL)
	{
		return;
	}
	nRevision=pModel->data(pModel->index(treeViewLogRecords->currentIndex().row(), LogRevisionTitle)).toInt();
	QString strFolder=pModel->data(pModel->index(treeViewLogRecords->currentIndex().row(), LogWorkspaceTitle)).toString();

	pModel=treeViewLogFiles->model();
	if (pModel == NULL)
	{
		return;
	}

	int nType=pModel->data(pModel->index(index.row(), LogFileTypeTitle)).toInt();
	if (nType != File && nType != HideFile)
	{
		return;
	}

	QString strFilName=pModel->data(pModel->index(index.row(), LogFileNameTitle)).toString();

	if (!isTextFormat(strFilName))
	{
		QMessageBox::information(this, tr("Information"), tr("It is not supported to diff binary files!"),
			QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	QString strSource=appendPath(m_strWorkspace, QString("%1/%2/%3").arg(m_strRecord).arg(nRevision).arg(strFilName));

	QString strTarget=appendPath(strFolder, strFilName);

	QString strConfigure=appendPath(getAppParentDir(), QString("%1%2.xml").arg(SYNC_H9000_FILE_REQUEST_RECORD_PATH)
		.arg(m_strRecord));

	QMainWndSyncFile* pWnd=new QMainWndSyncFile(strSource, nRevision, strTarget, strConfigure);
	pWnd->showMaximized();
}

void QFrameSyncFileShowLog::foundRecords(QStandardItemModel* model)
{
	QAbstractItemModel* pModel=treeViewLogRecords->model();
	treeViewLogRecords->setModel(model);

	treeViewLogRecords->setColumnWidth(LogRevisionTitle, 80);
	treeViewLogRecords->setColumnWidth(LogActionsTitle, 150);
	treeViewLogRecords->setColumnWidth(LogDateTimeTitle, 150);
	treeViewLogRecords->setColumnWidth(LogWorkspaceTitle, 200);

	SAFE_DELETE_CLASS(pModel);

	if (model != NULL && model->rowCount()> 0)
	{
		QModelIndex index=model->index(0, 0);
		treeViewLogRecords->setCurrentIndex(index);

		clicked(index);
	}
}

void QFrameSyncFileShowLog::foundFiles(QStandardItemModel* model)
{
	QAbstractItemModel* pModel=treeViewLogFiles->model();
	treeViewLogFiles->setModel(model);

	treeViewLogFiles->setColumnWidth(LogFileActionTitle, 80);
	treeViewLogFiles->setColumnWidth(LogFileNameTitle, 200);
	treeViewLogFiles->setColumnWidth(LogFileSizeTitle, 100);
	treeViewLogFiles->setColumnWidth(LogFileTypeTitle, 100);
	treeViewLogFiles->setColumnWidth(LogFileModifiedTitle, 150);

	SAFE_DELETE_CLASS(pModel);
}

QListViewLogRecordDelegate::QListViewLogRecordDelegate(QObject *parent)
: QStyledItemDelegate(parent)
{
	QPixmap pixmap=findPixmap(":/images/log.png").scaled(SYNC_FILE_SHOW_LOG_ICON_SIZE, SYNC_FILE_SHOW_LOG_ICON_SIZE);
	m_pixmaps[Unmodified]=pixmap;
	m_pixmaps[Modified]=addPixmapToPixmap(findPixmap(":/images/modified.png").scaled(SYNC_FILE_SHOW_LOG_STATE_ICON_SIZE, SYNC_FILE_SHOW_LOG_STATE_ICON_SIZE),
		pixmap, Qt::BottomLeftCorner);
	m_pixmaps[Added]=addPixmapToPixmap(findPixmap(":/images/added.png").scaled(SYNC_FILE_SHOW_LOG_STATE_ICON_SIZE, SYNC_FILE_SHOW_LOG_STATE_ICON_SIZE),
		pixmap, Qt::BottomLeftCorner);
	m_pixmaps[Deleted]=addPixmapToPixmap(findPixmap(":/images/deleted.png").scaled(SYNC_FILE_SHOW_LOG_STATE_ICON_SIZE, SYNC_FILE_SHOW_LOG_STATE_ICON_SIZE),
		pixmap, Qt::BottomLeftCorner);
}

QListViewLogRecordDelegate::~QListViewLogRecordDelegate()
{

}

void QListViewLogRecordDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QAbstractItemModel* pModel=index.model();

	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	if (index.column() == QFrameSyncFileShowLog::LogRevisionTitle)
	{
		opt.displayAlignment=Qt::AlignVCenter|Qt::AlignRight;

		painter->save();

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		painter->restore();

		return;
	}
	else if (index.column() == QFrameSyncFileShowLog::LogActionsTitle)
	{
		QRect rect=option.rect;

		opt.text="";
		opt.features&=~QStyleOptionViewItemV2::HasDecoration;

		painter->save();

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		painter->restore();

		int nActions=pModel->data(index).toInt();
		for (int i=0; i<ModifiedStatusEnd; i++)
		{
			if ((nActions&(1<<i)) != NULL)
			{
				const QPixmap& pixmap=m_pixmaps[i];
				if (pixmap.isNull())
				{
					continue;
				}

				painter->drawPixmap(rect.left()+i*(SYNC_FILE_SHOW_LOG_ICON_SIZE+2), rect.top()+(rect.height()-pixmap.height())/2, pixmap);
			}
		}

		return;
	}

	QStyledItemDelegate::paint(painter, opt, index);
}

QSize QListViewLogRecordDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QStyledItemDelegate::sizeHint(option, index);
}

QListViewLogFileDelegate::QListViewLogFileDelegate(QObject *parent)
: QStyledItemDelegate(parent)
{

}

QListViewLogFileDelegate::~QListViewLogFileDelegate()
{

}

void QListViewLogFileDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QAbstractItemModel* pModel=index.model();

	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	QColor color(Qt::black);

	int nStatus=pModel->data(pModel->index(index.row(), QFrameSyncFileShowLog::LogFileActionTitle, index.parent())).toInt();
	Q_ASSERT(nStatus>=0 && nStatus<ModifiedStatusEnd);
	switch (nStatus)
	{
	case Modified:
		{
			color=QColor(60, 90, 200);
		}
		break;
	case Added:
		{
			color=QColor(128, 60, 180);
		}
		break;
	case Deleted:
		{
			color=QColor(100, 0, 0);
		}
		break;
	default:
		break;
	}

	opt.palette.setColor(QPalette::Text, color);

	if (index.column() == QFrameSyncFileShowLog::LogFileActionTitle)
	{
		switch (nStatus)
		{
		case Unmodified:
			opt.text=tr("unmodified");
			break;
		case Modified:
			opt.text=tr("modified");
			break;
		case Added:
			opt.text=tr("added");
			break;
		case Deleted:
			opt.text=tr("deleted");
			break;
		case NonVersioned:
			opt.text=tr("non-versioned");
			break;
		default:
			break;
		}

		painter->save();

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		painter->restore();
		
		return;
	}
	else if (index.column() == QFrameSyncFileShowLog::LogFileNameTitle)
	{
		int nType=pModel->data(pModel->index(index.row(), QFrameSyncFileShowLog::LogFileTypeTitle, index.parent())).toInt();
		QString strName=pModel->data(index).toString();

		opt.icon=fileIcon(strName, nType);
		opt.features|=QStyleOptionViewItemV2::HasDecoration;

		painter->save();

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		painter->restore();

		return;
	}
	else if (index.column() == QFrameSyncFileShowLog::LogFileSizeTitle)
	{
		int nType=pModel->data(pModel->index(index.row(), QFrameSyncFileShowLog::LogFileTypeTitle, index.parent())).toInt();
		if (nType != File && nType != HideFile)
		{
			opt.text="";
		}
		else
		{
			opt.text=fileSize(pModel->data(index).toULongLong());
		}
		
		opt.displayAlignment=Qt::AlignVCenter|Qt::AlignRight;

		painter->save();

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		painter->restore();

		return;
	}
	else if (index.column() == QFrameSyncFileShowLog::LogFileTypeTitle)
	{
		FileInfo file;
		file.type=pModel->data(index).toInt();
		file.isExecutable=false;
		file.name=pModel->data(pModel->index(index.row(), QFrameSyncFileShowLog::LogFileNameTitle, index.parent())).toString();
		opt.text=fileType(file);
		opt.displayAlignment=Qt::AlignVCenter|Qt::AlignRight;

		painter->save();

		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

		painter->restore();

		return;
	}

	QStyledItemDelegate::paint(painter, opt, index);
}

QSize QListViewLogFileDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QStyledItemDelegate::sizeHint(option, index);
}

QSyncFileShowLogRecordsRunnable::QSyncFileShowLogRecordsRunnable(const QString& folder, const QString& record,QObject* parent/* =0 */)
:QObject(parent)
,m_strWorkspace(folder)
,m_strRecord(record)
{
	
}

QSyncFileShowLogRecordsRunnable::~QSyncFileShowLogRecordsRunnable()
{

}

void QSyncFileShowLogRecordsRunnable::run()
{
	QStringList titles;
	titles<<tr("Revision")<<tr("Actions")<<tr("Date")<<tr("Workspace");

	QStandardItemModel* pModel=new QStandardItemModel(0, titles.size());
	pModel->setHorizontalHeaderLabels(titles);

	QDir dir(m_strWorkspace);
	QStringList folders=dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks, QDir::Time|QDir::Reversed);
	foreach (QString folder, folders)
	{
		bool bOk;
		int nRevision=folder.toInt(&bOk);
		if (!bOk)
		{
			continue;
		}

		QReadLogRecordsFormXmlFile reader(appendPath(m_strWorkspace, QString("%1/%2.xml").arg(folder).arg(m_strRecord)));
		if (reader.read())
		{
			QPair<QString, QVector<DiffFile> > files=reader.result();
			
			int nRow=pModel->rowCount();
			pModel->insertRow(nRow);

			setItemData(pModel, nRow, QFrameSyncFileShowLog::LogRevisionTitle, nRevision);

			int nActions=0;
			for (int i=0; i<files.second.size(); i++)
			{
				DiffFile& diff=files.second[i];

				nActions|=(1<<diff.status);
			}

			setItemData(pModel, nRow, QFrameSyncFileShowLog::LogActionsTitle, nActions);
			setItemData(pModel, nRow, QFrameSyncFileShowLog::LogDateTimeTitle, QFileInfo(appendPath(m_strWorkspace, folder)).lastModified());
			setItemData(pModel, nRow, QFrameSyncFileShowLog::LogWorkspaceTitle, files.first);
		}
	}

	emit finished(pModel);
}

void QSyncFileShowLogRecordsRunnable::setItemData(QStandardItemModel* model, int row, int column, const QVariant& var)
{
	QModelIndex index=model->index(row, column);
	model->itemFromIndex(index)->setEditable(false);
	model->setData(index, var);
}

QSyncFileShowLogFilesRunnable::QSyncFileShowLogFilesRunnable(const QString& filename, const QString& folder,QObject* parent/* =0 */)
:QObject(parent)
,m_strFileName(filename)
,m_strWorkspace(folder)
{

}

QSyncFileShowLogFilesRunnable::~QSyncFileShowLogFilesRunnable()
{

}

void QSyncFileShowLogFilesRunnable::run()
{
	QStringList titles;
	titles<<tr("Name")<<tr("Action")<<tr("Size")<<tr("Type")<<tr("Last Modified");

	QStandardItemModel* pModel=new QStandardItemModel(0, titles.size());
	pModel->setHorizontalHeaderLabels(titles);

	QReadLogRecordsFormXmlFile reader(m_strFileName);
	if (reader.read())
	{
		QPair<QString, QVector<DiffFile> > files=reader.result();
		if (files.first == m_strWorkspace)
		{
			for (int nRow=0; nRow<files.second.size(); nRow++)
			{
				pModel->insertRow(nRow);

				DiffFile& diff=files.second[nRow];

				setItemData(pModel, nRow, QFrameSyncFileShowLog::LogFileActionTitle, diff.status);
				setItemData(pModel, nRow, QFrameSyncFileShowLog::LogFileNameTitle, diff.path);
				setItemData(pModel, nRow, QFrameSyncFileShowLog::LogFileSizeTitle, diff.size);
				setItemData(pModel, nRow, QFrameSyncFileShowLog::LogFileTypeTitle, diff.type);
				setItemData(pModel, nRow, QFrameSyncFileShowLog::LogFileModifiedTitle, diff.modified);
			}
		}
	}
	emit finished(pModel);
}

void QSyncFileShowLogFilesRunnable::setItemData(QStandardItemModel* model, int row, int column, const QVariant& var)
{
	QModelIndex index=model->index(row, column);
	model->itemFromIndex(index)->setEditable(false);
	model->setData(index, var);
}

QReadLogRecordsFormXmlFile::QReadLogRecordsFormXmlFile(const QString& filename)
:QAbstractReadXmlFile(filename)
{

}

QReadLogRecordsFormXmlFile::~QReadLogRecordsFormXmlFile()
{

}

void QReadLogRecordsFormXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "folder", readFolderNode);
}

void QReadLogRecordsFormXmlFile::readFolderNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	QXmlStreamAttributes attributes=reader.attributes();
	if (attributes.hasAttribute("path"))
	{
		m_results.first=attributes.value("path").toString();
		if (!m_results.first.isEmpty())
		{
			readNextNode(reader, "file", readFileNode);
		}
	}

	readItemFinished(reader);
}

void QReadLogRecordsFormXmlFile::readFileNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("path") && attributes.hasAttribute("modified") && attributes.hasAttribute("size")
		&& attributes.hasAttribute("type") && attributes.hasAttribute("status"))
	{
		DiffFile file;
		file.path=attributes.value("path").toString();
		file.modified=QDateTime::fromString(attributes.value("modified").toString(), FILE_MODIFIED_DATETIME_FORMAT);
		file.size=attributes.value("size").toString().toULongLong();
		file.type=attributes.value("type").toString().toInt();
		file.status=attributes.value("status").toString().toInt();

		m_results.second.append(file);
	}

	readItemFinished(reader);
}
