#include "qmessageboxlogout.h"

#define HMAN_LOGOUT_WAIT_INTERVAL	15
QMessageBoxLogout::QMessageBoxLogout(const QPixmap& icon, QWidget* parent /* = 0 */)
	: QMessageBox(parent)
	,m_nCount(HMAN_LOGOUT_WAIT_INTERVAL)
{
	setIconPixmap(icon);

	setWindowTitle(tr("Logout"));
	setText("<h2>"+tr("Are you sure to logout immediately?")+"</h2>");

	addButton(QMessageBox::Cancel);
	m_pButtonLogout=addButton(tr("&Logout"), QMessageBox::AcceptRole);
	setDefaultButton(m_pButtonLogout);

	m_timer.setInterval(1000);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	m_timer.start();
	timeout();
}

QMessageBoxLogout::~QMessageBoxLogout()
{

}

void QMessageBoxLogout::timeout()
{
	m_nCount--;

	if (m_nCount >= 0)
	{
		setInformativeText(tr("You will be logged out automatically in %1 seconds.").arg(m_nCount));
	}
	else
	{
		m_timer.stop();

		m_pButtonLogout->click();
	}
}
