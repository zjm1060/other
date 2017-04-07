#include "qtempsavepath.h"

#include "define.h"
#include "common.h"
#include "qmessagefiletransfer.h"
#include "qmessageparser.h"
#include "qfiletransferparser.h"
#include "qfilecontroller.h"
#include "qserverdelegate.h"

QTempSavePath::QTempSavePath(QAbstractItemModel* model, const QModelIndex& index, int kind,
							 const QString& subPath, QObject *parent)
	: QObject(parent),
	m_pClient(NULL)
{
	m_pModel=model;
	m_index=index;
	m_nKind=kind;

	m_strSubPath=subPath;

	m_pClientManager=new QClientManager(new QFileController(this), this);
	connect(m_pClientManager, SIGNAL(connected(QClient*)), this, SLOT(connected(QClient*)));
	connect(m_pClientManager, SIGNAL(disconnected(QClient*)), this, SLOT(disconnected(QClient*)));

	m_nUsrID=QFileTransferParser::instance()->registerInterface(this);

	if (model != NULL && index.isValid())
	{
		QServerManager::ServerInfo info=
			qvariant_cast<QServerManager::ServerInfo>(model->data(model->index(index.row(), 0), QServerDelegate::ServerInfoRole));

		QList<QString> keys=info.servers.keys();
		foreach (QString key, keys)
		{
			m_strDefault=parseConfig(getAppParentDir()+TRANSFER_SETTING_FILE, QString("%1/%2").arg(key).arg(m_nKind), "").toString();
			if (!m_strDefault.isEmpty())
			{
				break;
			}
		}
		
		setClient(info);
	}
}

QTempSavePath::~QTempSavePath()
{
	if (!m_strDefault.isEmpty() && m_pModel != NULL && m_index.isValid())
	{
		m_pModel->setData(m_pModel->index(m_index.row(), 0), m_strDefault, QServerDelegate::ServerSaveRole);

		QServerManager::ServerInfo info=
			qvariant_cast<QServerManager::ServerInfo>(m_pModel->data(m_pModel->index(m_index.row(), 0), QServerDelegate::ServerInfoRole));

		QList<QString> keys=info.servers.keys();
		foreach (QString key, keys)
		{
			codeConfig(getAppParentDir()+TRANSFER_SETTING_FILE, QString("%1/%2").arg(key).arg(m_nKind), m_strDefault);
		}
	}

	QFileTransferParser::instance()->unregisterInterface(m_nUsrID);
}

void QTempSavePath::setClient(const QServerManager::ServerInfo& info)
{
	if (m_pClient != NULL)
	{
		m_pClientManager->remove(m_pClient);
		m_pClient=NULL;
	}

	QClient* pClient=m_pClientManager->addClient(info);
	m_pClientManager->connectToHost(pClient);
}

void QTempSavePath::connected(QClient* client)
{
	m_pClient=client;
	if (m_pClient != NULL)
	{
		if (!m_strDefault.isEmpty())
		{
			queryPath(m_strDefault);
		}
		else
		{
			queryRoot();
		}
	}
}

void QTempSavePath::disconnected(QClient* client)
{
	Q_UNUSED(client);

	m_strDefault.clear();

	deleteLater();
}

void QTempSavePath::queryRoot()
{
	if (m_pClient == NULL)
	{
		return;
	}

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageFileTransfer::QueryRoot, m_nUsrID);

		m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
	}
}

void QTempSavePath::queryPath(const QString& path)
{
	if (m_pClient == NULL)
	{
		return;
	}

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageFileTransfer::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		stream.setVersion(DATA_STREAM_VERSION);

		stream<<path<<QMessageFileTransfer::SortName;

		buffer=pMsg->query(QMessageFileTransfer::QueryPath, m_nUsrID, buffer);

		m_pClient->sendMessage(buffer, QMessageFileTransfer::Type);
	}
}

void QTempSavePath::recvRoot(const QString& root)
{
	m_strRoot=root;

	queryPath(appendPath(root, m_strSubPath));
}

void QTempSavePath::recvPath(const QString& path, int flag, const QVector<FileInfo>& files)
{
	Q_UNUSED(files);

	if (flag == QMessageFileTransfer::QueryPathValid)
	{
		m_strDefault=path;

		deleteLater();
	}
	else if (flag == QMessageFileTransfer::QueryPathNoExist)
	{
		if (m_strRoot.isEmpty())
		{
			queryRoot();
		}
		else
		{
			m_strDefault=m_strRoot;
			deleteLater();
		}
	}
}
