package com.bitc.jk.xmis.export;

import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Locale;
import java.util.Map;

import org.apache.commons.lang.StringUtils;

import com.bitc.jk.xmis.model.PageInfo;

public class MillionFormat implements Formater {

	/*
	 * 处理金额是0,000.00结尾的格式（万元）
	 */
	public String getFormatValue(String value, Map row,int rowIndex, PageInfo pageInfo) {
			if(!StringUtils.isEmpty(value) && !StringUtils.isEmpty(value.trim())){
				double num = Double.valueOf(value)/10000;
				String str = new DecimalFormat("0.00").format(num);
//				NumberFormat numberFormat = NumberFormat.getInstance(Locale.US);
//				return numberFormat.format(Double.valueOf(str));
				return str;
			} else {
				return "";
			}
	}

}
