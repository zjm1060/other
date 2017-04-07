#include "qdlginstantupdate.h"

#include "qglobalinfo.h"
#include "common.h"
#include "qimessage.h"
#include "qdirex.h"
#include "qfileex.h"
#include "qmainwndsyncfile.h"
#include "qtreeviewupdatefile.h"
#include "qlogger.h"
#include "qutilreadwritexmlfile.h"

QDlgInstantUpdate::QDlgInstantUpdate(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	QStringList titles;
	titles<<tr("Action")<<tr("Path");

	ui.treeView->setRootIsDecorated(false);
	ui.treeView->setAlternatingRowColors(true);

	m_pModel=new QStandardItemModel(0, titles.size());
	ui.treeView->setModel(m_pModel);

	m_pDelegate=new QDelegateInstantUpdate(this);
	ui.treeView->setItemDelegate(m_pDelegate);

	m_pModel->setHorizontalHeaderLabels(titles);
}

QDlgInstantUpdate::~QDlgInstantUpdate()
{
	ui.treeView->setModel(NULL);
	SAFE_DELETE_CLASS(m_pModel);

	ui.treeView->setItemDelegate(NULL);
	SAFE_DELETE_CLASS(m_pDelegate);
}

void QDlgInstantUpdate::setTasks(const QString& workspace, const QString& history, const QString& configure, const QVector<QPair<DiffFile, FileInfo> >& files)
{
	ui.pushButtonOk->setEnabled(false);
	ui.pushButtonCancel->setEnabled(true);

	m_strConfigure=configure;

	QDlgInstantUpdateRunnable* pRunnable=new QDlgInstantUpdateRunnable(workspace, history, configure, files);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(sendMessage(int, QString)), this, SLOT(message(int, QString)));
	connect(pRunnable, SIGNAL(sendProcess(int, int)), this, SLOT(setProcess(int, int)));
	connect(pRunnable,SIGNAL(sendRecord(DiffFile, FileInfo)), this, SLOT(record(DiffFile, FileInfo)));

	connect(ui.pushButtonCancel, SIGNAL(clicked()), pRunnable, SLOT(onClickedBtnCancel()));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

void QDlgInstantUpdate::setRecordFile(const QString& workspace, const QString& hostname, const QString& address)
{
	m_strWorkspace=workspace;
	m_strHostName=hostname;
	m_strAddress=address;
}

void QDlgInstantUpdate::record(DiffFile diff, FileInfo file)
{
	//Ð´¼ÇÂ¼ÎÄ¼þ
	QMap<QString, DiffFile> files;

	QReadUpdateRecordsFromXmlFile reader(m_strConfigure, m_strWorkspace, m_strHostName, m_strAddress);
	if (reader.read())
	{
		files=reader.result();
	}

	if (diff.status != Delete)
	{
		FileInfo info=entryInfo(QFileInfo(appendPath(m_strWorkspace, file.name)), false);

		diff.path=file.name;
		diff.size=info.size;
		diff.type=info.type;
		diff.modified=info.lastModified;
		diff.status=Latest;

		files.insert(diff.path, diff);
	}
	else
	{
		files.remove(file.name);
	}

	QWriteUpdateRecordsToXmlFile writer(m_strConfigure, m_strWorkspace, m_strHostName, m_strAddress, files.values().toVector());
	writer.write(false);
}

