/********************************************************************************
** Form generated from reading ui file 'scene.ui'
**
** Created: Thu Jan 15 10:25:58 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SCENE_H
#define UI_SCENE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CSceneTable
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *listScene;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QTableWidget *listSceneMerger;
    QGridLayout *gridLayout;
    QComboBox *comboSceneName;
    QPushButton *buttonRunSceneMerger;
    QPushButton *buttonClearMerger;
    QLabel *label;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QPushButton *buttonSceneData;
    QPushButton *buttonSceneAdd;
    QPushButton *buttonSceneDel;
    QPushButton *buttonSceneModify;
    QPushButton *buttonSceneInto;
    QPushButton *buttonStnDev;
    QPushButton *buttonRefresh;
    QPushButton *buttonExit;

    void setupUi(QDialog *CSceneTable)
    {
        if (CSceneTable->objectName().isEmpty())
            CSceneTable->setObjectName(QString::fromUtf8("CSceneTable"));
        CSceneTable->resize(800, 550);
        horizontalLayout_2 = new QHBoxLayout(CSceneTable);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        listScene = new QTableWidget(CSceneTable);
        if (listScene->columnCount() < 9)
            listScene->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        listScene->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        listScene->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        listScene->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        listScene->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        listScene->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        listScene->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        listScene->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        listScene->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        listScene->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        listScene->setObjectName(QString::fromUtf8("listScene"));
        listScene->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        listScene->setSortingEnabled(true);
        listScene->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_2->addWidget(listScene);

        groupBox = new QGroupBox(CSceneTable);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMaximumSize(QSize(16777215, 150));
        groupBox->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listSceneMerger = new QTableWidget(groupBox);
        if (listSceneMerger->columnCount() < 1)
            listSceneMerger->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        listSceneMerger->setHorizontalHeaderItem(0, __qtablewidgetitem9);
        listSceneMerger->setObjectName(QString::fromUtf8("listSceneMerger"));
        listSceneMerger->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(listSceneMerger);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(6);
        gridLayout->setVerticalSpacing(9);
        comboSceneName = new QComboBox(groupBox);
        comboSceneName->setObjectName(QString::fromUtf8("comboSceneName"));
        comboSceneName->setMinimumSize(QSize(200, 0));
        comboSceneName->setStyleSheet(QString::fromUtf8(""));
        comboSceneName->setEditable(true);

        gridLayout->addWidget(comboSceneName, 1, 0, 1, 2);

        buttonRunSceneMerger = new QPushButton(groupBox);
        buttonRunSceneMerger->setObjectName(QString::fromUtf8("buttonRunSceneMerger"));
        buttonRunSceneMerger->setMinimumSize(QSize(0, 40));

        gridLayout->addWidget(buttonRunSceneMerger, 2, 0, 1, 1);

        buttonClearMerger = new QPushButton(groupBox);
        buttonClearMerger->setObjectName(QString::fromUtf8("buttonClearMerger"));
        buttonClearMerger->setMinimumSize(QSize(0, 40));

        gridLayout->addWidget(buttonClearMerger, 2, 1, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(16777215, 20));

        gridLayout->addWidget(label, 0, 0, 1, 2);


        horizontalLayout->addLayout(gridLayout);


        verticalLayout_2->addWidget(groupBox);


        horizontalLayout_2->addLayout(verticalLayout_2);

        frame = new QFrame(CSceneTable);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(150, 0));
        frame->setMaximumSize(QSize(150, 16777215));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonSceneData = new QPushButton(frame);
        buttonSceneData->setObjectName(QString::fromUtf8("buttonSceneData"));
        buttonSceneData->setMinimumSize(QSize(0, 40));
        buttonSceneData->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonSceneData);

        buttonSceneAdd = new QPushButton(frame);
        buttonSceneAdd->setObjectName(QString::fromUtf8("buttonSceneAdd"));
        buttonSceneAdd->setMinimumSize(QSize(0, 40));
        buttonSceneAdd->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonSceneAdd);

        buttonSceneDel = new QPushButton(frame);
        buttonSceneDel->setObjectName(QString::fromUtf8("buttonSceneDel"));
        buttonSceneDel->setMinimumSize(QSize(0, 40));
        buttonSceneDel->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonSceneDel);

        buttonSceneModify = new QPushButton(frame);
        buttonSceneModify->setObjectName(QString::fromUtf8("buttonSceneModify"));
        buttonSceneModify->setMinimumSize(QSize(0, 40));
        buttonSceneModify->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonSceneModify);

        buttonSceneInto = new QPushButton(frame);
        buttonSceneInto->setObjectName(QString::fromUtf8("buttonSceneInto"));
        buttonSceneInto->setMinimumSize(QSize(0, 40));
        buttonSceneInto->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonSceneInto);

        buttonStnDev = new QPushButton(frame);
        buttonStnDev->setObjectName(QString::fromUtf8("buttonStnDev"));
        buttonStnDev->setMinimumSize(QSize(0, 40));
        buttonStnDev->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonStnDev);

        buttonRefresh = new QPushButton(frame);
        buttonRefresh->setObjectName(QString::fromUtf8("buttonRefresh"));
        buttonRefresh->setMinimumSize(QSize(0, 40));
        buttonRefresh->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonRefresh);

        buttonExit = new QPushButton(frame);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setMinimumSize(QSize(0, 40));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(buttonExit);


        horizontalLayout_2->addWidget(frame);

        QWidget::setTabOrder(buttonSceneData, buttonSceneAdd);
        QWidget::setTabOrder(buttonSceneAdd, buttonSceneDel);
        QWidget::setTabOrder(buttonSceneDel, buttonSceneModify);
        QWidget::setTabOrder(buttonSceneModify, buttonRefresh);
        QWidget::setTabOrder(buttonRefresh, buttonExit);
        QWidget::setTabOrder(buttonExit, listScene);

        retranslateUi(CSceneTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CSceneTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CSceneTable);
    } // setupUi

    void retranslateUi(QDialog *CSceneTable)
    {
        CSceneTable->setWindowTitle(QApplication::translate("CSceneTable", "\351\235\231\346\200\201\345\267\245\345\206\265\345\272\223", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = listScene->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CSceneTable", "\345\267\245\345\206\265ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = listScene->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CSceneTable", "\345\267\245\345\206\265\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = listScene->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CSceneTable", "\347\224\250\346\210\267\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = listScene->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CSceneTable", "\346\235\245\346\272\220\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = listScene->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("CSceneTable", "\345\267\245\345\206\265\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = listScene->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("CSceneTable", "\345\244\207\346\263\250", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = listScene->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("CSceneTable", "\345\216\202\347\253\231\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = listScene->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("CSceneTable", "\350\256\276\345\244\207\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = listScene->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("CSceneTable", "\351\273\230\350\256\244\347\224\273\351\235\242", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CSceneTable", "\345\220\210\345\271\266\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = listSceneMerger->horizontalHeaderItem(0);
        ___qtablewidgetitem9->setText(QApplication::translate("CSceneTable", "\345\267\245\345\206\265\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        buttonRunSceneMerger->setText(QApplication::translate("CSceneTable", "\346\211\247\350\241\214\345\220\210\345\271\266", 0, QApplication::UnicodeUTF8));
        buttonClearMerger->setText(QApplication::translate("CSceneTable", "\346\270\205\347\251\272\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CSceneTable", "\345\220\210\345\271\266\345\267\245\345\206\265\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        buttonSceneData->setText(QApplication::translate("CSceneTable", "\345\267\245\345\206\265\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        buttonSceneAdd->setText(QApplication::translate("CSceneTable", "\346\267\273\345\212\240\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8));
        buttonSceneDel->setText(QApplication::translate("CSceneTable", "\345\210\240\351\231\244\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8));
        buttonSceneModify->setText(QApplication::translate("CSceneTable", "\344\277\256\346\224\271\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8));
        buttonSceneInto->setText(QApplication::translate("CSceneTable", "\345\257\274\345\205\245\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8));
        buttonStnDev->setText(QApplication::translate("CSceneTable", "\351\200\211\346\213\251\345\216\202\347\253\231\350\256\276\345\244\207", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QApplication::translate("CSceneTable", "\345\210\267   \346\226\260", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CSceneTable", "\351\200\200   \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CSceneTable);
    } // retranslateUi

};

namespace Ui {
    class CSceneTable: public Ui_CSceneTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCENE_H
