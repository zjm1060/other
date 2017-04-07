#include "qstatprocess.h"

#ifndef Q_OS_WIN
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <limits.h>
#include <pwd.h>

#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

#include <errno.h>

#ifdef Q_OS_SOLARIS
#include <procfs.h>
#endif

#ifdef Q_OS_AIX
#include <sys/procfs.h>
#endif

#define PROC_LOCATION "/proc"
#define MAX_FILE_LENGTH PATH_MAX

#endif

#include <string.h>
#include <QThread>

#include "define.h"
#include "common.h"
#include "qlogger.h"

QStatProcess::QStatProcess(QObject *parent)
: QObject(parent)
{
}

QStatProcess::~QStatProcess()
{
}

void QStatProcess::clear()
{
	m_vecProcess.clear();
	m_mapUserName.clear();
}

void QStatProcess::beferQuery() 
{
	m_vecProcess.clear(); 

	QMutableMapIterator<quint32, ProccessUserName> it(m_mapUserName);

	while (it.hasNext())
	{
		it.next();

		it.value().nBeatCount++;
	}
}

void QStatProcess::afterQuery()
{
	QMutableMapIterator<quint32, ProccessUserName> it(m_mapUserName);

	while (it.hasNext())
	{
		it.next();

		if (it.value().nBeatCount > 0)
		{
			it.remove();
		}
	}
}

void QStatProcess::query()
{
#ifndef Q_OS_WIN
	DIR *proc_dir;
	struct dirent *dir_entry;
	char filename[MAX_FILE_LENGTH];
	FILE *f;
	psinfo_t process_info;

	if((proc_dir=opendir(PROC_LOCATION)) == NULL)
	{
		return;
	}

	SystemProcess process;
	while((dir_entry=readdir(proc_dir))!=NULL)
	{
		if(atoi(dir_entry->d_name) == 0 || dir_entry->d_name[0] == '.') continue;
		
		snprintf(filename, MAX_FILE_LENGTH, "/proc/%s/psinfo", dir_entry->d_name);

		if((f=fopen(filename, "r"))==NULL)
		{
			continue;
		}

		fread(&process_info, sizeof(psinfo_t), 1, f);
		fclose(f);

		process.name=QString::fromLocal8Bit(process_info.pr_fname);
		if (process.name.trimmed().isEmpty())
		{
			continue;
		}

		process.executablePath=QString::fromLocal8Bit(process_info.pr_psargs);

		process.processId=process_info.pr_pid;
		process.sessionId=process_info.pr_sid;
		process.workingSetSize=process_info.pr_rssize*1024;

#ifdef Q_OS_SOLARIS
		process.percentProcessorTime=(process_info.pr_pctcpu*100.0)/0x8000;
#elif defined(Q_OS_AIX)
		process.processorTime=process_info.pr_time.tv_sec*1000000+process_info.pr_time.tv_nsec;
#endif
		

		if (!m_mapUserName.contains(process.processId))
		{
			ProccessUserName& usrName=m_mapUserName[process.processId];
			process.usrName=usrName.processName;
			usrName.nBeatCount=0;
		}
		else
		{
			struct passwd *pwd;
			if ((pwd = getpwuid(process_info.pr_uid)) != NULL)
			{
				process.usrName=QString::fromLocal8Bit(pwd->pw_name);
			}
			else
			{
				process.usrName=QString::number(process_info.pr_uid);
			}

			ProccessUserName usrName;
			usrName.nBeatCount=0;
			usrName.processName=process.usrName;

			m_mapUserName.insert(process.processId, usrName);
		}

		m_vecProcess.append(process);
	}
	closedir(proc_dir);
#endif
}

int QStatProcess::execMethod(int method, const QStringList& arguments)
{
	int nRet=ExecMethodFailed;

	switch (method)
	{
	case MethodCreate:
		{
			if (arguments.size() == 0)
			{
				break;
			}

#ifndef Q_OS_WIN
			pid_t pid, wpid;

			pid=fork();
			if (pid < 0)
			{
				nRet=ExecMethodFailed;
				break;
			}
			else if(pid == 0)
			{
				QFileInfo info(arguments[0]);
				if (info.exists() && !info.isExecutable() && isTextFormat(arguments[0]))
				{
					execl("/usr/dt/bin/dtpad", "dtpad", arguments[0].toLocal8Bit().data(), (char*)0);
				}
				else
				{
					execl("/bin/sh", "sh", "-c", arguments[0].toLocal8Bit().data(), (char*)0);
				}
			}
			else
			{
				QThread::currentThread()->wait(500);

				wpid=waitpid(pid, NULL, WNOHANG);

				if (wpid < 0 && errno != EINTR)
				{
					nRet=ExecMethodFailed;
				}
				else
				{
					nRet=ExecMethodSucceed;
				}
			}
#endif
		}
		break;
	case MethodTerminate:
		{
			if (arguments.size() == 0)
			{
				break;
			}

#ifndef Q_OS_WIN
			pid_t pid=(pid_t)arguments[0].toInt();

			if (kill(pid, SIGKILL) == 0)
			{
				nRet=ExecMethodSucceed;
			}
#endif
			
		}
		break;
	default:
		nRet=ExecMethodInvalid;
		break;
	}


	return nRet;
}

REGISTER_STAT_INSTANCE(QStatProcess)
