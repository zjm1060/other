#include "qframefilereceiver.h"

#include <QFileDialog>
#include <QTextStream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>

#include "define.h"
#include "common.h"
#include "qutildef.h"
#include "qimessage.h"
#include "qmessageboxex.h"
#include "qreadwritexmlfile.h"
#include "qtipsdelegate.h"
#include "qglobalinfo.h"
#include "qlogger.h"
#include "qfileex.h"
#include "qdirex.h"
#include "qservermanager.h"

QFrameFileReceiver::QFrameFileReceiver(const QString& record, const QString& subPath, QWidget *parent/* =0 */)
	: QFrame(parent)
	,m_pButtonWorkspace(NULL)
	,m_pListViewReveiver(NULL)
	,m_bRefreshing(false)
	,m_strSubPath(subPath)
{
	setFrameShape(QFrame::StyledPanel);
	setFrameShadow(QFrame::Plain);

	QVBoxLayout* pLayout=new QVBoxLayout();
	setLayout(pLayout);

	QHBoxLayout* pLayoutWorkspace=new QHBoxLayout();
	pLayout->addLayout(pLayoutWorkspace);

	m_pButtonWorkspace=new QLinkButton(QIcon(":/images/computer.png"), "", this);
	pLayoutWorkspace->addWidget(m_pButtonWorkspace);

	connect(m_pButtonWorkspace, SIGNAL(clicked()), this, SLOT(onClickedBtnWorkspace()));

	m_pButtonWorkspace->installEventFilter(this);
	m_pButtonWorkspace->setVisible(false);

	pLayoutWorkspace->addStretch();

// 	m_pButtonShowAll=new QCheckBox(tr("All"), this);
// 	pLayoutWorkspace->addWidget(m_pButtonShowAll);
// 
// 	connect(m_pButtonShowAll, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
// 
// 	m_pButtonShowAll->installEventFilter(this);

	m_pButtonSelectAll=new QLinkButton(tr("Select All"), this);
	pLayoutWorkspace->addWidget(m_pButtonSelectAll);
	connect(m_pButtonSelectAll, SIGNAL(clicked()), this, SLOT(onClickedBtnSelectAll()));

	m_pButtonSelectAll->installEventFilter(this);

	m_pButtonSelectNone=new QLinkButton(tr("Select None"), this);
	pLayoutWorkspace->addWidget(m_pButtonSelectNone);
	connect(m_pButtonSelectNone, SIGNAL(clicked()), this, SLOT(onClickedBtnSelectNone()));

	m_pButtonSelectNone->installEventFilter(this);

	m_pButtonRefresh=new QPushButton(QIcon(":/images/refresh.png"), "", this);
	pLayoutWorkspace->addWidget(m_pButtonRefresh);

	connect(m_pButtonRefresh, SIGNAL(clicked()), this, SLOT(currentTextChanged()));

	m_pButtonRefresh->installEventFilter(this);

	m_pListViewReveiver=new QListViewFileReceiver(appendPath(getAppParentDir(), TRANSFER_SETTING_FILE), record, subPath, this);
	pLayout->addWidget(m_pListViewReveiver);

	connect(m_pListViewReveiver, SIGNAL(selectedChanged()), this, SIGNAL(selectedChanged()));

	m_pListViewReveiver->installEventFilter(this);

	m_pMonitor=new QMonitorConfigureFile(appendPath(getAppParentDir(), TRANSFER_SETTING_FILE), this);

	connect(m_pMonitor, SIGNAL(changed(const QMap<QString, QString>&)), this, SLOT(configureChanged(const QMap<QString, QString>&)));
	m_pMonitor->load();
}

QFrameFileReceiver::~QFrameFileReceiver()
{
	m_pButtonWorkspace->removeEventFilter(this);
//	m_pButtonShowAll->removeEventFilter(this);
	m_pButtonSelectAll->removeEventFilter(this);
	m_pButtonSelectNone->removeEventFilter(this);
	m_pButtonRefresh->removeEventFilter(this);
	m_pListViewReveiver->removeEventFilter(this);

	writeConfigure();

	SAFE_DELETE_CLASS(m_pMonitor);
}

