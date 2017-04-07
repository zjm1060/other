#ifndef QTREEVIEWSYNCDSPFILE_H
#define QTREEVIEWSYNCDSPFILE_H

#include <QSortFilterProxyModel>

#include "qtreeviewsyncconffile.h"

class QTreeViewSyncDspFile : public QTreeViewSyncConfFile
{
	Q_OBJECT

public:
	QTreeViewSyncDspFile(QWidget* parent=0);
	~QTreeViewSyncDspFile();

public:
	virtual QString record() const
	{
		return "dsp";
	}

	virtual QString commonFolder() const
	{
		return H9000_DSP_PATH;
	}
};

#endif // QTREEVIEWSYNCDSPFILE_H
