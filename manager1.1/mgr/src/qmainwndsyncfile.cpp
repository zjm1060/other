#include "qmainwndsyncfile.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QToolBar>
#include <QHeaderView>
#include <QMessageBox>

#include "common.h"
#include "qglobalinfo.h"
#include "qlistviewsyncfilerequestserver.h"
#include "qtreeviewupdatefile.h"
#include "qlogger.h"
#include "qutilreadwritexmlfile.h"

QMainWndSyncFile::QMainWndSyncFile(const QString& source, int revision, const QString& target, const QString& configure, bool inverse/* =false */, QWidget *parent/* =0 */)
	: QMainWindow(parent)
	,m_strSource(source)
	,m_strTarget(target)
	,m_nRevision(revision)
	,m_nCount(0)
	,m_nIndex(0)
	,m_strConfigure(configure)
	,m_bLoading(false)
{
	setWindowTitle(QString("%1 - %2%3 - [%4]").arg(tr("Merge")).arg(tr("Manager")).arg(MANAGER_VERSION_STR).arg(target));
	setWindowIcon(QIcon(":/images/merge.png"));
	setAttribute(Qt::WA_DeleteOnClose, true);

	logplusInfo(MANAGER_LOGGER_STR, QString("Update: %1 --> %2").arg(source).arg(target));

	QToolBar* pToolBar=addToolBar(tr("Tools"));
	pToolBar->setMovable(false);
	pToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

	m_pUndoStack=new QUndoStack(pToolBar);

	m_pActions[ActionSave]=new QAction(QIcon(":/images/save.png"), tr("Save\tCtrl+S"), pToolBar);
	m_pActions[ActionSave]->setShortcuts(QKeySequence::Save);
	m_pActions[ActionRefresh]=new QAction(QIcon(":/images/refresh.png"), tr("Refresh\tF5"), pToolBar);
	m_pActions[ActionRefresh]->setShortcuts(QKeySequence::Refresh);
	m_pActions[ActionUndo]=m_pUndoStack->createUndoAction(pToolBar, tr("Undo\tCtrl+Z"));
	m_pActions[ActionUndo]->setIcon(QIcon(":/images/undo.png"));
	m_pActions[ActionUndo]->setShortcuts(QKeySequence::Undo);
	m_pActions[ActionRedo]=m_pUndoStack->createRedoAction(pToolBar, tr("Redo\tCtrl+Shift+Z"));
	m_pActions[ActionRedo]->setIcon(QIcon(":/images/redo.png"));
	m_pActions[ActionRedo]->setShortcuts(QKeySequence::Redo);
	m_pActions[ActionPreDiff]=new QAction(QIcon(":/images/gopervious.png"), tr("Previous Difference\tShift+F3"), pToolBar);
	m_pActions[ActionPreDiff]->setShortcut(QKeySequence(tr("Shift+F3")));
	m_pActions[ActionNextDiff]=new QAction(QIcon(":/images/gonext.png"), tr("Next Difference\tF3"), pToolBar);
	m_pActions[ActionNextDiff]->setShortcut(QKeySequence(tr("F3")));
	m_pActions[ActionPreDiffItem]=new QAction(QIcon(":/images/goleft.png"), tr("Previous Difference item\tShift+F4"), pToolBar);
	m_pActions[ActionPreDiffItem]->setShortcut(QKeySequence(tr("Shift+F4")));
	m_pActions[ActionNextDiffItem]=new QAction(QIcon(":/images/goright.png"), tr("Next Difference item\tF4"), pToolBar);
	m_pActions[ActionNextDiffItem]->setShortcut(QKeySequence(tr("F4")));
	m_pActions[ActionUseBlock]=new QAction(QIcon(":/images/apply.png"), tr("Use Block"), pToolBar);
	m_pActions[ActionUseWholeFile]=new QAction(QIcon(":/images/applywhole.png"), tr("Use the whole file"), pToolBar);

	pToolBar->addAction(m_pActions[ActionSave]);
	pToolBar->addAction(m_pActions[ActionRefresh]);
	pToolBar->addSeparator();
	pToolBar->addAction(m_pActions[ActionUndo]);
	pToolBar->addAction(m_pActions[ActionRedo]);
	pToolBar->addSeparator();
	pToolBar->addAction(m_pActions[ActionPreDiff]);
	pToolBar->addAction(m_pActions[ActionNextDiff]);
	pToolBar->addAction(m_pActions[ActionPreDiffItem]);
	pToolBar->addAction(m_pActions[ActionNextDiffItem]);
	pToolBar->addSeparator();
	pToolBar->addAction(m_pActions[ActionUseBlock]);
	pToolBar->addAction(m_pActions[ActionUseWholeFile]);

	m_pMenu=new QMenu(tr("Tools"), this);
	m_pMenu->addAction(m_pActions[ActionPreDiff]);
	m_pMenu->addAction(m_pActions[ActionNextDiff]);
	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionUseBlock]);
	m_pMenu->addAction(m_pActions[ActionUseWholeFile]);
	m_pMenu->addSeparator();
	m_pMenu->addAction(m_pActions[ActionRefresh]);

	//菜单触发事件
	m_pSignalMapper=new QSignalMapper(this);
	for (int i=0; i<ActionEnd; i++)
	{
		connect(m_pActions[i], SIGNAL(triggered()), m_pSignalMapper, SLOT(map()));
		m_pSignalMapper->setMapping(m_pActions[i], i);
	}

	connect(m_pSignalMapper, SIGNAL(mapped(int)), this, SLOT(actionTriggered(int)));

	QWidget* pWidgetCenter=new QWidget(this);
	setCentralWidget(pWidgetCenter);

	QHBoxLayout* pHBoxLayout=new QHBoxLayout();

	QVBoxLayout* pVBoxLayoutSource=new QVBoxLayout();

	QString strFileName=parseNameFromPath(source);
	if (strFileName.isEmpty())
	{
		strFileName=parseNameFromPath(target);
	}
	m_pButtonSource=new QPushButton(QString("%1 : %2,%3 %4").arg(strFileName).arg(tr("Committed")).arg(tr("Revision")).arg(m_nRevision),
		pWidgetCenter);
	QFont font=m_pButtonSource->font();
	font.setBold(true);
	m_pButtonSource->setFont(font);
	m_pButtonSource->setEnabled(false);

	pVBoxLayoutSource->addWidget(m_pButtonSource);

	m_pTreeViewSource=new QTreeViewCompareFile(inverse, false, pWidgetCenter);
	connect(m_pTreeViewSource, SIGNAL(blockSelection(const QModelIndexList&)), this, SLOT(blockSelection(const QModelIndexList&)));
	connect(m_pTreeViewSource, SIGNAL(itemSelection(const QModelIndex&)), this, SLOT(itemSelection(const QModelIndex&)));
	connect(m_pTreeViewSource, SIGNAL(mouseRightButtonPress(QPoint)), this, SLOT(mouseRightButtonPress(QPoint)));

	pVBoxLayoutSource->addWidget(m_pTreeViewSource);

	pHBoxLayout->addLayout(pVBoxLayoutSource);

	QVBoxLayout* pVBoxLayoutTarget=new QVBoxLayout();

	m_pButtonTarget=new QPushButton(QString("%1 : %2").arg(parseNameFromPath(target)).arg(tr("Current")), pWidgetCenter);
	font=m_pButtonTarget->font();
	font.setBold(true);
	m_pButtonTarget->setFont(font);
	m_pButtonTarget->setEnabled(false);

	pVBoxLayoutTarget->addWidget(m_pButtonTarget);

	m_pTreeViewTarget=new QTreeViewCompareFile(inverse, true, pWidgetCenter);
	connect(m_pTreeViewTarget, SIGNAL(blockSelection(const QModelIndexList&)), this, SLOT(blockSelection(const QModelIndexList&)));
	connect(m_pTreeViewTarget, SIGNAL(itemSelection(const QModelIndex&)), this, SLOT(itemSelection(const QModelIndex&)));
	connect(m_pTreeViewTarget, SIGNAL(mouseRightButtonPress(QPoint)), this, SLOT(mouseRightButtonPress(QPoint)));
	connect(m_pTreeViewTarget, SIGNAL(lockedChanged()), this, SLOT(lockedChanged()));

	pVBoxLayoutTarget->addWidget(m_pTreeViewTarget);

	pHBoxLayout->addLayout(pVBoxLayoutTarget);

