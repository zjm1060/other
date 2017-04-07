/********************************************************************************
** Form generated from reading ui file 'fault.ui'
**
** Created: Thu Jan 15 10:26:16 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FAULT_H
#define UI_FAULT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CFaultTable
{
public:
    QHBoxLayout *horizontalLayout;
    QTableWidget *listFault;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QPushButton *buttonFaultData;
    QPushButton *buttonFaultAdd;
    QPushButton *buttonFaultDel;
    QPushButton *buttonFaultModify;
    QPushButton *buttonStnDev;
    QPushButton *buttonRefresh;
    QPushButton *buttonExit;

    void setupUi(QDialog *CFaultTable)
    {
        if (CFaultTable->objectName().isEmpty())
            CFaultTable->setObjectName(QString::fromUtf8("CFaultTable"));
        CFaultTable->resize(800, 550);
        horizontalLayout = new QHBoxLayout(CFaultTable);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listFault = new QTableWidget(CFaultTable);
        if (listFault->columnCount() < 8)
            listFault->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        listFault->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        listFault->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        listFault->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        listFault->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        listFault->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        listFault->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        listFault->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        listFault->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        listFault->setObjectName(QString::fromUtf8("listFault"));
        listFault->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        listFault->setSortingEnabled(true);
        listFault->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout->addWidget(listFault);

        frame = new QFrame(CFaultTable);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(150, 0));
        frame->setMaximumSize(QSize(150, 16777215));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonFaultData = new QPushButton(frame);
        buttonFaultData->setObjectName(QString::fromUtf8("buttonFaultData"));
        buttonFaultData->setMinimumSize(QSize(0, 50));
        buttonFaultData->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonFaultData);

        buttonFaultAdd = new QPushButton(frame);
        buttonFaultAdd->setObjectName(QString::fromUtf8("buttonFaultAdd"));
        buttonFaultAdd->setMinimumSize(QSize(0, 50));
        buttonFaultAdd->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonFaultAdd);

        buttonFaultDel = new QPushButton(frame);
        buttonFaultDel->setObjectName(QString::fromUtf8("buttonFaultDel"));
        buttonFaultDel->setMinimumSize(QSize(0, 50));
        buttonFaultDel->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonFaultDel);

        buttonFaultModify = new QPushButton(frame);
        buttonFaultModify->setObjectName(QString::fromUtf8("buttonFaultModify"));
        buttonFaultModify->setMinimumSize(QSize(0, 50));
        buttonFaultModify->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonFaultModify);

        buttonStnDev = new QPushButton(frame);
        buttonStnDev->setObjectName(QString::fromUtf8("buttonStnDev"));
        buttonStnDev->setMinimumSize(QSize(0, 50));
        buttonStnDev->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonStnDev);

        buttonRefresh = new QPushButton(frame);
        buttonRefresh->setObjectName(QString::fromUtf8("buttonRefresh"));
        buttonRefresh->setMinimumSize(QSize(0, 50));
        buttonRefresh->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonRefresh);

        buttonExit = new QPushButton(frame);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setMinimumSize(QSize(0, 50));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonExit);


        horizontalLayout->addWidget(frame);

        QWidget::setTabOrder(buttonFaultData, buttonFaultAdd);
        QWidget::setTabOrder(buttonFaultAdd, buttonFaultDel);
        QWidget::setTabOrder(buttonFaultDel, buttonFaultModify);
        QWidget::setTabOrder(buttonFaultModify, buttonRefresh);
        QWidget::setTabOrder(buttonRefresh, buttonExit);
        QWidget::setTabOrder(buttonExit, listFault);

        retranslateUi(CFaultTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CFaultTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CFaultTable);
    } // setupUi

    void retranslateUi(QDialog *CFaultTable)
    {
        CFaultTable->setWindowTitle(QApplication::translate("CFaultTable", "\346\225\205\351\232\234\345\272\223", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = listFault->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CFaultTable", "\346\225\205\351\232\234ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = listFault->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CFaultTable", "\346\225\205\351\232\234\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = listFault->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CFaultTable", "\347\224\250\346\210\267\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = listFault->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CFaultTable", "\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = listFault->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("CFaultTable", "\345\210\233\345\273\272\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = listFault->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("CFaultTable", "\345\244\207\346\263\250", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = listFault->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("CFaultTable", "\345\216\202\347\253\231\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = listFault->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("CFaultTable", "\350\256\276\345\244\207\345\220\215", 0, QApplication::UnicodeUTF8));
        buttonFaultData->setText(QApplication::translate("CFaultTable", "\346\225\205\351\232\234\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        buttonFaultAdd->setText(QApplication::translate("CFaultTable", "\346\267\273\345\212\240\346\225\205\351\232\234", 0, QApplication::UnicodeUTF8));
        buttonFaultDel->setText(QApplication::translate("CFaultTable", "\345\210\240\351\231\244\346\225\205\351\232\234", 0, QApplication::UnicodeUTF8));
        buttonFaultModify->setText(QApplication::translate("CFaultTable", "\344\277\256\346\224\271\346\225\205\351\232\234", 0, QApplication::UnicodeUTF8));
        buttonStnDev->setText(QApplication::translate("CFaultTable", "\351\200\211\346\213\251\345\216\202\347\253\231\350\256\276\345\244\207", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QApplication::translate("CFaultTable", "\345\210\267  \346\226\260", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CFaultTable", "\351\200\200  \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CFaultTable);
    } // retranslateUi

};

namespace Ui {
    class CFaultTable: public Ui_CFaultTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FAULT_H
