#include "qlogingraphicsscene.h"

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

QWidgetLogin::QWidgetLogin(QWidget* parent/* =NULL */)
:QWidget(parent)
{
	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_pWndLogin=new Ui::FormLogin;
	m_pWndLogin->setupUi(this);

	connect(m_pWndLogin->pushButtonReset, SIGNAL(clicked()), this, SLOT(reset()));
	connect(m_pWndLogin->pushButtonLogin, SIGNAL(clicked()), this, SLOT(login()));

	m_timer.setInterval(LOGIN_RECHECK_PRIVILEGE_INTERVAL);
	m_timer.setSingleShot(true);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(checkPrivilege()));
}

QWidgetLogin::~QWidgetLogin()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);

	SAFE_DELETE_CLASS(m_pWndLogin);
}

void QWidgetLogin::reset()
{
	m_pWndLogin->lineEditUserName->clear();
	m_pWndLogin->lineEditPassword->clear();

	setInputFocus();
}

void QWidgetLogin::login()
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

void QWidgetLogin::checkPrivilege()
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

void QWidgetLogin::setInputFocus()
{
	m_pWndLogin->lineEditUserName->raise();
	m_pWndLogin->lineEditUserName->activateWindow();
	m_pWndLogin->lineEditUserName->setFocus(Qt::MouseFocusReason);
}

void QWidgetLogin::keyPressEvent(QKeyEvent* event)
{
	if (!QGlobalInfo::instance()->isLogin() && (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter))
	{
		m_pWndLogin->pushButtonLogin->click();

		return;
	}

	QWidget::keyPressEvent(event);
}

void QWidgetLogin::recvCheckPrivilege(int flag, const PrivilegeUser& user)
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

			emit logged();
		}
		break;
	default:
		Q_ASSERT(false);
		break;
	}
}

void QWidgetLogin::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter p(this);
	p.fillRect(rect(), QColor(32, 32, 32));
}

QLoginGraphicsScene::QLoginGraphicsScene(QGraphicsItem *parent/* =0 */)
	: QGraphicsWidget(parent)
{
	m_pLayout=new QGraphicsLinearLayout(Qt::Horizontal);

	m_pWidgetLogo=new QLabel;
	QPalette palette=m_pWidgetLogo->palette();
	palette.setColor(QPalette::Background, QColor(32, 32, 32));
	m_pWidgetLogo->setPalette(palette);
	m_pWidgetLogo->setPixmap(findPixmap(":/images/login.png").scaled(LOGIN_PIXMAP_SIZE, LOGIN_PIXMAP_SIZE));

	m_pProxyWidgetLogo=new QGraphicsProxyWidget(this);
	m_pProxyWidgetLogo->setWidget(m_pWidgetLogo);

	m_pWidgetLogin=new QWidgetLogin;

	m_pProxyWidgetLogin=new QGraphicsProxyWidget(this);
	m_pProxyWidgetLogin->setWidget(m_pWidgetLogin);

	m_pLayout->addItem(m_pProxyWidgetLogo);
	m_pLayout->addItem(m_pProxyWidgetLogin);

	setLayout(m_pLayout);
}

QLoginGraphicsScene::~QLoginGraphicsScene()
{

}

void QLoginGraphicsScene::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_ASSERT_NO_NULL(painter);

	painter->save();

	painter->setPen(QColor(79, 181, 247));
	painter->setBrush(QBrush(QColor(128, 128, 128, 64)));

	QRectF rect=option->rect;
	painter->drawRoundedRect(rect, 10, 10);

	painter->restore();

	QGraphicsWidget::paint(painter, option, widget);
}

