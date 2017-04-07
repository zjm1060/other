#ifndef QPANELTOOLS_H
#define QPANELTOOLS_H

#include <QWidget>
#include <QHBoxLayout>

#include "qabstractpanel.h"

class QPanelTools : public QWidget
{
	Q_OBJECT

public:
	QPanelTools(QWidget *parent=0);
	~QPanelTools();

private:
	QHBoxLayout*	m_pLayout;

	QHBoxLayout*	m_pLayouts[PanelEnd];
};

#endif // QPANELTOOLS_H
