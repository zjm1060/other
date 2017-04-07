#ifndef _SYSCONFIG_H_
#define _SYSCONFIG_H_

#include "ui_sysconfig.h"

class CSysConfigTable : public QDialog,public Ui_CSysConfigTable
{
	Q_OBJECT
	public:
		CSysConfigTable(QWidget *parent=0);
	private:
	private slots:
};
#endif