#include "qtreeviewsyncdspfile.h"

QTreeViewSyncDspFile::QTreeViewSyncDspFile(QWidget *parent)
	: QTreeViewSyncConfFile(parent)
{
}

QTreeViewSyncDspFile::~QTreeViewSyncDspFile()
{
}

REGISTER_TREE_VIEW_SYNC_COMMIT_CLASS(QTreeViewSyncDspFile)
