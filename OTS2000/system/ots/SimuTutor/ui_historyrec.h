/********************************************************************************
** Form generated from reading ui file 'historyrec.ui'
**
** Created: Thu Jan 15 10:26:37 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_HISTORYREC_H
#define UI_HISTORYREC_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CHistoryRecTable
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkCmd;
    QCheckBox *checkAlarm;
    QCheckBox *checkOpt;
    QCheckBox *checkCheck;
    QCheckBox *checkInfo;
    QPushButton *buttonSelectTask;
    QPushButton *buttonSearch;
    QPushButton *buttonTodayDisplay;
    QPushButton *buttonPrint;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *buttonRecordDel;
    QLabel *label_9;
    QDateTimeEdit *timeStart;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_10;
    QDateTimeEdit *timeEnd;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QComboBox *comboSearchType;
    QHBoxLayout *horizontalLayout_6;
    QLabel *labelName;
    QComboBox *comboSearchName;
    QTableView *viewRec;

    void setupUi(QWidget *CHistoryRecTable)
    {
        if (CHistoryRecTable->objectName().isEmpty())
            CHistoryRecTable->setObjectName(QString::fromUtf8("CHistoryRecTable"));
        CHistoryRecTable->resize(1026, 636);
        gridLayout_2 = new QGridLayout(CHistoryRecTable);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(6, -1, 6, -1);
        groupBox = new QGroupBox(CHistoryRecTable);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 0));
        groupBox->setMaximumSize(QSize(450, 16777215));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        checkCmd = new QCheckBox(groupBox);
        checkCmd->setObjectName(QString::fromUtf8("checkCmd"));
        checkCmd->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(checkCmd);

        checkAlarm = new QCheckBox(groupBox);
        checkAlarm->setObjectName(QString::fromUtf8("checkAlarm"));
        checkAlarm->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(checkAlarm);

        checkOpt = new QCheckBox(groupBox);
        checkOpt->setObjectName(QString::fromUtf8("checkOpt"));
        checkOpt->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(checkOpt);

        checkCheck = new QCheckBox(groupBox);
        checkCheck->setObjectName(QString::fromUtf8("checkCheck"));
        checkCheck->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(checkCheck);

        checkInfo = new QCheckBox(groupBox);
        checkInfo->setObjectName(QString::fromUtf8("checkInfo"));
        checkInfo->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(checkInfo);


        horizontalLayout_3->addWidget(groupBox);

        buttonSelectTask = new QPushButton(CHistoryRecTable);
        buttonSelectTask->setObjectName(QString::fromUtf8("buttonSelectTask"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonSelectTask->sizePolicy().hasHeightForWidth());
        buttonSelectTask->setSizePolicy(sizePolicy);
        buttonSelectTask->setMinimumSize(QSize(120, 45));
        buttonSelectTask->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(buttonSelectTask);

        buttonSearch = new QPushButton(CHistoryRecTable);
        buttonSearch->setObjectName(QString::fromUtf8("buttonSearch"));
        sizePolicy.setHeightForWidth(buttonSearch->sizePolicy().hasHeightForWidth());
        buttonSearch->setSizePolicy(sizePolicy);
        buttonSearch->setMinimumSize(QSize(120, 45));
        buttonSearch->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(buttonSearch);

        buttonTodayDisplay = new QPushButton(CHistoryRecTable);
        buttonTodayDisplay->setObjectName(QString::fromUtf8("buttonTodayDisplay"));
        sizePolicy.setHeightForWidth(buttonTodayDisplay->sizePolicy().hasHeightForWidth());
        buttonTodayDisplay->setSizePolicy(sizePolicy);
        buttonTodayDisplay->setMinimumSize(QSize(120, 45));
        buttonTodayDisplay->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(buttonTodayDisplay);

        buttonPrint = new QPushButton(CHistoryRecTable);
        buttonPrint->setObjectName(QString::fromUtf8("buttonPrint"));
        sizePolicy.setHeightForWidth(buttonPrint->sizePolicy().hasHeightForWidth());
        buttonPrint->setSizePolicy(sizePolicy);
        buttonPrint->setMinimumSize(QSize(120, 45));
        buttonPrint->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(buttonPrint);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(6, -1, 6, -1);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setSizeConstraint(QLayout::SetDefaultConstraint);
        buttonRecordDel = new QPushButton(CHistoryRecTable);
        buttonRecordDel->setObjectName(QString::fromUtf8("buttonRecordDel"));
        buttonRecordDel->setMaximumSize(QSize(25, 16777215));
        buttonRecordDel->setStyleSheet(QString::fromUtf8("background-color: rgb(240, 85, 255);\n"
"font: 75 9pt \"\351\273\221\344\275\223\";\n"
"color: rgb(255, 255, 255);"));

        horizontalLayout_4->addWidget(buttonRecordDel);

        label_9 = new QLabel(CHistoryRecTable);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy1);
        label_9->setMinimumSize(QSize(70, 0));
        label_9->setMaximumSize(QSize(70, 16777215));
        label_9->setSizeIncrement(QSize(0, 0));
        label_9->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label_9);

        timeStart = new QDateTimeEdit(CHistoryRecTable);
        timeStart->setObjectName(QString::fromUtf8("timeStart"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(timeStart->sizePolicy().hasHeightForWidth());
        timeStart->setSizePolicy(sizePolicy2);
        timeStart->setMinimumSize(QSize(160, 0));
        timeStart->setMaximumSize(QSize(160, 16777215));
        timeStart->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(timeStart);


        horizontalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_10 = new QLabel(CHistoryRecTable);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        sizePolicy1.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy1);
        label_10->setMinimumSize(QSize(70, 0));
        label_10->setMaximumSize(QSize(70, 16777215));
        label_10->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(label_10);

        timeEnd = new QDateTimeEdit(CHistoryRecTable);
        timeEnd->setObjectName(QString::fromUtf8("timeEnd"));
        timeEnd->setMinimumSize(QSize(160, 0));
        timeEnd->setMaximumSize(QSize(160, 16777215));
        timeEnd->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(timeEnd);


        horizontalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);

        label = new QLabel(CHistoryRecTable);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(80, 0));
        label->setMaximumSize(QSize(80, 16777215));
        label->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_8->addWidget(label);

        comboSearchType = new QComboBox(CHistoryRecTable);
        comboSearchType->setObjectName(QString::fromUtf8("comboSearchType"));
        comboSearchType->setMinimumSize(QSize(130, 0));
        comboSearchType->setMaximumSize(QSize(130, 16777215));

        horizontalLayout_8->addWidget(comboSearchType);


        horizontalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        labelName = new QLabel(CHistoryRecTable);
        labelName->setObjectName(QString::fromUtf8("labelName"));
        sizePolicy1.setHeightForWidth(labelName->sizePolicy().hasHeightForWidth());
        labelName->setSizePolicy(sizePolicy1);
        labelName->setMinimumSize(QSize(80, 0));
        labelName->setMaximumSize(QSize(80, 16777215));
        labelName->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_6->addWidget(labelName);

        comboSearchName = new QComboBox(CHistoryRecTable);
        comboSearchName->setObjectName(QString::fromUtf8("comboSearchName"));
        comboSearchName->setMinimumSize(QSize(130, 0));
        comboSearchName->setMaximumSize(QSize(130, 16777215));
        comboSearchName->setEditable(true);

        horizontalLayout_6->addWidget(comboSearchName);


        horizontalLayout->addLayout(horizontalLayout_6);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        viewRec = new QTableView(CHistoryRecTable);
        viewRec->setObjectName(QString::fromUtf8("viewRec"));
        viewRec->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        viewRec->setSortingEnabled(true);
        viewRec->horizontalHeader()->setStretchLastSection(true);

        gridLayout->addWidget(viewRec, 1, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(CHistoryRecTable);

        QMetaObject::connectSlotsByName(CHistoryRecTable);
    } // setupUi

    void retranslateUi(QWidget *CHistoryRecTable)
    {
        CHistoryRecTable->setWindowTitle(QApplication::translate("CHistoryRecTable", "\346\223\215\344\275\234\350\256\260\345\275\225\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CHistoryRecTable", "\350\256\260\345\275\225\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        checkCmd->setText(QApplication::translate("CHistoryRecTable", "\346\223\215\344\275\234\345\221\275\344\273\244", 0, QApplication::UnicodeUTF8));
        checkAlarm->setText(QApplication::translate("CHistoryRecTable", "\346\212\245\350\255\246\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        checkOpt->setText(QApplication::translate("CHistoryRecTable", "\347\216\260\345\234\260\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8));
        checkCheck->setText(QApplication::translate("CHistoryRecTable", "\346\243\200\346\237\245\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8));
        checkInfo->setText(QApplication::translate("CHistoryRecTable", "\347\237\255\346\266\210\346\201\257", 0, QApplication::UnicodeUTF8));
        buttonSelectTask->setText(QApplication::translate("CHistoryRecTable", "\351\200\211\346\213\251\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        buttonSearch->setText(QApplication::translate("CHistoryRecTable", "\346\237\245   \350\257\242", 0, QApplication::UnicodeUTF8));
        buttonTodayDisplay->setText(QApplication::translate("CHistoryRecTable", "\346\230\276\347\244\272\345\275\223\345\244\251\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        buttonPrint->setText(QApplication::translate("CHistoryRecTable", "\345\257\274\345\207\272\345\210\260Excel", 0, QApplication::UnicodeUTF8));
        buttonRecordDel->setText(QApplication::translate("CHistoryRecTable", "DEL", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("CHistoryRecTable", "\345\274\200\345\247\213\346\227\266\351\227\264\357\274\232", 0, QApplication::UnicodeUTF8));
        timeStart->setDisplayFormat(QApplication::translate("CHistoryRecTable", "yyyy-MM-dd HH:mm:ss", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("CHistoryRecTable", "\347\273\223\346\235\237\346\227\266\351\227\264\357\274\232", 0, QApplication::UnicodeUTF8));
        timeEnd->setDisplayFormat(QApplication::translate("CHistoryRecTable", "yyyy-MM-dd HH:mm:ss", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CHistoryRecTable", "\346\237\245\350\257\242\346\226\271\345\274\217\357\274\232", 0, QApplication::UnicodeUTF8));
        labelName->setText(QString());
        Q_UNUSED(CHistoryRecTable);
    } // retranslateUi

};

namespace Ui {
    class CHistoryRecTable: public Ui_CHistoryRecTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTORYREC_H
