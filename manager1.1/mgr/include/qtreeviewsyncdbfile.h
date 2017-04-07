#ifndef QTREEVIEWSYNCDBFILE_H
#define QTREEVIEWSYNCDBFILE_H

#include "qtreeviewsyncconffile.h"

class QTreeViewSyncDbFile : public QTreeViewSyncConfFile
{
	Q_OBJECT

public:
	QTreeViewSyncDbFile(QWidget* parent=0);
	~QTreeViewSyncDbFile();

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

class QTreeViewSyncDbFileEx : public QTreeViewSyncCommit
{
	Q_OBJECT

public:
	QTreeViewSyncDbFileEx(QWidget* parent=0);
	~QTreeViewSyncDbFileEx();

public:
	virtual QString record() const
	{
		return "database";
	}

	virtual QString commonFolder() const
	{
		return H9000_DATABASE_PATH;
	}

protected:
	void checkDiff();
};

class QSyncDbFileRunnable : public QTreeViewSyncCommitRunnable
{
	Q_OBJECT

public:
	QSyncDbFileRunnable(const QString& workspace, const QString& history, const QString& record, bool checkable, bool enableRestrict, QObject* parent=0);
	virtual ~QSyncDbFileRunnable();

protected:
	virtual void check();

private:
	void readCommonFiles();
	void readProjectFiles();
};

#endif // QTREEVIEWSYNCFILE_H
