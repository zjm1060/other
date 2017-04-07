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

#include "reportConfigReader.h"

//! [0]
ReportConfigReader::ReportConfigReader(QBuffer *reportConfig)
    : reportConfig(reportConfig)
{
}
//! [0]

//! [1]
bool ReportConfigReader::IsEndTimeValid()
{
	reportConfig->open(QIODevice::ReadOnly);
    setDevice(reportConfig);
	
    while (!atEnd()) 
	{
        readNext();
		
        if (isStartElement()) 
		{
            if (name() == "Configuration")
			{
				if (attributes().value("InputEndTime") == "True")
				{
					reportConfig->close();
					return true;
				}
				else
					break;
			}
            else
			{
                raiseError(QObject::tr("The file is not an report config version 1.0 file."));
				break;
			}
		}
    }
	
	reportConfig->close();
	return false;
}

int ReportConfigReader::getPeriodUnit()
{
	int periodUnit= -1;
	reportConfig->open(QIODevice::ReadOnly);
    setDevice(reportConfig);
	
    while (!atEnd()) 
	{
        readNext();
		
        if (isStartElement()) 
		{
            if (name() == "AutoGenerate")
			{
				QStringRef ref= attributes().value("periodUnit");
				if(!ref.isEmpty())
				{
					QString string= ref.toString();
					QByteArray info= string.toUtf8();
					periodUnit= info.toInt();
				}
				break;
			}
		}
    }
	
	reportConfig->close();
	return periodUnit;
}

