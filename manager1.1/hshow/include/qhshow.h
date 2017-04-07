#ifndef QHSHOW_H
#define QHSHOW_H

#include <QDockWidget>

#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
class QHBoxLayout;
class QSignalMapper;
class QMenu;
class QMouseEvent;
QT_END_NAMESPACE

class QPanelTools;

class QHShow : public QDockWidget
{
	Q_OBJECT

public:
	QHShow(const QRect& geometry, QWidget *parent=0);
	~QHShow();

public:
	static QHShow*	createWidget(const QRect& geometry);

protected:
	virtual void paintEvent(QPaintEvent* event);

	virtual void enterEvent(QEvent* event);
	virtual void leaveEvent(QEvent* event);

	virtual void mousePressEvent(QMouseEvent* event);

private slots:
	void actionTriggered(int index);

	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void focusChanged(QWidget* old, QWidget* now);

private:
	void setIcon(const QIcon& icon);
	void createActions();

	void showWidget();
	void hideWidget();

private:
	QPanelTools*	m_pPanel;

	QRect	m_geometry;

private:
	enum ActionType
	{
//		ActionConfigure,
		ActionExit,
		ActionEnd
	};

	QMenu*		m_pMenu;
	QAction*	m_pActions[ActionEnd];

	QSignalMapper*	m_pSignalMapper;

	QSystemTrayIcon*	m_pTrayIcon;

	bool		m_bFocus;		//»ñµÃ½¹µã
	bool		m_bShow;

private:
	static QVector<QHShow*>		g_sWidgets;
};

#endif // QHSHOW_H
