#include "qreadwritexmlfile.h"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include "common.h"
#include "qdirex.h"
#include "qfileex.h"

QReadConfigureFormXmlFile::QReadConfigureFormXmlFile(const QString& filename)
:QAbstractReadXmlFile(filename)
{
	m_configure.clear();
}

QReadConfigureFormXmlFile::~QReadConfigureFormXmlFile()
{

}

void QReadConfigureFormXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "configure", readConfigureNode);
}

void QReadConfigureFormXmlFile::readConfigureNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "conf", readConfNode);
}

void QReadConfigureFormXmlFile::readConfNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("key") && attributes.hasAttribute("value"))
	{
		QString key=attributes.value("key").toString();
		QString value=attributes.value("value").toString();

		m_configure.insert(key, value);
	}

	readItemFinished(reader);
}

QWriteConfigureToXmlFile::QWriteConfigureToXmlFile(const QString& filename, const QMap<QString, QString>& configure)
:QAbstractWriteXmlFile(filename)
,m_configure(configure)
{

}

QWriteConfigureToXmlFile::~QWriteConfigureToXmlFile()
{

}

void QWriteConfigureToXmlFile::save(QDomDocument& doc, QDomElement& parent)
{
	QMapIterator<QString, QString> it(m_configure);
	while (it.hasNext())
	{
		it.next();

		if (it.key().isEmpty() || it.value().isEmpty())
		{
			continue;
		}

		QDomElement node=doc.createElement("conf");

		node.setAttribute("key", it.key());
		node.setAttribute("value", it.value());

		parent.appendChild(node);
	}
}

QMonitorConfigureFile::QMonitorConfigureFile(const QString& filename, QObject* watched, QObject* parent/* =0 */)
:QAbstractMonitor(filename, watched, parent)
{
	qRegisterMetaType<QMap<QString, QString> >("QMap<QString, QString>");
}

QMonitorConfigureFile::~QMonitorConfigureFile()
{

}

bool QMonitorConfigureFile::loadFile()
{
	bool bChanged=false;

	m_mutex.lock();

	QReadConfigureFormXmlFile reader(m_strFileName);
	if (reader.read())
	{
		QMap<QString, QString> configure=reader.result();

		if (configure.size() != m_configure.size() || configure != m_configure)
		{
			m_configure=configure;

			bChanged=true;
		}
	}

	m_mutex.unlock();

	if (m_bFirst || bChanged)
	{
		if (m_bFirst)
		{
			m_bFirst=false;
		}

		emit changed(m_configure);
	}

	return bChanged;
}

void QMonitorConfigureFile::save(const QMap<QString, QString>& configure)
{
	QMutexLocker locker(&m_mutex);

	QWriteConfigureToXmlFile writer(m_strFileName, configure);
	writer.write();
}

QWriteWorkspaceToXmlFile::QWriteWorkspaceToXmlFile(const QString& filename, const QString& workspace)
:QAbstractWriteXmlFile(filename)
,m_strWorkspace(workspace)
{
}

QWriteWorkspaceToXmlFile::~QWriteWorkspaceToXmlFile()
{

}

void QWriteWorkspaceToXmlFile::save(QDomDocument& doc, QDomElement& parent)
{
	Q_UNUSED(doc);
	parent.setAttribute("path", m_strWorkspace);
}

bool QWriteWorkspaceToXmlFile::check(const QDomElement& node) const
{
	if (node.hasAttribute("path") && node.attribute("path") == m_strWorkspace)
	{
		return true;
	}

	return false;
}

QReadWorkspaceFromXmlFile::QReadWorkspaceFromXmlFile(const QString& filename)
:QAbstractReadXmlFile(filename)
{

}

QReadWorkspaceFromXmlFile::~QReadWorkspaceFromXmlFile()
{

}

void QReadWorkspaceFromXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "workspace", readWorkspaceNode);
}

void QReadWorkspaceFromXmlFile::readWorkspaceNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("path"))
	{
		QString strPath=attributes.value("path").toString();
		if (!strPath.isEmpty())
		{
			m_folders.append(strPath);
		}
	}

	readItemFinished(reader);
}

QReadWorkspaceRecordsFromXmlFile::QReadWorkspaceRecordsFromXmlFile(const QString& filename, const QString& path)
:QAbstractReadXmlFile(filename)
,m_strPath(path)
{

}

QReadWorkspaceRecordsFromXmlFile::~QReadWorkspaceRecordsFromXmlFile()
{

}

void QReadWorkspaceRecordsFromXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "workspace", readWorkspaceNode);
}

