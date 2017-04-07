/********************************************************************************
** Form generated from reading ui file 'SceneReset.ui'
**
** Created: Thu Jan 15 10:25:58 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SCENERESET_H
#define UI_SCENERESET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_SceneReset
{
public:
    QGroupBox *groupBox;
    QTreeWidget *treeWidget;
    QPushButton *pushButtonInit;
    QLabel *label;
    QLineEdit *lineEditANA;
    QLineEdit *lineEditINT;
    QLabel *label_2;
    QPushButton *pushButton_confirm;
    QLabel *label_3;
    QLineEdit *lineEditPSW;
    QPushButton *pushButton_OK;
    QPushButton *pushButton_CANCEL;

    void setupUi(QDialog *SceneReset)
    {
        if (SceneReset->objectName().isEmpty())
            SceneReset->setObjectName(QString::fromUtf8("SceneReset"));
        SceneReset->resize(526, 565);
        groupBox = new QGroupBox(SceneReset);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 501, 401));
        treeWidget = new QTreeWidget(groupBox);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setGeometry(QRect(10, 30, 211, 361));
        pushButtonInit = new QPushButton(groupBox);
        pushButtonInit->setObjectName(QString::fromUtf8("pushButtonInit"));
        pushButtonInit->setGeometry(QRect(230, 340, 261, 51));
        pushButtonInit->setStyleSheet(QString::fromUtf8("font: 75 12pt \"Agency FB\";"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(250, 42, 111, 41));
        label->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);font: 12pt \"Agency FB\";"));
        lineEditANA = new QLineEdit(groupBox);
        lineEditANA->setObjectName(QString::fromUtf8("lineEditANA"));
        lineEditANA->setGeometry(QRect(370, 49, 113, 31));
        lineEditINT = new QLineEdit(groupBox);
        lineEditINT->setObjectName(QString::fromUtf8("lineEditINT"));
        lineEditINT->setGeometry(QRect(370, 107, 113, 31));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(250, 100, 111, 41));
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);font: 12pt \"Agency FB\";"));
        pushButton_confirm = new QPushButton(groupBox);
        pushButton_confirm->setObjectName(QString::fromUtf8("pushButton_confirm"));
        pushButton_confirm->setGeometry(QRect(370, 270, 111, 41));
        pushButton_confirm->setStyleSheet(QString::fromUtf8("font: 12pt \"Agency FB\";"));
        label_3 = new QLabel(SceneReset);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(270, 233, 81, 41));
        label_3->setStyleSheet(QString::fromUtf8("font: 12pt \"Agency FB\";\n"
"color: rgb(170, 170, 0);"));
        lineEditPSW = new QLineEdit(SceneReset);
        lineEditPSW->setObjectName(QString::fromUtf8("lineEditPSW"));
        lineEditPSW->setGeometry(QRect(380, 240, 113, 31));
        lineEditPSW->setEchoMode(QLineEdit::Password);
        pushButton_OK = new QPushButton(SceneReset);
        pushButton_OK->setObjectName(QString::fromUtf8("pushButton_OK"));
        pushButton_OK->setGeometry(QRect(100, 490, 111, 41));
        pushButton_OK->setStyleSheet(QString::fromUtf8("font: 12pt \"Agency FB\";"));
        pushButton_CANCEL = new QPushButton(SceneReset);
        pushButton_CANCEL->setObjectName(QString::fromUtf8("pushButton_CANCEL"));
        pushButton_CANCEL->setGeometry(QRect(300, 490, 111, 41));
        pushButton_CANCEL->setStyleSheet(QString::fromUtf8("font: 12pt \"Agency FB\";"));

        retranslateUi(SceneReset);

        QMetaObject::connectSlotsByName(SceneReset);
    } // setupUi

    void retranslateUi(QDialog *SceneReset)
    {
        SceneReset->setWindowTitle(QApplication::translate("SceneReset", "\345\267\245\345\206\265\345\210\235\345\247\213", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("SceneReset", "\345\267\245\345\206\265\345\210\235\345\247\213", 0, QApplication::UnicodeUTF8));
        pushButtonInit->setText(QApplication::translate("SceneReset", "\346\211\247  \350\241\214  \345\210\235  \345\247\213  \345\214\226", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SceneReset", "\346\250\241\346\213\237\351\207\217\345\210\235\345\247\213\345\200\274", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SceneReset", "\346\225\264\345\236\213\351\207\217\345\210\235\345\247\213\345\200\274", 0, QApplication::UnicodeUTF8));
        pushButton_confirm->setText(QApplication::translate("SceneReset", "\351\252\214  \350\257\201", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SceneReset", "\345\210\235\345\247\213\345\214\226\345\257\206\347\240\201", 0, QApplication::UnicodeUTF8));
        pushButton_OK->setText(QApplication::translate("SceneReset", "\347\241\256  \350\256\244", 0, QApplication::UnicodeUTF8));
        pushButton_CANCEL->setText(QApplication::translate("SceneReset", "\345\217\226  \346\266\210", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SceneReset);
    } // retranslateUi

};

namespace Ui {
    class SceneReset: public Ui_SceneReset {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCENERESET_H
