#ifndef _Login_H
#define _Login_H

#include "ui_UserLogin.h"
#include "ui_ChangePassword.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"
#include "/home/ems/system/ots/SimuTutor/src/studentinfo.h"

class CLogin:public QDialog,public Ui_Login
{
    Q_OBJECT	
public:
	CLogin(QWidget *parent=0);
	int login_user_id;
	QString login_user_name;
	QString login_user_longname;
	QString login_user_password;
	QString login_time;
	bool is_user_priv;
	bool login_status;
	void init();
private:
	CStudentInfoAddTable* studentinfoaddtable; 
	void readConfig();
	int SetObjCond();
Q_SIGNALS:
	void sigUserLogin(int user_id,const QString &user_name);
private Q_SLOTS:
	void openStudentInfoAddTable();
	void addUser();
	void studentLogin();
	int checkLoginInfo(const QString &name,const QString &password);
	void userLogin(int current_user_id);
};

class CChangePassword:public QDialog,public Ui_ChangePassword
{
	Q_OBJECT
public:
	CChangePassword(QWidget *parent=0);
private:
	QString password_old;
	QString password_new;
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


extern CLogin *loginUser;

#endif