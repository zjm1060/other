#include "qfileappender.h"

#include <qmath.h>
#include <time.h>

const long MINIMUM_ROLLING_LOG_SIZE = 200*1024L;

QFileAppender::QFileAppender(const QString& filename, QIODevice::OpenMode mode, bool immediateFlush)
    : m_bImmediateFlush(immediateFlush)
    , m_nReopenDelay(1)
{
    init(filename, mode);
}


QFileAppender::QFileAppender(const QProperties& properties, QIODevice::OpenMode mode)
    : QAppender(properties)
    , m_bImmediateFlush(true)
    , m_nReopenDelay(1)
{
    QString filename=properties.property("File").toString();
    if (filename.isEmpty())
    {
        errorHandler()->error("Invalid filename");
        return;
    }

    if(properties.exists("ImmediateFlush"))
	{
        QString tmp=properties.property("ImmediateFlush").toString().toLower();
        m_bImmediateFlush=(tmp == "true");
    }

	bool bAppend=false;
    if(properties.exists("Append"))
	{
        QString tmp=properties.property("Append").toString().toLower();
        bAppend = (tmp == "true");
    }

    if(properties.exists("ReopenDelay"))
	{
        m_nReopenDelay= properties.property("ReopenDelay").toString().toInt();
    }

    init(filename, (bAppend ? QIODevice::Append:QIODevice::Truncate) | mode);
}

void QFileAppender::init(const QString& filename, QIODevice::OpenMode mode)
{
    m_strFileName=filename;
    open(mode);

    if(m_out.device() == NULL || !m_out.device()->isOpen())
	{
        errorHandler()->error("Unable to open file: "+ m_strFileName);
        return;
    }

    logLog().debug("Just opened file: " + filename);
}

QFileAppender::~QFileAppender()
{
    destroy();
}

void QFileAppender::close()
{
	QMutexLocker locker(&m_mutex);
	m_file.close();
	m_bClosed=true;
}

void QFileAppender::append(const QLoggingEvent& event)
{
    if(m_out.device() == NULL || !m_out.device()->isOpen())
	{
        if(!reopen())
		{
            errorHandler()->error("file is not open: " + m_strFileName);
            return;
        }
        else
		{
            errorHandler()->reset();
		}
    }

    m_layout->formatAndAppend(m_out, event);

    if(m_bImmediateFlush)
	{
        m_out.flush();
    }
}

void QFileAppender::open(QIODevice::OpenMode mode)
{
	if (m_file.isOpen())
	{
		m_file.close();
	}

	m_file.setFileName(m_strFileName);
	mode|=(QIODevice::WriteOnly | QIODevice::Text);
	if (m_file.open(mode))
	{
		m_out.setDevice(&m_file);
	}
}

bool QFileAppender::reopen()
{
	if (m_timeReopen.isValid()  && m_nReopenDelay != 0)
	{
        m_timeReopen = QDateTime::currentDateTime().addSecs(m_nReopenDelay);
	}
    else
	{
		if (m_timeReopen <= QDateTime::currentDateTime() || m_nReopenDelay == 0)
		{
			if (m_file.isOpen())
			{
				m_file.close();
			}
			
			open(QIODevice::Append);

            m_timeReopen=QDateTime();

            if(m_out.device() != NULL && m_out.device()->isOpen())
			{
                return true;
			}
        }
    }

    return false;
}

void QFileAppender::rolloverFiles(const QString& filename, int maxBackupIndex)
{
	QString strFileName=addSerialNumberToFileName(filename, maxBackupIndex);
	QFile::remove(strFileName);

	QString strSource, strTarget;
	for (int i = maxBackupIndex - 1; i >= 1; --i)
	{
		strSource=addSerialNumberToFileName(filename, i);
		strTarget=addSerialNumberToFileName(filename, i+1);

		QFile::rename(strSource, strTarget);
	}
}

QRollingFileAppender::QRollingFileAppender(const QString& filename, long maxFileSize, int maxBackupIndex, bool immediateFlush)
:QFileAppender(filename, QIODevice::Append, immediateFlush)
{
    init(maxFileSize, maxBackupIndex);
}


QRollingFileAppender::QRollingFileAppender(const QProperties& properties)
:QFileAppender(properties, QIODevice::Append)
{
    long maxFileSize = 10*1024*1024;
    int maxBackupIndex = 1;
    if(properties.exists("MaxFileSize"))
	{
        QString tmp = properties.property("MaxFileSize").toString().toUpper();
		if (tmp.endsWith("MB"))
		{
			tmp=tmp.left(tmp.length()-2);
			maxFileSize=tmp.toLong()*1024*1024;
		}
		else if (tmp.endsWith("KB"))
		{
			tmp=tmp.left(tmp.length()-2);
			maxFileSize=tmp.toLong()*1024;
		}
		else
		{
			maxFileSize=tmp.toLong();
		}
    }

    if(properties.exists("MaxBackupIndex"))
	{
		maxBackupIndex=properties.property("MaxBackupIndex").toInt();
    }

    init(maxFileSize, maxBackupIndex);
}

