#include "qmulticastmsgelect.h"

#include "qmibfactory.h"
#include "qutildef.h"
#include "qconnectionmanager.h"

QMulticastMsgElect::QMulticastMsgElect(QObject *parent)
	: QObject(parent)
{
	
}

QMulticastMsgElect::~QMulticastMsgElect()
{

}

QByteArray QMulticastMsgElect::query(int msgType, const QByteArray& contents/* = */)
{
	Q_UNUSED(contents);

	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<msgType;

	if (!contents.isEmpty())
	{
		in.writeRawData(contents.data(), contents.size());
	}

	return buffer;
}

bool QMulticastMsgElect::parse(const QString& from, const QByteArray& buffer, QObject* view/* =0 */)
{
	if (buffer.size() == 0)
	{
		return false;
	}

	int nMsgType;

	Q_READ_ONLY_DATA_STREAM(out, buffer)

	out>>nMsgType;

	switch (nMsgType)
	{
	case QueryElectLeader:
		{
			QMetaObject::invokeMethod(view, "queryElectLeader", Q_ARG(QString, from));
		}
		break;
	case AnswerElectLeader:
		{
			int lapse;
			ServerInfo info;

			out>>lapse>>info;
			QMetaObject::invokeMethod(view, "answerElectLeader", Q_ARG(QString, from), Q_ARG(int, lapse), Q_ARG(ServerInfo, info));
		}
		break;
	case QueryVote:
		{
			double dPerformance;
			out>>dPerformance;

			QMetaObject::invokeMethod(view, "queryVote", Q_ARG(QString, from), Q_ARG(double, dPerformance));
		}
		break;
	case AnswerVote:
		{
			QMetaObject::invokeMethod(view, "answerVote", Q_ARG(QString, from));
		}
		break;
	default:
		break;
	}

	return true;
}

REGISTER_MULTICAST_MESSAGE_INSTANCE(QMulticastMsgElect) 
