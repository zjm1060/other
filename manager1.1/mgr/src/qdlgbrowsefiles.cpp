#include "qdlgbrowsefiles.h"

#include "common.h"

QDlgBrowseFiles::QDlgBrowseFiles(const ServerInfo& info, const QString& initPath, const QString& subPath, QWidget *parent/* =0 */)
	: QDlgBrowseFolders(info, initPath, subPath, parent)
{
	setWindowTitle(tr("Browse files"));
	labelTitle->setText(tr("Browse files"));
}

QDlgBrowseFiles::~QDlgBrowseFiles()
{

}

QVector<FileInfo> QDlgBrowseFiles::selectedFiles()
{
	QVector<FileInfo> files;

	if (m_pFrameFileSystem != NULL)
	{
		QString strText;
		FileInfo file;

		strText=m_pFrameFileSystem->currentLocationText();

		QVector<FileInfo> items=m_pFrameFileSystem->selectedItems();
		for (int i=0; i<items.size(); i++)
		{
			const FileInfo& item=items[i];
			if (isFile(item.type))
			{
				files.append(item);
			}
		}
	}

	return files;
}
