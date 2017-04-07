#include "qpaneltrash.h"

#include <QHBoxLayout>
#include <QToolButton>

#include "common.h"
#include "qpanelfactory.h"

QPanelTrash::QPanelTrash(QWidget *parent)
	: QAbstractPanel(parent)
{
	m_pButton=new QToolButton;
	m_pButton->setIcon(QIcon(":/images/trash.png"));
	m_pButton->setText(tr("Trash"));
	m_pButton->setAutoRaise(true);
	m_pButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	setDefaultIconSize(m_pButton);

	m_pLayout->addWidget(m_pButton);

	connect(m_pButton, SIGNAL(clicked()), this, SLOT(onClickedBtn()));
}

QPanelTrash::~QPanelTrash()
{
}

void QPanelTrash::onClickedBtn()
{

}

REGISTER_PANEL_CLASS(QPanelTrash)
