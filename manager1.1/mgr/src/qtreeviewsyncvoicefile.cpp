#include "qtreeviewsyncvoicefile.h"

QTreeViewSyncVoiceFile::QTreeViewSyncVoiceFile(QWidget *parent)
	: QTreeViewSyncConfFile(parent)
{
}

QTreeViewSyncVoiceFile::~QTreeViewSyncVoiceFile()
{
}

REGISTER_TREE_VIEW_SYNC_COMMIT_CLASS(QTreeViewSyncVoiceFile)
