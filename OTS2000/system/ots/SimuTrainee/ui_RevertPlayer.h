/********************************************************************************
** Form generated from reading ui file 'RevertPlayer.ui'
**
** Created: Wed Jan 14 10:22:36 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_REVERTPLAYER_H
#define UI_REVERTPLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RevertPlayer
{
public:
    QGridLayout *gridLayout;
    QFrame *framePlayer;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelMsg;
    QHBoxLayout *horizontalLayout;
    QLabel *labelTime;
    QSlider *sliderProgress;
    QLabel *labelTimeTotle;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *buttonBackward;
    QPushButton *buttonStartPause;
    QPushButton *buttonForward;
    QPushButton *buttonStop;
    QHBoxLayout *horizontalLayout_2;
    QSlider *sliderSpeed;
    QLabel *labelSpeed;
    QSpacerItem *horizontalSpacer;
    QToolButton *buttonListRevert;
    QFrame *frameRevertList;
    QVBoxLayout *verticalLayout;
    QTableWidget *listRevert;

    void setupUi(QWidget *RevertPlayer)
    {
        if (RevertPlayer->objectName().isEmpty())
            RevertPlayer->setObjectName(QString::fromUtf8("RevertPlayer"));
        RevertPlayer->setWindowModality(Qt::ApplicationModal);
        RevertPlayer->resize(767, 579);
        gridLayout = new QGridLayout(RevertPlayer);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        framePlayer = new QFrame(RevertPlayer);
        framePlayer->setObjectName(QString::fromUtf8("framePlayer"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(framePlayer->sizePolicy().hasHeightForWidth());
        framePlayer->setSizePolicy(sizePolicy);
        framePlayer->setStyleSheet(QString::fromUtf8(""));
        framePlayer->setFrameShape(QFrame::StyledPanel);
        framePlayer->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(framePlayer);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setMargin(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(10);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        labelMsg = new QLabel(framePlayer);
        labelMsg->setObjectName(QString::fromUtf8("labelMsg"));
        labelMsg->setMinimumSize(QSize(0, 100));
        labelMsg->setMaximumSize(QSize(16777215, 100));
        labelMsg->setStyleSheet(QString::fromUtf8("border-image: url(:pic/images/screen.png); \n"
"border-width: 1px;\n"
"color:rgb(150, 202, 230);\n"
"border-radius:9px;    \n"
"border-width: 1px;\n"
"font: 11pt;\n"
""));

        verticalLayout_2->addWidget(labelMsg);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelTime = new QLabel(framePlayer);
        labelTime->setObjectName(QString::fromUtf8("labelTime"));
        labelTime->setMinimumSize(QSize(48, 0));
        labelTime->setMaximumSize(QSize(48, 16777215));

        horizontalLayout->addWidget(labelTime);

        sliderProgress = new QSlider(framePlayer);
        sliderProgress->setObjectName(QString::fromUtf8("sliderProgress"));
        sliderProgress->setEnabled(true);
        sliderProgress->setAcceptDrops(false);
        sliderProgress->setAutoFillBackground(false);
        sliderProgress->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgba(0, 0, 0, 255));"));
        sliderProgress->setMaximum(10000000);
        sliderProgress->setOrientation(Qt::Horizontal);
        sliderProgress->setInvertedControls(false);

        horizontalLayout->addWidget(sliderProgress);

        labelTimeTotle = new QLabel(framePlayer);
        labelTimeTotle->setObjectName(QString::fromUtf8("labelTimeTotle"));
        labelTimeTotle->setMinimumSize(QSize(48, 0));
        labelTimeTotle->setMaximumSize(QSize(48, 16777215));

        horizontalLayout->addWidget(labelTimeTotle);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        buttonBackward = new QPushButton(framePlayer);
        buttonBackward->setObjectName(QString::fromUtf8("buttonBackward"));
        buttonBackward->setMinimumSize(QSize(25, 25));
        buttonBackward->setMaximumSize(QSize(25, 25));
        buttonBackward->setCursor(QCursor(Qt::PointingHandCursor));
        buttonBackward->setStyleSheet(QString::fromUtf8("border-image: url(:pic/images/\345\277\253\351\200\200.PNG);"));

        horizontalLayout_3->addWidget(buttonBackward);

        buttonStartPause = new QPushButton(framePlayer);
        buttonStartPause->setObjectName(QString::fromUtf8("buttonStartPause"));
        buttonStartPause->setMinimumSize(QSize(50, 50));
        buttonStartPause->setMaximumSize(QSize(50, 50));
        buttonStartPause->setCursor(QCursor(Qt::PointingHandCursor));
        buttonStartPause->setStyleSheet(QString::fromUtf8("border-image: url(:pic/images/\346\222\255\346\224\276.PNG);"));

        horizontalLayout_3->addWidget(buttonStartPause);

        buttonForward = new QPushButton(framePlayer);
        buttonForward->setObjectName(QString::fromUtf8("buttonForward"));
        buttonForward->setMinimumSize(QSize(25, 25));
        buttonForward->setMaximumSize(QSize(25, 25));
        buttonForward->setCursor(QCursor(Qt::PointingHandCursor));
        buttonForward->setStyleSheet(QString::fromUtf8("border-image: url(:pic/images/\345\277\253\350\277\233.PNG);"));

        horizontalLayout_3->addWidget(buttonForward);

        buttonStop = new QPushButton(framePlayer);
        buttonStop->setObjectName(QString::fromUtf8("buttonStop"));
        buttonStop->setMinimumSize(QSize(25, 25));
        buttonStop->setMaximumSize(QSize(25, 25));
        buttonStop->setCursor(QCursor(Qt::PointingHandCursor));
        buttonStop->setStyleSheet(QString::fromUtf8("border-image: url(:pic/images/\345\201\234\346\255\242.PNG);"));

        horizontalLayout_3->addWidget(buttonStop);


        horizontalLayout_4->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        sliderSpeed = new QSlider(framePlayer);
        sliderSpeed->setObjectName(QString::fromUtf8("sliderSpeed"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sliderSpeed->sizePolicy().hasHeightForWidth());
        sliderSpeed->setSizePolicy(sizePolicy1);
        sliderSpeed->setMinimumSize(QSize(120, 0));
        sliderSpeed->setMaximumSize(QSize(120, 16777215));
        sliderSpeed->setMinimum(-3);
        sliderSpeed->setMaximum(2);
        sliderSpeed->setPageStep(1);
        sliderSpeed->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(sliderSpeed);

        labelSpeed = new QLabel(framePlayer);
        labelSpeed->setObjectName(QString::fromUtf8("labelSpeed"));
        labelSpeed->setMinimumSize(QSize(55, 0));
        labelSpeed->setMaximumSize(QSize(55, 16777215));
        labelSpeed->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(labelSpeed);


        horizontalLayout_4->addLayout(horizontalLayout_2);

        horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        buttonListRevert = new QToolButton(framePlayer);
        buttonListRevert->setObjectName(QString::fromUtf8("buttonListRevert"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(buttonListRevert->sizePolicy().hasHeightForWidth());
        buttonListRevert->setSizePolicy(sizePolicy2);
        buttonListRevert->setMinimumSize(QSize(0, 0));
        buttonListRevert->setMaximumSize(QSize(16777215, 16777215));
        buttonListRevert->setAutoExclusive(false);
        buttonListRevert->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        buttonListRevert->setAutoRaise(false);
        buttonListRevert->setArrowType(Qt::DownArrow);

        horizontalLayout_4->addWidget(buttonListRevert);


        verticalLayout_2->addLayout(horizontalLayout_4);


        verticalLayout_3->addLayout(verticalLayout_2);


        gridLayout->addWidget(framePlayer, 0, 0, 1, 1);

        frameRevertList = new QFrame(RevertPlayer);
        frameRevertList->setObjectName(QString::fromUtf8("frameRevertList"));
        frameRevertList->setMinimumSize(QSize(0, 350));
        frameRevertList->setMaximumSize(QSize(16777215, 350));
        frameRevertList->setFrameShape(QFrame::StyledPanel);
        frameRevertList->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frameRevertList);
        verticalLayout->setMargin(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        listRevert = new QTableWidget(frameRevertList);
        listRevert->setObjectName(QString::fromUtf8("listRevert"));
        listRevert->setMouseTracking(true);
        listRevert->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 255);\n"
"font: 14pt \"\344\273\277\345\256\213\";"));
        listRevert->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listRevert->setSelectionMode(QAbstractItemView::SingleSelection);
        listRevert->setSelectionBehavior(QAbstractItemView::SelectRows);
        listRevert->setShowGrid(false);
        listRevert->horizontalHeader()->setVisible(false);
        listRevert->horizontalHeader()->setStretchLastSection(true);
        listRevert->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(listRevert);


        gridLayout->addWidget(frameRevertList, 1, 0, 1, 1);


        retranslateUi(RevertPlayer);

        QMetaObject::connectSlotsByName(RevertPlayer);
    } // setupUi

    void retranslateUi(QWidget *RevertPlayer)
    {
        RevertPlayer->setWindowTitle(QApplication::translate("RevertPlayer", "\345\267\245\345\206\265\345\206\215\347\216\260", 0, QApplication::UnicodeUTF8));
        labelMsg->setText(QApplication::translate("RevertPlayer", "\347\212\266\346\200\201:\345\267\245\345\206\265\345\206\215\347\216\260\345\260\232\346\234\252\351\200\211\345\217\226\357\274\214\350\257\267\345\217\214\345\207\273\345\206\215\347\216\260\345\210\227\350\241\250\344\270\255\347\232\204\345\267\245\345\206\265\345\206\215\347\216\260\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        labelTime->setText(QApplication::translate("RevertPlayer", "00:00:00", 0, QApplication::UnicodeUTF8));
        labelTimeTotle->setText(QApplication::translate("RevertPlayer", "00:00:00", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonBackward->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        buttonBackward->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        buttonBackward->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        buttonBackward->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonStartPause->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonStartPause->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        buttonStartPause->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonForward->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        buttonForward->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        buttonForward->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        buttonForward->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonStop->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonStop->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        buttonStop->setText(QString());
        labelSpeed->setText(QApplication::translate("RevertPlayer", "1\345\200\215\351\200\237", 0, QApplication::UnicodeUTF8));
        buttonListRevert->setText(QApplication::translate("RevertPlayer", "\345\206\215\347\216\260\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(RevertPlayer);
    } // retranslateUi

};

namespace Ui {
    class RevertPlayer: public Ui_RevertPlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REVERTPLAYER_H
