#include "qabstractreadwritexmlfile.h"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include "common.h"
#include "qdirex.h"
#include "qfileex.h"

QAbstractReadXmlFile::QAbstractReadXmlFile(const QString& filename, const QString& root)
:m_strFileName(filename)
,m_strRoot(root)
{

}

QAbstractReadXmlFile::~QAbstractReadXmlFile()
{

}

bool QAbstractReadXmlFile::read()
{
	if (m_strFileName.isEmpty())
	{
		m_strError=QObject::tr("The xml file name is empty!");
		return false;
	}

	QFile file(m_strFileName);
	if (!file.exists())
	{
		m_strError=QString(QObject::tr("The xml file \"%1\" is not existed!")).arg(m_strFileName);
		return false;
	}

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		m_strError=QString(QObject::tr("Failed to open the xml file \"%1\" !")).arg(m_strFileName);
		return false;
	}

	QXmlStreamReader reader(&file);
	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isStartElement())
		{
			if (reader.name() == m_strRoot && reader.attributes().value("version") == "1.0")
			{
				readNode(reader);
			}
			else
			{
				reader.raiseError(QObject::tr("The file is not an xml version 1.0 file."));
			}
		}
	}

	return true;
}

void QAbstractReadXmlFile::readUnknownElement(QXmlStreamReader& reader)
{
	Q_ASSERT(reader.isStartElement());

	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isEndElement())
			break;

		if (reader.isStartElement())
		{
			readUnknownElement(reader);
		}
	}
}

QMap<QString, QPair<QMutex*, int> >	QAbstractWriteXmlFile::g_mapMutex;
QMutex QAbstractWriteXmlFile::g_mutex;

QAbstractWriteXmlFile::QAbstractWriteXmlFile(const QString& filename, const QString& root/* = */)
:m_strFileName(filename)
,m_strRoot(root)
{
	QMutexLocker locker(&g_mutex);

	if (!g_mapMutex.contains(m_strFileName))
	{
		QPair<QMutex*, int> peer;

		peer.first=new QMutex;
		peer.second=1;

		g_mapMutex.insert(m_strFileName, peer);
	}
	else
	{
		g_mapMutex[filename].second++;
	}
}

QAbstractWriteXmlFile::~QAbstractWriteXmlFile()
{
	QMutexLocker locker(&g_mutex);

	if (g_mapMutex.contains(m_strFileName))
	{
		QPair<QMutex*, int>& peer=g_mapMutex[m_strFileName];
		peer.second--;
		if (peer.second == 0)
		{
			SAFE_DELETE_CLASS(peer.first);
			g_mapMutex.remove(m_strFileName);
		}
	}
}

QString QAbstractWriteXmlFile::errorString() const
{
	return m_strError;
}

bool QAbstractWriteXmlFile::check(const QDomElement& node) const
{
	Q_UNUSED(node);

	return true;
}

bool QAbstractWriteXmlFile::write(bool override)
{
	if (m_strFileName.isEmpty())
	{
		m_strError=QObject::tr("The xml file name is empty!");
		return false;
	}

	QFileInfo info(m_strFileName);
	QDirEx dir;
	QString strPath=info.absolutePath();
	if (!dir.exists(strPath))
	{
		dir.mkpath(strPath);
	}

	QMutex* pMutex=g_mapMutex[m_strFileName].first;

	QMutexLocker locker(pMutex);

	QFileEx file(m_strFileName);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		m_strError=QString(QObject::tr("Failed to open the xml file \"%1\" !")).arg(m_strFileName);
		return false;
	}

	QDomDocument doc;

	QDomElement root;
	QDomElement child;
	if (doc.setContent(&file))
	{
		//原文件是合法的xml文件，修改xml文件
		root = doc.documentElement();
		if (root.tagName() == "root")
		{
			if (root.hasAttribute("version") && root.attribute("version") == "1.0")
			{
				child = root.firstChildElement(parentNodeName());
				if (override)
				{
					while (!child.isNull())
					{
						QDomElement node=child;

						child = child.nextSiblingElement(parentNodeName());

						root.removeChild(node);
					}
				}
				else
				{
					while (!child.isNull())
					{
						if (check(child))
						{
							break;
						}

						child = child.nextSiblingElement(parentNodeName());
					}
				}
			}
		}
		else
		{
			//文件格式不符合
			QDomProcessingInstruction instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
			doc.insertBefore(instruction, root);

			root=doc.createElement("root");
			root.setAttribute("version", "1.0");
			doc.appendChild(root);
		}
	}
	else
	{
		//原文件不是合法的xml文件，重新生成xml文件
		QDomProcessingInstruction instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
		doc.insertBefore(instruction, root);

		root=doc.createElement("root");
		root.setAttribute("version", "1.0");
		doc.appendChild(root);
	}

	if (child.isNull())
	{
		child=doc.createElement(parentNodeName());
		root.appendChild(child);
	}

	save(doc, child);

	file.resize(0);

	const int IndentSize = 4;

	QTextStream out(&file);
	doc.save(out, IndentSize, QDomNode::EncodingFromDocument);

	file.close();

	return true;
}
