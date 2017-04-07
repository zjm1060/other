#ifndef QDELEGATESYNCCOMMIT_H
#define QDELEGATESYNCCOMMIT_H

#include <QStyledItemDelegate>
#include <QPainter>

class QDelegateSyncCommit : public QStyledItemDelegate
{
	Q_OBJECT

public:
	QDelegateSyncCommit(QObject* parent=0);
	~QDelegateSyncCommit();

public:
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

public:
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
	void drawItem(QPainter* painter, const QStyleOptionViewItem& option, const QAbstractItemModel* model, const QModelIndex& index) const;
};

class QDelegateSortSyncCommit : public QDelegateSyncCommit
{
	Q_OBJECT

public:
	QDelegateSortSyncCommit(QObject* parent=0);
	~QDelegateSortSyncCommit();

public:
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};


#endif // QDELEGATESYNCCOMMIT_H
