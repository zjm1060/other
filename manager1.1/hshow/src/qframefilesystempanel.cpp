#include "qframefilesystempanel.h"

#include <QVector>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

#include "ui_filesystem.h"

#include "common.h"
#include "qdiskdelegate.h"

QFrameFileSystemPanel::QFrameFileSystemPanel(QWidget *parent)
	: QFrame(parent)
{
	m_ui=new Ui_FrameFileSystem;
	m_ui->setupUi(this);

	QStringList listLabels;
	listLabels<<tr("Name")<<tr("File System");
	m_pModelComputer=new QStandardItemModel(0, listLabels.size(), this);
	m_pModelComputer->setHorizontalHeaderLabels(listLabels);

	m_pSortModel=new QSortFilterProxyModel;
	m_pSortModel->setDynamicSortFilter(true);
	m_pSortModel->setSortCaseSensitivity(Qt::CaseInsensitive);
	m_pSortModel->setSourceModel(m_pModelComputer);

	m_pDelegate=new QDiskDelegate(this);
	
	m_ui->treeViewFileSystem->setRootIsDecorated(false);
	m_ui->treeViewFileSystem->setAlternatingRowColors(true);
	m_ui->treeViewFileSystem->setModel(m_pSortModel);
	m_ui->treeViewFileSystem->setSortingEnabled(true);
	m_ui->treeViewFileSystem->setItemDelegate(m_pDelegate);
	m_ui->treeViewFileSystem->setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_ui->treeViewFileSystem->setItemsExpandable(true);
	m_ui->treeViewFileSystem->header()->setStretchLastSection(true);

	m_ui->treeViewFileSystem->setColumnWidth(NameTitle, 300);
	m_ui->treeViewFileSystem->setColumnWidth(FileSystemTitle, 230);

	m_ui->treeViewFileSystem->sortByColumn(FileSystemTitle, Qt::AscendingOrder);
}

QFrameFileSystemPanel::~QFrameFileSystemPanel()
{

}

void QFrameFileSystemPanel::setLogicalDisk(const QVector<LogicalDisk>& disk)
{
	m_pModelComputer->setRowCount(0);
	m_vecLogicalDisk=disk;

	showLogicalDisk();
}

void QFrameFileSystemPanel::showLogicalDisk()
{
	for (int i=0; i<m_vecLogicalDisk.size(); i++)
	{
		const LogicalDisk& disk=m_vecLogicalDisk[i];

		m_pModelComputer->insertRow(i);

		m_pModelComputer->itemFromIndex(m_pModelComputer->index(i, NameTitle))->setEditable(false);

		if (!disk.deviceID.isEmpty())
		{
			m_pModelComputer->setData(m_pModelComputer->index(i, NameTitle), disk.description+"("+disk.deviceID+")");
		}
		else
		{
			m_pModelComputer->setData(m_pModelComputer->index(i, NameTitle), disk.description);
		}

		QVariant var;
		var.setValue(disk);

		m_pModelComputer->setData(m_pModelComputer->index(i, NameTitle), var, Qt::UserRole);

		m_pModelComputer->itemFromIndex(m_pModelComputer->index(i, FileSystemTitle))->setEditable(false);
		m_pModelComputer->setData(m_pModelComputer->index(i, FileSystemTitle), disk.size);
	}
}
