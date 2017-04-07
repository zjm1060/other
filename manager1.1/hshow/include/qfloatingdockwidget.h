#ifndef QFLOATINGDOCKWIDGET_H
#define QFLOATINGDOCKWIDGET_H

#include <QFrame>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
QT_END_NAMESPACE

class QFloatingDockWidget : public QFrame
{
	Q_OBJECT

public:
	QFloatingDockWidget(QWidget *parent=0);
	~QFloatingDockWidget();

public:
	void setWidget(QWidget* w);

private:
	QVBoxLayout*	m_pLayout;
};

#endif // QFLOATINGDOCKWIDGET_H