void QFrameFileReceiver::configureChanged(const QMap<QString, QString>& configure)
{
	m_configure=configure;

	refreshWorkspace();
}

void QFrameFileReceiver::writeConfigure()
{
	if (m_pMonitor != NULL)
	{
		m_pMonitor->save(m_configure);
	}
}

void QFrameFileReceiver::setMenuState(bool refresh)
{
	m_bRefreshing=refresh;
	if (m_bRefreshing)
	{
		emit sendTip(QReceiverTransferFile::Information, tr("Loading, please wait..."));

		m_pButtonWorkspace->setEnabled(false);
		m_pButtonRefresh->setEnabled(false);
	}
	else
	{
		emit sendTip(QReceiverTransferFile::Information, tr("Loaded."));

		m_pButtonRefresh->setEnabled(true);
		m_pButtonWorkspace->setEnabled(true);
	}
}

bool QFrameFileReceiver::eventFilter(QObject* watched, QEvent* event)
{
	if (!m_bRefreshing)
	{
		if (event->type() == QEvent::Enter)
		{
			if (watched == m_pButtonWorkspace)
			{
				emit sendTip(QReceiverTransferFile::Information, tr("Click to switch the configuration file of servers."));
			}
			else if (watched == m_pButtonRefresh)
			{
				emit sendTip(QReceiverTransferFile::Information, tr("Click to reload servers."));
			}
// 			else if (watched == m_pButtonShowAll)
// 			{
// 				if (m_pButtonShowAll->isChecked())
// 				{
// 					emit sendTip(QReceiverTransferFile::Information, tr("Uncheck to only show servers in the configure file."));
// 				}
// 				else
// 				{
// 					emit sendTip(QReceiverTransferFile::Information, tr("Check to show all servers."));
// 				}
// 			}
			else if (watched == m_pButtonSelectAll)
			{
				emit sendTip(QReceiverTransferFile::Information, tr("Select all alive servers in the list."));
			}
			else if (watched == m_pButtonSelectNone)
			{
				emit sendTip(QReceiverTransferFile::Information, tr("Cancel selected servers in the list."));
			}
			else if (watched == m_pListViewReveiver)
			{
			}
		}
		else if (event->type() == QEvent::Leave)
		{
			emit sendTip(QReceiverTransferFile::Information, "");
		}
	}

	return QFrame::eventFilter(watched, event);
}

bool QFrameFileReceiver::hasSelected() const
{
	return m_pListViewReveiver->hasSelected();
}

int QFrameFileReceiver::nextSelected(int row, ServerInfo& info) const
{
	return m_pListViewReveiver->nextSelected(row, info);
}

bool QFrameFileReceiver::isAddedItem(int row) const
{
	return m_pListViewReveiver->isAddedItem(row);
}

QString QFrameFileReceiver::savePath(int row) const
{
	return m_pListViewReveiver->savePath(row);
}

void QFrameFileReceiver::refreshWorkspace()
{
	setMenuState(true);

	readWorkspace();
}

void QFrameFileReceiver::readWorkspace()
{
// 	if (m_configure.contains("ShowAll") && m_configure["ShowAll"] == "true")
// 	{
// 		m_pButtonShowAll->setChecked(true);
// 	}
// 	else
// 	{
// 		m_pButtonShowAll->setChecked(false);
// 	}

	QString strFile;
	if (m_configure.contains("ServerFile"))
	{
		strFile=m_configure["ServerFile"];
		convertToStdDir(strFile);
	}
	else
	{
		strFile=appendPath(getAppParentDir(), "def/ems_config.def");
		m_configure["ServerFile"]=strFile;
	}

	m_pButtonWorkspace->setText(strFile);

	currentTextChanged();
}

void QFrameFileReceiver::stateChanged(int state)
{
	m_configure["ShowAll"]=(state == Qt::Checked? "true":"false");

	writeConfigure();

	currentTextChanged();

	emit selectedChanged();
}

