/********************************************************************************
** Form generated from reading ui file 'settaskOLD.ui'
**
** Created: Thu Jan 15 10:28:28 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SETTASKOLD_H
#define UI_SETTASKOLD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CSetTaskTable
{
public:
    QHBoxLayout *horizontalLayout_16;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QListWidget *listUserAndGroups;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label;
    QSpinBox *spinTimeLimit;
    QHBoxLayout *horizontalLayout_10;
    QRadioButton *radioSingle;
    QRadioButton *radioMulti;
    QPushButton *buttonSend;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_15;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QComboBox *comboStn;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QComboBox *comboDev;
    QHBoxLayout *horizontalLayout_14;
    QGroupBox *groupBoxTask;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_6;
    QComboBox *comboTaskFilterType;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QComboBox *comboTaskFilterName;
    QLineEdit *editTaskFilter;
    QPushButton *buttonTaskFilter;
    QPushButton *buttonTaskShowAll;
    QListView *listTask;
    QVBoxLayout *verticalLayout_8;
    QGroupBox *groupBoxScene;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_6;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QComboBox *comboSceneFilterType;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QComboBox *comboSceneFilterName;
    QLineEdit *editSceneFilter;
    QPushButton *buttonSceneFilter;
    QPushButton *buttonSceneShowAll;
    QListView *listScene;
    QGroupBox *groupBoxFault;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_11;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_9;
    QComboBox *comboFaultFilterType;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_10;
    QComboBox *comboFaultFilterName;
    QLineEdit *editFaultFilter;
    QPushButton *buttonFaultFilter;
    QPushButton *buttonFaultShowAll;
    QListView *listFault;

    void setupUi(QDialog *CSetTaskTable)
    {
        if (CSetTaskTable->objectName().isEmpty())
            CSetTaskTable->setObjectName(QString::fromUtf8("CSetTaskTable"));
        CSetTaskTable->resize(1188, 620);
        horizontalLayout_16 = new QHBoxLayout(CSetTaskTable);
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        groupBox = new QGroupBox(CSetTaskTable);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMaximumSize(QSize(250, 16777215));
        groupBox->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(30);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        listUserAndGroups = new QListWidget(groupBox);
        listUserAndGroups->setObjectName(QString::fromUtf8("listUserAndGroups"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listUserAndGroups->sizePolicy().hasHeightForWidth());
        listUserAndGroups->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(listUserAndGroups);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_9->addWidget(label);

        spinTimeLimit = new QSpinBox(groupBox);
        spinTimeLimit->setObjectName(QString::fromUtf8("spinTimeLimit"));
        spinTimeLimit->setCorrectionMode(QAbstractSpinBox::CorrectToPreviousValue);
        spinTimeLimit->setMaximum(1000);

        horizontalLayout_9->addWidget(spinTimeLimit);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        radioSingle = new QRadioButton(groupBox);
        radioSingle->setObjectName(QString::fromUtf8("radioSingle"));

        horizontalLayout_10->addWidget(radioSingle);

        radioMulti = new QRadioButton(groupBox);
        radioMulti->setObjectName(QString::fromUtf8("radioMulti"));

        horizontalLayout_10->addWidget(radioMulti);


        verticalLayout->addLayout(horizontalLayout_10);

        buttonSend = new QPushButton(groupBox);
        buttonSend->setObjectName(QString::fromUtf8("buttonSend"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonSend->sizePolicy().hasHeightForWidth());
        buttonSend->setSizePolicy(sizePolicy1);
        buttonSend->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonSend);


        horizontalLayout_16->addWidget(groupBox);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_3 = new QLabel(CSetTaskTable);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy2);
        label_3->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label_3);

        comboStn = new QComboBox(CSetTaskTable);
        comboStn->setObjectName(QString::fromUtf8("comboStn"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(comboStn->sizePolicy().hasHeightForWidth());
        comboStn->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(comboStn);


        horizontalLayout_15->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_4 = new QLabel(CSetTaskTable);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);
        label_4->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_4);

        comboDev = new QComboBox(CSetTaskTable);
        comboDev->setObjectName(QString::fromUtf8("comboDev"));
        sizePolicy3.setHeightForWidth(comboDev->sizePolicy().hasHeightForWidth());
        comboDev->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(comboDev);


        horizontalLayout_15->addLayout(horizontalLayout_2);


        verticalLayout_9->addLayout(horizontalLayout_15);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        groupBoxTask = new QGroupBox(CSetTaskTable);
        groupBoxTask->setObjectName(QString::fromUtf8("groupBoxTask"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(groupBoxTask->sizePolicy().hasHeightForWidth());
        groupBoxTask->setSizePolicy(sizePolicy4);
        groupBoxTask->setMinimumSize(QSize(500, 0));
        groupBoxTask->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));
        groupBoxTask->setCheckable(false);
        verticalLayout_3 = new QVBoxLayout(groupBoxTask);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_6 = new QLabel(groupBoxTask);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy5);
        label_6->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_6);

        comboTaskFilterType = new QComboBox(groupBoxTask);
        comboTaskFilterType->setObjectName(QString::fromUtf8("comboTaskFilterType"));

        horizontalLayout_3->addWidget(comboTaskFilterType);


        gridLayout->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_5 = new QLabel(groupBoxTask);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy5.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy5);
        label_5->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label_5);

        comboTaskFilterName = new QComboBox(groupBoxTask);
        comboTaskFilterName->setObjectName(QString::fromUtf8("comboTaskFilterName"));
        sizePolicy3.setHeightForWidth(comboTaskFilterName->sizePolicy().hasHeightForWidth());
        comboTaskFilterName->setSizePolicy(sizePolicy3);

        horizontalLayout_4->addWidget(comboTaskFilterName);


        gridLayout->addLayout(horizontalLayout_4, 0, 1, 1, 2);

        editTaskFilter = new QLineEdit(groupBoxTask);
        editTaskFilter->setObjectName(QString::fromUtf8("editTaskFilter"));
        editTaskFilter->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(editTaskFilter, 1, 0, 1, 2);

        buttonTaskFilter = new QPushButton(groupBoxTask);
        buttonTaskFilter->setObjectName(QString::fromUtf8("buttonTaskFilter"));
        sizePolicy5.setHeightForWidth(buttonTaskFilter->sizePolicy().hasHeightForWidth());
        buttonTaskFilter->setSizePolicy(sizePolicy5);
        buttonTaskFilter->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(buttonTaskFilter, 1, 2, 1, 1);


        horizontalLayout_5->addLayout(gridLayout);

        buttonTaskShowAll = new QPushButton(groupBoxTask);
        buttonTaskShowAll->setObjectName(QString::fromUtf8("buttonTaskShowAll"));
        sizePolicy2.setHeightForWidth(buttonTaskShowAll->sizePolicy().hasHeightForWidth());
        buttonTaskShowAll->setSizePolicy(sizePolicy2);

        horizontalLayout_5->addWidget(buttonTaskShowAll);


        verticalLayout_2->addLayout(horizontalLayout_5);

        listTask = new QListView(groupBoxTask);
        listTask->setObjectName(QString::fromUtf8("listTask"));
        listTask->setMinimumSize(QSize(0, 200));

        verticalLayout_2->addWidget(listTask);


        verticalLayout_3->addLayout(verticalLayout_2);


        horizontalLayout_14->addWidget(groupBoxTask);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        groupBoxScene = new QGroupBox(CSetTaskTable);
        groupBoxScene->setObjectName(QString::fromUtf8("groupBoxScene"));
        sizePolicy4.setHeightForWidth(groupBoxScene->sizePolicy().hasHeightForWidth());
        groupBoxScene->setSizePolicy(sizePolicy4);
        groupBoxScene->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));
        groupBoxScene->setFlat(false);
        groupBoxScene->setCheckable(true);
        groupBoxScene->setChecked(true);
        verticalLayout_4 = new QVBoxLayout(groupBoxScene);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_7 = new QLabel(groupBoxScene);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        sizePolicy5.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy5);
        label_7->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_7->addWidget(label_7);

        comboSceneFilterType = new QComboBox(groupBoxScene);
        comboSceneFilterType->setObjectName(QString::fromUtf8("comboSceneFilterType"));

        horizontalLayout_7->addWidget(comboSceneFilterType);


        gridLayout_2->addLayout(horizontalLayout_7, 0, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_8 = new QLabel(groupBoxScene);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy5.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy5);
        label_8->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_8->addWidget(label_8);

        comboSceneFilterName = new QComboBox(groupBoxScene);
        comboSceneFilterName->setObjectName(QString::fromUtf8("comboSceneFilterName"));
        sizePolicy3.setHeightForWidth(comboSceneFilterName->sizePolicy().hasHeightForWidth());
        comboSceneFilterName->setSizePolicy(sizePolicy3);

        horizontalLayout_8->addWidget(comboSceneFilterName);


        gridLayout_2->addLayout(horizontalLayout_8, 0, 1, 1, 2);

        editSceneFilter = new QLineEdit(groupBoxScene);
        editSceneFilter->setObjectName(QString::fromUtf8("editSceneFilter"));
        editSceneFilter->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_2->addWidget(editSceneFilter, 1, 0, 1, 2);

        buttonSceneFilter = new QPushButton(groupBoxScene);
        buttonSceneFilter->setObjectName(QString::fromUtf8("buttonSceneFilter"));
        sizePolicy5.setHeightForWidth(buttonSceneFilter->sizePolicy().hasHeightForWidth());
        buttonSceneFilter->setSizePolicy(sizePolicy5);
        buttonSceneFilter->setStyleSheet(QString::fromUtf8(""));

        gridLayout_2->addWidget(buttonSceneFilter, 1, 2, 1, 1);


        horizontalLayout_6->addLayout(gridLayout_2);

        buttonSceneShowAll = new QPushButton(groupBoxScene);
        buttonSceneShowAll->setObjectName(QString::fromUtf8("buttonSceneShowAll"));
        sizePolicy2.setHeightForWidth(buttonSceneShowAll->sizePolicy().hasHeightForWidth());
        buttonSceneShowAll->setSizePolicy(sizePolicy2);

        horizontalLayout_6->addWidget(buttonSceneShowAll);


        verticalLayout_5->addLayout(horizontalLayout_6);

        listScene = new QListView(groupBoxScene);
        listScene->setObjectName(QString::fromUtf8("listScene"));
        listScene->setMinimumSize(QSize(0, 150));

        verticalLayout_5->addWidget(listScene);


        verticalLayout_4->addLayout(verticalLayout_5);


        verticalLayout_8->addWidget(groupBoxScene);

        groupBoxFault = new QGroupBox(CSetTaskTable);
        groupBoxFault->setObjectName(QString::fromUtf8("groupBoxFault"));
        sizePolicy4.setHeightForWidth(groupBoxFault->sizePolicy().hasHeightForWidth());
        groupBoxFault->setSizePolicy(sizePolicy4);
        groupBoxFault->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));
        groupBoxFault->setFlat(false);
        groupBoxFault->setCheckable(true);
        groupBoxFault->setChecked(true);
        verticalLayout_6 = new QVBoxLayout(groupBoxFault);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_9 = new QLabel(groupBoxFault);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy5.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy5);
        label_9->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_12->addWidget(label_9);

        comboFaultFilterType = new QComboBox(groupBoxFault);
        comboFaultFilterType->setObjectName(QString::fromUtf8("comboFaultFilterType"));

        horizontalLayout_12->addWidget(comboFaultFilterType);


        gridLayout_3->addLayout(horizontalLayout_12, 0, 0, 1, 1);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_10 = new QLabel(groupBoxFault);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        sizePolicy5.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy5);
        label_10->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_13->addWidget(label_10);

        comboFaultFilterName = new QComboBox(groupBoxFault);
        comboFaultFilterName->setObjectName(QString::fromUtf8("comboFaultFilterName"));
        sizePolicy3.setHeightForWidth(comboFaultFilterName->sizePolicy().hasHeightForWidth());
        comboFaultFilterName->setSizePolicy(sizePolicy3);

        horizontalLayout_13->addWidget(comboFaultFilterName);


        gridLayout_3->addLayout(horizontalLayout_13, 0, 1, 1, 2);

        editFaultFilter = new QLineEdit(groupBoxFault);
        editFaultFilter->setObjectName(QString::fromUtf8("editFaultFilter"));
        editFaultFilter->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(editFaultFilter, 1, 0, 1, 2);

        buttonFaultFilter = new QPushButton(groupBoxFault);
        buttonFaultFilter->setObjectName(QString::fromUtf8("buttonFaultFilter"));
        sizePolicy5.setHeightForWidth(buttonFaultFilter->sizePolicy().hasHeightForWidth());
        buttonFaultFilter->setSizePolicy(sizePolicy5);
        buttonFaultFilter->setStyleSheet(QString::fromUtf8(""));

        gridLayout_3->addWidget(buttonFaultFilter, 1, 2, 1, 1);


        horizontalLayout_11->addLayout(gridLayout_3);

        buttonFaultShowAll = new QPushButton(groupBoxFault);
        buttonFaultShowAll->setObjectName(QString::fromUtf8("buttonFaultShowAll"));
        sizePolicy2.setHeightForWidth(buttonFaultShowAll->sizePolicy().hasHeightForWidth());
        buttonFaultShowAll->setSizePolicy(sizePolicy2);

        horizontalLayout_11->addWidget(buttonFaultShowAll);


        verticalLayout_7->addLayout(horizontalLayout_11);

        listFault = new QListView(groupBoxFault);
        listFault->setObjectName(QString::fromUtf8("listFault"));
        listFault->setMinimumSize(QSize(0, 150));

        verticalLayout_7->addWidget(listFault);


        verticalLayout_6->addLayout(verticalLayout_7);


        verticalLayout_8->addWidget(groupBoxFault);


        horizontalLayout_14->addLayout(verticalLayout_8);


        verticalLayout_9->addLayout(horizontalLayout_14);


        horizontalLayout_16->addLayout(verticalLayout_9);

        QWidget::setTabOrder(buttonSend, buttonTaskShowAll);

        retranslateUi(CSetTaskTable);

        QMetaObject::connectSlotsByName(CSetTaskTable);
    } // setupUi

    void retranslateUi(QDialog *CSetTaskTable)
    {
        CSetTaskTable->setWindowTitle(QApplication::translate("CSetTaskTable", "\345\255\246\345\221\230\344\273\273\345\212\241\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CSetTaskTable", "\345\267\262\347\231\273\345\275\225\345\255\246\345\221\230\346\210\226\345\210\206\347\273\204", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CSetTaskTable", "\350\256\276\347\275\256\344\273\273\345\212\241\346\227\266\351\227\264:", 0, QApplication::UnicodeUTF8));
        spinTimeLimit->setSuffix(QApplication::translate("CSetTaskTable", "   \345\210\206\351\222\237", 0, QApplication::UnicodeUTF8));
        spinTimeLimit->setPrefix(QString());
        radioSingle->setText(QApplication::translate("CSetTaskTable", "\345\215\225\346\234\272\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        radioMulti->setText(QApplication::translate("CSetTaskTable", "\345\244\232\346\234\272\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        buttonSend->setText(QApplication::translate("CSetTaskTable", "\345\217\221\351\200\201\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CSetTaskTable", "\345\216\202  \347\253\231\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CSetTaskTable", "\350\256\276  \345\244\207\357\274\232", 0, QApplication::UnicodeUTF8));
        groupBoxTask->setTitle(QApplication::translate("CSetTaskTable", "\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CSetTaskTable", "\347\255\233\351\200\211\345\210\206\347\261\273\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CSetTaskTable", "\347\255\233\351\200\211\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonTaskFilter->setText(QApplication::translate("CSetTaskTable", "\346\250\241\347\263\212\346\237\245\346\211\276", 0, QApplication::UnicodeUTF8));
        buttonTaskShowAll->setText(QApplication::translate("CSetTaskTable", "\346\230\276\347\244\272\345\205\250\351\203\250", 0, QApplication::UnicodeUTF8));
        groupBoxScene->setTitle(QApplication::translate("CSetTaskTable", "\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("CSetTaskTable", "\347\255\233\351\200\211\345\210\206\347\261\273\357\274\232", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("CSetTaskTable", "\347\255\233\351\200\211\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonSceneFilter->setText(QApplication::translate("CSetTaskTable", "\346\250\241\347\263\212\346\237\245\346\211\276", 0, QApplication::UnicodeUTF8));
        buttonSceneShowAll->setText(QApplication::translate("CSetTaskTable", "\346\230\276\347\244\272\345\205\250\351\203\250", 0, QApplication::UnicodeUTF8));
        groupBoxFault->setTitle(QApplication::translate("CSetTaskTable", "\346\225\205\351\232\234", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("CSetTaskTable", "\347\255\233\351\200\211\345\210\206\347\261\273\357\274\232", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("CSetTaskTable", "\347\255\233\351\200\211\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonFaultFilter->setText(QApplication::translate("CSetTaskTable", "\346\250\241\347\263\212\346\237\245\346\211\276", 0, QApplication::UnicodeUTF8));
        buttonFaultShowAll->setText(QApplication::translate("CSetTaskTable", "\346\230\276\347\244\272\345\205\250\351\203\250", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CSetTaskTable);
    } // retranslateUi

};

namespace Ui {
    class CSetTaskTable: public Ui_CSetTaskTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTASKOLD_H
