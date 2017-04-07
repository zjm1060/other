#include "qframeprivileges.h"

QFramePrivileges::QFramePrivileges(QWidget *parent)
	: QFrame(parent)
{
	m_ui=new Ui::FramePrivileges;
	m_ui->setupUi(this);

	m_pFrameAuthority=new QFrameAuthority;
	m_ui->verticalLayoutAuthorities->addWidget(m_pFrameAuthority);

	connect(m_pFrameAuthority, SIGNAL(privilegesChanged(bool)), this, SIGNAL(privilegesChanged(bool)));
}

QFramePrivileges::~QFramePrivileges()
{

}

void QFramePrivileges::setPrivileges(const PrivilegeUser& usr)
{
	Q_ASSERT_NO_NULL(m_ui);

	disconnect(m_ui->lineEditPassword, SIGNAL(textChanged(const QString&)), this, SLOT(checkChange()));

	m_user=usr;

	m_ui->lineEditUserName->setText(usr.name);
	m_ui->lineEditPassword->setText(usr.passwd);

	if (usr.name == HMAN_PRIVALEGE_DEFAULT_USER_NAME)
	{
		m_pFrameAuthority->setUserRoleEditable(false);
	}
	else
	{
		m_pFrameAuthority->setUserRoleEditable(true);
	}

	m_pFrameAuthority->setPrivileges(m_user.privileges);

	connect(m_ui->lineEditPassword, SIGNAL(textChanged(const QString&)), this, SLOT(checkChange()));
}

PrivilegeUser QFramePrivileges::privileges() const
{
	PrivilegeUser user;

	user.name=m_ui->lineEditUserName->text();
	user.passwd=m_ui->lineEditPassword->text();

	user.privileges=m_pFrameAuthority->privileges();
	return user;
}

void QFramePrivileges::checkChange()
{
	if (m_ui->lineEditPassword->text() != m_user.passwd)
	{
		emit privilegesChanged(true);
		return;
	}

	emit privilegesChanged(false);
}