void QDlgInstantUpdate::message(int flag, QString desc)
{
	if (m_pModel == NULL || ui.treeView == NULL)
	{
		return;
	}

	int nRow=m_pModel->rowCount();
	m_pModel->insertRow(nRow);

	QModelIndex index=m_pModel->index(nRow, ActionTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->setData(index, flag);

	index=m_pModel->index(nRow, PathTitle);
	m_pModel->itemFromIndex(index)->setEditable(false);
	m_pModel->setData(index, desc);

	if (flag == CompletedAction)
	{
		ui.progressBar->hide();

		ui.pushButtonCancel->setEnabled(false);
		ui.pushButtonOk->setEnabled(true);
	}

	ui.treeView->resizeColumnToContents(PathTitle);

	logplus(flag, desc);
}

void QDlgInstantUpdate::logplus(int flag, const QString& desc)
{
	switch (flag)
	{
	case CommandAction:
		logplusInfo(MANAGER_LOGGER_STR, QString("CommandAction:%1").arg(desc));
		break;
	case UpdatingAction:
		logplusInfo(MANAGER_LOGGER_STR, QString("UpdatingAction:%1").arg(desc));
		break;
	case AddedAction:
		logplusWarning(MANAGER_LOGGER_STR, QString("AddedAction:%1").arg(desc));
		break;
	case DeletedAction:
		logplusWarning(MANAGER_LOGGER_STR, QString("DeletedAction:%1").arg(desc));
		break;
	case UpdatedAction:
		logplusWarning(MANAGER_LOGGER_STR, QString("UpdatedAction:%1").arg(desc));
		break;
	case ConflictAction:
		logplusInfo(MANAGER_LOGGER_STR, QString("ConflictAction:%1").arg(desc));
		break;
	case ErrorAction:
		logplusWarning(MANAGER_LOGGER_STR, QString("ErrorAction:%1").arg(desc));
		break;
	case CompletedAction:
		logplusInfo(MANAGER_LOGGER_STR, QString("CompletedAction:%1").arg(desc));
		break;
	default:
		logplusInfo(MANAGER_LOGGER_STR, QString("InstantUpdate:%1").arg(desc));
		break;
	}
}

void QDlgInstantUpdate::setProcess(int current, int total)
{
	if (total > 0)
	{
		ui.progressBar->setValue(current*ui.progressBar->maximum()/total);
	}
	else
	{
		ui.progressBar->setValue(ui.progressBar->maximum());
	}
}

QDlgInstantUpdateRunnable::QDlgInstantUpdateRunnable(const QString& workspace, const QString& history,
													 const QString& configure, const QVector<QPair<DiffFile, FileInfo> >& files, QObject* parent/* =0 */)
:QObject(parent)
,m_strWorkspace(workspace)
,m_strHistory(history)
,m_strConfigure(configure)
,m_files(files)
,m_bCancel(false)
{

}

QDlgInstantUpdateRunnable::~QDlgInstantUpdateRunnable()
{

}

void QDlgInstantUpdateRunnable::onClickedBtnCancel()
{
	emit sendMessage(QDlgInstantUpdate::CommandAction, tr("Cancel..."));

	m_bCancel=true;
}

void QDlgInstantUpdateRunnable::run()
{
	emit sendMessage(QDlgInstantUpdate::CommandAction, tr("Update"));
	emit sendProcess(0, m_files.size());

	for (int i=0; i<m_files.size(); i++)
	{
		if (m_bCancel)
		{
			break;
		}

		QPair<DiffFile, FileInfo> file=m_files[i];

		QString strSource=appendPath(m_strHistory, file.first.path);
		QString strTarget=appendPath(m_strWorkspace, file.second.name);

		if (file.first.status == Add)
		{
			if (file.second.type == Dir || file.second.type == HideDir)
			{
				QDirEx dir;
				if (!dir.mkpath(strTarget))
				{
					emit sendMessage(QDlgInstantUpdate::ErrorAction, tr("Can\'t create folder \'%1\'.").arg(strTarget));
				}
				else
				{
					emit sendMessage(QDlgInstantUpdate::AddedAction, strTarget);
				}
			}
			else if (file.second.type == File || file.second.type == HideFile)
			{
				if (!QFileEx::copy(strSource, strTarget))
				{
					emit sendMessage(QDlgInstantUpdate::ErrorAction, tr("Can\'t create file \'%1\'.").arg(strTarget));
				}
				else
				{
					emit sendMessage(QDlgInstantUpdate::AddedAction, strTarget);
					emit sendRecord(file.first, file.second);
				}
			}
		}
		else if (file.first.status == Delete)
		{
			if (file.second.type == Dir || file.second.type == HideDir)
			{
				if (!deleteDir(strTarget))
				{
					emit sendMessage(QDlgInstantUpdate::ErrorAction, tr("Can\'t delete folder \'%1\'.").arg(strTarget));
				}
				else
				{
					emit sendMessage(QDlgInstantUpdate::DeletedAction, strTarget);
				}
			}
			else if (file.second.type == File || file.second.type == HideFile)
			{
				QFileEx fe(strTarget);
				if (fe.exists() && !fe.remove(strTarget))
				{
					emit sendMessage(QDlgInstantUpdate::ErrorAction, tr("Can\'t delete file \'%1\'. %2").arg(strTarget).arg(fe.errorString()));
				}
				else
				{
					emit sendMessage(QDlgInstantUpdate::DeletedAction, strTarget);
					emit sendRecord(file.first, file.second);
				}
			}
		}
		else if (file.first.status == Modify || file.first.status == Latest)
		{
			if (file.second.type == File || file.second.type == HideFile)
			{
				QVector<int> indexes;
				QReadMergeFromXmlFile reader(m_strConfigure, fileSuffix(strTarget));
				if (reader.read())
				{
					indexes=reader.result();
				}

				QFileCompareFactory::FileCompareResult result=QFileCompareFactory::instance()->compare(strSource, strTarget);
				copy(result.source.model, QModelIndex(), result.target.model, QModelIndex(), indexes);

				if (save(result.source, result.target))
				{
					emit sendMessage(QDlgInstantUpdate::UpdatedAction, strTarget);

					emit sendRecord(file.first, file.second);
				}
			}
		}

		emit sendProcess(i+1, m_files.size());
	}

	emit sendMessage(QDlgInstantUpdate::CompletedAction);
}

void QDlgInstantUpdateRunnable::copy(QAbstractItemModel* smodel, const QModelIndex& psindex, 
									 QAbstractItemModel* tmodel, const QModelIndex& ptindex, QVector<int>& indexes)
{
	if (smodel == NULL || tmodel == NULL)
	{
		return;
	}

	int nNumRows=smodel->rowCount(psindex);
	if (nNumRows == 0)
	{
		return;
	}

	for (int i=0; i<nNumRows; i++)
	{
		QModelIndex sindex=smodel->index(i, 0, psindex);
		QModelIndex tindex=tmodel->index(i, 0, ptindex);

		QVariant var=smodel->data(sindex, QAbstractFileCompare::RowRole);
		if (var.isValid() && var.toInt() != QAbstractFileCompare::BlankRowRole)
		{
			tmodel->setData(tindex, var, QAbstractFileCompare::AssignRowRole);
		}

		int nNumCols=smodel->columnCount(psindex);
		for (int j=0; j<nNumCols; j++)
		{
			if (!indexes.contains(j))
			{
				sindex=smodel->index(i, j, psindex);
				tindex=tmodel->index(i, j, ptindex);

				int nRowRole=tmodel->data(tmodel->index(i, 0, ptindex), QAbstractFileCompare::RowRole).toInt();
				if ((nRowRole != QAbstractFileCompare::CommonRowRole && nRowRole != QAbstractFileCompare::ModifyRowRole && nRowRole != QAbstractFileCompare::AppendRowRole) || 
					(nRowRole == QAbstractFileCompare::ModifyRowRole && tmodel->data(tindex, QAbstractFileCompare::ItemRole).toInt() != QAbstractFileCompare::CommonItemRole))
				{
					tmodel->setData(tindex, smodel->data(sindex));
					tmodel->setData(tindex, true, QAbstractFileCompare::AssignItemRole);

					logplusWarning(MANAGER_LOGGER_STR, tr("Line %1, Col %2:%3 --> %4").arg(tmodel->data(tindex).toString()).arg(smodel->data(sindex).toString()));
				}

				copy(smodel, sindex, tmodel, tindex, indexes);
			}
		}
	}
}

bool QDlgInstantUpdateRunnable::save(QAbstractFileCompare::FileCompareResult& source, QAbstractFileCompare::FileCompareResult& target)
{
	QPair<QModelIndex, QModelIndex> m_error;

	QAbstractFileCompare::ErrorCode code=QFileCompareFactory::instance()->write(source, target, m_error);
	switch (code)
	{
	case QAbstractFileCompare::NoError:
		return true;
	case QAbstractFileCompare::FileOpenError:
		{
			emit sendMessage(QDlgInstantUpdate::ErrorAction, tr("Failed to open file %1!").arg(target.filename));
		}
		break;
	case QAbstractFileCompare::FileWriteError:
		{
			emit sendMessage(QDlgInstantUpdate::ErrorAction, tr("Failed to write data to file %1!").arg(target.filename));
		}
		break;
	case QAbstractFileCompare::KeyPropertyIsEmpryError:
	case QAbstractFileCompare::KeyPropertyRepeatError:
		{
			emit sendMessage(QDlgInstantUpdate::ConflictAction, tr("Failed to update file %1! Please manually update!").arg(target.filename));
		}
		break;
	default:
		break;
	}

	return false;
}

QDelegateInstantUpdate::QDelegateInstantUpdate(QObject *parent)
: QStyledItemDelegate(parent)
{
}

QDelegateInstantUpdate::~QDelegateInstantUpdate()
{

}

void QDelegateInstantUpdate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QAbstractItemModel* pModel=index.model();

	drawItem(painter, option, pModel, index);
}

