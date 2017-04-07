#ifndef QFRAMELOCALFILESYSTEM_H
#define QFRAMELOCALFILESYSTEM_H

#include <QDir>

#include "qframefilesystem.h"

class QFrameLocalFileSystem : public QFrameFileSystem
{
	Q_OBJECT

public:
	QFrameLocalFileSystem(QWidget *parent=0);
	~QFrameLocalFileSystem();

public:
	void refreshDrives();
	void refreshFileSystem();

	void setDefaultLocation(const QString& location);
	void setFilters(const QStringList& nameFilters, QDir::Filters filters = QDir::NoFilter);

public:
	QVector<TransferFileItem> selectedFileItems();
	QVector<FileInfo> selectedFileInfos();

	QString locationText();
	QString currentLocationText();

	bool canUpload();
	bool canDelete();
	bool canOpen();
	bool canRename();

	void doubleClicked();

	void gotoLocation(const QString& location);

public slots:
	void refresh();
	void rename();

	void editTextChangedLocation(const QString& location);

signals:
	void enterInvalidChar(bool invalid, const QChar& ch);
	void focusStatusChanged(bool has);
	void mouseRightButtonPress(QPoint pos);

private slots:
	void enterPressed(QString location);
	void clickedToolButton(int type);

	void backspaceKeyPressed();
	void refreshKeyPressed();
	void deleteKeyPressed();

	void locationCurrentIndexChanged(QString location);
	void doubleClicked(const QModelIndex& index);
	void dataChanged(QModelIndex index, QString old, QString now);

	void mouseRightButtonPressed(QPoint pos);

private:
	QString		m_strHome;		//homeĿ¼

	QStringList		m_nameFilters;
	QDir::Filters	m_filters;
};

#endif // QFRAMELOCALFILESYSTEM_H
