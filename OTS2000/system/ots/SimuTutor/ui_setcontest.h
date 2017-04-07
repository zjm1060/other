/********************************************************************************
** Form generated from reading ui file 'setcontest.ui'
**
** Created: Thu Jan 15 10:26:46 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SETCONTEST_H
#define UI_SETCONTEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CSetContestTable
{
public:
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QListWidget *listUserAndGroups;
    QGroupBox *groupBoxTask;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QGridLayout *gridLayout;
    QLineEdit *editContestFilter;
    QPushButton *buttonContestFilter;
    QPushButton *buttonContestShowAll;
    QListView *listContest;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label;
    QSpinBox *spinTimeLimit;
    QPushButton *buttonAddSend;
    QListView *listContestToStart;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButtonStart;
    QPushButton *pushButtonDel;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_exit;

    void setupUi(QWidget *CSetContestTable)
    {
        if (CSetContestTable->objectName().isEmpty())
            CSetContestTable->setObjectName(QString::fromUtf8("CSetContestTable"));
        CSetContestTable->resize(858, 608);
        CSetContestTable->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        gridLayout_4 = new QGridLayout(CSetContestTable);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(CSetContestTable);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMaximumSize(QSize(300, 16777215));
        groupBox->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        listUserAndGroups = new QListWidget(groupBox);
        listUserAndGroups->setObjectName(QString::fromUtf8("listUserAndGroups"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listUserAndGroups->sizePolicy().hasHeightForWidth());
        listUserAndGroups->setSizePolicy(sizePolicy);
        listUserAndGroups->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        gridLayout_3->addWidget(listUserAndGroups, 0, 0, 1, 1);


        horizontalLayout->addWidget(groupBox);

        groupBoxTask = new QGroupBox(CSetContestTable);
        groupBoxTask->setObjectName(QString::fromUtf8("groupBoxTask"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBoxTask->sizePolicy().hasHeightForWidth());
        groupBoxTask->setSizePolicy(sizePolicy1);
        groupBoxTask->setMinimumSize(QSize(500, 0));
        groupBoxTask->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));
        groupBoxTask->setCheckable(false);
        gridLayout_2 = new QGridLayout(groupBoxTask);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        editContestFilter = new QLineEdit(groupBoxTask);
        editContestFilter->setObjectName(QString::fromUtf8("editContestFilter"));
        editContestFilter->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(editContestFilter, 0, 0, 1, 2);

        buttonContestFilter = new QPushButton(groupBoxTask);
        buttonContestFilter->setObjectName(QString::fromUtf8("buttonContestFilter"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(buttonContestFilter->sizePolicy().hasHeightForWidth());
        buttonContestFilter->setSizePolicy(sizePolicy2);
        buttonContestFilter->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(buttonContestFilter, 0, 2, 1, 1);


        horizontalLayout_5->addLayout(gridLayout);

        buttonContestShowAll = new QPushButton(groupBoxTask);
        buttonContestShowAll->setObjectName(QString::fromUtf8("buttonContestShowAll"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(buttonContestShowAll->sizePolicy().hasHeightForWidth());
        buttonContestShowAll->setSizePolicy(sizePolicy3);

        horizontalLayout_5->addWidget(buttonContestShowAll);


        verticalLayout_2->addLayout(horizontalLayout_5);

        listContest = new QListView(groupBoxTask);
        listContest->setObjectName(QString::fromUtf8("listContest"));
        listContest->setMinimumSize(QSize(0, 200));
        listContest->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout_2->addWidget(listContest);


        gridLayout_2->addLayout(verticalLayout_2, 0, 0, 1, 1);


        horizontalLayout->addWidget(groupBoxTask);


        gridLayout_4->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label = new QLabel(CSetContestTable);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_9->addWidget(label);

        spinTimeLimit = new QSpinBox(CSetContestTable);
        spinTimeLimit->setObjectName(QString::fromUtf8("spinTimeLimit"));
        spinTimeLimit->setCorrectionMode(QAbstractSpinBox::CorrectToPreviousValue);
        spinTimeLimit->setMaximum(1000);

        horizontalLayout_9->addWidget(spinTimeLimit);


        horizontalLayout_2->addLayout(horizontalLayout_9);

        buttonAddSend = new QPushButton(CSetContestTable);
        buttonAddSend->setObjectName(QString::fromUtf8("buttonAddSend"));
        QSizePolicy sizePolicy4(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(buttonAddSend->sizePolicy().hasHeightForWidth());
        buttonAddSend->setSizePolicy(sizePolicy4);
        buttonAddSend->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(buttonAddSend);


        gridLayout_4->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        listContestToStart = new QListView(CSetContestTable);
        listContestToStart->setObjectName(QString::fromUtf8("listContestToStart"));

        gridLayout_4->addWidget(listContestToStart, 2, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pushButtonStart = new QPushButton(CSetContestTable);
        pushButtonStart->setObjectName(QString::fromUtf8("pushButtonStart"));

        horizontalLayout_3->addWidget(pushButtonStart);

        pushButtonDel = new QPushButton(CSetContestTable);
        pushButtonDel->setObjectName(QString::fromUtf8("pushButtonDel"));

        horizontalLayout_3->addWidget(pushButtonDel);


        horizontalLayout_4->addLayout(horizontalLayout_3);

        horizontalSpacer = new QSpacerItem(138, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        pushButton_exit = new QPushButton(CSetContestTable);
        pushButton_exit->setObjectName(QString::fromUtf8("pushButton_exit"));

        horizontalLayout_4->addWidget(pushButton_exit);


        gridLayout_4->addLayout(horizontalLayout_4, 3, 0, 1, 1);


        retranslateUi(CSetContestTable);

        QMetaObject::connectSlotsByName(CSetContestTable);
    } // setupUi

    void retranslateUi(QWidget *CSetContestTable)
    {
        CSetContestTable->setWindowTitle(QApplication::translate("CSetContestTable", "\347\253\236\350\265\233\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CSetContestTable", "\351\200\211\346\213\251\345\217\202\344\270\216\347\253\236\350\265\233\345\210\206\347\273\204", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        listUserAndGroups->setWhatsThis(QApplication::translate("CSetContestTable", "\345\275\223\345\211\215\347\231\273\345\275\225\347\232\204\345\255\246\345\221\230\347\273\204", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        groupBoxTask->setTitle(QApplication::translate("CSetContestTable", "\347\253\236\350\265\233\351\242\230\347\233\256", 0, QApplication::UnicodeUTF8));
        buttonContestFilter->setText(QApplication::translate("CSetContestTable", "\346\250\241\347\263\212\346\237\245\346\211\276", 0, QApplication::UnicodeUTF8));
        buttonContestShowAll->setText(QApplication::translate("CSetContestTable", "\346\230\276\347\244\272\345\205\250\351\203\250", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        listContest->setToolTip(QApplication::translate("CSetContestTable", "\345\275\223\345\211\215\345\274\200\346\224\276\344\270\255\347\232\204\347\253\236\350\265\233\351\242\230\347\233\256", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("CSetContestTable", "\350\256\276\347\275\256\347\253\236\350\265\233\345\256\214\346\210\220\346\227\266\351\227\264:", 0, QApplication::UnicodeUTF8));
        spinTimeLimit->setSuffix(QApplication::translate("CSetContestTable", "   \345\210\206\351\222\237", 0, QApplication::UnicodeUTF8));
        spinTimeLimit->setPrefix(QString());
        buttonAddSend->setText(QApplication::translate("CSetContestTable", " \345\212\240 \345\205\245 \345\276\205 \345\220\257 \345\212\250 \347\253\236 \350\265\233", 0, QApplication::UnicodeUTF8));
        pushButtonStart->setText(QApplication::translate("CSetContestTable", "\345\220\257 \345\212\250 \347\253\236 \350\265\233", 0, QApplication::UnicodeUTF8));
        pushButtonDel->setText(QApplication::translate("CSetContestTable", "\345\210\240 \351\231\244 \345\276\205 \345\220\257 \345\212\250 \347\253\236 \350\265\233", 0, QApplication::UnicodeUTF8));
        pushButton_exit->setText(QApplication::translate("CSetContestTable", "  \351\200\200   \345\207\272  ", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CSetContestTable);
    } // retranslateUi

};

namespace Ui {
    class CSetContestTable: public Ui_CSetContestTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETCONTEST_H
