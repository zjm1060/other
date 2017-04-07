#ifndef QMESSAGEBOXEX_H
#define QMESSAGEBOXEX_H

#include <QMessageBox>
#include <QCheckBox>

enum ButtonEx
{
	CheckButton=0x80000000
};

class QMessageBoxEx : public QMessageBox
{
	Q_OBJECT

public:
	QMessageBoxEx(QWidget* parent=0);
	QMessageBoxEx(Icon icon, const QString &title, const QString &text, StandardButtons buttons = NoButton, QWidget *parent = 0,
		Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
	~QMessageBoxEx();

public:
	virtual void done(int r);

private:
	void init();

private:
	QCheckBox*	m_pCheckBox;
};

#endif // QMESSAGEBOXEX_H
