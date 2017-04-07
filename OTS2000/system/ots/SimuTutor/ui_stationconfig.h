/********************************************************************************
** Form generated from reading ui file 'stationconfig.ui'
**
** Created: Thu Jan 15 10:25:29 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STATIONCONFIG_H
#define UI_STATIONCONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CStationConfigTable
{
public:
    QHBoxLayout *horizontalLayout;
    QTableWidget *listStation;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QPushButton *buttonStationAdd;
    QPushButton *buttonStationModify;
    QPushButton *buttonStationDel;
    QSpacerItem *verticalSpacer;
    QPushButton *buttonRefresh;
    QPushButton *buttonExit;

    void setupUi(QDialog *CStationConfigTable)
    {
        if (CStationConfigTable->objectName().isEmpty())
            CStationConfigTable->setObjectName(QString::fromUtf8("CStationConfigTable"));
        CStationConfigTable->resize(800, 550);
        horizontalLayout = new QHBoxLayout(CStationConfigTable);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listStation = new QTableWidget(CStationConfigTable);
        if (listStation->columnCount() < 9)
            listStation->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        listStation->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        listStation->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        listStation->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        listStation->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        listStation->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        listStation->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        listStation->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        listStation->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        listStation->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        listStation->setObjectName(QString::fromUtf8("listStation"));
        listStation->setSortingEnabled(true);
        listStation->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout->addWidget(listStation);

        frame = new QFrame(CStationConfigTable);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(150, 0));
        frame->setMaximumSize(QSize(150, 16777215));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonStationAdd = new QPushButton(frame);
        buttonStationAdd->setObjectName(QString::fromUtf8("buttonStationAdd"));
        buttonStationAdd->setMinimumSize(QSize(0, 50));
        buttonStationAdd->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonStationAdd);

        buttonStationModify = new QPushButton(frame);
        buttonStationModify->setObjectName(QString::fromUtf8("buttonStationModify"));
        buttonStationModify->setMinimumSize(QSize(0, 50));
        buttonStationModify->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonStationModify);

        buttonStationDel = new QPushButton(frame);
        buttonStationDel->setObjectName(QString::fromUtf8("buttonStationDel"));
        buttonStationDel->setMinimumSize(QSize(0, 50));
        buttonStationDel->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonStationDel);

        verticalSpacer = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

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

        QWidget::setTabOrder(buttonStationAdd, buttonStationModify);
        QWidget::setTabOrder(buttonStationModify, buttonStationDel);
        QWidget::setTabOrder(buttonStationDel, buttonRefresh);
        QWidget::setTabOrder(buttonRefresh, buttonExit);
        QWidget::setTabOrder(buttonExit, listStation);

        retranslateUi(CStationConfigTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CStationConfigTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CStationConfigTable);
    } // setupUi

    void retranslateUi(QDialog *CStationConfigTable)
    {
        CStationConfigTable->setWindowTitle(QApplication::translate("CStationConfigTable", "\351\205\215\347\275\256\345\267\245\344\275\234\347\253\231", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = listStation->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CStationConfigTable", "\344\270\273\346\234\272ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = listStation->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CStationConfigTable", "\344\270\273\346\234\272\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = listStation->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CStationConfigTable", "\345\210\253\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = listStation->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CStationConfigTable", "A\347\275\221IP", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = listStation->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("CStationConfigTable", "B\347\275\221IP", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = listStation->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("CStationConfigTable", "\346\230\257\345\220\246\344\270\272\345\255\246\345\221\230\347\253\231", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = listStation->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("CStationConfigTable", "\346\230\257\345\220\246\344\270\272\346\225\231\345\221\230\347\253\231", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = listStation->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("CStationConfigTable", "\346\230\257\345\220\246\344\270\272\346\250\241\345\236\213\346\234\215\345\212\241\345\231\250", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = listStation->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("CStationConfigTable", "\346\250\241\345\236\213\346\234\215\345\212\241\345\231\250ID", 0, QApplication::UnicodeUTF8));
        buttonStationAdd->setText(QApplication::translate("CStationConfigTable", "\346\267\273\345\212\240\344\270\273\346\234\272", 0, QApplication::UnicodeUTF8));
        buttonStationModify->setText(QApplication::translate("CStationConfigTable", "\344\277\256\346\224\271\344\270\273\346\234\272", 0, QApplication::UnicodeUTF8));
        buttonStationDel->setText(QApplication::translate("CStationConfigTable", "\345\210\240\351\231\244\344\270\273\346\234\272", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QApplication::translate("CStationConfigTable", "\345\210\267  \346\226\260", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CStationConfigTable", "\351\200\200  \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CStationConfigTable);
    } // retranslateUi

};

namespace Ui {
    class CStationConfigTable: public Ui_CStationConfigTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATIONCONFIG_H
