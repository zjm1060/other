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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include "svgwindow.h"
#include <QXmlStreamReader>


QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextEdit;
class QTreeWidget;
class QTreeWidgetItem;
class QSplitter;
class ReportTreeReader;
class ReportService;
QT_END_NAMESPACE


//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:

private slots:
    void open();
    void about();
	void GetReportTree();
	void QueryReport();
    void GetTemplate(QTreeWidgetItem *, QTreeWidgetItem *);

private:
    void createReportService();
    void createClientArea();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void loadFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
	
	QString curFile;

    QMenu *fileMenu;
    QMenu *reportMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *reportToolBar;
    QAction *getReportListAct;
    QAction *reportQueryAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *aboutAct;

	QSplitter *mainWin;
    SvgWindow *reportWin;
    QTreeWidget *reportTree;
	ReportService *reportService;
	
	QBuffer templateContent;
	QBuffer configContent;
};
//! [0]

#endif
