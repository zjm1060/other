#include "qinspectdelegate.h"

#include <QApplication>

#include "common.h"
#include "qtreeviewinspect.h"

QInspectDelegate::QInspectDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

QInspectDelegate::~QInspectDelegate()
{

}

void QInspectDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QAbstractItemModel* pModel=index.model();

	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	int nRole=pModel->data(pModel->index(index.row(), QTreeViewInspect::NameTitle, index.parent()), QTreeViewInspect::ItemRole).toInt();

	if (index.column() == QTreeViewInspect::NameTitle)
	{
		if (nRole == QTreeViewInspect::PeerItem)
		{
			style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

			ServerInfo info=qVariantValue<ServerInfo>(pModel->data(
				pModel->index(index.row(), QTreeViewInspect::NameTitle, index.parent()), QTreeViewInspect::PeerInfoRole));

			bool bCompared=qVariantValue<bool>(pModel->data(
				pModel->index(index.row(), QTreeViewInspect::NameTitle, index.parent()), QTreeViewInspect::ComparedRole));

			int nMetric=style->pixelMetric(QStyle::PM_IconViewIconSize, 0, widget);

			QPixmap pixmap=findPixmap(":/images/host.png").scaled(nMetric, nMetric);

			QRect rect=option.rect;
			rect.adjust(2, 0, 0, 0);

			style->drawItemPixmap(painter, rect, Qt::AlignLeft|Qt::AlignVCenter, pixmap);

			rect.adjust(nMetric+6, 1, 0, 0);
			QString strDesc=info.hostName;

#ifndef Q_OS_WIN
			if ((opt.state & QStyle::State_Selected) != 0)
			{
				opt.palette.setColor(QPalette::Text, opt.palette.color(QPalette::Active, QPalette::HighlightedText));
			}
#endif
			
			style->drawItemText(painter, rect, Qt::AlignLeft|Qt::AlignTop, opt.palette, true, strDesc, QPalette::Text);

			if (bCompared)
			{
				opt.palette.setColor(QPalette::Text, QColor(235, 104, 24));

				int nWidth=opt.fontMetrics.width(strDesc);
				strDesc=tr("[Reference source]");
				style->drawItemText(painter, rect.adjusted(nWidth, 0, 0, 0), Qt::AlignLeft|Qt::AlignTop, 
					opt.palette, true, strDesc, QPalette::Text);
			}

			if (info.count > 0)
			{
				int nFontHeight=opt.fontMetrics.height();
				rect.adjust(0, nFontHeight, 0, 0);

				strDesc="[";
				int nIndex=0;

				QMapIterator<QString, QVector<Server> > it(info.servers);
				while (it.hasNext())
				{
					it.next();

					const QVector<Server>& servers=it.value();
					for (int i=0; i<servers.size(); i++)
					{
						strDesc+=(nIndex++ == 0?
							QString("%1:%2").arg(servers[i].addr).arg(servers[i].port):QString(", %1:%2").arg(servers[i].addr).arg(servers[i].port));
					}
				}

				strDesc+="]";

				strDesc=textWrapped(strDesc, rect.width(), opt.fontMetrics);

				opt.palette.setColor(QPalette::Text, QColor(121, 121, 121));

				style->drawItemText(painter, rect, Qt::AlignLeft|Qt::AlignTop, opt.palette, true, strDesc, QPalette::Text);
			}

			return;
		}
		else if (nRole == QTreeViewInspect::ChildItem)
		{
			QColor color(57, 57, 57);

			opt.palette.setColor(QPalette::Text, color);
		}
	}
	else if (index.column() == QTreeViewInspect::StatusTitle)
	{
		int nStatus=pModel->data(pModel->index(index.row(), QTreeViewInspect::NameTitle, index.parent()), QTreeViewInspect::StatusRole).toInt();
		switch (nStatus)
		{
		case QTreeViewInspect::Initialized:
			break;
		case QTreeViewInspect::Connecting:
			{
				opt.icon=QIcon(":/images/check.png");
				opt.text=tr("Connecting...");
			}
			break;
		case QTreeViewInspect::Reconnecting:
			{
				opt.icon=QIcon(":/images/check.png");
				opt.text=tr("Reconnecting...");
			}
			break;
		case QTreeViewInspect::Unconnected:
			{
				opt.icon=QIcon(":/images/error.png");
				opt.text=tr("Unconnected");
			}
			break;
		case QTreeViewInspect::Connected:
			{
				opt.icon=QIcon(":/images/checkmark.png");
				opt.text=tr("Connected");
			}
			break;
		case QTreeViewInspect::Collecting:
			{
				opt.icon=QIcon(":/images/information.png");
				opt.text=tr("Collecting...");
			}
			break;
		case QTreeViewInspect::Verifying:
			{
				opt.icon=QIcon(":/images/information.png");
				opt.text=tr("Verifying...");
			}
			break;
		case QTreeViewInspect::Skip:
			{
				opt.icon=QIcon(":/images/warning.png");
				opt.text=tr("Skip");

				int nCause=pModel->data(pModel->index(index.row(), QTreeViewInspect::NameTitle, index.parent()), QTreeViewInspect::CauseRole).toInt();
				switch (nCause)
				{
				case QTreeViewInspect::H9000NotRunning:
					opt.text+=" ["+tr("H9000 is not running")+"]";
					break;
				case QTreeViewInspect::DataSourceNoExisted:
					opt.text+=" ["+tr("Non-existent")+"]";
					break;
				case QTreeViewInspect::NoReferenceSource:
					opt.text+=" ["+tr("No reference source")+"]";
					break;
				case QTreeViewInspect::ServiceQuit:
					opt.text+=" ["+tr("Service quit")+"]";
					break;
				default:
					break;
				}
			}
			break;
		case QTreeViewInspect::Failed:
			{
				opt.icon=QIcon(":/images/error.png");
				opt.text=tr("Collection failed");
			}
			break;
		case QTreeViewInspect::Completed:
			{
				opt.icon=QIcon(":/images/checkmark.png");
				opt.text=tr("Collection completed");
			}
			break;
		case QTreeViewInspect::Match:
			{
				opt.icon=QIcon(":/images/checkmark.png");
				opt.text=tr("Match");
			}
			break;
		case QTreeViewInspect::Mismatch:
			{
				opt.icon=QIcon(":/images/warning.png");
				opt.text=tr("Mismatch");
			}
			break;
		default:
			break;
		}

		opt.decorationAlignment=Qt::AlignRight|Qt::AlignVCenter;
		opt.features|=QStyleOptionViewItemV2::HasDecoration;
	}
	
	QStyledItemDelegate::paint(painter, opt, index);
}

QString QInspectDelegate::textWrapped(const QString& text, int width, const QFontMetrics& metrics) const
{
	int nTextWidth=metrics.width(text);
	if (nTextWidth <= width)
	{
		return text;
	}

	int nNum=text.length()*width/nTextWidth-3;
	nNum=(nNum>1? nNum:1);

	return text.left(nNum)+"...";
}

QSize QInspectDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const QAbstractItemModel* pModel=index.model();

	const QWidget *widget=NULL;
	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
	{
		widget=v3->widget;
	}

	QStyle *style = widget ? widget->style() : QApplication::style();

	int nRole=pModel->data(pModel->index(index.row(), QTreeViewInspect::NameTitle, index.parent()), QTreeViewInspect::ItemRole).toInt();

	int nHeight=(nRole== QTreeViewInspect::PeerItem? 
		style->pixelMetric(QStyle::PM_IconViewIconSize, 0, widget):style->pixelMetric(QStyle::PM_ListViewIconSize, 0, widget))+6;

	QSize size=QStyledItemDelegate::sizeHint(option, index);
	if (size.height() < nHeight)
	{
		size.setHeight(nHeight);
	}

	return size;
}
