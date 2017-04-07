#include "qmessageconfigure.h"

#include <QFileInfo>

#include "common.h"

#include "qmibfactory.h"
#include "qutildef.h"
#include "qmessageparser.h"
#include "qconnectionmanager.h"
#include "qdirex.h"
#include "qutilreadwritexmlfile.h"
#include "qcscsdbscode.h"
#include "qlogger.h"
#include "qabstractfilecompare.h"

QMessageConfigure::QMessageConfigure(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<SyncCommands>("SyncCommands");
}

QMessageConfigure::~QMessageConfigure()
{

}

QByteArray QMessageConfigure::query(int ackType, quint32 usrID, const QByteArray& contents)
{
	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<ackType<<usrID;

	if (contents.size() > 0)
	{
		in.writeRawData(contents.data(), contents.size());
	}

	return buffer;
}

bool QMessageConfigure::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageConfigureRunnable* pRunnable=new QMessageConfigureRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QObject*, int, quint32, QByteArray)), 
		this, SLOT(finished(QObject*, int, quint32, QByteArray)));

	globalThreadPool()->start(pRunnable);

	return true;
}

void QMessageConfigure::finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer)
{
	if (sender == NULL)
	{
		return;
	}

	Q_WRITE_ONLY_DATA_STREAM(in, buf)

	switch (ackType)
	{
	case QuerySyncAssociationConfigure:
		in<<AnswerSyncAssociationConfigure;
		break;
	case QueryWriteSyncAssociateionConfig:
		in<<AnswerWriteSyncAssociateionConfig;
		break;
	case QueryExecSyncConfigureCommand:
		in<<AnswerExecSyncConfigureCommand;
		break;
	default:
		break;
	}

	in<<usrID;

	if (!buffer.isEmpty())
	{
		in.writeRawData(buffer.data(), buffer.size());
	}

	QObjectPtr ptr=QConnectionManager::instance()->connection(sender);
	if (ptr != NULL)
	{
		QMetaObject::invokeMethod(sender, "sendMessage", Q_ARG(QByteArray, buf), Q_ARG(int, Type));
	}
}

QMessageConfigureRunnable::QMessageConfigureRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageConfigureRunnable::~QMessageConfigureRunnable()
{

}

void QMessageConfigureRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageConfigureRunnable::run()
{
	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	int nAckType;
	quint32 nUsrID;
	out>>nAckType>>nUsrID;

	switch (nAckType)
	{
	case QuerySyncAssociationConfigure:
		{
			SyncCommands commands;
			QReadCommandsFromXmlFile reader(appendPath(getAppParentDir(), SYNC_H9000_FILE_COMMON_CONF_FILE));
			reader.read();
			
			commands=reader.result();

			QMutableMapIterator<int, SyncProgram> it(commands.programs);
			while (it.hasNext())
			{
				it.next();

				SyncProgram& program=it.value();

				for (int i=0; i<program.files.size(); i++)
				{
					SyncMonitorFile& file=program.files[i];

					file.file=entryInfo(QFileInfo(appendPath(file.path, file.file.name)), false);
				}
			}

			QMutableMapIterator<int, SyncCommand> iter(commands.commands);
			while (iter.hasNext())
			{
				iter.next();

				SyncCommand& command=iter.value();

				for (int i=0; i<command.files.size(); i++)
				{
					SyncMonitorFile& file=command.files[i];

					file.file=entryInfo(QFileInfo(appendPath(file.path, file.file.name)), false);
				}
			}

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<commands;

			emit finished(m_pSender, nAckType, nUsrID, buffer);
		}
		break;
	case AnswerSyncAssociationConfigure:
		{
			SyncCommands commands;

			out>>commands;

			QMetaObject::invokeMethod(m_pView, "recvSyncAssociationConfigure", Q_ARG(quint32, nUsrID), Q_ARG(SyncCommands, commands));
		}
		break;
	case QueryWriteSyncAssociateionConfig:
		{
			SyncCommands commands;

			out>>commands;

			QWriteCommandsToXmlFile writer(appendPath(getAppParentDir(), SYNC_H9000_FILE_COMMON_CONF_FILE), commands);
			bool bRet=writer.write(false);

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<(bRet? QueryWriteConfigureSuccess:QueryWriteConfigureFail);

			emit finished(m_pSender, nAckType, nUsrID, buffer);
		}
		break;
	case AnswerWriteSyncAssociateionConfig:
		{
			int nFlag;
			out>>nFlag;

			QMetaObject::invokeMethod(m_pView, "recvWriteSyncAssociationConfigure", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag));
		}
		break;
	case QueryExecSyncConfigureCommand:
		{
			QString strWorkspace;
			QVector<FileInfo> files;

			out>>strWorkspace>>files;

			QReadCommandsFromXmlFile reader(appendPath(getAppParentDir(), SYNC_H9000_FILE_COMMON_CONF_FILE));
			reader.read();
			
			QMap<QString, FileInfo> fs;

			for (int i=0; i<files.size(); i++)
			{
				fs.insert(files[i].name, files[i]);
			}

			SyncCommands commands=reader.result();

			QVariant var;
			QAbstractMibPtr pProcessInstance=QMibFactory::instance()->product(UTIL_TYPE_PROCESS);
			if (pProcessInstance != NULL && pProcessInstance->result().isValid())
			{
				int nIndex=0, nCount=commands.programs.size()+commands.commands.size();
				while (nCount > 0 )
				{
					if (commands.programs.contains(nIndex))
					{
						nCount--;

						const SyncProgram& program=commands.programs[nIndex];
						if (!program.enabled)
						{
							nIndex++;
							continue;
						}

						for (int j=0; j<program.files.size(); j++)
						{
							if (fs.contains(program.files[j].file.name) && program.files[j].enabled && 
								appendPath(strWorkspace, program.files[j].file.name) == appendPath(program.files[j].path, program.files[j].file.name))
							{
								if (program.terminal)
								{
									if (pProcessInstance != NULL)
									{
										var=pProcessInstance->result();
									}

									QVector<SystemProcess> processes=var.value<QVector<SystemProcess> >();
									for (int index=0; index<processes.size(); index++)
									{
										if (appendPath(getParentDir(processes[index].executablePath), processes[index].name ) == appendPath(program.path, program.program.name))
										{
											QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_PROCESS);
											int nRet=ExecMethodInvalid;
											if (pInstance != NULL)
											{
												logplusInfo(LIBMSG_LOGGER_STR, QString("Termimal process name=%1, pid=%2, path=%3").arg(processes[index].name)
													.arg(processes[index].processId).arg(processes[index].executablePath));

												QStringList arguments;
												arguments<<QString::number(processes[index].processId);
												nRet=pInstance->commond(CommandTerminateProcess, arguments);
											}

											if (nRet != ExecMethodSucceed)
											{
												Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
												stream<<QueryCommandKillProcessFail<<processes[index].name
													<<processes[index].processId<<getParentDir(processes[index].executablePath);

												emit finished(m_pSender, nAckType, nUsrID, buffer);

												break;
											}
										}
									}
								}

								if (program.running)
								{
									QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_PROCESS);
									int nRet=ExecMethodInvalid;
									if (pInstance != NULL)
									{
										logplusInfo(LIBMSG_LOGGER_STR, QString("Start process name=%1, path=%2").arg(program.program.name)
											.arg(program.path));

										QStringList arguments;
										arguments<<appendPath(program.path, program.program.name);
										nRet=pInstance->commond(CommandCreateProcess, arguments);
									}

									if (nRet != ExecMethodSucceed)
									{
										Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
										stream<<QueryCommandCreateProcessFail<<appendPath(program.path, program.program.name);

										emit finished(m_pSender, nAckType, nUsrID, buffer);

										break;
									}

								}

								break;
							}
						}
					}
					else if (commands.commands.contains(nIndex))
					{
						nCount--;

						QCscsDbsCode* pCscsInstance=QCscsDbsCode::instance(true);
						if (pCscsInstance == NULL)
						{
							Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
							stream<<QueryCommandFoundCommandFail;

							emit finished(m_pSender, nAckType, nUsrID, buffer);
						}
						else
						{
							for (int i=0; i<commands.commands.size(); i++)
							{
								const SyncCommand& command=commands.commands[i];
								if (!command.enabled)
								{
									continue;
								}

								for (int j=0; j<command.files.size(); j++)
								{
									if (fs.contains(command.files[j].file.name) && command.files[j].enabled && 
										appendPath(strWorkspace, command.files[j].file.name)  == appendPath(command.files[j].path, command.files[j].file.name))
									{
										int nRet=pCscsInstance->command(command.command, command.params);

										Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
										stream<<QueryCommandExecCommand<<nRet<<command.command;

										emit finished(m_pSender, nAckType, nUsrID, buffer);

										break;
									}
								}
							}
						}
					}

					nIndex++;
				}
			}
			else
			{
				if (!commands.programs.isEmpty())
				{
					Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
					stream<<QueryCommandFoundProcessesFail;

					emit finished(m_pSender, nAckType, nUsrID, buffer);
				}
			}

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<QueryCommandFinish;

			emit finished(m_pSender, nAckType, nUsrID, buffer);
		}
		break;
	case AnswerExecSyncConfigureCommand:
		{
			int nFlag;
			out>>nFlag;

			switch (nFlag)
			{
			case QueryCommandKillProcessFail:
				{
					QString strName;
					quint32 nProcessId;
					QString strPath;

					out>>strName>>nProcessId>>strPath;

					QMetaObject::invokeMethod(m_pView, "recvCommandKillProcessFail", Q_ARG(quint32, nUsrID),
						Q_ARG(QString, strName), Q_ARG(quint32, nProcessId), Q_ARG(QString, strPath));
				}
				break;
			case QueryCommandCreateProcessFail:
				{
					QString strProgram;

					out>>strProgram;
					QMetaObject::invokeMethod(m_pView, "recvCommandCreateProcessFail", Q_ARG(quint32, nUsrID), Q_ARG(QString, strProgram));
				}
				break;
			case QueryCommandFoundProcessesFail:
				{
					QMetaObject::invokeMethod(m_pView, "recvCommandFoundProcessesFail", Q_ARG(quint32, nUsrID));
				}
				break;
			case QueryCommandFoundCommandFail:
				{
					QMetaObject::invokeMethod(m_pView, "recvCommandFoundCommandFail", Q_ARG(quint32, nUsrID));
				}
				break;
			case QueryCommandExecCommand:
				{
					int nQueryFlag;
					QString strCommand;

					out>>nQueryFlag>>strCommand;
					QMetaObject::invokeMethod(m_pView, "recvCommandExecCommand", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag), Q_ARG(QString, strCommand));
				}
				break;
			case QueryCommandFinish:
				{
					QMetaObject::invokeMethod(m_pView, "recvCommandFinish", Q_ARG(quint32, nUsrID));
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageConfigure) 
