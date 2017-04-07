/********************************************************************************
** Form generated from reading ui file 'groups.ui'
**
** Created: Thu Jan 15 10:25:15 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GROUPS_H
#define UI_GROUPS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CGroupsTable
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QListWidget *listGroups;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *buttonAddGroups;
    QPushButton *buttonDelGroups;
    QPushButton *buttonModifyGroups;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QPushButton *buttonNoGroupsUser;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *buttonSetGroupsHead;
    QPushButton *buttonDelUser;
    QListWidget *listGroupsUser;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonExit;

    void setupUi(QDialog *CGroupsTable)
    {
        if (CGroupsTable->objectName().isEmpty())
            CGroupsTable->setObjectName(QString::fromUtf8("CGroupsTable"));
        CGroupsTable->resize(428, 486);
        CGroupsTable->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        gridLayout = new QGridLayout(CGroupsTable);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(CGroupsTable);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, -1, 0, 0);
        listGroups = new QListWidget(groupBox);
        listGroups->setObjectName(QString::fromUtf8("listGroups"));
        listGroups->setMaximumSize(QSize(180, 16777215));

        verticalLayout->addWidget(listGroups);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        buttonAddGroups = new QPushButton(groupBox);
        buttonAddGroups->setObjectName(QString::fromUtf8("buttonAddGroups"));
        buttonAddGroups->setMaximumSize(QSize(45, 16777215));

        horizontalLayout_2->addWidget(buttonAddGroups);

        buttonDelGroups = new QPushButton(groupBox);
        buttonDelGroups->setObjectName(QString::fromUtf8("buttonDelGroups"));
        buttonDelGroups->setMaximumSize(QSize(45, 16777215));

        horizontalLayout_2->addWidget(buttonDelGroups);

        buttonModifyGroups = new QPushButton(groupBox);
        buttonModifyGroups->setObjectName(QString::fromUtf8("buttonModifyGroups"));
        buttonModifyGroups->setMaximumSize(QSize(45, 16777215));

        horizontalLayout_2->addWidget(buttonModifyGroups);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addWidget(groupBox, 0, 0, 2, 1);

        groupBox_2 = new QGroupBox(CGroupsTable);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setSizeConstraint(QLayout::SetMaximumSize);
        verticalLayout_4->setContentsMargins(0, -1, 0, 0);
        buttonNoGroupsUser = new QPushButton(groupBox_2);
        buttonNoGroupsUser->setObjectName(QString::fromUtf8("buttonNoGroupsUser"));
        buttonNoGroupsUser->setCheckable(false);
        buttonNoGroupsUser->setChecked(false);
        buttonNoGroupsUser->setAutoRepeat(false);
        buttonNoGroupsUser->setAutoRepeatDelay(300);
        buttonNoGroupsUser->setDefault(false);
        buttonNoGroupsUser->setFlat(false);

        verticalLayout_4->addWidget(buttonNoGroupsUser);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        buttonSetGroupsHead = new QPushButton(groupBox_2);
        buttonSetGroupsHead->setObjectName(QString::fromUtf8("buttonSetGroupsHead"));

        horizontalLayout_3->addWidget(buttonSetGroupsHead);

        buttonDelUser = new QPushButton(groupBox_2);
        buttonDelUser->setObjectName(QString::fromUtf8("buttonDelUser"));
        buttonDelUser->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_3->addWidget(buttonDelUser);


        verticalLayout_4->addLayout(horizontalLayout_3);

        listGroupsUser = new QListWidget(groupBox_2);
        listGroupsUser->setObjectName(QString::fromUtf8("listGroupsUser"));

        verticalLayout_4->addWidget(listGroupsUser);


        gridLayout->addWidget(groupBox_2, 0, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonExit = new QPushButton(CGroupsTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));

        horizontalLayout->addWidget(buttonExit);


        gridLayout->addLayout(horizontalLayout, 1, 1, 1, 1);


        retranslateUi(CGroupsTable);

        QMetaObject::connectSlotsByName(CGroupsTable);
    } // setupUi

    void retranslateUi(QDialog *CGroupsTable)
    {
        CGroupsTable->setWindowTitle(QApplication::translate("CGroupsTable", "\345\210\206\347\273\204\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CGroupsTable", "\345\210\206\347\273\204", 0, QApplication::UnicodeUTF8));
        buttonAddGroups->setText(QApplication::translate("CGroupsTable", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        buttonDelGroups->setText(QApplication::translate("CGroupsTable", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        buttonModifyGroups->setText(QApplication::translate("CGroupsTable", "\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("CGroupsTable", "\345\210\206\347\273\204\346\210\220\345\221\230", 0, QApplication::UnicodeUTF8));
        buttonNoGroupsUser->setText(QApplication::translate("CGroupsTable", "\346\267\273\345\212\240\346\234\252\345\210\206\347\273\204\345\255\246\345\221\230", 0, QApplication::UnicodeUTF8));
        buttonSetGroupsHead->setText(QApplication::translate("CGroupsTable", "\350\256\276\347\275\256\344\270\272\347\273\204\351\225\277", 0, QApplication::UnicodeUTF8));
        buttonDelUser->setText(QApplication::translate("CGroupsTable", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CGroupsTable", "\351\200\200 \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CGroupsTable);
    } // retranslateUi

};

namespace Ui {
    class CGroupsTable: public Ui_CGroupsTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROUPS_H
