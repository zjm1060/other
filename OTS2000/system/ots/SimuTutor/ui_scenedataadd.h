/********************************************************************************
** Form generated from reading ui file 'scenedataadd.ui'
**
** Created: Thu Jan 15 10:25:49 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SCENEDATAADD_H
#define UI_SCENEDATAADD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CSceneDataAddTable
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editLogicalName;
    QPushButton *buttonPointName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *editStatus;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *editData;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *buttonEnter;
    QPushButton *buttonApply;
    QPushButton *buttonExit;

    void setupUi(QDialog *CSceneDataAddTable)
    {
        if (CSceneDataAddTable->objectName().isEmpty())
            CSceneDataAddTable->setObjectName(QString::fromUtf8("CSceneDataAddTable"));
        CSceneDataAddTable->resize(449, 249);
        CSceneDataAddTable->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        verticalLayout = new QVBoxLayout(CSceneDataAddTable);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CSceneDataAddTable);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label);

        editLogicalName = new QLineEdit(CSceneDataAddTable);
        editLogicalName->setObjectName(QString::fromUtf8("editLogicalName"));
        editLogicalName->setMinimumSize(QSize(0, 0));

        horizontalLayout->addWidget(editLogicalName);

        buttonPointName = new QPushButton(CSceneDataAddTable);
        buttonPointName->setObjectName(QString::fromUtf8("buttonPointName"));

        horizontalLayout->addWidget(buttonPointName);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(CSceneDataAddTable);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_2);

        editStatus = new QLineEdit(CSceneDataAddTable);
        editStatus->setObjectName(QString::fromUtf8("editStatus"));

        horizontalLayout_2->addWidget(editStatus);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(CSceneDataAddTable);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_3);

        editData = new QLineEdit(CSceneDataAddTable);
        editData->setObjectName(QString::fromUtf8("editData"));

        horizontalLayout_3->addWidget(editData);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(30);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        buttonEnter = new QPushButton(CSceneDataAddTable);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(buttonEnter);

        buttonApply = new QPushButton(CSceneDataAddTable);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(buttonApply);

        buttonExit = new QPushButton(CSceneDataAddTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(buttonExit);


        verticalLayout->addLayout(horizontalLayout_4);


        retranslateUi(CSceneDataAddTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CSceneDataAddTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CSceneDataAddTable);
    } // setupUi

    void retranslateUi(QDialog *CSceneDataAddTable)
    {
        CSceneDataAddTable->setWindowTitle(QApplication::translate("CSceneDataAddTable", "\346\267\273\345\212\240\345\267\245\345\206\265\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CSceneDataAddTable", "\347\202\271\351\200\273\350\276\221\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonPointName->setText(QApplication::translate("CSceneDataAddTable", "\351\200\211 \346\213\251", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CSceneDataAddTable", "\347\212\266    \346\200\201\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CSceneDataAddTable", "\346\225\260    \346\215\256\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("CSceneDataAddTable", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        buttonApply->setText(QApplication::translate("CSceneDataAddTable", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CSceneDataAddTable", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CSceneDataAddTable);
    } // retranslateUi

};

namespace Ui {
    class CSceneDataAddTable: public Ui_CSceneDataAddTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCENEDATAADD_H
