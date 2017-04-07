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

#include <timeInput.h>


void TimeInputDialog::updateTime(QDateTime startTime)
{
	if(periodUnit==Year) 
	{
		QDate date(startTime.date().year(), 1, 1);
		QTime time(0, 0, 0);
		queryStartTime.setDate(date);
		queryStartTime.setTime(time);
		queryEndTime= queryStartTime.addYears(1);
	}
	else if(periodUnit==Month) 
	{
		QDate date(startTime.date().year(), startTime.date().month(), 1);
		QTime time(0, 0, 0);
		queryStartTime.setDate(date);
		queryStartTime.setTime(time);
		queryEndTime= queryStartTime.addMonths(1);
	}
	else if(periodUnit==Week) 
	{
		QDate date(startTime.date().year(), startTime.date().month(), startTime.date().day());
		QTime time(0, 0, 0);
		queryStartTime.setDate(date);
		queryStartTime.setTime(time);
		queryStartTime.addDays(-7);
		queryEndTime= queryStartTime.addDays(7);
	}
	else if(periodUnit==Day) 
	{
		QDate date(startTime.date().year(), startTime.date().month(), startTime.date().day());
		QTime time(0, 0, 0);
		queryStartTime.setDate(date);
		queryStartTime.setTime(time);
		queryEndTime= queryStartTime.addDays(1);
	}
	else if(periodUnit==Hour)
	{
		QDate date(startTime.date().year(), startTime.date().month(), startTime.date().day());
		QTime time(startTime.time().hour(), 0, 0);
		queryStartTime.setDate(date);
		queryStartTime.setTime(time);
		queryEndTime= queryStartTime.addSecs(3600);
	}
	else if(periodUnit==Minute)
	{
		QDate date(startTime.date().year(), startTime.date().month(), startTime.date().day());
		QTime time(startTime.time().hour(), startTime.time().minute(), 0);
		queryStartTime.setDate(date);
		queryStartTime.setTime(time);
		queryEndTime= queryStartTime.addSecs(60);
	}
	else if(periodUnit==Second)
	{
		QDate date(startTime.date().year(), startTime.date().month(), startTime.date().day());
		QTime time(startTime.time().hour(), startTime.time().minute(), startTime.time().second());
		queryStartTime.setDate(date);
		queryStartTime.setTime(time);
		queryEndTime= queryStartTime.addSecs(1);
	}
	else if(periodUnit==MSecond)
	{
		QDate date(startTime.date().year(), startTime.date().month(), startTime.date().day());
		QTime time(startTime.time().hour(), startTime.time().minute(), startTime.time().second(), startTime.time().msec());
		queryStartTime.setDate(date);
		queryStartTime.setTime(time);
		queryEndTime= queryStartTime.addMSecs(1);
	}
	else
	{
		QDate date(startTime.date().year(), startTime.date().month(), startTime.date().day());
		QTime time(startTime.time().hour(), startTime.time().minute(), startTime.time().second());
		queryStartTime.setDate(date);
		queryStartTime.setTime(time);
		queryEndTime= queryStartTime.addSecs(1);
	}
	
}

//! [0]
TimeInputDialog::TimeInputDialog(int period, bool timeValid)
	:periodUnit(period), isEndTimeValid(timeValid)
{
	setWindowTitle(tr("查询时间输入"));
	
	updateTime(QDateTime().currentDateTime());
	
	QString timeFormat;
	switch(periodUnit)
	{
	case Year:
		timeFormat= "yyyy";
		break;
	case Month:
		timeFormat= "yyyy-MM";
		break;
	case Week:
		timeFormat= "yyyy-MM-dd";
		break;
	case Day:
		timeFormat= "yyyy-MM-dd";
		break;
	case Hour:
		timeFormat= "yyyy-MM-dd hh";
		break;
	case Minute:
		timeFormat= "yyyy-MM-dd hh:mm";
		break;
	case Second:
		timeFormat= "yyyy-MM-dd hh:mm:ss";
		break;
	case MSecond:
		timeFormat= "yyyy-MM-dd hh:mm:ss";
		break;
	default:
		timeFormat= "yyyy-MM-dd hh:mm:ss";
		break;
	}
//! [1]
    QLabel *startTimeLabel = new QLabel(tr("开始时间:"));
    startTimeEdit = new QDateTimeEdit;
    startTimeEdit->setDisplayFormat(timeFormat);
    startTimeEdit->setDateRange(QDate(2000, 1, 1), QDate(2099, 1, 1));
    startTimeEdit->setDateTime(queryStartTime);
	startTimeEdit->setCurrentSection(QDateTimeEdit::DaySection);
	startTimeEdit->setCalendarPopup(true);

    QHBoxLayout *startTimeLayout = new QHBoxLayout;
    startTimeLayout->addWidget(startTimeLabel);
    startTimeLayout->addWidget(startTimeEdit);

    QLabel *endTimeLabel = new QLabel(tr("结束时间:"));
    endTimeEdit = new QDateTimeEdit;
    endTimeEdit->setDisplayFormat(timeFormat);
    endTimeEdit->setDateRange(QDate(2000, 1, 1), QDate(2099, 1, 1));
    endTimeEdit->setDateTime(queryEndTime);
	endTimeEdit->setSelectedSection(QDateTimeEdit::DaySection);
	endTimeEdit->setCalendarPopup(true);
	if(!isEndTimeValid)
		endTimeEdit->setReadOnly(true);
	
    QHBoxLayout *endTimeLayout = new QHBoxLayout;
    endTimeLayout->addWidget(endTimeLabel);
    endTimeLayout->addWidget(endTimeEdit);
	
    QPushButton *ackButton = new QPushButton(tr("确定"));
    QPushButton *cancelButton = new QPushButton(tr("取消"));
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(ackButton);
	buttonLayout->addSpacing(80);
	buttonLayout->addWidget(cancelButton);
	
	QVBoxLayout *controlsLayout = new QVBoxLayout;
	controlsLayout->addLayout(startTimeLayout);
	controlsLayout->addLayout(endTimeLayout);
	controlsLayout->addStretch(1);
	controlsLayout->addLayout(buttonLayout);
	setLayout(controlsLayout);

	connect(startTimeEdit, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(setStartDateTime(QDateTime)));
	connect(endTimeEdit, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(setEndDateTime(QDateTime)));
	connect(ackButton, SIGNAL(released()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(released()), this, SLOT(reject()));
	
	useSavedReport= false;
}

void TimeInputDialog::setStartDateTime(QDateTime startTime)
{
	updateTime(startTime);
	if(!isEndTimeValid)
		endTimeEdit->setDateTime(queryEndTime);
}


void TimeInputDialog::setEndDateTime(QDateTime endTime)
{
	queryEndTime= endTime;
}
