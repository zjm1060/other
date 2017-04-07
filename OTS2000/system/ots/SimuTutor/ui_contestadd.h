/********************************************************************************
** Form generated from reading ui file 'contestadd.ui'
**
** Created: Thu Jan 15 10:25:36 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CONTESTADD_H
#define UI_CONTESTADD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CContestAddTable
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editName;
    QFormLayout *formLayout;
    QLabel *label_3;
    QComboBox *comboBox_Open;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *editTaskName;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QComboBox *editScene;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_8;
    QComboBox *editFault;
    QSpacerItem *verticalSpacer_4;
    QLabel *label_7;
    QTextEdit *editRemark;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_11;
    QPushButton *buttonApply;
    QPushButton *buttonEnter;
    QPushButton *buttonExit;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *CContestAddTable)
    {
        if (CContestAddTable->objectName().isEmpty())
            CContestAddTable->setObjectName(QString::fromUtf8("CContestAddTable"));
        CContestAddTable->resize(638, 561);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CContestAddTable->sizePolicy().hasHeightForWidth());
        CContestAddTable->setSizePolicy(sizePolicy);
        CContestAddTable->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        gridLayout = new QGridLayout(CContestAddTable);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalSpacer = new QSpacerItem(617, 25, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 0, 0, 1, 2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CContestAddTable);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(100, 0));
        label->setMaximumSize(QSize(100, 16777215));
        label->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label);

        editName = new QLineEdit(CContestAddTable);
        editName->setObjectName(QString::fromUtf8("editName"));
        sizePolicy.setHeightForWidth(editName->sizePolicy().hasHeightForWidth());
        editName->setSizePolicy(sizePolicy);
        editName->setMinimumSize(QSize(400, 0));
        editName->setMaximumSize(QSize(600, 16777215));

        horizontalLayout->addWidget(editName);


        verticalLayout->addLayout(horizontalLayout);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_3 = new QLabel(CContestAddTable);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_3);

        comboBox_Open = new QComboBox(CContestAddTable);
        comboBox_Open->setObjectName(QString::fromUtf8("comboBox_Open"));
        comboBox_Open->setMinimumSize(QSize(400, 0));

        formLayout->setWidget(0, QFormLayout::FieldRole, comboBox_Open);


        verticalLayout->addLayout(formLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(CContestAddTable);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(0, 0));
        label_2->setMaximumSize(QSize(100, 16777215));
        label_2->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_2);

        editTaskName = new QComboBox(CContestAddTable);
        editTaskName->setObjectName(QString::fromUtf8("editTaskName"));
        sizePolicy.setHeightForWidth(editTaskName->sizePolicy().hasHeightForWidth());
        editTaskName->setSizePolicy(sizePolicy);
        editTaskName->setMinimumSize(QSize(400, 0));
        editTaskName->setMaximumSize(QSize(600, 16777215));

        horizontalLayout_2->addWidget(editTaskName);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(CContestAddTable);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMaximumSize(QSize(100, 16777215));
        label_4->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_4);

        editScene = new QComboBox(CContestAddTable);
        editScene->setObjectName(QString::fromUtf8("editScene"));
        sizePolicy.setHeightForWidth(editScene->sizePolicy().hasHeightForWidth());
        editScene->setSizePolicy(sizePolicy);
        editScene->setMinimumSize(QSize(400, 0));
        editScene->setMaximumSize(QSize(600, 16777215));

        horizontalLayout_3->addWidget(editScene);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_8 = new QLabel(CContestAddTable);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMaximumSize(QSize(100, 16777215));
        label_8->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label_8);

        editFault = new QComboBox(CContestAddTable);
        editFault->setObjectName(QString::fromUtf8("editFault"));
        editFault->setMinimumSize(QSize(400, 0));
        editFault->setMaximumSize(QSize(600, 16777215));

        horizontalLayout_4->addWidget(editFault);


        verticalLayout->addLayout(horizontalLayout_4);

        verticalSpacer_4 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        label_7 = new QLabel(CContestAddTable);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy1);
        label_7->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(label_7);

        editRemark = new QTextEdit(CContestAddTable);
        editRemark->setObjectName(QString::fromUtf8("editRemark"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(editRemark->sizePolicy().hasHeightForWidth());
        editRemark->setSizePolicy(sizePolicy2);
        editRemark->setMaximumSize(QSize(16777215, 16777215));
        editRemark->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(editRemark);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 2);

        verticalSpacer_3 = new QSpacerItem(617, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 2, 0, 1, 2);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        buttonApply = new QPushButton(CContestAddTable);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_11->addWidget(buttonApply);

        buttonEnter = new QPushButton(CContestAddTable);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_11->addWidget(buttonEnter);

        buttonExit = new QPushButton(CContestAddTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_11->addWidget(buttonExit);


        gridLayout->addLayout(horizontalLayout_11, 3, 0, 1, 2);

        verticalSpacer_2 = new QSpacerItem(20, 25, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 4, 0, 1, 2);

        QWidget::setTabOrder(editTaskName, editRemark);
        QWidget::setTabOrder(editRemark, buttonExit);

        retranslateUi(CContestAddTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CContestAddTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CContestAddTable);
    } // setupUi

    void retranslateUi(QDialog *CContestAddTable)
    {
        CContestAddTable->setWindowTitle(QApplication::translate("CContestAddTable", "\346\267\273\345\212\240\347\253\236\350\265\233", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CContestAddTable", "\347\253\236\350\265\233\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CContestAddTable", "\347\253\236\350\265\233\347\212\266\346\200\201\357\274\232", 0, QApplication::UnicodeUTF8));
        comboBox_Open->clear();
        comboBox_Open->insertItems(0, QStringList()
         << QApplication::translate("CContestAddTable", "\346\232\202\344\270\215\345\274\200\346\224\276", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CContestAddTable", "\345\257\271\345\244\226\345\274\200\346\224\276", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("CContestAddTable", "\344\273\273\345\212\241\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        editTaskName->setToolTip(QApplication::translate("CContestAddTable", "\345\217\257\351\200\211\344\273\273\345\212\241\345\235\207\344\270\272\350\200\203\350\257\225\344\270\223\347\224\250\351\242\230\357\274\214\345\217\257\345\255\246\344\271\240\347\232\204\344\273\273\345\212\241\344\270\215\345\217\257\346\267\273\345\212\240\350\207\263\347\253\236\350\265\233\344\270\255", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_4->setText(QApplication::translate("CContestAddTable", "\345\267\245\345\206\265\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("CContestAddTable", "\346\225\205\351\232\234\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("CContestAddTable", "\347\253\236\350\265\233\345\244\207\346\263\250\357\274\232", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonApply->setToolTip(QApplication::translate("CContestAddTable", "\350\257\267\345\205\210\344\277\235\345\255\230\345\237\272\346\234\254\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        buttonApply->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        buttonApply->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        buttonApply->setText(QApplication::translate("CContestAddTable", "\345\272\224  \347\224\250", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonEnter->setToolTip(QApplication::translate("CContestAddTable", "\350\257\267\345\205\210\344\277\235\345\255\230\345\237\272\346\234\254\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        buttonEnter->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        buttonEnter->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        buttonEnter->setText(QApplication::translate("CContestAddTable", "\347\241\256  \345\256\232", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CContestAddTable", "\345\217\226  \346\266\210", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CContestAddTable);
    } // retranslateUi

};

namespace Ui {
    class CContestAddTable: public Ui_CContestAddTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTESTADD_H
