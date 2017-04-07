#ifndef QFRAMECURVE_H
#define QFRAMECURVE_H

#include <QFrame>
#include <QVector>
#include <QColor>
#include <QMap>
#include <QTimer>

class QFrameCurve : public QFrame
{
	Q_OBJECT

public:
	QFrameCurve(QWidget *parent);
	~QFrameCurve();

public:
	typedef struct _curve 
	{
		QColor color;

		int nIndex;
		QVector<double>	value;
	}Curve;

public:
	void addCurve(const QString& name, const QColor& color);
	void removeCurve(const QString& name);
	bool containsCurve(const QString& name);

	void setCurveValue(const QString& name, double value);
	void clearCurves();

	void setMaxValue(double maxValue);

protected:
	virtual void paintEvent(QPaintEvent* event);

private slots:
	void timeout();

private:
	void drawGrid(QPainter& painter);
	void drawCurves(QPainter& painter);
	void drawCurveMark(QPainter& painter);

private:
	quint64		m_nTick;

	quint32		m_nStep;
	quint32		m_nInterval;

	int		m_nGridWidth;
	QColor	m_colorGridLine;
	int		m_nLineWidth;
	int		m_nFontSize;

	double	m_dMaxValue;
	int		m_nNumMaxPoints;
	QMap<QString, Curve>	m_mapCurves;

	QTimer	m_timer;
};

#endif // QFRAMECURVE_H
