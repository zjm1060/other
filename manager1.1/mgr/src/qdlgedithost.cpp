#include "qdlgedithost.h"

#include <QHostAddress>

#include "common.h"

QDlgEditHost::QDlgEditHost(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	quint16 nPort=(quint16)parseConfig(getAppParentDir()+INI_FILE, "network/serverPort", 2300).toInt();

	QRegExp rx("^((\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])(\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])){3})$");
	m_pValidatorAddr=new QRegExpValidator(rx, this);

	lineEditAddress->setValidator(m_pValidatorAddr);

	m_pValidatorPort=new QIntValidator(0, (1<<16)-1, this);
	lineEditPort->setValidator(m_pValidatorPort);
	lineEditPort->setText(QString("%1").arg(nPort));

	connect(lineEditAddress, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));
	connect(lineEditPort, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&)));
}

QDlgEditHost::~QDlgEditHost()
{
}

void QDlgEditHost::textChanged(const QString& text)
{
	Q_UNUSED(text);

	QString strAddr=lineEditAddress->text();
	if (m_pValidatorAddr->regExp().exactMatch(strAddr) && !lineEditPort->text().isEmpty())
	{
		pushButtonAdd->setEnabled(true);
	}
	else
	{
		pushButtonAdd->setEnabled(false);
	}
}

Server QDlgEditHost::host()
{
	Server server;

	server.addr=lineEditAddress->text();
	server.netmask=QHostAddress("255.255.255.0").toIPv4Address();
	server.port=(quint16)lineEditPort->text().toInt();

	return server;
}