//	pVBoxLayout->addLayout(pHBoxLayout);

// 	m_pTreeViewSave=new QTreeView(pWidgetCenter);
// 	pVBoxLayout->addWidget(m_pTreeViewSave);

	pWidgetCenter->setLayout(pHBoxLayout);

	connect(m_pTreeViewSource->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(horizontalScroll(int)));
	connect(m_pTreeViewTarget->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(horizontalScroll(int)));

	connect(m_pTreeViewSource->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(verticalScroll(int)));
	connect(m_pTreeViewTarget->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(verticalScroll(int)));

	connect(m_pTreeViewSource->header(), SIGNAL(sectionResized(int, int, int)), this, SLOT(sectionResized(int, int, int)));
	connect(m_pTreeViewTarget->header(), SIGNAL(sectionResized(int, int, int)), this, SLOT(sectionResized(int, int, int)));

	onClickedBtnRefresh();
}

QMainWndSyncFile::~QMainWndSyncFile()
{
	
}

void QMainWndSyncFile::closeEvent(QCloseEvent* event)
{
	if (m_nCount != m_nIndex)
	{
		QMessageBox::StandardButton btn=QMessageBox::warning(this, tr("Warning"), tr("Save your changes before closing?"), 
			QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes);
		if (btn == QMessageBox::Yes)
		{
			if (onClickedBtnSave())
			{
				emit closing();

				event->accept();
			}
			else
			{
				event->ignore();
			}
		}
		else if (btn == QMessageBox::No)
		{
			emit closing();

			event->accept();
		}
		else if (btn == QMessageBox::Cancel)
		{
			event->ignore();
		}
	}
	else
	{
		emit closing();

		event->accept();
	}
}

