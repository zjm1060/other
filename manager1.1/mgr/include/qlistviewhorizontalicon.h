#ifndef QLISTVIEWHORIZONTALICON_H
#define QLISTVIEWHORIZONTALICON_H

#include <QListView>

#define LIST_VIEW_ICON_SIZE	48
#define LIST_VIEW_MARK_ICON_SIZE	16
#define LIST_VIEW_LARGE_MARK_ICON_SIZE	24

class QListViewHorizontalIcon : public QListView
{
	Q_OBJECT

public:
	QListViewHorizontalIcon(QWidget *parent = 0);
	~QListViewHorizontalIcon();

public:
	void updateListView();

public:
	virtual QSize sizeHint () const;

protected:
	virtual void showEvent(QShowEvent* event);

private:
	QSize viewportSizeHint() const;
	QSize adjustSizeHint(const QSize& size) const;
};

#endif // QLISTVIEWHORIZONTALICON_H
