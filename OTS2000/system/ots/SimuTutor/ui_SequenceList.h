/********************************************************************************
** Form generated from reading ui file 'SequenceList.ui'
**
** Created: Thu Jan 15 10:25:59 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SEQUENCELIST_H
#define UI_SEQUENCELIST_H

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

class Ui_SequenceList
{
public:
    QHBoxLayout *horizontalLayout;
    QTableWidget *tableWidget;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButtonAdd;
    QPushButton *pushButtonModi;
    QPushButton *pushButtonDel;
    QPushButton *buttonSequenceInto;
    QPushButton *buttonSequenceConfig;
    QPushButton *buttonStnDev;
    QPushButton *pushButtonRefresh;
    QPushButton *pushButtonQuit;

    void setupUi(QDialog *SequenceList)
    {
        if (SequenceList->objectName().isEmpty())
            SequenceList->setObjectName(QString::fromUtf8("SequenceList"));
        SequenceList->resize(804, 552);
        horizontalLayout = new QHBoxLayout(SequenceList);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tableWidget = new QTableWidget(SequenceList);
        if (tableWidget->columnCount() < 6)
            tableWidget->setColumnCount(6);
        QFont font;
        font.setPointSize(11);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font);
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font);
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font);
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font);
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setFont(font);
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setFont(font);
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        tableWidget->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout->addWidget(tableWidget);

        frame = new QFrame(SequenceList);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(150, 0));
        frame->setMaximumSize(QSize(150, 16777215));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButtonAdd = new QPushButton(frame);
        pushButtonAdd->setObjectName(QString::fromUtf8("pushButtonAdd"));
        pushButtonAdd->setMinimumSize(QSize(0, 40));
        pushButtonAdd->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(pushButtonAdd);

        pushButtonModi = new QPushButton(frame);
        pushButtonModi->setObjectName(QString::fromUtf8("pushButtonModi"));
        pushButtonModi->setMinimumSize(QSize(0, 40));
        pushButtonModi->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(pushButtonModi);

        pushButtonDel = new QPushButton(frame);
        pushButtonDel->setObjectName(QString::fromUtf8("pushButtonDel"));
        pushButtonDel->setMinimumSize(QSize(0, 40));
        pushButtonDel->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(pushButtonDel);

        buttonSequenceInto = new QPushButton(frame);
        buttonSequenceInto->setObjectName(QString::fromUtf8("buttonSequenceInto"));
        buttonSequenceInto->setMinimumSize(QSize(0, 40));
        buttonSequenceInto->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonSequenceInto);

        buttonSequenceConfig = new QPushButton(frame);
        buttonSequenceConfig->setObjectName(QString::fromUtf8("buttonSequenceConfig"));
        buttonSequenceConfig->setMinimumSize(QSize(0, 40));
        buttonSequenceConfig->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonSequenceConfig);

        buttonStnDev = new QPushButton(frame);
        buttonStnDev->setObjectName(QString::fromUtf8("buttonStnDev"));
        buttonStnDev->setMinimumSize(QSize(0, 40));
        buttonStnDev->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonStnDev);

        pushButtonRefresh = new QPushButton(frame);
        pushButtonRefresh->setObjectName(QString::fromUtf8("pushButtonRefresh"));
        pushButtonRefresh->setMinimumSize(QSize(0, 40));
        pushButtonRefresh->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(pushButtonRefresh);

        pushButtonQuit = new QPushButton(frame);
        pushButtonQuit->setObjectName(QString::fromUtf8("pushButtonQuit"));
        pushButtonQuit->setMinimumSize(QSize(0, 40));
        pushButtonQuit->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(pushButtonQuit);


        horizontalLayout->addWidget(frame);

        QWidget::setTabOrder(pushButtonAdd, pushButtonModi);
        QWidget::setTabOrder(pushButtonModi, pushButtonDel);
        QWidget::setTabOrder(pushButtonDel, pushButtonRefresh);
        QWidget::setTabOrder(pushButtonRefresh, pushButtonQuit);
        QWidget::setTabOrder(pushButtonQuit, tableWidget);

        retranslateUi(SequenceList);

        QMetaObject::connectSlotsByName(SequenceList);
    } // setupUi

    void retranslateUi(QDialog *SequenceList)
    {
        SequenceList->setWindowTitle(QApplication::translate("SequenceList", "\346\265\201\347\250\213\345\272\223", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("SequenceList", "\346\265\201\347\250\213ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("SequenceList", "\346\265\201\347\250\213\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("SequenceList", "\344\270\255\346\226\207\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("SequenceList", "\345\210\233\345\273\272\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("SequenceList", "\345\216\202\347\253\231\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("SequenceList", "\350\256\276\345\244\207\345\220\215", 0, QApplication::UnicodeUTF8));
        pushButtonAdd->setText(QApplication::translate("SequenceList", "\346\267\273\345\212\240\346\265\201\347\250\213", 0, QApplication::UnicodeUTF8));
        pushButtonModi->setText(QApplication::translate("SequenceList", "\344\277\256\346\224\271\346\265\201\347\250\213", 0, QApplication::UnicodeUTF8));
        pushButtonDel->setText(QApplication::translate("SequenceList", "\345\210\240\351\231\244\346\265\201\347\250\213", 0, QApplication::UnicodeUTF8));
        buttonSequenceInto->setText(QApplication::translate("SequenceList", "\345\257\274\345\205\245\346\265\201\347\250\213", 0, QApplication::UnicodeUTF8));
        buttonSequenceConfig->setText(QApplication::translate("SequenceList", "\346\211\223\345\274\200\346\265\201\347\250\213\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        buttonStnDev->setText(QApplication::translate("SequenceList", "\351\200\211\346\213\251\345\216\202\347\253\231\350\256\276\345\244\207", 0, QApplication::UnicodeUTF8));
        pushButtonRefresh->setText(QApplication::translate("SequenceList", "\345\210\267   \346\226\260", 0, QApplication::UnicodeUTF8));
        pushButtonQuit->setText(QApplication::translate("SequenceList", "\351\200\200   \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SequenceList);
    } // retranslateUi

};

namespace Ui {
    class SequenceList: public Ui_SequenceList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEQUENCELIST_H
