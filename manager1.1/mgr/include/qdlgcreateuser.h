#ifndef QDLGCREATEUSER_H
#define QDLGCREATEUSER_H

#include <QDialog>

#include "ui_createuser.h"
#include "qutildef.h"
#include "qframeauthority.h"

class QDlgCreateUser : public QDialog
{
	Q_OBJECT

public:
	QDlgCreateUser(const QPixmap& icon, QWidget *parent=0);
	~QDlgCreateUser();

public:
	const PrivilegeUser user() const;

private slots:
	void textChanged(const QString& text);
	void editingFinished();

private:
	Ui::DialogCreateUser*	m_ui;
	QFrameAuthority*		m_pFrameAuthority;
};

#endif // QDLGCREATEUSER_H
