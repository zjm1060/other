#include "qdlgmodifypassword.h"

#include "common.h"
#include "qframeauthority.h"

QDlgModifyPassword::QDlgModifyPassword(const QPixmap& icon, QWidget *parent/* =0 */)
:QDialog(parent)
{
	m_ui=new Ui::DialogChangePassword;
	m_ui->setupUi(this);

	setWindowIcon(QIcon(":/images/user_icon.png"));

	m_ui->labelIcon->setPixmap(icon);

	m_ui->labelInfoIcon->setPixmap(findPixmap(":/images/error.png"));
	m_ui->labelInfoIcon->hide();

	connect(m_ui->lineEditCurrentPassword, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));
	connect(m_ui->lineEditNewPassword, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));
	connect(m_ui->lineEditConfirm, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));

	connect(m_ui->lineEditCurrentPassword, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
	connect(m_ui->lineEditNewPassword, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
	connect(m_ui->lineEditConfirm, SIGNAL(editingFinished()), this, SLOT(editingFinished()));

	connect(m_ui->pushButtonAuthorities, SIGNAL(clicked()), this, SLOT(onClickedBtnAuthories()));

	editingFinished();
}

QDlgModifyPassword::~QDlgModifyPassword()
{
	SAFE_DELETE_CLASS(m_ui);
}

void QDlgModifyPassword::setUser(const PrivilegeUser& usr)
{
	m_user=usr;

	m_ui->lineEditUserName->setText(usr.name);
}

const PrivilegeUser QDlgModifyPassword::user() const
{
	PrivilegeUser usr=m_user;

	usr.passwd=m_ui->lineEditNewPassword->text();

	return usr;
}

void QDlgModifyPassword::textChanged(const QString& text)
{
	Q_UNUSED(text);

	QString strCurrentPassword=m_ui->lineEditCurrentPassword->text();
	if (strCurrentPassword != m_user.passwd)
	{
		m_ui->pushButtonOk->setEnabled(false);
		return;
	}

	QString strNewPassword=m_ui->lineEditNewPassword->text();
	if (strNewPassword.isEmpty() || strNewPassword == m_user.passwd)
	{
		m_ui->pushButtonOk->setEnabled(false);
		return;
	}

	QString strConfirm=m_ui->lineEditConfirm->text();
	if (strNewPassword == strConfirm)
	{
		m_ui->labelInfo->setText(tr("Update the user password."));
		m_ui->labelInfoIcon->hide();

		m_ui->pushButtonOk->setEnabled(true);
	}
	else
	{
		m_ui->pushButtonOk->setEnabled(false);
	}
}

void QDlgModifyPassword::editingFinished()
{
	QString strCurrentPassword=m_ui->lineEditCurrentPassword->text();
	QString strNewPassword=m_ui->lineEditNewPassword->text();
	QString strConfirm=m_ui->lineEditConfirm->text();

	if (!strCurrentPassword.isEmpty() && strCurrentPassword != m_user.passwd)
	{
		m_ui->labelInfo->setText(tr("The current password is not correct!"));
		m_ui->labelInfoIcon->show();
	}
	else if (!strCurrentPassword.isEmpty() && !strNewPassword.isEmpty() && strNewPassword == m_user.passwd)
	{
		m_ui->labelInfo->setText(tr("The old and new passwords can't the same!"));
		m_ui->labelInfoIcon->show();
	}
	else
	{
		if (strCurrentPassword.isEmpty() || strNewPassword.isEmpty() || strConfirm.isEmpty() || strNewPassword == strConfirm)
		{
			m_ui->labelInfo->setText(tr("Update the user password."));
			m_ui->labelInfoIcon->hide();
		}
		else
		{
			m_ui->labelInfo->setText(tr("The new password and the repeat password does not match!"));
			m_ui->labelInfoIcon->show();
		}
	}
}

void QDlgModifyPassword::onClickedBtnAuthories()
{
	QDialog dlg(this);

	dlg.setWindowTitle(tr("Authorities"));

	QFrameAuthority frame(&dlg);
	frame.setEditable(false);
	frame.setPrivileges(m_user.privileges);

	dlg.setModal(true);
	dlg.exec();
}
