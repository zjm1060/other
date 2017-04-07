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
#include "ui_SimuTrainee.h"
#include "Login.h"
#include "LocalOperate.h"
#include "taskManage.h"
#include "thread.h"
#include "RevertPlayer.h"
#include "SimuTip.h"
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "/home/ems/system/ots/SimuTutor/src/settask.h"
#include "/home/ems/system/ots/SimuTutor/src/historyrec.h"
#include "/home/ems/system/ots/SimuTutor/src/sendmessage.h"


Q_DECLARE_METATYPE(LCUID);
class SimuTrainee:public QMainWindow,Ui_SimuTrainee
{
	Q_OBJECT
public:
	explicit SimuTrainee(QWidget *parent = 0);
	~SimuTrainee();
private:
	CSetTaskTable *setTaskDialog;
	CSetSceneFaultTable *setSceneDialog;
	CSetSceneFaultTable *setFaultDialog;
	CSendMessageTable *sendMessageDialog;
	CLocalOprate *localOprateDialog;
	CHistoryRecTable *recordDialog;
	CChangePassword *changePasswordDialog;
	CNewTaskTip *newTaskTipDialog;
	CRevertPlayer *revertPlayerDialog;
	QAction *actionMinimize; 
	QAction *actionMaximize; 
	QAction *actionRestore; 
	QAction *actionQuit; 
	QMenu *trayIconMenu;
	QSystemTrayIcon *iTrayIcon;
	QMainWindow *windowSimply;
	short simuSpeed;
	QLabel *labelStatusBar;
	QDialog *closeTable;
	QRadioButton *chkMin;
	QRadioButton *chkClose;
	int isWarn;
	HeartBeatThread *heartBeatThread;
	DisplayThread *displayThread;
	bool flagSendMsg;
	QTimer *timerButtonMsg;
	void init();
	void createActions();
	void createTrayIcon();
	void createiTrayIcon();
	void creatWindowSimply();
protected:
	bool winEvent(MSG *message, long *result);
	void closeEvent(QCloseEvent *event);
Q_SIGNALS:
	void sigSpeedChanged(const QString &);
private Q_SLOTS:
	void openLogin();
	void login(int,const QString &);
	void logout();
	void forceLogout();
	void cleanUserInfo();
	void openChangePassword();
	void simuStart();
	void simuStop();
	void openSimuSpeed();
	void setSimuSpeed();
	void setSimuSpeedLabel(int);
	void setSimuSpeedMenu();
	void taskFinish();
	void openOix();
	void openRevert();
	void openTask();
	void openRecord();
	void openLocalOperate();
	void openSendMessage();
	void openSetScene();
	void openSetFault();
	void openSetTask();
	void openExam();
	void openSetSimuTip();
	void about();
	void help();
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void changeModeNormal();
	void changeModeSimply();
	void changeStyle1();
	void changeStyle2();
	void changeStyle3();
	void newTaskReceived();
	void updateStatusBar(const QString &);
	void updateCurTaskName(const QString &);
	void updateSimuProgress(int);
	void setOnTop(bool);
	void setTaskTime(const QTime &);
	void taskStarted();
	void taskFinished();
	void recvMsg();
	void blinkMsgButton();
	void isQuit();
	void onCheckBox(int);
	void checkQuit();
};

#endif