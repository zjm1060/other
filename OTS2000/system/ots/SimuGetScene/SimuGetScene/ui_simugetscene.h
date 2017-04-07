/********************************************************************************
** Form generated from reading ui file 'simugetscene.ui'
**
** Created: Thu Jan 9 11:40:15 2014
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SIMUGETSCENE_H
#define UI_SIMUGETSCENE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CSimuGetScene
{
public:
    QGridLayout *gridLayout;
    QTreeWidget *treeWidget;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *editPointName;
    QListView *listView;
    QVBoxLayout *verticalLayout;
    QPushButton *buttonSelectAll;
    QPushButton *buttonSelectOff;
    QPushButton *buttonSelectInvert;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonEnter;
    QPushButton *buttonCancel;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboPointMode;
    QComboBox *comboPointShowType;

    void setupUi(QDialog *CSimuGetScene)
    {
        if (CSimuGetScene->objectName().isEmpty())
            CSimuGetScene->setObjectName(QString::fromUtf8("CSimuGetScene"));
        CSimuGetScene->resize(800, 600);
        gridLayout = new QGridLayout(CSimuGetScene);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        treeWidget = new QTreeWidget(CSimuGetScene);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeWidget->setRootIsDecorated(true);
        treeWidget->setSortingEnabled(false);
        treeWidget->setAnimated(true);
        treeWidget->setHeaderHidden(true);

        gridLayout->addWidget(treeWidget, 1, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        editPointName = new QLineEdit(CSimuGetScene);
        editPointName->setObjectName(QString::fromUtf8("editPointName"));

        verticalLayout_2->addWidget(editPointName);

        listView = new QListView(CSimuGetScene);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout_2->addWidget(listView);


        gridLayout->addLayout(verticalLayout_2, 0, 1, 2, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        buttonSelectAll = new QPushButton(CSimuGetScene);
        buttonSelectAll->setObjectName(QString::fromUtf8("buttonSelectAll"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonSelectAll->sizePolicy().hasHeightForWidth());
        buttonSelectAll->setSizePolicy(sizePolicy);
        buttonSelectAll->setMaximumSize(QSize(30, 16777215));

        verticalLayout->addWidget(buttonSelectAll);

        buttonSelectOff = new QPushButton(CSimuGetScene);
        buttonSelectOff->setObjectName(QString::fromUtf8("buttonSelectOff"));
        sizePolicy.setHeightForWidth(buttonSelectOff->sizePolicy().hasHeightForWidth());
        buttonSelectOff->setSizePolicy(sizePolicy);
        buttonSelectOff->setMaximumSize(QSize(30, 16777215));

        verticalLayout->addWidget(buttonSelectOff);

        buttonSelectInvert = new QPushButton(CSimuGetScene);
        buttonSelectInvert->setObjectName(QString::fromUtf8("buttonSelectInvert"));
        sizePolicy.setHeightForWidth(buttonSelectInvert->sizePolicy().hasHeightForWidth());
        buttonSelectInvert->setSizePolicy(sizePolicy);
        buttonSelectInvert->setMaximumSize(QSize(30, 16777215));

        verticalLayout->addWidget(buttonSelectInvert);


        gridLayout->addLayout(verticalLayout, 1, 2, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        buttonEnter = new QPushButton(CSimuGetScene);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));

        horizontalLayout_2->addWidget(buttonEnter);

        buttonCancel = new QPushButton(CSimuGetScene);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));

        horizontalLayout_2->addWidget(buttonCancel);


        gridLayout->addLayout(horizontalLayout_2, 2, 1, 1, 2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        comboPointMode = new QComboBox(CSimuGetScene);
        comboPointMode->setObjectName(QString::fromUtf8("comboPointMode"));

        horizontalLayout->addWidget(comboPointMode);

        comboPointShowType = new QComboBox(CSimuGetScene);
        comboPointShowType->setObjectName(QString::fromUtf8("comboPointShowType"));

        horizontalLayout->addWidget(comboPointShowType);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        gridLayout->setColumnStretch(0, 2);
        gridLayout->setColumnStretch(1, 3);

        retranslateUi(CSimuGetScene);

        QMetaObject::connectSlotsByName(CSimuGetScene);
    } // setupUi

    void retranslateUi(QDialog *CSimuGetScene)
    {
        CSimuGetScene->setWindowTitle(QApplication::translate("CSimuGetScene", "\350\256\276\347\275\256\345\261\217\350\224\275\347\202\271", 0, QApplication::UnicodeUTF8));
        buttonSelectAll->setText(QApplication::translate("CSimuGetScene", "\345\205\250\n"
"\351\200\211", 0, QApplication::UnicodeUTF8));
        buttonSelectOff->setText(QApplication::translate("CSimuGetScene", "\345\217\226\n"
"\346\266\210\n"
"\351\200\211\n"
"\346\213\251", 0, QApplication::UnicodeUTF8));
        buttonSelectInvert->setText(QApplication::translate("CSimuGetScene", "\345\217\215\n"
"\351\200\211", 0, QApplication::UnicodeUTF8));
        buttonEnter->setText(QApplication::translate("CSimuGetScene", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("CSimuGetScene", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        comboPointMode->clear();
        comboPointMode->insertItems(0, QStringList()
         << QApplication::translate("CSimuGetScene", "\346\230\276\347\244\272\344\270\255\346\226\207\345\220\215", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CSimuGetScene", "\346\230\276\347\244\272\351\200\273\350\276\221\345\220\215", 0, QApplication::UnicodeUTF8)
        );
        comboPointShowType->clear();
        comboPointShowType->insertItems(0, QStringList()
         << QApplication::translate("CSimuGetScene", "\346\214\211\347\261\273\345\236\213\346\230\276\347\244\272", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CSimuGetScene", "\346\214\211\350\256\276\345\244\207\346\230\276\347\244\272", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CSimuGetScene", "\346\214\211\345\216\202\347\253\231\346\230\276\347\244\272", 0, QApplication::UnicodeUTF8)
        );
        Q_UNUSED(CSimuGetScene);
    } // retranslateUi

};

namespace Ui {
    class CSimuGetScene: public Ui_CSimuGetScene {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMUGETSCENE_H
