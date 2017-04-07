package com.bitc.jk.xmis.export;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Map;

import org.apache.commons.lang.StringUtils;

import com.bitc.jk.xmis.model.PageInfo;

public class DateFormat implements Formater {

	/*
	 * 处理时间类型是date,格式是导出是 MM/dd/yyyy 为yyyy-MM-dd
	 */
	public String getFormatValue(String value,Map row, int rowIndex, PageInfo pageInfo) {
		// "02/12/2012";
		if(!StringUtils.isEmpty(value) && !StringUtils.isEmpty(value.trim())){
			SimpleDateFormat s = new SimpleDateFormat("MM/dd/yyyy", Locale.CHINA);
			SimpleDateFormat sf = new SimpleDateFormat("yyyy-MM-dd", Locale.CHINA);
			Date date = null;
			try {
				date = s.parse(value);
			} catch (ParseException e) {
				e.printStackTrace();
			}
			return sf.format(date);
			
		} else {
			return "";
		}
	}

}
