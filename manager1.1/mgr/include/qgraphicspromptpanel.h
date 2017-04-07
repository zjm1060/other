#ifndef QGRAPHICSPROMPTPANEL_H
#define QGRAPHICSPROMPTPANEL_H

#include "qgraphicspanel.h"

#include <QTextDocument>

class QGraphicsPromptPanel : public QAbstractGrahicsPanelItem
{
	Q_OBJECT

public:
	QGraphicsPromptPanel(int x, int y, int w, const QBrush& brush, QObject *parent=0);
	~QGraphicsPromptPanel();

public:
	void paint(QPainter* painter);

	virtual void setGeometry(const QRect& rect);

	void setHtmlText(const QString& html);

private:
	int		m_nPointSize;

	QTextDocument*	m_pDocument;
};

#endif // QGRAPHICSPROMPTPANEL_H
