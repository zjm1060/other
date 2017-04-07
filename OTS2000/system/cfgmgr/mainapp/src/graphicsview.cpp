

#include<QApplication>
#include<QClipboard>
#include "appwindow.h"
#include<QMouseEvent>
#include<QMessageBox>
#include"drawtool.h"
#include<QFileDialog>
#include<QMenu>
#include<QAction>
#include<QIcon>

#include"graphicsview.h"


#include"drawline.h"
#include"drawpline.h"
#include"drawrect.h"
#include"drawarc.h"
#include"drawellipse.h"
#include"drawnode.h"
#include"removeobjundo.h"
#include"dlgtest.h"
#include"dlgstyle.h"
#include"colormanager.h"
#include "basegraphclass.h"

extern CColorManager colormanager;

extern CPolygonTool polygonTool;
extern CPolyTool    polyTool;

extern int OperateFlag;
extern int m_bOpenGLFlag, m_bRealFlag, m_bPosFlag;
GraphicsView::GraphicsView(graphAppWindow *pappwin)
{
	m_graphGrid=10.0;
    m_graphGridVisible=true;
    m_graphGridScale=1;

     m_bgIndex = 61; /* black */
	 m_gridLineColorIndex = 47 ;
	 m_gridWidth = 10 ;
	 m_gridHeight = 10; 
	 m_gridLineStyle = Qt::DotLine ;


	 /* Pen default attr */
	 m_penWidth = 2;
	 m_penStyle = Qt::SolidLine;
	 m_penColorIndex = 2 ; // dark green

	 /* brush default attr */
	 m_brushColorIndex = 14; // light yellow
	 m_brushStyle = Qt::SolidPattern; 
	 m_brush.setStyle(Qt::SolidPattern);



	QPalette  palette;
	palette.setBrush(QPalette::Window,QBrush(QColor(0,0,0)));
	setAutoFillBackground(true);
	setPalette(palette);


	m_pMainWindow = 0;
	selectMode=0;
	m_pDocument=new appGraphDocument();  
	b_Polyline = 0;
	CDrawTool::b_notPtsFlag=1;
	polygonTool.m_pDrawObj=0;
	polyTool.m_pDrawObj=0;
	m_selection.clear() ;   // clear selection of current view 

	// for clipboard 
	mimeData = new QMimeData;
	clipBoardBuf = 0;
	clipBoardPtBuf=0;
	m_selGrNum=0;
	m_selPtsNum=0;
	m_polyObj=0;  // polygon or seglines num to be copy into clipboard
	m_ptsNum=0; 

	m_pSelRect=0;

	QWidget::setFocusPolicy(Qt::ClickFocus);
}


GraphicsView::~GraphicsView()
{
 if(m_pSelRect) 
 {
	 delete m_pSelRect;
	 m_pSelRect=0;
 }

 if(m_pDocument)
 {
	 delete m_pDocument;
	 m_pDocument=0;
 }
}

void GraphicsView::newFile()
{
 //  	setMouseTracking(true); 
}


void GraphicsView::AddLine()
{

 
 
}

 void GraphicsView::paintEvent(QPaintEvent *pevent)
 {
     QPainter painter(this);

	/* fill background  */ 
	QPalette  palette;
	palette.setBrush(QPalette::Window,QBrush(colormanager.m_pColor[m_bgIndex]));
	setPalette(palette);
    
	drawGrids(painter);  // draw grids on background
	
     ObjectList objs=m_pDocument->GetObjects();
	 ObjectList::iterator obj;
printf("in paintEvent:objs.begin()=%d,objs.end()=%d\n",objs.begin(),objs.end());
    for (obj = objs.begin(); obj < objs.end(); ++obj)
	{
		(*obj)->Draw(painter);
	
		if( isSelected(*obj) )
		(*obj)->drawTracker(painter,CDrawObj::selected);
	}



 }

void GraphicsView::mousePressEvent(QMouseEvent *e)
{
	QPoint ptCur = e->pos();//GetGridPoint (point);
    posx = ptCur.x();
    posy = ptCur.y();
 
	switch( e->button() )
	{
	case Qt::LeftButton :
		{
     	CDrawTool * pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	    if( pTool)
		pTool->mousePressEvent(e,this);
	    break;
		}
	case Qt::RightButton:
		{
		QMenu * tmpMenu=new QMenu("right button menu test");
        QAction *tmpAction =new QAction(QIcon(" "),tr("&New"),this );
	    tmpMenu->addAction( tmpAction);
		connect(tmpAction,SIGNAL(triggered()),this,SLOT(RightButtonFunc()));
		tmpMenu->exec(QCursor::pos());

	
	    
		if(tmpMenu) 
		{
			delete tmpMenu;
			tmpMenu=0;
		}
         
		if(tmpAction)
		{
			delete tmpAction;
			tmpAction=0;
		}
		break;
		}
	default:
		break;
		
	}
}

