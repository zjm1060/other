#include "qfloatingdockwidget.h"

#include <QVBoxLayout>

QFloatingDockWidget::QFloatingDockWidget(QWidget *parent)
	: QFrame(parent)
{
	setWindowFlags(Qt::Tool);

	m_pLayout=new QVBoxLayout;
	m_pLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(m_pLayout);
}

QFloatingDockWidget::~QFloatingDockWidget()
{

}

void QFloatingDockWidget::setWidget(QWidget* w)
{
	if (m_pLayout != NULL && w != NULL)
	{
		w->setParent(this);
		m_pLayout->addWidget(w);
	}
}

