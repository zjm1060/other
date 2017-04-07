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
#include <QApplication>
#include <qtextcodec.h>

#include "mainwindow.h"


#pragma	comment(lib, "wsock32.lib")
//#pragma	comment(lib, "QtSvgd4.lib")
//#pragma	comment(lib, "QtXmld4.lib")
//#pragma	comment(lib, "QtGuid4.lib")
//#pragma	comment(lib, "QtCored4.lib")
	  
int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(hreportView);

    QApplication app(argc, argv);
	QTextCodec *zhCodec= QTextCodec::codecForName("GBK");
	QTextCodec::setCodecForTr(zhCodec);
//	QFont font("Times",12,QFont::Normal,FALSE);
//	app.setFont(font);
    MainWindow mainWin;
	
    mainWin.show();
    return app.exec();
}
//! [0]
