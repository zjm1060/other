#include "qtreeviewsyncdatfile.h"

QTreeViewSyncDatFile::QTreeViewSyncDatFile(QWidget *parent)
	: QTreeViewSyncConfFile(parent)
{
}

QTreeViewSyncDatFile::~QTreeViewSyncDatFile()
{
}

REGISTER_TREE_VIEW_SYNC_COMMIT_CLASS(QTreeViewSyncDatFile)
