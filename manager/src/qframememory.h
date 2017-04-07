#ifndef QFRAMEMEMORY_H
#define QFRAMEMEMORY_H

#include <QFrame>
#include <QVector>
#include <QStandardItemModel>
#include <QFileIconProvider>

#include "ui_memory.h"
#include "extern.h"
#include "qframecurve.h"

using namespace Ui;

class QFrameMemory : public QFrame, public Ui::FrameMemory
{
	Q_OBJECT

public:
	QFrameMemory(QWidget *parent=0);
	~QFrameMemory();

public:
	void clear();

	void setMemoryDevice(const QVector<PhysicalMemory>& memory);
	void setSystemMemory(const SystemMemory& memory);

private slots:
	void currentIndexChanged(int index);

private:
	void showPhysicalMemory();
	void showSystemMemory();

private:
	QVector<PhysicalMemory>	m_vecPhysicalMemory;
	SystemMemory			m_memory;

	QStandardItemModel*		m_pModelPhysicalMemory;

	QFrameCurve*			m_pFrameCurve;
	QVBoxLayout*			m_pLayout;
};

#endif // QFRAMEMEMORY_H