void QRollingFileAppender::init(long maxFileSize, int maxBackupIndex)
{
    if (maxFileSize < MINIMUM_ROLLING_LOG_SIZE)
    {
        logLog().warn("RollingFileAppender: MaxFileSize property value is too small. Resetting to %d .", MINIMUM_ROLLING_LOG_SIZE);
        maxFileSize = MINIMUM_ROLLING_LOG_SIZE;
    }

    m_nMaxFileSize = maxFileSize;
    m_nMaxBackupIndex = qMax(maxBackupIndex, 1);
}

QRollingFileAppender::~QRollingFileAppender()
{
    destroy();
}

void QRollingFileAppender::append(const QLoggingEvent& event)
{
    QFileAppender::append(event);

    if(m_file.size() > m_nMaxFileSize)
	{
        rollover();
    }
}

void QRollingFileAppender::rollover()
{
	if (m_file.isOpen())
	{
		m_file.close();
	}

    // If maxBackups <= 0, then there is no file renaming to be done.
    if (m_nMaxBackupIndex> 0)
    {
        rolloverFiles(m_strFileName, m_nMaxBackupIndex);

        // Rename fileName to fileName.1
        QString strTarget = addSerialNumberToFileName(m_strFileName, 1);

        logLog().debug ("Renaming file "+ m_strFileName + " to " + strTarget);

		bool ret=m_file.rename(strTarget);

		if (!ret)
		{
			logLog().debug("Failed to rename file "+ m_strFileName + " to " + strTarget);
		}
    }
    else
    {
        logLog().debug (m_strFileName + " has no backups specified");
    }

	open(QIODevice::Truncate);
}

QDailyRollingFileAppender::QDailyRollingFileAppender(const QString& filename, DailyRollingFileSchedule schedule,
    bool immediateFlush, int maxBackupIndex)
	:QFileAppender(fileName(filename, adjustDateTime(QDateTime::currentDateTime()), schedule), QIODevice::Append, immediateFlush)
	,m_strSourceFilename(filename)
    , m_maxBackupIndex(maxBackupIndex)
{
    init(schedule);
}

QDailyRollingFileAppender::QDailyRollingFileAppender(const QProperties& properties)
    : QFileAppender(adjustProperties(properties), QIODevice::Append)
    , m_maxBackupIndex(10)
{
    if(properties.exists("MaxBackupIndex"))
	{
        m_maxBackupIndex = properties.property("MaxBackupIndex").toString().toInt();
    }

    init(m_schedule);
}

QDailyRollingFileAppender::~QDailyRollingFileAppender()
{
	destroy();
}

QProperties QDailyRollingFileAppender::adjustProperties(const QProperties& properties)
{
	QProperties proper(properties);

	DailyRollingFileSchedule schedule = DAILY;
	QString strSchedule= proper.property("Schedule").toString().toUpper();

	if(strSchedule == "MONTHLY")
		schedule = MONTHLY;
	else if(strSchedule == "WEEKLY")
		schedule = WEEKLY;
	else if(strSchedule == "DAILY")
		schedule = DAILY;
	else if(strSchedule == "TWICE_DAILY")
		schedule = TWICE_DAILY;
	else if(strSchedule == "HOURLY")
		schedule = HOURLY;
	else if(strSchedule == "MINUTELY")
		schedule = MINUTELY;
	else
	{
		logLog().warn("DailyRollingFileAppender::ctor()- \"Schedule\" not valid: " + properties.property("Schedule").toString());
		schedule = DAILY;
	}

	m_schedule=schedule;

	QString filename=properties.property("File").toString();
	if (filename.isEmpty())
	{
		errorHandler()->error("Invalid filename");
	}
	else
	{
		proper.setProperty("File", fileName(filename, adjustDateTime(QDateTime::currentDateTime()), schedule));
	}

	return proper;
}

void QDailyRollingFileAppender::init(DailyRollingFileSchedule schedule)
{
    m_schedule= schedule;

	QDateTime now=adjustDateTime(QDateTime::currentDateTime());
    m_nextRolloverTime = calculateNextRolloverTime(now);
}

