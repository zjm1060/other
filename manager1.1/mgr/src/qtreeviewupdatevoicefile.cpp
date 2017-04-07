#include "qtreeviewupdatevoicefile.h"

QTreeViewUpdateVoiceFile::QTreeViewUpdateVoiceFile(QWidget *parent)
	: QTreeViewUpdateConfFile(parent)
{
}

QTreeViewUpdateVoiceFile::~QTreeViewUpdateVoiceFile()
{
}

REGISTER_TREE_VIEW_UPDATE_FILE_CLASS(QTreeViewUpdateVoiceFile)
