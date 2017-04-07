#ifndef _PRIVILEGEMANAGER_H_
#define _PRIVILEGEMANAGER_H_

#include "ui_privilegemanager.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "qcommon.h"
#include "login.h"

class CPrivilegeMananger : public QDialog,public Ui_CPrivilegeMananger
{
	Q_OBJECT
	public:
		CPrivilegeMananger(QWidget *parent=0);
		void loadChinese();
		void loadEnglish();
	private:
		UINT priv_mask_tmp[MAX_USER_NUM][MAX_PRIV_MODE_NUM];
		struct _priv_copy
		{
			int		user_id;
			short	role;
			UINT	priv_mask_copy[MAX_PRIV_MODE_NUM];
		}priv_copy;
		QMenu *popMenu;
		QAction *actionPrivCopy,*actionPrivPaste;
		QStandardItemModel *userSourceModel;
		QSortFilterProxyModel *userModel;
		QItemSelectionModel *selectionModel;
		void setUserSourceModel();
	signals:
		void sigPrivChanged(int);
	protected:
		void showEvent(QShowEvent *);
		void keyPressEvent(QKeyEvent *);
	private slots:
		void selectAll();
		void selectOff();
		void selectInvert();
		void privCopy();
		void privPaste();
		void popupUserMenu(const QPoint &);
		void privModeChanged(int);
		void showUserPriv(const QModelIndex &);
		void setPrivMask(QListWidgetItem *);
		void saveAllPriv();
		void userFilterChanged(const QString &);
};
#endif