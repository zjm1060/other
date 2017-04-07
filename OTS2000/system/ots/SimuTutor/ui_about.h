/********************************************************************************
** Form generated from reading ui file 'about.ui'
**
** Created: Thu Jan 15 10:28:24 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_CAboutTable
{
public:
    QPushButton *buttonEnter;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QTextEdit *CAboutEdit;

    void setupUi(QDialog *CAboutTable)
    {
        if (CAboutTable->objectName().isEmpty())
            CAboutTable->setObjectName(QString::fromUtf8("CAboutTable"));
        CAboutTable->resize(460, 280);
        buttonEnter = new QPushButton(CAboutTable);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setGeometry(QRect(355, 245, 91, 25));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 75 11pt \"\345\256\213\344\275\223\";"));
        label = new QLabel(CAboutTable);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(165, -5, 211, 31));
        label->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));
        label_2 = new QLabel(CAboutTable);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(165, 25, 151, 16));
        label_2->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));
        label_3 = new QLabel(CAboutTable);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(165, 50, 141, 16));
        label_3->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));
        label_4 = new QLabel(CAboutTable);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(165, 75, 261, 16));
        label_4->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));
        CAboutEdit = new QTextEdit(CAboutTable);
        CAboutEdit->setObjectName(QString::fromUtf8("CAboutEdit"));
        CAboutEdit->setGeometry(QRect(165, 100, 281, 141));
        QWidget::setTabOrder(CAboutEdit, buttonEnter);

        retranslateUi(CAboutTable);
        QObject::connect(buttonEnter, SIGNAL(clicked()), CAboutTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CAboutTable);
    } // setupUi

    void retranslateUi(QDialog *CAboutTable)
    {
        CAboutTable->setWindowTitle(QApplication::translate("CAboutTable", "\345\205\263\344\272\216OTS2000\346\225\231\350\202\262\347\256\241\347\220\206\347\263\273\347\273\237", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("CAboutTable", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CAboutTable", "OTS2000\346\225\231\345\221\230\347\256\241\347\220\206\347\263\273\347\273\237", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CAboutTable", "\347\211\210\346\234\254", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CAboutTable", "\347\211\210\346\235\203\346\211\200\346\234\211", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CAboutTable", "\345\214\227\344\272\254\344\270\255\346\260\264\347\247\221\346\260\264\347\224\265\347\247\221\346\212\200\345\274\200\345\217\221\346\234\211\351\231\220\345\205\254\345\217\270", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CAboutTable);
    } // retranslateUi

};

namespace Ui {
    class CAboutTable: public Ui_CAboutTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
