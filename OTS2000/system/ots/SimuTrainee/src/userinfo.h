#ifndef _USER_INFO_H_
#define _USER_INFO_H_

#include <Qt>
#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QSqlQuery>   
#include <QVariant>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QComboBox>

#include "ui_userinfo.h"
#include "ui_ChangePassword.h"
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"

class CChangePassword;
class UserInfo: public QWidget,public Ui_userinfo
{
	Q_OBJECT
public:
	UserInfo(QWidget *parent=0);
	~UserInfo();
	void loadChinese();
	void loadEnglish();
	void getuserinfo();
	void initgroupinfo();
	DATA_USERINFO u_seg,u_info;
	CChangePassword *d_CChangePassword;
private:
	QMenu* popMenu;
	void modifyUser();
protected:
	void showEvent(QShowEvent *event);
public slots:
	void resetPwd();
	int apply();
	void enter();
	void onclose();
	void setLanguage( int );
signals:
	void closedlg();
};

class CChangePassword:public QDialog,public Ui_ChangePassword
{
	Q_OBJECT
public:
	CChangePassword(QWidget *parent=0);
		QString password_old;
private:
	void loadChinese();
	void loadEnglish();
	QString password_new,paswordedited;
	QString password_new_confirm;
private slots:	
	void enter();
	void getOld(const QString &str);
	void getNew(const QString &str);
	void getNewConfirm(const QString &str);
protected:
	void showEvent(QShowEvent *);
signals:
	void sigUpdateOld(const QString &str);
	void sigUpdateNew(const QString &str);
	void sigUpdateNewConfirm(const QString &str);
};

#endif