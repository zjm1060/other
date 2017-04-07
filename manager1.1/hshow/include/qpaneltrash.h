#ifndef QPANELTRASH_H
#define QPANELTRASH_H

#include "qabstractpanel.h"

class QToolButton;

class QPanelTrash : public QAbstractPanel
{
	Q_OBJECT

public:
	QPanelTrash(QWidget *parent=0);
	~QPanelTrash();

public:
	virtual int belongsTo()	{ return PanelSystem; }
	virtual int sequence()	{ return TrashPanel; }

public slots:
	void onClickedBtn();

private:
	QToolButton*	m_pButton;
};

#endif // QPANELTRASH_H
