#ifndef QFRAMEAUTHORITY_H
#define QFRAMEAUTHORITY_H

#include <QFrame>

#include "ui_authorities.h"
#include "qutildef.h"

class QFrameAuthority : public QFrame
{
	Q_OBJECT

public:
	QFrameAuthority(QWidget *parent=0);
	~QFrameAuthority();

public:
	void setEditable(bool enable);
	void setUserRoleEditable(bool enable);

	void setPrivileges(const QMap<int, quint32>& privileges);

	QMap<int, quint32> privileges() const ;

signals:
	void privilegesChanged(bool changed);

private slots:
	void checkChange();

private:
	Ui::FrameAuthorities*		m_ui;

	bool	m_bEditable;
	bool	m_bUserRoleEditable;
	QMap<int, quint32>	m_privileges;
};

#endif // QFRAMEAUTHORITY_H
