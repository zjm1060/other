package com.bitc.jk.xmis.export;

import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Locale;
import java.util.Map;

import org.apache.commons.lang.StringUtils;

import com.bitc.jk.xmis.model.PageInfo;

public class MoneyFormat implements Formater {

	/*
	 * 处理金额格式是0,000.00结尾的格式 
	 */
	public String getFormatValue(String value, Map row,int rowIndex, PageInfo pageInfo) {
			if(!StringUtils.isEmpty(value) && !StringUtils.isEmpty(value.trim())){
				String str = new DecimalFormat("0.00").format(Double.valueOf(value));
				NumberFormat numberFormat = NumberFormat.getInstance(Locale.US);
				return numberFormat.format(Double.valueOf(str));
			} else {
				return "";
			}
	}

}
