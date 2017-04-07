/********************************************************************************
** Form generated from reading ui file 'selecttask.ui'
**
** Created: Wed Jan 14 10:22:21 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SELECTTASK_H
#define UI_SELECTTASK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CSelectTaskTable
{
public:
    QHBoxLayout *horizontalLayout;
    QTableWidget *listTask;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *buttonEnter;
    QPushButton *buttonExit;

    void setupUi(QDialog *CSelectTaskTable)
    {
        if (CSelectTaskTable->objectName().isEmpty())
            CSelectTaskTable->setObjectName(QString::fromUtf8("CSelectTaskTable"));
        CSelectTaskTable->resize(1035, 544);
        CSelectTaskTable->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        horizontalLayout = new QHBoxLayout(CSelectTaskTable);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listTask = new QTableWidget(CSelectTaskTable);
        if (listTask->columnCount() < 4)
            listTask->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        listTask->setObjectName(QString::fromUtf8("listTask"));
        listTask->setSortingEnabled(true);
        listTask->horizontalHeader()->setCascadingSectionResizes(false);
        listTask->horizontalHeader()->setDefaultSectionSize(200);
        listTask->horizontalHeader()->setMinimumSectionSize(150);
        listTask->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        listTask->horizontalHeader()->setStretchLastSection(true);
        listTask->verticalHeader()->setMinimumSectionSize(35);

        horizontalLayout->addWidget(listTask);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(30);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, -1, -1, 30);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonEnter = new QPushButton(CSelectTaskTable);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setMinimumSize(QSize(0, 40));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonEnter);

        buttonExit = new QPushButton(CSelectTaskTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setMinimumSize(QSize(0, 40));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonExit);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(CSelectTaskTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CSelectTaskTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CSelectTaskTable);
    } // setupUi

    void retranslateUi(QDialog *CSelectTaskTable)
    {
        CSelectTaskTable->setWindowTitle(QApplication::translate("CSelectTaskTable", "\351\200\211\346\213\251\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = listTask->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CSelectTaskTable", " \344\273\273   \345\212\241   \345\220\215   \347\247\260 ", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = listTask->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CSelectTaskTable", "\347\224\250\346\210\267\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = listTask->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CSelectTaskTable", "\344\273\273\345\212\241\345\220\257\345\212\250\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = listTask->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CSelectTaskTable", "\344\273\273\345\212\241\345\256\214\346\210\220\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("CSelectTaskTable", "\347\241\256   \345\256\232", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CSelectTaskTable", "\345\217\226   \346\266\210", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CSelectTaskTable);
    } // retranslateUi

};

namespace Ui {
    class CSelectTaskTable: public Ui_CSelectTaskTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTTASK_H
