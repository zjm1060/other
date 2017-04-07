#include "checkWin.h"
#include <QtGui>
#include <QDebug>
CcheckWin::CcheckWin(MdiMainWindow * parent): QSplitter(Qt::Horizontal)
{
	m_parent=parent;
	char splashfile[80];
	char *prun_home=NULL;//getenv("H9000_HOME");
	if(prun_home)
	{

		strcpy(splashfile,prun_home);
		strcat(splashfile,"/h9000/cfgmgr/images/cfgmgr.png");
	}
	else
	{
		strcpy(splashfile,"/home/ems/h9000/cfgmgr/images/cfgmgr.png");
	}
	QPixmap  splashpixfile(splashfile);
	QSplashScreen *gexploresplash = new QSplashScreen( splashpixfile );
	gexploresplash->setMask( splashpixfile.mask() );
	gexploresplash->show();
	gexploresplash->showMessage("Loaded modules");
	//app.processEvents();

	initClass();

	gexploresplash->finish( parent );


}
CcheckWin::~CcheckWin()
{
	if (pCheckInterlock) 
	{
		delete pCheckInterlock;
		pCheckInterlock = 0;             
	}

	if (pCheckCalcu) 
	{
		delete pCheckCalcu;
		pCheckCalcu = 0;             
	}

	if (pCheckDbin) 
	{
		delete pCheckDbin;
		pCheckDbin = 0;             
	}

	if (pCOperateXml) 
	{
		delete pCOperateXml;
		pCOperateXml = 0;             
	}

	if (psouceDefFile) 
	{
		delete psouceDefFile;
		psouceDefFile = 0;             
	}


}

void CcheckWin::initClass()
{	
	initOperateXml();

	initInterlock();
	initCalcu();
	initDbin();
	initDcom();
	initTemp();
	initWater();

	createMainView();
	createActions();
	creatToolbar();

	psouceDefFile=NULL;
	cursourceDefName.clear();

	for (int i=0;i<TABLECOLUMNUM;i++)
	{
		filterColumnComboBox->addItem(TABLECOLUMNAME[i]);
		m_paralist1->setColumnWidth(i,TABLECOLUMWIDTH[i]);
	}

}
void CcheckWin::initInterlock()
{
	pCheckInterlock=new CheckInterlock(m_parent,interlockcfg);
}
void CcheckWin::initCalcu()
{
	pCheckCalcu=new CheckCalcu(m_parent,calcucfg);
}
void CcheckWin::initDbin()
{
	pCheckDbin=new CheckDbin(m_parent,dbincfg);

}
void CcheckWin::initDcom()
{
	pCheckDcom=new CheckDcom(m_parent,dcomcfg);
}

void CcheckWin::initTemp()
{
	pCheckTempeigen=new CheckTempeigen(m_parent,tempcfg);
}
void CcheckWin::initWater()
{
	pCheckWaterRevert=new CheckWaterRevert(m_parent,watercfg);
}
void CcheckWin::initOperateXml()
{
	pCOperateXml=new COperateXml(m_parent);
	if (!pCOperateXml->openxml())
	{
		exit(-1);
	}
	
	getcfgmgrcfg();

}
void CcheckWin::getmainWincfg()
{
	QDomNode dbintypenode=pCOperateXml->getElementbyStr("mainwindow");

	QDomNodeList list = dbintypenode.childNodes();
	for(int i=0; i<list.count(); i++)
	{
		QDomNode node = list.at(i);
		if(node.isElement())
		{

			QString tmpstr;
			switch (i)
			{
			case 0 :
				tmpstr=node.toElement().text();
				mainWincfg.fileTypeName=tmpstr.split(",");
				break;
			}
		}
	}

}
void CcheckWin::getCheckcalcucfg()
{
	QDomNode typenode=pCOperateXml->getElementbyStr("checkcalcu");

	QDomNodeList list = typenode.childNodes();
	for(int i=0; i<list.count(); i++)
	{
		QDomNode node = list.at(i);
		if(node.isElement())
		{

			switch (i)
			{
			case 0 :

				calcucfg.calcufile=node.toElement().text();
				break;
			case 1:
				calcucfg.calcufile1=node.toElement().text();
				break;
			case 2 :
				calcucfg.errfile=node.toElement().text();
				break;
			case 3:
				calcucfg.filepath=node.toElement().text();
				break;
			}
		}
	}


}
void CcheckWin::getCheckdbincfg()
{
	QDomNode dbintypenode=pCOperateXml->getElementbyStr("checkdbin");

	QDomNodeList list = dbintypenode.childNodes();
	for(int i=0; i<list.count(); i++)
	{
		QDomNode node = list.at(i);
		if(node.isElement())
		{

			QString filetypestr;
			switch (i)
			{
			case 0 :
				filetypestr=node.toElement().text();
				dbincfg.filetype=filetypestr.split(",");
				break;
			case 1:
				dbincfg.filepath=node.toElement().text();
				break;
			case 2:
				dbincfg.errfile=node.toElement().text();
				break;
			}
		}
	}
}
void CcheckWin::getCheckinterlockcfg()
{
	QDomNode typenode=pCOperateXml->getElementbyStr("checkinterlock");

	QDomNodeList list = typenode.childNodes();
	for(int i=0; i<list.count(); i++)
	{
		QDomNode node = list.at(i);
		if(node.isElement())
		{

			switch (i)
			{
			case 0 :
				interlockcfg.interlockfile=node.toElement().text();
				break;
			case 1:
				interlockcfg.errfile=node.toElement().text();
				break;
			case 2 :
				interlockcfg.filepath=node.toElement().text();
				break;

			}
		}
	}
}
void CcheckWin::getChecktempcfg()
{
	QDomNode typenode=pCOperateXml->getElementbyStr("checktemp");

	QDomNodeList list = typenode.childNodes();
	for(int i=0; i<list.count(); i++)
	{
		QDomNode node = list.at(i);
		if(node.isElement())
		{

			switch (i)
			{
			case 0 :
				tempcfg.tempfile=node.toElement().text();
				break;
			case 1:
				tempcfg.errfile=node.toElement().text();
				break;
			case 2 :
				tempcfg.filepath=node.toElement().text();
				break;

			}
		}
	}
}
void CcheckWin::getCheckwatercfg()
{
	QDomNode typenode=pCOperateXml->getElementbyStr("checkwater");

	QDomNodeList list = typenode.childNodes();
	for(int i=0; i<list.count(); i++)
	{
		QDomNode node = list.at(i);
		if(node.isElement())
		{

			switch (i)
			{
			case 0 :
				watercfg.waterfile=node.toElement().text();
				break;
			case 1:
				watercfg.errfile=node.toElement().text();
				break;
			case 2 :
				watercfg.filepath=node.toElement().text();
				break;

			}
		}
	}
}

