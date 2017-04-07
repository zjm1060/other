#ifndef QLISTGROUPWIDGET_H
#define QLISTGROUPWIDGET_H

#include <QAbstractScrollArea>
#include <QListView>
#include <QStandardItemModel>

class QListGroupWidget : public QFrame
{
	Q_OBJECT

public:
	QListGroupWidget(QWidget *parent=0);
	~QListGroupWidget();

public:
	void addGroup(const QString& title);

	QStandardItemModel* viewModel(const QString& title);

public slots:
	void adjustWidget(int width, int height);
	void doubleClicked(const QModelIndex& index);

signals:
	void itemDoubleClicked(const QString& title, const QModelIndex& index);

protected:
	virtual void paintEvent(QPaintEvent* event);

private:
	QVector<QString>				m_vecTitle;
	QMap<QString, QListView*>		m_mapListView;

	QFont	m_font;
};

#endif // QLISTGROUPWIDGET_H
