/********************************************************************************
** Form generated from reading ui file 'SimuTool.ui'
**
** Created: Wed Jan 14 10:22:36 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SIMUTOOL_H
#define UI_SIMUTOOL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QToolBox>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Simutool
{
public:
    QWidget *page_1;
    QGridLayout *gridLayout;
    QToolButton *button_userinfo;
    QToolButton *button_record;
    QToolButton *button_userscore;
    QToolButton *buttonMydocument;
    QToolButton *button_logout;
    QWidget *page_3;
    QGridLayout *gridLayout_2;
    QToolButton *button_task;
    QToolButton *button_revert;
    QToolButton *button_localOprate;
    QToolButton *button_Exam;
    QToolButton *button_openOix;
    QWidget *page_5;
    QGridLayout *gridLayout_3;
    QToolButton *button_simuControl;
    QToolButton *button_setScene;
    QToolButton *button_setFault;
    QToolButton *buttondocuments;
    QToolButton *toolButton_13;
    QWidget *page_4;
    QGridLayout *gridLayout_4;
    QToolButton *button_help;
    QToolButton *toolButton_18;
    QToolButton *button_sendMessage;
    QToolButton *button_setDesplay;
    QToolButton *toolButton_19;

    void setupUi(QToolBox *Simutool)
    {
        if (Simutool->objectName().isEmpty())
            Simutool->setObjectName(QString::fromUtf8("Simutool"));
        Simutool->resize(142, 684);
        Simutool->setCursor(QCursor(Qt::PointingHandCursor));
        Simutool->setMouseTracking(false);
        Simutool->setLayoutDirection(Qt::LeftToRight);
        Simutool->setStyleSheet(QString::fromUtf8(""));
        page_1 = new QWidget();
        page_1->setObjectName(QString::fromUtf8("page_1"));
        page_1->setGeometry(QRect(0, 0, 142, 576));
        page_1->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(page_1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        button_userinfo = new QToolButton(page_1);
        button_userinfo->setObjectName(QString::fromUtf8("button_userinfo"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(button_userinfo->sizePolicy().hasHeightForWidth());
        button_userinfo->setSizePolicy(sizePolicy);
        button_userinfo->setStyleSheet(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/pic/images/background/gerenxinxi.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_userinfo->setIcon(icon);
        button_userinfo->setIconSize(QSize(80, 80));
        button_userinfo->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(button_userinfo, 0, 0, 1, 1);

        button_record = new QToolButton(page_1);
        button_record->setObjectName(QString::fromUtf8("button_record"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(button_record->sizePolicy().hasHeightForWidth());
        button_record->setSizePolicy(sizePolicy1);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/pic/images/background/\346\223\215\344\275\234\350\256\260\345\275\225.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_record->setIcon(icon1);
        button_record->setIconSize(QSize(80, 80));
        button_record->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(button_record, 1, 0, 1, 1);

        button_userscore = new QToolButton(page_1);
        button_userscore->setObjectName(QString::fromUtf8("button_userscore"));
        sizePolicy.setHeightForWidth(button_userscore->sizePolicy().hasHeightForWidth());
        button_userscore->setSizePolicy(sizePolicy);
        button_userscore->setLayoutDirection(Qt::LeftToRight);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/pic/images/background/\345\216\206\345\217\262\346\210\220\347\273\251.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_userscore->setIcon(icon2);
        button_userscore->setIconSize(QSize(80, 80));
        button_userscore->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(button_userscore, 2, 0, 1, 1);

        buttonMydocument = new QToolButton(page_1);
        buttonMydocument->setObjectName(QString::fromUtf8("buttonMydocument"));
        sizePolicy.setHeightForWidth(buttonMydocument->sizePolicy().hasHeightForWidth());
        buttonMydocument->setSizePolicy(sizePolicy);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/pic/images/background/\346\210\221\347\232\204\346\226\207\346\241\243.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonMydocument->setIcon(icon3);
        buttonMydocument->setIconSize(QSize(80, 80));
        buttonMydocument->setAutoRepeat(false);
        buttonMydocument->setAutoExclusive(false);
        buttonMydocument->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        buttonMydocument->setArrowType(Qt::NoArrow);

        gridLayout->addWidget(buttonMydocument, 4, 0, 1, 1);

        button_logout = new QToolButton(page_1);
        button_logout->setObjectName(QString::fromUtf8("button_logout"));
        sizePolicy.setHeightForWidth(button_logout->sizePolicy().hasHeightForWidth());
        button_logout->setSizePolicy(sizePolicy);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/pic/images/background/\351\200\200\345\207\272\347\231\273\345\275\225.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_logout->setIcon(icon4);
        button_logout->setIconSize(QSize(80, 80));
        button_logout->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(button_logout, 5, 0, 1, 1);

        Simutool->addItem(page_1, QString::fromUtf8("\344\270\252\344\272\272\344\277\241\346\201\257\347\256\241\347\220\206"));
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        page_3->setGeometry(QRect(0, 0, 142, 576));
        gridLayout_2 = new QGridLayout(page_3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        button_task = new QToolButton(page_3);
        button_task->setObjectName(QString::fromUtf8("button_task"));
        sizePolicy.setHeightForWidth(button_task->sizePolicy().hasHeightForWidth());
        button_task->setSizePolicy(sizePolicy);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/pic/images/background/\344\273\273\345\212\241\345\210\227\350\241\250.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_task->setIcon(icon5);
        button_task->setIconSize(QSize(80, 80));
        button_task->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(button_task, 0, 0, 1, 1);

        button_revert = new QToolButton(page_3);
        button_revert->setObjectName(QString::fromUtf8("button_revert"));
        sizePolicy.setHeightForWidth(button_revert->sizePolicy().hasHeightForWidth());
        button_revert->setSizePolicy(sizePolicy);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/pic/images/background/\345\216\206\345\217\262\346\223\215\344\275\234\345\206\215\347\216\260.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_revert->setIcon(icon6);
        button_revert->setIconSize(QSize(80, 80));
        button_revert->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(button_revert, 1, 0, 1, 1);

        button_localOprate = new QToolButton(page_3);
        button_localOprate->setObjectName(QString::fromUtf8("button_localOprate"));
        sizePolicy.setHeightForWidth(button_localOprate->sizePolicy().hasHeightForWidth());
        button_localOprate->setSizePolicy(sizePolicy);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/pic/images/background/\350\277\220\350\241\214\346\223\215\344\275\234.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_localOprate->setIcon(icon7);
        button_localOprate->setIconSize(QSize(80, 80));
        button_localOprate->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(button_localOprate, 3, 0, 1, 1);

        button_Exam = new QToolButton(page_3);
        button_Exam->setObjectName(QString::fromUtf8("button_Exam"));
        sizePolicy.setHeightForWidth(button_Exam->sizePolicy().hasHeightForWidth());
        button_Exam->setSizePolicy(sizePolicy);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/pic/images/background/\347\237\245\350\257\206\350\200\203\350\257\225.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_Exam->setIcon(icon8);
        button_Exam->setIconSize(QSize(80, 80));
        button_Exam->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(button_Exam, 5, 0, 1, 1);

        button_openOix = new QToolButton(page_3);
        button_openOix->setObjectName(QString::fromUtf8("button_openOix"));
        sizePolicy.setHeightForWidth(button_openOix->sizePolicy().hasHeightForWidth());
        button_openOix->setSizePolicy(sizePolicy);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/pic/images/background/\346\223\215\344\275\234\347\224\273\351\235\242.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_openOix->setIcon(icon9);
        button_openOix->setIconSize(QSize(80, 80));
        button_openOix->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_2->addWidget(button_openOix, 2, 0, 1, 1);

        Simutool->addItem(page_3, QString::fromUtf8("\345\237\271\350\256\255\350\200\203\346\240\270\347\256\241\347\220\206"));
        page_5 = new QWidget();
        page_5->setObjectName(QString::fromUtf8("page_5"));
        page_5->setGeometry(QRect(0, 0, 142, 576));
        gridLayout_3 = new QGridLayout(page_5);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        button_simuControl = new QToolButton(page_5);
        button_simuControl->setObjectName(QString::fromUtf8("button_simuControl"));
        sizePolicy.setHeightForWidth(button_simuControl->sizePolicy().hasHeightForWidth());
        button_simuControl->setSizePolicy(sizePolicy);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/pic/images/background/\351\200\237\345\272\246\351\200\211\346\213\251.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_simuControl->setIcon(icon10);
        button_simuControl->setIconSize(QSize(80, 80));
        button_simuControl->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_3->addWidget(button_simuControl, 0, 0, 1, 1);

        button_setScene = new QToolButton(page_5);
        button_setScene->setObjectName(QString::fromUtf8("button_setScene"));
        sizePolicy.setHeightForWidth(button_setScene->sizePolicy().hasHeightForWidth());
        button_setScene->setSizePolicy(sizePolicy);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/pic/images/background/\345\234\272\346\231\257\345\212\240\350\275\275.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_setScene->setIcon(icon11);
        button_setScene->setIconSize(QSize(80, 80));
        button_setScene->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_3->addWidget(button_setScene, 1, 0, 1, 1);

        button_setFault = new QToolButton(page_5);
        button_setFault->setObjectName(QString::fromUtf8("button_setFault"));
        sizePolicy.setHeightForWidth(button_setFault->sizePolicy().hasHeightForWidth());
        button_setFault->setSizePolicy(sizePolicy);
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/pic/images/background/\346\225\205\351\232\234\350\256\276\347\275\2563.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_setFault->setIcon(icon12);
        button_setFault->setIconSize(QSize(80, 80));
        button_setFault->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_3->addWidget(button_setFault, 2, 0, 1, 1);

        buttondocuments = new QToolButton(page_5);
        buttondocuments->setObjectName(QString::fromUtf8("buttondocuments"));
        sizePolicy.setHeightForWidth(buttondocuments->sizePolicy().hasHeightForWidth());
        buttondocuments->setSizePolicy(sizePolicy);
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/pic/images/background/\345\255\246\344\271\240\346\226\207\346\241\243.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttondocuments->setIcon(icon13);
        buttondocuments->setIconSize(QSize(80, 80));
        buttondocuments->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_3->addWidget(buttondocuments, 3, 0, 1, 1);

        toolButton_13 = new QToolButton(page_5);
        toolButton_13->setObjectName(QString::fromUtf8("toolButton_13"));
        sizePolicy.setHeightForWidth(toolButton_13->sizePolicy().hasHeightForWidth());
        toolButton_13->setSizePolicy(sizePolicy);
        toolButton_13->setLayoutDirection(Qt::LeftToRight);

        gridLayout_3->addWidget(toolButton_13, 4, 0, 1, 1);

        Simutool->addItem(page_5, QString::fromUtf8("\344\273\277\347\234\237\345\267\245\345\205\267\346\240\217"));
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        page_4->setGeometry(QRect(0, 0, 142, 576));
        gridLayout_4 = new QGridLayout(page_4);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        button_help = new QToolButton(page_4);
        button_help->setObjectName(QString::fromUtf8("button_help"));
        sizePolicy.setHeightForWidth(button_help->sizePolicy().hasHeightForWidth());
        button_help->setSizePolicy(sizePolicy);
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/pic/images/background/\345\270\256\345\212\251\344\277\241\346\201\257.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_help->setIcon(icon14);
        button_help->setIconSize(QSize(80, 80));
        button_help->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_4->addWidget(button_help, 3, 0, 1, 1);

        toolButton_18 = new QToolButton(page_4);
        toolButton_18->setObjectName(QString::fromUtf8("toolButton_18"));
        sizePolicy.setHeightForWidth(toolButton_18->sizePolicy().hasHeightForWidth());
        toolButton_18->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(toolButton_18, 6, 0, 1, 1);

        button_sendMessage = new QToolButton(page_4);
        button_sendMessage->setObjectName(QString::fromUtf8("button_sendMessage"));
        sizePolicy.setHeightForWidth(button_sendMessage->sizePolicy().hasHeightForWidth());
        button_sendMessage->setSizePolicy(sizePolicy);
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/pic/images/background/\350\201\224\347\263\273\346\225\231\345\221\230.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_sendMessage->setIcon(icon15);
        button_sendMessage->setIconSize(QSize(80, 80));
        button_sendMessage->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_4->addWidget(button_sendMessage, 1, 0, 1, 1);

        button_setDesplay = new QToolButton(page_4);
        button_setDesplay->setObjectName(QString::fromUtf8("button_setDesplay"));
        sizePolicy.setHeightForWidth(button_setDesplay->sizePolicy().hasHeightForWidth());
        button_setDesplay->setSizePolicy(sizePolicy);
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/pic/images/background/\347\263\273\347\273\237\350\256\276\347\275\256.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_setDesplay->setIcon(icon16);
        button_setDesplay->setIconSize(QSize(80, 80));
        button_setDesplay->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_4->addWidget(button_setDesplay, 2, 0, 1, 1);

        toolButton_19 = new QToolButton(page_4);
        toolButton_19->setObjectName(QString::fromUtf8("toolButton_19"));
        sizePolicy.setHeightForWidth(toolButton_19->sizePolicy().hasHeightForWidth());
        toolButton_19->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(toolButton_19, 5, 0, 1, 1);

        Simutool->addItem(page_4, QString::fromUtf8("\347\263\273\347\273\237\345\267\245\345\205\267"));

        retranslateUi(Simutool);

        Simutool->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Simutool);
    } // setupUi

    void retranslateUi(QToolBox *Simutool)
    {
        Simutool->setWindowTitle(QApplication::translate("Simutool", "\345\255\246\345\221\230\347\253\231", 0, QApplication::UnicodeUTF8));
        button_userinfo->setText(QApplication::translate("Simutool", "\345\237\272\346\234\254\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        button_record->setText(QApplication::translate("Simutool", "\346\223\215\344\275\234\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        button_userscore->setText(QApplication::translate("Simutool", "\345\216\206\345\217\262\346\210\220\347\273\251", 0, QApplication::UnicodeUTF8));
        buttonMydocument->setText(QApplication::translate("Simutool", "\346\210\221\347\232\204\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
        button_logout->setText(QApplication::translate("Simutool", "\351\200\200\345\207\272\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
        Simutool->setItemText(Simutool->indexOf(page_1), QApplication::translate("Simutool", "\344\270\252\344\272\272\344\277\241\346\201\257\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        button_task->setText(QApplication::translate("Simutool", "\345\275\223\345\211\215\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        button_revert->setText(QApplication::translate("Simutool", "\345\216\206\345\217\262\346\223\215\344\275\234\345\206\215\347\216\260", 0, QApplication::UnicodeUTF8));
        button_localOprate->setText(QApplication::translate("Simutool", "\350\277\220\350\241\214\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8));
        button_Exam->setText(QApplication::translate("Simutool", "\347\237\245\350\257\206\350\200\203\350\257\225", 0, QApplication::UnicodeUTF8));
        button_openOix->setText(QApplication::translate("Simutool", "\347\233\221\346\216\247\346\250\241\346\213\237\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8));
        Simutool->setItemText(Simutool->indexOf(page_3), QApplication::translate("Simutool", "\345\237\271\350\256\255\350\200\203\346\240\270\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        button_simuControl->setText(QApplication::translate("Simutool", "\344\273\277\347\234\237\350\277\233\345\272\246\346\216\247\345\210\266", 0, QApplication::UnicodeUTF8));
        button_setScene->setText(QApplication::translate("Simutool", "\345\234\272\346\231\257\345\212\240\350\275\275", 0, QApplication::UnicodeUTF8));
        button_setFault->setText(QApplication::translate("Simutool", "\346\225\205\351\232\234\345\212\240\350\275\275", 0, QApplication::UnicodeUTF8));
        buttondocuments->setText(QApplication::translate("Simutool", "\345\255\246\344\271\240\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
        toolButton_13->setText(QApplication::translate("Simutool", "...", 0, QApplication::UnicodeUTF8));
        Simutool->setItemText(Simutool->indexOf(page_5), QApplication::translate("Simutool", "\344\273\277\347\234\237\345\267\245\345\205\267\346\240\217", 0, QApplication::UnicodeUTF8));
        button_help->setText(QApplication::translate("Simutool", "\345\270\256\345\212\251\347\263\273\347\273\237", 0, QApplication::UnicodeUTF8));
        toolButton_18->setText(QApplication::translate("Simutool", "\345\276\205\346\211\251\345\261\225", 0, QApplication::UnicodeUTF8));
        button_sendMessage->setText(QApplication::translate("Simutool", "\345\215\263\346\227\266\351\200\232\350\256\257", 0, QApplication::UnicodeUTF8));
        button_setDesplay->setText(QApplication::translate("Simutool", "\346\230\276\347\244\272\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        toolButton_19->setText(QApplication::translate("Simutool", "\345\276\205\346\211\251\345\261\225", 0, QApplication::UnicodeUTF8));
        Simutool->setItemText(Simutool->indexOf(page_4), QApplication::translate("Simutool", "\347\263\273\347\273\237\345\267\245\345\205\267", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Simutool);
    } // retranslateUi

};

namespace Ui {
    class Simutool: public Ui_Simutool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMUTOOL_H
