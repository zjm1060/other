#include "qtreeviewsyncdbfile.h"

#include <QMouseEvent>
#include <QDir>

#include "common.h"
#include "qimessage.h"
#include "qglobalinfo.h"

QTreeViewSyncDbFile::QTreeViewSyncDbFile(QWidget *parent)
: QTreeViewSyncConfFile(parent)
{
}

QTreeViewSyncDbFile::~QTreeViewSyncDbFile()
{
}

REGISTER_TREE_VIEW_SYNC_COMMIT_CLASS(QTreeViewSyncDbFile)

QTreeViewSyncDbFileEx::QTreeViewSyncDbFileEx(QWidget *parent)
	: QTreeViewSyncCommit(parent)
{
}

QTreeViewSyncDbFileEx::~QTreeViewSyncDbFileEx()
{
	
}

void QTreeViewSyncDbFileEx::checkDiff()
{
	QSyncDbFileRunnable* pRunnable=new QSyncDbFileRunnable(m_strWorkspace, m_strHistory, record(), true, m_bRestrictFilter);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QTreeModel*)), this, SLOT(finished(QTreeModel*)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

QSyncDbFileRunnable::QSyncDbFileRunnable(const QString& workspace, const QString& history, const QString& record, 
										 bool checkable, bool enableRestrict, QObject* parent/* =0 */)
:QTreeViewSyncCommitRunnable(workspace, history, record, checkable, enableRestrict, parent)
{

}

QSyncDbFileRunnable::~QSyncDbFileRunnable()
{

}

void QSyncDbFileRunnable::check()
{
//	readCommonFiles();
	readProjectFiles();
}

void QSyncDbFileRunnable::readProjectFiles()
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

	for (int i=0; i<records.size(); i++)
	{
		const RECORD_STATION_ENTRY& station=records[i];
		int nRow=m_pModel->rowCount();
		m_pModel->insertRow(nRow);

		QModelIndex ppindex=m_pModel->index(nRow, QTreeViewSyncCommit::NameTitle);

		QString strStnName;
		for (int i=0; i<stations.size(); i++)
		{
			if (stations[i].stnName == station.stnName)
			{
				strStnName=stations[i].stnLongname;
				break;
			}
		}

		m_pModel->setData(ppindex, strStnName.isEmpty()? station.stnName:QString("%1(%2)").arg(strStnName).arg(station.stnName), Qt::DisplayRole);
		m_pModel->setData(ppindex, QTreeViewSyncCommit::GroupRole, QTreeViewSyncCommit::ItemRole);
		m_pModel->setData(ppindex, m_bCheckable, QTreeViewSyncCommit::CheckableRole);

		int nGroupChecked=-1;

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

			int nGrpRow=m_pModel->rowCount(ppindex);
			m_pModel->insertRow(nGrpRow, ppindex);

			QString strGroupName;
			for (int i=0; i<group.entries.size(); i++)
			{
				if (group.entries[i].grpName == device.grpName)
				{
					strGroupName=group.entries[i].grpComment;
					break;
				}
			}

			QModelIndex pindex=m_pModel->index(nGrpRow, QTreeViewSyncCommit::NameTitle, ppindex);
			m_pModel->setData(pindex, strGroupName.isEmpty()? device.grpName:QString("%1(%2)").arg(strGroupName).arg(device.grpName), Qt::DisplayRole);
			m_pModel->setData(pindex, QTreeViewSyncCommit::GroupRole, QTreeViewSyncCommit::ItemRole);
			m_pModel->setData(pindex, m_bCheckable, QTreeViewSyncCommit::CheckableRole);

			int nChecked=-1;

			for (int k=0; k<device.records.size(); k++)
			{
				bool bChecked=checkFile(device.records[k], pindex);
				if (nChecked == -1)
				{
					nChecked=bChecked? Qt::Checked:Qt::Unchecked;
				}
				else if (nChecked != Qt::PartiallyChecked)
				{
					if ((nChecked==Qt::Checked && !bChecked) || (nChecked == Qt::Unchecked && bChecked))
					{
						nChecked=Qt::PartiallyChecked;
					}
				}
			}

			m_pModel->setData(pindex, nChecked, QTreeViewSyncCommit::CheckedRole);

			if (nGroupChecked == -1)
			{
				nGroupChecked=nChecked;
			}
			else if (nGroupChecked != Qt::PartiallyChecked && nGroupChecked != nChecked)
			{
				nGroupChecked=Qt::PartiallyChecked;
			}
		}

		m_pModel->setData(ppindex, nGroupChecked, QTreeViewSyncCommit::CheckedRole);
	}
}

void QSyncDbFileRunnable::readCommonFiles()
{
	QStringList files;
	files<<STATION_DEF_FILE<<GROUP_DEF_FILE<<RECORD_DEF_FILE<<MSG_DEF_FILE<<CMD_MSG_FILE<<IOGROUP_DEF_FILE;
	
	QStringListIterator it(files);
	while (it.hasNext())
	{
		const QString& name=it.next();
		
		checkFile(name);
	}
}

REGISTER_TREE_VIEW_SYNC_COMMIT_CLASS(QTreeViewSyncDbFileEx)

