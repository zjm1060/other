#include "qpaneldatetime.h"

#include <QDateTime>
#include <QHBoxLayout>

#include "qlinkbutton.h"
#include "qpanelfactory.h"

QPanelDateTime::QPanelDateTime(QWidget *parent)
	: QAbstractPanel(parent)
{
	m_pButton=new QLinkButton;
	m_pButton->setReadOnly(true);
	m_pButton->setAlignment(Qt::AlignCenter);

	QFont ft=font();
	ft.setPointSize(9);
	ft.setBold(true);
	m_pButton->setFont(ft);

	m_pLayout->addWidget(m_pButton);

	m_timer.setInterval(1000);
	timeout();

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	m_timer.start();
}

QPanelDateTime::~QPanelDateTime()
{
	if (m_timer.isActive())
	{
		m_timer.stop();
	}
}

void QPanelDateTime::timeout()
{
	QDateTime time=QDateTime::currentDateTime();
	QString strText=time.toString(Qt::SystemLocaleLongDate);
	strText+="\n"+QDate::longDayName(time.date().dayOfWeek());

	m_pButton->setText(strText);
}

REGISTER_PANEL_CLASS(QPanelDateTime)
