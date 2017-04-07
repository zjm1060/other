/********************************************************************************
** Form generated from reading ui file 'WaitContest.ui'
**
** Created: Wed Jan 14 10:22:37 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WAITCONTEST_H
#define UI_WAITCONTEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WaitContest
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonOK;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonCancel;
    QSpacerItem *horizontalSpacer_3;
    QTextBrowser *textBrowser;
    QLabel *label;
    QTextBrowser *textBrowser_curContest;

    void setupUi(QWidget *WaitContest)
    {
        if (WaitContest->objectName().isEmpty())
            WaitContest->setObjectName(QString::fromUtf8("WaitContest"));
        WaitContest->resize(745, 676);
        WaitContest->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";\n"
"background-color: rgb(231, 240, 245);"));
        gridLayout = new QGridLayout(WaitContest);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonOK = new QPushButton(WaitContest);
        pushButtonOK->setObjectName(QString::fromUtf8("pushButtonOK"));
        pushButtonOK->setMinimumSize(QSize(80, 0));
        pushButtonOK->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius: 2;\n"
"    padding: 2px;\n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);font: 75 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(pushButtonOK);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButtonCancel = new QPushButton(WaitContest);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));
        pushButtonCancel->setMinimumSize(QSize(80, 0));
        pushButtonCancel->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius: 2;\n"
"    padding: 2px;\n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);font: 75 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(pushButtonCancel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        gridLayout->addLayout(horizontalLayout, 3, 0, 1, 1);

        textBrowser = new QTextBrowser(WaitContest);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius: 2;\n"
"    padding: 2px;\n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));
        textBrowser->setUndoRedoEnabled(false);
        textBrowser->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse);

        gridLayout->addWidget(textBrowser, 0, 0, 1, 1);

        label = new QLabel(WaitContest);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";\n"
"font: 75 16pt \"\345\256\213\344\275\223\";\n"
"color: rgb(85, 0, 255);"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        textBrowser_curContest = new QTextBrowser(WaitContest);
        textBrowser_curContest->setObjectName(QString::fromUtf8("textBrowser_curContest"));
        textBrowser_curContest->setStyleSheet(QString::fromUtf8("background-color: qradialgradient(spread:pad, cx:0.272727, cy:0.352, radius:1.088, fx:0, fy:0, stop:0 rgba(209, 209, 209, 255), stop:0.480226 rgba(207, 207, 207, 255), stop:1 rgba(230, 230, 230, 255));"));

        gridLayout->addWidget(textBrowser_curContest, 2, 0, 1, 1);


        retranslateUi(WaitContest);

        QMetaObject::connectSlotsByName(WaitContest);
    } // setupUi

    void retranslateUi(QWidget *WaitContest)
    {
        WaitContest->setWindowTitle(QApplication::translate("WaitContest", "\347\253\236\350\265\233\346\217\220\347\244\272", 0, QApplication::UnicodeUTF8));
        pushButtonOK->setText(QApplication::translate("WaitContest", "\347\241\256  \350\256\244", 0, QApplication::UnicodeUTF8));
        pushButtonCancel->setText(QApplication::translate("WaitContest", "\345\217\226  \346\266\210", 0, QApplication::UnicodeUTF8));
        textBrowser->setHtml(QApplication::translate("WaitContest", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:18pt; font-weight:600; color:#ff0000;\">\347\253\236\350\265\233\346\250\241\345\274\217\346\223\215\344\275\234\346\217\220\347\244\272\357\274\232</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'SimSun'; font-size:18pt; font-weight:600; color:#ff0000;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px"
                        "; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:18pt;\">    \347\253\236\350\265\233\346\250\241\345\274\217\344\270\255\357\274\214\350\213\245\344\270\272\346\210\220\347\273\204\347\253\236\350\265\233\346\250\241\345\274\217\357\274\214\345\210\231\350\246\201\347\224\261</span><span style=\" font-family:'SimSun'; font-size:18pt; font-weight:600; text-decoration: underline;\">\347\273\204\351\225\277\346\216\245\345\217\227\344\273\273\345\212\241\345\271\266\345\220\257\345\212\250\344\273\273\345\212\241</span><span style=\" font-family:'SimSun'; font-size:18pt;\">\357\274\214\347\273\204\345\221\230\346\262\241\346\234\211\344\273\277\347\234\237\346\216\247\345\210\266\346\235\203\357\274\214\345\217\252\346\234\211\344\273\277\347\234\237\347\263\273\347\273\237\346\223\215\344\275\234\346\235\203\357\274\214\347\273\204\345\221\230\345\234\250\347\273\204\351\225\277\346\216\245\345\210\260\345\271\266\344\270\224\345\220\257\345\212\250\344\273\273\345\212\241"
                        "\345\220\216\357\274\214\347\273\204\345\206\205\345\205\250\344\275\223\346\210\220\345\221\230\344\272\222\347\233\270\351\205\215\345\220\210\357\274\214\345\205\261\345\220\214\345\256\214\346\210\220\345\220\214\344\270\200\344\273\273\345\212\241\357\274\214\344\273\273\345\212\241\345\256\214\346\210\220\345\220\216</span><span style=\" font-family:'SimSun'; font-size:18pt; font-weight:600; text-decoration: underline;\">\347\224\261\347\273\204\351\225\277\346\217\220\344\272\244\345\256\214\346\210\220\344\273\273\345\212\241</span><span style=\" font-family:'SimSun'; font-size:18pt;\">\343\200\202</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'SimSun'; font-size:18pt;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:18pt;\">    \347\245\235\345\244\247"
                        "\345\256\266\345\217\226\345\276\227\345\245\275\346\210\220\347\273\251\343\200\202\342\206\226(^\317\211^)\342\206\227</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'SimSun'; font-size:18pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("WaitContest", "\345\275\223\345\211\215\345\274\200\346\224\276\344\270\255\347\232\204\347\253\236\350\265\233\357\274\232", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(WaitContest);
    } // retranslateUi

};

namespace Ui {
    class WaitContest: public Ui_WaitContest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAITCONTEST_H
