package com.bitc.jk.xmis.export;

import java.text.DecimalFormat;
import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;

public class FormatCrmContactAmt implements Formater {

	public String getFormatValue(String value, Map row, int rowIndex, PageInfo pageInfo) {
		if("".equals(value)||value ==null){
			return "--";
		}else{
			double data = Double.valueOf(value);
			DecimalFormat a = new DecimalFormat(".00");
			data = data/10000;
			if(data<0.01){
				return "--";
			} else if(data>=0.01 && data<1){
				return "0"+a.format(data);
			} else {
				return (int)data+".00";
			}
			
		}
		
	}
	
}