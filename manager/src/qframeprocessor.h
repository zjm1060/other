#ifndef QFRAMEPROCESSOR_H
#define QFRAMEPROCESSOR_H

#include <QFrame>
#include <QStandardItemModel>
#include <QMap>
#include <QGridLayout>

#include "ui_processor.h"
#include "extern.h"
#include "qframecurve.h"

using namespace Ui;

class QFrameProcessor : public QFrame, public Ui_FrameProcessor
{
	Q_OBJECT

public:
	QFrameProcessor(QWidget *parent);
	~QFrameProcessor();

public:
	void clear();

	void setProcessors(const QVector<ProcessorInfo>& processors);
	void setPerfProcessor(const PerfProcessor& processor);

private slots:
	void currentIndexChanged(int index);

private:
	void showProcessors();
	void showPerfProcessor();

private:
	QGridLayout*	m_pGridLayout;
	QMap<QString, QFrameCurve*>	m_mapFrameCurve;

	QVector<ProcessorInfo> m_vecProcessor;
	PerfProcessor	m_perfProcessor;

	QStandardItemModel*		m_pModel;
};

#endif // QFRAMEPROCESSOR_H
