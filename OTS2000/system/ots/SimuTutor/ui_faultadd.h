/********************************************************************************
** Form generated from reading ui file 'faultadd.ui'
**
** Created: Thu Jan 15 10:26:10 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FAULTADD_H
#define UI_FAULTADD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CFaultAddTable
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *editType;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QComboBox *editStation;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QComboBox *editDev;
    QVBoxLayout *verticalLayout;
    QLabel *label_5;
    QTextEdit *editRemark;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *buttonApply;
    QPushButton *buttonEnter;
    QPushButton *buttonExit;

    void setupUi(QDialog *CFaultAddTable)
    {
        if (CFaultAddTable->objectName().isEmpty())
            CFaultAddTable->setObjectName(QString::fromUtf8("CFaultAddTable"));
        CFaultAddTable->resize(300, 476);
        CFaultAddTable->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        gridLayout = new QGridLayout(CFaultAddTable);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setVerticalSpacing(20);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CFaultAddTable);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(120, 0));
        label->setMaximumSize(QSize(120, 16777215));
        label->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label);

        editName = new QLineEdit(CFaultAddTable);
        editName->setObjectName(QString::fromUtf8("editName"));

        horizontalLayout->addWidget(editName);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(CFaultAddTable);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(120, 0));
        label_2->setMaximumSize(QSize(120, 16777215));
        label_2->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_2);

        editType = new QComboBox(CFaultAddTable);
        editType->setObjectName(QString::fromUtf8("editType"));

        horizontalLayout_2->addWidget(editType);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(CFaultAddTable);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(120, 0));
        label_3->setMaximumSize(QSize(120, 16777215));
        label_3->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_3);

        editStation = new QComboBox(CFaultAddTable);
        editStation->setObjectName(QString::fromUtf8("editStation"));

        horizontalLayout_3->addWidget(editStation);


        gridLayout->addLayout(horizontalLayout_3, 2, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(CFaultAddTable);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(120, 0));
        label_4->setMaximumSize(QSize(120, 16777215));
        label_4->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label_4);

        editDev = new QComboBox(CFaultAddTable);
        editDev->setObjectName(QString::fromUtf8("editDev"));

        horizontalLayout_4->addWidget(editDev);


        gridLayout->addLayout(horizontalLayout_4, 3, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_5 = new QLabel(CFaultAddTable);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(label_5);

        editRemark = new QTextEdit(CFaultAddTable);
        editRemark->setObjectName(QString::fromUtf8("editRemark"));

        verticalLayout->addWidget(editRemark);


        gridLayout->addLayout(verticalLayout, 4, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        buttonApply = new QPushButton(CFaultAddTable);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(buttonApply);

        buttonEnter = new QPushButton(CFaultAddTable);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(buttonEnter);

        buttonExit = new QPushButton(CFaultAddTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(buttonExit);


        gridLayout->addLayout(horizontalLayout_5, 5, 0, 1, 1);

        QWidget::setTabOrder(editName, editRemark);
        QWidget::setTabOrder(editRemark, buttonExit);

        retranslateUi(CFaultAddTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CFaultAddTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CFaultAddTable);
    } // setupUi

    void retranslateUi(QDialog *CFaultAddTable)
    {
        CFaultAddTable->setWindowTitle(QApplication::translate("CFaultAddTable", "\346\267\273\345\212\240\346\225\205\351\232\234", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CFaultAddTable", "\346\225\205\351\232\234\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CFaultAddTable", "\347\261\273    \345\236\213\357\274\232", 0, QApplication::UnicodeUTF8));
        editType->clear();
        editType->insertItems(0, QStringList()
         << QApplication::translate("CFaultAddTable", "\347\263\273\347\273\237\344\272\213\346\225\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CFaultAddTable", "\347\272\277\350\267\257\350\256\276\345\244\207\344\272\213\346\225\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CFaultAddTable", "\344\270\273\345\217\230\350\256\276\345\244\207\344\272\213\346\225\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CFaultAddTable", "\346\234\272\347\273\204\350\256\276\345\244\207\344\272\213\346\225\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CFaultAddTable", "\345\216\202\347\224\250\347\224\265\344\272\213\346\225\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CFaultAddTable", "\347\233\264\346\265\201\344\272\213\346\225\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CFaultAddTable", "\345\205\254\347\224\250\350\256\276\345\244\207\344\272\213\346\225\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CFaultAddTable", "\346\234\272\346\242\260\350\256\276\345\244\207\344\272\213\346\225\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CFaultAddTable", "\345\205\266\344\273\226\350\256\276\345\244\207\344\272\213\346\225\205", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("CFaultAddTable", "\345\216\202 \347\253\231 \345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CFaultAddTable", "\350\256\276 \345\244\207 \345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CFaultAddTable", "\345\244\207    \346\263\250\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonApply->setText(QApplication::translate("CFaultAddTable", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("CFaultAddTable", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CFaultAddTable", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CFaultAddTable);
    } // retranslateUi

};

namespace Ui {
    class CFaultAddTable: public Ui_CFaultAddTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FAULTADD_H
