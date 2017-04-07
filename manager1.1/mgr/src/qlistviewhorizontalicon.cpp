#include "qlistviewhorizontalicon.h"

#include <QScrollBar>

QListViewHorizontalIcon::QListViewHorizontalIcon(QWidget *parent)
	: QListView(parent)
{
	setViewMode(QListView::IconMode);
	setLayoutMode(QListView::Batched);
//	setFlow(QListView::LeftToRight);

	setDragEnabled(false);
	setMovement(QListView::Static);

	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setResizeMode(QListView::Adjust);

	setWrapping(false);
	setWordWrap(false);

	setSpacing(10);
}

QListViewHorizontalIcon::~QListViewHorizontalIcon()
{

}

void QListViewHorizontalIcon::updateListView()
{
	QSize size=viewportSizeHint();
	viewport()->resize(size);

	size=adjustSizeHint(size);
	setFixedHeight(size.height());
}

QSize QListViewHorizontalIcon::viewportSizeHint() const
{
	int nWidth=0;
	int nHeight=0;

	QAbstractItemModel* pModel=model();
	if (pModel != NULL)
	{
		QFontMetrics metrics=fontMetrics();

		int nRowCount=pModel->rowCount();
		if (nRowCount > 0)
		{
			for (int nRow=0; nRow<nRowCount; nRow++)
			{
				const QModelIndex&index=pModel->index(nRow, 0);
				QString strText=pModel->data(index, Qt::EditRole).toString();

				QSize s=metrics.size(Qt::TextWordWrap, strText);
				nHeight=nHeight>s.height()? nHeight:s.height();
				nWidth+=(s.width()>LIST_VIEW_ICON_SIZE? s.width():LIST_VIEW_ICON_SIZE)+spacing()*2;
			}
		}
		else
		{
			nHeight=metrics.height();
		}
	}

	nWidth+=15;

	nHeight+=LIST_VIEW_ICON_SIZE+spacing()*2;

	return QSize(nWidth, nHeight);
}

QSize QListViewHorizontalIcon::sizeHint () const
{
	QSize size=viewportSizeHint();

	return adjustSizeHint(size);
}

QSize QListViewHorizontalIcon::adjustSizeHint(const QSize& size) const
{
	int nWidth=size.width();
	int nHeight=size.height();

	if (nWidth > frameGeometry().width())
	{
		QScrollBar* pBar=horizontalScrollBar();
		if (pBar != NULL)
		{
			nHeight+=pBar->frameGeometry().height()+2;
		}
	}

	return QSize(nWidth, nHeight);
}

void QListViewHorizontalIcon::showEvent(QShowEvent* event)
{
	QListView::showEvent(event);

	updateListView();
}
