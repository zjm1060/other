#ifndef QDLGMODIFYPASSWORD_H
#define QDLGMODIFYPASSWORD_H

#include <QDialog>

#include "ui_changepassword.h"
#include "qutildef.h"

class QDlgModifyPassword : public QDialog
{
	Q_OBJECT

public:
	QDlgModifyPassword(const QPixmap& icon, QWidget *parent=0);
	~QDlgModifyPassword();

public:
	void setUser(const PrivilegeUser& usr);
	const PrivilegeUser user() const;

private slots:
	void textChanged(const QString& text);
	void editingFinished();

	void onClickedBtnAuthories();

private:
	Ui::DialogChangePassword*	m_ui;

	PrivilegeUser	m_user;
};

#endif // QDLGMODIFYPASSWORD_H
