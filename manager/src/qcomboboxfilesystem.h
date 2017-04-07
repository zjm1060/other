#ifndef QCOMBOBOXFILESYSTEM_H
#define QCOMBOBOXFILESYSTEM_H

#include <QComboBox>
#include <QTreeView>
#include <QFileIconProvider>
#include <QVector>

#include "extern.h"
#include "qtreemodel.h"

#define COMPUTER_ITEM	tr("Computer")
//#define TRASH_ITEM		tr("Trash")

class QComboBoxFileSystem : public QComboBox
{
	Q_OBJECT

public:
	QComboBoxFileSystem(QWidget *parent);
	~QComboBoxFileSystem();

	enum ItemType
	{
		OtherItem=-1,
		Computer=0,
//		Trash,
		ItemEnd
	};

public:
	QTreeModel* treeModel();
	void clear();

	void setDrives(const QFileInfoList& drives);
	void setDrives(const QVector<FileInfo>& drives);

	void setCurrentLocation(const QString& location, bool test=true);
	QString currentLocation() const;

	QString parentLocation(const QString& location);
	
	QString selectedLocation() const;

	QModelIndex itemIndex(ItemType type);
	QString itemText(ItemType type);

	int topItemType(const QString& location);

public:
	virtual void showPopup();

Q_SIGNALS:
	void enterPressed(QString location);
	void itemPressed(QModelIndex index);
	void focusStatusChanged(bool has);

protected:
	virtual void paintEvent(QPaintEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void focusInEvent(QFocusEvent* event);
	virtual void focusOutEvent(QFocusEvent* event);

private:
	void adjustPopup();

private:
	QFileIconProvider		m_fileIconProvider;

	QTreeModel*		m_pTreeModel;
	QTreeView*		m_pTreeView;
	QModelIndex		m_currentModelIndex;

	QString		m_strCurrentLocation;
};

#endif // QCOMBOBOXFILESYSTEM_H
