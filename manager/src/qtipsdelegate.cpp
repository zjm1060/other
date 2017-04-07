#include "qtipsdelegate.h"

#include "common.h"

QTipsDelegate::QTipsDelegate(QObject *parent)
	: QItemDelegate(parent)
{
	m_nSize=parseConfig(getAppParentDir()+INI_FILE, "tip/height", 16).toInt()+2;
	if (m_nSize <= 0)
	{
		m_nSize=18;
	}

	m_color[Information]=parseConfigColor(getAppParentDir()+INI_FILE, "tip/information", QColor(80,80,80));
	m_color[Warning]=parseConfigColor(getAppParentDir()+INI_FILE, "tip/warning", QColor(255,158,0));
	m_color[Error]=parseConfigColor(getAppParentDir()+INI_FILE, "tip/error", QColor(255,0,0));
	m_color[Download]=parseConfigColor(getAppParentDir()+INI_FILE, "tip/download", QColor(0,0,0));
	m_color[Upload]=parseConfigColor(getAppParentDir()+INI_FILE, "tip/upload", QColor(0,0,0));
	m_color[Success]=parseConfigColor(getAppParentDir()+INI_FILE, "tip/success", QColor(0,255,0));
	m_color[Unknown]=parseConfigColor(getAppParentDir()+INI_FILE, "tip/unknown", QColor(0,0,0));

	m_pixmap[Information]=QPixmap(":/images/information.png");
	m_pixmap[Warning]=QPixmap(":/images/warning.png");
	m_pixmap[Error]=QPixmap(":/images/error1.png");
	m_pixmap[Download]=QPixmap(":/images/download.png");
	m_pixmap[Upload]=QPixmap(":/images/upload.png");
	m_pixmap[Success]=QPixmap(":/images/tick.png");
	m_pixmap[Unknown]=QPixmap(":/images/unknown.png");
}

QTipsDelegate::~QTipsDelegate()
{

}

void QTipsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	int nFlag = index.data(Qt::UserRole).toInt();
	if (nFlag < 0 || nFlag >= TypeEnd)
	{
		nFlag=Unknown;
	}

	painter->save();

	if (option.state & QStyle::State_Selected)
	{
		QPen pen=painter->pen();

		painter->setPen(Qt::NoPen);
		painter->setBrush(option.palette.highlight());

		QRect rect=option.rect;
		rect.setLeft(rect.left()+m_nSize);
		painter->drawRect(rect);

		painter->setPen(pen);
	}

	painter->setBrush(Qt::NoBrush);
	int x=option.rect.left();
	if (nFlag != Unknown)
	{
		QRect rect=option.rect;
		rect.setWidth(m_nSize);
		rect.adjust(1, 1, -1, -1);
		painter->drawPixmap(rect, m_pixmap[nFlag]);

		x+=rect.width()+3;
	}

	QString strText=index.data(Qt::EditRole).toString();

	if (!strText.isEmpty())
	{
		QPen pen(painter->pen());
		pen.setColor(m_color[nFlag]);

		painter->setPen(pen);

		QRect rect=option.rect;
		rect.setLeft(x);
		painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, strText);
	}

	painter->restore();
}

QSize QTipsDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QFontMetrics metrics(option.font);

	QString strText=index.data(Qt::EditRole).toString();

	return QSize(metrics.width(strText)+m_nSize, m_nSize);
}
