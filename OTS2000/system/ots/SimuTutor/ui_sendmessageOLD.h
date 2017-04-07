/********************************************************************************
** Form generated from reading ui file 'sendmessageOLD.ui'
**
** Created: Thu Jan 15 10:28:34 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SENDMESSAGEOLD_H
#define UI_SENDMESSAGEOLD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
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

QT_BEGIN_NAMESPACE

class Ui_CSendMessageTable
{
public:
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout;
    QTextEdit *editMsgShow;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *buttonFont;
    QToolButton *buttonColor;
    QToolButton *buttonImage;
    QToolButton *buttonShake;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *buttonMsgRec;
    QTextEdit *editMsgSend;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QToolButton *buttonExit;
    QToolButton *buttonSend;
    QGroupBox *groupAllUser;
    QGridLayout *gridLayout_2;
    QPushButton *buttonAddUser;
    QPushButton *buttonAddUserAll;
    QListView *listAllUser;
    QGroupBox *groupSendUser;
    QGridLayout *gridLayout;
    QListView *listSendUser;
    QPushButton *buttonDelUser;
    QPushButton *buttonDelUserAll;
    QPushButton *buttonHideAllUser;
    QTextEdit *editMsgRec;

    void setupUi(QDialog *CSendMessageTable)
    {
        if (CSendMessageTable->objectName().isEmpty())
            CSendMessageTable->setObjectName(QString::fromUtf8("CSendMessageTable"));
        CSendMessageTable->resize(600, 500);
        CSendMessageTable->setMinimumSize(QSize(600, 500));
        CSendMessageTable->setStyleSheet(QString::fromUtf8(""));
        gridLayout_3 = new QGridLayout(CSendMessageTable);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setSizeConstraint(QLayout::SetFixedSize);
        gridLayout_3->setContentsMargins(0, 0, 0, -1);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
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

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(15);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        buttonFont = new QToolButton(CSendMessageTable);
        buttonFont->setObjectName(QString::fromUtf8("buttonFont"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/pic/images/\345\255\227\344\275\223.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        buttonFont->setIcon(icon);
        buttonFont->setIconSize(QSize(20, 20));
        buttonFont->setAutoRaise(true);

        horizontalLayout_3->addWidget(buttonFont);

        buttonColor = new QToolButton(CSendMessageTable);
        buttonColor->setObjectName(QString::fromUtf8("buttonColor"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/pic/images/\351\242\234\350\211\262.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        buttonColor->setIcon(icon1);
        buttonColor->setIconSize(QSize(20, 20));
        buttonColor->setAutoRaise(true);

        horizontalLayout_3->addWidget(buttonColor);

        buttonImage = new QToolButton(CSendMessageTable);
        buttonImage->setObjectName(QString::fromUtf8("buttonImage"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/pic/images/\345\233\276\347\211\207.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        buttonImage->setIcon(icon2);
        buttonImage->setIconSize(QSize(20, 20));
        buttonImage->setCheckable(true);
        buttonImage->setAutoRaise(true);

        horizontalLayout_3->addWidget(buttonImage);

        buttonShake = new QToolButton(CSendMessageTable);
        buttonShake->setObjectName(QString::fromUtf8("buttonShake"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/pic/images/\346\231\203\345\212\250.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        buttonShake->setIcon(icon3);
        buttonShake->setIconSize(QSize(20, 20));
        buttonShake->setAutoRaise(true);

        horizontalLayout_3->addWidget(buttonShake);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        buttonMsgRec = new QToolButton(CSendMessageTable);
        buttonMsgRec->setObjectName(QString::fromUtf8("buttonMsgRec"));
        buttonMsgRec->setEnabled(true);
        buttonMsgRec->setMinimumSize(QSize(100, 30));
        buttonMsgRec->setCheckable(true);
        buttonMsgRec->setAutoRaise(true);

        horizontalLayout_3->addWidget(buttonMsgRec);


        verticalLayout->addLayout(horizontalLayout_3);

        editMsgSend = new QTextEdit(CSendMessageTable);
        editMsgSend->setObjectName(QString::fromUtf8("editMsgSend"));
        sizePolicy.setHeightForWidth(editMsgSend->sizePolicy().hasHeightForWidth());
        editMsgSend->setSizePolicy(sizePolicy);
        editMsgSend->setMinimumSize(QSize(404, 0));
        editMsgSend->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";"));
        editMsgSend->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(editMsgSend);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(40);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 9, 0, -1);
        label = new QLabel(CSendMessageTable);
        label->setObjectName(QString::fromUtf8("label"));
        label->setEnabled(false);

        horizontalLayout->addWidget(label);

        buttonExit = new QToolButton(CSendMessageTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setMinimumSize(QSize(0, 28));
        buttonExit->setMaximumSize(QSize(16777215, 16777215));
        buttonExit->setBaseSize(QSize(0, 0));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonExit);

        buttonSend = new QToolButton(CSendMessageTable);
        buttonSend->setObjectName(QString::fromUtf8("buttonSend"));
        buttonSend->setMinimumSize(QSize(0, 28));
        buttonSend->setMaximumSize(QSize(80, 16777215));
        buttonSend->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(buttonSend);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 7);
        verticalLayout->setStretch(2, 3);

        gridLayout_3->addLayout(verticalLayout, 0, 0, 2, 1);

        groupAllUser = new QGroupBox(CSendMessageTable);
        groupAllUser->setObjectName(QString::fromUtf8("groupAllUser"));
        groupAllUser->setMaximumSize(QSize(170, 16777215));
        groupAllUser->setStyleSheet(QString::fromUtf8(""));
        gridLayout_2 = new QGridLayout(groupAllUser);
        gridLayout_2->setMargin(0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setVerticalSpacing(0);
        buttonAddUser = new QPushButton(groupAllUser);
        buttonAddUser->setObjectName(QString::fromUtf8("buttonAddUser"));

        gridLayout_2->addWidget(buttonAddUser, 1, 0, 1, 1);

        buttonAddUserAll = new QPushButton(groupAllUser);
        buttonAddUserAll->setObjectName(QString::fromUtf8("buttonAddUserAll"));

        gridLayout_2->addWidget(buttonAddUserAll, 1, 1, 1, 1);

        listAllUser = new QListView(groupAllUser);
        listAllUser->setObjectName(QString::fromUtf8("listAllUser"));

        gridLayout_2->addWidget(listAllUser, 0, 0, 1, 2);


        gridLayout_3->addWidget(groupAllUser, 0, 1, 1, 1);

        groupSendUser = new QGroupBox(CSendMessageTable);
        groupSendUser->setObjectName(QString::fromUtf8("groupSendUser"));
        groupSendUser->setMaximumSize(QSize(170, 16777215));
        gridLayout = new QGridLayout(groupSendUser);
        gridLayout->setMargin(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setVerticalSpacing(0);
        listSendUser = new QListView(groupSendUser);
        listSendUser->setObjectName(QString::fromUtf8("listSendUser"));

        gridLayout->addWidget(listSendUser, 1, 0, 1, 2);

        buttonDelUser = new QPushButton(groupSendUser);
        buttonDelUser->setObjectName(QString::fromUtf8("buttonDelUser"));

        gridLayout->addWidget(buttonDelUser, 2, 0, 1, 1);

        buttonDelUserAll = new QPushButton(groupSendUser);
        buttonDelUserAll->setObjectName(QString::fromUtf8("buttonDelUserAll"));

        gridLayout->addWidget(buttonDelUserAll, 2, 1, 1, 1);

        buttonHideAllUser = new QPushButton(groupSendUser);
        buttonHideAllUser->setObjectName(QString::fromUtf8("buttonHideAllUser"));
        buttonHideAllUser->setMaximumSize(QSize(16777215, 20));
        buttonHideAllUser->setStyleSheet(QString::fromUtf8("font: 8pt \"\345\256\213\344\275\223\";\n"
"background-color: rgb(56, 255, 249);"));

        gridLayout->addWidget(buttonHideAllUser, 0, 0, 1, 2);


        gridLayout_3->addWidget(groupSendUser, 1, 1, 1, 1);

        editMsgRec = new QTextEdit(CSendMessageTable);
        editMsgRec->setObjectName(QString::fromUtf8("editMsgRec"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(editMsgRec->sizePolicy().hasHeightForWidth());
        editMsgRec->setSizePolicy(sizePolicy1);
        editMsgRec->setMinimumSize(QSize(320, 0));
        editMsgRec->setMaximumSize(QSize(350, 16777215));

        gridLayout_3->addWidget(editMsgRec, 0, 2, 2, 1);

        QWidget::setTabOrder(editMsgShow, editMsgSend);

        retranslateUi(CSendMessageTable);

        QMetaObject::connectSlotsByName(CSendMessageTable);
    } // setupUi

    void retranslateUi(QDialog *CSendMessageTable)
    {
        CSendMessageTable->setWindowTitle(QApplication::translate("CSendMessageTable", "\347\237\255\346\266\210\346\201\257", 0, QApplication::UnicodeUTF8));
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
        buttonExit->setText(QApplication::translate("CSendMessageTable", "\345\205\263\351\227\255(C)", 0, QApplication::UnicodeUTF8));
        buttonSend->setText(QApplication::translate("CSendMessageTable", "\345\217\221\351\200\201(S)", 0, QApplication::UnicodeUTF8));
        groupAllUser->setTitle(QApplication::translate("CSendMessageTable", "\346\211\200\346\234\211\347\224\250\346\210\267\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        buttonAddUser->setText(QApplication::translate("CSendMessageTable", "\346\267\273\345\212\240\346\211\200\351\200\211\347\224\250\346\210\267", 0, QApplication::UnicodeUTF8));
        buttonAddUserAll->setText(QApplication::translate("CSendMessageTable", "\346\267\273\345\212\240\346\211\200\346\234\211\347\224\250\346\210\267", 0, QApplication::UnicodeUTF8));
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

#endif // UI_SENDMESSAGEOLD_H