void CcheckWin::getCheckdcomcfg()
{
	QDomNode typenode=pCOperateXml->getElementbyStr("checkdcom");

	QDomNodeList list = typenode.childNodes();
	for(int i=0; i<list.count(); i++)
	{
		QDomNode node = list.at(i);
		if(node.isElement())
		{

			switch (i)
			{
			case 0 :
				{
					QDomNodeList list1 = node.childNodes();
					for(int j=0;j<list1.count();j++)
					{
						QDomNode node1 = list1.at(j);
						Dcomfile tmpcomfile;

						if (node1.isElement())
						{
							QDomElement e = node1.toElement();
							tmpcomfile.filename=e.attribute("filename");

							QString attribute=e.attribute("filetype");

							tmpcomfile.filetype=attribute.split(",", QString::SkipEmptyParts);

							attribute=e.attribute("nameindex");
							tmpcomfile.nameindex=attribute.toInt();

							attribute=e.attribute("checkrange");
							tmpcomfile.checkrange=attribute.toInt();

							attribute=e.attribute("LO_RANGE");
							tmpcomfile.LO_RANGEindex=attribute.toInt();

							attribute=e.attribute("HI_RANGE");
							tmpcomfile.HI_RANGEindex=attribute.toInt();

							attribute=e.attribute("checkinterlock");
							tmpcomfile.checkinterlock=attribute.toInt();

							attribute=e.attribute("InterlockIndex");
							tmpcomfile.interlockIndex=attribute.toInt();


							dcomcfg.dcomfilelist.append(tmpcomfile);


						}
					}
				}
				break;
			case 1:
				{
					QDomNodeList list1 = node.childNodes();
					for(int j=0;j<list1.count();j++)
					{
						QDomNode node1 = list1.at(j);
						Dcompath tmpcompath;

						if (node1.isElement())
						{
							QDomElement e = node1.toElement();
							tmpcompath.filetype=e.attribute("filetype");

							tmpcompath.filepath=e.attribute("filepath");

							tmpcompath.keyword=e.attribute("keyword");

							dcomcfg.dcompathlist.append(tmpcompath);


						}
					}
				}
				break;
			case 2:
				dcomcfg.errfile=node.toElement().text();
				break;


			}
		}
	}
}

