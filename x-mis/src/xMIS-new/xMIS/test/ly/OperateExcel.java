package ly;

import java.io.*; 
import java.util.Date; 
import jxl.*; 
import jxl.format.Colour; 
import jxl.format.UnderlineStyle; 
import jxl.read.biff.BiffException; 
import jxl.write.*; 
import jxl.format.UnderlineStyle; 
import jxl.format.CellFormat;; 

public class OperateExcel { 

/** 
  * Read data from a excel file 
  */ 
public static void  readExcel(String excelFileName){ 
  Workbook  rwb = null;  
  try{ 
   InputStream stream = new FileInputStream(excelFileName); 
   rwb = Workbook.getWorkbook(stream); 
   Sheet  sheet = rwb.getSheet(0); 
   Cell   cell  = null; 
   int columns = sheet.getColumns(); 
   int rows    = sheet.getRows(); 
   for( int i=0 ; i< rows ; i++ ) 
    for( int j=0 ; j< columns ; j++){ 
     //attention: The first parameter is column,the second parameter is row.  
     cell = sheet.getCell(j,i);    
     String str00 = cell.getContents(); 
     if( cell.getType() == CellType.LABEL ) 
       str00 += " LAEBL"; 
     else if( cell.getType() == CellType.NUMBER) 
       str00 += " number"; 
     else if( cell.getType() == CellType.DATE) 
       str00 += " date"; 
     System.out.println("00==>"+str00); 
    } 
   stream.close(); 
  } 
  catch(IOException e){  
   e.printStackTrace(); 
  } 
  catch(BiffException e){ 
   e.printStackTrace(); 
  } 
  finally{  
   rwb.close(); 
  } 
} 
/** 
  * create a new excelFile 
  * @param excelFileName create name 
  */ 
public static void createExcelFile(String excelFileName){ 
  try{ 
   WritableWorkbook wwb = Workbook.createWorkbook(new File(excelFileName)); 
   WritableSheet     ws  = wwb.createSheet("sheet1",0); 
   //also,The first parameter is  column,the second parameter is row. 
   // add normal label data 
   Label label00 = new Label(0,0,"Label00"); 
   ws.addCell(label00); 
   //add font formating data   
   WritableFont  wf = new WritableFont(WritableFont.TIMES, 18, WritableFont.BOLD , true); 
   WritableCellFormat wff = new WritableCellFormat(wf); 
   Label label10 = new Label(1,0,"Label10",wff); 
   ws.addCell(label10); 
   //add color font formating data 
   WritableFont wf_color = new WritableFont(WritableFont.ARIAL,10,WritableFont.NO_BOLD,false,UnderlineStyle.DOUBLE_ACCOUNTING,Colour.RED); 
   WritableCellFormat wff_color = new WritableCellFormat(wf_color); 
   wff_color.setBackground(Colour.GRAY_25); //set background coloe to gray  
   Label label20 = new Label(2,0,"Label20",wff_color);   
   ws.addCell(label20); 
   
   //合并单元格 
   WritableFont wf_merge = new WritableFont(WritableFont.ARIAL,10,WritableFont.NO_BOLD,false,UnderlineStyle.DOUBLE_ACCOUNTING,Colour.GREEN); 
   WritableCellFormat wff_merge = new WritableCellFormat(wf_merge); 
   Label label30 = new Label(3,0,"Label30",wff_merge);   
   ws.addCell(label30); 
   Label label40 = new Label(4,0,"Label40"); 
   ws.addCell(label40); 
   Label label50 = new Label(5,0,"Label50"); 
   ws.addCell(label50); 
     //合并 (3,0) (4,0) 
     //attention : 如果合并后面的列不为空,那么就把后面格的内容清空,格式也是按前一个单元格的格式 
   ws.mergeCells(3,0,4,0); 
   
   //添加Number格式数据 
   jxl.write.Number labelN = new jxl.write.Number(0, 1, 3.1415926); 
   ws.addCell(labelN); 
   
   //添加带有formatting的Number对象 
   jxl.write.NumberFormat nf = new jxl.write.NumberFormat("#.##"); 
   jxl.write.WritableCellFormat wcfN = new jxl.write.WritableCellFormat(nf); 
   jxl.write.Number labelNF = new jxl.write.Number(1, 1, 3.1415926, wcfN); 
   ws.addCell(labelNF); 
   
   //添加Boolean对象 
   jxl.write.Boolean labelBoolean = new jxl.write.Boolean(2,1,false); 
   ws.addCell(labelBoolean); 
   
   //添加DateTime对象 
   DateTime labelDT = new DateTime(3,1,new Date()); 
   ws.addCell(labelDT); 
   
   //添加带有格式的DataTime数据 
   DateFormat dtf = new DateFormat("yyyy-MM-dd hh:mm:ss"); 
   WritableCellFormat wcfDt = new WritableCellFormat(dtf);   
   wcfDt.setBackground(Colour.YELLOW); 
   DateTime labelDT_format =  new DateTime(4,1,new java.util.Date(),wcfDt); 
   ws.addCell(labelDT_format); 
   ws.mergeCells(4,1,5,1); //比较长,用两列来显示     
   
   wwb.write(); 
   wwb.close(); 
  } 
  catch(IOException e){ 
   e.printStackTrace(); 
  } 
  catch(WriteException e){ 
   e.printStackTrace(); 
  }  
} 
/** 
  * 如何更新Excel文件 
  * @param fileName 
  */ 
public static void updateExcel(String fileName){  
  try{ 
   jxl.Workbook rw = jxl.Workbook.getWorkbook(new File(fileName)); 
   WritableWorkbook wwb = Workbook.createWorkbook(new File(fileName),rw); 
   //这里其实执行的是一次copy操作,把文件先读到内存中,修改后再保存覆盖原来的文件来实现update操作 
   WritableSheet ws  = wwb.getSheet(0); 
   WritableCell wc = ws.getWritableCell(0,0); 
   if( wc.getType() == CellType.LABEL){ 
    Label l = (Label)wc; 
    l.setString(wc.getContents()+"_new"); 
   } 
   wwb.write(); 
   wwb.close(); 
  } 
  catch(IOException e){ 
   e.printStackTrace(); 
  } 
  catch(WriteException e){ 
   e.printStackTrace(); 
  } 
  catch(BiffException e){ 
   e.printStackTrace(); 
  } 
} 
/** 
  * 如何copy Excel文件 
  * @param fileName 
  */ 
public static void copyExcel(String sourFileName,String destFileName){  
  try{ 
   jxl.Workbook rw = jxl.Workbook.getWorkbook(new File(sourFileName)); 
   WritableWorkbook wwb = Workbook.createWorkbook(new File(destFileName),rw); 
   wwb.write(); 
   wwb.close(); 
  } 
  catch(IOException e){ 
   e.printStackTrace(); 
  } 
  catch(WriteException e){ 
   e.printStackTrace(); 
  } 
  catch(BiffException e){ 
   e.printStackTrace(); 
  } 
} 

public static void main(String [] argv){ 
  //OperateExcel.readExcel("E:\\test.xls"); 
  OperateExcel.createExcelFile("d:\\createExcelFile.xls"); 
  //OperateExcel.updateExcel("E:\\test.xls"); 
  //OperateExcel.copyExcel("E:\\test.xls","E:\\moon.xls"); 
} 

} 
