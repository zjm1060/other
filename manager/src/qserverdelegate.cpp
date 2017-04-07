#include "qserverdelegate.h"

#include <QPixmap>
#include <QApplication>

#include "common.h"
#include "qservermanager.h"

QServerDelegate::QServerDelegate(QObject *parent)
	: QItemDelegate(parent)
{
	m_iconSize=QSize(48, 48);

	m_color[HostNameColor]=QColor(0, 0, 0);
	m_color[AddressColor]=QColor(121, 121, 121);
	m_color[SaveAddrColor]=QColor(0, 0, 0);

	m_iconButton=QIcon(":/images/save.png");
}

QServerDelegate::~QServerDelegate()
{

}

void QServerDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.column() == 0)
	{
		QIcon icon=qvariant_cast<QIcon>(index.data(Qt::DecorationRole));

		QServerManager::ServerInfo serverInfo= qvariant_cast<QServerManager::ServerInfo>(index.data(ServerInfoRole));

		QPixmap pixmap=icon.pixmap(m_iconSize);

		painter->save();

		if (option.state & QStyle::State_Selected)
		{
			QPen pen=painter->pen();

			painter->setPen(Qt::NoPen);
			painter->setBrush(option.palette.highlight());

			painter->drawRect(option.rect);

			painter->setPen(pen);
		}

		painter->setBrush(Qt::NoBrush);
		int x=option.rect.left();
		int y=option.rect.top()+1;

		QRect rect=option.rect;
		rect.setWidth(m_iconSize.width());
		rect.adjust(1, 1, -1, -1);
		painter->drawPixmap(rect, pixmap);

		x+=rect.width()+5;

		int x0=x;
		QFontMetrics metrics(option.font);
		if (!serverInfo.hostName.isEmpty())
		{
			QPen pen(painter->pen());
			pen.setColor(m_color[HostNameColor]);

			painter->setPen(pen);

			QRect rect=option.rect;
			rect.setLeft(x0);
			rect.setWidth(metrics.width(serverInfo.hostName));
			painter->drawText(rect, Qt::AlignLeft | Qt::AlignTop, serverInfo.hostName);

			x0+=rect.width();
		}

		if (serverInfo.count > 0)
		{
			QString strAddr="[";
			foreach (QVector<QServerManager::Server> servers, serverInfo.servers)
			{
				foreach (QServerManager::Server server, servers)
				{
					strAddr+=QString("%1:%2 ").arg(server.addr).arg(server.port);
				}
			}

			strAddr.replace(strAddr.length()-1, 1, QChar(']'));

			QPen pen(painter->pen());
			pen.setColor(m_color[AddressColor]);

			painter->setPen(pen);

			QRect rect=option.rect;
			rect.setLeft(x0);
			rect.setWidth(metrics.width(strAddr));
			painter->drawText(rect, Qt::AlignLeft | Qt::AlignTop, strAddr);

			x0+=rect.width();
		}

		y+=metrics.height();
		QString strSaveAddr=index.data(ServerSaveRole).toString();
		if (!strSaveAddr.isEmpty())
		{
			QPen pen(painter->pen());
			pen.setColor(m_color[SaveAddrColor]);

			painter->setPen(pen);

			QRect rect=option.rect;
			rect.setLeft(x);
			rect.setTop(y);
			painter->drawText(rect, Qt::AlignLeft | Qt::AlignTop, strSaveAddr);
		}

		if (index.data(ServerSelectRole).toBool())
		{
			QStyleOptionButton styleOption;
			styleOption.features=QStyleOptionButton::DefaultButton;
			styleOption.state|=QStyle::State_Enabled;
			styleOption.rect=toolIconRect(option.rect);
			styleOption.icon=m_iconButton;
			styleOption.iconSize=QSize(16, 16);

			QApplication::style()->drawControl(QStyle::CE_PushButton, &styleOption, painter);
		}

		painter->restore();
	}
	else
	{
		QItemDelegate::paint(painter, option, index);
	}
}

QSize QServerDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Q_UNUSED(index);

	return QSize(option.rect.width(), m_iconSize.height()+2);
}

QRect QServerDelegate::toolIconRect(const QRect& rect, int index) const
{
	Q_UNUSED(index);
	return QRect(rect.left()+m_iconSize.width()+3, rect.bottom()-16, 16, 16);
}
