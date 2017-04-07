/********************************************************************************
** Form generated from reading ui file 'faultdataadd.ui'
**
** Created: Thu Jan 15 10:26:00 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FAULTDATAADD_H
#define UI_FAULTDATAADD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CFaultDataAddTable
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editLogicalName;
    QPushButton *buttonPointName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *editType;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QDoubleSpinBox *editInitValue;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QDoubleSpinBox *editDstValue;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QDoubleSpinBox *editNormValue;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QSpinBox *editChgTime;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QLineEdit *editChgMode;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QSpinBox *editDelayTime;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *buttonApply;
    QPushButton *buttonEnter;
    QPushButton *buttonExit;

    void setupUi(QDialog *CFaultDataAddTable)
    {
        if (CFaultDataAddTable->objectName().isEmpty())
            CFaultDataAddTable->setObjectName(QString::fromUtf8("CFaultDataAddTable"));
        CFaultDataAddTable->resize(390, 465);
        CFaultDataAddTable->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        verticalLayout = new QVBoxLayout(CFaultDataAddTable);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CFaultDataAddTable);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label);

        editLogicalName = new QLineEdit(CFaultDataAddTable);
        editLogicalName->setObjectName(QString::fromUtf8("editLogicalName"));
        editLogicalName->setMinimumSize(QSize(160, 0));
        editLogicalName->setMaximumSize(QSize(160, 16777215));

        horizontalLayout->addWidget(editLogicalName);

        buttonPointName = new QPushButton(CFaultDataAddTable);
        buttonPointName->setObjectName(QString::fromUtf8("buttonPointName"));

        horizontalLayout->addWidget(buttonPointName);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(CFaultDataAddTable);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_2);

        editType = new QComboBox(CFaultDataAddTable);
        editType->setObjectName(QString::fromUtf8("editType"));
        editType->setMinimumSize(QSize(160, 0));
        editType->setMaximumSize(QSize(160, 16777215));

        horizontalLayout_2->addWidget(editType);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(CFaultDataAddTable);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_3);

        editInitValue = new QDoubleSpinBox(CFaultDataAddTable);
        editInitValue->setObjectName(QString::fromUtf8("editInitValue"));
        editInitValue->setMinimumSize(QSize(160, 0));
        editInitValue->setMaximumSize(QSize(160, 16777215));
        editInitValue->setDecimals(3);
        editInitValue->setMaximum(100000);

        horizontalLayout_3->addWidget(editInitValue);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(CFaultDataAddTable);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label_4);

        editDstValue = new QDoubleSpinBox(CFaultDataAddTable);
        editDstValue->setObjectName(QString::fromUtf8("editDstValue"));
        editDstValue->setMinimumSize(QSize(160, 0));
        editDstValue->setMaximumSize(QSize(160, 16777215));
        editDstValue->setDecimals(3);
        editDstValue->setMaximum(100000);

        horizontalLayout_4->addWidget(editDstValue);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(CFaultDataAddTable);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(label_5);

        editNormValue = new QDoubleSpinBox(CFaultDataAddTable);
        editNormValue->setObjectName(QString::fromUtf8("editNormValue"));
        editNormValue->setMinimumSize(QSize(160, 0));
        editNormValue->setMaximumSize(QSize(160, 16777215));
        editNormValue->setDecimals(3);
        editNormValue->setMaximum(100000);

        horizontalLayout_5->addWidget(editNormValue);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_6 = new QLabel(CFaultDataAddTable);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_6->addWidget(label_6);

        editChgTime = new QSpinBox(CFaultDataAddTable);
        editChgTime->setObjectName(QString::fromUtf8("editChgTime"));
        editChgTime->setMinimumSize(QSize(160, 0));
        editChgTime->setMaximumSize(QSize(160, 16777215));
        editChgTime->setMaximum(100000);

        horizontalLayout_6->addWidget(editChgTime);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_7 = new QLabel(CFaultDataAddTable);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_7->addWidget(label_7);

        editChgMode = new QLineEdit(CFaultDataAddTable);
        editChgMode->setObjectName(QString::fromUtf8("editChgMode"));
        editChgMode->setMinimumSize(QSize(160, 0));
        editChgMode->setMaximumSize(QSize(160, 16777215));

        horizontalLayout_7->addWidget(editChgMode);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_8 = new QLabel(CFaultDataAddTable);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_8->addWidget(label_8);

        editDelayTime = new QSpinBox(CFaultDataAddTable);
        editDelayTime->setObjectName(QString::fromUtf8("editDelayTime"));
        editDelayTime->setMinimumSize(QSize(160, 0));
        editDelayTime->setMaximumSize(QSize(160, 16777215));
        editDelayTime->setMaximum(1000);

        horizontalLayout_8->addWidget(editDelayTime);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(30);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        buttonApply = new QPushButton(CFaultDataAddTable);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_9->addWidget(buttonApply);

        buttonEnter = new QPushButton(CFaultDataAddTable);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_9->addWidget(buttonEnter);

        buttonExit = new QPushButton(CFaultDataAddTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_9->addWidget(buttonExit);


        verticalLayout->addLayout(horizontalLayout_9);


        retranslateUi(CFaultDataAddTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CFaultDataAddTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CFaultDataAddTable);
    } // setupUi

    void retranslateUi(QDialog *CFaultDataAddTable)
    {
        CFaultDataAddTable->setWindowTitle(QApplication::translate("CFaultDataAddTable", "\346\267\273\345\212\240\346\225\205\351\232\234\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CFaultDataAddTable", "\347\202\271\351\200\273\350\276\221\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonPointName->setText(QApplication::translate("CFaultDataAddTable", "\351\200\211 \346\213\251", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CFaultDataAddTable", "\346\225\205\351\232\234\347\261\273\345\236\213\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CFaultDataAddTable", "\345\210\235 \345\247\213 \345\200\274\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CFaultDataAddTable", "\347\233\256 \346\240\207 \345\200\274\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CFaultDataAddTable", "\346\255\243 \345\270\270 \345\200\274\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CFaultDataAddTable", "\345\217\230\345\214\226\346\227\266\351\227\264\357\274\232", 0, QApplication::UnicodeUTF8));
        editChgTime->setSuffix(QApplication::translate("CFaultDataAddTable", "  \347\247\222", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("CFaultDataAddTable", "\345\217\230\345\214\226\346\226\271\345\274\217\357\274\232", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("CFaultDataAddTable", "\345\273\266\350\277\237\346\227\266\351\227\264\357\274\232", 0, QApplication::UnicodeUTF8));
        editDelayTime->setSuffix(QApplication::translate("CFaultDataAddTable", "  \347\247\222", 0, QApplication::UnicodeUTF8));
        buttonApply->setText(QApplication::translate("CFaultDataAddTable", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("CFaultDataAddTable", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CFaultDataAddTable", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CFaultDataAddTable);
    } // retranslateUi

};

namespace Ui {
    class CFaultDataAddTable: public Ui_CFaultDataAddTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FAULTDATAADD_H
