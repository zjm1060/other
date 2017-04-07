/****************************************************************************
**
** Copyright (C) 2005-2008 Trolltech ASA. All rights reserved.
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

#ifndef ReportTreeReader_H
#define ReportTreeReader_H

#include <QIcon>
#include <QXmlStreamReader>

QT_BEGIN_NAMESPACE
class QTreeWidget;
class QTreeWidgetItem;
class ReportTreeReader;
QT_END_NAMESPACE

//! [0]
class ReportTreeReader : public QXmlStreamReader
{
public:
//! [1]
    ReportTreeReader(QTreeWidget *treeWidget);
//! [1]

    bool readReportTree(QBuffer *reportTreeContent, QBuffer *reportInfoContent);
		
private:
//! [2]
    void readUnknownElement();
    void readTree();
    void readFolder(QTreeWidgetItem *item);
    void readReport(QTreeWidgetItem *item);
    void readReportNameFromId(int id, QString *name);

    QTreeWidgetItem *createChildItem(QTreeWidgetItem *item);

    QTreeWidget *treeWidget;
//! [2]

	QBuffer *reportInfo;
    QIcon folderIcon;
    QIcon reportIcon;
};
//! [0]

#endif
