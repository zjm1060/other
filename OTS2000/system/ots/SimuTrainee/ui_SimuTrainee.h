/********************************************************************************
** Form generated from reading ui file 'SimuTrainee.ui'
**
** Created: Wed Jan 14 10:22:37 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SIMUTRAINEE_H
#define UI_SIMUTRAINEE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SimuTrainee
{
public:
    QAction *action_logout;
    QAction *action_oprrecord;
    QAction *action_studyrecord;
    QAction *action_hisscoe;
    QAction *action_settask;
    QAction *action_loadscene;
    QAction *action_setfault;
    QAction *action_simustart;
    QAction *action_simustop;
    QAction *action0_125X;
    QAction *action0_25X;
    QAction *action0_5X;
    QAction *action1X;
    QAction *action2X;
    QAction *action4X;
    QAction *action_taketool;
    QAction *action_report;
    QAction *action_otheropr;
    QAction *action8X;
    QAction *action_OTS2000;
    QAction *action_help;
    QAction *action_simpstyle;
    QAction *action_elegantstyle;
    QAction *action_ontop;
    QAction *action_defaultstyle;
    QAction *action_CTRL_H;
    QAction *action_CTRL_L;
    QAction *action_Chinese;
    QAction *action_English;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_8;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QMenu *menu_5;
    QMenu *menu_6;
    QMenu *menu_7;
    QMenu *menu_language;
    QToolBar *toolBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;

    void setupUi(QMainWindow *SimuTrainee)
    {
        if (SimuTrainee->objectName().isEmpty())
            SimuTrainee->setObjectName(QString::fromUtf8("SimuTrainee"));
        SimuTrainee->resize(800, 600);
        SimuTrainee->setMinimumSize(QSize(800, 600));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/pic/images/ICO/ots2.ico"), QSize(), QIcon::Normal, QIcon::Off);
        SimuTrainee->setWindowIcon(icon);
        SimuTrainee->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        SimuTrainee->setDocumentMode(false);
        SimuTrainee->setTabShape(QTabWidget::Triangular);
        SimuTrainee->setDockNestingEnabled(false);
        SimuTrainee->setDockOptions(QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks|QMainWindow::VerticalTabs);
        action_logout = new QAction(SimuTrainee);
        action_logout->setObjectName(QString::fromUtf8("action_logout"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/pic/images/background/\351\200\200\345\207\272\347\231\273\345\275\225.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_logout->setIcon(icon1);
        action_logout->setMenuRole(QAction::TextHeuristicRole);
        action_oprrecord = new QAction(SimuTrainee);
        action_oprrecord->setObjectName(QString::fromUtf8("action_oprrecord"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/pic/images/background/\346\223\215\344\275\234\350\256\260\345\275\225.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_oprrecord->setIcon(icon2);
        action_studyrecord = new QAction(SimuTrainee);
        action_studyrecord->setObjectName(QString::fromUtf8("action_studyrecord"));
        action_hisscoe = new QAction(SimuTrainee);
        action_hisscoe->setObjectName(QString::fromUtf8("action_hisscoe"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/pic/images/background/\345\216\206\345\217\262\346\210\220\347\273\251.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_hisscoe->setIcon(icon3);
        action_settask = new QAction(SimuTrainee);
        action_settask->setObjectName(QString::fromUtf8("action_settask"));
        action_loadscene = new QAction(SimuTrainee);
        action_loadscene->setObjectName(QString::fromUtf8("action_loadscene"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/pic/images/background/\345\234\272\346\231\257\345\212\240\350\275\275.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_loadscene->setIcon(icon4);
        action_setfault = new QAction(SimuTrainee);
        action_setfault->setObjectName(QString::fromUtf8("action_setfault"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/pic/images/background/\346\225\205\351\232\234\350\256\276\347\275\256.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_setfault->setIcon(icon5);
        action_simustart = new QAction(SimuTrainee);
        action_simustart->setObjectName(QString::fromUtf8("action_simustart"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/pic/images/\346\222\255\346\224\276.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        action_simustart->setIcon(icon6);
        action_simustop = new QAction(SimuTrainee);
        action_simustop->setObjectName(QString::fromUtf8("action_simustop"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/pic/images/\345\201\234\346\255\242.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        action_simustop->setIcon(icon7);
        action0_125X = new QAction(SimuTrainee);
        action0_125X->setObjectName(QString::fromUtf8("action0_125X"));
        action0_25X = new QAction(SimuTrainee);
        action0_25X->setObjectName(QString::fromUtf8("action0_25X"));
        action0_5X = new QAction(SimuTrainee);
        action0_5X->setObjectName(QString::fromUtf8("action0_5X"));
        action1X = new QAction(SimuTrainee);
        action1X->setObjectName(QString::fromUtf8("action1X"));
        action2X = new QAction(SimuTrainee);
        action2X->setObjectName(QString::fromUtf8("action2X"));
        action4X = new QAction(SimuTrainee);
        action4X->setObjectName(QString::fromUtf8("action4X"));
        action_taketool = new QAction(SimuTrainee);
        action_taketool->setObjectName(QString::fromUtf8("action_taketool"));
        action_report = new QAction(SimuTrainee);
        action_report->setObjectName(QString::fromUtf8("action_report"));
        action_otheropr = new QAction(SimuTrainee);
        action_otheropr->setObjectName(QString::fromUtf8("action_otheropr"));
        action8X = new QAction(SimuTrainee);
        action8X->setObjectName(QString::fromUtf8("action8X"));
        action_OTS2000 = new QAction(SimuTrainee);
        action_OTS2000->setObjectName(QString::fromUtf8("action_OTS2000"));
        action_help = new QAction(SimuTrainee);
        action_help->setObjectName(QString::fromUtf8("action_help"));
        action_simpstyle = new QAction(SimuTrainee);
        action_simpstyle->setObjectName(QString::fromUtf8("action_simpstyle"));
        action_elegantstyle = new QAction(SimuTrainee);
        action_elegantstyle->setObjectName(QString::fromUtf8("action_elegantstyle"));
        action_ontop = new QAction(SimuTrainee);
        action_ontop->setObjectName(QString::fromUtf8("action_ontop"));
        action_ontop->setCheckable(true);
        action_defaultstyle = new QAction(SimuTrainee);
        action_defaultstyle->setObjectName(QString::fromUtf8("action_defaultstyle"));
        action_CTRL_H = new QAction(SimuTrainee);
        action_CTRL_H->setObjectName(QString::fromUtf8("action_CTRL_H"));
        action_CTRL_L = new QAction(SimuTrainee);
        action_CTRL_L->setObjectName(QString::fromUtf8("action_CTRL_L"));
        action_Chinese = new QAction(SimuTrainee);
        action_Chinese->setObjectName(QString::fromUtf8("action_Chinese"));
        action_English = new QAction(SimuTrainee);
        action_English->setObjectName(QString::fromUtf8("action_English"));
        centralwidget = new QWidget(SimuTrainee);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        SimuTrainee->setCentralWidget(centralwidget);
        menubar = new QMenuBar(SimuTrainee);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 24));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_8 = new QMenu(menu);
        menu_8->setObjectName(QString::fromUtf8("menu_8"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        menu_4 = new QMenu(menubar);
        menu_4->setObjectName(QString::fromUtf8("menu_4"));
        menu_5 = new QMenu(menu_4);
        menu_5->setObjectName(QString::fromUtf8("menu_5"));
        menu_6 = new QMenu(menubar);
        menu_6->setObjectName(QString::fromUtf8("menu_6"));
        menu_7 = new QMenu(menubar);
        menu_7->setObjectName(QString::fromUtf8("menu_7"));
        menu_language = new QMenu(menu_7);
        menu_language->setObjectName(QString::fromUtf8("menu_language"));
        SimuTrainee->setMenuBar(menubar);
        toolBar = new QToolBar(SimuTrainee);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMinimumSize(QSize(0, 0));
        toolBar->setContextMenuPolicy(Qt::ActionsContextMenu);
        SimuTrainee->addToolBar(Qt::TopToolBarArea, toolBar);
        dockWidget = new QDockWidget(SimuTrainee);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMinimumSize(QSize(155, 500));
        dockWidget->setMaximumSize(QSize(155, 524287));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        dockWidget->setWidget(dockWidgetContents);
        SimuTrainee->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menubar->addAction(menu_4->menuAction());
        menubar->addAction(menu_6->menuAction());
        menubar->addAction(menu_7->menuAction());
        menu->addAction(menu_8->menuAction());
        menu->addAction(action_ontop);
        menu->addAction(action_CTRL_H);
        menu->addAction(action_CTRL_L);
        menu->addAction(action_logout);
        menu_8->addAction(action_simpstyle);
        menu_8->addAction(action_elegantstyle);
        menu_8->addAction(action_defaultstyle);
        menu_2->addAction(action_oprrecord);
        menu_2->addAction(action_studyrecord);
        menu_2->addAction(action_hisscoe);
        menu_3->addAction(action_settask);
        menu_3->addAction(action_loadscene);
        menu_3->addAction(action_setfault);
        menu_4->addAction(action_simustart);
        menu_4->addAction(action_simustop);
        menu_4->addAction(menu_5->menuAction());
        menu_5->addAction(action0_125X);
        menu_5->addAction(action0_25X);
        menu_5->addAction(action0_5X);
        menu_5->addAction(action1X);
        menu_5->addAction(action2X);
        menu_5->addAction(action4X);
        menu_5->addAction(action8X);
        menu_6->addAction(action_taketool);
        menu_6->addAction(action_report);
        menu_6->addAction(action_otheropr);
        menu_7->addAction(action_OTS2000);
        menu_7->addAction(action_help);
        menu_7->addAction(menu_language->menuAction());
        menu_language->addAction(action_Chinese);
        menu_language->addAction(action_English);

        retranslateUi(SimuTrainee);

        QMetaObject::connectSlotsByName(SimuTrainee);
    } // setupUi

    void retranslateUi(QMainWindow *SimuTrainee)
    {
        SimuTrainee->setWindowTitle(QApplication::translate("SimuTrainee", "\345\255\246\345\221\230\347\253\231", 0, QApplication::UnicodeUTF8));
        action_logout->setText(QApplication::translate("SimuTrainee", "\351\200\200\345\207\272\347\231\273\345\275\225        (Esc)", 0, QApplication::UnicodeUTF8));
        action_logout->setShortcut(QApplication::translate("SimuTrainee", "Esc", 0, QApplication::UnicodeUTF8));
        action_oprrecord->setText(QApplication::translate("SimuTrainee", "\346\223\215\344\275\234\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        action_studyrecord->setText(QApplication::translate("SimuTrainee", "\345\255\246\344\271\240\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        action_hisscoe->setText(QApplication::translate("SimuTrainee", "\345\216\206\345\217\262\346\210\220\347\273\251", 0, QApplication::UnicodeUTF8));
        action_settask->setText(QApplication::translate("SimuTrainee", "\346\223\215\344\275\234\344\273\273\345\212\241\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        action_loadscene->setText(QApplication::translate("SimuTrainee", "\345\267\245\345\206\265\345\212\240\350\275\275", 0, QApplication::UnicodeUTF8));
        action_setfault->setText(QApplication::translate("SimuTrainee", "\346\225\205\351\232\234\345\212\240\350\275\275", 0, QApplication::UnicodeUTF8));
        action_simustart->setText(QApplication::translate("SimuTrainee", "\350\277\220\350\241\214\346\250\241\345\236\213(CTRL+R)", 0, QApplication::UnicodeUTF8));
        action_simustart->setShortcut(QApplication::translate("SimuTrainee", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        action_simustop->setText(QApplication::translate("SimuTrainee", "\345\201\234\346\255\242\344\273\277\347\234\237(CTRL+S)", 0, QApplication::UnicodeUTF8));
        action_simustop->setShortcut(QApplication::translate("SimuTrainee", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        action0_125X->setText(QApplication::translate("SimuTrainee", "0.125X", 0, QApplication::UnicodeUTF8));
        action0_25X->setText(QApplication::translate("SimuTrainee", "0.25X", 0, QApplication::UnicodeUTF8));
        action0_5X->setText(QApplication::translate("SimuTrainee", "0.5X", 0, QApplication::UnicodeUTF8));
        action1X->setText(QApplication::translate("SimuTrainee", "1X", 0, QApplication::UnicodeUTF8));
        action2X->setText(QApplication::translate("SimuTrainee", "2X", 0, QApplication::UnicodeUTF8));
        action4X->setText(QApplication::translate("SimuTrainee", "4X", 0, QApplication::UnicodeUTF8));
        action_taketool->setText(QApplication::translate("SimuTrainee", "\345\270\246\345\267\245\345\205\267", 0, QApplication::UnicodeUTF8));
        action_report->setText(QApplication::translate("SimuTrainee", "\346\261\207\346\212\245", 0, QApplication::UnicodeUTF8));
        action_otheropr->setText(QApplication::translate("SimuTrainee", "\345\205\266\344\273\226\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8));
        action8X->setText(QApplication::translate("SimuTrainee", "8X", 0, QApplication::UnicodeUTF8));
        action_OTS2000->setText(QApplication::translate("SimuTrainee", "\345\205\263\344\272\216OTS2000", 0, QApplication::UnicodeUTF8));
        action_help->setText(QApplication::translate("SimuTrainee", "\345\270\256\345\212\251\346\226\207\346\241\243(F1)", 0, QApplication::UnicodeUTF8));
        action_help->setShortcut(QApplication::translate("SimuTrainee", "F1", 0, QApplication::UnicodeUTF8));
        action_simpstyle->setText(QApplication::translate("SimuTrainee", "\347\256\200\346\264\201", 0, QApplication::UnicodeUTF8));
        action_elegantstyle->setText(QApplication::translate("SimuTrainee", "\345\205\270\351\233\205", 0, QApplication::UnicodeUTF8));
        action_ontop->setText(QApplication::translate("SimuTrainee", "\347\224\273\351\235\242\347\275\256\351\241\266", 0, QApplication::UnicodeUTF8));
        action_defaultstyle->setText(QApplication::translate("SimuTrainee", "\347\263\273\347\273\237\351\273\230\350\256\244", 0, QApplication::UnicodeUTF8));
        action_CTRL_H->setText(QApplication::translate("SimuTrainee", "\346\224\266\350\265\267/\345\261\225\345\274\200\345\267\245\345\205\267\346\235\241 (CTRL+H)", 0, QApplication::UnicodeUTF8));
        action_CTRL_H->setShortcut(QApplication::translate("SimuTrainee", "Ctrl+H", 0, QApplication::UnicodeUTF8));
        action_CTRL_L->setText(QApplication::translate("SimuTrainee", "\346\224\266\350\265\267\344\276\247\345\267\245\345\205\267\346\240\217  (CTRL+L)", 0, QApplication::UnicodeUTF8));
        action_CTRL_L->setShortcut(QApplication::translate("SimuTrainee", "Ctrl+L", 0, QApplication::UnicodeUTF8));
        action_Chinese->setText(QApplication::translate("SimuTrainee", "\344\270\255\346\226\207", 0, QApplication::UnicodeUTF8));
        action_English->setText(QApplication::translate("SimuTrainee", "english", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("SimuTrainee", "\347\263\273\347\273\237", 0, QApplication::UnicodeUTF8));
        menu_8->setTitle(QApplication::translate("SimuTrainee", "\347\225\214\351\235\242\351\243\216\346\240\274", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("SimuTrainee", "\344\270\252\344\272\272\344\277\241\346\201\257\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
        menu_3->setTitle(QApplication::translate("SimuTrainee", "\346\223\215\344\275\234\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        menu_4->setTitle(QApplication::translate("SimuTrainee", "\344\273\277\347\234\237\350\277\220\350\241\214\346\216\247\345\210\266", 0, QApplication::UnicodeUTF8));
        menu_5->setTitle(QApplication::translate("SimuTrainee", "\344\273\277\347\234\237\351\200\237\345\272\246\350\260\203\350\212\202", 0, QApplication::UnicodeUTF8));
        menu_6->setTitle(QApplication::translate("SimuTrainee", "\346\223\215\344\275\234\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        menu_7->setTitle(QApplication::translate("SimuTrainee", "\345\270\256\345\212\251", 0, QApplication::UnicodeUTF8));
        menu_language->setTitle(QApplication::translate("SimuTrainee", "\350\257\255\350\250\200\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("SimuTrainee", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SimuTrainee: public Ui_SimuTrainee {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMUTRAINEE_H
