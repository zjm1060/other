/********************************************************************************
** Form generated from reading ui file 'sendmessage.ui'
**
** Created: Fri Jan 16 16:07:06 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SENDMESSAGE_H
#define UI_SENDMESSAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CSendMessageTable
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QTextEdit *editMsgShow;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *buttonFont;
    QToolButton *buttonColor;
    QToolButton *buttonImage;
    QToolButton *buttonShake;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *buttonMsgRec;
    QTextEdit *editMsgSend;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *comboBox;
    QToolButton *buttonExit;
    QToolButton *buttonSend;
    QGroupBox *groupAllUser;
    QGridLayout *gridLayout_5;
    QPushButton *buttonAddUser;
    QPushButton *buttonAddUserAll;
    QListView *listAllUser;
    QTextEdit *editMsgRec;
    QGroupBox *groupSendUser;
    QGridLayout *gridLayout_3;
    QListView *listSendUser;
    QPushButton *buttonDelUser;
    QPushButton *buttonDelUserAll;
    QPushButton *buttonHideAllUser;

    void setupUi(QWidget *CSendMessageTable)
    {
        if (CSendMessageTable->objectName().isEmpty())
            CSendMessageTable->setObjectName(QString::fromUtf8("CSendMessageTable"));
        CSendMessageTable->resize(1039, 579);
        gridLayout = new QGridLayout(CSendMessageTable);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        editMsgShow = new QTextEdit(CSendMessageTable);
        editMsgShow->setObjectName(QString::fromUtf8("editMsgShow"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(editMsgShow->sizePolicy().hasHeightForWidth());
        editMsgShow->setSizePolicy(sizePolicy);
        editMsgShow->setMinimumSize(QSize(404, 228));
        editMsgShow->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(editMsgShow);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(15);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        buttonFont = new QToolButton(CSendMessageTable);
        buttonFont->setObjectName(QString::fromUtf8("buttonFont"));
        buttonFont->setStyleSheet(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/pic/images/\345\255\227\344\275\223.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        buttonFont->setIcon(icon);
        buttonFont->setIconSize(QSize(20, 20));
        buttonFont->setAutoRaise(true);

        horizontalLayout_4->addWidget(buttonFont);

        buttonColor = new QToolButton(CSendMessageTable);
        buttonColor->setObjectName(QString::fromUtf8("buttonColor"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/pic/images/\351\242\234\350\211\262.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        buttonColor->setIcon(icon1);
        buttonColor->setIconSize(QSize(20, 20));
        buttonColor->setAutoRaise(true);

        horizontalLayout_4->addWidget(buttonColor);

        buttonImage = new QToolButton(CSendMessageTable);
        buttonImage->setObjectName(QString::fromUtf8("buttonImage"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/pic/images/\345\233\276\347\211\207.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        buttonImage->setIcon(icon2);
        buttonImage->setIconSize(QSize(20, 20));
        buttonImage->setCheckable(true);
        buttonImage->setAutoRaise(true);

        horizontalLayout_4->addWidget(buttonImage);

        buttonShake = new QToolButton(CSendMessageTable);
        buttonShake->setObjectName(QString::fromUtf8("buttonShake"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/pic/images/\346\231\203\345\212\250.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        buttonShake->setIcon(icon3);
        buttonShake->setIconSize(QSize(20, 20));
        buttonShake->setAutoRaise(true);

        horizontalLayout_4->addWidget(buttonShake);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        buttonMsgRec = new QToolButton(CSendMessageTable);
        buttonMsgRec->setObjectName(QString::fromUtf8("buttonMsgRec"));
        buttonMsgRec->setEnabled(true);
        buttonMsgRec->setMinimumSize(QSize(100, 30));
        buttonMsgRec->setCheckable(true);
        buttonMsgRec->setAutoRaise(true);

        horizontalLayout_4->addWidget(buttonMsgRec);


        verticalLayout->addLayout(horizontalLayout_4);

        editMsgSend = new QTextEdit(CSendMessageTable);
        editMsgSend->setObjectName(QString::fromUtf8("editMsgSend"));
        sizePolicy.setHeightForWidth(editMsgSend->sizePolicy().hasHeightForWidth());
        editMsgSend->setSizePolicy(sizePolicy);
        editMsgSend->setMinimumSize(QSize(404, 0));
        editMsgSend->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";"));
        editMsgSend->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(editMsgSend);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 9, 0, -1);
        label = new QLabel(CSendMessageTable);
        label->setObjectName(QString::fromUtf8("label"));
        label->setEnabled(false);
        label->setMaximumSize(QSize(90, 16777215));

        horizontalLayout_2->addWidget(label);

        comboBox = new QComboBox(CSendMessageTable);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setMinimumSize(QSize(180, 0));

        horizontalLayout_2->addWidget(comboBox);

        buttonExit = new QToolButton(CSendMessageTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setMinimumSize(QSize(0, 28));
        buttonExit->setMaximumSize(QSize(16777215, 16777215));
        buttonExit->setBaseSize(QSize(0, 0));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(buttonExit);

        buttonSend = new QToolButton(CSendMessageTable);
        buttonSend->setObjectName(QString::fromUtf8("buttonSend"));
        buttonSend->setMinimumSize(QSize(0, 28));
        buttonSend->setMaximumSize(QSize(80, 16777215));
        buttonSend->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(buttonSend);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 0, 2, 1);

        groupAllUser = new QGroupBox(CSendMessageTable);
        groupAllUser->setObjectName(QString::fromUtf8("groupAllUser"));
        groupAllUser->setMinimumSize(QSize(210, 0));
        groupAllUser->setMaximumSize(QSize(170, 16777215));
        groupAllUser->setStyleSheet(QString::fromUtf8(""));
        gridLayout_5 = new QGridLayout(groupAllUser);
        gridLayout_5->setMargin(0);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setVerticalSpacing(0);
        buttonAddUser = new QPushButton(groupAllUser);
        buttonAddUser->setObjectName(QString::fromUtf8("buttonAddUser"));

        gridLayout_5->addWidget(buttonAddUser, 1, 0, 1, 1);

        buttonAddUserAll = new QPushButton(groupAllUser);
        buttonAddUserAll->setObjectName(QString::fromUtf8("buttonAddUserAll"));

        gridLayout_5->addWidget(buttonAddUserAll, 1, 1, 1, 1);

        listAllUser = new QListView(groupAllUser);
        listAllUser->setObjectName(QString::fromUtf8("listAllUser"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(listAllUser->sizePolicy().hasHeightForWidth());
        listAllUser->setSizePolicy(sizePolicy1);

        gridLayout_5->addWidget(listAllUser, 0, 0, 1, 2);


        gridLayout->addWidget(groupAllUser, 0, 1, 1, 1);

        editMsgRec = new QTextEdit(CSendMessageTable);
        editMsgRec->setObjectName(QString::fromUtf8("editMsgRec"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(editMsgRec->sizePolicy().hasHeightForWidth());
        editMsgRec->setSizePolicy(sizePolicy2);
        editMsgRec->setMinimumSize(QSize(320, 0));
        editMsgRec->setMaximumSize(QSize(350, 16777215));

        gridLayout->addWidget(editMsgRec, 0, 2, 2, 1);

        groupSendUser = new QGroupBox(CSendMessageTable);
        groupSendUser->setObjectName(QString::fromUtf8("groupSendUser"));
        groupSendUser->setMinimumSize(QSize(210, 0));
        groupSendUser->setMaximumSize(QSize(170, 16777215));
        gridLayout_3 = new QGridLayout(groupSendUser);
        gridLayout_3->setMargin(0);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setVerticalSpacing(0);
        listSendUser = new QListView(groupSendUser);
        listSendUser->setObjectName(QString::fromUtf8("listSendUser"));

        gridLayout_3->addWidget(listSendUser, 1, 0, 1, 2);

        buttonDelUser = new QPushButton(groupSendUser);
        buttonDelUser->setObjectName(QString::fromUtf8("buttonDelUser"));

        gridLayout_3->addWidget(buttonDelUser, 2, 0, 1, 1);

        buttonDelUserAll = new QPushButton(groupSendUser);
        buttonDelUserAll->setObjectName(QString::fromUtf8("buttonDelUserAll"));

        gridLayout_3->addWidget(buttonDelUserAll, 2, 1, 1, 1);

        buttonHideAllUser = new QPushButton(groupSendUser);
        buttonHideAllUser->setObjectName(QString::fromUtf8("buttonHideAllUser"));
        buttonHideAllUser->setMaximumSize(QSize(16777215, 20));
        buttonHideAllUser->setStyleSheet(QString::fromUtf8("font: 8pt \"\345\256\213\344\275\223\";\n"
"background-color: rgb(56, 255, 249);"));

        gridLayout_3->addWidget(buttonHideAllUser, 0, 0, 1, 2);


        gridLayout->addWidget(groupSendUser, 1, 1, 1, 1);


        retranslateUi(CSendMessageTable);

        QMetaObject::connectSlotsByName(CSendMessageTable);
    } // setupUi

    void retranslateUi(QWidget *CSendMessageTable)
    {
        CSendMessageTable->setWindowTitle(QApplication::translate("CSendMessageTable", "\345\215\263\346\227\266\346\266\210\346\201\257", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonFont->setToolTip(QApplication::translate("CSendMessageTable", "\345\255\227\344\275\223\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        buttonFont->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonColor->setToolTip(QApplication::translate("CSendMessageTable", "\351\242\234\350\211\262\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        buttonColor->setText(QString());
        buttonImage->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonShake->setToolTip(QApplication::translate("CSendMessageTable", "\345\217\221\351\200\201\347\252\227\345\217\243\346\212\226\345\212\250", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        buttonShake->setText(QString());
        buttonMsgRec->setStyleSheet(QApplication::translate("CSendMessageTable", "font: 11pt \"\345\256\213\344\275\223\";", 0, QApplication::UnicodeUTF8));
        buttonMsgRec->setText(QApplication::translate("CSendMessageTable", "\346\266\210\346\201\257\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CSendMessageTable", "   F5\357\274\232\345\210\267\346\226\260\347\224\250\346\210\267", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("CSendMessageTable", "\346\217\222\345\205\245\345\270\270\347\224\250\350\257\255", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CSendMessageTable", "\345\255\246\345\221\230\346\224\266\345\210\260\345\217\243\344\273\244\350\257\267\345\233\236\345\244\215", 0, QApplication::UnicodeUTF8)
         << QString()
        );
        buttonExit->setText(QApplication::translate("CSendMessageTable", "\345\205\263\351\227\255(C)", 0, QApplication::UnicodeUTF8));
        buttonSend->setText(QApplication::translate("CSendMessageTable", "\345\217\221\351\200\201(S)", 0, QApplication::UnicodeUTF8));
        groupAllUser->setTitle(QApplication::translate("CSendMessageTable", "\346\211\200\346\234\211\347\224\250\346\210\267\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        buttonAddUser->setText(QApplication::translate("CSendMessageTable", "\346\267\273\345\212\240\346\211\200\351\200\211\347\224\250\346\210\267", 0, QApplication::UnicodeUTF8));
        buttonAddUserAll->setText(QApplication::translate("CSendMessageTable", "\346\267\273\345\212\240\346\211\200\346\234\211\347\224\250\346\210\267", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        listAllUser->setToolTip(QApplication::translate("CSendMessageTable", "\350\257\267\345\217\214\345\207\273\351\200\211\346\213\251\345\271\266\346\267\273\345\212\240\345\210\260\345\257\271\350\257\235\347\224\250\346\210\267\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupSendUser->setTitle(QApplication::translate("CSendMessageTable", "\345\217\221\351\200\201\347\224\250\346\210\267\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        buttonDelUser->setText(QApplication::translate("CSendMessageTable", "\347\247\273\351\231\244\346\211\200\351\200\211\347\224\250\346\210\267", 0, QApplication::UnicodeUTF8));
        buttonDelUserAll->setText(QApplication::translate("CSendMessageTable", "\347\247\273\351\231\244\346\211\200\346\234\211\347\224\250\346\210\267", 0, QApplication::UnicodeUTF8));
        buttonHideAllUser->setText(QApplication::translate("CSendMessageTable", "\351\232\220\350\227\217\346\211\200\346\234\211\347\224\250\346\210\267\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CSendMessageTable);
    } // retranslateUi

};

namespace Ui {
    class CSendMessageTable: public Ui_CSendMessageTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENDMESSAGE_H
