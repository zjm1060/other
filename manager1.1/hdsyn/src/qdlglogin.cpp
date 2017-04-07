#include "qdlglogin.h"

#include <QLineEdit>

#include "common.h"
#include "qutildef.h"
#include "qreceiverparser.h"
#include "qinfomanager.h"
#include "qmessageparser.h"
#include "qprocesssocket.h"
#include "qglobalinfo.h"
#include "qndc.h"

#define LOGIN_PIXMAP_SIZE 96
#define LOGIN_RECHECK_PRIVILEGE_INTERVAL	5000

QDlgLogin::QDlgLogin(QWidget* parent/* =NULL */)
:QDialog(parent)
{
	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_pWndLogin=new Ui::DialogLogin;
	m_pWndLogin->setupUi(this);

	setWindowIcon(QIcon(":/images/login.png"));

	setTitleText(tr("Login"));
	setDefaultDescText(tr("Enter your account and password."));
	setThemePixmap(findPixmap(":/images/login.png"));
	showTipsPixmap(false);

	connect(m_pWndLogin->pushButtonReset, SIGNAL(clicked()), this, SLOT(reset()));
	connect(m_pWndLogin->pushButtonLogin, SIGNAL(clicked()), this, SLOT(login()));

	m_timer.setInterval(LOGIN_RECHECK_PRIVILEGE_INTERVAL);
	m_timer.setSingleShot(true);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(checkPrivilege()));
}

QDlgLogin::~QDlgLogin()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);

	SAFE_DELETE_CLASS(m_pWndLogin);
}

void QDlgLogin::reset()
{
	m_pWndLogin->lineEditUserName->clear();
	m_pWndLogin->lineEditPassword->clear();

	setInputFocus();
}

void QDlgLogin::login()
{
	QString strUsrName=m_pWndLogin->lineEditUserName->text();
	if (strUsrName.isEmpty())
	{
		QInfoManager::instance()->append(QInfoManager::Error, tr("Login failed! The user name cannot be empty!"));
		return;
	}

	QString strPassword=m_pWndLogin->lineEditPassword->text();
	if (strPassword.isEmpty())
	{
		QInfoManager::instance()->append(QInfoManager::Error, tr("Login failed! The password cannot be empty!"));
		return;
	}

	m_strUserName=strUsrName;
	m_strPassword=strPassword;

	checkPrivilege();
}

void QDlgLogin::checkPrivilege()
{
	QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_DDB);
	if (pInstance != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<m_strUserName<<m_strPassword;

		buffer=pInstance->query(QueryCheckPrivilege, m_nUsrID, buffer);

		QInfoManager::instance()->append(QInfoManager::Information, tr("User <%1> is logging in...").arg(m_strUserName));

		if (QProcessSocket::instance()->sendMessage(buffer, MSG_DDB))
		{
			m_pWndLogin->lineEditUserName->setEnabled(false);
			m_pWndLogin->lineEditPassword->setEnabled(false);
			m_pWndLogin->pushButtonReset->setEnabled(false);
			m_pWndLogin->pushButtonLogin->setEnabled(false);
		}
		else
		{
			QInfoManager::instance()->append(QInfoManager::Error, tr("Login failed! Make sure the service process is running!"));
		}
	}
}

void QDlgLogin::setInputFocus()
{
	m_pWndLogin->lineEditUserName->raise();
	m_pWndLogin->lineEditUserName->activateWindow();
	m_pWndLogin->lineEditUserName->setFocus(Qt::MouseFocusReason);
}

void QDlgLogin::setTitleText(const QString& title)
{
	m_pWndLogin->labelTitle->setText(title);
}

void QDlgLogin::setDefaultDescText(const QString& desc)
{
	m_strDefaultDesc=desc;

	m_pWndLogin->labelDesc->setText(desc);
}

void QDlgLogin::setThemePixmap(const QPixmap& icon)
{
	m_pWndLogin->labelIcon->setPixmap(icon.scaled(64, 64));
}

void QDlgLogin::showTipsPixmap(bool show)
{
	if (show)
	{
		m_pWndLogin->labelInfoIcon->show();
	}
	else
	{
		m_pWndLogin->labelInfoIcon->hide();
	}
}

void QDlgLogin::appendTip(QInfoManager::InfoType type, const QString& text)
{
	switch (type)
	{
	case QInfoManager::Warning:
		m_pWndLogin->labelInfoIcon->setPixmap(findPixmap(":/images/warning.png"));
		showTipsPixmap(true);
		m_pWndLogin->labelDesc->setText(text);
		break;
	case QInfoManager::Error:
		m_pWndLogin->labelInfoIcon->setPixmap(findPixmap(":/images/error.png"));
		showTipsPixmap(true);
		m_pWndLogin->labelDesc->setText(text);
		break;
	default:
//		m_pWndLogin->labelInfoIcon->setPixmap(findPixmap(":/images/information.png"));
		showTipsPixmap(false);

		m_pWndLogin->labelDesc->setText(text.isEmpty()? m_strDefaultDesc:text);

		break;
	}
}

void QDlgLogin::recvCheckPrivilege(int flag, const PrivilegeUser& user)
{
	if (flag != PrivilegeUncheckable)
	{
		m_pWndLogin->lineEditUserName->setEnabled(true);
		m_pWndLogin->lineEditPassword->setEnabled(true);
		m_pWndLogin->pushButtonReset->setEnabled(true);
		m_pWndLogin->pushButtonLogin->setEnabled(true);
	}

	switch (flag)
	{
	case PrivilegeUncheckable:
		{
			QInfoManager::instance()->append(QInfoManager::Information, 
				tr("The system is being initialized. Your logon request is denied, retry after %1 seconds").arg(LOGIN_RECHECK_PRIVILEGE_INTERVAL/1000));

			m_timer.start();
		}
		break;
	case PrivilegeUserNoExisted:
		{
			QInfoManager::instance()->append(QInfoManager::Error, tr("Login failed! Such user does not exist!"));

			m_pWndLogin->lineEditUserName->raise();
			m_pWndLogin->lineEditUserName->activateWindow();
			m_pWndLogin->lineEditUserName->setFocus(Qt::MouseFocusReason);

			m_pWndLogin->lineEditUserName->setSelection(0, m_pWndLogin->lineEditUserName->text().length());
		}
		break;
	case PrivilegePasswdError:
		{
			QInfoManager::instance()->append(QInfoManager::Error, tr("Login failed! The password you typed is incorrect!"));

			m_pWndLogin->lineEditPassword->raise();
			m_pWndLogin->lineEditPassword->activateWindow();
			m_pWndLogin->lineEditPassword->setFocus(Qt::MouseFocusReason);

			m_pWndLogin->lineEditPassword->setSelection(0, m_pWndLogin->lineEditPassword->text().length());
		}
		break;
	case PrivilegeCheckPass:
		{
			m_pWndLogin->lineEditPassword->clear();

			QInfoManager::instance()->append(QInfoManager::Important, tr("The user <%1> is logged on with %2 account.").arg(m_strUserName)
				.arg(checkPrivileges(user.privileges, PrivilegeRole, AdministratorRole)? tr("an administrator"):tr("a general")));

			QNdc::instance()->push(user.name);

			QGlobalInfo::instance()->setLogin(true);
			QGlobalInfo::instance()->setUser(user);

			accept();

			emit logged();
		}
		break;
	default:
		Q_ASSERT(false);
		break;
	}
}
