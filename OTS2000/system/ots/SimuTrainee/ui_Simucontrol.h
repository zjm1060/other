/********************************************************************************
** Form generated from reading ui file 'Simucontrol.ui'
**
** Created: Wed Jan 14 10:22:34 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SIMUCONTROL_H
#define UI_SIMUCONTROL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Simucontrol
{
public:
    QGroupBox *groupBox;
    QLabel *label;
    QSlider *m_simuctr_horizontalSlider;
    QLabel *m_simuctr_label;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *buttonStart;
    QSpacerItem *verticalSpacer_2;
    QPushButton *buttonpause;
    QSpacerItem *verticalSpacer_3;
    QPushButton *buttonStop;
    QSpacerItem *verticalSpacer_4;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_APPLY;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *m_simuctr_buttonEnter;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *m_simuctr_buttonCancel;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *Simucontrol)
    {
        if (Simucontrol->objectName().isEmpty())
            Simucontrol->setObjectName(QString::fromUtf8("Simucontrol"));
        Simucontrol->resize(564, 624);
        Simucontrol->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";\n"
"background-color: rgb(231, 240, 245);"));
        groupBox = new QGroupBox(Simucontrol);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(40, 10, 200, 550));
        groupBox->setMinimumSize(QSize(200, 550));
        groupBox->setStyleSheet(QString::fromUtf8("font: 75 16pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(255, 0, 0);\n"
"background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 100), stop:0.5 rgba(224, 234, 245, 100), stop:1 rgba(255, 255, 255, 100));\n"
" border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    padding: 2px;"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 500, 81, 31));
        label->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);\n"
"background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 0), stop:0.5 rgba(224, 234, 245, 0), stop:1 rgba(255, 255, 255, 0));\n"
"    border-width:0px;"));
        m_simuctr_horizontalSlider = new QSlider(groupBox);
        m_simuctr_horizontalSlider->setObjectName(QString::fromUtf8("m_simuctr_horizontalSlider"));
        m_simuctr_horizontalSlider->setGeometry(QRect(20, 39, 131, 441));
        m_simuctr_horizontalSlider->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0.0284091 rgba(221, 221, 221, 255), stop:0.511364 rgba(196, 196, 196, 255), stop:1 rgba(255, 255, 255, 255));\n"
""));
        m_simuctr_horizontalSlider->setOrientation(Qt::Vertical);
        m_simuctr_label = new QLabel(groupBox);
        m_simuctr_label->setObjectName(QString::fromUtf8("m_simuctr_label"));
        m_simuctr_label->setGeometry(QRect(120, 502, 54, 21));
        m_simuctr_label->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);\n"
"background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 0), stop:0.5 rgba(224, 234, 245, 0), stop:1 rgba(255, 255, 255, 0));\n"
"    border-width:0px;"));
        groupBox_2 = new QGroupBox(Simucontrol);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(300, 10, 200, 550));
        groupBox_2->setMinimumSize(QSize(200, 550));
        groupBox_2->setStyleSheet(QString::fromUtf8("font: 75 16pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(255, 0, 0);\n"
"background-color: qlineargradient(spread:pad, x1:0.528364, y1:1, x2:0.511, y2:0.0113636, stop:0 rgba(255, 255, 255, 100), stop:0.5 rgba(224, 234, 245, 100), stop:1 rgba(255, 255, 255, 100));\n"
" border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    padding: 2px;"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalSpacer = new QSpacerItem(20, 81, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 0, 0, 1, 1);

        buttonStart = new QPushButton(groupBox_2);
        buttonStart->setObjectName(QString::fromUtf8("buttonStart"));
        buttonStart->setMinimumSize(QSize(0, 51));
        buttonStart->setMaximumSize(QSize(100, 16777215));
        buttonStart->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));

        gridLayout->addWidget(buttonStart, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 82, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 2, 0, 1, 1);

        buttonpause = new QPushButton(groupBox_2);
        buttonpause->setObjectName(QString::fromUtf8("buttonpause"));
        buttonpause->setMinimumSize(QSize(0, 51));
        buttonpause->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));

        gridLayout->addWidget(buttonpause, 3, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 81, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 4, 0, 1, 1);

        buttonStop = new QPushButton(groupBox_2);
        buttonStop->setObjectName(QString::fromUtf8("buttonStop"));
        buttonStop->setMinimumSize(QSize(0, 51));
        buttonStop->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));

        gridLayout->addWidget(buttonStop, 5, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 81, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_4, 6, 0, 1, 1);

        widget = new QWidget(Simucontrol);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(40, 590, 318, 26));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_APPLY = new QPushButton(widget);
        pushButton_APPLY->setObjectName(QString::fromUtf8("pushButton_APPLY"));
        pushButton_APPLY->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));

        horizontalLayout->addWidget(pushButton_APPLY);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        m_simuctr_buttonEnter = new QPushButton(widget);
        m_simuctr_buttonEnter->setObjectName(QString::fromUtf8("m_simuctr_buttonEnter"));
        m_simuctr_buttonEnter->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));

        horizontalLayout->addWidget(m_simuctr_buttonEnter);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        m_simuctr_buttonCancel = new QPushButton(widget);
        m_simuctr_buttonCancel->setObjectName(QString::fromUtf8("m_simuctr_buttonCancel"));
        m_simuctr_buttonCancel->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius:2;\n"
"    padding: 2px;\n"
" \n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));

        horizontalLayout->addWidget(m_simuctr_buttonCancel);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        retranslateUi(Simucontrol);

        QMetaObject::connectSlotsByName(Simucontrol);
    } // setupUi

    void retranslateUi(QWidget *Simucontrol)
    {
        Simucontrol->setWindowTitle(QApplication::translate("Simucontrol", "\344\273\277\347\234\237\346\216\247\345\210\266", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Simucontrol", "\351\200\237\345\272\246\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Simucontrol", "\345\275\223\345\211\215\351\200\237\345\272\246", 0, QApplication::UnicodeUTF8));
        m_simuctr_label->setText(QString());
        groupBox_2->setTitle(QApplication::translate("Simucontrol", "\344\273\277\347\234\237\346\216\247\345\210\266", 0, QApplication::UnicodeUTF8));
        buttonStart->setText(QApplication::translate("Simucontrol", "\344\273\277\347\234\237\345\274\200\345\247\213", 0, QApplication::UnicodeUTF8));
        buttonpause->setText(QApplication::translate("Simucontrol", "\344\273\277\347\234\237\346\232\202\345\201\234", 0, QApplication::UnicodeUTF8));
        buttonStop->setText(QApplication::translate("Simucontrol", "\344\273\277\347\234\237\345\201\234\346\255\242", 0, QApplication::UnicodeUTF8));
        pushButton_APPLY->setText(QApplication::translate("Simucontrol", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        m_simuctr_buttonEnter->setText(QApplication::translate("Simucontrol", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        m_simuctr_buttonCancel->setText(QApplication::translate("Simucontrol", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Simucontrol);
    } // retranslateUi

};

namespace Ui {
    class Simucontrol: public Ui_Simucontrol {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMUCONTROL_H
