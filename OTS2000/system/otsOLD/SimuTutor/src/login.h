#ifndef _LOGIN_H_
#define _LOGIN_H_

#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QSqlQuery>   
#include <QVariant>

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "qcommon.h"
#include "ui_login.h"

class CLogin : public QDialog,public Ui_CLoginTable
{
	Q_OBJECT
	public:
		CLogin(QWidget *parent=0);
		void loadChinese();
		void loadEnglish();
		void init();
		int login_user_id;
		QString login_user_name;
		QString login_user_longname;
		QString login_user_password;
		QString login_time;
		bool login_status;
		int user_role;
	private:
		void userLogin(int);
		int checkLoginInfo(const QString &,const QString &);
	signals:
		void sigUserLogin(int user_id,const QString &user_name);
	private slots:
		void teacherLogin();
};

extern CLogin *loginUser;

#endif