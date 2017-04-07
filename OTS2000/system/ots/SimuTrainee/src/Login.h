#ifndef _Login_H
#define _Login_H

#include "ui_UserLogin.h"

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"
#include "/home/ems/system/ots/SimuTutor/src/studentinfo.h"

#define STUDYMODESELECTED 0
#define EXAMMODESELECTED 1
#define EVALUEMODESELECTED 2

class CLogin:public QDialog,public Ui_Login
{
    Q_OBJECT	
public:
	CLogin(QWidget *parent=0);
//	int login_user_id;
	QString login_user_name;
	QString login_user_longname;
	QString login_user_password;
	QString login_time;
	bool	is_user_priv;
	bool	login_status;
	int		train_mode;//保存用户使用模式 -1:未选 0：学习训练模式 1：考试模式 2：鉴定模式
	void	init();
	void loadChinese();
	void loadEnglish();
protected:
	QPoint windowPos,mousePos,dPos;
	void	mousePressEvent(QMouseEvent *event);
	void	mouseMoveEvent(QMouseEvent *event);
	void showEvent(QShowEvent *);
private:
	CStudentInfoAddTable* studentinfoaddtable; 
	void	readConfig();
	int		SetObjCond();
	void	setmyStyle();
Q_SIGNALS:
	void	sigUserLogin(int user_id,const QString &user_name,int mode);

private Q_SLOTS:
	void	openStudentInfoAddTable();
	void	addUser();
	void	studentLogin();
	int		checkLoginInfo(const QString &name,const QString &password);
	void	userLogin(int current_user_id);
// 	void	on_button_study();
// 	void	on_button_exam();
// 	void	on_button_enter();
	void  on_exit();
};

extern CLogin *loginUser;

#endif