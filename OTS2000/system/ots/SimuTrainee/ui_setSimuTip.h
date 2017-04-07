/********************************************************************************
** Form generated from reading ui file 'setSimuTip.ui'
**
** Created: Wed Jan 14 10:22:09 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SETSIMUTIP_H
#define UI_SETSIMUTIP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CSetSimuTip
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_13;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *checkIsAutoSize;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_14;
    QSpinBox *spinWidth;
    QLabel *label_15;
    QSpinBox *spinHeight;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *comboColor;
    QPushButton *buttonColor;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_5;
    QComboBox *comboCurTipColor;
    QPushButton *buttonCurTipColor;
    QLabel *label_9;
    QHBoxLayout *horizontalLayout_6;
    QComboBox *comboBackgroundColor;
    QPushButton *buttonBackgroundColor;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_16;
    QComboBox *comboMode;
    QLabel *label_17;
    QSplitter *splitter;
    QSlider *sliderAlpha;
    QSpinBox *spinAlpha;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *buttonFont;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_18;
    QLineEdit *editFont;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer;
    QToolButton *buttonOpenSimuTip;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *buttonApply;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *buttonEnter;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *buttonCancel;
    QSpacerItem *horizontalSpacer_7;

    void setupUi(QWidget *CSetSimuTip)
    {
        if (CSetSimuTip->objectName().isEmpty())
            CSetSimuTip->setObjectName(QString::fromUtf8("CSetSimuTip"));
        CSetSimuTip->resize(720, 371);
        CSetSimuTip->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";\n"
"background-color: rgb(231, 240, 245);"));
        gridLayout = new QGridLayout(CSetSimuTip);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_13 = new QLabel(CSetSimuTip);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        horizontalLayout->addWidget(label_13);

        horizontalSpacer_2 = new QSpacerItem(138, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        checkIsAutoSize = new QCheckBox(CSetSimuTip);
        checkIsAutoSize->setObjectName(QString::fromUtf8("checkIsAutoSize"));
        checkIsAutoSize->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        horizontalLayout->addWidget(checkIsAutoSize);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_14 = new QLabel(CSetSimuTip);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMaximumSize(QSize(45, 16777215));
        label_14->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        horizontalLayout_3->addWidget(label_14);

        spinWidth = new QSpinBox(CSetSimuTip);
        spinWidth->setObjectName(QString::fromUtf8("spinWidth"));
        spinWidth->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 100), stop:0.5 rgba(224, 234, 245, 100), stop:1 rgba(255, 255, 255, 100));\n"
" border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    padding: 2px;"));
        spinWidth->setMaximum(1280);

        horizontalLayout_3->addWidget(spinWidth);

        label_15 = new QLabel(CSetSimuTip);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMaximumSize(QSize(45, 16777215));
        label_15->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        horizontalLayout_3->addWidget(label_15);

        spinHeight = new QSpinBox(CSetSimuTip);
        spinHeight->setObjectName(QString::fromUtf8("spinHeight"));
        spinHeight->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 100), stop:0.5 rgba(224, 234, 245, 100), stop:1 rgba(255, 255, 255, 100));\n"
" border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    padding: 2px;"));
        spinHeight->setMaximum(1024);

        horizontalLayout_3->addWidget(spinHeight);


        verticalLayout->addLayout(horizontalLayout_3);

        label_6 = new QLabel(CSetSimuTip);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMaximumSize(QSize(16777215, 26));
        label_6->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout->addWidget(label_6);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        comboColor = new QComboBox(CSetSimuTip);
        comboColor->setObjectName(QString::fromUtf8("comboColor"));
        comboColor->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 100), stop:0.5 rgba(224, 234, 245, 100), stop:1 rgba(255, 255, 255, 100));\n"
" border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    padding: 2px;"));

        horizontalLayout_4->addWidget(comboColor);

        buttonColor = new QPushButton(CSetSimuTip);
        buttonColor->setObjectName(QString::fromUtf8("buttonColor"));
        buttonColor->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));

        horizontalLayout_4->addWidget(buttonColor);


        verticalLayout->addLayout(horizontalLayout_4);

        label_8 = new QLabel(CSetSimuTip);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMaximumSize(QSize(16777215, 26));
        label_8->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout->addWidget(label_8);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        comboCurTipColor = new QComboBox(CSetSimuTip);
        comboCurTipColor->setObjectName(QString::fromUtf8("comboCurTipColor"));
        comboCurTipColor->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 100), stop:0.5 rgba(224, 234, 245, 100), stop:1 rgba(255, 255, 255, 100));\n"
" border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    padding: 2px;"));

        horizontalLayout_5->addWidget(comboCurTipColor);

        buttonCurTipColor = new QPushButton(CSetSimuTip);
        buttonCurTipColor->setObjectName(QString::fromUtf8("buttonCurTipColor"));
        buttonCurTipColor->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));

        horizontalLayout_5->addWidget(buttonCurTipColor);


        verticalLayout->addLayout(horizontalLayout_5);

        label_9 = new QLabel(CSetSimuTip);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMaximumSize(QSize(16777215, 26));
        label_9->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout->addWidget(label_9);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        comboBackgroundColor = new QComboBox(CSetSimuTip);
        comboBackgroundColor->setObjectName(QString::fromUtf8("comboBackgroundColor"));
        comboBackgroundColor->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 100), stop:0.5 rgba(224, 234, 245, 100), stop:1 rgba(255, 255, 255, 100));\n"
" border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    padding: 2px;"));

        horizontalLayout_6->addWidget(comboBackgroundColor);

        buttonBackgroundColor = new QPushButton(CSetSimuTip);
        buttonBackgroundColor->setObjectName(QString::fromUtf8("buttonBackgroundColor"));
        buttonBackgroundColor->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));

        horizontalLayout_6->addWidget(buttonBackgroundColor);


        verticalLayout->addLayout(horizontalLayout_6);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_16 = new QLabel(CSetSimuTip);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setMinimumSize(QSize(0, 50));
        label_16->setMaximumSize(QSize(95, 16777215));
        label_16->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        horizontalLayout_2->addWidget(label_16);

        comboMode = new QComboBox(CSetSimuTip);
        comboMode->setObjectName(QString::fromUtf8("comboMode"));

        horizontalLayout_2->addWidget(comboMode);


        verticalLayout_2->addLayout(horizontalLayout_2);

        label_17 = new QLabel(CSetSimuTip);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout_2->addWidget(label_17);

        splitter = new QSplitter(CSetSimuTip);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        sliderAlpha = new QSlider(splitter);
        sliderAlpha->setObjectName(QString::fromUtf8("sliderAlpha"));
        sliderAlpha->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0.0284091 rgba(221, 221, 221, 255), stop:0.511364 rgba(196, 196, 196, 255), stop:1 rgba(255, 255, 255, 255));\n"
""));
        sliderAlpha->setMaximum(255);
        sliderAlpha->setSingleStep(1);
        sliderAlpha->setPageStep(1);
        sliderAlpha->setOrientation(Qt::Horizontal);
        sliderAlpha->setInvertedAppearance(false);
        sliderAlpha->setInvertedControls(false);
        sliderAlpha->setTickPosition(QSlider::NoTicks);
        sliderAlpha->setTickInterval(1);
        splitter->addWidget(sliderAlpha);
        spinAlpha = new QSpinBox(splitter);
        spinAlpha->setObjectName(QString::fromUtf8("spinAlpha"));
        spinAlpha->setMaximumSize(QSize(40, 16777215));
        spinAlpha->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));
        spinAlpha->setMaximum(255);
        spinAlpha->setSingleStep(1);
        splitter->addWidget(spinAlpha);

        verticalLayout_2->addWidget(splitter);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        buttonFont = new QPushButton(CSetSimuTip);
        buttonFont->setObjectName(QString::fromUtf8("buttonFont"));
        buttonFont->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));

        horizontalLayout_7->addWidget(buttonFont);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout_7);

        label_18 = new QLabel(CSetSimuTip);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);"));

        verticalLayout_2->addWidget(label_18);

        editFont = new QLineEdit(CSetSimuTip);
        editFont->setObjectName(QString::fromUtf8("editFont"));
        editFont->setMinimumSize(QSize(100, 70));
        editFont->setFocusPolicy(Qt::StrongFocus);
        editFont->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 100), stop:0.5 rgba(224, 234, 245, 100), stop:1 rgba(255, 255, 255, 100));\n"
" border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    padding: 2px;"));
        editFont->setAlignment(Qt::AlignCenter);
        editFont->setReadOnly(true);

        verticalLayout_2->addWidget(editFont);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);

        buttonOpenSimuTip = new QToolButton(CSetSimuTip);
        buttonOpenSimuTip->setObjectName(QString::fromUtf8("buttonOpenSimuTip"));
        buttonOpenSimuTip->setMinimumSize(QSize(0, 25));
        buttonOpenSimuTip->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));
        buttonOpenSimuTip->setCheckable(true);
        buttonOpenSimuTip->setAutoRaise(true);

        horizontalLayout_8->addWidget(buttonOpenSimuTip);


        verticalLayout_2->addLayout(horizontalLayout_8);


        gridLayout->addLayout(verticalLayout_2, 0, 1, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_4);

        buttonApply = new QPushButton(CSetSimuTip);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(214, 240, 253, 255), stop:0.536723 rgba(216, 239, 253, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 2px;\n"
"    border-color: rgb(0, 113, 173);\n"
"    border-style: solid;\n"
"    border-radius: 5;\n"
"    padding: 2px;\n"
"\n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));

        horizontalLayout_9->addWidget(buttonApply);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_5);

        buttonEnter = new QPushButton(CSetSimuTip);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));

        horizontalLayout_9->addWidget(buttonEnter);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_6);

        buttonCancel = new QPushButton(CSetSimuTip);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));

        horizontalLayout_9->addWidget(buttonCancel);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_7);


        gridLayout->addLayout(horizontalLayout_9, 1, 0, 1, 2);


        retranslateUi(CSetSimuTip);

        QMetaObject::connectSlotsByName(CSetSimuTip);
    } // setupUi

    void retranslateUi(QWidget *CSetSimuTip)
    {
        CSetSimuTip->setWindowTitle(QApplication::translate("CSetSimuTip", "\350\256\276\347\275\256\344\273\277\347\234\237\346\217\220\347\244\272\347\252\227\345\217\243\345\261\236\346\200\247", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("CSetSimuTip", "\346\217\220\347\244\272\347\252\227\345\217\243\345\244\247\345\260\217\357\274\232", 0, QApplication::UnicodeUTF8));
        checkIsAutoSize->setText(QApplication::translate("CSetSimuTip", "\350\207\252\345\212\250\345\260\272\345\257\270", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("CSetSimuTip", "\345\256\275\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("CSetSimuTip", "\351\253\230\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CSetSimuTip", "\346\217\220\347\244\272\351\242\234\350\211\262\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonColor->setText(QApplication::translate("CSetSimuTip", "\350\207\252\345\256\232\344\271\211", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("CSetSimuTip", "\345\275\223\345\211\215\346\217\220\347\244\272\351\242\234\350\211\262\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonCurTipColor->setText(QApplication::translate("CSetSimuTip", "\350\207\252\345\256\232\344\271\211", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("CSetSimuTip", "\350\203\214\346\231\257\351\242\234\350\211\262\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonBackgroundColor->setText(QApplication::translate("CSetSimuTip", "\350\207\252\345\256\232\344\271\211", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("CSetSimuTip", "\346\217\220\347\244\272\347\252\227\345\217\243\346\250\241\345\274\217\357\274\232", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("CSetSimuTip", "\347\252\227\344\275\223\351\200\217\346\230\216\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonFont->setText(QApplication::translate("CSetSimuTip", "\350\256\276\347\275\256\345\255\227\344\275\223", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("CSetSimuTip", "\347\244\272\344\276\213\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonOpenSimuTip->setText(QApplication::translate("CSetSimuTip", "\346\211\223\345\274\200\347\244\272\344\276\213\347\252\227\345\217\243", 0, QApplication::UnicodeUTF8));
        buttonApply->setText(QApplication::translate("CSetSimuTip", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("CSetSimuTip", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("CSetSimuTip", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CSetSimuTip);
    } // retranslateUi

};

namespace Ui {
    class CSetSimuTip: public Ui_CSetSimuTip {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETSIMUTIP_H