void CcheckWin::getcfgmgrcfg()
{
	getmainWincfg();
	getCheckdbincfg();
	getCheckcalcucfg();
	getCheckinterlockcfg();
	getChecktempcfg();
	getCheckwatercfg();
	getCheckdcomcfg();
}
void CcheckWin::setCheckdbincfg()
{

}
void CcheckWin::setCheckcalcucfg()
{

}
void CcheckWin::setCheckinterlockcfg()
{

}
void CcheckWin::setCheckdcomcfg()
{

}
void CcheckWin::setChecktempcfg()
{

}
void CcheckWin::setCheckwatercfg()
{

}
void CcheckWin::OntoplevelItemCheckChanged(QTreeWidgetItem * curitem, int colum)
{
	m_filetree->blockSignals(true);

	if ( curitem==m_toplevelItem  ) //顶级item
	{		
		for (int i=0;i<m_toplevelItem->childCount();i++)
		{
			m_toplevelItem->child(i)->setCheckState(0,m_toplevelItem->checkState(0));
			for (int j=0;j<m_toplevelItem->child(i)->childCount();j++) //遍历一级的孩子
			{
				if (!m_toplevelItem->child(i)->child(j)->isDisabled())
				{
					m_toplevelItem->child(i)->child(j)->setCheckState(0,m_toplevelItem->checkState(0));
				}

				for (int k=0;k<m_toplevelItem->child(i)->child(j)->childCount();k++)
				{
					if (!m_toplevelItem->child(i)->child(j)->child(k)->isDisabled())
					{

						m_toplevelItem->child(i)->child(j)->child(k)->setCheckState(0,m_toplevelItem->child(i)->child(j)->checkState(0));
					}
					for(int l=0;l<m_toplevelItem->child(i)->child(j)->child(k)->childCount();l++)
					{
						m_toplevelItem->child(i)->child(j)->child(k)->child(l)->setCheckState(0,m_toplevelItem->child(i)->child(j)->child(k)->checkState(0));
					}
				}
			}

			//遍历某些二级的孩子
			for (int j=0;j<2;j++)
			{
				for (int k=0;k<m_calcuItem[j]->childCount();k++)
				{
					if (!m_calcuItem[j]->child(k)->isDisabled())
					{

						m_calcuItem[j]->child(k)->setCheckState(0,m_calcuItem[j]->checkState(0));
					}
				}
			}


		}
	}else
	{		
		for (int i=0;i<m_toplevelItem->childCount();i++)
		{
			if (curitem==m_toplevelItem->child(i))
			{
				for (int j=0;j<m_toplevelItem->child(i)->childCount();j++)
				{
					if (!m_toplevelItem->child(i)->child(j)->isDisabled())
					{
						m_toplevelItem->child(i)->child(j)->setCheckState(0,m_toplevelItem->child(i)->checkState(0));
					}

					for (int k=0;k<m_toplevelItem->child(i)->child(j)->childCount();k++)
					{
						if (!m_toplevelItem->child(i)->child(j)->child(k)->isDisabled())
						{

							m_toplevelItem->child(i)->child(j)->child(k)->setCheckState(0,m_toplevelItem->child(i)->child(j)->checkState(0));
						}
						for(int l=0;l<m_toplevelItem->child(i)->child(j)->child(k)->childCount();l++)
						{
							m_toplevelItem->child(i)->child(j)->child(k)->child(l)->setCheckState(0,m_toplevelItem->child(i)->child(j)->child(k)->checkState(0));
						}
					}
				}

			}

			for (int j=0;j<m_toplevelItem->child(i)->childCount();j++)
			{
				if (curitem==m_toplevelItem->child(i)->child(j))
				{
					for (int k=0;k<m_toplevelItem->child(i)->child(j)->childCount();k++)
					{
						if (!m_toplevelItem->child(i)->child(j)->child(k)->isDisabled())
						{

							m_toplevelItem->child(i)->child(j)->child(k)->setCheckState(0,m_toplevelItem->child(i)->child(j)->checkState(0));
						}

						for(int l=0;l<m_toplevelItem->child(i)->child(j)->child(k)->childCount();l++)
						{
							m_toplevelItem->child(i)->child(j)->child(k)->child(l)->setCheckState(0,m_toplevelItem->child(i)->child(j)->child(k)->checkState(0));
						}
					}

				}
				for (int k=0;k<m_toplevelItem->child(i)->child(j)->childCount();k++)
				{
					if (curitem==m_toplevelItem->child(i)->child(j)->child(k))
					{
						for(int l=0;l<m_toplevelItem->child(i)->child(j)->child(k)->childCount();l++)
						{
							m_toplevelItem->child(i)->child(j)->child(k)->child(l)->setCheckState(0,m_toplevelItem->child(i)->child(j)->child(k)->checkState(0));
						}
					}
				}
			}

		}


	}

	m_filetree->blockSignals(false);
}
void CcheckWin::createFileTree()
{
	m_filetree->headerItem()->setText(0, tr("Check File"));
	QStringList stringList(tr("Select All"));
	m_toplevelItem =new QTreeWidgetItem(stringList);
	m_toplevelItem->setCheckState(0,Qt::Unchecked);
	m_filetree->addTopLevelItem(m_toplevelItem);
	for(int i=0;i<mainWincfg.fileTypeName.count();i++)  //创建一级树
	{
		QStringList stringList(mainWincfg.fileTypeName.at(i).trimmed());
		m_hFileType[i]=new QTreeWidgetItem(stringList,i);
		m_hFileType[i]->setCheckState(0,Qt::Unchecked);

		if (i==FTCalcu)
		{
			m_calcuItem[0] =new QTreeWidgetItem(stringList);
			m_calcuItem[0]->setCheckState(0,Qt::Unchecked);

			QStringList stringList(mainWincfg.fileTypeName.at(i).trimmed()+"1");
			m_calcuItem[1] =new QTreeWidgetItem(stringList);
			m_calcuItem[1]->setCheckState(0,Qt::Unchecked);

			m_hFileType[i]->addChild(m_calcuItem[0]);
			m_hFileType[i]->addChild(m_calcuItem[1]);


		}
			//加载画面文件
		if (i==FTDsp)
		{
			for (int j=0;j<pCheckDbin->fileTypeNamelist.count();j++)
			{
				QStringList stringList(pCheckDbin->fileTypeNamelist.at(j).filetype.trimmed());
				QTreeWidgetItem *tmpitem=new QTreeWidgetItem(stringList,j);
				tmpitem->setCheckState(0,Qt::Unchecked);
				m_hFileType[i]->addChild(tmpitem);

				for (int k=0;k<pCheckDbin->fileTypeNamelist.at(j).filenamelist.count();k++)
				{
					//qDebug()<<"pCheckDbin->fileTypeNamelist.at(j).filenamelist.at(k)=="<<pCheckDbin->fileTypeNamelist.at(j).filenamelist.at(k);
					QStringList stringList1(pCheckDbin->fileTypeNamelist.at(j).filenamelist.at(k).trimmed());
					QTreeWidgetItem *tmpitem1=new QTreeWidgetItem(stringList1,k);
					tmpitem1->setCheckState(0,Qt::Unchecked);
					tmpitem->addChild(tmpitem1);


				}


			}

		}

		if (i==FTCommu)
		{
			QString tmptype=NULL;
			QString cc_nd=NULL;

			for (int j=0;j<dcomcfg.dcomfilelist.count();j++)
			{
				if (tmptype.operator!=(dcomcfg.dcomfilelist.at(j).filetype.at(0)))
				{

					tmptype=dcomcfg.dcomfilelist.at(j).filetype.at(0);
					QStringList stringList(tmptype);
					QTreeWidgetItem *tmpitem=new QTreeWidgetItem(stringList,j);
					tmpitem->setCheckState(0,Qt::Unchecked);

					if (tmptype=="iec101"||tmptype=="iec104")
					{
						QStringList cc_ndList0("CC");
						QTreeWidgetItem *tmpitem0=new QTreeWidgetItem(cc_ndList0,0);
						tmpitem0->setCheckState(0,Qt::Unchecked);

						QStringList cc_ndList1("ND");
						QTreeWidgetItem *tmpitem1=new QTreeWidgetItem(cc_ndList1,1);
						tmpitem1->setCheckState(0,Qt::Unchecked);

						tmpitem->addChild(tmpitem0);
						tmpitem->addChild(tmpitem1);

					}
					m_hFileType[i]->addChild(tmpitem);
				}


			}
		}

		m_toplevelItem->addChild(m_hFileType[i]);
	}
	connect(m_filetree, SIGNAL(itemChanged( QTreeWidgetItem * , int )),
		this, SLOT(OntoplevelItemCheckChanged(QTreeWidgetItem * , int)));

	m_toplevelItem->setExpanded(true);

	//加载闭锁库文件
	for (int i=0;i<pCheckInterlock->filenamelist.count();i++)
	{
		QStringList stringList(pCheckInterlock->filenamelist.at(i).filename);
		QTreeWidgetItem *tmpItem=new QTreeWidgetItem(stringList,i);
		tmpItem->setCheckState(0,Qt::Unchecked);
		if (!pCheckInterlock->filenamelist.at(i).validflag)
		{
			tmpItem->setDisabled(true);
		}
		m_hFileType[0]->addChild(tmpItem);
	}

	//加载计算库文件calcu和calcu1
	for (int k=0;k<2;k++)
	{
		for (int i=0;i<pCheckCalcu->filenamelist[k].count();i++)
		{
			QStringList stringList(pCheckCalcu->filenamelist[k].at(i).filename);
			QTreeWidgetItem *tmpItem=new QTreeWidgetItem(stringList,i);
			tmpItem->setCheckState(0,Qt::Unchecked);
			if (!pCheckCalcu->filenamelist[k].at(i).validflag)
			{
				tmpItem->setDisabled(true);
			}
			//m_hFileType[1]->addChild(tmpItem);
			m_calcuItem[k]->addChild(tmpItem);
		}
	}



	//加载温度
	for (int i=0;i<pCheckTempeigen->filenamelist.count();i++)
	{
		QStringList stringList(pCheckTempeigen->filenamelist.at(i).filename);
		QTreeWidgetItem *tmpItem=new QTreeWidgetItem(stringList,i);
		tmpItem->setCheckState(0,Qt::Unchecked);

		m_hFileType[FTTemp]->addChild(tmpItem);
	}

	//加载水循环
	for (int i=0;i<pCheckWaterRevert->filenamelist.count();i++)
	{
		QStringList stringList(pCheckWaterRevert->filenamelist.at(i).filename);
		QTreeWidgetItem *tmpItem=new QTreeWidgetItem(stringList,i);
		tmpItem->setCheckState(0,Qt::Unchecked);

		m_hFileType[FTWater]->addChild(tmpItem);
	}

	//加载通信点表iccp
	for (int i=0;i<pCheckDcom->iccpfilelist.count();i++)
	{
		QStringList stringList(pCheckDcom->iccpfilelist.at(i).filename);
		QTreeWidgetItem *tmpItem=new QTreeWidgetItem(stringList,i);
		tmpItem->setCheckState(0,Qt::Unchecked);
		m_hFileType[FTCommu]->child(0)->addChild( tmpItem);
	}
	//加载通信点表iec101
	Dcomfile tmpDcomfile;
	for (int i=0;i<pCheckDcom->iec101filelist.count();i++)
	{
		QStringList stringList(pCheckDcom->iec101filelist.at(i).filename);
		QTreeWidgetItem *tmpItem=new QTreeWidgetItem(stringList,i);
		tmpItem->setCheckState(0,Qt::Unchecked);

		pCheckDcom->getDcomfileInfo(pCheckDcom->iec101filelist.at(i).filename,&tmpDcomfile);
		if (tmpDcomfile.filetype.at(1)=="CC")
		{
			m_hFileType[FTCommu]->child(1)->child(0)->addChild( tmpItem);
		}else
			if (tmpDcomfile.filetype.at(1)=="ND")
			{
				m_hFileType[FTCommu]->child(1)->child(1)->addChild( tmpItem);
			}

	}
	//加载通信点表iec104

	for (int i=0;i<pCheckDcom->iec104filelist.count();i++)
	{
		QStringList stringList(pCheckDcom->iec104filelist.at(i).filename);
		QTreeWidgetItem *tmpItem=new QTreeWidgetItem(stringList,i);
		tmpItem->setCheckState(0,Qt::Unchecked);

		pCheckDcom->getDcomfileInfo(pCheckDcom->iec104filelist.at(i).filename,&tmpDcomfile);
		if (tmpDcomfile.filetype.at(1)=="CC")
		{
			m_hFileType[FTCommu]->child(2)->child(0)->addChild( tmpItem);
		}else
			if (tmpDcomfile.filetype.at(1)=="ND")
			{
				m_hFileType[FTCommu]->child(2)->child(1)->addChild( tmpItem);
			}

	}
}

