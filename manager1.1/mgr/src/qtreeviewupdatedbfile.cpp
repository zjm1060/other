#include "qtreeviewupdatedbfile.h"

#include <QMouseEvent>
#include <QDir>
#include <QApplication>

#include "common.h"
#include "qimessage.h"
#include "qglobalinfo.h"
#include "qlistviewsyncfilerequestserver.h"

QTreeViewUpdateDbFile::QTreeViewUpdateDbFile(QWidget *parent)
: QTreeViewUpdateConfFile(parent)
{
}

QTreeViewUpdateDbFile::~QTreeViewUpdateDbFile()
{
}

REGISTER_TREE_VIEW_UPDATE_FILE_CLASS(QTreeViewUpdateDbFile)

QTreeViewUpdateDbFileEx::QTreeViewUpdateDbFileEx(QWidget *parent)
	: QTreeViewUpdateFile(parent)
{
}

QTreeViewUpdateDbFileEx::~QTreeViewUpdateDbFileEx()
{
}

void QTreeViewUpdateDbFileEx::check()
{	
	QUpdateDbFileRunnable* pRunnable=new QUpdateDbFileRunnable(m_strWorkspace, record(), m_strHostName, m_strAddress, m_bRestrictFilter, m_bLocalLastModified);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QTreeModel*)), this, SLOT(finished(QTreeModel*)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

void QTreeViewUpdateDbFileEx::checkModified(const QString& workspace, const QString& hostname, const QString& address)
{
	QUpdateDbFileRunnable* pRunnable=new QUpdateDbFileRunnable(workspace, record(), hostname, address, m_bRestrictFilter, m_bLocalLastModified);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(modified(QString, QString)), this, SIGNAL(findModified(QString, QString)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

REGISTER_TREE_VIEW_UPDATE_FILE_CLASS(QTreeViewUpdateDbFileEx)

QUpdateDbFileRunnable::QUpdateDbFileRunnable(const QString& workspace, const QString& catagory, const QString& hostname, const QString& address,
											 bool restrictFilter, bool localLastModified, bool immediate/* =false */, QObject* parent/* =0 */)
:QUpdateFileRunnable(workspace, catagory, hostname, address, restrictFilter, localLastModified, immediate, parent)
{

}

QUpdateDbFileRunnable::~QUpdateDbFileRunnable()
{

}

bool QUpdateDbFileRunnable::readFiles(bool immediate)
{
	bool bModified=false;

//	bModified|=readCommonFiles(immediate);
	bModified|=readProjectFiles(immediate);

	return bModified;
}

bool QUpdateDbFileRunnable::readProjectFiles(bool immediate)
{
	QVector<RECORD_STATION_ENTRY> records;

	QFile file(appendPath(m_strWorkspace, RECORD_DEF_FILE));
	if (file.exists() && file.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		QTextStream stream(&file);
		records=parseRecordDefFile(stream);
		file.close();
	}

	QVector<STATION_ENTRY> stations;
	QFile stnFile(appendPath(m_strWorkspace, STATION_DEF_FILE));
	if (stnFile.exists() && stnFile.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		QTextStream stream(&stnFile);
		stations=parseStationDefFile(stream);
		file.close();
	}

	QVector<GROUP_ENTRY_EX> groups;
	QFile grpFile(appendPath(m_strWorkspace, GROUP_DEF_FILE));
	if (grpFile.exists() && grpFile.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		QTextStream stream(&grpFile);
		groups=parseGroupDefFile(stream);
		file.close();
	}

	bool bIsModified=false;

	for (int i=0; i<records.size(); i++)
	{
		const RECORD_STATION_ENTRY& station=records[i];

		QModelIndex ppindex;
		int nRow;
		if (!immediate)
		{
			nRow=m_pModel->rowCount();
			m_pModel->insertRow(nRow);

			ppindex=m_pModel->index(nRow, QTreeViewUpdateFile::NameTitle);
		}

		QString strStnName;
		for (int i=0; i<stations.size(); i++)
		{
			if (stations[i].stnName == station.stnName)
			{
				strStnName=stations[i].stnLongname;
				break;
			}
		}

		if (!immediate)
		{
			m_pModel->setData(ppindex, strStnName.isEmpty()? station.stnName:QString("%1(%2)").arg(strStnName).arg(station.stnName), Qt::DisplayRole);
			m_pModel->setData(ppindex, QTreeViewUpdateFile::GroupRole, QTreeViewUpdateFile::ItemRole);
		}

		bool bGroupModified=false;

		GROUP_ENTRY_EX group;
		for (int i=0; i<groups.size(); i++)
		{
			if (groups[i].stnName == station.stnName)
			{
				group=groups[i];
				break;
			}
		}

		for (int j=0; j<station.groups.size(); j++)
		{
			const RECORD_GROUP_ENTRY& device=station.groups[j];

			int nGrpRow;
			if (!immediate)
			{
				nGrpRow=m_pModel->rowCount(ppindex);
				m_pModel->insertRow(nGrpRow, ppindex);
			}

			QString strGroupName;
			for (int i=0; i<group.entries.size(); i++)
			{
				if (group.entries[i].grpName == device.grpName)
				{
					strGroupName=group.entries[i].grpComment;
					break;
				}
			}

			QModelIndex pindex;
			if (!immediate)
			{
				pindex=m_pModel->index(nGrpRow, QTreeViewUpdateDbFile::NameTitle, ppindex);
				m_pModel->setData(pindex, strGroupName.isEmpty()? device.grpName:QString("%1(%2)").arg(strGroupName).arg(device.grpName), Qt::DisplayRole);
				m_pModel->setData(pindex, QTreeViewUpdateFile::GroupRole, QTreeViewUpdateFile::ItemRole);
			}

			bool bModified=false;

			for (int k=0; k<device.records.size(); k++)
			{
				bModified|=readFile(device.records[k], immediate, pindex);
				if (bModified && immediate)
				{
					return bModified;
				}
			}

			if (!immediate)
			{
				setParentCheckedState(pindex);

				QModelIndex index=m_pModel->index(nGrpRow, QTreeViewUpdateFile::StatusTitle, ppindex);
				m_pModel->setData(index, bModified? Modified:Unmodified, Qt::DisplayRole);
			}

			bGroupModified|=bModified;
		}

		bIsModified|=bGroupModified;

		if (!immediate)
		{
			setParentCheckedState(ppindex);

			QModelIndex index=m_pModel->index(nRow, QTreeViewUpdateFile::StatusTitle);
			m_pModel->setData(index, bGroupModified? Modified:Unmodified, Qt::DisplayRole);
		}
	}

	return bIsModified;
}

void QUpdateDbFileRunnable::setParentCheckedState(const QModelIndex& pindex)
{
	int nChecked=-1;
	int nRowCount=m_pModel->rowCount(pindex);
	for (int nRow=0; nRow<nRowCount; nRow++)
	{
		int nStatus=m_pModel->data(m_pModel->index(nRow, QTreeViewUpdateFile::NameTitle, pindex), QTreeViewUpdateFile::CheckedRole).toInt();
		if (nChecked == -1 || nStatus == Qt::PartiallyChecked)
		{
			nChecked=nStatus;
		}
		else if (nChecked != Qt::PartiallyChecked)
		{
			if ((nChecked==Qt::Checked && nStatus == Qt::Unchecked) || (nChecked == Qt::Unchecked && nStatus == Qt::Checked))
			{
				nChecked=Qt::PartiallyChecked;
			}
		}

		if (nChecked == Qt::PartiallyChecked)
		{
			m_pModel->setData(pindex, nChecked, QTreeViewUpdateFile::CheckedRole);
			break;
		}
	}
}

bool QUpdateDbFileRunnable::readCommonFiles(bool immediate)
{
	QStringList files;
	files<<STATION_DEF_FILE<<GROUP_DEF_FILE<<RECORD_DEF_FILE<<MSG_DEF_FILE<<CMD_MSG_FILE<<IOGROUP_DEF_FILE;
	
	bool bModified=false;

	QStringListIterator it(files);
	while (it.hasNext())
	{
		const QString& name=it.next();
		
		bModified|=readFile(name, immediate);
		if (bModified && immediate)
		{
			return bModified;
		}
	}

	return bModified;
}
