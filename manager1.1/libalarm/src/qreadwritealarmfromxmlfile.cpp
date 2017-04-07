#include "qalarmh9000event.h"

#include "extern.h"
#include "qlogger.h"

QReadH9000FileSystemAlarmFormXmlFile::QReadH9000FileSystemAlarmFormXmlFile(const QString& filename)
:QAbstractReadXmlFile(filename)
{
	m_configure.lowLevel=0.85;
	m_configure.upperLevel=0.95;
}

QReadH9000FileSystemAlarmFormXmlFile::~QReadH9000FileSystemAlarmFormXmlFile()
{

}

void QReadH9000FileSystemAlarmFormXmlFile::readNode(QXmlStreamReader& reader)
{
	readNextNode(reader, "H9000", readH9000Node);
}

void QReadH9000FileSystemAlarmFormXmlFile::readH9000Node(QXmlStreamReader& reader)
{
	readNextNode(reader, "FileSystem", readFileSystemNode);
}

void QReadH9000FileSystemAlarmFormXmlFile::readFileSystemNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("L1"))
	{
		QString key=attributes.value("L1").toString();

		if (!key.isEmpty())
		{
			m_configure.upperLevel=key.toDouble();
		}
	}

	if (attributes.hasAttribute("L2"))
	{
		QString key=attributes.value("L2").toString();

		if (!key.isEmpty())
		{
			m_configure.lowLevel=key.toDouble();
		}
	}

	readNextNode(reader, "Partition", readPartitionNode);
}

void QReadH9000FileSystemAlarmFormXmlFile::readPartitionNode(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	QXmlStreamAttributes attributes=reader.attributes();

	if (attributes.hasAttribute("path") && attributes.hasAttribute("longname"))
	{
		QString strPath=attributes.value("path").toString();
		QString strLongname=attributes.value("longname").toString();

		if (!strPath.isEmpty() && !strLongname.isEmpty())
		{
			m_configure.partitions.insert(strPath, strLongname);
		}
	}

	readItemFinished(reader);
}

