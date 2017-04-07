/********************************************************************************
** Form generated from reading ui file 'modeladd.ui'
**
** Created: Thu Jan 15 10:26:05 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MODELADD_H
#define UI_MODELADD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CModelAddForm
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_12;
    QLineEdit *editLongName;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_9;
    QComboBox *editStation;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_10;
    QComboBox *editDev;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *buttonApply;
    QPushButton *buttonEnter;
    QPushButton *buttonExit;

    void setupUi(QDialog *CModelAddForm)
    {
        if (CModelAddForm->objectName().isEmpty())
            CModelAddForm->setObjectName(QString::fromUtf8("CModelAddForm"));
        CModelAddForm->resize(351, 251);
        verticalLayout = new QVBoxLayout(CModelAddForm);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, -1, -1, 3);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CModelAddForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(120, 0));
        label->setMaximumSize(QSize(120, 16777215));
        label->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label);

        editName = new QLineEdit(CModelAddForm);
        editName->setObjectName(QString::fromUtf8("editName"));

        horizontalLayout->addWidget(editName);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_12 = new QLabel(CModelAddForm);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setMinimumSize(QSize(120, 0));
        label_12->setMaximumSize(QSize(120, 16777215));
        label_12->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_12);

        editLongName = new QLineEdit(CModelAddForm);
        editLongName->setObjectName(QString::fromUtf8("editLongName"));

        horizontalLayout_2->addWidget(editLongName);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_9 = new QLabel(CModelAddForm);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(120, 0));
        label_9->setMaximumSize(QSize(120, 16777215));
        label_9->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_9);

        editStation = new QComboBox(CModelAddForm);
        editStation->setObjectName(QString::fromUtf8("editStation"));

        horizontalLayout_3->addWidget(editStation);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_10 = new QLabel(CModelAddForm);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(120, 0));
        label_10->setMaximumSize(QSize(120, 16777215));
        label_10->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label_10);

        editDev = new QComboBox(CModelAddForm);
        editDev->setObjectName(QString::fromUtf8("editDev"));

        horizontalLayout_4->addWidget(editDev);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(30);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        buttonApply = new QPushButton(CModelAddForm);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(buttonApply);

        buttonEnter = new QPushButton(CModelAddForm);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(buttonEnter);

        buttonExit = new QPushButton(CModelAddForm);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(buttonExit);


        verticalLayout->addLayout(horizontalLayout_5);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 2);
        verticalLayout->setStretch(2, 2);
        verticalLayout->setStretch(3, 2);
        verticalLayout->setStretch(4, 3);
        QWidget::setTabOrder(editName, editLongName);
        QWidget::setTabOrder(editLongName, buttonEnter);
        QWidget::setTabOrder(buttonEnter, buttonExit);

        retranslateUi(CModelAddForm);
        QObject::connect(buttonExit, SIGNAL(clicked()), CModelAddForm, SLOT(reject()));

        QMetaObject::connectSlotsByName(CModelAddForm);
    } // setupUi

    void retranslateUi(QDialog *CModelAddForm)
    {
        CModelAddForm->setWindowTitle(QApplication::translate("CModelAddForm", "\346\267\273\345\212\240\346\250\241\345\236\213", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CModelAddForm", "\346\250\241\345\236\213\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("CModelAddForm", "\344\270\255\346\226\207\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("CModelAddForm", "\345\216\202    \347\253\231\357\274\232", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("CModelAddForm", "\350\256\276    \345\244\207\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonApply->setText(QApplication::translate("CModelAddForm", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("CModelAddForm", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CModelAddForm", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CModelAddForm);
    } // retranslateUi

};

namespace Ui {
    class CModelAddForm: public Ui_CModelAddForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELADD_H
