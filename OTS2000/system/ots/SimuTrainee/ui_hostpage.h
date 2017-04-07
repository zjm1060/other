/********************************************************************************
** Form generated from reading ui file 'hostpage.ui'
**
** Created: Wed Jan 14 10:22:28 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_HOSTPAGE_H
#define UI_HOSTPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_hostpage
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QFrame *frame;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_12;
    QSpacerItem *verticalSpacer_6;
    QGridLayout *gridLayout_6;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QSpacerItem *verticalSpacer_7;
    QLabel *label_5;
    QSpacerItem *verticalSpacer_8;
    QLabel *label_3;
    QSpacerItem *verticalSpacer_9;
    QLabel *label_10;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_username;
    QSpacerItem *verticalSpacer_10;
    QLabel *label_usertype;
    QSpacerItem *verticalSpacer_11;
    QLabel *label_userrange;
    QSpacerItem *verticalSpacer_12;
    QLabel *label_usermode;
    QSpacerItem *horizontalSpacer_11;
    QSpacerItem *verticalSpacer_13;
    QToolButton *toolButton_personinfo;
    QSpacerItem *verticalSpacer_14;
    QSpacerItem *verticalSpacer_5;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_13;
    QSpacerItem *verticalSpacer_15;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_4;
    QSpacerItem *verticalSpacer_16;
    QLabel *label_6;
    QSpacerItem *verticalSpacer_17;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_4;
    QPushButton *pushButton_oprrecord;
    QSpacerItem *verticalSpacer_18;
    QPushButton *pushButton_studyrecord;
    QSpacerItem *verticalSpacer_19;
    QPushButton *pushButton_scorerecord;
    QSpacerItem *horizontalSpacer_13;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_8;
    QToolButton *toolButton_ModeS;
    QToolButton *toolButton_ModeE;
    QToolButton *toolButton_ModeC;
    QSpacerItem *horizontalSpacer_6;
    QVBoxLayout *verticalLayout_10;
    QVBoxLayout *verticalLayout_6;
    QSpacerItem *verticalSpacer_22;
    QRadioButton *radioButton_ST;
    QRadioButton *radioButton_SO;
    QSpacerItem *verticalSpacer_20;
    QVBoxLayout *verticalLayout_7;
    QRadioButton *radioButton_ET;
    QRadioButton *radioButton_EO;
    QSpacerItem *verticalSpacer_21;
    QLabel *label_ContestNum;
    QSpacerItem *verticalSpacer_23;
    QSpacerItem *horizontalSpacer_7;
    QVBoxLayout *verticalLayout_9;
    QSpacerItem *verticalSpacer_26;
    QPushButton *pushButton_EtStudy;
    QSpacerItem *verticalSpacer_3;
    QPushButton *pushButton_EtExam;
    QSpacerItem *verticalSpacer_25;
    QPushButton *pushButton_EtComplet;
    QSpacerItem *verticalSpacer_27;
    QSpacerItem *horizontalSpacer_10;
    QSpacerItem *verticalSpacer_28;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_11;
    QSpacerItem *verticalSpacer_24;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_8;
    QLabel *label_8;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QWidget *hostpage)
    {
        if (hostpage->objectName().isEmpty())
            hostpage->setObjectName(QString::fromUtf8("hostpage"));
        hostpage->resize(1116, 904);
        hostpage->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";\n"
"background-image: url(:/pic/images/background/hostpagebackround.png);\n"
"border-color: rgb(150 194,219);\n"
"background-color: rgb(231, 240, 245);\n"
""));
        gridLayout = new QGridLayout(hostpage);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalSpacer = new QSpacerItem(957, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 0, 1, 1, 1);

        frame = new QFrame(hostpage);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(800, 31));
        frame->setMaximumSize(QSize(805, 32));
        frame->setStyleSheet(QString::fromUtf8("background-image: url(:/pic/images/background/\346\254\242\350\277\216\350\277\233\345\205\245\346\226\207\345\255\227.png);\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 0), stop:1 rgba(255, 255, 255,0));"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(frame, 1, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 3, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(957, 17, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 2, 1, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setSizeConstraint(QLayout::SetMinAndMaxSize);
        groupBox = new QGroupBox(hostpage);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8("font: 75 16pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"background-image: url(:/pic/images/background/\351\200\217\346\230\216\345\272\225\345\233\2761.png);\n"
"color: rgb(255, 0, 0);\n"
""));
        verticalLayout_12 = new QVBoxLayout(groupBox);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        verticalSpacer_6 = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_12->addItem(verticalSpacer_6);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout->addWidget(label);

        verticalSpacer_7 = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout->addItem(verticalSpacer_7);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout->addWidget(label_5);

        verticalSpacer_8 = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout->addItem(verticalSpacer_8);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout->addWidget(label_3);

        verticalSpacer_9 = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout->addItem(verticalSpacer_9);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout->addWidget(label_10);


        gridLayout_6->addLayout(verticalLayout, 0, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_5, 0, 1, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_username = new QLabel(groupBox);
        label_username->setObjectName(QString::fromUtf8("label_username"));
        label_username->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout_2->addWidget(label_username);

        verticalSpacer_10 = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout_2->addItem(verticalSpacer_10);

        label_usertype = new QLabel(groupBox);
        label_usertype->setObjectName(QString::fromUtf8("label_usertype"));
        label_usertype->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout_2->addWidget(label_usertype);

        verticalSpacer_11 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout_2->addItem(verticalSpacer_11);

        label_userrange = new QLabel(groupBox);
        label_userrange->setObjectName(QString::fromUtf8("label_userrange"));
        label_userrange->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout_2->addWidget(label_userrange);

        verticalSpacer_12 = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout_2->addItem(verticalSpacer_12);

        label_usermode = new QLabel(groupBox);
        label_usermode->setObjectName(QString::fromUtf8("label_usermode"));
        label_usermode->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout_2->addWidget(label_usermode);


        gridLayout_6->addLayout(verticalLayout_2, 0, 2, 1, 1);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_11, 0, 3, 1, 1);


        verticalLayout_12->addLayout(gridLayout_6);

        verticalSpacer_13 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_12->addItem(verticalSpacer_13);

        toolButton_personinfo = new QToolButton(groupBox);
        toolButton_personinfo->setObjectName(QString::fromUtf8("toolButton_personinfo"));
        toolButton_personinfo->setStyleSheet(QString::fromUtf8("    font: 13pt;\n"
"    color: rgb(7, 100, 143);"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/pic/images/\346\234\252\346\240\207\351\242\230-8.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_personinfo->setIcon(icon);
        toolButton_personinfo->setIconSize(QSize(20, 20));
        toolButton_personinfo->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        verticalLayout_12->addWidget(toolButton_personinfo);

        verticalSpacer_14 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_12->addItem(verticalSpacer_14);


        verticalLayout_5->addWidget(groupBox);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_5);

        groupBox_2 = new QGroupBox(hostpage);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setStyleSheet(QString::fromUtf8("background-image: url(:/pic/images/background/\351\200\217\346\230\216\345\272\225\345\233\2761.png);\n"
"font: 75 16pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(255, 0, 0);"));
        verticalLayout_13 = new QVBoxLayout(groupBox_2);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        verticalSpacer_15 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_13->addItem(verticalSpacer_15);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout_3->addWidget(label_4);

        verticalSpacer_16 = new QSpacerItem(10, 25, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer_16);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout_3->addWidget(label_6);

        verticalSpacer_17 = new QSpacerItem(20, 25, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer_17);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout_3->addWidget(label_7);


        horizontalLayout_2->addLayout(verticalLayout_3);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        pushButton_oprrecord = new QPushButton(groupBox_2);
        pushButton_oprrecord->setObjectName(QString::fromUtf8("pushButton_oprrecord"));
        pushButton_oprrecord->setStyleSheet(QString::fromUtf8("    font: 13pt;\n"
"    color: rgb(7, 100, 143);"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/pic/images/(09,13).png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_oprrecord->setIcon(icon1);
        pushButton_oprrecord->setIconSize(QSize(20, 20));

        verticalLayout_4->addWidget(pushButton_oprrecord);

        verticalSpacer_18 = new QSpacerItem(10, 25, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer_18);

        pushButton_studyrecord = new QPushButton(groupBox_2);
        pushButton_studyrecord->setObjectName(QString::fromUtf8("pushButton_studyrecord"));
        pushButton_studyrecord->setStyleSheet(QString::fromUtf8("    font: 13pt;\n"
"    color: rgb(7, 100, 143);"));
        pushButton_studyrecord->setIcon(icon1);
        pushButton_studyrecord->setIconSize(QSize(20, 20));

        verticalLayout_4->addWidget(pushButton_studyrecord);

        verticalSpacer_19 = new QSpacerItem(10, 25, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer_19);

        pushButton_scorerecord = new QPushButton(groupBox_2);
        pushButton_scorerecord->setObjectName(QString::fromUtf8("pushButton_scorerecord"));
        pushButton_scorerecord->setStyleSheet(QString::fromUtf8("    font: 13pt;\n"
"    color: rgb(7, 100, 143);"));
        pushButton_scorerecord->setIcon(icon1);
        pushButton_scorerecord->setIconSize(QSize(20, 20));

        verticalLayout_4->addWidget(pushButton_scorerecord);


        horizontalLayout_2->addLayout(verticalLayout_4);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_13);


        verticalLayout_13->addLayout(horizontalLayout_2);


        verticalLayout_5->addWidget(groupBox_2);


        horizontalLayout_4->addLayout(verticalLayout_5);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        groupBox_3 = new QGroupBox(hostpage);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setStyleSheet(QString::fromUtf8("background-image: url(:/pic/images/background/\351\200\217\346\230\216\345\272\225\345\233\2761.png);\n"
"font: 75 16pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(255, 0, 0);"));
        gridLayout_4 = new QGridLayout(groupBox_3);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        toolButton_ModeS = new QToolButton(groupBox_3);
        toolButton_ModeS->setObjectName(QString::fromUtf8("toolButton_ModeS"));
        toolButton_ModeS->setMinimumSize(QSize(110, 110));
        toolButton_ModeS->setStyleSheet(QString::fromUtf8("    font: 13pt;\n"
"    color: rgb(7, 100, 143);"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/pic/images/background/\345\255\246\344\271\240\345\233\276\346\240\207.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_ModeS->setIcon(icon2);
        toolButton_ModeS->setIconSize(QSize(60, 60));
        toolButton_ModeS->setCheckable(true);
        toolButton_ModeS->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        verticalLayout_8->addWidget(toolButton_ModeS);

        toolButton_ModeE = new QToolButton(groupBox_3);
        toolButton_ModeE->setObjectName(QString::fromUtf8("toolButton_ModeE"));
        toolButton_ModeE->setMinimumSize(QSize(110, 110));
        toolButton_ModeE->setStyleSheet(QString::fromUtf8("    font: 13pt;\n"
"    color: rgb(7, 100, 143);"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/pic/images/background/\350\200\203\350\257\225\345\233\276\346\240\207.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_ModeE->setIcon(icon3);
        toolButton_ModeE->setIconSize(QSize(60, 60));
        toolButton_ModeE->setCheckable(true);
        toolButton_ModeE->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        verticalLayout_8->addWidget(toolButton_ModeE);

        toolButton_ModeC = new QToolButton(groupBox_3);
        toolButton_ModeC->setObjectName(QString::fromUtf8("toolButton_ModeC"));
        toolButton_ModeC->setMinimumSize(QSize(110, 110));
        toolButton_ModeC->setStyleSheet(QString::fromUtf8("    font: 13pt;\n"
"    color: rgb(7, 100, 143);"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/pic/images/background/\347\253\236\350\265\233\346\250\241\345\274\217.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_ModeC->setIcon(icon4);
        toolButton_ModeC->setIconSize(QSize(60, 60));
        toolButton_ModeC->setCheckable(true);
        toolButton_ModeC->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        verticalLayout_8->addWidget(toolButton_ModeC);


        horizontalLayout_3->addLayout(verticalLayout_8);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalSpacer_22 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_22);

        radioButton_ST = new QRadioButton(groupBox_3);
        radioButton_ST->setObjectName(QString::fromUtf8("radioButton_ST"));
        radioButton_ST->setEnabled(false);
        radioButton_ST->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"background-image: url(:/pic/images/background/\351\200\217\346\230\216\345\272\225\345\233\2761.png);\n"
"color: rgb(4, 30, 103);"));

        verticalLayout_6->addWidget(radioButton_ST);

        radioButton_SO = new QRadioButton(groupBox_3);
        radioButton_SO->setObjectName(QString::fromUtf8("radioButton_SO"));
        radioButton_SO->setEnabled(false);
        radioButton_SO->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout_6->addWidget(radioButton_SO);


        verticalLayout_10->addLayout(verticalLayout_6);

        verticalSpacer_20 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_10->addItem(verticalSpacer_20);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        radioButton_ET = new QRadioButton(groupBox_3);
        radioButton_ET->setObjectName(QString::fromUtf8("radioButton_ET"));
        radioButton_ET->setEnabled(false);
        radioButton_ET->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout_7->addWidget(radioButton_ET);

        radioButton_EO = new QRadioButton(groupBox_3);
        radioButton_EO->setObjectName(QString::fromUtf8("radioButton_EO"));
        radioButton_EO->setEnabled(false);
        radioButton_EO->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout_7->addWidget(radioButton_EO);


        verticalLayout_10->addLayout(verticalLayout_7);

        verticalSpacer_21 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_10->addItem(verticalSpacer_21);

        label_ContestNum = new QLabel(groupBox_3);
        label_ContestNum->setObjectName(QString::fromUtf8("label_ContestNum"));
        label_ContestNum->setMinimumSize(QSize(150, 0));
        label_ContestNum->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout_10->addWidget(label_ContestNum);

        verticalSpacer_23 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_10->addItem(verticalSpacer_23);


        horizontalLayout_3->addLayout(verticalLayout_10);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalSpacer_26 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_26);

        pushButton_EtStudy = new QPushButton(groupBox_3);
        pushButton_EtStudy->setObjectName(QString::fromUtf8("pushButton_EtStudy"));
        pushButton_EtStudy->setEnabled(false);
        pushButton_EtStudy->setStyleSheet(QString::fromUtf8("    font: 13pt;\n"
"    color: rgb(7, 100, 143);"));

        verticalLayout_9->addWidget(pushButton_EtStudy);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_3);

        pushButton_EtExam = new QPushButton(groupBox_3);
        pushButton_EtExam->setObjectName(QString::fromUtf8("pushButton_EtExam"));
        pushButton_EtExam->setEnabled(false);
        pushButton_EtExam->setStyleSheet(QString::fromUtf8("    font: 13pt;\n"
"    color: rgb(7, 100, 143);"));

        verticalLayout_9->addWidget(pushButton_EtExam);

        verticalSpacer_25 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_25);

        pushButton_EtComplet = new QPushButton(groupBox_3);
        pushButton_EtComplet->setObjectName(QString::fromUtf8("pushButton_EtComplet"));
        pushButton_EtComplet->setEnabled(false);
        pushButton_EtComplet->setStyleSheet(QString::fromUtf8("    font: 13pt;\n"
"    color: rgb(7, 100, 143);"));

        verticalLayout_9->addWidget(pushButton_EtComplet);

        verticalSpacer_27 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_27);


        horizontalLayout_3->addLayout(verticalLayout_9);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_10);


        gridLayout_4->addLayout(horizontalLayout_3, 1, 0, 1, 1);

        verticalSpacer_28 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_4->addItem(verticalSpacer_28, 0, 0, 1, 1);


        horizontalLayout_4->addWidget(groupBox_3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        horizontalLayout_5->addLayout(horizontalLayout_4);


        gridLayout->addLayout(horizontalLayout_5, 3, 1, 1, 1);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        verticalSpacer_24 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_11->addItem(verticalSpacer_24);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_8);

        label_8 = new QLabel(hostpage);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 75 14pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 0), stop:1 rgba(255, 255, 255,0));"));

        horizontalLayout->addWidget(label_8);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_9);


        verticalLayout_11->addLayout(horizontalLayout);

        verticalSpacer_4 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_11->addItem(verticalSpacer_4);


        gridLayout->addLayout(verticalLayout_11, 4, 1, 1, 1);


        retranslateUi(hostpage);

        QMetaObject::connectSlotsByName(hostpage);
    } // setupUi

    void retranslateUi(QWidget *hostpage)
    {
        hostpage->setWindowTitle(QApplication::translate("hostpage", "\345\255\246\345\221\230\347\253\231\344\270\273\351\241\265\351\235\242", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("hostpage", "\345\275\223\345\211\215\345\255\246\345\221\230\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("hostpage", "\345\255\246 \345\221\230 \345\220\215 \357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("hostpage", "\345\255\246\345\221\230\345\267\245\347\247\215 \357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("hostpage", "\347\272\247    \345\210\253 \357\274\232", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("hostpage", "\344\273\277\347\234\237\346\250\241\345\274\217 \357\274\232", 0, QApplication::UnicodeUTF8));
        label_username->setText(QApplication::translate("hostpage", "lable", 0, QApplication::UnicodeUTF8));
        label_usertype->setText(QApplication::translate("hostpage", "Label", 0, QApplication::UnicodeUTF8));
        label_userrange->setText(QApplication::translate("hostpage", "lable", 0, QApplication::UnicodeUTF8));
        label_usermode->setText(QApplication::translate("hostpage", "lable", 0, QApplication::UnicodeUTF8));
        toolButton_personinfo->setText(QApplication::translate("hostpage", "  \346\237\245\350\257\242\344\277\256\346\224\271\344\270\252\344\272\272\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("hostpage", "\345\255\246\345\221\230\345\216\206\345\217\262\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("hostpage", "\345\255\246\345\221\230\345\216\206\345\217\262\346\223\215\344\275\234\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("hostpage", "\345\255\246\345\221\230\345\216\206\345\217\262\345\255\246\344\271\240\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("hostpage", "\345\255\246\345\221\230\345\216\206\345\217\262\346\210\220\347\273\251\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        pushButton_oprrecord->setText(QApplication::translate("hostpage", "\346\237\245 \350\257\242", 0, QApplication::UnicodeUTF8));
        pushButton_studyrecord->setText(QApplication::translate("hostpage", "\346\237\245 \350\257\242", 0, QApplication::UnicodeUTF8));
        pushButton_scorerecord->setText(QApplication::translate("hostpage", "\346\237\245 \350\257\242", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("hostpage", "\344\275\277\347\224\250\346\250\241\345\274\217\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        toolButton_ModeS->setText(QApplication::translate("hostpage", "\345\237\271\350\256\255\345\255\246\344\271\240\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8));
        toolButton_ModeE->setText(QApplication::translate("hostpage", "\344\270\252\344\272\272\351\211\264\345\256\232\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8));
        toolButton_ModeC->setText(QApplication::translate("hostpage", "\346\210\220\347\273\204\347\253\236\350\265\233\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8));
        radioButton_ST->setText(QApplication::translate("hostpage", "\347\220\206\350\256\272\345\255\246\344\271\240", 0, QApplication::UnicodeUTF8));
        radioButton_SO->setText(QApplication::translate("hostpage", "\346\223\215\344\275\234\345\255\246\344\271\240", 0, QApplication::UnicodeUTF8));
        radioButton_ET->setText(QApplication::translate("hostpage", "\347\220\206\350\256\272\350\200\203\350\257\225", 0, QApplication::UnicodeUTF8));
        radioButton_EO->setText(QApplication::translate("hostpage", "\346\223\215\344\275\234\351\211\264\345\256\232", 0, QApplication::UnicodeUTF8));
        label_ContestNum->setText(QApplication::translate("hostpage", "TextLabel", 0, QApplication::UnicodeUTF8));
        pushButton_EtStudy->setText(QApplication::translate("hostpage", "\350\277\233\345\205\245", 0, QApplication::UnicodeUTF8));
        pushButton_EtExam->setText(QApplication::translate("hostpage", "\350\277\233\345\205\245", 0, QApplication::UnicodeUTF8));
        pushButton_EtComplet->setText(QApplication::translate("hostpage", "\350\277\233\345\205\245", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("hostpage", "                         Copyright (C)    2014-2020   BITC                        ", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(hostpage);
    } // retranslateUi

};

namespace Ui {
    class hostpage: public Ui_hostpage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOSTPAGE_H
