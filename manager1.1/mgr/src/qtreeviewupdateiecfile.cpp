#include "qtreeviewupdateiecfile.h"

QTreeViewUpdateXLDIEC104File::QTreeViewUpdateXLDIEC104File(QWidget *parent)
	: QTreeViewUpdateConfFile(parent)
{
}

QTreeViewUpdateXLDIEC104File::~QTreeViewUpdateXLDIEC104File()
{
}

REGISTER_TREE_VIEW_UPDATE_FILE_CLASS(QTreeViewUpdateXLDIEC104File)

QTreeViewUpdateXJBIEC104File::QTreeViewUpdateXJBIEC104File(QWidget *parent)
: QTreeViewUpdateConfFile(parent)
{
}

QTreeViewUpdateXJBIEC104File::~QTreeViewUpdateXJBIEC104File()
{
}

REGISTER_TREE_VIEW_UPDATE_FILE_CLASS(QTreeViewUpdateXJBIEC104File)
