#include "callipm.h"
#include <QDebug>
 #include <QFileInfo>
CallIPM::CallIPM()//´øÂ·¾¶Ãû³Æ
{
	version4_flag=1;
	m_pCalldblib=new Calldblib(DBPATH);
	m_pfilemem=NULL;
	m_filemem=NULL;
	if(-1==m_pCalldblib->MountDB())
	{
		qDebug()<<"Fail to MountDB!";
		exit(-1);
	}
}
CallIPM::~CallIPM()
{
	if (m_pCalldblib) 
	{
		delete m_pCalldblib;
		m_pCalldblib = 0;             
	}

	if (m_pfilemem)
	{
		free (m_pfilemem);
		m_pfilemem=0;
		m_filemem=NULL;
	}

}

//void CallIPM::test(QString pathname)
//{
//	
//	openDbin(pathname,m_filemem);
//}

char* CallIPM::openDbin(QString pathname)
{

	QFile tmpFile(pathname);
	if( ! tmpFile.open(QIODevice::ReadOnly ) )
	{
		// here need to be improved for more debug information 
		qDebug()<<"Open file "<<pathname<<"Failed";
		//QMessageBox().information(0,QObject::tr("CGraphClass--ReadGraphFile Func (QString)"),QObject::tr("Open file %1 Failed").arg(fname));
		return  NULL; 
	}
	QFileInfo tmpFileInfo(pathname);
	qint64 fileLen=tmpFileInfo.size();

	m_filemem =(char* ) malloc(fileLen) ;
	m_pfilemem=m_filemem;
	if(fileLen != tmpFile.read(m_filemem,fileLen))
	{
		qDebug()<<"Open file "<<pathname<<"Failed";
	}

	tmpFile.close();

	return m_filemem;


}
void	CallIPM::ReadGdbaseData ()
{
	int	i, layer, nextpos;
	char version[6] ;
	DMS_COMMON	dms_cmm;
	NEW_GBRCD		rcd;
	char *pp = m_filemem;
	NEW_GDBUFF		v4_gdbase; //new
	//	fread(version,1,6,fp);
	memcpy (version, pp, 6);
	pp +=6;

	if (strcmp("v4.0a",version)==0)    //4.0a *.dbin file data formate
	{
		version4_flag= 1;

		//fread (&v4_gdbase.nextpos, 4, 1, fp);
		memcpy (&v4_gdbase.nextpos, pp, 4);

		nextpos =v4_gdbase.nextpos;
		for (i=0; i < nextpos; i++)
		{
			v4_freadRcd ( &v4_gdbase.buffer[i]);

		}

	}

}
int	CallIPM::Openfile_DynamicsTest ()
{
	int	i, layer, nextpos;
	char version[6] ;
	DMS_COMMON	dms_cmm;
	NEW_GBRCD		rcd;
	//char *pp = m_filemem;
	//NEW_GDBUFF		v4_gdbase; //new
	//	fread(version,1,6,fp);
	memcpy (version, m_filemem, 6);
	m_filemem +=6;


	if (strcmp("v4.0a",version)==0)    //4.0a *.dbin file data formate
	{
		version4_flag= 1;

		//fread (&v4_gdbase.nextpos, 4, 1, fp);
		//memcpy (&v4_gdbase.nextpos, m_filemem, 4);
		memcpy (&nextpos, m_filemem, 4);
		
		m_filemem +=4;

	//	nextpos =v4_gdbase.nextpos;
		int j=0;
		for (i=0; i < nextpos; i++)
		{
			//v4_freadRcd ( &v4_gdbase.buffer[i]);		
			//rcd	= v4_gdbase.buffer[i];

			v4_freadRcd(&rcd);

			if( strcmp ( rcd.lnk.dbname ,"") !=0)
			{				
				if (m_pCalldblib->qReadEntryByNameStr (rcd.lnk.dbname,&dms_cmm) == -1)
				{
					j++;
					//qDebug()<<rcd.lnk.dbname<<"not in db"<<j; 

					if (m_pfilemem)
					{
						free(m_pfilemem);
						m_pfilemem=NULL;
						m_filemem=NULL;
						//free( &v4_gdbase);
					}
					return -1;
				}

			}
		}

	}
	if (m_pfilemem)
	{
		free(m_pfilemem);
		m_pfilemem=NULL;
		m_filemem=NULL;
		//free( &v4_gdbase);
	}
	return 1;

}
void	CallIPM::v4_freadRcd ( NEW_GBRCD 	*rcd)

{
	
	freadBckRcd (&(rcd->bck));
	//filemem+=sizeof(BACKRCD);
	v4_freadLnkRcd ( &(rcd->lnk));
}

