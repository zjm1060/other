#ifndef _STUDENTINFO_H_
#define _STUDENTINFO_H_

#include <Qt>
#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>   
#include <QSqlDatabase>   
#include <QSqlQuery>   
#include <QVariant>


#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QComboBox>


#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "qcommon.h"
#include "ui_studentinfo.h"
//#include "ui_studentinfolookat.h"
#include "ui_studentinfoadd.h"
#include "ui_groups.h"

class CGroupsTable : public QDialog,public Ui_CGroupsTable
{
	Q_OBJECT
	public:
		CGroupsTable(QWidget *parent=0);
		QDialog *groupAddTable;
		QPushButton *btnEnter,*btnExit;
		QLineEdit *lineEdit;
		QGridLayout *gridLayout;
	private:
		QMenu *menuNoGroupsUser;
		QWidget *widgetNoGroupsUser;
		QAction* actionGroupsHead,*actionDelUser,*actionAddAllNoGroupsUser;
		QList<int> listNoGroupsUserId;
		QStandardItemModel *noGroupsUserSourceModel;
		QSortFilterProxyModel *noGroupsUserModel;
		void displayGroups();
		void creatGroupAddTable();
		void updateNoGroupsUserMenu();
		void writeGroupsRecord(int,int,short);
		void creatNoGroupUserWidget();
	protected:
		void showEvent(QShowEvent *);
	private slots:
		void showGroupsUsers(QListWidgetItem *);
		void openAddGroupsTable();
		void openModifyGroupsTable();
		void addGroups();
		void delGroups();
		void addGroupsUser(const QModelIndex &);
		void addAllNoGroupsUser();
		void chgUserGroups();
		void delGroupsUser();
		void setGroupsHead();
		void modifyGroups();
		void menuListGroupUser(const QPoint &);
		void openNoGroupUser();
		void filterNoGroupUser(const QString &);
};

class CStudentInfoAddTable : public QDialog,public Ui_CStudentInfoAddTable
{
	Q_OBJECT
public:
	CStudentInfoAddTable(QWidget *parent=0);
	DATA_USERINFO seg;
	bool flag_add,flag_modify;
	void setControlAtt();
	void loadChinese();
	void loadEnglish();
public:
signals:
	void insertRecord();
	void modifyRecord();
	private slots:
		int apply();
		void enter();
};
/*class CStudentInfoLookAtTable : public QDialog,public Ui_CStudentInfoLookAtTable
{
	Q_OBJECT
public:
	CStudentInfoLookAtTable(QWidget *parent=0);
	void loadChinese();
	void loadEnglish();
private:
	private slots:
};*/
class CStudentInfoTable : public QDialog,public Ui_CStudentInfoTable
{
	Q_OBJECT
public:
	CStudentInfoTable(QWidget *parent=0);
	CStudentInfoAddTable* studentinfoaddtable; 
	CStudentInfoAddTable* studentinfomodifytable;
	//CStudentInfoLookAtTable* studentinfolookattable;
	CGroupsTable* groupstable;
	int rownums;
	int rows;
	int columns;
	int max_user_id;
	bool select_status;
	QDateTime time_start,time_end;
	void delUserFromHdb(int);
	void setControlAtt();
	void loadChinese();
	void loadEnglish();
private:
	QMenu* popMenu;
	QStandardItemModel *userSourceModel;
	QSortFilterProxyModel *userModel;
	QItemSelectionModel *selectionModel;
protected:
	void contextMenuEvent(QContextMenuEvent *);
	void showEvent(QShowEvent *);
	public slots:
		void openStudentInfoLookAtTable();
		void openStudentInfoAddTable();
		void openStudentInfoModifyTable();
		void openGroupsTable();
		void addUser();
		void modifyUser();
		void displayOperateRecord(int);
		void displayAlarmRecord(int);
		void displayLoginRecord(int);
		void setUserModel();
		void changeNameFilter(const QString &);
		void setNameFilter(const QString &);
		void filterButtonClicked();
		void delUser();
		void resetPwd();
};
class ComboBoxDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	ComboBoxDelegate(QObject *parent = 0);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
#endif