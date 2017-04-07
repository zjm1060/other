#ifndef _SIMUTOOL_H
#define _SIMUTOOL_H
#include <QToolBox> 
#include <QToolButton> 
#include <QMouseEvent>
#include <QMenu>
#include "ui_SimuTool.h"
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"

class SimuTool:public QToolBox,Ui_Simutool
{
	Q_OBJECT
public:
	explicit SimuTool(QWidget *parent = 0);
	~SimuTool();
	bool mvwith;
	QPoint newpt;
	bool is_hiden;
	int cur_index;
	void moveEvent ( QMoveEvent * event );
	void loadChinese();
	void loadEnglish();
protected:
	void showEvent(QShowEvent *);
	void hideEvent ( QHideEvent * event );
private:	
	void init();
	void setButtonConnect();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
//vriables:
	QPoint windowPos;
	QPoint mousePos;
	QPoint dPos;
	QAction* actionHidetool;
	QAction* actionClosetool;
private slots:	
	void on_button_userinfo();
	void on_button_record();
	void on_button_userscore();
	void on_button_logout();
	void on_button_task();
	void on_button_revert();
	void on_button_localOprate();
	void on_button_openOix();
	void on_button_Exam();
	void on_button_simuControl();
	void on_button_setScene();
	void on_button_setFault();
	void on_button_sendMessage();
	void on_button_doccuments();
	void on_button_setDesplay();
	void on_button_help();
	void on_act_hide();
	void on_act_close();
	void on_menuList(const QPoint &pos);
	void MysetIndex( int index );
public slots:
	void setLanguage( int );
Q_SIGNALS:	
	void button_userinfo_clicked();
	void button_record_clicked();
	void button_userscore_clicked();
	void button_logout_clicked();
	void button_task_clicked();
	void button_revert_clicked();
	void button_localOprate_clicked();
	void button_openOix_clicked();
	void button_Exam_clicked();
	void button_simuControl_clicked();
	void button_setScene_clicked();
	void button_setFault_clicked();
	void button_sendMessage_clicked();
	void button_doccuments_clicked();
	void button_setDesplay_clicked();
	void button_help_clicked();
};

#endif