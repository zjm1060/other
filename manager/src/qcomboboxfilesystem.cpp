#include "qcomboboxfilesystem.h"

#include <QApplication>
#include <QStyle>
#include <QDir>
#include <QStylePainter>
#include <QLineEdit>
#include <QKeyEvent>
#include <QItemSelectionModel>

#include "common.h"

QComboBoxFileSystem::QComboBoxFileSystem(QWidget *parent)
	: QComboBox(parent)
{
	setEditable(true);
	setFocusPolicy(Qt::WheelFocus);

	m_pTreeModel=new QTreeModel(1, this);
	setModel(m_pTreeModel);

	m_pTreeView=new QTreeView(this);
	m_pTreeView->setHeaderHidden(true);
	m_pTreeView->setRootIsDecorated(false);
	connect(m_pTreeView, SIGNAL(pressed(QModelIndex)), this, SIGNAL(itemPressed(QModelIndex)));

	setView(m_pTreeView);

	QStyle* pStyle=QApplication::style();

	for (int i=0; i<ItemEnd; i++)
	{
		m_pTreeModel->insertRow(i);
	}
	
	QModelIndex index=itemIndex(Computer);
	m_pTreeModel->setData(index, COMPUTER_ITEM, Qt::EditRole);
	m_pTreeModel->setData(index, COMPUTER_ITEM, Qt::DisplayRole);
	m_pTreeModel->setData(index, pStyle->standardIcon(QStyle::SP_ComputerIcon), Qt::DecorationRole);

// 	index=itemIndex(Trash);
// 	m_pTreeModel->setData(index, TRASH_ITEM, Qt::EditRole);
// 	m_pTreeModel->setData(index, TRASH_ITEM, Qt::DisplayRole);
// 	m_pTreeModel->setData(index, pStyle->standardIcon(QStyle::SP_TrashIcon), Qt::DecorationRole);
}

QComboBoxFileSystem::~QComboBoxFileSystem()
{
}

QModelIndex QComboBoxFileSystem::itemIndex(ItemType type)
{
	return m_pTreeModel->index(type, 0);
}

QString QComboBoxFileSystem::itemText(ItemType type)
{
	QModelIndex index=itemIndex(type);

	return m_pTreeModel->data(index, Qt::EditRole).toString();
}

int QComboBoxFileSystem::topItemType(const QString& location)
{
	int nRows=m_pTreeModel->rowCount();
	for (int i=0; i<nRows; i++)
	{
		if (QString::compare(location, m_pTreeModel->data(m_pTreeModel->index(i, 0), Qt::EditRole).toString(),
			Qt::CaseInsensitive) == 0)
		{
			return i;
		}
	}

	return OtherItem;
}

void QComboBoxFileSystem::clear()
{
	m_currentModelIndex=itemIndex(Computer);

	if (m_pTreeModel != NULL)
	{
		m_pTreeModel->setRowCount(0, itemIndex(Computer));
	}
}

QTreeModel* QComboBoxFileSystem::treeModel()
{
	return m_pTreeModel;
}

void QComboBoxFileSystem::setCurrentLocation(const QString& location, bool test)
{
	if (test)
	{
		if (topItemType(location) != -1)
		{
			m_strCurrentLocation=location;
		}
		else
		{
			QDir dir(location);
			if (dir.exists())
			{
				m_strCurrentLocation=location;
			}
		}
	}
	else
	{
		m_strCurrentLocation=location;
	}

	adjustPopup();

	setEditText(m_strCurrentLocation);
}

QString QComboBoxFileSystem::currentLocation() const
{
	return m_strCurrentLocation;
}

QString QComboBoxFileSystem::parentLocation(const QString& location)
{
	if (topItemType(location) != -1)
	{
		return location;
	}

	int rows=m_pTreeModel->rowCount();
	QModelIndex index;
	QModelIndex child;
	int nNum;
	for (int i=0; i<rows; i++)
	{
		index=itemIndex((ItemType)i);
		nNum=m_pTreeModel->rowCount(index);
		for (int j=0; j<nNum; j++)
		{
			child=m_pTreeModel->index(j, 0, index);
			if (QString::compare(location, m_pTreeModel->data(child, Qt::EditRole).toString(), Qt::CaseInsensitive) == 0)
			{
				return m_pTreeModel->data(index, Qt::EditRole).toString();
			}
		}
	}

	return getParentDir(location);
}

QString QComboBoxFileSystem::selectedLocation() const
{
	QString strLocation=m_pTreeModel->data(m_pTreeView->currentIndex(), Qt::EditRole).toString();
	return strLocation;
}

void QComboBoxFileSystem::setDrives(const QFileInfoList& drives)
{
	if (m_pTreeModel == NULL)
	{
		return;
	}

	QModelIndex computerIndex=itemIndex(Computer);
	m_pTreeModel->setRowCount(0, computerIndex);

	int i=0;
	foreach (QFileInfo info, drives)
	{
		m_pTreeModel->insertRow(i, computerIndex);

		QModelIndex modelIndex=m_pTreeModel->index(i, 0, computerIndex);
		m_pTreeModel->setData(modelIndex, info.filePath(), Qt::EditRole);
		m_pTreeModel->setData(modelIndex, info.filePath(), Qt::DisplayRole);
		m_pTreeModel->setData(modelIndex, m_fileIconProvider.icon(info), Qt::DecorationRole);

		i++;
	}

	m_pTreeView->setExpanded(computerIndex, true);
}

