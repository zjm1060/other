/********************************************************************************
** Form generated from reading ui file 'study_task.ui'
**
** Created: Thu Jan 15 10:28:27 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STUDY_TASK_H
#define UI_STUDY_TASK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_study_list
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editName;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_11;
    QComboBox *editUsertype;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QComboBox *editStation;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QComboBox *editDev;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *editGrpName;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QComboBox *editType;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QComboBox *editScene;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_8;
    QComboBox *editFault;
    QVBoxLayout *verticalLayout;
    QLabel *label_7;
    QTextEdit *editRemark;

    void setupUi(QWidget *study_list)
    {
        if (study_list->objectName().isEmpty())
            study_list->setObjectName(QString::fromUtf8("study_list"));
        study_list->resize(346, 784);
        gridLayout = new QGridLayout(study_list);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(study_list);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(100, 0));
        label->setMaximumSize(QSize(100, 16777215));
        label->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label);

        editName = new QLineEdit(study_list);
        editName->setObjectName(QString::fromUtf8("editName"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(editName->sizePolicy().hasHeightForWidth());
        editName->setSizePolicy(sizePolicy);
        editName->setMinimumSize(QSize(210, 0));
        editName->setMaximumSize(QSize(210, 16777215));

        horizontalLayout->addWidget(editName);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_11 = new QLabel(study_list);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setMaximumSize(QSize(100, 16777215));
        label_11->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_13->addWidget(label_11);

        editUsertype = new QComboBox(study_list);
        editUsertype->setObjectName(QString::fromUtf8("editUsertype"));
        editUsertype->setMinimumSize(QSize(210, 0));
        editUsertype->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_13->addWidget(editUsertype);


        verticalLayout_2->addLayout(horizontalLayout_13);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_5 = new QLabel(study_list);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMaximumSize(QSize(100, 16777215));
        label_5->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_6->addWidget(label_5);

        editStation = new QComboBox(study_list);
        editStation->setObjectName(QString::fromUtf8("editStation"));
        editStation->setMinimumSize(QSize(210, 0));
        editStation->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_6->addWidget(editStation);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_6 = new QLabel(study_list);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMaximumSize(QSize(100, 16777215));
        label_6->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_7->addWidget(label_6);

        editDev = new QComboBox(study_list);
        editDev->setObjectName(QString::fromUtf8("editDev"));
        sizePolicy.setHeightForWidth(editDev->sizePolicy().hasHeightForWidth());
        editDev->setSizePolicy(sizePolicy);
        editDev->setMinimumSize(QSize(210, 0));
        editDev->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_7->addWidget(editDev);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(study_list);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(0, 0));
        label_2->setMaximumSize(QSize(100, 16777215));
        label_2->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_2);

        editGrpName = new QComboBox(study_list);
        editGrpName->setObjectName(QString::fromUtf8("editGrpName"));
        sizePolicy.setHeightForWidth(editGrpName->sizePolicy().hasHeightForWidth());
        editGrpName->setSizePolicy(sizePolicy);
        editGrpName->setMinimumSize(QSize(210, 0));
        editGrpName->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_2->addWidget(editGrpName);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_3 = new QLabel(study_list);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(100, 16777215));
        label_3->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(label_3);

        editType = new QComboBox(study_list);
        editType->setObjectName(QString::fromUtf8("editType"));
        editType->setMinimumSize(QSize(210, 0));
        editType->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_5->addWidget(editType);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(study_list);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMaximumSize(QSize(100, 16777215));
        label_4->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_4);

        editScene = new QComboBox(study_list);
        editScene->setObjectName(QString::fromUtf8("editScene"));
        sizePolicy.setHeightForWidth(editScene->sizePolicy().hasHeightForWidth());
        editScene->setSizePolicy(sizePolicy);
        editScene->setMinimumSize(QSize(210, 0));
        editScene->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_3->addWidget(editScene);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_8 = new QLabel(study_list);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMaximumSize(QSize(100, 16777215));
        label_8->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label_8);

        editFault = new QComboBox(study_list);
        editFault->setObjectName(QString::fromUtf8("editFault"));
        editFault->setMinimumSize(QSize(210, 0));
        editFault->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_4->addWidget(editFault);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_7 = new QLabel(study_list);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy1);
        label_7->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(label_7);

        editRemark = new QTextEdit(study_list);
        editRemark->setObjectName(QString::fromUtf8("editRemark"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(editRemark->sizePolicy().hasHeightForWidth());
        editRemark->setSizePolicy(sizePolicy2);
        editRemark->setMaximumSize(QSize(16777215, 16777215));
        editRemark->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(editRemark);


        verticalLayout_2->addLayout(verticalLayout);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);

        editRemark->raise();
        label_7->raise();
        label_11->raise();
        label_5->raise();
        editStation->raise();
        editScene->raise();
        label_4->raise();
        editName->raise();
        label->raise();
        label_8->raise();
        editFault->raise();
        label_6->raise();
        editDev->raise();

        retranslateUi(study_list);

        QMetaObject::connectSlotsByName(study_list);
    } // setupUi

    void retranslateUi(QWidget *study_list)
    {
        study_list->setWindowTitle(QApplication::translate("study_list", "\345\255\246\344\271\240\344\273\273\345\212\241\347\274\226\350\276\221", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("study_list", "\344\273\273\345\212\241\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("study_list", "\351\200\202\347\224\250\345\255\246\345\221\230\357\274\232", 0, QApplication::UnicodeUTF8));
        editUsertype->clear();
        editUsertype->insertItems(0, QStringList()
         << QApplication::translate("study_list", "\345\210\235\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("study_list", "\344\270\255\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("study_list", "\351\253\230\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("study_list", "\346\212\200\345\270\210\344\273\245\345\217\212\351\253\230\347\272\247\346\212\200\345\270\210", 0, QApplication::UnicodeUTF8)
        );
        label_5->setText(QApplication::translate("study_list", "\345\216\202    \347\253\231\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("study_list", "\350\256\276    \345\244\207\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("study_list", "\345\210\206\347\261\273\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("study_list", "\345\261\236    \346\200\247\357\274\232", 0, QApplication::UnicodeUTF8));
        editType->clear();
        editType->insertItems(0, QStringList()
         << QApplication::translate("study_list", "\346\255\243\345\270\270\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("study_list", "\346\225\205\351\232\234\345\244\204\347\220\206", 0, QApplication::UnicodeUTF8)
        );
        label_4->setText(QApplication::translate("study_list", "\345\267\245\345\206\265\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("study_list", "\346\225\205\351\232\234\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("study_list", "\346\223\215\344\275\234\346\217\220\347\244\272\357\274\232", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(study_list);
    } // retranslateUi

};

namespace Ui {
    class study_list: public Ui_study_list {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STUDY_TASK_H
