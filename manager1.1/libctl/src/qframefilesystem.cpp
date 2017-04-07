#include "qframefilesystem.h"

#include "common.h"

QFrameFileSystem::QFrameFileSystem(QWidget *parent)
	: QFrame(parent)
{
	init();
}

QFrameFileSystem::~QFrameFileSystem()
{

}

void QFrameFileSystem::init()
{
	setFrameStyle(QFrame::StyledPanel | QFrame::Plain);

	m_pVerticalLayout=new QVBoxLayout(this);
	m_pVerticalLayout->setContentsMargins(2, 2, 2, 2);

	m_pHorizontalLayout=new QHBoxLayout();
	m_pHorizontalLayout->setContentsMargins(0, 0, 0, 0);
	m_pHorizontalLayout->setSpacing(2);

	m_pSignalMapper=new QSignalMapper(this);

	m_pToolButton[ButtonHome]=new QToolButton(this);
	m_pToolButton[ButtonHome]->setIcon(QIcon(":/images/home.png"));
	m_pHorizontalLayout->addWidget(m_pToolButton[ButtonHome]);

	m_pToolButton[ButtonUp]=new QToolButton(this);
	m_pToolButton[ButtonUp]->setIcon(QIcon(":/images/stepup.png"));
	m_pHorizontalLayout->addWidget(m_pToolButton[ButtonUp]);

	for (int i=0; i<ButtonEnd; i++)
	{
		m_pToolButton[i]->setMinimumSize(QSize(23, 23));
		m_pToolButton[i]->setMaximumSize(QSize(23, 23));
		m_pToolButton[i]->setAutoRaise(true);

		connect(m_pToolButton[i], SIGNAL(clicked()), m_pSignalMapper, SLOT(map()));
		m_pSignalMapper->setMapping(m_pToolButton[i], i);
	}

	connect(m_pSignalMapper, SIGNAL(mapped(int)), this, SIGNAL(clickedToolButton(int)));

	m_pComboBoxFileSystem=new QComboBoxFileSystem(this);
	m_pComboBoxFileSystem->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
	m_pHorizontalLayout->addWidget(m_pComboBoxFileSystem);

	m_pVerticalLayout->addLayout(m_pHorizontalLayout);

	m_pTreeViewFileSystem=new QTreeViewFileSystem(this);
	m_pTreeViewFileSystem->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	m_pVerticalLayout->addWidget(m_pTreeViewFileSystem);

	m_pTreeViewFileSystem->setEditable(true);
	m_pTreeViewFileSystem->setEditTriggers(QAbstractItemView::SelectedClicked);

	connect(m_pComboBoxFileSystem, SIGNAL(itemPressed(QModelIndex)), this, SLOT(itemPressed(QModelIndex)));
}

void QFrameFileSystem::clear()
{
	m_pComboBoxFileSystem->clear();
	m_pTreeViewFileSystem->clear();
}

void QFrameFileSystem::setDrives(const QFileInfoList& drives)
{
	m_pComboBoxFileSystem->setDrives(drives);
}

void QFrameFileSystem::setDrives(const QVector<FileInfo>& drives)
{
	m_pComboBoxFileSystem->setDrives(drives);
}

void QFrameFileSystem::setFileInfo(const QFileInfoList& files, int itemType)
{
	bool isDrive=false;
	if (itemType == QComboBoxFileSystem::Computer)
	{
		isDrive=true;
		m_pComboBoxFileSystem->setDrives(files);
	}

	m_pTreeViewFileSystem->clear();
	m_pTreeViewFileSystem->insertFileInfo(files, isDrive);
}

void QFrameFileSystem::setFileInfo(const QVector<FileInfo>& files, int itemType)
{
	bool isDrive=false;
	if (itemType == QComboBoxFileSystem::Computer)
	{
		isDrive=true;
		m_pComboBoxFileSystem->setDrives(files);
	}

	m_pTreeViewFileSystem->clear();
	m_pTreeViewFileSystem->insertFileInfo(files, isDrive);
}

void QFrameFileSystem::setLocation(const QString& location, bool test)
{
	if (m_pComboBoxFileSystem->topItemType(location) != -1)
	{
		m_pToolButton[ButtonUp]->setEnabled(false);
	}
	else
	{
		m_pToolButton[ButtonUp]->setEnabled(true);
	}

	m_pComboBoxFileSystem->setCurrentLocation(location, test);
}

QString QFrameFileSystem::location() const
{
	return m_pComboBoxFileSystem->currentLocation();
}

void QFrameFileSystem::itemPressed(QModelIndex index)
{
	QString strLocation=m_pComboBoxFileSystem->selectedLocation();
	if (strLocation.isEmpty())
	{
		return;
	}

	emit currentIndexChanged(strLocation);
}
