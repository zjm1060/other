#ifndef _SIMUTRAINEE_H
#define _SIMUTRAINEE_H

#ifdef WIN32
#include <qt_windows.h>
#define WM_OPEN_SIMU_MAIN		(WM_USER+0x001)
#define WM_OPEN_SETTASK			(WM_USER+0x002)
#define WM_OPEN_SETSCENE		(WM_USER+0x003)
#define WM_OPEN_SETFAULT		(WM_USER+0x004)
#define WM_OPEN_CURRENTTASK		(WM_USER+0x005)
#define WM_OPEN_REVERT			(WM_USER+0x006)
#define WM_OPEN_LOCAL_OPR		(WM_USER+0x007)
#define WM_OPEN_HISTORY			(WM_USER+0x008)
#define WM_OPEN_SENDMSG			(WM_USER+0x009)
#define WM_OPEN_EXAM			(WM_USER+0x010)
#endif
#define MAX_TASK_NUM		5
#include <QtGui>
#include <QApplication>
#include <QtCore/QCoreApplication>  
#include <QtSql>
#include <QThread>
#include <QMainWindow>
#include <QLCDNumber>
#include "ui_SimuTrainee.h"


#include "thread.h"
#include "SimuTool.h"
#include "Login.h"
#include "userinfo.h"
#include "RevertPlayer.h"
#include "LocalOperate.h"
#include "taskManage.h"
#include "Simucontrol.h"
#include "hostpage.h"
#include "mydoc.h"
#include "mdichild.h"
#include "waitcontest.h"
#include "/home/ems/system/ots/SimuTutor/src/settask.h"
#include "/home/ems/system/ots/SimuTutor/src/sendmessage.h"
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"
#include "/home/ems/system/ots/SimuTutor/src/historyrec.h"
#include "/home/ems/system/ots/SimuTutor/src/scorerec.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "../src/cserver/interface.h"
#include "../src/cserver/controler.h"


Q_DECLARE_METATYPE(LCUID);
class SimuTool;
class MdiChild;
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;

class SimuTrainee:public QMainWindow,Ui_SimuTrainee
{
	Q_OBJECT
public:
	explicit SimuTrainee(QWidget *parent = 0);
	~SimuTrainee();
protected:
	void showEvent(QShowEvent *event);
	void hideEvent ( QHideEvent * event );
	void moveEvent ( QMoveEvent * event );
	void closeEvent(QCloseEvent *event);
	bool winEvent(MSG *message, long *result);
private:
	
//==Functions==:
	void init();
	void setToolBoxConnect();
	MdiChild *activeMdiChild();
	QMdiSubWindow *findMdiChild(const QString &fileName);

	void createToolBars();
	void createActions();
	void createTrayIcon();
	void createiTrayIcon();
	//void  recvMsg();

	void  setHDBSimuSpeed();
//==Variable==:
	MdiChild *subwindow[20];
	SimuTool *d_SimuTool;
	UserInfo *d_userinfo;
	int  groups_id;
	bool role_lerder;
	int trainmode;
	QString login_user_longname;
	CHistoryRecTable *d_historyrec;
	CStudyRecTable  *d_studyrec;
	CScoreRec *d_CScoreRec;
	CRevertPlayer *d_RevertPlayer;
	CLocalOprate *d_CLocalOprate;
	CTaskManage *d_CTaskManage;//
	bool dlg_task_clicked;
	CSetTaskTable *d_CSetTaskTable;
	CNewTaskTip *newTaskTipDialog;
	CSetSceneFaultTable *setSceneFaultDialog;
	CSendMessageTable *sendMessageDialog;
	Mydoc *d_mydoc;
	bool dlg_msg_clicked;
	bool flagSendMsg;
	Simucontrol *simuControlwidget;
	Hostpage *d_hostpage;
	CWaitContest *d_wait_contest;
	QAction *actionToolbox; 
	QAction *actionBaseinfo; 
	QAction *actionHostPage; 
	QAction *actionMinimize; 
	QAction *actionMaximize; 
	QAction *actionRestore; 
	QAction *actionQuit; 
	QToolButton *tbNewSms; 
	QMenu *trayIconMenu;
	QSystemTrayIcon *iTrayIcon;
	QDialog *closeTable;
	QTimer *timerButtonMsg;
	short simuSpeed;
	QLabel *labelStatusBar;
	QLabel *labelLogoToolBar;
	QComboBox *comBroadcast;//广播板
	QRadioButton *chkMin;
	QRadioButton *chkClose;