QVector<int> QMainWndSyncFile::lockedIndexes() const
{
	QVector<int> indexes;

	if (m_pTreeViewTarget == NULL || m_pTreeViewTarget->isHeaderHidden())
	{
		return indexes;
	}

	QReadMergeFromXmlFile reader(m_strConfigure, fileSuffix(m_strTarget));
	if (reader.read())
	{
		indexes=reader.result();
	}

	return indexes;
}

void QMainWndSyncFile::lockedChanged()
{
	saveLockedIndexes();
}

void QMainWndSyncFile::saveLockedIndexes() const
{
	if (m_pTreeViewTarget == NULL || m_pTreeViewTarget->isHeaderHidden())
	{
		return;
	}

	QVector<int> indexes=m_pTreeViewTarget->lockedColumns();

	QWriteMergeToXmlFile writer(m_strConfigure, fileSuffix(m_strTarget), indexes);
	writer.write(false);
}

void QMainWndSyncFile::horizontalScroll(int value)
{
	QObject* pSender=sender();
	if (pSender == m_pTreeViewSource->horizontalScrollBar())
	{
		disconnect(m_pTreeViewTarget->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(horizontalScroll(int)));

		m_pTreeViewTarget->horizontalScrollBar()->setValue(value);

		connect(m_pTreeViewTarget->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(horizontalScroll(int)));
	}
	else if (pSender == m_pTreeViewTarget->horizontalScrollBar())
	{
		disconnect(m_pTreeViewSource->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(horizontalScroll(int)));

		m_pTreeViewSource->horizontalScrollBar()->setValue(value);

		connect(m_pTreeViewSource->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(horizontalScroll(int)));
	}
}

void QMainWndSyncFile::verticalScroll(int value)
{
	QObject* pSender=sender();
	if (pSender == m_pTreeViewSource->verticalScrollBar())
	{
		disconnect(m_pTreeViewTarget->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(verticalScroll(int)));

		m_pTreeViewTarget->verticalScrollBar()->setValue(value);

		connect(m_pTreeViewTarget->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(verticalScroll(int)));
	}
	else if (pSender == m_pTreeViewTarget->verticalScrollBar())
	{
		disconnect(m_pTreeViewSource->header(), SIGNAL(sectionResized(int, int, int)), this, SLOT(sectionResized(int, int, int)));

		m_pTreeViewSource->verticalScrollBar()->setValue(value);

		connect(m_pTreeViewSource->header(), SIGNAL(sectionResized(int, int, int)), this, SLOT(sectionResized(int, int, int)));
	}
}

