/********************************************************************************
** Form generated from reading ui file 'savescene.ui'
**
** Created: Thu Jan 15 10:26:28 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SAVESCENE_H
#define UI_SAVESCENE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CSaveSceneTable
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *radioStn;
    QRadioButton *radioDev;
    QRadioButton *radioDsp;
    QHBoxLayout *horizontalLayout_5;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_6;
    QLineEdit *editSceneName;
    QLabel *label_7;
    QTextEdit *editSceneRemark;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboStn;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *comboDev;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *editDsp;
    QSpacerItem *verticalSpacer;
    QLabel *labelWait;
    QSpacerItem *verticalSpacer_2;
    QDialogButtonBox *buttonBox;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QDialog *CSaveSceneTable)
    {
        if (CSaveSceneTable->objectName().isEmpty())
            CSaveSceneTable->setObjectName(QString::fromUtf8("CSaveSceneTable"));
        CSaveSceneTable->resize(670, 380);
        gridLayout = new QGridLayout(CSaveSceneTable);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        groupBox = new QGroupBox(CSaveSceneTable);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        horizontalLayout_4 = new QHBoxLayout(groupBox);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        radioStn = new QRadioButton(groupBox);
        radioStn->setObjectName(QString::fromUtf8("radioStn"));

        horizontalLayout_4->addWidget(radioStn);

        radioDev = new QRadioButton(groupBox);
        radioDev->setObjectName(QString::fromUtf8("radioDev"));

        horizontalLayout_4->addWidget(radioDev);

        radioDsp = new QRadioButton(groupBox);
        radioDsp->setObjectName(QString::fromUtf8("radioDsp"));

        horizontalLayout_4->addWidget(radioDsp);


        verticalLayout_5->addWidget(groupBox);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        frame_2 = new QFrame(CSaveSceneTable);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::WinPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setMargin(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_6 = new QLabel(frame_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout_4->addWidget(label_6);

        editSceneName = new QLineEdit(frame_2);
        editSceneName->setObjectName(QString::fromUtf8("editSceneName"));
        editSceneName->setMaxLength(25);

        verticalLayout_4->addWidget(editSceneName);

        label_7 = new QLabel(frame_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        verticalLayout_4->addWidget(label_7);

        editSceneRemark = new QTextEdit(frame_2);
        editSceneRemark->setObjectName(QString::fromUtf8("editSceneRemark"));

        verticalLayout_4->addWidget(editSceneRemark);


        verticalLayout_2->addLayout(verticalLayout_4);


        horizontalLayout_5->addWidget(frame_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        frame = new QFrame(CSaveSceneTable);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::WinPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
#ifndef Q_OS_MAC
        verticalLayout->setSpacing(6);
#endif
        verticalLayout->setMargin(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(85, 0));
        label->setMaximumSize(QSize(85, 16777215));
        label->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label);

        comboStn = new QComboBox(frame);
        comboStn->setObjectName(QString::fromUtf8("comboStn"));
        comboStn->setMinimumSize(QSize(160, 0));

        horizontalLayout->addWidget(comboStn);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        label_2->setMinimumSize(QSize(85, 0));
        label_2->setMaximumSize(QSize(85, 16777215));
        label_2->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_2);

        comboDev = new QComboBox(frame);
        comboDev->setObjectName(QString::fromUtf8("comboDev"));
        comboDev->setMinimumSize(QSize(160, 0));

        horizontalLayout_2->addWidget(comboDev);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMinimumSize(QSize(85, 0));
        label_3->setMaximumSize(QSize(85, 16777215));
        label_3->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_3);

        editDsp = new QLineEdit(frame);
        editDsp->setObjectName(QString::fromUtf8("editDsp"));
        editDsp->setMinimumSize(QSize(160, 0));
        editDsp->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(editDsp);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_3->addWidget(frame);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        labelWait = new QLabel(CSaveSceneTable);
        labelWait->setObjectName(QString::fromUtf8("labelWait"));

        verticalLayout_3->addWidget(labelWait);


        horizontalLayout_5->addLayout(verticalLayout_3);


        verticalLayout_5->addLayout(horizontalLayout_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_2);

        buttonBox = new QDialogButtonBox(CSaveSceneTable);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setMaximumSize(QSize(16777215, 16777215));
        buttonBox->setContextMenuPolicy(Qt::DefaultContextMenu);
        buttonBox->setLayoutDirection(Qt::LeftToRight);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        verticalLayout_5->addWidget(buttonBox);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_3);


        gridLayout->addLayout(verticalLayout_5, 0, 0, 1, 1);


        retranslateUi(CSaveSceneTable);

        QMetaObject::connectSlotsByName(CSaveSceneTable);
    } // setupUi

    void retranslateUi(QDialog *CSaveSceneTable)
    {
        CSaveSceneTable->setWindowTitle(QApplication::translate("CSaveSceneTable", "\344\277\235\345\255\230\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CSaveSceneTable", "\345\267\245\345\206\265\346\235\245\346\272\220", 0, QApplication::UnicodeUTF8));
        radioStn->setText(QApplication::translate("CSaveSceneTable", "\345\205\250\345\216\202\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8));
        radioDev->setText(QApplication::translate("CSaveSceneTable", "\345\215\225\344\270\200\350\256\276\345\244\207", 0, QApplication::UnicodeUTF8));
        radioDsp->setText(QApplication::translate("CSaveSceneTable", "\345\275\223\345\211\215\347\224\273\351\235\242", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CSaveSceneTable", "\345\267\245\345\206\265\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("CSaveSceneTable", "\345\244\207\346\263\250\357\274\232", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CSaveSceneTable", "\345\216\202    \347\253\231\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CSaveSceneTable", "\350\256\276    \345\244\207\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CSaveSceneTable", "\345\275\223\345\211\215\347\224\273\351\235\242\357\274\232", 0, QApplication::UnicodeUTF8));
        labelWait->setText(QString());
        Q_UNUSED(CSaveSceneTable);
    } // retranslateUi

};

namespace Ui {
    class CSaveSceneTable: public Ui_CSaveSceneTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVESCENE_H