void QDelegateInstantUpdate::drawItem(QPainter* painter, const QStyleOptionViewItem& option, const QAbstractItemModel* model, const QModelIndex& index) const
{
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	QColor color(Qt::black);

	QModelIndex cindex=model->index(index.row(), QDlgInstantUpdate::ActionTitle, index.parent());
	int nAction=model->data(cindex).toInt();
	switch (nAction)
	{
	case QDlgInstantUpdate::UpdatedAction:
		{
			color=QColor(60, 90, 200);
		}
		break;
	case QDlgInstantUpdate::AddedAction:
		{
			color=QColor(128, 60, 180);
		}
		break;
	case QDlgInstantUpdate::CommandAction:
		{
			color=QColor(96, 96, 96);
		}
		break;
	case QDlgInstantUpdate::DeletedAction:
		{
			color=QColor(100, 0, 0);
		}
		break;
	case QDlgInstantUpdate::ErrorAction:
		{
			color=QColor(255, 0, 0);
		}
		break;
	case QDlgInstantUpdate::ConflictAction:
		{
			color=QColor(128, 0, 0);
		}
		break;
	default:
		break;
	}

	opt.palette.setColor(QPalette::Text, color);

	if (index.column() == QDlgInstantUpdate::ActionTitle)
	{
		switch (nAction)
		{
		case QDlgInstantUpdate::CommandAction:
			opt.text=tr("Command");
			break;
		case QDlgInstantUpdate::UpdatingAction:
			opt.text=tr("Updating");
			break;
		case QDlgInstantUpdate::AddedAction:
			opt.text=tr("Added");
			break;
		case QDlgInstantUpdate::DeletedAction:
			opt.text=tr("Deleted");
			break;
		case QDlgInstantUpdate::UpdatedAction:
			opt.text=tr("Updated");
			break;
		case QDlgInstantUpdate::ConflictAction:
			opt.text=tr("Conflict");
			break;
		case QDlgInstantUpdate::ErrorAction:
			opt.text=tr("Error");
			break;
		case QDlgInstantUpdate::CompletedAction:
			opt.text=tr("Completed");
			break;
		default:
			opt.text="";
			break;
		}

		painter->save();
		style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
		painter->restore();

		return;
	}

	QStyledItemDelegate::paint(painter, opt, index);
}

