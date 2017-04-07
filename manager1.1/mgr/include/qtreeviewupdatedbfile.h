#ifndef QTREEVIEWUPDATEDBFILE_H
#define QTREEVIEWUPDATEDBFILE_H

#include "qtreeviewupdateconffile.h"

class QTreeViewUpdateDbFile : public QTreeViewUpdateConfFile
{
	Q_OBJECT

public:
	QTreeViewUpdateDbFile(QWidget *parent=0);
	~QTreeViewUpdateDbFile();

public:
	virtual QString record() const
	{
		return "database";
	}

	virtual QString commonFolder() const
	{
		return H9000_DATABASE_PATH;
	}
};


class QTreeViewUpdateDbFileEx : public QTreeViewUpdateFile
{
	Q_OBJECT

public:
	QTreeViewUpdateDbFileEx(QWidget *parent=0);
	~QTreeViewUpdateDbFileEx();

public:
	virtual QString record() const
	{
		return "database";
	}

	virtual QString commonFolder() const
	{
		return H9000_DATABASE_PATH;
	}

public:
	virtual void checkModified(const QString& workspace, const QString& hostname, const QString& address);

protected:
	virtual void check();
};

class QUpdateDbFileRunnable : public QUpdateFileRunnable
{
	Q_OBJECT

public:
	QUpdateDbFileRunnable(const QString& workspace, const QString& catagory, 
		const QString& hostname, const QString& address, bool restrictFilter, bool localLastModified, bool immediate=false, QObject* parent=0);
	virtual ~QUpdateDbFileRunnable();

protected:
	virtual bool readFiles(bool immediate);

private:
	bool readCommonFiles(bool immediate);
	bool readProjectFiles(bool immediate);

	void setParentCheckedState(const QModelIndex& pindex);
};

#endif // QTREEVIEWUPDATEDBFILE_H
