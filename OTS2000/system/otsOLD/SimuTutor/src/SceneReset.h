#ifndef _SCENERESET_H_
#define _SCENERESET_H_
//工况选择性初始化 add by dzy 20120608
#include "ui_SceneReset.h"
#include "pointname.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
typedef struct
{
	UCHAR stnid;
	UCHAR devid;
	UCHAR data_type;
}D_type;
Q_DECLARE_METATYPE(D_type);

class SceneReset : public QDialog,public Ui_SceneReset
{
	Q_OBJECT
public:
	SceneReset(QWidget *parent=0);
	void InitTreeView();
	void resetpt(UCHAR stn_id,UCHAR dev_id,UCHAR data_type);
	QString pt_type;
	QStringList typelist;
	int ivalue;
	float fvalue;
	bool flagint,flagfloat;
	
signals:
private:
	private slots:
		void ItemSelected(QTreeWidgetItem *);
		void on_confirm();
		void on_init();
		void on_ok();
		void on_cancel();
		
};
#endif