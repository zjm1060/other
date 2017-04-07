/********************************************************************************
** Form generated from reading ui file 'traininfoadd.ui'
**
** Created: Thu Jan 15 10:24:24 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TRAININFOADD_H
#define UI_TRAININFOADD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CTrainInfoAddTable
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *editHostName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLineEdit *editUserName;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QLineEdit *editSendTaskId;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QLineEdit *editTaskId;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_6;
    QLineEdit *editSceneId;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_7;
    QLineEdit *editFaultId;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_8;
    QComboBox *editLoginStatus;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_9;
    QComboBox *editStatus;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_10;
    QLineEdit *editSpeed;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_11;
    QLineEdit *editDbinFile;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_12;
    QComboBox *editTaskStatus;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_13;
    QComboBox *editSceneStatus;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_14;
    QComboBox *editFaultStatus;
    QHBoxLayout *horizontalLayout_14;
    QPushButton *buttonEnter;
    QPushButton *buttonApply;
    QPushButton *buttonExit;

    void setupUi(QDialog *CTrainInfoAddTable)
    {
        if (CTrainInfoAddTable->objectName().isEmpty())
            CTrainInfoAddTable->setObjectName(QString::fromUtf8("CTrainInfoAddTable"));
        CTrainInfoAddTable->resize(350, 528);
        CTrainInfoAddTable->setMinimumSize(QSize(100, 0));
        CTrainInfoAddTable->setMaximumSize(QSize(350, 16777215));
        CTrainInfoAddTable->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        verticalLayout = new QVBoxLayout(CTrainInfoAddTable);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(CTrainInfoAddTable);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(100, 0));
        label_2->setMaximumSize(QSize(100, 16777215));
        label_2->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout->addWidget(label_2);

        editHostName = new QLineEdit(CTrainInfoAddTable);
        editHostName->setObjectName(QString::fromUtf8("editHostName"));

        horizontalLayout->addWidget(editHostName);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(CTrainInfoAddTable);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(100, 0));
        label_3->setMaximumSize(QSize(100, 16777215));
        label_3->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_2->addWidget(label_3);

        editUserName = new QLineEdit(CTrainInfoAddTable);
        editUserName->setObjectName(QString::fromUtf8("editUserName"));

        horizontalLayout_2->addWidget(editUserName);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(CTrainInfoAddTable);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(100, 0));
        label_4->setMaximumSize(QSize(100, 16777215));
        label_4->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_3->addWidget(label_4);

        editSendTaskId = new QLineEdit(CTrainInfoAddTable);
        editSendTaskId->setObjectName(QString::fromUtf8("editSendTaskId"));

        horizontalLayout_3->addWidget(editSendTaskId);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_5 = new QLabel(CTrainInfoAddTable);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(100, 0));
        label_5->setMaximumSize(QSize(100, 16777215));
        label_5->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_4->addWidget(label_5);

        editTaskId = new QLineEdit(CTrainInfoAddTable);
        editTaskId->setObjectName(QString::fromUtf8("editTaskId"));

        horizontalLayout_4->addWidget(editTaskId);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_6 = new QLabel(CTrainInfoAddTable);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(100, 0));
        label_6->setMaximumSize(QSize(100, 16777215));
        label_6->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_5->addWidget(label_6);

        editSceneId = new QLineEdit(CTrainInfoAddTable);
        editSceneId->setObjectName(QString::fromUtf8("editSceneId"));

        horizontalLayout_5->addWidget(editSceneId);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_7 = new QLabel(CTrainInfoAddTable);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(100, 0));
        label_7->setMaximumSize(QSize(100, 16777215));
        label_7->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_6->addWidget(label_7);

        editFaultId = new QLineEdit(CTrainInfoAddTable);
        editFaultId->setObjectName(QString::fromUtf8("editFaultId"));

        horizontalLayout_6->addWidget(editFaultId);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_8 = new QLabel(CTrainInfoAddTable);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(100, 0));
        label_8->setMaximumSize(QSize(100, 16777215));
        label_8->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_7->addWidget(label_8);

        editLoginStatus = new QComboBox(CTrainInfoAddTable);
        editLoginStatus->setObjectName(QString::fromUtf8("editLoginStatus"));

        horizontalLayout_7->addWidget(editLoginStatus);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_9 = new QLabel(CTrainInfoAddTable);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(100, 0));
        label_9->setMaximumSize(QSize(100, 16777215));
        label_9->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_8->addWidget(label_9);

        editStatus = new QComboBox(CTrainInfoAddTable);
        editStatus->setObjectName(QString::fromUtf8("editStatus"));

        horizontalLayout_8->addWidget(editStatus);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_10 = new QLabel(CTrainInfoAddTable);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(100, 0));
        label_10->setMaximumSize(QSize(100, 16777215));
        label_10->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_9->addWidget(label_10);

        editSpeed = new QLineEdit(CTrainInfoAddTable);
        editSpeed->setObjectName(QString::fromUtf8("editSpeed"));

        horizontalLayout_9->addWidget(editSpeed);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_11 = new QLabel(CTrainInfoAddTable);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setMinimumSize(QSize(100, 0));
        label_11->setMaximumSize(QSize(100, 16777215));
        label_11->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_10->addWidget(label_11);

        editDbinFile = new QLineEdit(CTrainInfoAddTable);
        editDbinFile->setObjectName(QString::fromUtf8("editDbinFile"));

        horizontalLayout_10->addWidget(editDbinFile);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_12 = new QLabel(CTrainInfoAddTable);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setMinimumSize(QSize(100, 0));
        label_12->setMaximumSize(QSize(100, 16777215));
        label_12->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_11->addWidget(label_12);

        editTaskStatus = new QComboBox(CTrainInfoAddTable);
        editTaskStatus->setObjectName(QString::fromUtf8("editTaskStatus"));

        horizontalLayout_11->addWidget(editTaskStatus);


        verticalLayout->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_13 = new QLabel(CTrainInfoAddTable);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setMinimumSize(QSize(100, 0));
        label_13->setMaximumSize(QSize(100, 16777215));
        label_13->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_12->addWidget(label_13);

        editSceneStatus = new QComboBox(CTrainInfoAddTable);
        editSceneStatus->setObjectName(QString::fromUtf8("editSceneStatus"));

        horizontalLayout_12->addWidget(editSceneStatus);


        verticalLayout->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_14 = new QLabel(CTrainInfoAddTable);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMinimumSize(QSize(100, 0));
        label_14->setMaximumSize(QSize(100, 16777215));
        label_14->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_13->addWidget(label_14);

        editFaultStatus = new QComboBox(CTrainInfoAddTable);
        editFaultStatus->setObjectName(QString::fromUtf8("editFaultStatus"));

        horizontalLayout_13->addWidget(editFaultStatus);


        verticalLayout->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(20);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        buttonEnter = new QPushButton(CTrainInfoAddTable);
        buttonEnter->setObjectName(QString::fromUtf8("buttonEnter"));
        buttonEnter->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_14->addWidget(buttonEnter);

        buttonApply = new QPushButton(CTrainInfoAddTable);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_14->addWidget(buttonApply);

        buttonExit = new QPushButton(CTrainInfoAddTable);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\256\213\344\275\223\";"));

        horizontalLayout_14->addWidget(buttonExit);


        verticalLayout->addLayout(horizontalLayout_14);

        QWidget::setTabOrder(editHostName, editUserName);
        QWidget::setTabOrder(editUserName, editSendTaskId);
        QWidget::setTabOrder(editSendTaskId, editTaskId);
        QWidget::setTabOrder(editTaskId, editSceneId);
        QWidget::setTabOrder(editSceneId, editFaultId);
        QWidget::setTabOrder(editFaultId, editLoginStatus);
        QWidget::setTabOrder(editLoginStatus, editStatus);
        QWidget::setTabOrder(editStatus, editSpeed);
        QWidget::setTabOrder(editSpeed, editDbinFile);
        QWidget::setTabOrder(editDbinFile, editTaskStatus);
        QWidget::setTabOrder(editTaskStatus, editSceneStatus);
        QWidget::setTabOrder(editSceneStatus, editFaultStatus);
        QWidget::setTabOrder(editFaultStatus, buttonEnter);
        QWidget::setTabOrder(buttonEnter, buttonExit);

        retranslateUi(CTrainInfoAddTable);
        QObject::connect(buttonExit, SIGNAL(clicked()), CTrainInfoAddTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CTrainInfoAddTable);
    } // setupUi

    void retranslateUi(QDialog *CTrainInfoAddTable)
    {
        CTrainInfoAddTable->setWindowTitle(QApplication::translate("CTrainInfoAddTable", "\346\267\273\345\212\240\345\237\271\350\256\255\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CTrainInfoAddTable", "\344\270\273\346\234\272\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CTrainInfoAddTable", "\347\224\250\346\210\267\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CTrainInfoAddTable", "\344\273\273\345\212\241\345\210\206\345\217\221ID\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CTrainInfoAddTable", "\344\273\273\345\212\241ID\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CTrainInfoAddTable", "\345\267\245\345\206\265ID\357\274\232", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("CTrainInfoAddTable", "\346\225\205\351\232\234ID\357\274\232", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("CTrainInfoAddTable", "\347\231\273\345\275\225\347\212\266\346\200\201\357\274\232", 0, QApplication::UnicodeUTF8));
        editLoginStatus->clear();
        editLoginStatus->insertItems(0, QStringList()
         << QApplication::translate("CTrainInfoAddTable", "\346\234\252\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTrainInfoAddTable", "\345\267\262\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8)
        );
        label_9->setText(QApplication::translate("CTrainInfoAddTable", "\350\277\220\350\241\214\347\212\266\346\200\201\357\274\232", 0, QApplication::UnicodeUTF8));
        editStatus->clear();
        editStatus->insertItems(0, QStringList()
         << QApplication::translate("CTrainInfoAddTable", "\345\201\234\346\255\242", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTrainInfoAddTable", "\350\277\220\350\241\214", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTrainInfoAddTable", "\346\232\202\345\201\234", 0, QApplication::UnicodeUTF8)
        );
        label_10->setText(QApplication::translate("CTrainInfoAddTable", "\344\273\277\347\234\237\351\200\237\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("CTrainInfoAddTable", "\351\273\230\350\256\244\347\224\273\351\235\242\357\274\232", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("CTrainInfoAddTable", "\344\273\273\345\212\241\347\212\266\346\200\201\357\274\232", 0, QApplication::UnicodeUTF8));
        editTaskStatus->clear();
        editTaskStatus->insertItems(0, QStringList()
         << QApplication::translate("CTrainInfoAddTable", "\346\227\240\346\226\260\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTrainInfoAddTable", "\346\226\260\347\232\204\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTrainInfoAddTable", "\344\273\273\345\212\241\345\267\262\345\244\204\347\220\206", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTrainInfoAddTable", "\344\273\273\345\212\241\347\273\223\346\235\237", 0, QApplication::UnicodeUTF8)
        );
        label_13->setText(QApplication::translate("CTrainInfoAddTable", "\345\267\245\345\206\265\347\212\266\346\200\201\357\274\232", 0, QApplication::UnicodeUTF8));
        editSceneStatus->clear();
        editSceneStatus->insertItems(0, QStringList()
         << QApplication::translate("CTrainInfoAddTable", "\346\227\240\346\226\260\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTrainInfoAddTable", "\346\226\260\347\232\204\345\267\245\345\206\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTrainInfoAddTable", "\345\267\245\345\206\265\345\267\262\345\244\204\347\220\206", 0, QApplication::UnicodeUTF8)
        );
        label_14->setText(QApplication::translate("CTrainInfoAddTable", "\346\225\205\351\232\234\347\212\266\346\200\201\357\274\232", 0, QApplication::UnicodeUTF8));
        editFaultStatus->clear();
        editFaultStatus->insertItems(0, QStringList()
         << QApplication::translate("CTrainInfoAddTable", "\346\227\240\346\226\260\346\225\205\351\232\234", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTrainInfoAddTable", "\346\226\260\347\232\204\346\225\205\351\232\234", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTrainInfoAddTable", "\346\225\205\351\232\234\345\267\262\345\244\204\347\220\206", 0, QApplication::UnicodeUTF8)
        );
        buttonEnter->setText(QApplication::translate("CTrainInfoAddTable", "\347\241\256  \345\256\232", 0, QApplication::UnicodeUTF8));
        buttonApply->setText(QApplication::translate("CTrainInfoAddTable", "\345\272\224  \347\224\250", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CTrainInfoAddTable", "\351\200\200  \345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CTrainInfoAddTable);
    } // retranslateUi

};

namespace Ui {
    class CTrainInfoAddTable: public Ui_CTrainInfoAddTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAININFOADD_H
