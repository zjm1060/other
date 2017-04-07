/********************************************************************************
** Form generated from reading ui file 'SimuTutor.ui'
**
** Created: Thu Jan 15 10:26:29 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SIMUTUTOR_H
#define UI_SIMUTUTOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CTeacherManage
{
public:
    QAction *actionStudentInfo_menu;
    QAction *actionStop;
    QAction *actionPause;
    QAction *actionGoOn;
    QAction *actionSetTask_menu;
    QAction *actionSendMessage_menu;
    QAction *actionLogin_menu;
    QAction *actionExit_menu;
    QAction *actionHelp;
    QAction *actionAbout;
    QAction *actionContact;
    QAction *actionStationConfig_menu;
    QAction *actionSimSpeed;
    QAction *actionLogin;
    QAction *actionStudentInfo;
    QAction *actionHistoryRec;
    QAction *actionSetTask;
    QAction *actionExit;
    QAction *actionModel;
    QAction *actionStationConfig;
    QAction *actionConfig_menu;
    QAction *actionLoginStudent;
    QAction *actionSendMessage;
    QAction *actionPaper_menu;
    QAction *actionOprManage;
    QAction *actionModel_menu;
    QAction *actionSequence;
    QAction *action_18;
    QAction *action_user_score;
    QAction *action_21;
    QAction *action_22;
    QAction *actionFault;
    QAction *actionTask;
    QAction *actionScene;
    QAction *actionRevert;
    QAction *actionPrivilege;
    QAction *actionLoginStatus;
    QAction *actionHistoryRec_menu;
    QAction *actionQuit;
    QAction *actionOprManage_menu;
    QAction *actionLoginRec;
    QAction *actionTrainInfo;
    QAction *actionChinese;
    QAction *actionEnglish;
    QAction *actionSubject_menu;
    QAction *actionTestinfo_menu;
    QAction *actionALL_reset;
    QAction *actionCallData;
    QAction *actionSaveScene;
    QAction *actionExam;
    QAction *actionSetScene;
    QAction *actionSetFault;
    QAction *actionSetPointMask;
    QAction *actionStudyRec;
    QAction *action_2;
    QAction *action_contestmanage;
    QAction *action_competetionscore;
    QAction *action_sendcontest;
    QAction *action_doc_manage;
    QAction *action_doc_study_plan;
    QAction *actionBrodcast;
    QWidget *centralwidget;
    QToolBar *toolBar;
    QMenuBar *menubar;
    QMenu *menu_user;
    QMenu *menu_help;
    QMenu *menuTrain;
    QMenu *menu_file;
    QMenu *menu_sys;
    QMenu *menu_language;
    QMenu *menu_exam;
    QMenu *menu_2;
    QMenu *menuMdlLib;
    QMenu *menuRecord;
    QMenu *menu;

    void setupUi(QMainWindow *CTeacherManage)
    {
        if (CTeacherManage->objectName().isEmpty())
            CTeacherManage->setObjectName(QString::fromUtf8("CTeacherManage"));
        CTeacherManage->resize(1000, 720);
        CTeacherManage->setMinimumSize(QSize(860, 600));
        CTeacherManage->setSizeIncrement(QSize(800, 600));
        CTeacherManage->setCursor(QCursor(Qt::ArrowCursor));
        CTeacherManage->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        CTeacherManage->setIconSize(QSize(18, 24));
        CTeacherManage->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        actionStudentInfo_menu = new QAction(CTeacherManage);
        actionStudentInfo_menu->setObjectName(QString::fromUtf8("actionStudentInfo_menu"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/pic/images/20090119102024707.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8("../../../../../../Documents and Settings/lenovo/\346\241\214\351\235\242/images/ots31.png"), QSize(), QIcon::Normal, QIcon::On);
        actionStudentInfo_menu->setIcon(icon);
        actionStop = new QAction(CTeacherManage);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/pic/images/media-stop-32.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon1);
        actionPause = new QAction(CTeacherManage);
        actionPause->setObjectName(QString::fromUtf8("actionPause"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/pic/images/media-pause-32.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        actionPause->setIcon(icon2);
        actionGoOn = new QAction(CTeacherManage);
        actionGoOn->setObjectName(QString::fromUtf8("actionGoOn"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/pic/images/media-play-32.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        actionGoOn->setIcon(icon3);
        actionSetTask_menu = new QAction(CTeacherManage);
        actionSetTask_menu->setObjectName(QString::fromUtf8("actionSetTask_menu"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/pic/images/ots06.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        actionSetTask_menu->setIcon(icon4);
        actionSendMessage_menu = new QAction(CTeacherManage);
        actionSendMessage_menu->setObjectName(QString::fromUtf8("actionSendMessage_menu"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/pic/images/20071224091514244[1].PNG"), QSize(), QIcon::Normal, QIcon::Off);
        actionSendMessage_menu->setIcon(icon5);
        actionLogin_menu = new QAction(CTeacherManage);
        actionLogin_menu->setObjectName(QString::fromUtf8("actionLogin_menu"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/pic/images/20090119102144587.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        actionLogin_menu->setIcon(icon6);
        actionExit_menu = new QAction(CTeacherManage);
        actionExit_menu->setObjectName(QString::fromUtf8("actionExit_menu"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/pic/images/ots03.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        icon7.addFile(QString::fromUtf8(":/images/quit.png"), QSize(), QIcon::Normal, QIcon::On);
        actionExit_menu->setIcon(icon7);
        actionHelp = new QAction(CTeacherManage);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/pic/images/find_normal.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHelp->setIcon(icon8);
        actionAbout = new QAction(CTeacherManage);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/pic/images/forums_icon.gif"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout->setIcon(icon9);
        actionContact = new QAction(CTeacherManage);
        actionContact->setObjectName(QString::fromUtf8("actionContact"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/pic/images/get_conn.gif"), QSize(), QIcon::Normal, QIcon::Off);
        actionContact->setIcon(icon10);
        actionStationConfig_menu = new QAction(CTeacherManage);
        actionStationConfig_menu->setObjectName(QString::fromUtf8("actionStationConfig_menu"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/pic/images/\345\217\202\346\225\260\350\256\276\347\275\2561.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        actionStationConfig_menu->setIcon(icon11);
        actionSimSpeed = new QAction(CTeacherManage);
        actionSimSpeed->setObjectName(QString::fromUtf8("actionSimSpeed"));
        actionSimSpeed->setCheckable(false);
        actionSimSpeed->setEnabled(false);
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/images/refresh-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSimSpeed->setIcon(icon12);
        actionLogin = new QAction(CTeacherManage);
        actionLogin->setObjectName(QString::fromUtf8("actionLogin"));
        actionLogin->setIcon(icon6);
        actionStudentInfo = new QAction(CTeacherManage);
        actionStudentInfo->setObjectName(QString::fromUtf8("actionStudentInfo"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/pic/images/20090119102024707.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        actionStudentInfo->setIcon(icon13);
        actionHistoryRec = new QAction(CTeacherManage);
        actionHistoryRec->setObjectName(QString::fromUtf8("actionHistoryRec"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/pic/images/ots16.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        actionHistoryRec->setIcon(icon14);
        actionSetTask = new QAction(CTeacherManage);
        actionSetTask->setObjectName(QString::fromUtf8("actionSetTask"));
        actionSetTask->setIcon(icon4);
        actionExit = new QAction(CTeacherManage);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/pic/images/ots03.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon15);
        actionModel = new QAction(CTeacherManage);
        actionModel->setObjectName(QString::fromUtf8("actionModel"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/pic/images/ots08.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        actionModel->setIcon(icon16);
        actionStationConfig = new QAction(CTeacherManage);
        actionStationConfig->setObjectName(QString::fromUtf8("actionStationConfig"));
        actionStationConfig->setIcon(icon11);
        actionConfig_menu = new QAction(CTeacherManage);
        actionConfig_menu->setObjectName(QString::fromUtf8("actionConfig_menu"));
        actionLoginStudent = new QAction(CTeacherManage);
        actionLoginStudent->setObjectName(QString::fromUtf8("actionLoginStudent"));
        actionLoginStudent->setIcon(icon13);
        actionSendMessage = new QAction(CTeacherManage);
        actionSendMessage->setObjectName(QString::fromUtf8("actionSendMessage"));
        actionSendMessage->setIcon(icon5);
        actionPaper_menu = new QAction(CTeacherManage);
        actionPaper_menu->setObjectName(QString::fromUtf8("actionPaper_menu"));
        actionPaper_menu->setEnabled(true);
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/pic/images/20071126111905477[1].PNG"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaper_menu->setIcon(icon17);
        actionOprManage = new QAction(CTeacherManage);
        actionOprManage->setObjectName(QString::fromUtf8("actionOprManage"));
        actionOprManage->setEnabled(true);
        actionOprManage->setIcon(icon17);
        actionModel_menu = new QAction(CTeacherManage);
        actionModel_menu->setObjectName(QString::fromUtf8("actionModel_menu"));
        actionSequence = new QAction(CTeacherManage);
        actionSequence->setObjectName(QString::fromUtf8("actionSequence"));
        action_18 = new QAction(CTeacherManage);
        action_18->setObjectName(QString::fromUtf8("action_18"));
        action_user_score = new QAction(CTeacherManage);
        action_user_score->setObjectName(QString::fromUtf8("action_user_score"));
        action_user_score->setCheckable(false);
        action_user_score->setEnabled(true);
        action_21 = new QAction(CTeacherManage);
        action_21->setObjectName(QString::fromUtf8("action_21"));
        action_22 = new QAction(CTeacherManage);
        action_22->setObjectName(QString::fromUtf8("action_22"));
        actionFault = new QAction(CTeacherManage);
        actionFault->setObjectName(QString::fromUtf8("actionFault"));
        actionTask = new QAction(CTeacherManage);
        actionTask->setObjectName(QString::fromUtf8("actionTask"));
        actionScene = new QAction(CTeacherManage);
        actionScene->setObjectName(QString::fromUtf8("actionScene"));
        actionRevert = new QAction(CTeacherManage);
        actionRevert->setObjectName(QString::fromUtf8("actionRevert"));
        actionPrivilege = new QAction(CTeacherManage);
        actionPrivilege->setObjectName(QString::fromUtf8("actionPrivilege"));
        actionPrivilege->setCheckable(false);
        actionPrivilege->setEnabled(true);
        actionLoginStatus = new QAction(CTeacherManage);
        actionLoginStatus->setObjectName(QString::fromUtf8("actionLoginStatus"));
        actionLoginStatus->setEnabled(false);
        actionLoginStatus->setMenuRole(QAction::AboutQtRole);
        actionHistoryRec_menu = new QAction(CTeacherManage);
        actionHistoryRec_menu->setObjectName(QString::fromUtf8("actionHistoryRec_menu"));
        actionQuit = new QAction(CTeacherManage);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/pic/images/quit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionQuit->setIcon(icon18);
        actionOprManage_menu = new QAction(CTeacherManage);
        actionOprManage_menu->setObjectName(QString::fromUtf8("actionOprManage_menu"));
        actionLoginRec = new QAction(CTeacherManage);
        actionLoginRec->setObjectName(QString::fromUtf8("actionLoginRec"));
        actionTrainInfo = new QAction(CTeacherManage);
        actionTrainInfo->setObjectName(QString::fromUtf8("actionTrainInfo"));
        actionChinese = new QAction(CTeacherManage);
        actionChinese->setObjectName(QString::fromUtf8("actionChinese"));
        actionEnglish = new QAction(CTeacherManage);
        actionEnglish->setObjectName(QString::fromUtf8("actionEnglish"));
        actionSubject_menu = new QAction(CTeacherManage);
        actionSubject_menu->setObjectName(QString::fromUtf8("actionSubject_menu"));
        actionTestinfo_menu = new QAction(CTeacherManage);
        actionTestinfo_menu->setObjectName(QString::fromUtf8("actionTestinfo_menu"));
        actionALL_reset = new QAction(CTeacherManage);
        actionALL_reset->setObjectName(QString::fromUtf8("actionALL_reset"));
        actionCallData = new QAction(CTeacherManage);
        actionCallData->setObjectName(QString::fromUtf8("actionCallData"));
        actionSaveScene = new QAction(CTeacherManage);
        actionSaveScene->setObjectName(QString::fromUtf8("actionSaveScene"));
        actionExam = new QAction(CTeacherManage);
        actionExam->setObjectName(QString::fromUtf8("actionExam"));
        actionSetScene = new QAction(CTeacherManage);
        actionSetScene->setObjectName(QString::fromUtf8("actionSetScene"));
        actionSetFault = new QAction(CTeacherManage);
        actionSetFault->setObjectName(QString::fromUtf8("actionSetFault"));
        actionSetPointMask = new QAction(CTeacherManage);
        actionSetPointMask->setObjectName(QString::fromUtf8("actionSetPointMask"));
        actionStudyRec = new QAction(CTeacherManage);
        actionStudyRec->setObjectName(QString::fromUtf8("actionStudyRec"));
        action_2 = new QAction(CTeacherManage);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        action_contestmanage = new QAction(CTeacherManage);
        action_contestmanage->setObjectName(QString::fromUtf8("action_contestmanage"));
        action_competetionscore = new QAction(CTeacherManage);
        action_competetionscore->setObjectName(QString::fromUtf8("action_competetionscore"));
        action_sendcontest = new QAction(CTeacherManage);
        action_sendcontest->setObjectName(QString::fromUtf8("action_sendcontest"));
        action_doc_manage = new QAction(CTeacherManage);
        action_doc_manage->setObjectName(QString::fromUtf8("action_doc_manage"));
        action_doc_study_plan = new QAction(CTeacherManage);
        action_doc_study_plan->setObjectName(QString::fromUtf8("action_doc_study_plan"));
        actionBrodcast = new QAction(CTeacherManage);
        actionBrodcast->setObjectName(QString::fromUtf8("actionBrodcast"));
        centralwidget = new QWidget(CTeacherManage);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        CTeacherManage->setCentralWidget(centralwidget);
        toolBar = new QToolBar(CTeacherManage);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setAutoFillBackground(false);
        toolBar->setAllowedAreas(Qt::AllToolBarAreas);
        toolBar->setIconSize(QSize(50, 40));
        toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        CTeacherManage->addToolBar(Qt::TopToolBarArea, toolBar);
        menubar = new QMenuBar(CTeacherManage);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1000, 20));
        menu_user = new QMenu(menubar);
        menu_user->setObjectName(QString::fromUtf8("menu_user"));
        menu_help = new QMenu(menubar);
        menu_help->setObjectName(QString::fromUtf8("menu_help"));
        menuTrain = new QMenu(menubar);
        menuTrain->setObjectName(QString::fromUtf8("menuTrain"));
        menu_file = new QMenu(menubar);
        menu_file->setObjectName(QString::fromUtf8("menu_file"));
        menu_sys = new QMenu(menubar);
        menu_sys->setObjectName(QString::fromUtf8("menu_sys"));
        menu_language = new QMenu(menu_sys);
        menu_language->setObjectName(QString::fromUtf8("menu_language"));
        menu_exam = new QMenu(menubar);
        menu_exam->setObjectName(QString::fromUtf8("menu_exam"));
        menu_2 = new QMenu(menu_exam);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menuMdlLib = new QMenu(menubar);
        menuMdlLib->setObjectName(QString::fromUtf8("menuMdlLib"));
        menuRecord = new QMenu(menubar);
        menuRecord->setObjectName(QString::fromUtf8("menuRecord"));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        CTeacherManage->setMenuBar(menubar);

        toolBar->addAction(actionLogin);
        toolBar->addAction(actionExit);
        toolBar->addAction(actionLoginStatus);
        toolBar->addSeparator();
        toolBar->addSeparator();
        toolBar->addAction(actionStudentInfo);
        toolBar->addAction(actionHistoryRec);
        toolBar->addAction(actionSetTask);
        toolBar->addSeparator();
        toolBar->addSeparator();
        toolBar->addAction(actionModel);
        toolBar->addAction(actionStationConfig);
        toolBar->addSeparator();
        toolBar->addSeparator();
        toolBar->addAction(actionOprManage);
        toolBar->addSeparator();
        toolBar->addSeparator();
        menubar->addAction(menu_file->menuAction());
        menubar->addAction(menu_user->menuAction());
        menubar->addAction(menuTrain->menuAction());
        menubar->addAction(menuMdlLib->menuAction());
        menubar->addAction(menuRecord->menuAction());
        menubar->addAction(menu_exam->menuAction());
        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_sys->menuAction());
        menubar->addAction(menu_help->menuAction());
        menu_user->addAction(actionStudentInfo_menu);
        menu_user->addAction(actionLoginRec);
        menu_user->addAction(action_user_score);
        menu_user->addAction(actionPrivilege);
        menu_user->addSeparator();
        menu_help->addAction(actionHelp);
        menu_help->addSeparator();
        menu_help->addAction(actionAbout);
        menu_help->addSeparator();
        menu_help->addAction(actionContact);
        menuTrain->addAction(actionSetTask_menu);
        menuTrain->addAction(action_2);
        menuTrain->addAction(actionSetScene);
        menuTrain->addAction(actionSetFault);
        menuTrain->addAction(actionLoginStudent);
        menuTrain->addAction(actionTrainInfo);
        menuTrain->addSeparator();
        menuTrain->addSeparator();
        menuTrain->addAction(actionSendMessage_menu);
        menuTrain->addSeparator();
        menuTrain->addAction(actionBrodcast);
        menu_file->addAction(actionLogin_menu);
        menu_file->addAction(actionExit_menu);
        menu_file->addAction(actionQuit);
        menu_sys->addAction(actionStationConfig_menu);
        menu_sys->addSeparator();
        menu_sys->addAction(actionALL_reset);
        menu_sys->addAction(actionSaveScene);
        menu_sys->addAction(actionCallData);
        menu_sys->addAction(actionSetPointMask);
        menu_sys->addAction(menu_language->menuAction());
        menu_language->addAction(actionChinese);
        menu_language->addAction(actionEnglish);
        menu_exam->addSeparator();
        menu_exam->addAction(actionExam);
        menu_exam->addAction(menu_2->menuAction());
        menu_2->addAction(action_doc_manage);
        menu_2->addAction(action_doc_study_plan);
        menuMdlLib->addAction(actionModel_menu);
        menuMdlLib->addAction(actionSequence);
        menuMdlLib->addSeparator();
        menuMdlLib->addAction(actionFault);
        menuMdlLib->addAction(actionTask);
        menuMdlLib->addSeparator();
        menuMdlLib->addAction(actionScene);
        menuMdlLib->addAction(actionRevert);
        menuMdlLib->addSeparator();
        menuMdlLib->addAction(actionOprManage_menu);
        menuRecord->addAction(actionHistoryRec_menu);
        menuRecord->addAction(actionStudyRec);
        menu->addAction(action_contestmanage);
        menu->addAction(action_sendcontest);
        menu->addAction(action_competetionscore);

        retranslateUi(CTeacherManage);

        QMetaObject::connectSlotsByName(CTeacherManage);
    } // setupUi

    void retranslateUi(QMainWindow *CTeacherManage)
    {
        CTeacherManage->setWindowTitle(QApplication::translate("CTeacherManage", "OTS2000  \346\225\231\345\221\230\347\253\231", 0, QApplication::UnicodeUTF8));
        actionStudentInfo_menu->setText(QApplication::translate("CTeacherManage", "\345\255\246\345\221\230\347\224\250\346\210\267\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        actionStop->setText(QApplication::translate("CTeacherManage", "\345\201\234\346\255\242", 0, QApplication::UnicodeUTF8));
        actionPause->setText(QApplication::translate("CTeacherManage", "\346\232\202\345\201\234", 0, QApplication::UnicodeUTF8));
        actionGoOn->setText(QApplication::translate("CTeacherManage", "\347\273\247\347\273\255", 0, QApplication::UnicodeUTF8));
        actionSetTask_menu->setText(QApplication::translate("CTeacherManage", "\344\270\213\345\217\221\346\223\215\344\275\234\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        actionSendMessage_menu->setText(QApplication::translate("CTeacherManage", "\345\217\221\351\200\201\346\217\220\347\244\272\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        actionLogin_menu->setText(QApplication::translate("CTeacherManage", "\347\231\273\345\275\225    (CTRL+ENTER)", 0, QApplication::UnicodeUTF8));
        actionLogin_menu->setShortcut(QApplication::translate("CTeacherManage", "Ctrl+Return", 0, QApplication::UnicodeUTF8));
        actionExit_menu->setText(QApplication::translate("CTeacherManage", "\346\263\250\351\224\200    (ESC)", 0, QApplication::UnicodeUTF8));
        actionExit_menu->setShortcut(QApplication::translate("CTeacherManage", "Esc", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("CTeacherManage", "\346\225\231\345\221\230\347\256\241\347\220\206\347\263\273\347\273\237\345\270\256\345\212\251 (F1)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionHelp->setToolTip(QApplication::translate("CTeacherManage", "\346\225\231\345\221\230\347\256\241\347\220\206\347\263\273\347\273\237\345\270\256\345\212\251", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionHelp->setShortcut(QApplication::translate("CTeacherManage", "F1", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("CTeacherManage", "\345\205\263\344\272\216\346\210\221\344\273\254", 0, QApplication::UnicodeUTF8));
        actionContact->setText(QApplication::translate("CTeacherManage", "\350\201\224\347\263\273\346\210\221\344\273\254", 0, QApplication::UnicodeUTF8));
        actionStationConfig_menu->setText(QApplication::translate("CTeacherManage", "\351\205\215\347\275\256\345\267\245\344\275\234\347\253\231", 0, QApplication::UnicodeUTF8));
        actionSimSpeed->setText(QApplication::translate("CTeacherManage", "\344\277\256\346\224\271\344\273\277\347\234\237\351\200\237\345\272\246", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSimSpeed->setToolTip(QApplication::translate("CTeacherManage", "\346\237\245\350\257\242\345\275\223\345\211\215\344\273\277\347\234\237\351\200\237\345\272\246", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionLogin->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionLogin->setToolTip(QApplication::translate("CTeacherManage", "\347\231\273\351\231\206", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionStudentInfo->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionStudentInfo->setToolTip(QApplication::translate("CTeacherManage", "\345\255\246\345\221\230\347\224\250\346\210\267\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionHistoryRec->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionHistoryRec->setToolTip(QApplication::translate("CTeacherManage", "\345\255\246\345\221\230\345\216\206\345\217\262\350\256\260\345\275\225\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSetTask->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionSetTask->setToolTip(QApplication::translate("CTeacherManage", "\350\256\276\347\275\256\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionExit->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionExit->setToolTip(QApplication::translate("CTeacherManage", "\346\263\250\351\224\200", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionModel->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionModel->setToolTip(QApplication::translate("CTeacherManage", "\346\250\241\345\236\213\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionStationConfig->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionStationConfig->setToolTip(QApplication::translate("CTeacherManage", "\351\205\215\347\275\256\345\267\245\344\275\234\347\253\231", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionConfig_menu->setText(QApplication::translate("CTeacherManage", "\347\263\273\347\273\237\345\217\202\346\225\260\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        actionLoginStudent->setText(QApplication::translate("CTeacherManage", "\345\234\250\347\272\277\345\255\246\345\221\230\345\237\271\350\256\255", 0, QApplication::UnicodeUTF8));
        actionSendMessage->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionSendMessage->setToolTip(QApplication::translate("CTeacherManage", "\345\217\221\351\200\201\346\217\220\347\244\272\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPaper_menu->setText(QApplication::translate("CTeacherManage", "\344\273\273\345\212\241\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        actionOprManage->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionOprManage->setToolTip(QApplication::translate("CTeacherManage", "\345\234\250\347\272\277\346\223\215\344\275\234\345\272\223", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionModel_menu->setText(QApplication::translate("CTeacherManage", "\346\250\241\345\236\213\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        actionSequence->setText(QApplication::translate("CTeacherManage", "\346\265\201\347\250\213\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        action_18->setText(QApplication::translate("CTeacherManage", "\345\217\230\351\207\217\350\241\250\347\273\264\346\212\244", 0, QApplication::UnicodeUTF8));
        action_user_score->setText(QApplication::translate("CTeacherManage", "\345\255\246\345\221\230\345\216\206\345\217\262\346\210\220\347\273\251", 0, QApplication::UnicodeUTF8));
        action_21->setText(QApplication::translate("CTeacherManage", "\351\235\231\346\200\201\345\267\245\345\206\265\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        action_22->setText(QApplication::translate("CTeacherManage", "\345\267\245\345\206\265\345\206\215\347\216\260\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        actionFault->setText(QApplication::translate("CTeacherManage", "\346\225\205\351\232\234\345\272\223\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        actionTask->setText(QApplication::translate("CTeacherManage", "\345\237\271\350\256\255\346\241\210\344\276\213\345\272\223", 0, QApplication::UnicodeUTF8));
        actionScene->setText(QApplication::translate("CTeacherManage", "\351\235\231\346\200\201\345\267\245\345\206\265\345\272\223", 0, QApplication::UnicodeUTF8));
        actionRevert->setText(QApplication::translate("CTeacherManage", "\345\267\245\345\206\265\345\206\215\347\216\260\345\272\223", 0, QApplication::UnicodeUTF8));
        actionPrivilege->setText(QApplication::translate("CTeacherManage", "\347\224\250\346\210\267\346\235\203\351\231\220", 0, QApplication::UnicodeUTF8));
        actionLoginStatus->setText(QApplication::translate("CTeacherManage", "\346\234\252\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
        actionHistoryRec_menu->setText(QApplication::translate("CTeacherManage", "\345\255\246\345\221\230\345\216\206\345\217\262\350\256\260\345\275\225\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("CTeacherManage", "\351\200\200\345\207\272    (CTRL+Q)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionQuit->setToolTip(QApplication::translate("CTeacherManage", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionQuit->setShortcut(QApplication::translate("CTeacherManage", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionOprManage_menu->setText(QApplication::translate("CTeacherManage", "\345\234\250\347\272\277\346\223\215\344\275\234\345\272\223", 0, QApplication::UnicodeUTF8));
        actionLoginRec->setText(QApplication::translate("CTeacherManage", "\345\255\246\345\221\230\347\231\273\345\275\225\345\216\206\345\217\262\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        actionTrainInfo->setText(QApplication::translate("CTeacherManage", "\345\237\271\350\256\255\344\277\241\346\201\257\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        actionChinese->setText(QApplication::translate("CTeacherManage", "\344\270\255\346\226\207(CHINESE)", 0, QApplication::UnicodeUTF8));
        actionEnglish->setText(QApplication::translate("CTeacherManage", "\350\213\261\346\226\207(ENGLISH)", 0, QApplication::UnicodeUTF8));
        actionSubject_menu->setText(QApplication::translate("CTeacherManage", "\351\242\230\345\272\223\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        actionTestinfo_menu->setText(QApplication::translate("CTeacherManage", "\346\210\220\347\273\251\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        actionALL_reset->setText(QApplication::translate("CTeacherManage", "\345\205\250\345\216\202\345\267\245\345\206\265\345\210\235\345\247\213", 0, QApplication::UnicodeUTF8));
        actionCallData->setText(QApplication::translate("CTeacherManage", "\345\217\254\345\224\244\347\233\221\346\216\247\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionCallData->setToolTip(QApplication::translate("CTeacherManage", "\345\217\254\345\224\244\347\233\221\346\216\247\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSaveScene->setText(QApplication::translate("CTeacherManage", "\344\277\235\345\255\230\345\275\223\345\211\215\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSaveScene->setToolTip(QApplication::translate("CTeacherManage", "\344\277\235\345\255\230\345\275\223\345\211\215\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionExam->setText(QApplication::translate("CTeacherManage", "\344\270\223\344\270\232\347\237\245\350\257\206\350\200\203\350\257\225", 0, QApplication::UnicodeUTF8));
        actionSetScene->setText(QApplication::translate("CTeacherManage", "\350\256\276\347\275\256\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8));
        actionSetFault->setText(QApplication::translate("CTeacherManage", "\350\256\276\347\275\256\346\225\205\351\232\234", 0, QApplication::UnicodeUTF8));
        actionSetPointMask->setText(QApplication::translate("CTeacherManage", "\350\256\276\347\275\256\345\261\217\350\224\275\347\202\271", 0, QApplication::UnicodeUTF8));
        actionStudyRec->setText(QApplication::translate("CTeacherManage", "\345\255\246\345\221\230\345\255\246\344\271\240\350\256\260\345\275\225\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
        action_2->setText(QApplication::translate("CTeacherManage", "\344\270\213\345\217\221\345\255\246\344\271\240\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        action_contestmanage->setText(QApplication::translate("CTeacherManage", "\347\253\236\350\265\233\345\272\223\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        action_competetionscore->setText(QApplication::translate("CTeacherManage", "\347\253\236\350\265\233\346\210\220\347\273\251\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
        action_sendcontest->setText(QApplication::translate("CTeacherManage", "\347\253\236\350\265\233\345\217\221\345\270\203", 0, QApplication::UnicodeUTF8));
        action_doc_manage->setText(QApplication::translate("CTeacherManage", "\346\226\207\346\241\243\345\272\223\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        action_doc_study_plan->setText(QApplication::translate("CTeacherManage", "\345\255\246\344\271\240\350\256\241\345\210\222\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        actionBrodcast->setText(QApplication::translate("CTeacherManage", "\345\217\221\351\200\201\345\271\277\346\222\255\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        toolBar->setStyleSheet(QApplication::translate("CTeacherManage", "font: 12pt \"\345\256\213\344\275\223\";", 0, QApplication::UnicodeUTF8));
        menubar->setStyleSheet(QApplication::translate("CTeacherManage", "font: 10pt \"\345\256\213\344\275\223\";", 0, QApplication::UnicodeUTF8));
        menu_user->setTitle(QApplication::translate("CTeacherManage", "\345\255\246\345\221\230\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        menu_help->setTitle(QApplication::translate("CTeacherManage", "\345\270\256\345\212\251(Help)", 0, QApplication::UnicodeUTF8));
        menuTrain->setTitle(QApplication::translate("CTeacherManage", "\345\237\271\350\256\255\344\273\273\345\212\241\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        menu_file->setTitle(QApplication::translate("CTeacherManage", "\346\226\207\344\273\266(File)", 0, QApplication::UnicodeUTF8));
        menu_sys->setTitle(QApplication::translate("CTeacherManage", "\350\256\276\347\275\256(Setting)", 0, QApplication::UnicodeUTF8));
        menu_language->setTitle(QApplication::translate("CTeacherManage", "\350\257\255\350\250\200\345\210\207\346\215\242\357\274\210LANGUAGE\357\274\211", 0, QApplication::UnicodeUTF8));
        menu_exam->setTitle(QApplication::translate("CTeacherManage", "\344\270\223\344\270\232\347\237\245\350\257\206\350\200\203\346\240\270", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("CTeacherManage", "\345\255\246\344\271\240\346\226\207\346\241\243\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        menuMdlLib->setTitle(QApplication::translate("CTeacherManage", "\345\267\245\345\206\265\344\270\216\346\241\210\344\276\213\345\272\223", 0, QApplication::UnicodeUTF8));
        menuRecord->setTitle(QApplication::translate("CTeacherManage", "\345\216\206\345\217\262\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("CTeacherManage", "\347\253\236\350\265\233\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CTeacherManage: public Ui_CTeacherManage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMUTUTOR_H
