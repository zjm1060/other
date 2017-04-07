/********************************************************************************
** Form generated from reading ui file 'loginstudent.ui'
**
** Created: Thu Jan 15 10:25:46 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LOGINSTUDENT_H
#define UI_LOGINSTUDENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableView>

QT_BEGIN_NAMESPACE

class Ui_CLoginStudentTable
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLabel *label_curstate;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QComboBox *comboBox;
    QPushButton *pushButtonLogoutAll;
    QPushButton *pushButtonExwait;
    QPushButton *pushButtonExamGo;
    QTableView *viewLoginStudent;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonTask;
    QPushButton *buttonScene;
    QPushButton *buttonFault;
    QPushButton *buttonRefresh;
    QPushButton *buttonExit;

    void setupUi(QDialog *CLoginStudentTable)
    {
        if (CLoginStudentTable->objectName().isEmpty())
            CLoginStudentTable->setObjectName(QString::fromUtf8("CLoginStudentTable"));
        CLoginStudentTable->resize(1183, 793);
        gridLayout_2 = new QGridLayout(CLoginStudentTable);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox = new QGroupBox(CLoginStudentTable);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        label_curstate = new QLabel(groupBox);
        label_curstate->setObjectName(QString::fromUtf8("label_curstate"));

        horizontalLayout_2->addWidget(label_curstate);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label);

        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setMinimumSize(QSize(300, 0));
        comboBox->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(comboBox);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 3);

        pushButtonLogoutAll = new QPushButton(groupBox);
        pushButtonLogoutAll->setObjectName(QString::fromUtf8("pushButtonLogoutAll"));

        gridLayout->addWidget(pushButtonLogoutAll, 1, 0, 1, 1);

        pushButtonExwait = new QPushButton(groupBox);
        pushButtonExwait->setObjectName(QString::fromUtf8("pushButtonExwait"));

        gridLayout->addWidget(pushButtonExwait, 1, 1, 1, 1);

        pushButtonExamGo = new QPushButton(groupBox);
        pushButtonExamGo->setObjectName(QString::fromUtf8("pushButtonExamGo"));

        gridLayout->addWidget(pushButtonExamGo, 1, 2, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);

        viewLoginStudent = new QTableView(CLoginStudentTable);
        viewLoginStudent->setObjectName(QString::fromUtf8("viewLoginStudent"));
        viewLoginStudent->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";\n"
""));
        viewLoginStudent->horizontalHeader()->setStretchLastSection(true);

        gridLayout_2->addWidget(viewLoginStudent, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(100);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buttonTask = new QPushButton(CLoginStudentTable);
        buttonTask->setObjectName(QString::fromUtf8("buttonTask"));
        buttonTask->setMinimumSize(QSize(121, 41));
        buttonTask->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonTask);

        buttonScene = new QPushButton(CLoginStudentTable);
        buttonScene->setObjectName(QString::fromUtf8("buttonScene"));
        buttonScene->setMinimumSize(QSize(121, 41));
        buttonScene->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonScene);

        buttonFault = new QPushButton(CLoginStudentTable);
        buttonFault->setObjectName(QString::fromUtf8("buttonFault"));
        buttonFault->setMinimumSize(QSize(121, 41));
        buttonFault->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonFault);

        buttonRefresh = new QPushButton(CLoginStudentTable);
        buttonRefresh->setObjectName(QString::fromUtf8("buttonRefresh"));
        buttonRefresh->setMinimumSize(QSize(121, 41));
        buttonRefresh->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonRefresh);

        buttonExit = new QPushButton(CLoginStudentTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setMinimumSize(QSize(121, 41));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonExit);


        gridLayout_2->addLayout(horizontalLayout, 2, 0, 1, 1);

        QWidget::setTabOrder(buttonTask, buttonExit);

        retranslateUi(CLoginStudentTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CLoginStudentTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CLoginStudentTable);
    } // setupUi

    void retranslateUi(QDialog *CLoginStudentTable)
    {
        CLoginStudentTable->setWindowTitle(QApplication::translate("CLoginStudentTable", "\345\275\223\345\211\215\347\231\273\345\275\225\345\255\246\345\221\230", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CLoginStudentTable", "\345\255\246\345\221\230\346\225\264\344\275\223\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CLoginStudentTable", "\345\205\250\344\275\223\345\255\246\345\221\230\345\275\223\345\211\215\346\250\241\345\274\217\345\217\212\347\212\266\346\200\201\357\274\232", 0, QApplication::UnicodeUTF8));
        label_curstate->setText(QApplication::translate("CLoginStudentTable", "TextLabel", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CLoginStudentTable", "    \345\237\271\350\256\255\346\250\241\345\274\217\345\210\207\346\215\242    ", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("CLoginStudentTable", "\350\257\267\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CLoginStudentTable", "\350\207\252\347\224\261\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CLoginStudentTable", "\345\205\250\344\275\223\345\237\271\350\256\255", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CLoginStudentTable", "\345\205\250\344\275\223\350\200\203\346\240\270", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CLoginStudentTable", "\347\253\236\350\265\233\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8)
        );
        pushButtonLogoutAll->setText(QApplication::translate("CLoginStudentTable", "\350\270\242\345\207\272\346\211\200\346\234\211\345\255\246\345\221\230\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
        pushButtonExwait->setText(QApplication::translate("CLoginStudentTable", "\344\273\273\345\212\241\345\207\206\345\244\207\357\274\210\344\270\215\345\217\257\345\274\200\345\247\213\357\274\214\347\255\211\345\276\205\345\267\245\345\206\265\346\225\205\351\232\234\344\270\213\345\217\221\357\274\211", 0, QApplication::UnicodeUTF8));
        pushButtonExamGo->setText(QApplication::translate("CLoginStudentTable", "\344\273\273\345\212\241\345\217\257\345\274\200\345\247\213", 0, QApplication::UnicodeUTF8));
        buttonTask->setText(QApplication::translate("CLoginStudentTable", "\350\256\276\347\275\256\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        buttonScene->setText(QApplication::translate("CLoginStudentTable", "\350\256\276\347\275\256\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8));
        buttonFault->setText(QApplication::translate("CLoginStudentTable", "\350\256\276\347\275\256\346\225\205\351\232\234", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QApplication::translate("CLoginStudentTable", "\345\210\267   \346\226\260", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CLoginStudentTable", "\351\200\200   \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CLoginStudentTable);
    } // retranslateUi

};

namespace Ui {
    class CLoginStudentTable: public Ui_CLoginStudentTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINSTUDENT_H
