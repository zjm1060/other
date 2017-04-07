#include "qtreeviewupdatertdbfile.h"

#include <QFileInfo>
#include <QDir>

#include "qglobalinfo.h"
#include "qimessage.h"

QTreeViewUpdateRtdbFile::QTreeViewUpdateRtdbFile(QWidget *parent)
	: QTreeViewUpdateConfFile(parent)
{
}

QTreeViewUpdateRtdbFile::~QTreeViewUpdateRtdbFile()
{
}

void QTreeViewUpdateRtdbFile::check()
{	
	QUpdateRtdbFileRunnable* pRunnable=new QUpdateRtdbFileRunnable(m_strWorkspace, record(), m_strHostName, m_strAddress, m_bLocalLastModified);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QTreeModel*)), this, SLOT(finished(QTreeModel*)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

void QTreeViewUpdateRtdbFile::checkModified(const QString& workspace, const QString& hostname, const QString& address)
{
	QUpdateRtdbFileRunnable* pRunnable=new QUpdateRtdbFileRunnable(workspace, record(), hostname, address, m_bLocalLastModified);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(modified(QString, QString)), this, SIGNAL(findModified(QString, QString)));

	QGlobalInfo::globalThreadPool()->start(pRunnable);
}

REGISTER_TREE_VIEW_UPDATE_FILE_CLASS(QTreeViewUpdateRtdbFile)

QUpdateRtdbFileRunnable::QUpdateRtdbFileRunnable(const QString& workspace, const QString& catagory, const QString& hostname, const QString& address, 
												 bool localLastModified, bool immediate/* =false */, QObject* parent/* =0 */)
											 :QUpdateFileRunnable(workspace, catagory, hostname, address, false, localLastModified, immediate, parent)
{

}

QUpdateRtdbFileRunnable::~QUpdateRtdbFileRunnable()
{

}

bool QUpdateRtdbFileRunnable::readFiles(bool immediate)
{
	bool bModified=false;

	QDir dir(m_strWorkspace);
	QFileInfoList files=dir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot|QDir::NoSymLinks);
	QListIterator<QFileInfo> it(files);
	while (it.hasNext())
	{
		const QFileInfo& file=it.next();

		if (file.isFile() && file.fileName() == H9000_DATABASE_FILE)
		{
			bModified|=readFile(file.fileName(), immediate);
		}
	}

	if (!m_filesRequest.isEmpty())
	{
		QMap<QString, DiffFile> requests=m_filesRequest;
		QMapIterator<QString, DiffFile> it(requests);
		while (it.hasNext())
		{
			it.next();

			const DiffFile& file=it.value();

			if ((file.type & (File|HideFile)) != 0 && file.path == H9000_DATABASE_FILE)
			{
				readFile(it.key(), immediate);
			}
		}
	}

	return bModified;
}
