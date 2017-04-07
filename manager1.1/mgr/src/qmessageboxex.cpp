#include "qmessageboxex.h"

#include <QGridLayout>

QMessageBoxEx::QMessageBoxEx(QWidget* parent/* =0 */)
:QMessageBox(parent)
{
	init();
}

QMessageBoxEx::QMessageBoxEx(Icon icon, const QString &title, const QString &text, StandardButtons buttons /* = NoButton */,
							 QWidget *parent /* = 0 */, Qt::WindowFlags flags /* = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint */)
							 :QMessageBox(icon, title, text, buttons, parent, flags)
{
	init();
}

QMessageBoxEx::~QMessageBoxEx()
{

}

void QMessageBoxEx::init()
{
	QGridLayout* pLayout=dynamic_cast<QGridLayout*>(layout());

	QWidget* pButtonBox=NULL;
#ifndef Q_WS_MAC
	pButtonBox=pLayout->itemAtPosition(2, 0)->widget();
#else
	pButtonBox=pLayout->itemAtPosition(3, 1)->widget();
#endif

	m_pCheckBox=new QCheckBox(tr("I know"));

#ifndef Q_WS_MAC
	pLayout->addWidget(m_pCheckBox, 2, 0, 1, 1, Qt::AlignVCenter);
	pLayout->addWidget(pButtonBox, 2, 1, 1, 1);
#else
	pLayout->addWidget(m_pCheckBox, 3, 0, 1, 1, Qt::AlignVCenter);
	pLayout->addWidget(pButtonBox, 3, 1, 1, 1);
#endif
}

void QMessageBoxEx::done(int r)
{
	m_pCheckBox->isChecked()? r|=CheckButton:r&=~CheckButton;

	QMessageBox::done(r);
}
