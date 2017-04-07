/********************************************************************************
** Form generated from reading ui file 'login.ui'
**
** Created: Wed Jan 14 10:22:49 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

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

class Ui_CLoginTable
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *labelLogin;
    QLineEdit *editUserName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelPassword;
    QLineEdit *editPassword;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *buttonLogin;
    QPushButton *buttonExit;

    void setupUi(QDialog *CLoginTable)
    {
        if (CLoginTable->objectName().isEmpty())
            CLoginTable->setObjectName(QString::fromUtf8("CLoginTable"));
        CLoginTable->resize(372, 232);
        verticalLayout = new QVBoxLayout(CLoginTable);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(30, -1, 50, -1);
        labelLogin = new QLabel(CLoginTable);
        labelLogin->setObjectName(QString::fromUtf8("labelLogin"));
        labelLogin->setStyleSheet(QString::fromUtf8("font: 16pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(labelLogin);

        editUserName = new QLineEdit(CLoginTable);
        editUserName->setObjectName(QString::fromUtf8("editUserName"));

        horizontalLayout->addWidget(editUserName);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(30, -1, 50, -1);
        labelPassword = new QLabel(CLoginTable);
        labelPassword->setObjectName(QString::fromUtf8("labelPassword"));
        labelPassword->setStyleSheet(QString::fromUtf8("font: 16pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(labelPassword);

        editPassword = new QLineEdit(CLoginTable);
        editPassword->setObjectName(QString::fromUtf8("editPassword"));

        horizontalLayout_2->addWidget(editPassword);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(20);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(150, -1, 30, -1);
        buttonLogin = new QPushButton(CLoginTable);
        buttonLogin->setObjectName(QString::fromUtf8("buttonLogin"));
        buttonLogin->setStyleSheet(QString::fromUtf8("font: 16pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(buttonLogin);

        buttonExit = new QPushButton(CLoginTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 16pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(buttonExit);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(CLoginTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CLoginTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CLoginTable);
    } // setupUi

    void retranslateUi(QDialog *CLoginTable)
    {
        CLoginTable->setWindowTitle(QApplication::translate("CLoginTable", "\346\225\231\345\221\230\347\256\241\347\220\206\347\263\273\347\273\237\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
        labelLogin->setText(QApplication::translate("CLoginTable", "\347\224\250\346\210\267\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        labelPassword->setText(QApplication::translate("CLoginTable", "\345\257\206  \347\240\201\357\274\232", 0, QApplication::UnicodeUTF8));
        editPassword->setInputMask(QString());
        buttonLogin->setText(QApplication::translate("CLoginTable", "\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CLoginTable", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CLoginTable);
    } // retranslateUi

};

namespace Ui {
    class CLoginTable: public Ui_CLoginTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
