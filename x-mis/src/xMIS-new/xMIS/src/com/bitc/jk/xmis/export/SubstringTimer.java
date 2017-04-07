package com.bitc.jk.xmis.export;

import java.util.Map;

import org.apache.commons.lang.StringUtils;

import com.bitc.jk.xmis.model.PageInfo;

public class SubstringTimer implements Formater {

	/*
	 * 处理时间格式是00.00.0结尾的格式
	 */
	public String getFormatValue(String value, Map row,int rowIndex, PageInfo pageInfo) {
		if (!StringUtils.isEmpty(value) && value.length() > 0) {
			value = value.substring(0, 10);
		}
		return value;
	}

}
