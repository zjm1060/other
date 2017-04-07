#ifndef QDLGBROWSEFOLDER_H
#define QDLGBROWSEFOLDER_H

#include <QDialog>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>

#include "ui_browsefolders.h"
#include "qimessage.h"
#include "qservermanager.h"
#include "qframeremotefilesystem.h"

using namespace Ui;

class QDlgBrowseFolders : public QDialog, public Ui::DialogBrowseFolders
{
	Q_OBJECT

public:
	QDlgBrowseFolders(const ServerInfo& info, const QString& initPath, const QString& subPath, QWidget *parent=0);
	~QDlgBrowseFolders();

public:
	QString text();
	QString folderPath();

public:
	void setFilters(const QStringList& nameFilters, QDir::Filters filters = QDir::NoFilter);

private slots:
	void enterInvalidChar(bool invalid, const QChar& ch);
	void mouseRightButtonPress(QPoint pos);
	void focusStatusChanged(bool has);

	void actionTriggered(int index);

	void tip(int type, const QString& tip);

protected:
	QFrameRemoteFileSystem*		m_pFrameFileSystem;

private:
	enum ActionType
	{
		ActionOpen,
		ActionDelete,
		ActionRefresh,
		ActionNewDirectory,
		ActionRename,
		ActionEnd
	};

	QMenu*		m_pMenu;
	QAction*	m_pActions[ActionEnd];
	QSignalMapper*	m_pSignalMapper;

	QPixmap		m_pixmap[QFrameRemoteFileSystem::TipEnd];
};

#endif // QDLGBROWSEFOLDER_H
