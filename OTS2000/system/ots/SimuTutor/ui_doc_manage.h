/********************************************************************************
** Form generated from reading ui file 'doc_manage.ui'
**
** Created: Thu Jan 15 10:25:57 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DOC_MANAGE_H
#define UI_DOC_MANAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QTableView>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_doc_manage
{
public:
    QGridLayout *gridLayout_5;
    QSplitter *splitter;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *pushButtonLoadAll;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_adddoc;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pushButton_modifydoc;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *pushButton_deldoc;
    QFrame *frame;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox_docdir;
    QGridLayout *gridLayout_2;
    QTreeView *treeView_docdir;
    QGroupBox *groupBox_docattribute;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboBox_doctype;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_2;
    QComboBox *comboBox_usertype;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QDoubleSpinBox *doubleSpinBox_studytime;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_2;
    QSpinBox *spinBox_studypoints;
    QLabel *label_5;
    QLabel *label_7;
    QLabel *label_docname;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *pushButton_OK;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *pushButton_CANCEL;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *doc_manage)
    {
        if (doc_manage->objectName().isEmpty())
            doc_manage->setObjectName(QString::fromUtf8("doc_manage"));
        doc_manage->resize(1217, 663);
        doc_manage->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        gridLayout_5 = new QGridLayout(doc_manage);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        splitter = new QSplitter(doc_manage);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        groupBox_3 = new QGroupBox(splitter);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(451, 0));
        gridLayout_4 = new QGridLayout(groupBox_3);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableView = new QTableView(groupBox_3);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setMinimumSize(QSize(300, 0));

        verticalLayout->addWidget(tableView);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        pushButtonLoadAll = new QPushButton(groupBox_3);
        pushButtonLoadAll->setObjectName(QString::fromUtf8("pushButtonLoadAll"));
        pushButtonLoadAll->setStyleSheet(QString::fromUtf8("color: rgb(170, 0, 0);\n"
"font: 75 ;"));

        horizontalLayout_7->addWidget(pushButtonLoadAll);

        horizontalSpacer_3 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);

        pushButton_adddoc = new QPushButton(groupBox_3);
        pushButton_adddoc->setObjectName(QString::fromUtf8("pushButton_adddoc"));

        horizontalLayout_7->addWidget(pushButton_adddoc);

        horizontalSpacer_5 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_5);

        pushButton_modifydoc = new QPushButton(groupBox_3);
        pushButton_modifydoc->setObjectName(QString::fromUtf8("pushButton_modifydoc"));

        horizontalLayout_7->addWidget(pushButton_modifydoc);

        horizontalSpacer_6 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);

        pushButton_deldoc = new QPushButton(groupBox_3);
        pushButton_deldoc->setObjectName(QString::fromUtf8("pushButton_deldoc"));

        horizontalLayout_7->addWidget(pushButton_deldoc);


        verticalLayout->addLayout(horizontalLayout_7);


        gridLayout_4->addLayout(verticalLayout, 0, 0, 1, 1);

        splitter->addWidget(groupBox_3);
        frame = new QFrame(splitter);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setEnabled(false);
        frame->setMaximumSize(QSize(522, 16777215));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox_docdir = new QGroupBox(frame);
        groupBox_docdir->setObjectName(QString::fromUtf8("groupBox_docdir"));
        gridLayout_2 = new QGridLayout(groupBox_docdir);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        treeView_docdir = new QTreeView(groupBox_docdir);
        treeView_docdir->setObjectName(QString::fromUtf8("treeView_docdir"));

        gridLayout_2->addWidget(treeView_docdir, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_docdir, 0, 0, 1, 1);

        groupBox_docattribute = new QGroupBox(frame);
        groupBox_docattribute->setObjectName(QString::fromUtf8("groupBox_docattribute"));
        gridLayout = new QGridLayout(groupBox_docattribute);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label = new QLabel(groupBox_docattribute);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_5->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        comboBox_doctype = new QComboBox(groupBox_docattribute);
        comboBox_doctype->setObjectName(QString::fromUtf8("comboBox_doctype"));
        comboBox_doctype->setMinimumSize(QSize(120, 0));

        horizontalLayout_5->addWidget(comboBox_doctype);


        gridLayout->addLayout(horizontalLayout_5, 1, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_6 = new QLabel(groupBox_docattribute);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_6->addWidget(label_6);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);

        comboBox_usertype = new QComboBox(groupBox_docattribute);
        comboBox_usertype->setObjectName(QString::fromUtf8("comboBox_usertype"));
        comboBox_usertype->setMinimumSize(QSize(120, 0));

        horizontalLayout_6->addWidget(comboBox_usertype);


        gridLayout->addLayout(horizontalLayout_6, 1, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_2 = new QLabel(groupBox_docattribute);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_4->addWidget(label_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        doubleSpinBox_studytime = new QDoubleSpinBox(groupBox_docattribute);
        doubleSpinBox_studytime->setObjectName(QString::fromUtf8("doubleSpinBox_studytime"));
        doubleSpinBox_studytime->setDecimals(1);
        doubleSpinBox_studytime->setMaximum(24);

        horizontalLayout->addWidget(doubleSpinBox_studytime);

        label_3 = new QLabel(groupBox_docattribute);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);


        horizontalLayout_4->addLayout(horizontalLayout);


        gridLayout->addLayout(horizontalLayout_4, 2, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(groupBox_docattribute);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_3->addWidget(label_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        spinBox_studypoints = new QSpinBox(groupBox_docattribute);
        spinBox_studypoints->setObjectName(QString::fromUtf8("spinBox_studypoints"));

        horizontalLayout_2->addWidget(spinBox_studypoints);

        label_5 = new QLabel(groupBox_docattribute);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_2->addWidget(label_5);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        gridLayout->addLayout(horizontalLayout_3, 2, 1, 1, 1);

        label_7 = new QLabel(groupBox_docattribute);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMaximumSize(QSize(125, 16777215));

        gridLayout->addWidget(label_7, 0, 0, 1, 1);

        label_docname = new QLabel(groupBox_docattribute);
        label_docname->setObjectName(QString::fromUtf8("label_docname"));

        gridLayout->addWidget(label_docname, 0, 1, 1, 1);


        gridLayout_3->addWidget(groupBox_docattribute, 2, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_7);

        pushButton_OK = new QPushButton(frame);
        pushButton_OK->setObjectName(QString::fromUtf8("pushButton_OK"));

        horizontalLayout_8->addWidget(pushButton_OK);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_8);

        pushButton_CANCEL = new QPushButton(frame);
        pushButton_CANCEL->setObjectName(QString::fromUtf8("pushButton_CANCEL"));

        horizontalLayout_8->addWidget(pushButton_CANCEL);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_9);


        gridLayout_3->addLayout(horizontalLayout_8, 4, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_3->addItem(verticalSpacer, 3, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_3->addItem(verticalSpacer_2, 1, 0, 1, 1);

        splitter->addWidget(frame);

        gridLayout_5->addWidget(splitter, 0, 0, 1, 1);


        retranslateUi(doc_manage);

        QMetaObject::connectSlotsByName(doc_manage);
    } // setupUi

    void retranslateUi(QDialog *doc_manage)
    {
        doc_manage->setWindowTitle(QApplication::translate("doc_manage", "\345\255\246\344\271\240\346\226\207\346\241\243\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("doc_manage", "\345\255\246\344\271\240\346\226\207\346\241\243\345\272\223\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        pushButtonLoadAll->setText(QApplication::translate("doc_manage", "\350\275\275\345\205\245\346\211\200\346\234\211\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
        pushButton_adddoc->setText(QApplication::translate("doc_manage", "\345\242\236\345\212\240\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
        pushButton_modifydoc->setText(QApplication::translate("doc_manage", "\344\277\256\346\224\271\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
        pushButton_deldoc->setText(QApplication::translate("doc_manage", "\345\210\240\351\231\244\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
        groupBox_docdir->setTitle(QApplication::translate("doc_manage", "\345\256\242\346\210\267\347\253\257\346\226\207\346\241\243\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        treeView_docdir->setToolTip(QApplication::translate("doc_manage", "\345\217\214\345\207\273\351\200\211\346\213\251\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupBox_docattribute->setTitle(QApplication::translate("doc_manage", "\350\256\276\347\275\256\346\226\207\346\241\243\345\261\236\346\200\247", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("doc_manage", "\346\226\207\346\241\243\347\261\273\345\210\253", 0, QApplication::UnicodeUTF8));
        comboBox_doctype->clear();
        comboBox_doctype->insertItems(0, QStringList()
         << QApplication::translate("doc_manage", "\350\257\267\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8)
        );
        label_6->setText(QApplication::translate("doc_manage", "\351\200\202\347\224\250\345\267\245\347\247\215", 0, QApplication::UnicodeUTF8));
        comboBox_usertype->clear();
        comboBox_usertype->insertItems(0, QStringList()
         << QApplication::translate("doc_manage", "\350\257\267\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("doc_manage", "\346\226\260\345\221\230\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("doc_manage", "\345\210\235\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("doc_manage", "\344\270\255\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("doc_manage", "\351\253\230\347\272\247\345\267\245", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("doc_manage", "\346\212\200\345\270\210", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("doc_manage", "\351\253\230\347\272\247\346\212\200\345\270\210", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("doc_manage", "\345\255\246\344\271\240\346\227\266\351\225\277", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("doc_manage", "\345\260\217\346\227\266", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("doc_manage", "\346\211\200\350\216\267\345\255\246\345\210\206", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("doc_manage", "\345\255\246\345\210\206", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("doc_manage", "\345\275\223\345\211\215\351\200\211\345\256\232\346\226\207\346\241\243\357\274\232", 0, QApplication::UnicodeUTF8));
        label_docname->setText(QApplication::translate("doc_manage", "\346\234\252\351\200\211\345\256\232\346\226\207\346\241\243\357\274\210\345\217\214\345\207\273\346\226\207\346\241\243\351\200\211\344\270\255\357\274\211", 0, QApplication::UnicodeUTF8));
        pushButton_OK->setText(QApplication::translate("doc_manage", "   \345\272\224    \347\224\250   ", 0, QApplication::UnicodeUTF8));
        pushButton_CANCEL->setText(QApplication::translate("doc_manage", "  \345\217\226     \346\266\210  ", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(doc_manage);
    } // retranslateUi

};

namespace Ui {
    class doc_manage: public Ui_doc_manage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOC_MANAGE_H