void QComboBoxFileSystem::setDrives(const QVector<FileInfo>& drives)
{
	if (m_pTreeModel == NULL)
	{
		return;
	}

	QModelIndex computerIndex=itemIndex(Computer);
	m_pTreeModel->setRowCount(0, computerIndex);

	int i=0;
	foreach (FileInfo info, drives)
	{
		m_pTreeModel->insertRow(i, computerIndex);

		QModelIndex modelIndex=m_pTreeModel->index(i, 0, computerIndex);
		m_pTreeModel->setData(modelIndex, info.name, Qt::EditRole);
		m_pTreeModel->setData(modelIndex, info.name, Qt::DisplayRole);
		m_pTreeModel->setData(modelIndex, fileIcon(info), Qt::DecorationRole);

		i++;
	}

	m_pTreeView->setExpanded(computerIndex, true);
}

void QComboBoxFileSystem::adjustPopup()
{
	if (m_pTreeModel == NULL || m_pTreeView == NULL)
	{
		return;
	}

	QModelIndex computerIndex=itemIndex(Computer);
	int nRows=m_pTreeModel->rowCount(computerIndex);
	if (nRows == 0)
	{
		return;
	}

	for (int i=0; i<nRows; i++)
	{
		m_pTreeModel->setRowCount(0, m_pTreeModel->index(i, 0, computerIndex));
	}

	QString strLocation=QDir::fromNativeSeparators(m_strCurrentLocation);

	if (QString::compare(strLocation , COMPUTER_ITEM, Qt::CaseInsensitive) == 0)
	{
		m_currentModelIndex=itemIndex(Computer);
	}
// 	else if (QString::compare(strLocation , TRASH_ITEM, Qt::CaseInsensitive) == 0)
// 	{
// 		m_currentModelIndex=itemIndex(Trash);
// 	}
	else
	{
		QStringList listDirs=strLocation.split('/');
		if (listDirs.size() > 0)
		{
			QStringListIterator it(listDirs);

			QString strDrive=it.next();
			if (strDrive.right(1) == ":")
			{
				strDrive+="/";
			}
			else if (strDrive.isEmpty())
			{
				strDrive="/";
			}

			QModelIndex modelIndex;
			int nRows=m_pTreeModel->rowCount(computerIndex);
			int index;
			for (index=0; index<nRows; index++)
			{
				modelIndex=m_pTreeModel->index(index, 0, computerIndex);
				if (QString::compare(m_pTreeModel->data(modelIndex, Qt::EditRole).toString(), strDrive, Qt::CaseInsensitive) == 0)
				{
					strDrive=m_pTreeModel->data(modelIndex, Qt::EditRole).toString();
					break;
				}
			}

			if (index >= nRows)
			{
				m_currentModelIndex=computerIndex;
			}
			else
			{
				QString strDir;
				while (it.hasNext())
				{
					strDir=it.next();
					if (strDir.isEmpty())
					{
						continue;
					}

					strDrive+=strDir+"/";

					if (m_pTreeModel->rowCount(modelIndex) == 0)
					{
						m_pTreeModel->insertRow(0, modelIndex);
					}
					modelIndex=m_pTreeModel->index(0, 0, modelIndex);

					m_pTreeModel->setData(modelIndex, strDrive, Qt::EditRole);
					m_pTreeModel->setData(modelIndex, strDir, Qt::DisplayRole);
					m_pTreeModel->setData(modelIndex, m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);

					m_pTreeView->setExpanded(modelIndex.parent(), true);
				}

				m_pTreeModel->setRowCount(0, modelIndex);

				m_currentModelIndex=modelIndex;
			}
		}
	}

	setRootModelIndex(m_currentModelIndex.parent());

	hidePopup();

	update();
}

void QComboBoxFileSystem::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QVariant decoration = m_pTreeModel->data(m_currentModelIndex, Qt::DecorationRole);
	QIcon icon=qvariant_cast<QIcon>(decoration);

	QStylePainter painter(this);
	painter.setPen(palette().color(QPalette::Text));

	// draw the combobox frame, focusrect and selected etc.
	QStyleOptionComboBox opt;
	initStyleOption(&opt);
	if (!icon.isNull())
	{
		opt.currentIcon=icon;
	}
	painter.drawComplexControl(QStyle::CC_ComboBox, opt);

	// draw the icon and text
	painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}

void QComboBoxFileSystem::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
	{
		QString strLocation=currentText();

		if (!strLocation.isEmpty())
		{
			if (!hasInvalidChar(strLocation, NULL))
			{
				QDir dir(strLocation);
				if (dir.exists())
				{
					emit enterPressed(strLocation);
				}
			}
		}
		else
		{
			setEditText(m_strCurrentLocation);
		}

		return;
	}

	QComboBox::keyPressEvent(event);
}

void QComboBoxFileSystem::showPopup()
{
	setRootModelIndex(QModelIndex());

	QComboBox::showPopup();
}

void QComboBoxFileSystem::focusInEvent(QFocusEvent* event)
{
	QComboBox::focusInEvent(event);

	emit focusStatusChanged(true);
}

void QComboBoxFileSystem::focusOutEvent(QFocusEvent* event)
{
	QComboBox::focusOutEvent(event);

	emit focusStatusChanged(false);
}