QDateTime QDailyRollingFileAppender::adjustDateTime(const QDateTime& date) const
{
	QString strTime=date.toString("yyyy:M:d:h:m");

	struct tm time;
	memset(&time, 0, sizeof(struct tm));

	sscanf_s(strTime.toAscii().data(), "%d:%d:%d:%d:%d", &time.tm_year, &time.tm_mon, &time.tm_mday, &time.tm_hour, &time.tm_min);
	time.tm_year-=1900;
	mktime(&time);

	switch (m_schedule)
	{
	case MONTHLY:
		time.tm_mday = 1;
		time.tm_hour = 0;
		time.tm_min = 0;
		break;

	case WEEKLY:
		time.tm_mday -= (time.tm_wday % 7);
		time.tm_hour = 0;
		time.tm_min = 0;
		break;

	case DAILY:
		time.tm_hour = 0;
		time.tm_min = 0;
		break;

	case TWICE_DAILY:
		if(time.tm_hour >= 12)
		{
			time.tm_hour = 12;
		}
		else
		{
			time.tm_hour = 0;
		}
		time.tm_min = 0;
		break;

	case HOURLY:
		time.tm_min = 0;
		break;

	case MINUTELY:
		break;
	};

	strTime.sprintf("%d:%d:%d:%d:%d", time.tm_year+1900, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min);

	return QDateTime::fromString(strTime, "yyyy:M:d:h:m");
}

void QDailyRollingFileAppender::close()
{
    rollover();
    QFileAppender::close();
}

void QDailyRollingFileAppender::append(const QLoggingEvent& event)
{
    if(event.timestamp() >= m_nextRolloverTime)
	{
        rollover();
    }

    QFileAppender::append(event);
}

void QDailyRollingFileAppender::rollover()
{
    QDateTime now = QDateTime::currentDateTime();
    if (now >= m_nextRolloverTime)
    {
		now=adjustDateTime(now);

		if (m_file.isOpen())
		{
			m_file.close();
		}

		QDateTime old=calculateOverTime(now);
		QString str=fileName(m_strSourceFilename, old, m_schedule);
		if (QFile::exists(str))
		{
			QFile::remove(str);
		}

		m_strFileName=fileName(m_strSourceFilename, now, m_schedule);
		open(QIODevice::Truncate);

		m_nextRolloverTime = calculateNextRolloverTime(now);
    }
	else
	{
		logLog().warn("The current datetime < next rollover time!");
	}
}

QDateTime QDailyRollingFileAppender::calculateNextRolloverTime(const QDateTime& time) const
{
    switch(m_schedule)
    {
    case MONTHLY: 
		return time.addMonths(1);

    case WEEKLY:
        return time.addDays(7);

	default:
		logLog ().error ("DailyRollingFileAppender::calculateNextRolloverTime() - invalid schedule value");
    case DAILY:
        return time.addDays(1);

    case TWICE_DAILY:
        return time.addSecs(12*60*60);

    case HOURLY:
        return time.addSecs(60*60);

    case MINUTELY:
        return time.addSecs(60);
    };
}

QDateTime QDailyRollingFileAppender::calculateOverTime(const QDateTime& time) const
{
	switch(m_schedule)
	{
	case MONTHLY: 
		return time.addMonths(-m_maxBackupIndex);

	case WEEKLY:
		return time.addDays(-m_maxBackupIndex);

	default:
		logLog ().error ("DailyRollingFileAppender::calculateOverTime() - invalid schedule value");
	case DAILY:
		return time.addDays(-m_maxBackupIndex);

	case TWICE_DAILY:
		return time.addSecs((-m_maxBackupIndex)*12*60*60);

	case HOURLY:
		return time.addSecs((-m_maxBackupIndex)*60*60);

	case MINUTELY:
		return time.addSecs((-m_maxBackupIndex)*60);
	};
}

QString QDailyRollingFileAppender::fileName(const QString& filename, const QDateTime& t, DailyRollingFileSchedule schedule) const
{
    QString format;
    switch (schedule)
    {
    case MONTHLY:
        format = "yyyy-MM";
        break;

    case WEEKLY:
        format = "yyyy-MM-dd";
        break;

    case DAILY:
        format = "yyyy-MM-dd";
        break;

    case TWICE_DAILY:
        format = "yyyy-MM-dd AP";
        break;

    case HOURLY:
        format = "yyyy-MM-dd-hh";
        break;

    case MINUTELY:
        format = "yyyy-MM-dd-hh-mm";
        break;
	default:
		logLog().error("QDailyRollingFileAppender::fileName() - invalid schedule value");
		format = "yyyy-MM-dd";
		break;
    };
   
    return addSerialStringToFileName(filename, t.toString(format));
}