void GraphicsView::mouseMoveEvent(QMouseEvent *e)
{
	QPoint ptCur;
	ptCur=e->pos();
	if(OperateFlag<GrLink)
	{
		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if (pTool != NULL)
			pTool->mouseMoveEvent(e,this);
	}
	else if(OperateFlag>GrLink && OperateFlag < toolEnd && OperateFlag != GrWire)
	{
		int m_posx=posx;
		int m_posy=posy;

		ONEGR *Buf = new ONEGR;
		memset (Buf, 0, sizeof(ONEGR));
		
		CreateOneGr(Buf);

		
 		CBaseGraphClass *GrClass = new CBaseGraphClass ((char*)Buf, 1, GRAPH_TYPE_ONEGR, 100, 100, TmpFontBuf);
// 		SetGrClassInfo (this, GrClass);
// 		SetGrClassBuf (GrClass);
// 		
// 		QRect rcPos = pDoc->m_pWorkClass->GetSubGraphPosition (-1);
// 		int wWidth = pDoc->m_pWorkClass->GetGraphWidth ();
// 		int wHeight = pDoc->m_pWorkClass->GetGraphHeight ();
// 		int wMinX = pDoc->m_pWorkClass->GetGraphMinX ();
// 		int wMinY = pDoc->m_pWorkClass->GetGraphMinY ();
// 		GrClass->SetGraphLocateInfo (rcPos, wMinX, wMinY, wWidth, wHeight);
// 		
 		Buf->comm.x0 = GetSrcCoordX (m_ptLButtonDown.x());
		Buf->comm.y0 = GetSrcCoordY (m_ptLButtonDown.y());
 		Buf->comm.x1 = GetSrcCoordX (m_ptMouseOld.x());
 		Buf->comm.y1 = GetSrcCoordY (m_ptMouseOld.y());
// 		
// 		//CDC *pDC = GetDC ();
 		QPainter pDC(this);
 		GrClass->DisplayOnePixel (pDC, Buf, 0, QPainter::CompositionMode_SourceOver, 1, 2, 0);
 		
 		Buf->comm.x1 = GetSrcCoordX (m_posx);
 		Buf->comm.y1 = GetSrcCoordY (m_posy);
 		GrClass->DisplayOnePixel (pDC, Buf, 0, QPainter::CompositionMode_SourceOver, 1, 2, 0);
// 		ReleaseDC (pDC);
// 		
 		if(Buf)delete Buf;	Buf = NULL;
 		if(GrClass)delete GrClass;	GrClass = NULL;
 		
 		m_ptMouseOld.setX(m_posx)  ;
 		m_ptMouseOld.setY(m_posy)  ;







		 
		
	}
		

}

void GraphicsView::Select(CDrawObj *pObj, bool bAdd)
{
  if (!bAdd)  //bAdd==false
	{
	//	OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
	//	m_selection.RemoveAll();
		m_selection.clear();
	}

	if (pObj == 0 /*|| IsSelected(pObj)*/)
		return;

	//m_selection.AddTail(pObj);
	m_selection.append(pObj);
	InvalObj(pObj);
}

