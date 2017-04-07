#ifndef QREADWRITEALARMFROMXMLFILE_H
#define QREADWRITEALARMFROMXMLFILE_H

#include "global.h"
#include "qabstractreadwritexmlfile.h"

class ALARM_EXPORT QReadH9000FileSystemAlarmFormXmlFile : public QAbstractReadXmlFile
{
public:
	QReadH9000FileSystemAlarmFormXmlFile(const QString& filename);
	~QReadH9000FileSystemAlarmFormXmlFile();

public:
	typedef struct _alarmConfigure 
	{
		double	lowLevel;
		double	upperLevel;

		QMap<QString, QString>	partitions;
	}AlarmConfigure;

public:
	AlarmConfigure result()
	{
		return m_configure;
	}

protected:
	virtual void readNode(QXmlStreamReader& reader);

private:
	void readH9000Node(QXmlStreamReader& reader);
	void readFileSystemNode(QXmlStreamReader& reader);
	void readPartitionNode(QXmlStreamReader& reader);

private:
	AlarmConfigure	m_configure;
};

#endif // QREADWRITEALARMFROMXMLFILE_H
