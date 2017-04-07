#include "qinfomanager.h"

#include "common.h"
#include "qlogger.h"

#define INFO_MANAGER_TICK_INTERVAL	0.05f
QInfoManager::QInfoManager(QObject *parent)
	: QObject(parent)
{
	m_color[Information]=parseConfigColor(getAppParentDir()+INI_FILE, "tip/information", QColor(80,80,80));
	m_color[Warning]=parseConfigColor(getAppParentDir()+INI_FILE, "tip/warning", QColor(255,158,0));
	m_color[Error]=parseConfigColor(getAppParentDir()+INI_FILE, "tip/error", QColor(255,0,0));
	m_color[Important]=parseConfigColor(getAppParentDir()+INI_FILE, "tip/important", QColor(0,88,97));

	m_nPointSize=parseConfig(getAppParentDir()+INI_FILE, "main/pointsize", 12).toInt();

	m_timer.setInterval(500);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

QInfoManager::~QInfoManager()
{

}

QInfoManagerPtr QInfoManager::instance()
{
	static QInfoManagerPtr ptr(new QInfoManager);

	return ptr;
}

void QInfoManager::timeout()
{
	m_mutex.lock();

	QMutableListIterator<Info> it(m_listInfos);
	while (it.hasNext())
	{
		Info& info=it.next();
		info.tick-=INFO_MANAGER_TICK_INTERVAL;
		if (info.tick < 0)
		{
			it.remove();
		}
	}

	if (m_listInfos.isEmpty())
	{
		m_timer.stop();
	}

	m_mutex.unlock();

	emit refresh();
}

void QInfoManager::append(QInfoManager::InfoType type, const QString& text)
{
	Info info;
	info.type=type;
	info.text=text;
	info.tick=2.0f;

	switch (type)
	{
	case Information:
	case Important:
		logplusInfo(MANAGER_LOGGER_STR, text);
		break;
	case Warning:
		logplusWarning(MANAGER_LOGGER_STR, text);
		break;
	case Error:
		logplusError(MANAGER_LOGGER_STR, text);
		break;
	default:
		break;
	}

	m_mutex.lock();

	bool bEmpty=m_listInfos.isEmpty();
	if (!bEmpty)
	{
		info.tick=m_listInfos.last().tick+INFO_MANAGER_TICK_INTERVAL*2;
		if (info.tick < 2.0f)
		{
			info.tick=2.0f;
		}
	}
	m_listInfos.append(info);

	m_mutex.unlock();

	emit appendTip(type, text);

	if (bEmpty)
	{
		m_timer.start();
	}
}

void QInfoManager::paint(QPainter* painter, int left, int bottom)
{
	QList<Info> infos;

	m_mutex.lock();
	infos=m_listInfos;
	m_mutex.unlock();

	painter->save();

	QFont font=painter->font();
	font.setPointSize(m_nPointSize);
	painter->setFont(font);

	QString strText;

	QListIterator<Info> it(infos);
	it.toBack();
	while (it.hasPrevious())
	{
		const Info& info=it.previous();

		switch (info.type)
		{
		case Information:
			strText=tr("[I]");
			break;
		case Warning:
			strText=tr("[W]");
			break;
		case Error:
			strText=tr("[E]");
			break;
		case Important:
			strText=tr("[M]");
			break;
		default:
			break;
		}

		strText+=" "+info.text;
		painter->setPen(m_color[info.type]);
		painter->setOpacity(info.tick);
		painter->drawText(left, bottom, strText);

		bottom-=m_nPointSize+4;
	}

	painter->restore();
}
