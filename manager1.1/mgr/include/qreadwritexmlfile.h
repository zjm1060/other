#ifndef QREADWRITEXMLFILE_H
#define QREADWRITEXMLFILE_H

#include <QXmlStreamReader>
#include <QDomDocument>
#include <QMap>

#include "qabstractreadwritexmlfile.h"
#include "qfilecomparefactory.h"
#include "qabstractmonitor.h"

class QReadConfigureFormXmlFile : public QAbstractReadXmlFile
{
public:
	QReadConfigureFormXmlFile(const QString& filename);
	~QReadConfigureFormXmlFile();

public:
	QMap<QString, QString> result()
	{
		return m_configure;
	}

protected:
	virtual void readNode(QXmlStreamReader& reader);

private:
	void readConfigureNode(QXmlStreamReader& reader);
	void readConfNode(QXmlStreamReader& reader);

private:
	QMap<QString, QString>	m_configure;
};

class QWriteConfigureToXmlFile : public QAbstractWriteXmlFile
{
public:
	QWriteConfigureToXmlFile(const QString& filename, const QMap<QString, QString>& configure);
	~QWriteConfigureToXmlFile();

protected:
	virtual QString parentNodeName() const
	{
		return "configure";
	}

	virtual void save(QDomDocument& doc, QDomElement& parent);

private:
	QMap<QString, QString>	m_configure;
};

class QMonitorConfigureFile : public QAbstractMonitor
{
	Q_OBJECT

public:
	QMonitorConfigureFile(const QString& filename, QObject* watched, QObject* parent=0);
	~QMonitorConfigureFile();

public:
	virtual bool loadFile();
	void save(const QMap<QString, QString>& configure);

signals:
	void changed(const QMap<QString, QString>& configure);

private:
	QMap<QString, QString>	m_configure;
};

class QReadWorkspaceFromXmlFile : public QAbstractReadXmlFile
{
public:
	QReadWorkspaceFromXmlFile(const QString& filename);
	~QReadWorkspaceFromXmlFile();

public:
	QList<QString>	result()
	{
		return m_folders;
	}

protected:
	virtual void readNode(QXmlStreamReader& reader);

private:
	void readWorkspaceNode(QXmlStreamReader& reader);

private:
	QList<QString> m_folders;
};

class QWriteWorkspaceToXmlFile : public QAbstractWriteXmlFile
{
public:
	QWriteWorkspaceToXmlFile(const QString& filename, const QString& workspace);
	~QWriteWorkspaceToXmlFile();

protected:
	virtual QString parentNodeName() const
	{
		return "workspace";
	}

	virtual void save(QDomDocument& doc, QDomElement& parent);
	virtual bool check(const QDomElement& node) const;

private:
	QString		m_strWorkspace;
};

class QReadWorkspaceRecordsFromXmlFile : public QAbstractReadXmlFile
{
public:
	QReadWorkspaceRecordsFromXmlFile(const QString& filename, const QString& path);
	~QReadWorkspaceRecordsFromXmlFile();

public:
	QMap<QString, DiffFile>	result()
	{
		return m_files;
	}

protected:
	virtual void readNode(QXmlStreamReader& reader);

private:
	void readWorkspaceNode(QXmlStreamReader& reader);
	void readFile(QXmlStreamReader& reader);

private:
	QMap<QString, DiffFile>	m_files;

	QString		m_strPath;
};

class QWriteWorkspaceRecordsToXmlFile : public QAbstractWriteXmlFile
{
public:
	QWriteWorkspaceRecordsToXmlFile(const QString& filename, const QString& workspace, const QVector<DiffFile>& files);
	~QWriteWorkspaceRecordsToXmlFile();

protected:
	virtual QString parentNodeName() const
	{
		return "workspace";
	}

	virtual void save(QDomDocument& doc, QDomElement& parent);

	virtual bool check(const QDomElement& node) const;

private:
	void saveRecords(QDomDocument& doc, QDomElement& node, const DiffFile& file);

private:
	QString		m_strWorkspace;
	QVector<DiffFile>	m_files;
};

class QMonitorServersFile : public QAbstractMonitor
{
	Q_OBJECT

public:
	QMonitorServersFile(const QString& filename, QObject* watched, QObject* parent=0);
	~QMonitorServersFile();

public:
	virtual bool loadFile();
	void save(const QList<Server>& servers);

signals:
	void changed(const QList<Server>& servers);

private:
	QList<Server>	m_servers;
};

class QReadServersFromXmlFile : public QAbstractReadXmlFile
{
public:
	QReadServersFromXmlFile(const QString& filename);
	~QReadServersFromXmlFile();

public:
	QList<Server>	result()
	{
		return m_servers;
	}

protected:
	virtual void readNode(QXmlStreamReader& reader);

private:
	void readServersNode(QXmlStreamReader& reader);
	void readServer(QXmlStreamReader& reader);

private:
	QList<Server>	m_servers;
};

class QReadRequestRecordFormXmlFile : public QAbstractReadXmlFile
{
public:
	QReadRequestRecordFormXmlFile(const QString& filename);
	~QReadRequestRecordFormXmlFile();

public:
	QVector<DiffFile> result()
	{
		return m_results;
	}

protected:
	virtual void readNode(QXmlStreamReader& reader);

private:
	void readFolderNode(QXmlStreamReader& reader);
	void readFileNode(QXmlStreamReader& reader);

private:
	QVector<DiffFile>	m_results;
};

class QReadCheckRecordFromXmlFile : public QAbstractReadXmlFile		//读取本地记录的已检测过的提交的文件更新信息
{
public:
	QReadCheckRecordFromXmlFile(const QString& filename);
	~QReadCheckRecordFromXmlFile();

public:
	void result(int& revision, QMap<QString, DiffFile>& files)
	{
		revision=m_nRevision;
		files=m_files;
	}

protected:
	virtual void readNode(QXmlStreamReader& reader);

private:
	void readRecord(QXmlStreamReader& reader);
	void readFile(QXmlStreamReader& reader);

private:
	int		m_nRevision;
	QMap<QString, DiffFile>		m_files;			//本地记录的文件
};

class QWriteCheckRecordToXmlFile : public QAbstractWriteXmlFile		//写本地记录的已检测过的提交的文件更新信息
{
public:
	QWriteCheckRecordToXmlFile(const QString& filename, int revision, const QVector<DiffFile>& files);
	~QWriteCheckRecordToXmlFile();

protected:
	virtual QString parentNodeName() const
	{
		return "record";
	}

	virtual void save(QDomDocument& doc, QDomElement& parent);

private:
	int		m_nRevision;
	QVector<DiffFile>	m_files;
};

class QWriteServersToXmlFile : public QAbstractWriteXmlFile
{
public:
	QWriteServersToXmlFile(const QString& filename, const QList<Server>& servers);
	~QWriteServersToXmlFile();

protected:
	virtual QString parentNodeName() const
	{
		return "servers";
	}

	virtual void save(QDomDocument& doc, QDomElement& parent);

private:
	QList<Server>	m_servers;
};

QString parseFileNameFromRevision(const QString& filename);

#endif // QREADWRITEXMLFILE_H
