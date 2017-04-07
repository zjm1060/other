#include "qutilreadwritexmlfile.h"

#include "common.h"
#include "qutildef.h"

QReadCommandsFromXmlFile::QReadCommandsFromXmlFile(const QString& filename)
:QAbstractReadXmlFile(filename)
{

}

QReadCommandsFromXmlFile::~QReadCommandsFromXmlFile()
{

}

SyncCommands QReadCommandsFromXmlFile::result()
{
	if (m_results.commands.isEmpty() && m_results.programs.isEmpty())
	{
		SyncCommand cmd;
		cmd.enabled=true;
		cmd.command=H9000_FUNCTION_UPDATE_DATABASE_FILE;

		SyncMonitorFile file;
		file.enabled=true;
		file.path=appendPath(getAppParentDir(), QString("%1/%2").arg(H9000_DAT_PATH).arg(H9000_TMP_PATH));
		file.file=entryInfo(QFileInfo(appendPath(file.path, H9000_DATABASE_FILE)), false);
		
		cmd.files.append(file);

		cmd.params.append(appendPath(file.path, file.file.name));
		
		m_results.commands.insert(0, cmd);
	}

	return m_results;
}

void QReadCommandsFromXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "commands", readCommandsNode);
}

void QReadCommandsFromXmlFile::readCommandsNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "command", readCommandNode);
}

void QReadCommandsFromXmlFile::readCommandNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "command");

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("kind"))
	{
		QString strKind=attributes.value("kind").toString();
		if (strKind == SYNC_COMMANDS_PROGRAM_KIND)
		{
			if (attributes.hasAttribute("id") && attributes.hasAttribute("path") && attributes.hasAttribute("name") && attributes.hasAttribute("modified") && 
				attributes.hasAttribute("executable") && attributes.hasAttribute("size") && attributes.hasAttribute("type") && attributes.hasAttribute("enabled"))
			{
				SyncProgram program;
				int nID=attributes.value("id").toString().toInt();
				program.path=attributes.value("path").toString();
				program.program.name=attributes.value("name").toString();
				program.program.size=attributes.value("size").toString().toULongLong();
				program.program.type=attributes.value("type").toString().toInt();
				program.program.isExecutable=(attributes.value("executable").toString()=="true");
				program.program.lastModified=QDateTime::fromString(attributes.value("modified").toString(), STANDARD_DATETIME_FORMAT);
				program.enabled=(attributes.value("enabled").toString()=="true");

				if (attributes.hasAttribute("running"))
				{
					program.running=(attributes.value("running").toString()=="true");
				}
				else
				{
					program.running=false;
				}

				if (attributes.hasAttribute("terminal"))
				{
					program.terminal=(attributes.value("terminal").toString()=="true");
				}
				else
				{
					program.terminal=false;
				}

				if (!program.path.isEmpty() && !program.program.name.isEmpty())
				{
					readNextNodeEx(reader, "file", readFileNode, program.files);

					m_results.programs.insert(nID, program);

					return;
				}
			}
		}
		else if (strKind == SYNC_COMMANDS_COMMAND_KIND)
		{
			SyncCommand command;
			if (attributes.hasAttribute("id") && attributes.hasAttribute("name") && attributes.hasAttribute("enabled"))
			{
				int nID=attributes.value("id").toString().toInt();
				command.command=attributes.value("name").toString();
				command.enabled=(attributes.value("enabled").toString()=="true");
				if (!command.command.isEmpty())
				{
					int nIndex=0;
					QString strParamName=QString("param%1").arg(nIndex++);
					while (attributes.hasAttribute(strParamName))
					{
						command.params.append(attributes.value(strParamName).toString());

						strParamName=QString("param%1").arg(nIndex++);
					}

					readNextNodeEx(reader, "file", readFileNode, command.files);

					m_results.commands.insert(nID, command);

					return;
				}
			}
		}
	}

	readItemFinished(reader);
}

void QReadCommandsFromXmlFile::readFileNode(QXmlStreamReader& reader, QVector<SyncMonitorFile>& files)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "file");

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("path") && attributes.hasAttribute("name") && attributes.hasAttribute("size") && attributes.hasAttribute("type") &&
		attributes.hasAttribute("executable") && attributes.hasAttribute("modified"))
	{
		SyncMonitorFile file;

		file.path=attributes.value("path").toString();
		file.file.name=attributes.value("name").toString();
		file.file.size=attributes.value("size").toString().toULongLong();
		file.file.type=attributes.value("type").toString().toInt();
		file.file.isExecutable=(attributes.value("executable").toString()=="true");
		file.file.lastModified=QDateTime::fromString(attributes.value("modified").toString(), STANDARD_DATETIME_FORMAT);

		if (attributes.hasAttribute("enabled"))
		{
			file.enabled=(attributes.value("enabled").toString()=="true");
		}
		else
		{
			file.enabled=false;
		}

		if (!file.path.isEmpty() && !file.file.name.isEmpty())
		{
			files.append(file);
		}
	}
	
	readItemFinished(reader);
}

QWriteCommandsToXmlFile::QWriteCommandsToXmlFile(const QString& filename, const SyncCommands& commands)
:QAbstractWriteXmlFile(filename)
,m_commands(commands)
{

}

QWriteCommandsToXmlFile::~QWriteCommandsToXmlFile()
{

}

