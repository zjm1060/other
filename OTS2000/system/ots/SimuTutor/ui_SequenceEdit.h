/********************************************************************************
** Form generated from reading ui file 'SequenceEdit.ui'
**
** Created: Thu Jan 15 10:25:58 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SEQUENCEEDIT_H
#define UI_SEQUENCEEDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SequenceEdit
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lename;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lechname;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *lestation;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *leunit;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButtonOK;
    QPushButton *pushButtonCancel;

    void setupUi(QDialog *SequenceEdit)
    {
        if (SequenceEdit->objectName().isEmpty())
            SequenceEdit->setObjectName(QString::fromUtf8("SequenceEdit"));
        SequenceEdit->resize(412, 223);
        verticalLayout = new QVBoxLayout(SequenceEdit);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(SequenceEdit);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(120, 0));
        label->setMaximumSize(QSize(120, 16777215));
        label->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label);

        lename = new QLineEdit(SequenceEdit);
        lename->setObjectName(QString::fromUtf8("lename"));

        horizontalLayout->addWidget(lename);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(SequenceEdit);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(120, 0));
        label_2->setMaximumSize(QSize(120, 16777215));
        label_2->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_2);

        lechname = new QLineEdit(SequenceEdit);
        lechname->setObjectName(QString::fromUtf8("lechname"));

        horizontalLayout_2->addWidget(lechname);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(SequenceEdit);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(120, 0));
        label_3->setMaximumSize(QSize(120, 16777215));
        label_3->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_3);

        lestation = new QLineEdit(SequenceEdit);
        lestation->setObjectName(QString::fromUtf8("lestation"));

        horizontalLayout_3->addWidget(lestation);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(SequenceEdit);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(120, 0));
        label_4->setMaximumSize(QSize(120, 16777215));
        label_4->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label_4);

        leunit = new QLineEdit(SequenceEdit);
        leunit->setObjectName(QString::fromUtf8("leunit"));

        horizontalLayout_4->addWidget(leunit);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(50);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(40, -1, 40, -1);
        pushButtonOK = new QPushButton(SequenceEdit);
        pushButtonOK->setObjectName(QString::fromUtf8("pushButtonOK"));
        pushButtonOK->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(pushButtonOK);

        pushButtonCancel = new QPushButton(SequenceEdit);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));
        pushButtonCancel->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(pushButtonCancel);


        verticalLayout->addLayout(horizontalLayout_5);

        QWidget::setTabOrder(lename, lechname);
        QWidget::setTabOrder(lechname, lestation);
        QWidget::setTabOrder(lestation, leunit);
        QWidget::setTabOrder(leunit, pushButtonOK);
        QWidget::setTabOrder(pushButtonOK, pushButtonCancel);

        retranslateUi(SequenceEdit);

        QMetaObject::connectSlotsByName(SequenceEdit);
    } // setupUi

    void retranslateUi(QDialog *SequenceEdit)
    {
        SequenceEdit->setWindowTitle(QApplication::translate("SequenceEdit", "\347\274\226\350\276\221\346\265\201\347\250\213", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SequenceEdit", "\346\265\201\347\250\213\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SequenceEdit", "\344\270\255\346\226\207\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SequenceEdit", "\345\216\202    \347\253\231\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("SequenceEdit", "\350\256\276    \345\244\207\357\274\232", 0, QApplication::UnicodeUTF8));
        pushButtonOK->setText(QApplication::translate("SequenceEdit", "\347\241\256  \345\256\232", 0, QApplication::UnicodeUTF8));
        pushButtonCancel->setText(QApplication::translate("SequenceEdit", "\345\217\226  \346\266\210", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SequenceEdit);
    } // retranslateUi

};

namespace Ui {
    class SequenceEdit: public Ui_SequenceEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEQUENCEEDIT_H
