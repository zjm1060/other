/********************************************************************************
** Form generated from reading ui file 'setscenefaultOLD.ui'
**
** Created: Thu Jan 15 10:28:29 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SETSCENEFAULTOLD_H
#define UI_SETSCENEFAULTOLD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableView>

QT_BEGIN_NAMESPACE

class Ui_CSetSceneFaultTable
{
public:
    QGridLayout *gridLayout;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonEnter;
    QPushButton *buttonCancel;
    QPushButton *buttonRefresh;
    QListWidget *listUserAndGroups;
    QLineEdit *editSearchName;
    QPushButton *buttonSearch;
    QPushButton *buttonShowAll;

    void setupUi(QDialog *CSetSceneFaultTable)
    {
        if (CSetSceneFaultTable->objectName().isEmpty())
            CSetSceneFaultTable->setObjectName(QString::fromUtf8("CSetSceneFaultTable"));
        CSetSceneFaultTable->resize(700, 400);
        gridLayout = new QGridLayout(CSetSceneFaultTable);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableView = new QTableView(CSetSceneFaultTable);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout->addWidget(tableView, 1, 1, 1, 3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(30);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonEnter = new QPushButton(CSetSceneFaultTable);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));

        horizontalLayout->addWidget(buttonEnter);

        buttonCancel = new QPushButton(CSetSceneFaultTable);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));

        horizontalLayout->addWidget(buttonCancel);


        gridLayout->addLayout(horizontalLayout, 2, 1, 1, 3);

        buttonRefresh = new QPushButton(CSetSceneFaultTable);
        buttonRefresh->setObjectName(QString::fromUtf8("buttonRefresh"));

        gridLayout->addWidget(buttonRefresh, 2, 0, 1, 1);

        listUserAndGroups = new QListWidget(CSetSceneFaultTable);
        listUserAndGroups->setObjectName(QString::fromUtf8("listUserAndGroups"));
        listUserAndGroups->setMaximumSize(QSize(150, 16777215));

        gridLayout->addWidget(listUserAndGroups, 0, 0, 2, 1);

        editSearchName = new QLineEdit(CSetSceneFaultTable);
        editSearchName->setObjectName(QString::fromUtf8("editSearchName"));
        editSearchName->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(editSearchName, 0, 1, 1, 1);

        buttonSearch = new QPushButton(CSetSceneFaultTable);
        buttonSearch->setObjectName(QString::fromUtf8("buttonSearch"));

        gridLayout->addWidget(buttonSearch, 0, 2, 1, 1);

        buttonShowAll = new QPushButton(CSetSceneFaultTable);
        buttonShowAll->setObjectName(QString::fromUtf8("buttonShowAll"));

        gridLayout->addWidget(buttonShowAll, 0, 3, 1, 1);


        retranslateUi(CSetSceneFaultTable);

        QMetaObject::connectSlotsByName(CSetSceneFaultTable);
    } // setupUi

    void retranslateUi(QDialog *CSetSceneFaultTable)
    {
        CSetSceneFaultTable->setWindowTitle(QString());
        buttonEnter->setText(QApplication::translate("CSetSceneFaultTable", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("CSetSceneFaultTable", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QApplication::translate("CSetSceneFaultTable", "\345\210\267\346\226\260", 0, QApplication::UnicodeUTF8));
        buttonSearch->setText(QApplication::translate("CSetSceneFaultTable", "\346\250\241\347\263\212\346\237\245\346\211\276", 0, QApplication::UnicodeUTF8));
        buttonShowAll->setText(QApplication::translate("CSetSceneFaultTable", "\346\230\276\347\244\272\345\205\250\351\203\250", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CSetSceneFaultTable);
    } // retranslateUi

};

namespace Ui {
    class CSetSceneFaultTable: public Ui_CSetSceneFaultTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETSCENEFAULTOLD_H
