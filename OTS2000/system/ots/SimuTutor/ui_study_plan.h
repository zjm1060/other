/********************************************************************************
** Form generated from reading ui file 'study_plan.ui'
**
** Created: Thu Jan 15 10:25:46 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STUDY_PLAN_H
#define UI_STUDY_PLAN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTableView>

QT_BEGIN_NAMESPACE

class Ui_study_plan
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QTableView *tableView;
    QPushButton *pushButtonAdd;
    QPushButton *pushButtonModify;
    QPushButton *pushButtonDel;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QLineEdit *lineEdit_planname;
    QLabel *label_7;
    QComboBox *comboBoxDoc;
    QLabel *label_6;
    QComboBox *comboBox_usertype;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox_studytime;
    QLabel *label_3;
    QLabel *label_4;
    QSpinBox *spinBox_studypoints;
    QLabel *label_5;
    QPushButton *pushButton_EXIT;
    QPushButton *pushButton_OK;

    void setupUi(QDialog *study_plan)
    {
        if (study_plan->objectName().isEmpty())
            study_plan->setObjectName(QString::fromUtf8("study_plan"));
        study_plan->resize(954, 528);
        study_plan->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));
        gridLayout = new QGridLayout(study_plan);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(study_plan);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tableView = new QTableView(groupBox);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout_3->addWidget(tableView, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 4);

        pushButtonAdd = new QPushButton(study_plan);
        pushButtonAdd->setObjectName(QString::fromUtf8("pushButtonAdd"));

        gridLayout->addWidget(pushButtonAdd, 1, 0, 1, 1);

        pushButtonModify = new QPushButton(study_plan);
        pushButtonModify->setObjectName(QString::fromUtf8("pushButtonModify"));

        gridLayout->addWidget(pushButtonModify, 1, 1, 1, 1);

        pushButtonDel = new QPushButton(study_plan);
        pushButtonDel->setObjectName(QString::fromUtf8("pushButtonDel"));

        gridLayout->addWidget(pushButtonDel, 1, 3, 1, 1);

        groupBox_2 = new QGroupBox(study_plan);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        lineEdit_planname = new QLineEdit(groupBox_2);
        lineEdit_planname->setObjectName(QString::fromUtf8("lineEdit_planname"));

        gridLayout_2->addWidget(lineEdit_planname, 0, 1, 1, 7);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMaximumSize(QSize(125, 16777215));

        gridLayout_2->addWidget(label_7, 1, 0, 1, 1);

        comboBoxDoc = new QComboBox(groupBox_2);
        comboBoxDoc->setObjectName(QString::fromUtf8("comboBoxDoc"));

        gridLayout_2->addWidget(comboBoxDoc, 1, 1, 1, 7);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 2, 0, 1, 1);

        comboBox_usertype = new QComboBox(groupBox_2);
        comboBox_usertype->setObjectName(QString::fromUtf8("comboBox_usertype"));
        comboBox_usertype->setMinimumSize(QSize(120, 0));

        gridLayout_2->addWidget(comboBox_usertype, 2, 1, 1, 1);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 2, 2, 1, 1);

        doubleSpinBox_studytime = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_studytime->setObjectName(QString::fromUtf8("doubleSpinBox_studytime"));
        doubleSpinBox_studytime->setDecimals(1);
        doubleSpinBox_studytime->setMaximum(24);

        gridLayout_2->addWidget(doubleSpinBox_studytime, 2, 3, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 2, 4, 1, 1);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 2, 5, 1, 1);

        spinBox_studypoints = new QSpinBox(groupBox_2);
        spinBox_studypoints->setObjectName(QString::fromUtf8("spinBox_studypoints"));

        gridLayout_2->addWidget(spinBox_studypoints, 2, 6, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 2, 7, 1, 1);


        gridLayout->addWidget(groupBox_2, 2, 0, 1, 4);

        pushButton_EXIT = new QPushButton(study_plan);
        pushButton_EXIT->setObjectName(QString::fromUtf8("pushButton_EXIT"));

        gridLayout->addWidget(pushButton_EXIT, 3, 2, 1, 1);

        pushButton_OK = new QPushButton(study_plan);
        pushButton_OK->setObjectName(QString::fromUtf8("pushButton_OK"));

        gridLayout->addWidget(pushButton_OK, 3, 1, 1, 1);


        retranslateUi(study_plan);

        QMetaObject::connectSlotsByName(study_plan);
    } // setupUi

    void retranslateUi(QDialog *study_plan)
    {
        study_plan->setWindowTitle(QApplication::translate("study_plan", "\345\255\246\344\271\240\350\256\241\345\210\222\345\256\232\345\210\266", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("study_plan", "\345\255\246\344\271\240\350\256\241\345\210\222\345\272\223", 0, QApplication::UnicodeUTF8));
        pushButtonAdd->setText(QApplication::translate("study_plan", "\346\267\273\345\212\240\350\256\241\345\210\222", 0, QApplication::UnicodeUTF8));
        pushButtonModify->setText(QApplication::translate("study_plan", "\344\277\256\346\224\271\350\256\241\345\210\222", 0, QApplication::UnicodeUTF8));
        pushButtonDel->setText(QApplication::translate("study_plan", "\345\210\240\351\231\244\350\256\241\345\210\222", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("study_plan", "\345\261\236\346\200\247\347\274\226\350\276\221", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("study_plan", "\350\256\241\345\210\222\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("study_plan", "\346\211\200\345\220\253\346\226\207\346\241\243\357\274\232", 0, QApplication::UnicodeUTF8));
        comboBoxDoc->clear();
        comboBoxDoc->insertItems(0, QStringList()
         << QApplication::translate("study_plan", "\350\257\267\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8)
        );
        label_6->setText(QApplication::translate("study_plan", "\351\200\202\347\224\250\345\267\245\347\247\215\357\274\232", 0, QApplication::UnicodeUTF8));
        comboBox_usertype->clear();
        comboBox_usertype->insertItems(0, QStringList()
         << QApplication::translate("study_plan", "\350\257\267\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("study_plan", "\346\226\260\345\221\230\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("study_plan", "\345\210\235\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("study_plan", "\344\270\255\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("study_plan", "\351\253\230\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("study_plan", "\346\212\200\345\270\210", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("study_plan", "\351\253\230\347\272\247\346\212\200\345\270\210", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("study_plan", "\345\255\246\344\271\240\346\227\266\351\225\277\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("study_plan", "\345\260\217\346\227\266", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("study_plan", "\346\211\200\350\216\267\345\255\246\345\210\206\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("study_plan", "\345\255\246\345\210\206", 0, QApplication::UnicodeUTF8));
        pushButton_EXIT->setText(QApplication::translate("study_plan", "\351\200\200      \345\207\272", 0, QApplication::UnicodeUTF8));
        pushButton_OK->setText(QApplication::translate("study_plan", "\345\272\224      \347\224\250", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(study_plan);
    } // retranslateUi

};

namespace Ui {
    class study_plan: public Ui_study_plan {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STUDY_PLAN_H
