#ifndef QUTILREADWRITEXMLFILE_H
#define QUTILREADWRITEXMLFILE_H

#include "global.h"
#include "qutildef.h"
#include "qabstractreadwritexmlfile.h"

class UTIL_EXPORT QReadCommandsFromXmlFile : public QAbstractReadXmlFile
{
public:
	QReadCommandsFromXmlFile(const QString& filename);
	~QReadCommandsFromXmlFile();

public:
	SyncCommands result();

protected:
	virtual void readNode(QXmlStreamReader& reader);

private:
	void readCommandsNode(QXmlStreamReader& reader);
	void readCommandNode(QXmlStreamReader& reader);
	void readFileNode(QXmlStreamReader& reader, QVector<SyncMonitorFile>& files);

private:
	SyncCommands	m_results;
};

class UTIL_EXPORT QWriteCommandsToXmlFile : public QAbstractWriteXmlFile
{
public:
	QWriteCommandsToXmlFile(const QString& filename, const SyncCommands& commands);
	~QWriteCommandsToXmlFile();

protected:
	virtual QString parentNodeName() const
	{
		return "commands";
	}

	virtual void save(QDomDocument& doc, QDomElement& parent);

private:
	QDomElement findChildElement(QDomDocument& doc, QDomElement& parent, const QString& tag);

	void saveFiles(QDomDocument& doc, QDomElement& parent, const QVector<SyncMonitorFile>& files);

private:
	SyncCommands		m_commands;
};

class UTIL_EXPORT QReadMergeFromXmlFile : public QAbstractReadXmlFile
{
public:
	QReadMergeFromXmlFile(const QString& filename, const QString& suffix);
	~QReadMergeFromXmlFile();

public:
	QVector<int>	result()
	{
		return m_indexes;
	}

protected:
	virtual void readNode(QXmlStreamReader& reader);

private:
	void readMergeNode(QXmlStreamReader& reader);
	void readLock(QXmlStreamReader& reader);

private:
	QString			m_strSuffix;
	QVector<int>	m_indexes;
};

class UTIL_EXPORT QWriteMergeToXmlFile : public QAbstractWriteXmlFile
{
public:
	QWriteMergeToXmlFile(const QString& filename, const QString& suffix, const QVector<int>& indexes);
	~QWriteMergeToXmlFile();

protected:
	virtual QString parentNodeName() const
	{
		return "merge";
	}

	virtual void save(QDomDocument& doc, QDomElement& parent);

private:
	QString			m_strSuffix;
	QVector<int>	m_indexes;
};

#endif // QUTILREADWRITEXMLFILE_H
