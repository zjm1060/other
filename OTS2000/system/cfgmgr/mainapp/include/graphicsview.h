


#ifndef _GRAPHICSVIEW_H
#define _GRAPHICSVIEW_H

#include<QTextEdit>
#include"drawdoc.h"
#include"colormanager.h"
class graphAppWindow;
class appGraphDocument;



enum ACTIONType
{
	wActDefault,
	wActAdd,
	wActDel,
	wActEdit,
	wActTurn,
	wActNo			   
};

extern CColorManager colormanager;
class GraphicsView:public QWidget
{
 Q_OBJECT
public:
  
	void AddLine();
	void newFile();
	GraphicsView(graphAppWindow *pappwin);

	virtual ~GraphicsView();
//	QPainter *painter;

private:
	void drawGrids(QPainter& painter);

	short m_bgIndex;  // canvas background color index. default 0;
	int   m_gridLineColorIndex;
	int   m_gridWidth;
	int   m_gridHeight;
	Qt::PenStyle   m_gridLineStyle;

	short m_penColorIndex; 
    short m_brushColorIndex; 
//	short m_penStyle;
	Qt::PenStyle m_penStyle;
//    short m_brushStyle;
	Qt::BrushStyle m_brushStyle;
	short m_penWidth; 

	qreal m_graphGrid;
	bool m_graphGridVisible;
    int m_graphGridScale;

	/*for clipboard */
	QMimeData *mimeData;
	ONEGR* clipBoardBuf;
	PointStr *clipBoardPtBuf;
	int *m_selGrNum;
	int *m_selPtsNum;
	int *m_polyObj;  // polygon or seglines num to be copy into clipboard
	int **m_ptsNum;   // according to the poly obj, pts num in each. 
					//m_ptsNum will have m_polyObj size.
	/* For clipBoard */


	QPen m_pen;
	QBrush m_brush;

	int selectMode;
	appGraphDocument * m_pDocument;
    graphAppWindow *m_pMainWindow;
	
public:
	void setMainWin(graphAppWindow *pMainW);
	virtual void keyPressEvent(QKeyEvent* keyevent);
    
	inline QPen  getPen() { return m_pen;}
	inline QBrush getBrush() { return m_brush;}
	inline void setPen(QPen pen) { m_pen = pen;}
	inline void setBrush(QBrush brush) { m_brush=brush;} 

	inline short getPenColorIndex()   { return (m_penColorIndex>=0&&m_penColorIndex<256)?m_penColorIndex:0; }
	inline short getBrushColorIndex() { return (m_brushColorIndex>=0&&m_brushColorIndex<256)?m_brushColorIndex:0; }
   // inline short getPenStyle()        { return m_penStyle;}
	inline Qt::PenStyle getPenStyle() { return m_penStyle;}
  //	inline short getBrushStyle()	  { return m_brushStyle; }
	inline Qt::BrushStyle getBrushStyle() { return m_brushStyle;}
	inline short getPenWidth()		  { return (m_penWidth>0?m_penWidth:1); }

	inline void setPenColorIndex(short colorindex)   { m_penColorIndex=colorindex; m_pen.setColor(colormanager.m_pColor[m_penColorIndex]); }
	inline void setBrushColorIndex(short colorindex) { m_brushColorIndex=colorindex; m_brush.setColor(colormanager.m_pColor[colorindex]); }
   // inline void setPenStyle(short style)        { m_penStyle=style ;}
	inline void setPenStyle( Qt::PenStyle style) { m_penStyle = style ; m_pen.setStyle(style); }
	//inline void setBrushStyle(short style )	  { m_brushStyle=style ; }
	inline void setBrushStyle( Qt::BrushStyle style) { m_brushStyle=style; m_brush.setStyle(style);}
	inline void setPenWidth(short penwidth)		  { m_penWidth=penwidth; m_pen.setWidth(m_penWidth);}

    inline int  getGridLineColorIndex() { return (m_gridLineColorIndex>=0&&m_gridLineColorIndex<=255)?m_gridLineColorIndex:47;}
	inline int  getGridWidth() {  return (m_gridWidth>=10&&m_gridWidth<=50)?m_gridWidth:10;}
	inline int  getGridHeight() { return ( m_gridHeight>=10&&m_gridHeight<=50)?m_gridHeight:10;}
	inline Qt::PenStyle  getGridStyle() { return  m_gridLineStyle; }
	inline short getBgColorIndex() { return (m_bgIndex>=0&&m_bgIndex<=255)?m_bgIndex:0 ;}

	inline void  setGridLineColorIndex(int colorindex) { (colorindex>=0&&colorindex<=255)?m_gridLineColorIndex=colorindex:m_gridLineColorIndex=47;}
	inline void  setGridWidth(int tmpwidth) {  (tmpwidth>=10&&tmpwidth<=50)?m_gridWidth=tmpwidth:m_gridWidth=10 ; }
	inline void  setGridHeight(int tmpH) {  ( tmpH>=10&&tmpH<=50)?m_gridHeight=tmpH:m_gridHeight=10;}
	inline void  setGridStyle(Qt::PenStyle tmpStyle) { m_gridLineStyle=tmpStyle; }
    inline void setBgColorIndex( short index) { (index>=0&&index<=255)?m_bgIndex  = index:0; }
	bool isSelected( CDrawObj * pObj);

	int GetGraphScale (int wAxisFlag);
	int GetGraphScroll (int wAxisFlag);
	
	int GetSrcCoordX (int wCoordX);
	int GetSrcCoordY (int wCoordY);
	int GetSrcCoordW (int wCoordW);
	int GetSrcCoordH (int wCoordH);
	
	int GetZoomCoordX (int wCoordX);
	int GetZoomCoordY (int wCoordY);
	int GetZoomCoordW (int wCoordW);
	int GetZoomCoordH (int wCoordH);
	float GetZoomCoordW (float fCoordW);
	float GetZoomCoordH (float fCoordH);
	double GetZoomCoordW (double dblCoordW);
	double GetZoomCoordH (double dblCoordH);
	
	void redoFunc();
	void undoFunc();
	void pasteFunc();
	void copyFunc();
	void openFile();
	void saveDocument();
	void InvalObj(CDrawObj* pObj);
	void Select(CDrawObj* pObj, bool bAdd = FALSE);
	QPointF endPt;
	QPointF begPt;
	appGraphDocument* getDocument() { return m_pDocument;} 

	void CreateOneGr(ONEGR *Buf);
	ObjectList m_selection;
 //   IconScene *scene;

	bool b_Polyline;
	QRect *m_pSelRect;

	int posx;
    int posy;

	LogFontStr *TmpFontBuf;

	QPoint m_ptLButtonDown, m_ptMouseOld;

	int m_wAdjustFlag;
	int m_wScaleX, m_wScaleY;
    int ScrollX, ScrollY;
	int AdjustScrollY;
	int AdjustScrollX;
	int m_wAdjustScaleX;
	int m_wAdjustScaleY;
protected:
	ACTIONType* m_UndoActions;  
	ACTIONType* m_RedoActions;


protected:
	virtual void mouseMoveEvent(QMouseEvent * e);
	virtual void mousePressEvent(QMouseEvent* e);
	virtual void paintEvent(QPaintEvent* );
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *e);
	
private slots:
	void RightButtonFunc();

};
#endif 