void GraphicsView::CreateOneGr(ONEGR *Buf)
{
//     CPaintDoc* pDoc = GetDocument();
// 	if (!pDoc || !(pDoc->m_pWorkClass)) 0;
// 
// 	ClipStruct* pClipBuf = &(PaintMainFrame->ClipBuf);
// 
//     POSITION pos = pDoc->GetFirstViewPosition();
// 	CPaintView* pView = NULL;

	bool ChangeFlag = 0;
	CBaseGraphClass *GrClass = NULL;
    
	
	char cSelIconName[PATH_LEN];
	memset(cSelIconName, 0, PATH_LEN);
    if (OperateFlag != GrLink && OperateFlag != GrWire && OperateFlag != GrBus)
	{
//         int nPixelIndex = PaintMainFrame->pIconMemClass->GetSelIconIndex (OperateFlag);
// 		int nIndex = PaintMainFrame->pIconMemClass->GetSelIconName (OperateFlag, cSelIconName);
//         if (nPixelIndex < 0 || nIndex == 0) 
// 		{
//             MessageBox ("\n  所选择图元无效 !    \n\n", "绘图包", MB_OK|MB_ICONSTOP);
//             return ;
//         }
    }

    Buf->gType = OperateFlag;
	Buf->comm.pos = !m_bPosFlag;
	Buf->style = 1;
	memcpy ((char *)Buf->pixel.pix.name, cSelIconName, PATH_LEN);

	Buf->power.bp = 0;
	Buf->power.count = 0;


    if (OperateFlag==GrBus) //yb++ 12.10
	{
		/*
		Buf->pixel.pix.cc[2] = PaintMainFrame->DrawBus.LBColor;

		Buf->pixel.pix.fore = PaintMainFrame->DrawBus.LTColor;
        Buf->pixel.pix.back = PaintMainFrame->DrawBus.RTColor;
        Buf->pixel.pix.cc[0] = PaintMainFrame->DrawBus.RBColor;
        Buf->pixel.pix.cc[1] = PaintMainFrame->DrawBus.LBColor;
        Buf->pixel.pix.cc[5] = PaintMainFrame->DrawBus.DigitalFlag;
		Buf->pixel.pix.cap = PaintMainFrame->DrawBus.ModeFlag;
		Buf->pixel.pix.x3 = PaintMainFrame->DrawBus.HVFlag;
		Buf->pixel.pix.fill = PaintMainFrame->DrawBus.wCols;*/
		if (Buf->pixel.pix.back < 0) 
			Buf->pixel.pix.arc = 0;
		else 
			Buf->pixel.pix.arc = 1;

//		Buf->pixel.pix.y3 = pDoc->BackColor;

// 		if (PaintMainFrame->DrawBus.ModeFlag == 0)
// 			Buf->pixel.pix.back = pDoc->BackColor;
//  		if (Buf->pixel.pix.back < 0)
// 			Buf->pixel.pix.arc = 0;
// 		else
// 			Buf->pixel.pix.arc = 1;
    }
	else if (OperateFlag == GrTransformer)
	{
//		Buf->pixel.pix.x3 = CurrentPasType;//PaintMainFrame->DrawTFM.wType;
// 		Buf->pixel.pix.fill = PaintMainFrame->DrawTFM.wLRFlag;
// 		Buf->pixel.pix.cc[0] = PaintMainFrame->DrawTFM.wHighFlag;
// 		Buf->pixel.pix.cc[1] = PaintMainFrame->DrawTFM.wMidFlag;
// 		Buf->pixel.pix.cc[2] = PaintMainFrame->DrawTFM.wLowFlag;
// 		Buf->pixel.pix.fore = PaintMainFrame->DrawTFM.crHigh;
// 		Buf->pixel.pix.cc[3] = PaintMainFrame->DrawTFM.crLow;
// 		Buf->pixel.pix.back = PaintMainFrame->DrawTFM.crMid;
	}
	else if (OperateFlag == GrPT)
	{
// 		Buf->pixel.pix.x3 = PaintMainFrame->DrawPT.wType;
// 		Buf->pixel.pix.fill = PaintMainFrame->DrawPT.wLRFlag;
// 		Buf->pixel.pix.cc[0] = PaintMainFrame->DrawPT.wHighFlag;
// 		Buf->pixel.pix.cc[1] = PaintMainFrame->DrawPT.wMidFlag;
// 		Buf->pixel.pix.cc[2] = PaintMainFrame->DrawPT.wLowFlag;
// 		Buf->pixel.pix.cc[5] = PaintMainFrame->DrawPT.w4THFlag;
// 		Buf->pixel.pix.fore = PaintMainFrame->DrawPT.crHigh;
// 		Buf->pixel.pix.back = PaintMainFrame->DrawPT.crMid;
// 		Buf->pixel.pix.cc[3] = PaintMainFrame->DrawPT.crLow;
// 		Buf->pixel.pix.cc[4] = PaintMainFrame->DrawPT.cr4TH;
	}
    else 
	{
//        Buf->pixel.pix.fore = pDoc->ForeColor;
//        Buf->pixel.pix.back = pDoc->BackColor;

		if(OperateFlag == GrLines)
		{
			if (Buf->pixel.pix.back < 0) 
				Buf->pixel.pix.fill = 0;
			else 
				Buf->pixel.pix.fill = 1;
		}
    }

	Buf->pixel.pix.glFlag = m_bOpenGLFlag;
//    Buf->pixel.pix.width = PenWidth;
//    Buf->pixel.pix.style = PenStyle;
//    Buf->pixel.pix.styleB = BrushStyle;
//	Buf->pixel.pix.y3 = PenAngle;

 	if (OperateFlag==GrWire || OperateFlag==GrLink)
	{
//		Buf->pixel.pix.cc[0] = CapBegStyle;
//		Buf->pixel.pix.cc[1] = CapEndStyle;
//		Buf->pixel.pix.cc[2] = m_wCapBegSize;
		//Buf->pixel.pix.cc[3] = m_wCapEndSize;
	}

   switch (OperateFlag) 
	{
    case GrSis:
//		Buf->pixel.pix.cap = CurrentPasType;
		break;
	case GrCap:
//		Buf->pixel.pix.cap = CurrentPasType;
//		Buf->pixel.pix.y3 = PenAngle;
		break;
    case GrRod:
	case GrMolten:
    case GrCT:
	//case GrIlink:
	case GrAgain:
	case GrIns:
	case GrCoil:
//		Buf->pixel.pix.cap = CurrentDevType;
		break;
	}

	if(OperateFlag == GrWire || OperateFlag == GrLink || OperateFlag == GrBus)
		Buf->style = 0;
	else
		Buf->style = 1;
	Buf->pixel.pix.image = 0;

//	m_wLayerNo = pDoc->m_wLayerNo;
//	memcpy(m_bLevel, pDoc->m_bLevel, LEVELNUM*sizeof(int));
//	Buf->comm.layer = m_wLayerNo;
//	for (int i = 0; i < LEVELNUM; i++)
//		Buf->comm.level[i] = m_bLevel[i];
//	m_wPaneNo = pDoc->m_wPaneNo;
//	Buf->serial = m_wPaneNo;
}

