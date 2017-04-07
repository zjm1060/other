#include "qmessagestatuschanged.h"

#include "qmibfactory.h"
#include "qutildef.h"

QMessageStatusChanged::QMessageStatusChanged(QObject *parent)
	: QObject(parent)
{
}

QMessageStatusChanged::~QMessageStatusChanged()
{

}

QByteArray QMessageStatusChanged::query(int msgType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);

	Q_WRITE_ONLY_DATA_STREAM(in, buffer);

	in<<msgType<<usrID;
	if (contents.size() > 0)
	{
		in.writeRawData(contents.data(), contents.size());
	}

	return buffer;
}

bool QMessageStatusChanged::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageStatusChangedRunnable* pRunnable=new QMessageStatusChangedRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	globalThreadPool()->start(pRunnable);

	return true;
}

QMessageStatusChangedRunnable::QMessageStatusChangedRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageStatusChangedRunnable::~QMessageStatusChangedRunnable()
{

}

void QMessageStatusChangedRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageStatusChangedRunnable::run()
{
	int nMsgType;
	quint32 nUsrID;

	Q_READ_ONLY_DATA_STREAM(out, m_buffer);

	out>>nMsgType>>nUsrID;

	switch (nMsgType)
	{
	case InformStatusChanged:
		{
			quint64 nFlag;
			out>>nFlag;

			QMetaObject::invokeMethod(m_pView, "recvInformStatusChanged", Q_ARG(quint32, nUsrID), Q_ARG(quint64, nFlag));
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageStatusChanged) 
