/********************************************************************************
** Form generated from reading ui file 'model.ui'
**
** Created: Thu Jan 15 10:26:16 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MODEL_H
#define UI_MODEL_H

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

class Ui_CModelForm
{
public:
    QHBoxLayout *horizontalLayout;
    QTableWidget *listModel;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QPushButton *buttonParam;
    QPushButton *buttonAdd;
    QPushButton *buttonDel;
    QPushButton *buttonModify;
    QPushButton *buttonModelInto;
    QPushButton *buttonModelConfig;
    QPushButton *buttonStnDev;
    QPushButton *buttonRefresh;
    QPushButton *buttonExit;

    void setupUi(QDialog *CModelForm)
    {
        if (CModelForm->objectName().isEmpty())
            CModelForm->setObjectName(QString::fromUtf8("CModelForm"));
        CModelForm->resize(800, 550);
        CModelForm->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        horizontalLayout = new QHBoxLayout(CModelForm);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listModel = new QTableWidget(CModelForm);
        if (listModel->columnCount() < 6)
            listModel->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        listModel->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        listModel->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        listModel->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        listModel->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        listModel->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        listModel->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        listModel->setObjectName(QString::fromUtf8("listModel"));
        listModel->horizontalHeader()->setDefaultSectionSize(150);
        listModel->horizontalHeader()->setMinimumSectionSize(150);
        listModel->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout->addWidget(listModel);

        frame = new QFrame(CModelForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(150, 0));
        frame->setMaximumSize(QSize(150, 16777215));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonParam = new QPushButton(frame);
        buttonParam->setObjectName(QString::fromUtf8("buttonParam"));
        buttonParam->setMinimumSize(QSize(0, 40));
        buttonParam->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonParam);

        buttonAdd = new QPushButton(frame);
        buttonAdd->setObjectName(QString::fromUtf8("buttonAdd"));
        buttonAdd->setMinimumSize(QSize(0, 40));
        buttonAdd->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonAdd);

        buttonDel = new QPushButton(frame);
        buttonDel->setObjectName(QString::fromUtf8("buttonDel"));
        buttonDel->setMinimumSize(QSize(0, 40));
        buttonDel->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonDel);

        buttonModify = new QPushButton(frame);
        buttonModify->setObjectName(QString::fromUtf8("buttonModify"));
        buttonModify->setMinimumSize(QSize(0, 40));
        buttonModify->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonModify);

        buttonModelInto = new QPushButton(frame);
        buttonModelInto->setObjectName(QString::fromUtf8("buttonModelInto"));
        buttonModelInto->setMinimumSize(QSize(0, 40));
        buttonModelInto->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonModelInto);

        buttonModelConfig = new QPushButton(frame);
        buttonModelConfig->setObjectName(QString::fromUtf8("buttonModelConfig"));
        buttonModelConfig->setMinimumSize(QSize(0, 40));
        buttonModelConfig->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonModelConfig);

        buttonStnDev = new QPushButton(frame);
        buttonStnDev->setObjectName(QString::fromUtf8("buttonStnDev"));
        buttonStnDev->setMinimumSize(QSize(0, 40));
        buttonStnDev->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonStnDev);

        buttonRefresh = new QPushButton(frame);
        buttonRefresh->setObjectName(QString::fromUtf8("buttonRefresh"));
        buttonRefresh->setMinimumSize(QSize(0, 40));
        buttonRefresh->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonRefresh);

        buttonExit = new QPushButton(frame);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setMinimumSize(QSize(0, 40));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonExit);


        horizontalLayout->addWidget(frame);

        QWidget::setTabOrder(buttonParam, buttonAdd);
        QWidget::setTabOrder(buttonAdd, buttonDel);
        QWidget::setTabOrder(buttonDel, buttonModify);
        QWidget::setTabOrder(buttonModify, buttonRefresh);
        QWidget::setTabOrder(buttonRefresh, buttonExit);

        retranslateUi(CModelForm);
        QObject::connect(buttonExit, SIGNAL(clicked()), CModelForm, SLOT(close()));

        QMetaObject::connectSlotsByName(CModelForm);
    } // setupUi

    void retranslateUi(QDialog *CModelForm)
    {
        CModelForm->setWindowTitle(QApplication::translate("CModelForm", "\346\250\241\345\236\213\345\272\223", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = listModel->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CModelForm", "\346\250\241\345\236\213ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = listModel->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CModelForm", "\346\250\241\345\236\213\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = listModel->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CModelForm", "\344\270\255\346\226\207\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = listModel->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CModelForm", "\345\273\272\347\253\213\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = listModel->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("CModelForm", "\345\216\202\347\253\231\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = listModel->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("CModelForm", "\350\256\276\345\244\207\345\220\215", 0, QApplication::UnicodeUTF8));
        buttonParam->setText(QApplication::translate("CModelForm", "\346\250\241\345\236\213\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        buttonAdd->setText(QApplication::translate("CModelForm", "\346\267\273\345\212\240\346\250\241\345\236\213", 0, QApplication::UnicodeUTF8));
        buttonDel->setText(QApplication::translate("CModelForm", "\345\210\240\351\231\244\346\250\241\345\236\213", 0, QApplication::UnicodeUTF8));
        buttonModify->setText(QApplication::translate("CModelForm", "\344\277\256\346\224\271\346\250\241\345\236\213", 0, QApplication::UnicodeUTF8));
        buttonModelInto->setText(QApplication::translate("CModelForm", "\345\257\274\345\205\245\346\250\241\345\236\213", 0, QApplication::UnicodeUTF8));
        buttonModelConfig->setText(QApplication::translate("CModelForm", "\346\211\223\345\274\200\346\250\241\345\236\213\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        buttonStnDev->setText(QApplication::translate("CModelForm", "\351\200\211\346\213\251\345\216\202\347\253\231\350\256\276\345\244\207", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QApplication::translate("CModelForm", "\345\210\267   \346\226\260", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CModelForm", "\351\200\200   \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CModelForm);
    } // retranslateUi

};

namespace Ui {
    class CModelForm: public Ui_CModelForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODEL_H
