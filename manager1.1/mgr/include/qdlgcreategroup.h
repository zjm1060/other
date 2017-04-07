#ifndef QDLGCREATEGROUP_H
#define QDLGCREATEGROUP_H

#include <QDialog>

#include "ui_creategroup.h"

class QDlgCreateGroup : public QDialog
{
	Q_OBJECT

public:
	QDlgCreateGroup(const QPixmap& icon, QWidget *parent=0);
	~QDlgCreateGroup();

public:
	const QString group() const;

private slots:
	void textChanged(const QString& text);

private:
	Ui::DialogCreateGroup*	m_ui;
};

#endif // QDLGCREATEGROUP_H