void GraphicsView::InvalObj(CDrawObj *pObj)
{
 	QRect rect = pObj->m_position;
//	DocToClient(rect);
      QRect tmpRect=rect.normalized();
//	if (m_bActive && IsSelected(pObj))
	if(isSelected(pObj))
	{
//		if( rect.isValid() )
		//rect.adjust(-4,-5,5,4);
        tmpRect.adjust(-6,-6,6,6);
//		rect.left -= 4;
//		rect.top -= 5;
//		rect.right += 5;
//		rect.bottom += 4;
	}
//	rect.InflateRect(1, 1); // handles CDrawOleObj objects
    
//	InvalidateRect(rect, FALSE);
   //  QRect tmpRect=rect.normalized();
#ifdef DEBUG_ICONMAKER
	printf("GraphicsView::InvalObj rect.left=%d,rect.top=%d,rect.right=%d,rect.bottom=%d\n ",rect.left(),rect.top(),rect.right(),rect.bottom());
    printf("GraphicsView::InvalObj tmpRect.left=%d,tmpRect.top=%d,tmpRect.right=%d,tmpRect.bottom=%d\n ",tmpRect.left(),tmpRect.top(),tmpRect.right(),tmpRect.bottom());
#endif 
 //   update(rect);	
	update(tmpRect);
 // update();
}

void GraphicsView::saveDocument()
{
#ifdef DEBUG_ICONMAKER
	printf(" GraphicsView::saveDocument()!\n");
#endif
    getDocument()->saveDocument();
}

void GraphicsView::openFile()
{

#ifdef DEBUG_ICONMAKER
	printf(" GraphicsView::openFile() !");
#endif
   QString fileName = QFileDialog::getOpenFileName ( this,
		                                              tr("Drawing select Dialog--BITC"),
													  tr("D:/wapp/icon/break"),
													  tr(""),
													   0,
													   0 ) ;   
	if(fileName.isEmpty()) return;
	getDocument()->openDocument(fileName);
	setWindowTitle(fileName);
	update();
//	repaint();
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
 #ifdef DEBUG_ICONMAKER
	printf(" GraphicsView:: mouseDoubleClickEvent() !\n");
#endif 
	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	setMouseTracking(false);
	if (pTool != 0)
		pTool->mouseDoubleClickEvent(event,this);
	
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
 #ifdef DEBUG_ICONMAKER
	printf(" GraphicsView:: mouseReleaseClickEvent() !\n");
#endif 
	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool != 0)
		pTool->mouseReleaseEvent(event,this);


}

