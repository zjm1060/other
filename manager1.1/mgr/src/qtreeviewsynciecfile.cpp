#include "qtreeviewsynciecfile.h"

QTreeViewSyncXLDIEC104File::QTreeViewSyncXLDIEC104File(QWidget *parent)
	: QTreeViewSyncConfFile(parent)
{
}

QTreeViewSyncXLDIEC104File::~QTreeViewSyncXLDIEC104File()
{
}

REGISTER_TREE_VIEW_SYNC_COMMIT_CLASS(QTreeViewSyncXLDIEC104File)

QTreeViewSyncXJBIEC104File::QTreeViewSyncXJBIEC104File(QWidget *parent)
: QTreeViewSyncConfFile(parent)
{
}

QTreeViewSyncXJBIEC104File::~QTreeViewSyncXJBIEC104File()
{
}

REGISTER_TREE_VIEW_SYNC_COMMIT_CLASS(QTreeViewSyncXJBIEC104File)
