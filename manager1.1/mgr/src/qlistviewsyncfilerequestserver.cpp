#include "qlistviewsyncfilerequestserver.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QDir>

#include "common.h"
#include "qtipsdelegate.h"
#include "qreadwritexmlfile.h"
#include "qglobalinfo.h"
#include "qtreeviewupdatefile.h"

QListViewSyncFileRequestServer::QListViewSyncFileRequestServer(QWidget *parent)
	: QListViewHorizontalIcon(parent)
{
	qRegisterMetaType<QVector<QPair<QString, QString> > >("QVector<QPair<QString, QString> >");

	m_pixmap[HostIcon]=findPixmap(":/images/host.png").scaled(QSize(LIST_VIEW_ICON_SIZE, LIST_VIEW_ICON_SIZE));
	m_pixmap[CheckIcon]=findPixmap(":/images/checkmark.png").scaled(QSize(LIST_VIEW_MARK_ICON_SIZE, LIST_VIEW_MARK_ICON_SIZE));
	m_pixmap[ModifiedIcon]=findPixmap(":/images/modified.png").scaled(QSize(LIST_VIEW_MARK_ICON_SIZE, LIST_VIEW_MARK_ICON_SIZE));

	setSelectionMode(QAbstractItemView::SingleSelection);

	connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));

	QStringList labels;
	labels<<tr("Server");

	m_pModel=new QStandardItemModel(0, labels.size());
	m_pModel->setHorizontalHeaderLabels(labels);

	setModel(m_pModel);

	setMouseTracking(true);
}

QListViewSyncFileRequestServer::~QListViewSyncFileRequestServer()
{
	setModel(NULL);

	SAFE_DELETE_CLASS(m_pModel);
}

void QListViewSyncFileRequestServer::entered(const QModelIndex& index)
{
	if (m_pModel == NULL)
	{
		return;
	}

	if (index.isValid())
	{
		QString strMessage;

		if (m_pModel->data(index, StatusRole).toBool())
		{
			strMessage=tr("Detect a new update request,please update your workspace!");

			emit sendTips(QReceiverTransferFile::Warning, strMessage);
		}
		else
		{
			bool bSelect=m_pModel->data(index, SelectedRole).toBool();
			if (bSelect)
			{
				strMessage=tr("Your workspace is already latest.");

				emit sendTips(QReceiverTransferFile::Information, strMessage);
			}
			else
			{
				strMessage=tr("Click to view the differences of files between the local workspace and uploaded files.");

				emit sendTips(QReceiverTransferFile::Information, strMessage);
			}
		}		
	}
	else
	{
		emit sendTips(QReceiverTransferFile::Information, "");
	}
}

void QListViewSyncFileRequestServer::mouseMoveEvent(QMouseEvent* event)
{
	QModelIndex index=indexAt(event->pos());

	entered(index);

	QListViewHorizontalIcon::mouseMoveEvent(event);
}

void QListViewSyncFileRequestServer::loadServers(const QString& category)
{
	QVector<QPair<QString, QString> > servers;

	QString strPath=appendPath(getAppParentDir(), SYNC_H9000_FILE_REQUEST_RECORD_PATH);
	QDir dir(strPath);
	QStringList names=dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks, QDir::Time|QDir::Reversed);
	if (!names.isEmpty())
	{
		QRegExp rx("(^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])(\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])){3})$");
		foreach (QString name, names)
		{
			QStringList parts=name.split(SERVER_HOST_IP_SEPARATOR);
			if (parts.isEmpty() || parts.size() > 2)
			{
				continue;
			}

			int nPos=rx.indexIn(parts[0]);
			if (nPos == -1)
			{
				continue;
			}

			QString strAddress=rx.cap(0);
			nPos+=rx.matchedLength();

			QString strHostName=parts[1];
			
			if (!dir.exists(appendPath(strPath, QString("%1/%2").arg(name).arg(category))))
			{
				continue;
			}

			QPair<QString, QString> server(strHostName, strAddress);

			addServer(strHostName, strAddress);

			servers.append(server);
		}
	}

	emit loaded(servers);

	updateListView();
}

void QListViewSyncFileRequestServer::modified(QString hostname, QString address)
{
	if (m_pModel == NULL)
	{
		return;
	}

	QPair<QString, QString> server(hostname, address);

	int nRowCount=m_pModel->rowCount();
	for (int nRow=0; nRow<nRowCount; nRow++)
	{
		QModelIndex index=m_pModel->index(nRow, 0);
		if (server == qvariant_cast<QPair<QString, QString> >(m_pModel->data(index,UserDataRole)))
		{
			setServerItemModifiedStatus(index, true);
		}
	}
}

const QModelIndex QListViewSyncFileRequestServer::findServer(const QString& hostname, const QString& address) const
{
	if (m_pModel != NULL)
	{
		QPair<QString, QString> server(hostname, address);

		int nRowCount=m_pModel->rowCount();
		for (int nRow=0; nRow<nRowCount; nRow++)
		{
			QModelIndex index=m_pModel->index(nRow, 0);
			if (server == qvariant_cast<QPair<QString, QString> >(m_pModel->data(index,UserDataRole)))
			{
				return index;
			}
		}	
	}

	return QModelIndex();
}

