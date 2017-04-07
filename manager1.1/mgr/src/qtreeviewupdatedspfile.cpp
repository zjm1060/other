#include "qtreeviewupdatedspfile.h"

QTreeViewUpdateDspFile::QTreeViewUpdateDspFile(QWidget *parent)
	: QTreeViewUpdateConfFile(parent)
{
}

QTreeViewUpdateDspFile::~QTreeViewUpdateDspFile()
{
}

REGISTER_TREE_VIEW_UPDATE_FILE_CLASS(QTreeViewUpdateDspFile)
