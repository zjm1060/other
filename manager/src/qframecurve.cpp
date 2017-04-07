#include "qframecurve.h"

#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QFont>
#include <QFontMetrics>
#include <QMutableMapIterator>

#include "common.h"

QFrameCurve::QFrameCurve(QWidget *parent)
	: QFrame(parent),
	m_nTick(0),
	m_dMaxValue(100.0)
{
	setFrameStyle(QFrame::Panel | QFrame::Raised);

	QPalette palette;
	palette.setColor(QPalette::Window, QColor(0, 0, 0));
	setPalette(palette);

	setAutoFillBackground(true);
	setBackgroundRole(QPalette::Window);

	m_nInterval=parseConfig(getAppParentDir()+INI_FILE, "framecurve/interval", 10).toUInt();
	m_nStep=parseConfig(getAppParentDir()+INI_FILE, "framecurve/step", 2).toUInt();
	if (m_nInterval < m_nStep*2)
	{
		m_nInterval=m_nStep*2;
	}

	m_nGridWidth=parseConfig(getAppParentDir()+INI_FILE, "framecurve/gridLineWidth", 1).toInt();
	m_colorGridLine=parseConfigColor(getAppParentDir()+INI_FILE, "framecurve/gridColor", QColor(0,128,64));

	m_nLineWidth=parseConfig(getAppParentDir()+INI_FILE, "framecurve/lineWidth", 1).toInt();
	m_nFontSize=parseConfig(getAppParentDir()+INI_FILE, "framecurve/fontSize", 14).toInt();

	m_nNumMaxPoints=QApplication::desktop()->width()/m_nStep;

	m_timer.setInterval(1000);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

QFrameCurve::~QFrameCurve()
{

}

void QFrameCurve::setMaxValue(double maxValue)
{
	m_dMaxValue=maxValue;
}

void QFrameCurve::clearCurves()
{
	m_timer.stop();

	m_nTick=0;
	m_mapCurves.clear();
}

void QFrameCurve::addCurve(const QString& name, const QColor& color)
{
	if (name.isEmpty())
	{
		return;
	}

	if (containsCurve(name))
	{
		return;
	}

	Curve curve;

	curve.color=color;
	curve.nIndex=(int)(m_nTick%m_nNumMaxPoints);
	curve.value.resize(m_nNumMaxPoints);
	curve.value.fill(0.0);

	m_mapCurves.insert(name, curve);

	if (m_mapCurves.size() > 0)
	{
		m_timer.start();
	}
}

void QFrameCurve::removeCurve(const QString& name)
{
	m_mapCurves.remove(name);

	if (m_mapCurves.size() == 0)
	{
		m_timer.stop();
	}
}

bool QFrameCurve::containsCurve(const QString& name)
{
	return m_mapCurves.contains(name);
}

void QFrameCurve::setCurveValue(const QString& name, double value)
{
	if (!m_mapCurves.contains(name))
	{
		return;
	}

	Curve& curve=m_mapCurves[name];

	curve.value[curve.nIndex]=value;
	curve.nIndex=(++curve.nIndex)%m_nNumMaxPoints;
}

void QFrameCurve::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	drawGrid(painter);
	drawCurves(painter);
	drawCurveMark(painter);
}

void QFrameCurve::timeout()
{
	m_nTick++;

	update();
}

void QFrameCurve::drawGrid(QPainter& painter)
{
	QRect rect=contentsRect();
	quint32 nTick=m_nTick%(m_nInterval/m_nStep);

	painter.save();

	QPen pen=QPen(m_colorGridLine);
	pen.setWidth(m_nGridWidth);
	painter.setPen(pen);

	//»­XÖá
	int nWidth=rect.width()-nTick*m_nStep;
	while (nWidth >= 0)
	{
		painter.drawLine(nWidth, 0, nWidth, rect.height());

		nWidth-=m_nInterval;
	}

	//»­YÖá
	int nHeight=rect.height();
	while (nHeight >= 0)
	{
		painter.drawLine(0, nHeight, rect.width(), nHeight);

		nHeight-=m_nInterval;
	}

	painter.restore();
}

void QFrameCurve::drawCurves(QPainter& painter)
{
	QRect rect=contentsRect();

	painter.save();

	int nWidth;
	int nHeight=rect.height();
	int x0,y0,x1,y1;
	int nPoints;
	int nIndex;

	QMutableMapIterator<QString, Curve> it(m_mapCurves);
	while (it.hasNext())
	{
		it.next();
		Curve& curve=it.value();

		QPen pen(curve.color);
		pen.setWidth(m_nLineWidth);
		painter.setPen(pen);

		nWidth=rect.width();
		nPoints=nWidth/m_nStep;

		nIndex=(int)(m_nTick%m_nNumMaxPoints);
		int nNum=(nIndex+m_nNumMaxPoints-curve.nIndex)%m_nNumMaxPoints;
		int nTemp=(curve.nIndex+m_nNumMaxPoints-nIndex)%m_nNumMaxPoints;
		if (nNum < nTemp)
		{
			for (int i=0; i<nNum; i++)
			{
				curve.value[(curve.nIndex+i)%m_nNumMaxPoints]=0.0;
			}
			curve.nIndex=(curve.nIndex+nNum)%m_nNumMaxPoints;
		}

		nIndex=(curve.nIndex-1+m_nNumMaxPoints)%m_nNumMaxPoints;
		x0=nWidth;
		y0=nHeight-(int)(curve.value[nIndex]*nHeight/m_dMaxValue);

		for (int i=1; i<nPoints; i++)
		{
			x1=x0-m_nStep;
			nIndex--;
			if (nIndex < 0)
			{
				nIndex+=m_nNumMaxPoints;
			}
			y1=nHeight-(int)(curve.value[nIndex]*nHeight/m_dMaxValue);

			painter.drawLine(x0, y0, x1, y1);

			x0=x1;
			y0=y1;
		}
	}

	painter.restore();
}

void QFrameCurve::drawCurveMark(QPainter& painter)
{
	painter.save();

	QFont font;
	font.setPointSize(m_nFontSize);
	painter.setFont(font);

	QFontMetrics metrics(font);
	int nHeight=metrics.height();
	int nWidth=0;

	int y=0;
	QMapIterator<QString, Curve> it(m_mapCurves);
	while (it.hasNext())
	{
		y+=nHeight;

		it.next();

		QPen pen(it.value().color);
		painter.setPen(pen);

		QString strText=it.key()+":";
		painter.drawText(0, y, strText);

		int nTemp=metrics.width(strText);
		nWidth=nTemp>nWidth? nTemp:nWidth;
	}

	it.toFront();
	y=0;
	while (it.hasNext())
	{
		y+=nHeight;

		it.next();

		QPen pen(it.value().color);
		painter.setPen(pen);

		QString strText=
			QString::number(it.value().value[(it.value().nIndex-1+m_nNumMaxPoints)%m_nNumMaxPoints]*100.0/m_dMaxValue, 'f', 2)+"%";
		painter.drawText(nWidth, y, strText);
	}

	painter.restore();
}
