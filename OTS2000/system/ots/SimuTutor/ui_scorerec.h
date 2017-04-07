/********************************************************************************
** Form generated from reading ui file 'scorerec.ui'
**
** Created: Thu Jan 15 10:26:22 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SCOREREC_H
#define UI_SCOREREC_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CScoreRec
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboMode;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QLabel *labelContent;
    QComboBox *comboContent;
    QPushButton *buttonSearch;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QDateTimeEdit *dateTimeEditStart;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QDateTimeEdit *dateTimeEditEnd;
    QPushButton *buttonSearchTaday;
    QPushButton *buttonExport;
    QTableView *listScoreRec;
    QPushButton *pushButton;

    void setupUi(QWidget *CScoreRec)
    {
        if (CScoreRec->objectName().isEmpty())
            CScoreRec->setObjectName(QString::fromUtf8("CScoreRec"));
        CScoreRec->resize(774, 544);
        verticalLayout_3 = new QVBoxLayout(CScoreRec);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox = new QGroupBox(CScoreRec);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8("border-color: rgb(255, 3, 15);"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(70, 0));
        label->setMaximumSize(QSize(70, 16777215));
        label->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";\n"
"color: rgb(0, 0, 255);"));

        horizontalLayout->addWidget(label);

        comboMode = new QComboBox(groupBox);
        comboMode->setObjectName(QString::fromUtf8("comboMode"));
        comboMode->setMinimumSize(QSize(100, 0));
        comboMode->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(comboMode);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        labelContent = new QLabel(groupBox);
        labelContent->setObjectName(QString::fromUtf8("labelContent"));
        labelContent->setMinimumSize(QSize(60, 0));
        labelContent->setMaximumSize(QSize(60, 16777215));
        labelContent->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";\n"
"color: rgb(0, 0, 255);"));

        horizontalLayout_3->addWidget(labelContent);

        comboContent = new QComboBox(groupBox);
        comboContent->setObjectName(QString::fromUtf8("comboContent"));
        comboContent->setMinimumSize(QSize(220, 0));
        comboContent->setEditable(false);

        horizontalLayout_3->addWidget(comboContent);


        gridLayout->addLayout(horizontalLayout_3, 0, 1, 1, 2);

        buttonSearch = new QPushButton(groupBox);
        buttonSearch->setObjectName(QString::fromUtf8("buttonSearch"));
        buttonSearch->setMinimumSize(QSize(0, 60));
        buttonSearch->setMaximumSize(QSize(80, 16777215));
        buttonSearch->setStyleSheet(QString::fromUtf8("font: 75 10pt \"\345\256\213\344\275\223\";"));

        gridLayout->addWidget(buttonSearch, 0, 4, 2, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(70, 0));
        label_2->setMaximumSize(QSize(70, 16777215));
        label_2->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";\n"
"color: rgb(0, 0, 255);"));

        horizontalLayout_2->addWidget(label_2);

        dateTimeEditStart = new QDateTimeEdit(groupBox);
        dateTimeEditStart->setObjectName(QString::fromUtf8("dateTimeEditStart"));
        dateTimeEditStart->setCalendarPopup(true);

        horizontalLayout_2->addWidget(dateTimeEditStart);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(70, 0));
        label_3->setMaximumSize(QSize(70, 16777215));
        label_3->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";\n"
"color: rgb(0, 0, 255);"));

        horizontalLayout_4->addWidget(label_3);

        dateTimeEditEnd = new QDateTimeEdit(groupBox);
        dateTimeEditEnd->setObjectName(QString::fromUtf8("dateTimeEditEnd"));
        dateTimeEditEnd->setCalendarPopup(true);

        horizontalLayout_4->addWidget(dateTimeEditEnd);


        gridLayout->addLayout(horizontalLayout_4, 1, 2, 1, 1);

        buttonSearchTaday = new QPushButton(groupBox);
        buttonSearchTaday->setObjectName(QString::fromUtf8("buttonSearchTaday"));
        buttonSearchTaday->setMinimumSize(QSize(0, 60));
        buttonSearchTaday->setMaximumSize(QSize(80, 16777215));
        buttonSearchTaday->setStyleSheet(QString::fromUtf8("font: 75 10pt \"\345\256\213\344\275\223\";"));

        gridLayout->addWidget(buttonSearchTaday, 0, 5, 2, 1);

        buttonExport = new QPushButton(groupBox);
        buttonExport->setObjectName(QString::fromUtf8("buttonExport"));
        buttonExport->setMinimumSize(QSize(0, 60));
        buttonExport->setMaximumSize(QSize(80, 16777215));
        buttonExport->setStyleSheet(QString::fromUtf8("font: 75 10pt \"\345\256\213\344\275\223\";"));

        gridLayout->addWidget(buttonExport, 0, 6, 2, 1);


        verticalLayout_3->addWidget(groupBox);

        listScoreRec = new QTableView(CScoreRec);
        listScoreRec->setObjectName(QString::fromUtf8("listScoreRec"));
        listScoreRec->setSortingEnabled(true);
        listScoreRec->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_3->addWidget(listScoreRec);

        pushButton = new QPushButton(CScoreRec);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(0, 30));
        pushButton->setContextMenuPolicy(Qt::DefaultContextMenu);
        pushButton->setStyleSheet(QString::fromUtf8("font: 75 16pt \"\345\256\213\344\275\223\";\n"
"background-color: rgb(167, 255, 192);\n"
"color: rgb(0, 141, 0);"));

        verticalLayout_3->addWidget(pushButton);


        retranslateUi(CScoreRec);

        QMetaObject::connectSlotsByName(CScoreRec);
    } // setupUi

    void retranslateUi(QWidget *CScoreRec)
    {
        CScoreRec->setWindowTitle(QApplication::translate("CScoreRec", "\346\210\220\347\273\251\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        label->setText(QApplication::translate("CScoreRec", "\346\237\245\350\257\242\346\226\271\345\274\217\357\274\232", 0, QApplication::UnicodeUTF8));
        labelContent->setText(QApplication::translate("CScoreRec", "\347\224\250\346\210\267\345\220\215:", 0, QApplication::UnicodeUTF8));
        buttonSearch->setText(QApplication::translate("CScoreRec", "\346\237\245  \350\257\242", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CScoreRec", "\345\274\200\345\247\213\346\227\266\351\227\264\357\274\232", 0, QApplication::UnicodeUTF8));
        dateTimeEditStart->setDisplayFormat(QApplication::translate("CScoreRec", "yyyy-MM-dd HH:mm:ss", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CScoreRec", "\347\273\223\346\235\237\346\227\266\351\227\264\357\274\232", 0, QApplication::UnicodeUTF8));
        dateTimeEditEnd->setDisplayFormat(QApplication::translate("CScoreRec", "yyyy-MM-dd HH:mm:ss", 0, QApplication::UnicodeUTF8));
        buttonSearchTaday->setText(QApplication::translate("CScoreRec", "\346\237\245\350\257\242\345\275\223\345\244\251", 0, QApplication::UnicodeUTF8));
        buttonExport->setText(QApplication::translate("CScoreRec", "\345\257\274\345\207\272EXCEL", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("CScoreRec", "\346\225\231  \345\221\230  \350\257\204  \345\210\206  \346\217\220  \344\272\244", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CScoreRec);
    } // retranslateUi

};

namespace Ui {
    class CScoreRec: public Ui_CScoreRec {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCOREREC_H
