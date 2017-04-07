#ifndef QDLGEDITFOLDER_H
#define QDLGEDITFOLDER_H

#include <QDialog>

#include "ui_editfolder.h"

using namespace Ui;

class QDlgEditFolder : public QDialog, public Ui::DialogEditFolder
{
	Q_OBJECT

public:
	QDlgEditFolder(const QPixmap& icon, const QString& folder, QWidget *parent=0);
	~QDlgEditFolder();

public:
	QString text();

protected:
	bool eventFilter(QObject* obj, QEvent* event);

private slots:
	void textChanged(const QString& text);

private:
	QString		m_strFolder;
};

#endif // QDLGEDITFOLDER_H
