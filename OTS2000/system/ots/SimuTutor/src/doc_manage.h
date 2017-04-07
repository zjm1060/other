#ifndef _ABOUT_H_
#define _ABOUT_H_

#include "ui_doc_manage.h"
#include "ui_study_plan.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"
#include <QAbstractItemModel>
#include <QAxWidget>
typedef struct  
{
	QFileInfo file;
	int depth;
}QFileitem;
static int maxdepth;
typedef QList<QFileitem> QFileitemList;
enum MYItemDataRole {
	UserRole_DIRINFO =33,
	UserRole_Studyinfo = 34,
	UserRole_Headerdata = 35,
	UserRole_DIRINFO2 =36
};
class TreeItem
{
public:
	TreeItem(const QFileitem &data, TreeItem *parent = 0);
	~TreeItem();

	void appendChild(TreeItem *child);

	TreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QString data(int column,int role) const;
	int row() const;
	TreeItem *parent();

private:
	QList<TreeItem*> childItems;
	QFileitem itemData;
	TreeItem *parentItem;
};
class MytreeModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	explicit MytreeModel(QFileitemList &data,QObject *parent=0);
	~MytreeModel();
	QModelIndex index(int row, int column,
		const QModelIndex &parent) const;
	QModelIndex parent(const QModelIndex &child)const;
	int rowCount(const QModelIndex &parent ) const ;
	int columnCount(const QModelIndex &parent ) const ;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const ;
	//bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
	Qt::ItemFlags flags(const QModelIndex &index) const;
	//File_item rootitem;

private:
	QFileitem *rootinfo;
	TreeItem *rootItem;
	void initModel();
	//void clear();
	void findFile(const QString & path);
	void setupModelData(const QFileitemList &lines);
};

class Doc_manage : public QDialog,public Ui_doc_manage
{
	Q_OBJECT
public:
	Doc_manage(QWidget *parent=0);
 	void loadChinese();
 	void loadEnglish();
	DATA_DOC m_doc;
	QString docname,updatetime;
protected:
	void showEvent(QShowEvent *event);
private:
	QStandardItemModel *docSourceModel;
	QSortFilterProxyModel *docModel;
	QItemSelectionModel *selectionModel;

	MytreeModel *m_mymodel;
	QFileitemList file_list;
	int depth;//记录文件深度
	bool is_root;
	bool new_doc;
	bool is_loadall;

	void findFile(const QString & path);
	void getDocType();
	void displayDoclist();
	void setdocModel();
	void 	setControlAtt();
	void cleardocinfo();
private slots:
	void on_view_DBclicked ( const QModelIndex & index );
	void on_pushButton_adddoc();
	void on_pushButton_LoadAlldoc();
	void on_pushButton_modifydoc();
	void on_pushButton_deldoc();
	void on_pushButton_OK();
	void on_pushButton_CANCEL();
public slots:
	void setLanguage( int );
};
class Study_plan: public QDialog,public Ui_study_plan
{
	Q_OBJECT
public:
	Study_plan(QWidget *parent=0);
	void loadChinese();
	void loadEnglish();
	bool flag_add,flag_modify;
	DATA_STUDY_PLAN m_plan;
 	QString planname,updatetime;
protected:
	void showEvent(QShowEvent *event);
private:
	QStandardItemModel *planSourceModel;
	QSortFilterProxyModel *planModel;
	QItemSelectionModel *selectionModel;

 	void setplanModel();
 	void 	setControlAtt();
	void init_doc_comobox();
 	void clearplaninfo();
private slots:
// 	void on_view_DBclicked ( const QModelIndex & index );
	void on_pushButton_addplan();
	void on_pushButton_modifyplan();
 	void on_pushButton_delplan();
	void on_pushButton_OK();
	void on_pushButton_EXIT();
};

#endif