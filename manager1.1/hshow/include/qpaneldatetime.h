#ifndef QPANELDATETIME_H
#define QPANELDATETIME_H

#include <QTimer>

#include "qabstractpanel.h"

class QLinkButton;

class QPanelDateTime : public QAbstractPanel
{
	Q_OBJECT

public:
	QPanelDateTime(QWidget *parent=0);
	~QPanelDateTime();

public:
	virtual int belongsTo()	{ return PanelSystem; }
	virtual int sequence()	{ return DataTimePanel; }

private slots:
	void timeout();

private:
	QLinkButton*	m_pButton;

	QTimer	m_timer;
};

#endif // QPANELDATETIME_H
