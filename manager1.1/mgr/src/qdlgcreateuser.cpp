#include "qdlgcreateuser.h"

#include "common.h"

QDlgCreateUser::QDlgCreateUser(const QPixmap& icon, QWidget *parent/* =0 */)
:QDialog(parent)
{
	m_ui=new Ui::DialogCreateUser;
	m_ui->setupUi(this);

	m_pFrameAuthority=new QFrameAuthority;
	m_ui->verticalLayoutAuthority->addWidget(m_pFrameAuthority);

	m_ui->labelIcon->setPixmap(icon);

	m_ui->labelInfoIcon->setPixmap(findPixmap(":/images/error.png"));
	m_ui->labelInfoIcon->hide();

	connect(m_ui->lineEditUserName, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));
	connect(m_ui->lineEditPassword, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));
	connect(m_ui->lineEditConfirm, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));

	connect(m_ui->lineEditPassword, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
	connect(m_ui->lineEditConfirm, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
}

QDlgCreateUser::~QDlgCreateUser()
{

}

const PrivilegeUser QDlgCreateUser::user() const
{
	PrivilegeUser usr;

	usr.name=m_ui->lineEditUserName->text();
	usr.passwd=m_ui->lineEditPassword->text();

	usr.privileges=m_pFrameAuthority->privileges();

	return usr;
}

void QDlgCreateUser::textChanged(const QString& text)
{
	Q_UNUSED(text);

	QString strUserName=m_ui->lineEditUserName->text();
	if (strUserName.isEmpty())
	{
		m_ui->pushButtonOk->setEnabled(false);
		return;
	}

	QString strPassword=m_ui->lineEditPassword->text();
	if (strPassword.isEmpty())
	{
		m_ui->pushButtonOk->setEnabled(false);
		return;
	}

	QString strConfirm=m_ui->lineEditConfirm->text();
	if (strPassword == strConfirm)
	{
		m_ui->labelInfo->setText(tr("Create a new user account."));
		m_ui->labelInfoIcon->hide();

		m_ui->pushButtonOk->setEnabled(true);
	}
	else
	{
		m_ui->pushButtonOk->setEnabled(false);
	}
}

void QDlgCreateUser::editingFinished()
{
	QString strPassword=m_ui->lineEditPassword->text();
	QString strConfirm=m_ui->lineEditConfirm->text();

	if (strConfirm.isEmpty() || strPassword.isEmpty() || strPassword == strConfirm)
	{
		m_ui->labelInfo->setText(tr("Create a new user account."));
		m_ui->labelInfoIcon->hide();
	}
	else
	{
		m_ui->labelInfo->setText(tr("The password and the repeat password does not match!"));
		m_ui->labelInfoIcon->show();
	}
}
