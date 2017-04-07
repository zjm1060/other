package ly;

import java.io.File;
import java.io.IOException;

import jxl.Cell;
import jxl.Sheet;
import jxl.Workbook;
import jxl.read.biff.BiffException;

public class ReadExcel {
	public static void readExc(File filename) throws BiffException, IOException{  
			
		
		System.out.println(filename.getName());
		    Workbook wb = Workbook.getWorkbook(filename);  
		    Sheet s = wb.getSheet(0);//第1个sheet  
		    Cell c = null;  
		    int row = s.getRows();//总行数  
		    int col = s.getColumns();//总列数  
		    for(int i=0;i<row;i++){  
		     for(int j=0;j<col;j++){  
		      c = s.getCell(j,0);  
		      System.out.print(c.getContents()+"  ");  
		     }  
		     System.out.println();  
		    }    
		 }  

	public static void main(String[] args) {
		try {
			readExc(new File("d:/date.xls"));
		} catch (BiffException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
