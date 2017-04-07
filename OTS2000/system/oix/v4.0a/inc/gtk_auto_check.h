#ifndef	_GTK_AUTO_CHECK_H
#define	_GTK_AUTO_CHECK_H

#include <gtk/gtk.h>
#include <glib.h>
#include	"../inc/oix.h"
#include "../inc/gtk_search_data.h"

#define AUTOCHECK_FILE	"autocheck.txt"
#define COMPRESULT_FILE	"compareresult.txt"
#define AUTOCHECK_DATA	"autocheck.data"

#define AUTOCHECK_XLS	"autocheck.xls"
#define COMPRESULT_XLS	"compareresult.xls"

#define MAX_DEV_NUM		50
#define MAX_GROUP_NUM	50
#define MAX_EVENT_NUM	1000 //事件缓冲区搜索条数
#define OPT_ITEM_NUM	4
#define MAX_SEARCH_STR_SIZE 12

#define COMPARE 1 //1产生比较结果

enum {
	eScan_state=0, 
	eAlm_state,
	eAlm_ack,
	eCtrl_lock,
	eUpdate_state,
	eRepeat_alm,
	eTag_state,
	eMan_set,
	eautocheckNum};

	
enum {
		NewResult=0, 
			DelResult,
			OldResult,
			AllResult,
		};

//打开上次巡检结果时flag全部置0,
//1 新结果生成时，每个都与上次比较
//	相等为0 上次存在值 删除老节点
//	不等为1 新产生值 删除老节点
//  剩余的为老的

typedef struct 
{	
	POINTER* ppoint;
	int flage[eautocheckNum];
	
} COMP_RESULT_PT;

COMP_RESULT_PT check_pt;

typedef struct
{
	int Evt_idx[MAX_EVENT_NUM];//符合条件点在事件区中索引
	int num;
}EVT_RESULT;

EVT_RESULT Impt_alm;//重要点事故报警
EVT_RESULT Dev_ctrl;//控制调节

typedef struct
{
	GSList *Scan_state; //扫查禁止
	GSList *Alm_state;//报警禁止
	GSList *Alm_ack;//报警未确认
	GSList *Tag_state;//挂牌
	GSList *Man_set;//人工置数
}CHECK_RESULT;
CHECK_RESULT checkresult;
//delpt_result初始值为上次巡检结果，
//oldpt_result初始值为空
//newpt_result初始值为空
//本次巡检结果中每个值作为查询条件在delpt_result中查找
//找到的为old 插入oldpt_result，并在delpt_result中删除该点
//未找到的为new,插入newpt_result
CHECK_RESULT oldpt_result;//compare result old point
CHECK_RESULT newpt_result;//compare result new point
CHECK_RESULT delpt_result;//compare result del point 

GtkWidget *CondCheckDlg[MAX_CRT_NUMBER]={ NULL,NULL,NULL};
GtkListStore *storeFilter[MAX_CRT_NUMBER];
GtkListStore *storeStn[MAX_CRT_NUMBER],*storeDev[MAX_CRT_NUMBER],*storeGroup[MAX_CRT_NUMBER],*storeType[MAX_CRT_NUMBER],*storeAttrib[MAX_CRT_NUMBER];
FILTER_T FilterCheck[MAX_CRT_NUMBER];
GtkWidget *buttonAdd[MAX_CRT_NUMBER],*buttonDel[MAX_CRT_NUMBER],*buttonEdit[MAX_CRT_NUMBER],*comboBox[MAX_CRT_NUMBER];
BOOL check_flag;//0: 条件巡检  1: 自动巡检
int curStn,curDev;
int template_flag; //0: 预定义模板  1:自定义模板  2:无模板


extern CUR_PAGE_INFO cur_search_page;
extern SEARCH_PT_INFO search_pt;
extern GtkWidget* SearchResultWin[MAX_CRT_NUMBER];

extern char *_fromUtf8(char *c);
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern BOOL  isStnInMask (UCHAR stn_id) ;
extern int ReadFilterXml(GtkListStore *listStore,const char *nodeName);
extern int SaveFilterXml(gchar *filterName,FILTER_T *pFilter,const char *nodeName);
extern int DelFilterXml(gchar *filterName,const char *nodeName);
extern void InitAlarmFilter(FILTER_T *pFilter,int crt_n);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] ); 
extern int SearchData();
extern GtkWidget *DspResult(int crt_n,int pt_num,int type);
extern void DspSearchPage (int crt_n, int pt_num );
int ProgAutoCheck(GtkWidget*parent,int crt_n);
void ProgCondCheckTab(GtkWidget*parent,int crt_n);

