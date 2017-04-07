#ifndef QPANELCOMPUTER_H
#define QPANELCOMPUTER_H

#include "qabstractpanel.h"
#include "qabstractreceiver.h"

class QToolButton;

class QPanelComputer : public QAbstractPanel, public QAbstractReceiver
{
	Q_OBJECT

public:
	QPanelComputer(QWidget *parent=0);
	~QPanelComputer();

public:
	virtual int belongsTo()	{ return PanelSystem; }
	virtual int sequence()	{ return ComputerPanel; }

public slots:
	void onClickedBtn();

private:
	QToolButton*	m_pButton;
};

#endif // QPANELCOMPUTER_H
