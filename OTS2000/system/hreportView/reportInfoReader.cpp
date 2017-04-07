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

#include "reportInfoReader.h"

//! [0]
ReportInfoReader::ReportInfoReader(QBuffer *reportInfo)
    : reportInfo(reportInfo)
{
}
//! [0]

//! [1]
QString ReportInfoReader::readReportNameFromId(QString &reportId)
{
	reportInfo->open(QIODevice::ReadOnly);
    setDevice(reportInfo);
	
    while (!atEnd()) 
	{
        readNext();
		
        if (isStartElement()) 
		{
            if (name() == "Reports")
			{
				while (!atEnd()) 
				{
					readNext();
					
					if (isEndElement())
						continue;
					
					if (isStartElement()) 
					{
						if (name() == "Report" && attributes().value("RID") == reportId )
						{
							reportInfo->close();
							return attributes().value("Name").toString();
						}
					}
				}
			}
            else
			{
                raiseError(QObject::tr("The file is not an report infomation version 1.0 file."));
				break;
			}
		}
    }
	
	reportInfo->close();
	return "Undefined";
}

