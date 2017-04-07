/********************************************************************************
** Form generated from reading ui file 'taskManage.ui'
**
** Created: Wed Jan 14 10:22:09 2015
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TASKMANAGE_H
#define UI_TASKMANAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CTaskManage
{
public:
    QGridLayout *gridLayout_2;
    QTabWidget *tabWidget;
    QWidget *curTask;
    QGridLayout *gridLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *labelTaskName;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLabel *labelTaskAttr;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QLabel *labelSendTime;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QLabel *labelStation;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_6;
    QLabel *labelDev;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_8;
    QLabel *labelStatus;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_9;
    QLabel *labelScene;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_10;
    QLabel *labelFault;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QLCDNumber *taskTime;
    QSpacerItem *verticalSpacer;
    QPushButton *buttonStart;
    QPushButton *buttonReStart;
    QPushButton *buttonFinish;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_11;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *checkSimuTip;
    QTextBrowser *textTaskTip;
    QWidget *taskList;
    QGridLayout *gridLayout_4;
    QTableWidget *listTask;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *buttonSetCurTask;
    QPushButton *buttonDelTask;
    QPushButton *buttonClearTask;
    QWidget *taskRec;
    QGridLayout *gridLayout_3;
    QTableWidget *listTaskRec;
    QWidget *tabPlan;
    QGridLayout *gridLayout_5;
    QTableWidget *tablePlan;
    QFormLayout *formLayout;
    QPushButton *buttonExit;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QWidget *CTaskManage)
    {
        if (CTaskManage->objectName().isEmpty())
            CTaskManage->setObjectName(QString::fromUtf8("CTaskManage"));
        CTaskManage->resize(827, 630);
        CTaskManage->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\256\213\344\275\223\";"));
        gridLayout_2 = new QGridLayout(CTaskManage);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tabWidget = new QTabWidget(CTaskManage);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QFont font;
        font.setFamily(QString::fromUtf8("\346\226\260\345\256\213\344\275\223"));
        font.setPointSize(12);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(9);
        tabWidget->setFont(font);
        tabWidget->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(231, 240, 245);\n"
"font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
""));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setDocumentMode(false);
        curTask = new QWidget();
        curTask->setObjectName(QString::fromUtf8("curTask"));
        gridLayout = new QGridLayout(curTask);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frame = new QFrame(curTask);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0.520599, y1:1, x2:0.508, y2:0.142, stop:0 rgba(255, 255, 255, 255), stop:0.0564972 rgba(236, 241, 245, 255), stop:0.536723 rgba(236, 241, 245, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
"    border-width: 1px;\n"
"    border-color: rgb(150, 194, 219);\n"
"    border-style: solid;\n"
"    border-radius: 2;\n"
"    padding: 2px;\n"
"font: 13pt \"\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200\";\n"
"color: rgb(7, 100, 143);"));
        frame->setFrameShape(QFrame::StyledPanel);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(100, 16777215));
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27, 0), stop:1 rgba(255, 255, 255, 0));\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27,0), stop:1 rgba(255, 255, 255, 0));"));

        horizontalLayout->addWidget(label);

        labelTaskName = new QLabel(frame);
        labelTaskName->setObjectName(QString::fromUtf8("labelTaskName"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\346\261\211\344\273\252\344\270\255\351\273\221\347\256\200"));
        font1.setPointSize(13);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        labelTaskName->setFont(font1);
        labelTaskName->setFrameShape(QFrame::StyledPanel);
        labelTaskName->setWordWrap(false);

        horizontalLayout->addWidget(labelTaskName);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(100, 16777215));
        label_3->setFont(font);
        label_3->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27, 0), stop:1 rgba(255, 255, 255, 0));\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27,0), stop:1 rgba(255, 255, 255, 0));"));

        horizontalLayout_2->addWidget(label_3);

        labelTaskAttr = new QLabel(frame);
        labelTaskAttr->setObjectName(QString::fromUtf8("labelTaskAttr"));
        labelTaskAttr->setFont(font1);
        labelTaskAttr->setFrameShape(QFrame::StyledPanel);

        horizontalLayout_2->addWidget(labelTaskAttr);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMaximumSize(QSize(100, 16777215));
        label_4->setFont(font);
        label_4->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27, 0), stop:1 rgba(255, 255, 255, 0));\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27,0), stop:1 rgba(255, 255, 255, 0));"));

        horizontalLayout_3->addWidget(label_4);

        labelSendTime = new QLabel(frame);
        labelSendTime->setObjectName(QString::fromUtf8("labelSendTime"));
        labelSendTime->setFont(font1);
        labelSendTime->setFrameShape(QFrame::StyledPanel);

        horizontalLayout_3->addWidget(labelSendTime);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_5 = new QLabel(frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMaximumSize(QSize(100, 16777215));
        label_5->setFont(font);
        label_5->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27, 0), stop:1 rgba(255, 255, 255, 0));\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27,0), stop:1 rgba(255, 255, 255, 0));"));

        horizontalLayout_4->addWidget(label_5);

        labelStation = new QLabel(frame);
        labelStation->setObjectName(QString::fromUtf8("labelStation"));
        labelStation->setFont(font1);
        labelStation->setFrameShape(QFrame::StyledPanel);

        horizontalLayout_4->addWidget(labelStation);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMaximumSize(QSize(100, 16777215));
        label_6->setFont(font);
        label_6->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27, 0), stop:1 rgba(255, 255, 255, 0));\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27,0), stop:1 rgba(255, 255, 255, 0));"));

        horizontalLayout_5->addWidget(label_6);

        labelDev = new QLabel(frame);
        labelDev->setObjectName(QString::fromUtf8("labelDev"));
        labelDev->setFont(font1);
        labelDev->setFrameShape(QFrame::StyledPanel);

        horizontalLayout_5->addWidget(labelDev);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_8 = new QLabel(frame);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMaximumSize(QSize(100, 16777215));
        label_8->setFont(font);
        label_8->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27, 0), stop:1 rgba(255, 255, 255, 0));\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27,0), stop:1 rgba(255, 255, 255, 0));"));

        horizontalLayout_6->addWidget(label_8);

        labelStatus = new QLabel(frame);
        labelStatus->setObjectName(QString::fromUtf8("labelStatus"));
        labelStatus->setFont(font1);
        labelStatus->setFrameShape(QFrame::StyledPanel);

        horizontalLayout_6->addWidget(labelStatus);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_9 = new QLabel(frame);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMaximumSize(QSize(100, 16777215));
        label_9->setFont(font);
        label_9->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27, 0), stop:1 rgba(255, 255, 255, 0));\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27,0), stop:1 rgba(255, 255, 255, 0));"));

        horizontalLayout_7->addWidget(label_9);

        labelScene = new QLabel(frame);
        labelScene->setObjectName(QString::fromUtf8("labelScene"));
        labelScene->setFont(font1);
        labelScene->setFrameShape(QFrame::StyledPanel);

        horizontalLayout_7->addWidget(labelScene);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_10 = new QLabel(frame);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMaximumSize(QSize(100, 16777215));
        label_10->setFont(font);
        label_10->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27, 0), stop:1 rgba(255, 255, 255, 0));\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27,0), stop:1 rgba(255, 255, 255, 0));"));

        horizontalLayout_8->addWidget(label_10);

        labelFault = new QLabel(frame);
        labelFault->setObjectName(QString::fromUtf8("labelFault"));
        labelFault->setFont(font1);
        labelFault->setFrameShape(QFrame::StyledPanel);

        horizontalLayout_8->addWidget(labelFault);


        verticalLayout->addLayout(horizontalLayout_8);


        gridLayout->addWidget(frame, 0, 0, 1, 1);

        frame_2 = new QFrame(curTask);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setMaximumSize(QSize(150, 16777215));
        frame_2->setStyleSheet(QString::fromUtf8(""));
        frame_2->setFrameShape(QFrame::StyledPanel);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\346\226\260\345\256\213\344\275\223\";\n"
"color: rgb(4, 30, 103);\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27, 0), stop:1 rgba(255, 255, 255, 0));\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0.0227273, x2:1, y2:0, stop:0 rgba(78, 78, 78, 0), stop:0.480226 rgba(27, 27, 27,0), stop:1 rgba(255, 255, 255, 0));"));

        verticalLayout_2->addWidget(label_2);

        taskTime = new QLCDNumber(frame_2);
        taskTime->setObjectName(QString::fromUtf8("taskTime"));
        taskTime->setFont(font);
        taskTime->setStyleSheet(QString::fromUtf8("background-color: qradialgradient(spread:pad, cx:0.432, cy:0.589545, radius:1.722, fx:0.017, fy:0.0238737, stop:0.0734463 rgba(7, 100, 143, 255), stop:1 rgba(124, 210, 252, 255));\n"
"color: rgb(31,217,242); "));

        verticalLayout_2->addWidget(taskTime);

        verticalSpacer = new QSpacerItem(20, 81, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        buttonStart = new QPushButton(frame_2);
        buttonStart->setObjectName(QString::fromUtf8("buttonStart"));
        buttonStart->setFont(font);
        buttonStart->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_2->addWidget(buttonStart);

        buttonReStart = new QPushButton(frame_2);
        buttonReStart->setObjectName(QString::fromUtf8("buttonReStart"));
        buttonReStart->setFont(font);
        buttonReStart->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_2->addWidget(buttonReStart);

        buttonFinish = new QPushButton(frame_2);
        buttonFinish->setObjectName(QString::fromUtf8("buttonFinish"));
        buttonFinish->setFont(font);
        buttonFinish->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_2->addWidget(buttonFinish);


        gridLayout->addWidget(frame_2, 0, 1, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_11 = new QLabel(curTask);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font);

        horizontalLayout_11->addWidget(label_11);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_2);

        checkSimuTip = new QCheckBox(curTask);
        checkSimuTip->setObjectName(QString::fromUtf8("checkSimuTip"));

        horizontalLayout_11->addWidget(checkSimuTip);


        verticalLayout_3->addLayout(horizontalLayout_11);

        textTaskTip = new QTextBrowser(curTask);
        textTaskTip->setObjectName(QString::fromUtf8("textTaskTip"));
        textTaskTip->setStyleSheet(QString::fromUtf8("background-color: qradialgradient(spread:pad, cx:0.272727, cy:0.352, radius:1.088, fx:0, fy:0, stop:0 rgba(209, 209, 209, 255), stop:0.480226 rgba(207, 207, 207, 255), stop:1 rgba(230, 230, 230, 255));"));

        verticalLayout_3->addWidget(textTaskTip);


        gridLayout->addLayout(verticalLayout_3, 1, 0, 1, 2);

        tabWidget->addTab(curTask, QString());
        frame->raise();
        frame_2->raise();
        taskList = new QWidget();
        taskList->setObjectName(QString::fromUtf8("taskList"));
        gridLayout_4 = new QGridLayout(taskList);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        listTask = new QTableWidget(taskList);
        if (listTask->columnCount() < 4)
            listTask->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        listTask->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        listTask->setObjectName(QString::fromUtf8("listTask"));
        listTask->horizontalHeader()->setDefaultSectionSize(120);
        listTask->horizontalHeader()->setMinimumSectionSize(40);

        gridLayout_4->addWidget(listTask, 0, 0, 1, 1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        buttonSetCurTask = new QPushButton(taskList);
        buttonSetCurTask->setObjectName(QString::fromUtf8("buttonSetCurTask"));
        buttonSetCurTask->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_10->addWidget(buttonSetCurTask);

        buttonDelTask = new QPushButton(taskList);
        buttonDelTask->setObjectName(QString::fromUtf8("buttonDelTask"));
        buttonDelTask->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_10->addWidget(buttonDelTask);

        buttonClearTask = new QPushButton(taskList);
        buttonClearTask->setObjectName(QString::fromUtf8("buttonClearTask"));
        buttonClearTask->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_10->addWidget(buttonClearTask);


        gridLayout_4->addLayout(horizontalLayout_10, 1, 0, 1, 1);

        tabWidget->addTab(taskList, QString());
        taskRec = new QWidget();
        taskRec->setObjectName(QString::fromUtf8("taskRec"));
        gridLayout_3 = new QGridLayout(taskRec);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        listTaskRec = new QTableWidget(taskRec);
        if (listTaskRec->columnCount() < 2)
            listTaskRec->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        listTaskRec->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        listTaskRec->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        listTaskRec->setObjectName(QString::fromUtf8("listTaskRec"));
        listTaskRec->setMouseTracking(true);

        gridLayout_3->addWidget(listTaskRec, 0, 0, 1, 1);

        tabWidget->addTab(taskRec, QString());
        tabPlan = new QWidget();
        tabPlan->setObjectName(QString::fromUtf8("tabPlan"));
        gridLayout_5 = new QGridLayout(tabPlan);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        tablePlan = new QTableWidget(tabPlan);
        if (tablePlan->columnCount() < 3)
            tablePlan->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tablePlan->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tablePlan->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tablePlan->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        tablePlan->setObjectName(QString::fromUtf8("tablePlan"));
        tablePlan->horizontalHeader()->setDefaultSectionSize(140);
        tablePlan->horizontalHeader()->setMinimumSectionSize(50);
        tablePlan->horizontalHeader()->setStretchLastSection(true);

        gridLayout_5->addWidget(tablePlan, 0, 0, 1, 1);

        tabWidget->addTab(tabPlan, QString());

        gridLayout_2->addWidget(tabWidget, 0, 0, 1, 1);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        buttonExit = new QPushButton(CTaskManage);
        buttonExit->setObjectName(QString::fromUtf8("buttonExit"));
        buttonExit->setStyleSheet(QString::fromUtf8(""));

        formLayout->setWidget(0, QFormLayout::FieldRole, buttonExit);

        horizontalSpacer_3 = new QSpacerItem(528, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(0, QFormLayout::LabelRole, horizontalSpacer_3);


        gridLayout_2->addLayout(formLayout, 1, 0, 1, 1);


        retranslateUi(CTaskManage);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CTaskManage);
    } // setupUi

    void retranslateUi(QWidget *CTaskManage)
    {
        CTaskManage->setWindowTitle(QApplication::translate("CTaskManage", "\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CTaskManage", "\344\273\273\345\212\241\345\220\215\347\247\260:", 0, QApplication::UnicodeUTF8));
        labelTaskName->setText(QString());
        label_3->setText(QApplication::translate("CTaskManage", "\344\273\273\345\212\241\345\261\236\346\200\247:", 0, QApplication::UnicodeUTF8));
        labelTaskAttr->setText(QString());
        label_4->setText(QApplication::translate("CTaskManage", "\346\216\245\346\224\266\346\227\266\351\227\264:", 0, QApplication::UnicodeUTF8));
        labelSendTime->setText(QString());
        label_5->setText(QApplication::translate("CTaskManage", "\345\216\202    \347\253\231:", 0, QApplication::UnicodeUTF8));
        labelStation->setText(QString());
        label_6->setText(QApplication::translate("CTaskManage", "\350\256\276    \345\244\207:", 0, QApplication::UnicodeUTF8));
        labelDev->setText(QString());
        label_8->setText(QApplication::translate("CTaskManage", "\344\273\273\345\212\241\347\212\266\346\200\201:", 0, QApplication::UnicodeUTF8));
        labelStatus->setText(QString());
        label_9->setText(QApplication::translate("CTaskManage", "\345\275\223\345\211\215\345\267\245\345\206\265:", 0, QApplication::UnicodeUTF8));
        labelScene->setText(QString());
        label_10->setText(QApplication::translate("CTaskManage", "\345\275\223\345\211\215\346\225\205\351\232\234:", 0, QApplication::UnicodeUTF8));
        labelFault->setText(QString());
        label_2->setText(QApplication::translate("CTaskManage", "\344\273\273\345\212\241\346\227\266\351\227\264:", 0, QApplication::UnicodeUTF8));
        buttonStart->setText(QApplication::translate("CTaskManage", "\344\273\273\345\212\241\345\274\200\345\247\213", 0, QApplication::UnicodeUTF8));
        buttonReStart->setText(QApplication::translate("CTaskManage", "\344\273\273\345\212\241\351\207\215\345\220\257", 0, QApplication::UnicodeUTF8));
        buttonFinish->setText(QApplication::translate("CTaskManage", "\344\273\273\345\212\241\345\256\214\346\210\220", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("CTaskManage", "\344\273\273\345\212\241\346\217\220\347\244\272:", 0, QApplication::UnicodeUTF8));
        checkSimuTip->setText(QApplication::translate("CTaskManage", "\346\230\257\345\220\246\346\230\276\347\244\272\344\273\273\345\212\241\345\212\250\346\200\201\346\217\220\347\244\272\346\241\206", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(curTask), QApplication::translate("CTaskManage", "\345\275\223\345\211\215\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = listTask->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CTaskManage", "\344\273\273\345\212\241\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = listTask->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CTaskManage", "\344\270\213\350\276\276\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = listTask->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CTaskManage", "\344\273\273\345\212\241\346\227\266\351\231\220", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = listTask->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CTaskManage", "\344\273\273\345\212\241\347\212\266\346\200\201", 0, QApplication::UnicodeUTF8));
        buttonSetCurTask->setText(QApplication::translate("CTaskManage", "\350\256\276\347\275\256\344\270\272\345\275\223\345\211\215\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        buttonDelTask->setText(QApplication::translate("CTaskManage", "\345\210\240\351\231\244\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        buttonClearTask->setText(QApplication::translate("CTaskManage", "\346\270\205\347\251\272\344\273\273\345\212\241", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(taskList), QApplication::translate("CTaskManage", "\344\273\273\345\212\241\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = listTaskRec->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("CTaskManage", "\344\273\273\345\212\241\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = listTaskRec->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("CTaskManage", "\345\276\227\345\210\206", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(taskRec), QApplication::translate("CTaskManage", "\344\273\273\345\212\241\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tablePlan->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("CTaskManage", "\345\255\246\344\271\240\350\256\241\345\210\222\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tablePlan->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("CTaskManage", "\350\256\241\345\210\222\345\255\246\345\210\206", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tablePlan->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QApplication::translate("CTaskManage", "\345\267\262\345\276\227\345\255\246\345\210\206", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabPlan), QApplication::translate("CTaskManage", "\345\255\246\344\271\240\350\256\241\345\210\222", 0, QApplication::UnicodeUTF8));
        buttonExit->setText(QApplication::translate("CTaskManage", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CTaskManage);
    } // retranslateUi

};

namespace Ui {
    class CTaskManage: public Ui_CTaskManage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TASKMANAGE_H
