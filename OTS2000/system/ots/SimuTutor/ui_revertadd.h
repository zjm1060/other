/********************************************************************************
** Form generated from reading ui file 'revertadd.ui'
**
** Created: Thu Jan 15 10:26:22 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_REVERTADD_H
#define UI_REVERTADD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CRevertAddTable
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editRevertName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QDateTimeEdit *editStartTime;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QDateTimeEdit *editEndTime;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *editFileName;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_6;
    QComboBox *editStation;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_7;
    QComboBox *editDev;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_8;
    QComboBox *editSrc;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_9;
    QComboBox *editType;
    QVBoxLayout *verticalLayout;
    QLabel *label_5;
    QTextEdit *editRemark;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *buttonApply;
    QPushButton *buttonEnter;
    QPushButton *buttonExit;

    void setupUi(QDialog *CRevertAddTable)
    {
        if (CRevertAddTable->objectName().isEmpty())
            CRevertAddTable->setObjectName(QString::fromUtf8("CRevertAddTable"));
        CRevertAddTable->resize(416, 508);
        CRevertAddTable->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        verticalLayout_2 = new QVBoxLayout(CRevertAddTable);
        verticalLayout_2->setSpacing(9);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CRevertAddTable);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(120, 0));
        label->setMaximumSize(QSize(120, 16777215));
        label->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label);

        editRevertName = new QLineEdit(CRevertAddTable);
        editRevertName->setObjectName(QString::fromUtf8("editRevertName"));

        horizontalLayout->addWidget(editRevertName);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(CRevertAddTable);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(120, 0));
        label_2->setMaximumSize(QSize(120, 16777215));
        label_2->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_2);

        editStartTime = new QDateTimeEdit(CRevertAddTable);
        editStartTime->setObjectName(QString::fromUtf8("editStartTime"));

        horizontalLayout_2->addWidget(editStartTime);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(CRevertAddTable);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(120, 0));
        label_3->setMaximumSize(QSize(120, 16777215));
        label_3->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_3);

        editEndTime = new QDateTimeEdit(CRevertAddTable);
        editEndTime->setObjectName(QString::fromUtf8("editEndTime"));

        horizontalLayout_3->addWidget(editEndTime);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(CRevertAddTable);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(120, 0));
        label_4->setMaximumSize(QSize(120, 16777215));
        label_4->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label_4);

        editFileName = new QLineEdit(CRevertAddTable);
        editFileName->setObjectName(QString::fromUtf8("editFileName"));

        horizontalLayout_4->addWidget(editFileName);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_6 = new QLabel(CRevertAddTable);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(120, 0));
        label_6->setMaximumSize(QSize(120, 16777215));
        label_6->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(label_6);

        editStation = new QComboBox(CRevertAddTable);
        editStation->setObjectName(QString::fromUtf8("editStation"));

        horizontalLayout_5->addWidget(editStation);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_7 = new QLabel(CRevertAddTable);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(120, 0));
        label_7->setMaximumSize(QSize(120, 16777215));
        label_7->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_6->addWidget(label_7);

        editDev = new QComboBox(CRevertAddTable);
        editDev->setObjectName(QString::fromUtf8("editDev"));

        horizontalLayout_6->addWidget(editDev);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_8 = new QLabel(CRevertAddTable);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(120, 0));
        label_8->setMaximumSize(QSize(120, 16777215));
        label_8->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_7->addWidget(label_8);

        editSrc = new QComboBox(CRevertAddTable);
        editSrc->setObjectName(QString::fromUtf8("editSrc"));

        horizontalLayout_7->addWidget(editSrc);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_9 = new QLabel(CRevertAddTable);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(120, 0));
        label_9->setMaximumSize(QSize(120, 16777215));
        label_9->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_8->addWidget(label_9);

        editType = new QComboBox(CRevertAddTable);
        editType->setObjectName(QString::fromUtf8("editType"));

        horizontalLayout_8->addWidget(editType);


        verticalLayout_2->addLayout(horizontalLayout_8);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_5 = new QLabel(CRevertAddTable);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(label_5);

        editRemark = new QTextEdit(CRevertAddTable);
        editRemark->setObjectName(QString::fromUtf8("editRemark"));

        verticalLayout->addWidget(editRemark);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(20);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        buttonApply = new QPushButton(CRevertAddTable);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_9->addWidget(buttonApply);

        buttonEnter = new QPushButton(CRevertAddTable);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_9->addWidget(buttonEnter);

        buttonExit = new QPushButton(CRevertAddTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_9->addWidget(buttonExit);


        verticalLayout_2->addLayout(horizontalLayout_9);

        QWidget::setTabOrder(editRevertName, editStartTime);
        QWidget::setTabOrder(editStartTime, editEndTime);
        QWidget::setTabOrder(editEndTime, editFileName);
        QWidget::setTabOrder(editFileName, editRemark);
        QWidget::setTabOrder(editRemark, buttonEnter);
        QWidget::setTabOrder(buttonEnter, buttonExit);

        retranslateUi(CRevertAddTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CRevertAddTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CRevertAddTable);
    } // setupUi

    void retranslateUi(QDialog *CRevertAddTable)
    {
        CRevertAddTable->setWindowTitle(QApplication::translate("CRevertAddTable", "\346\267\273\345\212\240\345\267\245\345\206\265\345\206\215\347\216\260", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CRevertAddTable", "\345\267\245\345\206\265\345\206\215\347\216\260\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CRevertAddTable", "\345\274\200\345\247\213\346\227\266\351\227\264\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CRevertAddTable", "\347\273\223\346\235\237\346\227\266\351\227\264\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CRevertAddTable", "\351\273\230\350\256\244\347\224\273\351\235\242\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CRevertAddTable", "\345\216\202 \347\253\231 \345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("CRevertAddTable", "\350\256\276 \345\244\207 \345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("CRevertAddTable", "\346\235\245    \346\272\220\357\274\232", 0, QApplication::UnicodeUTF8));
        editSrc->clear();
        editSrc->insertItems(0, QStringList()
         << QApplication::translate("CRevertAddTable", "\346\235\245\346\272\220\344\272\216\344\273\277\347\234\237\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CRevertAddTable", "\346\235\245\346\272\220\344\272\216\347\233\221\346\216\247\347\263\273\347\273\237", 0, QApplication::UnicodeUTF8)
        );
        label_9->setText(QApplication::translate("CRevertAddTable", "\347\261\273    \345\236\213\357\274\232", 0, QApplication::UnicodeUTF8));
        editType->clear();
        editType->insertItems(0, QStringList()
         << QApplication::translate("CRevertAddTable", "\347\224\273\351\235\242\345\216\206\345\217\262\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CRevertAddTable", "\350\256\276\345\244\207\345\216\206\345\217\262\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8)
        );
        label_5->setText(QApplication::translate("CRevertAddTable", "\350\257\264    \346\230\216\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonApply->setText(QApplication::translate("CRevertAddTable", "\345\272\224  \347\224\250", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("CRevertAddTable", "\347\241\256  \345\256\232", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CRevertAddTable", "\345\217\226  \346\266\210", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CRevertAddTable);
    } // retranslateUi

};

namespace Ui {
    class CRevertAddTable: public Ui_CRevertAddTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REVERTADD_H
