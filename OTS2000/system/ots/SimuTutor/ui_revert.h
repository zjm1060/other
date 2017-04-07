/********************************************************************************
** Form generated from reading ui file 'revert.ui'
**
** Created: Thu Jan 15 10:26:23 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_REVERT_H
#define UI_REVERT_H

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

class Ui_CRevertTable
{
public:
    QHBoxLayout *horizontalLayout;
    QTableWidget *listRevert;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QPushButton *buttonRevertAdd;
    QPushButton *buttonRevertDel;
    QPushButton *buttonRevertModify;
    QPushButton *buttonStnDev;
    QSpacerItem *verticalSpacer;
    QPushButton *buttonRefresh;
    QPushButton *buttonExit;

    void setupUi(QDialog *CRevertTable)
    {
        if (CRevertTable->objectName().isEmpty())
            CRevertTable->setObjectName(QString::fromUtf8("CRevertTable"));
        CRevertTable->resize(898, 538);
        horizontalLayout = new QHBoxLayout(CRevertTable);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listRevert = new QTableWidget(CRevertTable);
        if (listRevert->columnCount() < 11)
            listRevert->setColumnCount(11);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        listRevert->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        listRevert->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        listRevert->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        listRevert->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        listRevert->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        listRevert->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        listRevert->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        listRevert->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        listRevert->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        listRevert->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        listRevert->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        listRevert->setObjectName(QString::fromUtf8("listRevert"));
        listRevert->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        listRevert->setSortingEnabled(true);
        listRevert->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout->addWidget(listRevert);

        frame = new QFrame(CRevertTable);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(150, 0));
        frame->setMaximumSize(QSize(150, 16777215));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonRevertAdd = new QPushButton(frame);
        buttonRevertAdd->setObjectName(QString::fromUtf8("buttonRevertAdd"));
        buttonRevertAdd->setMinimumSize(QSize(0, 50));
        buttonRevertAdd->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonRevertAdd);

        buttonRevertDel = new QPushButton(frame);
        buttonRevertDel->setObjectName(QString::fromUtf8("buttonRevertDel"));
        buttonRevertDel->setMinimumSize(QSize(0, 50));
        buttonRevertDel->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonRevertDel);

        buttonRevertModify = new QPushButton(frame);
        buttonRevertModify->setObjectName(QString::fromUtf8("buttonRevertModify"));
        buttonRevertModify->setMinimumSize(QSize(0, 50));
        buttonRevertModify->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonRevertModify);

        buttonStnDev = new QPushButton(frame);
        buttonStnDev->setObjectName(QString::fromUtf8("buttonStnDev"));
        buttonStnDev->setMinimumSize(QSize(0, 50));
        buttonStnDev->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonStnDev);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

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

        QWidget::setTabOrder(buttonRevertAdd, buttonRevertDel);
        QWidget::setTabOrder(buttonRevertDel, buttonRevertModify);
        QWidget::setTabOrder(buttonRevertModify, buttonRefresh);
        QWidget::setTabOrder(buttonRefresh, buttonExit);
        QWidget::setTabOrder(buttonExit, listRevert);

        retranslateUi(CRevertTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CRevertTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CRevertTable);
    } // setupUi

    void retranslateUi(QDialog *CRevertTable)
    {
        CRevertTable->setWindowTitle(QApplication::translate("CRevertTable", "\345\267\245\345\206\265\345\206\215\347\216\260\346\241\210\344\276\213\345\272\223", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = listRevert->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CRevertTable", "\345\206\215\347\216\260ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = listRevert->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CRevertTable", "\345\206\215\347\216\260\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = listRevert->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CRevertTable", "\347\224\250\346\210\267\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = listRevert->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CRevertTable", "\345\274\200\345\247\213\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = listRevert->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("CRevertTable", "\347\273\223\346\235\237\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = listRevert->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("CRevertTable", "\351\273\230\350\256\244\347\224\273\351\235\242", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = listRevert->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("CRevertTable", "\350\257\264\346\230\216", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = listRevert->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("CRevertTable", "\345\216\202\347\253\231\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = listRevert->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("CRevertTable", "\350\256\276\345\244\207\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = listRevert->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QApplication::translate("CRevertTable", "\346\235\245\346\272\220", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem10 = listRevert->horizontalHeaderItem(10);
        ___qtablewidgetitem10->setText(QApplication::translate("CRevertTable", "\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        buttonRevertAdd->setText(QApplication::translate("CRevertTable", "\346\267\273\345\212\240\345\267\245\345\206\265\345\206\215\347\216\260", 0, QApplication::UnicodeUTF8));
        buttonRevertDel->setText(QApplication::translate("CRevertTable", "\345\210\240\351\231\244\345\267\245\345\206\265\345\206\215\347\216\260", 0, QApplication::UnicodeUTF8));
        buttonRevertModify->setText(QApplication::translate("CRevertTable", "\344\277\256\346\224\271\345\267\245\345\206\265\345\206\215\347\216\260", 0, QApplication::UnicodeUTF8));
        buttonStnDev->setText(QApplication::translate("CRevertTable", "\351\200\211\346\213\251\345\216\202\347\253\231\350\256\276\345\244\207", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QApplication::translate("CRevertTable", "\345\210\267   \346\226\260", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CRevertTable", "\351\200\200   \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CRevertTable);
    } // retranslateUi

};

namespace Ui {
    class CRevertTable: public Ui_CRevertTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REVERT_H
