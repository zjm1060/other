/********************************************************************************
** Form generated from reading ui file 'studentinfo.ui'
**
** Created: Wed Jan 14 10:22:08 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STUDENTINFO_H
#define UI_STUDENTINFO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CStudentInfoTable
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *comboFilterType;
    QLabel *label_3;
    QComboBox *comboFilter;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QLineEdit *editName;
    QPushButton *buttonSearch;
    QPushButton *buttonShowAll;
    QHBoxLayout *horizontalLayout_3;
    QTableView *viewUserInfo;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QPushButton *buttonRefresh;
    QPushButton *buttonAdd;
    QPushButton *buttonDel;
    QPushButton *buttonModify;
    QPushButton *buttonGroups;
    QPushButton *buttonExit;

    void setupUi(QDialog *CStudentInfoTable)
    {
        if (CStudentInfoTable->objectName().isEmpty())
            CStudentInfoTable->setObjectName(QString::fromUtf8("CStudentInfoTable"));
        CStudentInfoTable->resize(780, 510);
        verticalLayout_2 = new QVBoxLayout(CStudentInfoTable);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(CStudentInfoTable);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        comboFilterType = new QComboBox(CStudentInfoTable);
        comboFilterType->setObjectName(QString::fromUtf8("comboFilterType"));
        comboFilterType->setMinimumSize(QSize(100, 0));

        horizontalLayout_2->addWidget(comboFilterType);

        label_3 = new QLabel(CStudentInfoTable);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        comboFilter = new QComboBox(CStudentInfoTable);
        comboFilter->setObjectName(QString::fromUtf8("comboFilter"));
        comboFilter->setMinimumSize(QSize(150, 0));

        horizontalLayout_2->addWidget(comboFilter);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(CStudentInfoTable);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 0));

        horizontalLayout->addWidget(label);

        editName = new QLineEdit(CStudentInfoTable);
        editName->setObjectName(QString::fromUtf8("editName"));
        editName->setMaximumSize(QSize(150, 16777215));

        horizontalLayout->addWidget(editName);


        horizontalLayout_2->addLayout(horizontalLayout);

        buttonSearch = new QPushButton(CStudentInfoTable);
        buttonSearch->setObjectName(QString::fromUtf8("buttonSearch"));
        buttonSearch->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(buttonSearch);

        buttonShowAll = new QPushButton(CStudentInfoTable);
        buttonShowAll->setObjectName(QString::fromUtf8("buttonShowAll"));
        buttonShowAll->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(buttonShowAll);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        viewUserInfo = new QTableView(CStudentInfoTable);
        viewUserInfo->setObjectName(QString::fromUtf8("viewUserInfo"));
        viewUserInfo->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(viewUserInfo);

        frame = new QFrame(CStudentInfoTable);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(150, 0));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonRefresh = new QPushButton(frame);
        buttonRefresh->setObjectName(QString::fromUtf8("buttonRefresh"));
        buttonRefresh->setMinimumSize(QSize(0, 50));
        buttonRefresh->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonRefresh);

        buttonAdd = new QPushButton(frame);
        buttonAdd->setObjectName(QString::fromUtf8("buttonAdd"));
        buttonAdd->setMinimumSize(QSize(0, 50));
        buttonAdd->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonAdd);

        buttonDel = new QPushButton(frame);
        buttonDel->setObjectName(QString::fromUtf8("buttonDel"));
        buttonDel->setMinimumSize(QSize(0, 50));
        buttonDel->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonDel);

        buttonModify = new QPushButton(frame);
        buttonModify->setObjectName(QString::fromUtf8("buttonModify"));
        buttonModify->setMinimumSize(QSize(0, 50));
        buttonModify->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonModify);

        buttonGroups = new QPushButton(frame);
        buttonGroups->setObjectName(QString::fromUtf8("buttonGroups"));
        buttonGroups->setMinimumSize(QSize(0, 50));
        buttonGroups->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonGroups);

        buttonExit = new QPushButton(frame);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setMinimumSize(QSize(0, 50));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonExit);


        horizontalLayout_3->addWidget(frame);


        verticalLayout_2->addLayout(horizontalLayout_3);

        QWidget::setTabOrder(buttonSearch, buttonRefresh);
        QWidget::setTabOrder(buttonRefresh, buttonAdd);
        QWidget::setTabOrder(buttonAdd, buttonDel);
        QWidget::setTabOrder(buttonDel, buttonModify);
        QWidget::setTabOrder(buttonModify, buttonGroups);

        retranslateUi(CStudentInfoTable);

        QMetaObject::connectSlotsByName(CStudentInfoTable);
    } // setupUi

    void retranslateUi(QDialog *CStudentInfoTable)
    {
        CStudentInfoTable->setWindowTitle(QApplication::translate("CStudentInfoTable", "\345\255\246\345\221\230\347\224\250\346\210\267\344\277\241\346\201\257\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CStudentInfoTable", "\347\255\233\351\200\211\347\261\273\345\236\213\357\274\232", 0, QApplication::UnicodeUTF8));
        comboFilterType->clear();
        comboFilterType->insertItems(0, QStringList()
         << QApplication::translate("CStudentInfoTable", "\350\247\222\350\211\262", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoTable", "\346\200\247\345\210\253", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoTable", "\347\273\204\345\220\215", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CStudentInfoTable", "\345\267\245\347\247\215", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("CStudentInfoTable", "\347\255\233\351\200\211\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CStudentInfoTable", "\345\247\223\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonSearch->setText(QApplication::translate("CStudentInfoTable", "\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
        buttonShowAll->setText(QApplication::translate("CStudentInfoTable", "\346\230\276\347\244\272\345\205\250\351\203\250", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QApplication::translate("CStudentInfoTable", "\345\210\267 \346\226\260", 0, QApplication::UnicodeUTF8));
        buttonAdd->setText(QApplication::translate("CStudentInfoTable", "\346\267\273 \345\212\240", 0, QApplication::UnicodeUTF8));
        buttonDel->setText(QApplication::translate("CStudentInfoTable", "\345\210\240 \351\231\244", 0, QApplication::UnicodeUTF8));
        buttonModify->setText(QApplication::translate("CStudentInfoTable", "\344\277\256 \346\224\271", 0, QApplication::UnicodeUTF8));
        buttonGroups->setText(QApplication::translate("CStudentInfoTable", "\345\210\206\347\273\204\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CStudentInfoTable", "\351\200\200 \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CStudentInfoTable);
    } // retranslateUi

};

namespace Ui {
    class CStudentInfoTable: public Ui_CStudentInfoTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STUDENTINFO_H
