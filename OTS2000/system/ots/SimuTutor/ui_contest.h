/********************************************************************************
** Form generated from reading ui file 'contest.ui'
**
** Created: Thu Jan 15 10:25:45 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CONTEST_H
#define UI_CONTEST_H

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

class Ui_CContestTable
{
public:
    QHBoxLayout *horizontalLayout;
    QTableWidget *listContest;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QPushButton *buttonContestAdd;
    QPushButton *buttonContestModify;
    QPushButton *buttonContestDel;
    QPushButton *buttonRefresh;
    QPushButton *buttonExit;

    void setupUi(QDialog *CContestTable)
    {
        if (CContestTable->objectName().isEmpty())
            CContestTable->setObjectName(QString::fromUtf8("CContestTable"));
        CContestTable->resize(1197, 742);
        horizontalLayout = new QHBoxLayout(CContestTable);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listContest = new QTableWidget(CContestTable);
        if (listContest->columnCount() < 8)
            listContest->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        listContest->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        listContest->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        listContest->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        listContest->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        listContest->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        listContest->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        listContest->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        listContest->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        listContest->setObjectName(QString::fromUtf8("listContest"));
        listContest->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        listContest->setSortingEnabled(true);
        listContest->horizontalHeader()->setDefaultSectionSize(150);
        listContest->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        listContest->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout->addWidget(listContest);

        frame = new QFrame(CContestTable);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(150, 0));
        frame->setMaximumSize(QSize(150, 16777215));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setSpacing(20);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        buttonContestAdd = new QPushButton(frame);
        buttonContestAdd->setObjectName(QString::fromUtf8("buttonContestAdd"));
        buttonContestAdd->setMinimumSize(QSize(0, 40));
        buttonContestAdd->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout_2->addWidget(buttonContestAdd);

        buttonContestModify = new QPushButton(frame);
        buttonContestModify->setObjectName(QString::fromUtf8("buttonContestModify"));
        buttonContestModify->setMinimumSize(QSize(0, 40));
        buttonContestModify->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout_2->addWidget(buttonContestModify);

        buttonContestDel = new QPushButton(frame);
        buttonContestDel->setObjectName(QString::fromUtf8("buttonContestDel"));
        buttonContestDel->setMinimumSize(QSize(0, 40));
        buttonContestDel->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout_2->addWidget(buttonContestDel);

        buttonRefresh = new QPushButton(frame);
        buttonRefresh->setObjectName(QString::fromUtf8("buttonRefresh"));
        buttonRefresh->setMinimumSize(QSize(0, 40));
        buttonRefresh->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout_2->addWidget(buttonRefresh);

        buttonExit = new QPushButton(frame);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setMinimumSize(QSize(0, 40));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout_2->addWidget(buttonExit);


        horizontalLayout->addWidget(frame);

        QWidget::setTabOrder(buttonContestAdd, buttonContestDel);
        QWidget::setTabOrder(buttonContestDel, buttonContestModify);
        QWidget::setTabOrder(buttonContestModify, buttonRefresh);
        QWidget::setTabOrder(buttonRefresh, buttonExit);
        QWidget::setTabOrder(buttonExit, listContest);

        retranslateUi(CContestTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CContestTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CContestTable);
    } // setupUi

    void retranslateUi(QDialog *CContestTable)
    {
        CContestTable->setWindowTitle(QApplication::translate("CContestTable", "\347\253\236\350\265\233\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = listContest->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CContestTable", "\347\253\236\350\265\233ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = listContest->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CContestTable", "\347\253\236  \350\265\233  \345\220\215  \347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = listContest->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CContestTable", "\347\253\236\350\265\233\347\212\266\346\200\201", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = listContest->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CContestTable", "  \344\273\273  \345\212\241  \345\220\215  \347\247\260  ", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = listContest->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("CContestTable", " \345\267\245  \345\206\265  \345\220\215  \347\247\260 ", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = listContest->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("CContestTable", " \346\225\205  \351\232\234  \345\220\215  \347\247\260 ", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = listContest->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("CContestTable", "\345\210\233  \345\273\272  \346\227\266  \351\227\264", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = listContest->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("CContestTable", "\345\244\207   \346\263\250", 0, QApplication::UnicodeUTF8));
        buttonContestAdd->setText(QApplication::translate("CContestTable", "\346\267\273\345\212\240\347\253\236\350\265\233", 0, QApplication::UnicodeUTF8));
        buttonContestModify->setText(QApplication::translate("CContestTable", "\344\277\256\346\224\271\347\253\236\350\265\233", 0, QApplication::UnicodeUTF8));
        buttonContestDel->setText(QApplication::translate("CContestTable", "\345\210\240\351\231\244\347\253\236\350\265\233", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QApplication::translate("CContestTable", "\345\210\267  \346\226\260", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CContestTable", "\351\200\200  \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CContestTable);
    } // retranslateUi

};

namespace Ui {
    class CContestTable: public Ui_CContestTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTEST_H
