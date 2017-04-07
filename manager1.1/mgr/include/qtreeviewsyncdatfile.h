#ifndef QTREEVIEWSYNCDATFILE_H
#define QTREEVIEWSYNCDATFILE_H

#include <QSortFilterProxyModel>

#include "qtreeviewsyncconffile.h"

class QTreeViewSyncDatFile : public QTreeViewSyncConfFile
{
	Q_OBJECT

public:
	QTreeViewSyncDatFile(QWidget* parent=0);
	~QTreeViewSyncDatFile();

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

#endif // QTREEVIEWSYNCDATFILE_H