void QWriteCommandsToXmlFile::save(QDomDocument& doc, QDomElement& parent)
{
//	QDomElement node=findChildElement(doc, parent, m_strType);

	QDomElement child = parent.firstChildElement();
	while (!child.isNull())
	{
		QDomElement nd=child;

		child = child.nextSiblingElement();

		parent.removeChild(nd);
	}

	QMapIterator<int, SyncProgram> it(m_commands.programs);
	while (it.hasNext())
	{
		it.next();

		const SyncProgram& program=it.value();

		if (program.path.isEmpty() ||  program.program.name.isEmpty())
		{
			continue;
		}

		QDomElement child=doc.createElement("command");

		child.setAttribute("kind", SYNC_COMMANDS_PROGRAM_KIND);
		child.setAttribute("id", it.key());
		child.setAttribute("path", program.path);
		child.setAttribute("name", program.program.name);
		child.setAttribute("size", program.program.size);
		child.setAttribute("type", program.program.type);
		child.setAttribute("executable", program.program.isExecutable? "true":"false");
		child.setAttribute("modified", program.program.lastModified.toString(STANDARD_DATETIME_FORMAT));
		child.setAttribute("enabled", program.enabled? "true":"false");
		child.setAttribute("running", program.running? "true":"false");
		child.setAttribute("terminal", program.terminal? "true":"false");

		saveFiles(doc, child, program.files);

		parent.appendChild(child);
	}

	QMapIterator<int, SyncCommand> iter(m_commands.commands);
	while (iter.hasNext())
	{
		iter.next();

		const SyncCommand& command=iter.value();

		if (command.command.isEmpty())
		{
			continue;
		}

		QDomElement child=doc.createElement("command");

		child.setAttribute("kind", SYNC_COMMANDS_COMMAND_KIND);
		child.setAttribute("id", iter.key());
		child.setAttribute("enabled", command.enabled? "true":"false");
		child.setAttribute("name", command.command);
		for (int j=0; j<command.params.size(); j++)
		{
			child.setAttribute(QString("param%1").arg(j), command.params[j]);
		}


		saveFiles(doc, child, command.files);

		parent.appendChild(child);
	}
}

void QWriteCommandsToXmlFile::saveFiles(QDomDocument& doc, QDomElement& parent, const QVector<SyncMonitorFile>& files)
{
	for (int i=0; i<files.size(); i++)
	{
		QDomElement child=doc.createElement("file");

		SyncMonitorFile file=files[i];

		file.file=entryInfo(QFileInfo(appendPath(file.path, file.file.name)), false);

		child.setAttribute("path", file.path);
		child.setAttribute("name", file.file.name);
		child.setAttribute("size", file.file.size);
		child.setAttribute("type", file.file.type);
		child.setAttribute("executable", file.file.isExecutable? "true":"false");
		child.setAttribute("modified", file.file.lastModified.toString(STANDARD_DATETIME_FORMAT));
		child.setAttribute("enabled", file.enabled? "true":"false");

		parent.appendChild(child);
	}
}

QDomElement QWriteCommandsToXmlFile::findChildElement(QDomDocument& doc, QDomElement& parent, const QString& tag)
{
	QDomElement child = parent.firstChildElement();
	while (!child.isNull())
	{
		if (child.tagName() == tag)
		{
			return child;
		}

		child = child.nextSiblingElement();
	}

	child=doc.createElement(tag);
	parent.appendChild(child);

	return child;
}

QReadMergeFromXmlFile::QReadMergeFromXmlFile(const QString& filename, const QString& suffix)
:QAbstractReadXmlFile(filename)
,m_strSuffix(suffix)
{

}

QReadMergeFromXmlFile::~QReadMergeFromXmlFile()
{

}

void QReadMergeFromXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "merge", readMergeNode);
}

void QReadMergeFromXmlFile::readMergeNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "lock", readLock);
}

void QReadMergeFromXmlFile::readLock(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement() && reader.name() == "lock");

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("suffix") && attributes.hasAttribute("indexes"))
	{
		QString strSuffix=attributes.value("suffix").toString();
		if (strSuffix == m_strSuffix)
		{
			QStringList indexes=attributes.value("indexes").toString().split(QChar(','));
			foreach (QString index, indexes)
			{
				bool bOk;
				int nIndex=index.toInt(&bOk);
				if (bOk)
				{
					m_indexes.append(nIndex);
				}
			}
		}
	}

	readItemFinished(reader);
}

QWriteMergeToXmlFile::QWriteMergeToXmlFile(const QString& filename, const QString& suffix, const QVector<int>& indexes)
:QAbstractWriteXmlFile(filename)
,m_strSuffix(suffix)
,m_indexes(indexes)
{

}

QWriteMergeToXmlFile::~QWriteMergeToXmlFile()
{

}

void QWriteMergeToXmlFile::save(QDomDocument& doc, QDomElement& parent)
{
	QDomElement child = parent.firstChildElement("lock");
	while (!child.isNull())
	{
		if (child.hasAttribute("suffix") && child.attribute("suffix") == m_strSuffix)
		{
			break;
		}

		child = child.nextSiblingElement("lock");
	}

	if (child.isNull())
	{
		child=doc.createElement("lock");
		parent.appendChild(child);
	}

	QString strIndexes;
	for (int i=0; i<m_indexes.size(); i++)
	{
		if (i<m_indexes.size()-1)
		{
			strIndexes+=QString("%1,").arg(m_indexes[i]);
		}
		else
		{
			strIndexes+=QString("%1").arg(m_indexes[i]);
		}
	}

	child.setAttribute("indexes", strIndexes);
	child.setAttribute("suffix", m_strSuffix);
}

