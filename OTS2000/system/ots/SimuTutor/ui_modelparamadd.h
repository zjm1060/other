/********************************************************************************
** Form generated from reading ui file 'modelparamadd.ui'
**
** Created: Thu Jan 15 10:26:08 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MODELPARAMADD_H
#define UI_MODELPARAMADD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CModelParamAddForm
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *editType;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *editLogicalname;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_13;
    QComboBox *editDataType;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *buttonApply;
    QPushButton *buttonEnter;
    QPushButton *buttonExit;

    void setupUi(QDialog *CModelParamAddForm)
    {
        if (CModelParamAddForm->objectName().isEmpty())
            CModelParamAddForm->setObjectName(QString::fromUtf8("CModelParamAddForm"));
        CModelParamAddForm->resize(353, 245);
        verticalLayout = new QVBoxLayout(CModelParamAddForm);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CModelParamAddForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(120, 0));
        label->setMaximumSize(QSize(120, 16777215));
        label->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label);

        editName = new QLineEdit(CModelParamAddForm);
        editName->setObjectName(QString::fromUtf8("editName"));

        horizontalLayout->addWidget(editName);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(CModelParamAddForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(120, 0));
        label_2->setMaximumSize(QSize(120, 16777215));
        label_2->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_2);

        editType = new QComboBox(CModelParamAddForm);
        editType->setObjectName(QString::fromUtf8("editType"));

        horizontalLayout_2->addWidget(editType);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(CModelParamAddForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(120, 0));
        label_3->setMaximumSize(QSize(120, 16777215));
        label_3->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_3);

        editLogicalname = new QLineEdit(CModelParamAddForm);
        editLogicalname->setObjectName(QString::fromUtf8("editLogicalname"));

        horizontalLayout_3->addWidget(editLogicalname);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_13 = new QLabel(CModelParamAddForm);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setMinimumSize(QSize(120, 0));
        label_13->setMaximumSize(QSize(120, 16777215));
        label_13->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label_13);

        editDataType = new QComboBox(CModelParamAddForm);
        editDataType->setObjectName(QString::fromUtf8("editDataType"));

        horizontalLayout_4->addWidget(editDataType);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(30);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        buttonApply = new QPushButton(CModelParamAddForm);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(buttonApply);

        buttonEnter = new QPushButton(CModelParamAddForm);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(buttonEnter);

        buttonExit = new QPushButton(CModelParamAddForm);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(buttonExit);


        verticalLayout->addLayout(horizontalLayout_5);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 2);
        verticalLayout->setStretch(2, 2);
        verticalLayout->setStretch(3, 2);
        verticalLayout->setStretch(4, 3);
        QWidget::setTabOrder(editName, editType);
        QWidget::setTabOrder(editType, editLogicalname);
        QWidget::setTabOrder(editLogicalname, editDataType);
        QWidget::setTabOrder(editDataType, buttonEnter);
        QWidget::setTabOrder(buttonEnter, buttonExit);

        retranslateUi(CModelParamAddForm);
        QObject::connect(buttonExit, SIGNAL(clicked()), CModelParamAddForm, SLOT(reject()));

        QMetaObject::connectSlotsByName(CModelParamAddForm);
    } // setupUi

    void retranslateUi(QDialog *CModelParamAddForm)
    {
        CModelParamAddForm->setWindowTitle(QApplication::translate("CModelParamAddForm", "\346\267\273\345\212\240\346\250\241\345\236\213\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CModelParamAddForm", "\345\217\202 \346\225\260 \345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CModelParamAddForm", "\345\217\202\346\225\260\347\261\273\345\236\213\357\274\232", 0, QApplication::UnicodeUTF8));
        editType->clear();
        editType->insertItems(0, QStringList()
         << QApplication::translate("CModelParamAddForm", "\350\276\223\345\205\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CModelParamAddForm", "\350\276\223\345\207\272", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CModelParamAddForm", "\345\206\205\351\203\250\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("CModelParamAddForm", "\346\225\260\345\200\274\346\210\226\347\202\271\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("CModelParamAddForm", "\346\225\260\346\215\256\347\261\273\345\236\213\357\274\232", 0, QApplication::UnicodeUTF8));
        editDataType->clear();
        editDataType->insertItems(0, QStringList()
         << QApplication::translate("CModelParamAddForm", "H9000\347\202\271\345\220\215", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CModelParamAddForm", "\345\270\270\345\200\274", 0, QApplication::UnicodeUTF8)
        );
        buttonApply->setText(QApplication::translate("CModelParamAddForm", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("CModelParamAddForm", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CModelParamAddForm", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CModelParamAddForm);
    } // retranslateUi

};

namespace Ui {
    class CModelParamAddForm: public Ui_CModelParamAddForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELPARAMADD_H
