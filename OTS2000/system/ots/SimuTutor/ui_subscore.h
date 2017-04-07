/********************************************************************************
** Form generated from reading ui file 'subscore.ui'
**
** Created: Thu Jan 15 10:25:02 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SUBSCORE_H
#define UI_SUBSCORE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SubScore
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_username;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QLabel *label_TotalSc;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_3;
    QLabel *label_taskname;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_6;
    QLabel *label_TutorSc;
    QSpacerItem *horizontalSpacer_4;
    QTableView *tableView;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *SubScore)
    {
        if (SubScore->objectName().isEmpty())
            SubScore->setObjectName(QString::fromUtf8("SubScore"));
        SubScore->resize(711, 506);
        SubScore->setStyleSheet(QString::fromUtf8("font: 11pt \"\346\226\260\345\256\213\344\275\223\";"));
        gridLayout_2 = new QGridLayout(SubScore);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(SubScore);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_username = new QLabel(SubScore);
        label_username->setObjectName(QString::fromUtf8("label_username"));

        gridLayout->addWidget(label_username, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        label_2 = new QLabel(SubScore);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 3, 1, 1);

        label_TotalSc = new QLabel(SubScore);
        label_TotalSc->setObjectName(QString::fromUtf8("label_TotalSc"));
        label_TotalSc->setStyleSheet(QString::fromUtf8("font: 75 14pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(85, 0, 255);"));

        gridLayout->addWidget(label_TotalSc, 0, 4, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 5, 1, 1);

        label_3 = new QLabel(SubScore);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label_taskname = new QLabel(SubScore);
        label_taskname->setObjectName(QString::fromUtf8("label_taskname"));

        gridLayout->addWidget(label_taskname, 1, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 1, 2, 1, 1);

        label_6 = new QLabel(SubScore);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 1, 3, 1, 1);

        label_TutorSc = new QLabel(SubScore);
        label_TutorSc->setObjectName(QString::fromUtf8("label_TutorSc"));
        label_TutorSc->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 255);\n"
"font: 75 14pt \"\346\226\260\345\256\213\344\275\223\";"));

        gridLayout->addWidget(label_TutorSc, 1, 4, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 1, 5, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        tableView = new QTableView(SubScore);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setSortingEnabled(true);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->setStretchLastSection(false);

        gridLayout_2->addWidget(tableView, 1, 0, 1, 1);

        buttonBox = new QDialogButtonBox(SubScore);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Close|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        gridLayout_2->addWidget(buttonBox, 2, 0, 1, 1);


        retranslateUi(SubScore);

        QMetaObject::connectSlotsByName(SubScore);
    } // setupUi

    void retranslateUi(QWidget *SubScore)
    {
        SubScore->setWindowTitle(QApplication::translate("SubScore", "\345\276\227\345\210\206\350\257\246\346\203\205", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SubScore", "\347\224\250\346\210\267\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_username->setText(QApplication::translate("SubScore", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SubScore", "\346\200\273    \345\210\206\357\274\232", 0, QApplication::UnicodeUTF8));
        label_TotalSc->setText(QApplication::translate("SubScore", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SubScore", "\344\273\273\345\212\241\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_taskname->setText(QApplication::translate("SubScore", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("SubScore", "\346\225\231\345\221\230\347\273\231\345\210\206\357\274\232", 0, QApplication::UnicodeUTF8));
        label_TutorSc->setText(QApplication::translate("SubScore", "TextLabel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SubScore);
    } // retranslateUi

};

namespace Ui {
    class SubScore: public Ui_SubScore {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBSCORE_H
