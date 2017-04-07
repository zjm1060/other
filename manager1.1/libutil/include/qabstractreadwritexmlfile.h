#ifndef QABSTRACTREADWRITEXMLFILE_H
#define QABSTRACTREADWRITEXMLFILE_H

#include <QXmlStreamReader>
#include <QDomDocument>
#include <QMap>
#include <QMutex>
#include <QPair>

#include "global.h"

class UTIL_EXPORT QAbstractReadXmlFile
{
public:
	QAbstractReadXmlFile(const QString& filename, const QString& root="root");
	~QAbstractReadXmlFile();

public:
	bool read();

	QString errorString() const;

#define readNextNode(reader, node, func)	\
	do	\
	{	\
		Q_ASSERT(reader.isStartElement());	\
		\
		while (!reader.atEnd())		\
		{	\
			reader.readNext();	\
			\
			if (reader.isEndElement())	\
			{	\
				break;	\
			}	\
			\
			if (reader.isStartElement())	\
			{	\
				if (reader.name() == node)	\
				{	\
					func(reader);	\
				}	\
				else	\
				{	\
					readUnknownElement(reader);	\
				}	\
			}	\
		}	\
	}while (false);

#define readNextNodeEx(reader, node, func, param)	\
	do	\
	{	\
		Q_ASSERT(reader.isStartElement());	\
		\
		while (!reader.atEnd())		\
		{	\
			reader.readNext();	\
			\
			if (reader.isEndElement())	\
			{	\
				break;	\
			}	\
			\
			if (reader.isStartElement())	\
			{	\
				if (reader.name() == node)	\
				{	\
					func(reader, param);	\
				}	\
				else	\
				{	\
					readUnknownElement(reader);	\
				}	\
			}	\
		}	\
	}while (false);

#define readAllNode(reader, func, param)	\
	do	\
	{	\
		Q_ASSERT(reader.isStartElement());	\
		\
		while (!reader.atEnd())		\
		{	\
			reader.readNext();	\
			\
			if (reader.isEndElement())	\
			{	\
				break;	\
			}	\
			\
			if (reader.isStartElement())	\
			{	\
				func(reader, param);	\
			}	\
		}	\
	}while (false);

#define readItemFinished(reader)	\
	while (!reader.atEnd())	\
	{	\
		reader.readNext();	\
		if (reader.isEndElement())	\
		{	\
			break;	\
		}	\
		\
		if (reader.isStartElement())	\
		{	\
			readUnknownElement(reader);	\
		}	\
	}

protected:
	void readUnknownElement(QXmlStreamReader& reader);

protected:
	virtual void readNode(QXmlStreamReader& reader)=0;

private:
	QString m_strFileName;
	QString	m_strRoot;

	QString	m_strError;
};

class UTIL_EXPORT QAbstractWriteXmlFile
{
public:
	QAbstractWriteXmlFile(const QString& filename, const QString& root="root");
	~QAbstractWriteXmlFile();

public:
	bool write(bool override=true);

	QString errorString() const;

protected:
	virtual QString parentNodeName() const=0; 
	virtual void save(QDomDocument& doc, QDomElement& parent)=0;

	virtual bool check(const QDomElement& node) const;		//当写xml节点不是覆盖模式时，用于查找合适的节点

private:
	QString m_strFileName;
	QString	m_strRoot;

	QString	m_strError;

private:
	static QMutex g_mutex;
	static QMap<QString, QPair<QMutex*, int> >	g_mapMutex;
};

#endif // QABSTRACTREADWRITEXMLFILE_H
