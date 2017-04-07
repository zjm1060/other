#ifndef _COMPLETELINEEDIT_H
#define _COMPLETELINEEDIT_H

#include <QtGui/QLineEdit>
#include <QStringList>
#include <QListWidgetItem>
#include <QApplication>
#include <QKeyEvent>
#include <QtGui/QListView>
#include <QtGui/QStringListModel>
#include <QDebug>

class CompleteLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	CompleteLineEdit(QStringList stringList, QWidget *parent);
	~CompleteLineEdit();

	QStringList& stringList();

signals:
	void itemClicked(QListWidgetItem *item);
	protected slots:
		void textEditedSlot(const QString& text);
		void clickedSlot(QModelIndex modelIndex);
protected:
	virtual bool  eventFilter(QObject *obj, QEvent *event);

private:
	QListView    *m_pFindWnd;
	QStringListModel *m_stringListmodel;
	void         showFindWnd(const QString& text);
	QStringList  m_stringList;
	void         hideFineWnd();
	bool         m_bEditFocus;
};



#endif // COMPLETELINEEDIT_H

