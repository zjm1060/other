#include <QtGui>
#include <QIOdevice>

#include "reportGen.h"

//! [0]
void ReportGen::genReport(QWidget *window, QBuffer *reportTemplate, QBuffer *reportData)
{
    QString errorStr;
    int errorLine;
    int errorColumn;
	QDomDocument templateDoc;

	QBuffer temp(&reportTemplate->buffer());

	temp.open(QIODevice::ReadWrite | QFile::Text);
    if (!templateDoc.setContent(&temp, true, &errorStr, &errorLine,
		&errorColumn)) 
	{
        QMessageBox::information(window, window->tr("DOM Bookmarks"),
			window->tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr));
        return;
    }
	
    QDomElement tempRoot = templateDoc.documentElement();
    if (tempRoot.tagName() != "svg") {
        QMessageBox::information(window, window->tr("DOM Bookmarks"),
			window->tr("This is not a SVG content."));
        return;
    }
	
    QDomElement tempChild = tempRoot.firstChildElement("g");
    while (!tempChild.isNull()) 
	{
        if(tempChild.hasAttribute("id") && tempChild.attribute("id")=="mainContent")
		{
			QDomDocument dataDoc;

			reportData->open(QIODevice::ReadOnly | QFile::Text);
			if (!dataDoc.setContent(reportData, true, &errorStr, &errorLine,
				&errorColumn)) 
			{
				QMessageBox::information(window, window->tr("DOM Bookmarks"),
					window->tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr));
				reportData->close();
				return;
			}
			
			QDomElement root = dataDoc.documentElement();
			tempRoot.replaceChild(root, tempChild);
	
			reportData->close();
			break;
		}

        tempChild = tempChild.nextSiblingElement("g");
    }
	
	temp.close();
	
	content.open(QIODevice::ReadWrite | QFile::Text);
    QTextStream out(&content);
    templateDoc.save(out, 4);
	content.close();

}
//! [0]