enum
{
	eFilterName,
	eStnId,
	eStnMask,
	eFaultMask,
	eTypeIdMask,
	eDevMask,
	eGroupMask,
	eDataTypeMask,
	eAttribMask,
	num_columns
};


char AUTOCHECKXMLHEADSTR[]=  "<?xml version='1.0'?> \n "
"<?mso-application progid='Excel.Sheet'?> \n "
"<Workbook xmlns='urn:schemas-microsoft-com:office:spreadsheet' xmlns:o='urn:schemas-microsoft-com:office:office' xmlns:x='urn:schemas-microsoft-com:office:excel' xmlns:ss='urn:schemas-microsoft-com:office:spreadsheet' xmlns:html='http://www.w3.org/TR/REC-html40'>\n"
	"<OfficeDocumentSettings xmlns='urn:schemas-microsoft-com:office:office'>\n"
	"	<RemovePersonalInformation/>\n"
	"	<Colors>\n"
	"		<Color>\n"
	"			<Index>39</Index>\n"
	"			<RGB>#E3E3E3</RGB>\n"
	"		</Color>\n"
	"	</Colors>\n"
	"</OfficeDocumentSettings>\n"
	"<ExcelWorkbook xmlns='urn:schemas-microsoft-com:office:excel'>\n"
	"	<WindowHeight>9105</WindowHeight>\n"
	"	<WindowWidth>8775</WindowWidth>\n"
	"	<WindowTopX>-30</WindowTopX>\n"
	"	<WindowTopY>240</WindowTopY>\n"
	"	<ActiveSheet>3</ActiveSheet>\n"
	"	<FirstVisibleSheet>1</FirstVisibleSheet>\n"
	"	<ProtectStructure>False</ProtectStructure>\n"
	"	<ProtectWindows>False</ProtectWindows>\n"
	"</ExcelWorkbook>\n"
	"<Styles>\n"
	"	<Style ss:ID='Default' ss:Name='Normal'>\n"
	"		<Alignment ss:Vertical='Bottom'/>\n"
	"		<Borders/>\n"
