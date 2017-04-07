/********************************************************************************
** Form generated from reading ui file 'mydoc.ui'
**
** Created: Wed Jan 14 10:22:23 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MYDOC_H
#define UI_MYDOC_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Mydoc
{
public:
    QGridLayout *gridLayout_3;
    QSplitter *splitter;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QTreeView *treeView_docdir;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButtonStudy;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_MystudyRec;
    QSpacerItem *verticalSpacer;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_collapse;
    QLabel *label;
    QLabel *label_docname;
    QSpacerItem *horizontalSpacer;
    QLabel *label_3;
    QLCDNumber *lcdNumber_time;
    QPushButton *pushButton_finishdoc;
    QWidget *widget;

    void setupUi(QWidget *Mydoc)
    {
        if (Mydoc->objectName().isEmpty())
            Mydoc->setObjectName(QString::fromUtf8("Mydoc"));
        Mydoc->resize(924, 707);
        Mydoc->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        gridLayout_3 = new QGridLayout(Mydoc);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        splitter = new QSplitter(Mydoc);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        groupBox = new QGroupBox(splitter);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(5, 600));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        treeView_docdir = new QTreeView(groupBox);
        treeView_docdir->setObjectName(QString::fromUtf8("treeView_docdir"));
        treeView_docdir->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout_2->addWidget(treeView_docdir);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pushButtonStudy = new QPushButton(groupBox);
        pushButtonStudy->setObjectName(QString::fromUtf8("pushButtonStudy"));

        horizontalLayout_3->addWidget(pushButtonStudy);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        pushButton_MystudyRec = new QPushButton(groupBox);
        pushButton_MystudyRec->setObjectName(QString::fromUtf8("pushButton_MystudyRec"));

        horizontalLayout_3->addWidget(pushButton_MystudyRec);


        verticalLayout_2->addLayout(horizontalLayout_3);


        gridLayout->addLayout(verticalLayout_2, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 0, 0, 1, 1);

        splitter->addWidget(groupBox);
        frame = new QFrame(splitter);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(600, 600));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_collapse = new QPushButton(frame);
        pushButton_collapse->setObjectName(QString::fromUtf8("pushButton_collapse"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_collapse->sizePolicy().hasHeightForWidth());
        pushButton_collapse->setSizePolicy(sizePolicy);
        pushButton_collapse->setMinimumSize(QSize(24, 0));
        pushButton_collapse->setMaximumSize(QSize(25, 16777215));

        horizontalLayout->addWidget(pushButton_collapse);

        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        label_docname = new QLabel(frame);
        label_docname->setObjectName(QString::fromUtf8("label_docname"));

        horizontalLayout->addWidget(label_docname);

        horizontalSpacer = new QSpacerItem(20, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        lcdNumber_time = new QLCDNumber(frame);
        lcdNumber_time->setObjectName(QString::fromUtf8("lcdNumber_time"));
        lcdNumber_time->setMaximumSize(QSize(16777215, 40));
        lcdNumber_time->setStyleSheet(QString::fromUtf8("background-color: qradialgradient(spread:pad, cx:0.432, cy:0.589545, radius:1.722, fx:0.017, fy:0.0238737, stop:0.0734463 rgba(7, 100, 143, 255), stop:1 rgba(124, 210, 252, 255)); \n"
"color: rgb(31,217,242); "));
        lcdNumber_time->setSmallDecimalPoint(false);

        horizontalLayout->addWidget(lcdNumber_time);

        pushButton_finishdoc = new QPushButton(frame);
        pushButton_finishdoc->setObjectName(QString::fromUtf8("pushButton_finishdoc"));

        horizontalLayout->addWidget(pushButton_finishdoc);


        verticalLayout->addLayout(horizontalLayout);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(200, 600));

        verticalLayout->addWidget(widget);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

        splitter->addWidget(frame);

        gridLayout_3->addWidget(splitter, 0, 0, 1, 1);


        retranslateUi(Mydoc);

        QMetaObject::connectSlotsByName(Mydoc);
    } // setupUi

    void retranslateUi(QWidget *Mydoc)
    {
        Mydoc->setWindowTitle(QApplication::translate("Mydoc", "\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Mydoc", "\351\200\211\346\213\251\345\255\246\344\271\240\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        treeView_docdir->setToolTip(QApplication::translate("Mydoc", "\345\217\214\345\207\273\345\217\257\347\233\264\346\216\245\346\211\223\345\274\200\351\200\211\344\270\255\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushButtonStudy->setText(QApplication::translate("Mydoc", "\345\255\246\344\271\240\350\257\245\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
        pushButton_MystudyRec->setText(QApplication::translate("Mydoc", "\346\237\245\347\234\213\345\255\246\344\271\240\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        pushButton_collapse->setText(QApplication::translate("Mydoc", "<", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Mydoc", "\346\226\207\346\241\243\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_docname->setText(QString());
        label_3->setText(QApplication::translate("Mydoc", "\345\275\223\345\211\215\345\255\246\344\271\240\346\227\266\351\227\264\357\274\232", 0, QApplication::UnicodeUTF8));
        pushButton_finishdoc->setText(QApplication::translate("Mydoc", "\345\205\263\351\227\255\346\226\207\346\241\243\357\274\214\345\256\214\346\210\220\345\255\246\344\271\240", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Mydoc);
    } // retranslateUi

};

namespace Ui {
    class Mydoc: public Ui_Mydoc {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYDOC_H
