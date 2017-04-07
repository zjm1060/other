#include "qmessagecscs.h"

#include "qutildef.h"
#include "qconnectionmanager.h"
#include "qcscsdbscode.h"

QMessageCscs::QMessageCscs(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<CscsDatabase>("CscsDatabase");
}

QMessageCscs::~QMessageCscs()
{

}

QByteArray QMessageCscs::query(int msgType, quint32 usrID, const QByteArray& contents)
{
	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<msgType<<usrID;

	if (!contents.isEmpty())
	{
		in.writeRawData(contents.data(), contents.size());
	}

	return buffer;
}

bool QMessageCscs::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageCscsRunnable* pRunnable=new QMessageCscsRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QObject*, int, quint32, QByteArray)), this, SLOT(finished(QObject*, int, quint32, QByteArray)));

	globalThreadPool()->start(pRunnable);

	return true;
}

void QMessageCscs::finished(QObject* sender, int msgType, quint32 usrID, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	Q_WRITE_ONLY_DATA_STREAM(in, buf)

	switch (msgType)
	{
	case QueryCscsStruct:
		in<<AnswerCscsStruct;
		break;
	case QueryCscsStationEigenvalue:
		in<<AnswerCscsStationEigenvalue;
		break;
	case QueryCscsGroupEigenvalue:
		in<<AnswerCscsGroupEigenvalue;
		break;
	case QueryCscsDataTypeEigenvalue:
		in<<AnswerCscsDataTypeEigenvalue;
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

QMessageCscsRunnable::QMessageCscsRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageCscsRunnable::~QMessageCscsRunnable()
{

}

void QMessageCscsRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageCscsRunnable::run()
{
	int nMsgType;
	quint32 nUsrID;

	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	out>>nMsgType>>nUsrID;

	switch (nMsgType)
	{
	case QueryCscsStruct:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

			QCscsDbsCode* ptr=QCscsDbsCode::instance();
			if (ptr != NULL)
			{
				CscsDatabase db=ptr->database();

				stream<<CscsDataSourceFound<<db;
			}
			else
			{
				stream<<CscsDataSourceLost;
			}

			emit finished(m_pSender, nMsgType, nUsrID, buffer);
		}
		break;
	case AnswerCscsStruct:
		{
			CscsDatabase db;
			int nFlag;

			out>>nFlag;

			if (nFlag == CscsDataSourceFound)
			{
				out>>db;
			}

			QMetaObject::invokeMethod(m_pView, "recvCscsStruct", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag), Q_ARG(CscsDatabase, db));
		}
		break;
	case QueryCscsStationEigenvalue:
		{
			int nStn;
			out>>nStn;

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<nStn;

			QCscsDbsCode* ptr=QCscsDbsCode::instance();
			if (ptr != NULL)
			{
				QByteArray buf;
				int nFlag=ptr->code(buf, CscsStationEntry, nStn);
				stream<<nFlag;

				if (nFlag == CscsDataSourceFound)
				{
					stream<<buf.size();
					if (!buf.isEmpty())
					{
						stream.writeRawData(buf.data(), buf.size());
					}
				}
			}
			else
			{
				stream<<CscsDataSourceLost;
			}

			emit finished(m_pSender, nMsgType, nUsrID, buffer);
		}
		break;
	case AnswerCscsStationEigenvalue:
		{
			int nStn, nFlag;
			out>>nStn>>nFlag;

			QByteArray buffer;

			if (nFlag == CscsDataSourceFound)
			{
				int nSize;
				out>>nSize;
				buffer.resize(nSize);
				out.readRawData(buffer.data(), nSize);
			}

			QMetaObject::invokeMethod(m_pView, "recvCscsStationEigenvalue", Q_ARG(quint32, nUsrID), Q_ARG(int, nStn), Q_ARG(int, nFlag), 
				Q_ARG(QByteArray, buffer));
		}
		break;
	case QueryCscsGroupEigenvalue:
		{
			int nStn, nGrp;
			out>>nStn>>nGrp;

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<nStn<<nGrp;

			QCscsDbsCode* ptr=QCscsDbsCode::instance();
			if (ptr != NULL)
			{
				QByteArray buf;
				int nFlag=ptr->code(buf, CscsGroupEntry, nStn, nGrp);
				stream<<nFlag;

				if (nFlag == CscsDataSourceFound)
				{
					stream<<buf.size();
					if (!buf.isEmpty())
					{
						stream.writeRawData(buf.data(), buf.size());
					}
				}
			}
			else
			{
				stream<<CscsDataSourceLost;
			}

			emit finished(m_pSender, nMsgType, nUsrID, buffer);
		}
		break;
	case AnswerCscsGroupEigenvalue:
		{
			int nStn, nGrp, nFlag;
			out>>nStn>>nGrp>>nFlag;

			QByteArray buffer;

			if (nFlag == CscsDataSourceFound)
			{
				int nSize;
				out>>nSize;
				buffer.resize(nSize);
				out.readRawData(buffer.data(), nSize);
			}

			QMetaObject::invokeMethod(m_pView, "recvCscsGroupEigenvalue", Q_ARG(quint32, nUsrID), Q_ARG(int, nStn), Q_ARG(int, nGrp),
				Q_ARG(int, nFlag), Q_ARG(QByteArray, buffer));
		}
		break;
	case QueryCscsDataTypeEigenvalue:
		{
			int nStn, nGrp, nDataType;
			out>>nStn>>nGrp>>nDataType;

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<nStn<<nGrp<<nDataType;

			QCscsDbsCode* ptr=QCscsDbsCode::instance();
			if (ptr != NULL)
			{
				QByteArray buf;
				int nFlag=ptr->code(buf, CscsRecordEntry, nStn, nGrp, nDataType);
				stream<<nFlag;

				if (nFlag == CscsDataSourceFound)
				{
					stream<<buf.size();
					if (!buf.isEmpty())
					{
						stream.writeRawData(buf.data(), buf.size());
					}
				}
			}
			else
			{
				stream<<CscsDataSourceLost;
			}

			emit finished(m_pSender, nMsgType, nUsrID, buffer);
		}
		break;
	case AnswerCscsDataTypeEigenvalue:
		{
			int nStn, nGrp, nDateType, nFlag;
			out>>nStn>>nGrp>>nDateType>>nFlag;

			QByteArray buffer;

			if (nFlag == CscsDataSourceFound)
			{
				int nSize;
				out>>nSize;
				buffer.resize(nSize);
				out.readRawData(buffer.data(), nSize);
			}

			QMetaObject::invokeMethod(m_pView, "recvCscsRecordEigenvalue", Q_ARG(quint32, nUsrID), Q_ARG(int, nStn), Q_ARG(int, nGrp),
				Q_ARG(int, nDateType), Q_ARG(int, nFlag), Q_ARG(QByteArray, buffer));
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageCscs) 
