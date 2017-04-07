#include "qpanelcomputer.h"

#include <QHBoxLayout>
#include <QToolButton>

#include "common.h"
#include "qpanelfactory.h"

QPanelComputer::QPanelComputer(QWidget *parent)
	: QAbstractPanel(parent)
{
	m_pButton=new QToolButton;
	m_pButton->setIcon(QIcon(":/images/computer.png"));
	m_pButton->setText(tr("Computer"));
	m_pButton->setAutoRaise(true);
	m_pButton->setToolButtonStyle(Qt::ToolButtonIconOnly);

	setDefaultIconSize(m_pButton);

	m_pLayout->addWidget(m_pButton);

	connect(m_pButton, SIGNAL(clicked()), this, SLOT(onClickedBtn()));
}

QPanelComputer::~QPanelComputer()
{
}

void QPanelComputer::onClickedBtn()
{

}

REGISTER_PANEL_CLASS(QPanelComputer)
