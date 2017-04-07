#ifndef QDLGBROWSEFILES_H
#define QDLGBROWSEFILES_H

#include "qdlgbrowsefolders.h"

class QDlgBrowseFiles : public QDlgBrowseFolders
{
	Q_OBJECT

public:
	QDlgBrowseFiles(const ServerInfo& info, const QString& initPath, const QString& subPath, QWidget *parent=0);
	~QDlgBrowseFiles();

public:
	QVector<FileInfo> selectedFiles();
};

#endif // QDLGBROWSEFILES_H
