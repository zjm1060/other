#ifndef QTREEVIEWUPDATEIECFILE_H
#define QTREEVIEWUPDATEIECFILE_H

#include "qtreeviewupdateconffile.h"

class QTreeViewUpdateXLDIEC104File : public QTreeViewUpdateConfFile
{
	Q_OBJECT

public:
	QTreeViewUpdateXLDIEC104File(QWidget *parent=0);
	~QTreeViewUpdateXLDIEC104File();

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

class QTreeViewUpdateXJBIEC104File : public QTreeViewUpdateConfFile
{
	Q_OBJECT

public:
	QTreeViewUpdateXJBIEC104File(QWidget *parent=0);
	~QTreeViewUpdateXJBIEC104File();

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

#endif // QTREEVIEWUPDATEDATFILE_H