bool GraphicsView::isSelected(CDrawObj *pObj)
{
 CDrawObj* pDrawObj = (CDrawObj*)pObj;
 return m_selection.contains(pDrawObj) != 0;
}

void GraphicsView::keyPressEvent(QKeyEvent *keyevent)
{
#ifdef DEBUG_ICONMAKER
	printf("GraphicsView::keyPressEvent pressed!\n");
#endif 
	switch( keyevent->key() )
	{
	case Qt::Key_Delete:
		{
#ifdef DEBUG_ICONMAKER
			printf("GraphicsView::keyPressEvent--- delete pressed !\n");
#endif 
		 if( m_selection.empty() ) return;
		 
		 ObjectList::iterator obj;

        for (obj = m_selection.begin(); obj < m_selection.end(); ++obj)
		{
			m_pDocument->undoStack()->push(new CRemoveObjUndo(m_pDocument, (*obj)));
			m_pDocument->Remove( *obj );
		}
         
		m_selection.clear();		
		update();
		break;
		}

	}

 
}
void GraphicsView::undoFunc()
{
#ifdef DEBUG_ICONMAKER
//	printf("GraphicsView::undoFunc() ");
#endif
  m_pDocument->undoStack()->undo();
  update();
}

void GraphicsView::redoFunc()
{

 m_pDocument->undoStack()->redo();
 update();
}
void GraphicsView::copyFunc()
{
	if( m_selection.empty() ) return;  // no selection return.
	

	if( m_selGrNum ) { delete m_selGrNum; m_selGrNum=0; } 

	if( m_selPtsNum ) { delete m_selPtsNum; m_selPtsNum=0; }

	if( m_polyObj )
	{
	 for( int tt=0;tt<*m_polyObj;tt++)
	 {
		 if( m_ptsNum[tt] )  {
			 delete m_ptsNum[tt];
			 m_ptsNum[tt] = 0;
		 }
	 }
	 if( m_ptsNum ) 
	 {  
		free(m_ptsNum); 
	   m_ptsNum=0;
	 }

	 delete m_polyObj;
	 m_polyObj = 0;
	}

	m_selGrNum= new int(m_selection.count());
    m_selPtsNum = new int(0);
	m_polyObj   = new int(0);


//	int selNum = m_selection.count();
	
    if(clipBoardBuf) 
	{
		free(clipBoardBuf);
		clipBoardBuf = 0;
	}

	if(clipBoardPtBuf)
	{
		free(clipBoardPtBuf);
		clipBoardPtBuf =0 ;
	}


	int selPtsNum = *m_selPtsNum;
	clipBoardBuf = (ONEGR*)malloc(sizeof(ONEGR)*(*m_selGrNum));
	ONEGR *data = clipBoardBuf;
	memset((char*)data,0,sizeof(ONEGR)*(*m_selGrNum));

   // char *tmp=(char*)data;
    ONEGR grTmp;
	for( int i=0;i<(*m_selGrNum);i++)
	{
      m_selection.at(i)->getGrInfoFromObj(&grTmp);
	  memcpy(data,&grTmp,sizeof(ONEGR));
	  data += sizeof(ONEGR);
	  
	  if( grTmp.gType == GrLines || grTmp.gType == GrSegLine )
	  {
		  m_ptsNum = (int **)realloc(m_ptsNum,sizeof(int*)*((*m_polyObj)+1));  //int **

		  m_ptsNum[*m_polyObj] = new int(0);
		  ( *(m_ptsNum[*m_polyObj]) ) = m_selection.at(i)->m_nPoints; 

          (*m_polyObj) += 1;

		  int tmpPtsNum = selPtsNum;
		  selPtsNum += m_selection.at(i)->m_nPoints;
		  clipBoardPtBuf = (PointStr*) realloc(clipBoardPtBuf,sizeof(PointStr)*selPtsNum);
	      for(int j=0;j<m_selection.at(i)->m_nPoints;j++)
		  {
           clipBoardPtBuf[ tmpPtsNum +j ].x = m_selection.at(i)->m_points[j].x();
		   clipBoardPtBuf[ tmpPtsNum +j ].y = m_selection.at(i)->m_points[j].y();
		  }
	  }
      

	}

	*m_selPtsNum = selPtsNum;
	QByteArray byteArray = QByteArray::fromRawData((char*)m_selGrNum,sizeof(int) );
	byteArray += QByteArray::fromRawData((char*)m_polyObj,sizeof(int));
	for( int tt=0;tt<(*m_polyObj);tt++)
	{
		byteArray += QByteArray::fromRawData((char*)m_ptsNum[tt],sizeof(int));
	}
    //byteArray += QByteArray::fromRawData((char*)m_selPtsNum,sizeof(int) );
	byteArray += QByteArray::fromRawData((char*)clipBoardBuf,sizeof(ONEGR)*(*m_selGrNum ));
	byteArray += QByteArray::fromRawData((char*)clipBoardPtBuf,sizeof(PointStr)*selPtsNum );
    
	mimeData->setData("IPMPICS",byteArray);
  
//	mimeData->setData("IPMPICS",QByteArray::fromRawData((char*)clipBoardBuf,sizeof(ONEGR)*selNum));
	
	QApplication::clipboard()->setMimeData(mimeData);

}


