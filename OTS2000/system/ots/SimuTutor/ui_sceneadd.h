/********************************************************************************
** Form generated from reading ui file 'sceneadd.ui'
**
** Created: Thu Jan 15 10:25:47 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SCENEADD_H
#define UI_SCENEADD_H

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
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CSceneAddTable
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *editStation;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QComboBox *editDev;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QLineEdit *editFileName;
    QVBoxLayout *verticalLayout;
    QLabel *label_4;
    QTextEdit *editRemark;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *buttonApply;
    QPushButton *buttonEnter;
    QPushButton *buttonExit;

    void setupUi(QDialog *CSceneAddTable)
    {
        if (CSceneAddTable->objectName().isEmpty())
            CSceneAddTable->setObjectName(QString::fromUtf8("CSceneAddTable"));
        CSceneAddTable->resize(300, 400);
        verticalLayout_2 = new QVBoxLayout(CSceneAddTable);
        verticalLayout_2->setSpacing(15);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CSceneAddTable);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(120, 0));
        label->setMaximumSize(QSize(120, 16777215));
        label->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label);

        editName = new QLineEdit(CSceneAddTable);
        editName->setObjectName(QString::fromUtf8("editName"));

        horizontalLayout->addWidget(editName);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(CSceneAddTable);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(120, 0));
        label_2->setMaximumSize(QSize(120, 16777215));
        label_2->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_2);

        editStation = new QComboBox(CSceneAddTable);
        editStation->setObjectName(QString::fromUtf8("editStation"));

        horizontalLayout_2->addWidget(editStation);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(CSceneAddTable);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(120, 0));
        label_3->setMaximumSize(QSize(120, 16777215));
        label_3->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_3);

        editDev = new QComboBox(CSceneAddTable);
        editDev->setObjectName(QString::fromUtf8("editDev"));

        horizontalLayout_3->addWidget(editDev);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_5 = new QLabel(CSceneAddTable);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(120, 0));
        label_5->setMaximumSize(QSize(120, 16777215));
        label_5->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label_5);

        editFileName = new QLineEdit(CSceneAddTable);
        editFileName->setObjectName(QString::fromUtf8("editFileName"));

        horizontalLayout_4->addWidget(editFileName);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_4 = new QLabel(CSceneAddTable);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(label_4);

        editRemark = new QTextEdit(CSceneAddTable);
        editRemark->setObjectName(QString::fromUtf8("editRemark"));

        verticalLayout->addWidget(editRemark);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(20);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        buttonApply = new QPushButton(CSceneAddTable);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(buttonApply);

        buttonEnter = new QPushButton(CSceneAddTable);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(buttonEnter);

        buttonExit = new QPushButton(CSceneAddTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(buttonExit);


        verticalLayout_2->addLayout(horizontalLayout_5);

        QWidget::setTabOrder(editName, editFileName);
        QWidget::setTabOrder(editFileName, editRemark);
        QWidget::setTabOrder(editRemark, buttonEnter);
        QWidget::setTabOrder(buttonEnter, buttonExit);

        retranslateUi(CSceneAddTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CSceneAddTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CSceneAddTable);
    } // setupUi

    void retranslateUi(QDialog *CSceneAddTable)
    {
        CSceneAddTable->setWindowTitle(QApplication::translate("CSceneAddTable", "\345\267\245\345\206\265\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CSceneAddTable", "\345\267\245\345\206\265\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CSceneAddTable", "\345\216\202    \347\253\231\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CSceneAddTable", "\350\256\276    \345\244\207\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CSceneAddTable", "\351\273\230\350\256\244\347\224\273\351\235\242\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CSceneAddTable", "\345\244\207    \346\263\250\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonApply->setText(QApplication::translate("CSceneAddTable", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("CSceneAddTable", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CSceneAddTable", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CSceneAddTable);
    } // retranslateUi

};

namespace Ui {
    class CSceneAddTable: public Ui_CSceneAddTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCENEADD_H
