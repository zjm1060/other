#ifndef QTREEVIEWUPDATERTDBFILE_H
#define QTREEVIEWUPDATERTDBFILE_H

#include "qtreeviewupdateconffile.h"

class QTreeViewUpdateRtdbFile : public QTreeViewUpdateConfFile
{
	Q_OBJECT

public:
	QTreeViewUpdateRtdbFile(QWidget *parent=0);
	~QTreeViewUpdateRtdbFile();

public:
	virtual QString record() const
	{
		return "dat";
	}

	virtual QString commonFolder() const
	{
		return H9000_DAT_PATH;
	}

public:
	virtual void checkModified(const QString& workspace, const QString& hostname, const QString& address);

protected:
	virtual void check();
};

class QUpdateRtdbFileRunnable : public QUpdateFileRunnable
{
	Q_OBJECT

public:
	QUpdateRtdbFileRunnable(const QString& workspace, const QString& catagory, 
		const QString& hostname, const QString& address, bool localLastModified, bool immediate=false, QObject* parent=0);
	virtual ~QUpdateRtdbFileRunnable();

protected:
	virtual bool readFiles(bool immediate);
};

#endif // QTREEVIEWUPDATERTDBFILE_H
