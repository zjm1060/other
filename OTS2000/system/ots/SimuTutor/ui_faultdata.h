/********************************************************************************
** Form generated from reading ui file 'faultdata.ui'
**
** Created: Thu Jan 15 10:26:02 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FAULTDATA_H
#define UI_FAULTDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CFaultDataTable
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QGridLayout *gridLayout;
    QTableWidget *listFaultData;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonAdd;
    QPushButton *buttonModify;
    QPushButton *buttonDel;
    QPushButton *buttonExit;

    void setupUi(QDialog *CFaultDataTable)
    {
        if (CFaultDataTable->objectName().isEmpty())
            CFaultDataTable->setObjectName(QString::fromUtf8("CFaultDataTable"));
        CFaultDataTable->resize(898, 536);
        CFaultDataTable->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        verticalLayout = new QVBoxLayout(CFaultDataTable);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(CFaultDataTable);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listFaultData = new QTableWidget(frame);
        if (listFaultData->columnCount() < 8)
            listFaultData->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        listFaultData->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        listFaultData->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        listFaultData->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        listFaultData->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        listFaultData->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        listFaultData->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        listFaultData->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        listFaultData->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        listFaultData->setObjectName(QString::fromUtf8("listFaultData"));
        listFaultData->setAutoFillBackground(false);
        listFaultData->setLineWidth(1);
        listFaultData->setSortingEnabled(true);
        listFaultData->horizontalHeader()->setCascadingSectionResizes(false);
        listFaultData->horizontalHeader()->setDefaultSectionSize(100);
        listFaultData->horizontalHeader()->setMinimumSectionSize(50);
        listFaultData->horizontalHeader()->setStretchLastSection(true);

        gridLayout->addWidget(listFaultData, 0, 0, 1, 1);


        verticalLayout->addWidget(frame);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(40);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buttonAdd = new QPushButton(CFaultDataTable);
        buttonAdd->setObjectName(QString::fromUtf8("buttonAdd"));
        buttonAdd->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonAdd);

        buttonModify = new QPushButton(CFaultDataTable);
        buttonModify->setObjectName(QString::fromUtf8("buttonModify"));
        buttonModify->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonModify);

        buttonDel = new QPushButton(CFaultDataTable);
        buttonDel->setObjectName(QString::fromUtf8("buttonDel"));
        buttonDel->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonDel);

        buttonExit = new QPushButton(CFaultDataTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonExit);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CFaultDataTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CFaultDataTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CFaultDataTable);
    } // setupUi

    void retranslateUi(QDialog *CFaultDataTable)
    {
        CFaultDataTable->setWindowTitle(QApplication::translate("CFaultDataTable", "\346\225\205\351\232\234\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = listFaultData->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CFaultDataTable", "\347\202\271\351\200\273\350\276\221\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = listFaultData->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CFaultDataTable", "\346\225\205\351\232\234\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = listFaultData->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CFaultDataTable", "\345\210\235\345\247\213\345\200\274", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = listFaultData->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CFaultDataTable", "\347\233\256\346\240\207\345\200\274", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = listFaultData->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("CFaultDataTable", "\346\255\243\345\270\270\345\200\274", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = listFaultData->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("CFaultDataTable", "\345\217\230\345\214\226\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = listFaultData->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("CFaultDataTable", "\345\217\230\345\214\226\346\226\271\345\274\217", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = listFaultData->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("CFaultDataTable", "\345\273\266\350\277\237\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        buttonAdd->setText(QApplication::translate("CFaultDataTable", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        buttonModify->setText(QApplication::translate("CFaultDataTable", "\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        buttonDel->setText(QApplication::translate("CFaultDataTable", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CFaultDataTable", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CFaultDataTable);
    } // retranslateUi

};

namespace Ui {
    class CFaultDataTable: public Ui_CFaultDataTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FAULTDATA_H
