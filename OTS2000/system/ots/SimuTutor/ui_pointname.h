/********************************************************************************
** Form generated from reading ui file 'pointname.ui'
**
** Created: Thu Jan 15 10:24:49 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_POINTNAME_H
#define UI_POINTNAME_H

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
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CPointNameTable
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editPointName;
    QPushButton *buttonCancel;
    QPushButton *buttonEnter;
    QTreeWidget *treePointName;

    void setupUi(QDialog *CPointNameTable)
    {
        if (CPointNameTable->objectName().isEmpty())
            CPointNameTable->setObjectName(QString::fromUtf8("CPointNameTable"));
        CPointNameTable->resize(550, 400);
        verticalLayout = new QVBoxLayout(CPointNameTable);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CPointNameTable);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        editPointName = new QLineEdit(CPointNameTable);
        editPointName->setObjectName(QString::fromUtf8("editPointName"));

        horizontalLayout->addWidget(editPointName);

        buttonCancel = new QPushButton(CPointNameTable);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));

        horizontalLayout->addWidget(buttonCancel);

        buttonEnter = new QPushButton(CPointNameTable);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));

        horizontalLayout->addWidget(buttonEnter);


        verticalLayout->addLayout(horizontalLayout);

        treePointName = new QTreeWidget(CPointNameTable);
        treePointName->setObjectName(QString::fromUtf8("treePointName"));
        treePointName->setStyleSheet(QString::fromUtf8("font: 75 16pt \"\344\273\277\345\256\213\";"));

        verticalLayout->addWidget(treePointName);


        retranslateUi(CPointNameTable);
        QObject::connect(buttonCancel, SIGNAL(clicked()), CPointNameTable, SLOT(close()));

        QMetaObject::connectSlotsByName(CPointNameTable);
    } // setupUi

    void retranslateUi(QDialog *CPointNameTable)
    {
        CPointNameTable->setWindowTitle(QApplication::translate("CPointNameTable", "\347\202\271\345\220\215\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CPointNameTable", "\351\200\211\346\213\251\347\202\271\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("CPointNameTable", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("CPointNameTable", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CPointNameTable);
    } // retranslateUi

};

namespace Ui {
    class CPointNameTable: public Ui_CPointNameTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POINTNAME_H