	int isWarn;
	bool lang_is_chinese,lang_is_english;
//仿真控制相关


//MDI相关
	MdiArea *mdiArea;
	QSignalMapper *windowMapper;
	//void setActiveSubWindow(QWidget *window);
//THREAD相关
	HeartBeatThread *heartBeatThread;
	DisplayThread *displayThread;
	NetstateThread *m_NetstateThread;
	QMessageBox  *m_threadmsgbox;
//ABOUT 窗口有关
	QDialog *m_dlg;
	QLabel *label1,*label2,*label3,*label4;
	QTextEdit *textedt;
	QGridLayout *m_layout;
	//教员监视
	Controler *m_mornitor_controlor;
//== SLOTS ==:
public slots:
	void loadEnglish();
	void loadChinese();
	void show_toolbox();
	void on_userloged(int user_id,const QString &user_name,int mode);
	void cleanUserInfo();
	void show_userinfo();
	void show_record();
	void show_studyrecord();
	void show_userscore();
	void show_logout();	
	void show_task();
	void show_settask();
	void show_revert();
	void show_localOprate();
	void show_localOprate1();
	void show_localOprate2();
	void show_openOix();
	void show_Exam();
	void show_simuControl();
	void show_setScene();
	void show_setFault();
	void show_sendMessage();
	void show_doc();
	void show_setDesplay();
	void show_help();
	void show_OTS2000();
	void setActiveSubWindow(QWidget *window);
	void show_hostpage();
	void show_wait_contest();
	void show_tryconnectfail();
	void show_tryconnec(int i,bool j);
	void change_ots_mode(int mode);
	void recvMsg();
	void on_STclicked();
	void on_SOclicked();
	void on_ETclicked();
	void on_EOclicked();
	void on_CPTclicked();
	void on_simustart();
	void on_simustop();
	void on_action_1_8__triggered();
	void on_action_1_4__triggered();
	void on_action_1_2__triggered();
	void on_action_1__triggered();
	void on_action_2__triggered();
	void on_action_4__triggered();
	void on_action_8__triggered();
	void  blinkMsgButton();
Q_SIGNALS:	
		void sigSpeedChanged(const QString &);
		void sigModeChanged(int);
		void sigLanguageChanged(int);
private Q_SLOTS:
	void forceLogout();
	void on_mode_changed(int mode);
	void show_hide_toolbox();
	void on_simustatechanged(unsigned char flag);
	void on_action_setontop(bool i);
	void on_action_simplestyle();
	void on_action_elegantstyle();
	void on_action_defaultstyle();
	void updatehostpage();
	void hidemenu();
	void processPendingDatagrams();
//  	void simuStart();
//  	void simuStop();
// 	void openSimuSpeed();
// 	void setSimuSpeed();
// 	void setSimuSpeedLabel(int);
// 	void setSimuSpeedMenu();

// 	void about();
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
// 	void changeModeNormal();
// 	void changeModeSimply();

  	void newTaskReceived();
  	void taskFinish();
  	void taskStarted();
  	void taskFinished();
	void setTaskTime(const QTime &);
// 	void updateStatusBar(const QString &);
// 	void updateCurTaskName(const QString &);
// 	void updateSimuProgress(int);
// 	void setOnTop(bool);

 	void isQuit();
 	void onCheckBox(int);
 	void checkQuit();
};

#endif