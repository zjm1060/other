#ifndef QDLGEDITHOST_H
#define QDLGEDITHOST_H

#include <QDialog>
#include <QRegExpValidator>
#include <QIntValidator>

#include "ui_edithost.h"
#include "qutildef.h"

using namespace Ui;

class QDlgEditHost : public QDialog, public Ui::DialogEditHost
{
	Q_OBJECT

public:
	QDlgEditHost(QWidget *parent=0);
	~QDlgEditHost();

public:
	Server host();

private slots:
	void textChanged(const QString& text);

private:
	QRegExpValidator*	m_pValidatorAddr;
	QIntValidator*		m_pValidatorPort;
};

#endif // QDLGEDITHOST_H
