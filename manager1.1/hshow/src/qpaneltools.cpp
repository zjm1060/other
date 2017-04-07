#include "qpaneltools.h"

#include <QToolButton>

#include "qpanelfactory.h"

QPanelTools::QPanelTools(QWidget *parent)
	: QWidget(parent)
{
	m_pLayout=new QHBoxLayout;
	m_pLayout->setMargin(0);
	setLayout(m_pLayout);

	QPanelFactory::instance()->load();

	for (int i=0; i<PanelEnd; i++)
	{
		m_pLayouts[i]=new QHBoxLayout;
		m_pLayouts[i]->setMargin(0);
		m_pLayout->addLayout(m_pLayouts[i]);

		QList<QAbstractPanel*> panels=QPanelFactory::instance()->produces(i);
		foreach (QAbstractPanel* panel, panels)
		{
			m_pLayouts[i]->addWidget(panel);
		}
	}

	m_pLayout->insertStretch(PanelSystem);
}

QPanelTools::~QPanelTools()
{

}