void CcheckWin::createActions()
{
	saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(Onsave()));

	cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
	cutAct->setShortcuts(QKeySequence::Cut);
	cutAct->setStatusTip(tr("Cut the current selection's contents to the "
		"clipboard"));
	connect(cutAct, SIGNAL(triggered()), m_sourceDefEdit, SLOT(cut()));

	copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
	copyAct->setShortcuts(QKeySequence::Copy);
	copyAct->setStatusTip(tr("Copy the current selection's contents to the "
		"clipboard"));
	connect(copyAct, SIGNAL(triggered()), m_sourceDefEdit, SLOT(copy()));

	pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
	pasteAct->setShortcuts(QKeySequence::Paste);
	pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
		"selection"));
	connect(pasteAct, SIGNAL(triggered()), m_sourceDefEdit, SLOT(paste()));
}
void CcheckWin::creatToolbar()
{


	editToolBar = m_parent->addToolBar(tr("Edit"));
	editToolBar->addAction(saveAct);
	editToolBar->addAction(cutAct);
	editToolBar->addAction(copyAct);
	editToolBar->addAction(pasteAct);
}
void CcheckWin::createMainView()
{
	splitter1 = new QSplitter(Qt::Vertical,this); 
	splitter2 = new QSplitter(Qt::Vertical,this);

	groupbox=new QGroupBox(tr("Option"),this);

	labelCurline=new QLabel(tr("Current Line:"));
	columnName=new QLineEdit(tr("Line Number"));
	columnName->setReadOnly(true);
	columnValue=new QLineEdit();
	columnValue->setReadOnly(true);

	//labelOrder=new QLabel(tr("Filed OrderBy:"));
	//m_indexfield1=new QComboBox();
	//m_indexfield2=new QComboBox();
	//labelOrder->setBuddy(m_indexfield1);
	//QHBoxLayout *layout = new QHBoxLayout;
	//layout->addWidget(labelOrder);
	//layout->addWidget(m_indexfield1);
	//layout->addWidget(m_indexfield2);

	sortCaseSensitivityCheckBox = new QCheckBox(tr("Case sensitive sorting"));
	filterCaseSensitivityCheckBox = new QCheckBox(tr("Case sensitive filter"));
	ModifyUpdateCheckBox = new QCheckBox(tr("Modify Update"));


	filterPatternLineEdit = new QLineEdit;
	filterPatternLabel = new QLabel(tr("&Filter pattern:"));
	startfilterbtn=new QPushButton(tr("Start Filter"));


	filterSyntaxComboBox = new QComboBox;
	filterSyntaxComboBox->addItem(tr("Regular expression"), QRegExp::RegExp);
	filterSyntaxComboBox->addItem(tr("Wildcard"), QRegExp::Wildcard);
	filterSyntaxComboBox->addItem(tr("Fixed string"), QRegExp::FixedString);
	filterSyntaxLabel = new QLabel(tr("Filter &syntax:"));
	filterSyntaxLabel->setBuddy(filterSyntaxComboBox);

	filterColumnComboBox = new QComboBox;
	filterColumnLabel = new QLabel(tr("Filter &column:"));
	filterColumnLabel->setBuddy(filterColumnComboBox);


	if(filterColumnComboBox)
	{
		if(filterColumnComboBox->count()>0)filterColumnComboBox->clear( );

	}

	QGridLayout *proxyLayout = new QGridLayout;
	proxyLayout->addWidget(labelCurline, 0, 0 );
	proxyLayout->addWidget(columnName, 0, 1 );
	proxyLayout->addWidget(columnValue, 0, 2);

	//proxyLayout->addWidget(labelOrder, 1, 0 );
	//proxyLayout->addWidget(m_indexfield1, 1, 1 );
	//proxyLayout->addWidget(m_indexfield2, 1, 2);

	proxyLayout->addWidget(filterPatternLabel, 2, 0);
	proxyLayout->addWidget(filterPatternLineEdit, 2, 1,1,2);
	proxyLayout->addWidget(startfilterbtn,2,2,1,2);
	proxyLayout->addWidget(filterSyntaxLabel, 3, 0);
	proxyLayout->addWidget(filterSyntaxComboBox, 3, 1,1,2);
	proxyLayout->addWidget(filterColumnLabel, 4, 0);
	proxyLayout->addWidget(filterColumnComboBox, 4, 1,1,2);

	//	proxyLayout->addWidget(filterCaseSensitivityCheckBox, 5, 0);
	//	proxyLayout->addWidget(sortCaseSensitivityCheckBox, 5, 1,1,4);
	//	proxyLayout->addWidget(ModifyUpdateCheckBox,5,2);



	connect(startfilterbtn, SIGNAL(pressed()),
		this, SLOT(OnfilterRegExpChanged()));
	connect(filterSyntaxComboBox, SIGNAL(currentIndexChanged(int)),
		this, SLOT(OnfilterRegExpChanged()));
	connect(filterColumnComboBox, SIGNAL(currentIndexChanged(int)),
		this, SLOT(OnfilterColumnChanged()));
	connect(filterCaseSensitivityCheckBox, SIGNAL(toggled(bool)),
		this, SLOT(OnfilterRegExpChanged()));
	connect(sortCaseSensitivityCheckBox, SIGNAL(toggled(bool)),
		this, SLOT(OnsortChanged()));
	connect(ModifyUpdateCheckBox, SIGNAL(toggled(bool)),
		this, SLOT(OnModifyUpdatestateChanged()));
	groupbox->setLayout(proxyLayout);

	//	groupbox->hide();

	setStretchFactor(0, 1);
	setStretchFactor(1, 3);	

	m_filetree = new QTreeWidget;
	m_paralist1 =new QTableView;
	m_paralist2 =new QTableView;
	m_sourceDefEdit =new QTextEdit;
	connect(m_sourceDefEdit,SIGNAL(selectionChanged( )),this,SLOT(OnmousePressEdit()));


	leftgroupbox=new QGroupBox;  //确定取消
	btnCheck=new QPushButton(tr("Check"));
	btnCancel=new QPushButton(tr("Cancel"));
	connect(btnCheck, SIGNAL(pressed ()),
		this, SLOT(OnpushCheckbtn()));


	QHBoxLayout *layout1 = new QHBoxLayout;
	layout1->addWidget(btnCheck);
	layout1->addWidget(btnCancel);
	leftgroupbox->setLayout(layout1);

	splitter1->addWidget(m_filetree);
	splitter1->addWidget(leftgroupbox);

	splitter2->addWidget(m_paralist1);
	splitter2->addWidget(m_sourceDefEdit);
	splitter2->addWidget(groupbox);
	splitter1->setStretchFactor(0, 1);
	splitter1->setStretchFactor(1, 1);
	splitter2->setStretchFactor(0, 1);
	splitter2->setStretchFactor(1, 1);

	proxyModel = new QSortFilterProxyModel;
	//	proxyModel->setDynamicSortFilter(true); //
	proxyModel->setSourceModel(&model1);
	m_paralist2->setModel(proxyModel);
	m_paralist2->setSortingEnabled(true);
	m_paralist2->setVisible(false);


	//	m_paralist1->setModel(&model1);
	m_paralist1->setModel(proxyModel); //调试成功后使用proxyModel
	m_paralist1->setSortingEnabled(true);

	//m_paralist1->setEnabled(false);
	m_paralist1->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_paralist2->setSelectionBehavior(QAbstractItemView::SelectRows);


	paralist1_header = m_paralist1->horizontalHeader();
	//SortIndicator为水平标题栏文字旁边的三角指示器
	//	paralist1_header->setSortIndicator(0, Qt::AscendingOrder);
	paralist1_header->setSortIndicatorShown(true);
	paralist1_header->setClickable(true);
	connect(paralist1_header, SIGNAL(sectionClicked(int)), m_paralist1, SLOT (sortByColumn(int)));
	connect(paralist1_header, SIGNAL(sectionPressed(int)), m_paralist1, SLOT (sortByColumn(int)));

	connect(m_paralist1,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(Onparalist1doubleClick(const QModelIndex &)));


	createFileTree();
}
void CcheckWin::sortByColumn()
{
	//m_paralist1->resizeColumnsToContents(); //填充后自动根据内容调整列宽
}
void CcheckWin::OnmousePressEdit()
{

	QTextCursor cursor = m_sourceDefEdit->textCursor();
	int linenum=cursor.blockNumber()+1;
	QString strnumber = QString("%1").arg(linenum);
	columnValue->setText(strnumber);

}
void CcheckWin::OnfilterRegExpChanged()
{
	QRegExp::PatternSyntax syntax =
		QRegExp::PatternSyntax(filterSyntaxComboBox->itemData(
		filterSyntaxComboBox->currentIndex()).toInt());
	Qt::CaseSensitivity caseSensitivity =
		filterCaseSensitivityCheckBox->isChecked() ? Qt::CaseSensitive
		: Qt::CaseInsensitive;
	QRegExp regExp((filterPatternLineEdit->text()), caseSensitivity, syntax);;
	proxyModel->setFilterRegExp(regExp);
}
void CcheckWin::OnfilterColumnChanged()
{
	proxyModel->setFilterKeyColumn(filterColumnComboBox->currentIndex());
}
void CcheckWin::OnsortChanged()
{

}
void CcheckWin::OnModifyUpdatestateChanged()
{

}