void QReadWorkspaceRecordsFromXmlFile::readWorkspaceNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("path"))
	{
		QString strPath=attributes.value("path").toString();
		if (strPath == m_strPath)
		{
			readNextNode(reader, "file", readFile);
			return;
		}
	}

	readItemFinished(reader);
}

void QReadWorkspaceRecordsFromXmlFile::readFile(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "file");

	QXmlStreamAttributes attributes=reader.attributes();
	DiffFile file;

	if (attributes.hasAttribute("path") && attributes.hasAttribute("size") && attributes.hasAttribute("type")
		&& attributes.hasAttribute("modified") && attributes.hasAttribute("status"))
	{
		file.path=attributes.value("path").toString();
		if (!file.path.isEmpty())
		{
			file.size=attributes.value("size").toString().toULongLong();
			file.type=attributes.value("type").toString().toInt();
			file.modified=QDateTime::fromString(attributes.value("modified").toString(), FILE_MODIFIED_DATETIME_FORMAT);
			file.status=attributes.value("status").toString().toInt();
			if (file.status >= 0 && file.status < ModifiedStatusEnd)
			{
				m_files.insert(file.path, file);
			}
		}
	}

	readItemFinished(reader);
}

QWriteWorkspaceRecordsToXmlFile::QWriteWorkspaceRecordsToXmlFile(const QString& filename, const QString& workspace, const QVector<DiffFile>& files)
:QAbstractWriteXmlFile(filename)
,m_files(files)
,m_strWorkspace(workspace)
{

}

QWriteWorkspaceRecordsToXmlFile::~QWriteWorkspaceRecordsToXmlFile()
{

}

void QWriteWorkspaceRecordsToXmlFile::save(QDomDocument& doc, QDomElement& parent)
{
	parent.setAttribute("path", m_strWorkspace);
	QDomElement child = parent.firstChildElement();
	while (!child.isNull())
	{
		QDomElement node=child;

		child = child.nextSiblingElement();

		parent.removeChild(node);
	}

	for (int i=0; i<m_files.size(); i++)
	{
		saveRecords(doc, parent, m_files[i]);
	}
}

bool QWriteWorkspaceRecordsToXmlFile::check(const QDomElement& node) const
{
	if (node.hasAttribute("path") && node.attribute("path") == m_strWorkspace)
	{
		return true;
	}

	return false;
}

void QWriteWorkspaceRecordsToXmlFile::saveRecords(QDomDocument& doc, QDomElement& node, const DiffFile& file)
{
	if (file.status == Deleted)
	{
		return;
	}

	QDomElement child=doc.createElement("file");

	child.setAttribute("path", file.path);
	child.setAttribute("type", file.type);
	child.setAttribute("size", file.size);
	child.setAttribute("modified", file.modified.toString(FILE_MODIFIED_DATETIME_FORMAT));
	child.setAttribute("status", file.status);

	node.appendChild(child);
}

QMonitorServersFile::QMonitorServersFile(const QString& filename, QObject* watched, QObject* parent/* =0 */)
:QAbstractMonitor(filename, watched, parent)
{
	qRegisterMetaType<QList<Server> >("QList<Server>");
}

QMonitorServersFile::~QMonitorServersFile()
{

}

bool QMonitorServersFile::loadFile()
{
	bool bChanged=false;

	m_mutex.lock();

	QReadServersFromXmlFile reader(m_strFileName);
	if (reader.read())
	{
		QList<Server> servers=reader.result();
		if (servers.size() != m_servers.size() || servers != m_servers)
		{
			m_servers=servers;

			bChanged=true;
		}
	}

	m_mutex.unlock();

	if (m_bFirst || bChanged)
	{
		if (m_bFirst)
		{
			m_bFirst=false;
		}

		emit changed(m_servers);
	}

	return bChanged;
}

void QMonitorServersFile::save(const QList<Server>& servers)
{
	QMutexLocker locker(&m_mutex);

	QWriteServersToXmlFile writer(m_strFileName, servers);
	writer.write();
}

QReadServersFromXmlFile::QReadServersFromXmlFile(const QString& filename)
:QAbstractReadXmlFile(filename)
{

}

QReadServersFromXmlFile::~QReadServersFromXmlFile()
{

}

void QReadServersFromXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "servers", readServersNode);
}

void QReadServersFromXmlFile::readServersNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "server", readServer);
}

void QReadServersFromXmlFile::readServer(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "server");

	QXmlStreamAttributes attributes=reader.attributes();

	Server server;

	if (attributes.hasAttribute("ip") && attributes.hasAttribute("port"))
	{
		server.addr=attributes.value("ip").toString();
		bool bOk;
		server.port=(quint16)attributes.value("port").toString().toInt(&bOk);
		if (!server.addr.isEmpty() && bOk)
		{
			m_servers.append(server);
		}
	}

	readItemFinished(reader);
}

