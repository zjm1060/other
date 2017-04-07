/********************************************************************************
** Form generated from reading ui file 'modelparam.ui'
**
** Created: Thu Jan 15 10:26:15 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MODELPARAM_H
#define UI_MODELPARAM_H

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

class Ui_CModelParamForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout;
    QTableWidget *listModelParam;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonAdd;
    QPushButton *buttonModify;
    QPushButton *buttonDel;
    QPushButton *buttonExit;

    void setupUi(QDialog *CModelParamForm)
    {
        if (CModelParamForm->objectName().isEmpty())
            CModelParamForm->setObjectName(QString::fromUtf8("CModelParamForm"));
        CModelParamForm->resize(484, 407);
        verticalLayout_2 = new QVBoxLayout(CModelParamForm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame_2 = new QFrame(CModelParamForm);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::Box);
        frame_2->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        listModelParam = new QTableWidget(frame_2);
        if (listModelParam->columnCount() < 4)
            listModelParam->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        listModelParam->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        listModelParam->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        listModelParam->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        listModelParam->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        listModelParam->setObjectName(QString::fromUtf8("listModelParam"));
        listModelParam->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        listModelParam->horizontalHeader()->setStretchLastSection(true);

        verticalLayout->addWidget(listModelParam);


        verticalLayout_2->addWidget(frame_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(20);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buttonAdd = new QPushButton(CModelParamForm);
        buttonAdd->setObjectName(QString::fromUtf8("buttonAdd"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(12);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        buttonAdd->setFont(font);
        buttonAdd->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonAdd);

        buttonModify = new QPushButton(CModelParamForm);
        buttonModify->setObjectName(QString::fromUtf8("buttonModify"));
        buttonModify->setFont(font);
        buttonModify->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonModify);

        buttonDel = new QPushButton(CModelParamForm);
        buttonDel->setObjectName(QString::fromUtf8("buttonDel"));
        buttonDel->setFont(font);
        buttonDel->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonDel);

        buttonExit = new QPushButton(CModelParamForm);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setFont(font);
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonExit);


        verticalLayout_2->addLayout(horizontalLayout);

        QWidget::setTabOrder(buttonAdd, buttonModify);
        QWidget::setTabOrder(buttonModify, buttonDel);
        QWidget::setTabOrder(buttonDel, buttonExit);
        QWidget::setTabOrder(buttonExit, listModelParam);

        retranslateUi(CModelParamForm);
        QObject::connect(buttonExit, SIGNAL(clicked()), CModelParamForm, SLOT(close()));

        QMetaObject::connectSlotsByName(CModelParamForm);
    } // setupUi

    void retranslateUi(QDialog *CModelParamForm)
    {
        CModelParamForm->setWindowTitle(QApplication::translate("CModelParamForm", "\346\250\241\345\236\213\345\217\202\346\225\260\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = listModelParam->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CModelParamForm", "\345\217\202\346\225\260\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = listModelParam->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CModelParamForm", "\345\217\202\346\225\260\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = listModelParam->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CModelParamForm", "\345\217\202\346\225\260\345\200\274", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = listModelParam->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CModelParamForm", "\346\225\260\346\215\256\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        buttonAdd->setText(QApplication::translate("CModelParamForm", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        buttonModify->setText(QApplication::translate("CModelParamForm", "\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        buttonDel->setText(QApplication::translate("CModelParamForm", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CModelParamForm", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CModelParamForm);
    } // retranslateUi

};

namespace Ui {
    class CModelParamForm: public Ui_CModelParamForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELPARAM_H
