/********************************************************************************
** Form generated from reading ui file 'traininfo.ui'
**
** Created: Thu Jan 15 10:24:32 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TRAININFO_H
#define UI_TRAININFO_H

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

class Ui_CTrainInfoTable
{
public:
    QHBoxLayout *horizontalLayout;
    QTableWidget *listTrainInfo;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QPushButton *buttonTrainInfoAdd;
    QPushButton *buttonTrainInfoDel;
    QPushButton *buttonTrainInfoModify;
    QPushButton *buttonExport;
    QSpacerItem *verticalSpacer;
    QPushButton *buttonRefresh;
    QPushButton *buttonExit;

    void setupUi(QDialog *CTrainInfoTable)
    {
        if (CTrainInfoTable->objectName().isEmpty())
            CTrainInfoTable->setObjectName(QString::fromUtf8("CTrainInfoTable"));
        CTrainInfoTable->resize(1112, 650);
        horizontalLayout = new QHBoxLayout(CTrainInfoTable);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listTrainInfo = new QTableWidget(CTrainInfoTable);
        if (listTrainInfo->columnCount() < 18)
            listTrainInfo->setColumnCount(18);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(11, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(12, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(13, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(14, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(15, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(16, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        listTrainInfo->setHorizontalHeaderItem(17, __qtablewidgetitem17);
        listTrainInfo->setObjectName(QString::fromUtf8("listTrainInfo"));
        listTrainInfo->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        listTrainInfo->setSortingEnabled(true);
        listTrainInfo->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout->addWidget(listTrainInfo);

        frame = new QFrame(CTrainInfoTable);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(150, 0));
        frame->setMaximumSize(QSize(150, 16777215));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonTrainInfoAdd = new QPushButton(frame);
        buttonTrainInfoAdd->setObjectName(QString::fromUtf8("buttonTrainInfoAdd"));
        buttonTrainInfoAdd->setMinimumSize(QSize(0, 50));
        buttonTrainInfoAdd->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonTrainInfoAdd);

        buttonTrainInfoDel = new QPushButton(frame);
        buttonTrainInfoDel->setObjectName(QString::fromUtf8("buttonTrainInfoDel"));
        buttonTrainInfoDel->setMinimumSize(QSize(0, 50));
        buttonTrainInfoDel->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonTrainInfoDel);

        buttonTrainInfoModify = new QPushButton(frame);
        buttonTrainInfoModify->setObjectName(QString::fromUtf8("buttonTrainInfoModify"));
        buttonTrainInfoModify->setMinimumSize(QSize(0, 50));
        buttonTrainInfoModify->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonTrainInfoModify);

        buttonExport = new QPushButton(frame);
        buttonExport->setObjectName(QString::fromUtf8("buttonExport"));
        buttonExport->setMinimumSize(QSize(0, 50));
        buttonExport->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonExport);

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


        retranslateUi(CTrainInfoTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CTrainInfoTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CTrainInfoTable);
    } // setupUi

    void retranslateUi(QDialog *CTrainInfoTable)
    {
        CTrainInfoTable->setWindowTitle(QApplication::translate("CTrainInfoTable", "SimuTutor \345\237\271\350\256\255\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = listTrainInfo->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CTrainInfoTable", "\347\224\250\346\210\267ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = listTrainInfo->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CTrainInfoTable", "\347\224\250\346\210\267\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = listTrainInfo->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CTrainInfoTable", "\344\270\273\346\234\272ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = listTrainInfo->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CTrainInfoTable", "\344\270\273\346\234\272\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = listTrainInfo->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("CTrainInfoTable", "\344\273\273\345\212\241\345\210\206\345\217\221ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = listTrainInfo->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("CTrainInfoTable", "\344\273\273\345\212\241ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = listTrainInfo->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("CTrainInfoTable", "\344\273\273\345\212\241\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = listTrainInfo->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("CTrainInfoTable", "\345\267\245\345\206\265ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = listTrainInfo->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("CTrainInfoTable", "\345\267\245\345\206\265\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = listTrainInfo->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QApplication::translate("CTrainInfoTable", "\346\225\205\351\232\234ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem10 = listTrainInfo->horizontalHeaderItem(10);
        ___qtablewidgetitem10->setText(QApplication::translate("CTrainInfoTable", "\346\225\205\351\232\234\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem11 = listTrainInfo->horizontalHeaderItem(11);
        ___qtablewidgetitem11->setText(QApplication::translate("CTrainInfoTable", "\347\231\273\345\275\225\347\212\266\346\200\201", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem12 = listTrainInfo->horizontalHeaderItem(12);
        ___qtablewidgetitem12->setText(QApplication::translate("CTrainInfoTable", "\350\277\220\350\241\214\347\212\266\346\200\201", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem13 = listTrainInfo->horizontalHeaderItem(13);
        ___qtablewidgetitem13->setText(QApplication::translate("CTrainInfoTable", "\344\273\277\347\234\237\351\200\237\345\272\246", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem14 = listTrainInfo->horizontalHeaderItem(14);
        ___qtablewidgetitem14->setText(QApplication::translate("CTrainInfoTable", "\351\273\230\350\256\244\347\224\273\351\235\242", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem15 = listTrainInfo->horizontalHeaderItem(15);
        ___qtablewidgetitem15->setText(QApplication::translate("CTrainInfoTable", "\344\273\273\345\212\241\347\212\266\346\200\201", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem16 = listTrainInfo->horizontalHeaderItem(16);
        ___qtablewidgetitem16->setText(QApplication::translate("CTrainInfoTable", "\345\267\245\345\206\265\347\212\266\346\200\201", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem17 = listTrainInfo->horizontalHeaderItem(17);
        ___qtablewidgetitem17->setText(QApplication::translate("CTrainInfoTable", "\346\225\205\351\232\234\347\212\266\346\200\201", 0, QApplication::UnicodeUTF8));
        buttonTrainInfoAdd->setText(QApplication::translate("CTrainInfoTable", "\346\267\273\345\212\240\345\237\271\350\256\255\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        buttonTrainInfoDel->setText(QApplication::translate("CTrainInfoTable", "\345\210\240\351\231\244\345\237\271\350\256\255\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        buttonTrainInfoModify->setText(QApplication::translate("CTrainInfoTable", "\344\277\256\346\224\271\345\237\271\350\256\255\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        buttonExport->setText(QApplication::translate("CTrainInfoTable", "\345\257\274\345\207\272\344\270\272EXCEL", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QApplication::translate("CTrainInfoTable", "\345\210\267  \346\226\260", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CTrainInfoTable", "\351\200\200  \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CTrainInfoTable);
    } // retranslateUi

};

namespace Ui {
    class CTrainInfoTable: public Ui_CTrainInfoTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAININFO_H
