#include "qlistgroupwidget.h"

#include <math.h>
#include <QPainter>
#include <QResizeEvent>
#include <QStandardItemModel>

#include "common.h"
#include "define.h"

#define TITLE_BORDER	30
#define TITLE_SPACING	10
#define GROUP_SPACING	30
#define BORDER_SPACING	20
#define GROUP_LINE_WIDTH_MIN	400

QListGroupWidget::QListGroupWidget(QWidget *parent)
	: QFrame(parent)
{
	m_font.setPointSize(12);
	m_font.setBold(true);
}

QListGroupWidget::~QListGroupWidget()
{
}

void QListGroupWidget::addGroup(const QString& title)
{
	if (title.isEmpty() || m_mapListView.contains(title))
	{
		return;
	}

	QListView* listView=new QListView(this);
	listView->setViewMode(QListView::IconMode);
	listView->setFrameShape(QFrame::NoFrame);
	listView->setResizeMode(QListView::Adjust);
	listView->setMovement(QListView::Static);

	QStringList labels;
	labels<<tr("title");
	QStandardItemModel* pModel=new QStandardItemModel(0, labels.size(), this);
	pModel->setHorizontalHeaderLabels(labels);

	listView->setModel(pModel);

	int nGridSize=parseConfig(getAppParentDir()+INI_FILE, "main/gridSize", 128).toInt();
	listView->setGridSize(QSize(nGridSize, nGridSize));

	int nIconSize=parseConfig(getAppParentDir()+INI_FILE, "main/iconSize", 64).toInt();
	listView->setIconSize(QSize(nIconSize, nIconSize));

	listView->setMinimumHeight(nGridSize+GROUP_SPACING);

	m_mapListView.insert(title, listView);
	m_vecTitle.append(title);

	connect(listView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(doubleClicked(const QModelIndex&)));
}

QStandardItemModel* QListGroupWidget::viewModel(const QString& title)
{
	if (m_mapListView.contains(title))
	{
		QStandardItemModel* pModel=NULL;

		QListView* pListView=m_mapListView[title];
		if (pListView != NULL)
		{
			pModel=static_cast<QStandardItemModel*>(pListView->model());
		}

		return pModel;
	}
	else
	{
		return NULL;
	}
}

void QListGroupWidget::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	QRect rect=contentsRect();

	painter.setBrush(Qt::white);
	painter.drawRect(rect);

	QPalette palette;
	int nLineLen=rect.width()/3>GROUP_LINE_WIDTH_MIN? rect.width()/3:GROUP_LINE_WIDTH_MIN;
	QLinearGradient gradient(0.0, 0.0, nLineLen, 0.0);
	gradient.setColorAt(0.0, QColor(60, 150, 255));
	gradient.setColorAt(1.0, Qt::white);

	QBrush brush(gradient);

	QFontMetrics metrics(m_font);
	painter.setFont(m_font);

	int y=TITLE_SPACING;
	foreach (QString strTitle, m_vecTitle)
	{
		if (strTitle.isEmpty() || !m_mapListView.contains(strTitle))
		{
			continue;
		}

		const QListView* listView=m_mapListView.value(strTitle);
		if (listView == NULL)
		{
			continue;
		}

		y+=metrics.height();

		painter.setPen(QPen(Qt::black));
		painter.drawText(TITLE_BORDER, y, strTitle);

		y+=TITLE_SPACING;

		painter.setPen(Qt::NoPen);
		painter.setBrush(brush);
		painter.drawRect(0, y, nLineLen, 1);

		QSize size=listView->gridSize();
		int nRows=ceil((double)listView->model()->rowCount()/((rect.width()-BORDER_SPACING)/size.width()));
		int nHeight=size.height()*(nRows>1? nRows:1)+GROUP_SPACING;
		y+=nHeight+TITLE_SPACING+GROUP_SPACING;
	}
}

void QListGroupWidget::adjustWidget(int width, int height)
{
	width-=3;
	height-=3;

	int y=TITLE_SPACING;
	QFontMetrics metrics(m_font);
	QListView* listView;
	foreach (QString strTitle, m_vecTitle)
	{
		if (strTitle.isEmpty() || !m_mapListView.contains(strTitle))
		{
			continue;
		}

		listView=m_mapListView[strTitle];
		if (listView == NULL)
		{
			continue;
		}

		y+=metrics.height()+TITLE_SPACING*2;

		QSize size=listView->gridSize();
		int nRows=ceil((double)listView->model()->rowCount()/((width-BORDER_SPACING)/size.width()));
		int nHeight=size.height()*(nRows>1? nRows:1)+GROUP_SPACING;
		listView->setGeometry(1, y, width, nHeight);

		y+=nHeight+GROUP_SPACING;
	}

	resize(width, y>height? y:height);
}

void QListGroupWidget::doubleClicked(const QModelIndex& index)
{
	QListView* listView=dynamic_cast<QListView*>(sender());
	if (listView == NULL)
	{
		return;
	}

	QString strTitle=m_mapListView.key(listView);
	if (!strTitle.isEmpty())
	{
		emit itemDoubleClicked(strTitle, index);
	}
}
