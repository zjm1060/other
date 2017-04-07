/********************************************************************************
** Form generated from reading ui file 'taskadd.ui'
**
** Created: Thu Jan 15 10:24:40 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TASKADD_H
#define UI_TASKADD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CTaskAddTable
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *editGrpName;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QComboBox *editScene;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_8;
    QComboBox *editFault;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QComboBox *editType;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QComboBox *editStation;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QComboBox *editDev;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_11;
    QComboBox *editUsertype;
    QLabel *label_7;
    QTextEdit *editRemark;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_12;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_9;
    QLabel *labelStepCount;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_10;
    QLabel *labelTotleScore;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_8;
    QTableWidget *tableWidget;
    QVBoxLayout *verticalLayout_2;
    QPushButton *buttonAddSubject;
    QPushButton *buttonModifySubject;
    QPushButton *buttonDelSubject;
    QPushButton *buttonImportSubject;
    QPushButton *buttonExportSubject;
    QPushButton *buttonAverage;
    QPushButton *buttonMoveUp;
    QPushButton *buttonMoveDown;
    QPushButton *buttonRefresh;
    QHBoxLayout *horizontalLayout_11;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonApply;
    QPushButton *buttonEnter;
    QPushButton *buttonExit;
    QCheckBox *checkBoxstudy;

    void setupUi(QDialog *CTaskAddTable)
    {
        if (CTaskAddTable->objectName().isEmpty())
            CTaskAddTable->setObjectName(QString::fromUtf8("CTaskAddTable"));
        CTaskAddTable->resize(1088, 710);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CTaskAddTable->sizePolicy().hasHeightForWidth());
        CTaskAddTable->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(CTaskAddTable);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CTaskAddTable);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(100, 0));
        label->setMaximumSize(QSize(100, 16777215));
        label->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label);

        editName = new QLineEdit(CTaskAddTable);
        editName->setObjectName(QString::fromUtf8("editName"));
        sizePolicy.setHeightForWidth(editName->sizePolicy().hasHeightForWidth());
        editName->setSizePolicy(sizePolicy);
        editName->setMinimumSize(QSize(210, 0));
        editName->setMaximumSize(QSize(210, 16777215));

        horizontalLayout->addWidget(editName);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(CTaskAddTable);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(0, 0));
        label_2->setMaximumSize(QSize(100, 16777215));
        label_2->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_2);

        editGrpName = new QComboBox(CTaskAddTable);
        editGrpName->setObjectName(QString::fromUtf8("editGrpName"));
        sizePolicy.setHeightForWidth(editGrpName->sizePolicy().hasHeightForWidth());
        editGrpName->setSizePolicy(sizePolicy);
        editGrpName->setMinimumSize(QSize(210, 0));
        editGrpName->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_2->addWidget(editGrpName);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(CTaskAddTable);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMaximumSize(QSize(100, 16777215));
        label_4->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_4);

        editScene = new QComboBox(CTaskAddTable);
        editScene->setObjectName(QString::fromUtf8("editScene"));
        sizePolicy.setHeightForWidth(editScene->sizePolicy().hasHeightForWidth());
        editScene->setSizePolicy(sizePolicy);
        editScene->setMinimumSize(QSize(210, 0));
        editScene->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_3->addWidget(editScene);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_8 = new QLabel(CTaskAddTable);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMaximumSize(QSize(100, 16777215));
        label_8->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label_8);

        editFault = new QComboBox(CTaskAddTable);
        editFault->setObjectName(QString::fromUtf8("editFault"));
        editFault->setMinimumSize(QSize(210, 0));
        editFault->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_4->addWidget(editFault);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_3 = new QLabel(CTaskAddTable);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(100, 16777215));
        label_3->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(label_3);

        editType = new QComboBox(CTaskAddTable);
        editType->setObjectName(QString::fromUtf8("editType"));
        editType->setMinimumSize(QSize(210, 0));
        editType->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_5->addWidget(editType);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_5 = new QLabel(CTaskAddTable);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMaximumSize(QSize(100, 16777215));
        label_5->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_6->addWidget(label_5);

        editStation = new QComboBox(CTaskAddTable);
        editStation->setObjectName(QString::fromUtf8("editStation"));
        editStation->setMinimumSize(QSize(210, 0));
        editStation->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_6->addWidget(editStation);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_6 = new QLabel(CTaskAddTable);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMaximumSize(QSize(100, 16777215));
        label_6->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_7->addWidget(label_6);

        editDev = new QComboBox(CTaskAddTable);
        editDev->setObjectName(QString::fromUtf8("editDev"));
        sizePolicy.setHeightForWidth(editDev->sizePolicy().hasHeightForWidth());
        editDev->setSizePolicy(sizePolicy);
        editDev->setMinimumSize(QSize(210, 0));
        editDev->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_7->addWidget(editDev);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_11 = new QLabel(CTaskAddTable);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setMaximumSize(QSize(100, 16777215));
        label_11->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_13->addWidget(label_11);

        editUsertype = new QComboBox(CTaskAddTable);
        editUsertype->setObjectName(QString::fromUtf8("editUsertype"));
        editUsertype->setMinimumSize(QSize(210, 0));
        editUsertype->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_13->addWidget(editUsertype);


        verticalLayout->addLayout(horizontalLayout_13);

        label_7 = new QLabel(CTaskAddTable);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy1);
        label_7->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(label_7);

        editRemark = new QTextEdit(CTaskAddTable);
        editRemark->setObjectName(QString::fromUtf8("editRemark"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(editRemark->sizePolicy().hasHeightForWidth());
        editRemark->setSizePolicy(sizePolicy2);
        editRemark->setMaximumSize(QSize(16777215, 16777215));
        editRemark->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(editRemark);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        groupBox = new QGroupBox(CTaskAddTable);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(743, 0));
        groupBox->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_9->addWidget(label_9);

        labelStepCount = new QLabel(groupBox);
        labelStepCount->setObjectName(QString::fromUtf8("labelStepCount"));
        labelStepCount->setMinimumSize(QSize(30, 0));
        labelStepCount->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));

        horizontalLayout_9->addWidget(labelStepCount);


        horizontalLayout_12->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_10->addWidget(label_10);

        labelTotleScore = new QLabel(groupBox);
        labelTotleScore->setObjectName(QString::fromUtf8("labelTotleScore"));
        labelTotleScore->setMinimumSize(QSize(30, 0));
        labelTotleScore->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));

        horizontalLayout_10->addWidget(labelTotleScore);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_2);


        horizontalLayout_12->addLayout(horizontalLayout_10);


        verticalLayout_3->addLayout(horizontalLayout_12);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        tableWidget = new QTableWidget(groupBox);
        if (tableWidget->columnCount() < 5)
            tableWidget->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(5);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout_8->addWidget(tableWidget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        buttonAddSubject = new QPushButton(groupBox);
        buttonAddSubject->setObjectName(QString::fromUtf8("buttonAddSubject"));
        buttonAddSubject->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_2->addWidget(buttonAddSubject);

        buttonModifySubject = new QPushButton(groupBox);
        buttonModifySubject->setObjectName(QString::fromUtf8("buttonModifySubject"));
        buttonModifySubject->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_2->addWidget(buttonModifySubject);

        buttonDelSubject = new QPushButton(groupBox);
        buttonDelSubject->setObjectName(QString::fromUtf8("buttonDelSubject"));
        buttonDelSubject->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_2->addWidget(buttonDelSubject);

        buttonImportSubject = new QPushButton(groupBox);
        buttonImportSubject->setObjectName(QString::fromUtf8("buttonImportSubject"));
        buttonImportSubject->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_2->addWidget(buttonImportSubject);

        buttonExportSubject = new QPushButton(groupBox);
        buttonExportSubject->setObjectName(QString::fromUtf8("buttonExportSubject"));
        buttonExportSubject->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_2->addWidget(buttonExportSubject);

        buttonAverage = new QPushButton(groupBox);
        buttonAverage->setObjectName(QString::fromUtf8("buttonAverage"));
        buttonAverage->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_2->addWidget(buttonAverage);

        buttonMoveUp = new QPushButton(groupBox);
        buttonMoveUp->setObjectName(QString::fromUtf8("buttonMoveUp"));
        buttonMoveUp->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_2->addWidget(buttonMoveUp);

        buttonMoveDown = new QPushButton(groupBox);
        buttonMoveDown->setObjectName(QString::fromUtf8("buttonMoveDown"));
        buttonMoveDown->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_2->addWidget(buttonMoveDown);

        buttonRefresh = new QPushButton(groupBox);
        buttonRefresh->setObjectName(QString::fromUtf8("buttonRefresh"));
        buttonRefresh->setStyleSheet(QString::fromUtf8("background-color: rgb(156, 196, 255);"));

        verticalLayout_2->addWidget(buttonRefresh);


        horizontalLayout_8->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_8);


        gridLayout->addWidget(groupBox, 0, 1, 1, 1);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalSpacer = new QSpacerItem(528, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer);

        buttonApply = new QPushButton(CTaskAddTable);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_11->addWidget(buttonApply);

        buttonEnter = new QPushButton(CTaskAddTable);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_11->addWidget(buttonEnter);

        buttonExit = new QPushButton(CTaskAddTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_11->addWidget(buttonExit);


        gridLayout->addLayout(horizontalLayout_11, 1, 1, 1, 1);

        checkBoxstudy = new QCheckBox(CTaskAddTable);
        checkBoxstudy->setObjectName(QString::fromUtf8("checkBoxstudy"));
        checkBoxstudy->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);\n"
"font: 75 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        gridLayout->addWidget(checkBoxstudy, 1, 0, 1, 1);

        QWidget::setTabOrder(editName, editGrpName);
        QWidget::setTabOrder(editGrpName, editType);
        QWidget::setTabOrder(editType, editRemark);
        QWidget::setTabOrder(editRemark, buttonExit);

        retranslateUi(CTaskAddTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CTaskAddTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CTaskAddTable);
    } // setupUi

    void retranslateUi(QDialog *CTaskAddTable)
    {
        CTaskAddTable->setWindowTitle(QApplication::translate("CTaskAddTable", "\346\267\273\345\212\240\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CTaskAddTable", "\344\273\273\345\212\241\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CTaskAddTable", "\345\210\206\347\261\273\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CTaskAddTable", "\345\267\245\345\206\265\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("CTaskAddTable", "\346\225\205\351\232\234\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CTaskAddTable", "\345\261\236    \346\200\247\357\274\232", 0, QApplication::UnicodeUTF8));
        editType->clear();
        editType->insertItems(0, QStringList()
         << QApplication::translate("CTaskAddTable", "\346\255\243\345\270\270\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTaskAddTable", "\346\225\205\351\232\234\345\244\204\347\220\206", 0, QApplication::UnicodeUTF8)
        );
        label_5->setText(QApplication::translate("CTaskAddTable", "\345\216\202    \347\253\231\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CTaskAddTable", "\350\256\276    \345\244\207\357\274\232", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("CTaskAddTable", "\351\200\202\347\224\250\345\255\246\345\221\230\357\274\232", 0, QApplication::UnicodeUTF8));
        editUsertype->clear();
        editUsertype->insertItems(0, QStringList()
         << QApplication::translate("CTaskAddTable", "\350\257\267\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTaskAddTable", "\346\226\260\345\221\230\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTaskAddTable", "\345\210\235\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTaskAddTable", "\344\270\255\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTaskAddTable", "\351\253\230\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTaskAddTable", "\346\212\200\345\270\210", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTaskAddTable", "\351\253\230\347\272\247\346\212\200\345\270\210", 0, QApplication::UnicodeUTF8)
        );
        label_7->setText(QApplication::translate("CTaskAddTable", "\346\223\215\344\275\234\346\217\220\347\244\272\357\274\232", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CTaskAddTable", "\351\242\230\347\233\256\347\274\226\350\276\221", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("CTaskAddTable", "\345\275\223\345\211\215\344\273\273\345\212\241\351\242\230\347\233\256\346\225\260", 0, QApplication::UnicodeUTF8));
        labelStepCount->setText(QApplication::translate("CTaskAddTable", "0", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("CTaskAddTable", "\345\275\223\345\211\215\344\273\273\345\212\241\346\200\273\345\210\206\346\225\260", 0, QApplication::UnicodeUTF8));
        labelTotleScore->setText(QApplication::translate("CTaskAddTable", "0", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CTaskAddTable", "\346\255\245\351\252\244", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CTaskAddTable", "\351\242\230\347\233\256\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CTaskAddTable", "\345\210\206\345\200\274", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CTaskAddTable", "\346\227\266\351\231\220\357\274\210\347\247\222\357\274\211", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("CTaskAddTable", "\346\217\220\347\244\272\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        buttonAddSubject->setText(QApplication::translate("CTaskAddTable", "\346\267\273\345\212\240\351\242\230\347\233\256", 0, QApplication::UnicodeUTF8));
        buttonModifySubject->setText(QApplication::translate("CTaskAddTable", "\344\277\256\346\224\271\351\242\230\347\233\256", 0, QApplication::UnicodeUTF8));
        buttonDelSubject->setText(QApplication::translate("CTaskAddTable", "\345\210\240\351\231\244\351\242\230\347\233\256", 0, QApplication::UnicodeUTF8));
        buttonImportSubject->setText(QApplication::translate("CTaskAddTable", "\345\257\274\345\205\245\351\242\230\347\233\256", 0, QApplication::UnicodeUTF8));
        buttonExportSubject->setText(QApplication::translate("CTaskAddTable", "\345\257\274\345\207\272\351\242\230\347\233\256", 0, QApplication::UnicodeUTF8));
        buttonAverage->setText(QApplication::translate("CTaskAddTable", "\345\210\206\346\225\260\345\235\207\345\210\206", 0, QApplication::UnicodeUTF8));
        buttonMoveUp->setText(QApplication::translate("CTaskAddTable", "\344\270\212 \347\247\273", 0, QApplication::UnicodeUTF8));
        buttonMoveDown->setText(QApplication::translate("CTaskAddTable", "\344\270\213 \347\247\273", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QApplication::translate("CTaskAddTable", "\345\210\267 \346\226\260", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonApply->setToolTip(QApplication::translate("CTaskAddTable", "\350\257\267\345\205\210\344\277\235\345\255\230\345\237\272\346\234\254\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        buttonApply->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        buttonApply->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        buttonApply->setText(QApplication::translate("CTaskAddTable", "\345\272\224  \347\224\250", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonEnter->setToolTip(QApplication::translate("CTaskAddTable", "\350\257\267\345\205\210\344\277\235\345\255\230\345\237\272\346\234\254\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        buttonEnter->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        buttonEnter->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        buttonEnter->setText(QApplication::translate("CTaskAddTable", "\347\241\256  \345\256\232", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CTaskAddTable", "\351\200\200  \345\207\272", 0, QApplication::UnicodeUTF8));
        checkBoxstudy->setText(QApplication::translate("CTaskAddTable", "\350\257\245\344\273\273\345\212\241\345\217\257\344\276\233\345\255\246\345\221\230\347\273\203\344\271\240", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CTaskAddTable);
    } // retranslateUi

};

namespace Ui {
    class CTaskAddTable: public Ui_CTaskAddTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TASKADD_H