void QFrameFileReceiver::currentTextChanged()
{
	if (m_strWorkspace != m_pButtonWorkspace->text())
	{
		m_strWorkspace=m_pButtonWorkspace->text();
		m_servers=readConfigServers(m_strWorkspace);
	}

//	bool bSelectAll=(m_pButtonShowAll->checkState() == Qt::Checked? true:false);
	bool bSelectAll=true;

	QMap<QString, QVector<QString> > servers=m_servers;

	QList<QPair<ServerInfo, int> > infos=QGlobalInfo::instance()->serverList();
	QListIterator<QPair<ServerInfo, int> > it(infos);
	while (it.hasNext())
	{
		const QPair<ServerInfo, int>& info=it.next();
		if (!bSelectAll && !servers.contains(info.first.hostName))
		{
			continue;
		}

		if (servers.contains(info.first.hostName))
		{
			QVector<QString> addrs=servers[info.first.hostName];
			for (int i=0; i<addrs.size(); i++)
			{
				bool bFind=false;
				foreach(QVector<Server> ts, info.first.servers)
				{
					foreach (Server t, ts)
					{
						if (t.addr == addrs[i])
						{
							servers.remove(info.first.hostName);

							bFind=true;
							break;
						}
					}

					if (bFind)
					{
						break;
					}
				}

				if (bFind)
				{
					break;
				}
			}
		}

		m_pListViewReveiver->addReceiver(info);
	}

	QMapIterator<QString, QVector<QString> > iter(servers);
	while (iter.hasNext())
	{
		iter.next();

		Server s;
		QServerManager::findFirstServer(QGlobalInfo::instance()->local(), &s);

		QPair<ServerInfo, int> server;
		server.second=1;

		server.first.hostName=iter.key();
		server.first.count=0;
		server.first.home="/home/ems/h9000/";
		const QVector<QString>& addrs=iter.value();
		for (int i=0; i<addrs.size(); i++)
		{
			s.addr=addrs[i];

			server.first.servers[UNIVERSAL_MAC_ADDRESS].append(s);
			server.first.count++;
		}

		m_pListViewReveiver->addReceiver(server);
	}

	setMenuState(false);
}

QMap<QString, QVector<QString> > QFrameFileReceiver::readConfigServers(const QString& filename) const
{
	QMap<QString, QVector<QString> > servers;
	if (isTextFormat(filename))
	{
		QFileEx file(filename);
		if (file.exists() && file.open(QIODevice::ReadOnly|QIODevice::Text))
		{
			QTextStream stream(&file);
			bool bFind=false;
			QRegExp rx("^\\[\\S*\\]$");

			while (!stream.atEnd())
			{
				QString strText=stream.readLine().trimmed();

				if (strText.isEmpty() || strText.startsWith(QChar('!')) || strText.startsWith(QChar('#')))
				{
					continue;
				}

				if (!bFind)
				{
					if (strText == "[HOST_IPADDR]")
					{
						bFind=true;
					}

					continue;
				}
				else if (rx.indexIn(strText) != -1)
				{
					break;
				}

				QVector<QString> labels=QAbstractFileCompare::parseString(strText);
				if (labels.size() > 2)
				{
					QString strHostName=labels[0];
					if (strHostName.isEmpty())
					{
						continue;
					}

					for (int i=1; i<labels.size(); i++)
					{
						servers[strHostName].append(labels[i]);
					}
				}
			}
		}
	}

	return servers;
}

void QFrameFileReceiver::onClickedBtnWorkspace()
{
	QString strFileName=m_pButtonWorkspace->text();
	QString strFile = QFileDialog::getOpenFileName(this, tr("Switch Workspace"), getParentDir(strFileName));
	if (strFile.isEmpty() || strFileName == strFile)
	{
		return;
	}

	m_pButtonWorkspace->setText(strFile);

	m_configure["ServerFile"]=strFile;
	
	writeConfigure();

	currentTextChanged();
}

void QFrameFileReceiver::onClickedBtnSelectAll()
{
	if (m_pListViewReveiver != NULL)
	{
		m_pListViewReveiver->selectAll();
	}
}

void QFrameFileReceiver::onClickedBtnSelectNone()
{
	if (m_pListViewReveiver != NULL)
	{
		m_pListViewReveiver->selectNone();
	}
}
