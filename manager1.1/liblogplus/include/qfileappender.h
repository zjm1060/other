#ifndef QFILEAPPENDER_H
#define QFILEAPPENDER_H

#include "qappender.h"

class LOGPLUS_EXPORT QFileAppender : public QAppender
{
public:
	QFileAppender(const QString& filename, QIODevice::OpenMode mode, bool immediateFlush = true);
	QFileAppender(const QProperties& properties, QIODevice::OpenMode mode);
	virtual ~QFileAppender();

public:
	virtual void close();

protected:
	virtual void append(const QLoggingEvent& event);

	bool open(const QString& filename, QIODevice::OpenMode mode);
	bool reopen();

protected:
	void rolloverFiles(const QString& filename, int maxBackupIndex);

protected:
	bool m_bImmediateFlush;

	/**
	* When any append operation fails, <code>reopenDelay</code> says 
	* for how many seconds the next attempt to re-open the log file and 
	* resume logging will be delayed. If <code>reopenDelay</code> is zero, 
	* each failed append operation will cause log file to be re-opened. 
	* By default, <code>reopenDelay</code> is 1 second.
	*/
	int m_nReopenDelay;

	QString m_strFileName;

	QDateTime m_timeReopen;

private:
	void init(const QString& filename, QIODevice::OpenMode mode);

	// Disallow copying of instances of this class
	QFileAppender(const QFileAppender&);
	QFileAppender& operator=(const QFileAppender&);
};

/**
* RollingFileAppender extends FileAppender to backup the log
* files when they reach a certain size.
*/
class LOGPLUS_EXPORT QRollingFileAppender : public QFileAppender
{
public:
	QRollingFileAppender(const QString& filename,
		long maxFileSize = 10*1024*1024, // 10 MB
		int maxBackupIndex = 1, bool immediateFlush = true);
	QRollingFileAppender(const QProperties& properties);

	virtual ~QRollingFileAppender();

protected:
	virtual void append(const QLoggingEvent& event);
	void rollover();

protected:
	long m_nMaxFileSize;
	int	m_nMaxBackupIndex;

private:
	void init(long maxFileSize, int maxBackupIndex);
};

/**
* DailyRollingFileAppender extends {@link FileAppender} so that the
* underlying file is rolled over at a user chosen frequency.
*/
class LOGPLUS_EXPORT QDailyRollingFileAppender : public QFileAppender
{
public:
	enum DailyRollingFileSchedule
	{
		MONTHLY,
		WEEKLY,
		DAILY,
		TWICE_DAILY,
		HOURLY,
		MINUTELY
	};

public:
	QDailyRollingFileAppender(const QString& filename,
		DailyRollingFileSchedule schedule = DAILY,
		bool immediateFlush = true,
		int maxBackupIndex = 10);
	QDailyRollingFileAppender(const QProperties& properties);
	virtual ~QDailyRollingFileAppender();

public:
	virtual void close();

protected:
	virtual void append(const QLoggingEvent& event);

	void rollover();
	QDateTime calculateNextRolloverTime(const QDateTime& t);
	QDateTime calculateOverTime(const QDateTime& time);
	QString fileName(const QString& filename, const QDateTime& t, DailyRollingFileSchedule schedule);

protected:
	QDateTime adjustDateTime(const QDateTime& date, DailyRollingFileSchedule schedule) const;

private:
	QProperties adjustProperties(const QProperties& properties);

protected:
	DailyRollingFileSchedule m_schedule;
	QString m_strSourceFilename;
	QDateTime m_nextRolloverTime;
	int m_maxBackupIndex;

private:
	void init(DailyRollingFileSchedule schedule);
};

#endif // QFILEAPPENDER_H

