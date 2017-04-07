#ifndef QLOGINGRAPHICSSCENE_H
#define QLOGINGRAPHICSSCENE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QMutex>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QTimer>

#include "ui_login.h"
#include "qabstractreceiver.h"

class QWidgetLogin : public QWidget, public QAbstractReceiver
{
	Q_OBJECT 

public:
	QWidgetLogin(QWidget* parent=NULL);
	~QWidgetLogin();

public slots:
	void login();
	void reset();

public:
	void setInputFocus();

public:
	virtual void recvCheckPrivilege(int flag, const PrivilegeUser& user);

	virtual void keyPressEvent(QKeyEvent* event);

signals:
	void logged();

protected:
	virtual void paintEvent(QPaintEvent* event);

private slots:
	void checkPrivilege();

private:
	quint32		m_nUsrID;
	QTimer		m_timer;

	QString		m_strUserName;
	QString		m_strPassword;

	Ui::FormLogin* m_pWndLogin;
};

class QLoginGraphicsScene : public QGraphicsWidget
{
public:
	QLoginGraphicsScene(QGraphicsItem *parent=0);
	~QLoginGraphicsScene();

public:
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget=0);

public:
	QWidgetLogin*	m_pWidgetLogin;

private:
	QGraphicsLinearLayout*	m_pLayout;

	QLabel*			m_pWidgetLogo;
	QGraphicsProxyWidget*	m_pProxyWidgetLogo;

	QGraphicsProxyWidget*	m_pProxyWidgetLogin;
};

#endif // QLOGINGRAPHICSSCENE_H
