/********************************************************************************
** Form generated from reading ui file 'userinfo.ui'
**
** Created: Wed Jan 14 10:22:05 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_USERINFO_H
#define UI_USERINFO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_userinfo
{
public:
    QGridLayout *gridLayout_8;
    QGridLayout *gridLayout_7;
    QSpacerItem *verticalSpacer_14;
    QGridLayout *gridLayout_6;
    QSpacerItem *horizontalSpacer;
    QGridLayout *gridLayout_3;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *editName;
    QSpacerItem *verticalSpacer;
    QFormLayout *formLayout_2;
    QLabel *label_12;
    QLineEdit *editLongName;
    QSpacerItem *verticalSpacer_2;
    QFormLayout *formLayout_3;
    QLabel *label_2;
    QComboBox *editRole;
    QSpacerItem *verticalSpacer_3;
    QFormLayout *formLayout_4;
    QLabel *label_13;
    QComboBox *editSex;
    QSpacerItem *verticalSpacer_4;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QLineEdit *editPassword;
    QPushButton *pushButtonChgPsd;
    QSpacerItem *verticalSpacer_5;
    QFormLayout *formLayout_6;
    QLabel *label_4;
    QLineEdit *editIdCard;
    QSpacerItem *verticalSpacer_6;
    QHBoxLayout *horizontalLayout;
    QLabel *label_15;
    QComboBox *comboBox_userType;
    QSpacerItem *verticalSpacer_16;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_16;
    QComboBox *comboBox_userRange;
    QSpacerItem *verticalSpacer_17;
    QSpacerItem *horizontalSpacer_2;
    QGridLayout *gridLayout_2;
    QFormLayout *formLayout_8;
    QLabel *label_8;
    QLineEdit *editCompany;
    QSpacerItem *verticalSpacer_7;
    QFormLayout *formLayout_9;
    QLabel *label_9;
    QLineEdit *editDepartment;
    QSpacerItem *verticalSpacer_8;
    QFormLayout *formLayout_10;
    QLabel *label_10;
    QLineEdit *editDuty;
    QSpacerItem *verticalSpacer_9;
    QFormLayout *formLayout_11;
    QLabel *label_11;
    QLineEdit *editMTel;
    QSpacerItem *verticalSpacer_10;
    QFormLayout *formLayout_12;
    QLabel *label_6;
    QLineEdit *editTel;
    QSpacerItem *verticalSpacer_11;
    QFormLayout *formLayout_13;
    QLabel *label_7;
    QLineEdit *editEmail;
    QSpacerItem *verticalSpacer_12;
    QFormLayout *formLayout_14;
    QLabel *label_14;
    QComboBox *editGroup;
    QFormLayout *formLayout_7;
    QComboBox *editEdu;
    QLabel *label_5;
    QSpacerItem *verticalSpacer_15;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout_5;
    QSpacerItem *horizontalSpacer_6;
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *buttonExit;
    QPushButton *buttonEnter;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *buttonApply;
    QSpacerItem *verticalSpacer_13;

    void setupUi(QWidget *userinfo)
    {
        if (userinfo->objectName().isEmpty())
            userinfo->setObjectName(QString::fromUtf8("userinfo"));
        userinfo->resize(906, 684);
        userinfo->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        gridLayout_8 = new QGridLayout(userinfo);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        verticalSpacer_14 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_14, 0, 0, 1, 1);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        horizontalSpacer = new QSpacerItem(88, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer, 0, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(userinfo);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        editName = new QLineEdit(userinfo);
        editName->setObjectName(QString::fromUtf8("editName"));
        editName->setEnabled(false);

        formLayout->setWidget(0, QFormLayout::FieldRole, editName);


        gridLayout_3->addLayout(formLayout, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 38, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 1, 0, 1, 1);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        label_12 = new QLabel(userinfo);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_12);

        editLongName = new QLineEdit(userinfo);
        editLongName->setObjectName(QString::fromUtf8("editLongName"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, editLongName);


        gridLayout_3->addLayout(formLayout_2, 2, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 38, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 3, 0, 1, 1);

        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_2 = new QLabel(userinfo);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_2);

        editRole = new QComboBox(userinfo);
        editRole->setObjectName(QString::fromUtf8("editRole"));
        editRole->setEnabled(false);
        editRole->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        editRole->setEditable(false);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, editRole);


        gridLayout_3->addLayout(formLayout_3, 4, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_3, 7, 0, 1, 1);

        formLayout_4 = new QFormLayout();
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        formLayout_4->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_13 = new QLabel(userinfo);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_13);

        editSex = new QComboBox(userinfo);
        editSex->setObjectName(QString::fromUtf8("editSex"));
        editSex->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_4->setWidget(1, QFormLayout::FieldRole, editSex);


        gridLayout_3->addLayout(formLayout_4, 10, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_4, 11, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(userinfo);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        editPassword = new QLineEdit(userinfo);
        editPassword->setObjectName(QString::fromUtf8("editPassword"));
        editPassword->setEchoMode(QLineEdit::Password);
        editPassword->setReadOnly(true);

        gridLayout->addWidget(editPassword, 0, 1, 1, 1);

        pushButtonChgPsd = new QPushButton(userinfo);
        pushButtonChgPsd->setObjectName(QString::fromUtf8("pushButtonChgPsd"));

        gridLayout->addWidget(pushButtonChgPsd, 0, 2, 1, 1);


        gridLayout_3->addLayout(gridLayout, 12, 0, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_5, 13, 0, 1, 1);

        formLayout_6 = new QFormLayout();
        formLayout_6->setObjectName(QString::fromUtf8("formLayout_6"));
        label_4 = new QLabel(userinfo);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_6->setWidget(0, QFormLayout::LabelRole, label_4);

        editIdCard = new QLineEdit(userinfo);
        editIdCard->setObjectName(QString::fromUtf8("editIdCard"));
        editIdCard->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_6->setWidget(0, QFormLayout::FieldRole, editIdCard);


        gridLayout_3->addLayout(formLayout_6, 14, 0, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_6, 15, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_15 = new QLabel(userinfo);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label_15);

        comboBox_userType = new QComboBox(userinfo);
        comboBox_userType->setObjectName(QString::fromUtf8("comboBox_userType"));
        comboBox_userType->setEnabled(false);
        comboBox_userType->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(comboBox_userType);


        gridLayout_3->addLayout(horizontalLayout, 6, 0, 1, 1);

        verticalSpacer_16 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_16, 5, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_16 = new QLabel(userinfo);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_16);

        comboBox_userRange = new QComboBox(userinfo);
        comboBox_userRange->setObjectName(QString::fromUtf8("comboBox_userRange"));
        comboBox_userRange->setEnabled(false);
        comboBox_userRange->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(comboBox_userRange);


        gridLayout_3->addLayout(horizontalLayout_2, 8, 0, 1, 1);

        verticalSpacer_17 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_17, 9, 0, 1, 1);


        gridLayout_6->addLayout(gridLayout_3, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(118, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        formLayout_8 = new QFormLayout();
        formLayout_8->setObjectName(QString::fromUtf8("formLayout_8"));
        formLayout_8->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_8 = new QLabel(userinfo);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_8->setWidget(0, QFormLayout::LabelRole, label_8);

        editCompany = new QLineEdit(userinfo);
        editCompany->setObjectName(QString::fromUtf8("editCompany"));

        formLayout_8->setWidget(0, QFormLayout::FieldRole, editCompany);


        gridLayout_2->addLayout(formLayout_8, 0, 0, 1, 1);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_7, 1, 0, 1, 1);

        formLayout_9 = new QFormLayout();
        formLayout_9->setObjectName(QString::fromUtf8("formLayout_9"));
        label_9 = new QLabel(userinfo);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_9->setWidget(0, QFormLayout::LabelRole, label_9);

        editDepartment = new QLineEdit(userinfo);
        editDepartment->setObjectName(QString::fromUtf8("editDepartment"));

        formLayout_9->setWidget(0, QFormLayout::FieldRole, editDepartment);


        gridLayout_2->addLayout(formLayout_9, 2, 0, 1, 1);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_8, 5, 0, 1, 1);

        formLayout_10 = new QFormLayout();
        formLayout_10->setObjectName(QString::fromUtf8("formLayout_10"));
        label_10 = new QLabel(userinfo);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_10->setWidget(0, QFormLayout::LabelRole, label_10);

        editDuty = new QLineEdit(userinfo);
        editDuty->setObjectName(QString::fromUtf8("editDuty"));

        formLayout_10->setWidget(0, QFormLayout::FieldRole, editDuty);


        gridLayout_2->addLayout(formLayout_10, 6, 0, 1, 1);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_9, 7, 0, 1, 1);

        formLayout_11 = new QFormLayout();
        formLayout_11->setObjectName(QString::fromUtf8("formLayout_11"));
        label_11 = new QLabel(userinfo);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_11->setWidget(0, QFormLayout::LabelRole, label_11);

        editMTel = new QLineEdit(userinfo);
        editMTel->setObjectName(QString::fromUtf8("editMTel"));

        formLayout_11->setWidget(0, QFormLayout::FieldRole, editMTel);


        gridLayout_2->addLayout(formLayout_11, 8, 0, 1, 1);

        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_10, 9, 0, 1, 1);

        formLayout_12 = new QFormLayout();
        formLayout_12->setObjectName(QString::fromUtf8("formLayout_12"));
        label_6 = new QLabel(userinfo);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_12->setWidget(0, QFormLayout::LabelRole, label_6);

        editTel = new QLineEdit(userinfo);
        editTel->setObjectName(QString::fromUtf8("editTel"));

        formLayout_12->setWidget(0, QFormLayout::FieldRole, editTel);


        gridLayout_2->addLayout(formLayout_12, 10, 0, 1, 1);

        verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_11, 11, 0, 1, 1);

        formLayout_13 = new QFormLayout();
        formLayout_13->setObjectName(QString::fromUtf8("formLayout_13"));
        label_7 = new QLabel(userinfo);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_13->setWidget(0, QFormLayout::LabelRole, label_7);

        editEmail = new QLineEdit(userinfo);
        editEmail->setObjectName(QString::fromUtf8("editEmail"));

        formLayout_13->setWidget(0, QFormLayout::FieldRole, editEmail);


        gridLayout_2->addLayout(formLayout_13, 12, 0, 1, 1);

        verticalSpacer_12 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_12, 13, 0, 1, 1);

        formLayout_14 = new QFormLayout();
        formLayout_14->setObjectName(QString::fromUtf8("formLayout_14"));
        label_14 = new QLabel(userinfo);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_14->setWidget(0, QFormLayout::LabelRole, label_14);

        editGroup = new QComboBox(userinfo);
        editGroup->setObjectName(QString::fromUtf8("editGroup"));
        editGroup->setEnabled(false);
        editGroup->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        editGroup->setEditable(false);

        formLayout_14->setWidget(0, QFormLayout::FieldRole, editGroup);


        gridLayout_2->addLayout(formLayout_14, 14, 0, 1, 1);

        formLayout_7 = new QFormLayout();
        formLayout_7->setObjectName(QString::fromUtf8("formLayout_7"));
        formLayout_7->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        editEdu = new QComboBox(userinfo);
        editEdu->setObjectName(QString::fromUtf8("editEdu"));
        editEdu->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_7->setWidget(0, QFormLayout::FieldRole, editEdu);

        label_5 = new QLabel(userinfo);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        formLayout_7->setWidget(0, QFormLayout::LabelRole, label_5);


        gridLayout_2->addLayout(formLayout_7, 4, 0, 1, 1);

        verticalSpacer_15 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_15, 3, 0, 1, 1);


        gridLayout_6->addLayout(gridLayout_2, 0, 3, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(88, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_3, 0, 4, 1, 1);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        horizontalSpacer_6 = new QSpacerItem(178, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_6, 0, 0, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_4, 0, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_5, 0, 3, 1, 1);

        buttonExit = new QPushButton(userinfo);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setMinimumSize(QSize(120, 0));
        buttonExit->setStyleSheet(QString::fromUtf8(""));

        gridLayout_4->addWidget(buttonExit, 0, 4, 1, 1);

        buttonEnter = new QPushButton(userinfo);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setMinimumSize(QSize(120, 0));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        gridLayout_4->addWidget(buttonEnter, 0, 2, 1, 1);


        gridLayout_5->addLayout(gridLayout_4, 0, 2, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(178, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_7, 0, 3, 1, 1);

        buttonApply = new QPushButton(userinfo);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setMinimumSize(QSize(120, 0));
        buttonApply->setStyleSheet(QString::fromUtf8(""));

        gridLayout_5->addWidget(buttonApply, 0, 1, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 1, 0, 1, 5);


        gridLayout_7->addLayout(gridLayout_6, 1, 0, 1, 1);

        verticalSpacer_13 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_13, 2, 0, 1, 1);


        gridLayout_8->addLayout(gridLayout_7, 0, 0, 1, 1);


        retranslateUi(userinfo);

        QMetaObject::connectSlotsByName(userinfo);
    } // setupUi

    void retranslateUi(QWidget *userinfo)
    {
        userinfo->setWindowTitle(QApplication::translate("userinfo", "\344\270\252\344\272\272\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("userinfo", "  \347\224\250\346\210\267\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("userinfo", "\344\270\255\346\226\207\345\247\223\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("userinfo", "\350\247\222    \350\211\262\357\274\232", 0, QApplication::UnicodeUTF8));
        editRole->clear();
        editRole->insertItems(0, QStringList()
         << QApplication::translate("userinfo", "\345\255\246\345\221\230", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\346\225\231\345\221\230", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\347\263\273\347\273\237", 0, QApplication::UnicodeUTF8)
        );
        label_13->setText(QApplication::translate("userinfo", "\346\200\247    \345\210\253\357\274\232", 0, QApplication::UnicodeUTF8));
        editSex->clear();
        editSex->insertItems(0, QStringList()
         << QApplication::translate("userinfo", "\347\224\267", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\345\245\263", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("userinfo", "\345\257\206    \347\240\201\357\274\232", 0, QApplication::UnicodeUTF8));
        pushButtonChgPsd->setText(QApplication::translate("userinfo", "\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("userinfo", "\350\272\253\344\273\275\350\257\201\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("userinfo", "\345\255\246\345\221\230\345\267\245\347\247\215\357\274\232", 0, QApplication::UnicodeUTF8));
        comboBox_userType->clear();
        comboBox_userType->insertItems(0, QStringList()
         << QApplication::translate("userinfo", "\345\210\235\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\344\270\255\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\351\253\230\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\346\212\200\345\270\210\345\217\212\351\253\230\347\272\247\346\212\200\345\270\210", 0, QApplication::UnicodeUTF8)
        );
        label_16->setText(QApplication::translate("userinfo", "\345\255\246\345\221\230\347\272\247\345\210\253\357\274\232", 0, QApplication::UnicodeUTF8));
        comboBox_userRange->clear();
        comboBox_userRange->insertItems(0, QStringList()
         << QApplication::translate("userinfo", "\344\270\200\347\272\247", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\344\272\214\347\272\247", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\344\270\211\347\272\247", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\345\233\233\347\272\247", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\344\272\224\347\272\247", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\345\205\255\347\272\247", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\344\270\203\347\272\247", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\345\205\253\347\272\247", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\344\271\235\347\272\247", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\345\215\201\347\272\247", 0, QApplication::UnicodeUTF8)
        );
        label_8->setText(QApplication::translate("userinfo", "\345\215\225    \344\275\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("userinfo", "\351\203\250    \351\227\250\357\274\232", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("userinfo", "\350\201\214    \345\212\241\357\274\232", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("userinfo", "\345\212\236\345\205\254\347\224\265\350\257\235\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("userinfo", "\347\247\273\345\212\250\347\224\265\350\257\235\357\274\232", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("userinfo", "   Email\357\274\232", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("userinfo", "\345\210\206\347\273\204\344\277\241\346\201\257\357\274\232", 0, QApplication::UnicodeUTF8));
        editGroup->clear();
        editGroup->insertItems(0, QStringList()
         << QApplication::translate("userinfo", "\346\227\240\345\210\206\347\273\204", 0, QApplication::UnicodeUTF8)
        );
        editEdu->clear();
        editEdu->insertItems(0, QStringList()
         << QApplication::translate("userinfo", "\345\244\247\345\255\246\344\273\245\344\270\213", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\344\270\223\347\247\221", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\346\234\254\347\247\221", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("userinfo", "\347\240\224\347\251\266\347\224\237\345\217\212\344\273\245\344\270\212", 0, QApplication::UnicodeUTF8)
        );
        label_5->setText(QApplication::translate("userinfo", "\345\255\246    \345\216\206\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("userinfo", "\345\217\226  \346\266\210", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("userinfo", "\347\241\256  \345\256\232", 0, QApplication::UnicodeUTF8));
        buttonApply->setText(QApplication::translate("userinfo", "\345\272\224  \347\224\250", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(userinfo);
    } // retranslateUi

};

namespace Ui {
    class userinfo: public Ui_userinfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERINFO_H
