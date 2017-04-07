/********************************************************************************
** Form generated from reading ui file 'localopr.ui'
**
** Created: Thu Jan 15 10:25:59 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LOCALOPR_H
#define UI_LOCALOPR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CLocalOprTable
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QListWidget *listLocalOprType;
    QVBoxLayout *verticalLayout;
    QPushButton *buttonTypeAdd;
    QPushButton *buttonTypeDel;
    QPushButton *buttonTypeModify;
    QHBoxLayout *horizontalLayout_2;
    QTableWidget *listLocalOpr;
    QVBoxLayout *verticalLayout_2;
    QPushButton *buttonAdd;
    QPushButton *buttonDel;
    QPushButton *buttonModify;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *buttonRefresh;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *buttonExit;

    void setupUi(QDialog *CLocalOprTable)
    {
        if (CLocalOprTable->objectName().isEmpty())
            CLocalOprTable->setObjectName(QString::fromUtf8("CLocalOprTable"));
        CLocalOprTable->resize(620, 600);
        CLocalOprTable->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        gridLayout = new QGridLayout(CLocalOprTable);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listLocalOprType = new QListWidget(CLocalOprTable);
        listLocalOprType->setObjectName(QString::fromUtf8("listLocalOprType"));
        listLocalOprType->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listLocalOprType->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listLocalOprType->setSelectionBehavior(QAbstractItemView::SelectRows);

        horizontalLayout->addWidget(listLocalOprType);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonTypeAdd = new QPushButton(CLocalOprTable);
        buttonTypeAdd->setObjectName(QString::fromUtf8("buttonTypeAdd"));

        verticalLayout->addWidget(buttonTypeAdd);

        buttonTypeDel = new QPushButton(CLocalOprTable);
        buttonTypeDel->setObjectName(QString::fromUtf8("buttonTypeDel"));

        verticalLayout->addWidget(buttonTypeDel);

        buttonTypeModify = new QPushButton(CLocalOprTable);
        buttonTypeModify->setObjectName(QString::fromUtf8("buttonTypeModify"));

        verticalLayout->addWidget(buttonTypeModify);


        horizontalLayout->addLayout(verticalLayout);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        listLocalOpr = new QTableWidget(CLocalOprTable);
        listLocalOpr->setObjectName(QString::fromUtf8("listLocalOpr"));
        listLocalOpr->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listLocalOpr->setSelectionBehavior(QAbstractItemView::SelectRows);
        listLocalOpr->setShowGrid(false);
        listLocalOpr->horizontalHeader()->setVisible(false);
        listLocalOpr->horizontalHeader()->setStretchLastSection(true);
        listLocalOpr->verticalHeader()->setVisible(false);

        horizontalLayout_2->addWidget(listLocalOpr);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        buttonAdd = new QPushButton(CLocalOprTable);
        buttonAdd->setObjectName(QString::fromUtf8("buttonAdd"));

        verticalLayout_2->addWidget(buttonAdd);

        buttonDel = new QPushButton(CLocalOprTable);
        buttonDel->setObjectName(QString::fromUtf8("buttonDel"));

        verticalLayout_2->addWidget(buttonDel);

        buttonModify = new QPushButton(CLocalOprTable);
        buttonModify->setObjectName(QString::fromUtf8("buttonModify"));

        verticalLayout_2->addWidget(buttonModify);


        horizontalLayout_2->addLayout(verticalLayout_2);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(411, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        buttonRefresh = new QPushButton(CLocalOprTable);
        buttonRefresh->setObjectName(QString::fromUtf8("buttonRefresh"));

        horizontalLayout_3->addWidget(buttonRefresh);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        buttonExit = new QPushButton(CLocalOprTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));

        horizontalLayout_3->addWidget(buttonExit);


        gridLayout->addLayout(horizontalLayout_3, 2, 1, 1, 1);

        gridLayout->setRowStretch(0, 2);
        gridLayout->setRowStretch(1, 5);

        retranslateUi(CLocalOprTable);

        QMetaObject::connectSlotsByName(CLocalOprTable);
    } // setupUi

    void retranslateUi(QDialog *CLocalOprTable)
    {
        CLocalOprTable->setWindowTitle(QApplication::translate("CLocalOprTable", "\347\216\260\345\234\260\346\223\215\344\275\234\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        buttonTypeAdd->setText(QApplication::translate("CLocalOprTable", "\346\267\273\345\212\240\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        buttonTypeDel->setText(QApplication::translate("CLocalOprTable", "\345\210\240\351\231\244\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        buttonTypeModify->setText(QApplication::translate("CLocalOprTable", "\344\277\256\346\224\271\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        buttonAdd->setText(QApplication::translate("CLocalOprTable", "\346\267\273  \345\212\240", 0, QApplication::UnicodeUTF8));
        buttonDel->setText(QApplication::translate("CLocalOprTable", "\345\210\240  \351\231\244", 0, QApplication::UnicodeUTF8));
        buttonModify->setText(QApplication::translate("CLocalOprTable", "\344\277\256  \346\224\271", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QApplication::translate("CLocalOprTable", "\345\210\267\346\226\260", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CLocalOprTable", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CLocalOprTable);
    } // retranslateUi

};

namespace Ui {
    class CLocalOprTable: public Ui_CLocalOprTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOCALOPR_H
