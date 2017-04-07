#include "qmibprocess.h"

#include "common.h"
#include "qmibfactory.h"
#include "qlogger.h"
#include "qcscsinitialize.h"
#include "qalarmfactory.h"

QMibProcess::QMibProcess()
:QAbstractMib(1000)
,m_nTotalTimeProcess(0)
{

}

QMibProcess::~QMibProcess()
{

}

void QMibProcess::setResult(const QVariant& var)
{
	if (!var.isValid())
	{
		return;
	}

	QVector<SystemProcess> processes=var.value<QVector<SystemProcess> >();

	quint64 nOld, nNew;

	quint64 nTotalTimeProcess=0;
	QMap<quint32, quint64> mapTimeProcess;

	QMutableVectorIterator<SystemProcess> it(processes);
	while (it.hasNext())
	{
		it.next();

		const SystemProcess& process=it.value();

#ifdef Q_OS_WIN
		if (process.processorTime != (quint64)-1)
		{
			nTotalTimeProcess+=process.processorTime;
		}

#elif defined(Q_OS_AIX)
		if (process.name == "wait" && process.executablePath.trimmed().isEmpty())
		{
			it.remove();
		}
#endif
	}

	for (int i=0; i<processes.size(); i++)
	{
		SystemProcess& process=processes[i];

#if defined(Q_OS_WIN) || defined(Q_OS_AIX)
		if (process.processorTime == (quint64)-1)
		{
			process.processorTime=0;
		}

#ifdef Q_OS_WIN
		if (m_nTotalTimeProcess > 0 && nTotalTimeProcess > m_nTotalTimeProcess)
		{
#endif
			mapTimeProcess.insert(process.processId, process.processorTime);

			if (m_mapTimeProcess.contains(process.processId))
			{
				nOld=m_mapTimeProcess[process.processId];
				nNew=process.processorTime;
				if (nOld == (quint64)-1 || nNew == (quint64)-1)
				{
					process.percentProcessorTime=0.0;
				}
				else
				{
					if (nNew>nOld)
					{
#ifdef Q_OS_WIN
						process.percentProcessorTime=(nNew-nOld)*100.0/(nTotalTimeProcess-m_nTotalTimeProcess);
#elif defined(Q_OS_AIX)
						int count=0;
						QVariant vt;
						QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_COMPUTER_SYSTEM);
						if (pInstance != NULL)
						{
							vt=pInstance->result();
						}

						if (vt.isValid())
						{
							ComputerSystem system=vt.value<ComputerSystem>();
							count=system.numberOfProcessors;
						}

						count=count>0? count:1;
						process.percentProcessorTime=(nNew-nOld)*100.0/(1000000000.0*count);
#endif

					}
					else
					{
						process.percentProcessorTime=0.0;
					}
				}
			}
			else
			{
				process.percentProcessorTime=0.0;
			}
#ifdef Q_OS_WIN
		}
		else
		{
			process.percentProcessorTime=0.0;
		}
#endif

#endif
	}

	m_nTotalTimeProcess=nTotalTimeProcess;
	m_mapTimeProcess=mapTimeProcess;

	m_vecProcess=processes;
}

void QMibProcess::setResult(const QByteArray& buffer)
{
	Q_READ_ONLY_DATA_STREAM(out, buffer);

	QVector<SystemProcess> processes;
	out>>processes;

	m_vecProcess=processes;

#ifdef H9000_ENABLE
	bool bQuit=true;		//H9000ÊÇ·ñÍË³ö
	QRegExp rx(QString("^%1\\..xe$").arg(H9000_SYSMGR_MANE));
#endif

	QString strH9000Path=getEnvPath("h9000");
	for (int i=0; i<processes.size(); i++)
	{
		SystemProcess& process=processes[i];

		if (!strH9000Path.isEmpty() && !process.executablePath.contains('/'))
		{
			QStringList list=process.executablePath.split(QRegExp("\\W+"), QString::SkipEmptyParts);
			if (list.size() > 0)
			{
				if (QFile::exists(appendPath(strH9000Path, list[0])))
				{
					process.executablePath=appendPath(strH9000Path, list[0]);

					QStringListIterator it(list);
					it.next();

					while (it.hasNext())
					{
						process.executablePath=" "+it.next();
					}
				}
			}
		}

#ifdef H9000_ENABLE
		if (rx.indexIn(process.name) != -1)
		{
			bQuit=false;
		}
#endif
	}

#ifdef H9000_ENABLE
	QCscsInitialize* ptr=QCscsInitialize::instance();
	if (ptr != NULL)
	{
		bQuit? ptr->destroy():ptr->initialize();
	}

	QAbstractAlarmPtr pAlarm=QAlarmFactory::instance()->alarmInstance(H9000AlarmEvent);
	if (pAlarm != NULL)
	{
		bQuit? pAlarm->destroy():pAlarm->initialize();
	}
#endif
}

REGISTER_MIB_INSTANCE(QMibProcess)
