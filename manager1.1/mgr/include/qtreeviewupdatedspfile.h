#ifndef QTREEVIEWUPDATEDSPFILE_H
#define QTREEVIEWUPDATEDSPFILE_H

#include "qtreeviewupdateconffile.h"

class QTreeViewUpdateDspFile : public QTreeViewUpdateConfFile
{
	Q_OBJECT

public:
	QTreeViewUpdateDspFile(QWidget *parent=0);
	~QTreeViewUpdateDspFile();

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

#endif // QTREEVIEWUPDATEDSPFILE_H