void QMainWndSyncFile::sectionResized(int logicalIndex, int oldSize, int newSize)
{
	Q_UNUSED(oldSize);

	QObject* pSender=sender();
	if (pSender == m_pTreeViewSource->header())
	{
		disconnect(m_pTreeViewTarget->header(), SIGNAL(sectionResized(int, int, int)), this, SLOT(sectionResized(int, int, int)));

		m_pTreeViewTarget->setColumnWidth(m_pTreeViewSource->header()->visualIndex(logicalIndex), newSize);

		connect(m_pTreeViewTarget->header(), SIGNAL(sectionResized(int, int, int)), this, SLOT(sectionResized(int, int, int)));
	}
	else if (pSender == m_pTreeViewTarget->header())
	{
		disconnect(m_pTreeViewSource->header(), SIGNAL(sectionResized(int, int, int)), this, SLOT(sectionResized(int, int, int)));

		m_pTreeViewSource->setColumnWidth(m_pTreeViewTarget->header()->visualIndex(logicalIndex), newSize);

		connect(m_pTreeViewSource->header(), SIGNAL(sectionResized(int, int, int)), this, SLOT(sectionResized(int, int, int)));
	}
}

void QMainWndSyncFile::setMenuState()
{
	if (m_bLoading)
	{
		for (int i=0; i<ActionEnd; i++)
		{
			if (m_pActions[i] != NULL)
			{
				m_pActions[i]->setEnabled(false);
			}
		}
	}
	else
	{
		m_pActions[ActionSave]->setEnabled(m_nCount!=m_nIndex);

		m_pActions[ActionRefresh]->setEnabled(true);

		m_pActions[ActionPreDiff]->setEnabled(m_pTreeViewTarget->previousDifference(false));
		m_pActions[ActionNextDiff]->setEnabled(m_pTreeViewTarget->nextDifference(false));
		m_pActions[ActionPreDiffItem]->setEnabled(m_pTreeViewTarget->previousItemDifference(false));
		m_pActions[ActionNextDiffItem]->setEnabled(m_pTreeViewTarget->nextItemDifference(false));

		m_pActions[ActionUseBlock]->setEnabled(!m_pTreeViewTarget->selectedIndexes().isEmpty());

		m_pActions[ActionUseWholeFile]->setEnabled(true);
	}
}

void QMainWndSyncFile::blockSelection(const QModelIndexList& indexes)
{
	QTreeViewCompareFile* pView=NULL;
	QAbstractItemModel* pModel=NULL;

	QObject* pSender=sender();
	if (pSender == m_pTreeViewSource)
	{
		pView=m_pTreeViewTarget;
		pModel=m_pTreeViewTarget->model();
	}
	else if (pSender == m_pTreeViewTarget)
	{
		pView=m_pTreeViewSource;
		pModel=m_pTreeViewSource->model();
	}

	setMenuText(pSender);

	QModelIndexList tindexes;

	QListIterator<QModelIndex> it(indexes);
	while (it.hasNext())
	{
		const QModelIndex& index=it.next();

		QModelIndexList path=QTreeViewCompareFile::treePath(index);
		QModelIndex tindex;

		QListIterator<QModelIndex> iter(path);
		while (iter.hasNext())
		{
			const QModelIndex& cindex=iter.next();

			tindex=pModel->index(cindex.row(), cindex.column(), tindex);
		}

		tindexes.append(tindex);
	}

	pView->selectBlock(tindexes);

	setMenuState();
}

void QMainWndSyncFile::itemSelection(const QModelIndex& index)
{
	QTreeViewCompareFile* pView=NULL;
	QAbstractItemModel* pModel=NULL;

	QModelIndexList path=QTreeViewCompareFile::treePath(index);

	QObject* pSender=sender();
	if (pSender == m_pTreeViewSource)
	{
		pView=m_pTreeViewTarget;
		pModel=m_pTreeViewTarget->model();
	}
	else if (pSender == m_pTreeViewTarget)
	{
		pView=m_pTreeViewSource;
		pModel=m_pTreeViewSource->model();
	}

	setMenuText(pSender);

	QModelIndex tindex;

	QListIterator<QModelIndex> it(path);
	while (it.hasNext())
	{
		const QModelIndex& cindex=it.next();

		tindex=pModel->index(cindex.row(), cindex.column(), tindex);
	}

	pView->selectItem(tindex);

	setMenuState();
}

