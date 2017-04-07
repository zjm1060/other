#include "qmessagecontrolled.h"

#include "qconnectionmanager.h"

QMessageControlled::QMessageControlled(QObject *parent)
	: QObject(parent)
{

}

QMessageControlled::~QMessageControlled()
{

}

QByteArray QMessageControlled::query(int msgType, quint32 usrID, const QByteArray& contents)
{
	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<msgType<<usrID;
	if (contents.size() > 0)
	{
		in.writeRawData(contents.data(), contents.size());
	}

	return buffer;
}

bool QMessageControlled::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageControlledRunnable* pRunnable=new QMessageControlledRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	globalThreadPool()->start(pRunnable, 3);

	return true;
}

QMessageControlledRunnable::QMessageControlledRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageControlledRunnable::~QMessageControlledRunnable()
{

}

void QMessageControlledRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageControlledRunnable::run()
{
	int nAckType;
	quint32 nUsrID;

	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	out>>nAckType>>nUsrID;

	switch (nAckType)
	{
	case QueryConnectToTarget:
		{
			ServerInfo info;
			out>>info;

			QMetaObject::invokeMethod(m_pView, "queryConnectToTarget", Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID), Q_ARG(ServerInfo, info));
		}
		break;
	case AnswerConnectToTarget:
		{
			int nQueryFlag;
			out>>nQueryFlag;

			Q_ASSERT(nQueryFlag==QueryConnectToTargetSuccess || nQueryFlag==TargetDisconnected|| nQueryFlag==QueryConnectToTargetFailed);

			QMetaObject::invokeMethod(m_pView, "recvConnectToTarget", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag));
		}
		break;
	case QueryUploadDirToTarget:
	case QueryUploadSlientDirToTarget:
		{
			int nFileType;
			QString strSourcePath;
			QString strTargetPath;

			out>>nFileType>>strSourcePath>>strTargetPath;

			QMetaObject::invokeMethod(m_pView, "queryUploadDirToTarget", Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID),
				Q_ARG(int, nFileType), Q_ARG(QString, strSourcePath), Q_ARG(QString, strTargetPath), Q_ARG(bool, nAckType==QueryUploadSlientDirToTarget));
		}
		break;
	case AnswerUploadDirToTarget:
	case AnswerUploadSlientDirToTarget:
		{
			int nQueryFlag;
			QString strSourcePath;
			QString strTargetPath;
			QVector<FileInfo> files;

			out>>nQueryFlag;

			Q_ASSERT(nQueryFlag==QueryPathNoExist || nQueryFlag==QueryMakePathFail || nQueryFlag==QueryMakePathSuccess 
				|| nQueryFlag==QueryCreateSymLinkFail || nQueryFlag==QueryCreateSymLinkSuccess);

			if (nQueryFlag == QueryMakePathSuccess)
			{
				int nSize;
				out>>nSize;
				if (nSize > 0)
				{
					QByteArray buf;
					buf.resize(nSize);
					out.readRawData(buf.data(), nSize);

					Q_READ_ONLY_DATA_STREAM(stream, buf)

					stream>>strSourcePath>>strTargetPath>>files;
				}
			}

			QMetaObject::invokeMethod(m_pView, "recvUploadDirToTarget", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag),
				Q_ARG(QString, strSourcePath), Q_ARG(QString, strTargetPath), Q_ARG(QVector<FileInfo>, files));
		}
		break;
	case QueryUploadSymlinkToTarget:
	case QueryUploadSlientSymlinkToTarget:
		{
			int nFileType;
			QString strSourcePath;
			QString strTargetPath;

			out>>nFileType>>strSourcePath>>strTargetPath;

			QMetaObject::invokeMethod(m_pView, "queryUploadSymlinkToTarget", Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID),
				Q_ARG(int, nFileType), Q_ARG(QString, strSourcePath), Q_ARG(QString, strTargetPath), Q_ARG(bool, nAckType==QueryUploadSlientSymlinkToTarget));
		}
		break;
	case AnswerUploadSymlinkToTarget:
	case AnswerUploadSlientSymlinkToTarget:
		{
			int nQueryFlag;
			out>>nQueryFlag;

			Q_ASSERT(nQueryFlag==QueryFileNoExist || nQueryFlag==QuerySymLinkNoLink || nQueryFlag==QueryMakePathSuccess 
				|| nQueryFlag==QueryCreateSymLinkFail || nQueryFlag==QueryCreateSymLinkSuccess || nQueryFlag==QueryCreateSymLinkFail
				|| nQueryFlag==QueryCreateSymLinkSuccess);

			QMetaObject::invokeMethod(m_pView, "recvUploadSymlinkToTarget", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag));
		}
		break;
	case QueryUploadFileToTarget:
	case QueryUploadSlientFileToTarget:
		{
			int nFileType;
			int nFormat;
			QString strSourcePath;
			QString strTargetPath;

			out>>nFileType>>strSourcePath>>strTargetPath>>nFormat;

			QMetaObject::invokeMethod(m_pView, "queryUploadFileToTarget", Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID), Q_ARG(int, nFileType),
				Q_ARG(QString, strSourcePath), Q_ARG(QString, strTargetPath), Q_ARG(int, nFormat), Q_ARG(bool, nAckType==QueryUploadSlientFileToTarget));
		}
		break;
	case AnswerUploadFileToTarget:
	case AnswerUploadSlientFileToTarget:
		{
			int nQueryFlag;
			out>>nQueryFlag;

			quint64 nSize=0;
			
			if (nQueryFlag == QuerySourceTransferFile)
			{
				int nLen;
				out>>nLen;
				if (nLen > 0)
				{
					QByteArray buf;
					buf.resize(nLen);
					out.readRawData(buf.data(), nLen);

					Q_READ_ONLY_DATA_STREAM(stream, buf)

					stream>>nSize;
				}
			}

			QMetaObject::invokeMethod(m_pView, "recvUploadFileToTarget", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag), Q_ARG(quint64, nSize));
		}
		break;
	case QueryInterruptTransferToTarget:
		{
			QMetaObject::invokeMethod(m_pView, "queryInterruptTransferToTarget", Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID));
		}
		break;
	case AnswerInterruptTransferToTarget:
		{
			int nFlag;
			out>>nFlag;

			Q_ASSERT(nFlag==QueryFileRemoved || nFlag==QueryFileRemoveFail || nFlag==QueryFileRemoveSuccess);

			QMetaObject::invokeMethod(m_pView, "recvInterruptTransferToTarget", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag));
		}
		break;
	case QueryExecSyncCommandToTarget:
		{
			QString strWorkspace;
			QVector<FileInfo> files;

			out>>strWorkspace>>files;

			QMetaObject::invokeMethod(m_pView, "queryExecSyncCommandToTarget", Q_ARG(QObject*, m_pSender), Q_ARG(quint32, nUsrID),
				Q_ARG(QString, strWorkspace), Q_ARG(QVector<FileInfo>, files));
		}
		break;
	case AnswerExecSyncCommandToTarget:
		{
			int nFlag;
			out>>nFlag;

			QByteArray buf;
			if (nFlag == QueryCommandKillProcessFail || nFlag == QueryCommandCreateProcessFail || nFlag == QueryCommandExecCommand)
			{
				int nLen;
				out>>nLen;
				if (nLen > 0)
				{
					buf.resize(nLen);
					out.readRawData(buf.data(), nLen);
				}
			}
			Q_READ_ONLY_DATA_STREAM(stream, buf);

			switch (nFlag)
			{
			case QueryCommandKillProcessFail:
				{
					QString strName;
					quint32 nProcessId;
					QString strPath;

					stream>>strName>>nProcessId>>strPath;

					QMetaObject::invokeMethod(m_pView, "recvCommandKillProcessFail", Q_ARG(quint32, nUsrID),
						Q_ARG(QString, strName), Q_ARG(quint32, nProcessId), Q_ARG(QString, strPath));
				}
				break;
			case QueryCommandCreateProcessFail:
				{
					QString strProgram;

					stream>>strProgram;
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

					stream>>nQueryFlag>>strCommand;
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

			QMetaObject::invokeMethod(m_pView, "recvUploadExecSyncCommand", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag));
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageControlled)
