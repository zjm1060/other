/********************************************************************************
** Form generated from reading ui file 'study_task_list.ui'
**
** Created: Thu Jan 15 10:28:26 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STUDY_TASK_LIST_H
#define UI_STUDY_TASK_LIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_study_task_list
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QTableView *tableView;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButtonadd;
    QPushButton *pushButtonmodify;
    QPushButton *pushButtondel;
    QPushButton *pushButtonrefresh;
    QPushButton *pushButtonquit;

    void setupUi(QWidget *study_task_list)
    {
        if (study_task_list->objectName().isEmpty())
            study_task_list->setObjectName(QString::fromUtf8("study_task_list"));
        study_task_list->resize(759, 495);
        study_task_list->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        gridLayout = new QGridLayout(study_task_list);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tableView = new QTableView(study_task_list);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        horizontalLayout->addWidget(tableView);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButtonadd = new QPushButton(study_task_list);
        pushButtonadd->setObjectName(QString::fromUtf8("pushButtonadd"));

        verticalLayout->addWidget(pushButtonadd);

        pushButtonmodify = new QPushButton(study_task_list);
        pushButtonmodify->setObjectName(QString::fromUtf8("pushButtonmodify"));

        verticalLayout->addWidget(pushButtonmodify);

        pushButtondel = new QPushButton(study_task_list);
        pushButtondel->setObjectName(QString::fromUtf8("pushButtondel"));

        verticalLayout->addWidget(pushButtondel);

        pushButtonrefresh = new QPushButton(study_task_list);
        pushButtonrefresh->setObjectName(QString::fromUtf8("pushButtonrefresh"));

        verticalLayout->addWidget(pushButtonrefresh);

        pushButtonquit = new QPushButton(study_task_list);
        pushButtonquit->setObjectName(QString::fromUtf8("pushButtonquit"));

        verticalLayout->addWidget(pushButtonquit);


        horizontalLayout->addLayout(verticalLayout);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(study_task_list);

        QMetaObject::connectSlotsByName(study_task_list);
    } // setupUi

    void retranslateUi(QWidget *study_task_list)
    {
        study_task_list->setWindowTitle(QApplication::translate("study_task_list", "\345\255\246\344\271\240\344\273\273\345\212\241\345\272\223", 0, QApplication::UnicodeUTF8));
        pushButtonadd->setText(QApplication::translate("study_task_list", "\345\242\236\345\212\240\345\255\246\344\271\240\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        pushButtonmodify->setText(QApplication::translate("study_task_list", "\344\277\256\346\224\271\345\255\246\344\271\240\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        pushButtondel->setText(QApplication::translate("study_task_list", "\345\210\240\351\231\244\345\255\246\344\271\240\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        pushButtonrefresh->setText(QApplication::translate("study_task_list", "\345\210\267   \346\226\260", 0, QApplication::UnicodeUTF8));
        pushButtonquit->setText(QApplication::translate("study_task_list", "\351\200\200   \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(study_task_list);
    } // retranslateUi

};

namespace Ui {
    class study_task_list: public Ui_study_task_list {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STUDY_TASK_LIST_H
