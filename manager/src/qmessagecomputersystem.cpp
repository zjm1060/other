#include "qmessagecomputersystem.h"

#include <QThreadPool>

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#else
#include "qstatfactory.h"
#endif

QMessageComputerSystem::QMessageComputerSystem(QObject *parent)
	: QObject(parent)
{

}

QMessageComputerSystem::~QMessageComputerSystem()
{

}

QByteArray QMessageComputerSystem::query(int msgType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);
	Q_UNUSED(usrID);

	QByteArray buffer;
	QDataStream in(&buffer, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	in<<msgType;

	return buffer;
}

bool QMessageComputerSystem::parse(QConnection* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageComputerSystemRunnable* pRunnable=new QMessageComputerSystemRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QConnection*, int, QByteArray)), this, SLOT(finished(QConnection*, int, QByteArray)));

	QThreadPool::globalInstance()->start(pRunnable);

	return true;
}

void QMessageComputerSystem::finished(QConnection* sender, int msgType, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	QByteArray buf;
	QDataStream in(&buf, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	switch (msgType)
	{
	case QueryComputerSystem:
		in<<AnswerComputerSystem;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	default:
		break;
	}

	sender->sendMessage(buf, Type);
}

QMessageComputerSystemRunnable::QMessageComputerSystemRunnable(QConnection* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageComputerSystemRunnable::~QMessageComputerSystemRunnable()
{

}

void QMessageComputerSystemRunnable::setArg(QConnection* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageComputerSystemRunnable::run()
{
	int nMsgType;

	QDataStream out(m_buffer);
	out.setVersion(DATA_STREAM_VERSION);

	out>>nMsgType;

	switch (nMsgType)
	{
	case QMessageComputerSystem::QueryComputerSystem:
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			QVariant var;
#ifdef Q_OS_WIN
			var=QWmiFactory::instance()->exec(WMI_TYPE_COMPUTER_SYSTEM);
#else
			var=QStatFactory::instance()->exec(WMI_TYPE_COMPUTER_SYSTEM);
#endif
			
			QVector<ComputerSystem> vecSystem=var.value< QVector<ComputerSystem> >();
			if (vecSystem.isEmpty())
			{
				stream<<QMessageComputerSystem::QueryFailed;
			}
			else
			{
				stream<<QMessageComputerSystem::QuerySucceed;
				stream<<vecSystem.size();
				for (int i=0; i<vecSystem.size(); i++)
				{
					const ComputerSystem& system=vecSystem[i];
					stream<<system.name<<system.workgroup;
				}
			}

			emit finished(m_pSender, QMessageComputerSystem::QueryComputerSystem, buffer);
		}
		break;
	case QMessageComputerSystem::AnswerComputerSystem:
		{
			QVector<ComputerSystem> vecSystem;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QMessageComputerSystem::QuerySucceed)
			{
				int nNum;
				out>>nNum;

				ComputerSystem system;
				for (int i=0; i<nNum; i++)
				{
					out>>system.name>>system.workgroup;

					vecSystem.append(system);
				}
			}

			QMetaObject::invokeMethod(m_pView, "showComputerSystem", Q_ARG(int, nQueryFlag),
				Q_ARG(QVector<ComputerSystem>, vecSystem));
		}
		break;
	default:
		break;
	}
}

