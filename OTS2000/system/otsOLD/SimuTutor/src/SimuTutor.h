#ifndef _TEACHERMANAGE_H
#define _TEACHERMANAGE_H

#include <QtGui>
#include <QApplication>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QSqlQuery>   
#include <QVariant>

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "qcommon.h"
#include "ui_SimuTutor.h"
#include "login.h"
#include "fault.h"
#include "stationconfig.h"
#include "sendmessage.h"
#include "studentinfo.h"
#include "scene.h"
#include "loginstudent.h"
#include "historyrec.h"
#include "settask.h"
#include "task.h"
#include "revert.h"
#include "model.h"  //yyp
#include "Localopr.h"//dzy
#include "Sequence.h"//dzy
#include "SceneReset.h"//dzy
#include "loginrec.h"
#include "traininfo.h"
#include "privilegemanager.h"
#include "savescene.h"
#include "scorerec.h"
#include "setpointmask.h"

class CTeacherManage : public QMainWindow,public Ui_CTeacherManage
{
	Q_OBJECT
	public:
		CTeacherManage(QWidget *parent=0);
		~CTeacherManage();
		bool flagSendMsg;
		QDialog *closeTable;
		QCheckBox   *chk;
		QRadioButton *chkMin;
		QRadioButton *chkClose;
		QLabel      *lbl;
		QPushButton *butEnter;
		QPushButton *butExit;
		QLabel		*labelLogoToolBar;
		QToolButton	*buttonSendMsg;
		QFrame *frame;
		QLabel *labelName,*labelLogo,*labelNameCo;
		CFaultTable* faulttable;
		CStationConfigTable* stationconfigtable;
		CSendMessageTable* sendmessagetable;
		CStudentInfoTable* studentinfotable;
		CSceneTable* scenetable;
		CTaskTable* tasktable;
		CLoginStudentTable* loginstudenttable;
		CRevertTable* reverttable;
		CModelForm* modelform;
		CHistoryRecTable* historyrectable;
		CSetTaskTable* settasktable;
		CSetSceneFaultTable *setscenetable,*setfaulttable;
		CLocalOprTable *localopr;
		SequenceList *pSequence;
		CLoginRecTable *loginrec;
		CTrainInfoTable *traininfo;
		CScoreRec *scorerec;
		SceneReset *pSceneReset;
		CPrivilegeMananger *privmgr;
		CSaveSceneTable *savescenetable;
		CSetPointMaskTable *setpointmasktable;
		bool ifshowtip;
		QDialog *stndevTable;
		QPushButton *buttonEnter;
		QPushButton *buttonExit;
		QTreeWidget *stnView;
		QSystemTrayIcon* iTrayIcon;
		QMenu* trayIconMenu;
		QAction* minimizeAction;
		QAction* maximizeAction;
		QAction* restoreAction;
		QAction* quitAction;
		SaveSceneThread	*saveSceneThread[MAX_ALL_LCU_NUM];
		SaveSceneRunnable *saveSceneRunnable[MAX_ALL_LCU_NUM];
		//SceneReset *pSceneReset;//dzy
		void createActions();
		void createTrayIcon();
		void createiTrayIcon();
		void bgPic();
		void readConfig();
		void writeConfig();
	protected:
		void closeEvent(QCloseEvent *event);
		void resizeEvent ( QResizeEvent * event );
		void moveEvent( QMoveEvent *event);
	private:
		bool lang_is_english;
		bool lang_is_chinese;
	signals:
		void chinese();
		void english();
		void sigLogout();
	public slots:
		void openLoginTable();
		void openFaultTable();
		void openStationConfigTable();
		void openSendMessageTable();
		void openStudentInfoTable();
		void openSceneTable();
		void openLoginStudentTable();
		void openHistoryRecTable();
		void openSetTaskTable();
		void openSetSceneTable();
		void openSetFaultTable();
		void openRevertTable();
		void openModelForm();		//yyp
		void openLocalOpr(); //dzy
		void openDlgScore();//dzy
		void openSequence();
		void openLoginRecTable();
		void openSetPointMaskTable();
		void openPrivilegeManager();
		void openExamTable();
		void userLogin(int,const QString &);
		void userLogout();
		void isQuit();
		void iconActivated(QSystemTrayIcon::ActivationReason reason);
		void oncheckbox(int);
		void openTaskTable();
		void openTrainInfoTable();
		void checkQuit();
		void loadChinese();
		void loadEnglish();
		void onactionALL_reset();
		void initStnView();
		void openCallDataTable();
		void openSaveSceneTable();
		void callData();
		void saveScene();
		void treeItemChanged(QTreeWidgetItem *item,int column);
		void updateParentItem(QTreeWidgetItem* item);
		void recvMsg();
		void blinkChatButton();
		void about();
		void help();
};
Q_DECLARE_METATYPE(LCUID);
#endif