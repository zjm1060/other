#ifndef QTREEVIEWUPDATEDATFILE_H
#define QTREEVIEWUPDATEDATFILE_H

#include "qtreeviewupdateconffile.h"

class QTreeViewUpdateDatFile : public QTreeViewUpdateConfFile
{
	Q_OBJECT

public:
	QTreeViewUpdateDatFile(QWidget *parent=0);
	~QTreeViewUpdateDatFile();

public:
	virtual QString record() const
	{
		return "dat";
	}

	virtual QString commonFolder() const
	{
		return H9000_DAT_PATH;
	}
};

#endif // QTREEVIEWUPDATEDATFILE_H
