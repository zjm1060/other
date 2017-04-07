/********************************************************************************
** Form generated from reading ui file 'task.ui'
**
** Created: Thu Jan 15 10:24:44 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TASK_H
#define UI_TASK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CTaskTable
{
public:
    QHBoxLayout *horizontalLayout;
    QTableWidget *listTask;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QLabel *label_2;
    QComboBox *comboBox_2;
    QPushButton *buttonSelectByCond;
    QPushButton *buttonStnDev;
    QPushButton *buttonTaskAdd;
    QPushButton *buttonTaskModify;
    QPushButton *buttonTaskDel;
    QPushButton *buttonRefresh;
    QPushButton *buttonExit;

    void setupUi(QDialog *CTaskTable)
    {
        if (CTaskTable->objectName().isEmpty())
            CTaskTable->setObjectName(QString::fromUtf8("CTaskTable"));
        CTaskTable->resize(1197, 742);
        horizontalLayout = new QHBoxLayout(CTaskTable);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listTask = new QTableWidget(CTaskTable);
        if (listTask->columnCount() < 13)
            listTask->setColumnCount(13);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(11, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(12, __qtablewidgetitem12);
        listTask->setObjectName(QString::fromUtf8("listTask"));
        listTask->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        listTask->setSortingEnabled(true);
        listTask->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout->addWidget(listTask);

        frame = new QFrame(CTaskTable);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(150, 0));
        frame->setMaximumSize(QSize(150, 16777215));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setSpacing(20);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMaximumSize(QSize(16777215, 16777215));
        groupBox->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 20));
        label->setMaximumSize(QSize(16777215, 20));
        label->setStyleSheet(QString::fromUtf8(""));

        verticalLayout->addWidget(label);

        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        verticalLayout->addWidget(comboBox);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(0, 20));
        label_2->setMaximumSize(QSize(16777215, 20));
        label_2->setStyleSheet(QString::fromUtf8(""));

        verticalLayout->addWidget(label_2);

        comboBox_2 = new QComboBox(groupBox);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

        verticalLayout->addWidget(comboBox_2);

        buttonSelectByCond = new QPushButton(groupBox);
        buttonSelectByCond->setObjectName(QString::fromUtf8("buttonSelectByCond"));
        buttonSelectByCond->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonSelectByCond);

        buttonStnDev = new QPushButton(groupBox);
        buttonStnDev->setObjectName(QString::fromUtf8("buttonStnDev"));
        buttonStnDev->setMaximumSize(QSize(16777215, 16777215));
        buttonStnDev->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonStnDev);


        verticalLayout_2->addWidget(groupBox);

        buttonTaskAdd = new QPushButton(frame);
        buttonTaskAdd->setObjectName(QString::fromUtf8("buttonTaskAdd"));
        buttonTaskAdd->setMinimumSize(QSize(0, 40));
        buttonTaskAdd->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout_2->addWidget(buttonTaskAdd);

        buttonTaskModify = new QPushButton(frame);
        buttonTaskModify->setObjectName(QString::fromUtf8("buttonTaskModify"));
        buttonTaskModify->setMinimumSize(QSize(0, 40));
        buttonTaskModify->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout_2->addWidget(buttonTaskModify);

        buttonTaskDel = new QPushButton(frame);
        buttonTaskDel->setObjectName(QString::fromUtf8("buttonTaskDel"));
        buttonTaskDel->setMinimumSize(QSize(0, 40));
        buttonTaskDel->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout_2->addWidget(buttonTaskDel);

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

        QWidget::setTabOrder(buttonTaskAdd, buttonTaskDel);
        QWidget::setTabOrder(buttonTaskDel, buttonTaskModify);
        QWidget::setTabOrder(buttonTaskModify, buttonRefresh);
        QWidget::setTabOrder(buttonRefresh, buttonExit);
        QWidget::setTabOrder(buttonExit, listTask);

        retranslateUi(CTaskTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CTaskTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CTaskTable);
    } // setupUi

    void retranslateUi(QDialog *CTaskTable)
    {
        CTaskTable->setWindowTitle(QApplication::translate("CTaskTable", "\346\241\210\344\276\213\345\272\223", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = listTask->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CTaskTable", "\344\273\273\345\212\241ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = listTask->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CTaskTable", "\344\273\273\345\212\241\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = listTask->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CTaskTable", "\345\210\206\347\261\273\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = listTask->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CTaskTable", "\345\267\245\345\206\265\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = listTask->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("CTaskTable", "\346\225\205\351\232\234\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = listTask->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("CTaskTable", "\345\261\236\346\200\247", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = listTask->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("CTaskTable", "\347\224\250\346\210\267\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = listTask->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("CTaskTable", "\345\210\233\345\273\272\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = listTask->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("CTaskTable", "\346\223\215\344\275\234\346\217\220\347\244\272", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = listTask->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QApplication::translate("CTaskTable", "\345\216\202\347\253\231\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem10 = listTask->horizontalHeaderItem(10);
        ___qtablewidgetitem10->setText(QApplication::translate("CTaskTable", "\350\256\276\345\244\207\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem11 = listTask->horizontalHeaderItem(11);
        ___qtablewidgetitem11->setText(QApplication::translate("CTaskTable", "\351\200\202\347\224\250\345\267\245\347\247\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem12 = listTask->horizontalHeaderItem(12);
        ___qtablewidgetitem12->setText(QApplication::translate("CTaskTable", "\344\275\277\347\224\250\350\214\203\345\233\264", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CTaskTable", "\344\273\273\345\212\241\347\255\233\351\200\211", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CTaskTable", "\347\255\233\351\200\211\347\261\273\345\210\253\357\274\232", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("CTaskTable", "\350\257\267\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTaskTable", "\344\273\273\345\212\241\345\210\206\347\261\273", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTaskTable", "\345\261\236\346\200\247", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTaskTable", "\345\267\245\345\206\265\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTaskTable", "\346\225\205\351\232\234\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("CTaskTable", "\347\255\233\351\200\211\346\235\241\344\273\266\357\274\232", 0, QApplication::UnicodeUTF8));
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("CTaskTable", "\350\257\267\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8)
        );
        buttonSelectByCond->setText(QApplication::translate("CTaskTable", "\346\214\211\347\255\233\351\200\211\346\235\241\344\273\266\347\255\233\351\200\211", 0, QApplication::UnicodeUTF8));
        buttonStnDev->setText(QApplication::translate("CTaskTable", "\346\214\211\345\216\202\347\253\231\350\256\276\345\244\207\347\255\233\351\200\211", 0, QApplication::UnicodeUTF8));
        buttonTaskAdd->setText(QApplication::translate("CTaskTable", "\346\267\273\345\212\240\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        buttonTaskModify->setText(QApplication::translate("CTaskTable", "\344\277\256\346\224\271\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        buttonTaskDel->setText(QApplication::translate("CTaskTable", "\345\210\240\351\231\244\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QApplication::translate("CTaskTable", "\345\210\267  \346\226\260", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CTaskTable", "\351\200\200  \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CTaskTable);
    } // retranslateUi

};

namespace Ui {
    class CTaskTable: public Ui_CTaskTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TASK_H