void GraphicsView::pasteFunc()
{
	// get the data in clipboard.

	QByteArray byteData = QApplication::clipboard()->mimeData()->data("IPMPICS");

	int grNum=0;
	int pgrNum=0; // polygr
    int **ptsNum=0;

	int grPtsNum=0;
	memcpy((void*)&grNum,byteData,sizeof(int));
	memcpy((void*)&pgrNum,byteData.mid(4,4),sizeof(int));
	if(grNum==0 || grNum<pgrNum ) return;

    ptsNum = (int**)malloc(sizeof(int*));
	for(int tt=0;tt<pgrNum;tt++)
	{
		ptsNum[tt] = new int(0);
		memcpy((void*)ptsNum[tt],byteData.mid( 8+4*tt, 4),sizeof(int));
		grPtsNum += *ptsNum[tt];
	}

    
	//memcpy((void*)&grPtsNum,byteData.mid(4,4),sizeof(int));

  // int dataLen = grNum*sizeof(ONEGR) + grPtsNum*sizeof(PointStr);
  // if( dataLen ==0 ) return ;

   QByteArray actByteData = byteData.mid(8+4*pgrNum);
   if( actByteData.size() == 0  ) return;
		
	ONEGR *grTmp=(ONEGR*)malloc(sizeof(ONEGR)*grNum);
	PointStr *ptsTmp=(PointStr*)malloc(sizeof(PointStr)*grPtsNum);

	memcpy((void*)grTmp,actByteData,sizeof(ONEGR)*grNum);
	memcpy((void*)ptsTmp,actByteData.mid(sizeof(ONEGR)*grNum),sizeof(PointStr)*grPtsNum);
    
	int pgrIndex=0;
	int ptOffset=0;
	CDrawObj *pObj=0;
    for(int i=0;i<grNum;i++)
	{
     int j=0;
	QRect rect = QRect(grTmp[i].comm.x0,grTmp[i].comm.y0,
			          grTmp[i].comm.x1-grTmp[i].comm.x0,grTmp[i].comm.y1-grTmp[i].comm.y0); 
	 switch( grTmp[i].gType )
	 {
	 case GrLine:  
	/*	 CDrawLine */
		 pObj=new CDrawLine(rect);
	//	 pObj->SetObjInfoFromGr(&grTmp[i]);
	//	 m_pDocument->Add(pObj);
		 break;
	 case GrRectangle:
	/*	CDrawRect */
		 pObj=new CDrawRect(rect);
	//	pObj->SetObjInfoFromGr(&grTmp[i]);
	//	m_pDocument->Add(pObj);
		break;
	 case GrSegLine:
		 /*CDrawPLine */
		 pObj=new CDrawPLine(rect);
         pObj->SetObjPtsInfo(ptsTmp + ptOffset,(*ptsNum[pgrIndex]));
		 ptOffset += *(ptsNum[pgrIndex]);
		 pgrIndex += 1;
//		 pObj->SetObjPtsInfo();
	//	 pObj->SetObjInfoFromGr(&grTmp[i]);
	//	 m_pDocument->Add(pObj);
		 break;
     case GrArc:
		 pObj=new CDrawArc(rect);
		 break;
	 case GrLines:
		 pObj=new CDrawPolygon(rect);
		 pObj->SetObjPtsInfo(ptsTmp + ptOffset,(*ptsNum[pgrIndex]));
		 ptOffset += *(ptsNum[pgrIndex]);
		 pgrIndex += 1;
		 break;
	 case GrCircle:
		 pObj=new CDrawEllipse(rect);
		 break;
	 case GrNode:
		 pObj=new CDrawNode(rect);
		 break;
	 default:
		 break;

	 }
	 if(pObj)
	 {
	 pObj->SetObjInfoFromGr(&grTmp[i]);
	 m_pDocument->Add(pObj);
	 }

	}
    update();



	if(ptsTmp) 
	{
		free(ptsTmp) ; ptsTmp=0;
	}

	if(grTmp) 
	{
		free(grTmp);
		grTmp=0;
	}


	for(int tt=0;tt<pgrNum;tt++)
	{
		if(ptsNum[tt]){
	  delete ptsNum[tt] ;
	  ptsNum[tt] = 0;
		}
	}

	if(ptsNum)
	{
		free(ptsNum); ptsNum=0;
	}

}