void CcheckWin::OnpushCheckbtn()
{

	setCursor(Qt::WaitCursor);
	getAllSelectFile();
	pCheckInterlock->checkAllIntlkfile();

	pCheckCalcu->checkAllCalcufile();

	pCheckDbin->checkAllDbinfile();

	pCheckTempeigen->checkAllTempfile();

	pCheckWaterRevert->checkAllWaterfile();

	pCheckDcom->checkAllDcomfile();
	DispTable();
	setCursor(Qt::ArrowCursor);

}

void CcheckWin::OnpushCancelbtn()
{

}
bool CcheckWin::Onsave()
{
	return saveFile(cursourceDefName);
}
bool CcheckWin::saveFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("cfgmgr"),
			tr("Cannot write file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return false;
	}

	QTextStream out(&file);
	out << m_sourceDefEdit->toPlainText();

	m_parent->statusBar()->showMessage(fileName+tr(" File saved"), 4000);
	return true;
}


void CcheckWin::Onparalist1doubleClick(const QModelIndex &Index)
{
	QString fileName;
	int fileLine;

	int selectRow=Index.row();
	QModelIndex tmpindex = proxyModel->index(selectRow, FILENAME, QModelIndex());//proxyModel->index(recordnotmp, columnnotmp, QModelIndex());//
	QVariant vData=proxyModel->data(tmpindex);
	fileName=vData.toString().trimmed();

	tmpindex = proxyModel->index(selectRow, LINE, QModelIndex());//proxyModel->index(recordnotmp, columnnotmp, QModelIndex());//
	vData=proxyModel->data(tmpindex);
	fileLine=vData.toInt();

	//打开文件
	if (cursourceDefName!=fileName)
	{
		if (cursourceDefName.size()>0)
		{
			psouceDefFile->close();
			delete psouceDefFile;
			psouceDefFile=NULL;
		}

		cursourceDefName=fileName;

		psouceDefFile=new QFile(fileName);
		if (!psouceDefFile->open(QFile::ReadWrite | QFile::Text)) {
			QMessageBox::warning(this, tr("cfgmgr"),
				tr("Cannot read file %1:\n%2.")
				.arg(fileName)
				.arg(psouceDefFile->errorString()));
			return;
		}
		QTextStream in(psouceDefFile);

		textEditmaybeSave();
		m_sourceDefEdit->setPlainText(in.readAll());
		m_sourceDefEdit->setDocumentTitle(cursourceDefName);

		QApplication::restoreOverrideCursor();
	}


	//设置光标位置
	m_sourceDefEdit->setFocus();
	QTextCursor cursor = m_sourceDefEdit->textCursor();
	cursor.setPosition ( 0, QTextCursor::MoveAnchor );//移到第0行
	cursor.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,fileLine-1); //向下移到n行
	m_sourceDefEdit->setTextCursor(cursor);


}
void CcheckWin::CreateTableHead()//创建表头
{
	model1.clear();
	model1.setColumnCount(TABLECOLUMNUM);
	//if (GetOrderFieldIndex(tablename)>-1)
	//	{
	//		paralist1_header->setSortIndicator(GetOrderFieldIndex(tablename), Qt::AscendingOrder);
	//	}else
	{
		paralist1_header->setSortIndicator(2, Qt::DescendingOrder);
	}

	for (int i=0;i<TABLECOLUMNUM;i++)
	{
		model1.setHeaderData(i,Qt::Horizontal,TABLECOLUMNAME[i]);
	}


	if(filterColumnComboBox->count()>0)filterColumnComboBox->clear( );
	for (int i=0;i<TABLECOLUMNUM;i++)
	{
		filterColumnComboBox->addItem(TABLECOLUMNAME[i]);
		m_paralist1->setColumnWidth(i,TABLECOLUMWIDTH[i]);
	}
}
void CcheckWin::DispTableRecord()
{
	textEditmaybeSave();
	//model1.setRowCount(100);
	QFile *InterlockErrFile=new QFile(interlockcfg.errfile);
	if (!InterlockErrFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<interlockcfg.errfile<<" error";
		InterlockErrFile->close();
		return;
	}

	QFile *CalcuErrFile=new QFile(calcucfg.errfile);
	if (!CalcuErrFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<calcucfg.errfile<<" error";
		CalcuErrFile->close();
		return;
	}

	QFile *DbinErrFile=new QFile(dbincfg.errfile);
	if (!DbinErrFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<dbincfg.errfile<<" error";
		DbinErrFile->close();
		return;
	}

	QFile *TempErrFile=new QFile(tempcfg.errfile);
	if (!TempErrFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<tempcfg.errfile<<" error";
		TempErrFile->close();
		return;
	}

	QFile *WaterErrFile=new QFile(watercfg.errfile);
	if (!WaterErrFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<watercfg.errfile<<" error";
		WaterErrFile->close();
		return;
	}

	QFile *DcomErrFile=new QFile(dcomcfg.errfile);
	if (!DcomErrFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<dcomcfg.errfile<<" error";
		DcomErrFile->close();
		return;
	}
	QTextStream errInterLockStream1(InterlockErrFile);
	QTextStream errCalcuStream1(CalcuErrFile);
	QTextStream errDbinStream1(DbinErrFile);
	QTextStream errTempStream1(TempErrFile);
	QTextStream errWaterStream1(WaterErrFile);
	QTextStream errDcomStream1(DcomErrFile);


	long modelinenum=0;
	while (!errInterLockStream1.atEnd()){
		errInterLockStream1.readLine();
		modelinenum++;
		//qDebug()<<"modelinenum=="<<modelinenum;

	}
	while (!errCalcuStream1.atEnd()){
		errCalcuStream1.readLine();
		modelinenum++;
		//qDebug()<<"modelinenum=="<<modelinenum;

	}
	while (!errDbinStream1.atEnd()){
		errDbinStream1.readLine();
		modelinenum++;
		//qDebug()<<"modelinenum=="<<modelinenum;

	}
	while (!errTempStream1.atEnd()){
		errTempStream1.readLine();
		modelinenum++;
		//qDebug()<<"modelinenum=="<<modelinenum;

	}
	while (!errWaterStream1.atEnd()){
		errWaterStream1.readLine();
		modelinenum++;
		//qDebug()<<"modelinenum=="<<modelinenum;

	}

	while (!errDcomStream1.atEnd()){
		errDcomStream1.readLine();
		modelinenum++;
		//qDebug()<<"modelinenum=="<<modelinenum;

	}

	model1.setRowCount(modelinenum);

	InterlockErrFile->close();
	CalcuErrFile->close();
	DbinErrFile->close();
	TempErrFile->close();
	WaterErrFile->close();
	DcomErrFile->close();


	if (!InterlockErrFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<interlockcfg.errfile<<" error";
		InterlockErrFile->close();
		return;
	}

	if (!CalcuErrFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<calcucfg.errfile<<" error";
		CalcuErrFile->close();
		return;
	}
	if (!DbinErrFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<dbincfg.errfile<<" error";
		DbinErrFile->close();
		return;
	}

	if (!TempErrFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<tempcfg.errfile<<" error";
		TempErrFile->close();
		return;
	}

	if (!WaterErrFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<watercfg.errfile<<" error";
		WaterErrFile->close();
		return;
	}

	if (!DcomErrFile->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"open "<<dcomcfg.errfile<<" error";
		DcomErrFile->close();
		return;
	}
	QTextStream errInterLockStream(InterlockErrFile);
	QTextStream errCalcuStream(CalcuErrFile);
	QTextStream errDbinStream(DbinErrFile);
	QTextStream errTempStream(TempErrFile);
	QTextStream errWaterStream(WaterErrFile);
	QTextStream errDcomStream(DcomErrFile);



	int linenum=0;
	while (!errInterLockStream.atEnd()) {
		QString line =errInterLockStream.readLine();

		QStringList tmplist=line.split(QRegExp("(\\t\)"));
		//qDebug()<<"tmplist="<<tmplist.count()<<tmplist;
		for (int colum=0;colum<TABLECOLUMNUM;colum++)
		{			
			QModelIndex index = model1.index(linenum, colum, QModelIndex());
			if (colum==2)
			{
				model1.setData(index, QVariant((int)tmplist.at(colum).toInt()));
			}else{
				model1.setData(index, QVariant(tmplist.at(colum)));
			}

		}
		linenum++;
	}
	while (!errCalcuStream.atEnd()) {
		QString line =errCalcuStream.readLine();

		QStringList tmplist=line.split(QRegExp("(\\t\)"));
		//qDebug()<<"tmplist="<<tmplist.count()<<tmplist;
		for (int colum=0;colum<TABLECOLUMNUM;colum++)
		{			
			QModelIndex index = model1.index(linenum, colum, QModelIndex());
			if (colum==2)
			{
				model1.setData(index, QVariant((int)tmplist.at(colum).toInt()));
			}else{
				model1.setData(index, QVariant(tmplist.at(colum)));
			}

		}
		linenum++;
	}
	while (!errDbinStream.atEnd()) {
		QString line =errDbinStream.readLine();

		QStringList tmplist=line.split(QRegExp("(\\t\)"));
		//qDebug()<<"tmplist="<<tmplist.count()<<tmplist;
		for (int colum=0;colum<TABLECOLUMNUM;colum++)
		{			
			QModelIndex index = model1.index(linenum, colum, QModelIndex());
			if (colum==2)
			{
				model1.setData(index, QVariant((int)tmplist.at(colum).toInt()));
			}else{
				model1.setData(index, QVariant(tmplist.at(colum)));
			}

		}
		linenum++;
	}

	while (!errTempStream.atEnd()) {
		QString line =errTempStream.readLine();

		QStringList tmplist=line.split(QRegExp("(\\t\)"));
		//qDebug()<<"tmplist="<<tmplist.count()<<tmplist;
		for (int colum=0;colum<TABLECOLUMNUM;colum++)
		{			
			QModelIndex index = model1.index(linenum, colum, QModelIndex());
			if (colum==2)
			{
				model1.setData(index, QVariant((int)tmplist.at(colum).toInt()));
			}else{
				model1.setData(index, QVariant(tmplist.at(colum)));
			}

		}
		linenum++;
	}

	while (!errWaterStream.atEnd()) {
		QString line =errWaterStream.readLine();

		QStringList tmplist=line.split(QRegExp("(\\t\)"));
		//qDebug()<<"tmplist="<<tmplist.count()<<tmplist;
		for (int colum=0;colum<TABLECOLUMNUM;colum++)
		{			
			QModelIndex index = model1.index(linenum, colum, QModelIndex());
			if (colum==2)
			{
				model1.setData(index, QVariant((int)tmplist.at(colum).toInt()));
			}else{
				model1.setData(index, QVariant(tmplist.at(colum)));
			}

		}
		linenum++;
	}

	while (!errDcomStream.atEnd()) {
		QString line =errDcomStream.readLine();

		QStringList tmplist=line.split(QRegExp("(\\t\)"));
		//qDebug()<<"tmplist="<<tmplist.count()<<tmplist;
		for (int colum=0;colum<TABLECOLUMNUM;colum++)
		{			
			QModelIndex index = model1.index(linenum, colum, QModelIndex());
			if (colum==2)
			{
				model1.setData(index, QVariant((int)tmplist.at(colum).toInt()));
			}else{
				model1.setData(index, QVariant(tmplist.at(colum)));
			}

		}
		linenum++;
	}

	InterlockErrFile->close();
	CalcuErrFile->close();
	DbinErrFile->close();
	TempErrFile->close();
	WaterErrFile->close();
	DcomErrFile->close();
	//m_paralist1->resizeColumnsToContents(); //填充后自动根据内容调整列宽
}

