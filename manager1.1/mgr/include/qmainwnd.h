#ifndef QMAINWND_H
#define QMAINWND_H

#include <QMainWindow>

#include "qviewcontroller.h"
#include "qwndgraphics.h"

class QMainWnd : public QMainWindow
{
	Q_OBJECT

public:
	QMainWnd();
	~QMainWnd();

protected:
	virtual void closeEvent(QCloseEvent* event);

private:
	QViewController*	m_pView;

	QWndGraphics*	m_pWndGraphics;
};

#endif // QMAINWND_H
