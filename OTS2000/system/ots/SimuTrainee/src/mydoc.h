#ifndef _MYDOC_H
#define _MYDOC_H

#include "ui_mydoc.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"
#include "OfficeContainer.h"
#include <QAbstractItemModel>
#include <QAxWidget>
#include <QFocusEvent>

class OfficeContainer;
typedef struct  
{
	QFileInfo file;
	int depth;
}QFileitem;
static int maxdepth;
typedef QList<QFileitem> QFileitemList;
enum MYItemDataRole {
	UserRole_DIRINFO =33,
	UserRole_Studyinfo = 34
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
class Mydoc : public QWidget,Ui_Mydoc
{
	Q_OBJECT
public:
	 Mydoc(QWidget *parent=0);
	 ~Mydoc();
	 void loadChinese();
	 void loadEnglish();
protected:
	void showEvent(QShowEvent *);
	bool eventFilter(QObject *o, QEvent *e);//事件过滤器 
private:
	MytreeModel *m_mymodel;
	QFileitemList file_list;
	OfficeContainer *pOfficeContainer;

	QFocusEvent *pFocusEvent ;
	//QTimer *m_Timer_focus;
	QTimer *m_Timer_mousestop;
	int cout_mouseunmoved;

	int depth;//记录文件深度
	bool flag_windowact;//记录窗口活动
	bool is_root;
	DATA_DOC m_doc;
	DATA_STUDY_PLAN m_study_plan;
	QTimer *m_Timer;
	int timecount;
	int groupbox_w;
	void findFile(const QString & path);
	void initdata();
	void writerec();
	void pausetimer();
	void continuetimer();
private slots:
	void on_view_doubleClicked ( const QModelIndex & index );
	void on_pushButtonStudy_Clicked ();
	void on_pushButton_MystudyRec_Clicked ();
	void on_pushButton_finishdoc_Clicked ();
	void on_collapse();
	void timeplus();
	void timeplus_mouse();
public slots:
	void setLanguage( int );
signals:
	void sig_myrec();
};

#endif