void CcheckWin::DispTable()
{
	CreateTableHead();
	DispTableRecord();
}
void CcheckWin::clearAllSelectFile()
{
	for (int i=0;i<pCheckInterlock->filenamelist.count();i++)
	{

		FilePoint tpm =pCheckInterlock->filenamelist.at(i);
		tpm.selectflag=0;
		pCheckInterlock->filenamelist.removeAt(i);
		pCheckInterlock->filenamelist.insert(i,tpm);

	}

	for (int k=0;k<2;k++)
	{
		for (int i=0;i<pCheckCalcu->filenamelist[k].count();i++)
		{
			//		qDebug()<<pCheckCalcu->filenamelist[k].at(i).filename<<pCheckCalcu->filenamelist[k].at(i).selectflag;//=1;

			FilePoint tpm =pCheckCalcu->filenamelist[k].at(i);
			tpm.selectflag=0;
			pCheckCalcu->filenamelist[k].removeAt(i);
			pCheckCalcu->filenamelist[k].insert(i,tpm);

			//		qDebug()<<pCheckCalcu->filenamelist[k].at(i).filename<<pCheckCalcu->filenamelist[k].at(i).selectflag;//=1;

		}
	}

	for (int i=0;i<pCheckTempeigen->filenamelist.count();i++)
	{

		FilePoint tpm =pCheckTempeigen->filenamelist.at(i);
		tpm.selectflag=0;
		pCheckTempeigen->filenamelist.removeAt(i);
		pCheckTempeigen->filenamelist.insert(i,tpm);

	}

	for (int i=0;i<pCheckWaterRevert->filenamelist.count();i++)
	{

		FilePoint tpm =pCheckWaterRevert->filenamelist.at(i);
		tpm.selectflag=0;
		pCheckWaterRevert->filenamelist.removeAt(i);
		pCheckWaterRevert->filenamelist.insert(i,tpm);

	}

	for (int j=0;j<pCheckDbin->fileTypeNamelist.count();j++)
	{
		for (int k=0;k<pCheckDbin->fileTypeNamelist.at(j).filenamelist.count();k++)
		{
				//qDebug()<<"===clear pCheckDbin->fileTypeNamelist.at(j).filepointnamelist.at(k).filename=="<<pCheckDbin->fileTypeNamelist.at(j).filepointnamelist.at(k).filename<<pCheckDbin->fileTypeNamelist.at(j).filepointnamelist.at(k).selectflag;
				Type_Name tpmtypename =pCheckDbin->fileTypeNamelist.at(j);

				FilePoint tpmfilepoint=tpmtypename.filepointnamelist.at(k);
				tpmfilepoint.selectflag=0;

				tpmtypename.filepointnamelist.removeAt(k);
				tpmtypename.filepointnamelist.insert(k,tpmfilepoint);


				pCheckDbin->fileTypeNamelist.removeAt(j);
				pCheckDbin->fileTypeNamelist.insert(j,tpmtypename);

				//qDebug()<<"===clear pCheckDbin->fileTypeNamelist.at(j).filepointnamelist.at(k).filename=="<<pCheckDbin->fileTypeNamelist.at(j).filepointnamelist.at(k).filename<<pCheckDbin->fileTypeNamelist.at(j).filepointnamelist.at(k).selectflag;

		}

	}
	//通信点表
	for (int i=0;i<pCheckDcom->iccpfilelist.count();i++)
	{
		FilePoint tpm =pCheckDcom->iccpfilelist.at(i);
		tpm.selectflag=0;
		pCheckDcom->iccpfilelist.removeAt(i);
		pCheckDcom->iccpfilelist.insert(i,tpm);

	}
	for (int i=0;i<pCheckDcom->iec101filelist.count();i++)
	{
		FilePoint tpm =pCheckDcom->iec101filelist.at(i);
		tpm.selectflag=0;
		pCheckDcom->iec101filelist.removeAt(i);
		pCheckDcom->iec101filelist.insert(i,tpm);

	}
	for (int i=0;i<pCheckDcom->iec104filelist.count();i++)
	{
		FilePoint tpm =pCheckDcom->iec104filelist.at(i);
		tpm.selectflag=0;
		pCheckDcom->iec104filelist.removeAt(i);
		pCheckDcom->iec104filelist.insert(i,tpm);

	}


}
void CcheckWin::getAllSelectFile()
{

	clearAllSelectFile();

	for (int i=0;i<FILETYPENUM;i++)
	{
		switch (i)
		{
		case FTInterLock :  
			for (int j=0;j<m_hFileType[i]->childCount();j++)
			{
				if (m_hFileType[i]->child(j)->checkState(0)==Qt::Checked)
				{
					FilePoint tpm =pCheckInterlock->filenamelist.at(j);
					tpm.selectflag=1;
					pCheckInterlock->filenamelist.removeAt(j);
					pCheckInterlock->filenamelist.insert(j,tpm);
				}

			}

			break;
		case FTCalcu:
			{

				for (int k=0;k<2;k++)
				{
					for (int j=0;j<m_calcuItem[k]->childCount();j++)
					{
						if (m_calcuItem[k]->child(j)->checkState(0)==Qt::Checked)
						{

							FilePoint tpm =pCheckCalcu->filenamelist[k].at(j);
							tpm.selectflag=1;
							//QList<FilePoint> ::iterator ij;
							pCheckCalcu->filenamelist[k].removeAt(j);
							pCheckCalcu->filenamelist[k].insert(j,tpm);
						}
					}
				}

			}
			break;
		case FTTemp:
			{
				for (int j=0;j<m_hFileType[i]->childCount();j++)
				{
					if (m_hFileType[i]->child(j)->checkState(0)==Qt::Checked)
					{
						FilePoint tpm =pCheckTempeigen->filenamelist.at(j);
						tpm.selectflag=1;
						pCheckTempeigen->filenamelist.removeAt(j);
						pCheckTempeigen->filenamelist.insert(j,tpm);
					}

				}
				break;

			}
			break;
		case FTWater:
			{
				for (int j=0;j<m_hFileType[i]->childCount();j++)
				{
					if (m_hFileType[i]->child(j)->checkState(0)==Qt::Checked)
					{
						FilePoint tpm =pCheckWaterRevert->filenamelist.at(j);
						tpm.selectflag=1;
						pCheckWaterRevert->filenamelist.removeAt(j);
						pCheckWaterRevert->filenamelist.insert(j,tpm);
					}

				}
				break;
			}
			break;
		case FTCommu:
			{

				for (int k=0;k<m_hFileType[i]->child(0)->childCount();k++)
				{
					if (m_hFileType[i]->child(0)->child(k)->checkState(0)==Qt::Checked)
					{
						//	qDebug()<<"pCheckDcom->iccpfilelist.at(tmpindex).filename=="<<pCheckDcom->iccpfilelist.at(k).filename<<pCheckDcom->iccpfilelist.at(k).selectflag;
						FilePoint tpm =pCheckDcom->iccpfilelist.at(k);
						tpm.selectflag=1;
						pCheckDcom->iccpfilelist.removeAt(k);
						pCheckDcom->iccpfilelist.insert(k,tpm);
						//	qDebug()<<"pCheckDcom->iccpfilelist.at(tmpindex).filename=="<<pCheckDcom->iccpfilelist.at(k).filename<<pCheckDcom->iccpfilelist.at(k).selectflag;
					}
				}

				//
				int tmpindex=0;
				for (int k=0;k<m_hFileType[i]->child(1)->childCount();k++)
				{
					for (int l=0;l<m_hFileType[i]->child(1)->child(k)->childCount();l++)
					{
						if (m_hFileType[i]->child(1)->child(k)->child(l)->checkState(0)==Qt::Checked)
						{
							//	qDebug()<<"pCheckDcom->iec101filelist.at(tmpindex).filename=="<<pCheckDcom->iec101filelist.at(tmpindex).filename<<pCheckDcom->iec101filelist.at(tmpindex).selectflag;
							FilePoint tpm =pCheckDcom->iec101filelist.at(tmpindex);
							tpm.selectflag=1;
							pCheckDcom->iec101filelist.removeAt(tmpindex);
							pCheckDcom->iec101filelist.insert(tmpindex,tpm);
							//	qDebug()<<"pCheckDcom->iec101filelist.at(tmpindex).filename=="<<pCheckDcom->iec101filelist.at(tmpindex).filename<<pCheckDcom->iec101filelist.at(tmpindex).selectflag;
						}
						tmpindex++;
					}
				}

				tmpindex=0;
				for (int k=0;k<m_hFileType[i]->child(2)->childCount();k++)
				{
					for (int l=0;l<m_hFileType[i]->child(2)->child(k)->childCount();l++)
					{
						if (m_hFileType[i]->child(2)->child(k)->child(l)->checkState(0)==Qt::Checked)
						{
							//	qDebug()<<"pCheckDcom->iec104filelist.at(tmpindex).filename=="<<pCheckDcom->iec104filelist.at(tmpindex).filename<<pCheckDcom->iec104filelist.at(tmpindex).selectflag;
							FilePoint tpm =pCheckDcom->iec104filelist.at(tmpindex);
							tpm.selectflag=1;
							pCheckDcom->iec104filelist.removeAt(tmpindex);
							pCheckDcom->iec104filelist.insert(tmpindex,tpm);
							//	qDebug()<<"pCheckDcom->iec104filelist.at(tmpindex).filename=="<<pCheckDcom->iec104filelist.at(tmpindex).filename<<pCheckDcom->iec104filelist.at(tmpindex).selectflag;
						}
						tmpindex++;
					}
				}



			}
			break;
		case FTDsp:
			{
				for (int j=0;j<pCheckDbin->fileTypeNamelist.count();j++)
				{
					for (int k=0;k<pCheckDbin->fileTypeNamelist.at(j).filenamelist.count();k++)
					{
						if (m_hFileType[FTDsp]->child(j)->child(k)->checkState(0)==Qt::Checked)
						{
						//	qDebug()<<"pCheckDbin->fileTypeNamelist.at(j).filepointnamelist.at(k).filename=="<<pCheckDbin->fileTypeNamelist.at(j).filepointnamelist.at(k).filename<<pCheckDbin->fileTypeNamelist.at(j).filepointnamelist.at(k).selectflag;
							Type_Name tpmtypename =pCheckDbin->fileTypeNamelist.at(j);

							FilePoint tpmfilepoint=tpmtypename.filepointnamelist.at(k);
							tpmfilepoint.selectflag=1;

							tpmtypename.filepointnamelist.removeAt(k);
							tpmtypename.filepointnamelist.insert(k,tpmfilepoint);


							pCheckDbin->fileTypeNamelist.removeAt(j);
							pCheckDbin->fileTypeNamelist.insert(j,tpmtypename);


						//	qDebug()<<"pCheckDbin->fileTypeNamelist.at(j).filepointnamelist.at(k).filename=="<<pCheckDbin->fileTypeNamelist.at(j).filepointnamelist.at(k).filename<<pCheckDbin->fileTypeNamelist.at(j).filepointnamelist.at(k).selectflag;

						}

					}

				}

			}
			break;

		}
	}
}

bool CcheckWin::textEditmaybeSave()
{
	if (m_sourceDefEdit->document()->isModified()) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("cfgmgr"),
			tr("%1 has been modified.\n Do you want to save your changes?")
			.arg(m_sourceDefEdit->documentTitle()),
			QMessageBox::Save | QMessageBox::Discard
			| QMessageBox::Cancel);
		if (ret == QMessageBox::Save)
		{

			saveFile(m_sourceDefEdit->documentTitle());
			m_sourceDefEdit->clear();
			return true;
		}
		else if (ret == QMessageBox::Cancel)
			return false;
		if (ret == QMessageBox::Discard)
		{
			m_sourceDefEdit->clear();
			return false;
		}
	}else
	{
		m_sourceDefEdit->clear();
		return false;
	}
}