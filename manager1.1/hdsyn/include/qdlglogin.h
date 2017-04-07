#ifndef QDLGLOGIN_H
#define QDLGLOGIN_H

#include <QDialog>
#include <QMutex>
#include <QTimer>

#include "ui_loginex.h"
#include "qabstractreceiver.h"
#include "qinfomanager.h"

class QDlgLogin : public QDialog, public QAbstractReceiver
{
	Q_OBJECT 

public:
	QDlgLogin(QWidget* parent=NULL);
	~QDlgLogin();

public slots:
	void login();
	void reset();

public:
	void setInputFocus();

	void setTitleText(const QString& title);
	void setDefaultDescText(const QString& desc);

	void setThemePixmap(const QPixmap& pixmap);
	void showTipsPixmap(bool show);

public:
	virtual void recvCheckPrivilege(int flag, const PrivilegeUser& user);

public slots:
	void appendTip(QInfoManager::InfoType type, const QString& text);

signals:
	void logged();

private slots:
	void checkPrivilege();

private:
	quint32		m_nUsrID;
	QTimer		m_timer;

	QString		m_strUserName;
	QString		m_strPassword;

	QString		m_strDefaultDesc;

	Ui::DialogLogin* m_pWndLogin;
};

#endif // QDLGLOGIN_H
