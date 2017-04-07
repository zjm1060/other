#include "qgraphicspromptpanel.h"

#include "common.h"

QGraphicsPromptPanel::QGraphicsPromptPanel(int x, int y, int w, const QBrush& brush, QObject *parent)
	: QAbstractGrahicsPanelItem(QRect(x, y, w, 20), brush, parent)
{
	QPen pen=QPen(QColor(246, 187, 32));
	pen.setWidth(2);
	setBorderPen(pen);

	m_nPointSize=parseConfig(getAppParentDir()+INI_FILE, "main/pointsize", 12).toInt();

	m_pDocument=new QTextDocument(this);
	m_pDocument->setTextWidth(w);

	QFont font=m_pDocument->defaultFont();
	font.setPointSize(m_nPointSize);
	m_pDocument->setDefaultFont(font);
}

QGraphicsPromptPanel::~QGraphicsPromptPanel()
{

}

void QGraphicsPromptPanel::setGeometry(const QRect& rect)
{
	QAbstractGrahicsPanelItem::setGeometry(rect);

	if (m_pDocument != NULL)
	{
		m_pDocument->setTextWidth(rect.width());
	}
}

void QGraphicsPromptPanel::paint(QPainter* painter)
{
	if (m_pDocument == NULL || m_pDocument->isEmpty())
	{
		return;
	}

	QRect rect=geometry();

	rect.setHeight(m_pDocument->size().height());
	setGeometry(rect);

	QAbstractGrahicsPanelItem::paint(painter);

	QPixmap pixmap(rect.width(), rect.height());
	pixmap.fill(QColor(0, 0, 0, 0));

	QPainter p(&pixmap);

	m_pDocument->drawContents(&p);

	painter->drawPixmap(rect.topLeft(), pixmap);
}

void QGraphicsPromptPanel::setHtmlText(const QString& html)
{
	if (m_pDocument != NULL)
	{
		m_pDocument->setHtml(html);
	}
}