QReadRequestRecordFormXmlFile::QReadRequestRecordFormXmlFile(const QString& filename)
:QAbstractReadXmlFile(filename)
{

}

QReadRequestRecordFormXmlFile::~QReadRequestRecordFormXmlFile()
{

}

void QReadRequestRecordFormXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "folder", readFolderNode);
}

void QReadRequestRecordFormXmlFile::readFolderNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "file", readFileNode);
}

void QReadRequestRecordFormXmlFile::readFileNode(QXmlStreamReader& reader)
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

		m_results.append(file);
	}

	readItemFinished(reader);
}

QReadCheckRecordFromXmlFile::QReadCheckRecordFromXmlFile(const QString& filename)
:QAbstractReadXmlFile(filename)
{

}

QReadCheckRecordFromXmlFile::~QReadCheckRecordFromXmlFile()
{

}

void QReadCheckRecordFromXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "record", readRecord);
}

void QReadCheckRecordFromXmlFile::readRecord(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("revision"))
	{
		QString strRevision=attributes.value("revision").toString();
		bool bOk;
		m_nRevision=strRevision.toInt(&bOk);
		if (bOk)
		{
			readNextNode(reader, "file", readFile);
			return;
		}
	}

	readItemFinished(reader);
}

void QReadCheckRecordFromXmlFile::readFile(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "file");

	QXmlStreamAttributes attributes=reader.attributes();
	DiffFile file;

	if (attributes.hasAttribute("path") && attributes.hasAttribute("size") && attributes.hasAttribute("type")
		&& attributes.hasAttribute("modified") && attributes.hasAttribute("status"))
	{
		file.path=attributes.value("path").toString();
		if (!file.path.isEmpty())
		{
			file.size=attributes.value("size").toString().toULongLong();
			file.type=attributes.value("type").toString().toInt();
			file.modified=QDateTime::fromString(attributes.value("modified").toString(), FILE_MODIFIED_DATETIME_FORMAT);
			file.status=attributes.value("status").toString().toInt();
			file.version=attributes.value("version").toString().toInt();
			if (file.status >= 0 && file.status < UpdateFlagEnd)
			{
				QString strFileName=parseFileNameFromRevision(file.path);
				m_files.insert(strFileName, file);
			}
		}
	}

	readItemFinished(reader);
}

QWriteCheckRecordToXmlFile::QWriteCheckRecordToXmlFile(const QString& filename, int revision, const QVector<DiffFile>& files)
:QAbstractWriteXmlFile(filename)
,m_nRevision(revision)
,m_files(files)
{
	
}

QWriteCheckRecordToXmlFile::~QWriteCheckRecordToXmlFile()
{

}

void QWriteCheckRecordToXmlFile::save(QDomDocument& doc, QDomElement& parent)
{
	if (!parent.hasAttribute("revision"))
	{
		parent.setAttribute("revision", m_nRevision);
	}

	for (int i=0; i<m_files.size(); i++)
	{
		const DiffFile& file=m_files[i];

		QDomElement node=doc.createElement("file");

		node.setAttribute("path", file.path);
		node.setAttribute("type", file.type);
		node.setAttribute("size", file.size);
		node.setAttribute("modified", file.modified.toString(FILE_MODIFIED_DATETIME_FORMAT));
		node.setAttribute("status", file.status);
		node.setAttribute("version", file.version);

		parent.appendChild(node);
	}
}

QWriteServersToXmlFile::QWriteServersToXmlFile(const QString& filename, const QList<Server>& servers)
:QAbstractWriteXmlFile(filename)
,m_servers(servers)
{

}

QWriteServersToXmlFile::~QWriteServersToXmlFile()
{

}

void QWriteServersToXmlFile::save(QDomDocument& doc, QDomElement& parent)
{
	QListIterator<Server> it(m_servers);
	while (it.hasNext())
	{
		const Server& server=it.next();
		if (server.addr.isEmpty())
		{
			continue;
		}

		QDomElement node=doc.createElement("server");

		node.setAttribute("ip", server.addr);
		node.setAttribute("port", server.port);

		parent.appendChild(node);
	}
}

QString parseFileNameFromRevision(const QString& filename)
{
	QRegExp rx("^\\d+/");
	int nPos;
	if ((nPos=rx.indexIn(filename)) != -1)
	{
		nPos+=rx.matchedLength();
		return filename.right(filename.length()-nPos);
	}

	return filename;
}

