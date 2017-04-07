#ifndef _SCOREREC_H_
#define _SCOREREC_H_
#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QSqlQuery>   
#include <QVariant>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QProgressDialog>
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "qcommon.h"
#include "excelobject.h"
#include "ui_scorerec.h"
#include "subscore.h"
#include "historyrec.h"
class MyDelegate;
class MyDelegate : public QItemDelegate  
{  
	Q_OBJECT  
public:  
	MyDelegate(QObject *parent = 0): QItemDelegate(parent) { }  
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,  
		const QModelIndex &index) const  
	{  
		QSpinBox *editor = new QSpinBox(parent);  
		editor->setMinimum(-500);  
		editor->setMaximum(500);  
		return editor;  
	}  
	void setEditorData(QWidget *editor, const QModelIndex &index) const  
	{  
		int value = index.model()->data(index, Qt::EditRole).toInt();  
		QSpinBox *spinBox = static_cast<QSpinBox*>(editor);  
		spinBox->setValue(value);  
	}  
	void setModelData(QWidget *editor, QAbstractItemModel *model,  
		const QModelIndex &index) const  
	{  
		QSpinBox *spinBox = static_cast<QSpinBox*>(editor);  
		spinBox->interpretText();  
		int value = spinBox->value();  
		model->setData(index, value, Qt::EditRole);  
	}  
	void updateEditorGeometry(QWidget *editor,  
		const QStyleOptionViewItem &option, const QModelIndex &index) const  
	{  
		editor->setGeometry(option.rect);  
	}  
};  
class CScoreRec:public QWidget,public Ui_CScoreRec
{
	Q_OBJECT
public:
	CScoreRec(QWidget *parent=0);
	~CScoreRec();
	QAction *actionSubscore;
	QAction *actionSetscore;
	QAction *actionFindRcords;
	SubScore *m_SubScore;
	CHistoryRecTable *d_his;
	void contextMenuEvent(QContextMenuEvent *);
	void loadChinese();
	void loadEnglish();
private:
	QStandardItemModel *recSourceModel;
	QSortFilterProxyModel *recModel;
	QItemSelectionModel *selectionModel;
	MyDelegate spinBoxDelegate;  
protected:
	void showEvent(QShowEvent *);
private slots:
	void setScoreRecModel(DB_FMT *data);
	void modeChanged(int);
	void displayTodayRec();
	void displayScoreRec();
	void filterChanged(const QString &);
	void exportToExcel();
	//dzy
	void on_subscore();
	void on_setscore();
	void on_Actset();
	void on_ActRecd();
	public slots:
		void setLanguage( int );
};

#endif