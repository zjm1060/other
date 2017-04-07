/********************************************************************************
** Form generated from reading ui file 'subject_condition.ui'
**
** Created: Thu Jan 15 10:24:50 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SUBJECT_CONDITION_H
#define UI_SUBJECT_CONDITION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CSubjectCondition
{
public:
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editLogicalNameAlarm;
    QPushButton *buttonPointAlarm;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_6;
    QComboBox *comboSymbolAlarm;
    QSpinBox *editValueAlarm1;
    QSpinBox *editValueAlarm2;
    QVBoxLayout *verticalLayout_6;
    QPushButton *buttonAddAlarm;
    QPushButton *buttonSaveAlarm;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_2;
    QLineEdit *editLogicalNameCmd;
    QPushButton *buttonPointCmd;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_14;
    QComboBox *comboCmdTypeId;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_15;
    QComboBox *comboCmdOpt;
    QVBoxLayout *verticalLayout_7;
    QPushButton *buttonAddCmd;
    QPushButton *buttonSaveCmd;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_13;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_3;
    QLineEdit *editLogicalNameCheck;
    QPushButton *buttonPointCheck;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_22;
    QComboBox *comboCheckState;
    QVBoxLayout *verticalLayout_8;
    QPushButton *buttonAddCheck;
    QPushButton *buttonSaveCheck;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_14;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    QComboBox *comboOprType;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_8;
    QComboBox *comboOprName;
    QVBoxLayout *verticalLayout_11;
    QPushButton *buttonAddOpr;
    QPushButton *buttonSaveOpr;
    QTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout_15;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonSave;
    QPushButton *buttonExit;

    void setupUi(QDialog *CSubjectCondition)
    {
        if (CSubjectCondition->objectName().isEmpty())
            CSubjectCondition->setObjectName(QString::fromUtf8("CSubjectCondition"));
        CSubjectCondition->resize(836, 787);
        CSubjectCondition->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        verticalLayout_5 = new QVBoxLayout(CSubjectCondition);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        groupBox = new QGroupBox(CSubjectCondition);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(60, 0));
        label->setMaximumSize(QSize(60, 16777215));

        horizontalLayout->addWidget(label);

        editLogicalNameAlarm = new QLineEdit(groupBox);
        editLogicalNameAlarm->setObjectName(QString::fromUtf8("editLogicalNameAlarm"));

        horizontalLayout->addWidget(editLogicalNameAlarm);

        buttonPointAlarm = new QPushButton(groupBox);
        buttonPointAlarm->setObjectName(QString::fromUtf8("buttonPointAlarm"));

        horizontalLayout->addWidget(buttonPointAlarm);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(60, 0));
        label_6->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_2->addWidget(label_6);

        comboSymbolAlarm = new QComboBox(groupBox);
        comboSymbolAlarm->setObjectName(QString::fromUtf8("comboSymbolAlarm"));
        comboSymbolAlarm->setMinimumSize(QSize(80, 0));

        horizontalLayout_2->addWidget(comboSymbolAlarm);

        editValueAlarm1 = new QSpinBox(groupBox);
        editValueAlarm1->setObjectName(QString::fromUtf8("editValueAlarm1"));
        editValueAlarm1->setMinimum(-9999999);
        editValueAlarm1->setMaximum(9999999);

        horizontalLayout_2->addWidget(editValueAlarm1);

        editValueAlarm2 = new QSpinBox(groupBox);
        editValueAlarm2->setObjectName(QString::fromUtf8("editValueAlarm2"));
        editValueAlarm2->setMinimum(-9999999);
        editValueAlarm2->setMaximum(9999999);

        horizontalLayout_2->addWidget(editValueAlarm2);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        buttonAddAlarm = new QPushButton(groupBox);
        buttonAddAlarm->setObjectName(QString::fromUtf8("buttonAddAlarm"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonAddAlarm->sizePolicy().hasHeightForWidth());
        buttonAddAlarm->setSizePolicy(sizePolicy);
        buttonAddAlarm->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_6->addWidget(buttonAddAlarm);

        buttonSaveAlarm = new QPushButton(groupBox);
        buttonSaveAlarm->setObjectName(QString::fromUtf8("buttonSaveAlarm"));
        sizePolicy.setHeightForWidth(buttonSaveAlarm->sizePolicy().hasHeightForWidth());
        buttonSaveAlarm->setSizePolicy(sizePolicy);
        buttonSaveAlarm->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_6->addWidget(buttonSaveAlarm);


        horizontalLayout_3->addLayout(verticalLayout_6);


        verticalLayout_5->addWidget(groupBox);

        groupBox_2 = new QGroupBox(CSubjectCondition);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_8 = new QHBoxLayout(groupBox_2);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(60, 0));
        label_2->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_6->addWidget(label_2);

        editLogicalNameCmd = new QLineEdit(groupBox_2);
        editLogicalNameCmd->setObjectName(QString::fromUtf8("editLogicalNameCmd"));

        horizontalLayout_6->addWidget(editLogicalNameCmd);

        buttonPointCmd = new QPushButton(groupBox_2);
        buttonPointCmd->setObjectName(QString::fromUtf8("buttonPointCmd"));

        horizontalLayout_6->addWidget(buttonPointCmd);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_14 = new QLabel(groupBox_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMinimumSize(QSize(60, 0));
        label_14->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_4->addWidget(label_14);

        comboCmdTypeId = new QComboBox(groupBox_2);
        comboCmdTypeId->setObjectName(QString::fromUtf8("comboCmdTypeId"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboCmdTypeId->sizePolicy().hasHeightForWidth());
        comboCmdTypeId->setSizePolicy(sizePolicy1);
        comboCmdTypeId->setMinimumSize(QSize(280, 0));

        horizontalLayout_4->addWidget(comboCmdTypeId);


        horizontalLayout_7->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_15 = new QLabel(groupBox_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMinimumSize(QSize(60, 0));
        label_15->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_5->addWidget(label_15);

        comboCmdOpt = new QComboBox(groupBox_2);
        comboCmdOpt->setObjectName(QString::fromUtf8("comboCmdOpt"));
        sizePolicy1.setHeightForWidth(comboCmdOpt->sizePolicy().hasHeightForWidth());
        comboCmdOpt->setSizePolicy(sizePolicy1);
        comboCmdOpt->setMinimumSize(QSize(150, 0));

        horizontalLayout_5->addWidget(comboCmdOpt);


        horizontalLayout_7->addLayout(horizontalLayout_5);


        verticalLayout_2->addLayout(horizontalLayout_7);


        horizontalLayout_8->addLayout(verticalLayout_2);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        buttonAddCmd = new QPushButton(groupBox_2);
        buttonAddCmd->setObjectName(QString::fromUtf8("buttonAddCmd"));
        sizePolicy.setHeightForWidth(buttonAddCmd->sizePolicy().hasHeightForWidth());
        buttonAddCmd->setSizePolicy(sizePolicy);
        buttonAddCmd->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_7->addWidget(buttonAddCmd);

        buttonSaveCmd = new QPushButton(groupBox_2);
        buttonSaveCmd->setObjectName(QString::fromUtf8("buttonSaveCmd"));
        sizePolicy.setHeightForWidth(buttonSaveCmd->sizePolicy().hasHeightForWidth());
        buttonSaveCmd->setSizePolicy(sizePolicy);
        buttonSaveCmd->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_7->addWidget(buttonSaveCmd);


        horizontalLayout_8->addLayout(verticalLayout_7);


        verticalLayout_5->addWidget(groupBox_2);

        groupBox_4 = new QGroupBox(CSubjectCondition);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        horizontalLayout_13 = new QHBoxLayout(groupBox_4);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(60, 0));
        label_3->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_11->addWidget(label_3);

        editLogicalNameCheck = new QLineEdit(groupBox_4);
        editLogicalNameCheck->setObjectName(QString::fromUtf8("editLogicalNameCheck"));

        horizontalLayout_11->addWidget(editLogicalNameCheck);

        buttonPointCheck = new QPushButton(groupBox_4);
        buttonPointCheck->setObjectName(QString::fromUtf8("buttonPointCheck"));

        horizontalLayout_11->addWidget(buttonPointCheck);


        verticalLayout_3->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_22 = new QLabel(groupBox_4);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setMinimumSize(QSize(60, 0));
        label_22->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_12->addWidget(label_22);

        comboCheckState = new QComboBox(groupBox_4);
        comboCheckState->setObjectName(QString::fromUtf8("comboCheckState"));

        horizontalLayout_12->addWidget(comboCheckState);


        verticalLayout_3->addLayout(horizontalLayout_12);


        horizontalLayout_13->addLayout(verticalLayout_3);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        buttonAddCheck = new QPushButton(groupBox_4);
        buttonAddCheck->setObjectName(QString::fromUtf8("buttonAddCheck"));
        sizePolicy.setHeightForWidth(buttonAddCheck->sizePolicy().hasHeightForWidth());
        buttonAddCheck->setSizePolicy(sizePolicy);
        buttonAddCheck->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_8->addWidget(buttonAddCheck);

        buttonSaveCheck = new QPushButton(groupBox_4);
        buttonSaveCheck->setObjectName(QString::fromUtf8("buttonSaveCheck"));
        sizePolicy.setHeightForWidth(buttonSaveCheck->sizePolicy().hasHeightForWidth());
        buttonSaveCheck->setSizePolicy(sizePolicy);
        buttonSaveCheck->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_8->addWidget(buttonSaveCheck);


        horizontalLayout_13->addLayout(verticalLayout_8);


        verticalLayout_5->addWidget(groupBox_4);

        groupBox_3 = new QGroupBox(CSubjectCondition);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        horizontalLayout_14 = new QHBoxLayout(groupBox_3);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(60, 0));
        label_7->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_9->addWidget(label_7);

        comboOprType = new QComboBox(groupBox_3);
        comboOprType->setObjectName(QString::fromUtf8("comboOprType"));
        sizePolicy1.setHeightForWidth(comboOprType->sizePolicy().hasHeightForWidth());
        comboOprType->setSizePolicy(sizePolicy1);

        horizontalLayout_9->addWidget(comboOprType);


        verticalLayout_4->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(60, 0));
        label_8->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_10->addWidget(label_8);

        comboOprName = new QComboBox(groupBox_3);
        comboOprName->setObjectName(QString::fromUtf8("comboOprName"));
        sizePolicy1.setHeightForWidth(comboOprName->sizePolicy().hasHeightForWidth());
        comboOprName->setSizePolicy(sizePolicy1);

        horizontalLayout_10->addWidget(comboOprName);


        verticalLayout_4->addLayout(horizontalLayout_10);


        horizontalLayout_14->addLayout(verticalLayout_4);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        buttonAddOpr = new QPushButton(groupBox_3);
        buttonAddOpr->setObjectName(QString::fromUtf8("buttonAddOpr"));
        sizePolicy.setHeightForWidth(buttonAddOpr->sizePolicy().hasHeightForWidth());
        buttonAddOpr->setSizePolicy(sizePolicy);
        buttonAddOpr->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_11->addWidget(buttonAddOpr);

        buttonSaveOpr = new QPushButton(groupBox_3);
        buttonSaveOpr->setObjectName(QString::fromUtf8("buttonSaveOpr"));
        sizePolicy.setHeightForWidth(buttonSaveOpr->sizePolicy().hasHeightForWidth());
        buttonSaveOpr->setSizePolicy(sizePolicy);
        buttonSaveOpr->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_11->addWidget(buttonSaveOpr);


        horizontalLayout_14->addLayout(verticalLayout_11);


        verticalLayout_5->addWidget(groupBox_3);

        tableWidget = new QTableWidget(CSubjectCondition);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_5->addWidget(tableWidget);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer);

        buttonSave = new QPushButton(CSubjectCondition);
        buttonSave->setObjectName(QString::fromUtf8("buttonSave"));

        horizontalLayout_15->addWidget(buttonSave);

        buttonExit = new QPushButton(CSubjectCondition);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));

        horizontalLayout_15->addWidget(buttonExit);


        verticalLayout_5->addLayout(horizontalLayout_15);


        retranslateUi(CSubjectCondition);

        QMetaObject::connectSlotsByName(CSubjectCondition);
    } // setupUi

    void retranslateUi(QDialog *CSubjectCondition)
    {
        CSubjectCondition->setWindowTitle(QApplication::translate("CSubjectCondition", "\351\242\230\347\233\256\346\235\241\344\273\266\347\274\226\350\276\221", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CSubjectCondition", "\344\270\200\350\210\254\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CSubjectCondition", "\351\200\273\350\276\221\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonPointAlarm->setText(QApplication::translate("CSubjectCondition", "\351\200\211\346\213\251\347\202\271\345\220\215", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CSubjectCondition", "\347\233\256\346\240\207\347\212\266\346\200\201\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonAddAlarm->setText(QApplication::translate("CSubjectCondition", "\346\267\273\345\212\240\346\235\241\344\273\266", 0, QApplication::UnicodeUTF8));
        buttonSaveAlarm->setText(QApplication::translate("CSubjectCondition", "\344\277\235\345\255\230\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("CSubjectCondition", "\345\221\275\344\273\244\344\270\213\350\276\276", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CSubjectCondition", "\351\200\273\350\276\221\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonPointCmd->setText(QApplication::translate("CSubjectCondition", "\351\200\211\346\213\251\347\202\271\345\220\215", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("CSubjectCondition", "\345\221\275\344\273\244\347\261\273\345\236\213\357\274\232", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("CSubjectCondition", "\345\221\275\344\273\244\347\212\266\346\200\201\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonAddCmd->setText(QApplication::translate("CSubjectCondition", "\346\267\273\345\212\240\346\235\241\344\273\266", 0, QApplication::UnicodeUTF8));
        buttonSaveCmd->setText(QApplication::translate("CSubjectCondition", "\344\277\235\345\255\230\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("CSubjectCondition", "\350\256\276\345\244\207\346\243\200\346\237\245", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CSubjectCondition", "\351\200\273\350\276\221\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonPointCheck->setText(QApplication::translate("CSubjectCondition", "\351\200\211\346\213\251\347\202\271\345\220\215", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("CSubjectCondition", "\347\233\256\346\240\207\347\212\266\346\200\201\357\274\232", 0, QApplication::UnicodeUTF8));
        comboCheckState->clear();
        comboCheckState->insertItems(0, QStringList()
         << QApplication::translate("CSubjectCondition", "\346\243\200\346\237\245\347\212\266\346\200\201\346\255\243\345\270\270", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CSubjectCondition", "\346\243\200\346\237\245\347\212\266\346\200\201\345\274\202\345\270\270", 0, QApplication::UnicodeUTF8)
        );
        buttonAddCheck->setText(QApplication::translate("CSubjectCondition", "\346\267\273\345\212\240\346\235\241\344\273\266", 0, QApplication::UnicodeUTF8));
        buttonSaveCheck->setText(QApplication::translate("CSubjectCondition", "\344\277\235\345\255\230\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("CSubjectCondition", "\347\216\260\345\234\260\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("CSubjectCondition", "\346\223\215\344\275\234\347\261\273\345\236\213\357\274\232", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("CSubjectCondition", "\346\223\215\344\275\234\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonAddOpr->setText(QApplication::translate("CSubjectCondition", "\346\267\273\345\212\240\346\235\241\344\273\266", 0, QApplication::UnicodeUTF8));
        buttonSaveOpr->setText(QApplication::translate("CSubjectCondition", "\344\277\235\345\255\230\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CSubjectCondition", "\351\200\273\350\276\221\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CSubjectCondition", "\351\200\273\350\276\221\350\277\220\347\256\227\347\254\246", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CSubjectCondition", "\351\200\273\350\276\221\345\255\227\347\254\246\344\270\262", 0, QApplication::UnicodeUTF8));
        buttonSave->setText(QApplication::translate("CSubjectCondition", "\344\277\235   \345\255\230", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CSubjectCondition", "\351\200\200   \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CSubjectCondition);
    } // retranslateUi

};

namespace Ui {
    class CSubjectCondition: public Ui_CSubjectCondition {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBJECT_CONDITION_H