void GraphicsView::RightButtonFunc()
{
		if( m_selection.count()==1 )
		{
			CDrawObj *pObj = m_selection.at(0);
			CDlgStyle tmpDlg(pObj/*->metaObject()->className())*/);
            tmpDlg.exec();
			const char *classname = pObj->metaObject()->className() ;
			if( strcmp(classname,"CDrawArc")==0)
			{
				static_cast<CDrawArc*>(pObj)->setObjInfoFromArcStr( tmpDlg.getArcAttr() );

			}
            pObj->setPenWidth(tmpDlg.getPenWidth() );
			QPen tmpPen = pObj->getPen();
			tmpPen.setWidth(tmpDlg.getPenWidth() );
			pObj->setPen( tmpPen);
		}
} 

void GraphicsView::setMainWin(graphAppWindow *pMainW)
{
	if(pMainW==0 ) return ;

     m_pMainWindow = pMainW;
//	 connect(this,SIGNAL(
}

void GraphicsView::drawGrids(QPainter &painter)
{



	
	painter.save();
	
	// desactive tout antialiasing, sauf pour le texte
	painter.setRenderHint(QPainter::Antialiasing, false);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
	
	painter.setBrush(Qt::black);

	QRect r(0,0,width(),height());
	painter.drawRect(r);
	
	if (m_graphGridVisible) 
	{
		// to ease transition from qelec
		int xGrid=m_graphGridScale * (int) m_graphGrid;
		int yGrid=m_graphGridScale * (int) m_graphGrid;
		// dessine les points de la grille
		painter.setPen(Qt::white);
		painter . setBrush(Qt::NoBrush);
		qreal limite_x = r.x() + r.width();
		qreal limite_y = r.y() + r.height();
		
		int g_x = (int)ceil(r.x());
		while (g_x % xGrid) ++ g_x;
		int g_y = (int)ceil(r.y());
		while (g_y % yGrid) ++ g_y;
		
		for (int gx = g_x ; gx < limite_x ; gx += xGrid) 
		{
			for (int gy = g_y ; gy < limite_y ; gy += yGrid) 
			{
				painter. drawPoint(gx, gy);
			}
		}
	}
	
	painter .restore();





}
int GraphicsView::GetGraphScale (int wAxisFlag)
{
	int wGraphScaleX = m_wScaleX;
	int wGraphScaleY = m_wScaleY;

	if (m_wAdjustFlag == 1)
	{
		wGraphScaleX = m_wAdjustScaleX;
		wGraphScaleY = m_wAdjustScaleY;
	}
	if (wGraphScaleX == 0)
		wGraphScaleX = 100;
	if (wGraphScaleY == 0)
		wGraphScaleY = 100;

	if (wAxisFlag == 0)
		return wGraphScaleX;
	else
		return wGraphScaleY;
}

int GraphicsView::GetGraphScroll (int wAxisFlag)
{
	int wScrollX = ScrollX;
	int wScrollY = ScrollY;

	if (m_wAdjustFlag == 1)
	{
		wScrollX = AdjustScrollX;
		wScrollY = AdjustScrollY;
	}

	if (wAxisFlag == 0)
		return wScrollX;
	else
		return wScrollY;
}

int GraphicsView::GetSrcCoordX (int wCoordX)
{
	int wScrollX = GetGraphScroll (0);
	int wScrollY = GetGraphScroll (1);
	int wGraphScaleX = GetGraphScale (0);
	int wGraphScaleY = GetGraphScale (1);

	int wCoordXRet = (int)((float)(wCoordX + wScrollX)*100.0f/(float)wGraphScaleX);

	return wCoordXRet;
}

int GraphicsView::GetSrcCoordY (int wCoordY)
{
	int wScrollX = GetGraphScroll (0);
	int wScrollY = GetGraphScroll (1);
	int wGraphScaleX = GetGraphScale (0);
	int wGraphScaleY = GetGraphScale (1);

	//提高精度 YUQU EDIT 2001
	int wCoordYRet = (int)((float)(wCoordY + wScrollY)*100.0f/(float)wGraphScaleY);

	return wCoordYRet;
}

