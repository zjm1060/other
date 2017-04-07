#include "qframelocalfilesystem.h"

#include <QHBoxLayout>
#include <QFileDialog>
#include <QMouseEvent>
#include <QMessageBox>
#include <QLineEdit>

#include "define.h"
#include "common.h"
#include "qimessage.h"
#include "qglobalinfo.h"
#include "qprocesssocket.h"

QFrameLocalFileSystem::QFrameLocalFileSystem(QWidget *parent)
	: QFrameFileSystem(parent)
	,m_filters(QDir::NoFilter)
{
	connect(this, SIGNAL(clickedToolButton(int)), this, SLOT(clickedToolButton(int)));
	connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(locationCurrentIndexChanged(QString)));

	connect(comboBoxFileSystem(), SIGNAL(editTextChanged(const QString&)), this, SLOT(editTextChangedLocation(const QString&)));
	connect(comboBoxFileSystem(), SIGNAL(enterPressed(QString)), this, SLOT(enterPressed(QString)));
	connect(comboBoxFileSystem(), SIGNAL(focusStatusChanged(bool)), this, SIGNAL(focusStatusChanged(bool)));

	connect(treeViewFileSystem(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex&)));
	connect(treeViewFileSystem(), SIGNAL(backspaceKeyPress()), this, SLOT(backspaceKeyPressed()));
	connect(treeViewFileSystem(), SIGNAL(refreshKeyPress()), this, SLOT(refreshKeyPressed()));
	connect(treeViewFileSystem(), SIGNAL(deleteKeyPress()), this, SLOT(deleteKeyPressed()));
	connect(treeViewFileSystem(), SIGNAL(mouseRightButtonPress(QPoint)), this, SLOT(mouseRightButtonPressed(QPoint)));
	connect(treeViewFileSystem(), SIGNAL(dataChanged(QModelIndex, QString, QString)), this, SLOT(dataChanged(QModelIndex, QString, QString)));

	refreshDrives();
}

QFrameLocalFileSystem::~QFrameLocalFileSystem()
{
}

void QFrameLocalFileSystem::refreshDrives()
{
	QFileInfoList drives=QDir::drives();

	setDrives(drives);
}

void QFrameLocalFileSystem::setDefaultLocation(const QString& location)
{
	if (location.isEmpty())
	{
		return;
	}

	m_strHome=location;

	if (comboBoxFileSystem()->topItemType(m_strHome) != QComboBoxFileSystem::OtherItem)
	{
		gotoLocation(m_strHome);
	}
	else
	{
		QDir dir(m_strHome);
		if (dir.exists())
		{
			gotoLocation(m_strHome);
		}
		else
		{
			gotoLocation(getAppParentDir());
		}
	}
}

void QFrameLocalFileSystem::setFilters(const QStringList& nameFilters, QDir::Filters filters)
{
	m_nameFilters=nameFilters;
	m_filters=filters;
}

void QFrameLocalFileSystem::refreshFileSystem()
{
	QString strLocation=location();

	QFileInfoList files;

	QComboBoxFileSystem* pComboBoxFileSystem=comboBoxFileSystem();
	int nItemType;
	if ((nItemType=pComboBoxFileSystem->topItemType(strLocation)) != QComboBoxFileSystem::OtherItem)
	{
		switch (nItemType)
		{
		case QComboBoxFileSystem::Computer:
			{
				files=QDir::drives();
			}
			break;
// 		case QComboBoxFileSystem::Trash:
// 			break;
		default:
			break;
		}
	}
	else
	{
		QDir dir(strLocation);
		if (!dir.exists())
		{
			strLocation=getAppParentDir();
			setLocation(strLocation);

			dir.setPath(strLocation);
		}

		if (m_nameFilters.isEmpty())
		{
			files=dir.entryInfoList(QDir::AllDirs|QDir::Files|m_filters);
		}
		else
		{
			files=dir.entryInfoList(m_nameFilters, QDir::AllDirs|QDir::Files|m_filters);
		}
	}

	setFileInfo(files, nItemType);
}

QVector<TransferFileItem> QFrameLocalFileSystem::selectedFileItems()
{
	QVector<TransferFileItem> items;

	QTreeViewFileSystem* pFileSystem=treeViewFileSystem();
	if (pFileSystem == NULL)
	{
		return items;
	}

	QModelIndexList rows=pFileSystem->selectedRows();;
	if (rows.size() <= 0)
	{
		return items;
	}

	QString strName;
	TransferFileItem item;
	foreach (QModelIndex index, rows)
	{
		strName=pFileSystem->filename(index);
		if (strName == "..")
		{
			continue;
		}

		item.size=pFileSystem->size(index);
		item.type=pFileSystem->type(index);
		item.typeDesc=pFileSystem->typeDesc(index);
		item.name=strName;
		
		items.append(item);
	}

	return items;
}