"			<Font ss:FontName='song' x:CharSet='134' ss:Size='12'/>\n"
"			<Interior/>\n"
"			<NumberFormat/>\n"
"			<Protection/>\n"
"		</Style>\n"
"		<Style ss:ID='s21'>\n"
"			<Alignment ss:Vertical='Bottom' ss:WrapText='1'/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s22'>\n"
"			<Alignment ss:Horizontal='Left' ss:Vertical='Bottom' ss:WrapText='1'/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s23'>\n"
"			<Alignment ss:Vertical='Bottom' ss:WrapText='1'/>\n"
"			<Font ss:FontName='song' x:CharSet='134' ss:Size='12' ss:Color='#3366FF'/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s24'>\n"
"			<Alignment ss:Horizontal='Center' ss:Vertical='Center' ss:WrapText='1'/>\n"
"			<Font ss:FontName='song' x:CharSet='134' ss:Size='12' ss:Color='#3366FF'/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s25'>\n"
"			<Alignment ss:Horizontal='Left' ss:Vertical='Bottom' ss:WrapText='1'/>\n"
"			<Font ss:FontName='song' x:CharSet='134' ss:Size='12' ss:Color='#3366FF'/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s26'>\n"
"			<Alignment ss:Vertical='Bottom' ss:WrapText='1'/>\n"
"			<Font ss:FontName='song' x:CharSet='134'/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s27'>\n"
"			<Alignment ss:Horizontal='Left' ss:Vertical='Bottom' ss:WrapText='1'/>\n"
"			<Font ss:FontName='song' x:CharSet='134'/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s29'>\n"
"			<Alignment ss:Horizontal='Center' ss:Vertical='Bottom' ss:WrapText='1'/>\n"
"			<Font ss:FontName='song' x:CharSet='134'/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s30'>\n"
"			<Alignment ss:Horizontal='Center' ss:Vertical='Bottom'/>\n"
"			<Borders/>\n"
"			<Font ss:FontName='song' x:CharSet='134'/>\n"
"			<Interior/>\n"
"			<NumberFormat/>\n"
"			<Protection/>\n"
"		</Style>\n"
"		<Style ss:ID='s31'>\n"
"			<Alignment ss:Horizontal='Center' ss:Vertical='Bottom'/>\n"
"			<Font ss:FontName='song' x:CharSet='134'/>\n"
"		</Style>\n"
"		<Style ss:ID='s32'>\n"
"			<Alignment ss:Horizontal='Left' ss:Vertical='Center' ss:ReadingOrder='LeftToRight' ss:WrapText='1'/>\n"
"			<Font ss:FontName='song' x:CharSet='134' ss:Size='12' ss:Color='#3366FF'/>\n"
"			<NumberFormat ss:Format='000000'/>\n"
"		</Style>\n"
"		<Style ss:ID='s33'>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s34'>\n"
"			<Font ss:FontName='song' x:CharSet='134' ss:Size='12' ss:Color='#3366FF'/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s35'>\n"
"			<Alignment ss:Horizontal='Left' ss:Vertical='Center' ss:WrapText='1'/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s36'>\n"
"			<Alignment ss:Horizontal='Right' ss:Vertical='Bottom' ss:WrapText='1'/>\n"
"			<Interior/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s37'>\n"
"			<Alignment ss:Horizontal='Left' ss:Vertical='Bottom'/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s38'>\n"
"			<Alignment ss:Horizontal='Left' ss:Vertical='Bottom'/>\n"
"			<Font ss:FontName='song' x:CharSet='134' ss:Size='12' ss:Color='#3366FF'/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s39'>\n"
"			<Font ss:FontName='song' x:CharSet='134' ss:Size='12'/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s40'>\n"
"			<Alignment ss:Vertical='Bottom'/>\n"
"			<Borders/>\n"
"			<Interior/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"			<Protection/>\n"
"		</Style>\n"
"		<Style ss:ID='s41'>\n"
"			<Alignment ss:Horizontal='Left' ss:Vertical='Bottom'/>\n"
"			<Borders/>\n"
"			<Font ss:FontName='song' x:CharSet='134' ss:Size='12'/>\n"
"			<Interior/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"			<Protection/>\n"
"		</Style>\n"
"		<Style ss:ID='s42'>\n"
"			<Alignment ss:Horizontal='Right' ss:Vertical='Bottom'/>\n"
"			<Interior/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s43'>\n"
"			<Alignment ss:Horizontal='Left' ss:Vertical='Bottom'/>\n"
"			<Interior/>\n"
"			<NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s66'>\n"
"		    <Alignment ss:Horizontal='Left' ss:Vertical='Bottom' ss:WrapText='1'/>\n"
"		    <Font ss:FontName='宋体' x:CharSet='134' ss:Color='#FF0000'/>\n"
"		    <NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"		<Style ss:ID='s81'>\n"
"		    <Alignment ss:Horizontal='Left' ss:Vertical='Bottom' ss:WrapText='1'/>\n"
"		    <Font ss:FontName='宋体' x:CharSet='134' ss:Color='#00B050'/>\n"
"		    <NumberFormat ss:Format='@'/>\n"
"		</Style>\n"
"	</Styles>\n";




char AUTOCHECKXMLCOL[]= "	<Table ss:ExpandedColumnCount='16'  x:FullColumns='1' x:FullRows='1' ss:StyleID='s21' ss:DefaultColumnWidth='54' ss:DefaultRowHeight='14.25'> \n"
"	<Column ss:Index='1' StyleID='s22' ss:AutoFitWidth='0' ss:Width='82.5'/>\n"
"	<Column ss:StyleID='s22' ss:Width='80.5'/>\n"
"	<Column ss:StyleID='s22' ss:AutoFitWidth='0' ss:Width='200'/>\n"
"	<Column ss:StyleID='s21' ss:Width='240' ss:Span='2'/>\n";


char AUTOCHECKHEADXML[]="	<Row ss:AutoFitHeight='0' ss:StyleID='s23'>\n"
"		<Cell ss:StyleID='s24'>\n"
"			<Data ss:Type='String'>序号</Data>\n"
"		</Cell>\n"
"		<Cell ss:StyleID='s24'>\n"
"			<Data ss:Type='String'>点号</Data>\n"
"		</Cell>\n"
"		<Cell ss:StyleID='s24'>\n"
"			<Data ss:Type='String'>逻辑名</Data>\n"
"		</Cell>\n"
"		<Cell ss:StyleID='s24'>\n"
"			<Data ss:Type='String'>长名</Data>\n"
"		</Cell>\n"
"			</Row>\n";


#endif  /*_GTK_AUTO_CHECK_H*/

