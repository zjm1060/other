/********************************************************************************
** Form generated from reading ui file 'privilegemanager.ui'
**
** Created: Thu Jan 15 10:26:33 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PRIVILEGEMANAGER_H
#define UI_PRIVILEGEMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CPrivilegeMananger
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *editName;
    QListView *listUser;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout;
    QComboBox *comboMode;
    QListWidget *listPrivilege;
    QVBoxLayout *verticalLayout_3;
    QPushButton *buttonSelectAll;
    QPushButton *buttonSelectOff;
    QPushButton *buttonSelectInvert;
    QSpacerItem *verticalSpacer;
    QPushButton *buttonSave;
    QPushButton *buttonExit;

    void setupUi(QDialog *CPrivilegeMananger)
    {
        if (CPrivilegeMananger->objectName().isEmpty())
            CPrivilegeMananger->setObjectName(QString::fromUtf8("CPrivilegeMananger"));
        CPrivilegeMananger->resize(640, 592);
        CPrivilegeMananger->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        horizontalLayout = new QHBoxLayout(CPrivilegeMananger);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(CPrivilegeMananger);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFlat(false);
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 9, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(1);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        editName = new QLineEdit(groupBox);
        editName->setObjectName(QString::fromUtf8("editName"));

        verticalLayout_2->addWidget(editName);

        listUser = new QListView(groupBox);
        listUser->setObjectName(QString::fromUtf8("listUser"));
        listUser->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listUser->setSelectionBehavior(QAbstractItemView::SelectItems);

        verticalLayout_2->addWidget(listUser);


        verticalLayout_4->addLayout(verticalLayout_2);


        horizontalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(CPrivilegeMananger);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_5 = new QVBoxLayout(groupBox_2);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, -1, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        comboMode = new QComboBox(groupBox_2);
        comboMode->setObjectName(QString::fromUtf8("comboMode"));

        verticalLayout->addWidget(comboMode);

        listPrivilege = new QListWidget(groupBox_2);
        listPrivilege->setObjectName(QString::fromUtf8("listPrivilege"));

        verticalLayout->addWidget(listPrivilege);


        verticalLayout_5->addLayout(verticalLayout);


        horizontalLayout->addWidget(groupBox_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(12);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, 23, -1, -1);
        buttonSelectAll = new QPushButton(CPrivilegeMananger);
        buttonSelectAll->setObjectName(QString::fromUtf8("buttonSelectAll"));

        verticalLayout_3->addWidget(buttonSelectAll);

        buttonSelectOff = new QPushButton(CPrivilegeMananger);
        buttonSelectOff->setObjectName(QString::fromUtf8("buttonSelectOff"));

        verticalLayout_3->addWidget(buttonSelectOff);

        buttonSelectInvert = new QPushButton(CPrivilegeMananger);
        buttonSelectInvert->setObjectName(QString::fromUtf8("buttonSelectInvert"));

        verticalLayout_3->addWidget(buttonSelectInvert);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        buttonSave = new QPushButton(CPrivilegeMananger);
        buttonSave->setObjectName(QString::fromUtf8("buttonSave"));

        verticalLayout_3->addWidget(buttonSave);

        buttonExit = new QPushButton(CPrivilegeMananger);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));

        verticalLayout_3->addWidget(buttonExit);


        horizontalLayout->addLayout(verticalLayout_3);


        retranslateUi(CPrivilegeMananger);

        QMetaObject::connectSlotsByName(CPrivilegeMananger);
    } // setupUi

    void retranslateUi(QDialog *CPrivilegeMananger)
    {
        CPrivilegeMananger->setWindowTitle(QApplication::translate("CPrivilegeMananger", "\347\224\250\346\210\267\346\235\203\351\231\220\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CPrivilegeMananger", "\347\224\250\346\210\267\345\220\215", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("CPrivilegeMananger", "\346\235\203\351\231\220", 0, QApplication::UnicodeUTF8));
        comboMode->clear();
        comboMode->insertItems(0, QStringList()
         << QApplication::translate("CPrivilegeMananger", "\345\237\271\350\256\255\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CPrivilegeMananger", "\350\200\203\350\257\225\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8)
        );
        buttonSelectAll->setText(QApplication::translate("CPrivilegeMananger", "\345\205\250    \351\200\211", 0, QApplication::UnicodeUTF8));
        buttonSelectOff->setText(QApplication::translate("CPrivilegeMananger", "\345\217\226\346\266\210\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        buttonSelectInvert->setText(QApplication::translate("CPrivilegeMananger", "\345\217\215    \351\200\211", 0, QApplication::UnicodeUTF8));
        buttonSave->setText(QApplication::translate("CPrivilegeMananger", "\344\277\235  \345\255\230", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CPrivilegeMananger", "\351\200\200  \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CPrivilegeMananger);
    } // retranslateUi

};

namespace Ui {
    class CPrivilegeMananger: public Ui_CPrivilegeMananger {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVILEGEMANAGER_H