QVector<FileInfo> QFrameLocalFileSystem::selectedFileInfos()
{
	QVector<FileInfo> items;

	QTreeViewFileSystem* pFileSystem=treeViewFileSystem();
	if (pFileSystem == NULL)
	{
		return items;
	}

	QModelIndexList rows=pFileSystem->selectedRows();;
	if (rows.size() <= 0)
	{
		return items;
	}

	QString strName;
	foreach (QModelIndex index, rows)
	{
		strName=pFileSystem->filename(index);
		if (strName == "..")
		{
			continue;
		}

		FileInfo file=pFileSystem->fileInfo(index);

		items.append(file);
	}

	return items;
}

void QFrameLocalFileSystem::gotoLocation(const QString& location)
{
	setLocation(location);

	refreshFileSystem();
}

QString QFrameLocalFileSystem::locationText()
{
	return comboBoxFileSystem()->lineEdit()->text();
}

QString QFrameLocalFileSystem::currentLocationText()
{
	return comboBoxFileSystem()->currentLocation();
}

bool QFrameLocalFileSystem::canUpload()
{
	QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QModelIndexList rows=pTreeViewFileSystem->selectedRows();

		QString strLocation=comboBoxFileSystem()->currentLocation();

		if (!rows.isEmpty())
		{
			if (comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
			{
				if (rows.size() != 1)
				{
					return true;
				}
				else
				{
					QString strName=pTreeViewFileSystem->filename(rows[0]);
					if (strName != "..")
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool QFrameLocalFileSystem::canDelete()
{
	QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QModelIndexList rows=pTreeViewFileSystem->selectedRows();

		QString strLocation=comboBoxFileSystem()->currentLocation();

		if (!rows.isEmpty())
		{
			if (comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
			{
				if (rows.size() != 1)
				{
					return true;
				}
				else
				{
					QString strName=pTreeViewFileSystem->filename(rows[0]);
					if (strName != "..")
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool QFrameLocalFileSystem::canOpen()
{
	QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QModelIndexList rows=pTreeViewFileSystem->selectedRows();

		if (rows.size() == 1)
		{
			int nType=pTreeViewFileSystem->type(rows[0]);
			if ((nType == Drive) || (nType & Dir) != 0 || (nType & HideDir) != 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool QFrameLocalFileSystem::canRename()
{
	QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();
	if (pTreeViewFileSystem != NULL)
	{
		QModelIndexList rows=pTreeViewFileSystem->selectedRows();
		QString strLocation=comboBoxFileSystem()->currentLocation();

		if (rows.size() == 1)
		{
			if (comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
			{
				QString strName=pTreeViewFileSystem->filename(rows[0]);
				if (strName != "..")
				{
					return true;
				}
			}
		}
	}

	return false;
}

void QFrameLocalFileSystem::clickedToolButton(int type)
{
	switch (type)
	{
	case QFrameFileSystem::ButtonHome:
		{
			refreshDrives();

			setDefaultLocation(m_strHome);
		}
		break;
	case QFrameFileSystem::ButtonUp:
		{
			QString strLocation=location();
			if (comboBoxFileSystem()->topItemType(strLocation) != QComboBoxFileSystem::OtherItem)
			{
				break;
			}

			enterPressed(comboBoxFileSystem()->parentLocation(strLocation));
		}
		break;
	default:
		break;
	}
}

void QFrameLocalFileSystem::locationCurrentIndexChanged(QString location)
{
	gotoLocation(location);
}

void QFrameLocalFileSystem::enterPressed(QString location)
{
	gotoLocation(location);
}

void QFrameLocalFileSystem::backspaceKeyPressed()
{
	clickedToolButton(QFrameFileSystem::ButtonUp);
}

void QFrameLocalFileSystem::refreshKeyPressed()
{
	refresh();
}

void QFrameLocalFileSystem::deleteKeyPressed()
{
	QString strLocation=comboBoxFileSystem()->currentLocation();
	if (comboBoxFileSystem()->topItemType(strLocation) != QComboBoxFileSystem::OtherItem)
	{
		return;
	}
	else
	{
		QMessageBox::StandardButton button=QMessageBox::warning(this, tr("Warning"),
			tr("Will permanently delete these files! Are you sure to delete?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::No);

		if (button == QMessageBox::Yes)
		{
			QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();
			QModelIndexList list=pTreeViewFileSystem->selectedRows();

			QString strName;
			bool bSuccess=true;
			foreach (QModelIndex index, list)
			{
				strName=appendPath(strLocation, pTreeViewFileSystem->filename(index));
				if (strName == ".." || strName==".")
				{
					continue;
				}

				QFileInfo info(strName);
				if (info.isSymLink() || info.isFile())
				{
					bSuccess&=QFile::remove(strName);
				}
				else if (info.isDir())
				{
					bSuccess&=deleteDir(strName);
				}
			}

			if (!bSuccess)
			{
				QMessageBox::critical(this, tr("Error"), 
					tr("Local: Delete file(s) or file folder(s) failed! May be in use, or insufficient privileges!"), QMessageBox::Yes, QMessageBox::Yes);
			}

			refresh();
		}
	}
}

void QFrameLocalFileSystem::editTextChangedLocation(const QString& location)
{
	QString strLocation=QDir::fromNativeSeparators(location);

	QChar ch;
	if (!strLocation.isEmpty() && comboBoxFileSystem()->topItemType(strLocation) == QComboBoxFileSystem::OtherItem)
	{
		if (hasInvalidChar(strLocation, &ch))
		{
			emit enterInvalidChar(true, ch);
			return;
		}
	}

	emit enterInvalidChar(false, ch);
}

void QFrameLocalFileSystem::doubleClicked()
{
	QModelIndexList rows=treeViewFileSystem()->selectedRows();;

	if (rows.size() != 1)
	{
		return;
	}

	doubleClicked(rows[0]);
}

void QFrameLocalFileSystem::doubleClicked(const QModelIndex& index)
{
	QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();
	QString strFileName=pTreeViewFileSystem->filename(index);
	QString strLocation;
	if (strFileName == "..")
	{
		strLocation=getParentDir(location());
	}
	else
	{
		int nType=pTreeViewFileSystem->type(index);

		if (nType == File || nType == HideFile || nType == SymLinkFile || nType == SymLinkHideFile)
		{
			if (!QGlobalInfo::instance()->hasPrivileges(ProcessPrivilege, CreateProcessPrivilege))
			{
				QMessageBox::warning(this, tr("Warning"), tr("You do not have sufficient permissions to open the file!"),
					QMessageBox::Ok, QMessageBox::Ok);
				return;
			}

			QStringList arguments;
			arguments<<appendPath(location(), strFileName);

			QProcessSocket::instance()->queryExecCommand(arguments);

			return;
		}

		if ((nType & Dir) != 0 || (nType & HideDir) != 0)
		{
			strLocation=appendPath(location(), strFileName);
		}
		else if ((nType & Drive) != 0)
		{
			strLocation=strFileName;
		}

		if (nType == SymLinkDir || nType == SymLinkHideDir)
		{
			strLocation=QFile::symLinkTarget(strLocation);
		}
	}

	if (!strLocation.isEmpty())
	{
		gotoLocation(strLocation);
	}
}

void QFrameLocalFileSystem::dataChanged(QModelIndex index, QString old, QString now)
{
	if (!QGlobalInfo::instance()->hasPrivileges(FileTransferPrivilege, DownloadFileAndFolderPrivilege))
	{
		treeViewFileSystem()->setFileName(index, old);

		QMessageBox::warning(this, tr("Warning"), tr("You do not have sufficient permissions to rename files and folders!"),
			QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	QString strLocation=comboBoxFileSystem()->currentLocation();
	if (comboBoxFileSystem()->topItemType(strLocation) != QComboBoxFileSystem::OtherItem)
	{
		treeViewFileSystem()->setFileName(index, old);
	}
	else
	{
		QDir dir(appendPath(strLocation, old));
		if (dir.rename(appendPath(strLocation, old), appendPath(strLocation, now)))
		{
			treeViewFileSystem()->setFileName(index, now);

			refresh();
		}
		else
		{
			treeViewFileSystem()->setFileName(index, old);
		}
	}
}

void QFrameLocalFileSystem::refresh()
{
	locationCurrentIndexChanged(comboBoxFileSystem()->currentLocation());
}

void QFrameLocalFileSystem::rename()
{
	QModelIndexList list;
	QTreeViewFileSystem* pTreeViewFileSystem=treeViewFileSystem();

	if (pTreeViewFileSystem == NULL)
	{
		return;
	}

	list=pTreeViewFileSystem->selectedRows();
	if (list.size() == 1)
	{
		pTreeViewFileSystem->edit(list[0]);
	}
}

void QFrameLocalFileSystem::mouseRightButtonPressed(QPoint pos)
{
	emit mouseRightButtonPress(treeViewFileSystem()->mapToGlobal(pos));
}
