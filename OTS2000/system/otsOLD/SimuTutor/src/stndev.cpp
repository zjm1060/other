#include "stndev.h"

CStnDevTable::CStnDevTable(QWidget* parent)
	:QDialog(parent)
{
	setupUi(this);
	//this->setAttribute(Qt::WA_DeleteOnClose);
	//this->setModal(true);
	displayStnDev();
	listStnDev->header()->setHidden(1);
	connect(listStnDev,SIGNAL(itemClicked(QTreeWidgetItem *, int)),this,SLOT(select(QTreeWidgetItem *)));
}
void CStnDevTable::moveEvent ( QMoveEvent * event )
{	
	if (mvwith != false)
	{
		this->move(newpt.x()+xP,newpt.y()-30);
		mvwith = false;
	}	
}
void CStnDevTable::displayStnDev()
{
	listStnDev->setColumnCount(1);
	listStnDev->setIconSize(QSize(18, 24));
	for(int i=1;i<=g_stn_num;i++)
	{
		QTreeWidgetItem *root = new QTreeWidgetItem(listStnDev, QStringList(QString::fromLocal8Bit(data_station[i].station_name)));
		root->setIcon(0,QIcon(QString::fromLocal8Bit(":/pic/images/³§Õ¾.PNG")));	
		for (int j=1;j<=g_dev_num[i];j++)
		{
			QTreeWidgetItem *leaf = new QTreeWidgetItem(root, QStringList(QString::fromLocal8Bit(data_dev[i][j].dev_name)));  
			leaf->setIcon(0,QIcon(QString::fromLocal8Bit(":/pic/images/ots09.PNG")));
			root->addChild(leaf); 			
		}
	}
}
void CStnDevTable::select(QTreeWidgetItem * curItem)
{
	int stn_id,dev_id;
	bool flag = false;
	if (curItem->childCount() != 0)
	{
		for (int i=1;i<=g_stn_num;i++)
		{
			if (curItem->text(0)==QString::fromLocal8Bit(data_station[i].station_name))
			{
				stn_id = data_station[i].station_id;
				break;
			}
		}
		emit update(stn_id);
	}
	else
	{
		QTreeWidgetItem *parentItem = curItem->parent();
		for (int i=1;i<=g_stn_num;i++)
		{
			for (int j=1;j<=g_dev_num[i];j++)
			{
				if (curItem->text(0)==QString::fromLocal8Bit(data_dev[i][j].dev_name))
				{
					stn_id = data_dev[i][j].station_id;
					dev_id = data_dev[i][j].dev_id;
					flag = true;
					break;
				}
			}
			if (flag)
			{
				break;
			}
		}
		emit update(stn_id,dev_id);
	}
}