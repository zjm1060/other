/********************************************************************************
** Form generated from reading ui file 'loginrec.ui'
**
** Created: Thu Jan 15 10:26:40 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LOGINREC_H
#define UI_LOGINREC_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CLoginRecTable
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QComboBox *comboUserName;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QComboBox *comboHostName;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *buttonDisplay;
    QPushButton *buttonDisplayToday;
    QPushButton *buttonExport;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QDateTimeEdit *editStartTime;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QDateTimeEdit *editEndTime;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QLineEdit *editLoginTime;
    QLabel *label_6;
    QLineEdit *editTaskNum;
    QLabel *label_7;
    QLineEdit *editOptNum;
    QTableView *viewLoginRec;

    void setupUi(QDialog *CLoginRecTable)
    {
        if (CLoginRecTable->objectName().isEmpty())
            CLoginRecTable->setObjectName(QString::fromUtf8("CLoginRecTable"));
        CLoginRecTable->resize(800, 600);
        verticalLayout = new QVBoxLayout(CLoginRecTable);
        verticalLayout->setSpacing(9);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(CLoginRecTable);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(60, 0));
        label_3->setMaximumSize(QSize(60, 16777215));
        label_3->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(label_3);

        comboUserName = new QComboBox(groupBox);
        comboUserName->setObjectName(QString::fromUtf8("comboUserName"));
        comboUserName->setMaximumSize(QSize(140, 16777215));
        comboUserName->setEditable(true);

        horizontalLayout->addWidget(comboUserName);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(60, 0));
        label_4->setMaximumSize(QSize(60, 16777215));
        label_4->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_3->addWidget(label_4);

        comboHostName = new QComboBox(groupBox);
        comboHostName->setObjectName(QString::fromUtf8("comboHostName"));
        comboHostName->setMaximumSize(QSize(140, 16777215));

        horizontalLayout_3->addWidget(comboHostName);


        gridLayout->addLayout(horizontalLayout_3, 0, 1, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        buttonDisplay = new QPushButton(groupBox);
        buttonDisplay->setObjectName(QString::fromUtf8("buttonDisplay"));
        buttonDisplay->setMinimumSize(QSize(80, 50));
        buttonDisplay->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_5->addWidget(buttonDisplay);

        buttonDisplayToday = new QPushButton(groupBox);
        buttonDisplayToday->setObjectName(QString::fromUtf8("buttonDisplayToday"));
        buttonDisplayToday->setMinimumSize(QSize(80, 50));
        buttonDisplayToday->setMaximumSize(QSize(16777215, 16777215));
        buttonDisplayToday->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_5->addWidget(buttonDisplayToday);

        buttonExport = new QPushButton(groupBox);
        buttonExport->setObjectName(QString::fromUtf8("buttonExport"));
        buttonExport->setMinimumSize(QSize(80, 50));
        buttonExport->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_5->addWidget(buttonExport);


        gridLayout->addLayout(horizontalLayout_5, 0, 2, 2, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(60, 0));
        label->setMaximumSize(QSize(60, 16777215));
        label->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_2->addWidget(label);

        editStartTime = new QDateTimeEdit(groupBox);
        editStartTime->setObjectName(QString::fromUtf8("editStartTime"));
        editStartTime->setMaximumSize(QSize(140, 16777215));

        horizontalLayout_2->addWidget(editStartTime);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(60, 0));
        label_2->setMaximumSize(QSize(60, 16777215));
        label_2->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_4->addWidget(label_2);

        editEndTime = new QDateTimeEdit(groupBox);
        editEndTime->setObjectName(QString::fromUtf8("editEndTime"));
        editEndTime->setMaximumSize(QSize(140, 16777215));

        horizontalLayout_4->addWidget(editEndTime);


        gridLayout->addLayout(horizontalLayout_4, 1, 1, 1, 1);

        gridLayout->setColumnStretch(0, 3);
        gridLayout->setColumnStretch(1, 3);
        gridLayout->setColumnStretch(2, 5);

        verticalLayout->addWidget(groupBox);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_5 = new QLabel(CLoginRecTable);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_6->addWidget(label_5);

        editLoginTime = new QLineEdit(CLoginRecTable);
        editLoginTime->setObjectName(QString::fromUtf8("editLoginTime"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(editLoginTime->sizePolicy().hasHeightForWidth());
        editLoginTime->setSizePolicy(sizePolicy);
        editLoginTime->setMinimumSize(QSize(160, 0));
        editLoginTime->setMaximumSize(QSize(16777215, 16777215));
        editLoginTime->setStyleSheet(QString::fromUtf8("color: rgb(0, 85, 255);\n"
"font: 75 12pt \"\351\273\221\344\275\223\";"));
        editLoginTime->setAlignment(Qt::AlignCenter);
        editLoginTime->setDragEnabled(false);
        editLoginTime->setReadOnly(true);

        horizontalLayout_6->addWidget(editLoginTime);

        label_6 = new QLabel(CLoginRecTable);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_6->addWidget(label_6);

        editTaskNum = new QLineEdit(CLoginRecTable);
        editTaskNum->setObjectName(QString::fromUtf8("editTaskNum"));
        sizePolicy.setHeightForWidth(editTaskNum->sizePolicy().hasHeightForWidth());
        editTaskNum->setSizePolicy(sizePolicy);
        editTaskNum->setMaximumSize(QSize(50, 16777215));
        editTaskNum->setStyleSheet(QString::fromUtf8("color: rgb(0, 85, 255);\n"
"font: 75 12pt \"\351\273\221\344\275\223\";"));
        editTaskNum->setAlignment(Qt::AlignCenter);
        editTaskNum->setDragEnabled(false);
        editTaskNum->setReadOnly(true);

        horizontalLayout_6->addWidget(editTaskNum);

        label_7 = new QLabel(CLoginRecTable);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_6->addWidget(label_7);

        editOptNum = new QLineEdit(CLoginRecTable);
        editOptNum->setObjectName(QString::fromUtf8("editOptNum"));
        sizePolicy.setHeightForWidth(editOptNum->sizePolicy().hasHeightForWidth());
        editOptNum->setSizePolicy(sizePolicy);
        editOptNum->setMinimumSize(QSize(300, 0));
        editOptNum->setStyleSheet(QString::fromUtf8("color: rgb(0, 85, 255);\n"
"font: 75 12pt \"\351\273\221\344\275\223\";"));
        editOptNum->setAlignment(Qt::AlignCenter);
        editOptNum->setDragEnabled(false);
        editOptNum->setReadOnly(true);

        horizontalLayout_6->addWidget(editOptNum);


        verticalLayout->addLayout(horizontalLayout_6);

        viewLoginRec = new QTableView(CLoginRecTable);
        viewLoginRec->setObjectName(QString::fromUtf8("viewLoginRec"));
        viewLoginRec->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        viewLoginRec->setSortingEnabled(true);
        viewLoginRec->horizontalHeader()->setStretchLastSection(true);

        verticalLayout->addWidget(viewLoginRec);


        retranslateUi(CLoginRecTable);

        QMetaObject::connectSlotsByName(CLoginRecTable);
    } // setupUi

    void retranslateUi(QDialog *CLoginRecTable)
    {
        CLoginRecTable->setWindowTitle(QApplication::translate("CLoginRecTable", "\347\231\273\345\275\225\345\216\206\345\217\262\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        label_3->setText(QApplication::translate("CLoginRecTable", "\347\224\250\346\210\267\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CLoginRecTable", "\344\270\273\346\234\272\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonDisplay->setText(QApplication::translate("CLoginRecTable", "\346\237\245\346\211\276", 0, QApplication::UnicodeUTF8));
        buttonDisplayToday->setText(QApplication::translate("CLoginRecTable", "\346\230\276\347\244\272\345\275\223\345\244\251\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        buttonExport->setText(QApplication::translate("CLoginRecTable", "\345\257\274\345\207\272EXCEL", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CLoginRecTable", "\345\274\200\345\247\213\346\227\266\351\227\264\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CLoginRecTable", "\347\273\223\346\235\237\346\227\266\351\227\264\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CLoginRecTable", "\347\231\273\345\275\225\346\200\273\346\227\266\351\227\264\357\274\232", 0, QApplication::UnicodeUTF8));
        editLoginTime->setText(QString());
        label_6->setText(QApplication::translate("CLoginRecTable", "\345\256\214\346\210\220\344\273\273\345\212\241\346\200\273\346\225\260\357\274\232", 0, QApplication::UnicodeUTF8));
        editTaskNum->setText(QString());
        label_7->setText(QApplication::translate("CLoginRecTable", "\345\256\214\346\210\220\346\223\215\344\275\234\346\200\273\346\225\260\357\274\232", 0, QApplication::UnicodeUTF8));
        editOptNum->setText(QString());
        Q_UNUSED(CLoginRecTable);
    } // retranslateUi

};

namespace Ui {
    class CLoginRecTable: public Ui_CLoginRecTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINREC_H
