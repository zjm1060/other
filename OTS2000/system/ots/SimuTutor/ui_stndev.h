/********************************************************************************
** Form generated from reading ui file 'stndev.ui'
**
** Created: Thu Jan 15 10:24:55 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STNDEV_H
#define UI_STNDEV_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_CStnDevTable
{
public:
    QTreeWidget *listStnDev;

    void setupUi(QDialog *CStnDevTable)
    {
        if (CStnDevTable->objectName().isEmpty())
            CStnDevTable->setObjectName(QString::fromUtf8("CStnDevTable"));
        CStnDevTable->resize(219, 550);
        listStnDev = new QTreeWidget(CStnDevTable);
        listStnDev->setObjectName(QString::fromUtf8("listStnDev"));
        listStnDev->setGeometry(QRect(0, 0, 221, 551));
        listStnDev->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        retranslateUi(CStnDevTable);

        QMetaObject::connectSlotsByName(CStnDevTable);
    } // setupUi

    void retranslateUi(QDialog *CStnDevTable)
    {
        CStnDevTable->setWindowTitle(QApplication::translate("CStnDevTable", "\345\216\202\347\253\231\350\256\276\345\244\207", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = listStnDev->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("CStnDevTable", "\345\216\202\347\253\231\345\222\214\350\256\276\345\244\207", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CStnDevTable);
    } // retranslateUi

};

namespace Ui {
    class CStnDevTable: public Ui_CStnDevTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STNDEV_H