void CallIPM::freadBckRcd ( BACKRCD 	*rcd)
{
	int	len;

	len     =sizeof (rcd->rect.x1);
	//fread (&rcd->rect.x1, len, 1, fp);
	memcpy (&rcd->rect.x1, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->rect.y1, len, 1, fp);
	memcpy (&rcd->rect.y1, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->rect.x2, len, 1, fp);
	memcpy (&rcd->rect.x2, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->rect.y2, len, 1, fp);
	memcpy (&rcd->rect.y2, m_filemem, len);
	m_filemem +=len;


	len     =sizeof (rcd->draw_type);
	//fread (&rcd->draw_type, len, 1, fp);
	memcpy (&rcd->draw_type, m_filemem, len);
	m_filemem +=len;

	//fread (&rcd->fill.fill_type, len, 1, fp);
	memcpy (&rcd->fill.fill_type, m_filemem, len);
	m_filemem +=len;

	//if (rcd->fill.fill_type >= 8)
	//	printf ("rcd->fill.fill_type=%d  \n" , rcd->fill.fill_type ); 
	//fread (&rcd->fg, len, 1, fp);
	memcpy (&rcd->fg, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->bg, len, 1, fp);
	memcpy (&rcd->bg, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->line_width, len, 1, fp);
	memcpy (&rcd->line_width, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->layer, len, 1, fp);
	memcpy (&rcd->layer, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->plcnt, len, 1, fp);
	memcpy (&rcd->plcnt, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->arc_type, len, 1, fp);
	memcpy (&rcd->arc_type, m_filemem, len);
	m_filemem +=len;
//	fread (&rcd->ftset_id, len, 1, fp);
	memcpy (&rcd->ftset_id, m_filemem, len);
	m_filemem +=len;

	len     =sizeof (rcd->data);
	//fread (&rcd->data, len, 1, fp);
	memcpy (&rcd->data, m_filemem, len);
	m_filemem +=len;

	/*added by wzg 20060417 for v3->v4*/
	//if(version4_flag==0)
	//{
	//	rcd->fg= ColorId3to4(rcd->fg);
	//	rcd->bg= ColorId3to4(rcd->bg);
	//}
}

void	CallIPM::v4_freadLnkRcd ( NEW_LINKRCD 	*rcd)   

{
	int	len;

	len     =sizeof (rcd->act.ctrl_type);
	//fread (&rcd->act.ctrl_type, len, 1, fp);
	memcpy (&rcd->act.ctrl_type, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->act.ctrl_tag, len, 1, fp);
	memcpy (&rcd->act.ctrl_tag, m_filemem, len);
	m_filemem +=len;

	len     = sizeof (rcd->act.fmt_def.free_area);
	//fread (rcd->act.fmt_def.free_area, len, 1, fp);
	memcpy (&rcd->act.fmt_def.free_area, m_filemem, len);
	m_filemem +=len;

	len     =sizeof (rcd->dyn.dyn_type);
	//fread (&rcd->dyn.dyn_type, len, 1, fp);
	memcpy (&rcd->dyn.dyn_type, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->dyn.dyn_tag, len, 1, fp);
	memcpy (&rcd->dyn.dyn_tag, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->dyn.dio_type, len, 1, fp);
	memcpy (&rcd->dyn.dio_type, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->dyn.f_by_off, len, 1, fp);
	memcpy (&rcd->dyn.f_by_off, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->dyn.qi_loc, len, 1, fp);
	memcpy (&rcd->dyn.qi_loc, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->dyn.tagi_loc, len, 1, fp);
	memcpy (&rcd->dyn.tagi_loc, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->dyn.data_class, len, 1, fp);
	memcpy (&rcd->dyn.data_class, m_filemem, len);
	m_filemem +=len;

	len     =sizeof (rcd->dbname);   // 48   ;
	//fread (rcd->dbname, len, 1, fp);
	memcpy (&rcd->dbname, m_filemem, len);
	m_filemem +=len;

	len     =sizeof (rcd->state);   // 48   ;
	//fread (&rcd->state, len, 1, fp);
	memcpy (&rcd->state, m_filemem, len);
	m_filemem +=len;

	len     = sizeof (rcd->dyn.fmt_def.free_area);
	//fread (rcd->dyn.fmt_def.free_area, len, 1, fp);
	memcpy (&rcd->dyn.fmt_def.free_area, m_filemem, len);
	m_filemem +=len;

	len     =sizeof (rcd->cgrp.cgrp_id);
	//fread (&rcd->cgrp.cgrp_id, len, 1, fp);
	memcpy (&rcd->cgrp.cgrp_id, m_filemem, len);
	m_filemem +=len;
	//fread (&rcd->cgrp.tag,  len, 1, fp);
	memcpy (&rcd->cgrp.tag, m_filemem, len);
	m_filemem +=len;

}
