/********************************************************************************
** Form generated from reading ui file 'scenedata.ui'
**
** Created: Thu Jan 15 10:25:52 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SCENEDATA_H
#define UI_SCENEDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CSceneDataTable
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *editName;
    QPushButton *buttonSearch;
    QPushButton *buttonShowAll;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboStn;
    QComboBox *comboDev;
    QComboBox *comboType;
    QTableView *viewSceneData;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *buttonAdd;
    QPushButton *buttonModify;
    QPushButton *buttonDel;
    QPushButton *buttonExit;

    void setupUi(QDialog *CSceneDataTable)
    {
        if (CSceneDataTable->objectName().isEmpty())
            CSceneDataTable->setObjectName(QString::fromUtf8("CSceneDataTable"));
        CSceneDataTable->resize(500, 584);
        CSceneDataTable->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        verticalLayout_2 = new QVBoxLayout(CSceneDataTable);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame = new QFrame(CSceneDataTable);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        editName = new QLineEdit(frame);
        editName->setObjectName(QString::fromUtf8("editName"));

        horizontalLayout_3->addWidget(editName);

        buttonSearch = new QPushButton(frame);
        buttonSearch->setObjectName(QString::fromUtf8("buttonSearch"));

        horizontalLayout_3->addWidget(buttonSearch);

        buttonShowAll = new QPushButton(frame);
        buttonShowAll->setObjectName(QString::fromUtf8("buttonShowAll"));

        horizontalLayout_3->addWidget(buttonShowAll);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        comboStn = new QComboBox(frame);
        comboStn->setObjectName(QString::fromUtf8("comboStn"));
        comboStn->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(comboStn);

        comboDev = new QComboBox(frame);
        comboDev->setObjectName(QString::fromUtf8("comboDev"));
        comboDev->setMinimumSize(QSize(150, 0));

        horizontalLayout->addWidget(comboDev);

        comboType = new QComboBox(frame);
        comboType->setObjectName(QString::fromUtf8("comboType"));

        horizontalLayout->addWidget(comboType);


        verticalLayout->addLayout(horizontalLayout);

        viewSceneData = new QTableView(frame);
        viewSceneData->setObjectName(QString::fromUtf8("viewSceneData"));
        viewSceneData->horizontalHeader()->setStretchLastSection(true);

        verticalLayout->addWidget(viewSceneData);


        verticalLayout_2->addWidget(frame);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(20);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        buttonAdd = new QPushButton(CSceneDataTable);
        buttonAdd->setObjectName(QString::fromUtf8("buttonAdd"));
        buttonAdd->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(buttonAdd);

        buttonModify = new QPushButton(CSceneDataTable);
        buttonModify->setObjectName(QString::fromUtf8("buttonModify"));
        buttonModify->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(buttonModify);

        buttonDel = new QPushButton(CSceneDataTable);
        buttonDel->setObjectName(QString::fromUtf8("buttonDel"));
        buttonDel->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(buttonDel);

        buttonExit = new QPushButton(CSceneDataTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(buttonExit);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(CSceneDataTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CSceneDataTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CSceneDataTable);
    } // setupUi

    void retranslateUi(QDialog *CSceneDataTable)
    {
        CSceneDataTable->setWindowTitle(QApplication::translate("CSceneDataTable", "\345\267\245\345\206\265\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        editName->setText(QString());
        buttonSearch->setText(QApplication::translate("CSceneDataTable", "\346\250\241\347\263\212\346\237\245\346\211\276", 0, QApplication::UnicodeUTF8));
        buttonShowAll->setText(QApplication::translate("CSceneDataTable", "\346\230\276\347\244\272\345\205\250\351\203\250", 0, QApplication::UnicodeUTF8));
        buttonAdd->setText(QApplication::translate("CSceneDataTable", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        buttonModify->setText(QApplication::translate("CSceneDataTable", "\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        buttonDel->setText(QApplication::translate("CSceneDataTable", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CSceneDataTable", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CSceneDataTable);
    } // retranslateUi

};

namespace Ui {
    class CSceneDataTable: public Ui_CSceneDataTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCENEDATA_H