int GraphicsView::GetSrcCoordW (int wCoordW)
{
	int wScrollX = GetGraphScroll (0);
	int wScrollY = GetGraphScroll (1);
	int wGraphScaleX = GetGraphScale (0);
	int wGraphScaleY = GetGraphScale (1);

	//提高精度 YUQU EDIT 2001
	int wCoordWRet = (int)((float)wCoordW*100.0f/(float)wGraphScaleX);

	return wCoordWRet;
}

int GraphicsView::GetSrcCoordH (int wCoordH)
{
	int wScrollX = GetGraphScroll (0);
	int wScrollY = GetGraphScroll (1);
	int wGraphScaleX = GetGraphScale (0);
	int wGraphScaleY = GetGraphScale (1);

	//提高精度 YUQU EDIT 2001
	int wCoordHRet = (int)((float)wCoordH*100.0f/(float)wGraphScaleY);

	return wCoordHRet;
}

int GraphicsView::GetZoomCoordX (int wCoordX)
{
	int wScrollX = GetGraphScroll (0);
	int wScrollY = GetGraphScroll (1);
	int wGraphScaleX = GetGraphScale (0);
	int wGraphScaleY = GetGraphScale (1);

    int wCoordXRet = (int) ((float)wCoordX*(float)wGraphScaleX/100.0f - (float)wScrollX);

	return wCoordXRet;
}

int GraphicsView::GetZoomCoordY (int wCoordY)
{
	int wScrollX = GetGraphScroll (0);
	int wScrollY = GetGraphScroll (1);
	int wGraphScaleX = GetGraphScale (0);
	int wGraphScaleY = GetGraphScale (1);

    int wCoordYRet = (int) ((float)wCoordY*(float)wGraphScaleY/100.0f - (float)wScrollY);

	return wCoordYRet;
}

int GraphicsView::GetZoomCoordW (int wCoordW)
{
	int wScrollX = GetGraphScroll (0);
	int wScrollY = GetGraphScroll (1);
	int wGraphScaleX = GetGraphScale (0);
	int wGraphScaleY = GetGraphScale (1);

	int wCoordWRet = (int)((float)(wCoordW*wGraphScaleX)/100.0f);

	return wCoordWRet;
}

int GraphicsView::GetZoomCoordH (int wCoordH)
{
	int wScrollX = GetGraphScroll (0);
	int wScrollY = GetGraphScroll (1);
	int wGraphScaleX = GetGraphScale (0);
	int wGraphScaleY = GetGraphScale (1);

	int wCoordHRet = (int)((float)(wCoordH*wGraphScaleY)/100.0f);

	return wCoordHRet;
}

float GraphicsView::GetZoomCoordW (float fCoordW)
{
	int wScrollX = GetGraphScroll (0);
	int wScrollY = GetGraphScroll (1);
	int wGraphScaleX = GetGraphScale (0);
	int wGraphScaleY = GetGraphScale (1);

    float fCoordWRet = (float)(fCoordW*wGraphScaleX)/100.0f;

	return fCoordWRet;
}

float GraphicsView::GetZoomCoordH (float fCoordH)
{
	int wScrollX = GetGraphScroll (0);
	int wScrollY = GetGraphScroll (1);
	int wGraphScaleX = GetGraphScale (0);
	int wGraphScaleY = GetGraphScale (1);

    float fCoordHRet = (float) (fCoordH*wGraphScaleY)/100.0f;

	return fCoordHRet;
}

double GraphicsView::GetZoomCoordW (double dblCoordW)
{
	int wScrollX = GetGraphScroll (0);
	int wScrollY = GetGraphScroll (1);
	int wGraphScaleX = GetGraphScale (0);
	int wGraphScaleY = GetGraphScale (1);

    double dblCoordWRet = (double) (dblCoordW*wGraphScaleX)/100.0;

	return dblCoordWRet;
}

double GraphicsView::GetZoomCoordH (double dblCoordH)
{
	int wScrollX = GetGraphScroll (0);
	int wScrollY = GetGraphScroll (1);
	int wGraphScaleX = GetGraphScale (0);
	int wGraphScaleY = GetGraphScale (1);

    double dblCoordHRet = (double) (dblCoordH*wGraphScaleY)/100.0;

	return dblCoordHRet;
}