void QMainWndSyncFile::setMenuText(QObject* sender)
{
	if (sender == m_pTreeViewSource)
	{
		m_pActions[ActionUseBlock]->setText(tr("Use this block"));
		m_pActions[ActionUseWholeFile]->setText(tr("Use this whole file"));
	}
	else if (sender == m_pTreeViewTarget)
	{
		m_pActions[ActionUseBlock]->setText(tr("Use other block"));
		m_pActions[ActionUseWholeFile]->setText(tr("Use whole other file"));
	}
}

void QMainWndSyncFile::mouseRightButtonPress(QPoint pos)
{
	QTreeViewCompareFile* pView=NULL;

	QObject* pSender=sender();
	if (pSender == m_pTreeViewSource)
	{
		pView=m_pTreeViewSource;
	}
	else if (pSender == m_pTreeViewTarget)
	{
		pView=m_pTreeViewTarget;
	}

	setMenuText(pSender);

	m_pMenu->popup(pView->mapToGlobal(pos));
}

void QMainWndSyncFile::actionTriggered(int index)
{
	switch (index)
	{
	case ActionSave:
		onClickedBtnSave();
		break;
	case ActionRefresh:
		onClickedBtnRefresh();
		break;
	case ActionUndo:
		m_nCount=m_nCount>0? --m_nCount:0;
		break;
	case ActionPreDiff:
		onClickedBtnPreviousDifference();
		break;
	case ActionNextDiff:
		onClickedBtnNextDifference();
		break;
	case ActionPreDiffItem:
		onClickedBtnPreviousItemDifference();
		break;
	case ActionNextDiffItem:
		onClickedBtnNextItemDifference();
		break;
	case ActionUseBlock:
		onClickedBtnUseBlock();
		break;
	case ActionUseWholeFile:
		onClickedBtnUseWholeFile();
		break;
	default:
		break;
	}

	setMenuState();
}

bool QMainWndSyncFile::onClickedBtnSave()
{
	QAbstractFileCompare::ErrorCode code=QFileCompareFactory::instance()->write(m_result.source, m_result.target, m_error);
	switch (code)
	{
	case QAbstractFileCompare::NoError:
		{
			logplusInfo(MANAGER_LOGGER_STR, QString("Operation: Save File %1").arg(m_result.target.filename));

			m_nIndex=m_nCount;

			emit modified();
		}
		return true;
	case QAbstractFileCompare::FileOpenError:
		{
			QMessageBox::warning(this, tr("Warning"), QString(tr("Failed to write data to the file %1!")).arg(m_strTarget), QMessageBox::Ok, QMessageBox::Ok);

			logplusError(MANAGER_LOGGER_STR, QString("Failed to write data to the file %1!").arg(m_result.target.filename));
		}
	break;
	case QAbstractFileCompare::KeyPropertyIsEmpryError:
		{
			m_pTreeViewTarget->scrollTo(m_error.first);
			QMessageBox::critical(this, tr("Error"), QString("The property within the red border can not be empty!"), QMessageBox::Ok, QMessageBox::Ok);
		}
		break;
	case QAbstractFileCompare::KeyPropertyRepeatError:
		{
			m_pTreeViewTarget->scrollTo(m_error.first);
			QMessageBox::critical(this, tr("Error"), QString("The properties within the red border can not be repeated!"), QMessageBox::Ok, QMessageBox::Ok);
		}
		break;
	default:
		break;
	}

	return false;
}

void QMainWndSyncFile::onClickedBtnRefresh()
{
	if (m_pUndoStack != NULL && m_nCount > 0)
	{
		QMessageBox::StandardButton btn=QMessageBox::warning(this, tr("Warning"), tr("All operations will be lost! Are you sure?"), 
			QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Cancel);
		if (btn == QMessageBox::Cancel)
		{
			return;
		}

		m_pUndoStack->clear();
		m_nIndex=m_nCount=0;

		logplusInfo(MANAGER_LOGGER_STR, QString("All operations have been lost.Reload file %1").arg(m_strTarget));
	}

	setWindowTitle(QString("%1 - %2%3 - [%4]").arg(tr("Merge")).arg(tr("Manager")).arg(MANAGER_VERSION_STR).arg(tr("Loading...")));

	QAbstractItemModel* pModelSource=m_pTreeViewSource->model();
	QAbstractItemModel* pModelTarget=m_pTreeViewTarget->model();

	if (pModelSource != NULL)
	{
		pModelSource->removeRows(0, pModelSource->rowCount());
	}
	
	if (pModelTarget != NULL)
	{
		pModelTarget->removeRows(0, pModelTarget->rowCount());
	}

	m_bLoading=true;

	QMainWndSyncFileRunnable* pRunnable=new QMainWndSyncFileRunnable(m_strSource, m_strTarget);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QFileCompareFactory::FileCompareResult)), this, SLOT(parseFinished(QFileCompareFactory::FileCompareResult)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);	

	setMenuState();
}

