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

#include <QtGui>

#include "reportTreeReader.h"
#include "reportInfoReader.h"

//! [0]
ReportTreeReader::ReportTreeReader(QTreeWidget *treeWidget)
    : treeWidget(treeWidget)
{
    QStyle *style = treeWidget->style();

    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    reportIcon.addPixmap(style->standardPixmap(QStyle::SP_FileIcon));
}
//! [0]

//! [1]
bool ReportTreeReader::readReportTree(QBuffer *reportTreeContent, QBuffer *reportInfoContent)
{
	reportInfo= reportInfoContent;

	reportTreeContent->open(QIODevice::ReadOnly);
    setDevice(reportTreeContent);

    while (!atEnd()) {
        readNext();

        if (isStartElement()) {
            if (name() == "TREENODES")
                readTree();
            else
                raiseError(QObject::tr("The file is not an report tree version 1.0 file."));
        }
    }

	reportTreeContent->close();
	
    return !error();
}
//! [1]

//! [2]
void ReportTreeReader::readUnknownElement()
{
    Q_ASSERT(isStartElement());

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement())
            readUnknownElement();
    }
}
//! [2]

//! [3]
void ReportTreeReader::readTree()
{
    Q_ASSERT(isStartElement() && name() == "TREENODES");

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
            if (name() == "TREENODE" && attributes().value("Type") == "ReportCatalog" )
                readFolder(0);
            else if (name() == "TREENODE" && attributes().value("Type") == "Report" )
                readReport(0);
            else
                readUnknownElement();
        }
    }
}
//! [3]


void ReportTreeReader::readFolder(QTreeWidgetItem *item)
{
    Q_ASSERT(isStartElement() && name() == "TREENODE" && attributes().value("Type") == "ReportCatalog");

    QTreeWidgetItem *folder = createChildItem(item);
    folder->setIcon(0, reportIcon);
    folder->setText(0, attributes().value("Text").toString());
	
    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
            if (name() == "TREENODE" && attributes().value("Type") == "Report" )
                readReport(folder);
            else
                readUnknownElement();
        }
    }
}

void ReportTreeReader::readReport(QTreeWidgetItem *item)
{
    Q_ASSERT(isStartElement() && name() == "TREENODE" && attributes().value("Type") == "Report");

    QTreeWidgetItem *report = createChildItem(item);
    report->setIcon(0, reportIcon);
    report->setText(1, attributes().value("RID").toString());
	ReportInfoReader infoReader(reportInfo);
	QString reportName= infoReader.readReportNameFromId(attributes().value("RID").toString());
    report->setText(0, reportName);

    while (!atEnd()) 
	{
        readNext();
		
        if (isEndElement())
            break;

        if (isStartElement())
			readUnknownElement();
	}
}

QTreeWidgetItem *ReportTreeReader::createChildItem(QTreeWidgetItem *item)
{
    QTreeWidgetItem *childItem;
    if (item) {
        childItem = new QTreeWidgetItem(item);
    } else {
        childItem = new QTreeWidgetItem(treeWidget);
    }
    childItem->setData(0, Qt::UserRole, name().toString());
    return childItem;
}


