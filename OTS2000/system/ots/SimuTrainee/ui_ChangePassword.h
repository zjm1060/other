/********************************************************************************
** Form generated from reading ui file 'ChangePassword.ui'
**
** Created: Wed Jan 14 10:22:05 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CHANGEPASSWORD_H
#define UI_CHANGEPASSWORD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ChangePassword
{
public:
    QLabel *label_3;
    QLineEdit *editPasswordNewConfirm;
    QLabel *label_6;
    QLabel *label_2;
    QLineEdit *editPasswordNew;
    QLabel *label_5;
    QLabel *label;
    QLineEdit *editPasswordOld;
    QLabel *label_4;
    QPushButton *buttonEnter;
    QPushButton *buttonCancel;

    void setupUi(QDialog *ChangePassword)
    {
        if (ChangePassword->objectName().isEmpty())
            ChangePassword->setObjectName(QString::fromUtf8("ChangePassword"));
        ChangePassword->resize(401, 200);
        ChangePassword->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";\n"
"background-color: rgb(231, 240, 245);"));
        label_3 = new QLabel(ChangePassword);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 104, 96, 39));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMinimumSize(QSize(80, 0));
        label_3->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);\n"
"background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 0), stop:0.5 rgba(224, 234, 245, 0), stop:1 rgba(255, 255, 255, 0));\n"
"    border-width:0px;"));
        editPasswordNewConfirm = new QLineEdit(ChangePassword);
        editPasswordNewConfirm->setObjectName(QString::fromUtf8("editPasswordNewConfirm"));
        editPasswordNewConfirm->setGeometry(QRect(114, 109, 261, 29));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(editPasswordNewConfirm->sizePolicy().hasHeightForWidth());
        editPasswordNewConfirm->setSizePolicy(sizePolicy1);
        editPasswordNewConfirm->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 100), stop:0.5 rgba(224, 234, 245, 100), stop:1 rgba(255, 255, 255, 100));\n"
" border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    padding: 2px;"));
        editPasswordNewConfirm->setEchoMode(QLineEdit::Password);
        label_6 = new QLabel(ChangePassword);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(381, 104, 16, 39));
        label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        label_2 = new QLabel(ChangePassword);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 57, 80, 39));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(80, 0));
        label_2->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);\n"
"background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 0), stop:0.5 rgba(224, 234, 245, 0), stop:1 rgba(255, 255, 255, 0));\n"
"    border-width:0px;"));
        editPasswordNew = new QLineEdit(ChangePassword);
        editPasswordNew->setObjectName(QString::fromUtf8("editPasswordNew"));
        editPasswordNew->setGeometry(QRect(114, 62, 261, 29));
        sizePolicy1.setHeightForWidth(editPasswordNew->sizePolicy().hasHeightForWidth());
        editPasswordNew->setSizePolicy(sizePolicy1);
        editPasswordNew->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 100), stop:0.5 rgba(224, 234, 245, 100), stop:1 rgba(255, 255, 255, 100));\n"
" border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    padding: 2px;"));
        editPasswordNew->setEchoMode(QLineEdit::Password);
        label_5 = new QLabel(ChangePassword);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(381, 57, 16, 39));
        label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        label = new QLabel(ChangePassword);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 80, 39));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(80, 0));
        label->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);\n"
"background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 0), stop:0.5 rgba(224, 234, 245, 0), stop:1 rgba(255, 255, 255, 0));\n"
"    border-width:0px;"));
        editPasswordOld = new QLineEdit(ChangePassword);
        editPasswordOld->setObjectName(QString::fromUtf8("editPasswordOld"));
        editPasswordOld->setGeometry(QRect(114, 15, 261, 29));
        sizePolicy1.setHeightForWidth(editPasswordOld->sizePolicy().hasHeightForWidth());
        editPasswordOld->setSizePolicy(sizePolicy1);
        editPasswordOld->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 100), stop:0.5 rgba(224, 234, 245, 100), stop:1 rgba(255, 255, 255, 100));\n"
" border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    padding: 2px;"));
        editPasswordOld->setEchoMode(QLineEdit::Password);
        label_4 = new QLabel(ChangePassword);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(381, 10, 16, 39));
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        buttonEnter = new QPushButton(ChangePassword);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setGeometry(QRect(84, 151, 77, 30));
        buttonEnter->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));
        buttonCancel = new QPushButton(ChangePassword);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setGeometry(QRect(240, 151, 77, 30));
        buttonCancel->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(214, 240, 253, 255), stop:0.536723 rgba(216, 239, 253, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 2px;\n"
"    border-color: rgb(0, 113, 173);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
"\n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));

        retranslateUi(ChangePassword);

        QMetaObject::connectSlotsByName(ChangePassword);
    } // setupUi

    void retranslateUi(QDialog *ChangePassword)
    {
        ChangePassword->setWindowTitle(QApplication::translate("ChangePassword", "\344\277\256\346\224\271\345\257\206\347\240\201", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ChangePassword", "\347\241\256\350\256\244\346\226\260\345\257\206\347\240\201\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QString());
        label_2->setText(QApplication::translate("ChangePassword", "\346\226\260\345\257\206\347\240\201\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QString());
        label->setText(QApplication::translate("ChangePassword", "\345\216\237\345\257\206\347\240\201\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QString());
        buttonEnter->setText(QApplication::translate("ChangePassword", "\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("ChangePassword", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ChangePassword);
    } // retranslateUi

};

namespace Ui {
    class ChangePassword: public Ui_ChangePassword {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGEPASSWORD_H
