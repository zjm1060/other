/****************************************************************************
**
** Copyright (C) 2004-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

//! [0]
#include <QtGui>
#include <QIOdevice>

#include "mainwindow.h"
#include "svgwindow.h"
//! [0]

#include "reportService.h"
#include "reportTreeReader.h"
#include "reportConfigReader.h"
#include "timeInput.h"
#include "reportGen.h"

//! [1]
MainWindow::MainWindow()
//! [1] //! [2]
{
	setWindowTitle(tr("报表浏览"));
//	resize(600, 500);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
	createClientArea();

	createReportService();
	showMaximized();
}
//! [2]

void MainWindow::createReportService()
{
	reportService= new ReportService;
	reportService->window= this;
}

void MainWindow::createClientArea()
{
	mainWin= new QSplitter(Qt::Horizontal, this);
    setCentralWidget(mainWin);	
	
	reportTree = new QTreeWidget;
	reportTree->setColumnCount(2);
	reportTree->header()->setResizeMode(QHeaderView::ResizeToContents);
    QStringList labels;
	labels << tr("名称") << tr("ID");
	reportTree->setHeaderLabels(labels);
	mainWin->addWidget(reportTree);
	
    reportWin = new SvgWindow;	
	mainWin->addWidget(reportWin);
	
    QList<int> sizes;
	sizes.append(100);
	sizes.append(400);	
    mainWin->setSizes(sizes);
	
}



//! [7]
void MainWindow::open()
//! [7] //! [8]
{
	QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty())
		loadFile(fileName);
}
//! [8]


//! [13]
void MainWindow::about()
//! [13] //! [14]
{
   QMessageBox::about(this, tr("报表浏览"),
            tr("<b>报表浏览</b>程序版权归北京中水科水电科技开发有限公司所有"));
}
//! [14]


//! [17]
void MainWindow::createActions()
//! [17] //! [18]
{
    getReportListAct = new QAction(QIcon(":/images/new.png"), tr("&目录"), this);
    getReportListAct->setShortcut(tr("Ctrl+R"));
    getReportListAct->setStatusTip(tr("Get hreport list"));
    connect(getReportListAct, SIGNAL(triggered()), this, SLOT(GetReportTree()));

    reportQueryAct = new QAction(QIcon(":/images/paste.png"), tr("&查询"), this);
    reportQueryAct->setShortcut(tr("Ctrl+Q"));
    reportQueryAct->setStatusTip(tr("Query report"));
    connect(reportQueryAct, SIGNAL(triggered()), this, SLOT(QueryReport()));
    reportQueryAct->setEnabled(false);
	
//! [19]
    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
//! [18] //! [19]

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

//! [20]
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
//! [20]
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

}
//! [24]

//! [25] //! [26]
void MainWindow::createMenus()
//! [25] //! [27]
{
    fileMenu = menuBar()->addMenu(tr("&File"));
//! [28]
    fileMenu->addAction(openAct);
//! [28]
    fileMenu->addAction(saveAct);
//! [26]
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    reportMenu = menuBar()->addMenu(tr("&Report"));
    reportMenu->addAction(getReportListAct);
    reportMenu->addAction(reportQueryAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}
//! [27]

//! [29] //! [30]
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    reportToolBar = addToolBar(tr("Report"));
    reportToolBar->addAction(getReportListAct);
    reportToolBar->addAction(reportQueryAct);
}
//! [30]

//! [32]
void MainWindow::createStatusBar()
//! [32] //! [33]
{
    statusBar()->showMessage(tr("Ready"));
}
//! [33]


//! [42]
void MainWindow::loadFile(const QString &fileName)
//! [42] //! [43]
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
//    textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
	reportWin->displaySvg(fileName);
}
//! [43]

void MainWindow::setCurrentFile(const QString &fileName)
//! [46] //! [47]
{
    curFile = fileName;
    setWindowModified(false);
	
    QString shownName;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = curFile;
	
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Application")));
}



void MainWindow::GetReportTree()
{
	QBuffer reportTreeContent, reportInfoContent;
	
    QApplication::setOverrideCursor(Qt::WaitCursor);
	if(reportService->GetReportTree(&reportTreeContent) && reportService->GetTemplateInformation(&reportInfoContent))
	{
		reportTree->clear();
		ReportTreeReader reader(reportTree);
		reader.readReportTree(&reportTreeContent, &reportInfoContent);
		connect(reportTree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(GetTemplate(QTreeWidgetItem *, QTreeWidgetItem *)));
	}
	else
	{
		reportQueryAct->setEnabled(false);
		QMessageBox::warning(this, tr("错误"),tr("查询报表目录失败"));
	}
	QApplication::restoreOverrideCursor();
}


void MainWindow::QueryReport()
{
	QTreeWidgetItem *currItem= reportTree->currentItem();

	if(currItem==NULL)
	{
		reportQueryAct->setEnabled(false);
        QMessageBox::warning(this, tr("提示"),
			tr("请选择要查询的报表"));
		return;
	}
	
	int reportId= currItem->text(1).toInt();
	if(reportId>0)
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		
		if(!(reportService->GetTemplateConfig(reportId, &configContent)))
		{
			reportQueryAct->setEnabled(false);
			QMessageBox::warning(this, tr("错误"),tr("查询报表配置失败"));
			return;
		}
		ReportConfigReader configReader(&configContent);
		bool isEndTimeValid= configReader.IsEndTimeValid();
		int periodUnit= configReader.getPeriodUnit();
		
		QApplication::restoreOverrideCursor();

		TimeInputDialog	dialog(periodUnit, isEndTimeValid);
		if(dialog.exec())
		{
			QApplication::setOverrideCursor(Qt::WaitCursor);
			QBuffer	dataContent;
			if(reportService->queryReport(reportId, dialog.useSavedReport, dialog.queryStartTime.toString(Qt::ISODate), 
				dialog.queryEndTime.toString(Qt::ISODate), NULL, &dataContent))
			{
				ReportGen report;
				report.genReport(this, &templateContent, &dataContent);
				reportWin->displaySvg(report.content.buffer());
			}
			else
				QMessageBox::warning(this, tr("错误"),tr("查询报表失败"));
			QApplication::restoreOverrideCursor();				
		}
	}	
}


void MainWindow::GetTemplate(QTreeWidgetItem *currItem, QTreeWidgetItem *prevItem)
{
	if(currItem==NULL)
	{
        QMessageBox::warning(this, tr("提示"),
			tr("请选择要查询的报表"));
		return;
	}
	
	int reportId= currItem->text(1).toInt();
	if(reportId>0)
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		
		if(!(reportService->GetTemplate(reportId, &templateContent)))
		{
			QMessageBox::warning(this, tr("错误"),tr("查询报表模板失败"));
			reportQueryAct->setEnabled(false);
			return;
		}
		reportWin->displaySvg(templateContent.buffer());
		reportQueryAct->setEnabled(true);
		QApplication::restoreOverrideCursor();				
	}
}