void QListViewSyncFileRequestServer::addServer(const QString& hostname, const QString& address)
{
	if (m_pModel == NULL)
	{
		return;
	}

	int nRow=m_pModel->rowCount();
	for (int i=0; i<nRow; i++)
	{
		QPair<QString, QString> peer=qvariant_cast<QPair<QString, QString> >(m_pModel->data(m_pModel->index(i, 0), UserDataRole));
		if (peer.first == hostname && peer.second == address)
		{
			return;
		}
	}

	m_pModel->insertRow(nRow);
	const QModelIndex&index=m_pModel->index(nRow, 0);
	m_pModel->itemFromIndex(index)->setEditable(false);

	QString strText=hostname.isEmpty()? address:hostname+"\n"+address;

	m_pModel->setData(index, strText, Qt::EditRole);
	m_pModel->setData(index, qVariantFromValue(QPair<QString, QString>(hostname, address)), UserDataRole);

	m_pModel->itemFromIndex(index)->setToolTip(strText);
	
	setServerItemSelectStatus(index, false);
}

QPair<QString, QString> QListViewSyncFileRequestServer::server(const QModelIndex& index)
{
	QPair<QString, QString> peer;

	if (m_pModel != NULL)
	{
		peer=qvariant_cast<QPair<QString, QString> >(m_pModel->data(index, UserDataRole));
	}

	return peer;
}

void QListViewSyncFileRequestServer::setServerItemSelectStatus(const QModelIndex& index, bool select)
{
	if (m_pModel == NULL || !index.isValid())
	{
		return;
	}

	m_pModel->setData(index, select, SelectedRole);

	updateIcon(index);
}

void QListViewSyncFileRequestServer::setServerItemModifiedStatus(const QModelIndex& index, bool modified)
{
	if (m_pModel == NULL || !index.isValid())
	{
		return;
	}

	m_pModel->setData(index, modified, StatusRole);
	
	updateIcon(index);
}

void QListViewSyncFileRequestServer::updateIcon(const QModelIndex& index)
{
	QPixmap pixmap=m_pixmap[HostIcon];

	if (m_pModel->data(index, SelectedRole).toBool())
	{
		pixmap= addPixmapToPixmap(m_pixmap[CheckIcon], pixmap, Qt::BottomRightCorner);
	}

	if (m_pModel->data(index, StatusRole).toBool())
	{
		pixmap=addPixmapToPixmap(m_pixmap[ModifiedIcon], pixmap, Qt::TopLeftCorner);
	}

	m_pModel->setData(index, pixmap, Qt::DecorationRole);
}

void QListViewSyncFileRequestServer::clicked(const QModelIndex& index)
{
	if (m_pModel == NULL || index == m_index)
	{
		return;
	}

	setServerItemSelectStatus(m_index, false);
	setServerItemSelectStatus(index, true);

	m_index=index;

	emit currentChaged(index);
}

const QModelIndex QListViewSyncFileRequestServer::currentItem() const
{
	if (m_pModel == NULL)
	{
		return QModelIndex();
	}

	int nRow=m_pModel->rowCount();
	for (int i=0; i<nRow; i++)
	{
		bool bSelected=m_pModel->data(m_pModel->index(i, 0), SelectedRole).toBool();
		if (bSelected)
		{
			return m_pModel->index(i, 0);
		}
	}

	return QModelIndex();
}

QReadRevisionRecordFormXmlFile::QReadRevisionRecordFormXmlFile(const QString& filename, const QString& workspace)
:QAbstractReadXmlFile(filename)
,m_strWorkspace(workspace)
{
	m_results.clear();
}

QReadRevisionRecordFormXmlFile::~QReadRevisionRecordFormXmlFile()
{

}

void QReadRevisionRecordFormXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "workspace", readerWorkspaceNode);
}

void QReadRevisionRecordFormXmlFile::readerWorkspaceNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	QXmlStreamAttributes attributes=reader.attributes();
	if (attributes.hasAttribute("path") && attributes.value("path").toString() == m_strWorkspace)
	{
		readNextNode(reader, "folder", readFolderNode);

		return;
	}

	readItemFinished(reader);
}
 
void QReadRevisionRecordFormXmlFile::readFolderNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("hostname") && attributes.hasAttribute("ip"))
	{
		m_server.first=attributes.value("hostname").toString();
		m_server.second=attributes.value("ip").toString();

		m_results.insert(m_server, QVector<DiffFile>());

		readNextNode(reader, "file", readFileNode);
	}
}

void QReadRevisionRecordFormXmlFile::readFileNode(QXmlStreamReader& reader)
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

		m_results[m_server].append(file);
	}

	readItemFinished(reader);
}
