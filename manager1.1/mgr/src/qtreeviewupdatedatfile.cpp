#include "qtreeviewupdatedatfile.h"

QTreeViewUpdateDatFile::QTreeViewUpdateDatFile(QWidget *parent)
	: QTreeViewUpdateConfFile(parent)
{
}

QTreeViewUpdateDatFile::~QTreeViewUpdateDatFile()
{
}

REGISTER_TREE_VIEW_UPDATE_FILE_CLASS(QTreeViewUpdateDatFile)
