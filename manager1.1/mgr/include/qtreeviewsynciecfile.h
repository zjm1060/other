#ifndef QTREEVIEWSYNCIECFILE_H
#define QTREEVIEWSYNCIECFILE_H

#include <QSortFilterProxyModel>

#include "qtreeviewsyncconffile.h"

class QTreeViewSyncXLDIEC104File : public QTreeViewSyncConfFile
{
	Q_OBJECT

public:
	QTreeViewSyncXLDIEC104File(QWidget* parent=0);
	~QTreeViewSyncXLDIEC104File();

public:
	virtual QString record() const
	{
		return "iec104-xld";
	}

	virtual QString commonFolder() const
	{
		return H9000_IEC104_PATH;
	}
};

class QTreeViewSyncXJBIEC104File : public QTreeViewSyncConfFile
{
	Q_OBJECT

public:
	QTreeViewSyncXJBIEC104File(QWidget* parent=0);
	~QTreeViewSyncXJBIEC104File();

public:
	virtual QString record() const
	{
		return "iec104-xjb";
	}

	virtual QString commonFolder() const
	{
		return H9000_IEC104_PATH;
	}
};

#endif // QTREEVIEWSYNCIECFILE_H
