#include "qmessagesystem.h"

#include <QThreadPool>

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#else
#include "qstatfactory.h"
#endif

QMessageSystem::QMessageSystem(QObject *parent)
	: QObject(parent)
{

}

QMessageSystem::~QMessageSystem()
{

}

QByteArray QMessageSystem::query(int msgType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);
	Q_UNUSED(usrID);

	QByteArray buffer;
	QDataStream in(&buffer, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	in<<msgType;

	return buffer;
}

bool QMessageSystem::parse(QConnection* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageSystemRunnable* pRunnable=new QMessageSystemRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QConnection*, int, QByteArray)), this, SLOT(finished(QConnection*, int, QByteArray)));

	QThreadPool::globalInstance()->start(pRunnable);

	return true;
}

void QMessageSystem::finished(QConnection* sender, int msgType, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	QByteArray buf;
	QDataStream in(&buf, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	switch (msgType)
	{
	case QueryOperatingSystem:
		in<<AnswerOperatingSystem;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	default:
		break;
	}

	sender->sendMessage(buf, Type);
}

QMessageSystemRunnable::QMessageSystemRunnable(QConnection* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageSystemRunnable::~QMessageSystemRunnable()
{

}

void QMessageSystemRunnable::setArg(QConnection* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageSystemRunnable::run()
{
	int nMsgType;

	QDataStream out(m_buffer);
	out.setVersion(DATA_STREAM_VERSION);

	out>>nMsgType;

	switch (nMsgType)
	{
	case QMessageSystem::QueryOperatingSystem:
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			QVariant var;
#ifdef Q_OS_WIN
			var=QWmiFactory::instance()->exec(WMI_TYPE_SYSTEM);
#else
			var=QStatFactory::instance()->exec(WMI_TYPE_SYSTEM);
#endif
			
			QVector<OperatingSystem> vecSystem=var.value< QVector<OperatingSystem> >();
			if (vecSystem.isEmpty())
			{
				stream<<QMessageSystem::QueryFailed;
			}
			else
			{
				stream<<QMessageSystem::QuerySucceed;
				stream<<vecSystem.size();
				for (int i=0; i<vecSystem.size(); i++)
				{
					const OperatingSystem& system=vecSystem.at(i);
					stream<<system.caption<<system.version<<system.buildNumber<<system.CSDVersion;
				}
			}

			emit finished(m_pSender, QMessageSystem::QueryOperatingSystem, buffer);
		}
		break;
	case QMessageSystem::AnswerOperatingSystem:
		{
			QVector<OperatingSystem> vecSystem;
			OperatingSystem system;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QMessageSystem::QuerySucceed)
			{
				int nNum;
				out>>nNum;
				for (int i=0; i<nNum; i++)
				{
					out>>system.caption>>system.version>>system.buildNumber>>system.CSDVersion;

					vecSystem.append(system);
				}
			}

			QMetaObject::invokeMethod(m_pView, "showOperatingSystem", Q_ARG(int, nQueryFlag),
				Q_ARG(QVector<OperatingSystem>, vecSystem));
		}
		break;
	default:
		break;
	}
}

