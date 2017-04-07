/********************************************************************************
** Form generated from reading ui file 'subject_edit.ui'
**
** Created: Thu Jan 15 10:24:54 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SUBJECT_EDIT_H
#define UI_SUBJECT_EDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CSubjectEdit
{
public:
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_9;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_10;
    QLineEdit *editSubjectName;
    QLabel *label_12;
    QLineEdit *editTaskName;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_10;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_9;
    QSpinBox *editScore;
    QLabel *label;
    QSpinBox *editTime;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_11;
    QLineEdit *editStepTip;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_2;
    QTextEdit *editCondition;
    QVBoxLayout *verticalLayout;
    QPushButton *buttonEditCondition;
    QPushButton *buttonClearCondition;
    QGroupBox *groupBoxPreCondition;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_3;
    QTextEdit *editPreCondition;
    QVBoxLayout *verticalLayout_2;
    QPushButton *buttonEditPreCondition;
    QPushButton *buttonClearPreCondition;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonSave;
    QPushButton *buttonCancel;

    void setupUi(QDialog *CSubjectEdit)
    {
        if (CSubjectEdit->objectName().isEmpty())
            CSubjectEdit->setObjectName(QString::fromUtf8("CSubjectEdit"));
        CSubjectEdit->resize(793, 412);
        CSubjectEdit->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        verticalLayout_5 = new QVBoxLayout(CSubjectEdit);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_10 = new QLabel(CSubjectEdit);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_2->addWidget(label_10);

        editSubjectName = new QLineEdit(CSubjectEdit);
        editSubjectName->setObjectName(QString::fromUtf8("editSubjectName"));
        editSubjectName->setMinimumSize(QSize(220, 0));

        horizontalLayout_2->addWidget(editSubjectName);

        label_12 = new QLabel(CSubjectEdit);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_2->addWidget(label_12);

        editTaskName = new QLineEdit(CSubjectEdit);
        editTaskName->setObjectName(QString::fromUtf8("editTaskName"));
        editTaskName->setMinimumSize(QSize(0, 0));

        horizontalLayout_2->addWidget(editTaskName);


        horizontalLayout_9->addLayout(horizontalLayout_2);


        verticalLayout_5->addLayout(horizontalLayout_9);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));

        verticalLayout_5->addLayout(horizontalLayout_3);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_9 = new QLabel(CSubjectEdit);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_4->addWidget(label_9);

        editScore = new QSpinBox(CSubjectEdit);
        editScore->setObjectName(QString::fromUtf8("editScore"));
        editScore->setMinimumSize(QSize(50, 0));
        editScore->setMaximum(100);

        horizontalLayout_4->addWidget(editScore);


        horizontalLayout_10->addLayout(horizontalLayout_4);

        label = new QLabel(CSubjectEdit);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_10->addWidget(label);

        editTime = new QSpinBox(CSubjectEdit);
        editTime->setObjectName(QString::fromUtf8("editTime"));
        editTime->setMinimumSize(QSize(50, 0));

        horizontalLayout_10->addWidget(editTime);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_11 = new QLabel(CSubjectEdit);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_5->addWidget(label_11);

        editStepTip = new QLineEdit(CSubjectEdit);
        editStepTip->setObjectName(QString::fromUtf8("editStepTip"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(editStepTip->sizePolicy().hasHeightForWidth());
        editStepTip->setSizePolicy(sizePolicy);
        editStepTip->setMinimumSize(QSize(340, 0));

        horizontalLayout_5->addWidget(editStepTip);


        horizontalLayout_10->addLayout(horizontalLayout_5);


        verticalLayout_5->addLayout(horizontalLayout_10);

        groupBox = new QGroupBox(CSubjectEdit);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, -1, 0, -1);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_6->addWidget(label_2);

        editCondition = new QTextEdit(groupBox);
        editCondition->setObjectName(QString::fromUtf8("editCondition"));

        horizontalLayout_6->addWidget(editCondition);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonEditCondition = new QPushButton(groupBox);
        buttonEditCondition->setObjectName(QString::fromUtf8("buttonEditCondition"));
        buttonEditCondition->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout->addWidget(buttonEditCondition);

        buttonClearCondition = new QPushButton(groupBox);
        buttonClearCondition->setObjectName(QString::fromUtf8("buttonClearCondition"));
        buttonClearCondition->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout->addWidget(buttonClearCondition);


        horizontalLayout_6->addLayout(verticalLayout);


        verticalLayout_3->addLayout(horizontalLayout_6);


        verticalLayout_5->addWidget(groupBox);

        groupBoxPreCondition = new QGroupBox(CSubjectEdit);
        groupBoxPreCondition->setObjectName(QString::fromUtf8("groupBoxPreCondition"));
        groupBoxPreCondition->setCheckable(true);
        verticalLayout_4 = new QVBoxLayout(groupBoxPreCondition);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, -1, 0, -1);
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_3 = new QLabel(groupBoxPreCondition);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_7->addWidget(label_3);

        editPreCondition = new QTextEdit(groupBoxPreCondition);
        editPreCondition->setObjectName(QString::fromUtf8("editPreCondition"));

        horizontalLayout_7->addWidget(editPreCondition);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        buttonEditPreCondition = new QPushButton(groupBoxPreCondition);
        buttonEditPreCondition->setObjectName(QString::fromUtf8("buttonEditPreCondition"));
        buttonEditPreCondition->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_2->addWidget(buttonEditPreCondition);

        buttonClearPreCondition = new QPushButton(groupBoxPreCondition);
        buttonClearPreCondition->setObjectName(QString::fromUtf8("buttonClearPreCondition"));
        buttonClearPreCondition->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_2->addWidget(buttonClearPreCondition);


        horizontalLayout_7->addLayout(verticalLayout_2);


        verticalLayout_4->addLayout(horizontalLayout_7);


        verticalLayout_5->addWidget(groupBoxPreCondition);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);

        buttonSave = new QPushButton(CSubjectEdit);
        buttonSave->setObjectName(QString::fromUtf8("buttonSave"));

        horizontalLayout_8->addWidget(buttonSave);

        buttonCancel = new QPushButton(CSubjectEdit);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));

        horizontalLayout_8->addWidget(buttonCancel);


        verticalLayout_5->addLayout(horizontalLayout_8);


        retranslateUi(CSubjectEdit);

        QMetaObject::connectSlotsByName(CSubjectEdit);
    } // setupUi

    void retranslateUi(QDialog *CSubjectEdit)
    {
        CSubjectEdit->setWindowTitle(QApplication::translate("CSubjectEdit", "\351\242\230\347\233\256\347\274\226\350\276\221", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("CSubjectEdit", "\351\242\230\347\233\256\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("CSubjectEdit", "\346\211\200\345\261\236\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("CSubjectEdit", "\345\210\206    \345\200\274", 0, QApplication::UnicodeUTF8));
        editScore->setSuffix(QApplication::translate("CSubjectEdit", "\345\210\206", 0, QApplication::UnicodeUTF8));
        editScore->setPrefix(QString());
        label->setText(QApplication::translate("CSubjectEdit", "\346\227\266    \351\231\220", 0, QApplication::UnicodeUTF8));
        editTime->setSuffix(QApplication::translate("CSubjectEdit", "\347\247\222", 0, QApplication::UnicodeUTF8));
        editTime->setPrefix(QString());
        label_11->setText(QApplication::translate("CSubjectEdit", "\346\217\220\347\244\272\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        editStepTip->setText(QString());
        groupBox->setTitle(QString());
        label_2->setText(QApplication::translate("CSubjectEdit", "\345\275\223\345\211\215\347\272\246\346\235\237", 0, QApplication::UnicodeUTF8));
        buttonEditCondition->setText(QApplication::translate("CSubjectEdit", "\347\274\226\350\276\221", 0, QApplication::UnicodeUTF8));
        buttonClearCondition->setText(QApplication::translate("CSubjectEdit", "\346\270\205\351\231\244", 0, QApplication::UnicodeUTF8));
        groupBoxPreCondition->setTitle(QApplication::translate("CSubjectEdit", "\346\230\257\345\220\246\344\270\272\345\205\263\351\224\256\346\255\245\351\252\244", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CSubjectEdit", "\345\211\215\351\224\256\346\235\241\344\273\266", 0, QApplication::UnicodeUTF8));
        buttonEditPreCondition->setText(QApplication::translate("CSubjectEdit", "\347\274\226\350\276\221", 0, QApplication::UnicodeUTF8));
        buttonClearPreCondition->setText(QApplication::translate("CSubjectEdit", "\346\270\205\351\231\244", 0, QApplication::UnicodeUTF8));
        buttonSave->setText(QApplication::translate("CSubjectEdit", "\344\277\235   \345\255\230", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("CSubjectEdit", "\351\200\200   \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CSubjectEdit);
    } // retranslateUi

};

namespace Ui {
    class CSubjectEdit: public Ui_CSubjectEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBJECT_EDIT_H
