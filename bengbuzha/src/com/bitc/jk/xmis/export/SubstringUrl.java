package com.bitc.jk.xmis.export;

import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.TemplateUtil;

public class SubstringUrl implements Formater {

	/*
	 * 处理后面带有^的URL 去除单元格的html标签
	 */
	public String getFormatValue(String value, Map row,int rowIndex, PageInfo pageInfo) {
		if (value.indexOf("^") > -1) {
			value = value.substring(0, value.indexOf("^"));
		}
		value = TemplateUtil.delHtmlTag(value);
		return value;
	}

}
