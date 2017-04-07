/********************************************************************************
** Form generated from reading ui file 'trainmanager.ui'
**
** Created: Thu Jan 15 10:25:46 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TRAINMANAGER_H
#define UI_TRAINMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CTrainManagerTable
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboSimuMode;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QComboBox *comboBox_interlock;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QLabel *labelSimuSpeed;
    QVBoxLayout *verticalLayout_2;
    QSlider *sliderSimuSpeed;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *buttonSimuStart;
    QPushButton *buttonSimuPause;
    QPushButton *buttonSimuStop;

    void setupUi(QDialog *CTrainManagerTable)
    {
        if (CTrainManagerTable->objectName().isEmpty())
            CTrainManagerTable->setObjectName(QString::fromUtf8("CTrainManagerTable"));
        CTrainManagerTable->resize(380, 230);
        CTrainManagerTable->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        gridLayout = new QGridLayout(CTrainManagerTable);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CTrainManagerTable);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(60, 16777215));

        horizontalLayout->addWidget(label);

        comboSimuMode = new QComboBox(CTrainManagerTable);
        comboSimuMode->setObjectName(QString::fromUtf8("comboSimuMode"));

        horizontalLayout->addWidget(comboSimuMode);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_2 = new QLabel(CTrainManagerTable);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_4->addWidget(label_2);

        comboBox_interlock = new QComboBox(CTrainManagerTable);
        comboBox_interlock->setObjectName(QString::fromUtf8("comboBox_interlock"));

        horizontalLayout_4->addWidget(comboBox_interlock);


        gridLayout->addLayout(horizontalLayout_4, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_2->setContentsMargins(-1, -1, -1, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_3 = new QLabel(CTrainManagerTable);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_3->addWidget(label_3);

        labelSimuSpeed = new QLabel(CTrainManagerTable);
        labelSimuSpeed->setObjectName(QString::fromUtf8("labelSimuSpeed"));

        verticalLayout_3->addWidget(labelSimuSpeed);


        horizontalLayout_2->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        sliderSimuSpeed = new QSlider(CTrainManagerTable);
        sliderSimuSpeed->setObjectName(QString::fromUtf8("sliderSimuSpeed"));
        sliderSimuSpeed->setMinimum(0);
        sliderSimuSpeed->setMaximum(7);
        sliderSimuSpeed->setPageStep(1);
        sliderSimuSpeed->setValue(1);
        sliderSimuSpeed->setSliderPosition(1);
        sliderSimuSpeed->setOrientation(Qt::Horizontal);
        sliderSimuSpeed->setInvertedAppearance(false);
        sliderSimuSpeed->setInvertedControls(false);
        sliderSimuSpeed->setTickPosition(QSlider::TicksBothSides);

        verticalLayout_2->addWidget(sliderSimuSpeed);


        horizontalLayout_2->addLayout(verticalLayout_2);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        buttonSimuStart = new QPushButton(CTrainManagerTable);
        buttonSimuStart->setObjectName(QString::fromUtf8("buttonSimuStart"));
        buttonSimuStart->setMinimumSize(QSize(50, 50));

        horizontalLayout_3->addWidget(buttonSimuStart);

        buttonSimuPause = new QPushButton(CTrainManagerTable);
        buttonSimuPause->setObjectName(QString::fromUtf8("buttonSimuPause"));
        buttonSimuPause->setMinimumSize(QSize(50, 50));

        horizontalLayout_3->addWidget(buttonSimuPause);

        buttonSimuStop = new QPushButton(CTrainManagerTable);
        buttonSimuStop->setObjectName(QString::fromUtf8("buttonSimuStop"));
        buttonSimuStop->setMinimumSize(QSize(50, 50));

        horizontalLayout_3->addWidget(buttonSimuStop);


        gridLayout->addLayout(horizontalLayout_3, 3, 0, 1, 1);


        retranslateUi(CTrainManagerTable);

        QMetaObject::connectSlotsByName(CTrainManagerTable);
    } // setupUi

    void retranslateUi(QDialog *CTrainManagerTable)
    {
        CTrainManagerTable->setWindowTitle(QApplication::translate("CTrainManagerTable", "\345\255\246\345\221\230\345\237\271\350\256\255\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CTrainManagerTable", "\346\250\241 \345\274\217\357\274\232", 0, QApplication::UnicodeUTF8));
        comboSimuMode->clear();
        comboSimuMode->insertItems(0, QStringList()
         << QApplication::translate("CTrainManagerTable", "\345\237\271\350\256\255\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTrainManagerTable", "\350\200\203\350\257\225\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTrainManagerTable", "\347\253\236\350\265\233\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("CTrainManagerTable", "\351\227\255\351\224\201\346\216\247\345\210\266\357\274\232", 0, QApplication::UnicodeUTF8));
        comboBox_interlock->clear();
        comboBox_interlock->insertItems(0, QStringList()
         << QApplication::translate("CTrainManagerTable", "\351\227\255\351\224\201\345\271\266\346\217\220\347\244\272", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTrainManagerTable", "\351\227\255\351\224\201\344\270\215\346\217\220\347\244\272", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CTrainManagerTable", "\346\227\240\351\227\255\351\224\201\357\274\214\344\273\273\346\204\217\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("CTrainManagerTable", "\344\273\277\347\234\237\351\200\237\345\272\246\357\274\232", 0, QApplication::UnicodeUTF8));
        labelSimuSpeed->setText(QString());
        buttonSimuStart->setText(QApplication::translate("CTrainManagerTable", "\345\274\200\345\247\213\344\273\277\347\234\237", 0, QApplication::UnicodeUTF8));
        buttonSimuPause->setText(QApplication::translate("CTrainManagerTable", "\346\232\202\345\201\234\344\273\277\347\234\237", 0, QApplication::UnicodeUTF8));
        buttonSimuStop->setText(QApplication::translate("CTrainManagerTable", "\345\201\234\346\255\242\344\273\277\347\234\237", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CTrainManagerTable);
    } // retranslateUi

};

namespace Ui {
    class CTrainManagerTable: public Ui_CTrainManagerTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAINMANAGER_H