void QMainWndSyncFile::onClickedBtnUseBlock()
{
	QCompareFileUndoCommand* pUndoCommand=m_pTreeViewTarget->copy(m_pTreeViewSource);
	if (pUndoCommand != NULL)
	{
		m_pUndoStack->push(pUndoCommand);

		m_nCount++;
	}
}

void QMainWndSyncFile::onClickedBtnUseWholeFile()
{
	QCompareFileUndoCommand* pUndoCommand=m_pTreeViewTarget->copyWhole(m_pTreeViewSource);
	if (pUndoCommand != NULL)
	{
		m_pUndoStack->push(pUndoCommand);

		m_nCount++;
	}
}

void QMainWndSyncFile::onClickedBtnPreviousDifference()
{
	m_pTreeViewTarget->previousDifference();
}

void QMainWndSyncFile::onClickedBtnNextDifference()
{
	m_pTreeViewTarget->nextDifference();
}

void QMainWndSyncFile::onClickedBtnPreviousItemDifference()
{
	m_pTreeViewTarget->previousItemDifference();
}

void QMainWndSyncFile::onClickedBtnNextItemDifference()
{
	m_pTreeViewTarget->nextItemDifference();
}

void QMainWndSyncFile::parseFinished(QFileCompareFactory::FileCompareResult result)
{
	m_result=result;

	m_bLoading=false;

	QAbstractItemModel* pModel=m_pTreeViewSource->model();
	if (pModel != NULL)
	{
		pModel->deleteLater();
	}

	pModel=m_pTreeViewTarget->model();
	if (pModel != NULL)
	{
		pModel->deleteLater();
	}

	QItemSelectionModel* pSelectionModel=m_pTreeViewSource->selectionModel();
	if (pSelectionModel != NULL)
	{
		pSelectionModel->deleteLater();
	}

	pSelectionModel=m_pTreeViewTarget->selectionModel();
	if (pSelectionModel != NULL)
	{
		pSelectionModel->deleteLater();
	}
	
	m_pTreeViewSource->setModel(result.source.model);
	m_pTreeViewTarget->setModel(result.target.model);

	bool bIsHeaderHidden=QFileCompareFactory::instance()->isHeaderHidden(result);
	m_pTreeViewSource->setHeaderHidden(bIsHeaderHidden);
	m_pTreeViewTarget->setHeaderHidden(bIsHeaderHidden);
	
	if (result.target.model != NULL)
	{
		int nColumnCount=result.target.model->columnCount();
		for (int i=0; i<nColumnCount; i++)
		{
			m_pTreeViewTarget->resizeColumnToContents(i);
		}
	}

	m_pTreeViewSource->expandAll();
	m_pTreeViewTarget->expandAll();

	QVector<int> indexes=lockedIndexes();
	for (int i=0; i<indexes.size(); i++)
	{
		m_pTreeViewTarget->sectionClicked(indexes[i]);
	}

	setWindowTitle(QString("%1 - %2%3 - [%4]").arg(tr("Merge")).arg(tr("Manager")).arg(MANAGER_VERSION_STR).arg(m_strTarget));

	setMenuState();
}

QMainWndSyncFileRunnable::QMainWndSyncFileRunnable(const QString& source, const QString& target,QObject* parent/* =0 */)
:QObject(parent)
,m_strSourceFile(source)
,m_strTargetFile(target)
{

}

QMainWndSyncFileRunnable::~QMainWndSyncFileRunnable()
{

}

void QMainWndSyncFileRunnable::run()
{
	QFileCompareFactory::FileCompareResult result=QFileCompareFactory::instance()->compare(m_strSourceFile, m_strTargetFile);

	emit finished(result);
}
