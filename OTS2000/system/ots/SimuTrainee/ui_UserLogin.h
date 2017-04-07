/********************************************************************************
** Form generated from reading ui file 'UserLogin.ui'
**
** Created: Wed Jan 14 10:22:08 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_USERLOGIN_H
#define UI_USERLOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QWidget *widget;
    QLabel *label_welcom;
    QLineEdit *editPassword;
    QLineEdit *editHdbIp;
    QPushButton *buttonRegister;
    QPushButton *buttonLogin;
    QPushButton *buttonExit;
    QLabel *label;
    QLineEdit *editUserName;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QDialog *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QString::fromUtf8("Login"));
        Login->resize(451, 303);
        Login->setMinimumSize(QSize(330, 220));
        Login->setMaximumSize(QSize(660, 440));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/login.png"), QSize(), QIcon::Normal, QIcon::Off);
        Login->setWindowIcon(icon);
        Login->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image: url(:/pic/images/background/login_background.png);"));
        widget = new QWidget(Login);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 450, 300));
        widget->setMinimumSize(QSize(450, 300));
        widget->setMaximumSize(QSize(330, 220));
        widget->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        label_welcom = new QLabel(widget);
        label_welcom->setObjectName(QString::fromUtf8("label_welcom"));
        label_welcom->setGeometry(QRect(50, 40, 351, 30));
        label_welcom->setMinimumSize(QSize(0, 30));
        label_welcom->setMaximumSize(QSize(351, 30));
        label_welcom->setStyleSheet(QString::fromUtf8("border-image: url(:/pic/images/background/welcom_words.png);"));
        editPassword = new QLineEdit(widget);
        editPassword->setObjectName(QString::fromUtf8("editPassword"));
        editPassword->setGeometry(QRect(180, 160, 221, 21));
        editPassword->setMouseTracking(true);
        editPassword->setAcceptDrops(true);
        editPassword->setStyleSheet(QString::fromUtf8(""));
        editPassword->setEchoMode(QLineEdit::Password);
        editHdbIp = new QLineEdit(widget);
        editHdbIp->setObjectName(QString::fromUtf8("editHdbIp"));
        editHdbIp->setGeometry(QRect(180, 200, 221, 21));
        editHdbIp->setMouseTracking(true);
        editHdbIp->setAcceptDrops(true);
        editHdbIp->setStyleSheet(QString::fromUtf8(""));
        buttonRegister = new QPushButton(widget);
        buttonRegister->setObjectName(QString::fromUtf8("buttonRegister"));
        buttonRegister->setEnabled(false);
        buttonRegister->setGeometry(QRect(51, 250, 82, 25));
        buttonRegister->setMinimumSize(QSize(82, 20));
        buttonRegister->setStyleSheet(QString::fromUtf8("   border: 1px solid #cfcff1;		border-radius: 1px;	background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde);	min-width: 80px;\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        buttonRegister->setAutoDefault(false);
        buttonRegister->setDefault(true);
        buttonLogin = new QPushButton(widget);
        buttonLogin->setObjectName(QString::fromUtf8("buttonLogin"));
        buttonLogin->setGeometry(QRect(181, 250, 82, 25));
        buttonLogin->setMinimumSize(QSize(82, 20));
        buttonLogin->setFocusPolicy(Qt::TabFocus);
        buttonLogin->setStyleSheet(QString::fromUtf8("   border: 1px solid #cfcff1;		border-radius: 2px;	background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde);	min-width: 80px;\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        buttonLogin->setAutoDefault(false);
        buttonLogin->setDefault(true);
        buttonExit = new QPushButton(widget);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setGeometry(QRect(310, 250, 82, 25));
        buttonExit->setMinimumSize(QSize(82, 20));
        buttonExit->setStyleSheet(QString::fromUtf8("   border: 1px solid #cfcff1;		border-radius: 2px;	background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde);	min-width: 80px;\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        buttonExit->setAutoDefault(false);
        buttonExit->setDefault(true);
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(49, 110, 138, 32));
        label->setStyleSheet(QString::fromUtf8("background-image: url(:/pic/images/background/\351\200\217\346\230\216\345\272\225\345\233\2761.png);\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        editUserName = new QLineEdit(widget);
        editUserName->setObjectName(QString::fromUtf8("editUserName"));
        editUserName->setGeometry(QRect(180, 120, 221, 21));
        editUserName->setMouseTracking(true);
        editUserName->setAcceptDrops(true);
        editUserName->setStyleSheet(QString::fromUtf8(""));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(49, 150, 137, 32));
        label_2->setStyleSheet(QString::fromUtf8("background-image: url(:/pic/images/background/\351\200\217\346\230\216\345\272\225\345\233\2761.png);\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(49, 190, 139, 32));
        label_3->setStyleSheet(QString::fromUtf8("background-image: url(:/pic/images/background/\351\200\217\346\230\216\345\272\225\345\233\2761.png);\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        QWidget::setTabOrder(editUserName, editPassword);
        QWidget::setTabOrder(editPassword, buttonLogin);
        QWidget::setTabOrder(buttonLogin, buttonExit);
        QWidget::setTabOrder(buttonExit, buttonRegister);
        QWidget::setTabOrder(buttonRegister, editHdbIp);

        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QDialog *Login)
    {
        Login->setWindowTitle(QApplication::translate("Login", "\346\254\242\350\277\216", 0, QApplication::UnicodeUTF8));
        label_welcom->setText(QString());
        editPassword->setText(QString());
        editHdbIp->setText(QString());
        buttonRegister->setText(QApplication::translate("Login", "\346\263\250 \345\206\214", 0, QApplication::UnicodeUTF8));
        buttonLogin->setText(QApplication::translate("Login", "\347\231\273 \345\275\225", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("Login", "\351\200\200 \345\207\272", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Login", "\345\255\246 \345\221\230 \347\224\250 \346\210\267 \345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        editUserName->setText(QString());
        label_2->setText(QApplication::translate("Login", " \347\231\273  \345\275\225  \345\257\206  \347\240\201\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Login", " \345\216\206\345\217\262\345\272\223\346\234\215\345\212\241\345\231\250\357\274\232", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Login);
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERLOGIN_H
