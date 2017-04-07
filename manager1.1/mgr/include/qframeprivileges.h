#ifndef QFRAMEPRIVILEGES_H
#define QFRAMEPRIVILEGES_H

#include <QFrame>

#include "ui_privileges.h"
#include "qutildef.h"
#include "qframeauthority.h"

class QFramePrivileges : public QFrame
{
	Q_OBJECT

public:
	QFramePrivileges(QWidget *parent=0);
	~QFramePrivileges();

public:
	void setPrivileges(const PrivilegeUser& usr);

	PrivilegeUser privileges() const ;

signals:
	void privilegesChanged(bool changed);

private slots:
	void checkChange();

private:
	Ui::FramePrivileges*		m_ui;
	QFrameAuthority*			m_pFrameAuthority;

	PrivilegeUser	m_user;
};

#endif // QFRAMEPRIVILEGES_H
