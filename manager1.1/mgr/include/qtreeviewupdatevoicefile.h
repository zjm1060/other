#ifndef QTREEVIEWUPDATEVOICEFILE_H
#define QTREEVIEWUPDATEVOICEFILE_H

#include "qtreeviewupdateconffile.h"

class QTreeViewUpdateVoiceFile : public QTreeViewUpdateConfFile
{
	Q_OBJECT

public:
	QTreeViewUpdateVoiceFile(QWidget *parent=0);
	~QTreeViewUpdateVoiceFile();

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

#endif // QTREEVIEWUPDATEVOICEFILE_H
