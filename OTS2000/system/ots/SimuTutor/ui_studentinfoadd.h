/********************************************************************************
** Form generated from reading ui file 'studentinfoadd.ui'
**
** Created: Thu Jan 15 10:25:16 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STUDENTINFOADD_H
#define UI_STUDENTINFOADD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CStudentInfoAddTable
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_18;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QLineEdit *editName;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_12;
    QLineEdit *editLongName;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_2;
    QComboBox *editRole;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_13;
    QComboBox *editSex;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_3;
    QLineEdit *editPassword;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_4;
    QLineEdit *editIdCard;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_5;
    QComboBox *editEdu;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_11;
    QLineEdit *editMTel;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_6;
    QLineEdit *editTel;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_7;
    QLineEdit *editEmail;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_8;
    QLineEdit *editCompany;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_9;
    QLineEdit *editDepartment;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_10;
    QLineEdit *editDuty;
    QHBoxLayout *horizontalLayout;
    QLabel *label_16;
    QComboBox *comboBox_userRange;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_15;
    QComboBox *comboBox_userType;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_14;
    QComboBox *editGroup;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_17;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonApply;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *buttonEnter;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *buttonExit;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QDialog *CStudentInfoAddTable)
    {
        if (CStudentInfoAddTable->objectName().isEmpty())
            CStudentInfoAddTable->setObjectName(QString::fromUtf8("CStudentInfoAddTable"));
        CStudentInfoAddTable->resize(502, 696);
        CStudentInfoAddTable->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        gridLayout = new QGridLayout(CStudentInfoAddTable);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(188, 10, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        horizontalSpacer_5 = new QSpacerItem(30, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_18->addItem(horizontalSpacer_5);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(CStudentInfoAddTable);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label);

        editName = new QLineEdit(CStudentInfoAddTable);
        editName->setObjectName(QString::fromUtf8("editName"));
        editName->setEnabled(true);

        horizontalLayout_4->addWidget(editName);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_12 = new QLabel(CStudentInfoAddTable);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(label_12);

        editLongName = new QLineEdit(CStudentInfoAddTable);
        editLongName->setObjectName(QString::fromUtf8("editLongName"));

        horizontalLayout_5->addWidget(editLongName);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_2 = new QLabel(CStudentInfoAddTable);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_6->addWidget(label_2);

        editRole = new QComboBox(CStudentInfoAddTable);
        editRole->setObjectName(QString::fromUtf8("editRole"));

        horizontalLayout_6->addWidget(editRole);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_13 = new QLabel(CStudentInfoAddTable);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_7->addWidget(label_13);

        editSex = new QComboBox(CStudentInfoAddTable);
        editSex->setObjectName(QString::fromUtf8("editSex"));

        horizontalLayout_7->addWidget(editSex);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_3 = new QLabel(CStudentInfoAddTable);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_8->addWidget(label_3);

        editPassword = new QLineEdit(CStudentInfoAddTable);
        editPassword->setObjectName(QString::fromUtf8("editPassword"));

        horizontalLayout_8->addWidget(editPassword);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_4 = new QLabel(CStudentInfoAddTable);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_9->addWidget(label_4);

        editIdCard = new QLineEdit(CStudentInfoAddTable);
        editIdCard->setObjectName(QString::fromUtf8("editIdCard"));

        horizontalLayout_9->addWidget(editIdCard);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_5 = new QLabel(CStudentInfoAddTable);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_10->addWidget(label_5);

        editEdu = new QComboBox(CStudentInfoAddTable);
        editEdu->setObjectName(QString::fromUtf8("editEdu"));

        horizontalLayout_10->addWidget(editEdu);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_11 = new QLabel(CStudentInfoAddTable);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_11->addWidget(label_11);

        editMTel = new QLineEdit(CStudentInfoAddTable);
        editMTel->setObjectName(QString::fromUtf8("editMTel"));

        horizontalLayout_11->addWidget(editMTel);


        verticalLayout->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_6 = new QLabel(CStudentInfoAddTable);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_12->addWidget(label_6);

        editTel = new QLineEdit(CStudentInfoAddTable);
        editTel->setObjectName(QString::fromUtf8("editTel"));

        horizontalLayout_12->addWidget(editTel);


        verticalLayout->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_7 = new QLabel(CStudentInfoAddTable);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_13->addWidget(label_7);

        editEmail = new QLineEdit(CStudentInfoAddTable);
        editEmail->setObjectName(QString::fromUtf8("editEmail"));

        horizontalLayout_13->addWidget(editEmail);


        verticalLayout->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        label_8 = new QLabel(CStudentInfoAddTable);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_14->addWidget(label_8);

        editCompany = new QLineEdit(CStudentInfoAddTable);
        editCompany->setObjectName(QString::fromUtf8("editCompany"));

        horizontalLayout_14->addWidget(editCompany);


        verticalLayout->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_9 = new QLabel(CStudentInfoAddTable);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_15->addWidget(label_9);

        editDepartment = new QLineEdit(CStudentInfoAddTable);
        editDepartment->setObjectName(QString::fromUtf8("editDepartment"));

        horizontalLayout_15->addWidget(editDepartment);


        verticalLayout->addLayout(horizontalLayout_15);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        label_10 = new QLabel(CStudentInfoAddTable);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_16->addWidget(label_10);

        editDuty = new QLineEdit(CStudentInfoAddTable);
        editDuty->setObjectName(QString::fromUtf8("editDuty"));

        horizontalLayout_16->addWidget(editDuty);


        verticalLayout->addLayout(horizontalLayout_16);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_16 = new QLabel(CStudentInfoAddTable);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label_16);

        comboBox_userRange = new QComboBox(CStudentInfoAddTable);
        comboBox_userRange->setObjectName(QString::fromUtf8("comboBox_userRange"));
        comboBox_userRange->setEnabled(false);
        comboBox_userRange->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(comboBox_userRange);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_15 = new QLabel(CStudentInfoAddTable);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_15);

        comboBox_userType = new QComboBox(CStudentInfoAddTable);
        comboBox_userType->setObjectName(QString::fromUtf8("comboBox_userType"));
        comboBox_userType->setEnabled(true);
        comboBox_userType->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(comboBox_userType);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_14 = new QLabel(CStudentInfoAddTable);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_14);

        editGroup = new QComboBox(CStudentInfoAddTable);
        editGroup->setObjectName(QString::fromUtf8("editGroup"));
        editGroup->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        editGroup->setEditable(false);

        horizontalLayout_3->addWidget(editGroup);


        verticalLayout->addLayout(horizontalLayout_3);


        horizontalLayout_18->addLayout(verticalLayout);

        horizontalSpacer_6 = new QSpacerItem(30, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_18->addItem(horizontalSpacer_6);


        verticalLayout_2->addLayout(horizontalLayout_18);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer);

        buttonApply = new QPushButton(CStudentInfoAddTable);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_17->addWidget(buttonApply);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_2);

        buttonEnter = new QPushButton(CStudentInfoAddTable);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_17->addWidget(buttonEnter);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_3);

        buttonExit = new QPushButton(CStudentInfoAddTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_17->addWidget(buttonExit);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_4);


        verticalLayout_2->addLayout(horizontalLayout_17);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);

        QWidget::setTabOrder(editName, editLongName);
        QWidget::setTabOrder(editLongName, editRole);
        QWidget::setTabOrder(editRole, editSex);
        QWidget::setTabOrder(editSex, editPassword);
        QWidget::setTabOrder(editPassword, editIdCard);
        QWidget::setTabOrder(editIdCard, editEdu);
        QWidget::setTabOrder(editEdu, editMTel);
        QWidget::setTabOrder(editMTel, editTel);
        QWidget::setTabOrder(editTel, editEmail);
        QWidget::setTabOrder(editEmail, editCompany);
        QWidget::setTabOrder(editCompany, editDepartment);
        QWidget::setTabOrder(editDepartment, editDuty);

        retranslateUi(CStudentInfoAddTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CStudentInfoAddTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CStudentInfoAddTable);
    } // setupUi

    void retranslateUi(QDialog *CStudentInfoAddTable)
    {
        CStudentInfoAddTable->setWindowTitle(QApplication::translate("CStudentInfoAddTable", "\346\267\273\345\212\240\347\224\250\346\210\267\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CStudentInfoAddTable", "\347\224\250\346\210\267\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("CStudentInfoAddTable", "\344\270\255\346\226\207\345\247\223\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CStudentInfoAddTable", "\350\247\222\350\211\262\357\274\232", 0, QApplication::UnicodeUTF8));
        editRole->clear();
        editRole->insertItems(0, QStringList()
         << QApplication::translate("CStudentInfoAddTable", "\345\255\246\345\221\230", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoAddTable", "\346\225\231\345\221\230", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoAddTable", "\347\263\273\347\273\237", 0, QApplication::UnicodeUTF8)
        );
        label_13->setText(QApplication::translate("CStudentInfoAddTable", "\346\200\247\345\210\253\357\274\232", 0, QApplication::UnicodeUTF8));
        editSex->clear();
        editSex->insertItems(0, QStringList()
         << QApplication::translate("CStudentInfoAddTable", "\347\224\267", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoAddTable", "\345\245\263", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("CStudentInfoAddTable", "\345\257\206\347\240\201\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CStudentInfoAddTable", "\350\272\253\344\273\275\350\257\201\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CStudentInfoAddTable", "\345\255\246\345\216\206\357\274\232", 0, QApplication::UnicodeUTF8));
        editEdu->clear();
        editEdu->insertItems(0, QStringList()
         << QApplication::translate("CStudentInfoAddTable", "\345\244\247\345\255\246\344\273\245\344\270\213", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoAddTable", "\344\270\223\347\247\221", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoAddTable", "\346\234\254\347\247\221", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoAddTable", "\347\240\224\347\251\266\347\224\237\345\217\212\344\273\245\344\270\212", 0, QApplication::UnicodeUTF8)
        );
        label_11->setText(QApplication::translate("CStudentInfoAddTable", "\345\212\236\345\205\254\347\224\265\350\257\235\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CStudentInfoAddTable", "\347\247\273\345\212\250\347\224\265\350\257\235\357\274\232", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("CStudentInfoAddTable", "Email\357\274\232", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("CStudentInfoAddTable", "\345\215\225\344\275\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("CStudentInfoAddTable", "\351\203\250\351\227\250\357\274\232", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("CStudentInfoAddTable", "\350\201\214\345\212\241\357\274\232", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("CStudentInfoAddTable", "\345\255\246\345\221\230\347\272\247\345\210\253\357\274\232", 0, QApplication::UnicodeUTF8));
        comboBox_userRange->clear();
        comboBox_userRange->insertItems(0, QStringList()
         << QApplication::translate("CStudentInfoAddTable", "\344\270\200\347\272\247", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoAddTable", "\344\272\214\347\272\247", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoAddTable", "\344\270\211\347\272\247", 0, QApplication::UnicodeUTF8)
        );
        label_15->setText(QApplication::translate("CStudentInfoAddTable", "\345\255\246\345\221\230\345\267\245\347\247\215\357\274\232", 0, QApplication::UnicodeUTF8));
        comboBox_userType->clear();
        comboBox_userType->insertItems(0, QStringList()
         << QApplication::translate("CStudentInfoAddTable", "\346\226\260\345\255\246\345\221\230", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoAddTable", "\345\210\235\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoAddTable", "\344\270\255\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoAddTable", "\351\253\230\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoAddTable", "\346\212\200\345\270\210", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoAddTable", "\351\253\230\347\272\247\346\212\200\345\270\210", 0, QApplication::UnicodeUTF8)
        );
        label_14->setText(QApplication::translate("CStudentInfoAddTable", "\345\210\206\347\273\204\344\277\241\346\201\257\357\274\232", 0, QApplication::UnicodeUTF8));
        editGroup->clear();
        editGroup->insertItems(0, QStringList()
         << QApplication::translate("CStudentInfoAddTable", "\350\257\267\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8)
        );
        buttonApply->setText(QApplication::translate("CStudentInfoAddTable", "\345\272\224  \347\224\250", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("CStudentInfoAddTable", "\347\241\256  \345\256\232", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CStudentInfoAddTable", "\351\200\200  \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CStudentInfoAddTable);
    } // retranslateUi

};

namespace Ui {
    class CStudentInfoAddTable: public Ui_CStudentInfoAddTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STUDENTINFOADD_H
