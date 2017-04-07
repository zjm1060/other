#ifndef QLISTVIEWPROMPT_H
#define QLISTVIEWPROMPT_H

#include <QListView>
#include <QStandardItemModel>

#include "qtipsdelegate.h"

class QListViewPrompt : public QListView
{
	Q_OBJECT

public:
	QListViewPrompt(QWidget *parent);
	~QListViewPrompt();

public:
	void clear();

	void appendTip(int flag, const QString& tip);

private:
	QStandardItemModel*		m_pModelTips;
	QTipsDelegate*			m_pDelegate;
};

#endif // QLISTVIEWPROMPT_H
