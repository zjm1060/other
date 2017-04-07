/********************************************************************************
** Form generated from reading ui file 'stationadd.ui'
**
** Created: Thu Jan 15 10:25:27 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STATIONADD_H
#define UI_STATIONADD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CStationAddTable
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editHostName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *editOthName;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *editIpA;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *editIpB;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QCheckBox *checkStudent;
    QCheckBox *checkTeacher;
    QCheckBox *checkServer;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QLineEdit *editServerId;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *buttonApply;
    QPushButton *buttonEnter;
    QPushButton *buttonExit;

    void setupUi(QDialog *CStationAddTable)
    {
        if (CStationAddTable->objectName().isEmpty())
            CStationAddTable->setObjectName(QString::fromUtf8("CStationAddTable"));
        CStationAddTable->resize(409, 280);
        verticalLayout = new QVBoxLayout(CStationAddTable);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CStationAddTable);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(120, 0));
        label->setMaximumSize(QSize(120, 16777215));
        label->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label);

        editHostName = new QLineEdit(CStationAddTable);
        editHostName->setObjectName(QString::fromUtf8("editHostName"));

        horizontalLayout->addWidget(editHostName);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(CStationAddTable);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(120, 0));
        label_2->setMaximumSize(QSize(120, 16777215));
        label_2->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_2);

        editOthName = new QLineEdit(CStationAddTable);
        editOthName->setObjectName(QString::fromUtf8("editOthName"));

        horizontalLayout_2->addWidget(editOthName);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(CStationAddTable);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(120, 0));
        label_3->setMaximumSize(QSize(120, 16777215));
        label_3->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_3);

        editIpA = new QLineEdit(CStationAddTable);
        editIpA->setObjectName(QString::fromUtf8("editIpA"));

        horizontalLayout_3->addWidget(editIpA);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(CStationAddTable);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(120, 0));
        label_4->setMaximumSize(QSize(120, 16777215));
        label_4->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label_4);

        editIpB = new QLineEdit(CStationAddTable);
        editIpB->setObjectName(QString::fromUtf8("editIpB"));

        horizontalLayout_4->addWidget(editIpB);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(CStationAddTable);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(120, 0));
        label_5->setMaximumSize(QSize(120, 16777215));
        label_5->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(label_5);

        checkStudent = new QCheckBox(CStationAddTable);
        checkStudent->setObjectName(QString::fromUtf8("checkStudent"));
        checkStudent->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(checkStudent);

        checkTeacher = new QCheckBox(CStationAddTable);
        checkTeacher->setObjectName(QString::fromUtf8("checkTeacher"));
        checkTeacher->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(checkTeacher);

        checkServer = new QCheckBox(CStationAddTable);
        checkServer->setObjectName(QString::fromUtf8("checkServer"));
        checkServer->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(checkServer);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_6 = new QLabel(CStationAddTable);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(120, 0));
        label_6->setMaximumSize(QSize(120, 16777215));
        label_6->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_6->addWidget(label_6);

        editServerId = new QLineEdit(CStationAddTable);
        editServerId->setObjectName(QString::fromUtf8("editServerId"));

        horizontalLayout_6->addWidget(editServerId);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(40);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        buttonApply = new QPushButton(CStationAddTable);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_7->addWidget(buttonApply);

        buttonEnter = new QPushButton(CStationAddTable);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_7->addWidget(buttonEnter);

        buttonExit = new QPushButton(CStationAddTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_7->addWidget(buttonExit);


        verticalLayout->addLayout(horizontalLayout_7);

        QWidget::setTabOrder(editHostName, editOthName);
        QWidget::setTabOrder(editOthName, editIpA);
        QWidget::setTabOrder(editIpA, editIpB);
        QWidget::setTabOrder(editIpB, checkStudent);
        QWidget::setTabOrder(checkStudent, checkTeacher);
        QWidget::setTabOrder(checkTeacher, checkServer);
        QWidget::setTabOrder(checkServer, editServerId);
        QWidget::setTabOrder(editServerId, buttonEnter);
        QWidget::setTabOrder(buttonEnter, buttonExit);

        retranslateUi(CStationAddTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CStationAddTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CStationAddTable);
    } // setupUi

    void retranslateUi(QDialog *CStationAddTable)
    {
        CStationAddTable->setWindowTitle(QApplication::translate("CStationAddTable", "\346\267\273\345\212\240\345\267\245\344\275\234\347\253\231", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CStationAddTable", "\344\270\273  \346\234\272  \345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CStationAddTable", "\345\210\253      \345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CStationAddTable", "A  \347\275\221   IP\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CStationAddTable", "B  \347\275\221   IP\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CStationAddTable", "\345\267\245\344\275\234\347\253\231\347\261\273\345\236\213\357\274\232", 0, QApplication::UnicodeUTF8));
        checkStudent->setText(QApplication::translate("CStationAddTable", "\345\255\246\345\221\230\347\253\231", 0, QApplication::UnicodeUTF8));
        checkTeacher->setText(QApplication::translate("CStationAddTable", "\346\225\231\345\221\230\347\253\231", 0, QApplication::UnicodeUTF8));
        checkServer->setText(QApplication::translate("CStationAddTable", "\346\250\241\345\236\213\346\234\215\345\212\241\345\231\250", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CStationAddTable", "\346\250\241\345\236\213\346\234\215\345\212\241\345\231\250ID\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonApply->setText(QApplication::translate("CStationAddTable", "\345\272\224  \347\224\250", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("CStationAddTable", "\347\241\256  \345\256\232", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CStationAddTable", "\351\200\200  \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CStationAddTable);
    } // retranslateUi

};

namespace Ui {
    class CStationAddTable: public Ui_CStationAddTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATIONADD_H
