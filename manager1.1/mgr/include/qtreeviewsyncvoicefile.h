#ifndef QTREEVIEWSYNCVOICEFILE_H
#define QTREEVIEWSYNCVOICEFILE_H

#include <QSortFilterProxyModel>

#include "qtreeviewsyncconffile.h"

class QTreeViewSyncVoiceFile : public QTreeViewSyncConfFile
{
	Q_OBJECT

public:
	QTreeViewSyncVoiceFile(QWidget* parent=0);
	~QTreeViewSyncVoiceFile();

public:
	virtual QString record() const
	{
		return "wav";
	}

	virtual QString commonFolder() const
	{
		return H9000_VOICE_PATH;
	}
};

#endif // QTREEVIEWSYNCVOICEFILE_H
