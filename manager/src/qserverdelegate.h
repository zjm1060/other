#ifndef QSERVERDELEGATE_H
#define QSERVERDELEGATE_H

#include <QItemDelegate>
#include <QColor>
#include <QPainter>

class QServerDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	QServerDelegate(QObject* parent=0);
	~QServerDelegate();

public:
	enum userRole
	{
		ServerInfoRole=Qt::UserRole,
		ServerOnlineRole,
		ServerSelectRole,
		PixmapRole,
		ServerSaveRole,
		RoleEnd
	};

	enum colorType
	{
		HostNameColor,
		AddressColor,
		SaveAddrColor,
		ColorEnd
	};

public:
	QRect toolIconRect(const QRect& rect, int index=0) const;

public:
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
	QSize	m_iconSize;

	QColor	m_color[ColorEnd];

	QIcon	m_iconButton;
};

#endif // QSERVERDELEGATE_H
